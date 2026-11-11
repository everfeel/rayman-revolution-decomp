#!/usr/bin/env python3
"""Write automatically derived reimplementation status reports for the main ELF."""

import argparse
import csv
import hashlib
import html
import json
import re
import struct
import sys
from datetime import datetime
from pathlib import Path

from compare import Elf32LE, RETAIL, SHT_SYMTAB


ROOT = Path(__file__).resolve().parents[1]
BUILD_ELF = ROOT / "build" / "SLES_500.44.elf"
ASM_ROOT = ROOT / "asm" / "nonmatchings"
SOURCE_ROOT = ROOT / "src"
REPORT_DIR = ROOT / "progress"
HTML_REPORT = REPORT_DIR / "main_status.html"
CSV_REPORT = REPORT_DIR / "functions.csv"
SVG_REPORT = REPORT_DIR / "progress.svg"
SOURCE_BASELINE = REPORT_DIR / ".function_source_baseline.json"
README = ROOT / "README.md"
README_PROGRESS_START = "<!-- BEGIN GENERATED PROGRESS -->"
README_PROGRESS_END = "<!-- END GENERATED PROGRESS -->"
FUNCTION_HEADER = re.compile(
    r"(?m)^\s*nonmatching\s+(func_[0-9A-Fa-f]{8})\s*,\s*(0x[0-9A-Fa-f]+)\s*$"
)
DATA_LABEL = re.compile(r"(?m)^\s*dlabel\s+(func_[0-9A-Fa-f]{8})\s*$")
FUNCTION_NAME = re.compile(r"^func_[0-9A-Fa-f]{8}$")
NON_FUNCTION_SYMBOLS = {"func_00100000"}
INCLUDE_ASM = re.compile(
    r"INCLUDE_ASM\s*\([^;]*?\b(func_[0-9A-Fa-f]{8})\s*\)", re.DOTALL
)
FUNCTION_DEFINITION = re.compile(
    r"\b(func_[0-9A-Fa-f]{8})\s*\([^;{}]*\)"
    r"(?:\s|/\*[\s\S]*?\*/|//[^\r\n]*)*\{"
)
INLINE_ASM_FUNCTION = re.compile(
    r'"glabel\s+(func_[0-9A-Fa-f]{8})\\n"'
)
ASSEMBLY_DEFINITION = re.compile(
    r"(?m)^\s*glabel\s+(func_[0-9A-Fa-f]{8})\s*$"
)

CSV_FIELDS = (
    "function",
    "address",
    "status",
    "original_size_bytes",
    "matching_bytes",
    "first_difference",
)

STATUS_CLASS = {
    "NOT STARTED": "not-started",
    "NOT MATCHING": "not-matching",
    "MATCHING": "matching",
}
STATUS_LABEL = {
    "NOT STARTED": "Not implemented",
    "NOT MATCHING": "Implemented",
    "MATCHING": "Byte-match",
}
STATUS_COLOR = {
    "NOT STARTED": ("#555767", 0.4),
    "NOT MATCHING": ("#8286d1", 0.72),
    "MATCHING": ("#55c5df", 0.95),
}
STATUS_GRADIENT = {
    "NOT STARTED": "linear-gradient(90deg, #3e4050, #555767)",
    "NOT MATCHING": "linear-gradient(90deg, #585caa, #8286d1)",
    "MATCHING": "linear-gradient(90deg, #348da5, #55c5df)",
}


def fail(message):
    print(f"status: {message}", file=sys.stderr)
    return 1


def function_inventory():
    if not ASM_ROOT.is_dir():
        raise FileNotFoundError(f"Missing generated assembly directory: {ASM_ROOT}")

    inventory = {}
    for path in sorted(ASM_ROOT.rglob("func_*.s")):
        source = path.read_text(encoding="utf-8")
        matches = list(FUNCTION_HEADER.finditer(source))
        if not matches and DATA_LABEL.search(source):
            continue
        if len(matches) != 1:
            raise ValueError(
                f"{path}: expected one function size header, found {len(matches)}"
            )
        match = matches[0]
        name = match.group(1)
        if name in NON_FUNCTION_SYMBOLS:
            continue
        if name in inventory:
            raise ValueError(f"Duplicate assembly definition for {name}: {path}")
        inventory[name] = {
            "name": name,
            "address": int(name[5:], 16),
            "retail_size": int(match.group(2), 16),
            "assembly_file": path.relative_to(ROOT).as_posix(),
        }
    if not inventory:
        raise ValueError(f"No function size headers found under {ASM_ROOT}")
    return inventory


def project_implementations(inventory):
    asm_references = set()
    c_definitions = set()
    source_files = [
        *SOURCE_ROOT.rglob("*.c"),
        *SOURCE_ROOT.rglob("*.inc"),
        *SOURCE_ROOT.rglob("*.s"),
    ]
    for path in sorted(source_files):
        source = path.read_text(encoding="utf-8")
        asm_references.update(INCLUDE_ASM.findall(source))
        c_definitions.update(FUNCTION_DEFINITION.findall(source))
        c_definitions.update(INLINE_ASM_FUNCTION.findall(source))
        c_definitions.update(ASSEMBLY_DEFINITION.findall(source))

    baseline = {}
    if SOURCE_BASELINE.is_file():
        baseline = json.loads(SOURCE_BASELINE.read_text(encoding="utf-8"))
        if not isinstance(baseline, dict) or any(
            not isinstance(name, str) or not isinstance(digest, str)
            for name, digest in baseline.items()
        ):
            raise ValueError(f"{SOURCE_BASELINE}: malformed source baseline")
    current_baseline = {
        name: digest for name, digest in baseline.items() if name in inventory
    }

    implementations = c_definitions - asm_references
    updated = len(current_baseline) != len(baseline)
    baseline = current_baseline
    for name, function in inventory.items():
        path = ROOT / Path(function["assembly_file"].replace("/", "\\"))
        source = path.read_bytes()
        digest = hashlib.sha256(source).hexdigest()
        previous = baseline.get(name)
        if previous is None:
            baseline[name] = digest
            updated = True
        elif previous != digest:
            implementations.add(name)

    if updated or not SOURCE_BASELINE.exists():
        REPORT_DIR.mkdir(parents=True, exist_ok=True)
        SOURCE_BASELINE.write_text(
            json.dumps(baseline, indent=2, sort_keys=True) + "\n",
            encoding="utf-8",
            newline="\n",
        )
    return implementations


def function_symbols(elf):
    symbols = {}
    for section in elf.sections:
        if section["type"] != SHT_SYMTAB:
            continue
        if section["link"] >= len(elf.sections):
            raise ValueError(f"{elf.path}: symbol table has an invalid string table link")
        strings = elf._string_table(elf.sections[section["link"]])
        entry_size = section["entsize"] or 16
        if entry_size < 16 or section["size"] % entry_size:
            raise ValueError(f"{elf.path}: malformed symbol table")
        for index in range(section["size"] // entry_size):
            offset = section["offset"] + index * entry_size
            name_offset, value, size, info, _, _ = struct.unpack_from(
                "<IIIBBH", elf.data, offset
            )
            name = elf._string_at(strings, name_offset)
            if FUNCTION_NAME.fullmatch(name) and info & 0xF == 2:
                symbols.setdefault(name, {"value": value, "size": size})
    return symbols


def compare_functions(retail, build, inventory, implementations):
    symbols = function_symbols(build)
    rows = []
    for name in sorted(inventory, key=lambda item: int(item[5:], 16)):
        function = inventory[name]
        row = dict(function)
        row["status"] = "NOT STARTED"
        row["matching_bytes"] = ""
        row["first_difference"] = ""

        if name not in implementations:
            rows.append(row)
            continue

        symbol = symbols.get(name)
        if symbol is None:
            row["status"] = "NOT MATCHING"
            row["matching_bytes"] = 0
            row["first_difference"] = "Missing build symbol"
            rows.append(row)
            continue

        if symbol["size"] != function["retail_size"]:
            row["status"] = "NOT MATCHING"
            row["matching_bytes"] = 0
            row["first_difference"] = (
                f"Build size {symbol['size']} bytes differs from retail "
                f"size {function['retail_size']} bytes"
            )
            rows.append(row)
            continue

        row["matching_bytes"] = 0
        try:
            expected = retail.bytes_at_vaddr(
                function["address"], function["retail_size"]
            )
            actual = build.bytes_at_vaddr(
                symbol["value"], function["retail_size"]
            )
        except ValueError as error:
            row["status"] = "NOT MATCHING"
            row["first_difference"] = str(error)
            rows.append(row)
            continue

        common_size = min(len(expected), len(actual))
        differing = [
            index
            for index in range(common_size)
            if expected[index] != actual[index]
        ]
        if symbol["value"] != function["address"]:
            row["first_difference"] = (
                f"Build address 0x{symbol['value']:08X} differs from retail"
            )
            row["status"] = "NOT MATCHING"
            rows.append(row)
            continue
        row["matching_bytes"] = common_size - len(differing)
        if differing:
            row["first_difference"] = f"+0x{differing[0]:X}"
        if len(expected) == len(actual) and not differing:
            row["status"] = "MATCHING"
        else:
            row["status"] = "NOT MATCHING"
        rows.append(row)
    return rows


def write_csv(rows):
    with CSV_REPORT.open("w", encoding="utf-8-sig", newline="") as output:
        writer = csv.DictWriter(output, fieldnames=CSV_FIELDS)
        writer.writeheader()
        for row in rows:
            writer.writerow(
                {
                    "function": row["name"],
                    "address": f"0x{row['address']:08X}",
                    "status": row["status"],
                    "original_size_bytes": row["retail_size"],
                    "matching_bytes": row["matching_bytes"],
                    "first_difference": row["first_difference"],
                }
            )


def update_readme_progress(counts):
    readme = README.read_text(encoding="utf-8")
    start = readme.find(README_PROGRESS_START)
    end = readme.find(README_PROGRESS_END)
    if start == -1 or end == -1 or end < start:
        raise ValueError(f"{README}: generated progress markers are missing or out of order")

    summary = (
        f"![Progress: {counts['MATCHING']:,} byte-matches, "
        f"{counts['NOT MATCHING']:,} implemented, "
        f"{counts['NOT STARTED']:,} not implemented](progress/progress.svg)"
        "\n\n"
        f"**Byte-match:** {counts['MATCHING']:,} · "
        f"**Implemented:** {counts['NOT MATCHING']:,} · "
        f"**Not implemented:** {counts['NOT STARTED']:,}."
    )
    content_start = start + len(README_PROGRESS_START)
    updated = (
        readme[:content_start]
        + "\n"
        + summary
        + "\n"
        + readme[end:]
    )
    if updated != readme:
        README.write_text(updated, encoding="utf-8", newline="\n")


def format_percentage(count, total):
    return f"{count / total * 100:.1f}%" if total else "0.0%"


def write_svg(counts):
    width = 672
    total = counts["total"]
    segments = []
    offset = 24
    legend = []
    legend_items = (
        ("MATCHING", "Byte-match", counts["MATCHING"], 24),
        ("NOT MATCHING", "Implemented", counts["NOT MATCHING"], 250),
        ("NOT STARTED", "Not implemented", counts["NOT STARTED"], 490),
    )
    for status in ("MATCHING", "NOT MATCHING", "NOT STARTED"):
        count = counts[status]
        color, opacity = STATUS_COLOR[status]
        segment_width = width * count / total if total else 0
        if segment_width:
            segments.append(
                f'<rect x="{offset:.2f}" y="66" width="{segment_width:.2f}" '
                f'height="22" fill="url(#{STATUS_CLASS[status]}-gradient)" '
                f'fill-opacity="{opacity}">'
                f'<title>{STATUS_LABEL[status]}: {count:,} '
                f'({format_percentage(count, total)})</title></rect>'
            )
        offset += segment_width
    for status, label, count, x in legend_items:
        _, opacity = STATUS_COLOR[status]
        legend.append(
            f'<circle cx="{x}" cy="126" r="6" '
            f'fill="url(#{STATUS_CLASS[status]}-gradient)" '
            f'fill-opacity="{opacity}"/>'
            f'<text x="{x + 13}" y="131" fill="#c0c1d0" font-size="13">'
            f'{label}: {count:,} ({format_percentage(count, total)})</text>'
        )
    document = f"""<svg xmlns="http://www.w3.org/2000/svg" width="720" height="155" viewBox="0 0 720 155" role="img" aria-labelledby="title description">
<title id="title">Function progress</title>
<desc id="description">Byte-match: {counts["MATCHING"]:,} ({format_percentage(counts["MATCHING"], total)}). Implemented without a byte match: {counts["NOT MATCHING"]:,} ({format_percentage(counts["NOT MATCHING"], total)}). Not implemented: {counts["NOT STARTED"]:,} ({format_percentage(counts["NOT STARTED"], total)}).</desc>
<defs>
<linearGradient id="matching-gradient" x1="0" x2="1"><stop offset="0" stop-color="#348da5"/><stop offset="1" stop-color="#55c5df"/></linearGradient>
<linearGradient id="not-matching-gradient" x1="0" x2="1"><stop offset="0" stop-color="#585caa"/><stop offset="1" stop-color="#8286d1"/></linearGradient>
<linearGradient id="not-started-gradient" x1="0" x2="1"><stop offset="0" stop-color="#3e4050"/><stop offset="1" stop-color="#555767"/></linearGradient>
</defs>
<rect width="720" height="155" rx="16" fill="#10111a"/>
<text x="24" y="38" fill="#f1f1f7" font-family="Segoe UI, Arial, sans-serif" font-size="18" font-weight="600">Function progress</text>
<rect x="24" y="66" width="{width}" height="22" rx="11" fill="{STATUS_COLOR["NOT STARTED"][0]}" fill-opacity="{STATUS_COLOR["NOT STARTED"][1]}"/>
<clipPath id="progress-clip"><rect x="24" y="66" width="{width}" height="22" rx="11"/></clipPath>
<g clip-path="url(#progress-clip)">
{''.join(segments)}
</g>
{"".join(legend)}
</svg>
"""
    SVG_REPORT.write_text(document, encoding="utf-8", newline="\n")


def summary(rows):
    counts = {
        status: sum(row["status"] == status for row in rows)
        for status in STATUS_CLASS
    }
    counts["total"] = len(rows)
    counts["implemented"] = counts["MATCHING"] + counts["NOT MATCHING"]
    return counts


def write_html(rows, counts, generated_at):
    table_rows = []
    for row in rows:
        table_rows.append(
            f'<tr data-status="{html.escape(row["status"])}">'
            f"<td><code>{html.escape(row['name'])}</code></td>"
            f"<td>0x{row['address']:08X}</td>"
            f'<td><span class="badge {STATUS_CLASS[row["status"]]}">'
            f'{STATUS_LABEL[row["status"]]}</span></td>'
            f"<td>{row['retail_size']}</td>"
            f"<td>{(str(row['matching_bytes']) + ' / ' + str(row['retail_size'])) if row['matching_bytes'] != '' else '-'}</td>"
            f"<td>{html.escape(str(row['first_difference']) or '-')}</td>"
            "</tr>"
        )

    status_filters_html = "\n".join(
        f'<button class="status-filter {css_class}" type="button" '
        f'data-status-filter="{html.escape(status)}" aria-pressed="false">'
        f'<span class="filter-dot" aria-hidden="true"></span>'
        f'{html.escape(title)} <span class="filter-count">{counts[status]:,}</span>'
        "</button>"
        for status, title, css_class in (
            ("MATCHING", STATUS_LABEL["MATCHING"], "matching"),
            ("NOT MATCHING", STATUS_LABEL["NOT MATCHING"], "not-matching"),
            ("NOT STARTED", STATUS_LABEL["NOT STARTED"], "not-started"),
        )
    )
    matching_percent = format_percentage(counts["MATCHING"], counts["total"])
    implemented_percent = format_percentage(counts["NOT MATCHING"], counts["total"])
    not_implemented_percent = format_percentage(
        counts["NOT STARTED"], counts["total"]
    )
    progress_segments = []
    progress_legend = []
    for status, label, percentage in (
        ("MATCHING", "Byte-match", matching_percent),
        ("NOT MATCHING", "Implemented", implemented_percent),
        ("NOT STARTED", "Not implemented", not_implemented_percent),
    ):
        count = counts[status]
        color, opacity = STATUS_COLOR[status]
        segment_width = count / counts["total"] * 100 if counts["total"] else 0
        if count:
            progress_segments.append(
                f'<span class="progress-segment {STATUS_CLASS[status]}" '
                f'style="width:{segment_width:.5f}%;opacity:{opacity};'
                f'background:{STATUS_GRADIENT[status]}" '
                f'title="{label}: {count:,} ({percentage})"></span>'
            )
        progress_legend.append(
            f'<span class="progress-legend-item {STATUS_CLASS[status]}" '
            f'style="--status-gradient:{STATUS_GRADIENT[status]};'
            f'--status-opacity:{opacity}">'
            f'<i></i>{label} <strong>{count:,} ({percentage})</strong></span>'
        )
    progress_bar = (
        f'<div class="stacked-bar" role="img" aria-label="Function status: '
        f'byte match {counts["MATCHING"]:,} ({matching_percent}); implemented '
        f'{counts["NOT MATCHING"]:,} ({implemented_percent}); not implemented '
        f'{counts["NOT STARTED"]:,} ({not_implemented_percent})">'
        f'{"".join(progress_segments)}</div>'
        f'<div class="progress-legend">{"".join(progress_legend)}</div>'
    )
    document = f"""<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#08090f">
<title>Rayman Revolution decompilation progress</title>
<style>
:root {{ color-scheme: dark; font-family: "Segoe UI", Arial, sans-serif; color: #f1f1f7; background: #08090f; }}
* {{ box-sizing: border-box; }}
body {{ margin: 0; background: radial-gradient(ellipse at 15% 0%, #171a31 0, #0d0e16 36%, #08090f 78%); min-height: 100vh; }}
main {{ max-width: 1540px; margin: 0 auto; padding: 38px 34px 60px; }}
h1 {{ margin: 0 0 9px; font-size: clamp(2rem, 4vw, 3.25rem); letter-spacing: -.045em; line-height: 1.05; }}
.subtitle {{ color: #a7a9c1; }}
.hero {{ position: relative; overflow: hidden; padding: clamp(24px, 4vw, 42px); border: 1px solid #3a3d71; border-radius: 22px; background: linear-gradient(118deg, #141627 0%, #0d0e16 72%); box-shadow: 0 22px 65px #00000099; }}
.hero-copy {{ max-width: 100%; }}
.eyebrow {{ display: inline-flex; align-items: center; gap: 8px; margin-bottom: 18px; color: #c6c8ff; font-size: .75rem; font-weight: 750; letter-spacing: .16em; text-transform: uppercase; }}
.eyebrow::before {{ content: ""; width: 7px; height: 7px; border-radius: 50%; background: #8c90df; box-shadow: 0 0 12px #8c90df; }}
.updated {{ margin-top: 13px; font-size: .88rem; }}
.progress-track {{ margin: 28px 0 12px; }}
.progress-track-heading {{ display: flex; justify-content: space-between; gap: 12px; margin-bottom: 8px; }}
.progress-track-heading span {{ color: #a7a9c1; font-size: .82rem; }}
.stacked-bar {{ display: flex; height: 14px; overflow: hidden; background: #141622; border: 1px solid #3a3d71; border-radius: 99px; }}
.progress-segment {{ display: block; height: 100%; min-width: 1px; }}
.progress-segment.matching {{ box-shadow: 0 0 12px #55c5df88; }}
.progress-segment.not-matching {{ box-shadow: 0 0 12px #8286d188; }}
.progress-segment.not-started {{ box-shadow: 0 0 12px #55576788; }}
.progress-segment:first-child {{ border-radius: 99px 0 0 99px; }}
.progress-segment:last-child {{ border-radius: 0 99px 99px 0; }}
.progress-legend {{ display: flex; flex-wrap: wrap; gap: 6px 16px; margin-top: 8px; }}
.progress-legend-item {{ display: inline-flex; align-items: center; gap: 5px; color: #a7a9c1; font-size: .75rem; }}
.progress-legend-item i {{ width: 8px; height: 8px; border-radius: 50%; background: var(--status-gradient); opacity: var(--status-opacity); }}
.progress-legend-item strong {{ color: #e0e1f4; font-weight: 650; }}
.badge {{ display: inline-block; border: 1px solid transparent; border-radius: 999px; padding: 4px 10px; font-weight: 700; font-size: .75rem; letter-spacing: .025em; }}
.badge.matching {{ color: #bceffc; background: #12323a; border-color: #32798a; }}
.badge.not-matching {{ color: #d7d8ff; background: #202238; border-color: #55598e; }}
.badge.not-started {{ color: #c5c6d1; background: #20212b; border-color: #444654; }}
.note {{ margin: 20px 0 24px; padding: 15px 18px; background: #10111a; border: 1px solid #252738; border-left: 3px solid #3a3d71; border-radius: 10px; color: #c0c1d0; line-height: 1.6; }}
.section-head {{ display: flex; justify-content: space-between; align-items: end; gap: 16px; margin: 34px 0 14px; }}
.section-head h2 {{ margin: 0; font-size: 1.25rem; letter-spacing: -.02em; }}
.section-head p {{ margin: 0; color: #85879b; font-size: .85rem; }}
.toolbar {{ display: flex; justify-content: space-between; align-items: center; gap: 14px; padding: 14px; background: #0e0f17; border: 1px solid #252738; border-radius: 14px; }}
.controls {{ display: flex; gap: 8px; align-items: center; flex-wrap: wrap; color: #b6b7c8; }}
.status-filter {{ display: inline-flex; gap: 8px; align-items: center; padding: 9px 12px; color: #b9bac8; background: #11121b; border: 1px solid #282a39; border-radius: 9px; font: inherit; font-size: .82rem; cursor: pointer; transition: background .16s ease, border-color .16s ease, color .16s ease, transform .16s ease; }}
.status-filter:hover {{ transform: translateY(-1px); border-color: #3a3d71; color: #f1f1f7; }}
.status-filter[aria-pressed="true"] {{ color: #f1f1f7; background: #202238; border-color: #55598e; box-shadow: inset 0 0 0 1px #3a3d7144; }}
.filter-dot {{ width: 8px; height: 8px; border-radius: 50%; background: #555767; }}
.status-filter.matching .filter-dot {{ background: #55c5df; box-shadow: 0 0 10px #55c5df77; }}
.status-filter.not-matching .filter-dot {{ background: #8286d1; }}
.status-filter.not-started .filter-dot {{ background: #555767; }}
.filter-count {{ min-width: 24px; padding: 2px 6px; text-align: center; color: #aaaabd; background: #08090f; border-radius: 99px; font-size: .72rem; }}
.status-filter[aria-pressed="true"] .filter-count {{ color: #e3e3ff; background: #3a3d7144; }}
.search-wrap {{ display: flex; align-items: center; gap: 9px; min-width: min(320px, 100%); padding: 0 11px; background: #08090f; border: 1px solid #292b3d; border-radius: 9px; }}
.search-icon {{ color: #777a95; font-size: 1.1rem; }}
input {{ width: 100%; min-width: 0; padding: 10px 0; color: #f1f1f7; background: transparent; border: 0; outline: 0; font: inherit; font-size: .88rem; }}
.search-wrap:focus-within {{ border-color: #777bb8; box-shadow: 0 0 0 2px #3a3d7133; }}
a {{ color: #bfc1ff; }}
.edit-link {{ display: inline-block; padding: 10px 14px; border: 1px solid #3a3d71; border-radius: 8px; text-decoration: none; background: #17192a; white-space: nowrap; font-size: .85rem; }}
.table-wrap {{ overflow: auto; max-height: 68vh; background: #0c0d13; border: 1px solid #252738; border-radius: 12px; box-shadow: 0 12px 35px #00000088; }}
table {{ border-collapse: collapse; width: 100%; font-size: .88rem; }}
th, td {{ text-align: left; padding: 10px 12px; border-bottom: 1px solid #20212b; white-space: nowrap; }}
th {{ position: sticky; top: 0; color: #e0e1f4; background: #191b2d; z-index: 1; }}
tr:hover td {{ background: #141622; }}
code {{ color: #c6c8ff; font-family: Consolas, monospace; }}
.results {{ display: flex; justify-content: space-between; gap: 12px; padding: 11px 14px; color: #85879b; font-size: .82rem; }}
.results strong {{ color: #d0d1e2; font-weight: 650; }}
.empty {{ display: none; padding: 36px 16px; color: #9294a8; text-align: center; }}
.tabs {{ display: flex; gap: 8px; margin: 24px 0 0; border-bottom: 1px solid #252738; }}
.tab {{ padding: 11px 15px; color: #a7a9c1; background: transparent; border: 0; border-bottom: 2px solid transparent; font: inherit; cursor: pointer; }}
.tab[aria-selected="true"] {{ color: #e0e1f4; border-bottom-color: #8286d1; }}
.view-panel {{ display: none; }}
.view-panel.active {{ display: block; }}
.workspace {{ margin-top: 24px; padding: 20px; background: #0c0d13; border: 1px solid #252738; border-radius: 14px; }}
.workspace-bar {{ display: flex; gap: 10px; align-items: center; flex-wrap: wrap; margin-bottom: 14px; }}
.workspace select, .workspace button, .workspace input {{ padding: 9px 12px; color: #e0e1f4; background: #17192a; border: 1px solid #3a3d71; border-radius: 8px; font: inherit; }}
.workspace button {{ cursor: pointer; }}
.workspace select, .workspace input {{ min-width: min(320px, 100%); }}
.function-search {{ flex: 1 1 280px; }}
.compiler-flags-row {{ display: flex; flex: 1 0 100%; align-items: center; gap: 10px; }}
.compiler-flags-control {{ flex: 1 1 360px; }}
.checkbox-control {{ display: inline-flex; align-items: center; gap: 7px; color: #c5c6db; white-space: nowrap; }}
.workspace .checkbox-control input {{ width: auto; min-width: 0; margin: 0; padding: 0; accent-color: #999cff; }}
.workspace textarea, .workspace pre {{ width: 100%; min-width: 0; height: min(65vh, 680px); margin: 0; padding: 14px; overflow: auto; color: #d1d2e0; background: #08090f; border: 1px solid #252738; border-radius: 8px; font: 13px/1.55 Consolas, monospace; tab-size: 4; white-space: pre; }}
.workspace textarea {{ resize: vertical; outline-color: #777bb8; }}
.workspace-panes {{ display: grid; grid-template-columns: repeat(2, minmax(0, 1fr)); gap: 14px; }}
.workspace-panes h3 {{ color: #b6b7c8; font-size: .9rem; }}
.workspace-status {{ min-height: 1.5em; margin: 0 0 12px; color: #a7a9c1; }}
.playground-controls {{ justify-content: flex-start; padding: 14px; margin: 18px 0 14px; background: #10111a; border: 1px solid #252738; border-radius: 10px; }}
.workspace-bar label {{ color: #aeb0c4; font-size: .82rem; font-weight: 650; }}
.workspace select:focus, .workspace input:focus, .workspace button:focus-visible, .tab:focus-visible, .status-filter:focus-visible {{ outline: 2px solid #999cff; outline-offset: 2px; }}
.workspace button {{ background: #282b4b; border-color: #494d7e; transition: background .16s ease, transform .16s ease; }}
.workspace button:hover {{ background: #35395f; transform: translateY(-1px); }}
.playground-match-notice {{ display: flex; align-items: flex-start; gap: 10px; max-width: none; margin: 0 0 12px; padding: 13px 16px; color: #bceffc; background: #12323a; border: 1px solid #32798a; border-radius: 10px; line-height: 1.5; }}
.playground-match-notice::before {{ content: "LATEST BUILD"; flex: 0 0 auto; margin-top: 2px; color: #55c5df; font-size: .67rem; font-weight: 750; letter-spacing: .08em; }}
.playground-match-notice[hidden] {{ display: none; }}
.workspace-status {{ padding: 11px 14px; background: #10111a; border: 1px solid #252738; border-radius: 9px; line-height: 1.5; }}
.playground-result-status {{ margin: 12px 0 0; }}
.result-status {{ color: #d5d6e5; }}
.result-match {{ color: #bceffc; border-color: #32798a; background: #12323a; }}
.result-diff {{ color: #ffd2c5; border-color: #784239; background: #30191a; }}
.result-pending {{ color: #d7ddff; border-color: #3a4164; background: #171a2c; }}
.result-error {{ color: #ffd2c5; border-color: #784239; background: #30191a; }}
.playground-editor {{ margin: 0 0 20px; }}
.playground-editor h3 {{ margin: 18px 0 8px; color: #e0e1f4; font-size: 1rem; }}
.playground-editor textarea {{ display: block; height: min(42vh, 420px); min-height: 240px; border-color: #32354a; line-height: 1.6; }}
.playground-editor textarea:focus {{ border-color: #777bb8; box-shadow: 0 0 0 2px #3a3d7133; }}
.draft-status {{ min-height: 1.4em; margin: 7px 2px 0; color: #9294a8; font-size: .78rem; }}
.draft-status.error {{ color: #ffd2c5; }}
.playground-diff {{ max-height: 58vh; overflow: auto; border: 1px solid #252738; border-radius: 10px; }}
.playground-diff-row {{ display: grid; grid-template-columns: 76px minmax(0, 1fr) minmax(0, 1fr); gap: 12px; padding: 9px 12px; border-bottom: 1px solid #20212b; font: 12px/1.55 Consolas, monospace; }}
.playground-diff-row span {{ min-width: 0; white-space: pre-wrap; overflow-wrap: anywhere; }}
.playground-diff-row.header {{ position: sticky; top: 0; color: #dfe0f2; background: #191b2d; z-index: 1; font: 650 .76rem/1.4 "Segoe UI", Arial, sans-serif; letter-spacing: .02em; }}
.playground-diff-row.match {{ background: #12323a; }}
.playground-diff-row.diff {{ background: #30191a; }}
.assembly-row.unverified {{ background: #171821; color: #a7a9c1; }}
.playground-diff-row .offset {{ color: #9294a8; }}
.playground-diff-empty {{ padding: 18px; color: #9294a8; text-align: center; }}
.assembly-row {{ display: grid; grid-template-columns: 76px minmax(0, 1fr) minmax(0, 1fr); gap: 10px; padding: 9px 10px; border-bottom: 1px solid #20212b; font: 12px/1.55 Consolas, monospace; }}
.assembly-row.match {{ background: #12323a; }} .assembly-row.diff {{ background: #30191a; }}
.results {{ align-items: center; }}
.table-wrap {{ box-shadow: 0 10px 28px #00000055; }}
th {{ font-size: .78rem; letter-spacing: .025em; text-transform: uppercase; }}
tbody tr:nth-child(even) td {{ background: #0e0f16; }}
tbody tr:hover td {{ background: #17192a; }}
.tabs {{ gap: 4px; margin-top: 28px; }}
.tab {{ padding: 12px 16px; border-radius: 8px 8px 0 0; transition: color .16s ease, background .16s ease; }}
.tab:hover {{ color: #e0e1f4; background: #11121b; }}
.tab[aria-selected="true"] {{ color: #f1f1f7; background: #131522; }}
.view-panel.active {{ animation: panel-in .18s ease-out; }}
@keyframes panel-in {{ from {{ opacity: .7; transform: translateY(3px); }} to {{ opacity: 1; transform: translateY(0); }} }}
.function-action {{ color: #bfc1ff; background: transparent; border: 0; font: inherit; cursor: pointer; }}
.function-action:hover {{ text-decoration: underline; }}
@media (max-width: 900px) {{ .toolbar {{ align-items: stretch; flex-direction: column; }} .search-wrap {{ min-width: 0; }} }}
@media (max-width: 700px) {{ main {{ padding: 18px 14px 36px; }} .hero {{ padding: 24px 20px; }} .section-head {{ align-items: start; flex-direction: column; }} .controls {{ display: grid; grid-template-columns: 1fr; }} .status-filter {{ justify-content: space-between; }} .progress-track-heading {{ align-items: start; flex-direction: column; gap: 3px; }} }}
@media (max-width: 800px) {{ .workspace-panes {{ grid-template-columns: 1fr; }} .workspace textarea, .workspace pre {{ height: 45vh; }} .assembly-row {{ grid-template-columns: 52px minmax(0, 1fr) minmax(0, 1fr); gap: 5px; }} .playground-diff-row {{ grid-template-columns: 52px minmax(0, 1fr) minmax(0, 1fr); gap: 6px; padding: 7px 6px; font-size: 11px; }} }}
</style>
</head>
<body>
<main>
<section class="hero">
<div class="hero-copy">
<div class="eyebrow">Decompilation progress</div>
<h1>Rayman Revolution</h1>
<div class="subtitle">PAL PlayStation 2 main executable · SLES_500.44</div>
<div class="subtitle updated">Progress report · {html.escape(generated_at)} · {counts["total"]:,} functions</div>
<div class="subtitle">Automatically refreshed after successful builds; this page refreshes every 30 seconds.</div>
</div>
<section class="progress-track" aria-label="Function progress">
<div class="progress-track-heading"><strong>{counts["total"]:,} functions</strong><span>Progress by function status</span></div>
{progress_bar}
</section>
<p class="note"><strong>Function status:</strong> <b>Byte-match</b> means the compiled function matches the original bytes. <b>Implemented</b> means a C or hand-written assembly implementation replaces the retail fallback, but its compiled bytes differ. <b>Not implemented</b> means the function still uses the retail assembly fallback.</p>
</section>
<nav class="tabs" role="tablist" aria-label="Dashboard views">
  <button class="tab" id="progress-tab" role="tab" aria-selected="true" aria-controls="progress-view">Progress</button>
  <button class="tab" id="assembly-tab" role="tab" aria-selected="false" aria-controls="assembly-view">Assembly comparison</button>
  <button class="tab" id="playground-tab" role="tab" aria-selected="false" aria-controls="playground-view">C playground</button>
</nav>
<section class="view-panel active" id="progress-view" role="tabpanel" aria-labelledby="progress-tab">
<section aria-labelledby="functions-heading">
<div class="section-head">
  <h2 id="functions-heading">Function inventory</h2><p>Filter by progress status or search by function name or address.</p>
</div>
<div class="toolbar">
<div class="controls" aria-label="Filter by function status">
  <button class="status-filter" type="button" data-status-filter="ALL" aria-pressed="true">All <span class="filter-count">{counts["total"]:,}</span></button>
  {status_filters_html}
</div>
<label class="search-wrap" for="filter">
  <span class="search-icon" aria-hidden="true">⌕</span>
  <input id="filter" type="search" placeholder="Search functions…" autocomplete="off">
</label>
</div>
<div class="results" aria-live="polite"><span id="result-count"></span><span>Status updates after each successful build</span></div>
<div class="table-wrap">
<table id="functions">
<thead><tr><th>Function</th><th>Address</th><th>Status</th><th>Retail size</th><th>Matching bytes</th><th>First difference</th></tr></thead>
<tbody>{''.join(table_rows)}</tbody>
</table>
<div class="empty" id="empty-state">No functions match these filters. Try another status or search.</div>
</div>
</section>
</section>
<section class="view-panel" id="assembly-view" role="tabpanel" aria-labelledby="assembly-tab">
  <div class="workspace">
    <div class="workspace-bar">
      <label for="assembly-function">Selected function</label>
      <input class="function-search" id="assembly-function" type="search" list="function-options" placeholder="Search function name or address…" autocomplete="off" aria-label="Search assembly function">
      <button type="button" id="open-playground">Edit in playground</button>
    </div>
    <p class="workspace-status" id="assembly-status" role="status">Start the dashboard server to compare retail and build output.</p>
    <div class="assembly-row"><strong>Offset</strong><strong>Retail · bytes</strong><strong>Build · bytes</strong></div>
    <div id="assembly-output"></div>
  </div>
</section>
<section class="view-panel" id="playground-view" role="tabpanel" aria-labelledby="playground-tab">
  <div class="workspace">
    <p class="note">Edit a function and compare its linked bytes with retail. The playground links the selected function at its project ELF address and resolves project calls and globals against their built addresses; stale build inputs and linker errors are reported instead of an approximate result.</p>
    <div class="workspace-bar playground-controls">
      <label for="playground-function">Selected function</label>
      <input class="function-search" id="playground-function" type="search" list="function-options" placeholder="Search function name or address…" autocomplete="off" aria-label="Search playground function">
      <label for="optimization">Optimization</label>
      <select id="optimization" disabled><option>-O0</option><option>-O1</option><option selected>-O2</option><option>-O3</option></select>
      <label class="checkbox-control" for="preserve-frame-pointer" title="Pass -fno-omit-frame-pointer to the playground compiler.">
        <input id="preserve-frame-pointer" type="checkbox" disabled>
        Preserve frame pointer
      </label>
      <div class="compiler-flags-row">
        <label for="other-flags">Compiler arguments</label>
        <input class="compiler-flags-control" id="other-flags" type="text" placeholder="-fno-inline -DNAME &quot;value with spaces&quot;" title="Shell-style compiler arguments; quote values containing spaces." autocomplete="off" disabled>
      </div>
    </div>
    <p class="playground-match-notice" id="playground-match-notice" role="status" hidden></p>
    <p class="draft-status" id="compiler-profile-status" role="status">Compiler settings are saved per function.</p>
    <section class="playground-editor">
      <h3>Implementation</h3>
      <textarea id="playground-source" spellcheck="false"></textarea>
      <p class="draft-status" id="playground-draft-status" role="status">Drafts are saved locally for each function.</p>
    </section>
    <div class="playground-diff-row header"><strong>Offset</strong><strong>Retail instructions · bytes</strong><strong>Linked instructions · bytes</strong></div>
    <div class="playground-diff" id="playground-diff" aria-live="polite">
      <div class="playground-diff-empty">Edit the C source to see instruction differences.</div>
    </div>
    <p class="workspace-status result-status result-pending playground-result-status" id="playground-status" role="status">Start the dashboard server to compile and link a function.</p>
  </div>
</section>
</main>
<datalist id="function-options"></datalist>
<script>
const filter = document.getElementById("filter");
const buttons = [...document.querySelectorAll("[data-status-filter]")];
const rows = [...document.querySelectorAll("#functions tbody tr")];
const resultCount = document.getElementById("result-count");
const emptyState = document.getElementById("empty-state");
const refreshStateKey = "progress-dashboard-refresh";
const workspaceTabs = [...document.querySelectorAll(".tab")];
const workspaceViews = [...document.querySelectorAll(".view-panel")];
const assemblyFunction = document.getElementById("assembly-function");
const playgroundFunction = document.getElementById("playground-function");
const functionOptions = document.getElementById("function-options");
const assemblyStatus = document.getElementById("assembly-status");
const assemblyOutput = document.getElementById("assembly-output");
const playgroundStatus = document.getElementById("playground-status");
const playgroundSource = document.getElementById("playground-source");
const playgroundDraftStatus = document.getElementById("playground-draft-status");
const compilerProfileStatus = document.getElementById("compiler-profile-status");
const playgroundDiff = document.getElementById("playground-diff");
const playgroundMatchNotice = document.getElementById("playground-match-notice");
const optimizationControl = document.getElementById("optimization");
const preserveFramePointerControl = document.getElementById("preserve-frame-pointer");
const otherFlagsControl = document.getElementById("other-flags");
let activeStatus = "ALL";
let saveScrollTimer;
let compileTimer;
let draftSaveTimer;
let profileSaveTimer;
let draftSaveQueue = Promise.resolve();
let profileSaveQueue = Promise.resolve();
let compileRequest = 0;
let playgroundLoadRequest = 0;
let assemblyRequest = 0;
let assemblySymbol = "";
let playgroundSymbol = "";
let playgroundDraftDirty = false;
let compilerProfileDirty = false;
let functionStatuses = new Map();
let functionSymbols = new Map();

function applyFilters() {{
  const query = filter.value.trim().toLowerCase();
  let visible = 0;
  for (const row of rows) {{
    const statusMatches = activeStatus === "ALL" || row.dataset.status === activeStatus;
    const queryMatches = !query || row.textContent.toLowerCase().includes(query);
    row.hidden = !(statusMatches && queryMatches);
    if (!row.hidden) visible++;
  }}
  resultCount.innerHTML = `<strong>${{visible.toLocaleString()}}</strong> of ${{rows.length.toLocaleString()}} functions`;
  emptyState.style.display = visible ? "none" : "block";
}}

function saveViewState() {{
  const topRow = rows.find(row => !row.hidden && row.getBoundingClientRect().bottom > 0);
  sessionStorage.setItem(refreshStateKey, JSON.stringify({{
    search: filter.value,
    status: activeStatus,
    scrollY: window.scrollY,
    rowName: topRow ? topRow.cells[0].textContent.trim() : "",
    rowOffset: topRow ? topRow.getBoundingClientRect().top : 0
  }}));
}}

function activateView(name) {{
  for (const tab of workspaceTabs) {{
    const active = tab.id === `${{name}}-tab`;
    tab.setAttribute("aria-selected", String(active));
  }}
  for (const panel of workspaceViews) panel.classList.toggle("active", panel.id === `${{name}}-view`);
}}

async function requestJson(path, options) {{
  const response = await fetch(path, options);
  const result = await response.json();
  if (!response.ok) throw new Error(result.error || `Request failed (${{response.status}}).`);
  return result;
}}

function chooseFunction(symbol) {{
  for (const input of [assemblyFunction, playgroundFunction]) {{
    input.value = symbol;
  }}
}}

function chooseFunctionFromSearch(input, loader) {{
  const symbol = functionSymbols.get(input.value.trim().toLowerCase());
  if (symbol) loader(symbol);
}}

function renderAssembly(result) {{
  assemblyOutput.replaceChildren();
  for (const item of result.instructions) {{
    const row = document.createElement("div");
    row.className = `assembly-row ${{!result.implemented ? "unverified" : item.matches ? "match" : "diff"}}`;
    for (const text of [item.offset, `${{item.retail}}\\n${{item.retail_bytes}}`, `${{item.compiled}}\\n${{item.compiled_bytes}}`]) {{
      const cell = document.createElement("span");
      cell.textContent = text;
      row.append(cell);
    }}
    assemblyOutput.append(row);
  }}
  assemblyStatus.textContent = `${{result.symbol}} · ${{result.status}} · retail ${{result.retail_size}} bytes · build ${{result.compiled_size}} bytes${{result.notice ? ` · ${{result.notice}}` : ""}}`;
}}

async function loadAssembly(symbol) {{
  if (!symbol) return;
  const requestId = ++assemblyRequest;
  assemblySymbol = symbol;
  chooseFunction(symbol);
  activateView("assembly");
  assemblyStatus.textContent = "Loading function assembly…";
  try {{
    const result = await requestJson(`/api/assembly?symbol=${{encodeURIComponent(symbol)}}`);
    if (requestId !== assemblyRequest) return;
    renderAssembly(result);
  }} catch (error) {{
    if (requestId !== assemblyRequest) return;
    assemblyStatus.textContent = `Could not load assembly: ${{error.message}}`;
  }}
}}

async function loadPlaygroundFunction(symbol) {{
  if (!symbol || (symbol === playgroundSymbol && !playgroundSource.disabled)) return;
  const loadRequest = ++playgroundLoadRequest;
  const previousSymbol = playgroundSymbol;
  window.clearTimeout(draftSaveTimer);
  window.clearTimeout(profileSaveTimer);
  if (previousSymbol && playgroundDraftDirty) {{
    try {{
      await savePlaygroundDraft(previousSymbol, playgroundSource.value);
    }} catch (error) {{
      if (loadRequest === playgroundLoadRequest) {{
        playgroundFunction.value = previousSymbol;
        playgroundDraftStatus.textContent = `Could not save draft; kept ${{previousSymbol}} selected: ${{error.message}}`;
        playgroundDraftStatus.classList.add("error");
      }}
      return;
    }}
  }}
  if (previousSymbol && compilerProfileDirty) {{
    try {{
      await saveCompilerProfile(previousSymbol, currentCompilerProfile());
    }} catch (error) {{
      if (loadRequest === playgroundLoadRequest) {{
        playgroundFunction.value = previousSymbol;
        compilerProfileStatus.textContent = `Could not save compiler settings; kept ${{previousSymbol}} selected: ${{error.message}}`;
        compilerProfileStatus.classList.add("error");
      }}
      return;
    }}
  }}
  if (loadRequest !== playgroundLoadRequest) return;
  playgroundSymbol = symbol;
  playgroundDraftDirty = false;
  compilerProfileDirty = false;
  const alreadyMatches = functionStatuses.get(symbol) === "MATCHING";
  playgroundMatchNotice.hidden = !alreadyMatches;
  playgroundMatchNotice.textContent = alreadyMatches
    ? "This function already matches retail in the latest project build. The result below is separate and reflects only your current playground edit."
    : "";
  compileRequest++;
  window.clearTimeout(compileTimer);
  chooseFunction(symbol);
  playgroundSource.disabled = true;
  optimizationControl.disabled = true;
  preserveFramePointerControl.disabled = true;
  otherFlagsControl.disabled = true;
  playgroundSource.value = "";
  playgroundDraftStatus.textContent = "Loading this function's local draft…";
  playgroundDraftStatus.classList.remove("error");
  compilerProfileStatus.textContent = "Loading this function's compiler settings…";
  compilerProfileStatus.classList.remove("error");
  playgroundDiff.replaceChildren();
  playgroundDiff.append(emptyDiff("Edit the C source to see instruction differences."));
  playgroundStatus.className = "workspace-status result-status result-pending playground-result-status";
  playgroundStatus.textContent = "Playground result: loading function…";
  try {{
    const [draft, savedProfile] = await Promise.all([
      requestJson(`/api/draft?symbol=${{encodeURIComponent(symbol)}}`),
      requestJson(`/api/profile?symbol=${{encodeURIComponent(symbol)}}`)
    ]);
    if (loadRequest !== playgroundLoadRequest) return;
    applyCompilerProfile(savedProfile.profile);
    optimizationControl.disabled = false;
    preserveFramePointerControl.disabled = false;
    otherFlagsControl.disabled = false;
    compilerProfileStatus.textContent = savedProfile.saved
      ? "Restored this function's compiler settings; changes are saved locally."
      : "Default compiler settings; changes are saved per function.";
    playgroundSource.value = draft.source;
    playgroundSource.disabled = false;
    playgroundDraftStatus.textContent = draft.stale
      ? "The saved draft did not define this function; loaded a fresh starter. The old draft remains saved until you edit."
      : draft.saved
        ? "Restored this function's saved local draft."
        : draft.signature_inferred
          ? "Starter uses this function's project C signature; edits are saved locally."
          : "Starter signature is unknown for this function; confirm its return type and parameters.";
    if (draft.stale) playgroundDraftStatus.classList.add("error");
    if (draft.saved) {{
      scheduleCompile();
    }} else {{
      setPlaygroundStatus(
        draft.signature_inferred
          ? "Playground result: add an implementation before compiling."
          : "Playground result: confirm this function's signature and add an implementation before compiling.",
        "pending"
      );
    }}
  }} catch (error) {{
    if (loadRequest !== playgroundLoadRequest) return;
    playgroundDraftStatus.textContent = `Could not load this function's draft: ${{error.message}}`;
    playgroundDraftStatus.classList.add("error");
    compilerProfileStatus.textContent = `Could not load compiler settings: ${{error.message}}`;
    compilerProfileStatus.classList.add("error");
    setPlaygroundStatus(`Playground result: could not load function - ${{error.message}}`, "error");
  }}
}}

function currentCompilerProfile() {{
  return {{
    optimization: optimizationControl.value,
    preserve_frame_pointer: preserveFramePointerControl.checked,
    other_flags: otherFlagsControl.value
  }};
}}

function applyCompilerProfile(profile) {{
  optimizationControl.value = profile.optimization;
  preserveFramePointerControl.checked = profile.preserve_frame_pointer;
  otherFlagsControl.value = profile.other_flags;
}}

function saveCompilerProfile(symbol, profile) {{
  const operation = profileSaveQueue.then(async () => {{
    await requestJson("/api/profile", {{
      method: "POST",
      headers: {{ "Content-Type": "application/json" }},
      body: JSON.stringify({{ symbol, profile }})
    }});
    if (playgroundSymbol === symbol && JSON.stringify(currentCompilerProfile()) === JSON.stringify(profile)) {{
      compilerProfileDirty = false;
      compilerProfileStatus.textContent = "Compiler settings saved for this function.";
      compilerProfileStatus.classList.remove("error");
    }}
  }});
  profileSaveQueue = operation.catch(error => {{
    if (playgroundSymbol === symbol) {{
      compilerProfileStatus.textContent = `Could not save compiler settings: ${{error.message}}`;
      compilerProfileStatus.classList.add("error");
    }}
  }});
  return operation;
}}

function scheduleCompilerProfileSave() {{
  compilerProfileDirty = true;
  compilerProfileStatus.textContent = "Saving compiler settings for this function…";
  compilerProfileStatus.classList.remove("error");
  window.clearTimeout(profileSaveTimer);
  if (!playgroundSymbol) return;
  const symbol = playgroundSymbol;
  const profile = currentCompilerProfile();
  profileSaveTimer = window.setTimeout(() => {{
    saveCompilerProfile(symbol, profile).catch(() => {{}});
  }}, 300);
}}

function savePlaygroundDraft(symbol, source) {{
  const operation = draftSaveQueue.then(async () => {{
    await requestJson("/api/draft", {{
      method: "POST",
      headers: {{ "Content-Type": "application/json" }},
      body: JSON.stringify({{ symbol, source }})
    }});
    if (playgroundSymbol === symbol) {{
      if (playgroundSource.value === source) {{
        playgroundDraftDirty = false;
        playgroundDraftStatus.textContent = "Draft saved locally.";
        playgroundDraftStatus.classList.remove("error");
      }}
    }}
  }});
  draftSaveQueue = operation.catch(error => {{
    if (playgroundSymbol === symbol) {{
      playgroundDraftStatus.textContent = `Could not save draft: ${{error.message}}`;
      playgroundDraftStatus.classList.add("error");
    }}
  }});
  return operation;
}}

function scheduleDraftSave() {{
  window.clearTimeout(draftSaveTimer);
  if (!playgroundSymbol) return;
  const symbol = playgroundSymbol;
  const source = playgroundSource.value;
  playgroundDraftStatus.textContent = "Saving draft locally…";
  playgroundDraftStatus.classList.remove("error");
  draftSaveTimer = window.setTimeout(() => {{
    savePlaygroundDraft(symbol, source).catch(() => {{}});
  }}, 300);
}}

function setPlaygroundStatus(message, state) {{
  playgroundStatus.className = `workspace-status result-status result-${{state}} playground-result-status`;
  playgroundStatus.textContent = message;
}}

function emptyDiff(message) {{
  const empty = document.createElement("div");
  empty.className = "playground-diff-empty";
  empty.textContent = message;
  return empty;
}}

function renderPlaygroundDiff(result) {{
  playgroundDiff.replaceChildren();
  if (!result.comparisons.length) {{
    playgroundDiff.append(emptyDiff("No instructions found for this function."));
    return;
  }}
  for (const comparison of result.comparisons) {{
    const row = document.createElement("div");
    row.className = `playground-diff-row ${{comparison.matches ? "match" : "diff"}}`;
    for (const [className, text] of [
      ["offset", `+0x${{comparison.offset}}`],
      ["instruction", `${{comparison.retail || "(no retail instruction)"}}\\n${{comparison.retail_bytes || ""}}`],
      ["instruction", `${{comparison.compiled || "(no linked instruction)"}}\\n${{comparison.compiled_bytes || ""}}`]
    ]) {{
      const cell = document.createElement("span");
      cell.className = className;
      cell.textContent = text;
      row.append(cell);
    }}
    playgroundDiff.append(row);
  }}
}}

function scheduleCompile() {{
  window.clearTimeout(compileTimer);
  compileTimer = window.setTimeout(compilePlayground, 500);
}}

async function compilePlayground() {{
  const requestId = ++compileRequest;
  const symbol = playgroundFunction.value;
  if (!symbol) return;
  setPlaygroundStatus("Playground result: compiling and linking…", "pending");
  try {{
    const result = await requestJson("/api/compile", {{
      method: "POST",
      headers: {{ "Content-Type": "application/json" }},
      body: JSON.stringify({{
        symbol,
        source: playgroundSource.value,
        ...currentCompilerProfile()
      }})
    }});
    if (requestId !== compileRequest) return;
    renderPlaygroundDiff(result);
    const addressNotice = result.compiled_address !== result.retail_address
      ? `; linked at 0x${{result.compiled_address.toString(16).toUpperCase()}} (retail: 0x${{result.retail_address.toString(16).toUpperCase()}})`
      : "";
    const state = result.status === "MATCH" ? "match" : "diff";
    setPlaygroundStatus(
      `Playground result: ${{result.status}} - ${{result.matching_bytes}} of ${{result.retail_size}} retail bytes match; linked size: ${{result.compiled_size}} bytes${{addressNotice}}`,
      state
    );
  }} catch (error) {{
    if (requestId !== compileRequest) return;
    playgroundDiff.replaceChildren(emptyDiff("Fix the C source or project link error to see the byte comparison."));
    setPlaygroundStatus(`Playground result: compile/link failed - ${{error.message}}`, "error");
  }}
}}

for (const tab of workspaceTabs) {{
  tab.addEventListener("click", () => {{
    activateView(tab.id.replace("-tab", ""));
    if (tab.id === "assembly-tab" && assemblySymbol !== assemblyFunction.value) {{
      loadAssembly(assemblyFunction.value);
    }}
    if (tab.id === "playground-tab" && playgroundSymbol !== playgroundFunction.value) {{
      loadPlaygroundFunction(playgroundFunction.value);
    }}
  }});
}}
for (const row of rows) {{
  const name = row.cells[0].textContent.trim();
  const button = document.createElement("button");
  button.type = "button";
  button.className = "function-action";
  button.textContent = name;
  button.addEventListener("click", () => loadAssembly(name));
  row.cells[0].replaceChildren(button);
}}
assemblyFunction.addEventListener("input", () => chooseFunctionFromSearch(assemblyFunction, loadAssembly));
playgroundFunction.addEventListener("input", () => chooseFunctionFromSearch(playgroundFunction, loadPlaygroundFunction));
document.getElementById("open-playground").addEventListener("click", () => {{
  activateView("playground");
  loadPlaygroundFunction(assemblyFunction.value);
}});
playgroundSource.addEventListener("input", () => {{
  playgroundDraftDirty = true;
  scheduleCompile();
  scheduleDraftSave();
}});
optimizationControl.addEventListener("change", () => {{
  scheduleCompilerProfileSave();
  scheduleCompile();
}});
preserveFramePointerControl.addEventListener("change", () => {{
  scheduleCompilerProfileSave();
  scheduleCompile();
}});
otherFlagsControl.addEventListener("input", () => {{
  scheduleCompilerProfileSave();
  scheduleCompile();
}});
try {{
  const functions = rows.map(row => ({{
    symbol: row.cells[0].querySelector("button").textContent,
    status: row.dataset.status,
    statusLabel: row.cells[2].textContent.trim()
  }}));
  functionStatuses = new Map(functions.map(item => [item.symbol, item.status]));
  functionSymbols = new Map(functions.map(item => [item.symbol.toLowerCase(), item.symbol]));
  for (const item of functions) {{
      const address = item.symbol.match(/^func_([0-9a-f]+)$/i);
      if (address) {{
        functionSymbols.set(address[1].toLowerCase(), item.symbol);
        functionSymbols.set(`0x${{address[1].toLowerCase()}}`, item.symbol);
      }}
      const option = document.createElement("option");
    option.value = item.symbol;
      option.label = item.statusLabel;
    functionOptions.append(option);
  }}
  if (functions.length) {{
    assemblyFunction.value = functions[0].symbol;
    playgroundFunction.value = functions[0].symbol;
  }}
  requestJson("/api/ready").catch(() => {{
    assemblyStatus.textContent = "Start tools\\\\dashboard.py to compare retail and build assembly.";
    setPlaygroundStatus("Playground result: unavailable - start tools\\\\dashboard.py to compile and link functions.", "error");
  }});
}} catch (error) {{
  assemblyStatus.textContent = error.message;
}}

for (const button of buttons) {{
  button.addEventListener("click", () => {{
    activeStatus = button.dataset.statusFilter;
    for (const item of buttons) item.setAttribute("aria-pressed", String(item === button));
    applyFilters();
    saveViewState();
  }});
}}
filter.addEventListener("input", () => {{
  applyFilters();
  saveViewState();
}});
window.addEventListener("pagehide", () => {{
  saveViewState();
  window.clearTimeout(draftSaveTimer);
  window.clearTimeout(profileSaveTimer);
  if (playgroundSymbol && playgroundDraftDirty) {{
    const body = new Blob(
      [JSON.stringify({{ symbol: playgroundSymbol, source: playgroundSource.value }})],
      {{ type: "application/json" }}
    );
    if (!navigator.sendBeacon("/api/draft", body)) {{
      fetch("/api/draft", {{
        method: "POST",
        headers: {{ "Content-Type": "application/json" }},
        body,
        keepalive: true
      }}).catch(error => console.error("Could not save playground draft:", error));
    }}
  }}
  if (playgroundSymbol && compilerProfileDirty) {{
    const body = new Blob(
      [JSON.stringify({{ symbol: playgroundSymbol, profile: currentCompilerProfile() }})],
      {{ type: "application/json" }}
    );
    if (!navigator.sendBeacon("/api/profile", body)) {{
      fetch("/api/profile", {{
        method: "POST",
        headers: {{ "Content-Type": "application/json" }},
        body,
        keepalive: true
      }}).catch(error => console.error("Could not save compiler profile:", error));
    }}
  }}
}});
window.addEventListener("scroll", () => {{
  window.clearTimeout(saveScrollTimer);
  saveScrollTimer = window.setTimeout(saveViewState, 150);
}}, {{ passive: true }});
const refreshState = sessionStorage.getItem(refreshStateKey);
const state = refreshState ? JSON.parse(refreshState) : null;
if (state) {{
  filter.value = state.search;
  if (buttons.some(button => button.dataset.statusFilter === state.status)) {{
    activeStatus = state.status;
  }}
  for (const button of buttons) {{
    button.setAttribute("aria-pressed", String(button.dataset.statusFilter === activeStatus));
  }}
}}
applyFilters();
if (state) {{
  window.history.scrollRestoration = "manual";
  const row = rows.find(item => item.cells[0].textContent.trim() === state.rowName);
  if (row && !row.hidden) {{
    window.scrollBy(0, row.getBoundingClientRect().top - state.rowOffset);
  }} else {{
    window.scrollTo(0, state.scrollY);
  }}
}}
if (location.protocol !== "file:") function refreshProgressView() {{
  if (location.protocol !== "file:" && document.getElementById("progress-view").classList.contains("active")) {{
    saveViewState();
    window.location.reload();
  }} else {{
    window.setTimeout(refreshProgressView, 30000);
  }}
}}
window.setTimeout(refreshProgressView, 30000);
</script>
</body>
</html>
"""
    HTML_REPORT.write_text(document, encoding="utf-8", newline="\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("build_elf", nargs="?", type=Path, default=BUILD_ELF)
    args = parser.parse_args()

    try:
        retail = Elf32LE(RETAIL)
        build = Elf32LE(args.build_elf)
        inventory = function_inventory()
        implementations = project_implementations(inventory)
        rows = compare_functions(retail, build, inventory, implementations)
        counts = summary(rows)
        now = datetime.now().astimezone()
        generated_at = f"{now:%Y-%m-%d}"
        REPORT_DIR.mkdir(parents=True, exist_ok=True)
        write_csv(rows)
        write_html(rows, counts, generated_at)
        write_svg(counts)
        update_readme_progress(counts)
    except (OSError, UnicodeError, ValueError, struct.error) as error:
        return fail(str(error))

    print(
        f"Byte-match: {counts['MATCHING']}; implemented: {counts['NOT MATCHING']}; "
        f"not implemented: {counts['NOT STARTED']} (total: {counts['total']})."
    )
    print(f"Dashboard: {HTML_REPORT.relative_to(ROOT)}")
    print(f"CSV: {CSV_REPORT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
