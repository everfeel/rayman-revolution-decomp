#!/usr/bin/env python3
"""Temporarily swap C function bodies for retail assembly (INCLUDE_ASM).

Uses the same INCLUDE_ASM mechanism as the rest of the tree, so unlike
asm_overrides.txt it does NOT disturb module alignment/addresses.

Usage:
    python tools/swap_to_asm.py func_0010AC00 func_0018DA08 ...
    python tools/swap_to_asm.py --restore
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRCS = sorted((ROOT / "src").glob("*.c"))

DEF_RE = re.compile(
    r"(?m)^(?P<ret>(?:s32|u32|void|u8|s8|f32|u8\s*\*|s8\s*\*))\s+"
    r"(?P<name>func_[0-9A-Fa-f]{8})\s*\([^;{]*\)\s*(?:[^;{]*;\s*)*\{"
)


def orig_for(path):
    return path.with_suffix(path.suffix + ".swap_orig")


def find_def(name):
    for path in SRCS:
        text = path.read_text(encoding="utf-8", errors="replace")
        for m in DEF_RE.finditer(text):
            if m.group("name") != name:
                continue
            depth = 0
            i = text.find("{", m.start())
            j = i
            while j < len(text):
                if text[j] == "{":
                    depth += 1
                elif text[j] == "}":
                    depth -= 1
                    if depth == 0:
                        j += 1
                        break
                j += 1
            return path, m.start(), j
    return None


def swap(names):
    for name in names:
        found = find_def(name)
        if not found:
            print("NOT FOUND:", name)
            continue
        path, start, end = found
        if not orig_for(path).exists():
            orig_for(path).write_text(
                path.read_text(encoding="utf-8", errors="replace"), encoding="utf-8"
            )
        text = path.read_text(encoding="utf-8", errors="replace")
        text = text[:start] + f'INCLUDE_ASM(const s32, "core/main", {name});' + text[end:]
        path.write_text(text, encoding="utf-8")
        print("swapped", name, "->", path.name)


def restore():
    n = 0
    for orig in ROOT.glob("src/*.swap_orig"):
        target = orig.with_suffix("")
        target.write_text(orig.read_text(encoding="utf-8"), encoding="utf-8")
        orig.unlink()
        n += 1
    print("restored", n, "source file(s)")


if __name__ == "__main__":
    if "--restore" in sys.argv:
        restore()
    else:
        swap([a for a in sys.argv[1:] if a.startswith("func_")])
