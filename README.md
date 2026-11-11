I genuinely believe this game deserves to be preserved and to shine new light again someday. Through this decompilation project, I hope to contribute to its preservation and help pave the way for a future PC port.

YES, this project uses AI. However, so far, I've only used the free tier of GitHub Copilot to help set up the decompilation environment and handle the currently matching functions that you see.

I have no personal plans to expand this work using other AI subscription services, as I don't really use any of them. I'll continue using free AI models and tools when available and manually decompile and match functions myself using Ghidra whenever I have the time.

Contributions are always welcome, in any form!

----

# Rayman Revolution (PAL) decompilation project

<!-- BEGIN GENERATED PROGRESS -->
![Progress: 15.48% complete, 874 matching, 0 not matching, 4,771 not started](progress/progress.svg)

**15.48% complete** (874 / 5,645 functions). **Matching:** 874 · **Not matching:** 0 · **Not started:** 4,771.
<!-- END GENERATED PROGRESS -->

This project is a work-in-progress matching decompilation of the PAL PlayStation 2 main executable, `SLES_500.44`. Generated MIPS disassembly serves as the reference while functions are replaced with C or hand-written assembly. The project build links an ELF using the original layout so whole-image and per-function output can be compared with the authorized reference executable.

The repository contains source code, tools, configuration, and generated progress reports. It does not provide the game executable, disc image, extracted game assets, or proprietary SN Systems toolchain. The reference executable, SDK, generated disassembly/assets, and build products are local inputs and must not be committed.

## Legal

- This project aims to produce source implementations whose compiled code matches the original game, as a preservation effort.
- This repository distributes project tooling and matched source code only. It does not include the game, a playable build, the retail executable, disc images, extracted assets, or other game files.
- Only use game files and development tools that you are authorized to possess and use under applicable law and their licenses. Do not commit or share game files, extracted content, or proprietary SDK files.

## Requirements

- Windows for the supported SN Systems ProDG PS2 EE toolchain.
- Python 3 and the Windows `py` launcher.
- A legally obtained SN Systems ProDG PS2 EE toolchain with EE GCC version `2.9-ee-991111b/r4`; its compiler and linker must be named `ee-gcc.exe` and `ee-ld.exe`.
- An authorized PAL `SLES_500.44` executable that matches the version checked by the setup script.
- Optionally, Ghidra for interactive disassembly and analysis. It is not required by the build scripts; keep its distribution local and out of Git.

The Python dependencies are listed in `requirements.txt`; the disassembly-tool versions are checked by the setup script. Keep the SDK and game files local; neither is installed by the Python setup.

## Initial setup

Run the following commands in PowerShell from the project root.

1. Create a local Python environment and install the project dependencies:

   ```powershell
   py -3 -m venv .venv
   .\.venv\Scripts\python.exe -m pip install --upgrade pip
   .\.venv\Scripts\python.exe -m pip install -r requirements.txt
   ```

2. Place your authorized PAL executable here, with no extension:

   ```text
   baserom\SLES_500.44
   ```

   The setup script checks the file's SHA-1 against the expected PAL revision. A different revision is not supported by this configuration.

3. Install or copy your legally obtained SN Systems SDK into the project folder so these files exist:

   ```text
   toolchain\sn-prodg-24\local\sce\ee\gcc\bin\ee-gcc.exe
   toolchain\sn-prodg-24\local\sce\ee\gcc\bin\ee-ld.exe
   ```

   These paths are relative to the project root. The compiler and linker are not included in this repository or the Python requirements. Do not copy SDK files into a public repository.

   The Windows SDK compiler is the default C compiler frontend and is also used to assemble C compiler output and project `.s` files.
   The required compiler version is EE GCC `2.9-ee-991111b/r4`.

4. Generate the local disassembly and linker inputs:

   ```powershell
   .\.venv\Scripts\python.exe tools\setup_asm.py
   ```

   This verifies the executable and pinned disassembly-tool versions, extracts the main load image into `build\SLES_500.44.rom`, and runs Splat to generate assembly and asset inputs. Generated `asm\` and `assets\` content is ignored by Git. Setup can regenerate generated sources and assembly; it preserves the hand-written entry-point padding in `src\main.c`, but review any source changes before rerunning it.

5. Build and compare the main image:

   ```powershell
   .\.venv\Scripts\python.exe tools\build.py
   .\.venv\Scripts\python.exe tools\compare.py image build\SLES_500.44.elf
   ```

The build produces `build\SLES_500.44.elf`, logs diagnostics to `build\build.log`, and refreshes the function progress reports. C functions smaller than their retail size are padded with trailing nops so following functions retain their expected addresses; larger functions are not truncated or padded. The linker also prevents `.lit4` from moving earlier than its retail start when preceding sections shrink, reducing avoidable GP-relative range errors. Oversized functions can still shift later addresses, and genuine GP-range or section-overlap errors can still occur. Once setup is complete, the first successful baseline build should match the configured main image. Whole-image comparison covers the main load image, not DVP overlays.

The project build and C playground use the Windows SDK EE compiler for C compilation, assembly, and linking. Both explicitly enable assembler instruction reordering after `macro.inc` sets `.set noreorder`, so C-generated delay slots are scheduled consistently in the build and playground.

## Working on a function

Functions are organized in `src\functions.c` in retail address order, with the entry-point code and padding in `src\main.c`. Hand-written assembly modules live under `src\asm\`; generated Splat assembly under the repository-root `asm\` directory is local reference input. The large C translation unit keeps adjacent functions in one link object. Generated names such as `func_00100130` identify a function by its retail address; they do not imply what it does. Search the source tree for the function name and check call sites and assembly before deciding its signature or behavior.

### Implement it in C

Find its `INCLUDE_ASM` declaration, for example:

```c
INCLUDE_ASM(const s32, "core/main", func_00100130);
```

Replace only that declaration with a C definition of the same symbol. Determine the real return type, parameters, and calling convention from the reference assembly and callers rather than guessing from the generated name. Leave functions that are not yet implemented in C as reference assembly.

### Build and compare

After implementing the function in C, rebuild and compare it using its retail size from the reference assembly:

```powershell
.\.venv\Scripts\python.exe tools\build.py
.\.venv\Scripts\python.exe tools\compare.py function build\SLES_500.44.elf --symbol func_00100130 --size 0x58
```

Use the function's actual symbol and size in place of the example values. `MATCH` means the compiled C implementation has the same bytes and size as retail. A mismatch is expected while decompiling; the report identifies differing bytes. Add `--full` to show every 4-byte MIPS instruction word, including matching words and trailing partial bytes:

```powershell
.\.venv\Scripts\python.exe tools\compare.py function build\SLES_500.44.elf --symbol func_00100130 --size 0x58 --full
```

The whole-image comparison only reports `MATCH` when all included code and data bytes match the reference. Function comparison checks the selected function's retail range; use the progress report or dashboard to see implementation status and matching-byte counts across the project.

### Unified local dashboard

Start the combined progress, function-assembly, and C playground page:

```powershell
.\.venv\Scripts\python.exe tools\dashboard.py
```

The dashboard opens at `http://127.0.0.1:8765/` and binds to localhost only. The Progress tab filters and searches the generated function report. Assembly comparison shows retail instructions and bytes beside the current project build. The C playground resolves functions from the current `src\modules.txt` layout, including `main.c`, C modules, and standalone assembly modules. It compiles the selected C implementation with the chosen optimization and frame-pointer settings, links it at its current project address against the built project symbols, and compares the linked result with retail. It reports `MATCH` only when the linked address, size, and every byte match. Missing or stale build inputs, unresolved symbols, and compiler/linker failures are reported as errors rather than approximate matches.

The notice above the playground result describes whether the selected function matches retail in the latest project build. The result below it applies only to the current playground source; editing in the playground does not change project files or the build profile. When the project has a C definition, the starter uses that function's declared return type and parameters; otherwise, verify the placeholder signature yourself. Drafts and per-function compiler settings are saved in the ignored `progress\.playground\` directory. If a saved draft no longer defines the selected function after source files are reorganized, the playground loads a fresh starter and keeps the old draft on disk. Restart the dashboard after changing its Python tools. It needs the SDK compiler, a successful project build, and the generated `progress\main_status.html` report. Press `Ctrl+C` to stop it; use `--port` to select another port or `--no-browser` to suppress automatic browser launch.

The playground links only the selected function at its address in the project ELF and resolves project calls and globals against their built addresses. This keeps symbol addresses stable when trying a different optimization level. Use the **Other flags** field for additional space-separated compiler options; these flags are saved with the other per-function settings.

Optimization, **Preserve frame pointer**, and **Other flags** settings are saved per function in the ignored `progress\.playground\` directory. They apply only to playground compiles and do not change project build flags. For example, `func_00100188` matches retail with an `int` function that returns `func_00220568(D_00252100, main_BSS_START)` at `-O1` with frame-pointer preservation enabled. Treat a playground match as a candidate profile; it does not automatically update project build settings.

### Per-file project compiler settings

`src\modules.txt` determines the order of project C and assembly objects at link time. Keep entries in retail address order: moving a module can shift code and affect later function addresses. C entries can include an optimization level and optionally `-fno-omit-frame-pointer`:

```text
main.c -O2
functions.c -O2
some_module.c -O1 -fno-omit-frame-pointer
asm/some_function.s
```

The optimization level defaults to `-O2`; frame pointers are omitted unless `-fno-omit-frame-pointer` is specified. These settings apply to the entire C file, not an individual function. Use separate translation units only when needed, and place them in the correct address order. Assembly modules do not take C compiler flags. After changing the manifest, rebuild and verify the affected function addresses and their neighbors.

For a single function, `function_assembly.py` can print a retail-versus-build assembly comparison in the terminal:

```powershell
.\.venv\Scripts\python.exe tools\function_assembly.py func_00100130
```

Add `--web` to open a live local comparison page that refreshes after builds. The standalone compiler explorer is separate: it compiles independent C snippets to assembly and does not compare them with a retail function.

```powershell
.\.venv\Scripts\python.exe tools\compiler_explorer.py
```

The dashboard, standalone compiler, and single-function live view accept `--port` and `--no-browser`. The dashboard and compiler explorer default to port `8765`; the single-function view defaults to `8766`.

## Progress reports

The generated visual summary, static dashboard, per-function CSV, and README progress counts are tracked and refreshed after each successful build:

- [Generated dashboard report](progress/main_status.html) — searchable and filterable as a static report; run `tools\dashboard.py` for its assembly and live-playground features. GitHub displays the HTML source rather than running it.
- [Per-function CSV](progress/functions.csv).
- The SVG summary above and this README's progress counts.

The tracker scans C sources under `src\` and detects edited function assembly against a local source baseline. Untouched extracted retail assembly is `NOT STARTED`, even when its bytes match the reference. A C implementation or edited function assembly is considered an implementation:

- `NOT STARTED` — no project implementation detected.
- `NOT MATCHING` — an implementation exists, but its built bytes or address differ.
- `MATCHING` — an implementation exists and its built bytes match.

The dashboard percentage counts only `MATCHING` functions. It reports the retail function size and matching bytes as `matching / retail size`. The entry-point `nop` at `0x00100000` is padding, not a function, and is excluded. The local baseline is created automatically on the first report run; do not edit `progress\.function_source_baseline.json`.

To regenerate reports from an existing ELF without rebuilding:

```powershell
.\.venv\Scripts\python.exe tools\status.py build\SLES_500.44.elf
```

## Important files and generated paths

- `src\` — project C and hand-written assembly sources. `src\modules.txt` controls object link order and per-C-file compiler settings.
- `config\splat.yaml` — Splat configuration for the PAL main executable.
- `include\` — shared C and assembler headers.
- `tools\` — setup, build, comparison, progress, dashboard, playground, and live assembly-view scripts.
- `requirements.txt` — Python dependencies for disassembly and report generation.
- `baserom\` — local, authorized reference executable; ignored by Git.
- `src\asm\` — hand-written assembly source modules listed in `src\modules.txt`; these are project sources and should be committed.
- `asm\` — generated Splat output and local reference material. `matchings\` and `nonmatchings\` are Splat output categories, not necessarily unique copies of every function; for function lookup, the project uses `nonmatchings\` as its canonical inventory. `data\` contains generated assembly inputs used by the build. Ignored by Git.
- `assets\` — extracted data generated by Splat; ignored by Git.
- `toolchain\` — local proprietary SDK; ignored by Git.
- `tools\ghidra_12.2_PUBLIC\` — optional local Ghidra distribution; ignored by Git.
- `build\` — generated ELF, objects, linker inputs, and logs; ignored by Git. The build objects are used by the dashboard playground and should not be removed while using it.
- `progress\` — generated reports tracked by Git; the local source baseline, playground drafts, and extracted report assets are ignored.

Edit project C source under `src\` and hand-written function assembly under `src\asm\`; treat repository-root `asm\` as generated reference input, not project source. Do not edit generated files under `build\`. Run `setup_asm.py` only when intentionally regenerating the local disassembly and linker inputs, and review assembly changes afterward. Always run `tools\build.py` after setup or source regeneration: regenerated assembly makes the existing build objects stale, and the dashboard correctly refuses to link against them. Do not delete the ignored reference executable, SDK, generated `asm\`/`assets\`, or build objects if you plan to build or use the dashboard.

## Troubleshooting

- **Missing `baserom\SLES_500.44` or SHA-1 mismatch:** confirm the file is the authorized PAL executable revision supported by this project and is placed at the exact path above.
- **Pinned disassembly-tool version error:** invoke the project virtual environment's Python as shown, then reinstall `requirements.txt`.
- **Missing `ee-gcc.exe` or `ee-ld.exe`:** check that your legally obtained SDK is installed at the project-relative `toolchain\sn-prodg-24\...` paths.
- **Unexpected compiler version:** use the required EE GCC version `2.9-ee-991111b/r4`.
- **`asm\data` or linker inputs missing:** run `tools\setup_asm.py` successfully before building.
- **Multiple reference assemblies found:** update to the latest `function_assembly.py`; it now prefers the project's canonical `asm\nonmatchings\` inventory when both Splat output categories contain a function.
- **Dashboard reports unavailable or stale build inputs:** run `tools\build.py` successfully, then restart or refresh the dashboard.
- **Compiler or linker errors:** inspect `build\build.log` and confirm the SDK is the expected PS2 EE toolchain.
