#include "common.h"
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* Externs used by func_00131D78 (addresses taken from the retail relocation  */
/* table).  Types follow the access width used in the assembly.              */
/* ------------------------------------------------------------------------- */
extern f32 D_003931AC;          /* lwc1 in prologue */
extern s32 D_003931A4;
extern u8  D_003931A8;
extern s32 D_003931B0;
extern f32 D_003931B4;
extern s32 D_003931B8;
extern s32 D_003931C4;
extern s32 D_00330ED0[];        /* array form -> non-GP-relative addressing (symbol is far from $gp) */
extern s32 D_00393B08;
extern f32 D_00392558;

extern f32 D_003926D8;
extern f32 D_003926DC;
extern f32 D_003926E0;
extern f32 D_003926E4;
extern f32 D_003926E8;
extern f32 D_003926EC;
extern f32 D_003926F0;
extern f32 D_003926F4;
extern f32 D_003926F8;
extern f32 D_003926FC;
extern f32 D_00392700;
extern f32 D_00392704;
extern f32 D_00392708;
extern f32 D_0039270C;
extern f32 D_00392710;

/* Helpers.  Old-style declarations are used deliberately so that any        */
/* argument count is accepted, exactly like the surrounding decomp files.    */
/* The four f32-returning helpers MUST be declared or GCC assumes int return  */
/* and converts $v0 instead of keeping $f0.                                   */
/* f32-returning helpers MUST be declared, or GCC assumes an int return and
 * converts $v0 instead of keeping $f0.  Float-taking helpers MUST have full
 * prototypes, or default argument promotions widen f32 -> f64 at the call. */
f32 func_00139828(s32, s32);
f32 func_00139858(s32, s32);
f32 func_00217E28(f32);         /* atanf */
f32 func_00217DA0(f32);         /* tanf  */
s32 func_00108648(u8 *);
s32 func_00135530();
s32 func_0013AD90();
s32 func_0013B258();
/* Typed definition in funcs_11: (f32 *position, void *actor, s32 count,
 * SurfaceContact *contacts, s32 indexLimit).  Declared with s32 spellings here
 * because the call sites below keep every argument in an s32 register local;
 * all five are integers or pointers, so the call sequence is unchanged, but the
 * arity is now checked instead of guessed. */
s32 func_00136C08(s32 position, s32 actor, s32 count, s32 contacts, s32 indexLimit);
/* Surface-classification helpers, confirmed from the retail listings (see
 * REFERENCE-BEHAVIORMATCH.md): both are `s32 helper(actor, f32 *normal)`,
 * where `normal` points at the three floats of a SurfaceContact's +0x04 field
 * and the result compares the normal's z component (the up axis) against
 * `actor->+0x18`:
 *   func_00139708: 1 when `actor->+0x18 <= normal[2]`   (floor-like surface)
 *   func_001396A0: 1 when `|normal[2]| < actor->+0x18`  (wall-like surface)
 * `actor` is the actor object pointer this module keeps in an s32 local. */
s32 func_00139708(s32 actor, f32 *normal);
s32 func_001396A0(s32 actor, f32 *normal);
void func_001241D8();
s32 func_0012F6F0(s32, s32, f32);
s32 func_0013B420();
s32 func_00135CF8(s32, s32, s32, s32, s32);
/* Definition in funcs_11 is void: (ActorSurfaceState *actor, s32 count,
 * SurfaceContact *contacts, SurfaceContact *selected).  It returns nothing, so
 * a value-typed declaration here would make callers read a dead $v0. */
void func_00137208(s32 actor, s32 count, s32 contacts, s32 selected);
s32 func_001399D8();
s32 func_00139BA0();
s32 func_00139CE0();
s32 func_0013A910();
s32 func_001378C0();
s32 func_0012F180(s32, s32, s32, s32, s32, s32, f32);  /* 0x333C0/0x33894: $4-$9 + $f12 */
s32 func_0013A148(s32, f32);

s32 func_00131D78(s32, s32, f32);   /* recursive */
f32 fabsf(f32);

/* ------------------------------------------------------------------------- */
/* R5900 FPU accumulator forms.  The EE GCC 2.9 toolchain never emits         */
/* mula.s / madda.s / madd.s from C, so spell them out (identical to the       */
/* pattern used in funcs_06_001346E8_001346E8.c).                             */
/* ------------------------------------------------------------------------- */
#define F_SUM3(x, y, z) ({                                      \
    float r_;                                                   \
    __asm__ volatile ("mula.s %1,%1\n\t"                        \
                      "madda.s %2,%2\n\t"                       \
                      "madd.s %0,%3,%3"                         \
                      : "=&f"(r_) : "f"(x), "f"(y), "f"(z));   \
    r_;                                                         \
})

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

/* R5900 sqrt.s (retail word 0x84000046 / "c1 0x84").  This file is compiled
 * without -ffast-math, so plain sqrtf()/__builtin_sqrtf() emit a library call;
 * spell the instruction out instead (matches the funcs_06/funcs_07 idiom). */
#define F_SQRT(x) ({                                            \
    float r_;                                                   \
    __asm__ volatile ("sqrt.s %0,%1" : "=&f"(r_) : "f"(x));    \
    r_;                                                         \
})

/* Byte-wise copy loop.  GCC's inlined memcpy generates large unaligned
 * load/store storms for the 0x54/12-byte blocks; a simple volatile loop is
 * behaviourally identical (forward byte copy, as retail's ldl/ldr loop) and
 * keeps this function under its retail size so the fixed layout still links. */
#define COPY_N(d, s, n) do {                       \
    volatile u8 *_cd = (volatile u8 *)(d);         \
    volatile u8 *_cs = (volatile u8 *)(s);         \
    s32 _ci, _cn = (s32)(n);                       \
    for (_ci = 0; _ci < _cn; _ci++) _cd[_ci] = _cs[_ci]; \
} while (0)

/* Typed views of a byte address.  b may be an integer register local or a    */
/* pointer; the cast makes both work.                                         */
#define M8(b,o)   (*(volatile u8  *)((u8 *)(b) + (o)))
#define M16(b,o)  (*(volatile s16 *)((u8 *)(b) + (o)))
#define MU16(b,o) (*(volatile u16 *)((u8 *)(b) + (o)))
#define M32(b,o)  (*(volatile s32 *)((u8 *)(b) + (o)))
#define MU32(b,o) (*(volatile u32 *)((u8 *)(b) + (o)))
#define MF(b,o)   (*(volatile f32 *)((u8 *)(b) + (o)))

/* ------------------------------------------------------------------------- */
/* func_00131D78 -- actor surface/collision resolution.                       */
/*                                                                           */
/* The retail frame is 0x310 bytes.  A byte array is used here so that every  */
/* stack slot keeps its exact address; this preserves the stale-slot reads     */
/* across calls (the retail code relies on values left in sp+0x90/0x94/... by  */
/* earlier calls).  Register-named locals mirror $2..$23, $30 and $f0..$f25.   */
/* Delay-slot stores are emitted BEFORE their branch so they always execute.   */
/* ------------------------------------------------------------------------- */
s32 func_00131D78(s32 arg1, s32 arg2, f32 fArg)
{
    u64 stk[0x62];                 /* 0x310 bytes = the retail stack frame */
    u8 *sp = (u8 *)stk;
    s32 r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
    s32 r2x, r2y;
    s32 r16, r17, r18, r19, r20, r21, r22, r23, r30;
    f32 f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;
    f32 f20, f21, f22, f23, f24, f25;
    s32 count;

    f25 = 0.0f;

    /* ---- prologue: 0x31D78 - 0x31E44 ------------------------------------ */
    f1 = 1.0f;
    /* callee-saved spills (0x31D84..0x31DDC) are handled by the C compiler */
    r20 = (s32)sp + 0x54;
    r19 = arg1;
    r4  = r19 + 0x78;
    r17 = r4;
    r23 = r19 + 0xCC;
    f23 = fArg;                                   /* 0x31DBC: mov.s $f23,$f12 */
    M32(sp, 0x150) = 0;
    r16 = 0;
    M32(sp, 0x210) = arg2;
    r5 = 1;
    M32(sp, 0x218) = r4;
    f0 = MF(sp, 0x150);                           /* 0.0f */
    f22 = MF(r19, 0x14);
    r2  = M32(r19, 0xC);
    f21 = -f22;                                   /* neg.s */
    f24 = D_003931AC;
    r2 &= 0x4000;
    MF(sp, 0x158) = 1.0f;
    MF(sp, 0x1B8) = f0;
    M32(sp, 0x214) = (s32)sp;
    M32(sp, 0x224) = 1;
    M32(sp, 0x228) = 0;
    M32(sp, 0x154) = 0;
    MF(sp, 0x190) = f0;
    MF(sp, 0x194) = f0;
    MF(sp, 0x198) = f0;
    MF(sp, 0x1B0) = f0;
    MF(sp, 0x1B4) = f0;                           /* delay slot of beqz */
    if (r2 == 0) goto L_00131E44;
    r2 = M32(r19, 0x148);
    r16 = r2 + 0x30;

L_00131E44:                                       /* 0x31E44 */
    /* Two 0x54-byte state copies.  Retail inlines memcpy with alignment      */
    /* checks (ldl/ldr or ld/sd); the net effect is the two memcpy below.      */
    COPY_N(sp + 0x000, (void *)(r23), 0x54);      /* A+0xCC -> sp+0x00 (0x31E6C/0x31EE0/0x31F1C) */
    M32(sp, 0x230) = r19 + 0xFC;
    COPY_N(sp + 0x054, (void *)(r17), 0x54);      /* A+0x78 -> sp+0x54 (0x31F4C/0x31FA8/0x31FE0) */

    count = func_00108648(arg2);                  /* 0x32004 */
    r4 = M32(sp, 0x218);                          /* delay slot */
    if ((u32)count < 2u) goto L_00132140;         /* sltiu $2,$2,2 */

    /* ---- count >= 2: 0x32018 - 0x32140 --------------------------------- */
    r10 = (s32)sp + 0xB0;
    r3  = r17;                                    /* A+0x78 */
    M32(sp, 0x218) = r10;                         /* sp+0x218 = sp+0xB0 */
    r22 = (s32)sp + 0xE0;
    COPY_N(sp + 0x0B0, (void *)(r17), 0x54);      /* A+0x78 -> sp+0xB0 (0x32038/0x3209C/0x320D0) */
    r3 = 1;
    r8 = M32(sp, 0x218);                          /* sp+0xB0 */
    r2 = M8(r8, 0x50);                            /* byte at sp+0x100 */
    r9 = M32(sp, 0x218);                          /* delay slot of bne */
    if (r2 == r3) { r2 = 2; M8(r8, 0x50) = r2; }  /* 1 -> 2 at sp+0x100 */
    f0 = 1.0f;
    COPY_N((void *)((u8 *)r9 + 0x30), (void *)(r19 + 0x1A0), 12); /* 0x32120: ldl 0x1A7($19) */
    MF(r22, 0xC) = f0;                            /* delay slot: sp+0xEC = 1.0f */
    goto L_00132144;

L_00132140:                                       /* 0x32140 */
    r22 = r4 + 0x30;                              /* A+0xA8 */

L_00132144:                                       /* 0x32144 */
    r5 = M32(sp, 0x218);
    r6 = M32(sp, 0x230);
    f0 = MF(r5, 0x30);
    M32(sp, 0x21C) = r22;
    f1 = MF(r23, 0x30);
    r2 = M32(r19, 0xC);
    f1 = f1 - f0;
    r2 &= 0x10;
    MF(sp, 0x140) = f1;                           /* dx = *(A+0xFC) - ref.x */
    f1 = MF(r22, 0x4);
    f0 = MF(r6, 0x4);
    f0 = f0 - f1;
    MF(sp, 0x144) = f0;                           /* dy */
    f1 = MF(r6, 0x8);
    f0 = MF(r22, 0x8);
    f1 = f1 - f0;
    MF(sp, 0x148) = f1;                           /* delay: dz */
    if (r2 == 0) goto L_00132224;

    /* flags & 0x10 block: 0x3218C - 0x32224 */
    r2 = D_003931A4;
    r3 = r2 + 1;                                  /* delay slot */
    if (r2 != 0) goto L_0013222C;
    r8  = M32(sp, 0x218);
    r4  = (s32)sp + 0x204;
    r5  = D_003931B0;
    r6  = r19;
    r7  = M32(sp, 0x210);
    r9  = r8;
    r10 = 1;
    r2 = func_00135530(r4, r5, r6, r7, r8, r9, r10);  /* 0x321B0 */
    r4 = M32(sp, 0x204);                          /* delay slot */
    if (r2 == 0) goto L_00132224;
    r6 = r19;                                     /* delay slot a2 = arg1 at 0x321C0 */
    func_0013AD90(r4, D_003931B0, r6);            /* delay r5 = D_003931B0 */
    func_0013B258(r4, D_003931B0);
    r6 = M32(sp, 0x204);
    r4 = (s32)sp + 0x190;
    r7 = D_003931B0;
    r5 = r19;
    r8 = r6;                                      /* delay slot */
    func_00136C08(r4, r5, r6, r7, r8);            /* 0x321E8 */
    r7 = M32(sp, 0x218);
    f1 = MF(sp, 0x190);
    f0 = MF(r7, 0x30);
    f0 = f0 + f1;
    MF(r7, 0x30) = f0;
    f1 = MF(sp, 0x194);
    f0 = MF(r22, 0x4);
    f2 = MF(r22, 0x8);
    f0 = f0 + f1;
    MF(r22, 0x4) = f0;
    f1 = MF(sp, 0x198);
    f2 = f2 + f1;
    MF(r22, 0x8) = f2;

L_00132224:                                       /* 0x32224 */
    r2 = D_003931A4;
    r3 = r2 + 1;
L_0013222C:                                       /* 0x3222C */
    r2 = r3 < 4;                                  /* slti $2,$3,0x4 (signed) */
    D_003931A4 = r3;                              /* delay slot: sw $3 */
    if (r2 != 0) goto L_00132294;
    /* r3 >= 4: set flag bit 0x02000000 and copy the reference vector out */
    r2 = M32(r19, 0xC) | 0x02000000;
    M32(r19, 0xC) = r2;
    r3 = M8(r23, 0x50);
    r8 = M32(sp, 0x21C);                          /* delay slot */
    if (r3 == 1) { M8(r23, 0x50) = 2; }
    f1 = 1.0f;
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)r8, 12);  /* 0x32120-equivalent */
    f0 = 0.0f;
    r3 = M32(sp, 0x230);
    MF(r3, 0xC) = f1;                             /* delay slot: A+0x108 = 1.0f */
    goto L_00133C54;

    /* ---- normal-vector selection: 0x32294 - 0x322FC -------------------- */
L_00132294:
    r30 = (s32)sp + 0x150;
    r4  = r30;                                    /* delay slot of beqz */
    if (r16 == 0) goto L_001322F0;
    r2 = M32(r19, 0xC) & 0x10;
    f5 = MF(r30, 0x4);                            /* delay slot of bnel */
    if (r2 != 0) goto L_001322F4;
    f0 = MF(r16, 0x4);
    f1 = 0.0f;                                    /* 0x322B4 */
    r30 = r16 + 0x4;                              /* delay slot of bc1f (always) */
    if (f0 != 0.0f) goto L_001322F0;              /* bc1f: f0 != 0 */
    f5 = MF(r30, 0x4);                            /* 0x322C8 */
    f4 = MF(r30, 0x8);                            /* delay slot of bc1f at 0x322D4 */
    if (f5 != 0.0f) goto L_001322F8;
    f1 = MF(r30, 0x0);                            /* delay slot of bc1fl at 0x322E4 */
    if (f4 != 0.0f) goto L_001322FC;              /* bc1fl */
    r30 = r4;                                     /* sp+0x150 */
L_001322F0:
    f5 = MF(r30, 0x4);
L_001322F4:
    f4 = MF(r30, 0x8);
L_001322F8:
    f1 = MF(r30, 0x0);
L_001322FC:
    /* ---- project the offset onto the normal (0x322FC - 0x32364) ------- */
    f0 = MF(sp, 0x140);
    f2 = MF(sp, 0x144);
    f3 = MF(sp, 0x148);
    f7 = F_DOT3(f0, f1, f2, f5, f3, f4);          /* 0x32308 mula/madda/madd */
    f1 = f7 * f1;                                 /* dot * n.x */
    f4 = D_003926D8;
    MF(sp, 0x160) = f1;
    f6 = f0 - f1;
    f0 = MF(r30, 0x4);
    f0 = f7 * f0;                                 /* dot * n.y */
    MF(sp, 0x164) = f0;
    f5 = f2 - f0;
    f1 = MF(r30, 0x8);
    MF(sp, 0x170) = f6;
    f1 = f7 * f1;                                 /* dot * n.z */
    MF(sp, 0x174) = f5;
    f2 = f3 - f1;
    MF(sp, 0x168) = f1;
    f0 = F_SUM3(f6, f5, f2);                      /* 0x3234C: |reaction|^2 */
    /* Retail 0x32358 "c1 0x504" (word 04050046 -> 46000504) is
     * sqrt.s $f20,$f0: f20 = |reaction| = distance from wall.  This is used
     * in the c.lt.s at 0x3235C and all three divisions at 0x32374..0x3238C. */
    f20 = F_SQRT(f0);                             /* 0x32358: sqrt.s $f20,$f0 */
    MF(sp, 0x178) = f2;                           /* delay slot of bc1f */
    if (!(f4 < f20)) goto L_001323A0;             /* c.lt.s $f4,$f20 ; bc1f */
    f0 = f6 / f20;
    f1 = f5 / f20;
    f2 = f2 / f20;
    MF(sp, 0x1E0) = f0;
    MF(sp, 0x1E4) = f1;
    MF(sp, 0x1E8) = f2;                           /* delay slot */
    goto L_001323B0;
L_001323A0:
    f0 = 0.0f;
    MF(sp, 0x1E8) = f0;
    MF(sp, 0x1E0) = f0;
    MF(sp, 0x1E4) = f0;
L_001323B0:
    /* ---- D_003931A8 / flag updates (0x323B0 - 0x32458) ---------------- */
    r4 = 1;
    /* r3 still equals D_003931A4 + 1 from L_0013222C */
    if (r3 != r4) goto L_00132408;
    r2 = D_003931A8;                              /* delay slot */
    f0 = D_003926DC;
    r4 = 0;                                       /* delay slot of bc1t */
    if (f0 < f20) goto L_001323E8;                /* bc1t */
    r3 = M32(r19, 0xC) & 0x02000010;
    if (r3 == 0) { D_003931A8 = r4; goto L_001323F0; } /* beql (delay only if taken) */
L_001323E8:
    r4 = 1;
    D_003931A8 = r4;
L_001323F0:
    r3 = (s32)0xFDFFFFFF;
    r2 = M32(r19, 0xC) & r3;
    M32(r19, 0xC) = r2;                           /* delay slot of b */
    goto L_00132424;
L_00132408:
    if (D_003931A8 != 0) { f1 = MF(r19, 0x48); goto L_00132428; } /* bnel (delay if taken) */
    r2 = M32(r19, 0xC) & 0x02000000;
    if (r2 != 0) { D_003931A8 = 1; goto L_00132424; } /* bnel delay only if taken */
L_00132424:
    f1 = MF(r19, 0x48);
L_00132428:
    f0 = 0.5f;
    /* "c1 0x84" (0x84000046) targets $0: no effect */
    r4 = M32(r19, 0xC);                           /* bc1fl delay slot */
    if (!(f1 < 0.5f)) goto L_00132458;            /* bc1fl: f1 >= 0.5 */
    r4 = M32(sp, 0x210);                          /* jal delay slot: $4 = arg2 */
    r2 = func_0013B420(r4);                       /* 0x32440 */
    if (r2 == 0) goto L_00132454;
    r2 = 1;                                       /* delay slot */
    D_003931A8 = r2;
L_00132454:
    r4 = M32(r19, 0xC);
L_00132458:
    /* ---- optional extra projection against arg1+0x2B0 (0x32458 - 0x32510) */
    r2 = M32(r19, 0xC) & 0x4;
    if (r2 == 0) goto L_001324F8;
    r2 = M32(r19, 0x27C);
    if (r2 == 0) goto L_001324F8;
    r5 = M32(sp, 0x230);
    r2 = r19 + 0x2B0;
    f0 = MF(r19, 0x2B0);
    f5 = MF(r5, 0x0);
    f1 = MF(r2, 0x4);
    f5 = f5 - f0;
    f4 = MF(r2, 0x8);
    MF(sp, 0x180) = f5;
    f2 = MF(r5, 0x4);
    f2 = f2 - f1;
    MF(sp, 0x184) = f2;
    f3 = MF(r5, 0x8);
    f3 = f3 - f4;
    MF(sp, 0x188) = f3;
    f1 = MF(r30, 0x0);
    f0 = MF(r30, 0x8);
    f4 = MF(r30, 0x4);
    f7 = F_DOT3(f5, f1, f2, f4, f3, f0);          /* 0x324B0 */
    f1 = f7 * f1;
    MF(sp, 0x110) = f1;
    f5 = f5 - f1;
    f0 = MF(r30, 0x4);
    f0 = f7 * f0;
    MF(sp, 0x114) = f0;
    f2 = f2 - f0;
    f1 = MF(r30, 0x8);
    MF(sp, 0x180) = f5;
    f1 = f7 * f1;
    MF(sp, 0x184) = f2;
    MF(sp, 0x118) = f1;
    f3 = f3 - f1;
    MF(sp, 0x188) = f3;                           /* delay slot of b at 0x324F0 */
    goto L_00132510;
L_001324F8:
    /* copy sp+0x170..0x178 -> sp+0x180..0x188 (12 bytes) */
    COPY_N((void *)(sp + 0x180), (void *)(sp + 0x170), 12);
L_00132510:
    /* ---- gate the collision response on f20 / reaction length --------- */
    f3 = D_003926E0;
    /* "c1 0x4" (0x04000046) targets $0: no effect */
    f0 = MF(sp, 0x160);                           /* delay slot of bc1f at 0x3251C */
    f2 = MF(sp, 0x164);
    f1 = MF(sp, 0x168);
    if (!(f20 < f3)) goto L_001325C0;             /* bc1f */
    f0 = F_SUM3(f0, f2, f1);                      /* 0x3252C: f0 = |reaction|^2 */
    /* Retail 0x32538 "c1 0x4" (04000046) is sqrt.s $f0,$f0 -> |reaction| */
    f0 = F_SQRT(f0);                              /* 0x32538: sqrt.s $f0,$f0 */
    r2 = M32(r19, 0xC) & 0x4;
    if (!(f3 < f0)) goto L_001325C0;              /* bc1f; delay andi $2,$3,4 */
    if (r2 != 0) {
        r2 = M32(r19, 0x27C);
        if (r2 != 0) { f4 = f21 * f20; goto L_001325C4; } /* bnel delay */
    }
    r2 = M32(r19, 0xC) & 0x10;
    if (r2 != 0) { f4 = f21 * f20; goto L_001325C4; }     /* bnel delay */
    /* ---- call func_0012F6F0 and check D_00330ED0 ---------------------- */
    r5 = M32(sp, 0x210);
    f12 = f23;
    r4 = r19;
    func_0012F6F0(r4, r5, f12);                   /* 0x32574 */
    r3 = (s32)&D_00330ED0[0];
    r2 = D_00330ED0[0] & 0x1;
    r5 = (s32)((u8 *)&D_00330ED0[0] + 0x6C);
    if (r2 == 0) goto L_00133EF8;                 /* beqz */
    r4 = r19;
    f25 = func_00139828((u8 *)r4, r5);            /* 0x32594 */
    f1 = 0.0f;
    if (f25 != 0.0f) goto L_00133EFC;             /* bc1f */
    r2 = M32(r19, 0xC) | 0x8000;
    M32(r19, 0xC) = r2;
    goto L_00133EFC;
L_001325C0:
    f4 = f21 * f20;                               /* 0x325C0: mul.s */
L_001325C4:
    /* ---- clamp f4 against -D_003926E4 and the head float (0x325C4) ---- */
    f0 = -D_003926E4;
    f1 = MF(r19, 0x44);
    f1 = f1 * f23;                                /* delay slot of bc1f (always) */
    if (!(f0 < f4)) goto L_001325E4;              /* bc1f: f4 <= -D_003926E4 */
    f4 = f0;                                      /* mov.s $f4,$f0 */
L_001325E4:
    r2 = M32(r19, 0xC) & 0x10;
    if (r2 == 0) { f1 = MF(r22, 0x0); goto L_00132604; } /* beql (delay if taken) */
    /* c.lt.s $f1,$f4 ; bc1tl L_00132600 (delay f4 = f1, taken-only) */
    if (f1 < f4) { f4 = f1; goto L_00132600; }
L_00132600:
    f1 = MF(r22, 0x0);
L_00132604:
    /* ---- build position o at sp+0x1F0 and push it out (0x32604 - 0x326B0) */
    r16 = 1;
    f0 = MF(sp, 0x170);
    r10 = M32(sp, 0x214);
    f1 = f1 + f0;
    f2 = MF(sp, 0x174);
    r10 = r10 + 0x30;
    f3 = MF(sp, 0x178);
    M32(sp, 0x220) = r10;
    MF(sp, 0x1F0) = f1;
    r11 = M32(sp, 0x214);
    f0 = MF(r22, 0x4);
    f0 = f0 + f2;
    MF(sp, 0x1F4) = f0;
    f1 = MF(r22, 0x8);
    f1 = f1 + f3;
    MF(sp, 0x1F8) = f1;
    r2 = M8(r11, 0x50);
    r4 = M32(sp, 0x214);                          /* delay slot of bne: r4 = sp */
    if (r2 != r16) goto L_0013265C;
    r2 = 2;
    M8(r11, 0x50) = r2;
    r4 = M32(sp, 0x214);                          /* 0x32658 */
L_0013265C:
    f21 = 1.0f;                                   /* 0x3265C: reload f21 */
    /* copy sp+0x1F0..0x1F8 -> r4+0x30 (r4 = sp = stateCopy base) */
    COPY_N((void *)((u8 *)r4 + 0x30), (void *)(sp + 0x1F0), 12);
    r5 = (s32)sp + 0x130;
    r7 = M32(sp, 0x220);
    MF(r7, 0xC) = 1.0f;                           /* 0x32684 */
    f0 = MF(r30, 0x0);
    f0 = f4 * f0;
    MF(sp, 0x130) = f0;
    f1 = MF(r30, 0x4);
    f1 = f4 * f1;
    MF(sp, 0x134) = f1;
    f0 = MF(r30, 0x8);
    f0 = f4 * f0;
    MF(sp, 0x138) = f0;                           /* delay slot of jal at 0x326A8 */
    func_001241D8(r4, r5);                        /* 0x326A8: stateCopy, sp+0x130 */

    /* ---- D_003931A8 branch: re-query + mark type 0x63 (0x326B0 - 0x327E8) */
    r2 = D_003931A8;
    M32(sp, 0x208) = 0;                           /* delay slot of beqz (always) */
    if (r2 == 0) goto L_001327B4;
    f1 = MF(r22, 0x0);
    f0 = MF(sp, 0x170);
    f2 = MF(sp, 0x174);
    f1 = f1 + f0;
    f3 = MF(sp, 0x178);
    MF(sp, 0x1F0) = f1;
    f0 = MF(r22, 0x4);
    f0 = f0 + f2;
    MF(sp, 0x1F4) = f0;
    f1 = MF(r22, 0x8);
    f1 = f1 + f3;
    MF(sp, 0x1F8) = f1;
    r2 = M8(r20, 0x50);                           /* sp+0xA4 */
    r21 = r20 + 0x30;                             /* delay slot of bne */
    if (r2 != r16) goto L_00132700;
    r2 = 2;
    M8(r20, 0x50) = r2;
L_00132700:
    COPY_N((void *)((u8 *)r20 + 0x30), (void *)(sp + 0x1F0), 12);
    r4 = (s32)sp + 0x208;
    MF(r21, 0xC) = 1.0f;                          /* sp+0x90 = 1.0f */
    r6 = r19;
    r5 = D_003931B0;
    r9 = r20;
    r7 = M32(sp, 0x210);
    r10 = 1;
    r8 = M32(sp, 0x218);                          /* delay slot */
    r2 = func_00135530(r4, r5, r6, r7, r8, r9, r10); /* 0x32734 */
    r7 = M32(sp, 0x208);                          /* delay slot */
    if (r2 == 0) goto L_001327C8;
    if (r7 <= 0) goto L_001327D8;
    r18 = 0;
    M32(sp, 0x234) = (s32)sp + 0x204;
    f20 = f22 * f20;                              /* mul.s at 0x32750 */
    r17 = (s32)sp + 0x120;
    r3 = 0x68;
L_00132760:
    r4 = D_003931B0;
    r3 = r18 * r3;                                /* mult 3-operand (R5900) */
    r16 = r4 + r3;
    r2 = M16(r16, 0x2E);
    r2 ^= 0x63;
    r4 = r19;                                     /* delay slot of beqz */
    if (r2 == 0) goto L_0013279C;
    r2 = func_001396A0(r4, (f32 *)(r16 + 0x4));    /* 0x3277C (delay $5) */
    r7 = M32(sp, 0x208);                          /* delay slot */
    if ((r2 & 0xFF) != 0) goto L_0013279C;
    r2 = 0x63;
    M16(r16, 0x2E) = r2;
    r7 = M32(sp, 0x208);
L_0013279C:
    r18 = r18 + 1;
    r2 = r18 < r7;
    r3 = 0x68;                                    /* delay slot */
    if (r2 != 0) goto L_00132760;
    goto L_001327E8;
L_001327B4:
    M32(sp, 0x234) = (s32)sp + 0x204;
    f20 = f22 * f20;
    r17 = (s32)sp + 0x120;
    r21 = r20 + 0x30;                             /* delay slot of b at 0x327C0 (always) */
    goto L_001327E8;
L_001327C8:
    M32(sp, 0x234) = (s32)sp + 0x204;
    f20 = f22 * f20;
    goto L_001327E4;
L_001327D8:
    M32(sp, 0x234) = (s32)sp + 0x204;
    f20 = f22 * f20;
L_001327E4:
    r17 = (s32)sp + 0x120;
L_001327E8:
    f0 = D_003926E8;
    f4 = f20;
    /* c.lt.s $f4,$f0 ; bc1tl L_00132800 (delay f4 = f0, taken-only) */
    if (f4 < f0) { f4 = f0; goto L_00132800; }
    /* fall-through: f4 stays f20 */
    /* ---- build sp+0x120 push vector and write out (0x32800 - 0x328A0) -- */
L_00132800:
    f1 = MF(r22, 0x0);
    r18 = 1;
    f0 = MF(sp, 0x170);
    f2 = MF(sp, 0x174);
    f1 = f1 + f0;
    f3 = MF(sp, 0x178);
    MF(sp, 0x1F0) = f1;
    f0 = MF(r22, 0x4);
    f0 = f0 + f2;
    MF(sp, 0x1F4) = f0;
    f1 = MF(r22, 0x8);
    f1 = f1 + f3;
    MF(sp, 0x1F8) = f1;
    {
        s32 tmp = M8(r20, 0x50);                   /* sp+0xA4 */
        r2 = 2;                                   /* delay slot addiu $2,$0,2 */
        if (tmp == r18) M8(r20, 0x50) = r2;       /* bne: skip when != 1 */
    }
L_00132844:
    f0 = 1.0f;
    r5 = r17;
    COPY_N((void *)((u8 *)r20 + 0x30), (void *)(sp + 0x1F0), 12);
    r4 = r20;
    MF(r21, 0xC) = 1.0f;                          /* sp+0x90 = 1.0f */
    r17 = 0x68;
    r16 = 0x4B;
    f0 = MF(r30, 0x0);
    f0 = f4 * f0;
    MF(sp, 0x120) = f0;
    f1 = MF(r30, 0x4);
    f1 = f4 * f1;
    MF(sp, 0x124) = f1;
    f0 = MF(r30, 0x8);
    f0 = f4 * f0;
    MF(sp, 0x128) = f0;                           /* delay slot of jal */
    func_001241D8(r4, r5);                        /* 0x32898 */
    /* ---- read count, clamp 0x4B - count, re-query (0x328A0 - 0x32970) -- */
    r2 = M32(sp, 0x208);
    r8 = r20;
    r3 = D_003931B0;
    r6 = r19;
    r5 = r2 * r17;                                /* mult $5,$2,$17 */
    r4 = M32(sp, 0x234);
    r2 = r16 - r2;                                /* subu: 0x4B - count */
    r7 = M32(sp, 0x210);
    r9 = M32(sp, 0x214);
    D_00393B08 = r2;                              /* 0x328C8 */
    r10 = 1;                                      /* 0x328C4: addiu $10,$0,1 */
    r5 = r3 + r5;                                 /* delay slot of jal */
    r2 = func_00135530(r4, r5, r6, r7, r8, r9, r10); /* 0x328CC */
    r3 = M32(sp, 0x204);
    r7 = M32(sp, 0x208);
    D_00393B08 = r16;                             /* 0x328DC */
    r3 = r3 + r7;
    M32(sp, 0x204) = r3;
    if (r3 == 0) goto L_00133C64;                 /* beqz; delay sw $3 */
    r2 = r7 * r17;                                /* mult $2,$7,$17 */
    r6 = D_003931B0;
    r5 = r3 - r7;                                 /* subu */
    r4 = r19;
    r7 = (s32)sp + 0x170;
    r8 = (s32)sp + 0x200;
    r9 = (s32)sp + 0x20C;
    r10 = (s32)sp + 0x201;
    r6 = r6 + r2;                                 /* delay slot of jal */
    r20 = func_00139CE0(r4, r5, r6, r7, r8, r9, r10); /* 0x3290C: ret -> r20 */
    {
        s32 tmp = M8(sp, 0x201);
        r2 = M8(sp, 0x200);                       /* bnez delay slot at 0x32920: always */
        if (tmp != 0) goto L_0013295C;
    }
    r2 = D_003931A4;                              /* 0x32924 */
    {
        s32 tmp = r2;
        r2 = M8(sp, 0x200);                       /* bne delay slot at 0x3292C: always */
        if (tmp != r18) goto L_0013295C;          /* 0x32928 */
    }
    r5 = M32(sp, 0x208);
    r4 = r19;
    r2 = M32(sp, 0x204);
    r3 = r5 * r17;                                /* mult $3,$5,$17: count*0x68 */
    r5 = r2 - r5;                                 /* subu */
    r6 = D_003931B0;
    r7 = M32(sp, 0x20C);
    if (r7 == 0) r7 = r20;                        /* movz $7,$20,$7 */
    r6 = r6 + r3;                                 /* delay slot of jal */
    func_00137208(r4, r5, r6, r7);                /* 0x32950 */
    r2 = M8(sp, 0x200);
L_0013295C:
    r4 = M32(sp, 0x204);
    if (r2 == 0) goto L_00132970;                 /* beqz; delay lw $4,0x204 */
    r6 = r19;
    func_001399D8(r4, D_003931B0, r6);            /* 0x32968 (delay $5) */
L_00132970:
    r7 = M32(sp, 0x20C);
    r6 = r19;                                     /* beqz delay slot at 0x32978: always */
    if (r7 == 0) goto L_00132990;                 /* 0x32974 */
    r4 = M32(sp, 0x204);
    func_00139BA0(r4, D_003931B0, r6, r7);        /* 0x32980 (delay $5) */
    r4 = M32(sp, 0x204);                          /* delay slot of b */
    goto L_001329A4;
L_00132990:
    r4 = M32(sp, 0x204);
    r7 = r20;                                     /* delay slot $7 = $20 */
    func_00139BA0(r4, D_003931B0, r6, r7);        /* 0x32990 */
    r4 = M32(sp, 0x204);
L_001329A4:
    /* ---- final passes of the apply stage (0x329A4 - 0x32A08) ---------- */
    r6 = r19;
    r5 = D_003931B0;
    r7 = (s32)sp + 0x180;
    r8 = r20;                                     /* delay slot */
    func_0013A910(r4, r5, r6, r7, r8);            /* 0x329B0 */
    r4 = M32(sp, 0x210);                          /* jal delay slot: $4 = arg2 */
    r2 = func_0013B420(r4);                       /* 0x329B8 */
    if (r2 == 0) goto L_001329E4;                 /* beqz result of 3B420 */
    r3 = D_003931A4;
    r2 = 1;
    r7 = M32(sp, 0x208);
    if (r3 != r2) goto L_001329E8;                /* bne; delay lw $7,0x208 */
    r5 = M32(sp, 0x204);
    r4 = r19;
    func_001378C0(r4, r5, D_003931B0, r7);        /* 0x329DC (delay $6) */
L_001329E4:
    r7 = M32(sp, 0x208);
L_001329E8:
    r21 = 0;
    M32(sp, 0x22C) = 0;                           /* delay slot of beqz at 0x329EC */
    if (r7 == 0) goto L_00132A70;
    if (r7 <= 0) goto L_00132A70;
    r18 = 0;
    f20 = 0.0f;
    r17 = 0;

    /* ---- scan A: negative first-float entries (0x32A08 - 0x32A70) ----- */
L_00132A08:
    r3 = D_003931B0;
    r16 = r3 + r17;
    r2 = M16(r16, 0x2E) ^ 0x63;
    if (r2 == 0) { r18 = r18 + 1; goto L_00132A64; } /* beql at 0x32A18: increment only if taken */
    f0 = MF(r16, 0x0);
    r2 = 1;                                       /* delay slot of bc1t (always) */
    if (f0 < f20) goto L_00132A38;                /* bc1t */
    r2 = 0;
L_00132A38:
    r5 = r16 + 0x4;                               /* delay slot of beqz (always) */
    if (r2 == 0) goto L_00132A60;
    r4 = r19;
    r2 = func_00139708(r4, (f32 *)r5);            /* 0x32A40 */
    r11 = M32(sp, 0x228);
    r2 &= 0xFF;
    r10 = 1;
    r7 = M32(sp, 0x208);
    if (r2 != 0) r10 = r11;                       /* movn $10,$11,$2 */
    M32(sp, 0x228) = r10;
L_00132A60:
    r18 = r18 + 1;
L_00132A64:
    r2 = r18 < r7;
    r17 = r17 + 0x68;                             /* delay slot */
    if (r2 != 0) goto L_00132A08;
L_00132A70:
    /* ---- scan B: nearest surface / 0x22C selection (0x32A70 - 0x32B50) */
    r6 = M32(sp, 0x204);
    if (r6 <= 0) goto L_00132B50;
    r18 = 0;
    r3 = 0x68;
L_00132A80:
    r4 = D_003931B0;
    r3 = r18 * r3;                                /* mult 3-operand */
    r16 = r4 + r3;
    r2 = M16(r16, 0x2E) ^ 0x63;
    r17 = r16 + 0x4;                              /* delay slot of beqz */
    if (r2 == 0) goto L_00132B40;
    r4 = r19;
    r2 = func_00139708(r4, (f32 *)r17);           /* 0x32AA0 */
    r2 &= 0xFF;
    if (r2 != 0) {
        r2 = r20 ^ r16;                           /* delay slot of beqz */
        r6 = M32(sp, 0x204);
        if (r2 == 0) r21 = r18;                   /* movz $21,$18,$2 */
        goto L_00132B40;
    }
    r5 = r17;
    r2 = func_001396A0(r19, (f32 *)r5);           /* 0x32AC4 (delay $4=$19) */
    r2 &= 0xFF;
    if (r2 != 0) {
        r2 = M32(sp, 0x22C);                      /* delay slot of beqz */
        f0 = MF(r22, 0x0);
        f2 = MF(r16, 0x10);
        f2 = f2 - f0;
        MF(sp, 0x110) = f2;
        f2 = f2 * f2;
        f1 = MF(r22, 0x4);
        f0 = MF(r16, 0x14);
        f0 = f0 - f1;
        f3 = f0 * f0;
        MF(sp, 0x114) = f0;
        f1 = MF(r16, 0x18);
        f0 = MF(r22, 0x8);
        f2 = f2 + f3;
        f1 = f1 - f0;
        MF(sp, 0x118) = f1;                       /* bc1f delay slot: always executes */
        if (f2 >= f24) goto L_00132B3C;
        M32(sp, 0x22C) = r16;
        f24 = f2;                                 /* delay slot mov.s $f24,$f2 */
        r21 = r18;
        goto L_00132B3C;
    }
    /* result == 0 path (L_00132B2C) */
    r2 = M32(sp, 0x22C);                          /* delay slot of beqz */
    if (r2 != 0) goto L_00132B40;
    r6 = M32(sp, 0x204);                          /* delay slot */
    M32(sp, 0x22C) = r16;
    r21 = r18;                                    /* fall-through at 0x32B38 */
    goto L_00132B40;
L_00132B3C:
    r6 = M32(sp, 0x204);
L_00132B40:
    r18 = r18 + 1;
    r2 = r18 < r6;
    r3 = 0x68;                                    /* delay slot */
    if (r2 != 0) goto L_00132A80;
L_00132B50:
    r7 = D_003931B0;
    r8 = r21;
    r5 = M32(sp, 0x210);
    r6 = M32(sp, 0x204);                          /* len from func_00135530 */
    r4 = r19;                                     /* delay slot */
    func_00135CF8(r4, r5, r6, r7, r8);            /* 0x32B5C */
    /* ---- flags & 0x00200040 special case (0x32B64 - 0x32C00) ---------- */
    r4 = M32(r19, 0xC);
    r2 = 0x00200040;
    r3 = r4 & r2;
    if (r3 != r2) goto L_00132C00;
    r3 = M32(sp, 0x22C);                          /* delay slot of bne */
    if (r3 != 0) goto L_00132C00;
    r3 = 1;                                       /* delay slot */
    f1 = MF(r22, 0x0);
    f0 = MF(sp, 0x170);
    r2 = M8(r23, 0x50);
    f1 = f1 + f0;
    f2 = MF(sp, 0x174);
    f3 = MF(sp, 0x178);
    MF(sp, 0x1F0) = f1;
    f0 = MF(r22, 0x4);
    f0 = f0 + f2;
    MF(sp, 0x1F4) = f0;
    f1 = MF(r22, 0x8);
    f1 = f1 + f3;
    MF(sp, 0x1F8) = f1;                           /* delay slot of bne (always) */
    if (r2 != r3) goto L_00132BC4;
    r2 = 2;
    M8(r23, 0x50) = r2;
L_00132BC4:
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x1F0), 12);
    f0 = 1.0f;
    r4 = r19;
    f12 = f23;
    r9 = M32(sp, 0x230);
    MF(r9, 0xC) = f0;                             /* delay slot */
    r2 = func_0013A148(r4, f12);                  /* 0x32BF0 */
    return r2;                                    /* b -> L_00133EFC */
L_00132C00:
    r9 = M32(sp, 0x22C);                          /* beqz delay slot: always executes */
    if (r20 == 0) goto L_0013353C;

    /* ---- r20 (39CE0 result) path: projection weight (0x32C00 - 0x32D40) */
    r4 = r19;
    f25 = func_00139828((u8 *)r4, r20);           /* 0x32C0C (delay $5=$20) */
    f1 = 0.0f;
    r10 = M32(sp, 0x22C);                         /* delay slot of bc1f */
    if (f25 != 0.0f) goto L_00132C3C;             /* bc1f: f25 == 0 sets flag */
    r2 = M32(r19, 0xC) | 0x8000;
    M32(r19, 0xC) = r2;
    r10 = M32(sp, 0x22C);
L_00132C3C:
    if (r10 != 0) { r4 = M32(r19, 0xC); goto L_00132CEC; } /* bnel (delay if taken) */
    r11 = M32(sp, 0x220);                         /* sp+0x30 */
    r3 = 1;
    f0 = MF(r20, 0x30);
    f1 = MF(r11, 0x0);
    r2 = M8(r23, 0x50);
    f1 = f1 + f0;
    MF(sp, 0x110) = f1;
    f1 = MF(r20, 0x34);
    f0 = MF(r11, 0x4);
    f0 = f0 + f1;
    MF(sp, 0x114) = f0;
    f1 = MF(r11, 0x8);
    f0 = MF(r20, 0x38);
    f1 = f1 + f0;
    MF(sp, 0x118) = f1;
    if (r2 != r3) goto L_00132C8C;                /* bne; delay swc1 */
    r2 = 2;
    M8(r23, 0x50) = r2;
L_00132C8C:
    MF(sp, 0x118) = f1;                           /* delay slot of bne */
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x110), 12);
    f0 = 1.0f;
    r7 = M32(sp, 0x230);
    MF(r7, 0xC) = f0;                             /* delay slot of bne */
    r2 = D_003931A4;
    if (r2 != 1) goto L_00133BE8;
    r4 = M32(r19, 0xC);
    r2 = r4 & 0x4000;
    f12 = f23;
    if (r2 == 0) goto L_00133BEC;                 /* beqz; delay $f12 = fArg */
    r3 = M32(r19, 0x148);
    r2 = M32(r3, 0x0) & 0x4;
    if (r2 != 0) goto L_00133BEC;                 /* bnez */
    r2 = r4 | 0x00800000;
    M32(r19, 0xC) = r2;                           /* delay slot of b */
    goto L_00133BEC;
L_00132CEC:
    r2 = r4 & 0x00800000;
    if (r2 == 0) goto L_00132D34;                 /* beqz; delay andi $2,$4,0x4000 */
    r2 = r4 & 0x4000;
    if (r2 == 0) { r2 = 0x02000000; goto L_00132D38; } /* beqz + delay lui at 0x32D00 */
    r3 = M32(r19, 0x148);
    r2 = M32(r3, 0x0) & 0x4;                       /* event->head & 4 */
    if (r2 != 0) { r2 = 0x02000000; goto L_00132D38; } /* bnez + delay lui at 0x32D14 */
    r2 = (s32)0xFF7FFFFF;                         /* -FLT_MAX */
    r8 = 1;
    M32(sp, 0x228) = 1;
    r2 = r4 & r2;
    M32(r19, 0xC) = r2;
    r4 = r2;
L_00132D34:
    r2 = 0x02000000;                              /* 0x32D34: lui 0x2000000>>16 */
L_00132D38:
    r3 = r4 | r2;
    M32(r19, 0xC) = r3;
    /* ---- decide whether to re-query (0x32D40 - 0x32D94) --------------- */
    r9 = M32(sp, 0x22C);
    r2 = r3 & 0x10;
    if (r9 == 0) goto L_0013334C;                 /* beqz; delay andi $2,$3,0x10 */
    r11 = M32(sp, 0x22C);                          /* bnez delay slot at 0x32D50: always */
    if (r2 != 0) goto L_00132D74;                 /* 0x32D4C */
    r2 = r3 & 0x4;
    r10 = M32(sp, 0x228);
    if (r2 == 0) goto L_00132D6C;                 /* beqz; delay lw $10,0x228 */
    r2 = M32(r19, 0x27C);
    if (r2 != 0) goto L_00132D74;
L_00132D6C:
    r11 = M32(sp, 0x22C);
    if (r10 == 0) goto L_0013334C;                /* beqz; delay lw $11,0x22C */
L_00132D74:
    f20 = 0.0f;
    r2 = (MF(r11, 0x0) < 0.0f) ? 1 : 0;           /* c.lt.s $f0,$f20 ; bc1t */
    r11 = M32(sp, 0x21C);
    if (r2 == 0) goto L_00133350;                 /* beqz; delay lw $11,0x21C */
    if (f25 != 0.0f) goto L_00133350;             /* bc1f: f25 != 0 */
    r2 = M32(sp, 0x208);                          /* delay slot */

    /* ---- two func_00136C08 passes + near-vector fixup (0x32DA8 - 0x32F04) */
    r7 = 0x68;
    r6 = M32(sp, 0x204);
    r4 = (s32)sp + 0x190;
    r7 = r2 * r7;                                 /* count * 0x68 */
    r3 = D_003931B0;
    r6 = r6 - r2;                                 /* len - count */
    r5 = r19;
    r8 = r6;
    r7 = r3 + r7;                                 /* delay slot */
    r2x = func_00136C08(r4, r5, r6, r7, r8);      /* 0x32DC8 */
    r7 = M32(sp, 0x208);
    M32(sp, 0x224) = r2x;                         /* delay slot of beqz */
    if (r7 == 0) goto L_00132E0C;
    r6 = r7;
    r4 = (s32)sp + 0x1A0;
    r7 = D_003931B0;
    r5 = r19;
    r8 = r6;                                      /* delay slot */
    r2y = func_00136C08(r4, r5, r6, r7, r8);      /* 0x32DEC */
    r3 = M32(sp, 0x224);
    r7 = M32(sp, 0x208);
    r2 = (r3 | r2y) & 0xFF;
    M32(sp, 0x224) = r2;                          /* delay slot of b */
    goto L_00132E18;
L_00132E0C:
    MF(sp, 0x1A8) = 0.0f;
    MF(sp, 0x1A0) = 0.0f;
    MF(sp, 0x1A4) = 0.0f;
L_00132E18:
    f11 = MF(sp, 0x190);                          /* delay slot of beqz */
    if (r7 == 0) goto L_00132EF0;
    r2 = 0;
    f1 = 0.0f;
    f2 = D_003926EC;
    f3 = f11;
    f0 = fabsf(f11 - f1);
    if (!(f0 < f2)) { f10 = MF(sp, 0x194); goto L_00132E80; } /* bc1f */
    f10 = MF(sp, 0x194);
    f0 = fabsf(f10 - f1);
    if (!(f0 < f2)) { f8 = MF(sp, 0x198); goto L_00132E80; } /* bc1f */
    f8 = MF(sp, 0x198);
    f0 = fabsf(f8 - f1);
    if (f0 < f2) { r2 = 1; goto L_00132E84; }     /* bc1tl */
    goto L_00132E84;
L_00132E80:
    f8 = MF(sp, 0x198);
L_00132E84:
    f5 = MF(sp, 0x1A0);                           /* delay slot of bnez */
    if (r2 != 0) goto L_00132EFC;
    r2 = 0;
    f1 = 0.0f;
    f2 = D_003926F0;
    f0 = fabsf(f5 - f1);
    if (!(f0 < f2)) { f0 = MF(sp, 0x1A4); goto L_00132EE0; } /* bc1f */
    f0 = MF(sp, 0x1A4);
    f0 = fabsf(f0 - f1);
    if (!(f0 < f2)) { f0 = MF(sp, 0x1A8); goto L_00132EE0; } /* bc1f */
    f0 = MF(sp, 0x1A8);
    f0 = fabsf(f0 - f1);
    if (f0 < f2) r2 = 1;                          /* bc1tl */
L_00132EE0:
    f0 = MF(sp, 0x1A4);                           /* delay slot of beqz */
    if (r2 == 0) goto L_00132F20;
    f2 = f11 + f5;                                /* delay slot of b */
    goto L_00132F04;
L_00132EF0:
    f10 = MF(sp, 0x194);
    f8 = MF(sp, 0x198);
    f5 = MF(sp, 0x1A0);
L_00132EFC:
    f0 = MF(sp, 0x1A4);
    f2 = f11 + f5;
L_00132F04:
    f1 = MF(sp, 0x1A8);
    f0 = f10 + f0;
    f1 = f8 + f1;
    MF(sp, 0x190) = f2;
    MF(sp, 0x194) = f0;
    MF(sp, 0x198) = f1;                           /* delay slot of b */
    goto L_00133000;
L_00132F20:
    f0 = F_SUM3(f3, f10, f8);                     /* 0x32F20 mula/madda/madd */
    f1 = 0.0f;
    f2 = F_SQRT(f0);                              /* 0x32F30 sqrt.s $f2,$f0 -> vector length */
    if (f2 == 0.0f) goto L_00132F70;              /* c.eq.s $f2,$f1 ; bc1t */
    f0 = 1.0f / f2;
    f1 = f0 * f8;
    f2 = f0 * f11;
    f0 = f0 * f10;
    MF(sp, 0x118) = f1;
    MF(sp, 0x110) = f2;
    MF(sp, 0x114) = f0;                           /* delay slot of b */
    goto L_00132F98;
L_00132F70:
    COPY_N((void *)(sp + 0x110), (void *)(sp + 0x190), 12);
    f11 = MF(sp, 0x190);
    f10 = MF(sp, 0x194);
    f8 = MF(sp, 0x198);
    f5 = MF(sp, 0x1A0);
L_00132F98:
    /* project sp+0x1A0 vector off sp+0x110 vector (0x32F98 - 0x32FFC) */
    f1 = MF(sp, 0x110);
    f4 = MF(sp, 0x1A4);
    f2 = MF(sp, 0x114);
    f3 = MF(sp, 0x1A8);
    f0 = MF(sp, 0x118);
    f7 = F_DOT3(f5, f1, f4, f2, f3, f0);          /* 0x32FAC */
    f0 = f7 * f0;
    f1 = f7 * f1;
    f2 = f7 * f2;
    f3 = f3 - f0;
    MF(sp, 0x118) = f0;
    f5 = f5 - f1;
    MF(sp, 0x110) = f1;
    f4 = f4 - f2;
    MF(sp, 0x114) = f2;
    f6 = f8 + f3;
    MF(sp, 0x1A8) = f3;
    f1 = f11 + f5;
    MF(sp, 0x1A0) = f5;
    f0 = f10 + f4;
    MF(sp, 0x1A4) = f4;
    MF(sp, 0x198) = f6;
    MF(sp, 0x190) = f1;
    MF(sp, 0x194) = f0;
L_00133000:

    /* ---- flags & 0x00100010 gate, then projection (0x33000 - 0x330B4) -- */
    r3 = M32(r19, 0xC) & 0x00100010;
    r10 = M32(sp, 0x220);                         /* delay slot of bnez */
    if (r3 != 0) goto L_001332D0;
    f1 = MF(r20, 0x4);
    f0 = MF(r20, 0xC);
    f5 = MF(r20, 0x8);
    f2 = MF(sp, 0x190);
    f3 = MF(sp, 0x194);
    f4 = MF(sp, 0x198);
    f7 = F_DOT3(f2, f1, f3, f5, f4, f0);          /* 0x33030 */
    r8 = M32(sp, 0x22C);
    f1 = f7 * f1;
    f6 = 0.5f;
    MF(sp, 0x110) = f1;
    f2 = f2 - f1;
    f0 = MF(r20, 0x8);
    f0 = f7 * f0;
    MF(sp, 0x114) = f0;
    f3 = f3 - f0;
    f1 = MF(r20, 0xC);
    MF(sp, 0x190) = f2;
    f1 = f7 * f1;
    MF(sp, 0x194) = f3;
    f4 = f4 - f1;
    MF(sp, 0x118) = f1;
    MF(sp, 0x198) = f4;
    f0 = MF(r20, 0x30);
    f2 = f2 + f0;
    MF(sp, 0x190) = f2;
    f0 = MF(r20, 0x34);
    f3 = f3 + f0;
    MF(sp, 0x194) = f3;
    f0 = MF(r20, 0x38);
    f4 = f4 + f0;
    MF(sp, 0x198) = f4;
    f0 = MF(r8, 0xC);
    if (!(0.5f < f0)) goto L_001332D0;            /* bc1f; delay lw $10,0x220 */
    /* ---- angle block: atan/tan (0x330B8 - 0x33294) -------------------- */
    r10 = M32(sp, 0x220);                         /* delay slot */
    f1 = MF(r20, 0x10);
    f0 = MF(r8, 0x10);
    f0 = f0 - f1;
    MF(sp, 0x1F0) = f0;
    f0 = MF(r20, 0x14);
    f1 = MF(r8, 0x14);
    f1 = f1 - f0;
    MF(sp, 0x1F4) = f1;
    f1 = MF(r20, 0x18);
    f0 = MF(r8, 0x18);
    f0 = f0 - f1;
    MF(sp, 0x1F8) = f0;
    f4 = MF(r8, 0x4);
    f3 = MF(r20, 0x4);
    f2 = MF(r8, 0x8);
    f1 = MF(r20, 0x8);
    f0 = MF(r8, 0xC);
    f12 = MF(r20, 0xC);
    f12 = F_DOT3(f4, f3, f2, f1, f0, f12);        /* 0x33100 */
    f0 = func_00217E28(f12);                      /* atanf */
    f12 = f0;                                     /* 0x33114: mov.s $f12,$f0 */
    f20 = MF(sp, 0x1F0);
    f4 = MF(r20, 0x4);
    f3 = MF(sp, 0x1F4);
    f2 = MF(r20, 0x8);
    f1 = MF(sp, 0x1F8);
    f0 = MF(r20, 0xC);
    f20 = F_DOT3(f20, f4, f3, f2, f1, f0);        /* 0x33130 */
    f0 = func_00217DA0(f12);                      /* tanf(atan result) */
    r9 = M32(sp, 0x22C);
    f20 = f20 / f0;                               /* 0x33150 */
    f4 = MF(r20, 0x4);
    r2 = 0;
    f3 = MF(r9, 0x8);
    f2 = MF(r20, 0x8);
    f1 = MF(r9, 0xC);
    f0 = MF(r20, 0xC);
    f5 = MF(r9, 0x4);
    f7 = F_DOT3(f5, f4, f3, f2, f1, f0);          /* 0x33170 */
    f6 = 0.0f;
    f4 = f7 * f4;
    f8 = D_003926F4;
    MF(sp, 0x110) = f4;
    f2 = f7 * f2;
    MF(sp, 0x114) = f2;
    /* Retail reloads $f3 from $20+0xC at 0x33198 before mul.s at 0x3319C;
     * f3 was previously r9+0x8 (dot-product input) and must NOT be reused
     * here -- the z-component of the projected vector comes from r20, not r9. */
    f3 = f7 * MF(r20, 0xC);
    MF(sp, 0x118) = f3;
    f0 = MF(r9, 0x4);
    f0 = f0 - f4;
    MF(sp, 0x110) = f0;
    f0 = f0 - f6;
    f1 = MF(r9, 0x8);
    f0 = fabsf(f0);
    /* Retail 0x331BC sets the condition here, then executes 0x331C0-0x331CC
     * and the bc1f delay slot 0x331D4 unconditionally before branching at
     * 0x331D0.  Capture the compare result first; all four ops below must run
     * regardless of whether the branch is taken (they are NOT conditional). */
    {
        s32 comp1 = (f0 < f8);                    /* c.lt.s at 0x331BC */
        f1 = f1 - f2;                             /* 0x331C0 */
        MF(sp, 0x114) = f1;                       /* 0x331C4 */
        f0 = MF(r9, 0xC);                         /* 0x331C8 */
        f2 = f0 - f3;                             /* 0x331CC */
        MF(sp, 0x118) = f2;                       /* 0x331D4 delay slot: always */
        if (!comp1) goto L_00133204;              /* bc1f at 0x331D0 */
    }
    if (!(fabsf(f1 - f6) < f8)) goto L_00133204;  /* bc1f */
    if (fabsf(f2 - f6) < f8) { r2 = 1; goto L_00133204; } /* bc1tl */
    goto L_00133204;
L_00133204:
    f6 = MF(sp, 0x110);
    if (r2 != 0) goto L_00133274;                 /* bnez; delay f6 = M(sp+0x110) */
    f4 = MF(sp, 0x114);
    f3 = MF(sp, 0x118);
    f0 = F_SUM3(f6, f4, f3);                      /* 0x33214: f0 = |v|^2 */
    f1 = 0.0f;
    /* Retail 0x33224 "c1 0x84" (word 84000046) is sqrt.s $f2,$f0 -- the
     * vector length, NOT a no-op (same encoding as the handled 0x32F30).
     * f2 must be replaced by sqrt(f0) before the c.eq.s at 0x33228 and the
     * 1.0/f2 division at 0x33248. */
    f2 = F_SQRT(f0);                              /* 0x33224: sqrt.s $f2,$f0 */
    if (f2 == 0.0f) { f5 = f20 * f3; goto L_00133294; } /* c.eq.s $f2,$f1 ; bc1tl (delay if taken) */
    f0 = 1.0f / f2;
    f1 = f0 * f3;
    f2 = f0 * f6;
    f0 = f0 * f4;
    f3 = f1;
    MF(sp, 0x118) = f1;
    f6 = f2;
    MF(sp, 0x110) = f2;
    f4 = f0;
    MF(sp, 0x114) = f0;                           /* delay slot of b */
    goto L_00133290;
L_00133274:
    f0 = 0.0f;
    f4 = f0;
    MF(sp, 0x110) = f0;
    MF(sp, 0x114) = f0;
    f6 = f4;
    MF(sp, 0x118) = f0;
    f3 = f4;
L_00133290:
    f5 = f20 * f3;                                /* 0x33290 */
L_00133294:
    f2 = MF(sp, 0x190);
    f3 = f20 * f6;
    f1 = MF(sp, 0x194);
    f4 = f20 * f4;
    f0 = MF(sp, 0x198);
    MF(sp, 0x118) = f5;
    f2 = f2 + f3;
    MF(sp, 0x110) = f3;
    f1 = f1 + f4;
    MF(sp, 0x114) = f4;
    f0 = f0 + f5;
    MF(sp, 0x190) = f2;
    MF(sp, 0x194) = f1;
    MF(sp, 0x198) = f0;
    r10 = M32(sp, 0x220);                         /* 0x332CC */
L_001332D0:

    /* ---- compose sp+0x1D0 offset and re-store (0x332D0 - 0x333C4) ----- */
    f0 = MF(sp, 0x190);
    f3 = MF(r10, 0x0);
    f2 = MF(sp, 0x194);
    f3 = f3 + f0;
    f7 = MF(sp, 0x198);
    f4 = MF(sp, 0x140);
    f5 = MF(sp, 0x144);
    MF(sp, 0x1D0) = f3;
    f6 = MF(sp, 0x148);
    f1 = MF(r10, 0x4);
    f1 = f1 + f2;
    MF(sp, 0x1D4) = f1;
    f2 = MF(r10, 0x8);
    f2 = f2 + f7;
    MF(sp, 0x1D8) = f2;
    f0 = MF(r22, 0x0);
    f3 = f3 - f0;
    MF(sp, 0x110) = f3;
    f4 = f4 - f3;
    f0 = MF(r22, 0x4);
    f1 = f1 - f0;
    MF(sp, 0x114) = f1;
    f5 = f5 - f1;
    f0 = MF(r22, 0x8);
    MF(sp, 0x140) = f4;
    f2 = f2 - f0;
    MF(sp, 0x144) = f5;
    f6 = f6 - f2;
    MF(sp, 0x118) = f2;
    MF(sp, 0x148) = f6;                           /* delay slot of b */
    goto L_00133368;
L_0013334C:
    r11 = M32(sp, 0x21C);
L_00133350:
    COPY_N((void *)(sp + 0x1D0), (void *)r11, 12);
L_00133368:
    {
        s32 tmp = M8(r23, 0x50);
        r2 = 2;                                   /* delay slot of bne */
        if (tmp == 1) M8(r23, 0x50) = r2;
    }
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x1D0), 12);
    r7 = r30;
    f0 = 1.0f;
    r4 = (s32)sp + 0x1C0;
    r11 = M32(sp, 0x22C);
    r5 = r19;
    r2 = M32(sp, 0x230);
    r6 = (s32)sp + 0x140;
    r8 = r11 + 0x4;
    f12 = f23;
    MF(r2, 0xC) = f0;                             /* delay slot */
    r9 = 0;
    func_0012F180(r4, r5, r6, r7, r8, r9, f23);   /* 0x333C0: $4-$9 + $f12 = fArg */
    /* ---- near-vector recursive check (0x333C8 - 0x33528) ------------- */
    r3 = M32(sp, 0x224);
    r2 = M32(r19, 0xC) & 0x4;
    if (r3 == 0) goto L_001334C4;                 /* beqz; delay lw $5,D_003931B0 */
    f4 = MF(sp, 0x1C0);                           /* delay slot of beqz */
    if (r2 == 0) goto L_001333F4;
    r2 = M32(r19, 0x27C);
    if (r2 != 0) { r6 = r19; goto L_001334C8; }   /* bnez; delay $6 = arg1 */
    f4 = MF(sp, 0x1C0);
L_001333F4:
    r2 = 0;
    f7 = 0.0f;
    f2 = MF(sp, 0x140);
    f1 = f4 - f7;
    f3 = MF(sp, 0x144);
    f2 = f2 - f4;
    f5 = MF(sp, 0x1C4);
    f0 = MF(sp, 0x148);
    f1 = fabsf(f1);
    f6 = MF(sp, 0x1C8);
    f3 = f3 - f5;
    f4 = D_003926F8;
    f0 = f0 - f6;
    MF(sp, 0x110) = f2;
    MF(sp, 0x114) = f3;
    MF(sp, 0x118) = f0;
    if (!(f1 < f4)) goto L_00133468;              /* bc1f; delay swc1 */
    f0 = f5 - f7;
    f0 = fabsf(f0);
    if (!(f0 < f4)) goto L_00133468;              /* bc1f */
    f0 = f6 - f7;
    f0 = fabsf(f0);
    if (f0 < f4) { r2 = 1; goto L_00133468; }     /* bc1tl */
    goto L_00133468;
L_00133468:
    r3 = M8(r23, 0x50);                           /* delay slot of bnel */
    if (r2 != 0) goto L_00133488;
    r4 = r19;
    r5 = M32(sp, 0x210);
    func_00131D78(r4, r5, f23);                   /* 0x33478: recursive call */
    r4 = r19;                                     /* delay slot of b */
    goto L_0013352C;
L_00133488:
    r2 = 1;
    if (r3 != r2) goto L_00133498;
    r2 = 2;                                       /* delay slot of bne */
    M8(r23, 0x50) = r2;
L_00133498:
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x1D0), 12);
    f0 = 1.0f;
    r8 = M32(sp, 0x230);
    MF(r8, 0xC) = f0;                             /* delay slot of b */
    goto L_00133528;
L_001334C4:
    r6 = r19;
L_001334C8:
    r4 = M32(sp, 0x234);
    r8 = r23;
    r7 = M32(sp, 0x210);
    r9 = r23;
    r10 = 1;
    r2 = func_00135530(r4, D_003931B0, r6, r7, r8, r9, r10); /* 0x334D8 */
    r4 = M32(sp, 0x204);                          /* delay slot */
    if (r2 == 0) goto L_00133528;
    r6 = r19;
    func_0013AD90(r4, D_003931B0, r6);            /* delay $5 */
    func_0013B258(r4, D_003931B0);                /* delay $5 */
    r6 = M32(sp, 0x204);
    r16 = (s32)sp + 0x190;
    r7 = D_003931B0;
    r4 = r16;
    r5 = r19;
    r8 = r6;                                      /* delay slot */
    func_00136C08(r4, r5, r6, r7, r8);            /* 0x33514 */
    r4 = r23;
    r5 = r16;                                     /* delay slot */
    func_001241D8(r4, r5);                        /* 0x33520 */
L_00133528:
    r4 = r19;
L_0013352C:
    r2 = func_0013A148(r4, f23);                  /* 0x3352C (delay $f12) */
    return r2;                                    /* b -> L_00133EFC */

    /* ---- r20 == 0 path: two 36C08 passes + near tests (0x3353C - 0x337B8) */
L_0013353C:
    r2 = M32(sp, 0x208);
    if (r9 == 0) goto L_00133950;                 /* beqz; delay lw $2,0x208 */
    r7 = 0x68;
    r6 = M32(sp, 0x204);
    r4 = (s32)sp + 0x190;
    r7 = r2 * r7;
    r3 = D_003931B0;
    r6 = r6 - r2;
    r5 = r19;
    r8 = r6;
    r7 = r3 + r7;                                 /* delay slot */
    r2x = func_00136C08(r4, r5, r6, r7, r8);      /* 0x33564 */
    r7 = M32(sp, 0x208);
    r6 = r7;
    if (r7 == 0) goto L_00133594;                 /* beqz; delay $6 = r7 */
    r4 = (s32)sp + 0x1A0;
    r7 = D_003931B0;
    r5 = r19;
    r8 = r6;                                      /* delay slot */
    r2y = func_00136C08(r4, r5, r6, r7, r8);      /* 0x33584 */
    r7 = M32(sp, 0x208);                          /* delay slot of b */
    goto L_001335A4;
L_00133594:
    MF(sp, 0x1A8) = 0.0f;
    MF(sp, 0x1A0) = 0.0f;
    MF(sp, 0x1A4) = 0.0f;
L_001335A4:
    f11 = MF(sp, 0x190);
    if (r7 == 0) goto L_0013367C;                 /* beqz; delay $f11 */
    r2 = 0;
    f1 = 0.0f;
    f2 = D_003926FC;
    f0 = fabsf(f11 - f1);
    if (!(f0 < f2)) goto L_00133600;              /* bc1f; delay $f0 */
    f0 = MF(sp, 0x194);
    f0 = fabsf(f0 - f1);
    if (!(f0 < f2)) goto L_00133600;              /* bc1f; delay $f0 */
    f0 = MF(sp, 0x198);
    f0 = fabsf(f0 - f1);
    if (f0 < f2) { r2 = 1; goto L_00133600; }     /* bc1tl */
    goto L_00133600;
L_00133600:
    f5 = MF(sp, 0x1A0);                           /* delay slot of bnez */
    if (r2 != 0) goto L_00133680;
    r2 = 0;
    f1 = 0.0f;
    f2 = D_00392700;
    f3 = f5;
    f0 = fabsf(f5 - f1);
    f9 = MF(sp, 0x1A4);
    if (!(f0 < f2)) goto L_00133668;              /* bc1f; delay $f9 */
    f0 = fabsf(f9 - f1);
    f6 = MF(sp, 0x1A8);
    if (!(f0 < f2)) goto L_00133668;              /* bc1f; delay $f6 */
    f0 = fabsf(f6 - f1);
    if (f0 < f2) { r2 = 1; goto L_0013366C; }     /* bc1tl */
    goto L_0013366C;
L_00133668:
    f6 = MF(sp, 0x1A8);
L_0013366C:
    f0 = MF(sp, 0x194);                           /* delay slot of beqz */
    if (r2 == 0) goto L_001336B8;
    f2 = f11 + f5;                                /* delay slot of b */
    goto L_00133690;
L_0013367C:
    f5 = MF(sp, 0x1A0);
L_00133680:
    f9 = MF(sp, 0x1A4);
    f6 = MF(sp, 0x1A8);
    f0 = MF(sp, 0x194);
    f2 = f11 + f5;
L_00133690:
    f1 = MF(sp, 0x198);
    f0 = f0 + f9;
    f1 = f1 + f6;
    f11 = f2;
    MF(sp, 0x190) = f2;
    f10 = f0;
    MF(sp, 0x194) = f0;
    f8 = f1;
    MF(sp, 0x198) = f1;                           /* delay slot of b */
    goto L_001337B8;
L_001336B8:
    f0 = F_SUM3(f3, f9, f6);                      /* 0x336B8: f0 = |v|^2 */
    f1 = 0.0f;
    /* Retail 0x336C8 "c1 0x84" (84000046) is sqrt.s $f2,$f0 -> vector length
     * (same encoding as 0x32F30 / 0x33224).  Stale f2 must be replaced. */
    f2 = F_SQRT(f0);                              /* 0x336C8: sqrt.s $f2,$f0 */
    if (f2 == 0.0f) goto L_00133708;              /* c.eq.s $f2,$f1 ; bc1t */
    f0 = 1.0f / f2;
    f1 = f0 * f6;
    f2 = f0 * f5;
    f0 = f0 * f9;
    MF(sp, 0x118) = f1;
    MF(sp, 0x110) = f2;
    MF(sp, 0x114) = f0;                           /* delay slot of b */
    goto L_00133730;
L_00133708:
    COPY_N((void *)(sp + 0x110), (void *)(sp + 0x1A0), 12);
    f11 = MF(sp, 0x190);
    f5 = MF(sp, 0x1A0);
    f9 = MF(sp, 0x1A4);
    f6 = MF(sp, 0x1A8);
L_00133730:
    f3 = MF(sp, 0x110);
    f0 = F_SUM3(f5, f9, f6);                      /* 0x33734: f0 = |v|^2 */
    f10 = MF(sp, 0x194);
    /* Retail 0x33744 "c1 0x4" (04000046) is sqrt.s $f0,$f0 -> |v| */
    f0 = F_SQRT(f0);                              /* 0x33744: sqrt.s $f0,$f0 */
    f1 = MF(sp, 0x114);
    f8 = MF(sp, 0x198);
    f2 = MF(sp, 0x118);
    f7 = F_DOT3(f11, f3, f10, f1, f8, f2);        /* 0x33754 */
    r4 = M32(sp, 0x220);
    if (!(f7 < f0)) goto L_001337BC;              /* bc1f; delay $f4 */
    f2 = f7 * f2;
    f4 = f7 * f1;
    f3 = f7 * f3;
    f0 = f8 - f2;
    MF(sp, 0x118) = f2;
    f2 = f10 - f4;
    MF(sp, 0x114) = f4;
    f1 = f11 - f3;
    MF(sp, 0x110) = f3;
    f0 = f0 + f6;
    MF(sp, 0x198) = f0;
    f8 = f0;
    f2 = f2 + f9;
    MF(sp, 0x194) = f2;
    f10 = f2;
    f1 = f1 + f5;
    MF(sp, 0x190) = f1;
    f11 = f1;
    goto L_001337BC;
L_001337B8:
    r4 = M32(sp, 0x220);
L_001337BC:
    r3 = 0x02000000;                              /* 0x337BC: lui 0x2000000>>16 */
    r2 = M32(r19, 0xC);
    f3 = MF(r4, 0x0);                             /* r4 = sp+0x30 */
    r5 = M32(sp, 0x220);                          /* sp+0x30 */
    r2 = r2 | r3;
    f3 = f3 + f11;
    f4 = MF(sp, 0x140);
    f5 = MF(sp, 0x144);
    r4 = 1;
    f6 = MF(sp, 0x148);
    MF(sp, 0x1D0) = f3;
    f1 = MF(r5, 0x4);
    f1 = f1 + f10;
    MF(sp, 0x1D4) = f1;
    f2 = MF(r5, 0x8);
    f2 = f2 + f8;
    MF(sp, 0x1D8) = f2;
    f0 = MF(r22, 0x0);
    f3 = f3 - f0;
    MF(sp, 0x110) = f3;
    f4 = f4 - f3;
    f0 = MF(r22, 0x4);
    f1 = f1 - f0;
    MF(sp, 0x114) = f1;
    f5 = f5 - f1;
    f0 = MF(r22, 0x8);
    M32(r19, 0xC) = r2;                           /* 0x33824: flags |= 0x200000 */
    f2 = f2 - f0;
    MF(sp, 0x140) = f4;
    r2 = M8(r23, 0x50);
    f6 = f6 - f2;
    MF(sp, 0x144) = f5;
    MF(sp, 0x118) = f2;
    MF(sp, 0x148) = f6;                           /* delay slot of bne */
    {
        s32 tmp = r2;
        if (tmp == r4) { r2 = 2; M8(r23, 0x50) = r2; }
    }
L_00133850:
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x1D0), 12);
    r7 = r30;
    f0 = 1.0f;
    r4 = (s32)sp + 0x1C0;
    r11 = M32(sp, 0x22C);
    r5 = r19;
    r2 = M32(sp, 0x230);
    r6 = (s32)sp + 0x140;
    r8 = r11 + 0x4;
    f12 = f23;
    MF(r2, 0xC) = f0;                             /* delay slot */
    r9 = 0;
    func_0012F180(r4, r5, r6, r7, r8, r9, f23);   /* 0x33894: $4-$9 + $f12 = fArg */
    /* ---- near check + second recursive call (0x3389C - 0x3394C) ------- */
    f1 = 0.0f;
    r2 = 0;
    f0 = MF(sp, 0x1C0);
    f2 = D_00392704;
    f0 = fabsf(f0 - f1);
    if (!(f0 < f2)) goto L_001338F4;              /* bc1f; delay $f0 */
    f0 = MF(sp, 0x1C4);
    f0 = fabsf(f0 - f1);
    if (!(f0 < f2)) goto L_001338F4;              /* bc1f; delay $f0 */
    f0 = MF(sp, 0x1C8);
    f0 = fabsf(f0 - f1);
    if (f0 < f2) { r2 = 1; goto L_001338F4; }     /* bc1tl */
    goto L_001338F4;
L_001338F4:
    r3 = M8(r23, 0x50);                           /* delay slot of bnel */
    if (r2 != 0) goto L_00133914;
    r4 = r19;
    r5 = M32(sp, 0x210);
    func_00131D78(r4, r5, f23);                   /* 0x33904: recursive call */
    goto L_00133BEC;                              /* delay $f12 = f23 */
L_00133914:
    r2 = 1;
    if (r3 != r2) goto L_00133924;
    r2 = 2;                                       /* delay slot of bne */
    M8(r23, 0x50) = r2;
L_00133924:
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x1D0), 12);
    f0 = 1.0f;
    r7 = M32(sp, 0x230);
    MF(r7, 0xC) = f0;                             /* delay slot of b */
    goto L_00133BE8;
    /* ---- final velocity test / state restore (0x33950 - 0x33EFC) ------- */
L_00133950:
    f0 = MF(sp, 0x160);
    r2 = M32(r19, 0xC) | 0x02000000;              /* 0x33954/0x3395C */
    f1 = MF(sp, 0x164);
    f3 = MF(sp, 0x168);
    f0 = F_SUM3(f0, f1, f3);                      /* 0x33964: f0 = |reaction|^2 */
    f2 = D_00392708;
    /* Retail 0x33974 "c1 0x4" (04000046) is sqrt.s $f0,$f0 -> |reaction| */
    f0 = F_SQRT(f0);                              /* 0x33974: sqrt.s $f0,$f0 */
    M32(r19, 0xC) = r2;                           /* 0x33984: bc1f delay sw (always) */
    if (!(f2 < f0)) goto L_00133A6C;              /* c.lt.s $f2,$f0 ; bc1f */
    f0 = 0.0f;
    if (!(f3 < f0)) goto L_00133A6C;              /* bc1f; delay $5=$23 */
    COPY_N((void *)r23, (void *)M32(sp, 0x214), 0x54);  /* 0x339AC/0x33A10/0x33A44 */
L_00133A6C:
    r2 = M32(r19, 0xC) & 0x00200000;
    r3 = 1;
    if (r2 == 0) goto L_00133AE8;                 /* beqz; delay $3 = 1 */
    f1 = MF(r22, 0x0);
    f0 = MF(sp, 0x170);
    r2 = M8(r23, 0x50);
    f1 = f1 + f0;
    f2 = MF(sp, 0x174);
    f3 = MF(sp, 0x178);
    MF(sp, 0x1F0) = f1;
    f0 = MF(r22, 0x4);
    f0 = f0 + f2;
    MF(sp, 0x1F4) = f0;
    f1 = MF(r22, 0x8);
    f1 = f1 + f3;
    {
        s32 tmp = r2;
        MF(sp, 0x1F8) = f1;                       /* delay slot of bne */
        if (tmp == r3) { r2 = 2; M8(r23, 0x50) = r2; }
    }
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x1F0), 12);
    f0 = 1.0f;
    r9 = M32(sp, 0x230);
    MF(r9, 0xC) = f0;                             /* 0x33AE4 */
L_00133AE8:
    r2 = D_003931A4 < 2;
    r3 = M32(r19, 0xC);                           /* delay slot of bnel */
    if (r2 != 0) goto L_00133BD8;
    r10 = M32(sp, 0x218);
    r4 = r23;
    COPY_N((void *)r4, (void *)r10, 0x54);        /* 0x33B14/0x33B78/0x33BAC */
    r3 = M32(r19, 0xC);                           /* 0x33BD4 */
L_00133BD8:
    r3 = r3 & (s32)0xFFFEFFFF;
    M32(r19, 0xC) = r3;
L_00133BE8:
    f12 = f23;
L_00133BEC:
    r2 = func_0013A148(r19, f12);                 /* 0x33BEC (delay $4=arg1) */
    /* ---- final velocity check (0x33BF4 - 0x33C60) --------------------- */
    f0 = MF(r19, 0x3C);
    f3 = MF(r19, 0x40);
    f1 = MF(r19, 0x44);
    f2 = D_0039270C;
    f0 = F_SUM3(f0, f3, f1);                      /* 0x33C04 */
    if (!(f0 < f2)) goto L_00133EFC;              /* bc1f */
    f0 = 0.0f;
    r10 = M32(sp, 0x218);
    if (f25 != 0.0f) goto L_00133EFC;             /* c.eq.s $f25,$f0 ; bc1f; delay lw #10 */
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)((u8 *)r10 + 0x30), 16); /* 0x33C34 */
L_00133C54:
    MF(r19, 0x44) = 0.0f;                         /* 0x33C54 */
    MF(r19, 0x3C) = 0.0f;
    MF(r19, 0x40) = 0.0f;                         /* delay slot of b */
    goto L_00133EF8;
L_00133C64:
    /* ---- flags 0x2000000 + state restore (0x33C64 - 0x33D7C) --------- */
    f0 = MF(sp, 0x160);
    r3 = 0x02000000;                              /* 0x33C68: lui 0x2000000>>16 */
    f1 = MF(sp, 0x164);
    f3 = MF(sp, 0x168);
    f0 = F_SUM3(f0, f1, f3);                      /* 0x33C74: f0 = |reaction|^2 */
    f2 = D_00392710;
    /* Retail 0x33C88 "c1 0x4" (04000046) is sqrt.s $f0,$f0 -> |reaction| */
    f0 = F_SQRT(f0);                              /* 0x33C88: sqrt.s $f0,$f0 */
    r2 = M32(r19, 0xC) | r3;
    M32(r19, 0xC) = r2;                           /* 0x33C98: bc1f delay sw (always) */
    if (!(f2 < f0)) goto L_00133D7C;              /* c.lt.s $f2,$f0 ; bc1f */
    f0 = 0.0f;
    r4 = M32(sp, 0x214);
    if (!(f3 < f0)) goto L_00133D7C;              /* bc1f; delay $4=sp */
    COPY_N((void *)r23, (void *)r4, 0x54);        /* 0x33CC0/0x33D20/0x33D54 */
L_00133D7C:
    /* ---- optional ref-vector push (0x33D7C - 0x33DF8) ---------------- */
    r2 = M32(r19, 0xC) & 0x00200000;
    r3 = 1;
    if (r2 == 0) goto L_00133DF8;                 /* beqz; delay $3 = 1 */
    f1 = MF(r22, 0x0);
    f0 = MF(sp, 0x170);
    r2 = M8(r23, 0x50);
    f1 = f1 + f0;
    f2 = MF(sp, 0x174);
    f3 = MF(sp, 0x178);
    MF(sp, 0x1F0) = f1;
    f0 = MF(r22, 0x4);
    f0 = f0 + f2;
    MF(sp, 0x1F4) = f0;
    f1 = MF(r22, 0x8);
    f1 = f1 + f3;
    {
        s32 tmp = r2;
        MF(sp, 0x1F8) = f1;                       /* delay slot of bne */
        if (tmp == r3) { r2 = 2; M8(r23, 0x50) = r2; }
    }
    COPY_N((void *)((u8 *)r23 + 0x30), (void *)(sp + 0x1F0), 12);
    f0 = 1.0f;
    r4 = M32(sp, 0x230);
    MF(r4, 0xC) = f0;                             /* 0x33DF4 */
L_00133DF8:
    r2 = D_003931A4 < 2;
    r3 = M32(r19, 0xC);                           /* delay slot of bnel */
    if (r2 != 0) goto L_00133EDC;
    r8 = M32(sp, 0x218);
    r4 = r23;
    COPY_N((void *)r4, (void *)r8, 0x54);         /* 0x33E20/0x33E7C/0x33EB0 */
    r3 = M32(r19, 0xC);                           /* 0x33ED8 */
L_00133EDC:
    r3 = r3 & (s32)0xFFFEFFFF;
    r4 = r19;
    M32(r19, 0xC) = r3;                           /* delay slot */
    r2 = func_0013A148(r4, f23);                  /* 0x33EF0 */
L_00133EF8:
    /* epilogue (0x33EF8 - 0x33F3C): register/fpu restores are elided --- */
L_00133EFC:
    return r2;
}
