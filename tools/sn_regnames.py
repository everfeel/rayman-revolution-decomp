#!/usr/bin/env python3
"""Rewrite MIPS GPR aliases to the numeric names required by SN assembler."""

import argparse
import re
import sys
from pathlib import Path


REGISTER_NUMBERS = {
    "zero": 0, "at": 1, "v0": 2, "v1": 3,
    "a0": 4, "a1": 5, "a2": 6, "a3": 7,
    "t0": 8, "t1": 9, "t2": 10, "t3": 11,
    "t4": 12, "t5": 13, "t6": 14, "t7": 15,
    "s0": 16, "s1": 17, "s2": 18, "s3": 19,
    "s4": 20, "s5": 21, "s6": 22, "s7": 23,
    "t8": 24, "t9": 25, "k0": 26, "k1": 27,
    "gp": 28, "sp": 29, "s8": 30, "fp": 30, "ra": 31,
}
REGISTER = re.compile(r"\$(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b")


def convert(text):
    return REGISTER.sub(lambda match: f"${REGISTER_NUMBERS[match.group(1)]}", text)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="+", type=Path)
    args = parser.parse_args()
    changed = 0
    try:
        for root in args.paths:
            files = [root] if root.is_file() else sorted(root.rglob("*.s"))
            for path in files:
                original = path.read_text(encoding="utf-8")
                updated = convert(original)
                if updated != original:
                    path.write_text(updated, encoding="utf-8", newline="\n")
                    changed += 1
    except OSError as error:
        parser.exit(2, f"sn_regnames: {error}\n")
    print(f"Converted register aliases in {changed} assembly files.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
