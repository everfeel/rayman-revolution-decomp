#include "common.h"

/* CRT entry point at 0x00100008.  Handwritten assembly is required here:
   the .bss clear uses the 128-bit `sq` store, thread/heap setup use the
   R5900 `syscall` instruction with fixed $3 syscall numbers, and the
   interrupt enable (`ei`) has no C equivalent.  The retail bytes are kept
   exactly by src/asm/func_00100008.s. */
ASM_FUNC(void, func_00100008);

/* Exit the process with status zero.  A plain C tail-call reproduces
   `j func_001C0840` with `move $4,$0` in the delay slot. */
void func_001C0840(s32);
void func_001000B0(void)
{
    func_001C0840(0);
}

/* ExitThread syscall (0x23).  Control never returns, so the hand-written
   assembly omits an epilogue; keeping it as asm avoids a spurious `jr`. */
ASM_FUNC(void, func_001000B8);
