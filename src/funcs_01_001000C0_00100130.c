#include "common.h"

// funcs_01_001000C0_00100130.c
// -O2

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

void func_001007F8(void);
extern s32 func_00100CD0(void);

int func_00100360();

int func_001000E8(void)
{
    int result = func_00100360();
    return result;
}

s32 func_00100108(void) { 
    return func_00100378(); 
}

void func_00100128(void) { 
    func_001007F8();
 }

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