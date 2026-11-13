#!/usr/bin/env python3
"""Extract the main PT_LOAD image and generate Rayman Revolution disassembly."""

import hashlib
import importlib.metadata
import re
import subprocess
import sys
from pathlib import Path

from compare import Elf32LE, RETAIL


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config" / "splat.yaml"
ROM = ROOT / "build" / "SLES_500.44.rom"
SPLAT_MAIN_SOURCE = ROOT / "src" / "core" / "main.c"
EXPECTED_RETAIL_SHA1 = "425c777247a56b4d6d7120088c70d970c45cc41b"
EXPECTED_ROM_SHA1 = "8a47cf3d4ca5a9b21380a494c8d6fcdf62d3207a"
EXPECTED_TOOL_VERSIONS = {
    "splat64": "0.50.0",
    "spimdisasm": "1.42.4",
    "rabbitizer": "1.16.2",
}


def verify_inputs():
    if not RETAIL.is_file():
        raise FileNotFoundError(f"Missing retail executable: {RETAIL}")
    if not CONFIG.is_file():
        raise FileNotFoundError(f"Missing Splat configuration: {CONFIG}")

    retail_sha1 = hashlib.sha1(RETAIL.read_bytes()).hexdigest()
    if retail_sha1 != EXPECTED_RETAIL_SHA1:
        raise ValueError(
            f"{RETAIL.name} SHA-1 is {retail_sha1}; "
            f"expected PAL executable {EXPECTED_RETAIL_SHA1}"
        )

    installed = {dist.metadata["Name"]: dist.version for dist in importlib.metadata.distributions()}
    mismatches = {
        name: (installed.get(name), expected)
        for name, expected in EXPECTED_TOOL_VERSIONS.items()
        if installed.get(name) != expected
    }
    if mismatches:
        details = ", ".join(
            f"{name}={actual or 'missing'} (need {expected})"
            for name, (actual, expected) in mismatches.items()
        )
        raise RuntimeError(f"Disassembly tools do not match pinned versions: {details}. "
                           "Run `python -m pip install -r requirements.txt`.")


def extract_main_image():
    elf = Elf32LE(RETAIL)
    if elf.elf_type != 2:
        raise ValueError(f"{RETAIL} is not an executable ELF")
    if len(elf.segments) != 1:
        raise ValueError(f"Expected one PT_LOAD segment, found {len(elf.segments)}")

    segment = elf.segments[0]
    if segment["vaddr"] != 0x00100000 or segment["filesz"] != 0x293BF0:
        raise ValueError(
            "Unexpected retail PT_LOAD layout: "
            f"vaddr={segment['vaddr']:#x}, filesz={segment['filesz']:#x}"
        )
    image = elf.data[segment["offset"]:segment["offset"] + segment["filesz"]]
    image_sha1 = hashlib.sha1(image).hexdigest()
    if image_sha1 != EXPECTED_ROM_SHA1:
        raise ValueError(
            f"Extracted main-image SHA-1 is {image_sha1}; "
            f"expected {EXPECTED_ROM_SHA1}"
        )

    ROM.parent.mkdir(parents=True, exist_ok=True)
    ROM.write_bytes(image)
    print(f"Extracted {len(image):#x} bytes at {segment['vaddr']:#x} to {ROM.relative_to(ROOT)}")


def make_assembly_paths_portable():
    root_prefix = ROOT.as_posix().rstrip("/") + "/"
    for path in (ROOT / "asm" / "data").rglob("*.s"):
        source = path.read_text(encoding="utf-8")
        portable = re.sub(
            r'(\.incbin\s+")' + re.escape(root_prefix),
            r"\1",
            source,
        )
        if portable != source:
            path.write_text(portable, encoding="utf-8", newline="\n")

    function_dir = ROOT / "asm" / "nonmatchings"
    for path in function_dir.rglob("func_*.s"):
        source = path.read_text(encoding="utf-8")
        portable = re.sub(
            r"(?m)^(\s*)ehlabel (func_[0-9A-Fa-f]{8})$",
            r"\1glabel \2",
            source,
            count=1,
        )
        portable = re.sub(
            r"(?m)^jlabel (func_[0-9A-Fa-f]{8})$",
            r"glabel \1",
            portable,
            count=1,
        )
        if portable != source:
            path.write_text(portable, encoding="utf-8", newline="\n")


def main():
    try:
        verify_inputs()
        extract_main_image()
        if SPLAT_MAIN_SOURCE.exists():
            raise FileExistsError(
                f"Legacy Splat source path already exists: "
                f"{SPLAT_MAIN_SOURCE.relative_to(ROOT)}"
            )
        # Splat requires the main C source path during splitting; startup
        # padding and code are maintained in src/asm/func_00100008.s.
        SPLAT_MAIN_SOURCE.parent.mkdir(parents=True)
        SPLAT_MAIN_SOURCE.write_text(
            "",
            encoding="utf-8",
            newline="\n",
        )
        try:
            subprocess.run(
                [sys.executable, "-m", "splat", "split", str(CONFIG)],
                cwd=ROOT,
                check=True,
            )
        finally:
            SPLAT_MAIN_SOURCE.unlink()
            SPLAT_MAIN_SOURCE.parent.rmdir()
        make_assembly_paths_portable()
        subprocess.run(
            [sys.executable, "tools/sn_regnames.py", "asm"],
            cwd=ROOT,
            check=True,
        )
        subprocess.run(
            [sys.executable, "tools/fix_gp_rel.py"],
            cwd=ROOT,
            check=True,
        )
    except (OSError, RuntimeError, ValueError, subprocess.CalledProcessError) as error:
        print(f"setup_asm: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
