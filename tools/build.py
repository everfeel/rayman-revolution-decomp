#!/usr/bin/env python3
"""Build the main Rayman Revolution ELF with the configured SN Systems SDK."""

import re
import shlex
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

from compare import Elf32LE


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
COMPILER = (
    ROOT
    / "toolchain"
    / "sn-prodg-24"
    / "local"
    / "sce"
    / "ee"
    / "gcc"
    / "bin"
    / "ee-gcc.exe"
)
LINKER = COMPILER.with_name("ee-ld.exe")
LINKER_SCRIPT = BUILD / "SLES_500.44.ld"
OUTPUT = BUILD / "SLES_500.44.elf"
LOG = BUILD / "build.log"
STATUS_SCRIPT = ROOT / "tools" / "status.py"
INCBIN = re.compile(r'^(\s*)\.incbin\s+"([^"]+)"\s*$')
AUTO_SYMBOL = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+);$")
MODULE_MANIFEST = ROOT / "src" / "modules.txt"
FUNCTION_HEADER = re.compile(
    r"(?m)^\s*(?:matching|nonmatching)\s+"
    r"(func_[0-9A-Fa-f]{8})\s*,\s*(0x[0-9A-Fa-f]+)\s*$"
)
C_FUNCTION = re.compile(r"(?m)^\s*\.ent\s+(func_[0-9A-Fa-f]{8})\s*$")
LIT4_MIN_OFFSET = 0x292400

DEFAULT_OPTIMIZATION = "-O2"
OPTIMIZATION_FLAG = re.compile(r"-O(?:[0-9sgz]|fast)?")
# Flags the build script controls itself (pipeline stage, output path, input language).
RESERVED_FLAG = re.compile(r"-(?:[cSE]|o.*|x.*)")
GLOBAL_DIRECTIVE = "@global"
# Set to True to apply @global arguments to every C module in modules.txt.
USE_GLOBAL_COMPILER_FLAGS = False


@dataclass(frozen=True)
class SourceModule:
    path: Path
    flags: tuple = ()


def is_optimization_flag(flag):
    return OPTIMIZATION_FLAG.fullmatch(flag) is not None


def parse_manifest_flags(tokens, location):
    for token in tokens:
        if not token:
            raise ValueError(f"{location}: empty compiler flag")
        if RESERVED_FLAG.fullmatch(token):
            raise ValueError(
                f"{location}: {token!r} is controlled by the build script "
                "and cannot be set in the manifest"
            )
    return tuple(tokens)


def c_flags(module, global_flags):
    """Choose global or per-module C flags, with the default optimization."""
    explicit = global_flags if USE_GLOBAL_COMPILER_FLAGS else module.flags
    default = (
        () if any(is_optimization_flag(flag) for flag in explicit)
        else (DEFAULT_OPTIMIZATION,)
    )
    return [*default, *explicit]


def auto_symbol_definitions():
    undefined_symbols = ROOT / "undefined_syms_auto.txt"
    definitions = []
    for line in undefined_symbols.read_text(encoding="ascii").splitlines():
        match = AUTO_SYMBOL.fullmatch(line)
        if match:
            definitions.append((match.group(1), match.group(2)))
    return definitions


def linker_gp_value():
    script = LINKER_SCRIPT.read_text(encoding="ascii")
    match = re.search(
        r"(?m)^\s*_gp\s*=\s*(0x[0-9A-Fa-f]+)\s*;",
        script,
    )
    if not match:
        raise ValueError(f"{LINKER_SCRIPT}: could not find the _gp assignment")
    return int(match.group(1), 16)


def small_data_threshold(flags):
    threshold = 0
    index = 0
    while index < len(flags):
        flag = flags[index]
        value = None
        if flag == "-G" and index + 1 < len(flags):
            index += 1
            value = flags[index]
        elif flag.startswith("-G") and len(flag) > 2:
            value = flag[2:]
        if value is not None:
            try:
                threshold = int(value, 10)
            except ValueError:
                pass
        index += 1
    return threshold


def keep_out_of_range_externals_absolute(
    assembly_lines, symbol_addresses, gp, small_data_size
):
    if small_data_size <= 0:
        return assembly_lines

    absolute_externals = set()
    for name, address in symbol_addresses.items():
        offset = (address - gp) & 0xFFFFFFFF
        if offset & 0x80000000:
            offset -= 0x100000000
        if not -0x8000 <= offset <= 0x7FFF:
            absolute_externals.add(name)

    updated_lines = []
    for line in assembly_lines:
        match = re.match(
            r"^([ \t]*\.extern[ \t]+)([A-Za-z_][A-Za-z0-9_]*)"
            r"(?:[ \t]*,[ \t]*(\d+))?[ \t]*$",
            line,
        )
        if (
            match
            and match.group(2) in absolute_externals
            and int(match.group(3) or 0) <= small_data_size
        ):
            # Keep the assembler from selecting an out-of-range GPREL16 relocation.
            line = f"{match.group(1)}{match.group(2)}, {small_data_size + 1}"
        updated_lines.append(line)
    return updated_lines


def assembly_flags(global_flags, module_flags=()):
    """Assembly sources never received an -O level, so global -O flags are not forwarded."""
    inherited = [flag for flag in global_flags if not is_optimization_flag(flag)]
    return [*inherited, *module_flags]


def fail(message):
    print(f"build: {message}", file=sys.stderr)
    return 1


def compile_source(command, label, log):
    result = subprocess.run(
        command,
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    with log.open("a", encoding="utf-8") as output:
        output.write(f"\n===== {label} =====\n")
        output.write(result.stdout)
    if result.returncode:
        print(f"build: {label} failed; see {log.relative_to(ROOT)}", file=sys.stderr)
        print("\n".join(result.stdout.splitlines()[-60:]), file=sys.stderr)
        return False
    return True


def enable_assembler_reordering(assembly_path):
    """Override macro.inc's noreorder setting for compiler-generated C code."""
    assembly = assembly_path.read_text(encoding="utf-8")
    macro_include = re.search(
        r'(?m)^[ \t]*\.include[ \t]+"include/macro\.inc"[ \t]*$', assembly
    )
    if not macro_include:
        raise ValueError(
            f"{assembly_path}: compiler output does not include include/macro.inc"
        )
    no_app = re.search(
        r"(?m)^[ \t]*#NO_APP[ \t]*$", assembly[macro_include.end():]
    )
    if not no_app:
        raise ValueError(
            f"{assembly_path}: missing #NO_APP after include/macro.inc"
        )
    insertion = macro_include.end() + no_app.end()
    assembly = assembly[:insertion] + "\n\t.set reorder" + assembly[insertion:]
    assembly_path.write_text(assembly, encoding="utf-8", newline="\n")


def retail_function_sizes():
    assembly_root = ROOT / "asm" / "nonmatchings"
    if not assembly_root.is_dir():
        raise FileNotFoundError(f"missing retail function assembly: {assembly_root}")

    sizes = {}
    for path in sorted(assembly_root.rglob("func_*.s")):
        source = path.read_text(encoding="utf-8")
        for match in FUNCTION_HEADER.finditer(source):
            name = match.group(1)
            if name in sizes:
                raise ValueError(f"duplicate retail function size for {name}")
            sizes[name] = int(match.group(2), 16)
    if not sizes:
        raise ValueError(f"no retail function sizes found under {assembly_root}")
    return sizes


def pad_undersized_functions(assembly_path, object_path, retail_sizes):
    assembly = assembly_path.read_text(encoding="utf-8")
    functions = C_FUNCTION.findall(assembly)
    if not functions:
        return assembly, 0, 0
    if len(functions) != len(set(functions)):
        raise ValueError(f"{assembly_path}: duplicate C function entries")

    object_elf = Elf32LE(object_path)
    padded = 0
    total_padding = 0
    for name in functions:
        retail_size = retail_sizes.get(name)
        if retail_size is None:
            continue
        symbol = object_elf.symbol(name)
        if symbol["type"] != 2 or symbol["size"] <= 0:
            raise ValueError(
                f"{object_path}: {name} has no nonempty function symbol"
            )
        padding = retail_size - symbol["size"]
        if padding <= 0:
            continue
        if padding % 4:
            raise ValueError(
                f"{name}: required padding of {padding} bytes is not instruction-aligned"
            )

        end_pattern = re.compile(
            rf"(?m)^([ \t]*)\.end[ \t]+{re.escape(name)}[ \t]*$"
        )
        matches = list(end_pattern.finditer(assembly))
        if len(matches) != 1:
            raise ValueError(
                f"{assembly_path}: expected one .end for {name}, found {len(matches)}"
            )
        match = matches[0]
        replacement = (
            f"{match.group(1)}.space {padding}, 0\n"
            f"{match.group(0)}"
        )
        assembly = assembly[:match.start()] + replacement + assembly[match.end():]
        padded += 1
        total_padding += padding

    return assembly, padded, total_padding


def compile_c_source(source, object_path, common_flags, log, function_sizes):
    relative = source.relative_to(ROOT)
    assembly_path = BUILD / "c-input" / relative.with_suffix(".s")
    assembly_path.parent.mkdir(parents=True, exist_ok=True)
    object_path.parent.mkdir(parents=True, exist_ok=True)

    if not compile_source(
        [
            str(COMPILER),
            *common_flags,
            "-S",
            str(source),
            "-o",
            str(assembly_path),
        ],
        f"compile {relative.as_posix()}",
        log,
    ):
        return None

    try:
        enable_assembler_reordering(assembly_path)
        auto_symbols = auto_symbol_definitions()
        symbol_addresses = {
            name: int(address, 16) for name, address in auto_symbols
        }
        assembly_lines = assembly_path.read_text(encoding="utf-8").splitlines()
        adjusted_lines = keep_out_of_range_externals_absolute(
            assembly_lines,
            symbol_addresses,
            linker_gp_value(),
            small_data_threshold(common_flags),
        )
        if adjusted_lines != assembly_lines:
            assembly_path.write_text(
                "\n".join(adjusted_lines) + "\n",
                encoding="utf-8",
                newline="\n",
            )
    except (OSError, UnicodeError, ValueError) as error:
        fail(
            f"could not prepare C assembler input in "
            f"{relative.as_posix()}: {error}"
        )
        return None

    assemble = [
        str(COMPILER),
        *common_flags,
        "-c",
        str(assembly_path),
        "-o",
        str(object_path),
    ]
    if not compile_source(
        assemble,
        f"assemble {relative.as_posix()}",
        log,
    ):
        return None

    try:
        assembly, padded, padding_bytes = pad_undersized_functions(
            assembly_path, object_path, function_sizes
        )
        if padded:
            assembly_path.write_text(assembly, encoding="utf-8", newline="\n")
            if not compile_source(
                assemble,
                f"assemble padded {relative.as_posix()}",
                log,
            ):
                return None
    except (OSError, UnicodeError, ValueError) as error:
        fail(f"could not pad short C functions in {relative.as_posix()}: {error}")
        return None

    return padded, padding_bytes


def assembly_input(source):
    text = source.read_text(encoding="utf-8")
    rewritten = []
    changed = False
    for line in text.splitlines():
        match = INCBIN.fullmatch(line)
        if not match:
            rewritten.append(line)
            continue

        binary = ROOT / Path(match.group(2).replace("/", "\\"))
        if not binary.is_file():
            raise FileNotFoundError(f"{source}: missing .incbin input {binary}")
        data = binary.read_bytes()
        prefix = match.group(1)
        for offset in range(0, len(data), 16):
            values = ", ".join(f"0x{byte:02X}" for byte in data[offset:offset + 16])
            rewritten.append(f"{prefix}.byte {values}")
        changed = True

    if not changed:
        return source

    generated = BUILD / "asm-input" / source.relative_to(ROOT)
    generated.parent.mkdir(parents=True, exist_ok=True)
    generated.write_text("\n".join(rewritten) + "\n", encoding="ascii", newline="\n")
    return generated


def compile_assembly_source(source, object_path, common_flags, log):
    object_path.parent.mkdir(parents=True, exist_ok=True)
    try:
        assembler_source = assembly_input(source)
    except (OSError, ValueError) as error:
        fail(f"could not prepare assembly source {source.relative_to(ROOT)}: {error}")
        return False
    return compile_source(
        [
            str(COMPILER),
            *common_flags,
            "-c",
            str(assembler_source),
            "-o",
            str(object_path),
        ],
        f"assemble {source.relative_to(ROOT).as_posix()}",
        log,
    )


def make_legacy_linker_script(output_path=None):
    script = LINKER_SCRIPT.read_text(encoding="utf-8")
    sections_marker = "SECTIONS\n{\n"
    if script.count(sections_marker) != 1:
        raise ValueError("linker script must contain one top-level SECTIONS block")
    script = script.replace(
        sections_marker,
        "PHDRS\n{\n    main PT_LOAD FLAGS(7);\n}\nSECTIONS\n{\n",
        1,
    )
    main_section_start = "    .main 0x100000 : AT(main_ROM_START)\n"
    if script.count(main_section_start) != 1:
        raise ValueError("linker script must contain one file-backed main section")
    script = script.replace(main_section_start, "    .main 0x100000 :\n", 1)
    script = script.replace("    HIDDEN(__romPos = 0);", "    __romPos = 0;")
    script = re.sub(
        r"(?m)^\s*build/[^()\n]+\(([^)\n]+)\);$",
        lambda match: f"        *({match.group(1)});",
        script,
    )
    script = script.replace("ALIGN(., 16)", "ALIGN(16)")
    lit4_marker = "        main_LIT4_START = .;\n"
    if script.count(lit4_marker) != 1:
        raise ValueError("linker script must contain exactly one main_LIT4_START")
    script = script.replace(
        lit4_marker,
        f"        . = ({LIT4_MIN_OFFSET:#x} > .) ? "
        f"{LIT4_MIN_OFFSET:#x} : .;\n" + lit4_marker,
        1,
    )
    script = re.sub(r"(?m)^\s*\w+_VRAM = ADDR\([^\n]+\);\n", "", script)
    script = re.sub(r"(?m)^\s*__romPos \+= SIZEOF\(\.main\);\n", "", script)
    script = re.sub(r"(?m)^\s*__romPos = ALIGN\(__romPos, 16\);\n", "", script)
    script = re.sub(
        r"(?ms)\n\s*/DISCARD/\s*:\s*\{\s*\*\(\*\);\s*\}\s*\n\}",
        "\n}",
        script,
    )
    script = script.replace(
        "    .main_bss (NOLOAD) :\n",
        "    .main_bss 0x393BF0 (NOLOAD) :\n",
        1,
    )
    script = script.replace(
        "        FILL(0x00000000);\n        main_SBSS_START = .;",
        "        FILL(0x00000000);\n        . = 0x10;\n        main_SBSS_START = .;",
        1,
    )
    script = script.replace(
        "        . = ALIGN(16);\n        main_BSS_END = .;",
        "        . = 0x7D204;\n        main_BSS_END = .;",
        1,
    )
    main_section_end = (
        "        main_SDATA_SIZE = ABSOLUTE(main_SDATA_END - main_SDATA_START);\n"
        "    }\n    .main_bss"
    )
    if script.count(main_section_end) != 1:
        raise ValueError("linker script must contain one main section boundary")
    script = script.replace(
        main_section_end,
        "        main_SDATA_SIZE = ABSOLUTE(main_SDATA_END - main_SDATA_START);\n"
        "    } :main\n    .main_bss",
        1,
    )
    bss_section_end = (
        "        main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);\n"
        "    }\n    . = ALIGN(16);"
    )
    if script.count(bss_section_end) != 1:
        raise ValueError("linker script must contain one main BSS section boundary")
    script = script.replace(
        bss_section_end,
        "        main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);\n"
        "    } :main\n"
        "    .reginfo : { *(.reginfo) } :NONE\n"
        "    . = ALIGN(16);",
        1,
    )
    legacy_script = output_path or BUILD / "SLES_500.44.sn.ld"
    legacy_script.write_text(script, encoding="ascii", newline="\n")
    return legacy_script


def source_modules():
    """Parse src/modules.txt.

    Returns (modules, global_flags). Each non-comment line is either:

        <source path> [flags...]     a C or assembly source with optional flags
        @global [flags...]           C flags used when global mode is enabled

    Flags support shell-style quoting and trailing '#' comments.
    """
    if not MODULE_MANIFEST.is_file():
        raise FileNotFoundError(
            f"{MODULE_MANIFEST.relative_to(ROOT)} is missing"
        )

    manifest_name = MODULE_MANIFEST.relative_to(ROOT)
    modules = []
    global_flags = []
    for line_number, line in enumerate(
        MODULE_MANIFEST.read_text(encoding="utf-8").splitlines(), start=1
    ):
        entry = line.strip()
        if not entry or entry.startswith("#"):
            continue
        location = f"{manifest_name}:{line_number}"

        # The first field is split on whitespace only, so paths keep working
        # exactly as before; the remaining flags support quoting and comments.
        parts = entry.split(None, 1)
        source_name = parts[0]
        rest = parts[1] if len(parts) > 1 else ""
        try:
            tokens = shlex.split(rest, comments=True)
        except ValueError as error:
            raise ValueError(f"{location}: invalid flag syntax: {error}") from error
        flags = parse_manifest_flags(tokens, location)

        if source_name.startswith("@"):
            if source_name != GLOBAL_DIRECTIVE:
                raise ValueError(f"{location}: unknown directive {source_name!r}")
            global_flags.extend(flags)
            continue

        relative = Path(source_name.replace("/", "\\"))
        if (
            relative.is_absolute()
            or relative.suffix not in {".c", ".s"}
            or ".." in relative.parts
        ):
            raise ValueError(
                f"{location}: invalid C or assembly source path {source_name!r}"
            )
        source = ROOT / "src" / relative
        if not source.is_file():
            raise FileNotFoundError(
                f"{location}: missing source {source.relative_to(ROOT)}"
            )
        modules.append(SourceModule(source, flags))

    if not modules:
        raise ValueError(f"{manifest_name} has no source files")
    module_paths = [module.path for module in modules]
    if len(set(module_paths)) != len(module_paths):
        raise ValueError(f"{manifest_name} contains duplicate paths")
    main_source = ROOT / "src" / "main.c"
    if modules[0].path != main_source:
        raise ValueError(f"{manifest_name} must list main.c first")
    if sum(module.path == main_source for module in modules) != 1:
        raise ValueError(f"{manifest_name} must list main.c exactly once")
    return modules, tuple(global_flags)


def main():
    if not COMPILER.is_file():
        return fail(f"SDK compiler/assembler not found: {COMPILER}")
    if not LINKER.is_file():
        return fail(f"SDK linker not found: {LINKER}")

    if not LINKER_SCRIPT.is_file():
        return fail(
            f"{LINKER_SCRIPT.relative_to(ROOT)} is missing; "
            "run `py tools\\setup_asm.py` first"
        )
    undefined_symbols = ROOT / "undefined_syms_auto.txt"
    if not undefined_symbols.is_file():
        return fail(
            "undefined_syms_auto.txt is missing; run `py tools\\setup_asm.py` first"
        )

    BUILD.mkdir(parents=True, exist_ok=True)
    LOG.write_text("", encoding="utf-8")
    objects = []
    common_flags = ["-G4", "-Iinclude", "-Isrc", "-Wa,-I,include,-I,."]
    try:
        function_sizes = retail_function_sizes()
    except (OSError, UnicodeError, ValueError) as error:
        return fail(f"could not read retail function sizes: {error}")
    padded_functions = 0
    padding_bytes = 0

    try:
        modules, global_flags = source_modules()
    except (OSError, UnicodeError, ValueError) as error:
        return fail(f"could not read C source manifest: {error}")
    for module in modules:
        source = module.path
        relative = source.relative_to(ROOT)
        object_path = BUILD / relative.parent / f"{source.name}.o"
        if source.suffix == ".c":
            compile_result = compile_c_source(
                source,
                object_path,
                [*common_flags, *c_flags(module, global_flags)],
                LOG,
                function_sizes,
            )
            if compile_result is None:
                return 1
            padded_functions += compile_result[0]
            padding_bytes += compile_result[1]
        elif not compile_assembly_source(
            source,
            object_path,
            [*common_flags, *assembly_flags((), module.flags)],
            LOG,
        ):
            return 1
        objects.append(object_path)

    data_root = ROOT / "asm" / "data"
    if not data_root.is_dir():
        return fail("asm/data is missing; run `py tools\\setup_asm.py` first")
    data_flags = common_flags
    for source in sorted(data_root.rglob("*.s")):
        relative = source.relative_to(ROOT)
        object_path = BUILD / relative.parent / f"{source.name}.o"
        object_path.parent.mkdir(parents=True, exist_ok=True)
        if not compile_assembly_source(source, object_path, data_flags, LOG):
            return 1
        objects.append(object_path)

    try:
        legacy_script = make_legacy_linker_script()
        definitions = [
            f"--defsym={name}={address}"
            for name, address in auto_symbol_definitions()
        ]
    except (OSError, UnicodeError) as error:
        return fail(f"could not prepare linker inputs: {error}")

    if not compile_source(
        [
            str(LINKER),
            "-e",
            "0x00100008",
            "-T",
            str(legacy_script),
            "-o",
            str(OUTPUT),
            *definitions,
            *(str(path) for path in objects),
        ],
        "link main ELF",
        LOG,
    ):
        return 1

    status_result = subprocess.run(
        [sys.executable, str(STATUS_SCRIPT), str(OUTPUT)],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    with LOG.open("a", encoding="utf-8") as output:
        output.write("\n===== generate function status reports =====\n")
        output.write(status_result.stdout)
    if status_result.returncode:
        print("build: function status report generation failed:", file=sys.stderr)
        print("\n".join(status_result.stdout.splitlines()[-60:]), file=sys.stderr)
        return status_result.returncode

    print(f"Built {OUTPUT.relative_to(ROOT)} from {len(objects)} objects.")
    print(
        f"Padded {padded_functions} undersized C functions "
        f"with {padding_bytes} bytes of trailing nops."
    )
    print(f"Build diagnostics: {LOG.relative_to(ROOT)}")
    print(status_result.stdout, end="")
    return 0


if __name__ == "__main__":
    sys.exit(main())