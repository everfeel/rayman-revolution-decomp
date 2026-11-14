#include "common.h"

extern s32 D_00392F24;
extern s32 D_0039347C;
extern s32 D_00393314;
extern s32 D_00393318;
void func_001826C8(void);
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

s32 func_001346E8(s32 arg1, s32 arg2, f32 fArg)
{
    u8 stateCopy[0x54];     /* copy of arg1+0x78 (sp+0x00) */
    u8 state2[12];          /* copy of arg1+0x1A0 (sp+0x30) */
    f32 one;                /* sp+0x3C */
    f32 dx, dy, dz;         /* sp+0x60/64/68 */
    f32 acc0, acc4, acc8;   /* sp+0x70/74/78: the f20/f10 mixed vector */
    f32 n80, n84, n88;      /* sp+0x80/84/88: normalized mixed */
    f32 b90, b94, b98;      /* sp+0x90/94/98: normalized direction */
    f32 entryCopy[3];       /* sp+0xA0/A4/A8: o-vector then arg1+0xA8 backup */
    f32 d0, d4, d8;         /* work vector */
    f32 cB0, cB4, cB8;      /* sp+0xB0/B4/B8: reflected */
    f32 cC0, cC4, cC8;      /* sp+0xC0/C4/C8: projected */
    f32 cD0, cD4, cD8;      /* sp+0xD0/D4/D8: reflection terms / refinement */
    f32 cE0, cE4, cE8;      /* sp+0xE0/E4/E8 */
    f32 o0, o4, o8;         /* sp+0xF0/F4/F8: output position */
    f32 sum0, sum4, sum8;   /* sp+0x110/114/118: entry sum */
    f32 g130, g134;         /* sp+0x130/134 */
    f32 g138;               /* sp+0x138 */
    u8 out[0x40];           /* sp+0x140: func_00135530 output struct */
    u8 out2[0x10];          /* sp+0x100: func_00136C08 output */
    s32 arg2s;              /* sp+0x144 */
    s32 r;                  /* call results */
    s32 count8;             /* func_00108648 result (sp+0x148) */
    s32 len;                /* out.count (sp+0x140) */
    s32 *objPtr;            /* out+0x08 */
    s32 *ptr14C;            /* out+0x0C */
    s32 *ptr150;            /* arg1+0xA8 (sp+0x150) */
    s32 *sumPtr;            /* $18: &sum0 or first+4 */
    s32 flagA;              /* sp+0x154 */
    s32 flagB;              /* sp+0x158 */
    s32 idx;                /* sp+0x15C */
    s32 flagC;              /* sp+0x160 */
    s32 valA;               /* sp+0x164 */
    s32 valB;               /* sp+0x168 */
    s32 *statePtr;          /* sp+0x16C = arg1+0x78 */
    s32 *outPtr;            /* sp+0x170 = out */
    s32 *found;             /* scan 1 result ($21) */
    s32 *first;             /* scan 2 result ($23) */
    s32 *entry;             /* $16 */
    s32 i;                  /* $17 */
    s32 t19;                /* $19 walk-state */
    s32 near1;              /* $2 loop temp */
    s32 cond;
    s32 t2;
    f32 neg;                /* the -dot reflect scale */
    f32 f5v;                /* the .L001351C8 temp */
    f32 f20, f10;           /* func_00139828/58 results */
    f32 f9v;                /* 0.0f then o8 */
    f32 f11v;               /* D_00392718 threshold */
    f32 f12v;               /* the dx/dy/dz sqrt length */
    f32 f6v;                /* D_003931B4 - D_00392724/2730 */
    f32 f4r, f5r, f7r;      /* refinement vectors */
    f32 f8r;                /* refinement length */
    f32 f3r;                /* refinement scale */
    f32 f3v;                /* the f20 scale factor */
    s32 small;

    count8 = func_00108648(arg1);
    flagB = 0;
    idx = 0;
    flagC = 0;
    valA = 0;
    valB = 0;
    ptr150 = (s32 *)(arg1 + 0xA8);
    statePtr = (s32 *)(arg1 + 0x78);
    outPtr = out;
    if (count8 < 2) {
        /* short init: no state copy */
    } else {
        memcpy(stateCopy, (void *)(arg1 + 0x78), 0x54);
        if (stateCopy[0x50] == 1) {
            stateCopy[0x50] = 2;
        }
        memcpy(state2, (void *)(arg1 + 0x1A0), 12);
        *(f32 *)(state2 + 12) = 1.0f;
    }

    /* B4: func_00135530 fills the output struct */
    r = func_00135530(outPtr, D_003931B0, arg1, arg2);
    len = *(s32 *)out;
    objPtr = *(s32 **)(out + 8);
    ptr14C = *(s32 **)(out + 0xC);

    /* B5: early exit */
    if (r == 0) {
        f32 *p30;
        *(s32 *)(arg1 + 0xC) &= ~0x10000;
        if (D_003931B8 >= 10) {
            return r;
        }
        p30 = (f32 *)((s8 *)objPtr + 0x30);
        {
            f32 f6 = D_003931B4 - D_00392730;
            if (f6 < p30[2]) {
                d0 = 0.0f;
                d4 = 0.0f;
                d8 = f6 - p30[2];
                func_001241D8(out, &d0);
            }
        }
        return r;
    }

    /* B6: main path */
    ptr14C = ((*(s32 *)(arg1 + 0xC) & 4) != 0) ? (s32 *)(arg1 + 0x280) : ptr14C;
    sum0 = 0.0f;
    sum4 = 0.0f;
    sum8 = 0.0f;
    first = 0;
    found = 0;
    t19 = 0;

    /* scan 1: first entry (type != 0x63) accepted by func_00139708 */
    if (len > 0) {
        s32 *base = (s32 *)D_003931B0;
        for (i = 0; i < len; i++) {
            entry = (s32 *)((s8 *)base + i * 0x68);
            if (*(u16 *)((s8 *)entry + 0x2E) != 0x63) {
                if ((func_00139708(arg1, (s32)entry + 4) & 0xFF) != 0) {
                    found = entry;
                    valB = i;
                }
            }
            if (found != 0) {
                break;
            }
        }
    }

    /* found && D_00330ED0 bit6 clear: func_00131D78 then bail */
    if (found != 0) {
        if (((D_00330ED0[0] >> 6) & 1) == 0) {
            func_00131D78(arg1, arg2, fArg);
            D_00330ED0[0] &= ~0x40;
            return -65;  /* the mask constant retail leaves in $2 */
        }
    }
    flagA = 1;

    /* scan 2: first entry (type != 0x63) */
    first = 0;
    if (len > 0) {
        s32 *base = (s32 *)D_003931B0;
        for (i = 0; i < len; i++) {
            entry = (s32 *)((s8 *)base + i * 0x68);
            if (*(u16 *)((s8 *)entry + 0x2E) == 0x63) {
                continue;
            }
            valA = i;
            first = entry;
            break;
        }
    }

    /* scan 3: per-entry flag/index updates */
    if (len > 0) {
        s32 *base = (s32 *)D_003931B0;
        for (i = 0; i < len; i++) {
            s32 *data;
            s32 type;
            s32 flags1;
            s32 cond1;
            s32 cond2 = 0;
            s32 sel;
            entry = (s32 *)((s8 *)base + i * 0x68);
            data = *(s32 **)((s8 *)entry + 0x20);
            type = *(u16 *)((s8 *)entry + 0x2E);
            flags1 = *(u16 *)((u8 *)data + 6);
            cond1 = (flags1 & 0x4000) != 0;
            idx = cond1 ? 1 : idx;
            if (type == 0x63) {
                continue;
            }
            if (cond1) {
                s32 *data2 = *(s32 **)((s8 *)entry + 0x1C);
                s32 flags2 = *(u16 *)((u8 *)data2 + 6);
                cond2 = (flags2 & 0x4000) ? 1 : 0;
            }
            sel = i;
            if (t19 != 0) {
                sel = t19;
            }
            if ((u32)(type - 7) < 2) {
                if (cond2 != 0) {
                    t19 = sel;
                }
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
        valA = (valA < valB) ? valA : valB;
    }
    if (valA < t19) {
        valA = t19;
    }
    func_00135CF8(arg1, arg2, len, D_003931B0, valA);

    /* float accumulation of non-0x63 entry positions */
    sumPtr = &sum0;
    if (flagA != 0) {
        f32 sx = 0.0f;
        f32 sy = 0.0f;
        f32 sz = 0.0f;
        s32 *base = (s32 *)D_003931B0;
        if (len > 0) {
            for (i = 0; i < len; i++) {
                entry = (s32 *)((s8 *)base + i * 0x68);
                if (*(u16 *)((s8 *)entry + 0x2E) == 0x63) {
                    continue;
                }
                sx = sx + *(f32 *)((s8 *)entry + 4);
                sy = sy + *(f32 *)((s8 *)entry + 8);
                sz = sz + *(f32 *)((s8 *)entry + 0xC);
            }
        }
        sum0 = sx;
        sum4 = sy;
        sum8 = sz;
        if (sx == 0.0f && sy == 0.0f && sz == 0.0f) {
            ptr150 = (s32 *)((s8 *)first + 4);
            sumPtr = (s32 *)((s8 *)first + 4);
        } else {
            f32 l2 = sx * sx + sy * sy + sz * sz;
            f32 l = sqrtf(l2);
            if (l != 0.0f) {
                f32 s = 1.0f / l;
                sum0 = s * sx;
                sum4 = s * sy;
                sum8 = s * sz;
            }
        }
    } else {
        ptr150 = (s32 *)((s8 *)first + 4);
        sumPtr = (s32 *)((s8 *)first + 4);
    }

    /* the func_00139828/58 results + the reflect/projection block */
    f20 = func_00139828(arg1, (s32)first);
    f10 = func_00139858(arg1, (s32)first);
    f9v = 0.0f;
    n80 = 0.0f;
    n84 = 0.0f;
    n88 = 0.0f;
    b90 = 0.0f;
    b94 = 0.0f;
    b98 = 0.0f;
    g130 = 0.0f;
    g134 = 0.0f;
    g138 = 1.0f;
    {
        f32 *p150 = (f32 *)ptr150;
        f32 *p22 = (f32 *)(arg1 + 0xFC);
        f32 nx, ny, nz;
        f32 dot;
        f32 px, py, pz, rx, ry, rz;

        f11v = D_00392718;
        dx = p22[0] - p150[0];
        dy = p22[1] - p150[1];
        dz = p22[2] - p150[2];
        {
            f32 l2 = dx * dx + dy * dy + dz * dz;
            f12v = sqrtf(l2);
        }
        nx = p150[0];
        ny = p150[1];
        nz = p150[2];
        dot = dx * nx + dy * ny + dz * nz;
        neg = -dot;
        cD0 = neg * nx;
        px = dx + cD0;
        rx = px + cD0;
        cD4 = neg * ny;
        py = dy + cD4;
        ry = py + cD4;
        cE4 = neg * nz;
        pz = dz + cE4;
        rz = pz + cE4;
        cC0 = px;
        cC4 = py;
        cC8 = pz;
        cB0 = rx;
        cB4 = ry;
        cB8 = rz;
        acc0 = f20 * px + f10 * rx;
        acc4 = f20 * py + f10 * ry;
        acc8 = f20 * pz + f10 * rz;
        small = 0;
        if (fabsf(acc0 - 0.0f) < f11v) {
            if (fabsf(acc4 - 0.0f) < f11v) {
                if (fabsf(acc8 - 0.0f) < f11v) {
                    small = 1;
                }
            }
        }
        if (small == 0) {
            f32 l2 = acc0 * acc0 + acc4 * acc4 + acc8 * acc8;
            f32 l = sqrtf(l2);
            if (l != 0.0f) {
                f32 s = 1.0f / l;
                n80 = s * acc0;
                n88 = s * acc8;
                n84 = s * acc4;
            }
        }
    }

    /* the dx/dy/dz threshold check + the direction normalize */
    small = 0;
    if (fabsf(dx - 0.0f) < D_0039271C) {
        if (fabsf(dy - 0.0f) < D_0039271C) {
            if (fabsf(dz - 0.0f) < D_0039271C) {
                small = 1;
            }
        }
    }
    if (small == 0) {
        f32 s = 1.0f / f12v;
        b90 = s * dx;
        b98 = s * dz;
        b94 = s * dy;
    }
    /* the dot2 check */
    if (flagA != 0) {
        f32 dot2 = b90 * ((f32 *)sumPtr)[0] + b94 * ((f32 *)sumPtr)[1] +
                   b98 * ((f32 *)sumPtr)[2];
        if (dot2 >= D_00392720) {
            goto skip_a;
        }
        if (*(u8 *)(arg1 + 0x11C) == 1) {
            *(u8 *)(arg1 + 0x11C) = 2;
        }
        memcpy((s8 *)objPtr + 0x30, ptr150, 12);
        *(f32 *)((s8 *)arg1 + 0x108) = 1.0f;
        *(f32 *)(arg1 + 0x44) = 0.0f;
        *(f32 *)(arg1 + 0x3C) = 0.0f;
        *(f32 *)(arg1 + 0x40) = 0.0f;
        return 1;
    }

skip_a:
    /* the func_00136C08 pass */
    func_00136C08((s32 *)out2, arg1, valA, D_003931B0, valA);
    o0 = *(f32 *)((s8 *)arg1 + 0xFC) + *(f32 *)(out2 + 0);
    o4 = *(f32 *)((s8 *)arg1 + 0xFC) + *(f32 *)(out2 + 4);
    f9v = *(f32 *)((s8 *)arg1 + 0xFC) + *(f32 *)(out2 + 8);
    entryCopy[0] = o0;
    entryCopy[1] = o4;
    entryCopy[2] = f9v;
    o8 = f9v;
    if (D_003931B8 < 10) {
        f6v = D_003931B4 - D_00392724;
        if (f6v < f9v) {
            flagC = 1;
            cond = (*(u16 *)((u8 *)*(s32 **)((s8 *)first + 0x20) + 6) & 0x4000) ? 1 : 0;
            if (cond != 0) {
                s32 *data2 = *(s32 **)((s8 *)first + 0x1C);
                cond = (*(u16 *)((u8 *)data2 + 6) & 0x4000) ? 0 : 1;
            }
            if (cond != 0) {
                /* the simple path */
                d0 = 0.0f;
                d4 = 0.0f;
                d8 = f6v - f9v;
            } else {
                /* the complex path */
                f10 = 1.0f;  /* retail reuses the f10 reg for the constant here */
                f8r = *(f32 *)((s8 *)first + 0xC);
                cE0 = f8r * *(f32 *)((s8 *)first + 4);
                cE4 = f8r * *(f32 *)((s8 *)first + 8);
                cE8 = f8r * *(f32 *)((s8 *)first + 0xC);
                f7r = -cE0;
                f5r = -cE4;
                f4r = 1.0f - cE8;
                f8r = sqrtf(f7r * f7r + f5r * f5r + f4r * f4r);
                if (f8r == 0.0f) {
                    d0 = 0.0f;
                    d4 = 0.0f;
                    d8 = f6v - f9v;
                } else {
                    f32 inv = 1.0f / f8r;
                f3r = inv * inv * (f6v - f9v);
                    d8 = f3r * f4r;
                    d0 = f3r * f7r;
                    d4 = f3r * f5r;
                }
            }
        }
    }
    /* o += d */
    o0 = entryCopy[0] + d0;
    o4 = entryCopy[1] + d4;
    o8 = entryCopy[8] + d8;
    entryCopy[0] = o0;
    entryCopy[4] = o4;
    entryCopy[8] = o8;

    /* the f3v scale + the near-check refinement (.L001350A4-351C8) */
    f3v = (0.0f <= f20) ? 0.0f : 1.0f - f20;
    d0 = f3v * acc0;
    d4 = f3v * acc4;
    d8 = f3v * acc8;
    if (flagC != 0) {
        d8 = 0.0f;
        near1 = (fabsf(d0 - 0.0f) < D_00392728 && fabsf(d4 - 0.0f) < D_00392728) ? 1 : 0;
        if (near1 != 0) {
            acc8 = 0.0f;
            /* d unchanged */
        } else {
            f32 l2 = d0 * d0 + d4 * d4 + d8 * d8;
            f32 l = sqrtf(l2);
            acc8 = 0.0f;
            if (l == 0.0f) {
                d0 = 0.0f;
                d4 = 0.0f;
                d8 = 0.0f;
            } else {
                /* the normalize-then-rescale cancels; only the rounding differs */
                f32 s = 1.0f / l;
                f32 d0n = s * d0;
                f32 d4n = s * d4;
                d0 = l * d0n;
                d4 = l * d4n;
                d8 = 0.0f;
            }
        }
    }

    /* .L001351E4: the output position = o + d (d re-added; f5 = 0 per retail) */
    o0 = entryCopy[0] + d0;
    o4 = entryCopy[1] + d4;
    o8 = entryCopy[2] + 0.0f;
    if (*(u8 *)(arg1 + 0x11C) != 1) {
        *(u8 *)(arg1 + 0x11C) = 2;
    }
    memcpy(entryCopy, ptr150, 12);
    if (*(u8 *)(arg1 + 0x11C) != 1) {
        *(u8 *)(arg1 + 0x11C) = 2;
    }
    /* .L00135248: objPtr+0x30 = o; arg1+0xFC+0xC = 1.0f; arg1+0xA8 = arg1+0xFC - acc */
    memcpy((s8 *)objPtr + 0x30, &o0, 12);
    *(f32 *)((s8 *)arg1 + 0x108) = 1.0f;
    *(f32 *)ptr150 = *(f32 *)((s8 *)arg1 + 0xFC) - acc0;
    *(f32 *)((s8 *)ptr150 + 4) = *(f32 *)((s8 *)arg1 + 0xFC) - acc4;
    *(f32 *)((s8 *)ptr150 + 8) = *(f32 *)((s8 *)arg1 + 0xFC) - acc8;
    if ((*(s32 *)(arg1 + 0xC) & 4) != 0) {
        if (*(u8 *)(arg1 + 0x11C) != 1) {
            *(u8 *)(arg1 + 0x11C) = 2;
        }
        memcpy((s8 *)ptr14C + 0x30, &o0, 12);
        *(f32 *)((s8 *)ptr14C + 0x3C) = 1.0f;
    }
    memcpy(state2, (s8 *)first + 4, 12);
    if (flagB != 0) {
        return func_0013A148(arg1, fArg);
    }
    /* the second func_00135530 pass (6 args) + the apply loop */
    r = func_00135530(outPtr, D_003931B0, arg1, arg2, (s32)statePtr, (s32)((s8 *)arg1 + 0xCC));
    if (r == 0) {
        return func_0013A148(arg1, fArg);
    }
    if (len > 0) {
        f32 f6 = 1.0f;
        s32 *base = (s32 *)D_003931B0;
        s32 off = 0;
        for (i = 0; i < len; i++) {
            s32 cond = 0;
            s32 nearv = 0;
            entry = (s32 *)((s8 *)base + off);
            if (*(u16 *)((s8 *)entry + 0x2E) == 0x63) {
                goto next_i;
            }
            {
                s32 *data = *(s32 **)((s8 *)entry + 0x20);
                if ((*(u16 *)((u8 *)data + 6) & 0x4000) != 0) {
                    s32 *data2 = *(s32 **)((s8 *)entry + 0x1C);
                    cond = (*(u16 *)((u8 *)data2 + 6) & 0x4000) ? 1 : 0;
                }
            }
            if (cond != 0) {
                goto next_i;
            }
            {
                f32 ddx = *(f32 *)(state2 + 0) - *(f32 *)((s8 *)entry + 4);
                f32 ddy = *(f32 *)(state2 + 4) - *(f32 *)((s8 *)entry + 8);
                f32 ddz = *(f32 *)(state2 + 8) - *(f32 *)((s8 *)entry + 0xC);
                f32 lim = D_0039272C;
                nearv = (fabsf(ddx - 0.0f) < lim && fabsf(ddy - 0.0f) < lim &&
                         fabsf(ddz - 0.0f) < lim) ? 1 : 0;
            }
            if (nearv != 0) {
                goto next_i;
            }
            if (*(u8 *)(arg1 + 0x11C) == 1) {
                *(u8 *)(arg1 + 0x11C) = 2;
            }
            memcpy((s8 *)objPtr + 0x30, entryCopy, 12);
            *(f32 *)((s8 *)arg1 + 0x108) = 1.0f;
            memcpy(ptr150, entryCopy, 12);
        next_i:
            i++;
        }
    }
    return func_0013A148(arg1, fArg);
}
