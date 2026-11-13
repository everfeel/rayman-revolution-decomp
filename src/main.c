#include "common.h"

#ifndef __INTELLISENSE__
/* Hand-written entry-point padding; preserved by setup_asm.py. */
__asm__("nop");
#endif

extern u32 D_00393C00[];
extern u32 D_00394640;
extern u32 D_0039A3F0;
extern u8 D_410DF4[];

extern void func_001000B8(void);
extern void func_001C0EA0(s32 arg0);
extern s32 func_001002A0(s32 arg0, s32 arg1);
extern void func_001C0840(s32 status);

/* CRT entry point, derived from ps2sdk's __start: clear .bss, start the main
   thread, then set up the heap, flush the cache, enable interrupts and enter
   main(). No stack may be used before SetupThread returns the new $sp, and the
   function never returns (Exit is the last call), so everything up to and
   including main() has to stay in the asm block. */
void func_00100008(void)
{
    s32 status;

    __asm__ volatile (
        ".set noreorder\n"
        ".set noat\n"
        /* Clear bss area: .bss runs from D_00393C00 up to _end. */
        "lui $2, %%hi(D_00393C00)\n"
        "lui $3, %%hi(D_410DF4)\n"
        "addiu $2, $2, %%lo(D_00393C00)\n"
        "addiu $3, $3, %%lo(D_410DF4)\n"
        "1:\n"
        "nop\n"
        "nop\n"
        "sq $0, 0x0($2)\n"
        "sltu $1, $2, $3\n"
        "nop\n"
        "bnez $1, 1b\n"
        "addiu $2, $2, 0x10\n"
        /* SetupThread(gp, _stack, _stack_size, &args, ExitThread) */
        "lui $4, %%hi(D_0039A3F0)\n"
        "lui $5, %%hi(0x01FF0000)\n"
        "lui $6, %%hi(0x00010000)\n"
        "lui $7, %%hi(D_00394640)\n"
        "lui $8, %%hi(func_001000B8)\n"
        "addiu $4, $4, %%lo(D_0039A3F0)\n"
        "addiu $5, $5, %%lo(0x01FF0000)\n"
        "addiu $6, $6, %%lo(0x00010000)\n"
        "addiu $7, $7, %%lo(D_00394640)\n"
        "addiu $8, $8, %%lo(func_001000B8)\n"
        "daddu $28, $4, $0\n"
        "addiu $3, $0, 0x3C\n"
        "syscall\n"
        "daddu $29, $2, $0\n"
        /* SetupHeap(_end, -1) */
        "lui $4, %%hi(D_410DF4)\n"
        "lui $5, 0\n"
        "addiu $4, $4, %%lo(D_410DF4)\n"
        "addiu $5, $5, -0x1\n"
        "addiu $3, $0, 0x3D\n"
        "syscall\n"
        /* FlushCache(0) */
        "jal func_001C0EA0\n"
        "daddu $4, $0, $0\n"
        "ei\n"
        /* main(args.argc, args.argv) */
        "lui $2, %%hi(D_00394640)\n"
        "addiu $2, $2, %%lo(D_00394640)\n"
        "lw $4, 0x0($2)\n"
        "jal func_001002A0\n"
        "addiu $5, $2, 0x4\n"
        ".set at\n"
        ".set reorder\n"
        : "=r"(status)
        :
        : "memory", "$1", "$3", "$4", "$5", "$6", "$7", "$8", "$28", "$29"
    );

    /* Exit(main(...)) */
    func_001C0840(status);
}

void func_001000B0(void)
{
    func_001C0840(0);
}
