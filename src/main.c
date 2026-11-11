#include "common.h"

#ifndef __INTELLISENSE__
/* Hand-written entry-point padding; preserved by setup_asm.py. */
__asm__("nop");
#endif

extern void func_001C0840(s32 status);

extern u32 D_00393C00[];
extern u32 D_00394640;
extern u32 D_0039A3F0;

#define BSS_END_ADDRESS 0x00410DF4u

extern void func_001000B8(void);
extern void func_001C0EA0(s32 arg0);
extern s32 func_001002A0(s32 arg0, s32 arg1);
extern void func_001C0840(s32 status);

void func_00100008(void)
{
    u32 *ptr;
    bool cond;
    s32 status;

    ptr = D_00393C00;

    do
    {
        ptr[0] = 0;
        ptr[1] = 0;
        ptr[2] = 0;
        ptr[3] = 0;

        cond = (u32)ptr < BSS_END_ADDRESS;
        ptr += 4;
    }
    while (cond);

    {
        register s32 syscall_number __asm__("$3") = 0x3C;
        register s32 syscall_result __asm__("$2");

        __asm__ volatile (
            "li $4, 0x0039A3F0\n"
            "li $5, 0x01FF0000\n"
            "li $6, 0x00010000\n"
            "li $7, 0x00394640\n"
            "li $8, 0x001000B8\n"
            "daddu $gp, $4, $0\n"
            "syscall\n"
            "daddu $sp, $2, $0"
            : "=r"(syscall_result), "+r"(syscall_number)
            :
            : "$4", "$5", "$6", "$7", "$8", "$sp", "memory"
        );
    }

    {
        register s32 syscall_number __asm__("$3") = 0x3D;
        register s32 syscall_result __asm__("$2");

        __asm__ volatile (
            "li $4, 0x00410DF4\n"
            "li $5, -1\n"
            "syscall"
            : "=r"(syscall_result), "+r"(syscall_number)
            :
            : "$4", "$5", "memory"
        );
    }

    func_001C0EA0(0);
    __asm__ volatile ("ei" ::: "memory");

    status = func_001002A0(D_00394640, (s32)((u32)&D_00394640 + 4));
    func_001C0840(status);
}

void func_001000B0(void)
{
    func_001C0840(0);
}
