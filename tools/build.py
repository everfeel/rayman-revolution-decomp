#!/usr/bin/env python3
"""Build the main Rayman Revolution ELF with the configured SN Systems SDK."""

import re
import subprocess
import sys
from pathlib import Path


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


def make_legacy_linker_script():
    script = LINKER_SCRIPT.read_text(encoding="utf-8")
    script = script.replace("    HIDDEN(__romPos = 0);", "    __romPos = 0;")
    script = re.sub(
        r"(?m)^\s*build/[^()\n]+\(([^)\n]+)\);$",
        lambda match: f"        *({match.group(1)});",
        script,
    )
    script = script.replace("ALIGN(., 16)", "ALIGN(16)")
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
    legacy_script = BUILD / "SLES_500.44.sn.ld"
    legacy_script.write_text(script, encoding="ascii", newline="\n")
    return legacy_script


def source_modules():
    if not MODULE_MANIFEST.is_file():
        raise FileNotFoundError(
            f"{MODULE_MANIFEST.relative_to(ROOT)} is missing"
        )

    modules = []
    for line_number, line in enumerate(
        MODULE_MANIFEST.read_text(encoding="utf-8").splitlines(), start=1
    ):
        source_name = line.strip()
        if not source_name or source_name.startswith("#"):
            continue
        relative = Path(source_name.replace("/", "\\"))
        if relative.is_absolute() or relative.suffix != ".c" or ".." in relative.parts:
            raise ValueError(
                f"{MODULE_MANIFEST.relative_to(ROOT)}:{line_number}: "
                f"invalid source path {source_name!r}"
            )
        source = ROOT / "src" / relative
        if not source.is_file():
            raise FileNotFoundError(
                f"{MODULE_MANIFEST.relative_to(ROOT)}:{line_number}: "
                f"missing source {source.relative_to(ROOT)}"
            )
        modules.append(source)
    if not modules:
        raise ValueError(f"{MODULE_MANIFEST.relative_to(ROOT)} has no source files")
    if len(set(modules)) != len(modules):
        raise ValueError(f"{MODULE_MANIFEST.relative_to(ROOT)} contains duplicate paths")
    return modules


def main():
    if not COMPILER.is_file():
        return fail(f"SDK compiler not found: {COMPILER}")
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
    common_flags = ["-O2", "-G4", "-Iinclude", "-Isrc", "-Wa,-I,include,-I,."]

    main_source = ROOT / "src" / "main.c"
    if not main_source.is_file():
        return fail(f"missing C source: {main_source.relative_to(ROOT)}")
    main_object = BUILD / "src" / "main.c.o"
    main_object.parent.mkdir(parents=True, exist_ok=True)
    if not compile_source(
        [str(COMPILER), *common_flags, "-c", str(main_source), "-o", str(main_object)],
        "compile src/main.c",
        LOG,
    ):
        return 1
    objects.append(main_object)

    try:
        modules = source_modules()
    except (OSError, UnicodeError, ValueError) as error:
        return fail(f"could not read C source manifest: {error}")
    for source in modules:
        relative = source.relative_to(ROOT)
        object_path = BUILD / relative.parent / f"{source.name}.o"
        object_path.parent.mkdir(parents=True, exist_ok=True)
        if not compile_source(
            [
                str(COMPILER),
                *common_flags,
                "-c",
                str(source),
                "-o",
                str(object_path),
            ],
            f"compile {relative.as_posix()}",
            LOG,
        ):
            return 1
        objects.append(object_path)

    data_root = ROOT / "asm" / "data"
    if not data_root.is_dir():
        return fail("asm/data is missing; run `py tools\\setup_asm.py` first")
    for source in sorted(data_root.rglob("*.s")):
        relative = source.relative_to(ROOT)
        object_path = BUILD / relative.parent / f"{source.name}.o"
        object_path.parent.mkdir(parents=True, exist_ok=True)
        try:
            assembler_source = assembly_input(source)
        except (OSError, ValueError) as error:
            return fail(str(error))
        if not compile_source(
            [
                str(COMPILER),
                *common_flags,
                "-c",
                str(assembler_source),
                "-o",
                str(object_path),
            ],
            f"assemble {relative.as_posix()}",
            LOG,
        ):
            return 1
        objects.append(object_path)

    try:
        legacy_script = make_legacy_linker_script()
        definitions = []
        for line in undefined_symbols.read_text(encoding="ascii").splitlines():
            match = AUTO_SYMBOL.fullmatch(line)
            if match:
                definitions.append(f"--defsym={match.group(1)}={match.group(2)}")
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
    print(f"Build diagnostics: {LOG.relative_to(ROOT)}")
    print(status_result.stdout, end="")
    return 0


if __name__ == "__main__":
    sys.exit(main())
