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
    source_files = [*SOURCE_ROOT.rglob("*.c"), *SOURCE_ROOT.rglob("*.inc")]
    for path in sorted(source_files):
        source = path.read_text(encoding="utf-8")
        asm_references.update(INCLUDE_ASM.findall(source))
        c_definitions.update(FUNCTION_DEFINITION.findall(source))
        c_definitions.update(INLINE_ASM_FUNCTION.findall(source))

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
        f"![Progress: {counts['decomp_percent']:.2f}% complete, "
        f"{counts['MATCHING']:,} matching, "
        f"{counts['NOT MATCHING']:,} not matching, "
        f"{counts['NOT STARTED']:,} not started](progress/progress.svg)"
        "\n\n"
        f"**{counts['decomp_percent']:.2f}% complete** "
        f"({counts['MATCHING']:,} / {counts['total']:,} functions). "
        f"**Matching:** {counts['MATCHING']:,} · "
        f"**Not matching:** {counts['NOT MATCHING']:,} · "
        f"**Not started:** {counts['NOT STARTED']:,}."
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


def write_svg(counts):
    width = 672
    segments = (
        ("MATCHING", "#63c99b"),
        ("NOT MATCHING", "#cf765e"),
        ("NOT STARTED", "#555767"),
    )
    bar = []
    offset = 24
    for status, color in segments:
        segment_width = (
            width * counts[status] / counts["total"]
            if counts["total"]
            else 0
        )
        if segment_width:
            bar.append(
                f'<rect x="{offset:.2f}" y="90" width="{segment_width:.2f}" '
                f'height="22" fill="{color}"/>'
            )
        offset += segment_width

    labels = (
        ("Matching", counts["MATCHING"], "#63c99b", 24),
        ("Not matching", counts["NOT MATCHING"], "#cf765e", 252),
        ("Not started", counts["NOT STARTED"], "#a7a9c1", 480),
    )
    legend = "\n".join(
        f'<circle cx="{x}" cy="147" r="5" fill="{color}"/>'
        f'<text x="{x + 12}" y="152" fill="#c0c1d0" font-size="14">'
        f'{label}: {count:,}</text>'
        for label, count, color, x in labels
    )
    document = f"""<svg xmlns="http://www.w3.org/2000/svg" width="720" height="180" viewBox="0 0 720 180" role="img" aria-labelledby="title description">
<title id="title">Decompilation progress: {counts["decomp_percent"]:.2f}% complete</title>
<desc id="description">{counts["MATCHING"]:,} matching, {counts["NOT MATCHING"]:,} not matching, and {counts["NOT STARTED"]:,} not started out of {counts["total"]:,} functions.</desc>
<rect width="720" height="180" rx="16" fill="#10111a"/>
<text x="24" y="42" fill="#f1f1f7" font-family="Segoe UI, Arial, sans-serif" font-size="18" font-weight="600">Decompilation progress</text>
<text x="696" y="44" text-anchor="end" fill="#d3d4ff" font-family="Segoe UI, Arial, sans-serif" font-size="24" font-weight="700">{counts["decomp_percent"]:.2f}%</text>
<rect x="24" y="90" width="{width}" height="22" rx="11" fill="#252738"/>
<clipPath id="bar-clip"><rect x="24" y="90" width="{width}" height="22" rx="11"/></clipPath>
<g clip-path="url(#bar-clip)">
{''.join(bar)}
</g>
{legend}
</svg>
"""
    SVG_REPORT.write_text(document, encoding="utf-8", newline="\n")


def summary(rows):
    counts = {
        status: sum(row["status"] == status for row in rows)
        for status in STATUS_CLASS
    }
    counts["total"] = len(rows)
    counts["decomp_percent"] = (
        100.0 * counts["MATCHING"] / counts["total"]
        if counts["total"]
        else 0.0
    )
    return counts


def write_html(rows, counts, generated_at):
    table_rows = []
    for row in rows:
        status_class = STATUS_CLASS[row["status"]]
        table_rows.append(
            "<tr>"
            f"<td><code>{html.escape(row['name'])}</code></td>"
            f"<td>0x{row['address']:08X}</td>"
            f'<td><span class="badge {status_class}">{html.escape(row["status"])}</span></td>'
            f"<td>{row['retail_size']}</td>"
            f"<td>{(str(row['matching_bytes']) + ' / ' + str(row['retail_size'])) if row['matching_bytes'] != '' else '-'}</td>"
            f"<td>{html.escape(str(row['first_difference']) or '-')}</td>"
            "</tr>"
        )

    cards = (
        ("Not started", counts["NOT STARTED"], "not-started"),
        ("Not matching", counts["NOT MATCHING"], "not-matching"),
        ("Matching", counts["MATCHING"], "matching"),
    )
    cards_html = "\n".join(
        f'<article class="card {css_class}"><span>{title}</span>'
        f"<strong>{count:,}</strong></article>"
        for title, count, css_class in cards
    )
    filters_html = "\n".join(
        f'<button class="status-filter {css_class}" type="button" '
        f'data-status-filter="{html.escape(status)}" aria-pressed="false">'
        f'<span class="filter-dot" aria-hidden="true"></span>'
        f'{html.escape(title)} <span class="filter-count">{counts[status]:,}</span>'
        "</button>"
        for status, title, css_class in (
            ("MATCHING", "Matching", "matching"),
            ("NOT MATCHING", "Not matching", "not-matching"),
            ("NOT STARTED", "Not started", "not-started"),
        )
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
.summary {{ display: grid; grid-template-columns: minmax(200px, .65fr) 2fr; gap: 30px; align-items: center; margin: 32px 0 12px; }}
.hero .summary, .hero .cards, .hero .note {{ max-width: 100%; }}
.percent {{ font-size: 2.6rem; font-weight: 760; color: #d3d4ff; letter-spacing: -.04em; }}
.percent small {{ display: block; margin-top: 3px; color: #a7a9c1; font-size: .82rem; font-weight: 500; letter-spacing: 0; }}
.bar {{ height: 14px; background: #141622; border: 1px solid #3a3d71; border-radius: 99px; overflow: hidden; }}
.bar span {{ display: block; height: 100%; width: {counts["decomp_percent"]:.2f}%; background: linear-gradient(90deg, #3a3d71, #8286d1); box-shadow: 0 0 20px #3a3d7188; }}
.cards {{ display: grid; grid-template-columns: repeat(3, minmax(0, 1fr)); gap: 14px; margin: 23px 0; }}
.card {{ background: linear-gradient(135deg, #11121c, #0c0d13); border: 1px solid #252738; border-radius: 15px; padding: 18px 20px; box-shadow: 0 8px 26px #00000055; }}
.card span {{ display: block; color: #b6b7c8; font-size: .88rem; }}
.card strong {{ display: block; margin-top: 7px; font-size: 1.9rem; letter-spacing: -.03em; }}
.card.not-started {{ border-top: 2px solid #555767; }} .card.not-matching {{ border-top: 2px solid #cf765e; }}
.card.matching {{ border-top: 2px solid #63c99b; }}
.badge {{ display: inline-block; border: 1px solid transparent; border-radius: 999px; padding: 4px 10px; font-weight: 700; font-size: .75rem; letter-spacing: .025em; }}
.badge.not-started {{ color: #c5c6d1; background: #20212b; border-color: #444654; }}
.badge.not-matching {{ color: #ffd2c5; background: #321c1b; border-color: #784239; }}
.badge.matching {{ color: #b5f5ce; background: #173c2c; border-color: #38835c; }}
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
.status-filter.matching .filter-dot {{ background: #63c99b; box-shadow: 0 0 10px #63c99b77; }}
.status-filter.not-matching .filter-dot {{ background: #cf765e; }}
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
@media (max-width: 900px) {{ .toolbar {{ align-items: stretch; flex-direction: column; }} .search-wrap {{ min-width: 0; }} }}
@media (max-width: 700px) {{ main {{ padding: 18px 14px 36px; }} .hero {{ padding: 24px 20px; }} .summary {{ grid-template-columns: 1fr; gap: 14px; margin-top: 25px; }} .cards {{ gap: 8px; }} .card {{ padding: 14px 12px; }} .card strong {{ font-size: 1.5rem; }} .section-head {{ align-items: start; flex-direction: column; }} .controls {{ display: grid; grid-template-columns: 1fr; }} .status-filter {{ justify-content: space-between; }} }}
</style>
</head>
<body>
<main>
<section class="hero">
<div class="hero-copy">
<div class="eyebrow">Project progress</div>
<h1>Rayman Revolution</h1>
<div class="subtitle">Main executable - decompilation progress</div>
<div class="subtitle updated">Updated {html.escape(generated_at)} · {counts["total"]:,} functions tracked</div>
<div class="subtitle">This page refreshes automatically every 30 seconds.</div>
</div>
<section class="summary" aria-label="Decompilation progress">
  <div class="percent">{counts["decomp_percent"]:.2f}%<small>reimplemented and matching</small></div>
  <div class="bar" role="progressbar" aria-valuenow="{counts["decomp_percent"]:.2f}" aria-valuemin="0" aria-valuemax="100"><span></span></div>
</section>
<section class="cards">{cards_html}</section>
<p class="note"><strong>Automatic tracking</strong> · Retail assembly fallbacks stay NOT STARTED even when their bytes match. C definitions and changed assembly count as reimplementations; they are MATCHING only when the built bytes match retail.</p>
<p class="note"><strong>Byte counts</strong> · Original bytes is the size of the retail function. Matching bytes is shown as matching / total; for example, 8 / 8 means all 8 bytes in your built function match the original.</p>
</section>
<section aria-labelledby="functions-heading">
<div class="section-head">
  <h2 id="functions-heading">Function breakdown</h2><p>Filter by status, then narrow by name or address.</p>
</div>
<div class="toolbar">
<div class="controls">
  <button class="status-filter" type="button" data-status-filter="ALL" aria-pressed="true">All <span class="filter-count">{counts["total"]:,}</span></button>
  {filters_html}
</div>
<label class="search-wrap" for="filter">
  <span class="search-icon" aria-hidden="true">⌕</span>
  <input id="filter" type="search" placeholder="Search function, address, or file" autocomplete="off">
</label>
</div>
<div class="results" aria-live="polite"><span id="result-count"></span><span>Statuses are detected automatically during build</span></div>
<div class="table-wrap">
<table id="functions">
<thead><tr><th>Function</th><th>Address</th><th>Status</th><th>Original bytes</th><th>Matching bytes</th><th>First difference</th></tr></thead>
<tbody>{''.join(table_rows)}</tbody>
</table>
<div class="empty" id="empty-state">No functions match these filters. Try another status or search.</div>
</div>
</section>
</main>
<script>
const filter = document.getElementById("filter");
const buttons = [...document.querySelectorAll("[data-status-filter]")];
const rows = [...document.querySelectorAll("#functions tbody tr")];
const resultCount = document.getElementById("result-count");
const emptyState = document.getElementById("empty-state");
const refreshStateKey = "progress-dashboard-refresh";
let activeStatus = "ALL";
let saveScrollTimer;

function applyFilters() {{
  const query = filter.value.trim().toLowerCase();
  let visible = 0;
  for (const row of rows) {{
    const status = row.cells[2].textContent.trim();
    const statusMatches = activeStatus === "ALL" || status === activeStatus;
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
window.addEventListener("pagehide", saveViewState);
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
window.setTimeout(() => {{
  saveViewState();
  window.location.reload();
}}, 30000);
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
        f"Reimplementation progress: {counts['MATCHING']}/"
        f"{counts['total']} functions ({counts['decomp_percent']:.2f}%). "
        f"Not started: {counts['NOT STARTED']}; not matching: "
        f"{counts['NOT MATCHING']}; "
        f"matching: {counts['MATCHING']}."
    )
    print(f"Dashboard: {HTML_REPORT.relative_to(ROOT)}")
    print(f"CSV: {CSV_REPORT.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
