#include "common.h"

/* func_001C0090 through func_001CFFD0; retail address range 0x001C0000-0x001D0000. */



s32 D_00378AC0[8];
u8 D_00379530[8];
s32 D_0038E278[8];

INCLUDE_ASM(const s32, "core/main", func_001C0090);

INCLUDE_ASM(const s32, "core/main", func_001C0150);

INCLUDE_ASM(const s32, "core/main", func_001C0218);

INCLUDE_ASM(const s32, "core/main", func_001C0400);

INCLUDE_ASM(const s32, "core/main", func_001C0508);

INCLUDE_ASM(const s32, "core/main", func_001C05F0);

s32 func_001C0800(void) {
    register s32 syscall_result __asm__("$2");
    __asm__ volatile (
        "addiu $3, $0, 0\n"
        "syscall"
        : "=r"(syscall_result)
        :
        : "$3", "memory"
    );
    return syscall_result;
}

s32 func_001C0810(void) {
    register s32 syscall_number __asm__("$3") = 0x1;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0820(void) {
    register s32 syscall_number __asm__("$3") = 0x2;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0830(void) {
    register s32 syscall_number __asm__("$3") = 0x3;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

void func_001C0840(s32 status) // Exit (error code: 4) syscall wrapper
{
#ifndef __INTELLISENSE__
    register s32 syscall_status __asm__("$4") = status;
    register s32 syscall_number __asm__("$3") = 4;

    __asm__ volatile (
        "syscall"
        : "+r"(syscall_number)
        : "r"(syscall_status)
        : "memory"
    );
#else
    (void)status;
#endif
}

s32 func_001C0850(void) {
    register s32 syscall_number __asm__("$3") = 0x5;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0860(void) {
    register s32 syscall_number __asm__("$3") = 0x6;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0870(void) {
    register s32 syscall_number __asm__("$3") = 0x7;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0880(void) {
    register s32 syscall_number __asm__("$3") = 0x8;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0890(void) {
    register s32 syscall_number __asm__("$3") = 0x9;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08A0(void) {
    register s32 syscall_number __asm__("$3") = 0xA;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08B0(void) {
    register s32 syscall_number __asm__("$3") = 0xB;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08C0(void) {
    register s32 syscall_number __asm__("$3") = 0xC;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08D0(void) {
    register s32 syscall_number __asm__("$3") = 0xD;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08E0(void) {
    register s32 syscall_number __asm__("$3") = 0xE;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08F0(void) {
    register s32 syscall_number __asm__("$3") = 0xF;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0900(void)
{
    register s32 syscall_number __asm__("$3") = 0x10;
    register s32 syscall_result __asm__("$2");

    __asm__ volatile (
        "syscall"
        : "=r"(syscall_result), "+r"(syscall_number)
        :
        : "memory"
    );
    return syscall_result;
}

s32 func_001C0910(void) {
    register s32 syscall_number __asm__("$3") = 0x10;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0920(void) {
    register s32 syscall_number __asm__("$3") = 0x11;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0930(void) {
    register s32 syscall_number __asm__("$3") = 0x12;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0940(void) {
    register s32 syscall_number __asm__("$3") = 0x12;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0950(void) {
    register s32 syscall_number __asm__("$3") = 0x13;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0960(void) {
    register s32 syscall_number __asm__("$3") = 0x14;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0970(void) {
    register s32 syscall_number __asm__("$3") = 0x15;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0980(void) {
    register s32 syscall_number __asm__("$3") = 0x16;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0990(void) {
    register s32 syscall_number __asm__("$3") = 0x17;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09A0(void) {
    register s32 syscall_number __asm__("$3") = 0x18;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09B0(void) {
    register s32 syscall_number __asm__("$3") = 0x19;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09C0(void) {
    register s32 syscall_number __asm__("$3") = -0x1A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09D0(void) {
    register s32 syscall_number __asm__("$3") = -0x1B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09E0(void) {
    register s32 syscall_number __asm__("$3") = -0x1C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09F0(void) {
    register s32 syscall_number __asm__("$3") = -0x1D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A00(void) {
    register s32 syscall_number __asm__("$3") = -0x1E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A10(void) {
    register s32 syscall_number __asm__("$3") = -0x1F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A20(void) {
    register s32 syscall_number __asm__("$3") = 0x20;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A30(void) {
    register s32 syscall_number __asm__("$3") = 0x21;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A40(void) {
    register s32 syscall_number __asm__("$3") = 0x22;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A50(void) {
    register s32 syscall_number __asm__("$3") = 0x23;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A60(void) {
    register s32 syscall_number __asm__("$3") = 0x24;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A70(void) {
    register s32 syscall_number __asm__("$3") = 0x25;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A80(void) {
    register s32 syscall_number __asm__("$3") = -0x26;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A90(void) {
    register s32 syscall_number __asm__("$3") = 0x27;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AA0(void) {
    register s32 syscall_number __asm__("$3") = 0x28;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AB0(void) {
    register s32 syscall_number __asm__("$3") = 0x29;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AC0(void) {
    register s32 syscall_number __asm__("$3") = -0x2A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AD0(void) {
    register s32 syscall_number __asm__("$3") = 0x2B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AE0(void) {
    register s32 syscall_number __asm__("$3") = -0x2C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AF0(void) {
    register s32 syscall_number __asm__("$3") = 0x2D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B00(void) {
    register s32 syscall_number __asm__("$3") = -0x2E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B10(void) {
    register s32 syscall_number __asm__("$3") = 0x2F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B20(void) {
    register s32 syscall_number __asm__("$3") = 0x30;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B30(void) {
    register s32 syscall_number __asm__("$3") = -0x31;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

void func_001C0B40(void)
{
#ifndef __INTELLISENSE__
    __asm__ __volatile__("addiu $3, $0, 0x32\n\tsyscall");
#endif
}

s32 func_001C0B50(void) {
    register s32 syscall_number __asm__("$3") = 0x33;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B60(void) {
    register s32 syscall_number __asm__("$3") = -0x34;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B70(void) {
    register s32 syscall_number __asm__("$3") = 0x35;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B80(void) {
    register s32 syscall_number __asm__("$3") = -0x36;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B90(void) {
    register s32 syscall_number __asm__("$3") = 0x37;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BA0(void) {
    register s32 syscall_number __asm__("$3") = -0x38;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BB0(void) {
    register s32 syscall_number __asm__("$3") = 0x39;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BC0(void) {
    register s32 syscall_number __asm__("$3") = -0x3A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BD0(void) {
    register s32 syscall_number __asm__("$3") = 0x3B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BE0(void) {
    register s32 syscall_number __asm__("$3") = 0x3C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BF0(void) {
    register s32 syscall_number __asm__("$3") = 0x3D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C00(void) {
    register s32 syscall_number __asm__("$3") = 0x3E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C10(void) {
    register s32 syscall_number __asm__("$3") = 0x3F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C20(void) {
    register s32 syscall_number __asm__("$3") = 0x40;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C30(void) {
    register s32 syscall_number __asm__("$3") = 0x41;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C40(void) {
    register s32 syscall_number __asm__("$3") = 0x42;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C50(void) {
    register s32 syscall_number __asm__("$3") = -0x43;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C60(void) {
    register s32 syscall_number __asm__("$3") = 0x44;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C70(void) {
    register s32 syscall_number __asm__("$3") = 0x45;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C80(void) {
    register s32 syscall_number __asm__("$3") = -0x46;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C90(void) {
    register s32 syscall_number __asm__("$3") = 0x47;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CA0(void) {
    register s32 syscall_number __asm__("$3") = -0x48;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CB0(void) {
    register s32 syscall_number __asm__("$3") = 0x49;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CC0(void) {
    register s32 syscall_number __asm__("$3") = 0x4A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CD0(void) {
    register s32 syscall_number __asm__("$3") = 0x4B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CE0(void) {
    register s32 syscall_number __asm__("$3") = 0x4C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CF0(void) {
    register s32 syscall_number __asm__("$3") = 0x4D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D00(void) {
    register s32 syscall_number __asm__("$3") = 0x4E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D10(void) {
    register s32 syscall_number __asm__("$3") = 0x4F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D20(void) {
    register s32 syscall_number __asm__("$3") = 0x50;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D30(void) {
    register s32 syscall_number __asm__("$3") = 0x51;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D40(void) {
    register s32 syscall_number __asm__("$3") = 0x52;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D50(void) {
    register s32 syscall_number __asm__("$3") = -0x53;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D60(void) {
    register s32 syscall_number __asm__("$3") = 0x54;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D70(void) {
    register s32 syscall_number __asm__("$3") = -0x55;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D80(void) {
    register s32 syscall_number __asm__("$3") = 0x56;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D90(void) {
    register s32 syscall_number __asm__("$3") = 0x57;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DA0(void) {
    register s32 syscall_number __asm__("$3") = -0x58;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DB0(void) {
    register s32 syscall_number __asm__("$3") = 0x59;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DC0(void) {
    register s32 syscall_number __asm__("$3") = -0x5A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DD0(void) {
    register s32 syscall_number __asm__("$3") = 0x5B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DE0(void) {
    register s32 syscall_number __asm__("$3") = 0x5C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DF0(void) {
    register s32 syscall_number __asm__("$3") = -0x5C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E00(void) {
    register s32 syscall_number __asm__("$3") = 0x5D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E10(void) {
    register s32 syscall_number __asm__("$3") = -0x5D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E20(void) {
    register s32 syscall_number __asm__("$3") = 0x5E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E30(void) {
    register s32 syscall_number __asm__("$3") = -0x5E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E40(void) {
    register s32 syscall_number __asm__("$3") = 0x5F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E50(void) {
    register s32 syscall_number __asm__("$3") = -0x5F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E60(void) {
    register s32 syscall_number __asm__("$3") = 0x60;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E70(void) {
    register s32 syscall_number __asm__("$3") = 0x61;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E80(void) {
    register s32 syscall_number __asm__("$3") = 0x62;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E90(void) {
    register s32 syscall_number __asm__("$3") = 0x63;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EA0(void) {
    register s32 syscall_number __asm__("$3") = 0x64;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EB0(void) {
    register s32 syscall_number __asm__("$3") = 0x66;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EC0(void) {
    register s32 syscall_number __asm__("$3") = -0x67;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0ED0(void) {
    register s32 syscall_number __asm__("$3") = -0x68;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EE0(void) {
    register s32 syscall_number __asm__("$3") = -0x6A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EF0(void) {
    register s32 syscall_number __asm__("$3") = 0x6B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F00(void) {
    register s32 syscall_number __asm__("$3") = 0x6C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F10(void) {
    register s32 syscall_number __asm__("$3") = 0x6D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F20(void) {
    register s32 syscall_number __asm__("$3") = 0x6E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F30(void) {
    register s32 syscall_number __asm__("$3") = 0x6F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F40(void) {
    register s32 syscall_number __asm__("$3") = 0x70;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F50(void) {
    register s32 syscall_number __asm__("$3") = -0x70;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F60(void) {
    register s32 syscall_number __asm__("$3") = 0x71;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F70(void) {
    register s32 syscall_number __asm__("$3") = -0x71;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F80(void) {
    register s32 syscall_number __asm__("$3") = 0x72;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F90(void) {
    register s32 syscall_number __asm__("$3") = 0x73;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FA0(void) {
    register s32 syscall_number __asm__("$3") = 0x74;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FB0(void) {
    register s32 syscall_number __asm__("$3") = 0x75;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FC0(void) {
    register s32 syscall_number __asm__("$3") = 0x76;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FD0(void) {
    register s32 syscall_number __asm__("$3") = -0x76;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FE0(void) {
    register s32 syscall_number __asm__("$3") = 0x77;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FF0(void) {
    register s32 syscall_number __asm__("$3") = -0x77;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1000(void) {
    register s32 syscall_number __asm__("$3") = 0x78;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1010(void) {
    register s32 syscall_number __asm__("$3") = -0x78;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1020(void) {
    register s32 syscall_number __asm__("$3") = 0x79;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1030(void) {
    register s32 syscall_number __asm__("$3") = 0x7A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1040(void) {
    register s32 syscall_number __asm__("$3") = 0x7B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1050(void) {
    register s32 syscall_number __asm__("$3") = 0x7C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1060(void) {
    register s32 syscall_number __asm__("$3") = 0x7D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1070(void) {
    register s32 syscall_number __asm__("$3") = 0x7E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1080(void) {
    register s32 syscall_number __asm__("$3") = 0x7F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

INCLUDE_ASM(const s32, "core/main", func_001C1090);

INCLUDE_ASM(const s32, "core/main", func_001C10D0);

INCLUDE_ASM(const s32, "core/main", func_001C1130);

INCLUDE_ASM(const s32, "core/main", func_001C11B0);

INCLUDE_ASM(const s32, "core/main", func_001C1228);

s32 func_001C1250(void) { return -1; }

s32 func_001C1258(void) { return -1; }

s32 func_001C1260(void) { return -1; }

INCLUDE_ASM(const s32, "core/main", func_001C1268);

s32 func_001C1310(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001C1318);

s32 func_001C1330(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001C1338);

INCLUDE_ASM(const s32, "core/main", func_001C1360);

INCLUDE_ASM(const s32, "core/main", func_001C1388);

INCLUDE_ASM(const s32, "core/main", func_001C13B0);

INCLUDE_ASM(const s32, "core/main", func_001C1408);

INCLUDE_ASM(const s32, "core/main", func_001C1420);

INCLUDE_ASM(const s32, "core/main", func_001C14C8);

INCLUDE_ASM(const s32, "core/main", func_001C1540);

INCLUDE_ASM(const s32, "core/main", func_001C1558);

INCLUDE_ASM(const s32, "core/main", func_001C1600);

INCLUDE_ASM(const s32, "core/main", func_001C1678);

INCLUDE_ASM(const s32, "core/main", func_001C1690);

INCLUDE_ASM(const s32, "core/main", func_001C16F8);

INCLUDE_ASM(const s32, "core/main", func_001C1760);

INCLUDE_ASM(const s32, "core/main", func_001C17C8);

INCLUDE_ASM(const s32, "core/main", func_001C1830);

INCLUDE_ASM(const s32, "core/main", func_001C18A0);

INCLUDE_ASM(const s32, "core/main", func_001C1908);

INCLUDE_ASM(const s32, "core/main", func_001C1928);

INCLUDE_ASM(const s32, "core/main", func_001C1948);

INCLUDE_ASM(const s32, "core/main", func_001C1968);

INCLUDE_ASM(const s32, "core/main", func_001C1988);

INCLUDE_ASM(const s32, "core/main", func_001C19A8);

INCLUDE_ASM(const s32, "core/main", func_001C19C8);

INCLUDE_ASM(const s32, "core/main", func_001C19F0);

INCLUDE_ASM(const s32, "core/main", func_001C1A30);

INCLUDE_ASM(const s32, "core/main", func_001C1A70);

INCLUDE_ASM(const s32, "core/main", func_001C1C08);

INCLUDE_ASM(const s32, "core/main", func_001C1D58);

INCLUDE_ASM(const s32, "core/main", func_001C1E28);

INCLUDE_ASM(const s32, "core/main", func_001C1EE8);

INCLUDE_ASM(const s32, "core/main", func_001C1F20);

INCLUDE_ASM(const s32, "core/main", func_001C1FD0);

INCLUDE_ASM(const s32, "core/main", func_001C2008);

INCLUDE_ASM(const s32, "core/main", func_001C2098);

INCLUDE_ASM(const s32, "core/main", func_001C2160);

INCLUDE_ASM(const s32, "core/main", func_001C22C8);

INCLUDE_ASM(const s32, "core/main", func_001C2890);

INCLUDE_ASM(const s32, "core/main", func_001C28C8);

INCLUDE_ASM(const s32, "core/main", func_001C2928);

INCLUDE_ASM(const s32, "core/main", func_001C2AE8);

INCLUDE_ASM(const s32, "core/main", func_001C2B10);

INCLUDE_ASM(const s32, "core/main", func_001C2BB8);

INCLUDE_ASM(const s32, "core/main", func_001C2BD8);

INCLUDE_ASM(const s32, "core/main", func_001C2C08);

INCLUDE_ASM(const s32, "core/main", func_001C2C48);

INCLUDE_ASM(const s32, "core/main", func_001C2D00);

INCLUDE_ASM(const s32, "core/main", func_001C2D60);

INCLUDE_ASM(const s32, "core/main", func_001C2EB8);

INCLUDE_ASM(const s32, "core/main", func_001C2F08);

INCLUDE_ASM(const s32, "core/main", func_001C2FB8);

INCLUDE_ASM(const s32, "core/main", func_001C30F8);

INCLUDE_ASM(const s32, "core/main", func_001C3188);

INCLUDE_ASM(const s32, "core/main", func_001C3378);

INCLUDE_ASM(const s32, "core/main", func_001C33B8);

INCLUDE_ASM(const s32, "core/main", func_001C3448);

INCLUDE_ASM(const s32, "core/main", func_001C34D8);

INCLUDE_ASM(const s32, "core/main", func_001C3558);

INCLUDE_ASM(const s32, "core/main", func_001C35E0);

INCLUDE_ASM(const s32, "core/main", func_001C3630);

INCLUDE_ASM(const s32, "core/main", func_001C3810);

INCLUDE_ASM(const s32, "core/main", func_001C3850);

INCLUDE_ASM(const s32, "core/main", func_001C38C0);

void func_001C38E8(void) {
}

void func_001C38F0(void) {
}

s32 func_001C38F8(void) { return 0; }

void func_001C3900(void) {
}

void func_001C3908(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001C3910);

INCLUDE_ASM(const s32, "core/main", func_001C3A58);

INCLUDE_ASM(const s32, "core/main", func_001C3AC0);

INCLUDE_ASM(const s32, "core/main", func_001C3AF8);

INCLUDE_ASM(const s32, "core/main", func_001C3CA0);

INCLUDE_ASM(const s32, "core/main", func_001C3D98);

INCLUDE_ASM(const s32, "core/main", func_001C3EE8);

INCLUDE_ASM(const s32, "core/main", func_001C3F80);

INCLUDE_ASM(const s32, "core/main", func_001C4128);

INCLUDE_ASM(const s32, "core/main", func_001C4310);

INCLUDE_ASM(const s32, "core/main", func_001C45B8);

INCLUDE_ASM(const s32, "core/main", func_001C4640);

INCLUDE_ASM(const s32, "core/main", func_001C46B0);

INCLUDE_ASM(const s32, "core/main", func_001C4728);

INCLUDE_ASM(const s32, "core/main", func_001C4818);

INCLUDE_ASM(const s32, "core/main", func_001C4918);

INCLUDE_ASM(const s32, "core/main", func_001C4980);

INCLUDE_ASM(const s32, "core/main", func_001C49B8);

INCLUDE_ASM(const s32, "core/main", func_001C4BA8);

INCLUDE_ASM(const s32, "core/main", func_001C4DA8);

INCLUDE_ASM(const s32, "core/main", func_001C4DC8);

INCLUDE_ASM(const s32, "core/main", func_001C4ED0);

INCLUDE_ASM(const s32, "core/main", func_001C4EF0);

INCLUDE_ASM(const s32, "core/main", func_001C4F18);

INCLUDE_ASM(const s32, "core/main", func_001C5008);

INCLUDE_ASM(const s32, "core/main", func_001C50E8);

INCLUDE_ASM(const s32, "core/main", func_001C5218);

INCLUDE_ASM(const s32, "core/main", func_001C5240);

INCLUDE_ASM(const s32, "core/main", func_001C5288);

INCLUDE_ASM(const s32, "core/main", func_001C5388);

INCLUDE_ASM(const s32, "core/main", func_001C5500);

INCLUDE_ASM(const s32, "core/main", func_001C5578);

INCLUDE_ASM(const s32, "core/main", func_001C55D8);

INCLUDE_ASM(const s32, "core/main", func_001C5658);

INCLUDE_ASM(const s32, "core/main", func_001C56A0);

INCLUDE_ASM(const s32, "core/main", func_001C56C8);

INCLUDE_ASM(const s32, "core/main", func_001C56F8);

INCLUDE_ASM(const s32, "core/main", func_001C5720);

INCLUDE_ASM(const s32, "core/main", func_001C5758);

INCLUDE_ASM(const s32, "core/main", func_001C5790);

INCLUDE_ASM(const s32, "core/main", func_001C57C0);

INCLUDE_ASM(const s32, "core/main", func_001C57E8);

INCLUDE_ASM(const s32, "core/main", func_001C5810);

INCLUDE_ASM(const s32, "core/main", func_001C5838);

INCLUDE_ASM(const s32, "core/main", func_001C5858);

INCLUDE_ASM(const s32, "core/main", func_001C5868);

INCLUDE_ASM(const s32, "core/main", func_001C5880);

INCLUDE_ASM(const s32, "core/main", func_001C58A0);

INCLUDE_ASM(const s32, "core/main", func_001C58B0);

INCLUDE_ASM(const s32, "core/main", func_001C5B50);

INCLUDE_ASM(const s32, "core/main", func_001C5B88);

INCLUDE_ASM(const s32, "core/main", func_001C5BA0);

INCLUDE_ASM(const s32, "core/main", func_001C5BB8);

INCLUDE_ASM(const s32, "core/main", func_001C5BE8);

INCLUDE_ASM(const s32, "core/main", func_001C5C10);

INCLUDE_ASM(const s32, "core/main", func_001C5D48);

INCLUDE_ASM(const s32, "core/main", func_001C5D88);

INCLUDE_ASM(const s32, "core/main", func_001C5DC8);

INCLUDE_ASM(const s32, "core/main", func_001C5EE8);

INCLUDE_ASM(const s32, "core/main", func_001C5F94);

INCLUDE_ASM(const s32, "core/main", func_001C5F98);

INCLUDE_ASM(const s32, "core/main", func_001C5FB0);

INCLUDE_ASM(const s32, "core/main", func_001C5FC8);

INCLUDE_ASM(const s32, "core/main", func_001C6028);

INCLUDE_ASM(const s32, "core/main", func_001C6078);

INCLUDE_ASM(const s32, "core/main", func_001C60A8);

INCLUDE_ASM(const s32, "core/main", func_001C60D8);

INCLUDE_ASM(const s32, "core/main", func_001C60E8);

INCLUDE_ASM(const s32, "core/main", func_001C60F8);

INCLUDE_ASM(const s32, "core/main", func_001C6138);

INCLUDE_ASM(const s32, "core/main", func_001C6148);

INCLUDE_ASM(const s32, "core/main", func_001C6158);

INCLUDE_ASM(const s32, "core/main", func_001C6238);

INCLUDE_ASM(const s32, "core/main", func_001C63A8);

INCLUDE_ASM(const s32, "core/main", func_001C63D8);

INCLUDE_ASM(const s32, "core/main", func_001C6418);

INCLUDE_ASM(const s32, "core/main", func_001C6460);

INCLUDE_ASM(const s32, "core/main", func_001C6478);

INCLUDE_ASM(const s32, "core/main", func_001C64D8);

INCLUDE_ASM(const s32, "core/main", func_001C6528);

INCLUDE_ASM(const s32, "core/main", func_001C6558);

INCLUDE_ASM(const s32, "core/main", func_001C6588);

INCLUDE_ASM(const s32, "core/main", func_001C6598);

INCLUDE_ASM(const s32, "core/main", func_001C65A8);

INCLUDE_ASM(const s32, "core/main", func_001C65F0);

INCLUDE_ASM(const s32, "core/main", func_001C6600);

INCLUDE_ASM(const s32, "core/main", func_001C6628);

INCLUDE_ASM(const s32, "core/main", func_001C67B8);

INCLUDE_ASM(const s32, "core/main", func_001C6900);

INCLUDE_ASM(const s32, "core/main", func_001C6918);

INCLUDE_ASM(const s32, "core/main", func_001C6930);

INCLUDE_ASM(const s32, "core/main", func_001C6968);

INCLUDE_ASM(const s32, "core/main", func_001C69D0);

INCLUDE_ASM(const s32, "core/main", func_001C69E8);

INCLUDE_ASM(const s32, "core/main", func_001C6A50);

INCLUDE_ASM(const s32, "core/main", func_001C6A70);

INCLUDE_ASM(const s32, "core/main", func_001C6AA0);

INCLUDE_ASM(const s32, "core/main", func_001C6B58);

INCLUDE_ASM(const s32, "core/main", func_001C6BB8);

INCLUDE_ASM(const s32, "core/main", func_001C6BE0);

INCLUDE_ASM(const s32, "core/main", func_001C6C48);

INCLUDE_ASM(const s32, "core/main", func_001C6C88);

INCLUDE_ASM(const s32, "core/main", func_001C6CA8);

INCLUDE_ASM(const s32, "core/main", func_001C6CB8);

INCLUDE_ASM(const s32, "core/main", func_001C6D08);

INCLUDE_ASM(const s32, "core/main", func_001C6E88);

INCLUDE_ASM(const s32, "core/main", func_001C6EE8);

INCLUDE_ASM(const s32, "core/main", func_001C6F10);

INCLUDE_ASM(const s32, "core/main", func_001C6F78);

INCLUDE_ASM(const s32, "core/main", func_001C6FB8);

INCLUDE_ASM(const s32, "core/main", func_001C6FD8);

INCLUDE_ASM(const s32, "core/main", func_001C6FE8);

INCLUDE_ASM(const s32, "core/main", func_001C7038);

INCLUDE_ASM(const s32, "core/main", func_001C7210);

INCLUDE_ASM(const s32, "core/main", func_001C7270);

INCLUDE_ASM(const s32, "core/main", func_001C72D0);

INCLUDE_ASM(const s32, "core/main", func_001C7308);

INCLUDE_ASM(const s32, "core/main", func_001C7330);

s32 func_001C7410(s32 value) { s32 old = D_00378AC0[0]; D_00378AC0[0] = value; return old; }

INCLUDE_ASM(const s32, "core/main", func_001C7420);

INCLUDE_ASM(const s32, "core/main", func_001C75F8);

INCLUDE_ASM(const s32, "core/main", func_001C7630);

INCLUDE_ASM(const s32, "core/main", func_001C7658);

INCLUDE_ASM(const s32, "core/main", func_001C7730);

INCLUDE_ASM(const s32, "core/main", func_001C7818);

INCLUDE_ASM(const s32, "core/main", func_001C7900);

INCLUDE_ASM(const s32, "core/main", func_001C79C8);

INCLUDE_ASM(const s32, "core/main", func_001C7AB8);

INCLUDE_ASM(const s32, "core/main", func_001C7BA8);

INCLUDE_ASM(const s32, "core/main", func_001C7BB8);

INCLUDE_ASM(const s32, "core/main", func_001C7C80);

INCLUDE_ASM(const s32, "core/main", func_001C7D50);

INCLUDE_ASM(const s32, "core/main", func_001C7D78);

INCLUDE_ASM(const s32, "core/main", func_001C7D88);

INCLUDE_ASM(const s32, "core/main", func_001C7DA8);

INCLUDE_ASM(const s32, "core/main", func_001C7DC8);

INCLUDE_ASM(const s32, "core/main", func_001C7DF0);

INCLUDE_ASM(const s32, "core/main", func_001C7E78);

INCLUDE_ASM(const s32, "core/main", func_001C7FB0);

INCLUDE_ASM(const s32, "core/main", func_001C8050);

INCLUDE_ASM(const s32, "core/main", func_001C80D0);

INCLUDE_ASM(const s32, "core/main", func_001C8228);

INCLUDE_ASM(const s32, "core/main", func_001C82A0);

INCLUDE_ASM(const s32, "core/main", func_001C8300);

INCLUDE_ASM(const s32, "core/main", func_001C8320);

INCLUDE_ASM(const s32, "core/main", func_001C8368);

INCLUDE_ASM(const s32, "core/main", func_001C83A8);

INCLUDE_ASM(const s32, "core/main", func_001C83E0);

INCLUDE_ASM(const s32, "core/main", func_001C8410);

INCLUDE_ASM(const s32, "core/main", func_001C8430);

INCLUDE_ASM(const s32, "core/main", func_001C8468);

INCLUDE_ASM(const s32, "core/main", func_001C8558);

INCLUDE_ASM(const s32, "core/main", func_001C8640);

INCLUDE_ASM(const s32, "core/main", func_001C8748);

INCLUDE_ASM(const s32, "core/main", func_001C8800);

INCLUDE_ASM(const s32, "core/main", func_001C8898);

INCLUDE_ASM(const s32, "core/main", func_001C8970);

INCLUDE_ASM(const s32, "core/main", func_001C89E0);

INCLUDE_ASM(const s32, "core/main", func_001C8A90);

INCLUDE_ASM(const s32, "core/main", func_001C8AC0);

INCLUDE_ASM(const s32, "core/main", func_001C8AE0);

INCLUDE_ASM(const s32, "core/main", func_001C8B00);

INCLUDE_ASM(const s32, "core/main", func_001C8BD0);

INCLUDE_ASM(const s32, "core/main", func_001C8C38);

INCLUDE_ASM(const s32, "core/main", func_001C8CA0);

INCLUDE_ASM(const s32, "core/main", func_001C8D08);

void func_001C8D78(s32 *p, s32 value) {
    p[0] = value;
    __asm__ volatile("" ::: "memory");
    p[2] = 0;
    __asm__ volatile("" ::: "memory");
    p[1] = value;
}

s32 func_001C8D88(s32 *p) { s32 value = p[1]; p[2] = 0; p[0] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_001C8D98);

INCLUDE_ASM(const s32, "core/main", func_001C8DF0);

INCLUDE_ASM(const s32, "core/main", func_001C8E08);

INCLUDE_ASM(const s32, "core/main", func_001C8E30);

INCLUDE_ASM(const s32, "core/main", func_001C8F88);

INCLUDE_ASM(const s32, "core/main", func_001C8FB0);

INCLUDE_ASM(const s32, "core/main", func_001C9040);

INCLUDE_ASM(const s32, "core/main", func_001C90C8);

INCLUDE_ASM(const s32, "core/main", func_001C91B0);

INCLUDE_ASM(const s32, "core/main", func_001C91E8);

INCLUDE_ASM(const s32, "core/main", func_001C9278);

INCLUDE_ASM(const s32, "core/main", func_001C9308);

INCLUDE_ASM(const s32, "core/main", func_001C9398);

INCLUDE_ASM(const s32, "core/main", func_001C9470);

INCLUDE_ASM(const s32, "core/main", func_001C95B0);

INCLUDE_ASM(const s32, "core/main", func_001C9698);

INCLUDE_ASM(const s32, "core/main", func_001C96F0);

INCLUDE_ASM(const s32, "core/main", func_001C9828);

INCLUDE_ASM(const s32, "core/main", func_001C9910);

INCLUDE_ASM(const s32, "core/main", func_001C9998);

INCLUDE_ASM(const s32, "core/main", func_001C9A80);

INCLUDE_ASM(const s32, "core/main", func_001C9B10);

INCLUDE_ASM(const s32, "core/main", func_001C9BE8);

INCLUDE_ASM(const s32, "core/main", func_001C9C78);

INCLUDE_ASM(const s32, "core/main", func_001C9DD0);

INCLUDE_ASM(const s32, "core/main", func_001C9ED0);

INCLUDE_ASM(const s32, "core/main", func_001C9F60);

INCLUDE_ASM(const s32, "core/main", func_001CA018);

INCLUDE_ASM(const s32, "core/main", func_001CA0B8);

INCLUDE_ASM(const s32, "core/main", func_001CA0F8);

INCLUDE_ASM(const s32, "core/main", func_001CA158);

INCLUDE_ASM(const s32, "core/main", func_001CA178);

INCLUDE_ASM(const s32, "core/main", func_001CA1B8);

INCLUDE_ASM(const s32, "core/main", func_001CA1F8);

INCLUDE_ASM(const s32, "core/main", func_001CA260);

INCLUDE_ASM(const s32, "core/main", func_001CA2E0);

INCLUDE_ASM(const s32, "core/main", func_001CA360);

INCLUDE_ASM(const s32, "core/main", func_001CA3E0);

INCLUDE_ASM(const s32, "core/main", func_001CA448);

INCLUDE_ASM(const s32, "core/main", func_001CA4A0);

INCLUDE_ASM(const s32, "core/main", func_001CA528);

INCLUDE_ASM(const s32, "core/main", func_001CA5B0);

INCLUDE_ASM(const s32, "core/main", func_001CA698);

INCLUDE_ASM(const s32, "core/main", func_001CA770);

INCLUDE_ASM(const s32, "core/main", func_001CA7E0);

INCLUDE_ASM(const s32, "core/main", func_001CA848);

INCLUDE_ASM(const s32, "core/main", func_001CA918);

INCLUDE_ASM(const s32, "core/main", func_001CA960);

INCLUDE_ASM(const s32, "core/main", func_001CA9C8);

INCLUDE_ASM(const s32, "core/main", func_001CAA08);

INCLUDE_ASM(const s32, "core/main", func_001CAAB8);

INCLUDE_ASM(const s32, "core/main", func_001CAC50);

INCLUDE_ASM(const s32, "core/main", func_001CACF0);

INCLUDE_ASM(const s32, "core/main", func_001CAD88);

INCLUDE_ASM(const s32, "core/main", func_001CADF0);

INCLUDE_ASM(const s32, "core/main", func_001CAEB8);

INCLUDE_ASM(const s32, "core/main", func_001CAF18);

INCLUDE_ASM(const s32, "core/main", func_001CB060);

INCLUDE_ASM(const s32, "core/main", func_001CB350);

INCLUDE_ASM(const s32, "core/main", func_001CB4C0);

INCLUDE_ASM(const s32, "core/main", func_001CB558);

INCLUDE_ASM(const s32, "core/main", func_001CB588);

INCLUDE_ASM(const s32, "core/main", func_001CB878);

INCLUDE_ASM(const s32, "core/main", func_001CBA48);

INCLUDE_ASM(const s32, "core/main", func_001CBB90);

INCLUDE_ASM(const s32, "core/main", func_001CBE20);

INCLUDE_ASM(const s32, "core/main", func_001CBF08);

INCLUDE_ASM(const s32, "core/main", func_001CBFC8);

INCLUDE_ASM(const s32, "core/main", func_001CC090);

INCLUDE_ASM(const s32, "core/main", func_001CC150);

INCLUDE_ASM(const s32, "core/main", func_001CC1F8);

INCLUDE_ASM(const s32, "core/main", func_001CC260);

INCLUDE_ASM(const s32, "core/main", func_001CC3D0);

INCLUDE_ASM(const s32, "core/main", func_001CC6B8);

INCLUDE_ASM(const s32, "core/main", func_001CC8B0);

INCLUDE_ASM(const s32, "core/main", func_001CC968);

INCLUDE_ASM(const s32, "core/main", func_001CCA00);

INCLUDE_ASM(const s32, "core/main", func_001CCAB8);

INCLUDE_ASM(const s32, "core/main", func_001CCB50);

INCLUDE_ASM(const s32, "core/main", func_001CCC28);

INCLUDE_ASM(const s32, "core/main", func_001CCD10);

INCLUDE_ASM(const s32, "core/main", func_001CCDD0);

INCLUDE_ASM(const s32, "core/main", func_001CCE90);

INCLUDE_ASM(const s32, "core/main", func_001CCF88);

INCLUDE_ASM(const s32, "core/main", func_001CD070);

INCLUDE_ASM(const s32, "core/main", func_001CD0A0);

INCLUDE_ASM(const s32, "core/main", func_001CD0D8);

INCLUDE_ASM(const s32, "core/main", func_001CD108);

INCLUDE_ASM(const s32, "core/main", func_001CD138);

INCLUDE_ASM(const s32, "core/main", func_001CD170);

INCLUDE_ASM(const s32, "core/main", func_001CD2E0);

INCLUDE_ASM(const s32, "core/main", func_001CD330);

INCLUDE_ASM(const s32, "core/main", func_001CD388);

INCLUDE_ASM(const s32, "core/main", func_001CD3D0);

INCLUDE_ASM(const s32, "core/main", func_001CD530);

INCLUDE_ASM(const s32, "core/main", func_001CD5D8);

s32 func_001CD818(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001CD820);

INCLUDE_ASM(const s32, "core/main", func_001CD858);

INCLUDE_ASM(const s32, "core/main", func_001CD8A0);

INCLUDE_ASM(const s32, "core/main", func_001CD8E8);

INCLUDE_ASM(const s32, "core/main", func_001CD938);

INCLUDE_ASM(const s32, "core/main", func_001CD950);

INCLUDE_ASM(const s32, "core/main", func_001CD970);

s32 func_001CD980(s32 *p) { return ((s32 *)p[0x40 / 4])[1] == 0; }

INCLUDE_ASM(const s32, "core/main", func_001CD990);

INCLUDE_ASM(const s32, "core/main", func_001CD9D0);

INCLUDE_ASM(const s32, "core/main", func_001CDA38);

INCLUDE_ASM(const s32, "core/main", func_001CDA60);

INCLUDE_ASM(const s32, "core/main", func_001CDAB0);

INCLUDE_ASM(const s32, "core/main", func_001CDAD8);

void func_001CDAF0(s32 *p) { s32 *object = (s32 *)p[0x40 / 4]; object[0x70 / 4] = 0; *(unsigned long long *)(object + 0x78 / 4) = 0; }

INCLUDE_ASM(const s32, "core/main", func_001CDB00);

INCLUDE_ASM(const s32, "core/main", func_001CDB18);

INCLUDE_ASM(const s32, "core/main", func_001CDB28);

INCLUDE_ASM(const s32, "core/main", func_001CDB38);

INCLUDE_ASM(const s32, "core/main", func_001CDB48);

s32 func_001CDB58(s32 *p, s32 value) { s32 *field = (s32 *)p[0x40 / 4]; s32 old = field[0xE8 / 4]; field[0xE8 / 4] = value; return old; }

INCLUDE_ASM(const s32, "core/main", func_001CDB68);

INCLUDE_ASM(const s32, "core/main", func_001CDB80);

INCLUDE_ASM(const s32, "core/main", func_001CDB98);

INCLUDE_ASM(const s32, "core/main", func_001CDBA8);

INCLUDE_ASM(const s32, "core/main", func_001CDBB8);

INCLUDE_ASM(const s32, "core/main", func_001CDC20);

INCLUDE_ASM(const s32, "core/main", func_001CDC38);

INCLUDE_ASM(const s32, "core/main", func_001CDDA8);

INCLUDE_ASM(const s32, "core/main", func_001CDED0);

INCLUDE_ASM(const s32, "core/main", func_001CDF10);

INCLUDE_ASM(const s32, "core/main", func_001CE080);

void func_001CE0F0(void) { *(s32 *)&D_00379530[0] = 0; func_001D0A58(1); }

INCLUDE_ASM(const s32, "core/main", func_001CE100);

INCLUDE_ASM(const s32, "core/main", func_001CE188);

INCLUDE_ASM(const s32, "core/main", func_001CE1D8);

INCLUDE_ASM(const s32, "core/main", func_001CE2A8);

INCLUDE_ASM(const s32, "core/main", func_001CE2B8);

INCLUDE_ASM(const s32, "core/main", func_001CE2E8);

INCLUDE_ASM(const s32, "core/main", func_001CE340);

INCLUDE_ASM(const s32, "core/main", func_001CE418);

INCLUDE_ASM(const s32, "core/main", func_001CE438);

INCLUDE_ASM(const s32, "core/main", func_001CE590);

INCLUDE_ASM(const s32, "core/main", func_001CE8D8);

INCLUDE_ASM(const s32, "core/main", func_001CE9C0);

INCLUDE_ASM(const s32, "core/main", func_001CEA80);

INCLUDE_ASM(const s32, "core/main", func_001CEBE8);

INCLUDE_ASM(const s32, "core/main", func_001CEC88);

INCLUDE_ASM(const s32, "core/main", func_001CEC98);

INCLUDE_ASM(const s32, "core/main", func_001CECA8);

INCLUDE_ASM(const s32, "core/main", func_001CECB8);

INCLUDE_ASM(const s32, "core/main", func_001CECC8);

INCLUDE_ASM(const s32, "core/main", func_001CECD8);

INCLUDE_ASM(const s32, "core/main", func_001CECF0);

INCLUDE_ASM(const s32, "core/main", func_001CF028);

INCLUDE_ASM(const s32, "core/main", func_001CF6D8);

INCLUDE_ASM(const s32, "core/main", func_001CFAF8);

INCLUDE_ASM(const s32, "core/main", func_001CFC88);

INCLUDE_ASM(const s32, "core/main", func_001CFD00);

INCLUDE_ASM(const s32, "core/main", func_001CFD98);

INCLUDE_ASM(const s32, "core/main", func_001CFE50);

INCLUDE_ASM(const s32, "core/main", func_001CFF20);

INCLUDE_ASM(const s32, "core/main", func_001CFFD0);
