#!/usr/bin/env python3
"""Serve the combined progress, function assembly, and C playground dashboard."""

import argparse
import json
import re
import subprocess
import sys
import tempfile
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse

import compiler_explorer
import function_assembly
import status as progress
from compare import Elf32LE, RETAIL


ROOT = Path(__file__).resolve().parents[1]
BUILD_ELF = ROOT / "build" / "SLES_500.44.elf"
MAX_SOURCE_BYTES = compiler_explorer.MAX_SOURCE_BYTES
MAX_REQUEST_BYTES = MAX_SOURCE_BYTES * 6 + 2048
FUNCTION_NAME = re.compile(r"^func_[0-9A-Fa-f]{8}$")
DRAFT_DIRECTORY = progress.REPORT_DIR / ".playground"


def function_reference(symbol):
    if not isinstance(symbol, str) or not FUNCTION_NAME.fullmatch(symbol):
        raise ValueError("Invalid function symbol.")
    reference = function_assembly.reference_path(symbol, None)
    size = function_assembly.parse_reference(reference, symbol)
    return reference, size, reference.read_text(encoding="utf-8")


def draft_path(symbol):
    if not isinstance(symbol, str) or not FUNCTION_NAME.fullmatch(symbol):
        raise ValueError("Invalid function symbol.")
    return DRAFT_DIRECTORY / f"{symbol}.c"


def profile_path(symbol):
    if not isinstance(symbol, str) or not FUNCTION_NAME.fullmatch(symbol):
        raise ValueError("Invalid function symbol.")
    return DRAFT_DIRECTORY / f"{symbol}.json"


def validate_profile(profile):
    if not isinstance(profile, dict):
        raise ValueError("Compiler profile must be a JSON object.")
    optimization = profile.get("optimization", "-O2")
    preserve_frame_pointer = profile.get("preserve_frame_pointer", False)
    if optimization not in ("-O0", "-O1", "-O2", "-O3"):
        raise ValueError("Unsupported optimization level in compiler profile.")
    if not isinstance(preserve_frame_pointer, bool):
        raise ValueError("Preserve frame pointer must be true or false.")
    return {
        "optimization": optimization,
        "preserve_frame_pointer": preserve_frame_pointer,
    }


def load_profile(symbol):
    function_reference(symbol)
    path = profile_path(symbol)
    if not path.is_file():
        return {"optimization": "-O2", "preserve_frame_pointer": False}, False
    try:
        profile = json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as error:
        raise ValueError(f"Invalid saved compiler profile for {symbol}: {error}") from error
    return validate_profile(profile), True


def save_profile(symbol, profile):
    function_reference(symbol)
    profile = validate_profile(profile)
    DRAFT_DIRECTORY.mkdir(parents=True, exist_ok=True)
    path = profile_path(symbol)
    temporary_path = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            encoding="utf-8",
            newline="\n",
            dir=DRAFT_DIRECTORY,
            prefix=f".{symbol}.",
            suffix=".tmp",
            delete=False,
        ) as temporary_file:
            temporary_path = Path(temporary_file.name)
            json.dump(profile, temporary_file, indent=2)
            temporary_file.write("\n")
        temporary_path.replace(path)
    finally:
        if temporary_path and temporary_path.exists():
            temporary_path.unlink()


def starter_source(symbol):
    _, module_source = compiler_explorer.module_source_for(symbol)
    signature = compiler_explorer.function_signature(module_source, symbol)
    if signature:
        source = (
            f"{signature}\n"
            "{\n"
            "    // Add the function implementation here.\n"
            "}\n"
        )
        return source, True
    source = (
        f"void {symbol}(void)\n"
        "{\n"
        "    // Replace this signature with the function's real signature.\n"
        "}\n"
    )
    return source, False


def load_draft(symbol):
    function_reference(symbol)
    path = draft_path(symbol)
    if path.is_file():
        source = path.read_text(encoding="utf-8")
        if compiler_explorer.function_definition_span(source, symbol):
            return source, True, False, False
        source, signature_inferred = starter_source(symbol)
        return source, False, signature_inferred, True
    source, signature_inferred = starter_source(symbol)
    return source, False, signature_inferred, False


def save_draft(symbol, source):
    function_reference(symbol)
    if not isinstance(source, str):
        raise ValueError("Draft source must be text.")
    if len(source.encode("utf-8")) > MAX_SOURCE_BYTES:
        raise ValueError("Draft source must be 128 KiB or smaller.")

    DRAFT_DIRECTORY.mkdir(parents=True, exist_ok=True)
    path = draft_path(symbol)
    temporary_path = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            encoding="utf-8",
            newline="\n",
            dir=DRAFT_DIRECTORY,
            prefix=f".{symbol}.",
            suffix=".tmp",
            delete=False,
        ) as temporary_file:
            temporary_path = Path(temporary_file.name)
            temporary_file.write(source)
        temporary_path.replace(path)
    finally:
        if temporary_path and temporary_path.exists():
            temporary_path.unlink()


def assembly_comparison(symbol):
    reference, retail_size, reference_source = function_reference(symbol)
    retail_start = int(symbol[5:], 16)
    retail_elf = Elf32LE(RETAIL)
    retail_bytes = retail_elf.bytes_at_vaddr(retail_start, retail_size)
    retail_instructions = function_assembly.disassemble(RETAIL, retail_start, retail_size)

    build_size = 0
    compiled_bytes = b""
    compiled_instructions = {}
    build_start = 0
    address_notice = ""
    build_notice = ""
    if BUILD_ELF.is_file():
        build = Elf32LE(BUILD_ELF)
        try:
            build_symbol = build.symbol(symbol)
        except ValueError as error:
            if "symbol" not in str(error) or "not found" not in str(error):
                raise
            build_notice = "Function is not present in the current build."
        else:
            if build_symbol["type"] != 2:
                raise ValueError(f"{symbol} is not an ELF function symbol.")
            build_start = build_symbol["value"]
            if build_start != retail_start:
                address_notice = (
                    f"Build address 0x{build_start:08X} differs from retail "
                    f"address 0x{retail_start:08X}."
                )
            build_size = build_symbol["size"]
            compiled_bytes = build.bytes_at_vaddr(build_start, build_size)
            compiled_instructions = function_assembly.disassemble(
                BUILD_ELF, build_start, build_size
            )
    else:
        build_notice = "Build ELF not found; build the project to compare compiled bytes."

    instructions = []
    max_size = max(retail_size, build_size)
    for offset in range(0, max_size, 4):
        retail_instruction = retail_instructions.get(retail_start + offset)
        compiled_instruction = None
        if compiled_instructions:
            compiled_instruction = compiled_instructions.get(build_start + offset)
        retail_word = retail_bytes[offset:offset + 4]
        compiled_word = compiled_bytes[offset:offset + 4]
        matches = len(retail_word) == 4 and retail_word == compiled_word
        instructions.append(
            {
                "offset": f"+0x{offset:04X}",
                "matches": matches,
                "retail": retail_instruction[1] if retail_instruction else "<no instruction>",
                "compiled": (
                    compiled_instruction[1]
                    if compiled_instruction else "<no build instruction>"
                ),
                "retail_bytes": retail_word.hex(" ").upper() or "<no bytes>",
                "compiled_bytes": compiled_word.hex(" ").upper() or "<no bytes>",
            }
        )

    build_notice = build_notice or address_notice
    status_text = (
        "MATCH"
        if build_size == retail_size
        and not build_notice
        and all(item["matches"] for item in instructions)
        else ("BUILD NOT FOUND" if build_notice else "DIFF")
    )
    return {
        "symbol": symbol,
        "reference": reference.relative_to(ROOT).as_posix(),
        "retail_size": retail_size,
        "compiled_size": build_size,
        "build_address": build_start or None,
        "status": status_text,
        "notice": build_notice,
        "instructions": instructions,
        "reference_assembly": reference_source,
    }


def compile_function(request):
    symbol = request.get("symbol")
    source = request.get("source")
    optimization = request.get("optimization", "-O2")
    preserve_frame_pointer = request.get("preserve_frame_pointer", False)
    if not isinstance(symbol, str) or not FUNCTION_NAME.fullmatch(symbol):
        raise ValueError("Select a valid retail function.")
    function_reference(symbol)
    if not isinstance(source, str):
        raise ValueError("Request must include C source text.")
    try:
        source_size = len(source.encode("utf-8"))
    except UnicodeEncodeError as error:
        raise ValueError("Source must contain valid Unicode text.") from error
    if source_size > MAX_SOURCE_BYTES:
        raise ValueError("Source must be 128 KiB or smaller.")
    if optimization not in ("-O0", "-O1", "-O2", "-O3"):
        raise ValueError("Unsupported optimization level.")
    if not isinstance(preserve_frame_pointer, bool):
        raise ValueError("Preserve frame pointer must be true or false.")
    if not re.search(rf"\b{re.escape(symbol)}\s*\(", source):
        raise ValueError(f"C source must define {symbol}.")

    reference_path, retail_size, _ = function_reference(symbol)
    compiled, error = compiler_explorer.compile_function(
        source, optimization, symbol, retail_size, preserve_frame_pointer
    )
    if error:
        return None, error
    retail_start = int(symbol[5:], 16)
    retail_bytes = Elf32LE(RETAIL).bytes_at_vaddr(retail_start, retail_size)
    retail_instructions = function_assembly.disassemble(
        RETAIL, retail_start, retail_size
    )
    compiled_start = compiled["value"]
    compiled_size = compiled["size"]
    compiled_bytes = compiled["bytes"]
    compiled_instructions = compiled["instructions"]
    comparisons = []
    matching_bytes = sum(
        expected == actual
        for expected, actual in zip(retail_bytes, compiled_bytes)
    )
    for offset in range(0, max(retail_size, compiled_size), 4):
        retail_instruction = retail_instructions.get(retail_start + offset)
        compiled_instruction = compiled_instructions.get(compiled_start + offset)
        retail_word = retail_bytes[offset:offset + 4]
        compiled_word = compiled_bytes[offset:offset + 4]
        matches = len(retail_word) == 4 and retail_word == compiled_word
        comparisons.append(
            {
                "offset": f"{offset:04X}",
                "retail": retail_instruction[1] if retail_instruction else "",
                "compiled": compiled_instruction[1] if compiled_instruction else "",
                "retail_bytes": retail_word.hex(" ").upper(),
                "compiled_bytes": compiled_word.hex(" ").upper(),
                "matches": matches,
            }
        )
    status_text = (
        "MATCH"
        if compiled_start == retail_start
        and compiled_size == retail_size
        and compiled_bytes == retail_bytes
        else "DIFF"
    )
    return {
        "reference_assembly": reference_path.relative_to(ROOT).as_posix(),
        "instruction_count": len(compiled_instructions),
        "matching_lines": sum(item["matches"] for item in comparisons),
        "matching_bytes": matching_bytes,
        "retail_size": retail_size,
        "compiled_size": compiled_size,
        "retail_address": retail_start,
        "compiled_address": compiled_start,
        "status": status_text,
        "compared_lines": len(comparisons),
        "comparisons": comparisons,
    }, None


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
        route = urlparse(self.path)
        if route.path == "/":
            try:
                body = progress.HTML_REPORT.read_bytes()
            except OSError as error:
                self.send_json(503, {"error": f"Could not read dashboard report: {error}"})
                return
            self.send_response(200)
            self.send_header("Content-Type", "text/html; charset=utf-8")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)
            return
        if route.path == "/api/ready":
            self.send_json(200, {"ready": True})
            return
        if route.path == "/api/draft":
            symbol = parse_qs(route.query).get("symbol", [""])[0]
            try:
                source, saved, signature_inferred, stale = load_draft(symbol)
            except (OSError, UnicodeError, ValueError, RuntimeError) as error:
                self.send_json(400, {"error": str(error)})
                return
            self.send_json(
                200,
                {
                    "source": source,
                    "saved": saved,
                    "signature_inferred": signature_inferred,
                    "stale": stale,
                },
            )
            return
        if route.path == "/api/profile":
            symbol = parse_qs(route.query).get("symbol", [""])[0]
            try:
                profile, saved = load_profile(symbol)
            except (OSError, UnicodeError, ValueError, RuntimeError) as error:
                self.send_json(400, {"error": str(error)})
                return
            self.send_json(200, {"profile": profile, "saved": saved})
            return
        if route.path == "/api/assembly":
            symbol = parse_qs(route.query).get("symbol", [""])[0]
            try:
                self.send_json(200, assembly_comparison(symbol))
            except (OSError, UnicodeError, ValueError, RuntimeError) as error:
                self.send_json(400, {"error": str(error)})
            return
        self.send_error(404)

    def do_POST(self):
        route = urlparse(self.path)
        if route.path not in ("/api/compile", "/api/draft", "/api/profile"):
            self.send_error(404)
            return
        try:
            content_length = int(self.headers.get("Content-Length", ""))
        except ValueError:
            self.send_json(400, {"error": "A valid Content-Length header is required."})
            return
        if content_length <= 0 or content_length > MAX_REQUEST_BYTES:
            self.send_json(413, {"error": "Request is empty or too large."})
            return
        try:
            request = json.loads(self.rfile.read(content_length))
        except (UnicodeDecodeError, json.JSONDecodeError):
            self.send_json(400, {"error": "Request body must be valid JSON."})
            return
        if not isinstance(request, dict):
            self.send_json(400, {"error": "Request body must be a JSON object."})
            return
        if route.path == "/api/draft":
            try:
                save_draft(request.get("symbol"), request.get("source"))
            except (UnicodeError, ValueError) as error:
                self.send_json(400, {"error": str(error)})
                return
            except OSError as error:
                self.send_json(500, {"error": f"Could not save playground draft: {error}"})
                return
            self.send_json(200, {"saved": True})
            return
        if route.path == "/api/profile":
            try:
                save_profile(request.get("symbol"), request.get("profile"))
            except (UnicodeError, ValueError) as error:
                self.send_json(400, {"error": str(error)})
                return
            except OSError as error:
                self.send_json(500, {"error": f"Could not save compiler profile: {error}"})
                return
            self.send_json(200, {"saved": True})
            return
        try:
            result, error = compile_function(request)
        except subprocess.TimeoutExpired as error:
            is_link_timeout = (
                isinstance(error.cmd, (list, tuple))
                and error.cmd
                and Path(error.cmd[0]).resolve()
                == compiler_explorer.project_build.LINKER.resolve()
            )
            step = "linking" if is_link_timeout else "compilation"
            timeout = (
                compiler_explorer.LINK_TIMEOUT_SECONDS
                if is_link_timeout
                else compiler_explorer.COMPILE_TIMEOUT_SECONDS
            )
            self.send_json(
                504,
                {"error": f"Playground {step} exceeded {timeout} seconds."},
            )
            return
        except (UnicodeError, ValueError) as error:
            self.send_json(400, {"error": str(error)})
            return
        except OSError as error:
            self.send_json(500, {"error": f"Could not run the EE compiler: {error}"})
            return
        except RuntimeError as error:
            self.send_json(500, {"error": str(error)})
            return
        if error:
            self.send_json(422, {"error": error})
            return
        self.send_json(200, result)

    def log_message(self, format_string, *args):
        return


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", type=int, default=8765, help="local website port (default: 8765)")
    parser.add_argument(
        "--no-browser", action="store_true", help="do not open the website automatically"
    )
    arguments = parser.parse_args()
    if not 0 <= arguments.port <= 65535:
        parser.error("port must be between 0 and 65535")
    if not progress.HTML_REPORT.is_file():
        parser.error(
            f"dashboard report not found: {progress.HTML_REPORT}; "
            "run tools\\status.py after building first"
        )
    if not compiler_explorer.COMPILER.is_file():
        parser.error(f"EE compiler not found: {compiler_explorer.COMPILER}")
    try:
        server = ThreadingHTTPServer(("127.0.0.1", arguments.port), RequestHandler)
    except OSError as error:
        parser.error(f"could not start local website: {error}")
    url = f"http://127.0.0.1:{server.server_port}/"
    print(f"Rayman Revolution dashboard: {url}")
    print("Press Ctrl+C to stop.")
    if not arguments.no_browser:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopping dashboard.")
    finally:
        server.server_close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
