#include "common.h"

extern s32 D_00392F24;
extern s32 D_0039347C;
extern s32 D_00393314;
extern s32 D_00393318;
void func_001826C8(void);
/* These two return their result in $f0.  Without a prototype gcc assumes an
 * int return, discards $f0 and converts the unrelated value left in $v0 with
 * cvt.s.w -- which fed garbage into the reflection weights below. */
f32 func_00139828(u8 *arg0, s32 arg1);
f32 func_00139858(u8 *arg0, s32 arg1);
/* Confirmed from the retail listing (see REFERENCE-BEHAVIORMATCH.md):
 * `func_00139708(actor, f32 *normal)` returns 1 when
 * `actor->+0x18 <= normal[2]`, i.e. a floor-like surface test.  The second
 * argument is the ADDRESS of a SurfaceContact's three-float normal at +0x04,
 * not the contact pointer and not a scalar. */
s32 func_00139708(s32 actor, f32 *normal);
extern s32 D_00393480;
extern s32 D_00393484;
extern s32 D_0039313C;
extern s32 D_00393140;
extern s32 D_00330ED0[];
extern f32 D_00392718;
extern f32 D_0039271C;
extern f32 D_00392720;
extern f32 D_00392724;
extern f32 D_00392728;
extern f32 D_0039272C;
extern f32 D_00392730;
extern s32 D_003931B0;
extern f32 D_003931B4;
extern s32 D_003931B8;
extern u8 D_00388B00[];
extern u8 D_00388B10[];
extern u8 D_00393B58[];
extern s32 D_00391480[];
extern s32 D_00391498[];
extern s32 D_003914B0[];
extern s32 D_003914C8[];
extern s32 D_003914E0[];
extern s32 D_003914F8[];
extern s32 D_00410D88[];
extern s32 D_00410D98[];
extern s32 D_00410DA8[];
extern s32 D_410DC8[];
extern s32 D_410DD8[];
extern s32 D_410DE8[];
extern s32 D_00391100[];
extern s32 D_00391220[];
extern s32 D_003912E8[];
extern s32 D_00391300[];
extern s32 D_00391318[];
extern s32 D_00391330[];
extern s32 D_00391340[];
extern s32 D_00391468[];
extern s32 D_00393DD0[];
extern s32 D_00393DD8[];
extern s32 D_00410D18[];
extern s32 D_00410D28[];
extern s32 D_00410D38[];
extern s32 D_00410D48[];
extern s32 D_00410D58[];
extern s32 D_00410D68[];
extern s32 D_00410D78[];
extern s32 D_00410DB8[];
extern s32 D_00372478[];
extern s32 D_00372490[];
extern s32 D_003724A8[];
extern s32 D_003724C0[];
extern s32 D_003724D8[];
extern s32 D_00372518[];
extern s32 D_00373048[];
extern s32 D_00373060[];
extern s32 D_003771B8[];
extern s32 D_003771D0[];
extern s32 D_003771E8[];
extern s32 D_00377200[];
extern s32 D_00377218[];

// funcs_03_001001F0_0022E7B8.c
// -O2

/* Global startup state block at D_00252230, shared by the boot and transition
 * code of this module.  The fields below are the ones addressed directly by
 * the retail code. */
typedef struct StartupBlock {
    u8 state;                               /* +0x0000: current startup state */
    u8 unk0001[0x43];
    s16 unk44;                              /* +0x0044 */
    s16 unk46;                              /* +0x0046 */
    u8 unk0048[0xF8];
    u32 unk0140;                            /* +0x0140 */
    u32 unk0144;                            /* +0x0144 */
    u32 unk0148;                            /* +0x0148 */
    u8 unk014C[0x0C];
    u32 unk0158;                            /* +0x0158 */
    u32 unk015C;                            /* +0x015C */
    u32 unk0160;                            /* +0x0160 */
    u32 saved;                              /* +0x0164: world restored on the next step */
    u32 active;                             /* +0x0168: actor root in use */
    u32 unk016C;                            /* +0x016C */
    u32 pending;                            /* +0x0170: second actor root */
    u32 unk0174;                            /* +0x0174 */
    u8 recordA[0x54];                       /* +0x0178: scene record of the active root */
    u8 recordB[0x54];                       /* +0x01CC: scene record of the pending root */
    u8 unk0220[0x11D4];
    u8 unk13F4;                             /* +0x13F4 */
    u8 unk13F5;                             /* +0x13F5 */
    u8 unk13F6;                             /* +0x13F6 */
    u8 unk13F7;                             /* +0x13F7 */
    u8 unk13F8;                             /* +0x13F8: selected scene layout */
    u8 unk13F9;                             /* +0x13F9 */
    u8 unk13FA;                             /* +0x13FA */
    u8 flag;                                /* +0x13FB: scene records have to be copied */
    u8 unk13FC;                             /* +0x13FC */
} StartupBlock;

extern StartupBlock D_00252230;

/* ------------------------------------------------------------------------- */
/* Actor / scene types shared by the object, world, animation and script      */
/* functions of this module.                                                 */
/* ------------------------------------------------------------------------- */

struct ObjectList;
struct ActorEntity;
struct ActorManager;

/* Object holding the script descriptor of a node at +0x18. */
typedef struct ActorScriptHolder {
    u8 unk00[0x18];
    s32 current;                            /* +0x18 */
} ActorScriptHolder;

/* Per-node animation owner: the script holder at +0x04 and the frame counter
 * at +0xF4. */
typedef struct ActorAnimOwner {
    u32 unk00;
    struct ActorScriptHolder *scripts;      /* +0x04 */
    u8 unk08[0xEC];
    u16 frame;                              /* +0xF4 */
} ActorAnimOwner;

/* Animation state of a component: the actor it drives at +0x1C. */
typedef struct ActorAnimState {
    u8 unk00[0x1C];
    struct ActorEntity *actor;              /* +0x1C */
} ActorAnimState;

/* Event object attached to an entity at +0x148: the current slot at +0x00, its
 * state flags at +0x04 and the payload at +0xA4. */
typedef struct ActorEvent {
    u32 head;                               /* +0x00 */
    u32 flags;                              /* +0x04 */
    u8 unk08[0x9C];
    s32 value;                              /* +0xA4 */
} ActorEvent;

/* Shared entity state reached through a node's component. */
typedef struct ActorEntity {
    u32 unk00;                              /* +0x00 */
    void *script;                           /* +0x04: script currently attached */
    void *world;                            /* +0x08: world this entity belongs to */
    u32 flags0C;                            /* +0x0C: low state flags; 0x4000 = queued event */
    u8 unk10[0x0D];
    u8 unk1D;                               /* +0x1D: sub-type index */
    u8 unk1E[0x0E];
    u32 flags2;                             /* +0x2C: secondary state flags */
    u8 unk30[4];
    u8 flags;                               /* +0x34: state flags; bit 2 is queried here */
    u8 unk35[0x1F];
    u32 motionLow;                          /* +0x54: animation sample of the last step */
    u32 motionHigh;                         /* +0x58 */
    s32 motionExtra;                        /* +0x5C */
    u8 unk60[0xE8];
    struct ActorEvent *event;               /* +0x148: pending event object */
} ActorEntity;

/* Per-node runtime state: the animation owner at +0x00, the entity at +0x04,
 * the animation state at +0x08 and the active script at +0x0C. */
typedef struct ActorComponent {
    struct ActorAnimOwner *owner;           /* +0x00 */
    struct ActorEntity *entity;             /* +0x04 */
    struct ActorAnimState *anim;            /* +0x08 */
    void *script;                           /* +0x0C */
    s32 value;                              /* +0x10: payload checked by func_00106400 */
} ActorComponent;

/* Node in the game-object hierarchy.  Only nodes of kind 2 carry a usable
 * component pointer; the other kinds are skipped by the predicates below.
 * The same node doubles as an entry of the object lists, which use the link
 * fields at +0x14/+0x18 and the owner pointer at +0x1C. */
typedef struct ActorNode {
    s32 kind;
    struct ActorComponent *component;
    u8 unk08[0x0C];
    struct ActorNode *prev;                 /* +0x14 */
    struct ActorNode *next;                 /* +0x18 */
    struct ObjectList *owner;               /* +0x1C */
} ActorNode;

/* Entry of the manager's actor lists: the actor at +0x00, the next entry at
 * +0x04. */
typedef struct ActorListEntry {
    struct ActorNode *value;
    struct ActorListEntry *next;
} ActorListEntry;

typedef struct ActorList {
    struct ActorListEntry *head;
} ActorList;

typedef struct ActorGroup {
    u8 unk00[4];
    struct ActorList *list;                 /* +0x04 */
} ActorGroup;

/* Actor scene: the main list at +0x00 and a group array at +0x30 with its
 * entry count at +0x34. */
typedef struct ActorScene {
    struct ActorListEntry *head;            /* +0x00 */
    u8 unk04[0x2C];
    struct ActorGroup **groups;             /* +0x30 */
    s32 groupCount;                         /* +0x34 */
} ActorScene;

/* World holding the current scene at +0x04. */
typedef struct ActorManager {
    u8 unk00[4];
    struct ActorScene *scene;               /* +0x04 */
} ActorManager;

/* Per-frame actor context: the active world at +0x20. */
typedef struct ActorContext {
    u8 unk00[0x20];
    struct ActorManager **world;            /* +0x20 */
} ActorContext;

/* Root of the actor system, reached through D_00252398: the per-frame context
 * at +0x04. */
typedef struct ActorRoot {
    u8 unk00[4];
    struct ActorContext *context;           /* +0x04 */
} ActorRoot;

typedef struct StartupConfig {
    u8 unk00[0x50];
    s32 unk50;
    s32 unk54;
    u8 unk58[0x10];
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    u8 unk80[0x10];
} StartupConfig;

typedef struct StartupState {
    u8 unk00[0x44];
    s16 unk44;
    s16 unk46;
} StartupState;

extern u8 D_00252274[5];
extern s32 D_00392F10;


/* Externs referenced by functions below (declared in funcs_03) */
extern u8 D_00253627;
extern f32 D_003731D0[];

extern f32 D_003731D4[];

extern f32 D_00393258;

extern f32 D_0039325C;

extern f32 D_00393260;

extern f32 D_00393264;

extern f32 D_00393268;

extern s32 D_00355448[];

extern s32 D_003700D0[];

extern s32 D_00370238[];

extern s32 D_003702D0[];

extern s32 D_003702D8[];

extern s32 D_003724B4[];

extern s32 D_003731B8[];

extern s32 D_003732FC[];

extern s32 D_00378C6C[];

extern s32 D_00383908[];

extern s32 D_00386D18[];

extern s32 D_003910D0[];

extern s32 D_003910E8[];

extern s32 D_00391290[];

extern s32 D_003912A8[];

extern s32 D_003912C0[];

extern s32 D_003912D8[];

extern s32 D_003913F8[];

extern s32 D_00391408[];

extern s32 D_00391418[];

extern s32 D_00391428[];

extern s32 D_003931C4;

extern s32 D_003932B4;

extern s32 D_003932D0;

extern s32 D_003932D8;

extern s32 D_00393310;

extern s32 D_0039331C;

extern s32 D_00393320;

extern s32 D_00393324;

extern s32 D_00393328;

extern s32 D_003933DC;

extern s32 D_00393544;

extern s32 D_003935DC;

extern s32 D_003935E4;

extern s32 D_003937A8;

extern s32 D_003937B0;

extern s32 D_00393990;

extern s32 D_00393AC8;

extern s32 D_00393C68;

extern s32 D_00393CCC;

extern s32 D_00393CD4;

extern s32 D_00393CDC;

extern s32 D_00393CE4;

extern s32 D_00393CF4;

extern s32 D_00393D00;

extern s32 D_00393D20;

extern s32 D_00393D30;

extern s32 D_00393DC4;

extern s32 D_003D7D80[];

extern s32 D_003D7DE0[];

extern s32 D_003E1A30[];

extern s32 D_003E72D8[];

extern s32 D_003E7400[];

extern s32 func_0016C7F8(s32 value);

extern u16 D_00383900[];

extern u8 D_00393AE6;

extern u8 D_00393AE7;

extern u8 D_003D7E10[];

extern void func_0014EBA8();




s32 func_00131D78(s32, s32, f32);
s32 func_0013A148(s32, f32);

/* Retail computes every three-term sum of products with the R5900 FPU
 * accumulator form: mula.s (ACC = x*y), madda.s (ACC += x*y), madd.s
 * (dest = ACC + x*y).  This toolchain never emits those from C -- tested
 * with -O1/-O2/-O3, -ffast-math, -mmad, -m4650, -mcpu=r4650, -msingle-float
 * and every accumulation shape, all of which produce mul.s/add.s -- and no
 * already-matched function in the tree contains one.  So spell them out; the
 * only difference is that the products are summed without intermediate
 * rounding, exactly as retail does. */
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

s32 func_001346E8(s32 arg1, s32 arg2, f32 fArg)
{
    u8 stateCopy[0x54];     /* sp+0x00: copy of the actor state block */
    u8 out[0x40];           /* sp+0x140: func_00135530 result */
    f32 out2[4];            /* sp+0x100: func_00136C08 result */
    f32 e[3];               /* sp+0xA0: working position */
    f32 o[3];               /* sp+0xF0: resolved position */
    f32 d[3];               /* sp+0xD0: positional correction */
    f32 sumv[3];            /* sp+0x110: summed surface direction */
    f32 nearRef[3];         /* sp+0x120: apply-pass reference position */
    f32 dx, dy, dz;         /* sp+0x60/64/68: arg1+0xFC minus arg1+0xA8 */
    f32 acc0, acc4, acc8;   /* sp+0x70/74/78: surface reaction vector */
    /* sp+0x90/94/98: normalized direction.  Retail only writes these when the
     * offset is non-degenerate (small == 0) and otherwise runs the wall-snap
     * test below against the stale stack slot -- which, because this frame
     * sits at the same address on every call, still holds the last real
     * direction.  As plain locals they were whatever dead register the
     * compiler happened to pick, so dot2 < -0.866 fired at random and the
     * actor could be pushed through a wall instead of being stopped. */
    /* volatile keeps them in stack slots, as retail does: the slot is written
     * only on the non-degenerate path, so it retains the previous call's
     * direction instead of a dead register value.  A static would do the same
     * but would add 12 bytes to .sbss and shift every other global. */
    volatile f32 b90, b94, b98;
    s32 r;
    s32 len;
    s32 *objPtr;            /* out+0x08 */
    s32 *ptr14C;            /* out+0x0C, or arg1+0x280 */
    s32 *statePtr;          /* sp+0x16C: arg1+0x78 */
    s32 *listBase;          /* D_003931B0 */
    s32 *entry;
    s32 *found;             /* scan 1 result */
    s32 *first;             /* scan 2 result */
    f32 *sumP;              /* $18: &sumv, or first+4 */
    s32 flagA, flagB, flagC;
    s32 idx, valA, valB, t19;
    s32 i;
    s32 base;               /* sp+0x148: arg1+0xCC */
    u32 count;              /* func_00108648(arg2) */
    f32 f20, f10;
    f32 f12v;
    f32 f6v;
    f32 f9v;
    f32 f3v;
    s32 small;

    base = arg1 + 0xCC;
    statePtr = (s32 *)(arg1 + 0x78);
    flagB = 0;
    idx = 0;
    flagC = 0;
    valA = 0;
    valB = 0;
    count = (u32)func_00108648(arg2);

    if (count >= 2) {
        memcpy(stateCopy, (void *)(arg1 + 0x78), 0x54);
        if (stateCopy[0x50] == 1) {
            stateCopy[0x50] = 2;
        }
        /* These go into the copy's own +0x30 anchor, not a separate buffer:
         * $8 is sp (the copy base) at 0x34848, and the stores use 0x30($8). */
        memcpy(stateCopy + 0x30, (void *)(arg1 + 0x1A0), 12);
        *(f32 *)(stateCopy + 0x3C) = 1.0f;
    }

    /* First pass.  Retail passes seven arguments: the state copy (or the live
     * state when the actor has fewer than two sub-objects), arg1+0xCC and bit
     * 22 of the actor flags. */
    r = func_00135530(out, D_003931B0, arg1, arg2,
                      (count >= 2) ? (s32 *)stateCopy : statePtr,
                      (s32 *)base,
                      (*(s32 *)(arg1 + 0xC) >> 22) & 1);

    len = *(s32 *)out;
    objPtr = *(s32 **)(out + 8);
    ptr14C = *(s32 **)(out + 0xC);

    if (r == 0) {
        *(s32 *)(arg1 + 0xC) &= ~0x10000;
        if (D_003931B8 < 10) {
            f6v = D_003931B4 - D_00392730;
            if (f6v < *(f32 *)(arg1 + 0x104)) {
                d[0] = 0.0f;
                d[1] = 0.0f;
                d[2] = f6v - *(f32 *)(arg1 + 0x104);
                func_001241D8((s32 *)base, d);
            }
        }
        return r;
    }

    if ((*(s32 *)(arg1 + 0xC) & 4) != 0) {
        ptr14C = (s32 *)(arg1 + 0x280);
    }
    listBase = (s32 *)D_003931B0;
    sumv[0] = 0.0f;
    sumv[1] = 0.0f;
    sumv[2] = 0.0f;
    found = 0;
    first = 0;
    t19 = 0;

    /* scan 1: first entry (type != 0x63) accepted by func_00139708 */
    if (len > 0) {
        for (i = 0; i < len; i++) {
            entry = (s32 *)((s8 *)listBase + i * 0x68);
            if (*(s16 *)((s8 *)entry + 0x2E) != 0x63) {
                if ((func_00139708(arg1, (f32 *)((s8 *)entry + 0x4)) & 0xFF) != 0) {
                    found = entry;
                    valB = i;
                }
            }
            if (found != 0) {
                break;
            }
        }
    }

    if (found != 0 && ((D_00330ED0[0] >> 6) & 1) == 0) {
        func_00131D78(arg1, arg2, fArg);
        D_00330ED0[0] &= ~0x40;
        return -65;
    }
    flagA = 1;

    /* scan 2: first entry whose type is not 0x63 */
    if (len > 0) {
        for (i = 0; i < len; i++) {
            entry = (s32 *)((s8 *)listBase + i * 0x68);
            if (*(s16 *)((s8 *)entry + 0x2E) == 0x63) {
                continue;
            }
            valA = i;
            first = entry;
            break;
        }
    }

    /* scan 3: flag/index update */
    if (len > 0) {
        for (i = 0; i < len; i++) {
            s32 *data;
            s32 type;
            s32 cond1;
            s32 cond2 = 0;
            s32 sel;

            entry = (s32 *)((s8 *)listBase + i * 0x68);
            data = *(s32 **)((s8 *)entry + 0x20);
            type = *(s16 *)((s8 *)entry + 0x2E);
            cond1 = (*(u16 *)((u8 *)data + 6) & 0x4000) != 0;
            /* The store of this flag sits in the delay slot of the type test,
             * so it is evaluated for every entry, 0x63 ones included. */
            if (cond1 != 0) {
                idx = 1;
            }
            if (type == 0x63) {
                continue;
            }
            if (cond1 != 0) {
                s32 *data2 = *(s32 **)((s8 *)entry + 0x1C);
                cond2 = (*(u16 *)((u8 *)data2 + 6) & 0x4000) ? 1 : 0;
            }
            sel = (t19 != 0) ? t19 : i;
            /* Likewise a delay slot: t19 advances for every entry type. */
            if (cond2 != 0) {
                t19 = sel;
            }
            if ((u32)(type - 7) < 2) {
                flagB = 1;
            } else {
                flagA = 0;
            }
        }
    }

    if (first == 0) {
        return 0;
    }
    if (found != 0) {
        /* movz $4, $3, $2 with $2 = (valA < valB): keeps valA when it is the
         * larger one, so this is a maximum, not a minimum. */
        valA = (valA < valB) ? valB : valA;
    }
    if (valA < t19) {
        valA = t19;
    }
    func_00135CF8(arg1, arg2, len, D_003931B0, valA);

    /* summed direction over every non-0x63 entry */
    if (flagA != 0) {
        f32 sx = 0.0f;
        f32 sy = 0.0f;
        f32 sz = 0.0f;

        if (len > 0) {
            for (i = 0; i < len; i++) {
                entry = (s32 *)((s8 *)listBase + i * 0x68);
                if (*(s16 *)((s8 *)entry + 0x2E) == 0x63) {
                    continue;
                }
                sx = sx + *(f32 *)((s8 *)entry + 4);
                sy = sy + *(f32 *)((s8 *)entry + 8);
                sz = sz + *(f32 *)((s8 *)entry + 0xC);
            }
        }
        sumv[0] = sx;
        sumv[1] = sy;
        sumv[2] = sz;
        if (sx == 0.0f && sy == 0.0f && sz == 0.0f) {
            sumP = (f32 *)((s8 *)first + 4);
        } else {
            f32 l = sqrtf(F_SUM3(sx, sy, sz));
            sumP = sumv;
            if (l != 0.0f) {
                f32 s = 1.0f / l;
                sumP[0] = s * sx;
                sumP[1] = s * sy;
                sumP[2] = s * sz;
            }
        }
    } else {
        sumP = (f32 *)((s8 *)first + 4);
    }

    /* surface reaction: reflect the offset around the summed direction */
    f20 = func_00139828((u8 *)arg1, (s32)first);
    f10 = func_00139858((u8 *)arg1, (s32)first);
    /* func_00136C08 takes this buffer as an in/out position: it reads all
     * three floats (lwc1 0x0/0x4/0x8($4), ldl 0x7($4)) before writing the
     * corrected ones back, so retail clears them here at 0x34C40/0x34C44/
     * 0x34C48.  They must be initialised -- leaving the array untouched feeds
     * whatever was left on the stack into the position write below. */
    out2[0] = 0.0f;
    out2[1] = 0.0f;
    out2[2] = 0.0f;
    {
        f32 *pos = (f32 *)(arg1 + 0xFC);
        f32 *ref = (f32 *)(arg1 + 0xA8);
        f32 dot;
        f32 neg;
        f32 px, py, pz, rx, ry, rz;

        dx = pos[0] - ref[0];
        dy = pos[1] - ref[1];
        dz = pos[2] - ref[2];
        f12v = sqrtf(F_SUM3(dx, dy, dz));
        dot = F_DOT3(dx, sumP[0], dy, sumP[1], dz, sumP[2]);
        neg = -dot;
        d[0] = neg * sumP[0];
        px = dx + d[0];
        rx = px + d[0];
        d[1] = neg * sumP[1];
        py = dy + d[1];
        ry = py + d[1];
        d[2] = neg * sumP[2];
        pz = dz + d[2];
        rz = pz + d[2];
        acc0 = f20 * px + f10 * rx;
        acc4 = f20 * py + f10 * ry;
        acc8 = f20 * pz + f10 * rz;
        /* Retail normalises this vector into sp+0x80/84/88 here; those slots
         * are never read again, so the accumulator itself stays unnormalised. */
        small = (fabsf(acc0) < D_00392718 && fabsf(acc4) < D_00392718 &&
                 fabsf(acc8) < D_00392718);
    }

    small = 0;
    if (fabsf(dx) < D_0039271C) {
        if (fabsf(dy) < D_0039271C) {
            if (fabsf(dz) < D_0039271C) {
                small = 1;
            }
        }
    }
    if (small == 0) {
        f32 s = 1.0f / f12v;
        b90 = s * dx;
        b94 = s * dy;
        b98 = s * dz;
    }

    /* the offset already faces the surface: snap back and stop */
    if (flagA != 0) {
        f32 dot2 = F_DOT3(sumP[0], b90, sumP[1], b94, sumP[2], b98);
        if (dot2 < D_00392720) {
            if (*(u8 *)(base + 0x50) == 1) {
                *(u8 *)(base + 0x50) = 2;
            }
            memcpy((void *)(arg1 + 0xFC), (void *)(arg1 + 0xA8), 12);
            *(f32 *)(arg1 + 0x108) = 1.0f;
            *(f32 *)(arg1 + 0x44) = 0.0f;
            *(f32 *)(arg1 + 0x3C) = 0.0f;
            *(f32 *)(arg1 + 0x40) = 0.0f;
            return 1;
        }
    }

    func_00136C08((s32 *)out2, arg1, len, D_003931B0, valA);
    f20 = *(f32 *)first;
    o[0] = *(f32 *)(arg1 + 0xFC) + out2[0];
    o[1] = *(f32 *)(arg1 + 0x100) + out2[1];
    f9v = *(f32 *)(arg1 + 0x104) + out2[2];
    e[0] = o[0];
    e[1] = o[1];
    e[2] = f9v;

    /* push out of the floor when the resolved position is under the surface */
    if (D_003931B8 < 10) {
        f6v = D_003931B4 - D_00392724;
        if (f6v < f9v) {
            s32 cond = 0;

            flagC = 1;
            if ((*(u16 *)((u8 *)*(s32 **)((s8 *)first + 0x20) + 6) & 0x4000) != 0) {
                s32 *data2 = *(s32 **)((s8 *)first + 0x1C);
                /* movz $4, $0, $2 clears cond when this bit is clear, so cond
                 * stays 1 only when both 0x4000 bits are set. */
                cond = (*(u16 *)((u8 *)data2 + 6) & 0x4000) ? 1 : 0;
            }
            if (cond != 0) {
                d[0] = 0.0f;
                d[1] = 0.0f;
                d[2] = f6v - f9v;
            } else {
                f32 f8r;
                f32 f7r, f5r, f4r;

                f10 = 1.0f;
                f8r = *(f32 *)((s8 *)first + 0xC);
                d[0] = 0.0f;
                d[1] = 0.0f;
                d[2] = 1.0f;
                f7r = 0.0f - f8r * *(f32 *)((s8 *)first + 4);
                f5r = 0.0f - f8r * *(f32 *)((s8 *)first + 8);
                f4r = 1.0f - f8r * *(f32 *)((s8 *)first + 0xC);
                d[0] = f7r;
                d[1] = f5r;
                d[2] = f4r;
                f8r = sqrtf(F_SUM3(f7r, f5r, f4r));
                if (f8r == 0.0f) {
                    d[0] = 0.0f;
                    d[1] = 0.0f;
                    d[2] = f6v - f9v;
                } else {
                    f32 inv = 1.0f / f8r;
                    f32 t = (inv * inv) * (f6v - f9v);

                    d[0] = t * f7r;
                    d[1] = t * f5r;
                    d[2] = t * f4r;
                }
            }
        }
    }
    o[0] = e[0] + d[0];
    o[1] = e[1] + d[1];
    o[2] = e[2] + d[2];
    e[0] = o[0];
    e[1] = o[1];
    e[2] = o[2];

    /* scale the surface reaction by how far the actor is above the surface */
    f3v = (f20 >= 0.0f) ? (1.0f - f20) : 0.0f;
    d[0] = f3v * acc0;
    d[1] = f3v * acc4;
    d[2] = f3v * acc8;

    if (flagC != 0 || idx != 0) {
        f32 d8old = d[2];
        f32 f8v;
        f32 l;

        d[2] = 0.0f;
        f8v = sqrtf(F_SUM3(d[0], d[1], d8old));
        small = (fabsf(d[0]) < D_00392728 && fabsf(d[1]) < D_00392728 &&
                 fabsf(0.0f) < D_00392728);
        acc8 = 0.0f;
        if (small == 0) {
            l = sqrtf(F_SUM3(d[0], d[1], d[2]));
            if (l == 0.0f) {
                d[0] = f8v * d[0];
                d[1] = f8v * d[1];
                d[2] = 0.0f;
            } else {
                f32 inv = 1.0f / l;
                f32 nd0 = inv * d[0];
                f32 nd1 = inv * d[1];

                d[2] = inv * d[2];
                d[0] = f8v * nd0;
                d[1] = f8v * nd1;
            }
        }
    }

    o[0] = e[0] + d[0];
    o[1] = e[1] + d[1];
    o[2] = e[2] + d[2];
    if (*(u8 *)(base + 0x50) == 1) {
        *(u8 *)(base + 0x50) = 2;
    }
    e[0] = *(f32 *)(arg1 + 0xA8);
    e[1] = *(f32 *)(arg1 + 0xAC);
    e[2] = *(f32 *)(arg1 + 0xB0);

    memcpy((void *)(arg1 + 0xFC), o, 12);
    *(f32 *)(arg1 + 0x108) = 1.0f;
    *(f32 *)(arg1 + 0xA8) = *(f32 *)(arg1 + 0xFC) - acc0;
    *(f32 *)(arg1 + 0xAC) = *(f32 *)(arg1 + 0x100) - acc4;
    *(f32 *)(arg1 + 0xB0) = *(f32 *)(arg1 + 0x104) - acc8;

    if ((*(s32 *)(arg1 + 0xC) & 4) != 0) {
        if (*(u8 *)((s8 *)ptr14C + 0x50) == 1) {
            *(u8 *)((s8 *)ptr14C + 0x50) = 2;
        }
        memcpy((s8 *)ptr14C + 0x30, (void *)(arg1 + 0xA8), 12);
        *(f32 *)((s8 *)ptr14C + 0x3C) = 1.0f;
    }

    nearRef[0] = *(f32 *)((s8 *)first + 4);
    nearRef[1] = *(f32 *)((s8 *)first + 8);
    nearRef[2] = *(f32 *)((s8 *)first + 0xC);

    if (flagB != 0) {
        return func_0013A148(arg1, fArg);
    }

    /* second pass: re-resolve and apply the entry that rejected the position */
    r = func_00135530(out, D_003931B0, arg1, arg2, statePtr, (s32 *)base, 0);
    if (r == 0) {
        return func_0013A148(arg1, fArg);
    }
    /* func_00135530 rebinds out[0] on every call: the apply loop is bounded
     * by this second count, not the one from the first query. */
    len = *(s32 *)out;
    if (len > 0) {
        f32 one = 1.0f;

        for (i = 0; i < len; i++) {
            s32 cond = 0;

            entry = (s32 *)((s8 *)listBase + i * 0x68);
            if (*(s16 *)((s8 *)entry + 0x2E) == 0x63) {
                continue;
            }
            if ((*(u16 *)((u8 *)*(s32 **)((s8 *)entry + 0x20) + 6) & 0x4000) != 0) {
                s32 *data2 = *(s32 **)((s8 *)entry + 0x1C);
                cond = (*(u16 *)((u8 *)data2 + 6) & 0x4000) ? 1 : 0;
            }
            if (cond != 0) {
                continue;
            }
            dx = nearRef[0] - *(f32 *)((s8 *)entry + 4);
            dy = nearRef[1] - *(f32 *)((s8 *)entry + 8);
            dz = nearRef[2] - *(f32 *)((s8 *)entry + 0xC);
            if (fabsf(dx) < D_0039272C && fabsf(dy) < D_0039272C &&
                fabsf(dz) < D_0039272C) {
                continue;
            }
            if (*(u8 *)(base + 0x50) == 1) {
                *(u8 *)(base + 0x50) = 2;
            }
            memcpy((void *)(arg1 + 0xFC), e, 12);
            *(f32 *)(arg1 + 0x108) = one;
            memcpy((void *)(arg1 + 0xA8), e, 12);
        }
    }
    return func_0013A148(arg1, fArg);
}
