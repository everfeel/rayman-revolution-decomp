#include "common.h"

/* func_00100008 through func_00102298; retail address range 0x00100000-0x001022E8. */

/* Entry and syscall wrappers. */

INCLUDE_ASM(const s32, "core/main", func_00100008);

void func_001000B0(void) // Exit syscall wrapper of the wrapper
{
    func_001C0840(0);
}

/* The syscall exits the thread; this compiler otherwise appends a return. */
#ifndef __INTELLISENSE__
__asm__(
    "glabel func_001000B8\n"
    "addiu $3, $0, 0x23\n"
    "syscall\n"
    "endlabel func_001000B8\n"
);
#else
void func_001000B8(void);
#endif

void func_001000C0(void) {
    func_001415E8();
}

void func_001000C8(void) {
    func_00102548();
}

void func_001000D0(void) {
}

void func_001000D8(void) {
    func_00100460();
}

void func_001000E0(void) {
    func_00100728();
}

/* Main-loop and lifecycle helpers. */

s32 func_001000E8(void)
{
    return func_00100360();
}

s32 func_00100108(void) { return func_00100378(); }

void func_00100128(void) { func_001007F8(); }

extern int D_00392F00;

void func_00100130(void)
{
    while (func_001000E8() == 0)
    {
        func_001000D8();

        D_00392F00 = 0;

        while (func_00100108() == 0)
        {
            func_00100128();
        }

        func_001000E0();
    }
}

INCLUDE_ASM(const s32, "core/main", func_00100188);

INCLUDE_ASM(const s32, "core/main", func_001001C0);

INCLUDE_ASM(const s32, "core/main", func_001001F0);

void func_00100288(void)
{
    while (1) {
        func_001C0B40();
    }
}

INCLUDE_ASM(const s32, "core/main", func_001002A0);

extern u8 D_00252230[9];

s32 func_00100360(void)
{
    return D_00252230[0] == 2;
}

/* Process-state predicates. */

s32 func_00100378(void)
{
    u8 state = D_00252230[0];
    return state == 2 || state == 6 || state == 8;
}

/* Startup routines. */

INCLUDE_ASM(const s32, "core/main", func_001003B0);

INCLUDE_ASM(const s32, "core/main", func_00100460);

INCLUDE_ASM(const s32, "core/main", func_00100728);

INCLUDE_ASM(const s32, "core/main", func_001007F8);

INCLUDE_ASM(const s32, "core/main", func_00100848);

INCLUDE_ASM(const s32, "core/main", func_00100A38);

INCLUDE_ASM(const s32, "core/main", func_00100BD0);

INCLUDE_ASM(const s32, "core/main", func_00100CD0);

INCLUDE_ASM(const s32, "core/main", func_00100F58);

INCLUDE_ASM(const s32, "core/main", func_00100F88);

INCLUDE_ASM(const s32, "core/main", func_00101068);

INCLUDE_ASM(const s32, "core/main", func_001010F8);

INCLUDE_ASM(const s32, "core/main", func_00101260);

INCLUDE_ASM(const s32, "core/main", func_001012A0);

INCLUDE_ASM(const s32, "core/main", func_00101320);

INCLUDE_ASM(const s32, "core/main", func_00101380);

INCLUDE_ASM(const s32, "core/main", func_00101438);

INCLUDE_ASM(const s32, "core/main", func_001015B8);

INCLUDE_ASM(const s32, "core/main", func_001016B0);

INCLUDE_ASM(const s32, "core/main", func_00101828);

void func_001018B8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001018C0);

INCLUDE_ASM(const s32, "core/main", func_00101908);

INCLUDE_ASM(const s32, "core/main", func_00101980);

INCLUDE_ASM(const s32, "core/main", func_001019A0);

INCLUDE_ASM(const s32, "core/main", func_001019C8);

INCLUDE_ASM(const s32, "core/main", func_00101A78);

INCLUDE_ASM(const s32, "core/main", func_00101B20);

INCLUDE_ASM(const s32, "core/main", func_00101BF0);

INCLUDE_ASM(const s32, "core/main", func_00101CB8);

INCLUDE_ASM(const s32, "core/main", func_00101CF0);

INCLUDE_ASM(const s32, "core/main", func_00101D90);

INCLUDE_ASM(const s32, "core/main", func_00101E30);

INCLUDE_ASM(const s32, "core/main", func_00102138);

INCLUDE_ASM(const s32, "core/main", func_00102160);

INCLUDE_ASM(const s32, "core/main", func_00102298);
