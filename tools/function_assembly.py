#!/usr/bin/env python3
"""Show original and linked assembly side by side for one function."""

import argparse
import json
import re
import subprocess
import sys
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse

from compare import Elf32LE, RETAIL


ROOT = Path(__file__).resolve().parents[1]
BUILD_ELF = ROOT / "build" / "SLES_500.44.elf"
OBJDUMP = (
    ROOT
    / "toolchain"
    / "sn-prodg-24"
    / "local"
    / "sce"
    / "ee"
    / "gcc"
    / "bin"
    / "ee-objdump.exe"
)
REFERENCE_HEADER = re.compile(
    r"(?m)^\s*(?:matching|nonmatching)\s+"
    r"(func_[0-9A-Fa-f]{8})\s*,\s*(0x[0-9A-Fa-f]+)\s*$"
)
DISASSEMBLED_INSTRUCTION = re.compile(
    r"^\s*([0-9A-Fa-f]+):\s+([0-9A-Fa-f]{8})\s+(.+?)\s*$"
)

PAGE = r"""<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Function Assembly Watch</title>
<style>
:root { color-scheme: dark; font: 15px/1.5 system-ui, sans-serif; background: #111827; color: #e5e7eb; }
* { box-sizing: border-box; }
body { margin: 0 auto; padding: 24px; max-width: 1440px; }
h1 { margin: 0; font-size: 1.4rem; }
.meta, #status { color: #9ca3af; }
.meta { margin: 5px 0 16px; }
#status { margin: 0 0 14px; }
.row { display: grid; grid-template-columns: 90px minmax(0, 1fr) minmax(0, 1fr); gap: 12px; align-items: start; padding: 8px 10px; border-bottom: 1px solid #263244; }
.row.header { position: sticky; top: 0; background: #111827; color: #cbd5e1; font-weight: 600; }
.offset, .instruction, .bytes { font: 13px/1.55 ui-monospace, SFMono-Regular, Consolas, monospace; }
.offset { color: #94a3b8; }
.instruction { white-space: pre-wrap; overflow-wrap: anywhere; }
.bytes { display: block; color: #94a3b8; }
.match { background: #052e1a; }
.diff { background: #3b1117; }
.badge { display: inline-block; border-radius: 4px; padding: 1px 5px; font-size: 11px; font-weight: 700; }
.match .badge { color: #86efac; background: #14532d; }
.diff .badge { color: #fca5a5; background: #7f1d1d; }
@media (max-width: 700px) {
  body { padding: 14px; }
  .row { grid-template-columns: 58px minmax(0, 1fr) minmax(0, 1fr); gap: 6px; padding: 7px 4px; }
  .instruction { font-size: 11px; }
}
</style>
</head>
<body>
<h1>Function Assembly Watch</h1>
<p class="meta" id="meta"></p>
<p id="status" role="status">Waiting for the linked ELF…</p>
<div class="row header"><span>Offset</span><span>Original · bytes</span><span>Compiled · bytes</span></div>
<main id="instructions"></main>
<script>
const meta = document.querySelector("#meta");
const status = document.querySelector("#status");
const instructions = document.querySelector("#instructions");
let renderedVersion = null;
let pendingVersion = null;
let polling = false;

async function getJson(path) {
  const response = await fetch(path, { cache: "no-store" });
  const result = await response.json();
  if (!response.ok) throw new Error(result.error || `Request failed (${response.status}).`);
  return result;
}

function showComparison(result) {
  meta.textContent = `${result.symbol} · Reference: ${result.reference} · ELF: ${result.elf}`;
  status.textContent = `${result.status} · retail ${result.retail_size} bytes · compiled ${result.compiled_size} bytes · Updated ${new Date().toLocaleTimeString()}`;
  status.style.color = result.status === "MATCH" ? "#86efac" : "#fca5a5";
  instructions.replaceChildren();
  for (const instruction of result.instructions) {
    const row = document.createElement("div");
    row.className = `row ${instruction.matches ? "match" : "diff"}`;
    const offset = document.createElement("span");
    offset.className = "offset";
    offset.textContent = instruction.offset;
    const badge = document.createElement("span");
    badge.className = "badge";
    badge.textContent = instruction.matches ? "MATCH" : "DIFF";
    const retail = document.createElement("div");
    retail.className = "instruction";
    const retailText = document.createElement("span");
    retailText.textContent = instruction.retail;
    retailText.prepend(badge, document.createTextNode(" "));
    const retailBytes = document.createElement("span");
    retailBytes.className = "bytes";
    retailBytes.textContent = instruction.retail_bytes;
    retail.append(retailText, retailBytes);
    const compiled = document.createElement("div");
    compiled.className = "instruction";
    const compiledText = document.createElement("span");
    compiledText.textContent = instruction.compiled;
    const compiledBytes = document.createElement("span");
    compiledBytes.className = "bytes";
    compiledBytes.textContent = instruction.compiled_bytes;
    compiled.append(compiledText, compiledBytes);
    row.append(offset, retail, compiled);
    instructions.append(row);
  }
  renderedVersion = result.version;
  pendingVersion = null;
}

async function refreshAfterBuild(version) {
  await new Promise(resolve => setTimeout(resolve, 1200));
  try {
    const current = await getJson("/version");
    if (current.version !== version) {
      pendingVersion = null;
      return;
    }
    status.textContent = "Build detected; comparing instructions…";
    showComparison(await getJson("/api"));
  } catch (error) {
    status.textContent = `Could not compare build: ${error.message}`;
    status.style.color = "#fca5a5";
    pendingVersion = null;
  }
}

async function poll() {
  if (polling) return;
  polling = true;
  try {
    const current = await getJson("/version");
    if (current.version && current.version !== renderedVersion && current.version !== pendingVersion) {
      pendingVersion = current.version;
      refreshAfterBuild(current.version);
    }
  } catch (error) {
    status.textContent = `Waiting for build output: ${error.message}`;
  } finally {
    polling = false;
  }
}

poll();
setInterval(poll, 1000);
</script>
</body>
</html>
"""


def fail(message):
    print(f"function_assembly: {message}", file=sys.stderr)
    return 1


def reference_path(symbol, specified_path):
    if specified_path:
        path = specified_path.resolve()
        if not path.is_file():
            raise FileNotFoundError(f"reference assembly not found: {path}")
        return path

    assembly_root = ROOT / "asm"
    for category in ("nonmatchings", "matchings"):
        candidate = assembly_root / category
        matches = list(candidate.rglob(f"{symbol}.s"))
        if len(matches) == 1:
            return matches[0]
        if len(matches) > 1:
            paths = ", ".join(str(path.relative_to(ROOT)) for path in matches)
            raise ValueError(
                f"multiple {category} reference assemblies found for "
                f"{symbol}: {paths}; use --reference to specify one"
            )

    matches = list(assembly_root.rglob(f"{symbol}.s"))
    if not matches:
        raise FileNotFoundError(
            f"no reference assembly found for {symbol}; use --reference to specify it"
        )
    if len(matches) > 1:
        paths = ", ".join(str(path.relative_to(ROOT)) for path in matches)
        raise ValueError(
            f"multiple reference assemblies found for {symbol}: {paths}; "
            "use --reference to choose one"
        )
    return matches[0]


def parse_reference(path, symbol):
    source = path.read_text(encoding="utf-8")
    headers = list(REFERENCE_HEADER.finditer(source))
    if len(headers) != 1:
        raise ValueError(
            f"{path}: expected one function size header, found {len(headers)}"
        )
    header = headers[0]
    if header.group(1) != symbol:
        raise ValueError(
            f"{path}: header is for {header.group(1)}, expected {symbol}"
        )

    size = int(header.group(2), 16)
    if size <= 0 or size % 4:
        raise ValueError(f"{path}: invalid function size {size:#x}")
    return size


def disassemble(path, start, size):
    result = subprocess.run(
        [
            str(OBJDUMP),
            "-d",
            f"--start-address={start:#x}",
            f"--stop-address={start + size:#x}",
            str(path),
        ],
        cwd=ROOT,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        errors="replace",
        check=False,
    )
    if result.returncode:
        raise RuntimeError(
            f"EE objdump failed with status {result.returncode}:\n{result.stdout.strip()}"
        )
    instructions = {}
    for line in result.stdout.splitlines():
        match = DISASSEMBLED_INSTRUCTION.match(line)
        if match:
            address = int(match.group(1), 16)
            encoded = int(match.group(2), 16).to_bytes(4, "little")
            instructions[address] = (encoded, match.group(3).strip())
    return instructions


def file_version(path):
    try:
        stat = path.stat()
    except FileNotFoundError:
        return None
    return f"{stat.st_mtime_ns:x}-{stat.st_size:x}"


def compare_function(elf_path, symbol_name, reference_arg=None):
    initial_version = file_version(elf_path)
    reference = reference_path(symbol_name, reference_arg)
    retail_size = parse_reference(reference, symbol_name)
    retail_start = int(symbol_name[5:], 16)
    retail_elf = Elf32LE(RETAIL)
    retail_bytes = retail_elf.bytes_at_vaddr(retail_start, retail_size)
    retail_instructions = disassemble(RETAIL, retail_start, retail_size)
    build = Elf32LE(elf_path)
    symbol = build.symbol(symbol_name)
    if symbol["type"] != 2:
        raise ValueError(f"{symbol_name!r} is not an ELF function symbol")

    build_size = symbol["size"]
    compiled_instructions = disassemble(elf_path, symbol["value"], build_size)
    compiled_bytes = build.bytes_at_vaddr(symbol["value"], build_size)
    if file_version(elf_path) != initial_version:
        raise RuntimeError("linked ELF changed while comparing; retrying after the build")
    offsets = sorted(
        {
            address - retail_start
            for address in retail_instructions
        }
        | {
            address - symbol["value"]
            for address in compiled_instructions
        }
    )
    instructions = []
    matched = retail_size == symbol["size"]
    for offset in offsets:
        retail = retail_instructions.get(retail_start + offset)
        compiled = compiled_instructions.get(symbol["value"] + offset)
        retail_code = retail_bytes[offset:offset + 4]
        compiled_code = compiled_bytes[offset:offset + 4]
        equal = (
            retail is not None
            and compiled is not None
            and len(retail_code) == 4
            and retail_code == compiled_code
        )
        matched = equal and matched
        instructions.append(
            {
                "offset": f"+0x{offset:04X}",
                "matches": equal,
                "retail": retail[1] if retail else "<no instruction>",
                "compiled": compiled[1] if compiled else "<no instruction>",
                "retail_bytes": retail_code.hex(" ").upper() or "<no bytes>",
                "compiled_bytes": compiled_code.hex(" ").upper() or "<no bytes>",
            }
        )

    try:
        reference_label = str(reference.relative_to(ROOT))
    except ValueError:
        reference_label = str(reference)
    return {
        "symbol": symbol_name,
        "reference": reference_label,
        "elf": str(elf_path),
        "version": file_version(elf_path),
        "retail_size": retail_size,
        "compiled_size": symbol["size"],
        "status": "MATCH" if matched else "DIFF",
        "instructions": instructions,
    }


def serve_web(symbol_name, elf_path, reference_arg, port, open_browser):
    class RequestHandler(BaseHTTPRequestHandler):
        def send_json(self, status_code, payload):
            body = json.dumps(payload).encode("utf-8")
            self.send_response(status_code)
            self.send_header("Content-Type", "application/json; charset=utf-8")
            self.send_header("Cache-Control", "no-store")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)

        def do_GET(self):
            route = urlparse(self.path).path
            if route == "/":
                body = PAGE.encode("utf-8")
                self.send_response(200)
                self.send_header("Content-Type", "text/html; charset=utf-8")
                self.send_header("Content-Length", str(len(body)))
                self.end_headers()
                self.wfile.write(body)
                return
            if route == "/version":
                self.send_json(200, {"version": file_version(elf_path)})
                return
            if route == "/api":
                try:
                    self.send_json(
                        200, compare_function(elf_path, symbol_name, reference_arg)
                    )
                except (OSError, UnicodeError, ValueError, RuntimeError) as error:
                    self.send_json(503, {"error": str(error)})
                return
            self.send_error(404)

        def log_message(self, format_string, *args):
            return

    try:
        server = ThreadingHTTPServer(("127.0.0.1", port), RequestHandler)
    except OSError as error:
        return fail(f"could not start local website: {error}")
    url = f"http://127.0.0.1:{server.server_port}/"
    print(f"Function assembly watch: {url}")
    print("Rebuild the project to refresh the comparison. Press Ctrl+C to stop.")
    if open_browser:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopping function assembly watch.")
    finally:
        server.server_close()
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("symbol", help="function symbol, for example func_001C8D78")
    parser.add_argument(
        "--elf", type=Path, default=BUILD_ELF, help="linked ELF (default: build ELF)"
    )
    parser.add_argument(
        "--reference",
        type=Path,
        help="reference assembly file (auto-detected when unique)",
    )
    parser.add_argument(
        "--web", action="store_true", help="open a local page that refreshes after builds"
    )
    parser.add_argument(
        "--port", type=int, default=8766, help="local website port (default: 8766)"
    )
    parser.add_argument(
        "--no-browser", action="store_true", help="do not open the website automatically"
    )
    arguments = parser.parse_args()

    try:
        if not OBJDUMP.is_file():
            raise FileNotFoundError(f"EE objdump not found: {OBJDUMP}")
        elf_path = arguments.elf.resolve()
        if not arguments.web and not elf_path.is_file():
            raise FileNotFoundError(
                f"linked ELF not found: {elf_path}; run tools\\build.py first"
            )
        reference_path(arguments.symbol, arguments.reference)
        if arguments.web:
            if not 0 <= arguments.port <= 65535:
                raise ValueError("port must be between 0 and 65535")
            return serve_web(
                arguments.symbol,
                elf_path,
                arguments.reference,
                arguments.port,
                not arguments.no_browser,
            )
        result = compare_function(elf_path, arguments.symbol, arguments.reference)

        print(f"Function: {result['symbol']}")
        print(f"Reference: {result['reference']}")
        print(f"Linked ELF: {result['elf']}")
        print(
            f"Sizes: retail={result['retail_size']} bytes, "
            f"compiled={result['compiled_size']} bytes"
        )
        print(f"{'Offset':>8} {'':>5} | {'RETAIL':<45} | COMPILED")
        print("-" * 100)
        for instruction in result["instructions"]:
            status = "MATCH" if instruction["matches"] else "DIFF"
            print(
                f"{instruction['offset']:>8} {status:>5} | "
                f"{instruction['retail']:<45} | {instruction['compiled']}"
            )
        print(f"\n{'MATCH' if result['status'] == 'MATCH' else 'DIFFERENT'}")
        return 0 if result["status"] == "MATCH" else 1
    except (OSError, UnicodeError, ValueError, RuntimeError) as error:
        return fail(str(error))


if __name__ == "__main__":
    sys.exit(main())
