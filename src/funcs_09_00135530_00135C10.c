#include "common.h"

/* 64-bit moves through byte pointers: the original code copies these fields
   without any alignment guarantee, so the compiler has to use ldl/ldr pairs. */
typedef struct
{
    unsigned long long value;
} __attribute__((packed)) UnalignedU64;

/* Same idea, written as two 32-bit halves: with a single 64-bit member this
   gcc emits aligned ld/sd, which would fault at a 4-byte-aligned address. */
typedef struct
{
    u32 lo;
    u32 hi;
} __attribute__((packed)) UnalignedPair;

void func_001D0A58(u32 arg0);

extern s32 D_00392F24;
extern s32 D_0039347C;
extern s32 D_00393314;
extern s32 D_00393318;
void func_001826C8(void);
extern s32 D_00393480;
extern s32 D_00393484;
extern s32 D_0039313C;
extern s32 D_00393140;
extern s32 D_00330ED0;
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
extern s32 D_00373060[];
extern s32 D_00373078[];
extern s32 D_003731DC[];
extern s32 D_003731E0[];
extern s32 D_003731E4[];
extern s32 D_003731E8[];
extern s32 D_003731EC[];
extern s32 D_003E3E00[];
extern u8 D_003E6790[];
extern u8 D_0038DAD8[];
extern u8 D_0038DB30[];
extern s32 D_0039384C;
extern u8 D_00378A70[];
extern u8 D_003935F8[];
extern s32 D_00393D18;
void func_001651C0();
void func_00169AC0();
extern u8 D_00393ADC;
extern u8 D_003E8E40[];
extern u8 D_00378BB0[];
extern u8 D_003E7880[];
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

extern s32 D_00393218;
extern s32 D_003938E4;
extern s32 D_003939C4;
extern u8 D_00254029[];
extern u8 D_00253628[];
extern u8 D_00254028[];
extern u8 D_00391278[];
extern u8 D_003E16A8[];
extern s32 D_00393628;
extern s32 D_00410CC0[];
extern u8 D_00391260[];
extern u8 D_00391458[];
extern u8 D_00253627;
extern f32 D_003731D0[];

extern f32 D_003731D4[];

extern f32 D_00393258;

extern f32 D_0039325C;

extern f32 D_00393260;

extern f32 D_00393264;

extern f32 D_00393268;

extern s32 D_00355448[];

extern s32 D_003932C4;

extern s32 D_00393090;

extern s16 D_003932C0;

extern s32 D_00355348[];

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

extern s32 D_00331158[];

extern s32 D_003931C4;

extern s32 D_00393294;

extern s32 D_003932A0[];

extern s32 D_00355298[];

extern s32 D_003552F0[];

extern s32 D_003932B4;

extern s32 D_003932D0;

extern s32 D_003932E0;

extern s32 D_003932E4;

extern u8 D_00393908;

extern s32 D_00393388;

extern s32 D_003932D8;

extern s32 D_00393310;

extern s32 D_0039331C;

extern s32 D_00393320;

extern s32 D_00393324;

extern s32 D_00393328;

extern s32 D_003933DC;

extern s32 D_003D7E00[];

extern s32 D_00393540;

extern s32 D_00393568;

extern s32 D_0038A468[];

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
extern u8 D_0034A380[];
extern u8 D_00378BC8[];
extern s32 D_00393D08;

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




INCLUDE_ASM(const s32, "core/main", func_00135530);

INCLUDE_ASM(const s32, "core/main", func_00135C10);
