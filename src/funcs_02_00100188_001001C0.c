#include "common.h"

// funcs_02_00100188_001001C0.c 
// -O1 -fno-omit-frame-pointer

extern void *D_00252100[];
extern unsigned char main_BSS_START[];
extern int func_00220568(void *arg1, void *arg2);

int func_00100188(void)
{
    return func_00220568(D_00252100, main_BSS_START);
}

extern void *D_00252100[];
extern int func_002205C0(int *arg1);

int func_001001C0(void)
{
    return func_002205C0((int *)D_00252100);
}