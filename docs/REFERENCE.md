# REFERENCE — PS2 byte-matching, central source of truth

> **This is the authoritative standing reference. Read it in full before doing
> any reverse-engineering or byte-matching work.** It is the single file that
> accumulates every confirmed discovery. When a new reusable pattern or
> confirmed fact is found, add it here in the right section. Never silently
> overwrite existing knowledge.

Objective: **byte-perfect matching** of reconstructed C against the original
`SLES_500.44` machine code. Behavioral equivalence is not the goal. Priority
order, highest first:

**original binary bytes > original assembly > Ghidra pseudocode > assumptions**

Use `tools/` for existing utilities (do not reinvent). Use `export/` (Ghidra
pseudocode and exports) as interpretation only — assembly and bytes are
authoritative.

**Working rules (never violated):**
- Byte matching only, not behavioral equivalence.
- **No inline assembly** unless syscalls, VU microcode, or ops the compiler cannot
  generate (R5900 FPU accumulator ops `mula.s`/`madda.s`/`madd.s`, `sqrt.s`).
  No register bindings (`register x asm("$2")`) — user forbids them.
- Never assume logically equivalent C generates identical assembly.
- Investigate systematically; controlled changes; verify with binary comparison.
- Never claim a match without `compare.py` / `sweep_matches.py` evidence.
- Do not refactor/simplify/optimize unless it helps the exact match.
- Every confirmed discovery goes into this file.

---

## 1. Build facts that decide codegen

| What | Value |
|---|---|
| compiler | `toolchain/sn-prodg-24/local/sce/ee/gcc/bin/ee-gcc.exe` = `2.9-ee-991111b/r4` |
| common flags | `-G4 -Iinclude -Isrc -Wa,-I,include,-I,.` (`tools/build.py` `common_flags`) |
| per-module flags | `src/modules.txt` (e.g. `funcs_11_...c -O2 -ffast-math`, `funcs_04_...c -Os -fno-caller-saves -fno-omit-frame-pointer`) |
| default optimization | `-O2` when a module names none |

**Pipeline matters more than anything else:** `build.py` does NOT run `gcc -c`.
It compiles to assembly (`-S`), rewrites that assembly (assembler reordering,
out-of-range externals made absolute, padding of undersized functions), then
assembles and links. A candidate compiled with plain `-c` can differ or fail.
`tools/try_func.py` calls `build.py`'s own `compile_c_source`, so use it.

`ee-gcc295.exe` (2.95.2) and `ee-gcc2953.exe` (2.95.3) exist in the toolchain
but **retail is 2.9-ee** — proven (see §9). Never use the others for the build.

---

## 2. Pattern → C construct (the lookup table)

This is the heart of the file. Retail instruction pattern on the left, the C
construct that produces it on the right, with the function that proved it.
**Consult this table before writing or changing a candidate.**

### Address arithmetic / operand order

| Retail pattern | C construct | Evidence |
|---|---|---|
| `addu $3,$3,$2` where you get `addu $2,$2,$3` | split the indirect access into named locals: `s32 i = p[10]; s32 *q = p + i; return *q;` | `func_00196060` |
| base-first vs index-first `addu` | type the table: `extern Rec T[]; T[i].field;` = base-first; `extern char T[]; p = T + i*STRIDE;` = index-first; `table[idx]` = base-first; `void **b = table + idx; *b` = index-first | `idioms.md` |
| copy survives an increment | `t = g; p = (int*)t; t += 0x10; g = t;` (a plain `p = g; g += 0x10;` folds the copy away) | `idioms.md` |
| 8-byte unaligned copy `ldl/ldr + sdl/sdr` | `typedef struct { u32 a,b; } Pair; dst = src;` (struct assignment), NOT byte copies (`swl/swr`) | `func_001D0A58`, `func_001E70F0` |
| 16-byte aligned `lq/sq` | aligned type: `typedef float V[4] __attribute__((aligned(16)))` | `matching-reference.md` |
| `sq $zero,0(p)` | inline asm is the only exact form; plain C emits `por` first | `matching-reference.md` |

### Structs, types, offsets

| Retail shape | C form |
|---|---|
| `lbu`/`sb` on fields | `u8` members |
| `lhu`/`sh` on fields | `s16`/`u16` members |
| `lw`/`sw` on fields | `s32`/`u32` members |
| `lwc1`/`swc1` | `f32` members |
| `andi $x,$y,0xFF` on a parameter | source truncated a wider argument into a `u8` local of its own |
| field offset | read the immediate off the load/store; keep gaps `char padNN[len]` with offset comments |
| float literal | `f`-suffixed constant (`.float` → `swc1` of a pool entry); a double emits two words |

Declare structs from the widths the assembly actually loads/stores. Cast the
**value**, not the field's address: `x = (s32)h->field;` NOT
`*(s32 *)&h->pad_x` — an address cast folds the offset into `%hi(D_sym+off)`
and changes code.

### Aggregate copy: whole record vs per-statement

Copying a 12-byte record **as one aggregate assignment** keeps the loads together
and the stores together, which is what retail does:

```c
typedef struct { UnalignedU64 v; u32 w; } S12;   /* 8 unaligned + 4 aligned */
*(S12 *)dst = *(S12 *)(src + 0x48);
```

→ `ldl $2,0x4F($4)` / `ldr $2,0x48($4)` / `lw $3,0x50($4)` / `sdl $2,7($5)` /
`sdr $2,0($5)` / `sw $3,8($5)` — 32/32 EXACT (`func_00145F60`, `func_00145F80`).

The same code written as two statements (`*(UnalignedU64 *)dst = ...;` then
`*(u32 *)(dst + 8) = ...;`) emits the same instruction *set* but the scheduler
interleaves load/store and pushes the word store into the `jr` delay slot
(20/32). Also wrong: a fully `packed` 12-byte struct (trailing word degrades to
`lwl/lwr`), a single `packed { u64; u32; }` (whole copy goes byte-wise, `sb`), and
hoisting both loads into locals (`UnalignedU64 v = ...; u32 w = ...;`) which
spills a frame (52 B). The split that works is a **packed 8-byte member plus an
aligned 4-byte member** in one aggregate.

The rule generalises to 16 bytes with `typedef struct { UnalignedU64 lo, hi; }
Unaligned16;` — one aggregate copy gives `ldl/ldr/ldl/ldr` then
`sdl/sdr/sdl/sdr` (`func_00119518`, 44/48; two statements gave 14/48).

**Known wall:** GCC does not copy the *pointers* into fresh registers. Retail's
`func_00119518` starts `daddu $3,$4,$0` / `daddu $2,$5,$0` (dst and src copied)
before the four loads; every source shape tried keeps `$4`/`$5` direct and is
one instruction short (44/48). Tried: aggregate copy, pointer locals, `void *` /
`u32` params, struct member access, pointer-to-struct locals, inlined static
helper, u64 locals, array subscripts — all 44/48 or worse.

### Branches / delay slots

| Retail pattern | C construct | Evidence |
|---|---|---|
| `beql reg,$0` with delay slot zeroing the SAME reg (`daddu $2,$0,$0`) | tested value must be a **function result in $2** and the zero the **return value** (also $2): `v = call(...); if (v != 0) return <compute>; return 0;` / `__builtin_expect(v != 0, 0)` | `func_00115978`, `func_00113760`, `func_0014CE30` |
| `beql $4,$0` (argument) + `daddu $2,$0,$0` — **UNSOLVED** | no matched func tests `$4` with `beql`; `__builtin_expect` on an `$4` arg zeroes `$a0`. Sole residual of `func_00108648` (39/40). See §9. | `func_00108648` |
| non-likely branch (`beqz`/`bnez`/`bc1f`) whose delay slot must always run | put the delay statement BEFORE the conditional | `func_00135CF8` |
| likely branch (`beql`/`bnel`/`bc1fl`) | put the delay statement INSIDE the taken arm only | `idioms.md` |
| `sltiu` vs `slti` | unsigned vs signed compare | `matching-reference.md` |
| two `return x;` sites that should stay separate | assign in each arm, `return` once — GCC cross-jumps duplicate returns | `near_match_techniques.md` |
| `bnezl` merging two call sites differing only in `$7` | not a bug; GCC merged them, behaviourally identical | `func_00131D78` |

### Scheduling / statement order

- Scheduler ties go to the source's **last** store first; a twice-stored value has its first store last. If store order is wrong, permute statements; if no permutation works, the source shape is wrong.
- `addu` operand order follows source: `base + i*4` puts index first, `i*4 + base` keeps base first.
  Precise form, confirmed 44/44: the **left** operand of `+` is the `rs` of the
  `addu`. Retail `addu $2,$2,$4` (rs = the `idx << 2` register, rt = the table
  base) needs `(idx << 2) + base`; writing `base + (idx << 2)` gives the reversed
  `addu` and is the *only* difference in `func_0022C278` (44/44 vs 38/44).
- For loops, the order of the three `for(...)` updates decides which lands in the delay slot.
- Copies that survive CSE: read a value twice (test then assign), `n = x++;`, re-read a global at each use, assign a pointer in the loop condition.
- Return shape: one `return` with value set per arm, or `if (x) return 1;` per arm plus shared `return 0;` — failure path last.
- A tail written once in retail is often in **both** source arms; extra references change who keeps a saved register.
- A guarded loop branches on the **exit** polarity with a productive delay slot: write `if (count > 0) { body }` + trailing `return 1;`, not `if (count <= 0) return 1;` - the body's first statement then fills the `blez` slot (`func_00146850`).
- `for (j = 0; j < n; j++)` with `n` already known non-zero gets its **first iteration peeled**: the direct `arg1[0]` probe under `beql` is the peeled loop body, not a hand-written check. Starting at `j = 1` instead makes gcc rotate the loop (extra guard + copied body) (`func_00146850`).
- A constant compared against a call result lives in the loop preheader; materialisation order follows the source, so put it in a local (`s32 type = 6;`) declared *before* the next initialiser (`func_00146850`).

### 16.16 loop counters

`func_00146850` keeps its outer counter as a fixed-point value: `pos = 0x10000`,
`pos += 0x10000`, test `(pos >> 16) < n`. The compare reads the value from
**before** the increment, so the source needs an explicit copy:

```c
cur = pos; pos += 0x10000;   /* ... */
} while ((cur >> 16) < count);
```

Testing the incremented value shifts the trip count by one element, and a `for`
whose condition is tested at the top emits a leading guard that retail does not
have (the loop must run at least once).

### Assembler macros and listing forms — not diffs

The retail `asm/` files are *disassembly* and the build's `.s` is *assembler
text*; these pairs are the same 32-bit word and must not be chased as
differences (normalise first — `build/try/hist.py` does):

| retail listing | generated listing | note |
|---|---|---|
| `slti $2,$2,0x1389` | `slt $2,$2,5001` | GAS folds an in-range immediate `slt` into `slti` |
| `sltiu $3,$3,0x1` | `sltu $3,$3,1` | same |
| `daddu $17,$0,$0` | `move $17,$0` | `move` is a `daddu` macro |
| `addiu $29,$29,-0x80` | `subu $sp,$sp,128` | `subu` macro |
| `negu $2,$2` | `subu $2,$0,$2` | `negu` is a `subu` macro |
| `bnez` / `beqz` | `bne`/`beq` with `$0` | macro |
| `dsll32 $3,$4,0` | `dsll $4,$4,32` | `dsll32` encodes sa+32 — same word |
| `dsra32 $3,$3,0` | `dsra $3,$3,32` | same |
| `lui $20,(0x380000 >> 16)` | `lui $22,%hi(D_0037951C)` | **same word**, different printing |

That last row matters: GAS `%hi(sym)` is `(sym + 0x8000) >> 16`, so
`D_0037951C = 0x0037951C` gives immediate `0x38`; the disassembler prints it as
`(0x380000 >> 16)` when it cannot pair the `lui` with a symbolised `%lo`. Same
for `D_00378D88`. Do **not** read it as "retail used a constant base".

### cc1 2.9-ee deletes a loop whose condition holds a 64-bit volatile load

```c
while ((status = *(volatile s64 *)0x10002000) < 0) { body; }   /* loop GONE */
```

cc1 removes the entire poll loop (23 instructions, 92 B) and keeps a single
read, so the function comes out 344 B instead of 436 B. A 32-bit volatile load in
the same position (`*(volatile u32 *)...`) is compiled normally. Write the test
twice, which also gives retail's rotated shape:

```c
status = *(volatile s64 *)ADDR;
while (status < 0) { body; status = *(volatile s64 *)ADDR; }
```

(`for (;;) { status = *ADDR; if (status >= 0) break; body; }` is equivalent.)
Check the object size whenever a volatile 64-bit load drives a loop.

### Un-rotated `while` — the `goto` form

GCC 2.9-ee turns `while (p) { body }` into the rotated `if (p) do { body }
while (p)` shape: bottom test, no top guard, +12 B. Retail sometimes keeps the
**unrotated** CFG instead — test at the top, unconditional `b` back edge whose
delay slot carries the induction copy:

```c
    i = head;
test:
    if (i == 0)
        return;
    body;
    i = next;
    goto test;
```

emits retail's `beqz $k, out` / body / `b test` / `daddu $k, $t, $0`
instruction-for-instruction (`func_0015BBF0`; `for`/`while` with the identical
body rotated and was 12 B larger). Use it when retail shows `beqz` + `b` to the
same test label.

### Modulo: the divide check and the guard collapse

```c
u32 v = *p;
u32 rem = v % arg1;              /* divisor not provably non-zero */
if (rem != 0) *p = v + (arg1 - rem);
```

emits the annulled trap `beql $5,$0,out` + `break 0,7` (traps only on the zero
path), `divu $0,$3,$5` + `mfhi $2`, then `beqz $2` with `subu $2,$5,$2` scheduled
into the delay slot and `addu` + `sw` in the arm — 44/44 EXACT
(`func_00157E30`). Operand types are **unsigned**: `s32 %` gives `div` and a
different guard.

### `$gp` vs `lui`, `-G4`

With `-G4`, 1–4 byte globals use `$gp`; larger use `lui`/`%lo`. Retail shows the form:
- `lw $2, %gp_rel(D_x)($gp)` → small data
- `lui $2,%hi(D_x)` + `lw $2,%lo(D_x)($2)` → absolute, one register
- the same pair in two registers → plain absolute reference

One small declaration changes the form for the WHOLE file — re-run `tools/file_check.py` after touching one.

### Forcing a re-read (defeat CSE)

In order of preference:
1. `static inline` accessor called at each use
2. re-read the global rather than caching in a local
3. `volatile` on the specific member (also keeps that store out of delay slots)
4. memory barrier `__asm__ volatile("" ::: "memory")` — last resort, but the only thing that worked for `func_00222468` (88/88)

`volatile` on the whole object blows size (104 B vs 88 B) — use per field, never per object.

### Calls / returns

- `$v0` vs `$v1` after a call tells whether the callee returns a value.
- A callee that never returns is `__attribute__((noreturn))`.
- Changing a callee's return type reorders the CALLER (`func_00225E58` grew 28→32 B). Re-check callers after any signature fix.
- A callee needs a no-prototype declaration `T f();` when a matching wrapper calls it with no args.
- GCC 2.9-ee emits **void tail-calls** (`j`) where retail often has `jal` + epilogue — blocker for `func_0021E720`, `func_002261D8`.

### Register / allocation levers

- Declaration order affects spill-slot assignment; the prologue tells you retail's order.
- Reusing a temporary vs a fresh one changes live ranges — the most common cause of "same instructions, different registers".
- `regalloc.py` explains why a variable landed where it did — use it after 5–8 failed attempts, not before.
- `(s32)(p-b)>>4` gives `sra`; the `u32` version gives `srl`.

---

## 3. Known walls — stop and report

- COP2 / VU0 (`cfc2`, `ctc2`, `qmfc2`, `qmtc2`, `vcallms`) — no C form here.
- `cvt.w.s` sequences (`func_001C6418`, `func_001C60F8`, `func_001C63D8`, `func_00102A00`).
- Three separate `div.s` with two `nop` slots each (`func_00126578`, `func_0012664C`): `1.0f/x` is CSE'd into one division; no reload shape tried changes it.
- Jump-table bodies need `jtbl_*` labels (`func_00189220`, `func_00126510`, `func_00213958`).
- 128-bit zero store at non-zero offset.
- void tail-calls (above).
- Function-pointer wrappers (`func_0021B5A8` family): GCC emits `subu`/`jalr` where retail has `addiu`/`lui+addiu`.

---

## 4. Tool map

Use existing tools; do not reinvent. Run from repo root.

| Task | Tool |
|---|---|
| candidate in its real file + verdict | `tools/try_func.py FUNC cand.c [--diff]` |
| why a variable landed in a register | `tools/regalloc.py FUNC cand.c` |
| whole file after a landing | `tools/file_check.py` |
| which words differ across build | `tools/diff_words.py [--func X] [--worst]` |
| everything to start a function | `tools/dossier.py FUNC` |
| pick next function | `tools/rank_candidates.py` |
| near-miss queue | `tools/nonmatching.py` |
| land a passing candidate | `tools/apply_candidate.py` |
| brute-force statement order (≤8 stmts) | `tools/permute.py FUNC cand.c` / `tools/perm_search.py` |
| **verify ALL claims on size+bytes + freshness** | `tools/sweep_matches.py` |
| compile-only gate for src/ | `tools/check_syntax.py` |
| guards | `tools/check_docs.py`, `tools/check_no_retail.py`, `tools/check_regressions.py` |
| **look up a pattern in this knowledge** | `tools/pattern_lookup.py TERM [TERM...]` |

Fixed 2026-11-18: `tools/try_func.py` resolved relocations on big-endian words.
Every `jal`/`j`/`la` it could resolve came out byte-swapped and was reported as
a false byte difference (a 304 B candidate showed 4 phantom diff words). It now
reads and writes the instruction word little-endian.

---

## 5. Workflow

1. `python tools/dossier.py func_X` — read assembly, neighbours, callers.
2. Declare the struct from observed widths; keep gaps.
3. Decide global-addressing form before writing statements.
4. Check `docs/REFERENCE.md` §2 for the pattern you see (or `pattern_lookup.py`).
5. Write plain C; test with `try_func.py` (or `build/try/h.py` for a bare snippet).
6. Only registers differ → `regalloc.py`, change refcounts/lifetimes.
7. Sizes differ → the shape is wrong (duplicated/missing load, operand order), not allocation.
8. Land, full build, read `progress/functions.csv`, revert anything not `MATCHING`, then `file_check.py` + `check_regressions.py` + `sweep_matches.py`.

**Never claim a match without `compare.py` / `sweep_matches.py` byte evidence.**
`try_func` reports unresolved `%hi/%lo` relocations as byte diffs ("BYTES 4/36" can be pure relocation) — always confirm with a full build before believing it.

---

## 6. Verified log — confirmed in this tree

| Tried | Result |
|---|---|
| `__asm__ volatile("" ::: "memory")` before last store, `func_00222468` | **88/88** — only form that worked |
| plain C natural store order, same | 88 B but self-store hoisted to +0x3C |
| plain C fully reversed | self-store first — worse |
| `volatile` whole object | 104 B — rejected |
| `Pair{u32,u32}` assignment, `func_001DF128` | retail `ldl/ldr + sdl/sdr` at 44/44 B |
| `u8 v = arg1;` after header load, `func_00214300` | 48/48 B, only base register differs |
| `try_func` vs shipped match `func_00222468` | EXACT (8 words masked) — tool agrees with build |
| `func_00196060` split pointer into named locals | **36/36 EXACT** |
| `func_001ABBC0` busy-wait `while(*(volatile s32 *)&D) {}` | **36/36 EXACT** |
| `func_00220C68` / `func_00221828` 4-store writers | **36/36 EXACT** (twin stores b/c in opposite slots — read real asm) |
| `volatile` import for `func_001C6EE8` | 7/9 words; volatile store never reaches `jr;sw` delay slot |
| 16.16 do-while counter + `s32 type = 6` local, `func_00146850` | **304/304 EXACT** - first match above 256 B |
| `volatile` MMIO + `s64` status, `func_001D1E50` | retail's whole shape reached (0x80 frame, 7 saves + ra, 107-instruction multiset); 296/436 bytes differ = register permutation only, see §9 |
| `while ((x = *(volatile s64 *)A) < 0)` | loop silently deleted, 344 B — never write a 64-bit volatile poll this way |
| block-scope no-prototype decl for a zero-arg call to a prototyped callee, `func_0022E200` | **36/36 EXACT** (2 jal relocations masked) |
| `u32 %` + `if (rem) *= v + (d - rem)`, `func_00157E30` | **44/44 EXACT**, no masked words — first compile |
| `(idx << 2) + base` instead of `base + (idx << 2)`, `func_0022C278` | **44/44 EXACT** — was 38/44, only the `addu` operand order differed |
| one `S12` aggregate copy instead of two statements, `func_00145F60` / `func_00145F80` | **32/32 EXACT** each — loads grouped before stores, word store not in the `jr` slot (two statements: 20/32) |
| `Unaligned12` aggregate copy inside a guard, `func_00106ED8` | **52/52 EXACT** — was 43/52 with a `UnalignedU64` copy plus a separate word copy |
| `volatile` MMIO read in `s32 f(void) { return *(volatile s32 *)0x10003820; }` | 20 B vs retail 16 B: the load cannot fill the `jr` delay slot, so retail's `lui/ori/jr/lw` shape is **still unexplained** (`func_001C6CA8`) — non-volatile folds to the `$at` macro instead |

---

## 7. Dead ends — empirically closed, do not retry

See `docs/deadends.md` for full detail. Summary:
- **Compiler swap to 2.95.3 / -G2**: fails on `s64` ops, breaks `func_00108648`. Retail is 2.9-ee.
- **Assembler post-processing** (`ps2eeas` nop/dli): retail text is GNU-assembled; matched funcs already have retail's exact bytes at those sites (`func_001E0F10` adjacent `c.lt.s→bc1f`).
- **`register x asm("$f20")`**: no prologue save/restore, cannot match.
- **Register locals + gotos**: behaviour-only, useless for bytes.
- **Building a function from an empty body**: codegen is a whole-function fixed point; deleting a return drops matching head 25/25 → 0/25 (`func_001E0F10`).
- **Hill-climbing on byte counts**: a 2004 B func 20 B short matches 12/501 words — no directional signal.
- **Permutation on large bodies**: pays off ≤8 statements; hopeless at 50+.
- **`func_00108648` beql** (§2): all shapes tried regress; UNSOLVED.
- **`func_001B1050` loop padding**: retail's loop body is a bare `srl` followed
  by FOUR `nop`s with `addiu $2,$2,1` in the back-edge `bnez` delay slot.
  `do/while` puts the increment in the body (38/44, best), `while` and
  `for (; cond; n++)` both land at 8/44 — the four nops never reproduce.
  UNSOLVED.
- **`func_002133F0` global base register**: retail `lui $3` + `addu $3,$3,$4` +
  `lw $2,4($3)`; every shape (`*(s32 *)(D + index*0x10 + 4)`, `Struct16`
  indexing, pointer + index, `&arr[index]`) folds the `+4` into the `addiu`
  (17/24 best) or takes `$2` as the base (12/24). Not reached.
- **`func_002093E0` MMIO-style global read**: retail schedules `daddu $2,$5,$0`
  before `lbu $3,%lo(D)($4)` and keeps the address in `$4` with the value in
  `$3`; the build reuses `$3` for both and schedules the copy last (11/20).
  Pointer local / result local / deref forms all give 8/20.
- **TOOL PITFALL — `tools/try_func.py` `source_file()`**: it scans `src/*.c` in
  sorted order and accepts a *prototype* as a definition, so a function whose
  name is also `extern`-declared in an earlier file is resolved to the wrong
  file ("no INCLUDE_ASM line and no definition to replace"). `func_00119518`
  hits this. Workaround: `build/try/try.py FUNC SRC CAND` (same pipeline, file
  forced) and `build/try/show.py FUNC` (word-by-word retail vs linked build,
  which is what `file_check`'s one-line "BYTES n/m" does not show).

---

## 8. Big-function method (proof-of-method / large targets)

Data: **0 of 181 functions >=1 KB match; 1 of 1108 above 256 B**
(`func_00146850`, 304 B, `docs/fn/func_00146850.md`; the next target,
`func_001D1E50` 436 B, reached retail's exact frame and instruction multiset but
stalled on a register permutation - `docs/fn/func_001D1E50.md`). Matched code is almost all
<=128 B (2045 of ~2052). Matching a huge function needs a
structured approach, not brute force.

Three classes of failure:
1. **Transcription error** (type width, delay slot, operand, wrong struct offset) —
   a >1 KB function with wrong C needs retranscription from the asm.
2. **Scheduling cluster** — C correct, compiler orders two instructions differently.
   Small: fixable with statement/variable-order levers. Large: likely unfixable
   (`func_00177628` is the recorded example).
3. **Not expressible in C** — retail reads a callee-saved register it never writes
   (`func_00131D78` reads incoming `$f20`). No standard-C form emits "read whatever
   the caller left in $f20" AND saves/restores it. Documented dead end.

Workflow for one big function (a step is done only when its record exists):
1. **Claim + read** — `tools/claims.py`; read asm and any existing notes.
2. **Frame** — from the prologue: frame size, saved GPRs/FPRs, each spill slot with width.
3. **Signature** — types from the load/store widths of the parameter registers.
4. **Block map** — split at branches; address range + one-line purpose per block.
5. **Declaration block** — locals in the order/widths the frame implies. Empty-body
   compile to compare only prologue/epilogue (a hint, not a gate — frame is body-dependent).
6. **Transcribe the whole body** — asm order, asm branch polarity, one C group per
   block, arguments as `$4-$7` show them. Never leave a partial body as working state.
7. **Repair loop** — compile, diff, change ONE decision at a time: type width, temp
   reuse vs fresh, association, statement order, loop shape. Log every attempt;
   revert ideas go to §7.
8. **Stall rule** — same cluster after 5–8 attempts: stop guessing, read `regalloc.py`,
   change a declaration/lifetime. Still stuck: check failure class 2 or 3 above.
9. **Close** — EXACT → `integrate.py`, `sweep_matches.py`. BLOCKED → record reason +
   measurement so the next session does not retry.

Cost model (rough): **3–4 h per 100 retail instructions** at current tooling.
129–512 B band: 3–12 h each (do these first). 513–1024 B: 1–3 days (only after a
pilot proves the cost). >1 KB: weeks (only with a proven per-block cost).
Pilot = one 300–500 B function with matched callees and no float-heavy inner loops;
record hours-per-100-instructions before estimating anything larger.

Leave unmatched: class-3 functions (keep `INCLUDE_ASM`, record finding, move on);
straight-line slabs with no branches (`func_0018B130`: 576 instr, 2 branches, 0 calls —
allocator has maximum freedom); scheduling clusters that survived §2 levers.
Session ownership: `tools/claims.py` — two sessions on one file is how
`func_00101CF0` and `func_001001F0` (both listed MATCHING) stopped verifying.
Measure before trusting either the CSV or a note.

---

## 9. Function-specific confirmed findings

### func_00146850 (304 B, `funcs_11`, -O2 -ffast-math) - EXACT
- First byte match above 256 B; dossier `docs/fn/func_00146850.md`. Dedupes a
  pointer list into a caller buffer: `arg0[0] == 4`, `p = arg0[1]`,
  `list = *(u32 **)(p+0xC)`, `count = *(s32 *)(p+0x10)`; skips entities whose
  `func_00119700(v)` (u16 at +2) is 6; returns 0 when `*arg3` reaches `arg2`.
- Outer loop is a 16.16 countdown with a pre-increment copy (`cur = pos;
  pos += 0x10000; } while ((cur >> 16) < count)`), so the loop runs `count`
  times with the test only at the bottom.
- `s32 type = 6;` local is what fixes the preheader constant order
  (`addiu $30,$0,6` before `lui $21,1`).
- Compile cost: 12 compiles to EXACT from a Ghidra-shape first draft; the four
  real levers were branch polarity, `for` start index (peel), the pre-increment
  copy, and the constant local. See the dossier's attempt table.

### Small allocator/MMIO misses: func_002133F0, func_001C6900, func_001C6CA8 - NOT MATCHING

- **`func_002133F0` (24 B)**, 12/24 bytes. Retail:
  `lui $3,%hi(D_00386D18)` / `sll $4,$4,4` / `addiu $3,$3,%lo` / `addu $3,$3,$4` /
  `jr $31` / `lw $2,4($3)`. A struct-field access
  (`struct S { s32 a,b,c,d; };` then `((struct S *)D_00386D18)[index].b`) gives
  the right size, the right `+4`-folded load and the load in the delay slot, but
  the base lands in `$2` and the add folds into the index register
  (`addu $4,$4,$2`). Regressions: `D_00386D18[index * 4 + 1]` adds
  `ori $4,$4,4` (28 B); `u8 *` arithmetic; a `u32 *table` local (same as struct).
  Only the base register choice is left.
- **`func_001C6900` (20 B)** and **`func_001C6CA8` (16 B)**: both need retail's
  *materialised* MMIO address (`lui $3` + `ori $3` + `sw/lw 0($3)`) **and** the
  access in the `jr` delay slot. `volatile` gives the address but the access is
  then never moved into the delay slot (20 B / 20 B, one instruction over);
  non-volatile folds the address into the `$at` macro form instead
  (`sw $2,268447744`, `lw $2,268449824`). No source shape found that gives both.
- **`func_001C6C88` (32 B)**, 20/32. Retail: `sll $5,$5,1` / `sll $6,$6,2` /
  `or $2,$4,$5` / `lui $3,0x1000` / `or $2,$2,$6` / `ori $3,$3,0x3820` /
  `jr $31` / `sw $2,0($3)` — the *same* wall: a materialised address **and** the
  store in the return delay slot. `volatile` reproduces the lui/`or` interleave
  and the address materialisation exactly, but then the store cannot enter the
  delay slot (36 B, and the value sits in `$4` instead of `$2`). A pointer local,
  a `u32 v` local, or `return <assignment>` all still fold to the `$at` macro
  form. Whether the value in `$2` (return register) is what lets retail place the
  store last is untested.

### func_00145F60 / func_00145F80 (32 B each, `funcs_11`, -O2 -ffast-math) - EXACT

12-byte unaligned record copy (two twins, source offset `0x48` / `0x54`).
Implemented before as two statements and stuck at 20/32; both matched by copying
the record as one aggregate:

```c
void func_00145F60(u8 *arg0, u8 *arg1)
{
    *(S12 *)arg1 = *(S12 *)(arg0 + 0x48);
}
```

with the file-scope type `typedef struct { UnalignedU64 v; u32 w; } S12;`
next to `UnalignedU64` (see §2 for why the packed/aligned split matters).
`EXACT` on the first compile, 32/32 each; full build `Byte-match: 2057`;
`sweep_matches.py --func` 1/1 for both.

### func_00106ED8 (52 B, `funcs_03`, -O2) - EXACT

Bit-test on `arg0[0xF1]` and, when `arg2 != 0`, a 12-byte record copy from
`arg0 + 0xE4`. Retail groups `ldl/ldr/lw` then `sdl/sdr/sw`; the two-statement
version interleaved them (43/52). Fixed by copying the record as one aggregate
with the same packed-doubleword + aligned-word split as `S12` (§2):

```c
typedef struct { UnalignedU64 v; u32 w; } Unaligned12;   /* in funcs_03 */

    if (arg2 != 0)
        *(Unaligned12 *)arg2 = *(Unaligned12 *)(arg0 + 0xE4);
```

`EXACT` first try; `file_check` 52/52 MATCH; build `Byte-match: 2058`.

### func_00119518 (48 B, `funcs_05`, -O2) - NEAR MISS, 44/48

16-byte unaligned record copy from `arg1` to `arg0 + 0x70`, then the tail call
`func_0014E7D8(arg0)` (`j` + `nop`). The aggregate copy fixed the load/store
grouping (14/48 -> 44/48):

```c
typedef struct { UnalignedU64 lo; UnalignedU64 hi; } Unaligned16;

    *(Unaligned16 *)(arg0 + 0x70) = *(Unaligned16 *)arg1;
    func_0014E7D8(arg0);
```

Residual: retail opens with `daddu $3,$4,$0` and `daddu $2,$5,$0`, copying both
incoming pointers into fresh registers before the loads; the build uses `$4`/`$5`
directly, so the function is one instruction short. Eleven shapes tried (see §2),
none emit the copies.

### func_0022C278 (44 B, `funcs_11`, -O2 -ffast-math) - EXACT

Hash-bucket unlink. Was implemented and 38/44; the single wrong word pair was
`addu $2,$2,$4` (retail) vs the reversed order. Fix: put the index on the left.

```c
void func_0022C278(u8 *arg0, u32 *arg1)
{
    u32 *bucket;
    u32 idx;

    if (arg1 == 0)
        return;
    idx = arg1[1];
    bucket = (u32 *)((idx << 2) + *(u32 *)(arg0 + 0x4C));
    arg1[0] = *bucket;
    *bucket = (u32)arg1;
}
```

`EXACT`, 44/44; full build `Byte-match: 2055`. The guard also puts
`lw $2,4($5)` in the `beqz` delay slot exactly as retail. Note the needed shape
keeps `idx` **and** the table base as live values loaded before the add — hoisting
the base into its own `u32 *table` local regressed to 20/44.

### func_00157E30 (44 B, `funcs_11`, -O2 -ffast-math) - EXACT

```c
void func_00157E30(u32 *arg0, u32 arg1)
{
    u32 v = *arg0;
    u32 rem = v % arg1;

    if (rem != 0)
        *arg0 = v + (arg1 - rem);
}
```

`EXACT` on the first compile, 44/44 with **no** masked words; full build
`Byte-match: 2054`, `sweep_matches.py --func` 1/1. Unsigned operands are load
bearing (`divu`/guard shape, see §2). Note `*arg0` is read once into `v`; the
store reuses it, so retail's `lw $3` / `addu $2,$3,$2` comes out directly.

### func_0015BBF0 (48 B, `funcs_11`, -O2 -ffast-math) - NOT MATCHING, near miss

Sorted-list flush: `D_00393568 = arg0;` then walk `D_003D7E00[0]` copying
`node[0]` to `node[1]` until null. Structure matched exactly with the `goto`
loop form (§2): same 12 instructions, same `beqz`/`b`-to-test CFG, same delay
slots, loop variable in `$3` as retail. Remaining 20 differing bytes (5 words)
are all inside the loop body:

| retail | candidate |
|---|---|
| `beqz $3, out` + **`nop`** delay | `beqz $3, out` + `lw $5,0($3)` delay |
| `lw $4,8($3)` then `lw $2,0($3)` | `lw $5,0($3)` then `lw $2,8($3)` |
| value in `$2`, temp in `$4` | value in `$5`, temp in `$2` |

Levers tried without effect: `s32` vs `s32 *` loop variable, `while`/`for`/
`do-while` bodies, body statement order, declaration order, `void` vs
`return result` (the returning form is what puts the loop variable in `$3`). The
delay slot and the load order move together — retail's first body instruction is
`lw 8` and its slot stayed a `nop`, so the scheduler ordering is upstream of the
slot fill. Candidates: `build/try/cand_0015BBF0_{g,i,s,t,u,y,z}.c` (20 B diff,
size exact).

### func_0022E200 (36 B, `funcs_11`, -O2 -ffast-math) - EXACT

Two-call wrapper: `return func_0021E5E8(func_0022E1D0());`. Retail calls
`func_0022E1D0` with **no argument setup** (`jal` + `nop` delay) even though the
file prototypes it `s32 func_0022E1D0(s32, s32);`. Calling a prototyped function
with no args is an error, so use a *block-scope* no-prototype declaration — it
overrides the file-wide prototype for this body only and leaves every other
caller untouched:

```c
s32 func_0022E200(void)
{
    s32 func_0022E1D0();

    return func_0021E5E8(func_0022E1D0());
}
```

`EXACT` on the first compile (2 `jal` relocations masked); confirmed by the full
build (`Byte-match: 2053`) and `sweep_matches.py --func` (1/1).

### func_001D1E50 (436 B, `funcs_11`, -O2 -ffast-math) - NOT MATCHING, near miss

SIF handshake helper (mask `0x80004000`/`0x80000000`, command `arg0 << 26`,
two 5001-iteration timeouts around `func_001CDAB0`). Full detail:
`docs/fn/func_001D1E50.md`.

Reached in 20 compiles: identical 107-instruction multiset after macro
normalisation, identical 0x80 frame / save set / epilogue, `n` in `$6` and `i` in
`$17` as retail. The remaining 296 differing bytes are a **callee-saved
permutation** (`cmd` in `$18` instead of `$16`; the five hoisted `%hi` bases
rotated) plus intra-prologue scheduling.

Levers that did NOT move it: all 6 declaration orders of `n`/`cmd`/`i`
(296-300 B), late `cmd` assignment (prologue matches to +0x20 but 440 B
oversize), `for(;;)` vs duplicated test, tail statement reorder,
`while`-with-assignment vs pre-load.

`regalloc.py -dlg` on the candidate: the five `%hi` base pseudos have `refs = 4`
and live lengths 124-260, `i` has 9/110, `n` 9/46. Retail's allocation order
(`cmd`, `i`, then `79588`, `78CA4`, `7951C`, `79518`, `78D88`) is the order you
get when every base has a **single** reference (priority 0, ties by pseudo
number) - so the way in is making each base single-use, not statement order.

### func_00131D78 (8648 B collision function, own module `funcs_06`)
- Semantically correct C, NOT byte-matching (949/8648 is a coincidence floor: only
  9 whole instructions match, 6 are nop). Byte-matching needs the developers'
  near-exact original C (declarations/scoping/volatile/statement order); -O2 is
  optimal (-Os 908, -O1/O2+fno-omit-fp fail on size). Divergence cause is SOURCE
  SHAPE: retail uses ONE frame -0x310 ($sp as stack-view base); our C has
  `u8 stk[0x62]` array + GCC save area → frame -0x400, sp a pointer.
- **8 sqrt fixes applied** (retail COP1 pseudo-ops were `sqrt.s`): encodings
  `84000046→sqrt.s $f2,$f0`, `04000046→sqrt.s $f0,$f0`, `04050046→sqrt.s $f20,$f0`.
  Sites: 0x32358 (f20=|reaction|, collision distance, used by c.lt.s at 0x3235C
  and 3 divisions), 0x32538, 0x32F30, 0x33224, 0x336C8, 0x33744, 0x33974, 0x33C88.
  Ghidra confirms `_sqrtf()` at each. Compiled emits exactly 8 sqrt.s.
- Angle-block z-component: `f3 = f7 * MF(r20, 0xC)` (retail 0x33198).
- **Warping fix = `volatile` on M8/M16/MU16/M32/MU32/MF** (stale stack-slot reads
  across opaque calls). Removing volatile: byte-match 949→994 (+45) but user
  reported a NEW bug and reverted. **volatile stays.** No inline asm / register
  binding allowed (user constraint).
- `$f20` implicit input: retail reads incoming callee-saved `$f20` (saved in
  prologue, restored in epilogue, never written before first use). A file-scope
  `register float x asm("$f20")` emits no prologue save/restore → cannot match.
  Note: compiled shows `c.lt.s $f4,$f20` at 0x1321cc with no prior write; Ghidra
  shows v74(=$f20) first assigned via sqrt at 0x32358, so this build overwrites it
  — the "implicit input" may describe a different assumption.
- Verified MATCH blocks: 07 normal selection, 08 F_DOT3 operand order, 18 scan B,
  19 func_00135CF8 call args, 22 two 36C08 passes, 23 normalize/project.
  `bnezl` merging two func_00139BA0 call sites (differing only in $7) is expected
  GCC behavior, behaviorally identical. Stuck-at-wall bug NOT reproduced by any
  static difference — root cause still open.

### func_00135CF8 (3192 B surface categorizer, own module `funcs_10`, -Os)
- Behaviorally correct, NOT byte-matching. Purpose: scans 0x68-byte entities
  (base a3, count a2); by type (+0x2E), float (+0x00), flags (+0x28, +0x0C),
  referenced-object flags (+0x20→+6, +0x1C→+6) copies 0x60/0x20 blocks into 6
  arrays (D_00330ED4, D_0033100C, D_00331074, D_00330F3C, D_00330FA4, D_003310DC)
  and sets bits in D_00330ED0; then calls func_00135C10 and copies two 12-byte
  blocks to arg0+0x1B8/+0x1C4.
- **Root-cause bug fixed**: `r16` was in a zeroing chain, clobbering the actor
  pointer (a0) → threshold from 0x18, flags from 0xC, surface copies skipped →
  broken walls/clipping. Removed r16 from the chain. In-game verified fixed.
- Delay-slot bugs fixed: final copies 8 B to +0x1B8 + separate 4 B to +0x1C0
  (and +0x1C4/+0x1CC); f0 load at 0x35DBC and r11=r5+4 at 0x35DE4 always execute;
  r3=r5 in 12 alignment-branch delay slots; flag-check delay slots restructured.
- NO structs in src/ (typedefs only) — all access is raw u8* + numeric offset.
  Full structure/offset verification MATCHED retail — this function is NOT the
  source of the stuck/phasing bug.

---

## 10. Empirical dead end — passthrough-return delay slot (11 functions)

**Class**: `func_00185140/158`, `func_00188780/798`, `func_001889B8/9D0`,
`func_00188CA0/CB8`, `func_0018DA78`, `func_001BDF50/F68` — 11 identical
20-byte functions. Retail shape (`func_00185140`):
```
+0 lw   $3, D_00393CCC        ; load global
+4 addiu $3, $3, 1            ; increment
+8 sw   $3, D_00393CCC        ; store global
+C jr   $31
+10 daddu $2, $4, $0          ; DELAY SLOT: return a1 passthrough
```
C is already minimal and semantically exact:
```c
u8 *func_00185140(u8 *p) { D_00393CCC++; return p; }   // Ghidra: __int64 func(__int64 a1){++D; return a1;}
```
**Residual**: 3 of 5 words differ (12/20 bytes). GCC 2.9-ee always schedules the
independent `daddu $2,$4` (return passthrough) EARLY, at +4, and fills the `jr`
delay slot with the `sw` instead:
```
retail: lw, addiu, sw, jr, daddu    |    build: lw, daddu, addiu, sw, jr
```
**~35 source shapes tried, all 2/5**: `return p` / named-return local / `s32/s64/u64/u8*`
types / `D++` vs `D+=1` vs `D=D+1` / `v=D;D=v+1` / postfix-in-expression / comma
operator / `__builtin_expect` / `volatile` global (worse: 1/5) / memory barrier /
struct/union returns / pointer-cast returns / alias pointer / combined 64-bit
return hypothesis. No matched function anywhere in the tree uses `jr` with a
`$2,$4` delay-slot move (0 in 2051).

**NOT necessarily unreachable — corrected 2026-11-18** (I overclaimed before):
- Compiler version ruled out: compiled the same C with **2.95.3** — identical wrong
  schedule (3/5), so it is NOT a different-compiler artifact.
- Meaning of "unreachable": only that *I* could not find a C form producing this
  schedule with either EE compiler. It does NOT prove the original was asm or a
  different toolchain. The most likely explanations, untested:
  1. **A C source shape I did not think of** — an ordering/lifetime idiom that makes
     GCC 2.9-ee schedule the return move last. (Most likely; scheduler ties are
     notoriously source-sensitive.)
  2. **Handwritten asm** for these 11 trivial stubs (possible but unlikely — they are
     exactly the kind of code a compiler emits; a human would use `move $v0,$a0`).
  3. **A build-config difference** (per-file flag) not yet tried.
- Retest if a new scheduling lever is learned. A solution fixes all 11 at once.
- **Also tested (no help)**: optimization level `-O2/-Os/-O3/-O1` — all give the
  same 3/5; the other EE compiler (2.95.3) gives the identical wrong schedule.
  So neither the compiler version nor -O level explains it.

## 12. Empirical dead end — absolute store in jr delay slot

`func_001C6900` (20 B): retail is
```
lui $2,0x1000 ; addiu $3,$0,1 ; ori $2,$2,0x3000 ; jr $31 ; sw $3,0($2)
```
with the `sw` in the `jr` delay slot. Neighbour `func_001C6930` (matched) uses
the same address `*(volatile u32*)0x10003000 = 0` and gets full address
materialization (`lui $3; ori $3`) but its store is NOT in a delay slot (jr's
slot holds the epilogue) — so the volatile cast is the right source form for the
address, but GCC refuses to put a volatile store in a delay slot.

**~20 shapes tried, best 3/5** (only the `jr`/`sw` order wrong): volatile cast,
volatile pointer local, non-volatile pointer/address-local/struct-pointer (all
fold to `lui $at; sw` at 1/5 size 16), extern named global at the address (uses
`$gp` form, 16/20 wrong). Optimization levels all give the same 2/5. Same
wall class as `func_001C6EE8` (§6): a store that must occupy a delay slot but
whose source form (absolute hardware address) requires volatile, which GCC will
not schedule there. Not proved unreachable — no matched function anywhere uses
an absolute hardware-address store in a delay slot — but no C form found.

## 11. Behavioral implementations (non-byte-matching, accepted)

- **`func_001BE398`** (856 B, `funcs_11`): 4-component u8 vector interpolation
  across a 2D byte array over a 16.16 fixed-point range. Implemented from retail
  asm + Ghidra: `p = base + (row<<stride) + (start>>16)*4`; span=(end-start)/65536;
  if same row scale by D_00392908 (=1/255), else blend first partial row
  (D_0039290C), full rows, last partial row (D_00392910), then divide result by
  span. `func_0021F7A8` (stub) supplies the next-row boundary. Exact retail size
  0x358; NOT byte-matching (261/856 bytes, frame 0x90 vs 0x80 from 4 extra locals).
  Behaviour preserved, in-game test not performed. The three scale globals all
  read 0.003921568859 (1/255).

- **`func_0018BC08`** (936 B, `funcs_11`) — REVERTED. Behavioral implementation
  (transform-matrix builder with sign bytes) broke the game in-game; user asked
  to revert. Restored to `INCLUDE_ASM` retail asm. Lesson: a "behaviorally
  faithful" C rewrite of a transform/drawing function can still break the game
  (subtle field aliasing / call side effects / ordering not captured by the
  Ghidra reading). Do NOT re-attempt behavioral rewrites of transform/render
  functions without in-game verification; keep them on `INCLUDE_ASM`.

## 12. Empirical facts settled by experiment

- **Compiler = 2.9-ee-991111**: 2.95.3 breaks `func_00108648` (4/10 vs 39/40) and rejects `s64` ops; whole swap impossible. `-G2` no help. Full test harness removed.
- **Assembler = GNU as**: adjacent FP-compare→bc1 and other ps2eeas "rule" sites appear WITH retail bytes in matched functions. No post-processor needed.
- **Byte-match baseline after these experiments: 2051** (`sweep_matches.py` verifies 2051/2051).
- **Current baseline: 2058** after `func_00146850` (304 B), `func_0022E200`
  (36 B), `func_00157E30` (44 B), `func_0022C278` (44 B),
  `func_00145F60` + `func_00145F80` (32 B each) and `func_00106ED8` (52 B);
  the last full build reports `Byte-match: 2058; implemented: 117`.

Tooling note: the matching tools in `tools/` were smoke-tested here —
`diff_words.py`, `dossier.py`, `rank_candidates.py`, `regalloc.py`,
`try_func.py`, `permute.py`/`perm_search.py`, `check_regressions.py`,
`file_check.py`, `nonmatching.py`, `apply_candidate.py`, `gate_loop.py`,
`integrate.py`, `privatize.py`, `claims.py`, `check_syntax.py`,
`check_no_retail.py`, `sn_regnames.py`, and `sweep_matches.py` (audits all
byte-match claims on size+bytes with a freshness check).
