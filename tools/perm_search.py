"""Search C statement orders/variants that reproduce a function's retail bytes.

Candidates are compiled and linked (so gp-relative displacements are resolved
exactly like in the real build) and the resulting words are compared with the
retail words stored in asm/nonmatchings/core/main/<func>.s. Both dumps use the
same byte-swapped display convention, so they compare directly.
"""

import itertools
import pathlib
import re
import subprocess

ROOT = pathlib.Path(__file__).resolve().parent.parent
TOOLS = ROOT / 'toolchain/sn-prodg-24/local/sce/ee/gcc/bin'
GCC = TOOLS / 'ee-gcc.exe'
LD = TOOLS / 'ee-ld.exe'
OBJDUMP = TOOLS / 'ee-objdump.exe'
WORK = ROOT / 'build/perm'
ELF = ROOT / 'build/SLES_500.44'
GP = '0x39A3F0'
BASE = 0x100000

WORD_RE = re.compile(r'/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]{8}\s+([0-9A-Fa-f]{8})\s*\*/')
LINKER_SCRIPT = """SECTIONS
{
  . = 0x100000;
  .text : { *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}
"""

_symbols = None


def elf_symbols():
    global _symbols
    if _symbols is None:
        _symbols = {}
        out = subprocess.run([str(OBJDUMP), '-t', str(ELF)],
                             capture_output=True, text=True).stdout
        for line in out.splitlines():
            parts = line.split()
            if len(parts) >= 2 and re.fullmatch(r'[0-9a-fA-F]{8,16}', parts[0]):
                _symbols.setdefault(parts[-1], int(parts[0], 16))
    return _symbols


def retail_size(func):
    import csv
    for row in csv.reader(open(ROOT / 'progress/functions.csv')):
        if len(row) > 3 and row[0] == func:
            return int(row[3])
    raise KeyError(func)


def retail_words(func):
    """Retail instruction words, trimmed to the function's real size."""
    text = (ROOT / 'asm/nonmatchings/core/main' / f'{func}.s').read_text(
        encoding='utf-8', errors='ignore')
    words = [int(m.group(1), 16) for m in WORD_RE.finditer(text)]
    return words[:retail_size(func) // 4]


def compile_body(func, header, body):
    """Compile + link one candidate body, return the words emitted for func."""
    WORK.mkdir(parents=True, exist_ok=True)
    tu = WORK / 'tu.c'
    obj = WORK / 'tu.o'
    elf = WORK / 'tu.elf'
    script = WORK / 'mini.ld'
    script.write_text(LINKER_SCRIPT, encoding='ascii')
    tu.write_text(f'#include "common.h"\n{header}\n\n{body}\n', encoding='utf-8')
    r = subprocess.run([str(GCC), '-O2', '-G4', '-Iinclude', '-Isrc', '-c',
                        str(tu), '-o', str(obj)], capture_output=True, text=True)
    if r.returncode != 0:
        return None, r.stderr.strip().splitlines()[:3]
    known = elf_symbols()
    defsyms = ['--defsym=_gp=' + GP]
    for name in re.findall(r'extern\s+[A-Za-z0-9_ *]+?\b([A-Za-z_][A-Za-z0-9_]*)\s*(?:\[\s*\])?\s*;',
                           header):
        if name in known:
            defsyms.append(f'--defsym={name}=0x{known[name]:X}')
    r = subprocess.run([str(LD), '-T', str(script), *defsyms, '-o', str(elf), str(obj)],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return None, r.stderr.strip().splitlines()[:3]
    addr = size = None
    for line in subprocess.run([str(OBJDUMP), '-t', str(elf)],
                               capture_output=True, text=True).stdout.splitlines():
        m = re.search(r'^([0-9a-f]{8,16}) g\s+F \.text\s+([0-9a-f]+) ' + func + r'$', line)
        if m:
            addr, size = int(m.group(1), 16), int(m.group(2), 16)
    if addr is None:
        return None, ['symbol not found']
    data = subprocess.run([str(OBJDUMP), '-s', f'--start-address={addr}',
                           f'--stop-address={addr + size}', str(elf)],
                          capture_output=True, text=True).stdout
    words = []
    for line in data.splitlines():
        m = re.match(r'\s+[0-9a-f]+ ((?:[0-9a-f]{8} ){1,4})', line)
        if m:
            words += [int(w, 16) for w in m.group(1).split()]
    return words, None


def report(func, header, candidates, verbose=True):
    want = retail_words(func)
    best = None
    for label, body in candidates:
        words, err = compile_body(func, header, body)
        if words is None:
            if verbose:
                print(f'  {label}: COMPILE FAIL {err}')
            continue
        good = sum(1 for a, b in zip(words, want) if a == b)
        exact = words[:len(want)] == want
        if verbose:
            print(f'  {label}: {"EXACT" if exact else f"{good}/{len(want)} words"} '
                  f'(size {len(words) * 4})')
        if best is None or good > best[0]:
            best = (good, label, body, len(words) == len(want))
        if exact:
            return (good, label, body, True)
    return best


def permute(signature, statements, tail=''):
    out = []
    for i, order in enumerate(itertools.permutations(statements)):
        body = signature + '\n{\n' + ''.join(f'    {s}\n' for s in order)
        if tail:
            body += f'    {tail}\n'
        body += '}'
        out.append((f'perm{i}', body))
    return out
