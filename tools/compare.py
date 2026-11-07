#!/usr/bin/env python3
"""Compare a linked ELF image or one function against SLES_500.44."""

import argparse
import re
import struct
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
RETAIL = ROOT / "baserom" / "SLES_500.44"
PT_LOAD = 1
SHT_SYMTAB = 2
SHF_EXECINSTR = 0x4


class Elf32LE:
    def __init__(self, path):
        self.path = Path(path)
        self.data = self.path.read_bytes()
        if len(self.data) < 52 or self.data[:4] != b"\x7fELF":
            raise ValueError(f"{self.path}: not an ELF file")
        if self.data[4:6] != b"\x01\x01":
            raise ValueError(f"{self.path}: expected 32-bit little-endian ELF")

        header = struct.unpack_from("<16sHHIIIIIHHHHHH", self.data)
        self.elf_type, self.machine = header[1:3]
        self.entry = header[4]
        phoff, shoff = header[5:7]
        self.phentsize, phnum = header[9:11]
        self.shentsize, shnum, shstrndx = header[11:14]

        if self.machine != 8:
            raise ValueError(f"{self.path}: expected MIPS ELF (machine 8), got {self.machine}")
        if (phnum and self.phentsize < 32) or self.shentsize < 40:
            raise ValueError(f"{self.path}: invalid ELF header table sizes")

        self.segments = []
        for index in range(phnum):
            offset = phoff + index * self.phentsize
            self._check_range(offset, 32, "program header")
            fields = struct.unpack_from("<IIIIIIII", self.data, offset)
            if fields[0] == PT_LOAD:
                segment = {
                    "offset": fields[1],
                    "vaddr": fields[2],
                    "filesz": fields[4],
                    "memsz": fields[5],
                }
                self._check_range(segment["offset"], segment["filesz"], "load segment")
                self.segments.append(segment)

        self.sections = []
        for index in range(shnum):
            offset = shoff + index * self.shentsize
            self._check_range(offset, 40, "section header")
            fields = struct.unpack_from("<IIIIIIIIII", self.data, offset)
            section = {
                "name_offset": fields[0],
                "type": fields[1],
                "flags": fields[2],
                "addr": fields[3],
                "offset": fields[4],
                "size": fields[5],
                "link": fields[6],
                "entsize": fields[9],
            }
            if section["type"] != 8:
                self._check_range(section["offset"], section["size"], "section")
            self.sections.append(section)

        if shstrndx >= len(self.sections):
            raise ValueError(f"{self.path}: invalid section-name string table index")
        self.section_names = self._string_table(self.sections[shstrndx])

    def _check_range(self, offset, size, label):
        if offset > len(self.data) or size > len(self.data) - offset:
            raise ValueError(f"{self.path}: {label} extends beyond end of file")

    def _string_table(self, section):
        return self.data[section["offset"]:section["offset"] + section["size"]]

    @staticmethod
    def _string_at(table, offset):
        if offset >= len(table):
            return ""
        end = table.find(b"\0", offset)
        if end < 0:
            end = len(table)
        return table[offset:end].decode("ascii", "replace")

    def bytes_at_vaddr(self, vaddr, size):
        for segment in self.segments:
            start = segment["vaddr"]
            end = start + segment["filesz"]
            if start <= vaddr and vaddr + size <= end:
                offset = segment["offset"] + vaddr - start
                return self.data[offset:offset + size]

        for section in self.sections:
            start = section["addr"]
            end = start + section["size"]
            if section["flags"] & SHF_EXECINSTR and start <= vaddr and vaddr + size <= end:
                offset = section["offset"] + vaddr - start
                return self.data[offset:offset + size]
        raise ValueError(
            f"{self.path}: virtual range {vaddr:#x}..{vaddr + size:#x} "
            "is not file-backed (overlay functions need an extracted overlay image)"
        )

    def symbol(self, name):
        for section in self.sections:
            if section["type"] != SHT_SYMTAB:
                continue
            if section["link"] >= len(self.sections):
                raise ValueError(f"{self.path}: symbol table has an invalid string table link")
            strings = self._string_table(self.sections[section["link"]])
            entry_size = section["entsize"] or 16
            if entry_size < 16 or section["size"] % entry_size:
                raise ValueError(f"{self.path}: malformed symbol table")
            count = section["size"] // entry_size
            for index in range(count):
                offset = section["offset"] + index * entry_size
                name_offset, value, size, info, _, _ = struct.unpack_from(
                    "<IIIBBH", self.data, offset
                )
                if self._string_at(strings, name_offset) == name:
                    return {"value": value, "size": size, "type": info & 0xF}
        raise ValueError(f"{self.path}: symbol {name!r} not found in a symbol table")


def parse_int(value):
    return int(value, 0)


def format_instruction(data):
    if not data:
        return "-"
    if len(data) == 4:
        return f"0x{int.from_bytes(data, 'little'):08X}"
    return f"bytes:{data.hex(' ').upper()}"


def report_difference(expected, actual, label, full=False):
    limit = min(len(expected), len(actual))
    differing = [index for index in range(limit) if expected[index] != actual[index]]
    exact_size = len(expected) == len(actual)
    print(f"{label}: retail={len(expected)} bytes, build={len(actual)} bytes")
    if not exact_size:
        print("  SIZE MISMATCH")
    if not differing and exact_size and not full:
        print("  MATCH")
        return True

    if not full:
        print(f"  differing bytes: {len(differing)} of {limit}")
    if differing and not full:
        index = differing[0]
        start = max(0, index - 8)
        end = min(limit, index + 16)
        print(f"  first difference at +{index:#x}")
        print(f"    retail {expected[start:end].hex(' ')}")
        print(f"    build  {actual[start:end].hex(' ')}")
    if full:
        print("  Offset   Retail instruction  Build instruction   Status")
        for offset in range(0, max(len(expected), len(actual)), 4):
            retail_word = expected[offset:offset + 4]
            build_word = actual[offset:offset + 4]
            status = "MATCH" if retail_word == build_word else "DIFF"
            print(
                f"  +0x{offset:04X}    "
                f"{format_instruction(retail_word):<20} "
                f"{format_instruction(build_word):<20} {status}"
            )
        if differing:
            print(f"  differing bytes: {len(differing)} of {limit}")
            print(f"  first difference at +{differing[0]:#x}")
        elif exact_size:
            print("  MATCH")
    return not differing and exact_size


def compare_image(retail, build):
    retail_segments = sorted(retail.segments, key=lambda segment: segment["vaddr"])
    build_segments = sorted(build.segments, key=lambda segment: segment["vaddr"])
    retail_file_segments = [segment for segment in retail_segments if segment["filesz"]]
    build_file_segments = [segment for segment in build_segments if segment["filesz"]]
    if len(retail_file_segments) != len(build_file_segments):
        print(
            f"file-backed load-segment count mismatch: retail={len(retail_file_segments)} "
            f"build={len(build_file_segments)}"
        )
        return False

    matched = True
    for index, (expected, actual) in enumerate(
        zip(retail_file_segments, build_file_segments)
    ):
        if expected["vaddr"] != actual["vaddr"]:
            print(
                f"segment {index}: address mismatch "
                f"retail={expected['vaddr']:#x} build={actual['vaddr']:#x}"
            )
            matched = False
            continue
        expected_bytes = retail.data[
            expected["offset"]:expected["offset"] + expected["filesz"]
        ]
        actual_bytes = build.data[
            actual["offset"]:actual["offset"] + actual["filesz"]
        ]
        matched = report_difference(
            expected_bytes, actual_bytes, f"load segment {index} at {expected['vaddr']:#x}"
        ) and matched

    def memory_extent(segments):
        if not segments:
            return None
        start = segments[0]["vaddr"]
        end = start
        for segment in segments:
            if segment["vaddr"] > end:
                return (start, end, False)
            end = max(end, segment["vaddr"] + segment["memsz"])
        return (start, end, True)

    expected_memory = memory_extent(retail_segments)
    actual_memory = memory_extent(build_segments)
    if expected_memory != actual_memory:
        print(
            "load-memory extent mismatch: "
            f"retail={expected_memory} build={actual_memory}"
        )
        matched = False
    return matched


def compare_function(
    retail, build, retail_address, build_address, retail_size, build_size, full=False
):
    expected = retail.bytes_at_vaddr(retail_address, retail_size)
    actual = build.bytes_at_vaddr(build_address, build_size)
    label = f"function retail={retail_address:#x} build={build_address:#x}"
    return report_difference(expected, actual, label, full=full)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    image_parser = subparsers.add_parser(
        "image", help="compare all file-backed PT_LOAD bytes"
    )
    image_parser.add_argument("build_elf", type=Path)

    function_parser = subparsers.add_parser(
        "function", help="compare one function by address or linked symbol"
    )
    function_parser.add_argument("build_elf", type=Path)
    function_parser.add_argument("--address", type=parse_int)
    function_parser.add_argument("--size", type=parse_int)
    function_parser.add_argument("--symbol")
    function_parser.add_argument("--retail-address", type=parse_int)
    function_parser.add_argument(
        "--full",
        action="store_true",
        help="show every retail/build machine-code word, including matches",
    )
    args = parser.parse_args()

    try:
        retail = Elf32LE(RETAIL)
        build = Elf32LE(args.build_elf)
        if args.command == "image":
            matched = compare_image(retail, build)
        else:
            retail_address = args.address
            build_address = args.address
            retail_size = args.size
            build_size = args.size
            if args.symbol:
                symbol = build.symbol(args.symbol)
                if symbol["type"] != 2:
                    raise ValueError(f"{args.symbol!r} is not an ELF function symbol")
                build_address = symbol["value"]
                build_size = symbol["size"]
                retail_address = args.retail_address
                if retail_address is None:
                    match = re.fullmatch(r"FUN_([0-9A-Fa-f]{8})", args.symbol)
                    if match:
                        retail_address = int(match.group(1), 16)
            if (retail_address is None or build_address is None or retail_size is None
                    or build_size is None or retail_size <= 0 or build_size <= 0):
                parser.error(
                    "function requires --address and --size, or --symbol with "
                    "--size (retail size), a nonzero linked size, and an "
                    "inferable/explicit retail address"
                )
            matched = compare_function(
                retail,
                build,
                retail_address,
                build_address,
                retail_size,
                build_size,
                full=args.full,
            )
    except (OSError, ValueError, struct.error) as error:
        parser.exit(2, f"error: {error}\n")
    return 0 if matched else 1


if __name__ == "__main__":
    sys.exit(main())
