#ifndef RAYMAN_COMMON_H
#define RAYMAN_COMMON_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;

#if defined(__INTELLISENSE__)
#define INCLUDE_ASM(type, path, name) type name()
#else
__asm__(".include \"include/macro.inc\"");
#define INCLUDE_ASM(type, path, name) \
    __asm__(".set noreorder\n.include \"asm/nonmatchings/" path "/" #name ".s\"\n.set reorder")
#endif

#define ASM_FUNC(type, path, name) INCLUDE_ASM(type, path, name)

#endif
