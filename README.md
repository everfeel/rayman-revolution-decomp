I genuinely believe this game deserves to be preserved and given the opportunity to shine again someday. Through this decompilation project, I hope to contribute to its preservation and help pave the way for a potential future PC port.

**Yes, this project uses AI.** However, so far, I've only used the free tier of GitHub Copilot to help set up the decompilation environment and work on matching some of the functions you currently see in the project.

I have no plans to expand this work through other paid AI subscriptions, as I don't really use them. I'll continue making use of free AI tools whenever they're available, while also manually implementing/matching functions myself whenever I have the time.

This project originally started with the goal of achieving a byte-matching decompilation. However, given how tedious that process can be, my current priority will be to implement anyway the functions that I can't byte-match. There's always time to work toward byte-matching them later. Of course, for any non-matching implementations I will still test if the game works as intended. If you contribute implementations that aren't byte-matching, please make sure to test the game as well.

**Contributions are always welcome, in any form!**

----

# Rayman Revolution (PAL) decompilation project

<!-- BEGIN GENERATED PROGRESS -->
![Progress: 878 byte-matches, 3 implemented, 4,764 not implemented](progress/progress.svg)

**Byte-match:** 878 · **Implemented:** 3 · **Not implemented:** 4,764.
<!-- END GENERATED PROGRESS -->

This is a decompilation of the PAL PlayStation 2 main executable, `SLES_500.44`. The primary goal is to understand and express the original game's behavior in readable C. Matching the original machine code is a valuable secondary goal.

The project builds an ELF at the original executable layout so behavior-oriented implementations can also be compared with the authorized reference. The repository contains source code, tools, configuration, and progress reports; it does not provide the game executable, disc image, extracted game assets, or proprietary SN Systems toolchain.

## Project goals

1. **Understand and preserve behavior.** Analyze functions in context, recover meaningful names and types when evidence supports them, and write maintainable C that expresses what the original code does.
2. **Validate implementations.** Use callers, data layout, assembly, and runtime observations when available. Clearly distinguish facts from hypotheses.
3. **Pursue byte matching where practical.** Exact output is useful evidence and helps characterize the original compiler, but do not sacrifice clear, correct behavior merely to force a match.

The progress tracker uses three simple states: **Byte-match** for exact byte matches, **Implemented** for C or edited assembly that does not byte-match, and **Not implemented** for functions still using the retail assembly fallback. Byte-different implementations are not considered broken just because they differ.

The progress bar shows byte-matched functions in cyan, implemented-but-not-byte-matched functions in purple, and functions still using the retail assembly fallback in gray. The categories are separate and add up to the total.

## Contributing

Contributions are welcome, including analysis, function implementations, data types, documentation, and tooling. Before starting, check the source and dashboard for existing work on the function. Keep changes focused and explain the evidence behind recovered behavior.

### Implementing a function

1. Find the function in `src\` by its retail-address symbol, such as `func_00100130`. Check its callers, nearby functions, global data, and reference assembly under the generated `asm\` directory. Ghidra can help investigate control flow and data use.
2. Replace its `INCLUDE_ASM` fallback with a C definition in the appropriate source module. Preserve the symbol name. Infer signatures and types from call sites and instruction behavior; use `arg1`, `arg2`, or `unk1` style names where intent is not known rather than inventing semantics.
3. Prefer a readable behavioral implementation. Keep uncertain details explicit and conservative. Do not edit generated reference files under repository-root `asm\`.
4. Build and inspect the function comparison. A byte difference is expected during decompilation; use the differing instructions to guide further analysis, not as an automatic rejection.
5. In a pull request or contribution description, summarize what the function appears to do, the evidence used, any assumptions, and the checks you ran. State whether byte matching was achieved, but do not present it as the only measure of correctness.

### Build and compare

Commands below run from the project root in PowerShell. Use the local virtual environment after setup:

```powershell
.\.venv\Scripts\python.exe tools\build.py
.\.venv\Scripts\python.exe tools\compare.py function build\SLES_500.44.elf --symbol func_00100130 --size 0x58
```

Replace the example symbol and size with the target function's values from its reference assembly. `MATCH` means the linked function's address, size, and bytes match retail exactly. A `DIFF` is a comparison result, not a behavior verdict. Add `--full` to show every instruction word:

```powershell
.\.venv\Scripts\python.exe tools\compare.py function build\SLES_500.44.elf --symbol func_00100130 --size 0x58 --full
```

To compare the whole main image:

```powershell
.\.venv\Scripts\python.exe tools\compare.py image build\SLES_500.44.elf
```

Whole-image comparison covers the main load image, not DVP overlays. C functions smaller than their retail size are padded with trailing nops to retain later addresses; larger functions are not truncated or padded.

## Local dashboard and playground

Start the combined progress, function-assembly, and C playground page:

```powershell
.\.venv\Scripts\python.exe tools\dashboard.py
```

Open `http://127.0.0.1:8765/`. The server binds to localhost; press `Ctrl+C` to stop it. Use `--port` to choose another port or `--no-browser` to suppress automatic browser launch.

- **Progress** groups functions as **Byte-match**, **Implemented**, or **Not implemented**. A byte-different implementation is not treated as incorrect.
- **Assembly comparison** shows retail instructions and the current build side by side. Unimplemented fallback assembly is marked unverified; identical fallback bytes do not count as a C implementation.
- **C playground** compiles an isolated edit and links it at the selected function's project address, then compares it with retail. It reports an exact match only when address, size, and every byte agree. Compiler/linker errors and stale build inputs are reported explicitly.

Playground edits and per-function compiler settings are saved locally in the ignored `progress\.playground\` directory; they do not edit project sources or build flags. The playground uses the current module layout and built project symbols. It needs a successful project build and generated progress report. Restart the dashboard after changing its Python tools.

The playground keeps its compiler controls, including **Optimization**, **Preserve frame pointer**, and **Compiler arguments**. Additional flags accept shell-style arguments; quote values containing spaces. Playground settings are per function and do not alter project build settings. The standalone compiler page is available with:

```powershell
.\.venv\Scripts\python.exe tools\compiler_explorer.py
```

The standalone compiler explorer defaults to port `8765`; the single-function live assembly view is available with `tools\function_assembly.py --web` and defaults to `8766`.

## Compiler and module settings

The supported build environment is Windows, Python 3, and an authorized SN Systems ProDG PS2 EE toolchain using EE GCC `2.9-ee-991111b/r4`. The current common build flags are `-G4`, `-Iinclude`, `-Isrc`, and assembler include paths. Do not assume modern GCC documentation describes this legacy compiler exactly.

`src\modules.txt` controls object link order and C compilation arguments. Preserve the existing retail address order when editing the manifest; moving modules can shift code and affect later addresses. Each C-file line can have compiler arguments, for example:

```text
main.c -O2
some_module.c -O1 -fno-omit-frame-pointer -include "header file.h"
asm/some_function.s
```

Arguments apply to the entire C file, so separate functions into different modules if they genuinely need different compiler options. C files default to `-O2`. Assembly entries do not receive C compiler arguments.

The optional `@global` line in `src\modules.txt` defines C arguments to use for all C files when `USE_GLOBAL_COMPILER_FLAGS = True` is selected in `tools\build.py`. It is `False` by default, so per-file arguments are active. Prefer the existing per-file mode unless a project-wide change is intentional and has been checked against existing implementations.

For detailed options supported by this installed compiler, see [docs/ee-compiler-options.txt](docs/ee-compiler-options.txt). The playground is useful for isolated compiler experiments, but a playground match does not automatically change the project build settings.

## Setup

The reference executable, SDK, generated disassembly/assets, and build products are local inputs and must not be committed. Only use game files and development tools you are authorized to possess and use under applicable law and their licenses. Do not commit or share game files, extracted content, or proprietary SDK files.

### Requirements

- Windows for the supported SN Systems ProDG PS2 EE toolchain.
- Python 3 and the Windows `py` launcher.
- A legally obtained SN Systems ProDG PS2 EE toolchain with EE GCC `2.9-ee-991111b/r4`; required compiler and linker names are `ee-gcc.exe` and `ee-ld.exe`.
- An authorized PAL `SLES_500.44` executable matching the revision checked by the setup script.
- Python packages listed in `requirements.txt`.
- Ghidra is optional and is not required by the build scripts.

### Initial setup

1. Create a local Python environment and install dependencies:

   ```powershell
   py -3 -m venv .venv
   .\.venv\Scripts\python.exe -m pip install --upgrade pip
   .\.venv\Scripts\python.exe -m pip install -r requirements.txt
   ```

2. Place the authorized PAL executable here, with no extension:

   ```text
   baserom\SLES_500.44
   ```

   The setup script checks its SHA-1 against the expected PAL revision.

3. Install or copy your legally obtained SDK locally so these paths exist:

   ```text
   toolchain\sn-prodg-24\local\sce\ee\gcc\bin\ee-gcc.exe
   toolchain\sn-prodg-24\local\sce\ee\gcc\bin\ee-ld.exe
   ```

   The SDK is not included in this repository or installed by the Python requirements.

4. Generate the local disassembly and linker inputs:

   ```powershell
   .\.venv\Scripts\python.exe tools\setup_asm.py
   ```

   Generated assembly and assets are local inputs. Review source changes after regeneration.

5. Build and compare:

   ```powershell
   .\.venv\Scripts\python.exe tools\build.py
   .\.venv\Scripts\python.exe tools\compare.py image build\SLES_500.44.elf
   ```

## Progress reports

The tracker assigns each function one status:

- **Byte-match** — the built function matches the retail address, size, and bytes.
- **Implemented** — C or edited assembly replaces the retail fallback, but the result does not byte-match. This status does not mean the implementation behaves incorrectly.
- **Not implemented** — the function still uses its retail assembly fallback.

After a successful build, the project refreshes `progress\main_status.html`, `progress\functions.csv`, `progress\progress.svg`, and the generated progress block in this README. The dashboard has one progress bar and matching status filters. The CSV contains each function's status and byte-comparison details.

The local dashboard adds assembly comparison and playground features. The static HTML report is useful for browsing the inventory; GitHub displays its source rather than running its interactive features. The `progress\functions.csv` file provides the same per-function information for scripts.

To regenerate reports from an existing build without rebuilding:

```powershell
.\.venv\Scripts\python.exe tools\status.py build\SLES_500.44.elf
```

## Repository layout

- `src\` — project C and hand-written assembly. `src\modules.txt` controls link order and per-file C flags.
- `include\` — shared C and assembler headers.
- `config\splat.yaml` — Splat configuration for the PAL main executable.
- `tools\` — setup, build, comparison, progress, dashboard, playground, and live assembly-view scripts.
- `docs\` — project reference documentation, including compiler options.
- `requirements.txt` — Python dependencies for disassembly and report generation.
- `baserom\` — local authorized reference executable; ignored by Git.
- `asm\` and `assets\` — generated local reference material; ignored by Git.
- `toolchain\` — local proprietary SDK; ignored by Git.
- `build\` — generated ELF, objects, linker inputs, and logs; ignored by Git.
- `progress\` — generated reports; local baseline and playground drafts are ignored.

Edit project C under `src\` and hand-written assembly under `src\asm\`. Treat repository-root `asm\` as generated reference input, not project source. Do not edit generated files under `build\`. Run `tools\build.py` after setup or source regeneration so the dashboard has current objects.

## Troubleshooting

- **Missing reference executable or SHA-1 mismatch:** confirm the authorized PAL revision and exact path.
- **Pinned disassembly-tool version error:** use the project virtual environment and reinstall `requirements.txt`.
- **Missing compiler or linker:** check the local SDK paths and required toolchain version.
- **Missing `asm\data` or linker inputs:** run `tools\setup_asm.py` successfully before building.
- **Dashboard reports stale build inputs:** run `tools\build.py`, then refresh the page.
- **Compiler or linker errors:** inspect `build\build.log`; for playground-only failures, read the reported compiler/linker output and check unresolved symbols and declarations.
