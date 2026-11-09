#ifndef RAYMAN_COMMON_H
#define RAYMAN_COMMON_H

/* Signed integer types */
typedef signed char        int8;
typedef signed short       int16;
typedef signed int         int32;
typedef signed long long   int64;

/* Unsigned integer types */
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

/* Short aliases */
typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

/* Standard fixed-width aliases */
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

/* Floating-point types */
typedef float              float32;
typedef double             float64;
typedef float              f32;
typedef double             f64;

/* Boolean types */
typedef int                bool;
typedef int                bool_t;

/* Pointer-sized types for 32-bit EE */
typedef unsigned int       uintptr;
typedef signed int         intptr;

/* Volatile signed types */
typedef volatile signed char      vs8;
typedef volatile signed short     vs16;
typedef volatile signed int       vs32;
typedef volatile signed long long vs64;

/* Volatile unsigned types */
typedef volatile unsigned char      vu8;
typedef volatile unsigned short     vu16;
typedef volatile unsigned int       vu32;
typedef volatile unsigned long long vu64;

/* Generic pointer aliases */
typedef void *              pointer;
typedef const void *        const_pointer;

#if defined(__INTELLISENSE__)
#define INCLUDE_ASM(type, path, name) type name()
#else
__asm__(".include \"include/macro.inc\"");
#define INCLUDE_ASM(type, path, name) \
    __asm__(".set noreorder\n.include \"asm/nonmatchings/" path "/" #name ".s\"\n.set reorder")
#endif

#define ASM_FUNC(type, path, name) INCLUDE_ASM(type, path, name)

#endif
