#include "common.h"

typedef struct __attribute__((packed)) {
    u64 value;
} UnalignedU64;

/* 16-byte unaligned record: two packed doublewords. Copying it as one
   aggregate keeps both loads before both stores. */
typedef struct
{
    UnalignedU64 lo;
    UnalignedU64 hi;
} Unaligned16;
typedef struct __attribute__((packed)) {
    u64 value;
    u32 extra;
} UnalignedU96;
typedef struct { UnalignedU64 v; u32 w; } S12_flash;

/* Plain 12-byte record; the codegen for copying it is the ldl/ldr + sdl/sdr
   doubleword pair plus a trailing word, which is what retail does. */
typedef struct {
    u32 a;
    u32 b;
    u32 c;
} Struct12;

typedef struct StartupBlock {
    u8 state;
    u8 unk0001[0x43];
    s16 unk44;
    s16 unk46;
    u8 unk0048[0xF8];
    u32 unk0140;
    u32 unk0144;
    u32 unk0148;
    u8 unk014C[0x0C];
    u32 unk0158;
    u32 unk015C;
    u32 unk0160;
    u32 saved;
    u32 active;
    u32 unk016C;
    u32 pending;
    u32 unk0174;
    u8 recordA[0x54];
    u8 recordB[0x54];
    u8 unk0220[0x11D4];
    u8 unk13F4;
    u8 unk13F5;
    u8 unk13F6;
    u8 unk13F7;
    u8 unk13F8;
    u8 unk13F9;
    u8 unk13FA;
    u8 flag;
    u8 unk13FC;
} StartupBlock;

typedef struct ActorNodeTable {
    s32 count;
    u8 unk04[0x10];
    u8 *nodes;
    u8 unk18[4];
    u32 *cursor;
} ActorNodeTable;

extern StartupBlock D_00252230;
extern ActorNodeTable D_00252210;
extern u8 D_00252274[5];
extern s32 D_00252270[];
extern s32 D_002522EC[];
extern s32 D_00393C00;
extern s32 D_0039347C;
extern s16 D_003947A8[];
extern u8 D_0034A380[];
extern s32 D_0034A490[];
extern s32 D_0034A48C[];
extern s32 D_0034A494[];
extern s32 D_00392FA8;
extern s32 D_00392FA4;
extern s32 D_00392FA0;
extern s32 D_00392F24;
extern u8 D_0034A300[];
extern u8 D_003931E9;
extern s32 D_00393294;
extern s32 D_00393028[];
extern s32 D_00254030[];
extern s32 D_00254034[];
extern f32 D_00393050;
extern s32 D_00258038[];
extern f32 D_003924C4;
extern s32 D_0026E038[];

extern s32 D_00393178;
extern s32 D_00393174;
extern u32 D_00330990[];
extern s32 D_0026F210[];
extern u8 D_0026F220[];
extern s32 D_0026DFB8[];
extern u8 D_0026BEE8[];
extern s32 D_00330A18[];
extern f32 D_003930F8;
extern u8 D_0026F1E0[];
s32 func_0011DDE8(s32 arg0);
void func_00127AE8(u8 *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4);
extern s32 D_00330E78[];
extern s32 D_003933DC;
extern u8 D_00393C20;
extern f32 D_0026F290[];
extern f32 D_0026F280[];
extern f32 D_003930A0;
extern f32 D_0039309C;
extern s32 D_003930A4;

/* NOTE: deliberately no `const` here. Old gcc (2.x / ee-gcc) can treat `const`
 * on a function declaration as the "const function" attribute (no side effects,
 * result depends only on args), which lets -O2 delete any call whose result is
 * unused and CSE/hoist the rest. Several of these exist purely for side effects. */
extern void func_00112FF8(u8 *arg0, u8 arg1);
extern s32 func_0010DD18();
extern s32 func_0010E920();
extern s32 func_0010C9F0();
extern s32 func_00115BE0(f32 *, s16 *);
extern s32 func_00115C60();
extern s32 func_00103B70();
extern s32 func_0014BA58();
extern void func_00226A34();
extern s32 func_00118DA0();
extern s32 func_0011C680();
extern s32 func_00154AF8();
extern s32 func_001558C8();
extern s32 func_00152200();
extern s32 func_00151B38();
extern s32 func_00157DE8();
extern s32 func_001104A0();
extern s32 func_00127330();
extern s32 func_00158E60();
extern s32 func_00158EC8();
extern s32 func_00159300();
extern void func_00159308(u8 *);
extern u8 *func_0010CB00();
extern void func_00120D48();
extern void func_0010DE30();
extern void func_001106D0();

extern s32 D_00252214[];
// existing: extern u8 D_0026F220[];
extern s32 D_00330A20[];
extern s32 D_003890C8[];
extern f32 D_00392500;
extern f32 D_00392504;
extern u8 D_00393018[];
extern s32 D_003930AC;
extern s32 D_00393134;
extern s32 D_00393138;
extern s32 D_00393148[];
extern u8 D_0039343E;
extern u8 D_0039343F;
extern u8 D_00393440;

// moved from block scope
extern s32 func_00144D98();
extern s32 func_00145168();
extern s32 func_001451C0();
extern s32 func_001454A0();
extern s32 func_001454C8();
extern s32 func_001454F8();
extern s32 func_00145560();
extern s32 func_00193038();
extern u8 * func_00211980(u8 *s, s32 c);

// moved from block scope (funcs)
extern s32 func_0018C180();
extern s32 func_0018C5B0();
extern s32 func_0018C5E0();
extern s32 func_0018C5F0();
extern s32 func_0018C5F8();
extern s32 func_0018C608();
extern s32 func_0018C660();
extern s32 func_0018C820(s32, f32);
extern s32 func_0018C900(s32, f32);
extern s32 func_0018DC88();

// targeted extern var hoist
f32 func_00217DA0(f32);
void func_0014E7F8(s32 *);

// moved from block scope
extern void func_001224F8();
extern void func_00124738();
extern s32 func_0018FE28();
extern s32 func_00190B80();
extern s32 func_00192250();

// targeted extern var hoist
extern s32 func_00117B28();
extern void func_00118D58(u8 *, f32, f32);

INCLUDE_ASM(const s32, "core/main", func_0010D408);

INCLUDE_ASM(const s32, "core/main", func_0010D5C8);

INCLUDE_ASM(const s32, "core/main", func_0010DA20);

void func_00120D48(s32);
void func_0010DBB8(void)
{
    s32 i = 0;
    s32 off = 0;
    do {
        func_00120D48(*(s32 *)((u8 *)&D_00393C00 + 0xC) + off);
        i++;
        off += 0x54;
    } while ((u32)i < 0x201);
}

void func_0010DC00(void)
{
    *(s32 *)((u8 *)&D_00393C00 + 0xC) = (s32)func_00157DE8(D_0039347C, 0xA854);
}

INCLUDE_ASM(const s32, "core/main", func_0010DC28);

s32 func_0014AB00(s32);

void func_0010DCA8(void)
{
    D_00393C00 = func_0014AB00(0x201);
}

void func_0010DCC8(void)
{
    func_0010DBB8();
    func_0010DC28();
}

void func_0010DCE8(void)
{
    func_0010DC00();
    func_0010DCA8();
    func_0010DBB8();
    func_0010DC28();
}

INCLUDE_ASM(const s32, "core/main", func_0010DD18);

void func_0010DE30(s32 arg0)
{
    s32 base = D_00393C00;
    s32 *counter = (s32 *)((u8 *)&D_00393C00 + 8);

    /* Test the offset, not base + 0x8000: identical result modulo 2^32 and one
       instruction shorter, which keeps the function inside its retail slot. */
    if (arg0 - base != 0x8000)
        D_003947A8[(arg0 - base) >> 6] = 0;
    counter[0] = counter[0] - 1;
}

INCLUDE_ASM(const s32, "core/main", func_0010DE70);

INCLUDE_ASM(const s32, "core/main", func_0010DED0);

INCLUDE_ASM(const s32, "core/main", func_0010E638);

extern s32 D_003932D0;
extern s32 func_0010E8B0();

s32 func_0010E8B0(u32 *a1)
{
  s32 result;
  s32 v3;

  result = *a1 & 9LL;
  if ( result != 0 )
  {
    func_001224F8((s32)a1[9], *(s32 *)(a1[7] + 36), (s32)a1[8]);
    v3 = (s32)a1[2];
    result = D_003932D0;
    for ( a1[10] = result; v3 != 0; v3 = *(s32 *)(v3 + 20) )
      result = func_0010E8B0(v3);
  }
  return result;
}

s32 func_0010E920(s32 a1)
{
    s32 result;
    u8 *i;

    result = func_0010C9F0(a1) & 2;
    if (result != 0)
    {
        i = *(u8 **)(a1 + 8);
        while (i != 0)
        {
            result = func_0010E8B0((s32 *)i);
            i = *(u8 **)(i + 0x14);
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0010E970);

INCLUDE_ASM(const s32, "core/main", func_0010EB68);

INCLUDE_ASM(const s32, "core/main", func_0010EC88);

s32 func_0010EDF8(u8 *p)
{
    u8 *a = *(u8 **)(p + 0x4);
    u8 *b = *(u8 **)(a + 0x0);
    u8 *c = *(u8 **)(b + 0x4);
    u8 *d = *(u8 **)(c + 0x0);

    return *(u16 *)(d + 0x18);
}

INCLUDE_ASM(const s32, "core/main", func_0010EE10);

INCLUDE_ASM(const s32, "core/main", func_0010EED8);

s32 func_0010EF58(u8 *p, s32 value)
{
    u8 *a = *(u8 **)(p + 0x4);
    u8 *b = *(u8 **)(a + 0x0);

    return value == *(s32 *)(b + 0x8);
}

INCLUDE_ASM(const s32, "core/main", func_0010EF70);

INCLUDE_ASM(const s32, "core/main", func_0010F0F0);

s32 func_0010F2E8(u8 *arg0)
{
    u8 *p = *(u8 **)(arg0 + 4);
    s32 result = 0;

    p = *(u8 **)p;
    if (p[0x86] != 0)
        result = (*(u32 *)(p + 4) ^ *(u32 *)(p + 0x80)) == 0;
    return result;
}

s32 func_0010F318(p) u8 *p;
{
    return *(u8 *)(*(u32 *)(*(u32 *)(p + 4)) + 0x87) != 0;
}

INCLUDE_ASM(const s32, "core/main", func_0010F330);

INCLUDE_ASM(const s32, "core/main", func_0010F448);

INCLUDE_ASM(const s32, "core/main", func_0010F6D0);

INCLUDE_ASM(const s32, "core/main", func_0010F8E0);

INCLUDE_ASM(const s32, "core/main", func_0010FAA8);

INCLUDE_ASM(const s32, "core/main", func_0010FC40);


/* Source: gameplay/region_0011_before_player.c */
/* func_00110340 through func_00110F00; retail address range 0x00110000-0x00110F20. */


s32 func_00110340(u8 *a1, u8 *a2)
{
    u16 v2 = *(u16 *)(a2 + 4);
    s32 *v3 = *(s32 **)(a1 + 0x10);
    u8 *base = (u8 *)(20 * v2 + *v3);
    s32 v = *(s32 *)(base + 4);
    if (v != 0 && v2 < *(u16 *)((u8 *)v3 + 8))
        return v;
    return *(s32 *)(20 * v2 + **(s32 **)((u8 *)(*(s32 *)(a1 + 0x14)) + 0x14) + 4);
}

INCLUDE_ASM(const s32, "core/main", func_00110398);

INCLUDE_ASM(const s32, "core/main", func_001104A0);

INCLUDE_ASM(const s32, "core/main", func_001106D0);

s32 func_001108F8(u8 *a1, u16 a2)
{
    u8 *base;

    if (a2 >= *(u16 *)(a1 + 0x18)) return 0;
    base = *(u8 **)(*(u8 **)a1 + 0x10) + 10 * a2;
    if (*(u16 *)(base + 2) == 0xFFFF) return 0;
    return **(s32 **)a1 + 12 * *(u16 *)(base + 2);
}

s32 func_00110950(u16 *a1, u16 a2, u8 *a3)
{
    f32 v6[4];
    s32 v4;

    if (a2 >= a1[12])
        return 0;
    v4 = *(s32 *)(*(s32 *)a1 + 0x10) + 10 * a2;
    if (*(u16 *)v4 == 0xFFFF)
        return 0;
    func_00115BE0(v6, (s32 *)(*(s32 *)(*(s32 *)a1 + 4) + *(u16 *)v4 * 8));
    func_00115C60((f32 *)a3, v6);
    return (s32)a3;
}

void func_001109D8(void) { func_00115AD8(); }

INCLUDE_ASM(const s32, "core/main", func_001109E0);

void func_00110E88(void)
{
    u8 *base = D_0034A380;

    func_00110EC0(*(s32 *)(base + 0x110));
    func_00110EE0(*(s32 *)(base + 0x10C));
    func_00110F00(*(s32 *)(base + 0x114));
}

void func_00110EC0(u32 arg0)
{
    s32 value = arg0;

    if (arg0 >= 0x7F)
        value = 0x7E;
    D_0034A490[0] = value;
    func_0015CF28((u8 *)D_00392FA8, value);
}

void func_00110EE0(u32 arg0)
{
    s32 value = arg0;

    if (arg0 >= 0x80)
        value = 0x7F;
    D_0034A48C[0] = value;
    func_0015CF28((u8 *)D_00392FA4, value);
}

void func_00110F00(u32 arg0)
{
    s32 value = arg0;

    if (arg0 >= 0x80)
        value = 0x7F;
    D_0034A494[0] = value;
    func_0015CF28((u8 *)D_00392FA0, value);
}


/* Source: player/rayman_init.c */
/* func_00110F20 through func_00110F20; retail address range 0x00110F20-0x00110F78. */


void func_00110F20(void)
{
    typedef struct __attribute__((packed)) { u32 v; s8 b0, b1, b2; } S7;
    u8 *p = (u8 *)D_0034A300;

    *(s32 *)D_0034A300 = 0;
    *(s16 *)(p + 0x68) = -1;
    p[0x4A] = 0;
    *(s32 *)(p + 0x44) = 0;
    p[0x52] = 0;
    *(S7 *)(p + 0x4B) = *(S7 *)D_00393018;
}


/* Source: gameplay/region_0011_tail.c */
/* func_00110F78 through func_0011FFE0; retail address range 0x00110F78-0x00120000. */







extern s32 *D_003930B4;
extern s32 D_003930CC;
extern s32 D_003930F4;

extern s8 D_0025224F[8];

extern s32 D_00393020;
extern s32 D_00393098;
extern s32 D_0039310C;

extern f32 D_0039304C;
extern f32 D_00393054;
extern s32 D_00393084;

extern f32 D_00393048;
extern s32 D_003930C4;
extern u8 D_003930E1;
extern u8 D_003930E2;
extern f32 D_003930F0;
extern s32 D_00393130;

extern u8 D_0034A34B[];
extern s32 func_00226B48();
extern s32 func_00227040();
extern void func_00226A34();

void func_00110F78(s32 a1)
{
    if ((u32)func_00226B48(a1) < 7) {
        func_00226A34(D_0034A34B, a1);
        return;
    }
    func_00227040(D_0034A34B, a1, 6);
    D_0034A34B[6] = 0;
}


/* Arrays (not scalars) so the compiler addresses them absolutely instead of
 * gp-relative, like the retail code does. */
extern u16 D_0034A348[];
extern u8 D_0034A34B[];
extern u8 D_0034A35C[];
extern s32 D_0034A340[];
extern u8 D_002535E6[];
extern u8 D_00253604[];
extern u8 D_00252450[];
/* These three are close enough to gp for a one-instruction store, as retail. */
extern s32 D_00393058;
extern s32 D_00393090;
extern s32 D_00393094;
extern s32 D_0026F0DC[];
extern s32 D_003932B4;
extern s32 D_00355448[];
extern void func_00112C10();
extern s32 func_00117AD8(s16, s16);
extern void func_001241D8();
extern void func_001266B0();
extern s32 func_00149880();

/* Address of a byte-sized counter. */
u8 *func_00110FE8(void)
{
    return D_0034A34B;
}

/* Sets the 16-bit value at D_0034A348. */
void func_00110FF8(u16 value)
{
    D_0034A348[0] = value;
}

/* Returns the 16-bit value at D_0034A348. */
u16 func_00111008(void)
{
    return D_0034A348[0];
}

extern s32 func_00226B48(void *);
extern s32 func_00227040(u8 *, s32, s32);

void func_00111018(s32 a1)
{
    if ((u32)func_00226B48((void *)a1) < 0xC) {
        func_00226A34(D_0034A35C, a1);
        return;
    }
    func_00227040(D_0034A35C, a1, 0xB);
    D_0034A35C[0xB] = 0;
}


/* Address of a byte-sized table. */
u8 *func_00111088(void)
{
    return D_0034A35C;
}

/* Sets the 32-bit value at D_0034A340. */
void func_00111098(s32 value)
{
    D_0034A340[0] = value;
}

/* Returns the 32-bit value at D_0034A340. */
s32 func_001110A8(void)
{
    return D_0034A340[0];
}

INCLUDE_ASM(const s32, "core/main", func_001110B8);

s32 func_00111128(void)
{







    s32 result = 0;
    u8 *p;

    if (func_001454A0() != 0) {
        if (func_001454C8() != 0) {
            p = (u8 *)D_0034A300;
            if (p[0x4A] != 0) {
                if (func_00145560(p + 0x40) != 0)
                    return func_001451C0(p);
            } else {
                s32 v = func_00144D98();
                if (func_001454F8(v) != 0)
                    return func_00145168(p);
            }
        } else {
            return 0;
        }
    }
    return result;
}

s32 func_001111C0(s32 arg0)
{
    func_00110F20();
    func_00110F78(arg0);
    func_00144DA8();
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001111F8);

s32 func_001112C8(u8 *a1)
{
    s32 v2 = *(s32 *)(a1 + 8);
    s32 v3;
    if (v2 != 0)
    {
        do {
            func_001112C8((u8 *)v2);
            v2 = *(s32 *)((u8 *)v2 + 0x14);
        } while (v2 != 0);
        v3 = *(s32 *)a1;
    }
    else
    {
        v3 = *(s32 *)a1;
    }
    if (v3 == 2) func_001DDEB8(*(s32 *)(a1 + 4));
    return 1;
}

/* Byte-matching; returning the callee's result avoids a sibling call. */
s32 func_00111330(void)
{
    func_00110E88();
    return func_001112C8(D_00392F24);
}

INCLUDE_ASM(const s32, "core/main", func_00111358);

void func_001113F0(void)
{
    u8 *p = (u8 *)D_0034A300;

    *(s16 *)(p + 0x68) = -1;
    p[0x4A] = 0;
    *(s32 *)(p + 0x44) = 0;
    func_00145168(p);
}

void func_00111428(void)
{
    s32 *p = (s32 *)D_00393294;

    if (p != 0) {
        while (p != 0) {
            s32 *arg = p;
            p = *(s32 *)p;
            func_0014CBE8(D_0039347C, (s32)arg);
        }
    }
    D_00393294 = 0;
    D_003931E9 = 0;
}

void func_00111470(void)
{
    if (D_003931E9 == 0)
    {
        func_00111428();
        D_00393294 = func_00140FE8();
    }
}

s32 func_001114A0(void) { return 0; }

s32 func_001114A8(void)
{
    u8 *p = (u8 *)D_00393294;
    s32 count = 0;

    while (p != 0)
    {
        p = *(u8 **)p;
        count++;
    }
    return count;
}

s32 func_001114E0(s32 a1)
{
    u8 *p = (u8 *)a1;
    s32 v2 = *(s32 *)(p + 8);
    s32 v3;
    if (v2 != 0)
    {
        do {
            func_001114E0(v2);
            v2 = *(s32 *)((u8 *)v2 + 0x14);
        } while (v2 != 0);
        v3 = *(s32 *)p;
    }
    else
    {
        v3 = *(s32 *)p;
    }
    if (v3 == 2) func_001DDC90(*(s32 *)(p + 4));
    return 1;
}

s32 func_001114E0(s32);

s32 func_00111548(void)
{
    return func_001114E0(D_00392F24);
}

INCLUDE_ASM(const s32, "core/main", func_00111568);

void func_001115F8(s32 *p, s32 value) { p[0] = value; }

s32 func_00111600(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

INCLUDE_ASM(const s32, "core/main", func_00111608);

INCLUDE_ASM(const s32, "core/main", func_00111658);

INCLUDE_ASM(const s32, "core/main", func_00111758);

s32 func_001118A8(u8 *p)
{
    u8 *a = *(u8 **)(p + 0x4);
    u8 *b = *(u8 **)(a + 0x4);

    return (*(u8 *)(b + 0x34) >> 6) & 1;
}

void *func_001118C0(void)
{
    return (void *)D_00252230.unk0158;
}

void *func_001118D0(void *arg1)
{
    u8 *p = *(u8 **)((u8 *)arg1 + 4);

    if (p == 0)
        return 0;
    if (func_00111600(p) != 0)
        return p;
    return 0;
}

void func_00111910(void)
{
    u8 *p;

    func_00112120();
    p = (u8 *)&D_00252210;
    if (*(s32 *)p != 0)
    {
        *(s32 *)p = 0;
        *(s32 *)(p + 0x18) = 0;
        *(s32 *)(p + 0x14) = 0;
        *(s32 *)(p + 0x1C) = 0;
        *(s32 *)(p + 0x10) = 0;
    }
}

void func_00111950(u8 a1)
{
    u8 *v2;
    u8 *i;

    v2 = (u8 *)D_00252214[0];
    if (v2 != 0)
    {
        i = *(u8 **)(v2 + 0xC);
        for (;;)
        {
            func_00103B70(i, a1);
            v2 = *(u8 **)v2;
            if (v2 == 0) break;
            i = *(u8 **)(v2 + 0xC);
        }
    }
}

INCLUDE_ASM(const s32, "core/main", func_001119A0);

INCLUDE_ASM(const s32, "core/main", func_001119F8);

INCLUDE_ASM(const s32, "core/main", func_00111FC8);

INCLUDE_ASM(void, "core/main", func_00112120);

INCLUDE_ASM(const s32, "core/main", func_00112200);

INCLUDE_ASM(const s32, "core/main", func_00112270);

s32 func_00112370(s32 arg0, s32 arg1)
{
    if (arg1 == 0)
        return 0;
    return func_001123B0(arg0) == arg1;
}

extern s32 D_0025222C[];

s32 func_001123B0(u8 *arg0)
{
    u8 *a;
    u8 *b;
    s32 idx;

    if (arg0 == 0) goto zero;
    a = *(u8 **)(arg0 + 4);
    b = *(u8 **)(a + 4);
    if (b == 0) goto zero;
    if (a != 0) goto lookup;
zero:
    return 0;
lookup:
    idx = *(u16 *)(b + 0x12) - 0x8000;
    return *(s32 *)(D_0025222C[0] + idx * 4);
}

/* Sets the startup state. */
void func_001123F8(u8 state)
{
    D_00252230.state = state;
}

INCLUDE_ASM(const s32, "core/main", func_00112408);

s32 func_001124D8(void) { return 0; }

/* Address right after the startup state byte. */
u8 *func_001124E0(void)
{
    return (u8 *)&D_00252230 + 1;
}

INCLUDE_ASM(const s32, "core/main", func_001124F0);

void func_001125B0(u8 *a1)
{
    u64 func_0021D408();
    s32 v1 = func_0014BA58();
    s32 v3 = (s32)func_0021D408(v1, D_00252230.unk0220[0x11D2]);

    func_00226A34(a1, (s32)((u8 *)&D_00252230.unk0220[2] + 30 * v3));
}

u8 *func_00112610(void)
{
    return D_002535E6;
}

u8 *func_00112620(void)
{
    return D_00253604;
}

extern s32 D_0034C3DC[];
extern s32 D_00389AD0[];
extern s32 D_00371428[];
extern s32 D_003E1AF0[];
extern s32 D_0038E278[];
extern s32 D_00393314;
extern s32 D_00393130;
extern void func_00226A34();
extern void func_002249D4();
extern void func_00226310();
extern void func_001C7BB8();
extern void func_0018C4D8();
extern void func_00168A10();
extern void func_00168A40();
extern void func_00168AA8();
extern void func_00169EB8();
extern void func_001DE2F0();
extern void func_00215EC8(u32 address, s32 command, u64 value);
extern void func_00222978();

void func_00112630(s32 arg)
{
    func_00226A34((s32)D_0025224F, arg);
}

void func_00112640(s32 arg0)
{
    if (arg0 != 0)
    {
        func_00226A34(D_002535E6, arg0);
        return;
    }
    *(UnalignedU64 *)D_002535E6 = *(UnalignedU64 *)&D_00393028;
}

void func_00112680(s32 arg0)
{
    if (arg0 != 0)
    {
        func_00226A34(D_00253604, arg0);
        return;
    }
    *(UnalignedU64 *)D_00253604 = *(UnalignedU64 *)&D_00393028;
}

s8 *func_001126C0(void)
{
    return D_0025224F;
}

void func_001126D0(void)
{
    D_0025224F[0] = 0;
}

s32 func_001126E0(void) { return D_0025224F[0] != 0; }

void func_001126F0(s32 arg, s32 value)
{
    func_00112708((s32)D_00252270, arg, value & 0xFF);
}

INCLUDE_ASM(const s32, "core/main", func_00112708);

INCLUDE_ASM(const s32, "core/main", func_00112828);

INCLUDE_ASM(const s32, "core/main", func_001129F0);

INCLUDE_ASM(const s32, "core/main", func_00112AA0);

void func_00112B10(void)
{
    u8 *base = (u8 *)&D_00252230;

    *(s32 *)(base + 0x138) = 0;
    func_00112B48(*(s16 *)(base + 0x44));
    func_00112D28();
}

void func_00112B38(void)
{
    func_00112C10(*(s16 *)D_00252274);
}

INCLUDE_ASM(const s32, "core/main", func_00112B48);

void func_00112C10(u32 value)
{
    func_00117900((s16)value, *(s16 *)D_002522EC);
}

INCLUDE_ASM(const s32, "core/main", func_00112C28);

void func_00112D10(void)
{
    func_00118DA0(D_00252230.unk44, D_00252230.unk46,
                  *(s32 *)((u8 *)&D_00252230 + 0xB8));
}

void func_00112D28(void)
{
    u8 *p = (u8 *)&D_00252230 + 0xB8;

    ((void (*)(int **, s32))func_00117B28)((int **)p, 103);
    p -= 0xB8;
    ((void (*)(f32 *, f32, f32))func_00118D58)(*(f32 **)(p + 0xB8), 1.5f, 0.75f);
    func_00118DA0(*(s16 *)(p + 0x44), *(s16 *)(p + 0x46), *(s32 *)(p + 0xB8));
}

s32 func_00112D80(u8 *arg0)
{

    u8 buf[0x50];
    u8 *v2;
    u8 *v3;

    func_00226A34(buf, arg0);
    v2 = func_00211980(buf, 0x24);
    v3 = func_00211980(buf, 0x23);
    func_00112630((s32)arg0);
    func_00112680((s32)v3);
    if (v3 != 0) *v3 = 0;
    func_00112640((s32)v2);
    func_001123F8(6);
}

s32 func_00112E08(void)
{
    return (u32)D_00393020 < 1;
}

void func_00112E18(void) { D_00393020 = 0; }

void func_00112E20(void)
{
    D_00393020 = 1;
}

void func_00112E30(void) {
}

u8 func_00112E38(void)
{
    return D_00252450[0];
}

void func_00112E48(void)
{
    u8 *base = (u8 *)&D_00252230;
    u8 flag = base[0x13FA];

    base[0x220] = 1;
    if (flag == 0)
        func_0014B940();
}

void func_00112E70(void)
{
    u8 *base = (u8 *)&D_00252230;
    u8 flag = base[0x13FA];

    base[0x220] = 0;
    if (flag == 0)
        func_0014B960();
}

INCLUDE_ASM(const s32, "core/main", func_00112E98);

INCLUDE_ASM(const s32, "core/main", func_00112F10);

void func_00112F88(u32 *arg0, u16 arg1, s32 arg2)
{
    u8 *obj = (u8 *)*arg0;
    u8 *base = (u8 *)*(u32 *)(obj + 0x88);
    s32 count;
    u8 v;
    if (base == 0) return;
    v = (u8)arg2;
    count = *(s32 *)(obj + 0x8C);
    if (arg1 == 0xFFFF) goto loop;
    *(u8 *)(base + arg1 * 0x20 + 0x10) = v;
    return;
loop:
    {
        u8 *end = base + count * 0x20;
        u8 *p;
        for (p = base; (u32)p < (u32)end; p += 0x20)
            *(u8 *)(p + 0x10) = v;
    }
}

void func_00112FF8(u8 *arg0, u8 arg1)
{
    u32 value;

    if (arg0 == 0)
        return;
    value = *(u32 *)(arg0 + 0x30);
    if (arg1 != 0)
        *(u32 *)(arg0 + 0x30) = value & 0xFFFFFF7C;
    else
        *(u32 *)(arg0 + 0x30) = value | 0x83;
}

INCLUDE_ASM(const s32, "core/main", func_00113030);

u8 *func_001130F0(void)
{
    u8 *p = func_00157DE8(D_0039347C, 0x1C);

    *(s32 *)p = 0;
    *(s32 *)(p + 4) = 0;
    *(s32 *)(p + 8) = 0;
    return p;
}

s32 func_00113120();

s32 func_00113120(s32 arg1)
{
    return func_0014CBE8(D_0039347C, arg1);
}

extern s32 D_0039347C;

void func_00113148(u32 *a1)
{
    if (*a1 == 0) {
        u32 *p = (u32 *)func_00157DE8(D_0039347C, 0xC);
        *a1 = (u32)p;
        *(u32 *)p = 0;
        *(u32 *)((u8 *)*a1 + 4) = 0;
        *(u32 *)((u8 *)*a1 + 8) = 0;
    }
}

INCLUDE_ASM(const s32, "core/main", func_00113198);

INCLUDE_ASM(const s32, "core/main", func_00113280);

void func_001134C8(s32 a0, u8 *a1)
{
    s32 result;
    s32 cmp;

    if (*(s32 *)(a1 + 0) != 0) { result = *(s32 *)(a1 + 4); *(s32 *)(*(s32 *)(a1 + 0) + 4) = result; }
    if (*(s32 *)(a1 + 4) != 0) { result = *(s32 *)(a1 + 0); *(s32 *)(*(s32 *)(a1 + 4) + 0) = result; }
    if (*(s32 *)(a1 + 8) != 0)
    {
        *(s32 *)(*(s32 *)(a1 + 8) + 8) -= 1;
        __asm__ volatile("" ::: "memory");
        if (**((s32 **)(a1 + 8)) == (s32)a1)
        {
            **(s32 **)(a1 + 8) = *(s32 *)(a1 + 0);
            __asm__ volatile("" ::: "memory");
        }
        cmp = (*(s32 **)(a1 + 8))[1];
        if (cmp == (s32)a1)
            (*(s32 **)(a1 + 8))[1] = *(s32 *)(a1 + 4);
    }
    *(s32 *)(a1 + 0) = 0;
    *(s32 *)(a1 + 4) = 0;
    *(s32 *)(a1 + 8) = 0;
    func_00113120((s32)a1);
}

INCLUDE_ASM(const s32, "core/main", func_00113548);

s32 func_00113760(s32 arg0, u16 arg1)
{
    s32 result = func_00113548(arg0, arg1, 0);

    if (__builtin_expect(result != 0, 0))
        return func_00159300(result);
    return 0;
}

INCLUDE_ASM(void, "core/main", func_00113798);

s32 func_00113838(const s32 *p) { return *(const s32 *)((const u8 *)p + (32)); }

s32 func_00113840(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_00113848(u8 *p, s32 index)
{
    return *(s32 *)(p + 0x18) + (s16)index * 0xC;
}

u8 *func_00113868(void)
{
    u8 *p = func_00157DE8(D_0039347C, 0xC);

    *(s16 *)(p + 8) = 0;
    *(s32 *)p = 0;
    *(s32 *)(p + 4) = 0;
    *(s16 *)(p + 0xA) = 0;
    return p;
}

void func_001138A0(void) {
}

s32 func_001138A8(s32 *p, s32 value) { p[6] = value; p[7] = 1; return 1; }

INCLUDE_ASM(const s32, "core/main", func_001138B8);

typedef struct {
    u8 data[16];
    u32 value;
} S00113998Blk;

typedef struct {
    S00113998Blk *arr;
} S00113998Hdr;

void func_00113998(S00113998Hdr *a, S00113998Hdr *b, u16 i, u16 j)
{
    a->arr[i] = b->arr[j];
}

u16 func_001139E8(const u16 *p) { return p[4]; }

INCLUDE_ASM(const s32, "core/main", func_001139F0);

void func_00113A70(u8 *p)
{
    func_002249D4(*(s32 *)p, *(s32 *)(p + 0x4), *(u16 *)(p + 0x8) * 0x14);
}

s32 func_00113A90(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

u8 *func_00113A98(u8 *arg0, s32 arg1)
{
    s32 count = *(s32 *)(arg0 + 8);
    u8 *item = *(u8 **)(arg0 + 4);
    s32 i;

    for (i = 0; i < count; i++, item += 0xC)
    {
        if (*(s32 *)item == arg1)
            return item;
    }
    return 0;
}

s32 func_00113AD8(u8 *arg0, s32 arg1) {
    typedef struct { char pad[0xC]; s32 *ptr; s32 n; } S;
    S *s = (S *)arg0;
    s32 *p = s->ptr;
    s32 n = s->n;
    s32 i = 0;
    s32 v;
    if (p == 0) goto zero;
    if (n <= 0) goto zero;
    v = *p;
    for (;;) {
        if (v == arg1) return 1;
        p++;
        if (p == 0) goto zero;
        i++;
        if (i >= n) goto zero;
        v = *p;
    }
zero:
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00113B28);

INCLUDE_ASM(const s32, "core/main", func_00113C68);

INCLUDE_ASM(const s32, "core/main", func_00113DF8);

INCLUDE_ASM(const s32, "core/main", func_00113F80);

INCLUDE_ASM(const s32, "core/main", func_00114110);

INCLUDE_ASM(const s32, "core/main", func_00114208);

extern void func_00121888(u8 *, u8 *);
extern void func_00121278(f32 *, u8 *, f32 *);
extern s32 func_00210940(f32 *, s32);

s32 func_001143C8(s32 a1, s32 a2, s32 a3)
{
    u8 v6[96];
    f32 v7[4];
    s32 one = 1;

    if (((*(u8 *)(*(s32 *)(*(s32 *)(a3 + 4) + 4) + 52) >> 2) & 1) != one)
        return 0;
    func_00121888(v6, (u8 *)a2);
    func_00121278(v7, v6, (f32 *)(*(s32 *)(a3 + 36) + 48));
    return func_00210940(v7, a1);
}


s32 func_00114448(u8 arg0, u8 *arg1)
{
    u8 *p = *(u8 **)(arg1 + 4);
    u8 *obj = *(u8 **)(p + 0x14);

    if (obj != 0)
        return func_00108760(arg0, obj);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00114480);

s32 func_001145D8(const u16 *left, const u16 *right) { return left[2] - right[2]; }

void func_001145E8(u8 *p)
{
    func_00224CB8(*(s32 *)p, *(s32 *)(p + 0x4), 0x8, (s32)func_001145D8);
}

#include "common.h"

void func_00114600(u8 *arg0)
{
    u8 *p = (u8 *)*(s32 *)arg0;
    u8 *last = p + 8 * *(s32 *)(arg0 + 4) - 8;
    s32 flag = 0;
    u16 i = 0;

    while (p < last)
    {
        if (i < *(u16 *)(p + 4))
        {
            *(u16 *)(last + 4) = i;
            flag = -1;
            last = p - 8;
        }
        else
        {
            p += 8;
        }
        i = (u16)(i + 1);
    }
    if (flag != 0)
    {
        func_001145E8(arg0);
        return;
    }
}

u16 func_00114670(const u16 *p) { return p[0]; }

s32 func_00114678(u8 arg0, u16 arg1, s32 arg2)
{
    u8 *p = (u8 *)func_001084E8(arg0, arg2);
    u8 *base = *(u8 **)p;

    return *(s32 *)(base + (arg1 << 2));
}

s32 func_001146B8(void)
{
    return func_00157DE8(D_0039347C, 0x8);
}

extern s32 func_00114110(s32, u8, u8);
extern s32 func_00114768(s32, s32, s32, s32, u16, u16, int *);

u16 func_001146D8(s32 a1, u8 a2, s32 a3, u16 *a4, u16 a5)
{
    u16 v9 = *a4;
    int st = 0;
    s32 v10 = func_00114110(a1, 2, a2 - 1);
    u16 r;

    if (v10 != 0)
        func_00114768(a1, v10, *(s32 *)((u8 *)a1 + 0x24), a3, a5, v9, &st);
    r = (u16)st;
    *a4 = r;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_00114768);

INCLUDE_ASM(const s32, "core/main", func_001148F0);

INCLUDE_ASM(const s32, "core/main", func_001149A0);

s32 func_00114CE8(s32 a1, s32 a2, s32 a3, s32 *a4)
{
    u8 b = a2;
    u8 c = a3 - 1;
    s32 v = func_00114110(a1, b, c);
    if (v != 0) {
        *a4 = *(s32 *)(a1 + 0x24);
        return v;
    }
    return 0;
}

s32 func_00114D38(s32 a1, u8 a2, s32 a3, u16 a4)
{
    u32 v8;
    s32 v6 = func_00114CE8(a1, 2, a2, &v8);
    s32 r;
    if (v6 == 0)
        r = 0;
    else
        r = func_001149A0(v6, v8, a3, a4);
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_00114D98);

INCLUDE_ASM(const s32, "core/main", func_00114E28);

INCLUDE_ASM(const s32, "core/main", func_00114FF0);

s32 func_00115290(s32 a1, u8 a2, s32 a3)
{
    u32 v6;
    s32 v4 = func_00114CE8(a1, 0, a2, &v6);
    s32 r;
    if (v4 == 0)
        r = 0;
    else
        r = func_001143C8(v4, v6, a3);
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001152E0);

INCLUDE_ASM(const s32, "core/main", func_00115438);

extern s32 func_00114110(s32, u8, u8);
extern void func_00115438(s32, s32, s32, s32, u16, s32 *);

s32 func_00115620(s32 a1, s32 a2, s32 a3, u16 *a4)
{
    s32 v10[4];
    u8 c = a2 - 1;
    s32 v8;
    u16 v7 = *a4;
    s32 result;

    v10[0] = 0;
    v8 = func_00114110(a1, 0, c);
    if (v8 != 0)
        func_00115438(a1, v8, *(s32 *)(a1 + 36), a3, v7, v10);
    result = (u16)v10[0];
    *a4 = v10[0];
    return result;
}


extern u32 D_00392F38;

s64 func_001156A0(u8 arg1, s64 arg2)
{
  return func_001156E0(arg1, arg2, D_00392F38);
}

extern u32 D_00392F3C;

s64 func_001156C0(u8 arg1, s64 arg2)
{
  return func_001156E0(arg1, arg2, D_00392F3C);
}

INCLUDE_ASM(const s32, "core/main", func_001156E0);

s32 func_00115900(u8 arg0, u8 *arg1, s16 arg2)
{
    u8 *p = *(u8 **)(arg1 + 4);
    u8 *obj = *(u8 **)(p + 0x14);

    if (obj != 0) return func_00108950(arg0, obj, arg2); return 3; }

s32 func_00115940(u8 arg0, u8 *arg1)
{
    u8 *p = *(u8 **)(arg1 + 4);
    u8 *obj = *(u8 **)(p + 0x14);

    if (obj != 0)
        return func_001087C8(arg0, obj) == 0;
    return 1;
}

s32 func_00115978(u32 arg0)
{
    u8 *entry = (u8 *)func_001084E8(arg0 & 0xFF);

    if (entry != 0)
        return *(s32 *)(entry + 4);
    return 0;
}

u8 *func_001159A0(void)
{
    u8 *p = func_00157DE8(D_0039347C, 8);

    *(s32 *)p = 0;
    *(u16 *)(p + 4) = 0;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_001159D0);

s32 func_00115AA8(s32 arg0)
{
    s32 *table;

    if (arg0 >= 0x4E20)
    {
        table = (s32 *)D_00254030[0];
        arg0 -= 0x4E20;
    }
    else
    {
        table = (s32 *)D_00254034[0];
    }
    return table[arg0];
}

INCLUDE_ASM(const s32, "core/main", func_00115AD8);

extern f32 D_003924B4;

s32 func_00115BE0(f32 *out, s16 *in)
{
    f32 s = D_003924B4;
    s32 r;

    out[0] = (f32)in[0] * s;
    out[1] = (f32)in[1] * s;
    out[2] = (f32)in[2] * s;
    r = in[3];
    out[3] = (f32)r * s;
    return r;
}

void func_00115C38(f32 *arg0, f32 *arg1)
{
    arg0[0] = arg1[0];
    arg0[1] = arg1[1];
    arg0[2] = arg1[2];
    arg0[3] = arg1[3];
}

INCLUDE_ASM(const s32, "core/main", func_00115C60);

INCLUDE_ASM(const s32, "core/main", func_00115D10);

f32 func_00115FA0(void) { return D_00393048; }

void func_00115FA8(f32 value) { D_00393048 = value; }

f32 func_00115FB0(void) { return D_0039304C; }

void func_00115FB8(f32 arg0)
{
    D_0039304C = arg0;
    if (arg0 != 0.0f)
        D_00393050 = 1.0f / arg0;
}

f32 func_00115FF0(void) { return D_00393054; }

INCLUDE_ASM(const s32, "core/main", func_00115FF8);

INCLUDE_ASM(const s32, "core/main", func_00116308);

INCLUDE_ASM(const s32, "core/main", func_001163F0);

INCLUDE_ASM(const s32, "core/main", func_00116470);

void func_00116A90(void)
{
    func_00117AD8(0, 0);
}

s32 func_00116AA0(void)
{
    func_00224B88(D_00258038, 0, 0x40);
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00116AD0);

INCLUDE_ASM(const s32, "core/main", func_00116D58);

INCLUDE_ASM(const s32, "core/main", func_00116DB0);

s32 func_00116E28(s32 arg0, s32 arg1)
{
    s32 offset = (s16)arg1 * 4;
    u8 *table = (u8 *)D_00258038 + (s16)arg0 * 4;
    u8 *entry = *(u8 **)table;

    return *(s32 *)(entry + offset + 0x1C);
}

s32 func_00116E58(s32 index)
{
    return *(s32 *)((u8 *)D_00258038 + (s16)index * 4);
}

/* Byte-matching; returning the callee's result avoids a sibling call. */
s32 func_00116E78(s32 arg0)
{
    return func_00116EA0((s16)arg0, 3);
}

s32 func_00116EA0(s32 arg0, s16 arg1)
{
    func_0011C5F8(arg1 & 2, arg1 & 1, 0);
    return 1;
}

void func_0011C5F8(s32 arg0, s32 arg1, s32 arg2);

s32 func_00116ED0(s32 arg0, s32 arg1, s16 arg2)
{
    func_0011C5F8(arg2 & 2, arg2 & 1, D_00393058);
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00116F00);

typedef struct { UnalignedU64 a, b, c; u32 d; } S28;

s32 func_00117000(s16 a1, u8 *out)
{
    u8 *p = (u8 *)func_00116E58(a1);

    if (p == 0)
        return 0;
    *(S28 *)out = *(S28 *)p;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00117078);

INCLUDE_ASM(const s32, "core/main", func_00117350);

s64 func_001174A0(void)
{
  return func_0011BBE8();
}

/* Byte-matching.  Returning the callee's result (instead of discarding it in
   a void wrapper) keeps gcc from turning this into a sibling call: retail
   emits jal + jr through a saved $ra, which is what the return value forces. */
s32 func_001174C0(void) { return func_0011BC68(); }

INCLUDE_ASM(const s32, "core/main", func_001174E0);

s32 func_00117658(s32 arg0, s16 arg1)
{
    s32 v2 = arg1 + 15;
    s32 neg1 = -1;
    s32 *p;
    if (arg1 > neg1)
        v2 = arg1;
    p = (s32 *)func_00116E58(v2 >> 4);
    p[23] = 0;
    func_0018C470(D_00393130);
    return 1;
}

s64 func_001176A0(void)
{
  func_0011C5A8();
  return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001176C0);

INCLUDE_ASM(const s32, "core/main", func_00117900);

s32 func_001179A0(s16 a1)
{
    s32 v1 = a1;
    s32 v2 = 0x10000;
    s32 v3 = func_00116E58(a1);
    s32 *v4 = (s32 *)(v3 + 28);
    s32 cur;
    s32 v5;

    do {
        if (*v4 != 0) {
            func_00224B88(*v4, 0xCCu, 0x74u);
            *v4 = 0;
        }
        ++v4;
        cur = v2;
        v2 += 0x10000;
        v5 = (cur >> 16) < 16;
    } while (v5);
    func_00224B88(v3, 0xCCu, 0x7Cu);
    func_0014CBE8(D_0039347C, v3);
    D_00258038[v1] = 0;
    return 1;
}

s32 func_00117A60(s32 a1, s16 a2, s32 a3)
{
    s32 neg1 = -1;
    s32 v4 = a2 + 15;
    s32 v6;

    if (neg1 < a2)
        v4 = a2;
    v6 = v4 >> 4;
    func_00116E58(v6);
    *(s32 *)(func_00116E28(v6, (s16)(a2 - 16 * v6)) + 0x68) = a3;
    return 1;
}

s32 func_00117AD8(s16 arg0, s16 arg1)
{
    func_00117900(arg0, arg1);
    func_001179A0(arg0);
    return 1;
}

s32 func_00117B18(s32 unused, s32 value)
{
    D_00393058 = value;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00117B28);

INCLUDE_ASM(const s32, "core/main", func_00117BF0);

INCLUDE_ASM(const s32, "core/main", func_00117C80);

INCLUDE_ASM(const s32, "core/main", func_00117E68);

INCLUDE_ASM(const s32, "core/main", func_00117FB8);

INCLUDE_ASM(const s32, "core/main", func_00118360);

INCLUDE_ASM(const s32, "core/main", func_00118AC0);

void func_00118B88(u8 *arg0, f32 arg1, f32 arg2)
{
    *(f32 *)(arg0 + 0x60) = 0.25f;
    if (arg2 == 0.0f)
        *(f32 *)(arg0 + 0x64) = D_003924C4;
    else
        *(f32 *)(arg0 + 0x64) = arg2;
}

INCLUDE_ASM(const s32, "core/main", func_00118BB8);

INCLUDE_ASM(const s32, "core/main", func_00118C88);

void func_00118D58(u8 *p, float a, float b)
{
    *(f32 *)(p + 0x58) = a;
    *(f32 *)(p + 0xCC) = b;
    *(f32 *)(p + 0x5C) = a * b;
}

void func_00118D70(u8 *p, float *out1, float *out2)
{
    *out1 = *(f32 *)(p + 0x58);
    *out2 = *(f32 *)(p + 0xCC);
}

void func_00118D88(u8 *p, float value)
{
    *(f32 *)(p + 0xCC) = value;
    *(f32 *)(p + 0x5C) = *(f32 *)(p + 0x58) * value;
}

INCLUDE_ASM(const s32, "core/main", func_00118DA0);

s32 func_00117350();
void func_00118E18(s16 a1, s16 a2, s32 *a3)
{
    u8 buf[0x6C];
    s32 *p;
    if (func_00117350(a1, a2, buf) == 0) return;
    p = *(s32 **)(buf + 0x68);
    *a3 = *p;
}

INCLUDE_ASM(const s32, "core/main", func_00118E60);

void func_00118FE8(void) { D_003930B4[0x20A8 / 4] = 1; }

void func_00118FF8(void)
{
    D_003930B4[0x20A8 / 4] = 0;
}

typedef struct FrameRateConfig {
    u8 unk00[4];
    s32 flags;
    u8 unk08[0x20];
    f32 frames_per_second;
} FrameRateConfig;

extern FrameRateConfig D_003305A0;
extern s32 D_00393088;
extern s32 D_003930B8;
extern s32 D_0039313C;
extern s32 D_00393140;
extern s32 D_0039347C;
extern void func_00119B50(s32 *);
extern void func_0011B148(void);
extern void func_0011B0D8(void);
extern void func_0011A5A0(void);
extern s32 func_0011C558(s32, s32, s32, s32);
extern s32 func_0014BA38(f32);

s32 func_00119008(void)
{
    if (D_00393088 != 0) {
        uintptr address = (uintptr)func_00157DE8(D_0039347C, 0x20CC);
        D_003930B4 = (s32 *)((address + 0xF) & ~0xF);
        D_003930B4[0x20BC / sizeof(s32)] = 0;
        D_003930B4[0x20A0 / sizeof(s32)] = 0;
        D_003930B4[0x20A4 / sizeof(s32)] = 0;
        D_003930B4[0x20A8 / sizeof(s32)] = 0;
        D_003930B4[0x20B0 / sizeof(s32)] = 0;
        D_003930B4[0x20B4 / sizeof(s32)] = 0;
        D_003930B4[0x20B8 / sizeof(s32)] = 0;
        func_00119B50(&D_003930B8);
        func_0011B148();
        func_0011B0D8();
        func_0011A5A0();
        D_00393088 = 0;
    }

    D_003305A0.frames_per_second = 0.0f;
    func_0011C558(0, 1, D_0039313C, D_00393140);
    if (D_003305A0.frames_per_second < 40.0f ||
        D_003305A0.frames_per_second >= 210.0f) {
        D_003305A0.flags |= 0x40;
        D_003305A0.frames_per_second = 60.0f;
    }
    func_0014BA38(1.0f / D_003305A0.frames_per_second);
    D_00393084 = 1;
    return D_00393084;
}

void func_00119140(void) { D_00393084 = 0; func_0011C5A0(); }

s32 func_00119148(void) { return D_00393084; }

void func_00119150(s32 index)
{
    func_00224B88((s32)D_0026E038, (s8)index, 0x20);
}

void func_00119168(s32 index, u8 value)
{
    ((u8 *)D_0026E038)[index] = value;
}

INCLUDE_ASM(const s32, "core/main", func_00119180);

void func_00119230(s32 arg0, s32 arg1)
{
    if (D_00393098 != 0x20)
        D_0026DFB8[D_00393098++] = arg1;
}

void func_00119260(s32 unused, s32 value)
{
    D_00393098 -= value;
    if (D_00393098 < 0)
        D_00393098 = 0;
}

void func_00119280(void) { D_00393098 = 0; }

void func_00119288(u8 arg0)
{
    s32 i;
    for (i = 0; i < D_00393098; i++)
        *(u8 *)(D_0026DFB8[i] + 0x84) = arg0;
}

INCLUDE_ASM(const s32, "core/main", func_001192D0);

INCLUDE_ASM(const s32, "core/main", func_001193B0);

INCLUDE_ASM(const s32, "core/main", func_00119460);

void func_001194F8(u8 *p, u32 value)
{
    p[0] = value != 0;
}

s32 func_00119508(const u8 *source, s32 *destination) { s32 value = *source != 0; *destination = value; return value; }

void func_00119518(u8 *arg0, u8 *arg1)
{
    *(Unaligned16 *)(arg0 + 0x70) = *(Unaligned16 *)arg1;
    func_0014E7D8(arg0);
}

void func_00119548(u8 *arg0, u8 *arg1)
{
    u64 a, b;
    a = ((UnalignedU64 *)(arg0 + 0x70))->value;
    b = ((UnalignedU64 *)(arg0 + 0x78))->value;
    ((UnalignedU64 *)arg1)->value = a;
    ((UnalignedU64 *)(arg1 + 8))->value = b;
}

INCLUDE_ASM(const s32, "core/main", func_00119570);

void func_00119640(u8 *arg0, f32 arg1, f32 arg2)
{
    u16 type = *(u16 *)(arg0 + 2);

    *(f32 *)(arg0 + 8) = arg1;
    *(f32 *)(arg0 + 4) = arg2;
    if (type != 4)
        func_0014E818(arg0);
}

void func_00119668(u8 *p, float *a, float *b)
{
    *a = *(f32 *)(p + 0x8);
    *b = *(f32 *)(p + 0x4);
}

void func_00119680(f32 *a1, f32 a2, f32 a3)
{
    f32 half = 0.5f;

    a1[4] = a3;
    a1[3] = a2;
    a1[5] = func_00217DA0(a2 * half);
    a1[6] = func_00217DA0(a3 * half);
    func_0014E7F8((s32 *)a1);
}

void func_001196E8(u8 *p, float *a, float *b)
{
    *a = *(f32 *)(p + 0xC);
    *b = *(f32 *)(p + 0x10);
}

u16 func_00119700(const u16 *p) { return p[1]; }

void func_00119708(s16 *p, u16 value) { p[1] = value; }

void func_00119710(u8 *p)
{
    p[0x86] = 1;
}

void func_00119720(u8 *p, u8 value) { p[135] = value; }

void func_00119728(u8 *p)
{
    p[0x87] = 1;
}

void func_00119738(u8 *arg0, u8 *arg1)
{
    u64 a, b;
    a = ((UnalignedU64 *)arg1)->value;
    b = ((UnalignedU64 *)(arg1 + 8))->value;
    ((UnalignedU64 *)(arg0 + 0xD0))->value = a;
    ((UnalignedU64 *)(arg0 + 0xD8))->value = b;
}

void func_00119760(u8 *arg0, u8 *arg1)
{
    u64 a, b;
    a = ((UnalignedU64 *)(arg0 + 0xD0))->value;
    b = ((UnalignedU64 *)(arg0 + 0xD8))->value;
    ((UnalignedU64 *)arg1)->value = a;
    ((UnalignedU64 *)(arg1 + 8))->value = b;
}

void func_00119788(u8 *p, float a, float b)
{
    *(f32 *)(p + 0xC8) = a;
    *(f32 *)(p + 0xCC) = b;
}

/* Byte-matching.  Copies the 12-byte record at arg1 into the table at +0x88 and then
   rebuilds the frame. */
void func_00119798(u8 *arg0, Struct12 *arg1)
{
    Struct12 tmp = *arg1;

    *(Struct12 *)(arg0 + 0x88) = tmp;
    func_00119460(arg0);
}

/* Byte-matching.  Copies the 12-byte record at arg1 into the table at +0xA0 and then
   rebuilds the frame. */
void func_001197E8(u8 *arg0, Struct12 *arg1)
{
    Struct12 tmp = *arg1;

    *(Struct12 *)(arg0 + 0xA0) = tmp;
    func_00119460(arg0);
}

/* Byte-matching.  Copies the 12-byte record at arg1 into the table at +0x94 and then
   rebuilds the frame. */
void func_00119838(u8 *arg0, Struct12 *arg1)
{
    Struct12 tmp = *arg1;

    *(Struct12 *)(arg0 + 0x94) = tmp;
    func_00119460(arg0);
}

/* Byte-matching.  Copies the 12-byte record at arg1 into the table at +0xAC and then
   rebuilds the frame. */
void func_00119888(u8 *arg0, Struct12 *arg1)
{
    Struct12 tmp = *arg1;

    *(Struct12 *)(arg0 + 0xAC) = tmp;
    func_00119460(arg0);
}

u8 *func_001198D8(u8 *p) { return p + 136; }

u8 *func_001198E0(u8 *p) { return p + 160; }

u8 *func_001198E8(u8 *p) { return p + 148; }

u8 *func_001198F0(u8 *p) { return p + 172; }

u8 *func_001198F8(u8 *p) { return p + 200; }

u8 *func_00119900(u8 *p) { return p + 204; }

s32 func_00119908(s32 value) { return value; }

u8 *func_00119910(u8 *p) { return p + 2; }

u8 *func_00119918(u8 *p) { return p + 4; }

u8 *func_00119920(u8 *p) { return p + 8; }

u8 *func_00119928(u8 *p) { return p + 12; }

u8 *func_00119930(u8 *p) { return p + 16; }

u8 *func_00119938(u8 *p) { return p + 112; }

u8 *func_00119940(u8 *p) { return p + 208; }

u8 *func_00119948(u8 *p) { return p + 28; }

u8 func_00119950(const u8 *p) { return p[133]; }

void func_00119958(u8 *arg0, u32 arg1)
{
    if ((arg1 & 0xFF) != 0)
        arg0[0x85] |= 1;
    else
        arg0[0x85] &= 0xFE;
}

void func_00119980(u8 *arg0, u32 arg1)
{
    if ((arg1 & 0xFF) != 0)
        arg0[0x85] |= 2;
    else
        arg0[0x85] &= 0xFD;
}

void func_001199A8(u8 *p, u8 value) { p[133] = value; }

f32 func_001199B0(const f32 *p) { return p[2]; }

f32 func_001199B8(const f32 *p) { return p[1]; }

f32 func_001199C0(const f32 *p) { return p[3]; }

f32 func_001199C8(const f32 *p) { return p[4]; }

f32 func_001199D0(const f32 *p) { return p[50]; }

void func_001199D8(u8 *p, float f, s32 value)
{
    p[1] = value;
    *(f32 *)(p + 0x80) = f;
}

void func_001199E8(u8 *p, u8 value) { p[1] = value; }

u8 func_001199F0(const u8 *p) { return p[1]; }

void func_001199F8(f32 *p, f32 value) { p[32] = value; }

f32 func_00119A00(const f32 *p) { return p[32]; }

INCLUDE_ASM(const s32, "core/main", func_00119A08);

INCLUDE_ASM(const s32, "core/main", func_00119B50);

INCLUDE_ASM(const s32, "core/main", func_00119BF0);

void func_00119D68(void) {
}

void func_00119D70(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00119D78);

/* Source of the quadword pair copied by func_00119E08. */
typedef struct __attribute__((packed)) {
    u32 unk00;
    u64 first;                              /* +0x04 */
    u64 second;                             /* +0x0C */
} QuadPairSource;

/* Destination of the quadword pair copied by func_00119E08. */
typedef struct __attribute__((packed)) {
    u64 first;                              /* +0x00 */
    u64 second;                             /* +0x08 */
} QuadPair;

/* Copies two packed quadwords into the destination and returns the first. */
s64 func_00119E08(QuadPairSource *source, QuadPair *dest)
{
    u64 first = source->first;
    u64 second = source->second;

    dest->first = first;
    dest->second = second;

    return first;
}

void func_00119E30(s32 *src, s32 *dst)
{
    *dst = *src;
}

void func_00119E40(s32 *p, s32 value) { p[0] = value; }

void func_00119E48(u32 *arg0, s32 arg1)
{
    if (arg1 != 0)
        *arg0 &= 0xFFBFFFFF;
    else
        *arg0 |= 0x400000;
}

s32 func_00119E78(u32 *p)
{
    return (((s32)*p >> 26) ^ 1) & 1;
}

INCLUDE_ASM(const s32, "core/main", func_00119E90);

void func_00119F30(u8 *src, s32 *dst)
{
    *dst = *(s32 *)(src + 0x28);
}

void func_00119F40(u8 *a1, s32 dummy, s32 a2, f32 a3, f32 a4)
{
    if (a2 == 0)
        return;
    if (a3 != 0.0f)
    {
        *(f32 *)(a1 + 0x30) = a3;
        a1[0x2F] |= 2;
    }
    if (a4 != 0.0f)
    {
        *(f32 *)(a1 + 0x34) = a4;
        a1[0x2F] |= 4;
    }
}

void func_00119F90(u8 *a1, s32 dummy, s32 a2, f32 a3, f32 a4)
{
    if (a2 != 0)
    {
        *(f32 *)(a1 + 0x40) = a3;
        *(f32 *)(a1 + 0x30) = a3;
        *(f32 *)(a1 + 0x44) = a4;
        *(f32 *)(a1 + 0x34) = a4;
        if (a3 != 0.0f || a4 != 0.0f)
            a1[0x2F] |= 0x10;
    }
}

/* Byte-matching.  Copies two floats out and decodes the mode byte. */
void func_00119FD8(u8 *a, u32 x, s32 *o, float *o1, float *o2)
{
    u8 t;

    *o1 = *(float *)(a + 0x30);
    *o2 = *(float *)(a + 0x34);
    t = *(u8 *)(a + 0x2F);
    if (t & 6)
        *o = 1;
    else if (t & 0x10)
        *o = 2;
    else
        *o = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0011A020);

extern s32 D_0039347C;

s32 func_0011A0A8(u32 *a1)
{
    u32 v2;
    s32 result;
    v2 = func_00157DE8(D_0039347C, 0xC);
    *a1 = v2;
    *(u32 *)v2 = 0;
    result = (s32)*a1;
    *(u32 *)(result + 4) = 0;
    *(u32 *)(*a1 + 8) = 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0011A0F0);

u16 func_0011A1E0(const u16 *p) { return p[18]; }

s32 func_0011A1E8(s32 *a1)
{
    s32 i;
    s32 v2 = a1[6];
    s32 result;

    for (i = 1; v2 != a1[7]; i++)
        v2 = *(s32 *)(v2 + 8);
    result = 0;
    if ((v2 ^ a1[7]) == 0)
        result = i;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0011A230);

void func_0011A280(u16 *p) { p[0x13] |= 1; }

void func_0011A290(u16 *p) { p[0x13] &= 0xFFFE; }

void func_0011A2A0(u8 *p, s32 flag)
{
    u16 v = *(u16 *)(p + 0x26);
    *(u16 *)(p + 0x26) = flag != 0 ? (u16)(v | 2) : (u16)(v & 0xFFFD);
}

s32 func_0011A2C0(u8 *p)
{
    return *(u16 *)(p + 0x26) & 2;
}

void func_0011A2D0(f32 arg)
{
    D_003930A0 = D_003930A0 + arg;
    D_0039309C = arg;
    D_003930A4 = D_003930A4 + 1;
}

INCLUDE_ASM(const s32, "core/main", func_0011A2F0);

INCLUDE_ASM(const s32, "core/main", func_0011A4B0);

s32 func_0011A518(s32 a1)
{
    s32 *p;
    s32 *v1;
    s32 *v3;
    s32 *v4;
    s32 *q;

    if (D_003930AC == 0)
        return 0;
    p = *(s32 **)D_003930AC;
    v1 = *(s32 **)((u8 *)p + 0x10);
    if ((u16)*(u16 *)((u8 *)v1 + 8) < (u32)a1)
        return 0;
    v3 = *(s32 **)((u8 *)*(s32 *)v1 + a1 * 20 + 4);
    if (v3 == 0)
        return 0;
    v4 = *(s32 **)v3;
    if (v4 == 0)
        return 0;
    if (*(s16 *)((u8 *)v4 + 0x22) == 1 && **(s16 **)v4 == 1) {
        q = *(s32 **)((u8 *)v4 + 4);
        return *(*(s32 **)q);
    }
    return 0;
}

void func_0011A5A0(void)
{
    s32 pos = 0x10000;
    s32 cur;
    u8 *p = (u8 *)D_0026BEE8;
    s32 count;

    D_00393094 = 0;
    do {
        func_00120D48((s32)p);
        p += 0x54;
        cur = pos;
        pos += 0x10000;
    } while ((cur >> 16) < 100);

    count = D_00393094;
    D_00393094 = count + 1;
    D_00393090 = (s32)(D_0026BEE8 + count * 0x54);
}

void func_0011A630(void)
{
    s32 count = D_00393094;

    if (count != 0)
    {
        count--;
        D_00393094 = count;
        D_00393090 = (s32)(D_0026BEE8 + count * 0x54);
    }
}

INCLUDE_ASM(const s32, "core/main", func_0011A660);

void func_0011A778(void)
{
    D_00393094 = 0;
    D_00393090 = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0011A788);

void func_0011A7E0(f32 arg0)
{
    u8 *p = (u8 *)D_003930B4;
    if (arg0 > 1.0f) { *(f32 *)(p + 0x20BC) = 1.0f; return; }
    if (arg0 < 0.0f) { *(f32 *)(p + 0x20BC) = 0.0f; return; }
    *(f32 *)(p + 0x20BC) = arg0;
}

float func_0011A818(void)
{
    return *(f32 *)((u8 *)D_003930B4 + 0x20BC) * 255.0f;
}

f32 func_0011A830(void)
{
    return *(f32 *)((u8 *)D_003930B4 + 0x20BC);
}

INCLUDE_ASM(const s32, "core/main", func_0011A840);

void func_0011A990(void) { func_00152AC0(); }

INCLUDE_ASM(const s32, "core/main", func_0011A998);

INCLUDE_ASM(const s32, "core/main", func_0011AA40);

INCLUDE_ASM(const s32, "core/main", func_0011AE28);

INCLUDE_ASM(const s32, "core/main", func_0011AF10);

void func_0011B0A0(void)
{
    s32 *p = D_003930B4;
    s32 v = p[2];
    D_003930CC = v;
    p[2] = v & ~0x408;
    p[3] = v & ~0x408;
}

void func_0011B0C0(void) { ((s32 *)D_003930B4)[2] = D_003930CC; }

void func_0011B0D0(s32 value) { D_003930C4 = value; }

void func_0011B0D8(void)
{
    D_0026F0DC[0] = 0;
}

void func_0011B0E8(u8 *arg0, u8 *arg1, f32 arg2)
{
    *(S12_flash *)D_0026F1E0 = *(S12_flash *)arg1;
    D_003930F8 = arg2;
}


void func_0011B110(void) { D_003930F4 |= 1; }

extern s32 D_003930F4;

void func_0011B120(void)
{
    u32 mask = 0xFFFFFFFE;
    D_003930F4 &= mask;
}

void func_0011B138(void) { D_003930F4 |= 0x20; }

INCLUDE_ASM(const s32, "core/main", func_0011B148);

void func_0011B2D8(u8 value) { D_003930E1 = value; }

s8 func_0011B2E0(void) { return D_003930E1; }

void func_0011B2E8(u8 value) { D_003930E2 = value; }

s8 func_0011B2F0(void) { return D_003930E2; }

s32 func_0011B2F8(void)
{
    return D_003930E2 & 1;
}

void func_0011B308(s32 a, s32 b, s32 c)
{
    D_0026F210[0] = a;
    D_0026F210[1] = b;
    D_0026F210[2] = c;
}

void func_0011B320(u8 *arg0)
{
    typedef struct { UnalignedU64 v; u32 w; } S12;

    *(S12 *)D_0026F220 = *(S12 *)arg0;
}

INCLUDE_ASM(const s32, "core/main", func_0011B348);

INCLUDE_ASM(const s32, "core/main", func_0011B3E0);

void func_0011B4C0(float a, float b, float c)
{
    D_0026F290[0] = a;
    D_0026F290[1] = b;
    D_0026F290[2] = c;
}

void func_0011B4D8(float a, float b, float c)
{
    D_0026F280[0] = a;
    D_0026F280[1] = b;
    D_0026F280[2] = c;
}

void func_0011B4F0(f32 value) { D_003930F0 = value; }

INCLUDE_ASM(const s32, "core/main", func_0011B4F8);

INCLUDE_ASM(const s32, "core/main", func_0011B5B8);

void func_0011B678(void) { D_0039310C = 0; }

INCLUDE_ASM(const s32, "core/main", func_0011B680);

INCLUDE_ASM(const s32, "core/main", func_0011B960);

INCLUDE_ASM(const s32, "core/main", func_0011BA28);

void func_0011BB78(void)
{
    if (D_0039343F == 0) {
        func_00193038(8, 1);
        D_0039343F = 1;
    }
    {
        s32 four = 4;
        if (D_0039343E != four) {
            func_00193038(0xC, 0);
            func_00193038(0xB, 1);
            func_00193038(0xA, 2);
            D_0039343E = four;
        }
    }
}

extern s32 func_0018C450();
extern void func_0011BB78();
extern void func_0011B960();
extern void func_0018C470();

s32 func_0011BBE8(void)
{
    s32 result = 0;

    if (func_0018C450(D_00393130) == 0)
        return result;
    func_0011BB78();
    if (D_0039343E != 0)
    {
        s32 one = 1;
        D_0039343E = 0;
        func_00193038(6, 1);
        D_00393440 = one;
        func_00193038(8, 1);
        D_0039343F = one;
        D_0039343E = 0;
    }
    func_0011B960();
    func_0018C470(D_00393130);
    return 1;
}

s32 func_0011BC68(void)
{
    if (D_003933DC != 0)
        func_00152500();
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0011BC90);

s32 func_0011C090(void)
{


    if (D_0039343E != 0) {
        D_0039343E = 0;
        func_00193038(6, 1);
        D_00393440 = 1;
        func_00193038(8, 1);
        D_0039343F = 1;
        D_0039343E = 0;
    }
    if (D_0039343F != 0) {
        func_00193038(8, 0);
        D_0039343F = 0;
    }
    if (D_0039343E != 3) {
        func_00193038(0xC, 1);
        func_00193038(0xA, 5);
        func_00193038(0xB, 6);
        D_0039343E = 3;
    }
}

INCLUDE_ASM(const s32, "core/main", func_0011C130);

s32 func_0011C420(s32 *a1, s32 *a2)
{
    s32 v3 = func_0018C5F0(D_00393130);
    s32 v6;
    if (v3 == 0) return 0;
    v6 = func_00196190(v3, 0, 1);
    if (v6 == 0) return 0;
    *a1 = v6;
    *a2 = func_00195DA8(v3);
    return 1;
}

s32 func_0011C498(void)
{
    u8 *value = (u8 *)func_0018C5F0(D_00393130);

    return value == (u8 *)func_001961D0(value);
}

INCLUDE_ASM(const s32, "core/main", func_0011C4D8);

extern s32 D_00393124;

s32 func_0011C558(s32 a1, s32 a2, s32 a3, s32 a4)
{

    func_0011C680(D_00393124);
    func_00154AF8();
    func_001558C8();
    func_00152200();
    func_0011C4D8((s32)&D_003305A0);
    func_00151B38();
}

void func_0011C5A0(void) { func_0011C660(1); }

extern s32 D_00393128;
s32 func_00119148();
void func_0018C4D8();
void func_0011C5A8(void)
{
    if (func_00119148() != 0) {
        if (D_00393128 != 0) {
            s32 *p = D_003930B4;
            p[0x20B0/4] = 0;
            p[0x20B8/4] = 0;
            func_0018C4D8(D_00393130, 0, 0);
        }
    }
}

void func_0011C5F8(s32 arg0, s32 arg1, s32 arg2)
{
    s32 local[4];

    if (func_00119148() != 0)
        func_0018C4A0(D_00393130, local, arg0 != 0 ? 2 : 0);
}

void func_0011C638(void)
{
    u8 *base = (u8 *)D_003930B4;

    if (*(s32 *)(base + 0x20B4) == 0)
        func_0011C5F8(1, 0, 0);
}

void func_0011CA80(void);

s32 func_0011C660(void)
{
    func_0011CA80();
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0011C680);

s32 func_0011C768(void)
{



    if (func_0018FE28((s32)D_00393148, (s32)func_00192250, 0) == 0)
        return 0;
    func_00190B80((s32)D_003890C8);
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0011C7B0);

INCLUDE_ASM(const s32, "core/main", func_0011C990);

void func_0011CA80(void)
{
    func_0018CED8();
    func_0018D350();
    func_0018D3D8();
}

INCLUDE_ASM(const s32, "core/main", func_0011CAA8);

INCLUDE_ASM(const s32, "core/main", func_0011CC68);

void func_0011CE10(void)
{
    f32 v3[2];
    s32 v0;
    s32 v1;
    s32 v2;

    D_00393138 = 0;
    if (D_00393134 != 0) {
        v3[0] = D_00392500;
        v3[1] = D_00392504;
        v0 = func_0018C5F0(D_00393134);
        v1 = func_0018C608(D_00393134);
        D_00393138 = func_0018C180();
        v2 = func_0018DC88();
        func_0018C5B0(D_00393138, v2);
        func_0018C5E0(D_00393138, v0);
        func_0018C5F8(D_00393138, v1);
        func_0018C900(D_00393138, 500.0f);
        func_0018C820(D_00393138, 0.25f);
        func_0018C660(D_00393138, v3);
    }
}

void func_0011CEC0(s32 value) { D_00393130 = value; }

INCLUDE_ASM(const s32, "core/main", func_0011CEC8);

INCLUDE_ASM(const s32, "core/main", func_0011CF50);

INCLUDE_ASM(const s32, "core/main", func_0011CFC8);

INCLUDE_ASM(const s32, "core/main", func_0011D0C0);

/* Byte-matching; see func_001174C0 for why the result is returned. */
s32 func_0011D290(void) { return func_0011D0C0(); }

INCLUDE_ASM(const s32, "core/main", func_0011D2B0);

INCLUDE_ASM(const s32, "core/main", func_0011D320);

INCLUDE_ASM(const s32, "core/main", func_0011D380);

extern s32 func_001C4128(s32 arg0, s32 arg1, s32 arg2);

s32 func_0011D4C0(u32 arg0, s32 arg1, s32 arg2, s32 *arg3)
{
    s32 v;
    s32 sum;
    s32 result;

    v = func_001C4128(*arg3, arg0, arg1 * arg2);
    result = 0;
    if (v != -1)
    {
        sum = arg3[1] + v;
        arg3[1] = sum;
        if (arg3[2] < sum)
            arg3[2] = sum;
        result = 0;
        if (arg1 != 0)
            return v / (u32)arg1;
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0011D550);

INCLUDE_ASM(const s32, "core/main", func_0011D680);

INCLUDE_ASM(const s32, "core/main", func_0011D790);

s64 func_0011D818(int arg1)
{
  return *(int *)(arg1 + 4) >= (s64)*(int *)(arg1 + 8);
}

s32 func_0011D830(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_0011D838);

INCLUDE_ASM(const s32, "core/main", func_0011D988);

s32 func_0011D838();
s32 func_0011D988();

extern s32 D_00393154;

extern s32 func_0018D2E8();

s32 func_0011DA30(void)
{
    s32 result = func_0018D2E8(0x2C, D_00393154, func_0011D838, func_0011D988);
    s32 invalid = -1;

    return invalid < result;
}

void func_0011DA68(f32 arg0)
{
    u8 *base = (u8 *)D_003930B4;

    *(f32 *)(base + 0x2014) = arg0;
    *(f32 *)(base + 0x2018) = 1.0f / arg0;
}

void func_0011DA90(float x, float y, float z, float w)
{
    *(f32 *)((u8 *)D_003930B4 + 0x201C) = x;
    *(f32 *)((u8 *)D_003930B4 + 0x2020) = y;
    *(f32 *)((u8 *)D_003930B4 + 0x2024) = z;
    *(f32 *)((u8 *)D_003930B4 + 0x2028) = w;
}

s32 func_0011DAA8(u32 value)
{
    return value == 0xCDCDCDCD ? 0 : value != 0;
}

s32 func_0011DAC8(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

void func_0011DAD0(s32 *p, s32 value) { p[0] = value; }

u8 *func_0011DAD8(u8 *p) { return p + 4; }

s32 func_0011DAE0(s32 arg0, u16 arg1)
{
    u32 p = (u32)func_0011DAD8((u8 *)arg0);

    if (p == 0xFFFFFFFF)
        return 0;
    return func_0011DB90(p, arg1);
}

s32 func_0011DBC8(s32, u16);

s32 func_0011DB20(s32 arg0, u16 arg1)
{
    u32 v;

    if (arg1 == 0) return 1;
    v = (u32)func_0011DAD8((u8 *)arg0);
    if (v == 0 || v == 0xFFFFFFFFu) return 0;
    return func_0011DBC8((s32)v, arg1);
}

void func_0011DB78(s16 *p, s16 value) { p[0] = value; }

u16 func_0011DB80(const u16 *p) { return p[1]; }

void func_0011DB88(s16 *p, s16 value) { p[1] = value; }

s32 func_0011DB90(s32 arg0, u32 arg1)
{
    u32 mask = (1 << arg1) & 0xFFFF;

    return (mask & func_0011DB80(arg0)) != 0;
}

s32 func_0011DBC8(s32 arg0, u16 arg1)
{
    u16 mask = arg1;

    return (func_0011DB80(arg0) & mask) == mask;
}

void func_0011DBF8(s16 *p) { p[0] = 0; }

s32 func_0011DC00(void)
{
    func_00224B88(&D_00393C20, 0, 8);
    func_0011E910();
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0011DC30);

INCLUDE_ASM(const s32, "core/main", func_0011DD00);

INCLUDE_ASM(const s32, "core/main", func_0011DD60);

s32 func_0011DDE8(s32 arg0)
{
    s32 result;

    if (func_0011DD00(arg0) == 0) goto zero;
    result = 1;
    *(*(u8 **)(arg0 + 8)) &= 0xFD;
    return result;
zero:
    result = 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0011DE30);

INCLUDE_ASM(const s32, "core/main", func_0011DFE8);

s32 func_0011EA50(s32 unused, u8 *p);

s32 func_0011E0A8(u8 *p)
{
    s32 result;

    if (*(u16 *)p == 1)
        result = func_0011EA50(*(s16 *)(p + 2), *(u8 **)(p + 4));
    else
        result = -1;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0011E0E0);

s32 func_0011EC58();
void func_0011E1E8(u8 *p)
{
    if (*(u16 *)p == 1) {
        func_0011EC58(p);
    }
}

INCLUDE_ASM(const s32, "core/main", func_0011E208);

s32 func_0011E330(u8 *arg0)
{
    s32 result;

    if (*(u16 *)arg0 == 1)
        result = func_0011EA70(*(s16 *)(arg0 + 2));
    else
        result = -1;
    return result;
}

s32 func_0011E368(u8 *arg0)
{
    s32 result;

    if (*(u16 *)arg0 == 1)
        result = func_0011EA20(*(s16 *)(arg0 + 2));
    else
        result = -1;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0011E3A0);

INCLUDE_ASM(const s32, "core/main", func_0011E4A0);

s32 func_0011E910(void)
{
    return func_001C7E78(0) == 1 ? 1 : -1;
}

INCLUDE_ASM(const s32, "core/main", func_0011E940);

s32 func_0011EA20(s16 a1) {
func_001C8748(a1 & 1, a1 >> 1, 1, 3);
return 1;
}

s32 func_0011EA50(s32 unused, u8 *p)
{
    *(s32 *)p = 0x40;
    *(s32 *)(p + 0x4) = 0x40;
    *(s32 *)(p + 0xC) = 0x40;
    *(s32 *)(p + 0x10) = 0x40;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0011EA70);

INCLUDE_ASM(const s32, "core/main", func_0011EB20);

s32 func_0011EC58(u8 *a1)
{
    u8 *p = *(u8 **)(a1 + 8);
    u8 *q = *(u8 **)(a1 + 0xC);
    u8 b = *p;
    s32 r;
    func_00224B88(p, 0, 0x60);
    *p = b;
    b = *q;
    r = func_00224B88(q, 0, 0x60);
    *q = b;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_0011ECC0);

s32 func_0011EE28(u8 *p)
{
    return p != 0 && *(s32 *)(p + 0xC) == 1;
}

s32 func_0011EE48(u8 *p)
{
    return p != 0 && *(s32 *)(p + 0xC) > 0;
}

s32 func_0011EE60(u8 *p)
{
    return p != 0 && *(u32 *)(p + 0xC) == 0xFFFFFFFF;
}

s32 func_0011EE80(s32 *arg1)
{
    s32 result = 0;

    if (arg1 == 0 || arg1[3] < 0) {
        result = 1;
    }
    return result;
}

float func_0011EEA0(u8 *p)
{
    if (p != 0)
        return *(f32 *)(p + 0x10);
    return 0.0f;
}

INCLUDE_ASM(const s32, "core/main", func_0011EEB8);

s32 func_0011F2A8(u8 *arg0)
{
    u8 *node = *(u8 **)(arg0 + 4);
    s32 result = 0;

    D_00393174 = (s32)arg0;
    *(s32 *)(arg0 + 0x10) = 0;
    if (node != 0) {
        D_00393178 = (s32)node;
        result = func_0011F2E0(node);
    }
    return result;
}

s32 func_0011F2E0(u8 *arg0)
{
    s32 (*handler)(u8 *) = (s32 (*)(u8 *))D_00330990[*arg0];

    return handler(arg0);
}

s32 func_0011F318(s32 a1)
{
    s32 v1;
    s32 result;
    D_00393178 = a1 + 4;
    v1 = func_0011F2E0((u8 *)(a1 + 4));
    result = func_0011F2E0((u8 *)D_00393178) == 1;
    if (v1 != 1)
        result = 0;
    return result;
}

s32 func_0011F360(u8 *arg0)
{
    s32 v1;
    D_00393178 = (s32)(arg0 + 4);
    v1 = func_0011F2E0(arg0 + 4);
    return (v1 == 1) | (func_0011F2E0((u8 *)D_00393178) == 1);
}

s32 func_0011F3B0(u8 *arg0)
{
    u8 *entry = arg0 + 4;

    D_00393178 = (s32)entry;
    return (u32)func_0011F2E0(entry) < 1;
}

s32 func_0011F3D8(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_0011F3E0);

INCLUDE_ASM(const s32, "core/main", func_0011F508);

INCLUDE_ASM(const s32, "core/main", func_0011F700);

INCLUDE_ASM(const s32, "core/main", func_0011F8E0);

INCLUDE_ASM(const s32, "core/main", func_0011FB50);

INCLUDE_ASM(const s32, "core/main", func_0011FC30);

INCLUDE_ASM(const s32, "core/main", func_0011FD10);

INCLUDE_ASM(const s32, "core/main", func_0011FDB0);

INCLUDE_ASM(const s32, "core/main", func_0011FE50);

s32 func_0011FF20(u8 *arg0)
{
    s32 result;

    D_00393178 = (s32)(arg0 + 4);
    result = func_0011EE28(*(u8 **)(arg0 + 4));
    D_00393178 += 4;
    return result == 1;
}

s32 func_0011FF60(u8 *arg0)
{
    s32 result;

    D_00393178 = (s32)(arg0 + 4);
    result = func_0011EE48(*(u8 **)(arg0 + 4));
    D_00393178 += 4;
    return result == 1;
}

s32 func_0011FFA0(u8 *arg0)
{
    s32 result;

    D_00393178 = (s32)(arg0 + 4);
    result = func_0011EE60(*(u8 **)(arg0 + 4));
    D_00393178 += 4;
    return result == 1;
}

s32 func_0011FFE0(u8 *arg0)
{
    s32 result;

    D_00393178 = (s32)(arg0 + 4);
    result = func_0011EE80(*(s32 **)(arg0 + 4));
    D_00393178 += 4;
    return result == 1;
}


/* Source: math/region_0012.c */
/* func_00120020 through func_0012F8D8; retail address range 0x00120000-0x00130000. */




extern s32 D_00393184;

extern s32 D_00393180;
extern s32 D_00393194;

INCLUDE_ASM(const s32, "core/main", func_00120020);

INCLUDE_ASM(const s32, "core/main", func_001200B0);

INCLUDE_ASM(const s32, "core/main", func_00120130);

INCLUDE_ASM(const s32, "core/main", func_001201B8);

void func_00120270(void)
{
    u32 i;
    for (i = 0; i < (u32)D_00330A18[18]; i++)
        *(s32 *)((i * 24) + D_00330A18[19] + 12) = -2;
}

s32 func_001202B8(s32 *p)
{
    return p ? p[2] : 0;
}

s32 func_001202D0(s32 index)
{
    if ((u32)index < (u32)D_00330A18[18])
        return D_00330A18[19] + index * 24;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00120300);

void func_00120368(u8 *p)
{
    if (p != 0)
    {
        p[0x14] = 1;
        *(s32 *)(p + 0xC) = -2;
    }
}

void func_00120388(u8 *p)
{
    if (p != 0)
    {
        p[0x14] = 0;
        *(s32 *)(p + 0xC) = -2;
    }
}

void func_001203A0(s32 *p, s32 value) { p[3] = value; }

s32 func_001203A8(void)
{
    u8 *base;
    u32 i;
    s32 *p;
    u8 *q;

    base = (u8 *)D_00330A18;
    i = 2;
    p = (s32 *)(base + 8);
    q = base + 0x59;
    do {
        i++;
        if ((u32)(*q - 3) < 2) {
            *q = 1;
            func_0011DDE8(*p);
        }
        p++;
        q++;
    } while (i < 0x12);
    return i < 0x12;
}

extern s32 func_0011DD60();

s32 func_00120428(void)
{
    s32 one = 1;
    s32 four = 4;
    s32 i = 2;
    u8 *base = D_00330A18;
    s32 *v1 = (s32 *)(base + 8);
    u8 *v2 = base + 0x59;
    s32 r;

    do {
        i++;
        if (*v2 == one) {
            *v2 = four;
            func_0011DD60(*v1);
        }
        v1++;
        v2++;
        r = (u32)i < 0x12;
    } while ((u32)i < 0x12);
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001204A8);

INCLUDE_ASM(const s32, "core/main", func_00120538);

INCLUDE_ASM(const s32, "core/main", func_00120588);

extern s32 func_00120768();

s32 func_00120708(u8 *arg0)
{
    u8 *p = *(u8 **)(arg0 + 4);

    if (*(s16 *)p == -4070)
        return func_00120768(*(u16 *)(p + 8) | 0x8000, 0);
    return 0;
}

s32 func_00120748(void)
{
    return func_00157DE8(D_0039347C, 0x10);
}

extern s32 D_00393188;

s32 func_00120768(arg0, arg1)
u16 arg0;
u8 arg1;
{
    s32 result = 0;
    s32 count = 0;
    u16 *base = (u16 *)D_00393188;
    u16 *p;
    u16 v;
    u16 *q;

    if (base[0] != 0xFFFF && arg0 != base[0])
    {
        p = base;
        for (;;)
        {
            p += 6;
            v = *p;
            count++;
            if (v == 0xFFFF) break;
            if (arg0 == v) break;
        }
    }

    q = (count * 6) + base;
    if (*q != 0xFFFF)
    {
        if (arg1 == 0)
            result = *(s32 *)((u8 *)q + 4);
        else if (arg1 == 1)
            result = *(s32 *)((u8 *)q + 8);
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001207E8);

extern s32 D_00330D78[];

s32 func_001208C0(s16 arg0, s16 arg1)
{
    s32 *p = &D_00330D78[4 * arg0];

    switch (arg1) {
    case 0:
        return p[0];
    case 1:
        return p[1];
    case 2:
        return p[2];
    case 3:
        return p[3];
    }
    return 0;
}

/* Sign-extends the low 16 bits of the argument. */
s32 func_00120940(s32 value)
{
    return (s16)value;
}

s32 func_00120950(s32 index)
{
    s32 off = (index + 2) * 4;
    u8 *entry = *(u8 **)((u8 *)D_00330A18 + off);
    u8 *field = *(u8 **)(entry + 8);

    return *field & 1;
}

s32 func_00120978(void)
{
    s32 *p = D_00330A20;
    s32 i = 1;

    for (;;)
    {
        i--;
        if (*p != 0)
        {
            if ((*(u8 *)(*(u8 **)(*p + 8)) & 1) != 0)
                return 1;
        }
        if (i < 0)
            return 0;
        p++;
    }
}

s32 func_001209C0(s32 index, s32 mask)
{
    u8 *entry = (u8 *)D_00330A18[index + 2];
    u8 *field = *(u8 **)(entry + 8);

    return (~*(u32 *)(field + 4) & mask) == 0;
}

s32 func_00214F20(void *);

s32 func_001209F0(s32 arg0, s32 arg1)
{
    s32 idx = arg0 + 2;
    u8 *e = *(u8 **)((u8 *)D_00330A18 + idx * 4);
    s32 v = *(s32 *)(*(u8 **)(e + 8) + 4);
    s32 m = v & arg1;
    s32 r = 0;

    if (m == arg1)
    {
        s32 t = func_00214F20((void *)e);
        t &= m;
        r = t != m;
    }
    return r;
}

s32 func_00120A50(void)
{
    s32 i = 1;
    u32 base = (u32)D_00330A18;
    s32 **p = (s32 **)(base + 0xC);
    s32 val;

    do
    {
        val = D_00393184 >> i;
        if ((val & 1) != 0)
        {
            i = i - 1;
        }
        else if ((*(s32 *)(*(s32 *)((u8 *)*p + 8) + 4) & 8) != 0)
        {
            i = i - 1;
        }
        else
        {
            if ((func_00214F20(*p) & 8) != 0)
                return i;
            i = i - 1;
        }
        p = (s32 **)((u8 *)p - 4);
    } while (i >= 0);
    return -1;
}

s32 func_00120AD8(void)
{
    s32 i = 1;
    s32 val;

    do
    {
        val = D_00393184 >> i;
        if ((val & 1) == 0)
        {
            if (func_001209C0(i, 8) != 0)
                return i;
        }
        i = i - 1;
    } while (i >= 0);
    return -1;
}

extern s32 func_0011DD60();

s32 func_00120B38(s32 index)
{
    u8 *entry = (u8 *)D_00330A18[index + 2];

    return func_0011DD60(entry) == 1;
}

s32 func_00120B70(s32 arg0)
{
    return func_0011DDE8(D_00330A18[arg0 + 2]) == 1;
}

INCLUDE_ASM(const s32, "core/main", func_00120BA8);

void func_00120C38(void) { D_00393184 = 0; }

s32 func_00120C40(void) { return D_00393180; }

s32 func_00120C48(void) {
    register s32 value __asm__("$2") = D_00393180;
    value++;
    D_00393180 = value;
    return value;
}

s32 func_00120C58(void) {
    register s32 value __asm__("$2") = 1;
    D_00393184 = value;
    D_00393180 = value;
    return value;
}

/* Byte-matching.  Array syntax keeps gcc from folding (index + 2) * 4 into
   index * 4 plus a displacement baked into the base address. */
void func_00120C68(s32 index)
{
    func_0011E1E8(D_00330A18[index + 2]);
}

s32 func_00120C88(void)
{
    s32 result = -1;
    s32 i;
    s32 mask;
    s32 v;

    for (i = D_00393180 - 1; i >= 0; i--)
    {
        mask = 1 << i;
        if (func_00120950(i) == 0)
        {
            D_00393184 &= ~mask;
            func_00120C68(i);
        }
        v = D_00393184 & mask;
        if (v == 0)
            result = i;
    }
    return result;
}

void func_00120D18(void)
{
    D_00393194 = (s32)D_00330E78;
    func_00120D48((s32)D_00330E78);
}

void func_00120D30(s32 *dst)
{
    *dst = D_00393194;
}

s32 func_00120D40(void) { return D_00393194; }

INCLUDE_ASM(const s32, "core/main", func_00120D48);

INCLUDE_ASM(const s32, "core/main", func_00120DB0);

INCLUDE_ASM(const s32, "core/main", func_00120F90);

INCLUDE_ASM(const s32, "core/main", func_00121278);

INCLUDE_ASM(const s32, "core/main", func_00121480);

INCLUDE_ASM(const s32, "core/main", func_00121610);

#define M_F(p, o) (*(f32 *)((p) + (o)))

#define M_COPY(d, s, n) do { \
    u32 i_; \
    for (i_ = 0; i_ < (n); i_++) \
        *((u8 *)(d) + i_) = *((u8 *)(s) + i_); \
} while (0)

#define M_TRANSPOSE(p) do { \
    f32 t_; \
    t_ = M_F(p, 0x04); M_F(p, 0x04) = M_F(p, 0x10); M_F(p, 0x10) = t_; \
    t_ = M_F(p, 0x08); M_F(p, 0x08) = M_F(p, 0x20); M_F(p, 0x20) = t_; \
    t_ = M_F(p, 0x18); M_F(p, 0x18) = M_F(p, 0x24); M_F(p, 0x24) = t_; \
} while (0)

#define M_NEGROW(d, s) do { \
    f32 r0_; f32 r1_; f32 r2_; \
    r0_ = -M_F(s, 0x30); \
    r1_ = -M_F(s, 0x34); \
    r2_ = -M_F(s, 0x38); \
    M_F(d, 0x30) = r0_; \
    M_F(d, 0x34) = r1_; \
    M_F(d, 0x3C) = 1.0f; \
    M_F(d, 0x38) = r2_; \
} while (0)

#define M_DOTROWS(d) do { \
    f32 t0_; f32 t1_; f32 t2_; \
    f32 a0_; f32 a1_; f32 a2_; \
    t0_ = M_F(d, 0x30); \
    t1_ = M_F(d, 0x34); \
    t2_ = M_F(d, 0x38); \
    a0_ = M_F(d, 0x00) * t0_ + M_F(d, 0x10) * t1_ + M_F(d, 0x20) * t2_; \
    a1_ = M_F(d, 0x04) * t0_ + M_F(d, 0x14) * t1_ + M_F(d, 0x24) * t2_; \
    a2_ = M_F(d, 0x08) * t0_ + M_F(d, 0x18) * t1_ + M_F(d, 0x28) * t2_; \
    M_F(d, 0x30) = a0_; \
    M_F(d, 0x34) = a1_; \
    M_F(d, 0x38) = a2_; \
} while (0)


void func_00121888(u8 *arg0, u8 *arg1)
{
    u8 tmp[0x54];
    u8 t;

    t = *(u8 *)(arg1 + 0x50);
    if (t == 1) {
        if (arg0 != arg1)
            func_00120D48((u32)arg0);
        return;
    }

    if (t == 2) {
        if (arg0 != arg1) {
            func_00120D48((u32)arg0);
            *(u8 *)(arg0 + 0x50) = 2;
        }
        M_NEGROW(arg0, arg1);
        return;
    }

    if (t == 3 || t == 4) {
        if (arg0 != arg1)
            M_COPY(arg0, arg1, 0x54);
        M_F(arg0, 0x00) = 1.0f / M_F(arg1, 0x00);
        M_F(arg0, 0x40) = M_F(arg0, 0x00);
        M_F(arg0, 0x14) = 1.0f / M_F(arg1, 0x14);
        M_F(arg0, 0x44) = M_F(arg0, 0x14);
        M_F(arg0, 0x28) = 1.0f / M_F(arg1, 0x28);
        M_F(arg0, 0x48) = M_F(arg0, 0x28);
        M_NEGROW(arg0, arg1);
        M_F(arg0, 0x30) = M_F(arg0, 0x40) * M_F(arg0, 0x30);
        M_F(arg0, 0x34) = M_F(arg0, 0x44) * M_F(arg0, 0x34);
        M_F(arg0, 0x3C) = 1.0f;
        M_F(arg0, 0x38) = M_F(arg0, 0x48) * M_F(arg0, 0x38);
        return;
    }

    if (t == 5) {
        *(u8 *)(arg0 + 0x50) = 5;
        if (arg0 == arg1) {
            M_TRANSPOSE(arg0);
        } else {
            M_F(arg0, 0x00) = M_F(arg1, 0x00);
            M_F(arg0, 0x04) = M_F(arg1, 0x10);
            M_F(arg0, 0x08) = M_F(arg1, 0x20);
            M_F(arg0, 0x0C) = M_F(arg1, 0x0C);
            M_F(arg0, 0x10) = M_F(arg1, 0x04);
            M_F(arg0, 0x14) = M_F(arg1, 0x14);
            M_F(arg0, 0x18) = M_F(arg1, 0x24);
            M_F(arg0, 0x1C) = M_F(arg1, 0x1C);
            M_F(arg0, 0x20) = M_F(arg1, 0x08);
            M_F(arg0, 0x24) = M_F(arg1, 0x18);
            M_F(arg0, 0x28) = M_F(arg1, 0x28);
            M_F(arg0, 0x2C) = M_F(arg1, 0x2C);
            M_F(arg0, 0x30) = M_F(arg1, 0x30);
            M_F(arg0, 0x34) = M_F(arg1, 0x34);
            M_F(arg0, 0x38) = M_F(arg1, 0x38);
            M_F(arg0, 0x3C) = M_F(arg1, 0x3C);
        }
        M_NEGROW(arg0, arg1);
        M_DOTROWS(arg0);
        M_F(arg0, 0x3C) = 1.0f;
        M_F(arg0, 0x40) = 1.0f;
        M_F(arg0, 0x44) = 1.0f;
        M_F(arg0, 0x48) = 1.0f;
        M_F(arg0, 0x4C) = 1.0f;
        return;
    }

    if (t == 6) {
        *(u8 *)(arg0 + 0x50) = 6;
        if (arg0 == arg1) {
            M_TRANSPOSE(arg0);
        } else {
                        M_F(arg0, 0x00) = M_F(arg1, 0x00);
            M_F(arg0, 0x04) = M_F(arg1, 0x10);
            M_F(arg0, 0x08) = M_F(arg1, 0x20);
            M_F(arg0, 0x0C) = M_F(arg1, 0x0C);
            M_F(arg0, 0x10) = M_F(arg1, 0x04);
            M_F(arg0, 0x14) = M_F(arg1, 0x14);
            M_F(arg0, 0x18) = M_F(arg1, 0x24);
            M_F(arg0, 0x1C) = M_F(arg1, 0x1C);
            M_F(arg0, 0x20) = M_F(arg1, 0x08);
            M_F(arg0, 0x24) = M_F(arg1, 0x18);
            M_F(arg0, 0x28) = M_F(arg1, 0x28);
            M_F(arg0, 0x2C) = M_F(arg1, 0x2C);
            M_F(arg0, 0x30) = M_F(arg1, 0x30);
            M_F(arg0, 0x34) = M_F(arg1, 0x34);
            M_F(arg0, 0x38) = M_F(arg1, 0x38);
            M_F(arg0, 0x3C) = M_F(arg1, 0x3C);
        }
        {
            f32 s;
            f32 s2;

            s = 1.0f / (M_F(arg1, 0x40) * M_F(arg1, 0x40));
            M_F(arg0, 0x00) = M_F(arg0, 0x00) * s;
            M_F(arg0, 0x04) = M_F(arg0, 0x04) * s;
            M_F(arg0, 0x08) = M_F(arg0, 0x08) * s;
            M_F(arg0, 0x0C) = 1.0f;
            M_F(arg0, 0x10) = M_F(arg0, 0x10) * s;
            M_F(arg0, 0x1C) = 1.0f;
            M_F(arg0, 0x14) = M_F(arg0, 0x14) * s;
            M_F(arg0, 0x18) = M_F(arg0, 0x18) * s;
            M_F(arg0, 0x20) = M_F(arg0, 0x20) * s;
            M_F(arg0, 0x2C) = 1.0f;
            M_F(arg0, 0x24) = M_F(arg0, 0x24) * s;
            M_F(arg0, 0x28) = M_F(arg0, 0x28) * s;
            M_F(arg0, 0x0C) = 0.0f;
            M_F(arg0, 0x2C) = 0.0f;
            M_F(arg0, 0x1C) = 0.0f;
            s2 = 1.0f / M_F(arg1, 0x40);
            M_F(arg0, 0x40) = s2;
            M_F(arg0, 0x48) = s2;
            M_F(arg0, 0x4C) = 1.0f;
            M_F(arg0, 0x44) = s2;
        }
        M_NEGROW(arg0, arg1);
        M_DOTROWS(arg0);
        M_F(arg0, 0x3C) = 1.0f;
        return;
    }

    if (t == 7) {
        *(u8 *)(arg0 + 0x50) = 8;
        {
            f32 s0;
            f32 s1;
            f32 s2;

            s0 = 1.0f / (M_F(arg1, 0x40) * M_F(arg1, 0x40));
            s1 = 1.0f / (M_F(arg1, 0x44) * M_F(arg1, 0x44));
            s2 = 1.0f / (M_F(arg1, 0x48) * M_F(arg1, 0x48));
            M_F(arg0, 0x00) = M_F(arg1, 0x00) * s0;
            M_F(arg0, 0x04) = M_F(arg1, 0x04) * s0;
            M_F(arg0, 0x0C) = 1.0f;
            M_F(arg0, 0x08) = M_F(arg1, 0x08) * s0;
            M_F(arg0, 0x10) = M_F(arg1, 0x10) * s1;
            M_F(arg0, 0x14) = M_F(arg1, 0x14) * s1;
            M_F(arg0, 0x1C) = 1.0f;
            M_F(arg0, 0x18) = M_F(arg1, 0x18) * s1;
            M_F(arg0, 0x20) = M_F(arg1, 0x20) * s2;
            M_F(arg0, 0x24) = M_F(arg1, 0x24) * s2;
            M_F(arg0, 0x2C) = 1.0f;
            M_F(arg0, 0x28) = M_F(arg1, 0x28) * s2;
        }
        M_F(arg0, 0x30) = M_F(arg1, 0x30);
        M_F(arg0, 0x34) = M_F(arg1, 0x34);
        M_F(arg0, 0x38) = M_F(arg1, 0x38);
        M_F(arg0, 0x3C) = M_F(arg1, 0x3C);
        M_F(arg0, 0x2C) = 0.0f;
        M_F(arg0, 0x1C) = 0.0f;
        M_F(arg0, 0x0C) = 0.0f;
        M_TRANSPOSE(arg0);
        M_NEGROW(arg0, arg1);
        M_DOTROWS(arg0);
        M_F(arg0, 0x3C) = 1.0f;
        return;
    }

    if (t == 8) {
        {
            u8 *d;
            f32 c00, c01, c02, c10, c11, c12, c20, c21, c22;
            f32 det;

            d = (arg0 != arg1) ? arg0 : tmp;
            *(u8 *)(d + 0x50) = 8;
            c00 = M_F(arg1, 0x14) * M_F(arg1, 0x28) - M_F(arg1, 0x18) * M_F(arg1, 0x24);
            M_F(d, 0x00) = c00;
            c10 = M_F(arg1, 0x18) * M_F(arg1, 0x20) - M_F(arg1, 0x10) * M_F(arg1, 0x28);
            M_F(d, 0x10) = c10;
            c20 = M_F(arg1, 0x10) * M_F(arg1, 0x24) - M_F(arg1, 0x14) * M_F(arg1, 0x20);
            M_F(d, 0x20) = c20;
            c01 = M_F(arg1, 0x08) * M_F(arg1, 0x24) - M_F(arg1, 0x04) * M_F(arg1, 0x28);
            det = M_F(arg1, 0x00) * c00 + M_F(arg1, 0x04) * c10 + M_F(arg1, 0x08) * c20;
            M_F(d, 0x04) = c01;
            c02 = M_F(arg1, 0x04) * M_F(arg1, 0x18) - M_F(arg1, 0x08) * M_F(arg1, 0x14);
            M_F(d, 0x08) = c02;
            det = 1.0f / det;
            M_F(d, 0x04) = det * c01;
            M_F(d, 0x08) = det * c02;
            M_F(d, 0x00) = det * c00;
            c11 = M_F(arg1, 0x00) * M_F(arg1, 0x28) - M_F(arg1, 0x08) * M_F(arg1, 0x20);
            M_F(d, 0x14) = c11;
            c12 = M_F(arg1, 0x08) * M_F(arg1, 0x10) - M_F(arg1, 0x00) * M_F(arg1, 0x18);
            M_F(d, 0x18) = c12;
            c21 = M_F(arg1, 0x04) * M_F(arg1, 0x20) - M_F(arg1, 0x00) * M_F(arg1, 0x24);
            M_F(d, 0x24) = c21;
            c22 = M_F(arg1, 0x00) * M_F(arg1, 0x14) - M_F(arg1, 0x04) * M_F(arg1, 0x10);
            M_F(d, 0x28) = c22;
            M_F(d, 0x0C) = 1.0f;
            M_F(d, 0x10) = det * c10;
            M_F(d, 0x1C) = 1.0f;
            M_F(d, 0x14) = det * c11;
            M_F(d, 0x18) = det * c12;
            M_F(d, 0x20) = det * c20;
            M_F(d, 0x2C) = 1.0f;
            M_F(d, 0x24) = det * c21;
            M_F(d, 0x28) = det * c22;
            M_F(d, 0x2C) = 0.0f;
            M_F(d, 0x1C) = 0.0f;
            M_F(d, 0x0C) = 0.0f;
            M_NEGROW(d, arg1);
            M_DOTROWS(d);
            M_NEGROW(d, arg1);
            M_DOTROWS(d);
            if (arg0 == arg1)
                M_COPY(arg0, d, 0x54);
        }
        return;
    }
}

#undef M_F
#undef M_COPY
#undef M_TRANSPOSE
#undef M_NEGROW
#undef M_DOTROWS

INCLUDE_ASM(const s32, "core/main", func_001224F8);

INCLUDE_ASM(const s32, "core/main", func_001232E8);

void func_001241D8(u8 *arg0, f32 *arg1)
{
    s32 state;
    f32 v3;
    f32 v4;

    state = arg0[0x50];
    if (state == 1)
        goto hy4x_case1;
    if (state != 0)
    {
        if (state < 9)
        {
            v3 = *(f32 *)(arg0 + 52);
            v4 = *(f32 *)(arg0 + 56);
            *(f32 *)(arg0 + 48) = *(f32 *)(arg0 + 48) + arg1[0];
            *(f32 *)(arg0 + 52) = v3 + arg1[1];
            *(f32 *)(arg0 + 56) = v4 + arg1[2];
            return;
        }
    }
    return;
hy4x_case1:
    arg0[0x50] = 2;
    *(S12_flash *)(arg0 + 48) = *(S12_flash *)arg1;
    *(f32 *)(arg0 + 60) = 1.0f;
}

/* The matrix macros above are #undef'd before func_001224F8; the classifier
 * below needs the field accessor again. */
#define M_F(p, o) (*(f32 *)((p) + (o)))

/* ------------------------------------------------------------------------- */
/* func_00124260 - transform node classifier (0x448 bytes)                    */
/* ------------------------------------------------------------------------- */
/* Classifies the 0x54-byte transform record (16 floats at +0x00: four rows of
 * four, translation in row 3 at +0x30/+0x34/+0x38 with 1.0 at +0x3C), stores
 * the scale factors the inverse path (func_00121888) expects at +0x40/+0x44/
 * +0x48 (w at +0x4C) and writes the type tag at +0x50:
 *
 *   1  identity                  (func_00120D48 writes this tag)
 *   2  translation only          matrix forced to identity, scale = 1
 *   3  uniform scale             +0x40/+0x44/+0x48 = the diagonal value
 *   4  scale                     +0x40/+0x44/+0x48 = the three diagonal values
 *   5  rotation                  rows have unit length, scale = 1
 *   6  uniform scale + rotation  +0x40/+0x44/+0x48 = sqrt(|row0|^2)
 *   7  scale + rotation          +0x40/+0x44/+0x48 = sqrt of each row length
 *   8  general (shear)           scale slots = D_0039318C (0x7F7FFFF8,
 *                                "not decomposable" marker)
 *
 * Nothing is returned: only the record is classified in place.  The function
 * has no stack frame and, when the record is already an identity, tail-calls
 * func_00120D48 (the identity reset).                                             */
extern f32 D_0039255C;          /* 0.0001f - off-diagonal is zero */
extern f32 D_00392560;          /* 0.0001f - diagonal entry is one */
extern f32 D_00392564;          /* 0.0001f - diagonal entries agree */
extern f32 D_00392568;          /* 0.0001f - length / orthogonality */
extern f32 D_0039318C;          /* 3.40282205e+38f (0x7F7FFFF8), unknown scale */

f32 fabsf(f32);

/* R5900 sqrt.s.  This module is compiled with -O2 only (no -ffast-math), so
 * sqrtf() would become a library call that the retail link does not provide;
 * spell the instruction out (same idiom as funcs_06).  ee-as encodes the
 * source operand in the ft field, e.g. `sqrt.s $f1,$f1` is 0x46010044, which
 * is exactly how the retail build encodes the three square roots below. */
#define F_SQRT(x) ({                                            \
    float r_;                                                   \
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(r_) : "f"(x));    \
    r_;                                                         \
})

/* Three-term dot product in the FPU accumulator, which rounds once like the
 * retail mula.s/madda.s/madd.s triples (a plain a*b+c*d+e*f rounds twice and
 * can flip the 1e-4 comparisons below at the last bit). */
#define F_DOT3(a, b, c, d, e, f) ({                             \
    float r_;                                                   \
    __asm__ volatile ("mula.s %1,%2\n\t"                        \
                      "madda.s %3,%4\n\t"                       \
                      "madd.s %0,%5,%6"                         \
                      : "=&f"(r_)                               \
                      : "f"(a), "f"(b), "f"(c),                 \
                        "f"(d), "f"(e), "f"(f));                \
    r_;                                                         \
})

void func_00124260(u8 *arg0)
{
    f32 d0;
    f32 d1;
    f32 d2;
    f32 mx;
    f32 s;
    s32 diagonal;
    s32 identity;
    s32 untranslated;

    /* the 3x3 part is diagonal (all six off-diagonal entries are zero) */
    diagonal = 0;
    if (fabsf(M_F(arg0, 0x04)) < D_0039255C &&
        fabsf(M_F(arg0, 0x08)) < D_0039255C &&
        fabsf(M_F(arg0, 0x10)) < D_0039255C &&
        fabsf(M_F(arg0, 0x18)) < D_0039255C &&
        fabsf(M_F(arg0, 0x20)) < D_0039255C &&
        fabsf(M_F(arg0, 0x24)) < D_0039255C)
        diagonal = 1;

    /* ... and diagonal with unit scale, i.e. an identity rotation */
    identity = 0;
    if (diagonal &&
        fabsf(M_F(arg0, 0x00) - 1.0f) < D_00392560 &&
        fabsf(M_F(arg0, 0x14) - 1.0f) < D_00392560 &&
        fabsf(M_F(arg0, 0x28) - 1.0f) < D_00392560)
        identity = 1;

    /* exact zero test on the translation row */
    untranslated = 0;
    if (M_F(arg0, 0x30) == 0.0f && M_F(arg0, 0x34) == 0.0f &&
        M_F(arg0, 0x38) == 0.0f)
        untranslated = 1;

    if (identity) {
        if (untranslated != 0) {
            /* fully identity: let the canonical reset tag and fill it */
            func_00120D48((u32)arg0);
            return;
        }

        /* pure translation: identity rotation, scale 1, translation kept */
        *(u8 *)(arg0 + 0x50) = 2;
        M_F(arg0, 0x00) = 1.0f;
        M_F(arg0, 0x04) = 0.0f;
        M_F(arg0, 0x08) = 0.0f;
        M_F(arg0, 0x0C) = 0.0f;
        M_F(arg0, 0x10) = 0.0f;
        M_F(arg0, 0x14) = 1.0f;
        M_F(arg0, 0x18) = 0.0f;
        M_F(arg0, 0x1C) = 0.0f;
        M_F(arg0, 0x20) = 0.0f;
        M_F(arg0, 0x24) = 0.0f;
        M_F(arg0, 0x28) = 1.0f;
        M_F(arg0, 0x2C) = 0.0f;
        M_F(arg0, 0x40) = 1.0f;
        M_F(arg0, 0x44) = 1.0f;
        M_F(arg0, 0x48) = 1.0f;
        M_F(arg0, 0x4C) = 1.0f;
        return;
    }

    if (diagonal) {
        if (fabsf(M_F(arg0, 0x00) - M_F(arg0, 0x14)) < D_00392564 &&
            fabsf(M_F(arg0, 0x00) - M_F(arg0, 0x28)) < D_00392564) {
            /* uniform scale */
            *(u8 *)(arg0 + 0x50) = 3;
            M_F(arg0, 0x40) = M_F(arg0, 0x00);
            M_F(arg0, 0x44) = M_F(arg0, 0x00);
            M_F(arg0, 0x48) = M_F(arg0, 0x00);
            M_F(arg0, 0x4C) = 1.0f;
            return;
        }

        /* non-uniform scale */
        *(u8 *)(arg0 + 0x50) = 4;
        M_F(arg0, 0x40) = M_F(arg0, 0x00);
        M_F(arg0, 0x44) = M_F(arg0, 0x14);
        M_F(arg0, 0x48) = M_F(arg0, 0x28);
        M_F(arg0, 0x4C) = 1.0f;
        return;
    }

    /* squared length of each basis row (row 0 at +0x00, row 1 at +0x10,
     * row 2 at +0x20) */
    d0 = F_DOT3(M_F(arg0, 0x00), M_F(arg0, 0x00),
                M_F(arg0, 0x04), M_F(arg0, 0x04),
                M_F(arg0, 0x08), M_F(arg0, 0x08));
    d2 = F_DOT3(M_F(arg0, 0x10), M_F(arg0, 0x10),
                M_F(arg0, 0x14), M_F(arg0, 0x14),
                M_F(arg0, 0x18), M_F(arg0, 0x18));
    d1 = F_DOT3(M_F(arg0, 0x20), M_F(arg0, 0x20),
                M_F(arg0, 0x24), M_F(arg0, 0x24),
                M_F(arg0, 0x28), M_F(arg0, 0x28));
    M_F(arg0, 0x40) = d0;
    M_F(arg0, 0x44) = d2;
    M_F(arg0, 0x48) = d1;
    M_F(arg0, 0x4C) = 1.0f;

    mx = d0;
    if (mx < d2)
        mx = d2;
    if (mx < d1)
        mx = d1;

    if (fabsf(mx - 1.0f) < D_00392568) {
        /* every row already has unit length: pure rotation */
        *(u8 *)(arg0 + 0x50) = 5;
        M_F(arg0, 0x40) = 1.0f;
        M_F(arg0, 0x44) = 1.0f;
        M_F(arg0, 0x48) = 1.0f;
        M_F(arg0, 0x4C) = 1.0f;
        return;
    }

    /* the three rows are mutually orthogonal: rotation times a diagonal scale */
    if (fabsf(F_DOT3(M_F(arg0, 0x00), M_F(arg0, 0x10),
                     M_F(arg0, 0x04), M_F(arg0, 0x14),
                     M_F(arg0, 0x08), M_F(arg0, 0x18))) < D_00392568 &&
        fabsf(F_DOT3(M_F(arg0, 0x00), M_F(arg0, 0x20),
                     M_F(arg0, 0x04), M_F(arg0, 0x24),
                     M_F(arg0, 0x08), M_F(arg0, 0x28))) < D_00392568 &&
        fabsf(F_DOT3(M_F(arg0, 0x10), M_F(arg0, 0x20),
                     M_F(arg0, 0x14), M_F(arg0, 0x24),
                     M_F(arg0, 0x18), M_F(arg0, 0x28))) < D_00392568) {
        if (fabsf(d0 - d2) < D_00392568 && fabsf(d0 - d1) < D_00392568) {
            /* uniform scale + rotation */
            *(u8 *)(arg0 + 0x50) = 6;
            s = F_SQRT(M_F(arg0, 0x40));
            M_F(arg0, 0x40) = s;
            M_F(arg0, 0x44) = s;
            M_F(arg0, 0x48) = s;
            M_F(arg0, 0x4C) = 1.0f;
            return;
        }

        /* non-uniform scale + rotation */
        *(u8 *)(arg0 + 0x50) = 7;
        M_F(arg0, 0x40) = F_SQRT(M_F(arg0, 0x40));
        M_F(arg0, 0x44) = F_SQRT(M_F(arg0, 0x44));
        M_F(arg0, 0x48) = F_SQRT(M_F(arg0, 0x48));
        M_F(arg0, 0x4C) = 1.0f;
        return;
    }

    /* skewed basis: the scale cannot be extracted (retail loads the marker
     * once and stores it in all three slots) */
    s = D_0039318C;
    *(u8 *)(arg0 + 0x50) = 8;
    M_F(arg0, 0x40) = s;
    M_F(arg0, 0x44) = s;
    M_F(arg0, 0x48) = s;
    M_F(arg0, 0x4C) = 1.0f;
}

#undef M_F

void func_001246A8(u8 *a1, u8 *a2, u8 *a3)
{
    typedef struct { UnalignedU64 lo; u32 w; } S12;

    *(S12 *)a1 = *(S12 *)a2;
    *(f32 *)(a1 + 0xC) = 1.0f;
    *(S12 *)(a1 + 0x10) = *(S12 *)(a2 + 0xC);
    *(f32 *)(a1 + 0x1C) = 1.0f;
    *(S12 *)(a1 + 0x20) = *(S12 *)(a2 + 0x18);
    *(f32 *)(a1 + 0x3C) = 1.0f;
    *(f32 *)(a1 + 0x2C) = 1.0f;
    *(s32 *)(a1 + 0x38) = 0;
    *(s32 *)(a1 + 0x34) = 0;
    *(s32 *)(a1 + 0x30) = 0;
    *(S12 *)(a1 + 0x30) = *(S12 *)a3;
    *(f32 *)(a1 + 0x3C) = 1.0f;
    func_00124260(a1);
}

INCLUDE_ASM(const s32, "core/main", func_00124738);

INCLUDE_ASM(const s32, "core/main", func_00124988);

INCLUDE_ASM(const s32, "core/main", func_00124BB0);

INCLUDE_ASM(const s32, "core/main", func_00124EA8);

void func_001250C8(u8 *arg0, f32 *arg1)
{
    f32 *p;

    *(u8 *)(arg0 + 0x50) = 5;
    p = (f32 *)(arg0 + 0x40);
    *(f32 *)(arg0 + 0x40) = 1.0f;
    p[1] = 1.0f;
    p[2] = 1.0f;
    p[3] = 1.0f;
    func_00127AE8(arg0, arg1[0], arg1[1], arg1[2], arg1[3]);
}

INCLUDE_ASM(const s32, "core/main", func_00125100);

INCLUDE_ASM(const s32, "core/main", func_001252F0);

INCLUDE_ASM(const s32, "core/main", func_001255C8);

INCLUDE_ASM(const s32, "core/main", func_00125B18);

INCLUDE_ASM(const s32, "core/main", func_00125C80);

INCLUDE_ASM(const s32, "core/main", func_00125DE8);

INCLUDE_ASM(const s32, "core/main", func_00126160);

INCLUDE_ASM(const s32, "core/main", func_00126210);

INCLUDE_ASM(const s32, "core/main", func_001262F8);

INCLUDE_ASM(const s32, "core/main", func_001263AC);

INCLUDE_ASM(const s32, "core/main", func_00126460);

INCLUDE_ASM(const s32, "core/main", func_00126510);

INCLUDE_ASM(const s32, "core/main", func_00126578);

INCLUDE_ASM(const s32, "core/main", func_001265E0);

INCLUDE_ASM(const s32, "core/main", func_0012664C);

INCLUDE_ASM(const s32, "core/main", func_001266B0);

INCLUDE_ASM(const s32, "core/main", func_00126868);

INCLUDE_ASM(const s32, "core/main", func_00126B00);

INCLUDE_ASM(const s32, "core/main", func_00126C40);

INCLUDE_ASM(const s32, "core/main", func_00126D50);

INCLUDE_ASM(const s32, "core/main", func_00126EF0);

INCLUDE_ASM(const s32, "core/main", func_00126FB4);

INCLUDE_ASM(const s32, "core/main", func_00127020);

INCLUDE_ASM(const s32, "core/main", func_00127108);

INCLUDE_ASM(const s32, "core/main", func_00127330);

INCLUDE_ASM(const s32, "core/main", func_001273F4);

INCLUDE_ASM(const s32, "core/main", func_001274C0);

INCLUDE_ASM(const s32, "core/main", func_001275C0);

void func_00127868(u8 *arg0, f32 *arg1)
{
    typedef struct { UnalignedU64 v; u32 w; } S12;
    u8 *p = arg0;
    u32 kind = p[0x50];

    /* Retail dispatches this through jtbl_00389820 (8 entries at 0x00389820,
     * already linked from asm/data/main/rodata.rodata.s): 1, 2 and 5 -> 1.0f;
     * 3, 4, 6 and 7 -> copy 12 bytes; 8 and everything else -> return.
     * Written as a compare chain because a `switch` here emits a second
     * .rodata table, and this linker script places main.c.o(.rodata*) inside
     * .main, which then overflows its retail end and the link fails. */
    if (kind == 1 || kind == 2 || kind == 5)
    {
        arg1[0] = 1.0f;
        arg1[1] = 1.0f;
        arg1[2] = 1.0f;
    }
    else if (kind >= 3 && kind <= 7)
    {
        *(S12 *)arg1 = *(S12 *)(p + 0x40);
    }
    else
    {
        return;
    }
}

INCLUDE_ASM(const s32, "core/main", func_001278D0);

INCLUDE_ASM(const s32, "core/main", func_001279E8);

static inline f32 hy4xI_sqrt(f32 x)
{
    __asm__ volatile ("sqrt.s %0,%0" : "+f"(x));
    return x;
}

f32 func_00127A68(u8 *p)
{
    u8 m = p[0x50];

    /* If-chain, not switch: gcc would emit a jump table into .rdata, which the
       link script folds into .main and pushes it past its retail end. */
    if ((u32)(m - 1) < 8) {
        if ((u32)(m - 3) < 2)
            return *(f32 *)(p + 0x44);
        if (m >= 6)
            return hy4xI_sqrt(*(f32 *)(p + 0x4) * *(f32 *)(p + 0x4)
                            + *(f32 *)(p + 0x14) * *(f32 *)(p + 0x14)
                            + *(f32 *)(p + 0x24) * *(f32 *)(p + 0x24));
        return 1.0f;
    }
    return 0.0f;
}

INCLUDE_ASM(const s32, "core/main", func_00127AE8);

INCLUDE_ASM(const s32, "core/main", func_00127BB8);

INCLUDE_ASM(const s32, "core/main", func_00127E10);

INCLUDE_ASM(const s32, "core/main", func_001281F8);

INCLUDE_ASM(const s32, "core/main", func_001283A0);

INCLUDE_ASM(const s32, "core/main", func_00128580);

INCLUDE_ASM(const s32, "core/main", func_00128618);

INCLUDE_ASM(const s32, "core/main", func_00128C40);

INCLUDE_ASM(const s32, "core/main", func_00128F88);

INCLUDE_ASM(const s32, "core/main", func_00128FC8);

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
} SolverVec;

s32 func_00129038(SolverVec *list, SolverVec *normal, s32 count)
{
    u8 *p = (u8 *)list;
    u8 *n = (u8 *)normal;
    u32 i;

    for (i = 0; i < (u32)count; i++) {
        u8 *v = p + i * 12;
        if (*(f32 *)v == *(f32 *)n &&
            *(f32 *)(v + 4) == *(f32 *)(n + 4) &&
            *(f32 *)(v + 8) == *(f32 *)(n + 8))
            return 1;
    }
    return 0;
}
/* func_001290B8 - collision-aware motion solver.
 *
 * Live code: func_0012A628 is the one caller (jal at 0x0012A6A0); no other jal and
 * no function-pointer word anywhere in the image holds 0x001290B8.
 *
 * Behavioural reconstruction from asm/matchings/core/main/func_001290B8.s.  Four
 * retail properties drive the control flow and are easy to get wrong:
 *
 *  - func_00129038 does not fill a buffer: it *searches* the first `count` entries of
 *    its first argument for `normal` and returns 1 on a hit.  So status != 0 means
 *    "this probe direction was already tried" and takes the contact path, while
 *    status == 0 records the probe in candA, slides the position, and retries.
 *  - the state block at 0x0012945C runs at most once per call (it sets bit 0 of
 *    n[0x168], which gates it) and scales its two probe vectors by `step` divided by
 *    their lengths.  Retail reads that factor out of $f20; the only caller
 *    (func_0012A628, jal at 0x0012A6A0) sets $f12 = $f20 just before the call
 *    (mov.s $f20,$f12 at 0x0012A654, mov.s $f12,$f20 at 0x0012A668), so the same
 *    value arrives here as the seventh argument and no register is touched.
 *  - the contact path rewrites the local target (0x00129D40: target = pos + the
 *    remaining vector); the swept test that follows uses the rewritten value.
 *  - the remaining vector is always taken from the caller's arg4 (0x001293F0 and
 *    0x00129884 read the arg4 slot), never from that rewritten target.
 *  - the fall-through of the 0x00129038 duplicate test at 0x00129B28/0x00129B30 is a
 *    pointer comparison the compiler could never satisfy, so only the 0x00129BB0
 *    cross product is ever written.
 *  - it is NOT void.  Its only caller (func_0012A628) branches on $2 (`beqz $2` at
 *    0x0012A6A8), and retail leaves $2 non-zero on every path: the two
 *    "degenerate fixup" shortcuts store 1 (addiu $2,$0,1 at 0x00129F64 and
 *    0x0012A030), and the main path leaves func_0012B1B0's return value there,
 *    which is that callee's own `out` argument (`daddu $2,$16,$0` at 0x0012B3A4),
 *    i.e. the address of this frame's scratch slot.  0x00129EA4 clears $2 first,
 *    so the leftovers from the mid-function paths never reach the caller.
 *    Declaring the function void left $2 to the compiler - the build ended with
 *    `lw $2,pos.z`, so a pos.z of exactly +0.0 made the caller take its
 *    "solver failed" path.
 */

extern f32 D_003925F8;   /* 0.001f  */
extern f32 D_003925FC;   /* 0.02f   */
extern f32 D_00392600;   /* 0.02f   */
extern f32 D_00392604;   /* -0.7f   */
extern f32 D_00392608;   /* 0.7f    */
extern f32 D_0039260C;   /* 0.5235988f  (pi/6) */
extern f32 D_00392610;   /* 0.001f  */
extern f32 D_00392614;   /* 0.02f   */
extern f32 D_00392618;   /* 0.001f  */
extern f32 D_0039261C;   /* 0.001f  */
extern f32 D_00392620;   /* 2.6179938f  (5pi/6) */
extern f32 D_00392624;   /* 0.02f   */
extern f32 D_00392628;   /* 0.001f  */
extern f32 D_0039262C;   /* 0.001f  */

typedef struct
{
    f32 m[12];      /* 0x00, filled by func_00120D48 */
    f32 pos[3];     /* 0x30, rewritten before every swept test */
    f32 one;        /* 0x3C */
    f32 unk[4];     /* 0x40, never touched here */
    u8  kind;       /* 0x50, 1 becomes 2 before a swept test */
    u8  pad[15];
} SolverPose;

extern s32 func_00128FC8(SolverVec *pos, SolverVec *target, f32 eps);
extern s32 func_00129038(SolverVec *list, SolverVec *normal, s32 count);
extern s32 func_0012B1B0(f32 *out, SolverVec *a, SolverVec *b);
extern s32 func_0012B098(SolverVec *dst, SolverVec *a, SolverVec *b, SolverVec *c);
extern s32 func_00128C40(u8 *mover, u8 *ctx, SolverPose *a, SolverPose *b);

/* Retail inlines every one of these (1044 instructions, no internal calls), so they
 * are spelled as macros rather than functions.  Each site reads its own constant from
 * the 0x003926xx pool; they all hold 0.001f, but the address used at each site is
 * kept so the correspondence with the listing stays visible. */
#define SV_TINY3(p, e)                                                      \
    (fabsf((p)->x) < (e) && fabsf((p)->y) < (e) && fabsf((p)->z) < (e))
#define SV_ZERO3(p) SV_TINY3(p, D_003925F8)
#define SV_NORM(p)                                                          \
    do {                                                                    \
        f32 svl_ = F_SQRT(F_DOT3((p)->x, (p)->x, (p)->y, (p)->y,            \
                                 (p)->z, (p)->z));                          \
        if (svl_ != 0.0f) {                                                 \
            f32 svk_ = 1.0f / svl_;                                         \
            (p)->z = svk_ * (p)->z;                                         \
            (p)->x = svk_ * (p)->x;                                         \
            (p)->y = svk_ * (p)->y;                                         \
        }                                                                   \
    } while (0)

s32 func_001290B8(u8 *arg0, u8 *arg1, u8 *arg2, u8 *arg3, SolverVec *arg4,
                  SolverVec *arg5, f32 step)
{
    f32  scratch[4];        /* retail's sp+0: func_0012B1B0 writes out[0..3] */
    SolverVec candA[6];     /* retail's sp+0x10, stride 0xC, indexed by countA */
    SolverVec trace[6];     /* retail's sp+0x50, stride 0xC, indexed by countB */
    SolverVec t90, t9C, tA8, tB4, pos, target, nrm, source, tmp, rem, crv;
    SolverPose nodeA, nodeB;
    SolverVec *prev;
    u8   *n;
    s32   countA, countB, blocked, status, ret;
    f32   f8, len, k;

    n       = *(u8 **)(arg1 + 0x148);
    countA  = 0;
    countB  = 0;
    blocked = 0;
    status  = 0;

    /* 0x00129114: start position from the node behind arg3 */
    source = *(SolverVec *)(*(u8 **)(arg3 + 0x24) + 0x30);
    pos    = source;
    target = *arg4;
    nrm    = *(SolverVec *)(n + 0xF8);

    /* 0x001291A8: normalise the running displacement */
    if (!(SV_ZERO3(&nrm)))
        SV_NORM(&nrm);

    /* 0x00129270 */
    n[0x168] &= 0xFE;
    *(u32 *)(n + 0x04) = 0;

    /* 0x00129280: "already resolved" flag */
    if ((*(u16 *)(arg1 + 0x256) & 0x40) != 0) {
        pos = *arg4;
        goto epilogue;
    }

    /* 0x00129368 */
    func_00120D48(&nodeA);
    func_00120D48(&nodeB);
    if (func_00128FC8(&pos, &target, D_00392600) != 0) {
        if ((*(u16 *)(arg1 + 0x256) & 0x100) != 0)
            goto epilogue;
    }
    goto swept;

/* ---------------- main iteration loop ---------------- */
loop:
    /* 0x001293C0 */
    *(SolverVec *)(n + 0x0C) = *(SolverVec *)(arg0 + 0x04);
    t90 = *(SolverVec *)(arg0 + 0x04);

    /* 0x001293F0: the remaining distance comes from the caller's vector, not
       from the running `target`, which the contact path rewrites */
    rem.x = arg4->x - source.x;
    rem.y = arg4->y - source.y;
    rem.z = arg4->z - source.z;
    len   = F_SQRT(F_DOT3(rem.x, rem.x, rem.y, rem.y, rem.z, rem.z));

    /* 0x00129438: the once-per-call state block */
    if ((n[0x168] & 1) == 0 && len != 0.0f) {
        f32 ratio;
        f32 len2;
        f32 dot;

        n[0x168] &= 0xFD;
        ratio = F_DOT3(rem.x, t90.x, rem.y, t90.y, rem.z, t90.z) / len;
        /* 0x001294A0 is a bc1fl on `ratio < -0.7`: the block runs only while the
           movement opposes the probe direction */
        if (ratio < D_00392604) {
            tB4.x = *(f32 *)(arg1 + 0x258) - source.x;
            tB4.y = *(f32 *)(arg1 + 0x25C) - source.y;
            tB4.z = *(f32 *)(arg1 + 0x260) - source.z;
            k = step / len;
            rem.x *= k;
            rem.y *= k;
            rem.z *= k;
            len2 = F_SQRT(F_DOT3(tB4.x, tB4.x, tB4.y, tB4.y, tB4.z, tB4.z));
            if (len2 != 0.0f) {
                k = step / len2;
                tB4.z *= k;
                tB4.x *= k;
                tB4.y *= k;
            }
            dot = F_DOT3(rem.x, tB4.x, rem.y, tB4.y, rem.z, tB4.z);
            if (dot > D_00392608)
                n[0x168] |= 2;
        }
    }
    /* 0x001295A4 */
    n[0x168] |= 1;
    *(u32 *)(n + 0x04) = 4;

    /* 0x001295B8: a negative speed pushes the object off the wall instead of
       stepping towards the target */
    if (*(f32 *)arg0 < 0.0f) {
        SolverVec v = *(SolverVec *)(arg0 + 0x30);
        f32 d = F_SQRT(F_DOT3(v.x, v.x, v.y, v.y, v.z, v.z));
        f32 s = 1.0f / d;
        f32 e = d + D_003925FC;

        n[0x168] |= 1;
        pos.x = target.x + (s * v.x) * e;
        pos.y = target.y + (s * v.y) * e;
        pos.z = target.z + (s * v.z) * e;
        goto epilogue;
    }

    /* 0x001295D0: tmp = pos + (target - pos) * arg0->t */
    tmp.x = target.x - pos.x;
    tmp.y = target.y - pos.y;
    tmp.z = target.z - pos.z;
    tmp.x = *(f32 *)arg0 * tmp.x;
    tmp.y = *(f32 *)arg0 * tmp.y;
    tmp.z = *(f32 *)arg0 * tmp.z;
    tmp.x += pos.x;
    tmp.y += pos.y;
    tmp.z += pos.z;

    status = func_00129038(&candA[0], &t90, countA);
    if (status != 0)
        goto dispatch;

    /* 0x00129658: the probe is free - record it and slide */
    if (countA == 5) {
        pos = source;                   /* 0x00129348 */
        goto epilogue;
    }
    candA[countA] = t90;
    countA++;

    t9C.x = tmp.x - *(f32 *)(arg0 + 0x10);
    t9C.y = tmp.y - *(f32 *)(arg0 + 0x14);
    t9C.z = tmp.z - *(f32 *)(arg0 + 0x18);
    SV_NORM(&t9C);
    func_0012B1B0(scratch, &t9C, &t90);
    if (scratch[0] < D_0039260C)
        t9C = t90;

    /* 0x00129750: shorten the step by the skin width */
    tA8.x = tmp.x - pos.x;
    tA8.y = tmp.y - pos.y;
    tA8.z = tmp.z - pos.z;
    if (!(SV_TINY3(&tA8, D_00392610))) {
        len = F_SQRT(F_DOT3(tA8.x, tA8.x, tA8.y, tA8.y, tA8.z, tA8.z));
        if (len - D_00392614 < 0.0f) {
            tA8.x = 0.0f;
            tA8.y = 0.0f;
            tA8.z = 0.0f;
        } else {
            k = (len - D_00392614) / len;
            tA8.z *= k;
            tA8.x *= k;
            tA8.y *= k;
        }
        pos.x += tA8.x;
        pos.y += tA8.y;
        pos.z += tA8.z;
    }

    /* 0x00129884: what is left, with the contact normal projected out */
    tA8.x = arg4->x - pos.x;
    tA8.y = arg4->y - pos.y;
    tA8.z = arg4->z - pos.z;
    func_0012B098(&tA8, &tA8, &t9C, &t9C);

    if (!(SV_TINY3(&nrm, D_00392618)) && !(SV_TINY3(&tA8, D_0039261C))) {
        len = F_SQRT(F_DOT3(tA8.x, tA8.x, tA8.y, tA8.y, tA8.z, tA8.z));
        if (len != 0.0f) {
            k = 1.0f / len;
            tA8.x = k * tA8.x;
            tA8.y = k * tA8.y;
            tA8.z = k * tA8.z;
        }
        func_0012B1B0(scratch, &tA8, &nrm);
        if (scratch[0] > D_00392620)
            blocked = 1;
    }

dispatch:
    /* 0x00129A30 */
    if (blocked == 0 && status == 0)
        goto nrm_decision;

contact:
    /* 0x00129A48 */
    if (countA < 2)
        goto duplicate;
    prev = &candA[countA - 1];
    if (t90.x == prev->x && t90.y == prev->y && t90.z == prev->z)
        goto duplicate;
    if (countB == 5)
        goto duplicate;

    /* 0x00129ACC */
    tA8.x = arg4->x - pos.x;
    tA8.y = arg4->y - pos.y;
    tA8.z = arg4->z - pos.z;
    crv.x = prev->y * t90.z - prev->z * t90.y;
    crv.y = prev->z * t90.x - prev->x * t90.z;
    crv.z = prev->x * t90.y - prev->y * t90.x;
    SV_NORM(&crv);
    if (func_00129038(&trace[0], &crv, countB) != 0)
        goto epilogue;
    trace[countB] = crv;
    countB++;
    func_0012B098(&tB4, &tA8, &crv, &crv);
    tA8.x -= tB4.x;
    tA8.y -= tB4.y;
    tA8.z -= tB4.z;

nrm_decision:
    /* 0x00129CF0 */
    if (tA8.x == 0.0f && tA8.y == 0.0f && tA8.z == 0.0f) {
        nrm.x = 0.0f;
        nrm.y = 0.0f;
        nrm.z = 0.0f;
    } else {
        target.x = pos.x + tA8.x;
        target.y = pos.y + tA8.y;
        target.z = pos.z + tA8.z;
        SV_NORM(&tA8);
        nrm = tA8;
    }

    /* 0x00129DE0 */
    if (func_00128FC8(&pos, &target, D_00392624) != 0)
        goto epilogue;

swept:
    /* 0x00129DF8: build the two poses and ask for one more swept test */
    if (nodeA.kind == 1)
        nodeA.kind = 2;
    nodeA.pos[0] = pos.x;
    nodeA.pos[1] = pos.y;
    nodeA.pos[2] = pos.z;
    nodeA.one     = 1.0f;
    if (nodeB.kind == 1)
        nodeB.kind = 2;
    nodeB.pos[0] = target.x;
    nodeB.pos[1] = target.y;
    nodeB.pos[2] = target.z;
    nodeB.one     = 1.0f;
    if (func_00128C40(arg0, arg3, &nodeA, &nodeB) > 0)
        goto loop;

    pos = target;

epilogue:
    /* 0x00129EA0: displacement written back into the node */
    ((SolverVec *)(n + 0xF8))->x = pos.x - source.x;
    ((SolverVec *)(n + 0xF8))->y = pos.y - source.y;
    ((SolverVec *)(n + 0xF8))->z = pos.z - source.z;

    /* 0x00129EF8: second half.  $f8 is seeded with 0.0f here and only replaced when
       both vectors are non-degenerate, so a degenerate fixup reports 0.0.  $2 is
       seeded with 1 for the same reason: each tiny-vector shortcut returns that
       boolean, and 0x00129EA4 clears $2 before the tests, so no path returns 0. */
    f8  = 0.0f;
    ret = 1;
    tA8.x = *(f32 *)(arg1 + 0x258) - source.x;
    tA8.y = *(f32 *)(arg1 + 0x25C) - source.y;
    tA8.z = *(f32 *)(arg1 + 0x260) - source.z;
    if (!(SV_TINY3(&tA8, D_00392628))) {
        SV_NORM(&tA8);
        tB4.x = *(f32 *)(arg1 + 0x258) - pos.x;
        tB4.y = *(f32 *)(arg1 + 0x25C) - pos.y;
        tB4.z = *(f32 *)(arg1 + 0x260) - pos.z;
        if (!(SV_TINY3(&tB4, D_0039262C))) {
            SV_NORM(&tB4);
            ret = func_0012B1B0(scratch, &tA8, &tB4);
            f8  = scratch[0];
        }
    }
    *(f32 *)(arg2 + 0x0C) = f8;
    *arg5 = pos;
    return ret;

duplicate:
    /* 0x00129AC0 */
    n[0x168] |= 2;
    goto epilogue;
}


INCLUDE_ASM(const s32, "core/main", func_0012A108);

INCLUDE_ASM(const s32, "core/main", func_0012A468);

INCLUDE_ASM(void, "core/main", func_0012A628);

INCLUDE_ASM(const s32, "core/main", func_0012A740);

void func_0012A860(void) { func_0012A740(); }

INCLUDE_ASM(const s32, "core/main", func_0012A868);

void func_0012ACF0(void *arg0, void *arg1)
{
    s32 result = 1;
    if (*(s32 *)arg1 == 1)
        result = func_0012A868(arg0, arg1);
    *(u32 *)((u8 *)arg0 + 4) = (s32)arg1;
}

INCLUDE_ASM(const s32, "core/main", func_0012AD38);

INCLUDE_ASM(const s32, "core/main", func_0012B098);

INCLUDE_ASM(const s32, "core/main", func_0012B1B0);

INCLUDE_ASM(const s32, "core/main", func_0012B3B8);

s32 func_0012B668();

s32 func_0012B668(s32 arg1)
{
    func_00224B88(arg1, 0, 364);
    return arg1;
}

s32 func_0012B698(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

s32 func_0012B6A0(s32 a1)
{
    s32 p;

    switch (a1)
    {
    case 0:
        p = func_00157DE8(D_0039347C, 0x10);
        *(s32 *)p = 0;
        break;
    case 1:
        p = func_00157DE8(D_0039347C, 0x3C);
        *(s32 *)p = a1;
        break;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "core/main", func_0012B700);

INCLUDE_ASM(const s32, "core/main", func_0012B800);

INCLUDE_ASM(const s32, "core/main", func_0012B908);

/* ------------------------------------------------------------------------- */
/* func_0012BA18 - per-frame update of a transform object (0x1D34 = 7476 B)  */
/* ------------------------------------------------------------------------- */
/* The object is a 0x114-byte-plus record that owns two 0x54-byte transform
 * nodes: node A at +0x78 and node B at +0xCC (in this function's terms).  The
 * routine is the "update" callback of the object vtable installed by
 * func_00101980 (ptr[1] = func_0012BA18), so it runs once per frame per object.
 *
 * It does not return a status: the return value is the object pointer itself
 * (`daddu $2,$16` on the way out), which the callers ignore.  Everything it
 * does is driven by two flag words:
 *   *(u32 *)(obj + 0x08)  behaviour bits, tested all over the function
 *   *(u32 *)(obj + 0x0C)  state bits, cleared/re-set as the update proceeds
 * plus a 16-bit-ish flag word at ctx+0x04 and the pointer at ctx+0x24 (the
 * node the object is supposed to mirror).
 *
 * Two mutually exclusive paths, selected by bit 0x200 of the +0x0C word:
 *   set    - "constrained" path: only integrates the stored velocity vector
 *            (+0x3C/+0x40/+0x44), copies node B into ctx->node and returns.
 *   clear  - the full update: up to `count` (= func_0014B930()) sub-steps of
 *            dt (= func_0014B910()), running the whole per-bit behaviour chain
 *            (func_00137CB8/func_00138170/func_00138390/... ) and finishing
 *            with the node bookkeeping at the tail.
 *
 * Arguments of the tail calls are always (obj, ctx, step) or
 * (obj, ctx_stack_node_copy, step, scratch node) - see the confirmed
 * prototypes below. */

/* 0x54-byte node copy, done as a struct assignment so that the compiler emits
 * an inline block move (this module links without a libc memcpy). */
typedef struct UBytes54 { u32 w[0x15]; } UBytes54;
typedef struct UBytes12 { u32 w[3]; } UBytes12;
#define U_COPY54(d, s) (*(UBytes54 *)(d) = *(UBytes54 *)(s))
#define U_COPY12(d, s) (*(UBytes12 *)(d) = *(UBytes12 *)(s))

/* Callees; all of them are still INCLUDE_ASM.  The float arguments must be
 * declared, otherwise an implicit int prototype would put the value in the
 * wrong register class. */
s32 func_00108618(u8 *);
s32 func_00108648(u8 *);
s32 func_0010C9F0(u8 *);
s32 func_0010CA50(u8 *);
s32 func_0013B440(u8 *);
f32 func_0014B910(void);
s32 func_0014B930(void);
void func_00121610(u8 *, u8 *, u8 *);
u32 func_00124BB0(u8 *, u8 *, f32);
s32 func_00124988(u8 *, u8 *);
void func_00126510(u8 *, f32 *);
void func_00127868(u8 *, f32 *);
void func_00138030(u8 *);
void func_00138170(u8 *, u8 *, f32, u8 *);
void func_00138390(u8 *, u8 *, f32, u8 *);
void func_001385D0(u8 *, u8 *, f32, u8 *);
void func_001387F0(u8 *, s32, f32);
void func_00138830(u8 *);
void func_00138840(u8 *);
void func_00138850(u8 *, f32 *);
void func_00138988(u8 *, u8 *, f32, u8 *);
void func_00138C28(u8 *, u8 *, u8 *);
void func_00138ED0(u8 *, u8 *, u8 *, u8 *);
u32 func_00138FA0(u8 *, u8 *);
/* Second argument is a gate: the body returns obj->f1C only when it is
 * non-zero (and obj->flags has 0x80), otherwise 0.0f.  Retail passes a
 * non-zero global address at both call sites, so any non-null value is
 * behaviourally identical. */
f32 func_00139828(u8 *, void *);
s32 func_00139800(u8 *);
s32 func_0013A148(u8 *, f32);
void func_0013A1C8(u8 *);
void func_0013A468(u8 *, u8 *);
void func_0013A4A8(u8 *, f32 *);
void func_0013A5D0(u8 *, f32 *);
void func_0013A610(u8 *, f32);
void func_0013B320(u8 *, f32 *);
void func_0013B388(u8 *, f32);
void func_00136970(u8 *);
void func_00137C68(u8 *, f32);
void func_00137CB8(u8 *, u8 *, f32, u8 *);
void func_00137E10(u8 *, f32);
void func_00133F40(u8 *, f32);
void func_0012B800(u8 *, u8 *, f32);
void func_0012B908(u8 *, u8 *, f32);
s32 func_0012F6F0(u8 *, u8 *, f32);
void func_0012F8D8(u8 *, u8 *, f32);
void func_00130888(u8 *, u8 *, f32);
s32 func_00131D78(u8 *, u8 *, f32);
void func_001340F0(u8 *, u8 *, f32);
s32 func_001346E8(u8 *, u8 *, f32);
void func_0014AD50(u8 *);
void func_0014AF20(u8 *);
void func_0014B050(u8 *);

extern f32 D_00392654;                  /* 0.0001f-ish tolerance, scale == 1 */
extern f32 D_00392658;                  /* argument handed to func_00124BB0 */
extern f32 D_00393198;                  /* "velocity is zero" threshold */
extern f32 D_0039319C;
extern f32 D_003931A0;
extern u32 D_003931A4;
extern u8  D_003931A8;
extern s32 D_003931B8;                  /* frame counter, saturates at 10 */

/* D_00330ED0 lives at 0x00330ED0, far outside gp range: retail addresses it
 * absolutely with lui/addiu, so it is reached through an absolute pointer here
 * (an extern would be assembled gp-relative and fail to link). Retail reads it
 * twice in the tail, spelled D_00330ED4[-1] and D_00330F3C[-27] in the listing;
 * both folds resolve to this same word. */
#define U_D30ED0 (*(u32 *)0x00330ED0)

u8 *func_0012BA18(u8 *obj, u8 *ctx)
{
    u32 n0[0x15];                       /* node copies (0x54 bytes each) */
    u32 n60[0x15];
    u32 nC0[0x15];
    u32 n120[0x15];
    u32 n180[0x15];
    f32 v1E0[3];
    f32 v1F0[3];
    f32 v200[3];
    f32 v210[3];
    f32 v220[7];                        /* +0x00..; +0x0C is the w slot */
    f32 v240[3];
    u32 v250[4];
    u8 *nodeA;                          /* obj + 0x78 */
    u8 *nodeB;                          /* obj + 0xCC */
    u8 *nodeC;                          /* obj + 0x2D4 */
    u8 *nodeD;                          /* obj + 0x280 */
    u8 *p148;                           /* *(obj + 0x148) */
    u8 *p24;                            /* *(ctx + 0x24) */
    u8 *pd;
    u32 ctrl;
    u32 flg8;
    f32 dt;
    f32 cnt;
    f32 step;
    f32 len;
    s32 count;
    s32 i;
    s32 n;
    s32 flag30;
    u32 r274;
    u32 r278;
    u32 r27C;
    u8 *scratch;                        /* 4th argument of func_00138ED0 */
    u32 done;
    s32 res;

    nodeA = obj + 0x78;
    nodeB = obj + 0xCC;
    nodeC = obj + 0x2D4;
    nodeD = obj + 0x280;
    p148 = 0;
    r274 = 0;
    r278 = 0;
    r27C = 0;

    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x4000) != 0) {
        p148 = *(u8 **)(obj + 0x148);
        if ((ctrl & 0x4004) == 0x4004) {
            for (i = 0; i < 7; i++)
                v220[i] = 0.0f;
        }
        r274 = (u32)p148;
        if ((*(u32 *)(p148 + 0x04) & 0x80) != 0)
            *(u32 *)(p148 + 0x04) = 0x10000;
        else
            *(u32 *)(p148 + 0x04) = 0;
        ctrl = *(u32 *)(obj + 0x0C);
    }
    /* mark "updated" and drop the pending-transform bit */
    ctrl = (ctrl | 0x2000) & 0xFEFFFFFF;
    *(u32 *)(obj + 0x0C) = ctrl;

    /* early out: invisible / not attached / nothing pending */
    if (func_00108618(ctx) == 0) {
        if (func_0013B440(obj) != 0) {
            ctrl = *(u32 *)(obj + 0x0C);
            if ((ctrl & 0x1080) == 0)
                return obj;
        }
    }

    dt = func_0014B910();
    count = func_0014B930();
    cnt = (f32)count;
    step = cnt * dt;

    ctrl = *(u32 *)(obj + 0x0C);

    func_00120D48((u8 *)n0);
    func_00120D48((u8 *)nC0);
    p24 = *(u8 **)(ctx + 0x24);
    func_00120D48((u8 *)n180);

    i = (s32)D_003931B8;
    *(u32 *)(obj + 0x0C) |= 0x800;
    U_D30ED0 = 0;
    D_003931A4 = 0;
    D_003931A8 = 0;
    if (i < 10)
        D_003931B8 = i + 1;

    /* Everything down to the 0x80 clear lives INSIDE `if (ctrl & 0x80)`: retail
     * jumps from 12BBC0 (`beqz v0,+0x40C`) straight to 12BFD0, past the node
     * copies, the 0x02/0x04/0x1000 blocks, the velocity zeroing at
     * 12BFB0/12BFB4/12BFB8 and the `ctrl &= ~0x80` at 12BFBC.  Zeroing
     * 0x3C/0x40/0x44 outside that test kills the object: those three floats are
     * the sub-step loop's stop condition, so the loop exits on its first pass
     * and no sub-step (movement, animation, ground follow) ever runs. */
    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x80) != 0) {
        U_COPY54(nodeB, p24);
        U_COPY54(nodeA, p24);
        ctrl = *(u32 *)(obj + 0x0C);
        if ((ctrl & 0x04) != 0) {
            U_COPY54(nodeC, p24);
            U_COPY54(nodeD, p24);
        }
        ctrl = *(u32 *)(obj + 0x0C);
        if ((ctrl & 0x02) != 0)
            U_COPY12(obj + 0x1A0, obj + 0xFC);
        if ((ctrl & 0x1000) == 0) {
            func_00127868(p24, v200);
            *(f32 *)(obj + 0x48) = v200[0];
            *(f32 *)(obj + 0x4C) = v200[1];
            *(f32 *)(obj + 0x50) = v200[2];
        }
        *(f32 *)(obj + 0x3C) = 0.0f;
        *(f32 *)(obj + 0x40) = 0.0f;
        *(f32 *)(obj + 0x44) = 0.0f;
        ctrl = *(u32 *)(obj + 0x0C) & 0xFFFFFF7F;
        *(u32 *)(obj + 0x0C) = ctrl;
    }
    if ((ctrl & 0x14) == 0x14)
        func_00138030(obj);
    U_COPY54(n180, nodeA);

    if ((*(u32 *)(obj + 0x08) & 0x20000000) != 0)
        func_001266B0((u8 *)n180, obj + 0x120);

    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x02) != 0) {
        if (*(s8 *)(obj + 0x1D0) <= 0) {
            *(u8 *)(obj + 0x1D0) = (u8)func_00108648(ctx);
            ctrl = *(u32 *)(obj + 0x0C);
        }
    }

    if ((ctrl & 0x200) != 0) {
        /* ---- constrained path: integrate the stored velocity ------------ */
        v240[0] = step * *(f32 *)(obj + 0x3C);
        v240[1] = step * *(f32 *)(obj + 0x40);
        v240[2] = step * *(f32 *)(obj + 0x44);
        flag30 = (s32)((*(u32 *)(obj + 0x08) >> 29) & 1);
        func_0013B320(obj, v240);
        U_COPY54(n0, nodeA);
        if ((*(u32 *)(obj + 0x08) & 0x000E0000) != 0) {
            func_00138170(obj, (u8 *)n180, step, (u8 *)n120);
            *(u32 *)(obj + 0x08) &= 0xFFF1FFFF;
        }
        if (flag30 != 0) {
            func_00138830(obj);
            *(u32 *)(obj + 0x08) &= 0xDFFFFFFF;
        }
        if ((*(u32 *)(obj + 0x08) & 0x02) != 0) {
            if ((*(u32 *)(obj + 0x0C) & 0x02) != 0) {
                n = (s32)(s8)(*(u8 *)(obj + 0x1D0) - 1);
                *(u8 *)(obj + 0x1D0) = (u8)n;
                if (n == 0) {
                    func_0012B800(obj, ctx, step);
                    if ((u32)func_00108648(ctx) >= 2)
                        U_COPY12(obj + 0x1A0, obj + 0xFC);
                }
            } else {
                func_0012B800(obj, ctx, step);
            }
        }
        func_0013A148(obj, step);
        done = 0;
        func_0013A468(obj, nodeB);
        if (func_00124988(p24, nodeB) == 0) {
            U_COPY54(p24, nodeB);
            done = 1;
            func_0014AD50(ctx);
        }
        if (done == 0 && func_00124988(nodeA, nodeB) != 0)
            goto l_4e8;
        if (func_0010CA50(ctx) != 0) {
            func_0014AF20(ctx);
            /* retail keeps the 4th argument in $s1: it is n120 unless the bit-29
               branch replaces it with n60, and that survives into the call. */
            scratch = (u8 *)n120;
            if (flag30 != 0) {
                func_00121888((u8 *)n60, (u8 *)n0);
                scratch = (u8 *)n60;
            } else if ((*(u32 *)(obj + 0x0C) & 0x800) != 0) {
                func_00121888((u8 *)n120, (u8 *)n180);
                *(u32 *)(obj + 0x0C) &= 0xFFFFF7FF;
            }
            func_00138ED0(obj, ctx, (u8 *)n0, scratch);
        } else {
            if ((func_0010C9F0(ctx) & 0x02) != 0)
                func_0014AF20(ctx);
            else
                func_0014B050(ctx);
        }
    l_4e8:
        func_0013A1C8(obj);
        func_00136970(obj);
        return obj;
    }

    /* ---- full update --------------------------------------------------- */
    U_COPY12(v1F0, obj + 0xA8);
    U_COPY54(n0, nodeA);
    if ((ctrl & 0x04) != 0)
        U_COPY54(nC0, nodeC);
    ctrl = *(u32 *)(obj + 0x0C) & 0xFFFFFFDF & 0xFFFFFFBF;
    *(u32 *)(obj + 0x0C) = ctrl;

    if ((*(u32 *)(obj + 0x08) & 0x2000) != 0) {
        if (!(fabsf(*(f32 *)(obj + 0x48) - 1.0f) < D_00392654 &&
              fabsf(*(f32 *)(obj + 0x4C) - 1.0f) < D_00392654 &&
              fabsf(*(f32 *)(obj + 0x50) - 1.0f) < D_00392654))
            func_00138C28(obj, (u8 *)n180, (u8 *)n120);
    }

    *(f32 *)(obj + 0x54) = dt * *(f32 *)(obj + 0x54);
    *(f32 *)(obj + 0x58) = dt * *(f32 *)(obj + 0x58);
    *(f32 *)(obj + 0x5C) = dt * *(f32 *)(obj + 0x5C);
    func_00121610(obj + 0x54, nodeB, obj + 0x54);
    *(f32 *)(obj + 0x6C) = *(f32 *)(obj + 0x6C) / cnt;
    *(f32 *)(obj + 0x70) = *(f32 *)(obj + 0x70) / cnt;
    *(f32 *)(obj + 0x74) = *(f32 *)(obj + 0x74) / cnt;
    func_0013A4A8(obj, (f32 *)v250);

    /* Retail re-reads obj+0x08 before every single flag test: the helpers
     * called here clear their own request bits, so a cached copy takes the
     * wrong branches. */
    for (i = 0; i < count; i++) {
        if ((*(u32 *)(obj + 0x08) & 0x00000001) != 0)
            func_0013B320(obj, (f32 *)(obj + 0x54));
        if ((*(u32 *)(obj + 0x08) & 0x00700000) != 0)
            func_001385D0(obj, (u8 *)n180, dt, (u8 *)n120);
        if ((*(u32 *)(obj + 0x08) & 0x00000800) != 0) {
            if (*(f32 *)(obj + 0x158) == 0.0f)
                func_001387F0(obj, 1, dt);
        }
        func_00137CB8(obj, (u8 *)n180, dt, (u8 *)n120);
        if ((*(u32 *)(obj + 0x08) & 0x00000004) != 0)
            func_00137C68(obj, dt);
        if ((*(u32 *)(obj + 0x08) & 0x000E0000) != 0)
            func_00138170(obj, (u8 *)n180, dt, (u8 *)n120);
        if ((*(u32 *)(obj + 0x08) & 0x03800000) != 0)
            func_00138390(obj, (u8 *)n180, dt, (u8 *)n120);
        /* Retail runs func_00138840 on BOTH paths: the 0x20000000 branch does
           func_00138830 (node <- obj+0x120) then jumps to the single
           func_00138840 call (rebuild the basis from obj+0x6C), and the
           `flags < 0` branch falls into that same call.  The bit-29 path must
           not skip the basis rebuild.  Both tests read one cached copy, and
           there is only one call site in retail. */
        flg8 = *(u32 *)(obj + 0x08);
        if ((flg8 & 0x20000000) != 0)
            func_00138830(obj);
        if ((flg8 & 0x20000000) != 0 || (s32)flg8 < 0)
            func_00138840(obj);
        if ((*(u32 *)(obj + 0x08) & 0x00000800) != 0) {
            if (*(f32 *)(obj + 0x158) == 1.0f)
                func_001387F0(obj, 0, dt);
        }
        if ((*(u32 *)(obj + 0x08) & 0x00000200) != 0)
            func_00138988(obj, (u8 *)n180, dt, (u8 *)n120);
        func_0013A148(obj, dt);
        if ((*(u32 *)(obj + 0x0C) & 0x02) != 0)
            func_0013B388(obj, dt);
        if ((*(u32 *)(obj + 0x0C) & 0x04) != 0)
            U_COPY54(nodeC, nodeA);
        U_COPY54(nodeA, nodeB);
        if (*(f32 *)(obj + 0x3C) == 0.0f &&
            *(f32 *)(obj + 0x40) == 0.0f &&
            *(f32 *)(obj + 0x44) == 0.0f)
            break;
    }

    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x04) != 0)
        U_COPY54(nodeC, (u8 *)nC0);
    U_COPY54(nodeA, (u8 *)n0);
    func_0013A5D0(obj, (f32 *)v250);
    func_0013A610(obj, step);
    flg8 = *(u32 *)(obj + 0x08);
    if ((flg8 & 0x08) != 0)
        func_00137E10(obj, step);
    res = (s32)(u8)func_00139800(obj);
    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x04) != 0)
        r278 = (*(u32 *)(obj + 0x27C) != 0);
    ctrl &= 0xFFFF7FFF;
    *(u32 *)(obj + 0x0C) = ctrl;

    if ((ctrl & 0x02) != 0) {
        n = (s32)(s8)(*(u8 *)(obj + 0x1D0) - 1);
        *(u8 *)(obj + 0x1D0) = (u8)n;
        if (n != 0) {
            goto l_cfd4;
        }
        if (res == 0 && r278 == 0 && func_00108618(ctx) == 0)
            goto l_cfa0;
        {
            u32 f8b;
            f8b = *(u32 *)(obj + 0x08);
            if ((f8b & 0x10) != 0) {
                func_0012F8D8(obj, ctx, step);
            } else if ((f8b & 0x40) != 0) {
                func_00130888(obj, ctx, step);
                len = F_SQRT(F_DOT3(*(f32 *)(obj + 0x3C), *(f32 *)(obj + 0x3C),
                                    *(f32 *)(obj + 0x40), *(f32 *)(obj + 0x40),
                                    *(f32 *)(obj + 0x44), *(f32 *)(obj + 0x44)));
                if (len < D_00393198) {
                    if (*(u8 *)(obj + 0x11C) == 1)
                        *(u8 *)(obj + 0x11C) = 2;
                    U_COPY12(obj + 0xFC, obj + 0xA8);
                    *(f32 *)(obj + 0x108) = 1.0f;
                    *(f32 *)(obj + 0x3C) = 0.0f;
                    *(f32 *)(obj + 0x40) = 0.0f;
                    *(f32 *)(obj + 0x44) = 0.0f;
                }
            } else if ((f8b & 0x20) != 0) {
                func_00131D78(obj, ctx, step);
            } else if ((ctrl & 0x100) != 0) {
                func_001340F0(obj, ctx, step);
            } else if ((ctrl & 0x100000) != 0) {
                func_001346E8(obj, ctx, step);
            } else if ((f8b & 0x02) != 0) {
                func_0012F6F0(obj, ctx, step);
            }
        }
    l_cfa0:
        if ((u32)func_00108648(ctx) >= 2)
            U_COPY12(obj + 0x1A0, obj + 0xFC);
        goto l_d154;

    l_cfd4:
        flg8 = *(u32 *)(obj + 0x08);
        if ((flg8 & 0x20) != 0)
            func_00133F40(obj, step);
        ctrl = *(u32 *)(obj + 0x0C);
        if ((ctrl & 0x4000) != 0) {
            pd = (u8 *)r274;
            *(u32 *)(pd + 0x04) = *(u32 *)pd;
        }
        goto l_d150;
    } else {
        if (res == 0 && func_00108618(ctx) == 0)
            goto l_d154;
        {
            u32 f8b;
            f8b = *(u32 *)(obj + 0x08);
            if ((f8b & 0x10) != 0) {
                func_0012F8D8(obj, ctx, step);
            } else if ((f8b & 0x40) != 0) {
                func_00130888(obj, ctx, step);
                len = F_SQRT(F_DOT3(*(f32 *)(obj + 0x3C), *(f32 *)(obj + 0x3C),
                                    *(f32 *)(obj + 0x40), *(f32 *)(obj + 0x40),
                                    *(f32 *)(obj + 0x44), *(f32 *)(obj + 0x44)));
                if (len < D_00393198) {
                    if (*(u8 *)(obj + 0x11C) == 1)
                        *(u8 *)(obj + 0x11C) = 2;
                    U_COPY12(obj + 0xFC, obj + 0xA8);
                    *(f32 *)(obj + 0x108) = 1.0f;
                    *(f32 *)(obj + 0x3C) = 0.0f;
                    *(f32 *)(obj + 0x40) = 0.0f;
                    *(f32 *)(obj + 0x44) = 0.0f;
                }
            } else if ((f8b & 0x20) != 0) {
                func_0012B908(obj, ctx, step);
            } else if ((ctrl & 0x100) != 0) {
                func_001340F0(obj, ctx, step);
            } else if ((ctrl & 0x100000) != 0) {
                func_001346E8(obj, ctx, step);
            } else if ((f8b & 0x02) != 0) {
                func_0012B800(obj, ctx, step);
            }
        }
    }

    l_d150:
    l_d154:
    func_00138850(obj, v1F0);
    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x04) != 0)
        r27C = func_00138FA0(obj, ctx);
    func_0013A468(obj, nodeB);

    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x4004) == 0x4004) {
        v1E0[0] = *(f32 *)(nodeB + 0x30) - *(f32 *)(nodeD + 0x30);
        v1E0[1] = *(f32 *)(nodeB + 0x34) - *(f32 *)(nodeD + 0x34);
        v1E0[2] = *(f32 *)(nodeB + 0x38) - *(f32 *)(nodeD + 0x38);
        v1E0[0] = v1E0[0] / step;
        v1E0[1] = v1E0[1] / step;
        v1E0[2] = v1E0[2] / step;
        func_00126510(nodeD, v210);
        v220[3] = 1.0f;
        U_COPY12(&v220[4], v210);
        U_COPY12(v220, v1E0);
        /* retail reloads obj+0x148 here, after func_00126510, rather than reusing the
           pointer saved at entry: the callee may have attached a different node. */
        pd = *(u8 **)(obj + 0x148);
        U_COPY12(pd + 0x114, v220);
        *(f32 *)(pd + 0x120) = v220[3];
        U_COPY12(pd + 0x124, &v220[4]);
    }

    done = 0;
    /* retail: `beqz ret,.publish` then `beqz r27C,.skip`, so the mirror node is
       refreshed whenever func_00124BB0 reports "same" or r27C is set. */
    if (func_00124BB0(p24, nodeB, D_00392658) != 0 && r27C == 0)
        goto l_d39c;
    U_COPY54(p24, nodeB);
    done = 1;
    func_0014AD50(ctx);
    *(u32 *)(obj + 0x0C) |= 0x01000000;
    l_d39c:
    if (done == 0 && func_00124988(nodeA, nodeB) != 0)
        goto l_d46c;
    if (func_0010CA50(ctx) != 0) {
        func_0014AF20(ctx);
        scratch = (u8 *)n120;
        if ((*(u32 *)(obj + 0x08) & 0x20000000) != 0) {
            func_00121888((u8 *)n60, (u8 *)n0);
            scratch = (u8 *)n60;
        } else if ((*(u32 *)(obj + 0x0C) & 0x800) != 0) {
            func_00121888((u8 *)n120, (u8 *)n180);
            *(u32 *)(obj + 0x0C) &= 0xFFFFF7FF;
        }
        func_00138ED0(obj, ctx, (u8 *)n0, scratch);
    } else {
        if ((func_0010C9F0(ctx) & 0x02) != 0)
            func_0014AF20(ctx);
        else
            func_0014B050(ctx);
    }

    l_d46c:
    func_0013A1C8(obj);
    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x4004) == 0x4004) {
        pd = *(u8 **)(obj + 0x148);
        U_COPY12(v240, (u8 *)nodeB + 0x30);
        v220[3] = 0.0f;
        v220[4] = 0.0f;
        v220[5] = 0.0f;
        v220[6] = 0.0f;
        U_COPY12(v220, v240);
        U_COPY12(pd + 0x14C, v220);
        *(f32 *)(pd + 0x158) = v220[3];
        U_COPY12(pd + 0x15C, &v220[4]);
    }
    func_00136970(obj);

    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x4000) != 0) {
        pd = (u8 *)r274;
        *(u32 *)pd = *(u32 *)(pd + 0x04);
    }

    *(u8 *)(obj + 0x144) = (u8)count;
    *(f32 *)(obj + 0x24) = 0.0f;
    *(f32 *)(obj + 0x28) = 0.0f;
    *(f32 *)(obj + 0x2C) = 0.0f;
    *(f32 *)(obj + 0x30) = 0.0f;
    *(f32 *)(obj + 0x34) = 0.0f;
    *(f32 *)(obj + 0x38) = 0.0f;
    *(f32 *)(obj + 0x54) = 0.0f;
    *(f32 *)(obj + 0x58) = 0.0f;
    *(f32 *)(obj + 0x5C) = 0.0f;
    *(f32 *)(obj + 0x6C) = 0.0f;
    *(f32 *)(obj + 0x70) = 0.0f;
    *(f32 *)(obj + 0x74) = 0.0f;

    flg8 = *(u32 *)(obj + 0x08);
    flg8 &= 0xFFFFBFFF;
    flg8 &= 0xFFFF7FFF;
    flg8 &= 0xFFFEFFFF;
    flg8 &= 0xFFFDFFFF;
    flg8 &= 0xFFFBFFFF;
    flg8 &= 0xFFF7FFFF;
    flg8 &= 0xFFEFFFFF;
    flg8 &= 0xFFDFFFFF;
    flg8 &= 0xFFBFFFFF;
    flg8 &= 0xFF7FFFFF;
    flg8 &= 0xFEFFFFFF;
    flg8 &= 0xFDFFFFFF;
    flg8 &= 0xDFFFFFFF;
    flg8 &= 0x7FFFFFFF;
    *(u32 *)(obj + 0x08) = flg8;

    ctrl = *(u32 *)(obj + 0x0C);
    *(u32 *)(obj + 0x0C) = ctrl & 0xFFFFFFEF & 0xFFFFEFFF;

    /* retail clears both accumulators unconditionally (one store sits in a
       branch delay slot); only obj+0x16C is behind the 0x0C & 2 test. */
    D_003931A0 = 0.0f;
    D_0039319C = 0.0f;
    if ((ctrl & 0x02) != 0) {
        if (U_D30ED0 != 0)
            *(f32 *)(obj + 0x16C) = func_00139828(obj, (void *)1);
        else
            *(f32 *)(obj + 0x16C) = 0.0f;
    }

    if ((U_D30ED0 & 0x01) != 0 && func_00139828(obj, (void *)1) != 0.0f) {
        /* the set path re-reads 0x0C after the call, so keep whatever it set */
        *(u32 *)(obj + 0x0C) = *(u32 *)(obj + 0x0C) | 0x00020000;
    } else {
        /* retail re-reads 0x0C in the branch-likely delay slot here, so the getter's
           effect on +0x0C survives into the clear. */
        *(u32 *)(obj + 0x0C) = *(u32 *)(obj + 0x0C) & 0xFFFDFFFF;
    }

    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x02) != 0) {
        *(f32 *)(obj + 0x17C) = 0.0f;
        *(f32 *)(obj + 0x180) = 0.0f;
        *(f32 *)(obj + 0x184) = 0.0f;
        *(f32 *)(obj + 0x188) = 0.0f;
        *(f32 *)(obj + 0x18C) = 0.0f;
        *(f32 *)(obj + 0x190) = 0.0f;
    }
    return obj;
}

#undef U_COPY54
#undef U_COPY12

INCLUDE_ASM(const s32, "core/main", func_0012D750);

INCLUDE_ASM(const s32, "core/main", func_0012D810);

void func_0012D888(s32 a1, s32 a2)
{



    u8 v3[96];

    if ((*(s32 *)(a1 + 0xC) & 0x80) == 0) {
        func_00124738((s32)v3, 5, a2, a2 + 0xC, a2 + 0x18, 0);
        func_001224F8(a1 + 0xCC, a1 + 0xCC, (s32)v3);
        if ((*(s32 *)(a1 + 0xC) & 4) != 0)
            func_00138030(a1);
    }
}

INCLUDE_ASM(const s32, "core/main", func_0012D900);

INCLUDE_ASM(const s32, "core/main", func_0012D988);

INCLUDE_ASM(const s32, "core/main", func_0012DC18);

INCLUDE_ASM(const s32, "core/main", func_0012DE80);

void func_0012E110(u8 *arg0, f32 arg1)
{
    u8 *entry = *(u8 **)(arg0 + 0x27C);

    if (entry != 0)
    {
        u8 *field = *(u8 **)(entry + 0x24);
        *(f32 *)(arg0 + 0x1E4) = arg1 - *(f32 *)(field + 0x38);
    }
    else
    {
        *(f32 *)(arg0 + 0x1E4) = arg1;
    }
}

INCLUDE_ASM(const s32, "core/main", func_0012E138);

/* ------------------------------------------------------------------------- */
/* func_0012E260 - contact-constrained movement against a caller-given list   */
/* (0xF20 = 3872 bytes)                                                      */
/* ------------------------------------------------------------------------- */
/* Called as (obj, ctx, count, contacts, step).  Same machinery as
 * func_0012F8D8 / func_00130888 (contact records of 0x68 bytes, "type" word at
 * +0x2E whose 0x63 value means "already handled", func_00139708 probe,
 * func_00135CF8 re-query, func_00136C08 offset fold, func_0013A148 commit) but
 * the contact list is the 4th argument instead of D_003931B0, and the motion
 * resolved here is the pair of node translations the caller left behind:
 *   delta = nodeB(translation obj+0xFC) - nodeA(translation obj+0xA8).
 *
 * Behaviour, in order:
 *   1. D_003931A4++ (the contact-pass counter func_0012F8D8 limits to 4).
 *   2. obj+0x0C bit 0x40 clear: find the first usable contact (skipping type
 *      0x63 and the "both owners carry 0x4000" pair contacts) that
 *      func_00139708 accepts -> probe/probeIdx.  Bit 0x40 set: instead mark
 *      every contact func_00139708 accepts as type 0x63.
 *   3. probe && obj+0x0C bit 0x100000 && !(D_00330ED0 bit 6): func_00131D78,
 *      clear D_00330ED0 bit 6 and leave without touching the nodes.
 *   4. first type!=0x63 contact -> first/firstIdx; across all live contacts
 *      note the first pair contact (firstPair) and whether all of them are
 *      type 7 or 8 (all78).  No live contact at all -> plain return.
 *   5. firstIdx = min(firstIdx, probeIdx), then max(firstIdx, firstPair), then
 *      func_00135CF8(obj, ctx, count, contacts, firstIdx).
 *   6. Contact normal: if all78 && !(obj+0x0C bit 0x40) the normal is the
 *      normalised sum of every live contact normal (falling back to the first
 *      contact's own normal when the sum is zero), otherwise it is simply the
 *      first contact's normal at +0x04.
 *   7. Motion direction: with s28 = func_00139828(obj, first) and
 *      s58 = func_00139858(obj, first),
 *         q   = -(delta . n)
 *         t1  = delta + q*n            (delta's tangential part)
 *         t2  = t1 + q*n               (its mirror image)
 *         dir = s28*t1 + s58*t2
 *      and udelta = delta/|delta| (left at zero when |delta| < 0.001).
 *   8. obj+0x08 bit 0x100 && the contact normal is horizontal (|n.z| <=
 *      obj+0x18, the "wall" case) && dir.z != 0:  t = delta.z/dir.z;
 *      -1 < t <= 1 scales dir by t; t > 1 rebuilds dir inside the
 *      {axisB x n, n x (axisB x n), n} frame so that its z component lands on
 *      delta.z.  Everything else is left alone.
 *   9. pos = nodeB translation + func_00136C08 offset (the offset's z is
 *      dropped when obj+0x0C has bit 0x400000), except for the "type 7/8 with
 *      a negative distance and facing the contact" special case, which uses
 *      node A's translation instead.  With bit 0x100000 a pos.z above
 *      D_003931B4-1.2 is clamped back to node A's z and floorHit is set.
 *  10. dir2 = (1 - contact->distance) * dir (zero when the distance is
 *      negative), clipped to the contact's +0x54 length; floorHit flattens
 *      dir2 onto the XY plane.
 *  11. node B translation = pos + dir2 (or pos alone depending on the
 *      separation/floor/probe tests below), tag 1 -> 2, +0x3C = 1.0;
 *      node A translation = nodeB - dir, tag 1 -> 2, +0x3C = 1.0;
 *      with obj+0x0C bit 0x04 node C (obj+0x280) translation = node A's,
 *      tag 1 -> 2, +0x3C = 1.0.  Then func_0013A148(obj, step).
 *
 * The 0x100000 early-out returns "-65" in Hex-Rays only because the caller's
 * $v0 holds func_00131D78's result there; no caller reads the value. */

s32 func_00139708(u8 *, f32 *);
f32 func_00139858(u8 *, void *);
s32 func_00135CF8(u8 *, u8 *, s32, u8 *, s32);
s32 func_00136C08(f32 *, u8 *, s32, u8 *, s32);

extern f32 D_00392668;          /* 0.001  - "flat" direction epsilon         */
extern f32 D_0039266C;          /* 0.001  - "no delta" epsilon               */
extern f32 D_00392670;          /* -0.42  - dot limit for entering a contact */
extern f32 D_00392674;          /* 1.2    - ceiling margin                   */
extern f32 D_00392678;          /* 0.001  - horizontal epsilon               */
extern f32 D_0039267C;          /* 0.001  - "clear of the plane" epsilon     */
extern f32 D_003931B4;          /* movement ceiling (z)                      */

/* A node written by this handler is either left alone or becomes a
 * translation-only node (tag 1 -> 2) with a 1.0 homogeneous element. */
#define ME_TAG2(p)                                                            \
    do {                                                                      \
        if (*(u8 *)((p) + 0x50) == 1)                                         \
            *(u8 *)((p) + 0x50) = 2;                                          \
    } while (0)
#define ME_COPY12(d, s) (*(UBytes12 *)(d) = *(UBytes12 *)(s))

void func_0012E260(u8 *obj, u8 *ctx, s32 count, u8 *contacts, f32 step)
{
    u8 *nodeA = obj + 0x78;
    u8 *nodeB = obj + 0xCC;
    u8 *nodeC = obj + 0x280;            /* only used with obj+0x0C bit 0x04 */
    f32 *pa = (f32 *)(obj + 0xA8);      /* node A translation               */
    f32 *pb = (f32 *)(obj + 0xFC);      /* node B translation               */
    f32 acc[3];                         /* sum of live contact normals      */
    f32 off[3];                         /* func_00136C08 offset fold        */
    f32 axisB[3];
    f32 delta[3];
    f32 udelta[3];
    f32 dir[3];
    f32 dir2[3];
    f32 t1[3];
    f32 t2[3];
    f32 pos[3];
    f32 npos[3];
    f32 tmp[3];
    f32 *n;
    u8 *c;
    u8 *first = 0;                      /* first contact that is not 0x63   */
    u8 *probe = 0;                      /* first contact func_00139708 likes */
    s32 firstIdx = 0;
    s32 probeIdx = 0;
    s32 firstPair = 0;                  /* first "pair" contact index       */
    s32 all78 = 1;                      /* all live contacts are type 7/8   */
    s32 wall;                           /* contact normal is horizontal-ish */
    s32 floorFlag;                      /* contact normal points up         */
    s32 floorHit = 0;                   /* pos.z was clamped to node A's    */
    s32 writeNpos;
    s32 i;
    f32 s28;
    f32 s58;
    f32 dot;
    f32 q;
    f32 len;
    f32 d;

    D_003931A4 = D_003931A4 + 1;

    /* The zero fills are spelled out: an aggregate initialiser is lowered to
     * memset(), which this module does not link. */
    acc[0] = 0.0f;
    acc[1] = 0.0f;
    acc[2] = 0.0f;
    off[0] = 0.0f;
    off[1] = 0.0f;
    off[2] = 0.0f;
    udelta[0] = 0.0f;
    udelta[1] = 0.0f;
    udelta[2] = 0.0f;
    axisB[0] = 0.0f;
    axisB[1] = 0.0f;
    axisB[2] = 1.0f;

    /* 1./2. contact pre-pass */
    if ((*(u32 *)(obj + 0x0C) & 0x40) == 0)
    {
        for (i = 0; i < count && probe == 0; i++)
        {
            c = contacts + i * 0x68;
            if (*(s16 *)(c + 0x2E) == 0x63)
                continue;
            if ((*(u16 *)(*(u32 *)(c + 0x20) + 0x06) & 0x4000) != 0 &&
                (*(u16 *)(*(u32 *)(c + 0x1C) + 0x06) & 0x4000) != 0)
                continue;
            if ((func_00139708(obj, (f32 *)(c + 0x04)) & 0xFF) == 0)
                continue;
            probe = c;
            probeIdx = i;
        }
    }
    else
    {
        for (i = 0; i < count; i++)
        {
            c = contacts + i * 0x68;
            if (*(s16 *)(c + 0x2E) == 0x63)
                continue;
            if ((func_00139708(obj, (f32 *)(c + 0x04)) & 0xFF) == 0)
                continue;
            *(s16 *)(c + 0x2E) = 0x63;
        }
    }

    /* 3. hard contact while the caller asked to give up: hand over to
     * func_00131D78 once per D_00330ED0 bit-6 episode. */
    if (probe != 0 && (*(u32 *)(obj + 0x0C) & 0x100000) != 0)
    {
        if ((U_D30ED0 & 0x40) == 0)
        {
            func_00131D78(obj, ctx, step);
            U_D30ED0 = U_D30ED0 & ~0x40u;
            return;
        }
    }

    /* 4. first live contact, first pair contact, "all of them are 7/8" */
    for (i = 0; i < count && first == 0; i++)
    {
        c = contacts + i * 0x68;
        if (*(s16 *)(c + 0x2E) == 0x63)
            continue;
        first = c;
        firstIdx = i;
    }

    for (i = 0; i < count; i++)
    {
        s32 pair = 0;
        s32 idx;

        c = contacts + i * 0x68;
        if (*(s16 *)(c + 0x2E) == 0x63)
            continue;
        if ((*(u16 *)(*(u32 *)(c + 0x20) + 0x06) & 0x4000) != 0)
            pair = (*(u16 *)(*(u32 *)(c + 0x1C) + 0x06) & 0x4000) != 0;
        idx = (firstPair != 0) ? firstPair : i;
        if (pair)
            firstPair = idx;
        if ((u16)(*(u16 *)(c + 0x2E) - 7) >= 2)
            all78 = 0;
    }

    if (first == 0)
        return;

    /* 5. agree on the contact the machinery works from */
    if (probe != 0)
    {
        if (firstIdx < probeIdx)
            probeIdx = firstIdx;
        firstIdx = probeIdx;
    }
    if (firstIdx < firstPair)
        firstIdx = firstPair;
    func_00135CF8(obj, ctx, count, contacts, firstIdx);

    /* 6. the plane the motion is resolved against */
    if (all78 != 0 && (*(u32 *)(obj + 0x0C) & 0x40) == 0)
    {
        for (i = 0; i < count; i++)
        {
            c = contacts + i * 0x68;
            if (*(s16 *)(c + 0x2E) == 0x63)
                continue;
            acc[0] = acc[0] + *(f32 *)(c + 0x04);
            acc[1] = acc[1] + *(f32 *)(c + 0x08);
            acc[2] = acc[2] + *(f32 *)(c + 0x0C);
        }
        if (acc[0] != 0.0f || acc[1] != 0.0f || acc[2] != 0.0f)
        {
            len = F_SQRT(F_DOT3(acc[0], acc[0], acc[1], acc[1],
                                acc[2], acc[2]));
            if (len != 0.0f)
            {
                f32 s = 1.0f / len;
                acc[0] = s * acc[0];
                acc[1] = s * acc[1];
                acc[2] = s * acc[2];
            }
            n = acc;
        }
        else
        {
            n = (f32 *)(first + 0x04);
        }
    }
    else
    {
        n = (f32 *)(first + 0x04);
    }

    /* the two per-contact scalars func_00139828/58 hand back */
    s28 = func_00139828(obj, first);
    s58 = func_00139858(obj, first);

    /* 7. the tangential motion the caller left between the two nodes */
    for (i = 0; i < 3; i++)
        delta[i] = pb[i] - pa[i];

    len = F_SQRT(F_DOT3(delta[0], delta[0], delta[1], delta[1],
                        delta[2], delta[2]));
    dot = F_DOT3(delta[0], n[0], delta[1], n[1], delta[2], n[2]);
    q = -dot;
    for (i = 0; i < 3; i++)
    {
        t1[i] = delta[i] + q * n[i];
        t2[i] = t1[i] + q * n[i];
        dir[i] = s28 * t1[i] + s58 * t2[i];
    }

    /* wall/floor classification of the contact normal against obj+0x18 */
    {
        f32 lim = *(f32 *)(obj + 0x18);
        f32 nz = n[2];

        wall = ((-lim <= nz) && (nz <= lim));
        floorFlag = (lim <= nz);
    }

    if (!(fabsf(delta[0]) < D_0039266C &&
          fabsf(delta[1]) < D_0039266C &&
          fabsf(delta[2]) < D_0039266C))
    {
        f32 s = 1.0f / len;
        udelta[0] = s * delta[0];
        udelta[1] = s * delta[1];
        udelta[2] = s * delta[2];
    }

    /* 8. match the motion's z span to delta.z */
    if ((*(u32 *)(obj + 0x08) & 0x100) != 0 && wall != 0 && dir[2] != 0.0f)
    {
        f32 t = delta[2] / dir[2];

        if (-1.0f < t)
        {
            if (t <= 1.0f)
            {
                dir[0] = t * dir[0];
                dir[1] = t * dir[1];
                dir[2] = t * dir[2];
            }
            else
            {
                /* {axisB x n, n x (axisB x n), n} frame; the two crosses are
                 * spelled out the way retail computes them (a plain array
                 * temporary would fold the same way, so keep the operands). */
                f32 nn[3];
                f32 dx[3];
                f32 sec[3];
                f32 a;
                f32 b;
                f32 k;

                nn[0] = n[0];
                nn[1] = n[1];
                nn[2] = n[2];
                dx[0] = axisB[1] * nn[2] - axisB[2] * nn[1];
                dx[1] = axisB[2] * nn[0] - axisB[0] * nn[2];
                dx[2] = axisB[0] * nn[1] - axisB[1] * nn[0];
                sec[0] = nn[1] * dx[2] - nn[2] * dx[1];
                sec[1] = nn[2] * dx[0] - nn[0] * dx[2];
                sec[2] = nn[0] * dx[1] - nn[1] * dx[0];
                a = F_DOT3(dir[0], dx[0], dir[1], dx[1], dir[2], dx[2]);
                b = F_DOT3(dir[0], nn[0], dir[1], nn[1], dir[2], nn[2]);
                k = delta[2] / sec[2];
                dir[0] = a * dx[0] + k * sec[0] + b * nn[0];
                dir[1] = a * dx[1] + k * sec[1] + b * nn[1];
                dir[2] = a * dx[2] + k * sec[2] + b * nn[2];
            }
        }
    }

    /* 9. where the step ends up */
    d = *(f32 *)(first + 0x00);         /* contact distance */
    func_00136C08(off, obj, count, contacts, firstIdx);

    if ((*(u32 *)(obj + 0x0C) & 0x400000) != 0)
        off[2] = 0.0f;

    /* the "distance is negative" test is re-read after func_00136C08 (retail
     * keeps only the 1-distance factor from before the call) */
    if ((*(u32 *)(obj + 0x0C) & 0x100000) != 0 &&
        (u16)(*(u16 *)(first + 0x2E) - 7) < 2 &&
        *(f32 *)(first + 0x00) < 0.0f &&
        F_DOT3(n[0], udelta[0], n[1], udelta[1], n[2], udelta[2]) <
            D_00392670)
    {
        pos[0] = pa[0];
        pos[1] = pa[1];
        pos[2] = pa[2];
    }
    else
    {
        pos[0] = pb[0] + off[0];
        pos[1] = pb[1] + off[1];
        pos[2] = pb[2] + off[2];
    }

    if ((*(u32 *)(obj + 0x0C) & 0x100000) != 0 &&
        (D_003931B4 - D_00392674) < pos[2])
    {
        pos[2] = pa[2];
        floorHit = 1;
    }

    /* 10. the actual displacement */
    {
        f32 s = (0.0f <= d) ? (1.0f - d) : 0.0f;

        dir2[0] = s * dir[0];
        dir2[1] = s * dir[1];
        dir2[2] = s * dir[2];
    }

    len = F_SQRT(F_DOT3(dir2[0], dir2[0], dir2[1], dir2[1],
                        dir2[2], dir2[2]));
    {
        f32 m = *(f32 *)(first + 0x54);

        if (m < len)
        {
            f32 s = m / len;
            dir2[0] = s * dir2[0];
            dir2[1] = s * dir2[1];
            dir2[2] = s * dir2[2];
        }
    }

    if (floorHit != 0)
    {
        /* Flatten the step.  Retail then re-normalises the (now horizontal)
         * vector and rescales it by its own horizontal length, which is a
         * no-op apart from rounding; the zeroed z is what matters. */
        f32 h;

        dir2[2] = 0.0f;
        h = F_SQRT(F_DOT3(dir2[0], dir2[0], dir2[1], dir2[1],
                         dir2[2], dir2[2]));
        if (!(fabsf(dir2[0]) < D_00392678 &&
              fabsf(dir2[1]) < D_00392678 &&
              fabsf(dir2[2]) < D_00392678))
        {
            f32 l = F_SQRT(F_DOT3(dir2[0], dir2[0], dir2[1], dir2[1],
                                  dir2[2], dir2[2]));

            if (l != 0.0f)
            {
                f32 s = 1.0f / l;
                dir2[0] = s * dir2[0];
                dir2[1] = s * dir2[1];
                dir2[2] = s * dir2[2];
            }
            dir2[0] = h * dir2[0];
            dir2[1] = h * dir2[1];
            dir2[2] = h * dir2[2];
        }
    }

    npos[0] = pos[0] + dir2[0];
    npos[1] = pos[1] + dir2[1];
    npos[2] = pos[2] + dir2[2];
    for (i = 0; i < 3; i++)
        tmp[i] = npos[i] - pa[i];

    /* 11. node B: only leave the contact plane early when the motion really
     * separates from it and the contact is a floor we are not approaching. */
    dot = F_DOT3(n[0], tmp[0], n[1], tmp[1], n[2], tmp[2]);
    if (D_0039267C < dot && floorFlag != 0 && 0.0f < s58)
    {
        writeNpos = (*(u32 *)(obj + 0x0C) & 0x40) != 0;
    }
    else if (probe == 0)
    {
        writeNpos = 1;
    }
    else if ((func_00139708(obj, (f32 *)(first + 0x04)) & 0xFF) != 0)
    {
        writeNpos = 1;
    }
    else
    {
        writeNpos = (*(u32 *)(obj + 0x0C) & 0x40) != 0;
    }

    ME_TAG2(nodeB);
    ME_COPY12(nodeB + 0x30, writeNpos ? npos : pos);
    *(f32 *)(nodeB + 0x3C) = 1.0f;

    /* node A becomes the start of the next step */
    for (i = 0; i < 3; i++)
        tmp[i] = pb[i] - dir[i];
    ME_TAG2(nodeA);
    ME_COPY12(nodeA + 0x30, tmp);
    *(f32 *)(nodeA + 0x3C) = 1.0f;

    if ((*(u32 *)(obj + 0x0C) & 0x04) != 0)
    {
        ME_TAG2(nodeC);
        ME_COPY12(nodeC + 0x30, pa);
        *(f32 *)(nodeC + 0x3C) = 1.0f;
    }

    func_0013A148(obj, step);
}

INCLUDE_ASM(const s32, "core/main", func_0012F180);

INCLUDE_ASM(const s32, "core/main", func_0012F518);

INCLUDE_ASM(const s32, "core/main", func_0012F6F0);

/* ------------------------------------------------------------------------- */
/* func_0012F8D8 - contact-constrained movement step, obj+0x08 bit 0x10 mode */
/* (0xFB0 = 4016 bytes)                                                      */
/* ------------------------------------------------------------------------- */
/* Called by func_0012BA18 as (obj, ctx, step) when obj+0x08 has bit 0x10 set,
 * i.e. this is the "mode 0x10" sibling of func_00130888 (bit 0x40).  It uses
 * exactly the same contact machinery (func_00135530 fills D_003931B0 from the
 * two probe nodes, func_00135CF8 re-queries, func_00136C08 folds the usable
 * contacts' offsets, func_0012F180 snaps the result onto a plane) but the
 * movement it resolves is different: a step along the *path* direction that
 * the record attached to obj+0x148 provides, and it sub-steps by recursing
 * until the correction comes out zero.
 *
 * Confirmed argument/return use: 2 pointer arguments + 1 float, no return
 * value read by any caller (the last call leaves whatever the callee left in
 * $2).  The probe nodes handed to func_00135530 are two 0x54-byte copies of
 * node B (obj+0xCC): one at the substep start, one at its end.
 */

/* Callees (declared again before func_00130888 below; they must be declared
 * here too because that block comes after this function in the file).  The
 * float parameter of func_0012F180 fixes the register class of arg 7. */
s32 func_00135530(s32 *, u8 *, u8 *, u8 *, u8 *, u8 *, s32);
void func_0012F180(f32 *, u8 *, f32 *, f32 *, f32 *, s32, f32);
s32 func_00136C08(f32 *, u8 *, s32, u8 *, s32);
s32 func_00135CF8(u8 *, u8 *, s32, u8 *, s32);
s32 func_00139760(u8 *, u8 *);
s32 func_0013AB00(s32, u8 *, u8 *, f32 *);

/* Contact-list pointer, the movement-sequence pass counter (already declared
 * above), and the thresholds this handler reads from the literal pool. */
extern u8 *D_003931B0;
extern f32 D_0039269C;          /* 0.45   - minimum substep length          */
extern f32 D_003926A0;          /* 0.707  - contact is "not facing" above   */
extern f32 D_003926A4;          /* -0.01  - contact must not be behind us   */
extern f32 D_003926A8;          /* 0.001  - points coincide epsilon         */
extern f32 D_003926AC;          /* 0.001  - correction-is-zero epsilon (A)  */
extern f32 D_003926B0;          /* 0.001  - correction-is-zero epsilon (B)  */

#define MV_COPY54(d, s) (*(UBytes54 *)(d) = *(UBytes54 *)(s))
#define MV_COPY12(d, s) (*(UBytes12 *)(d) = *(UBytes12 *)(s))
/* A node written by this handler is either left alone or becomes a
 * translation-only node (tag 1 -> 2) with a 1.0 homogeneous element. */
#define MV_TAG2(p)                                                         \
    do {                                                                   \
        if (*(u8 *)((p) + 0x50) == 1)                                      \
            *(u8 *)((p) + 0x50) = 2;                                       \
    } while (0)

void func_0012F8D8(u8 *obj, u8 *ctx, f32 step)
{
    u8  probeA[0x54];           /* sp+0xD4  substep start (node copy)      */
    u8  probeB[0x54];           /* sp+0x80  substep end   (node copy)      */
    f32 stepv[3];               /* sp+0x00  len * dir                      */
    f32 probe[3];               /* sp+0x10  node A translation + tangent   */
    f32 par[3];                 /* sp+0x20  parallel part (never read)      */
    f32 tang[3];                /* sp+0x30  tangential part of the delta    */
    f32 delta[3];               /* sp+0x40  node B translation - node A     */
    f32 out[3];                 /* sp+0x50  func_0012F180 correction        */
    f32 defdir[3];              /* sp+0x60  (0,0,1) used when obj+0x148 == 0 */
    f32 zeropt[3];              /*          stand-in for the address-0 read  */
    f32 off[3];                 /* sp+0x70  func_00136C08 folded offset     */
    f32 tgt[3];                 /* sp+0x130 target translation              */
    s32 count;                  /* sp+0x140 contact count                   */
    s32 hit;                    /* sp+0x148 coincident-point counter        */
    s32 stopFlag;               /* sp+0x14C first hit was not type 7/8      */
    u8 *nodeA = obj + 0x78;
    u8 *nodeB = obj + 0xCC;
    u8 *attach;
    f32 *dir;                   /* path direction (attach+0xD4 / (0,0,1))  */
    f32 *point;                 /* reference point (attach+0xE0 / hit+0x10) */
    f32 *nrm;                   /* dir, or the first hit's contact normal   */
    u8 *firstHit;
    u8 *best;
    u8 *c;
    f32 d;
    f32 len;
    u16 f6w;
    u32 ctrl;
    u8 *parent;
    s32 scanEnd;
    s32 i;

    /* ---- two probe nodes, both seeded from node B ------------------------- */
    MV_COPY54(probeA, nodeB);

    delta[0] = *(f32 *)(nodeB + 0x30) - *(f32 *)(nodeA + 0x30);
    delta[1] = *(f32 *)(nodeB + 0x34) - *(f32 *)(nodeA + 0x34);
    delta[2] = *(f32 *)(nodeB + 0x38) - *(f32 *)(nodeA + 0x38);

    MV_COPY54(probeB, nodeB);

    /* ---- path direction taken from the attached record --------------------
     * Retail tests `attach + 0xD0 == 0` (a pointer-biased null test, only true
     * for attach == 0xFFFFFF30) and otherwise reads attach+0xD4 as the
     * direction and attach+0xE0 as the reference point; the (0,0,1) fallback
     * vector is built unconditionally in the prologue.  The fallback reference
     * point is a zero vector here: retail would read low RAM (zeros) at
     * address 0 in that never-taken case. */
    defdir[0] = 0.0f;
    defdir[1] = 0.0f;
    defdir[2] = 1.0f;
    zeropt[0] = 0.0f;
    zeropt[1] = 0.0f;
    zeropt[2] = 0.0f;

    attach = *(u8 **)(obj + 0x148);
    if ((u32)attach + 0xD0 == 0)
    {
        dir = defdir;
        point = zeropt;
    }
    else
    {
        dir = (f32 *)(attach + 0xD4);
        point = (f32 *)(attach + 0xE0);
    }

    /* ---- pass budget: the fourth pass of one movement sequence gives up --- */
    D_003931A4 = D_003931A4 + 1;
    if ((s32)D_003931A4 >= 4)
    {
        MV_TAG2(nodeB);
        MV_COPY12(nodeB + 0x30, nodeA + 0x30);
        *(f32 *)(obj + 0x108) = 1.0f;
        *(f32 *)(obj + 0x3C) = 0.0f;
        *(f32 *)(obj + 0x40) = 0.0f;
        *(f32 *)(obj + 0x44) = 0.0f;
        return;
    }

    /* ---- substep length: speed * distance, floored at 0.45 ---------------- */
    len = *(f32 *)(obj + 0x14) * F_SQRT(F_DOT3(delta[0], delta[0],
                                               delta[1], delta[1],
                                               delta[2], delta[2]));
    if (len < D_0039269C)
        len = D_0039269C;

    /* ---- split the delta into the part along the path and the rest -------- */
    d = F_DOT3(delta[0], dir[0], delta[1], dir[1], delta[2], dir[2]);
    par[0] = d * dir[0];
    par[1] = d * dir[1];
    par[2] = d * dir[2];
    tang[0] = delta[0] - par[0];
    tang[1] = delta[1] - par[1];
    tang[2] = delta[2] - par[2];

    stepv[0] = len * dir[0];
    stepv[1] = len * dir[1];
    stepv[2] = len * dir[2];

    /* ---- substep end probe: node A translation + tangent + step ----------- */
    probe[0] = *(f32 *)(nodeA + 0x30) + tang[0] + stepv[0];
    probe[1] = *(f32 *)(nodeA + 0x34) + tang[1] + stepv[1];
    probe[2] = *(f32 *)(nodeA + 0x38) + tang[2] + stepv[2];
    MV_TAG2(probeB);
    MV_COPY12(probeB + 0x30, probe);
    *(f32 *)(probeB + 0x3C) = 1.0f;

    /* ---- substep start probe: the same without the step ------------------- */
    probe[0] = *(f32 *)(nodeA + 0x30) + tang[0];
    probe[1] = *(f32 *)(nodeA + 0x34) + tang[1];
    probe[2] = *(f32 *)(nodeA + 0x38) + tang[2];
    MV_TAG2(probeA);
    MV_COPY12(probeA + 0x30, probe);
    *(f32 *)(probeA + 0x3C) = 1.0f;

    firstHit = 0;
    best = 0;
    stopFlag = 0;
    hit = 0;
    scanEnd = 0;
    count = 0;

    /* ---- contact query over the substep segment --------------------------- */
    if (func_00135530(&count, D_003931B0, obj, ctx, probeB, probeA, 1) == 0)
        goto l_reset;                       /* no contacts at all          */

    /* Drop the contacts that do not face the movement. */
    func_0013AB00(count, D_003931B0, obj, delta);

    /* ---- scan 1: first ceiling-like contact that belongs to this actor ----
     * "Belongs to us" is bit 0x20 of the scene flags with 0x8000 clear; the
     * scan stops at the first such contact whose type is not 7 or 8. */
    for (i = 0; i < count; i++)
    {
        c = D_003931B0 + i * 0x68;
        scanEnd = i + 1;
        if (*(s16 *)(c + 0x2E) == 0x63)
            continue;
        f6w = *(u16 *)(*(u8 **)(c + 0x20) + 0x06);
        if ((f6w & 0x20) == 0)
            continue;
        if ((f6w & 0x8000) != 0)
            continue;
        if ((func_00139760(obj, c) & 0xFF) == 0)
            continue;
        if (firstHit == 0)
            firstHit = c;
        if (*(u16 *)(c + 0x2E) != 7 && *(u16 *)(c + 0x2E) != 8)
            stopFlag = 1;
        if (stopFlag != 0)
            break;
    }

    /* From here on the first hit supplies both the reference point and the
     * direction every later dot product uses. */
    if (firstHit != 0)
    {
        nrm = (f32 *)(firstHit + 0x04);
        point = (f32 *)(firstHit + 0x10);
    }
    else
    {
        nrm = dir;
    }

    /* ---- scan 2: first contact ahead of us that is not wall-parallel ------ */
    for (i = 0; i < count; i++)
    {
        c = D_003931B0 + i * 0x68;
        if (*(s16 *)(c + 0x2E) == 0x63)
            continue;
        f6w = *(u16 *)(*(u8 **)(c + 0x20) + 0x06);
        if ((f6w & 0x20) != 0 && (f6w & 0x8000) == 0)
            continue;
        d = F_DOT3(*(f32 *)(c + 0x04), nrm[0], *(f32 *)(c + 0x08), nrm[1],
                   *(f32 *)(c + 0x0C), nrm[2]);
        if (!(d < D_003926A0))
            continue;
        off[0] = *(f32 *)(c + 0x10) - point[0];
        off[1] = *(f32 *)(c + 0x14) - point[1];
        off[2] = *(f32 *)(c + 0x18) - point[2];
        d = F_DOT3(off[0], nrm[0], off[1], nrm[1], off[2], nrm[2]);
        if (d > D_003926A4)
        {
            best = c;
            break;
        }
    }
    if (best != 0 && scanEnd < i + 1)
        scanEnd = i + 1;

    func_00135CF8(obj, ctx, count, D_003931B0, scanEnd - 1);

    /* ======================= no ceiling-like contact ====================== */
    if (firstHit == 0)
    {
        if (best == 0)
            goto l_reset;

        if (*(f32 *)best < 0.0f && (s32)D_003931A4 >= 3)
        {
            /* Far enough into the sequence: fold the usable contacts into an
             * offset and place the substep end there. */
            func_00136C08(off, obj, count, D_003931B0, scanEnd);
            MV_TAG2(probeB);
            tgt[0] = *(f32 *)(nodeA + 0x30) + off[0];
            tgt[1] = *(f32 *)(nodeA + 0x34) + off[1];
            tgt[2] = *(f32 *)(nodeA + 0x38) + off[2];
            MV_COPY12(probeB + 0x30, tgt);
            *(f32 *)(obj + 0x108) = 1.0f;

            delta[0] = delta[0] - (tgt[0] - *(f32 *)(nodeA + 0x30));
            delta[1] = delta[1] - (tgt[1] - *(f32 *)(nodeA + 0x34));
            delta[2] = delta[2] - (tgt[2] - *(f32 *)(nodeA + 0x38));

            MV_TAG2(probeA);
            MV_COPY12(nodeA + 0x30, nodeB + 0x30);
            *(f32 *)(nodeA + 0x3C) = 1.0f;
        }
        else
        {
            MV_TAG2(probeB);
            MV_COPY12(probeB + 0x30, nodeA + 0x30);
            *(f32 *)(obj + 0x108) = 1.0f;
        }

        func_0012F180(out, obj, delta, nrm, (f32 *)(best + 0x04), 0, step);

        if (fabsf(out[0]) < D_003926B0 && fabsf(out[1]) < D_003926B0 &&
            fabsf(out[2]) < D_003926B0)
        {
            MV_TAG2(probeB);
            MV_COPY12(probeB + 0x30, nodeA + 0x30);
            *(f32 *)(obj + 0x108) = 1.0f;
        }
        else
        {
            /* Correction left over: run another substep. */
            func_0012F8D8(obj, ctx, step);
        }
        func_0013A148(obj, step);
        return;
    }

    /* ====================== a ceiling-like contact exists ================== */
    /* Landing test: a type 7/8 contact, nothing ahead and nothing of ours
     * beyond it, exactly one contact sharing its point, and bit 2 of the
     * contact's +0x28 clear -> settle on node A and stop the movement. */
    if ((*(u16 *)(firstHit + 0x2E) == 7 || *(u16 *)(firstHit + 0x2E) == 8) &&
        best == 0 && stopFlag == 0)
    {
        hit = 0;
        for (i = 0; i < count; i++)
        {
            c = D_003931B0 + i * 0x68;
            if (*(s16 *)(c + 0x2E) == 0x63)
                continue;
            if (fabsf(point[0] - *(f32 *)(c + 0x10)) < D_003926A8 &&
                fabsf(point[1] - *(f32 *)(c + 0x14)) < D_003926A8 &&
                fabsf(point[2] - *(f32 *)(c + 0x18)) < D_003926A8)
                hit = hit + 1;
        }

        if (hit == 1 && (*(u32 *)(firstHit + 0x28) & 4) == 0)
        {
            ctrl = *(u32 *)(obj + 0x0C);
            if ((ctrl & 0x10) == 0)
                *(u32 *)(obj + 0x0C) = ctrl | 0x10000;
            MV_TAG2(probeB);
            MV_COPY12(probeB + 0x30, nodeA + 0x30);
            *(f32 *)(obj + 0x108) = 1.0f;
            goto l_stop;
        }
    }

    /* Nothing ahead: park the substep end on the contact's own offset. */
    if (best == 0)
    {
        MV_TAG2(probeB);
        tgt[0] = *(f32 *)(nodeA + 0x30) + *(f32 *)(firstHit + 0x30);
        tgt[1] = *(f32 *)(nodeA + 0x34) + *(f32 *)(firstHit + 0x34);
        tgt[2] = *(f32 *)(nodeA + 0x38) + *(f32 *)(firstHit + 0x38);
        MV_COPY12(probeB + 0x30, tgt);
        *(f32 *)(obj + 0x108) = 1.0f;
        func_0013A148(obj, step);
        return;
    }

    /* A contact ahead: fold the contacts' offsets once the sequence is deep
     * enough, otherwise just snap the end probe back onto node A. */
    if (*(f32 *)best < 0.0f && (s32)D_003931A4 >= 3)
    {
        func_00136C08(off, obj, count, D_003931B0, scanEnd);
        MV_TAG2(probeB);
        tgt[0] = *(f32 *)(nodeA + 0x30) + off[0];
        tgt[1] = *(f32 *)(nodeA + 0x34) + off[1];
        tgt[2] = *(f32 *)(nodeA + 0x38) + off[2];
        MV_COPY12(probeB + 0x30, tgt);
        *(f32 *)(obj + 0x108) = 1.0f;

        delta[0] = delta[0] - (tgt[0] - *(f32 *)(nodeA + 0x30));
        delta[1] = delta[1] - (tgt[1] - *(f32 *)(nodeA + 0x34));
        delta[2] = delta[2] - (tgt[2] - *(f32 *)(nodeA + 0x38));

        MV_TAG2(probeA);
        MV_COPY12(nodeA + 0x30, nodeB + 0x30);
        *(f32 *)(nodeA + 0x3C) = 1.0f;
    }
    else
    {
        MV_TAG2(probeB);
        MV_COPY12(probeB + 0x30, nodeA + 0x30);
        *(f32 *)(obj + 0x108) = 1.0f;
    }

    /* Two planes: the first hit's and the best contact's. */
    func_0012F180(out, obj, delta, nrm, (f32 *)(best + 0x04), 0, step);

    if (fabsf(out[0]) < D_003926AC && fabsf(out[1]) < D_003926AC &&
        fabsf(out[2]) < D_003926AC)
    {
        MV_TAG2(probeB);
        MV_COPY12(probeB + 0x30, nodeA + 0x30);
        *(f32 *)(obj + 0x108) = 1.0f;
        func_0013A148(obj, step);
        return;
    }

    /* Correction left over: run another substep, then close the step. */
    func_0012F8D8(obj, ctx, step);
    func_0013A148(obj, step);
    return;

l_reset:
    /* The substep never moved: node B becomes a copy of node A. */
    MV_COPY54(nodeB, nodeA);

l_stop:
    *(f32 *)(obj + 0x3C) = 0.0f;
    *(f32 *)(obj + 0x40) = 0.0f;
    *(f32 *)(obj + 0x44) = 0.0f;
    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x10) == 0)
    {
        ctrl = ctrl | 0x10000;
        *(u32 *)(obj + 0x0C) = ctrl;
    }
    /* Bit 0x4000 keeps the attached record's transform in sync with its own
     * first word (retail copies *(u32 *)attach into *(u32 *)(attach + 4)). */
    if ((ctrl & 0x4000) != 0)
    {
        parent = *(u8 **)(obj + 0x148);
        *(u32 *)(parent + 0x04) = *(u32 *)parent;
    }
}

#undef MV_COPY54
#undef MV_COPY12
#undef MV_TAG2


/* Source: math/region_0013.c */
/* func_00130888 through func_0013FFA0; retail address range 0x00130000-0x00140000. */



extern s32 D_003931C4;

/* ------------------------------------------------------------------------- */
/* func_00130888 - contact-constrained movement step (0x14F0 = 5360 bytes)   */
/* ------------------------------------------------------------------------- */
/* The actor-side mover used when obj+0x08 has bit 0x40 set (see func_0012BA18,
 * which calls it as (obj, ctx, step) right after the countdown dispatch).  One
 * call resolves the current sub-step's translation against the contact list
 * that func_00135530 fills from D_003931B0, writes the result into node B
 * (obj+0xCC) and, when the actor cannot make progress, stops it
 * (velocity +0x3C/+0x40/+0x44 zeroed, ctrl |= 0x10000).
 *
 * The query is swept: node B is copied twice and the copies' translations are
 * moved along the last stored surface normal (obj+0x1C4) by +-scale, so the
 * contact builder sees a short segment across the surface instead of a point.
 * The result is then snapped to the surface plane (func_0012F180) and the whole
 * step is retried while the correction still points along the global movement
 * direction (D_00331148).
 *
 * D_003931A4 counts the passes of one movement sequence (it is cleared by
 * func_0012BA18 each frame): pass 1 remembers the tangential delta in
 * D_00331148, and from pass 4 on the actor is considered stuck and snapped
 * back onto node A.
 *
 * Layout of the 0x250 stack frame used below (retail offsets):
 *   sp+0x00 axis[3]     node B's local Y axis (func_00126510)
 *   sp+0x10 off[3]      scratch vector (normal component, then position delta)
 *   sp+0x20 pos[3]      func_00136C08 accumulator (in/out position)
 *   sp+0x30 delta[3]    node B translation - node A translation
 *   sp+0x40 nComp[3]    normal part of delta
 *   sp+0x50 tang[3]     tangential part of delta
 *   sp+0x60 out[3]      func_0012F180 correction
 *   sp+0x70            (0,0,1) written and never read - retail dead store
 *   sp+0x80 resolved[3] candidate node B translation
 *   sp+0x90 dir[3]      unit movement direction
 *   sp+0xA0 tdir[3]     unit tangential direction
 *   sp+0xB0 probe[3]    probe translation under construction
 *   sp+0xC0 node copy A ("+scale" probe, 5th argument of func_00135530)
 *   sp+0x114 node copy B ("-scale" probe, 6th argument)
 *   sp+0x170 count      contact count written by func_00135530
 *   sp+0x178 lastIndex  last contact index the scans looked at
 *   sp+0x180 best       best contact candidate (0 = none)
 *   sp+0x184 node A     obj+0x78
 *   sp+0x188 &nodeB+0x30, sp+0x18C &nodeA+0x30
 */

/* Callees (all still INCLUDE_ASM; the declarations fix the float register
 * class and the argument count):
 *   func_00135530(out, contacts, actor, ctx, node5, node6, flag) -> nonzero
 *                 when at least one contact was produced
 *   func_00135CF8(actor, ctx, count, contacts, indexLimit) rebuilds the list
 *   func_00136C08(position, actor, count, contacts, indexLimit) folds the
 *                 usable contacts' offsets into `position`
 *   func_0012F180(out, actor, delta, normalA, normalB, flag, step) snaps node B
 *                 onto the plane spanned by the two normals and moves it by the
 *                 remaining depth; the float argument is ignored by the callee
 *   func_0012F518(actor, normal) wall response for a contact normal that is not
 *                 the actor's own up axis
 *   func_0013A148(actor, step) closes the movement step
 */
s32 func_00139708(u8 *, f32 *);
s32 func_001396A0(u8 *, f32 *);
void func_0013AC60(s32, u8 *, u8 *, f32 *);
s32 func_00135530(s32 *, u8 *, u8 *, u8 *, u8 *, u8 *, s32);
void func_0012F180(f32 *, u8 *, f32 *, f32 *, f32 *, s32, f32);
void func_0012F518(u8 *, f32 *);
s32 func_00136C08(f32 *, u8 *, s32, u8 *, s32);
f32 func_00217DA0(f32);

/* Globals: the contact array, the movement-sequence pass counter, the
 * tangential delta remembered for the first pass, and the literal-pool
 * constants (referenced by symbol, never by value). */
/* D_003931B0 is a POINTER variable, not the array itself: the word at
 * 0x003931B0 holds 0x00384178 in the retail image and every one of the 67
 * references in the game is a `lw` (load the pointer), never an `addiu`/
 * `la` (take the address).  Passing its address hands the contact routines
 * the 4 bytes of the pointer plus whatever follows them (0xFF7FFFF8 lives at
 * 0x003931BC), so contacts used to be written over the globals instead of
 * into the buffer. */
extern u8 *D_003931B0;
extern f32 D_00331148[3];
extern f32 D_003926B4;          /* 1.0471976 = pi/3, fed to tanf          */
extern f32 D_003926B8;          /* 0.001  - delta-is-zero epsilon         */
extern f32 D_003926BC;          /* 0.001  - tangential delta epsilon      */
extern f32 D_003926C0;          /* 0.45   - minimum probe scale           */
extern f32 D_003926C4;          /* 0.707  - contact normals too parallel  */
extern f32 D_003926C8;          /* 0.1    - contact behind the wall plane */
extern f32 D_003926CC;          /* 0.001  - correction-is-zero epsilon    */
extern f32 D_003926D0;          /* 0.001  - correction-is-zero epsilon    */
extern f32 D_003926D4;          /* 0.001  - normal == node axis epsilon   */

/* U_COPY54 / U_COPY12 were #undef'd above; the typedefs survive. */
#define BM_COPY54(d, s) (*(UBytes54 *)(d) = *(UBytes54 *)(s))
#define BM_COPY12(d, s) (*(UBytes12 *)(d) = *(UBytes12 *)(s))

/* A node written by this function is either left alone or becomes a
 * translation-only node (tag 1 -> 2) with a 1.0 homogeneous element. */
#define BM_TAG2(p)                                                         \
    do {                                                                   \
        if (*(u8 *)((p) + 0x50) == 1)                                      \
            *(u8 *)((p) + 0x50) = 2;                                       \
    } while (0)

void func_00130888(u8 *obj, u8 *ctx, f32 step)
{
    u8  nPos[0x54];             /* sp+0xC0 */
    u8  nNeg[0x54];             /* sp+0x114 */
    f32 axis[3];                /* sp+0x00 */
    f32 off[3];                 /* sp+0x10 */
    f32 pos[3];                 /* sp+0x20 */
    f32 delta[3];               /* sp+0x30 */
    f32 nComp[3];               /* sp+0x40 */
    f32 tang[3];                /* sp+0x50 */
    f32 out[3];                 /* sp+0x60 */
    f32 resolved[3];            /* sp+0x80 */
    f32 dir[3];                 /* sp+0x90 */
    f32 tdir[3];                /* sp+0xA0 */
    f32 probe[3];               /* sp+0xB0 */
    s32 count;                  /* sp+0x170 */
    s32 lastIndex;              /* sp+0x178 */
    u8 *best;                   /* sp+0x180 */
    u8 *wall;                   /* $21: first own, wall-like contact */
    u8 *nodeA = obj + 0x78;
    u8 *nodeB = obj + 0xCC;
    f32 *surfNormal = (f32 *)(obj + 0x1C4);
    f32 tanStep;
    f32 scale;
    f32 len;
    f32 d;
    f32 minDot;
    s32 scanEnd;                /* $22: one past the last contact examined */
    s32 stop;
    s32 anySmall;
    u16 f6w;
    u32 ctrl;
    u8 *c;
    s32 i;

    /* tan(pi/3) = sqrt(3): the probe scale for a full-length step. */
    tanStep = func_00217DA0(D_003926B4);

    /* ---- entry: respond to the surface normal func_00135CF8 stored ---------
     * Bit 0x10 of ctrl means "a near-side contact was found last query"; the
     * 12 bytes at obj+0x1C4 then hold that contact's normal.  A zero normal
     * (never written) must not call the wall helper. */
    if ((*(u32 *)(obj + 0x0C) & 0x10) != 0 &&
        (surfNormal[0] != 0.0f || surfNormal[1] != 0.0f || surfNormal[2] != 0.0f))
        func_0012F518(obj, surfNormal);

    /* ---- probe nodes: two copies of node B (retail copies "-scale" first) - */
    BM_COPY54(nNeg, nodeB);

    delta[0] = *(f32 *)(nodeB + 0x30) - *(f32 *)(nodeA + 0x30);
    delta[1] = *(f32 *)(nodeB + 0x34) - *(f32 *)(nodeA + 0x34);
    delta[2] = *(f32 *)(nodeB + 0x38) - *(f32 *)(nodeA + 0x38);

    BM_COPY54(nPos, nodeB);

    /* ---- movement direction -------------------------------------------------
     * Unit node-B-minus-node-A translation; zero when the two nodes coincide,
     * and the raw difference when its length is exactly zero (NaN/degenerate). */
    if (fabsf(delta[0]) < D_003926B8 && fabsf(delta[1]) < D_003926B8 &&
        fabsf(delta[2]) < D_003926B8)
    {
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        dir[2] = 0.0f;
    }
    else
    {
        len = F_SQRT(F_DOT3(delta[0], delta[0], delta[1], delta[1],
                            delta[2], delta[2]));
        if (len == 0.0f)
        {
            dir[0] = delta[0];
            dir[1] = delta[1];
            dir[2] = delta[2];
        }
        else
        {
            d = 1.0f / len;
            dir[0] = d * delta[0];
            dir[1] = d * delta[1];
            dir[2] = d * delta[2];
        }
    }

    /* ---- stuck detection: from the fourth pass the actor is snapped back --
     * Node B is placed exactly on node A, the velocity is dropped and the
     * movement sequence ends (no ctrl bit and no parent update here). */
    D_003931A4 = D_003931A4 + 1;
    if ((s32)D_003931A4 >= 4)
    {
        BM_TAG2(nodeB);
        BM_COPY12(nodeB + 0x30, nodeA + 0x30);
        *(f32 *)(nodeB + 0x3C) = 1.0f;
        *(f32 *)(obj + 0x3C) = 0.0f;
        *(f32 *)(obj + 0x40) = 0.0f;
        *(f32 *)(obj + 0x44) = 0.0f;
        return;
    }

    /* ---- split the movement into normal / tangential components ------------
     * The normal is the surface normal of the previous query, not one of the
     * contacts about to be built. */
    func_00126510(nodeB, axis);

    d = F_DOT3(delta[0], surfNormal[0], delta[1], surfNormal[1],
               delta[2], surfNormal[2]);
    nComp[0] = d * surfNormal[0];
    nComp[1] = d * surfNormal[1];
    nComp[2] = d * surfNormal[2];
    tang[0] = delta[0] - nComp[0];
    tang[1] = delta[1] - nComp[1];
    tang[2] = delta[2] - nComp[2];

    /* First pass of a movement sequence: remember the tangential delta, it is
     * what every later pass is compared against (func_0012F180). */
    if (D_003931A4 == 1)
        BM_COPY12(&D_00331148[0], tang);

    /* ---- unit tangential direction ----------------------------------------- */
    len = F_SQRT(F_DOT3(tang[0], tang[0], tang[1], tang[1], tang[2], tang[2]));
    if (fabsf(tang[0]) < D_003926BC && fabsf(tang[1]) < D_003926BC &&
        fabsf(tang[2]) < D_003926BC)
    {
        tdir[0] = 0.0f;
        tdir[1] = 0.0f;
        tdir[2] = 0.0f;
    }
    else if (len == 0.0f)
    {
        tdir[0] = tang[0];
        tdir[1] = tang[1];
        tdir[2] = tang[2];
    }
    else
    {
        d = 1.0f / len;
        tdir[0] = d * tang[0];
        tdir[1] = d * tang[1];
        tdir[2] = d * tang[2];
    }

    /* ---- probe half-length --------------------------------------------------
     * ctrl bit 0x10 ("contact already known") uses a fixed 0.75 instead of the
     * distance-proportional thickness.  0.45 is the floor for both. */
    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x10) == 0)
        scale = tanStep * len;
    else
        scale = 0.75f;
    if (scale < D_003926C0)
        scale = D_003926C0;

    /* ---- build the two probe nodes -----------------------------------------
     * nPos = A + tang + scale*n, nNeg = A + tang - scale*n (node A's
     * translation, not node B's: the probes straddle the ground at the actor's
     * own position advanced by the tangential movement). */
    off[0] = scale * surfNormal[0];
    off[1] = scale * surfNormal[1];
    off[2] = scale * surfNormal[2];

    probe[0] = *(f32 *)(nodeA + 0x30) + tang[0] + off[0];
    probe[1] = *(f32 *)(nodeA + 0x34) + tang[1] + off[1];
    probe[2] = *(f32 *)(nodeA + 0x38) + tang[2] + off[2];
    BM_TAG2(nPos);
    BM_COPY12(nPos + 0x30, probe);
    *(f32 *)(nPos + 0x3C) = 1.0f;

    probe[0] = *(f32 *)(nodeA + 0x30) + tang[0] - off[0];
    probe[1] = *(f32 *)(nodeA + 0x34) + tang[1] - off[1];
    probe[2] = *(f32 *)(nodeA + 0x38) + tang[2] - off[2];
    BM_TAG2(nNeg);
    BM_COPY12(nNeg + 0x30, probe);
    *(f32 *)(nNeg + 0x3C) = 1.0f;

    /* ---- contact query over the swept segment ------------------------------- */
    best = 0;
    wall = 0;
    lastIndex = 0;
    scanEnd = 0;
    stop = 0;
    minDot = 0.0f;
    count = 0;

    if (func_00135530(&count, D_003931B0, obj, ctx, nPos, nNeg, 1) == 0)
        goto l_reset;                       /* 0x31C30 */

    /* Drop the contacts that do not face the movement direction. */
    func_0013AC60(count, D_003931B0, obj, tdir);

    /* ---- scan 1: first contact that belongs to this actor and is wall-like - */
    if (count > 0)
    {
        for (i = 0; i < count; i++)
        {
            c = D_003931B0 + i * 0x68;
            scanEnd = i + 1;
            if (*(s16 *)(c + 0x2E) == 0x63)
                continue;
            f6w = *(u16 *)(*(u8 **)(c + 0x20) + 0x06);
            if ((f6w & 0x40) == 0 || (f6w & 0x8000) != 0)
                continue;
            if ((func_001396A0(obj, (f32 *)(c + 0x04)) & 0xFF) != 0)
            {
                wall = c;
                break;
            }
        }
    }

    /* ---- scan 2: first floor-like contact, re-queried on its own ----------- */
    if (count > 0)
    {
        for (i = 0; i < count; i++)
        {
            c = D_003931B0 + i * 0x68;
            if (*(s16 *)(c + 0x2E) == 0x63)
                continue;
            if ((func_00139708(obj, (f32 *)(c + 0x04)) & 0xFF) == 0)
                continue;
            func_00135CF8(obj, ctx, 1, c, 0);
            break;
        }
    }

    /* ---- scan 3 (only with a wall contact): pick the contact the actor is
     * moving into.  `best` is the one with the smallest dot(tdir, normal),
     * which for surfaces not facing the movement is negative.  All the dots
     * here use the wall contact's own normal (wall+0x04), not the copy that
     * func_00135CF8 may have left at obj+0x1C4. */
    if (wall != 0)
    {
        best = 0;
        minDot = 0.0f;
        lastIndex = 0;
        stop = 0;
        if (count > 0)
        {
            for (i = 0; i < count; i++)
            {
                c = D_003931B0 + i * 0x68;
                lastIndex = i + 1;
                if (*(s16 *)(c + 0x2E) != 0x63)
                {
                    d = F_DOT3(*(f32 *)(c + 0x04), *(f32 *)(wall + 0x04),
                               *(f32 *)(c + 0x08), *(f32 *)(wall + 0x08),
                               *(f32 *)(c + 0x0C), *(f32 *)(wall + 0x0C));
                    if (d < D_003926C4)
                    {
                        off[0] = *(f32 *)(c + 0x10) - *(f32 *)(wall + 0x10);
                        off[1] = *(f32 *)(c + 0x14) - *(f32 *)(wall + 0x14);
                        off[2] = *(f32 *)(c + 0x18) - *(f32 *)(wall + 0x18);
                        d = F_DOT3(off[0], *(f32 *)(wall + 0x04),
                                   off[1], *(f32 *)(wall + 0x08),
                                   off[2], *(f32 *)(wall + 0x0C));
                        if (d > D_003926C8)
                        {
                            f6w = *(u16 *)(*(u8 **)(c + 0x20) + 0x06);
                            d = F_DOT3(tdir[0], *(f32 *)(c + 0x04),
                                       tdir[1], *(f32 *)(c + 0x08),
                                       tdir[2], *(f32 *)(c + 0x0C));
                            if ((f6w & 0x40) != 0 && (f6w & 0x8000) == 0)
                            {
                                /* A contact of ours: the search is over. */
                                best = 0;
                                stop = 1;
                            }
                            else if (d < minDot)
                            {
                                best = c;
                                minDot = d;
                            }
                        }
                    }
                }
                if (i + 1 >= count)
                    break;
                if (stop != 0)
                    break;
            }
        }

        /* Reject a "best" that is one of our own wall contacts unless its
         * normal is exactly perpendicular to the wall contact's normal. */
        if (best != 0)
        {
            f6w = *(u16 *)(*(u8 **)(best + 0x20) + 0x06);
            if ((f6w & 0x40) != 0 && (f6w & 0x8000) == 0 &&
                (func_001396A0(obj, (f32 *)(best + 0x04)) & 0xFF) != 0)
            {
                d = F_DOT3(*(f32 *)(best + 0x04), *(f32 *)(wall + 0x04),
                           *(f32 *)(best + 0x08), *(f32 *)(wall + 0x08),
                           *(f32 *)(best + 0x0C), *(f32 *)(wall + 0x0C));
                if (d != 0.0f)
                    best = 0;
            }
        }
    }

    /* ---- walking down a step edge: stop at the first floor contact ---------
     * Only for actors with a scene record that is not flagged yet, that do not
     * already know a contact, while moving "down" (tangential z < 0). */
    ctrl = *(u32 *)(obj + 0x0C);
    if ((ctrl & 0x4000) != 0 && (*(u32 *)(*(u8 **)(obj + 0x148)) & 1) == 0 &&
        (ctrl & 0x10) == 0 && tang[2] < 0.0f && count > 0)
    {
        for (i = 0; i < count; i++)
        {
            c = D_003931B0 + i * 0x68;
            if (*(s16 *)(c + 0x2E) == 0x63)
                continue;
            if ((*(u16 *)(*(u8 **)(c + 0x20) + 0x06) & 0x8000) != 0)
                continue;
            if ((func_00139708(obj, (f32 *)(c + 0x04)) & 0xFF) == 0)
                continue;

            BM_TAG2(nodeB);
            BM_COPY12(nodeB + 0x30, nodeA + 0x30);
            /* Retail hoists a fresh 1.0 into $f21 at 0x312C4 here (overwriting
             * tan(pi/3)), so this is the plain homogeneous 1.0, not the step
             * scale. */
            *(f32 *)(nodeB + 0x3C) = 1.0f;
            *(f32 *)(obj + 0x3C) = 0.0f;
            *(f32 *)(obj + 0x40) = 0.0f;
            *(f32 *)(obj + 0x44) = 0.0f;
            *(u32 *)(obj + 0x0C) = ctrl | 0x10000;
            func_00135CF8(obj, ctx, count, D_003931B0, lastIndex - 1);
            return;
        }
    }

    if (best != 0 && scanEnd < lastIndex)
        scanEnd = lastIndex;

    func_00135CF8(obj, ctx, count, D_003931B0, scanEnd - 1);

    if (wall == 0)
        goto l_no_wall;                     /* 0x31884 */

    /* ---- wall contact found: publish its normal, then place node B ---------- */
    BM_COPY12(obj + 0x1C4, wall + 0x04);

    if (best != 0)
        goto l_best;                        /* 0x315CC */

    if ((u16)(*(u16 *)(wall + 0x2E) - 7) < 2)
    {
        /* Step edge of type 7/8: look for a wall-like partner of the same kind. */
        best = 0;
        if (count > 0)
        {
            for (i = 0; i < count; i++)
            {
                c = D_003931B0 + i * 0x68;
                if (*(s16 *)(c + 0x2E) == 0x63)
                    continue;
                if ((func_001396A0(obj, (f32 *)(c + 0x04)) & 0xFF) == 0)
                    continue;
                if ((u16)(*(u16 *)(c + 0x2E) - 7) < 2)
                    best = c;
                if (best != 0)
                    break;
            }
        }

        if (best != 0)
        {
            resolved[0] = *(f32 *)(nNeg + 0x30) + *(f32 *)(best + 0x30);
            resolved[1] = *(f32 *)(nNeg + 0x34) + *(f32 *)(best + 0x34);
            resolved[2] = *(f32 *)(nNeg + 0x38) + *(f32 *)(best + 0x38);
        }
        else
        {
            resolved[0] = *(f32 *)(nNeg + 0x30) + *(f32 *)(wall + 0x30);
            resolved[1] = *(f32 *)(nNeg + 0x34) + *(f32 *)(wall + 0x34);
            resolved[2] = *(f32 *)(nNeg + 0x38) + *(f32 *)(wall + 0x38);
        }
        BM_TAG2(nodeB);
        BM_COPY12(nodeB + 0x30, resolved);
        *(f32 *)(nodeB + 0x3C) = 1.0f;
        goto l_check;
    }

    /* Plain wall: node B goes to the negative probe plus the contact offset. */
    resolved[0] = *(f32 *)(nNeg + 0x30) + *(f32 *)(wall + 0x30);
    resolved[1] = *(f32 *)(nNeg + 0x34) + *(f32 *)(wall + 0x34);
    resolved[2] = *(f32 *)(nNeg + 0x38) + *(f32 *)(wall + 0x38);
    BM_TAG2(nodeB);
    BM_COPY12(nodeB + 0x30, resolved);
    *(f32 *)(nodeB + 0x3C) = 1.0f;
    goto l_check;

l_best:                                     /* 0x315CC */
    /* Near-side contact (distance < 0): resolve the movement against the wall
     * plane.  The full version needs the pass counter to be 3 or the surface
     * normal to be known; otherwise node A's own translation is used. */
    if (*(f32 *)(best + 0x00) < 0.0f)
    {
        if ((s32)D_003931A4 < 3 && (*(u32 *)(obj + 0x0C) & 0x10) == 0)
        {
            resolved[0] = *(f32 *)(nodeA + 0x30);
            resolved[1] = *(f32 *)(nodeA + 0x34);
            resolved[2] = *(f32 *)(nodeA + 0x38);
        }
        else
        {
            func_00136C08(pos, obj, count, D_003931B0, scanEnd - 1);

            /* Remove the normal part of the accumulated offset and add the
             * contact's own offset: the point on the wall plane. */
            d = F_DOT3(pos[0], *(f32 *)(wall + 0x04), pos[1], *(f32 *)(wall + 0x08),
                       pos[2], *(f32 *)(wall + 0x0C));
            off[0] = d * *(f32 *)(wall + 0x04);
            off[1] = d * *(f32 *)(wall + 0x08);
            off[2] = d * *(f32 *)(wall + 0x0C);
            pos[0] = pos[0] - off[0];
            pos[1] = pos[1] - off[1];
            pos[2] = pos[2] - off[2];
            pos[0] = pos[0] + *(f32 *)(wall + 0x30);
            pos[1] = pos[1] + *(f32 *)(wall + 0x34);
            pos[2] = pos[2] + *(f32 *)(wall + 0x38);

            resolved[0] = *(f32 *)(nNeg + 0x30) + pos[0];
            resolved[1] = *(f32 *)(nNeg + 0x34) + pos[1];
            resolved[2] = *(f32 *)(nNeg + 0x38) + pos[2];

            /* The delta handed to func_0012F180 is what is left to resolve. */
            off[0] = resolved[0] - *(f32 *)(nodeA + 0x30);
            off[1] = resolved[1] - *(f32 *)(nodeA + 0x34);
            off[2] = resolved[2] - *(f32 *)(nodeA + 0x38);
            delta[0] = delta[0] - off[0];
            delta[1] = delta[1] - off[1];
            delta[2] = delta[2] - off[2];
        }
    }
    else
    {
        resolved[0] = *(f32 *)(nodeA + 0x30);
        resolved[1] = *(f32 *)(nodeA + 0x34);
        resolved[2] = *(f32 *)(nodeA + 0x38);
    }

    BM_TAG2(nodeB);
    BM_COPY12(nodeB + 0x30, resolved);
    *(f32 *)(nodeB + 0x3C) = 1.0f;
    func_0012F180(out, obj, delta, (f32 *)(wall + 0x04), (f32 *)(best + 0x04),
                  1, step);

    /* 0x31794: a correction that points back along the movement direction, or
     * one that is (practically) zero, leaves node B on the resolved position;
     * only a correction that still moves the actor forward is resolved again.
     * This variant uses D_003926CC, not the D_003926D0 of the other one. */
    if (fabsf(out[0]) < D_003926CC && fabsf(out[1]) < D_003926CC &&
        fabsf(out[2]) < D_003926CC)
        goto l_snapResolved;                /* 0x31848 */
    if (F_DOT3(D_00331148[0], out[0], D_00331148[1], out[1],
               D_00331148[2], out[2]) < 0.0f)
        goto l_snapResolved;                /* 0x31848 */
    goto l_recurse;

l_no_wall:                                  /* 0x31884 */
    if (best == 0)
        goto l_reset;                       /* 0x31A8C */

    func_00136C08(pos, obj, count, D_003931B0, scanEnd);

    resolved[0] = *(f32 *)(nNeg + 0x30) + pos[0];
    resolved[1] = *(f32 *)(nNeg + 0x34) + pos[1];
    resolved[2] = *(f32 *)(nNeg + 0x38) + pos[2];
    off[0] = resolved[0] - *(f32 *)(nodeA + 0x30);
    off[1] = resolved[1] - *(f32 *)(nodeA + 0x34);
    off[2] = resolved[2] - *(f32 *)(nodeA + 0x38);
    delta[0] = delta[0] - off[0];
    delta[1] = delta[1] - off[1];
    delta[2] = delta[2] - off[2];

    BM_TAG2(nodeB);
    BM_COPY12(nodeB + 0x30, resolved);
    *(f32 *)(nodeB + 0x3C) = 1.0f;
    /* No wall normal here: the node's own axis and the best contact normal
     * define the plane. */
    func_0012F180(out, obj, delta, axis, (f32 *)(best + 0x04), 1, step);

    /* 0x3197C: unlike the wall variant, a correction pointing back along the
     * movement direction stops the actor (node B on node A, velocity zeroed);
     * a zero correction keeps the resolved position; only a forward correction
     * is resolved again. */
    if (F_DOT3(D_00331148[0], out[0], D_00331148[1], out[1],
               D_00331148[2], out[2]) < 0.0f)
        goto l_snapA;                       /* 0x319BC */

    if (fabsf(out[0]) < D_003926D0 && fabsf(out[1]) < D_003926D0 &&
        fabsf(out[2]) < D_003926D0)
        goto l_snapResolved;                /* 0x31A50 */

l_recurse:                                  /* 0x31830 */
    /* The actor moved, so resolve the rest of the step from the new position. */
    func_00130888(obj, ctx, step);
    func_0013A148(obj, step);
    return;

l_snapResolved:                             /* 0x31A50 */
    BM_TAG2(nodeB);
    BM_COPY12(nodeB + 0x30, resolved);
    *(f32 *)(nodeB + 0x3C) = 1.0f;
    goto l_check;

l_snapA:                                    /* 0x319BC */
    BM_TAG2(nodeB);
    BM_COPY12(nodeB + 0x30, nodeA + 0x30);
    *(f32 *)(nodeB + 0x3C) = 1.0f;
    *(f32 *)(obj + 0x3C) = 0.0f;
    *(f32 *)(obj + 0x40) = 0.0f;
    *(f32 *)(obj + 0x44) = 0.0f;
    return;

l_check:                                    /* 0x31B78 */
    /* A wall contact whose normal is not the actor's own up axis needs the wall
     * response before the step is closed.  Contacts of type 7/8 (step edges)
     * never do. */
    anySmall = 0;
    if (wall != 0)
    {
        if ((u16)(*(u16 *)(wall + 0x2E) - 7) < 2)
            goto l_tail;
        d = *(f32 *)(wall + 0x04) - axis[0];
        anySmall = (fabsf(d) < D_003926D4);
        if (anySmall)
        {
            d = *(f32 *)(wall + 0x08) - axis[1];
            anySmall = (fabsf(d) < D_003926D4);
            if (anySmall)
            {
                d = *(f32 *)(wall + 0x0C) - axis[2];
                anySmall = (fabsf(d) < D_003926D4);
            }
        }
        if (anySmall)
            goto l_tail;
        func_0012F518(obj, (f32 *)(wall + 0x04));
    }

l_tail:
    func_0013A148(obj, step);
    return;

l_reset:                                    /* 0x31C30 / 0x31A8C */
    /* Nothing usable came out of the query: node B is node A, the velocity is
     * dropped and the movement sequence is told to end. */
    BM_COPY54(nodeB, nodeA);
    *(f32 *)(obj + 0x3C) = 0.0f;
    *(f32 *)(obj + 0x40) = 0.0f;
    *(f32 *)(obj + 0x44) = 0.0f;
    ctrl = *(u32 *)(obj + 0x0C) | 0x10000;
    *(u32 *)(obj + 0x0C) = ctrl;
    if ((ctrl & 0x4000) != 0)
    {
        u8 *pd = *(u8 **)(obj + 0x148);

        *(u32 *)(pd + 0x04) = *(u32 *)pd;
    }
    return;
}

#undef BM_COPY54
#undef BM_COPY12
#undef BM_TAG2


