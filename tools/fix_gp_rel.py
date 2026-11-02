#!/usr/bin/env python3
"""Adapt spimdisasm GP-relative operands for the SN Systems EE assembler."""

import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ASM_ROOT = ROOT / "asm"
CONFIG = ROOT / "config" / "splat.yaml"
EXTERNS = ROOT / "build" / "gp_externs.inc"
GP_VALUE = re.compile(r"(?m)^\s*gp_value:\s*(0x[0-9A-Fa-f]+)\s*$")
GP_REL = re.compile(
    r"%gp_rel\(\s*(D_[0-9A-Fa-f]+)"
    r"(?:(\s*[+-]\s*)(0x[0-9A-Fa-f]+|\d+))?\s*\)"
)
INSTRUCTION = re.compile(r"^(.*?\*/\s*)([A-Za-z0-9_.]+)\s+(.*)$")
GP_ADDRESS = re.compile(r"^\$(\d+)\s*,\s*\$28\s*,\s*(%gp_rel\(.*\))$")
GP_MEMORY = re.compile(r"^(\$(?:\d+|f\d+))\s*,\s*(%gp_rel\(.*\))\(\$28\)$")
GP_DIRECT_MEMORY = re.compile(
    r"^(\$(?:\d+|f\d+))\s*,\s*(D_[0-9A-Fa-f]+)"
    r"(?:(\s*[+-]\s*)(0x[0-9A-Fa-f]+|\d+))?$"
)
GP_LOW_EDGE = -0x7FE4


def gp_offset(symbol, operator, addend):
    offset = int(symbol[2:], 16)
    if addend:
        value = int(addend, 0)
        offset += value if operator.strip() == "+" else -value
    return offset


def gp_memory_operand(register, symbol, operator, addend, gp_value):
    offset = gp_offset(symbol, operator, addend) - gp_value
    if not -0x8000 <= offset <= 0x7FFF:
        raise ValueError(f"{symbol} is outside the signed GP-relative range")
    if offset <= GP_LOW_EDGE:
        return f"{register}, {offset}($28)"
    expression = symbol
    if operator:
        expression += operator.strip() + addend
    return f"{register}, {expression}"


def main():
    if not ASM_ROOT.is_dir():
        raise SystemExit("fix_gp_rel: asm/ is missing; run tools/setup_asm.py first")
    config = CONFIG.read_text(encoding="utf-8")
    gp_match = GP_VALUE.search(config)
    if not gp_match:
        raise ValueError(f"{CONFIG}: missing gp_value")
    gp_value = int(gp_match.group(1), 16)

    symbols = set()
    changed = 0
    for path in sorted(ASM_ROOT.rglob("*.s")):
        original = path.read_text(encoding="utf-8")

        updated_lines = []
        for line in original.splitlines(keepends=True):
            instruction = INSTRUCTION.match(line.rstrip("\r\n"))
            if not instruction:
                updated_lines.append(line)
                continue

            operands = instruction.group(3)
            direct_memory = GP_DIRECT_MEMORY.fullmatch(operands)
            if direct_memory:
                symbol = direct_memory.group(2)
                operator = direct_memory.group(3) or ""
                addend = direct_memory.group(4) or ""
                symbols.add(symbol)
                offset = gp_offset(symbol, operator, addend) - gp_value
                if offset <= GP_LOW_EDGE:
                    replacement = gp_memory_operand(
                        direct_memory.group(1), symbol, operator, addend, gp_value
                    )
                    updated_lines.append(
                        instruction.group(1)
                        + instruction.group(2)
                        + " "
                        + replacement
                        + line[len(line.rstrip("\r\n")):]
                    )
                    continue
            if "%gp_rel(" not in operands:
                updated_lines.append(line)
                continue

            address = GP_ADDRESS.match(operands)
            memory = GP_MEMORY.match(operands)
            if address:
                expression = address.group(2)
                gp_operand = True
            elif memory:
                expression = memory.group(2)
                gp_operand = False
            else:
                raise ValueError(f"{path}: unsupported GP-relative operand: {line.strip()}")

            symbol_match = GP_REL.fullmatch(expression)
            if not symbol_match:
                raise ValueError(f"{path}: malformed GP-relative operand: {line.strip()}")
            symbol = symbol_match.group(1)
            symbols.add(symbol)
            addend = ""
            if symbol_match.group(2):
                addend = symbol_match.group(2).strip() + symbol_match.group(3)
            if gp_operand:
                replacement = f"la ${address.group(1)}, {symbol}{addend}"
            else:
                symbol_match_addend = GP_REL.fullmatch(expression)
                replacement = (
                    f"{instruction.group(2)} "
                    + gp_memory_operand(
                        memory.group(1),
                        symbol_match_addend.group(1),
                        symbol_match_addend.group(2) or "",
                        symbol_match_addend.group(3) or "",
                        gp_value,
                    )
                )
            updated_lines.append(
                instruction.group(1) + replacement + line[len(line.rstrip("\r\n")):]
            )

        updated = "".join(updated_lines)
        if updated != original:
            path.write_text(updated, encoding="utf-8", newline="\n")
            changed += 1

    EXTERNS.parent.mkdir(parents=True, exist_ok=True)
    EXTERNS.write_text(
        "".join(f".extern {symbol}, 1\n" for symbol in sorted(symbols)),
        encoding="ascii",
        newline="\n",
    )
    print(f"Adapted GP-relative operands in {changed} files; declared {len(symbols)} small-data symbols.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
