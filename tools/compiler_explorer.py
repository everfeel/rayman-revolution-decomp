#!/usr/bin/env python3
"""Run a local browser-based C-to-assembly viewer with the project EE compiler."""

import argparse
import json
import re
import shlex
import subprocess
import tempfile
import webbrowser
from http.server import BaseHTTPRequestHandler, HTTPServer
from pathlib import Path
from urllib.parse import urlparse

import build as project_build
from compare import Elf32LE, RETAIL
from function_assembly import OBJDUMP, disassemble

ROOT = Path(__file__).resolve().parents[1]
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
MAX_SOURCE_BYTES = 128 * 1024
MAX_COMPILER_FLAGS_LENGTH = 1024
COMPILE_TIMEOUT_SECONDS = 20
LINK_TIMEOUT_SECONDS = 60
COMMON_FLAGS = ["-G4", "-Iinclude", "-Isrc", "-Wa,-I,include,-I,."] 
OBJCOPY = COMPILER.with_name("ee-objcopy.exe")
ASSEMBLY_SECTION = re.compile(
    r"^\s*(?:\.section\s+(\S+)|\.(text|rdata|rodata|data|sdata|bss|sbss|lit4)\b)"
)
GPREL16_TRUNCATED = re.compile(
    r"relocation truncated to fit:\s*R_MIPS_GPREL16\s+([A-Za-z_.$][A-Za-z0-9_.$]*)"
)

PAGE = r"""<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>EE Compiler Explorer</title>
<style>
:root { color-scheme: dark; font: 15px/1.5 system-ui, sans-serif; background: #111827; color: #e5e7eb; }
* { box-sizing: border-box; }
body { margin: 0 auto; padding: 24px; max-width: 1440px; }
h1 { margin: 0; font-size: 1.4rem; }
p { color: #9ca3af; margin: 5px 0 18px; }
.bar { display: flex; align-items: center; gap: 12px; margin-bottom: 12px; }
label { color: #cbd5e1; }
select, button { border: 1px solid #475569; border-radius: 5px; background: #1f2937; color: #e5e7eb; padding: 8px 12px; }
button { cursor: pointer; background: #2563eb; border-color: #2563eb; font-weight: 600; }
button:disabled { opacity: .6; cursor: wait; }
#status { color: #9ca3af; }
.panes { display: grid; grid-template-columns: 1fr 1fr; gap: 14px; }
section { min-width: 0; }
h2 { font-size: .9rem; font-weight: 600; color: #cbd5e1; }
textarea, pre { width: 100%; height: min(72vh, 760px); margin: 0; border: 1px solid #374151; border-radius: 6px; background: #030712; color: #d1d5db; padding: 14px; font: 13px/1.55 ui-monospace, SFMono-Regular, Consolas, monospace; tab-size: 4; }
textarea { resize: vertical; outline-color: #3b82f6; }
pre { overflow: auto; white-space: pre; }
@media (max-width: 800px) { .panes { grid-template-columns: 1fr; } textarea, pre { height: 45vh; } }
</style>
</head>
<body>
<h1>EE Compiler Explorer</h1>
<p>Compile a standalone C snippet with this project's SN Systems PlayStation 2 EE compiler.</p>
<div class="bar">
  <label for="optimization">Optimization</label>
  <select id="optimization"><option>-O0</option><option>-O1</option><option selected>-O2</option><option>-O3</option></select>
  <button id="compile">Compile</button>
  <button id="copy" type="button">Copy assembly</button>
  <span id="status" role="status">Ready</span>
</div>
<div class="panes">
  <section><h2>C source</h2><textarea id="source" spellcheck="false">int add(int a, int b)
{
    return a + b;
}</textarea></section>
  <section><h2>EE assembly</h2><pre id="assembly">Click Compile to generate assembly.</pre></section>
</div>
<script>
const source = document.querySelector("#source");
const assembly = document.querySelector("#assembly");
const status = document.querySelector("#status");
const compileButton = document.querySelector("#compile");

compileButton.addEventListener("click", async () => {
  compileButton.disabled = true;
  status.textContent = "Compiling…";
  try {
    const response = await fetch("/compile", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        source: source.value,
        optimization: document.querySelector("#optimization").value
      })
    });
    const result = await response.json();
    if (!response.ok) throw new Error(result.error || "Compilation failed.");
    assembly.textContent = result.assembly;
    status.textContent = "Compiled";
  } catch (error) {
    assembly.textContent = error.message;
    status.textContent = "Error";
  } finally {
    compileButton.disabled = false;
  }
});

document.querySelector("#copy").addEventListener("click", async () => {
  try {
    await navigator.clipboard.writeText(assembly.textContent);
    status.textContent = "Assembly copied";
  } catch (error) {
    status.textContent = `Copy failed: ${error.message}`;
  }
});
</script>
</body>
</html>
"""


def compile_source(source, optimization):
    with tempfile.TemporaryDirectory(prefix="ee-compiler-") as temporary_directory:
        temporary_path = Path(temporary_directory)
        input_path = temporary_path / "snippet.c"
        output_path = temporary_path / "snippet.s"
        input_path.write_text(source, encoding="utf-8", newline="\n")

        result = subprocess.run(
            [
                str(COMPILER),
                optimization,
                *COMMON_FLAGS,
                "-S",
                str(input_path),
                "-o",
                str(output_path),
            ],
            cwd=ROOT,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            errors="replace",
            timeout=COMPILE_TIMEOUT_SECONDS,
            check=False,
        )
        if result.returncode:
            return None, result.stdout.strip() or (
                f"Compiler exited with status {result.returncode}."
            )
        if not output_path.is_file():
            return None, "Compiler succeeded but did not produce assembly."
        return output_path.read_text(encoding="utf-8", errors="replace"), None


SOURCE_FUNCTION = re.compile(
    r"(?m)^[ \t]*(?:(?:[A-Za-z_][A-Za-z0-9_]*|\*)[ \t*]+)*"
    r"\b(func_[0-9A-Fa-f]{8})\s*\([^;{}]*\)"
    r"(?:\s|/\*[\s\S]*?\*/|//[^\r\n]*)*\{"
)


def function_definition_span(source, symbol):
    match = function_definition_match(source, symbol)
    if not match:
        return None

    start = source.rfind("\n", 0, match.start()) + 1
    brace_start = match.end() - 1
    depth = 0
    index = brace_start
    state = "code"
    while index < len(source):
        char = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""
        if state == "line_comment":
            if char == "\n":
                state = "code"
        elif state == "block_comment":
            if char == "*" and following == "/":
                state = "code"
                index += 1
        elif state in ("string", "character"):
            if char == "\\":
                index += 1
            elif (state == "string" and char == '"') or (
                state == "character" and char == "'"
            ):
                state = "code"
        elif char == "/" and following == "/":
            state = "line_comment"
            index += 1
        elif char == "/" and following == "*":
            state = "block_comment"
            index += 1
        elif char == '"':
            state = "string"
        elif char == "'":
            state = "character"
        elif char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return start, index + 1
        index += 1
    raise ValueError(f"C source has an unterminated definition for {symbol}.")


def function_definition_match(source, symbol):
    pattern = re.compile(
        rf"\b{re.escape(symbol)}\s*\([^;{{}}]*\)"
        r"(?:\s|/\*[\s\S]*?\*/|//[^\r\n]*)*\{"
    )
    return pattern.search(source)


def function_signature(source, symbol):
    match = function_definition_match(source, symbol)
    if not match:
        return None

    declaration_start = source.rfind("\n", 0, match.start()) + 1
    while declaration_start:
        previous_end = declaration_start - 1
        previous_start = source.rfind("\n", 0, previous_end) + 1
        previous_line = source[previous_start:previous_end].strip()
        if not previous_line or previous_line.startswith("#"):
            break
        declaration_start = previous_start

    signature = source[declaration_start:match.end() - 1].strip()
    return signature or None


def assembly_section_name(line):
    match = ASSEMBLY_SECTION.match(line)
    if not match:
        return None
    return match.group(1) or f".{match.group(2)}"


def module_source_for(symbol):
    assembly_pattern = re.compile(
        rf"(?m)^[ \t]*INCLUDE_ASM\s*\([^;]*?\b{re.escape(symbol)}\s*\)\s*;"
    )
    assembly_definition = re.compile(
        rf"(?m)^[ \t]*glabel\s+{re.escape(symbol)}\s*$"
    )
    source_modules = project_build.source_modules()
    for module in source_modules:
        source_path = module.path
        source = source_path.read_text(encoding="utf-8")
        if (
            function_definition_span(source, symbol)
            or assembly_pattern.search(source)
            or inline_assembly_span(source, symbol)
        ):
            return source_path, source
        if source_path.suffix == ".s" and assembly_definition.search(source):
            return (
                source_path.with_suffix(".c"),
                '#include "common.h"\n',
            )
    raise ValueError(f"Could not find the project C module for {symbol}.")


def inline_assembly_span(source, symbol):
    pattern = re.compile(
        rf"(?ms)^[ \t]*__asm__\s*\("
        rf"(?:(?!^[ \t]*\);)[\s\S])*?glabel\s+{re.escape(symbol)}\\n"
        rf"(?:(?!^[ \t]*\);)[\s\S])*?^[ \t]*\);"
    )
    match = pattern.search(source)
    return (match.start(), match.end()) if match else None


def playground_module_source(module_source, submitted_source, symbol):
    submitted_span = function_definition_span(submitted_source, symbol)
    if not submitted_span:
        raise ValueError(f"C source must define {symbol}.")
    other_functions = {
        match.group(1)
        for match in SOURCE_FUNCTION.finditer(submitted_source)
        if match.group(1) != symbol
    }
    if other_functions:
        replacement = submitted_source[submitted_span[0]:submitted_span[1]]
    else:
        replacement = submitted_source.strip()

    module_span = function_definition_span(module_source, symbol)
    if module_span:
        return (
            module_source[:module_span[0]]
            + replacement
            + module_source[module_span[1]:]
        )

    assembly_pattern = re.compile(
        rf"(?m)^[ \t]*INCLUDE_ASM\s*\([^;]*?\b{re.escape(symbol)}\s*\)\s*;"
    )
    matches = list(assembly_pattern.finditer(module_source))
    if len(matches) == 1:
        match = matches[0]
        return module_source[:match.start()] + replacement + module_source[match.end():]
    inline_span = inline_assembly_span(module_source, symbol)
    if inline_span:
        return (
            module_source[:inline_span[0]]
            + replacement
            + module_source[inline_span[1]:]
        )
    return module_source.rstrip() + "\n\n" + replacement + "\n"


def project_link_snapshots():
    if not project_build.LINKER_SCRIPT.is_file():
        raise FileNotFoundError(
            f"Linker script missing: {project_build.LINKER_SCRIPT}; run tools\\setup_asm.py."
        )
    undefined_symbols = ROOT / "undefined_syms_auto.txt"
    if not undefined_symbols.is_file():
        raise FileNotFoundError(
            "undefined_syms_auto.txt is missing; run tools\\setup_asm.py."
        )
    if not project_build.OUTPUT.is_file():
        raise FileNotFoundError(
            f"Project ELF missing: {project_build.OUTPUT}; run tools\\build.py."
        )

    input_pairs = []
    modules = project_build.source_modules()
    for module in modules:
        source_path = module.path
        relative = source_path.relative_to(ROOT)
        object_path = project_build.BUILD / relative.parent / f"{source_path.name}.o"
        input_pairs.append((source_path, object_path))

    data_root = ROOT / "asm" / "data"
    if not data_root.is_dir():
        raise FileNotFoundError(f"Data assembly directory missing: {data_root}.")
    data_paths = sorted(data_root.rglob("*.s"))
    for source_path in data_paths:
        relative = source_path.relative_to(ROOT)
        object_path = project_build.BUILD / relative.parent / f"{source_path.name}.o"
        input_pairs.append((source_path, object_path))

    newest_input = 0
    snapshots = []
    for source_path, object_path in input_pairs:
        if not source_path.is_file() or not object_path.is_file():
            raise FileNotFoundError(
                f"Link input missing: {source_path} or {object_path}; run tools\\build.py."
            )
        source_stat = source_path.stat()
        object_stat = object_path.stat()
        if object_stat.st_mtime_ns < source_stat.st_mtime_ns:
            raise ValueError(
                f"Project object is older than its source: {object_path}; run tools\\build.py."
            )
        newest_input = max(newest_input, object_stat.st_mtime_ns)
        snapshots.append((source_path, source_stat.st_mtime_ns, source_stat.st_size))
        snapshots.append((object_path, object_stat.st_mtime_ns, object_stat.st_size))

    for path in (
        project_build.LINKER_SCRIPT,
        undefined_symbols,
        project_build.MODULE_MANIFEST,
    ):
        stat = path.stat()
        snapshots.append((path, stat.st_mtime_ns, stat.st_size))

    elf_stat = project_build.OUTPUT.stat()
    if elf_stat.st_mtime_ns < newest_input:
        raise ValueError(
            f"Project ELF is older than its link inputs: {project_build.OUTPUT}; "
            "run tools\\build.py."
        )
    if elf_stat.st_mtime_ns < max(
        project_build.LINKER_SCRIPT.stat().st_mtime_ns,
        undefined_symbols.stat().st_mtime_ns,
        project_build.MODULE_MANIFEST.stat().st_mtime_ns,
    ):
        raise ValueError(
            f"Project ELF predates linker inputs; run tools\\build.py."
        )
    snapshots.append((project_build.OUTPUT, elf_stat.st_mtime_ns, elf_stat.st_size))

    return snapshots


def parse_compiler_flags(flags):
    if not isinstance(flags, str):
        raise ValueError("Additional compiler flags must be text.")
    if len(flags) > MAX_COMPILER_FLAGS_LENGTH:
        raise ValueError(
            f"Additional compiler flags must be {MAX_COMPILER_FLAGS_LENGTH} "
            "characters or shorter."
        )
    try:
        parsed_flags = shlex.split(flags)
    except ValueError as error:
        raise ValueError(f"Could not parse additional compiler flags: {error}") from error
    if any(not flag.startswith("-") or flag == "-" for flag in parsed_flags):
        raise ValueError(
            "Additional compiler flags must be options; use attached values "
            "such as -DNAME or -Ipath."
        )
    controlled_flags = {
        "-c",
        "-S",
        "-E",
        "-M",
        "-MM",
        "-MD",
        "-MMD",
        "-MF",
        "-MT",
        "-MQ",
        "-x",
        "-o",
    }
    if any(
        flag in controlled_flags
        or flag.startswith(("-o", "-MF", "-MT", "-MQ"))
        for flag in parsed_flags
    ):
        raise ValueError(
            "Additional flags cannot override compiler output or input modes."
        )
    return parsed_flags


def compile_function(
    source,
    optimization,
    symbol,
    retail_size,
    preserve_frame_pointer=False,
    other_flags="",
):
    module_path, module_source = module_source_for(symbol)
    linked_source = playground_module_source(module_source, source, symbol)
    snapshots = project_link_snapshots()
    project_elf = Elf32LE(project_build.OUTPUT)
    project_function = project_elf.symbol(symbol)
    if project_function["type"] != 2 or project_function["value"] <= 0:
        return None, f"Project ELF has no linked function {symbol}."
    compiler_flags = (
        ["-fno-omit-frame-pointer"] if preserve_frame_pointer else []
    )
    additional_flags = parse_compiler_flags(other_flags)

    with tempfile.TemporaryDirectory(prefix="ee-playground-") as temporary_directory:
        temporary_path = Path(temporary_directory)
        source_path = temporary_path / module_path.name
        assembly_path = temporary_path / module_path.with_suffix(".s").name
        object_path = temporary_path / module_path.with_suffix(".o").name
        output_path = temporary_path / "playground.elf"
        linker_script = temporary_path / "playground.ld"
        symbol_map_path = temporary_path / "project-symbols.elf"
        source_path.write_text(
            f'#line 1 "{module_path.relative_to(ROOT).as_posix()}"\n{linked_source}',
            encoding="utf-8",
            newline="\n",
        )

        result = subprocess.run(
            [
                str(COMPILER),
                optimization,
                *COMMON_FLAGS,
                *compiler_flags,
                *additional_flags,
                "-ffunction-sections",
                "-fdata-sections",
                "-S",
                str(source_path),
                "-o",
                str(assembly_path),
            ],
            cwd=ROOT,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            errors="replace",
            timeout=COMPILE_TIMEOUT_SECONDS,
            check=False,
        )
        if result.returncode:
            return None, result.stdout.strip() or (
                f"Compiler exited with status {result.returncode}."
            )
        if not assembly_path.is_file():
            return None, "Compiler succeeded but did not produce assembly."
        assembly_lines = assembly_path.read_text(encoding="utf-8").splitlines()
        section_start = next(
            (
                index
                for index, line in enumerate(assembly_lines)
                if re.match(
                    rf'^\s*\.section\s+\.text\.{re.escape(symbol)}(?:,|\s|$)',
                    line,
                )
            ),
            None,
        )
        if section_start is None:
            return None, f"Compiler did not emit an isolated section for {symbol}."
        function_end = next(
            (
                index
                for index in range(section_start + 1, len(assembly_lines))
                if re.match(
                    rf"^\s*\.end\s+{re.escape(symbol)}\s*$",
                    assembly_lines[index],
                )
            ),
            None,
        )
        if function_end is None:
            return None, f"Compiler emitted an incomplete section for {symbol}."
        function_assembly = ["\t.set reorder"]
        function_assembly.extend(
            line for line in assembly_lines if re.match(r"^\s*\.extern\b", line)
        )
        index = 0
        while index < len(assembly_lines):
            section_name = assembly_section_name(assembly_lines[index])
            if not section_name or not section_name.startswith(
                (".rdata", ".rodata", ".lit4")
            ):
                index += 1
                continue
            section_end = next(
                (
                    end
                    for end in range(index + 1, len(assembly_lines))
                    if assembly_section_name(assembly_lines[end])
                ),
                len(assembly_lines),
            )
            function_assembly.extend(assembly_lines[index:section_end])
            index = section_end
        function_assembly.extend(assembly_lines[section_start:function_end + 1])
        function_assembly.append(f"\t.size {symbol}, .-{symbol}")
        assembly_path.write_text(
            "\n".join(function_assembly) + "\n",
            encoding="utf-8",
            newline="\n",
        )

        result = subprocess.run(
            [
                str(COMPILER),
                optimization,
                *COMMON_FLAGS,
                *compiler_flags,
                *additional_flags,
                "-c",
                str(assembly_path),
                "-o",
                str(object_path),
            ],
            cwd=ROOT,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            errors="replace",
            timeout=COMPILE_TIMEOUT_SECONDS,
            check=False,
        )
        if result.returncode:
            return None, result.stdout.strip() or (
                f"Assembler exited with status {result.returncode}."
            )
        if not object_path.is_file():
            return None, "Assembler succeeded but did not produce an object."

        try:
            retail_sizes = project_build.retail_function_sizes()
            recorded_size = retail_sizes.get(symbol)
            if recorded_size is not None and recorded_size != retail_size:
                raise ValueError(
                    f"{symbol}: retail size differs between assembly references "
                    f"({recorded_size:#x} and {retail_size:#x})."
                )
            retail_sizes[symbol] = retail_size
            padded_assembly, _, _ = project_build.pad_undersized_functions(
                assembly_path, object_path, retail_sizes
            )
            if padded_assembly != assembly_path.read_text(encoding="utf-8"):
                assembly_path.write_text(
                    padded_assembly, encoding="utf-8", newline="\n"
                )
                result = subprocess.run(
                    [
                        str(COMPILER),
                        optimization,
                        *COMMON_FLAGS,
                        *compiler_flags,
                        *additional_flags,
                        "-c",
                        str(assembly_path),
                        "-o",
                        str(object_path),
                    ],
                    cwd=ROOT,
                    text=True,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    errors="replace",
                    timeout=COMPILE_TIMEOUT_SECONDS,
                    check=False,
                )
                if result.returncode:
                    return None, result.stdout.strip() or (
                        f"Assembler exited with status {result.returncode}."
                    )

        except (OSError, UnicodeError, ValueError) as error:
            if f"{symbol} has no nonempty function symbol" in str(error):
                return None, (
                    f"The compiler emitted no code for {symbol}. Add a non-empty "
                    "function implementation and verify its signature."
                )
            return None, str(error)

        result = subprocess.run(
            [
                str(OBJCOPY),
                "-N",
                symbol,
                str(project_build.OUTPUT),
                str(symbol_map_path),
            ],
            cwd=ROOT,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            errors="replace",
            timeout=LINK_TIMEOUT_SECONDS,
            check=False,
        )
        if result.returncode:
            return None, result.stdout.strip() or (
                "Could not prepare fixed project symbol addresses."
            )
        linker_script.write_text(
            f"""SECTIONS
{{
    . = {project_function["value"]:#x};
    .text : {{ *(.text.{symbol}) }}
    .rodata : {{ *(.rodata*) *(.lit4*) }}
}}
""",
            encoding="ascii",
            newline="\n",
        )
        result = subprocess.run(
            [
                str(project_build.LINKER),
                "-e",
                symbol,
                "--gc-sections",
                f"--just-symbols={symbol_map_path}",
                "-T",
                str(linker_script),
                "-o",
                str(output_path),
                str(object_path),
            ],
            cwd=ROOT,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            errors="replace",
            timeout=LINK_TIMEOUT_SECONDS,
            check=False,
        )
        if result.returncode:
            detail = "\n".join(result.stdout.strip().splitlines()[-30:])
            gprel_match = GPREL16_TRUNCATED.search(result.stdout)
            if gprel_match:
                symbol = gprel_match.group(1)
                return None, (
                    f"GP-relative relocation for {symbol} is outside the signed "
                    "16-bit range from _gp. The linker assigns the symbol's "
                    "address but cannot make this 16-bit relocation reach it. "
                    f"If you meant {symbol}'s address, declare it as an array or "
                    "pointer and pass that address (for example, `extern int "
                    f"{symbol}[];` and use `{symbol}`); `(int){symbol}` reads "
                    "the stored value, not its address. Linker boundary symbols "
                    "should likewise be declared as arrays or used by address."
                    + (f"\n{detail}" if detail else "")
                )
            return None, (
                "Could not link the playground function with project symbols."
                + (f"\n{detail}" if detail else "")
            )
        if not output_path.is_file():
            return None, "Linker succeeded but did not produce an ELF."
        if any(
            not path.is_file()
            or path.stat().st_mtime_ns != mtime_ns
            or path.stat().st_size != size
            for path, mtime_ns, size in snapshots
        ):
            return None, "Project link inputs changed during comparison; retry."

        linked_elf = Elf32LE(output_path)
        function = linked_elf.symbol(symbol)
        if function["type"] != 2 or function["size"] <= 0:
            raise ValueError(f"Linked ELF has no nonempty function {symbol}.")
        if function["value"] != project_function["value"]:
            raise ValueError(
                f"Playground linked {symbol} at {function['value']:#x}, expected "
                f"its project address {project_function['value']:#x}."
            )
        function["bytes"] = linked_elf.bytes_at_vaddr(
            function["value"], function["size"]
        )
        function["instructions"] = disassemble(
            output_path, function["value"], function["size"]
        )
        return function, None


class RequestHandler(BaseHTTPRequestHandler):
    def send_json(self, status, payload):
        body = json.dumps(payload).encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        if urlparse(self.path).path != "/":
            self.send_error(404)
            return
        body = PAGE.encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_POST(self):
        if urlparse(self.path).path != "/compile":
            self.send_error(404)
            return
        try:
            content_length = int(self.headers.get("Content-Length", ""))
        except ValueError:
            self.send_json(400, {"error": "A valid Content-Length header is required."})
            return
        if content_length < 0 or content_length > MAX_SOURCE_BYTES + 1024:
            self.send_json(413, {"error": "Request is too large."})
            return
        try:
            request = json.loads(self.rfile.read(content_length))
        except (UnicodeDecodeError, json.JSONDecodeError):
            self.send_json(400, {"error": "Request body must be valid JSON."})
            return
        if not isinstance(request, dict) or not isinstance(request.get("source"), str):
            self.send_json(400, {"error": "Request must include source text."})
            return
        source = request["source"]
        try:
            source_size = len(source.encode("utf-8"))
        except UnicodeEncodeError:
            self.send_json(400, {"error": "Source must contain valid Unicode text."})
            return
        if source_size > MAX_SOURCE_BYTES:
            self.send_json(413, {"error": "Source must be 128 KiB or smaller."})
            return
        optimization = request.get("optimization", "-O2")
        if optimization not in ("-O0", "-O1", "-O2", "-O3"):
            self.send_json(400, {"error": "Unsupported optimization level."})
            return
        try:
            assembly, error = compile_source(source, optimization)
        except subprocess.TimeoutExpired:
            self.send_json(
                504,
                {"error": f"Compilation exceeded {COMPILE_TIMEOUT_SECONDS} seconds."},
            )
            return
        except OSError as error:
            self.send_json(500, {"error": f"Could not run the EE compiler: {error}"})
            return
        if error:
            self.send_json(422, {"error": error})
            return
        self.send_json(200, {"assembly": assembly})


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", type=int, default=8765, help="local HTTP port (default: 8765)")
    parser.add_argument(
        "--no-browser", action="store_true", help="do not open the page automatically"
    )
    arguments = parser.parse_args()

    if not COMPILER.is_file():
        parser.error(f"EE compiler not found: {COMPILER}")
    if not 0 <= arguments.port <= 65535:
        parser.error("port must be between 0 and 65535")

    try:
        server = HTTPServer(("127.0.0.1", arguments.port), RequestHandler)
    except OSError as error:
        parser.error(f"could not start local server: {error}")
    url = f"http://127.0.0.1:{server.server_port}/"
    print(f"EE Compiler Explorer: {url}")
    print("Press Ctrl+C to stop.")
    if not arguments.no_browser:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopping EE Compiler Explorer.")
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
