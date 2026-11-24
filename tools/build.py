#!/usr/bin/env python3
"""Build the main Rayman Revolution ELF with the configured SN Systems SDK."""

import argparse
import hashlib
import json
import re
import shlex
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

from compare import Elf32LE
from function_units import AssemblyFunctionUnit, CFunctionUnit, iter_function_units


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build"
OBJECT_CACHE = BUILD / ".object_cache.json"
FUNCTION_UNIT_DIR = BUILD / "function_units"
# Keep this fingerprint stable across cache-only changes. Bump it when the
# generated assembly or object layout rules change.
BUILD_PIPELINE_FINGERPRINT = (
    "7288743a5059ec2bffacded557f6d5d7378ffdb9298cc4bbf32f4cba18e32c26"
)
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
ASM_OVERRIDE_FILE = ROOT / "asm_overrides.txt"
ASM_OVERRIDE_ROOT = ROOT / "asm" / "nonmatchings"
FUNCTION_HEADER = re.compile(
    r"(?m)^\s*(?:matching|nonmatching)\s+"
    r"(func_[0-9A-Fa-f]{8})\s*,\s*(0x[0-9A-Fa-f]+)\s*$"
)
C_FUNCTION = re.compile(r"(?m)^\s*\.ent\s+(func_[0-9A-Fa-f]{8})\s*$")
FUNCTION_SYMBOL = re.compile(r"func_[0-9A-Fa-f]{8}\Z")
AUTO_PADDING = re.compile(
    r"(?m)^([ \t]*)# BUILD_PADDING (func_[0-9A-Fa-f]{8})\r?\n"
    r"\1\.space ([0-9]+), 0\r?\n"
)
MAX_PADDING_PASSES = 8
JUMP_TABLE_LABEL = re.compile(
    r"(?m)^[ \t]*jlabel[ \t]+([.$A-Za-z_][.$A-Za-z_0-9]*)[ \t]*$"
)
# A data blob spells a jump table the way retail did: its words are the retail
# labels of the case bodies (`.word .L001DA714`), and splat names such a label
# after its address.
DATA_REFERENCE = re.compile(r"\.(?:word|long)[ \t]+(\.L([0-9A-Fa-f]{8}))\b")
LIT4_MIN_OFFSET = 0x292400

DEFAULT_OPTIMIZATION = "-O2"
OPTIMIZATION_FLAG = re.compile(r"-O(?:[0-9sgz]|fast)?")
# Flags the build script controls itself (pipeline stage, output path, input language).
RESERVED_FLAG = re.compile(r"-(?:[cSE]|o.*|x.*)")
GLOBAL_DIRECTIVE = "@global"
# Set to True to apply @global arguments to every C module in modules.txt.
USE_GLOBAL_COMPILER_FLAGS = False
# Optional alternate C frontend (a cc1.exe) for experiments, set by
# tools/compiler_sweep.py. Only the -S step uses it; assembling and linking
# stay with the real driver, and the normal build leaves it None.
C_FRONTEND = None
# C files that emitted bytes into sections retail already ships as verbatim
# data blobs. Collected by compile_c_source, reported by main.
EXTRA_SECTIONS = []
# Sections whose retail content is linked from asm/data rather than generated.
RETAIL_DATA_SECTIONS = (
    ".data",
    ".rodata",
    ".gcc_except_table",
    ".lit4",
    ".sdata",
    ".sbss",
    ".bss",
)


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


def data_label_aliases(objects):
    """Retail labels that only the data blobs still name, as (name, address).

    While a function is included from its retail assembly, that assembly defines
    the `.L...` labels its jump table points at. Once the function is written in
    C, nothing defines them and the data object's words become undefined
    references. The values are recovered from the label names, which splat forms
    from each label's address, so no source file has to carry them.
    """
    defined = set()
    for path in objects:
        defined.update(Elf32LE(path).symbol_names())

    wanted = {}
    for source in sorted((ROOT / "asm" / "data").rglob("*.s")):
        text = source.read_text(encoding="utf-8", errors="replace")
        for name, address in DATA_REFERENCE.findall(text):
            wanted.setdefault(name, int(address, 16))
    return sorted(
        (name, address) for name, address in wanted.items() if name not in defined
    )

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


PREPROCESSED = BUILD / "pp"


def preprocessed_source(source, common_flags, log):
    """The preprocessed form of a C input, cached by its text and flags.

    A frontend sweep compiles the same input once per frontend; without this
    the preprocessor runs again for each. Keyed by content, so a candidate that
    is tried twice, or in two frontends, costs one cpp run. Returns None on
    failure.
    """
    text = source.read_bytes()
    digest = hashlib.sha1()
    for flag in common_flags:
        digest.update(str(flag).encode() + b"\0")
    digest.update(b"\0" + text)
    target = PREPROCESSED / (digest.hexdigest() + ".i")
    if target.is_file() and target.stat().st_size:
        return target
    PREPROCESSED.mkdir(parents=True, exist_ok=True)
    if not compile_source(
        [str(COMPILER), *common_flags, "-E", str(source), "-o", str(target)],
        f"preprocess {source.name}",
        log,
    ):
        return None
    return target


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
    if no_app:
        insertion = macro_include.end() + no_app.end()
    else:
        # A translation unit made only of asm blocks has no #NO_APP; the
        # reorder switch goes directly after the macro.inc include, where
        # no instructions have been emitted yet.
        insertion = macro_include.end()
    assembly = assembly[:insertion] + "\n\t.set reorder" + assembly[insertion:]
    assembly_path.write_text(assembly, encoding="utf-8", newline="\n")


def retail_function_sizes():
    assembly_root = ASM_OVERRIDE_ROOT
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


def extra_data_sections(object_path):
    """Bytes a C object adds to sections retail already provides verbatim.

    Retail's .data/.rodata/.lit4/.sdata blobs are linked from asm/data, so any
    amount a C file contributes to those sections (a float pool, a string
    literal, a gcc jump table in .rdata) grows .main and shifts every later
    section, even when the functions themselves are the right size.
    """
    elf = Elf32LE(object_path)
    sizes = {}
    for section in elf.sections:
        name = (
            elf.section_names[section["name_offset"]:]
            .split(b"\0", 1)[0]
            .decode("ascii", "replace")
        )
        suffix = name.split(".", 2)
        base = "." + suffix[1] if len(suffix) > 2 else name
        if base in RETAIL_DATA_SECTIONS and section["size"]:
            sizes[base] = sizes.get(base, 0) + section["size"]
    return sizes


def report_extra_sections(extra_sections):
    """Warn about the section growth that overruns .main before it happens."""
    for source, sizes in extra_sections:
        detail = ", ".join(f"{name} {size:#x}" for name, size in sorted(sizes.items()))
        print(
            f"build: note: {source} adds {detail} to sections retail links as "
            "fixed data; this grows .main and shifts later sections",
            file=sys.stderr,
        )


def load_asm_overrides():
    if not ASM_OVERRIDE_FILE.is_file():
        return {}

    overrides = {}
    for line_number, line in enumerate(
        ASM_OVERRIDE_FILE.read_text(encoding="ascii").splitlines(), start=1
    ):
        symbol = line.split("#", 1)[0].strip()
        if not symbol:
            continue
        if not FUNCTION_SYMBOL.fullmatch(symbol):
            raise ValueError(
                f"{ASM_OVERRIDE_FILE.name}:{line_number}: "
                f"expected a function symbol, got {symbol!r}"
            )
        if symbol in overrides:
            raise ValueError(
                f"{ASM_OVERRIDE_FILE.name}:{line_number}: "
                f"duplicate override for {symbol}"
            )

        assembly_files = list(
            ASM_OVERRIDE_ROOT.rglob(f"{symbol}.s")
        )
        if len(assembly_files) != 1:
            raise ValueError(
                f"{ASM_OVERRIDE_FILE.name}:{line_number}: expected one "
                f"retail assembly file for {symbol}, found {len(assembly_files)}"
            )
        overrides[symbol] = assembly_files[0].relative_to(ROOT).as_posix()

    return overrides


def apply_asm_overrides(assembly_path, overrides):
    assembly = assembly_path.read_text(encoding="utf-8")
    applied = set()

    for name in sorted(set(C_FUNCTION.findall(assembly)) & set(overrides)):
        override_path = ROOT / overrides[name]
        override_source = override_path.read_text(encoding="utf-8")
        jump_labels = JUMP_TABLE_LABEL.findall(override_source)
        # Retail assembly defines these labels, so discard the C-side aliases
        # emitted for its jump table.
        for label in jump_labels:
            alias_pattern = re.compile(
                rf"(?m)^[ \t]*\.(?:globl|equ)[ \t]+"
                rf"{re.escape(label)}(?:[ \t,][^\r\n]*)?\r?\n?"
            )
            assembly = alias_pattern.sub("", assembly)

        start_pattern = re.compile(
            rf"(?m)^[ \t]*\.ent[ \t]+{re.escape(name)}[ \t]*$"
        )
        end_pattern = re.compile(
            rf"(?m)^[ \t]*\.end[ \t]+{re.escape(name)}[ \t]*$"
        )
        starts = list(start_pattern.finditer(assembly))
        ends = list(end_pattern.finditer(assembly))
        if (
            len(starts) != 1
            or len(ends) != 1
            or ends[0].start() < starts[0].end()
        ):
            raise ValueError(
                f"{assembly_path}: could not isolate generated assembly for {name}"
            )

        replacement = (
            "\t.set noreorder\n"
            f'\t.include "{overrides[name]}"\n'
            "\t.set reorder"
        )
        assembly = (
            assembly[:starts[0].start()]
            + replacement
            + assembly[ends[0].end():]
        )
        applied.add(name)

    if applied:
        assembly_path.write_text(assembly, encoding="utf-8", newline="\n")
    return applied


def pad_undersized_functions(assembly_path, object_path, retail_sizes):
    assembly = assembly_path.read_text(encoding="utf-8")
    functions = C_FUNCTION.findall(assembly)
    if not functions:
        return assembly, set(), 0, []
    if len(functions) != len(set(functions)):
        raise ValueError(f"{assembly_path}: duplicate C function entries")

    padding_values = {}
    for match in AUTO_PADDING.finditer(assembly):
        name = match.group(2)
        if name in padding_values:
            raise ValueError(f"{assembly_path}: duplicate build padding for {name}")
        if name not in functions:
            raise ValueError(f"{assembly_path}: padding marker for missing {name}")
        padding_values[name] = int(match.group(3))

    object_elf = Elf32LE(object_path)
    adjusted = set()
    padding_delta = 0
    oversized = []
    for name in functions:
        retail_size = retail_sizes.get(name)
        if retail_size is None:
            continue
        symbol = object_elf.symbol(name)
        if symbol["type"] != 2 or symbol["size"] <= 0:
            raise ValueError(
                f"{object_path}: {name} has no nonempty function symbol"
            )
        current_padding = padding_values.get(name, 0)
        match = next(
            (
                item
                for item in AUTO_PADDING.finditer(assembly)
                if item.group(2) == name
            ),
            None,
        )
        padding = current_padding + retail_size - symbol["size"]
        if padding < 0 and match is None:
            # The C function is larger than retail's. A few bytes are harmless
            # when they fit the alignment gap before the next retail function;
            # anything larger shifts every later symbol, so report it here
            # instead of letting the linker fail with a confusing overlap.
            oversized.append((name, symbol["size"], retail_size))
            continue
        if padding == current_padding:
            continue
        if padding < 0:
            padding = 0
        if padding % 4 or current_padding % 4:
            raise ValueError(
                f"{name}: required padding adjustment is not instruction-aligned"
            )

        if match:
            replacement = (
                f"{match.group(1)}# BUILD_PADDING {name}\n"
                f"{match.group(1)}.space {padding}, 0\n"
                if padding
                else ""
            )
            assembly = (
                assembly[:match.start()]
                + replacement
                + assembly[match.end():]
            )
        elif padding:
            end_pattern = re.compile(
                rf"(?m)^([ \t]*)\.end[ \t]+{re.escape(name)}[ \t]*$"
            )
            matches = list(end_pattern.finditer(assembly))
            if len(matches) != 1:
                raise ValueError(
                    f"{assembly_path}: expected one .end for {name}, "
                    f"found {len(matches)}"
                )
            end_match = matches[0]
            replacement = (
                f"{end_match.group(1)}# BUILD_PADDING {name}\n"
                f"{end_match.group(1)}.space {padding}, 0\n"
                f"{end_match.group(0)}"
            )
            assembly = (
                assembly[:end_match.start()]
                + replacement
                + assembly[end_match.end():]
            )

        adjusted.add(name)
        padding_delta += padding - current_padding

    return assembly, adjusted, padding_delta, oversized


def compile_c_source(
    source, object_path, common_flags, log, function_sizes, asm_overrides
):
    relative = source.relative_to(ROOT)
    assembly_path = BUILD / "c-input" / relative.with_suffix(".s")
    assembly_path.parent.mkdir(parents=True, exist_ok=True)
    object_path.parent.mkdir(parents=True, exist_ok=True)

    compiler_input = source
    if C_FRONTEND is None:
        frontend_command = [str(COMPILER), *common_flags, "-S"]
    else:
        frontend_command = [
            str(C_FRONTEND),
            *[flag for flag in common_flags if not flag.startswith("-Wa")],
            "-quiet",
        ]
    if not compile_source(
        [
            *frontend_command,
            str(compiler_input),
            "-o",
            str(assembly_path),
        ],
        f"compile {relative.as_posix()}",
        log,
    ):
        return None

    try:
        enable_assembler_reordering(assembly_path)
        c_functions = set(
            C_FUNCTION.findall(assembly_path.read_text(encoding="utf-8"))
        )
        applied_overrides = apply_asm_overrides(assembly_path, asm_overrides)
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
        padded_functions = set()
        padding_bytes = 0
        for padding_pass in range(MAX_PADDING_PASSES + 1):
            assembly, padded, added_padding, oversized = pad_undersized_functions(
                assembly_path, object_path, function_sizes
            )
            padded_functions.update(padded)
            padding_bytes += added_padding
            if not padded:
                break
            if padding_pass == MAX_PADDING_PASSES:
                fail(
                    f"could not stabilize function padding in "
                    f"{relative.as_posix()} after {MAX_PADDING_PASSES} passes"
                )
                return None

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

    extra_sections = extra_data_sections(object_path)
    if extra_sections:
        EXTRA_SECTIONS.append((relative.as_posix(), extra_sections))

    return (
        len(padded_functions),
        padding_bytes,
        applied_overrides,
        oversized,
        c_functions,
    )


def file_sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def shared_object_fingerprint(function_sizes):
    digest = hashlib.sha256()
    files = [
        Path(__file__),
        Path(__file__).with_name("function_units.py"),
        COMPILER,
        LINKER_SCRIPT,
        ROOT / "undefined_syms_auto.txt",
        BUILD / "gp_externs.inc",
    ]
    if C_FRONTEND is not None:
        files.append(Path(C_FRONTEND))
    files.extend(path for path in (ROOT / "include").rglob("*") if path.is_file())
    files.extend(
        path
        for path in (ROOT / "src").rglob("*")
        if path.is_file() and path.suffix.lower() in {".h", ".inc", ".inl", ".def"}
    )
    files.extend(
        path for path in (ROOT / "src" / "asm").rglob("*") if path.is_file()
    )
    for path in sorted(set(files)):
        try:
            path_label = path.relative_to(ROOT).as_posix()
        except ValueError:
            path_label = str(path.resolve())
        digest.update(path_label.encode("utf-8") + b"\0")
        file_digest = (
            BUILD_PIPELINE_FINGERPRINT
            if path.resolve() == Path(__file__).resolve()
            else file_sha256(path)
        )
        digest.update(file_digest.encode("ascii") + b"\0")
    for name, size in sorted(function_sizes.items()):
        digest.update(f"{name}:{size}\n".encode("ascii"))
    for name, address in auto_symbol_definitions():
        digest.update(f"{name}:{address}\n".encode("ascii"))
    digest.update(str(C_FRONTEND).encode("utf-8"))
    return digest.hexdigest()


def c_object_fingerprint(
    source, flags, functions, overrides, shared_fingerprint
):
    relative = source.relative_to(ROOT)
    assembly_path = BUILD / "c-input" / relative.with_suffix(".s")
    digest = hashlib.sha256()
    digest.update(shared_fingerprint.encode("ascii"))
    digest.update(relative.as_posix().encode("utf-8") + b"\0")
    digest.update(source.read_bytes())
    digest.update(json.dumps(flags, separators=(",", ":")).encode("utf-8"))
    for name in sorted(functions & overrides.keys()):
        override_path = ROOT / overrides[name]
        digest.update(name.encode("ascii") + b"\0")
        digest.update(overrides[name].encode("utf-8") + b"\0")
        digest.update(file_sha256(override_path).encode("ascii") + b"\0")
    return digest.hexdigest(), assembly_path


def load_object_cache():
    if not OBJECT_CACHE.is_file():
        return {}
    try:
        cache = json.loads(OBJECT_CACHE.read_text(encoding="utf-8"))
    except (OSError, ValueError) as error:
        print(
            f"build: warning: ignoring unreadable object cache: {error}",
            file=sys.stderr,
        )
        return {}
    if not isinstance(cache, dict) or any(
        not isinstance(key, str)
        or not isinstance(entry, dict)
        or not all(
            isinstance(entry.get(field), str)
            for field in ("fingerprint", "assembly_sha256", "object_sha256")
        )
        or not isinstance(entry.get("functions"), list)
        or any(
            not isinstance(name, str) or not FUNCTION_SYMBOL.fullmatch(name)
            for name in entry["functions"]
        )
        for key, entry in cache.items()
    ):
        print("build: warning: ignoring malformed object cache", file=sys.stderr)
        return {}
    return cache


def save_object_cache(cache):
    temporary_path = OBJECT_CACHE.with_name(OBJECT_CACHE.name + ".tmp")
    temporary_path.write_text(
        json.dumps(cache, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
        newline="\n",
    )
    temporary_path.replace(OBJECT_CACHE)


def cached_c_compile_result(
    source,
    object_path,
    flags,
    function_sizes,
    overrides,
    shared_fingerprint,
    cache,
):
    relative = source.relative_to(ROOT).as_posix()
    entry = cache.get(relative)
    if entry is None:
        return None
    functions = set(entry["functions"])
    fingerprint, assembly_path = c_object_fingerprint(
        source, flags, functions, overrides, shared_fingerprint
    )
    if (
        entry["fingerprint"] != fingerprint
        or not object_path.is_file()
        or not assembly_path.is_file()
    ):
        return None
    try:
        if (
            entry["assembly_sha256"] != file_sha256(assembly_path)
            or entry["object_sha256"] != file_sha256(object_path)
        ):
            return None
    except OSError:
        return None

    assembly = assembly_path.read_text(encoding="utf-8")
    padding_matches = list(AUTO_PADDING.finditer(assembly))
    padded_functions = {match.group(2) for match in padding_matches}
    padding_bytes = sum(int(match.group(3)) for match in padding_matches)
    object_elf = Elf32LE(object_path)
    oversized = []
    for name in functions:
        retail_size = function_sizes.get(name)
        if retail_size is None:
            continue
        symbol = object_elf.symbol(name)
        if symbol["type"] != 2 or symbol["size"] <= 0:
            raise ValueError(f"{object_path}: {name} has no nonempty function symbol")
        if symbol["size"] > retail_size:
            oversized.append((name, symbol["size"], retail_size))

    extra_sections = extra_data_sections(object_path)
    if extra_sections:
        EXTRA_SECTIONS.append((relative, extra_sections))
    applied_overrides = functions & overrides.keys()
    return len(padded_functions), padding_bytes, applied_overrides, oversized


def update_object_cache(
    source, object_path, flags, functions, overrides, shared_fingerprint, cache
):
    fingerprint, assembly_path = c_object_fingerprint(
        source, flags, functions, overrides, shared_fingerprint
    )
    cache[source.relative_to(ROOT).as_posix()] = {
        "fingerprint": fingerprint,
        "assembly_sha256": file_sha256(assembly_path),
        "object_sha256": file_sha256(object_path),
        "functions": sorted(functions),
    }


def function_unit_fingerprint(
    module, unit, flags, overrides, shared_fingerprint
):
    relative = (
        FUNCTION_UNIT_DIR
        / module.path.relative_to(ROOT).stem
        / f"{unit.name}.i"
    ).relative_to(ROOT)
    digest = hashlib.sha256()
    digest.update(shared_fingerprint.encode("ascii"))
    digest.update(relative.as_posix().encode("utf-8") + b"\0")
    digest.update(unit.source_digest.encode("ascii"))
    digest.update(json.dumps(flags, separators=(",", ":")).encode("utf-8"))
    if unit.name in overrides:
        override_path = ROOT / overrides[unit.name]
        digest.update(unit.name.encode("ascii") + b"\0")
        digest.update(overrides[unit.name].encode("utf-8") + b"\0")
        digest.update(file_sha256(override_path).encode("ascii") + b"\0")
    return digest.hexdigest()


def function_unit_source_sha256(unit):
    digest = hashlib.sha256()
    for part in unit.prefix_parts:
        digest.update(part.encode("utf-8"))
    digest.update(unit.body.encode("utf-8"))
    for part in unit.postlude_parts:
        digest.update(part.encode("utf-8"))
    return digest.hexdigest()


def cache_file_matches(entry, path, key_prefix):
    if not path.is_file():
        return False
    stat = path.stat()
    size_key = f"{key_prefix}_size"
    modified_key = f"{key_prefix}_mtime_ns"
    if (
        entry.get(size_key) == stat.st_size
        and entry.get(modified_key) == stat.st_mtime_ns
    ):
        return True
    expected_hash = entry.get(f"{key_prefix}_sha256")
    if not expected_hash or expected_hash != file_sha256(path):
        return False
    entry[size_key] = stat.st_size
    entry[modified_key] = stat.st_mtime_ns
    return True


def cached_function_unit_result(
    cache, key, fingerprint, unit, object_path, overrides
):
    entry = cache.get(key)
    if (
        entry is None
        or entry.get("unit_fingerprint") != fingerprint
        or entry.get("unit_source_digest") != unit.source_digest
        or not isinstance(entry.get("unit_metrics"), dict)
        or not cache_file_matches(entry, object_path, "object")
    ):
        return None
    metrics = entry["unit_metrics"]
    if (
        not isinstance(metrics.get("padded_functions"), int)
        or not isinstance(metrics.get("padding_bytes"), int)
        or not isinstance(metrics.get("oversized"), list)
        or not isinstance(metrics.get("extra_sections"), dict)
    ):
        return None
    extra_sections = metrics["extra_sections"]
    if extra_sections:
        EXTRA_SECTIONS.append(
            (key, {name: int(size) for name, size in extra_sections.items()})
        )
    applied_overrides = {unit.name} if unit.name in overrides else set()
    oversized = [tuple(item) for item in metrics["oversized"]]
    return (
        metrics["padded_functions"],
        metrics["padding_bytes"],
        applied_overrides,
        oversized,
    )


def store_function_unit_metadata(
    cache,
    key,
    fingerprint,
    unit,
    result,
    assembly_path,
    object_path,
):
    entry = cache[key]
    extra_sections = extra_data_sections(object_path)
    entry["unit_fingerprint"] = fingerprint
    entry["unit_source_digest"] = unit.source_digest
    entry["unit_metrics"] = {
        "padded_functions": result[0],
        "padding_bytes": result[1],
        "oversized": [list(item) for item in result[3]],
        "extra_sections": extra_sections,
    }
    for prefix, path in (
        ("assembly", assembly_path),
        ("object", object_path),
    ):
        stat = path.stat()
        entry[f"{prefix}_size"] = stat.st_size
        entry[f"{prefix}_mtime_ns"] = stat.st_mtime_ns


def compile_function_unit(
    module,
    unit,
    flags,
    log,
    function_sizes,
    overrides,
    shared_fingerprint,
    cache,
    no_cache,
):
    module_path = module.path.relative_to(ROOT)
    source_path = FUNCTION_UNIT_DIR / module_path.stem / f"{unit.name}.i"
    object_path = source_path.with_suffix(".o")
    assembly_path = (
        BUILD
        / "c-input"
        / source_path.relative_to(ROOT).with_suffix(".s")
    )
    cache_key = source_path.relative_to(ROOT).as_posix()

    cache_hit = False
    metadata_needs_update = False
    result = None
    if not no_cache:
        fingerprint = function_unit_fingerprint(
            module, unit, flags, overrides, shared_fingerprint
        )
        result = cached_function_unit_result(
            cache, cache_key, fingerprint, unit, object_path, overrides
        )
        if result is None:
            entry = cache.get(cache_key)
            if entry is not None and not entry.get("unit_fingerprint"):
                if (
                    source_path.is_file()
                    and function_unit_source_sha256(unit)
                    == file_sha256(source_path)
                ):
                    result = cached_c_compile_result(
                        source_path,
                        object_path,
                        flags,
                        function_sizes,
                        overrides,
                        shared_fingerprint,
                        cache,
                    )
                    metadata_needs_update = result is not None
        cache_hit = result is not None
    if result is None:
        source_path.parent.mkdir(parents=True, exist_ok=True)
        source_path.write_text(unit.source(), encoding="utf-8", newline="\n")
        result = compile_c_source(
            source_path,
            object_path,
            flags,
            log,
            function_sizes,
            overrides,
        )
        if result is None:
            return None, False, None
        if not no_cache:
            update_object_cache(
                source_path,
                object_path,
                flags,
                result[4],
                overrides,
                shared_fingerprint,
                cache,
            )
            metadata_needs_update = True
    compiled_functions = set(result[4]) if len(result) > 4 else {unit.name}
    if compiled_functions != {unit.name}:
        fail(
            f"{source_path.relative_to(ROOT).as_posix()} generated "
            f"{sorted(compiled_functions)}, expected only {unit.name}"
        )
        return None, cache_hit, None
    if not no_cache and metadata_needs_update:
        store_function_unit_metadata(
            cache,
            cache_key,
            fingerprint,
            unit,
            result,
            assembly_path,
            object_path,
        )
    for generated_path in (source_path, assembly_path):
        if generated_path.is_file():
            generated_path.unlink()
    return object_path, cache_hit, result


def assembly_unit_fingerprint(module, units, flags, shared_fingerprint):
    digest = hashlib.sha256()
    digest.update(str(shared_fingerprint).encode("ascii"))
    digest.update(module.path.relative_to(ROOT).as_posix().encode("utf-8") + b"\0")
    digest.update(json.dumps(flags, separators=(",", ":")).encode("utf-8"))
    for unit in units:
        path = ROOT / unit.include_path
        digest.update(unit.name.encode("ascii") + b"\0")
        digest.update(unit.include_path.encode("utf-8") + b"\0")
        digest.update(file_sha256(path).encode("ascii") + b"\0")
    return digest.hexdigest()


def cached_assembly_unit(cache, key, fingerprint, source_path, object_path):
    entry = cache.get(key)
    if (
        entry is None
        or entry["fingerprint"] != fingerprint
        or not source_path.is_file()
        or not object_path.is_file()
    ):
        return False
    try:
        return (
            entry["assembly_sha256"] == file_sha256(source_path)
            and entry["object_sha256"] == file_sha256(object_path)
        )
    except OSError:
        return False


def update_assembly_unit_cache(
    cache, key, fingerprint, source_path, object_path, units
):
    cache[key] = {
        "fingerprint": fingerprint,
        "assembly_sha256": file_sha256(source_path),
        "object_sha256": file_sha256(object_path),
        "functions": [unit.name for unit in units],
    }


def compile_assembly_units(
    module, units, flags, log, cache, shared_fingerprint, no_cache
):
    relative = module.path.relative_to(ROOT)
    first = units[0].name
    source_path = (
        FUNCTION_UNIT_DIR
        / relative.stem
        / f"asm_{first}.s"
    )
    object_path = source_path.with_suffix(".o")
    source_path.parent.mkdir(parents=True, exist_ok=True)
    source_path.write_text(
        '\t.include "include/macro.inc"\n'
        + "".join(
            f'\t.set noreorder\n\t.include "{unit.include_path}"\n\t.set reorder\n'
            for unit in units
        ),
        encoding="utf-8",
        newline="\n",
    )
    flags = list(flags)
    key = f"asm:{relative.as_posix()}:{first}"
    fingerprint = assembly_unit_fingerprint(
        module, units, flags, shared_fingerprint
    )
    if not no_cache and cached_assembly_unit(
        cache, key, fingerprint, source_path, object_path
    ):
        return object_path, True
    if not compile_assembly_source(source_path, object_path, flags, log):
        return None, False
    update_assembly_unit_cache(
        cache, key, fingerprint, source_path, object_path, units
    )
    return object_path, False


def link_module_objects(module, unit_objects, log):
    module_output = BUILD / "function_modules" / f"{module.path.stem}.o"
    module_output.parent.mkdir(parents=True, exist_ok=True)
    inputs = list(unit_objects)
    if not inputs:
        raise ValueError(f"{module.path.name}: no function objects were generated")

    level = 0
    while len(inputs) > 64:
        next_inputs = []
        for group_index, start in enumerate(range(0, len(inputs), 64)):
            output = (
                BUILD
                / "function_modules"
                / f"{module.path.stem}.part{level}.{group_index}.o"
            )
            group = inputs[start:start + 64]
            if not compile_source(
                [
                    str(LINKER),
                    "-r",
                    "-o",
                    str(output),
                    *(str(path) for path in group),
                ],
                f"partial link {module.path.name} group {group_index}",
                log,
            ):
                return None
            next_inputs.append(output)
        inputs = next_inputs
        level += 1

    if not compile_source(
        [
            str(LINKER),
            "-r",
            "-o",
            str(module_output),
            *(str(path) for path in inputs),
        ],
        f"partial link {module.path.name}",
        log,
    ):
        return None
    return module_output


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
    entry_source = ROOT / "src" / "funcs_00_00100008_001000B8.c"
    if modules[0].path != entry_source:
        raise ValueError(f"{manifest_name} must list the CRT entry module first")
    return modules, tuple(global_flags)


def report_oversized_functions(oversized, retail_sizes):
    """Check C functions that compile to more bytes than their retail size.

    Every function is laid out at its retail address, so an oversized function
    is only harmless while it still fits the gap up to the next retail function
    (the assembler pads each function to 8 bytes). A larger one pushes every
    later symbol and eventually overruns .main, which is what the link reports
    as ".main_bss overlaps .main". Returns 1 when a function exceeds its slot.
    """
    if not oversized:
        return 0

    addresses = sorted(int(name[5:], 16) for name in retail_sizes)
    following = {address: next_address for address, next_address in zip(addresses, addresses[1:])}
    failed = False
    for name, built, retail in oversized:
        address = int(name[5:], 16)
        slot = following.get(address, address + retail) - address
        if built > slot:
            print(
                f"build: {name} builds to {built} bytes, past its {slot}-byte "
                f"retail slot (retail size {retail}); later functions would shift",
                file=sys.stderr,
            )
            failed = True
            continue
        print(
            f"build: note: {name} builds to {built} bytes (retail {retail}), "
            f"inside the alignment gap before the next function",
            file=sys.stderr,
        )
    return 1 if failed else 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--no-cache",
        action="store_true",
        help="rebuild every function unit and fallback assembly group",
    )
    args = parser.parse_args()

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
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    LOG.write_text("", encoding="utf-8")
    objects = []
    common_flags = ["-G4", "-Iinclude", "-Isrc", "-Wa,-I,include,-I,."]
    try:
        function_sizes = retail_function_sizes()
        asm_overrides = load_asm_overrides()
        object_cache = {} if args.no_cache else load_object_cache()
        shared_fingerprint = (
            None if args.no_cache else shared_object_fingerprint(function_sizes)
        )
    except (OSError, UnicodeError, ValueError) as error:
        return fail(f"could not read build function inputs: {error}")
    padded_functions = 0
    padding_bytes = 0
    applied_asm_overrides = set()
    oversized_functions = []
    cache_hits = 0
    cache_misses = 0

    try:
        modules, global_flags = source_modules()
    except (OSError, UnicodeError, ValueError) as error:
        return fail(f"could not read C source manifest: {error}")
    for module in modules:
        source = module.path
        relative = source.relative_to(ROOT)
        object_path = BUILD / relative.parent / f"{source.name}.o"
        if source.suffix == ".c":
            module_flags = [*common_flags, *c_flags(module, global_flags)]
            preprocessed = preprocessed_source(source, module_flags, LOG)
            if preprocessed is None:
                return 1
            try:
                units = list(
                    iter_function_units(
                        preprocessed.read_text(encoding="utf-8")
                    )
                )
            except (OSError, UnicodeError, ValueError) as error:
                return fail(
                    f"could not split C functions in "
                    f"{relative.as_posix()}: {error}"
                )

            ordered_items = []
            assembly_run = []
            for unit in units:
                if isinstance(unit, AssemblyFunctionUnit):
                    assembly_run.append(unit)
                    continue
                if assembly_run:
                    ordered_items.append(assembly_run)
                    assembly_run = []
                ordered_items.append(unit)
            if assembly_run:
                ordered_items.append(assembly_run)

            unit_objects = []
            for item in ordered_items:
                if isinstance(item, list):
                    try:
                        run_object, cache_hit = compile_assembly_units(
                            module,
                            item,
                            [
                                *common_flags,
                                *assembly_flags((), module.flags),
                            ],
                            LOG,
                            object_cache,
                            shared_fingerprint,
                            args.no_cache,
                        )
                    except (OSError, UnicodeError, ValueError) as error:
                        return fail(
                            f"could not compile retail assembly units in "
                            f"{relative.as_posix()}: {error}"
                        )
                    if cache_hit:
                        cache_hits += 1
                    else:
                        cache_misses += 1
                    if run_object is None:
                        return 1
                    unit_objects.append(run_object)
                    continue

                try:
                    unit_object, cache_hit, compile_result = compile_function_unit(
                        module,
                        item,
                        module_flags,
                        LOG,
                        function_sizes,
                        asm_overrides,
                        shared_fingerprint,
                        object_cache,
                        args.no_cache,
                    )
                except (OSError, UnicodeError, ValueError) as error:
                    return fail(
                        f"could not compile {item.name} in "
                        f"{relative.as_posix()}: {error}"
                    )
                if unit_object is None or compile_result is None:
                    return 1
                if cache_hit:
                    cache_hits += 1
                else:
                    cache_misses += 1
                padded_functions += compile_result[0]
                padding_bytes += compile_result[1]
                oversized_functions.extend(compile_result[3])
                duplicate_overrides = (
                    applied_asm_overrides & compile_result[2]
                )
                if duplicate_overrides:
                    return fail(
                        "multiple C modules define overridden functions: "
                        + ", ".join(sorted(duplicate_overrides))
                    )
                applied_asm_overrides.update(compile_result[2])
                unit_objects.append(unit_object)

            module_object = link_module_objects(module, unit_objects, LOG)
            if module_object is None:
                return 1
            objects.append(module_object)
            if not args.no_cache:
                try:
                    save_object_cache(object_cache)
                except (OSError, UnicodeError, ValueError) as error:
                    print(
                        f"build: warning: could not save function object cache: "
                        f"{error}",
                        file=sys.stderr,
                    )
        elif not compile_assembly_source(
            source,
            object_path,
            [*common_flags, *assembly_flags((), module.flags)],
            LOG,
        ):
            return 1
        else:
            objects.append(object_path)

    if not args.no_cache:
        print(
            f"Function object cache: reused {cache_hits}, rebuilt {cache_misses} "
            f"of {cache_hits + cache_misses} units."
        )
    if report_oversized_functions(oversized_functions, function_sizes):
        return 1
    report_extra_sections(EXTRA_SECTIONS)

    unapplied_overrides = set(asm_overrides) - applied_asm_overrides
    if unapplied_overrides:
        return fail(
            "asm_overrides.txt names functions that are not compiled C "
            "definitions: " + ", ".join(sorted(unapplied_overrides))
        )

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

    data_labels = data_label_aliases(objects)
    if data_labels:
        label_source = BUILD / "data_labels.s"
        label_source.write_text(
            "/* Generated by tools/build.py. Addresses of retail code labels that\n"
            "   only the data blobs still reference, for functions now written in C.\n"
            "   Each value comes from the label name, which splat derives from the\n"
            "   label's address. */\n"
            + "".join(
                f"\t.globl {name}\n\t.equ {name}, 0x{address:08X}\n"
                for name, address in data_labels
            ),
            encoding="ascii",
            newline="\n",
        )
        label_object = BUILD / "data_labels.s.o"
        if not compile_assembly_source(label_source, label_object, common_flags, LOG):
            return 1
        objects.append(label_object)
        print(f"Restored {len(data_labels)} retail code label(s) named only by data.")

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
        "Retail assembly overrides: "
        + (", ".join(sorted(asm_overrides)) if asm_overrides else "none")
    )
    print(
        f"Padded {padded_functions} undersized C functions "
        f"with {padding_bytes} bytes of trailing nops."
    )
    print(f"Build diagnostics: {LOG.relative_to(ROOT)}")
    print(status_result.stdout, end="")
    return 0


if __name__ == "__main__":
    sys.exit(main())