#include "common.h"

/* Plain 12-byte aggregate: copies emit ldl/ldr + sdl/sdr + sw. */
typedef struct { u32 a, b, c; } Struct12;
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

/* A 12-byte record copied unaligned as a WHOLE: gcc moves the first eight bytes
   with ldl/ldr and the trailing word with lw/sw, and keeps loads before stores.
   Two separate statements instead let the scheduler interleave load/store, which
   is what retail does not do. */
typedef struct
{
    UnalignedU64 v;
    u32 w;
} S12;

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
extern f32 D_00392918;  /* 0.1f, shared lit4 constant - used to avoid a new .lit4 entry */
extern f32 D_00392728;
extern f32 D_0039272C;
extern f32 D_00392730;
extern s32 D_003931B0;
extern f32 D_003931B4;
extern s32 D_003931B8;
extern u8 D_00388B00[];
extern u8 D_00388B10[];
extern u8 D_00393B58;
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
extern u8 D_00391448[];
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









// moved extern vars from block scope
extern u8 D_0022F910[];;
extern u8 D_00230B30[];;
extern u8 D_00234650[];;
extern u32 D_00250C30[];;
extern s32 D_0025362C[];;
extern f32 D_00253F30[];;
extern f32 D_00253F78[];;
extern u8 D_0034C3FC[];;
extern s32 D_0035F690[];;
extern u8 D_00370270[];;
extern s32 D_003702D0[];;
extern s32 D_003731B8[];;
extern s32 D_003731CC[];;
extern s32 D_00373264[];;
extern s32 D_00373270[];;
extern s32 D_003732D0[];;
extern s32 D_003732E8[];;
extern s32 D_003732EC[];;
extern s32 D_003732F8[];;
extern s32 D_00377218[];;
extern u32 D_00377B90[];;
extern u8 D_00378A5C[];;
extern s32 D_00378A60[];;
extern u32 D_00378A7C[];;
extern s32 D_00378A80[];;
extern u32 D_00378A88[];;
extern s32 D_00378A90[];;
extern s32 D_00378B58[];;
extern s32 D_00378BF0[];;
extern s32 D_00378C38[];;
extern u8 D_00378C48[];;
extern s32 D_00378C80[];;
extern s32 D_00378C84[];;
extern s32 D_00378C8C[];;
extern s32 D_00378C90[];;
extern s32 D_00378C98[];;
extern s32 D_00378C9C[];;
extern s32 D_00378D84[];;
extern s32 D_00378DDC[];;
extern s32 D_00378DE0[];;
extern s32 D_00378DE4[];;
extern s32 D_00378DE8[];;
extern s32 D_00378DEC[];;
extern s32 D_00378DF0[];;
extern s32 D_00378DF4[];;
extern s32 D_00379500[];;
extern u8 D_003824E0[];;
extern s32 D_003825C8[];;
extern s32 D_00382F30[];;
extern u8 D_00383780[];;
extern s32 D_00383DD8[];;
extern f32 D_00383E00[];;
extern u8 D_00383EA0[];;
extern u8 D_00385FF0[];;
extern s32 D_003864C8[];;
extern s32 D_00386EF0[];;
extern s32 D_00386F3C[];;
extern s32 D_0038A080[];;
extern s32 D_0038A0B8[];;
extern s32 D_0038A498[];;
extern s32 D_0038AAB0[];;
extern s32 D_0038AAD0[];;
extern s32 D_0038ABA0[];;
extern u8 D_0038BE48[];;
extern s32 D_0038DD88[];;
extern u8 D_0038DF88[];;
extern u8 D_0038E138[];;
extern u8 D_0038E168[];;
extern s32 D_00391110[];;
extern s32 D_00391350[];;
extern u8 D_00391541[];;
extern f32 D_00392E34;;
extern u16 D_003932C2;;
extern u8 D_003932CC;;
extern s32 D_003932F8;;
extern u8 D_00393358;;
extern s32 D_003933A0;;
extern s32 D_003933F0;;
extern s32 D_003933F4;;
extern s32 D_003933F8;;
extern f32 D_00393420;;
extern f32 D_00393424;;
extern s32 D_0039347C;;
extern s32 D_00393530;;
extern s32 D_0039353C;;
extern s32 D_00393544;;
extern s32 D_0039354C;;
extern s32 D_00393558[];;
extern s32 D_003935B8[];;
extern s32 D_003935E0;;
extern u8 D_003935E8;;
extern s32 D_00393618;;
extern s32 D_00393628;;
extern s32 D_00393644;;
extern s32 D_00393650;;
extern s32 D_00393654;;
extern u8 D_00393740;;
extern s32 D_00393744;;
extern s32 D_0039374C;;
extern s32 D_00393750[];;
extern s32 D_00393754;;
extern s32 D_00393758;;
extern s32 D_0039375C;;
extern s32 D_00393760;;
extern s32 D_00393798;;
extern s32 D_0039379C;;
extern s32 D_00393824;;
extern s32 D_00393868;;
extern s32 D_0039386C;;
extern s32 D_00393870;;
extern s32 D_00393990;;
extern s32 D_003939CC;;
extern u8 D_00393A30;;
extern s32 D_00393A64;;
extern s32 D_00393A98[];;
extern s32 D_00393AA0[];;
extern s32 D_00393AA8[];;
extern s32 D_00393AE0;;
extern u8 D_00393B28;;
extern s32 D_00393B34;;
extern s32 D_00393BC0;;
extern s32 D_00393BE0;;
extern s32 D_00393BEC;;
extern s32 D_00393C5C;;
extern s32 D_00393C68;;
extern s32 D_00393C74;;
extern s32 D_00393C90;;
extern s32 D_00393CA0;;
extern s32 D_00393D24;;
extern s32 D_00393D28;;
extern s32 D_00393DD0[];;
extern s32 D_00393DE0[];;
extern s32 D_003D2A30[];;
extern s32 D_003D3CF0[];;
extern s32 D_003D7DE8[];;
extern s32 D_003D82C8[];;
extern u8 D_003E19B0[];;
extern u8 D_003E68B8[];;
extern s32 D_003E6900[];;
extern s32 D_003E6940[];;
extern s32 D_003E6980[];;
extern s32 D_003E69C0[];;
extern u8 D_003E6D28[];;
extern s32 D_003E72D4[];;
extern s32 D_003E7480[];;
extern s32 D_003E7800[];;
extern s32 D_003E78A8[];;
extern s32 D_003E78AC[];;
extern s32 D_003E78B0[];;
extern u8 D_003E7E40[];;
extern u8 D_003EA2E0[];;

// targeted extern var hoist
extern u8 D_0038DD78[];
extern u8 D_0038DDA0[];

// targeted extern var hoist
extern volatile s32 D_00393D2C;

// hoisted function decls
extern s32 func_001012A0();
extern s32 func_00101908();
extern void func_00104A10(f32, f32, f32);
extern void func_00106D28();
extern s32 func_0010C9F0();
extern s32 func_0010E970();
extern s32 func_0010EF58();
extern s32 func_0010F2E8();
extern s32 func_00111568();
extern s32 func_001124E0();
extern s32 func_00112610();
extern s32 func_00112620();
extern s32 func_001134C8();
extern s32 func_00113548();
extern s32 func_001146D8();
extern f32 func_00115FF0();
extern void func_00119150();
extern void func_00119B50();
extern void func_00119D68();
extern void func_00119D78();
extern void func_00119E40();
extern f32 func_0011A788(f32 arg0);
extern f32 func_0011A818();
extern void func_0011B0A0();
extern void func_0011B0C0();
extern void func_0011B4F0(float arg0);
extern s32 func_0011DAA8();
extern s32 func_0011DAD8();
extern s32 func_0011DB80();
extern s32 func_001203A8();
extern s32 func_00120428();
extern s32 func_00120708();
extern s32 func_001207E8();
extern void func_00120C58();
extern void func_00121278(f32 *dst, u8 *node, f32 *src);
extern void func_00121480(f32 *dst, u8 *node, f32 *src);
extern void func_001255C8(u8 *node, f32 *axis, f32 angle);
extern void func_001262F8(u8 *, f32 *, f32 *, f32 *);
extern void func_00126868(u8 *node, f32 *a, f32 *b, f32 *c);
extern s32 func_00127330(u8 *node, f32 *v);
extern s32 func_001275C0(f32 *out, u8 *node);
extern f32 func_001278D0();
extern f32 func_00127BB8(f32 *, f32 *, f32 *);
extern f32 func_00127E10(u32, u32, f32 *);
extern void func_0012D750();
extern void func_0012D900(u8 *node, f32 *v);
extern void func_00139108();
extern s32 func_0013FAE8();
extern s32 func_00145560();
extern s32 func_001455E0();
extern s32 func_00145A90();
extern void func_00149810();
extern s32 func_00149A90();
extern void func_0014AC40();
extern void func_0014AF20();
extern void func_0014B880();
extern s32 func_0014B8F0();
extern void func_0014C280();
extern void func_0014C2B8();
extern s32 func_0014C320();
extern s32 func_0014C9A0();
extern void func_0014D3D0();
extern void func_0014D5B0();
extern void func_0014D6C8();
extern s32 func_0014DD30();
extern void func_0014DEC0();
extern void func_00151570();
extern void func_001517A0();
extern s32 func_00154848();
extern s32 func_00157DE8();
extern s32 func_00158C58();
extern void func_001597F0();
extern void func_00159808();
extern void func_00159E38();
extern void func_00159F00();
extern void func_00159F88();
extern s32 func_0015A0F0();
extern s32 func_0015A108();
extern s32 func_0015A708();
extern void func_0015A7E8();
extern void func_0015A9D8();
extern void func_0015AA30();
extern void func_0015BD30();
extern s32 func_0015C050();
extern void func_0015DEB8(float arg0);
extern void func_0015F768();
extern void func_0015FBC0();
extern s32 func_0015FF18();
extern void func_00160078();
extern void func_001600B8();
extern s32 func_00160A48();
extern s32 func_00160B40();
extern void func_00160B48();
extern s32 func_00160BF0();
extern void func_00161300();
extern void func_001616B0();
extern void func_00161738();
extern s32 func_00161858();
extern void func_00162020();
extern void func_001621C8();
extern void func_00162228();
extern s32 func_001626A8();
extern void func_00162B98();
extern void func_00163088();
extern void func_00163838();
extern void func_00163B88();
extern s32 func_00163DD8();
extern void func_00163EE0();
extern void func_00163F10();
extern void func_00164318();
extern void func_00164348();
extern s32 func_00164768();
extern void func_00164978();
extern s32 func_00164CE8();
extern void func_00164F28();
extern s32 func_00164F50();
extern void func_001650C0();
extern void func_00165120();
extern void func_00165128();
extern void func_00165130();
extern void func_001651C0();
extern s32 func_001651D0();
extern s32 func_00165320();
extern s32 func_00165428();
extern s32 func_00165448();
extern void func_001654F8();
extern s32 func_00165500();
extern s32 func_00165550();
extern void func_001657F8();
extern s32 func_00165978();
extern s32 func_00165F90();
extern s32 func_00166440();
extern void func_00166518();
extern void func_001665C8();
extern void func_00166A18();
extern s32 func_00166DE0();
extern s32 func_00166E70();
extern s32 func_00166ED8();
extern s32 func_00166FA8();
extern s32 func_001670E0();
extern s32 func_00167158();
extern void func_00167160();
extern void func_00167168();
extern s32 func_00167190();
extern void func_00167428();
extern void func_00167488();
extern void func_00167490();
extern void func_00167498();
extern s32 func_001674A0();
extern s32 func_001674C0();
extern s32 func_001674E0();
extern s32 func_00167750();
extern s32 func_00167820();
extern s32 func_00167840();
extern s32 func_00167860();
extern void func_00167880();
extern void func_00167888();
extern void func_00167890();
extern void func_00167970();
extern s32 func_00167978();
extern s32 func_001679E8();
extern s32 func_00167BA0();
extern s32 func_00168028();
extern void func_00168270();
extern void func_00168738();
extern s32 func_00168BC0();
extern s32 func_00168D70();
extern s32 func_00168E50();
extern s32 func_001692F0();
extern s32 func_00169308();
extern s32 func_00169320();
extern s32 func_001697A0();
extern void func_001698C8();
extern void func_00169938();
extern s32 func_001699B0();
extern void func_00169AC0();
extern s32 func_00169B40();
extern void func_00169B78();
extern void func_00169F50();
extern s32 func_0016C8D0();
extern void func_0016CA88();
extern s32 func_0016CD60();
extern s32 func_0016CE78();
extern s32 func_0016DFE8();
extern s32 func_0016F780();
extern void func_0016F8D8();
extern s32 func_0016F930();
extern void func_00170C38();
extern s32 func_00170DF8();
extern void func_001743F8();
extern void func_00181820(u8 *arg0, u8 *arg1, u32 arg2, f32 arg3);
extern s32 func_00188D28();
extern void func_00188E50();
extern s32 func_0018C5D8();
extern s32 func_0018C608();
extern s32 func_0018DA08();
extern s32 func_0018DAD0();
extern u8 func_0018DB30();
extern void func_0018E670();
extern s32 func_0018E6D0();
extern s32 func_0018EC88();
extern void func_0018FA18();
extern void func_001910D8();
extern void func_00192E78();
extern s32 func_00195578();
extern s32 func_001956D0();
extern s32 func_00195848();
extern s32 func_00195948();
extern s32 func_00195CE0();
extern s32 func_00195DB8();
extern s32 func_00196128();
extern s32 func_001969E8();
extern s32 func_00197768();
extern void func_00197810();
extern s32 func_00197B38();
extern s32 func_00198420();
extern s32 func_001986A8();
extern s32 func_00199AD0();
extern s32 func_0019A668();
extern s32 func_0019A7D0();
extern s32 func_0019A830();
extern s32 func_0019B060();
extern s32 func_0019B118();
extern s32 func_001A06B0();
extern s32 func_001A0D00();
extern void func_001A1738();
extern void func_001A2B90();
extern void func_001B6128();
extern void func_001B62A0();
extern void func_001B6468();
extern void func_001B6F50();
extern void func_001B86F8();
extern void func_001B89A8();
extern void func_001BA128();
extern s32 func_001C09C0();
extern s32 func_001C09D0();
extern s32 func_001C09E0();
extern s32 func_001C09F0();
extern s32 func_001C0A10();
extern s32 func_001C0B10();
extern s32 func_001C0C20();
extern s32 func_001C0C30();
extern s32 func_001C0C40();
extern s32 func_001C0C60();
extern s32 func_001C1E28();
extern s32 func_001C22C8();
extern void func_001C28C8();
extern s32 func_001C3378();
extern s32 func_001C45B8();
extern s32 func_001C5C10();
extern s32 func_001C69D0();
extern s32 func_001C6A70();
extern u8 func_001C82A0();
extern s32 func_001C90C8();
extern void func_001CA018();
extern s32 func_001CA1F8();
extern s32 func_001CA2E0();
extern s32 func_001CA448();
extern void func_001CA5B0();
extern void func_001CA848();
extern s32 func_001CC150();
extern s32 func_001CD3D0();
extern s32 func_001CD990();
extern void func_001CDA60();
extern s32 func_001CDDA8();
extern s32 func_001CDF10();
extern void func_001CE0F0();
extern void func_001CE1D8();
extern void func_001CE2E8();
extern void func_001D2140();
extern s32 func_001D2258();
extern void func_001D5568();
extern void func_001DC640();
extern s32 func_001DE818();
extern s32 func_001DF560();
extern s32 func_001DF760();
extern void func_001E0190();
extern void func_001E0F78();
extern void func_001E3DC8();
extern f32 func_001E3F18(f32);
extern s32 func_001E67B0();
extern s32 func_001F1388();
extern s32 func_001F1420();
extern s32 func_001F1450();
extern s32 func_001F1480();
extern s32 func_001F14E0();
extern s32 func_00200550();
extern s32 func_00200AA8();
extern void func_00200FA0();
extern s32 func_00201F80();
extern void func_00202050();
extern void func_0020B398();
extern s32 func_0020B530();
extern s32 func_0020BFD8();
extern f32 func_0020BFE0();
extern s32 func_0020F5A0(s32, s32, u32, f32);
extern s32 func_00211B88();
extern void func_00212348();
extern s32 func_00212CA8();
extern s32 func_00212E28();
extern s32 func_00213578();
extern void func_00215508();
extern s32 func_00215600();
extern void func_00215FA8();
extern void func_00216BE8();
extern void func_002174C0();
extern s32 func_0021B5A8();
extern s32 func_0021D948();
extern void func_0021DA78();
extern s32 func_0021DB18();
extern void func_0021E228();
extern s32 func_0021E340();
extern s32 func_0021E448();
extern s32 func_0021E640() __attribute__((noreturn));
extern s32 func_0021F7A8(f32);
extern void func_00220510();
extern void func_00220568();
extern s32 func_00220A58();
extern s32 func_00220C28();
extern void func_00220CB0();
extern s32 func_002220A0();
extern s32 func_002220E8();
extern s32 func_00222110();
extern s32 func_00222120();
extern s32 func_00222228();
extern s32 func_00222238();
extern s32 func_00222340();
extern s32 func_002227D8();
extern s32 func_00222808();
extern s32 func_00222858();
extern s32 func_00222E08();
extern s32 func_00223768();
extern s32 func_00223E58();
extern s32 func_00223E80();
extern s32 func_00224940();
extern s32 func_002249D4();
extern s32 func_00224B88();
extern s32 func_002268F0();
extern s32 func_00227750();
extern s32 func_00228498();
extern s32 func_0022ABB0();
extern u8 func_0022C1D0();
extern u8 func_0022C2A8();
extern s32 func_0022C508();
extern u8 func_0022C688();
extern u8 func_0022C8B8();
extern u8 func_0022C9B8();
extern s32 func_0022CB30();
extern u8 func_0022CB98();
extern u8 func_0022CF40();

INCLUDE_ASM(const s32, "core/main", func_00136970);

/* ------------------------------------------------------------------------- */
/* func_00136C08 - surface contact offset accumulator (0x600 bytes)          */
/* ------------------------------------------------------------------------- */

/* One entry of the contact list D_003931B0 (filled in by func_00135CF8),
 * stride 0x68.  Only the fields this function addresses are named; the gaps
 * keep the offsets the retail code uses. */
typedef struct SurfaceContact {
    f32 distance;                       /* +0x00: lateral distance, < 0 = near side */
    f32 normalX;                        /* +0x04: contact normal */
    f32 normalY;                        /* +0x08 */
    f32 normalZ;                        /* +0x0C */
    f32 posX;                           /* +0x10: contact position */
    f32 posY;                           /* +0x14 */
    f32 posZ;                           /* +0x18 */
    u8 *objectA;                        /* +0x1C: u16 flags at +0x06, bit 0x4000 */
    u8 *objectB;                        /* +0x20: same */
    u8  unk24[0x04];                    /* +0x24 */
    u32 flags;                          /* +0x28: bit0 = pass 1, bit1 = pass 2, bit2 = done */
    u8  unk2C[0x02];                    /* +0x2C */
    s16 type;                           /* +0x2E: 0x63 = no surface, 7/8 = step surfaces */
    f32 offsetX;                        /* +0x30: contact offset */
    f32 offsetY;                        /* +0x34 */
    f32 offsetZ;                        /* +0x38 */
    u8  unk3C[0x0C];                    /* +0x3C */
    f32 extraX;                         /* +0x48: second offset, see func_00137208 */
    f32 extraY;                         /* +0x4C */
    f32 extraZ;                         /* +0x50 */
    u8  unk54[0x14];                    /* +0x54 */
} SurfaceContact;                       /* 0x68 */

/* Both objects linked to a contact carry the "participates in a surface pair"
 * flag.  Retail tests objectA first and only then loads objectB. */
#define OWNERS_FLAGGED(c) \
    (((*(u16 *)((c)->objectA + 6) & 0x4000) != 0) && \
     ((*(u16 *)((c)->objectB + 6) & 0x4000) != 0))

/* R5900 FPU accumulator forms: `mula.s` / `madda.s` / `madd.s`.  C cannot make
 * this toolchain emit them (see REFERENCE-BYTEMATCH.md), and they round once
 * instead of twice, so spell them out - same idiom as funcs_06 / funcs_08.
 *
 * The parameters are two vectors, component by component, so the products pair
 * as (ax*bx), (ay*by), (az*bz); retail's three dot products here are
 * `mula.s a,b` / `madda.s c,d` / `madd.s r,e,f` in that order. */
#define F_DOT3(ax, ay, az, bx, by, bz) (                        \
    {                                                           \
        float r_;                                               \
        __asm__ volatile("mula.s %1,%4\n\t"                     \
                         "madda.s %2,%5\n\t"                    \
                         "madd.s %0,%3,%6"                      \
                         : "=&f"(r_)                            \
                         : "f"(ax), "f"(ay), "f"(az),            \
                           "f"(bx), "f"(by), "f"(bz));           \
        r_;                                                     \
    })

/* ax*ax + ay*ay + az*az */
#define F_LEN2(x, y, z) F_DOT3(x, y, z, x, y, z)

f32 sqrtf(f32);
f32 fabsf(f32);

/* Thresholds of this function.  Retail loads them from the literal pool at
 * these addresses (lwc1 %hi/%lo(D)($2)), so name them instead of writing float
 * literals: a local literal would add a new .lit4 entry to this object, which
 * pushes .main past its retail end and breaks the fixed layout at link time. */
extern f32 D_0039274C;                 /* 0.001f, "accumulation is zero" */
extern f32 D_00392750;                 /* 0.001f, "contact offset is zero" */
/* Callees and tables that live elsewhere; these were declared inside the
   bodies of the functions below. */
extern s32 func_0014C470();
extern s32 func_0014C498();
extern void func_0014C4C0();
extern s32 func_00224A84();
extern s32 func_00225720();
extern s32 D_0038A440[];
extern s32 D_00393D70, D_00393894, D_00393890, D_00393898;
extern s32 D_0038C848[], D_0038C868[];
extern f32 D_00392908, D_0039290C, D_00392910;

/* Declarations of callees, variables and types whose definitions live
   elsewhere; previously repeated inside function bodies. */
typedef struct { s32 a; s32 b; s32 c; u64 d; } ArgBlock;
typedef struct { s32 v4[3]; s16 v5; s16 pad0; s32 v6; s32 v7; s32 pad[15]; s32 v8; } Blk;
typedef u8 *(*Fn)(u8 *);
typedef struct { s32 out; char pad[12]; s32 buf[4]; } L;
typedef struct { s32 a, b, c; } Rec12;
typedef struct { s32 v4[8]; s32 v5; s32 v6; s32 pad[6]; s32 v7[8]; u64 v8; u64 v9; } Stk;
typedef struct { s32 v4[4]; s32 v5; s32 v6; s32 pad0[2]; s32 v7[4]; f32 v8; f32 v9; s32 pad1[2]; } StkAlt;
typedef struct { UnalignedU64 lo, hi; } U16;
extern Rec12 D_00382818[];
extern f32 D_00392DDC;
extern f32 D_00392DE0;
extern f32 D_00392DE4;
extern f32 D_00392DE8;
extern f32 D_00392DEC;
extern f32 D_00392DF0;
extern f32 D_00392DF4;
extern f32 D_00392DF8;
extern s32 func_0014C1A0();
extern s32 func_0014C1B8();
extern void func_0015F748();
extern void func_0015F770();
extern s32 func_00160E28();
extern void func_00162438();
extern s64 func_00162E70();
extern void func_00166FC8();
extern void func_00167018();
extern void func_00167058();
extern s32 func_001677E8(s32 arg1, s32 arg2);
extern u8 * func_0016F938();
extern u8 * func_0016F950();
extern s32 func_0018AF78();
extern s32 func_0018C550();
extern s32 func_0019A700();
extern s32 func_001C0B20();
extern s32 func_001C1130();
extern s32 func_001C46B0();
extern void func_001C5B50();
extern u32 func_001C6CB8();
extern u32 func_001C6FE8();
extern s32 func_001DFC30();
extern void func_002098F8();
extern void func_0020AF68();
extern void func_0020B6B8();
extern s32 func_00211918(s32 a0, ...);
extern s32 func_002175B0(u8 *a1, s32 a2, s32 a3, u8 *a4, s32 a5, s32 a6, s16 a7, u8 a8, u8 a9);
extern f32 func_00217AC0(f32 arg0);
extern s32 func_0021CDD8();
extern u64 func_0021CE38();
extern u64 func_0021DD58();
extern u64 func_0021DE18();
extern u64 func_0021E0C0();
extern u64 func_0021E390();
extern s32 func_0021E640() __attribute__((noreturn));
extern void func_0021ED98();
extern void func_0021EEA8();
extern s32 func_0021EF38();
extern s32 * func_00221FD8();
extern void func_00222068();
extern s32 func_002226C0(void *a1, void *a2, ...);
extern s32 func_002275C0();
extern void func_0022C278();
extern u8 * func_0022C650();


extern f32 D_00392754;                 /* -0.258f, pass-2 facing threshold */
extern f32 D_00392758;                 /* 0.01f, bias of the final snap */

/* Accumulates the offset vectors of a set of surface contacts into
 * `position` (three floats, written from scratch) and finally snaps the result
 * to the plane of the reference contact.
 *
 *   position    output vector (three floats), zeroed on entry
 *   actor       the actor the contacts belong to; every caller passes it, this
 *               function never reads it
 *   count       number of contacts (0 skips both passes)
 *   contacts    contact array, stride 0x68
 *   indexLimit  contacts past this index are only used when distance < 0; the
 *               first one that is not ends the current pass
 *
 * Returns 1 when no contact needed the "faces against the accumulation" path,
 * 0 otherwise. */
s32 func_00136C08(f32 *position, void *actor, s32 count, SurfaceContact *contacts,
                  s32 indexLimit)
{
    SurfaceContact *anchor;
    SurfaceContact *contact;
    f32 e[3];
    f32 n[3];
    f32 d[3];
    f32 len;
    f32 inv;
    f32 proj;
    f32 bias;
    s32 negative;
    s32 seeded;
    s32 pass;
    s32 i;

    (void)actor;

    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;

    /* Reference contact: the first entry that is a surface and whose two
     * linked objects are both flagged.  It is the plane the result is snapped
     * to at the end and is skipped by the accumulation passes. */
    anchor = 0;
    if (count > 0) {
        contact = contacts;
        i = 0;
        for (;;) {
            if (contact->type != 0x63 && OWNERS_FLAGGED(contact))
                anchor = contact;
            i++;
            if (i >= count)
                break;
            contact++;
            if (anchor != 0)
                break;
        }
    }

    /* Pass 1 uses the contacts flagged bit 0, pass 2 those flagged bit 1.  A
     * contact beyond `indexLimit` is only kept while it is behind the actor
     * (distance < 0); the first one that is not abandons the rest of the
     * pass. */
    seeded = 0;
    negative = 0;
    pass = 1;
    while (pass != 9) {
        for (i = 0; i < count; i++) {
            contact = contacts + i;

            if ((contact->flags & (u32)pass) != (u32)pass)
                continue;
            if (i > indexLimit && !(contact->distance < 0.0f))
                break;
            if (contact->type == 0x63)
                continue;
            if (OWNERS_FLAGGED(contact))
                continue;

            e[0] = contact->offsetX;
            e[1] = contact->offsetY;
            e[2] = contact->offsetZ;

            /* the first accepted contact seeds the accumulation */
            if (seeded == 0) {
                position[0] = e[0];
                position[1] = e[1];
                position[2] = e[2];
                seeded = 1;
                continue;
            }

            /* unit direction of what is accumulated so far */
            if (fabsf(position[0]) < D_0039274C &&
                fabsf(position[1]) < D_0039274C &&
                fabsf(position[2]) < D_0039274C) {
                n[0] = position[0];
                n[1] = position[1];
                n[2] = position[2];
            } else {
                len = sqrtf(F_LEN2(position[0], position[1], position[2]));
                if (len == 0.0f) {
                    n[0] = position[0];
                    n[1] = position[1];
                    n[2] = position[2];
                } else {
                    inv = 1.0f / len;
                    n[0] = inv * position[0];
                    n[1] = inv * position[1];
                    n[2] = inv * position[2];
                }
            }

            /* signed length of this contact offset along that direction */
            proj = F_DOT3(n[0], n[1], n[2], e[0], e[1], e[2]);

            if (proj > 0.0f) {
                if (proj < sqrtf(F_LEN2(position[0], position[1], position[2]))) {
                    /* add the part of the offset across the accumulation */
                    position[0] = position[0] + (e[0] - proj * n[0]);
                    position[1] = position[1] + (e[1] - proj * n[1]);
                    position[2] = position[2] + (e[2] - proj * n[2]);
                } else {
                    /* the contact reaches further than the accumulation */
                    position[0] = e[0];
                    position[1] = e[1];
                    position[2] = e[2];
                }
                continue;
            }

            /* the contact faces against the accumulation */
            negative = 1;
            if (fabsf(e[0]) < D_00392750 && fabsf(e[1]) < D_00392750 &&
                fabsf(e[2]) < D_00392750) {
                d[0] = e[0];
                d[1] = e[1];
                d[2] = e[2];
            } else {
                len = sqrtf(F_LEN2(e[0], e[1], e[2]));
                if (len == 0.0f) {
                    d[0] = e[0];
                    d[1] = e[1];
                    d[2] = e[2];
                } else {
                    inv = 1.0f / len;
                    d[0] = inv * e[0];
                    d[1] = inv * e[1];
                    d[2] = inv * e[2];
                }
            }

            /* pass 2 only keeps contacts that do not face away */
            if (pass == 1 ||
                F_DOT3(n[0], n[1], n[2], d[0], d[1], d[2]) > D_00392754) {
                position[0] = position[0] + (e[0] - proj * n[0]);
                position[1] = position[1] + (e[1] - proj * n[1]);
                position[2] = position[2] + (e[2] - proj * n[2]);
            }
        }
        pass = (pass == 1) ? 2 : 9;
    }

    /* Snap to the reference contact: drop the component of the accumulated
     * offset along its normal (leaving a 0.01 bias) and add its own offset. */
    if (anchor != 0) {
        d[0] = anchor->normalX;
        d[1] = anchor->normalY;
        d[2] = anchor->normalZ;
        len = sqrtf(F_LEN2(d[0], d[1], d[2]));
        if (len != 0.0f) {
            inv = 1.0f / len;
            d[0] = inv * anchor->normalX;
            d[1] = inv * anchor->normalY;
            d[2] = inv * anchor->normalZ;
        }
        bias = F_DOT3(d[0], d[1], d[2], position[0], position[1], position[2]) -
               D_00392758;
        position[0] = (position[0] - bias * d[0]) + anchor->offsetX;
        position[1] = (position[1] - bias * d[1]) + anchor->offsetY;
        position[2] = (position[2] - bias * d[2]) + anchor->offsetZ;
    }

    return negative ^ 1;
}

/* ------------------------------------------------------------------------- */
/* func_00137208 - actor surface resolution (0x6B8 bytes)                     */
/* ------------------------------------------------------------------------- */

/* The actor is much larger than this; only the fields this function touches
 * are named and the gaps keep the retail offsets. */
typedef struct ActorSurfaceState {
    u8  unk00[0x0C];
    u32 flags0C;                        /* +0x0C: bit 0x40 = resolved, bit 0x04 = has slope dir */
    u8  unk10[0x08];
    f32 minSurfaceZ;                    /* +0x18: floor/wall threshold on normal.z */
    u8  unk1C[0x150];
    f32 slopeScale;                     /* +0x16C: compared against 0.5f */
    u8  unk170[0x84];
    f32 groundDirX;                     /* +0x1F4: horizontal direction of the slope */
    f32 groundDirY;                     /* +0x1F8 */
    f32 groundDirZ;                     /* +0x1FC */
} ActorSurfaceState;

/* func_00139708: `actor->minSurfaceZ <= normal[2]` (surface counts as a floor).
 * func_001396A0: `|normal[2]| < actor->minSurfaceZ` (surface counts as a wall).
 * Both read the three floats the pointer points at.  Still retail assembly in
 * this module; only the symbols are needed here. */
s32 func_00139708(ActorSurfaceState *actor, f32 *normal);
s32 func_001396A0(ActorSurfaceState *actor, f32 *normal);

/* Thresholds and gates, all read from the literal pool (see func_00136C08). */
extern f32 D_0039275C;                 /* 0.001f,  coincident-contact epsilon */
extern f32 D_00392760;                 /* 0.001f,  same-spot epsilon for type 8 */
extern f32 D_00392764;                 /* 0.707f,  parallel-surface gate (45 deg) */
extern f32 D_00392768;                 /* -0.001f, opposing wall gate */
extern f32 D_0039276C;                 /* 0.001f,  normal-sum epsilon */
extern f32 D_00392770;                 /* 0.9848f, direction must not be vertical (10 deg) */
extern f32 D_00392774;                 /* 0.001f,  ground-direction epsilon */

/* Resolves which surface the actor is standing on and writes the horizontal
 * slope direction into the actor.
 *
 *   actor     the actor being resolved; `+0x0C` bit 0x40 is cleared on entry and
 *             set again when a direction is accepted
 *   count     number of contacts in `contacts`
 *   contacts  contact array (stride 0x68)
 *   selected  the contact the caller already picked, or 0 to search for one
 *
 * Nothing is returned; the only side effects are `actor->flags0C` and
 * `actor->groundDir` (0x1F4).  A surface contact only participates when its
 * `+0x10` position is at the actor's contact position and its type is 7 or 8. */
void func_00137208(ActorSurfaceState *actor, s32 count, SurfaceContact *contacts,
                   SurfaceContact *selected)
{
    SurfaceContact *entry;
    SurfaceContact *best;
    f32 origin[3];
    f32 delta[3];
    f32 dir[3];
    f32 sum[3];
    f32 len;
    f32 inv;
    f32 proj;
    /* Retail initialises this to 0.0f and never writes it again, so the "best"
     * contact is really the first one with a positive projection. */
    f32 bestDot = 0.0f;
    s32 coincident;
    s32 found;
    s32 near;
    s32 i;

    actor->flags0C &= ~0x40u;

    /* Phase 1 (only when the caller passes no contact): the first contact the
     * actor overlaps that is floor-like.  Unlike the main scan this does not
     * require a coincident position. */
    if (selected == 0) {
        entry = contacts;
        for (i = 0; i < count; i++, entry++) {
            if (entry->type == 0x63)
                continue;
            if (func_00139708(actor, &entry->normalX) == 0)
                continue;
            if ((u16)(entry->type - 7) >= 2)
                return;
            selected = entry;
            break;
        }
        if (selected == 0)
            return;
    }

    if ((u16)(selected->type - 7) >= 2)
        return;
    if (((selected->flags >> 2) & 1) != 0)
        return;

    origin[0] = selected->posX;
    origin[1] = selected->posY;
    origin[2] = selected->posZ;

    /* direction of the selected contact: its (offset + extra) minus its own
     * position */
    dir[0] = (selected->offsetX + selected->extraX) - origin[0];
    dir[1] = (selected->offsetY + selected->extraY) - origin[1];
    dir[2] = (selected->offsetZ + selected->extraZ) - origin[2];
    len = sqrtf(F_LEN2(dir[0], dir[1], dir[2]));
    if (len != 0.0f) {
        inv = 1.0f / len;
        dir[0] = inv * dir[0];
        dir[1] = inv * dir[1];
        dir[2] = inv * dir[2];
    }

    /* Phase 2: classify the other contacts at the same spot. */
    coincident = 0;
    found = 0;
    best = 0;
    for (i = 0; i < count; i++) {
        entry = contacts + i;

        if (entry->type == 0x63)
            continue;

        near = func_00139708(actor, &entry->normalX);
        if (near != 0) {
            /* a floor-like contact that is not a step surface aborts */
            if ((u16)(entry->type - 7) >= 2)
                return;
            delta[0] = origin[0] - entry->posX;
            delta[1] = origin[1] - entry->posY;
            delta[2] = origin[2] - entry->posZ;
            if (fabsf(delta[0]) < D_0039275C && fabsf(delta[1]) < D_0039275C &&
                fabsf(delta[2]) < D_0039275C)
                coincident++;
            continue;
        }

        if (entry->type == 8) {
            delta[0] = origin[0] - entry->posX;
            delta[1] = origin[1] - entry->posY;
            delta[2] = origin[2] - entry->posZ;
            if (!(fabsf(delta[0]) < D_00392760 && fabsf(delta[1]) < D_00392760 &&
                  fabsf(delta[2]) < D_00392760))
                continue;
            /* two nearly parallel step surfaces on a slope: give up */
            if (0.5f < actor->slopeScale &&
                F_DOT3(selected->normalX, selected->normalY, selected->normalZ,
                       entry->normalX, entry->normalY, entry->normalZ) >
                    D_00392764)
                return;
            /* the contact whose normal faces the contact direction wins */
            proj = F_DOT3(entry->normalX, entry->normalY, entry->normalZ,
                          dir[0], dir[1], dir[2]);
            if (best == 0 && bestDot < proj) {
                found = 1;
                best = entry;
            }
            continue;
        }

        /* wall-like contacts: only those the actor stands in front of, and only
         * when the contact is not itself a step surface */
        if (func_001396A0(actor, &entry->normalX) == 0)
            continue;
        if (entry->type == 7)
            continue;
        delta[0] = origin[0] - entry->posX;
        delta[1] = origin[1] - entry->posY;
        delta[2] = origin[2] - entry->posZ;
        if (!(F_DOT3(selected->normalX, selected->normalY, selected->normalZ,
                     delta[0], delta[1], delta[2]) < 0.0f))
            continue;
        if (F_DOT3(entry->normalX, entry->normalY, entry->normalZ,
                   delta[0], delta[1], delta[2]) < D_00392768)
            return;
    }

    /* A slope direction is only derived with at least two coincident contacts
     * and with the contact direction pointing into the surface. */
    if (coincident < 2)
        return;

    if (found != 0) {
        sum[0] = selected->normalX + best->normalX;
        sum[1] = selected->normalY + best->normalY;
        sum[2] = selected->normalZ + best->normalZ;
        if (!(fabsf(sum[0]) < D_0039276C && fabsf(sum[1]) < D_0039276C &&
              fabsf(sum[2]) < D_0039276C)) {
            len = sqrtf(F_LEN2(sum[0], sum[1], sum[2]));
            if (len != 0.0f) {
                inv = 1.0f / len;
                sum[0] = inv * sum[0];
                sum[1] = inv * sum[1];
                sum[2] = inv * sum[2];
            }
            if (F_DOT3(dir[0], dir[1], dir[2], sum[0], sum[1], sum[2]) < 0.0f)
                return;
        }
    }

    /* the contact direction has to be at least ~10 degrees away from vertical */
    if (!(dir[2] < D_00392770))
        return;

    actor->flags0C |= 0x40u;
    if ((actor->flags0C & 4u) == 0)
        return;

    /* keep only the horizontal part of the direction.  Retail also tests |z|
     * here after zeroing it, which is always true, so two tests are enough. */
    dir[2] = 0.0f;
    if (fabsf(dir[0]) < D_00392774 && fabsf(dir[1]) < D_00392774) {
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        dir[2] = 0.0f;
    } else {
        len = sqrtf(F_LEN2(dir[0], dir[1], dir[2]));
        if (len != 0.0f) {
            inv = 1.0f / len;
            dir[0] = inv * dir[0];
            dir[1] = inv * dir[1];
            dir[2] = inv * dir[2];
        }
    }

    actor->groundDirX = dir[0];
    actor->groundDirY = dir[1];
    actor->groundDirZ = dir[2];
}

INCLUDE_ASM(const s32, "core/main", func_001378C0);

INCLUDE_ASM(const s32, "core/main", func_001379E0);

INCLUDE_ASM(const s32, "core/main", func_00137C68);

INCLUDE_ASM(const s32, "core/main", func_00137CB8);

INCLUDE_ASM(const s32, "core/main", func_00137E10);

INCLUDE_ASM(const s32, "core/main", func_00138030);

INCLUDE_ASM(const s32, "core/main", func_00138170);

INCLUDE_ASM(const s32, "core/main", func_00138390);

INCLUDE_ASM(const s32, "core/main", func_001385D0);

void func_001387F0(u8 *arg0, u8 arg1)
{
    u8 buf[0x10];
    u8 *p = arg0 + 0xCC;

    func_0013B328(buf, arg0, arg1);
    func_001241D8(p, buf);
}

void func_00138830(u8 *p)
{
    func_001266B0(p + 0xCC, p + 0x120);
}

void func_00138840(u8 *p)
{
    func_001241D8(p + 0xCC, p + 0x6C);
}

INCLUDE_ASM(const s32, "core/main", func_00138850);

INCLUDE_ASM(const s32, "core/main", func_00138988);

INCLUDE_ASM(const s32, "core/main", func_00138C28);

INCLUDE_ASM(const s32, "core/main", func_00138ED0);

INCLUDE_ASM(const s32, "core/main", func_00138FA0);

INCLUDE_ASM(const s32, "core/main", func_00139108);

void func_00139228(u8 *arg0, u8 *arg1)
{
    if ((*(u32 *)(arg0 + 8) & 0x40000000) == 0)
    {
        *(s32 *)(arg0 + 0x27C) = 0;
        func_00100F58(arg1);
    }
}

void func_00139250(u8 *arg0)
{

    u32 v;

    func_00139108(arg0);
    v = *(u32 *)(arg0 + 8);
    *(u32 *)(arg0 + 8) = v | 0x40000000;
}

void func_00139288(u8 *arg0, u8 *arg1)
{
    *(u32 *)(arg0 + 0x8) &= 0xBFFFFFFF;
    func_00139228(arg0, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_001392A8);

INCLUDE_ASM(const s32, "core/main", func_001396A0);

INCLUDE_ASM(const s32, "core/main", func_00139708);

INCLUDE_ASM(const s32, "core/main", func_00139760);

s32 func_001397C0(u8 *arg0)
{
    u32 v = *(u32 *)(arg0 + 0x24);
    s32 r = 0;

    if (v != 0)
        r = ((func_0010C9F0(func_0013B2E8(v)) >> 6) ^ 1) & 1;
    return r;
}

s32 func_00139800(u8 *arg0)
{
    return (u32)func_00124988((s32)(arg0 + 0xCC), (s32)(arg0 + 0x78)) < 1;
}

f32 func_00139828(u8 *arg0, s32 arg1)
{
    if (arg1 != 0 && (*(u32 *)(arg0 + 8) & 0x80) != 0)
        return *(f32 *)(arg0 + 0x1C);
    return 0.0f;
}

f32 func_00139858(u8 *arg0, s32 arg1)
{
    if (arg1 == 0)
        return 0.0f;
    if ((*(u32 *)(arg0 + 8) & 0x80) == 0)
        return 0.0f;
    return *(f32 *)(arg0 + 0x20);
}

INCLUDE_ASM(const s32, "core/main", func_00139888);

INCLUDE_ASM(const s32, "core/main", func_001399D8);

INCLUDE_ASM(const s32, "core/main", func_00139BA0);

INCLUDE_ASM(const s32, "core/main", func_00139CE0);

INCLUDE_ASM(const s32, "core/main", func_0013A148);

INCLUDE_ASM(const s32, "core/main", func_0013A1C8);

void func_0013A468(u8 *arg0, u8 *arg1)
{
    func_00125100(arg1);
    func_00127330(arg1, arg0 + 0x48);
}

INCLUDE_ASM(const s32, "core/main", func_0013A4A8);

void func_0013A5D0(u8 *p) { func_001241D8((u8 *)p + 0xCC); }

void func_0013A5D8(u8 *arg0, f32 *arg1)
{
    arg1[0] *= *(f32 *)(arg0 + 0x48);
    arg1[1] *= *(f32 *)(arg0 + 0x4C);
    arg1[2] *= *(f32 *)(arg0 + 0x50);
}

INCLUDE_ASM(const s32, "core/main", func_0013A610);

f32 func_0013A7C0(f32 arg0, f32 arg1)
{
    return (f32)(s32)(arg0 / arg1) * arg1;
}

void func_0013A7F8(f32 *a1, f32 a2)
{
    a1[0] = func_0013A7C0(a1[0], a2);
    a1[1] = func_0013A7C0(a1[1], a2);
    a1[2] = func_0013A7C0(a1[2], a2);
}

INCLUDE_ASM(const s32, "core/main", func_0013A858);

INCLUDE_ASM(const s32, "core/main", func_0013A910);

INCLUDE_ASM(const s32, "core/main", func_0013AB00);

INCLUDE_ASM(const s32, "core/main", func_0013AC60);

INCLUDE_ASM(const s32, "core/main", func_0013AD90);

INCLUDE_ASM(const s32, "core/main", func_0013AFB8);

INCLUDE_ASM(const s32, "core/main", func_0013B060);

INCLUDE_ASM(const s32, "core/main", func_0013B258);

/* Byte-matching: linked-list scan for type 2; bnel loop from plain C. */
s32 func_0013B2E8(u8 *p)
{
    if (*(s32 *)p != 2) {
        do {
            p = *(u8 **)(p + 0x1C);
            if (p == 0) return 0;
        } while (*(s32 *)p != 2);
    }
    return (s32)p;
}

void func_0013B320(u8 *p) { func_001241D8((u8 *)p + 0xCC); }

void func_0013B328(f32 *out, float *src, u8 flag, f32 scale)
{
    f32 v4, v5, v6, v7;

    v4 = scale * src[87];
    v5 = v4 * src[95];
    out[0] = v5;
    v6 = v4 * src[96];
    out[1] = v6;
    v7 = v4 * src[97];
    out[2] = v7;
    if (flag != 0) {
        out[0] = src[91] * v5;
        out[1] = src[91] * v6;
        out[2] = src[91] * v7;
    }
}

INCLUDE_ASM(const s32, "core/main", func_0013B388);

s32 func_0013B420(void)
{
    return (u32)func_0010C9F0() >> 31;
}

INCLUDE_ASM(const s32, "core/main", func_0013B440);

INCLUDE_ASM(const s32, "core/main", func_0013B500);

INCLUDE_ASM(const s32, "core/main", func_0013B5E0);

INCLUDE_ASM(const s32, "core/main", func_0013B6B8);

INCLUDE_ASM(const s32, "core/main", func_0013B750);

INCLUDE_ASM(const s32, "core/main", func_0013B870);

typedef struct {
    float x;
    float y;
    float z;
    u8 pad[0x54];
    float m00;
    float m01;
    float m02;
    float m10;
    float m11;
    float m12;
    float m20;
    float m21;
    float m22;
} S13BA88Obj;

void func_0013BA88(u8 *a)
{
    S13BA88Obj *t = *(S13BA88Obj **)(a + 0x38);

    t->m00 = 0.0f;
    t->m01 = -t->z;
    t->m02 = t->y;
    t->m10 = t->z;
    t->m11 = 0.0f;
    t->m12 = -t->x;
    t->m20 = -t->y;
    t->m21 = t->x;
    t->m22 = 0.0f;
}

INCLUDE_ASM(const s32, "core/main", func_0013BAD0);

INCLUDE_ASM(const s32, "core/main", func_0013BD70);

INCLUDE_ASM(const s32, "core/main", func_0013BDC8);

INCLUDE_ASM(const s32, "core/main", func_0013C138);

INCLUDE_ASM(const s32, "core/main", func_0013C590);

INCLUDE_ASM(const s32, "core/main", func_0013C9D8);

INCLUDE_ASM(const s32, "core/main", func_0013CCC0);

INCLUDE_ASM(const s32, "core/main", func_0013CD48);

u8 *func_0013CEB8(u8 *arg0) {
    s32 n = *(s32 *)(arg0 + 0x20);
    u8 *p = *(u8 **)(arg0 + 0x190);
    u8 *end = p + (n << 6);
    u32 v;
    if (p >= end) goto zero;
    v = *p;
    for (;;) {
        if ((v & 1) != 0) return p;
        p += 0x40;
        if (p >= end) goto zero;
        v = *p;
    }
zero:
    return 0;
}

u8 *func_0013CF00(u8 *arg0) {
    s32 n = *(s32 *)(arg0 + 0x20);
    u8 *p = *(u8 **)(arg0 + 0x190);
    u8 *end = p + (n << 6);
    u32 v;
    if (p >= end) goto zero;
    v = *p;
    for (;;) {
        if (((v >> 2) & 1) != 0) return p;
        p += 0x40;
        if (p >= end) goto zero;
        v = *p;
    }
zero:
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_0013CF50);

INCLUDE_ASM(const s32, "core/main", func_0013D038);

/* ------------------------------------------------------------------------- */
/* func_0013D1B0 - orientation node update (0x77C bytes)                     */
/* ------------------------------------------------------------------------- */

/* The 0x54-byte transform node written by func_001246A8 / func_00120D48 and
 * classified by func_00124260. */
typedef struct OrientMatrix {
    f32 m[4][4];                        /* +0x00: right / up / forward / pos */
    f32 sx;                             /* +0x40: decomposed scale */
    f32 sy;                             /* +0x44 */
    f32 sz;                             /* +0x48 */
    f32 s1;                             /* +0x4C: 1.0 */
    u8  type;                           /* +0x50: classification tag */
    u8  unk51[0x03];
} OrientMatrix;                         /* 0x54 */

/* *(object + 0x38): the orientation record owned by the object.  The transform
 * node sits at +0x0C; the 3x3 rotation delta applied on every update follows it
 * at +0x60 (rows at +0x60 / +0x6C / +0x78). */
typedef struct OrientNode {
    f32 angle;                          /* +0x00: spin accumulator (radians) */
    f32 rate;                           /* +0x04: spin rate */
    f32 unk08;                          /* +0x08 */
    OrientMatrix mtx;                   /* +0x0C */
    f32 rot[3][3];                      /* +0x60: rotation delta */
} OrientNode;                           /* 0x84 */

void func_00120D48(u8 *node);
void func_001246A8(u8 *node, f32 *basis, f32 *translation);

extern f32 D_0039280C;                 /* 6.28318548f, 2*pi */
extern f32 D_00392810;                 /* 0.0001f, direction length threshold */
extern f32 D_00392814;                 /* 0.0001f, perpendicular length */
extern f32 D_00392818;                 /* 0.0001f, side vector length */

/* Three orientation modes, selected by flags at arg1+4:
 *   bit 0x10  master enable, without it the function does nothing
 *   bit 0x01  spin: angle += rate * arg2, wrapped into [0, 2*pi)
 *   bit 0x20  look-at: rebuild an orthonormal frame from the direction at
 *             arg0+0x20, ignoring the current basis
 *   otherwise rotate the current basis by the delta at node+0x60 and add the
 *   scaled result to the basis itself (angular velocity integration)
 * The object's own byte 0 bit 2 marks it as already updated this frame. */
void func_0013D1B0(u8 *arg0, u8 *arg1, f32 arg2)
{
    OrientNode *node;
    f32 *v;
    f32 dir[3];
    f32 perp[3];
    f32 side[3];
    f32 basis[3][3];
    f32 cur[3][3];
    f32 acc[3][3];
    f32 zero[3];
    f32 len;
    u32 flags;
    s32 r, c;

    flags = *(u16 *)(arg1 + 0x04);
    if ((flags & 0x10) == 0)
        return;
    if ((*(u8 *)arg0 & 0x04) != 0)
        return;

    node = *(OrientNode **)(arg0 + 0x38);

    if ((flags & 0x01) != 0) {
        /* Retail stores the un-wrapped angle first and then overwrites it with
           the wrapped one on the same slot. */
        f32 a = node->angle + node->rate * arg2;

        node->angle = a;
        if (D_0039280C < a)
            node->angle = a - D_0039280C;
        else if (a < 0.0f)
            node->angle = a + D_0039280C;
        return;
    }

    if ((flags & 0x20) != 0) {
        /* Look-at: the direction lives in the object at +0x20..+0x28. */
        v = (f32 *)(arg0 + 0x20);
        len = sqrtf(F_LEN2(v[0], v[1], v[2]));
        if (len < D_00392810) {
            func_00120D48((u8 *)&node->mtx);
            return;
        }
        dir[0] = v[0] / len;
        dir[1] = v[1] / len;
        dir[2] = v[2] / len;

        /* perp = (0,0,1) x dir, the horizontal perpendicular */
        perp[0] = -dir[1];
        perp[1] = dir[0];
        perp[2] = 0.0f;
        len = sqrtf(F_LEN2(perp[0], perp[1], perp[2]));
        if (len < D_00392814) {
            func_00120D48((u8 *)&node->mtx);
            return;
        }
        perp[0] = perp[0] / len;
        perp[1] = perp[1] / len;
        perp[2] = perp[2] / len;

        side[0] = dir[1] * perp[2] - dir[2] * perp[1];
        side[1] = dir[2] * perp[0] - dir[0] * perp[2];
        side[2] = dir[0] * perp[1] - dir[1] * perp[0];
        len = sqrtf(F_LEN2(side[0], side[1], side[2]));
        if (len < D_00392818) {
            func_00120D48((u8 *)&node->mtx);
            return;
        }
        side[0] = side[0] / len;
        side[1] = side[1] / len;
        side[2] = side[2] / len;

        /* Rows: side, up (the perpendicular), forward (the direction). */
        basis[0][0] = side[0];
        basis[0][1] = side[1];
        basis[0][2] = side[2];
        basis[1][0] = perp[0];
        basis[1][1] = perp[1];
        basis[1][2] = perp[2];
        basis[2][0] = dir[0];
        basis[2][1] = dir[1];
        basis[2][2] = dir[2];
        zero[0] = 0.0f;
        zero[1] = 0.0f;
        zero[2] = 0.0f;
        func_001246A8((u8 *)&node->mtx, &basis[0][0], zero);
        return;
    }

    /* General case: acc = cur + arg2 * (cur x rot), written back with a zero
       translation.  The current basis is read first, so the update can use the
       old value for every component. */
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++)
            cur[r][c] = node->mtx.m[r][c];
    }
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            acc[r][c] = F_DOT3(cur[r][0], cur[r][1], cur[r][2],
                               node->rot[0][c], node->rot[1][c],
                               node->rot[2][c]);
        }
    }
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++)
            acc[r][c] = cur[r][c] + arg2 * acc[r][c];
    }
    func_00120D48((u8 *)&node->mtx);
    zero[0] = 0.0f;
    zero[1] = 0.0f;
    zero[2] = 0.0f;
    func_001246A8((u8 *)&node->mtx, &acc[0][0], zero);
}

INCLUDE_ASM(const s32, "core/main", func_0013D930);

INCLUDE_ASM(const s32, "core/main", func_0013DE78);

INCLUDE_ASM(const s32, "core/main", func_0013E038);

INCLUDE_ASM(const s32, "core/main", func_0013E260);

INCLUDE_ASM(const s32, "core/main", func_0013E4E0);

INCLUDE_ASM(const s32, "core/main", func_0013E568);

INCLUDE_ASM(const s32, "core/main", func_0013E708);

INCLUDE_ASM(const s32, "core/main", func_0013E8A8);

/* Byte-matching.  Stores a value into every object of a fixed-stride array. */
void func_0013EA20(u8 *a, u32 v)
{
    u8 *p = *(u8 **)(a + 0x190);
    u8 *end = p + (*(u32 *)(a + 0x20) << 6);

    while (p < end)
    {
        *(u32 *)(*(u32 *)(p + 0x38) + 0x84) = v;
        p += 0x40;
    }
}

INCLUDE_ASM(const s32, "core/main", func_0013EA68);

void func_0013EB08(void)
{
    u8 *p = (u8 *)D_003931C4;

    while (p != 0)
    {
        p[0x164] &= 0xFE;
        p = (u8 *)*(u32 *)(p + 0x194);
    }
}

void func_0013EB40(void)
{
    u8 *p = (u8 *)D_003931C4;

    while (p != 0)
    {
        p[0x164] |= 0x1;
        p = (u8 *)*(u32 *)(p + 0x194);
    }
}

void func_0013EB78(void) { D_003931C4 = 0; }

INCLUDE_ASM(const s32, "core/main", func_0013EB80);

INCLUDE_ASM(const s32, "core/main", func_0013ED28);

INCLUDE_ASM(const s32, "core/main", func_0013EEE8);

INCLUDE_ASM(const s32, "core/main", func_0013EFB8);

INCLUDE_ASM(const s32, "core/main", func_0013F020);

INCLUDE_ASM(const s32, "core/main", func_0013F100);

INCLUDE_ASM(const s32, "core/main", func_0013F298);

INCLUDE_ASM(const s32, "core/main", func_0013F3E0);

INCLUDE_ASM(const s32, "core/main", func_0013F590);

INCLUDE_ASM(const s32, "core/main", func_0013F758);

INCLUDE_ASM(const s32, "core/main", func_0013F858);

s32 func_0013F910(s32 a1) {
return *(u8 *)(a1 + 9)
+ 60
* (*(u8 *)(a1 + 10)
+ 60
* (*(u8 *)(a1 + 11)
+ 24
* (*(u8 *)(a1 + 12)
+ 31 * (*(u8 *)(a1 + 13) + 12 * (*(u16 *)(a1 + 14) - 2000)))));
}

s32 func_0013F968(void)
{
    s32 i;
    s32 r = 0;
    u32 *p = (u32 *)D_00331158;

    for (i = 0; i < 3; i++)
    {
        if (p[0] != 0)
        {
            if (p[1] != 0)
                r++;
        }
        p += 3;
    }
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_0013F9A8);

INCLUDE_ASM(const s32, "core/main", func_0013FA48);

INCLUDE_ASM(const s32, "core/main", func_0013FAE8);

INCLUDE_ASM(const s32, "core/main", func_0013FD40);

INCLUDE_ASM(const s32, "core/main", func_0013FFA0);


/* Source: rendering/region_0014.c */
/* func_001400D0 through func_0014FE90; retail address range 0x00140000-0x00150000. */






s32 D_0034C3DC[8];
s32 D_00389AD0[8];
s32 D_00393314;

extern s32 D_003932D4;
extern u8 D_00355530[8];

extern s32 D_00393250;
extern u8 D_003933D8;

extern s32 D_00393398;

INCLUDE_ASM(const s32, "core/main", func_001400D0);

INCLUDE_ASM(const s32, "core/main", func_00140190);

INCLUDE_ASM(const s32, "core/main", func_00140310);

INCLUDE_ASM(const s32, "core/main", func_00140810);

INCLUDE_ASM(const s32, "core/main", func_00140898);

INCLUDE_ASM(const s32, "core/main", func_00140948);

INCLUDE_ASM(const s32, "core/main", func_00140B78);

INCLUDE_ASM(const s32, "core/main", func_00140DC0);

u8 *func_00140FE0(u8 *p) { return p + 12; }

INCLUDE_ASM(const s32, "core/main", func_00140FE8);

INCLUDE_ASM(const s32, "core/main", func_00141210);

INCLUDE_ASM(const s32, "core/main", func_00141310);

s32 func_001413C8(void)
{
    D_00393218 = D_00393218 + 1;
    __asm__ volatile("sync");
    __asm__ volatile("ei");
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001413E8);

INCLUDE_ASM(const s32, "core/main", func_001415E8);

extern u8 D_0033129C[];
s32 func_00151CD8();
void func_001417A8(void)
{
    s32 *p = (s32 *)D_0033129C;
    s32 *end = (s32 *)((u8 *)p + 0xFF0);
    do {
        func_00151CD8(p, 100);
        p = (s32 *)((u8 *)p + 0xCC);
    } while ((s32)p < (s32)end);
}

void func_001417F8(void) { D_00393250 = 0; }

INCLUDE_ASM(const s32, "core/main", func_00141800);

INCLUDE_ASM(const s32, "core/main", func_00141A20);

INCLUDE_ASM(const s32, "core/main", func_00141BC0);

INCLUDE_ASM(const s32, "core/main", func_00141D20);

INCLUDE_ASM(const s32, "core/main", func_00141F00);

INCLUDE_ASM(const s32, "core/main", func_00142328);

INCLUDE_ASM(const s32, "core/main", func_00142450);

INCLUDE_ASM(const s32, "core/main", func_00142528);

INCLUDE_ASM(const s32, "core/main", func_00142C30);

INCLUDE_ASM(const s32, "core/main", func_00142DC8);

INCLUDE_ASM(const s32, "core/main", func_00142F28);

INCLUDE_ASM(const s32, "core/main", func_00143638);

INCLUDE_ASM(const s32, "core/main", func_001438B8);

INCLUDE_ASM(const s32, "core/main", func_00143B38);

INCLUDE_ASM(const s32, "core/main", func_00143DB8);

INCLUDE_ASM(const s32, "core/main", func_00144038);

void func_001442B8(float a, float b, float c, float d, float e)
{
    D_00393258 = a;
    /* Retail callers pass these in $f14/$f13, opposite the C parameter order. */
    D_0039325C = c;
    D_00393260 = b;
    D_00393264 = d;
    D_00393268 = e;
}

INCLUDE_ASM(const s32, "core/main", func_001442D0);

INCLUDE_ASM(const s32, "core/main", func_00144328);

INCLUDE_ASM(const s32, "core/main", func_00144380);

INCLUDE_ASM(const s32, "core/main", func_001443D8);

INCLUDE_ASM(const s32, "core/main", func_00144430);

INCLUDE_ASM(const s32, "core/main", func_00144488);

INCLUDE_ASM(const s32, "core/main", func_00144598);

INCLUDE_ASM(const s32, "core/main", func_00144670);

INCLUDE_ASM(const s32, "core/main", func_00144798);

s32 func_00144830(u32 arg0, u32 arg1)
{
    u32 p;
    u32 n;
    u32 q;

    if (arg0 == arg1)
        return 1;
    p = *(u32 *)(arg0 + 4);
    n = *(u32 *)(p + 0x24);
    q = *(u32 *)(p + 0x20);
    if (n != 0)
    {
        while (1)
        {
            if (arg1 == *(u32 *)(q + 4))
                return 1;
            n--;
            if (n == 0)
                break;
            q += 8;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00144870);

INCLUDE_ASM(const s32, "core/main", func_00144A40);

INCLUDE_ASM(const s32, "core/main", func_00144BF0);

extern volatile u32 D_00393288;
extern volatile u32 D_0039328C;

void func_00144C80(void)
{
    D_00393288 = 0;
    D_0039328C = 0;
}

/* Byte-matching; returning the callee's result avoids a sibling call. */
s32 func_00144C90(u32 arg0)
{
    return func_00145BB8((s32 *)&D_00393288, arg0 & 0xFFFF);
}

INCLUDE_ASM(const s32, "core/main", func_00144CB8);

u32 func_00144D98(void) { return 0x9D7C; }

u32 func_00144DA0(void) { return 0x9D7C; }

void func_00144DA8(void)
{
    u8 *base = D_0034A380;

    *(u16 *)(base + 0x2058) = 0;
    *(u16 *)(base + 0x205A) = 0;
    *(s32 *)(base + 0x10C) = 100;
    *(s32 *)(base + 0x110) = 100;
    *(s32 *)(base + 0x114) = 100;
}

INCLUDE_ASM(const s32, "core/main", func_00144DD0);

INCLUDE_ASM(const s32, "core/main", func_00144E40);

INCLUDE_ASM(const s32, "core/main", func_00144EB8);

typedef struct {
    char pad[0x205A];
    u16 val;
} BmS;

typedef struct {
    char pad[0x205A];
    u16 val;
} FF22BmS;

s32 func_00144F28(s32 a1, s32 a2, u32 a3)
{


    u8 *base;
    FF22BmS *s;
    u16 v;
    u32 rem;

    func_002249D4(a1, a2, a3);
    a1 = a1 + a3;
    rem = a3 & 3;
    if (rem != 0) {
        s32 t = a1 + 4;
        a1 = t - rem;
    }
    base = D_0034C3FC;
    s = (FF22BmS *)(base - 0x207C);
    v = (u16)(a1 - (s32)base);
    if (s->val < v)
        s->val = v;
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_00144F98);

void func_00144FE8(s32 arg)
{
    func_002249D4(arg + (s32)D_0034C3DC);
}

void func_00144FF8(s32 arg, s32 other)
{
    func_002249D4(other, arg + (s32)D_0034C3DC);
}

u32 func_00145010(s16 arg0)
{
    u8 *p = (u8 *)D_00393294;

    while (p != 0)
    {
        if (*(s16 *)(p + 0x68) == arg0)
            return (u32)p;
        p = (u8 *)*(u32 *)p;
    }
    return 0;
}

u8 *func_00145048(u8 *p)
{
    if (p != 0) return p + 0x4B;
    else return 0;
}

u16 func_00145060(u8 *p)
{
    return p ? *(u16 *)(p + 0x48) : 0;
}

u8 *func_00145078(u8 *p)
{
    if (p != 0) return p + 0x5C;
    else return 0;
}

s32 func_00145090(u8 *p)
{
    return p ? *(s32 *)(p + 0x40) : 0;
}

INCLUDE_ASM(const s32, "core/main", func_001450A8);

s32 func_00145168(u8 *arg0)
{
    s32 v = func_00144D98();
    if (func_001454F8(v) != 0)
        return func_001456F0((s32)arg0, (s32)D_0034A380, 0x9D7C, 0x9D7C);
    return 0;
}

s32 func_001451C0(u8 *arg0)
{

    if (func_00145560(arg0 + 0x40) != 0) {
        return func_00145790((s32)arg0, (s32)D_0034A380, 0x9D7C);
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00145210);

s32 func_00145378(s32 a1) {
bool v2;
s32 result;
v2 = func_00145560(a1 + 64) == 0;
result = 0;
if ( !v2 )
return func_00145808(a1);
return result;
}

/* Byte-matching; returning the callee's result avoids a sibling call. */
s32 func_001453B8(void)
{
    return func_001454F8(func_00144DA0());
}

void func_001453E0(s32 arg, s32 value)
{
    func_00226310(arg, (s32)D_00389AD0, value);
}

void func_001453F0(s32 arg, s32 value) { func_001453E0(arg, value); }

void func_001453F8(u8 *arg0, u32 unused0)
{
    u8 buf[0x70];

    ((void (*)())func_001453E0)(buf);
    func_00226310(arg0, D_003932A0, buf, buf);
}

s32 func_00145438()
{
    return func_001454F8() != 0 ? 0 : -1;
}

s32 func_00145460(void)
{
    s32 v;
    if (func_001454A0() != 0) {
        v = func_001454C8();
        if (v == 0) return 0;
    }
    return -1;
}

s32 func_001454A0(void)
{
    return (u32)func_0013FFA0(0, 0, 0) > 0;
}

s32 func_001454C8(void)
{
    return (u32)(func_0013FFA0(0, 0, 0) ^ 2) < 1;
}

s32 func_001454F8(s32 a1)
{
    s32 v5[4];
    s32 v2;
    s32 v3;
    v2 = 0;
    if (func_0013FFA0(0, 0, v5) == 2) {
        v3 = func_00140810(0, 0, a1);
        v2 = v5[0] >= v3;
    }
    return v2;
}

INCLUDE_ASM(const s32, "core/main", func_00145560);

INCLUDE_ASM(const s32, "core/main", func_001455E0);

s32 func_001456F0(s32 a1, char *a2, s32 a3, s32 a4)
{


    s32 r;

    if (func_00145560(a1 + 0x40) != 0) {
        r = func_001455E0(a1, a2, a3, 0);
        goto done;
    }
    if (func_00145438(a4) == -1)
        goto zero;
    r = func_001455E0(a1, a2, a3, 1);
    goto done;
zero:
    r = 0;
done:
    return r;
}

/* Byte-matching.  Returning the callee's result keeps gcc from turning this
   into a sibling call: retail emits jal + epilogue through a saved $ra. */
s32 func_00145790(s32 a, s32 b, s32 c)
{
    return func_001456F0(a, b, c, c);
}

s32 func_001457B0(s32 a1, s32 a2, s64 a3)
{

    u8 v6[256];

    func_001453F8(v6, (u8 *)a1 + 82);
    return func_0013FAE8(0, 0, (s32)v6, a2, a3) != 0;
}

s32 func_00145808(u8 *arg0)
{
    u8 buf[0x100];

    func_001453F0(buf, arg0 + 0x52);
    return (u32)func_00140948(0, 0, buf) > 0;
}

INCLUDE_ASM(const s32, "core/main", func_00145840);

INCLUDE_ASM(const s32, "core/main", func_00145958);

s32 func_001459C0(void)
{
    s32 r;
    if (func_00145460() != -1)
        r = (u32)func_0013F9A8(0, 0) > 0;
    else
        r = 0;
    return r;
}

/* Byte-matching; returning the callee's result avoids a sibling call. */
s32 func_00145A00(void)
{
    return func_00140810(0, 0, 0x9D7C);
}

s32 func_00145A28(s32 arg0)
{
    arg0 &= 0x3FF;
    if ((arg0 & 0xC0) != 0)
        arg0 |= 4;
    if ((arg0 & 0x300) != 0)
        arg0 |= 8;
    return arg0;
}

u32 func_00145A50(u32 *arg0, u32 arg1, u16 arg2)
{
    u32 *p = (u32 *)*arg0;

    while (p != 0)
    {
        if (p[1] == arg1 && (*(u16 *)((u8 *)p + 0x10) & 0x3FF) == arg2)
            return (u32)p;
        p = (u32 *)*p;
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00145A90);

s32 func_00145BB8(s32 *a1, u16 a2)
{

    s32 v2;
    s32 v3;
    s32 i;

    v2 = *a1;
    v3 = a2;
    if (*a1 != 0) {
        for (i = *(u16 *)(v2 + 16); ; i = *(u16 *)(v2 + 16)) {
            if ((i & v3) == v3)
                func_00145A90(v2);
            v2 = *(s32 *)v2;
            if (v2 == 0)
                break;
        }
    }
    return 1;
}

s32 func_00145C10(u32 *arg0, u32 arg1, u16 arg2)
{
    u32 node = func_00145A50(arg0, arg1, arg2);
    s32 r = 0;

    if (node != 0)
    {
        r = 1;
        func_00145A90(node);
    }
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_00145C50);

INCLUDE_ASM(const s32, "core/main", func_00145EC0);

void func_00145F60(u8 *arg0, u8 *arg1)
{
    *(S12 *)arg1 = *(S12 *)(arg0 + 0x48);
}

void func_00145F80(u8 *arg0, u8 *arg1)
{
    *(S12 *)arg1 = *(S12 *)(arg0 + 0x54);
}

void func_00145FA0(f32 *a1, f32 *out)
{
    f32 v2 = a1[18] + a1[21];
    f32 v3, v4, v5;

    out[0] = v2;
    v2 = v2 * 0.5f;
    v3 = a1[19] + a1[22];
    out[1] = v3;
    v3 = v3 * 0.5f;
    v5 = a1[20];
    v4 = a1[23];
    out[0] = v2;
    out[1] = v3;
    out[2] = (v5 + v4) * 0.5f;
}

void *func_00145FF0(void)
{
    u8 *p = (u8 *)func_00157DE8(D_0039347C, 0x10);

    *(u32 *)(p + 4) = 0;
    *(u32 *)(p + 8) = 0;
    *(u32 *)(p + 0xC) = 0;
    return p;
}

void func_00146020(s32 *p, s32 value) { p[0] = value; }

void *func_00146028(void)
{
    u8 *p = (u8 *)func_00157DE8(D_0039347C, 0x10);

    *(u32 *)(p + 4) = 0;
    *(u32 *)(p + 8) = 0;
    *(u32 *)(p + 0xC) = 0;
    return p;
}

void func_00146058(s32 *p, s32 value) { p[0] = value; }

INCLUDE_ASM(const s32, "core/main", func_00146060);

INCLUDE_ASM(const s32, "core/main", func_00146118);

INCLUDE_ASM(const s32, "core/main", func_001462A8);

/* Byte-matching: linked-list unlink with refcount; the distinct cmp local
   keeps the compared value in its own register, and the barriers pin the
   re-reads after the decrement store. */
s32 func_001463D0(u8 *a1)
{
    s32 result;
    s32 cmp;

    if (*(s32 *)(a1 + 4) != 0) { result = *(s32 *)(a1 + 8); *(s32 *)(*(s32 *)(a1 + 4) + 8) = result; }
    if (*(s32 *)(a1 + 8) != 0) { result = *(s32 *)(a1 + 4); *(s32 *)(*(s32 *)(a1 + 8) + 4) = result; }
    if (*(s32 *)(a1 + 0xC) != 0)
    {
        *(s32 *)(*(s32 *)(a1 + 0xC) + 8) -= 1;
        __asm__ volatile("" ::: "memory");
        if (**((s32 **)(a1 + 0xC)) == (s32)a1)
        {
            **(s32 **)(a1 + 0xC) = *(s32 *)(a1 + 4);
            __asm__ volatile("" ::: "memory");
        }
        cmp = (*(s32 **)(a1 + 0xC))[1];
        if (cmp == (s32)a1)
        {
            result = *(s32 *)(a1 + 8);
            (*(s32 **)(a1 + 0xC))[1] = result;
        }
        else
            result = cmp;
        *(s32 *)(a1 + 0xC) = 0;
    }
    else *(s32 *)(a1 + 0xC) = 0;
    *(s32 *)(a1 + 8) = 0;
    *(s32 *)(a1 + 4) = 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00146450);

INCLUDE_ASM(const s32, "core/main", func_00146518);

/* Byte-matching: twin of func_001463D0. */
s32 func_001465E0(u8 *a1)
{
    s32 result;
    s32 cmp;

    if (*(s32 *)(a1 + 4) != 0) { result = *(s32 *)(a1 + 8); *(s32 *)(*(s32 *)(a1 + 4) + 8) = result; }
    if (*(s32 *)(a1 + 8) != 0) { result = *(s32 *)(a1 + 4); *(s32 *)(*(s32 *)(a1 + 8) + 4) = result; }
    if (*(s32 *)(a1 + 0xC) != 0)
    {
        *(s32 *)(*(s32 *)(a1 + 0xC) + 8) -= 1;
        __asm__ volatile("" ::: "memory");
        if (**((s32 **)(a1 + 0xC)) == (s32)a1)
        {
            **(s32 **)(a1 + 0xC) = *(s32 *)(a1 + 4);
            __asm__ volatile("" ::: "memory");
        }
        cmp = (*(s32 **)(a1 + 0xC))[1];
        if (cmp == (s32)a1)
        {
            result = *(s32 *)(a1 + 8);
            (*(s32 **)(a1 + 0xC))[1] = result;
        }
        else
            result = cmp;
        *(s32 *)(a1 + 0xC) = 0;
    }
    else *(s32 *)(a1 + 0xC) = 0;
    *(s32 *)(a1 + 8) = 0;
    *(s32 *)(a1 + 4) = 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00146660);

s32 func_00146730(u8 *p)
{
    return *(u8 *)(*(u32 *)(p + 4) + 0x64);
}

s32 func_00146740(u8 *p)
{
    return *(s8 *)(*(u32 *)(p + 4) + 0x65);
}

f32 func_00146750(u8 *p)
{
    return *(f32 *)(*(u32 *)(p + 4) + 0x60);
}

u8 *func_00146760(u8 *p) { return p + 72; }

u8 *func_00146768(u8 *p) { return p + 84; }

void func_00146770(void)
{
    D_003932B4 = 1;
}

void func_00146780(u32 arg0, u32 arg1)
{
    u32 p = *(u32 *)(arg1 + 0x14);

    while (p != 0)
    {
        func_0014B1F8(*(u32 *)p);
        p = *(u32 *)(p + 4);
    }
}

INCLUDE_ASM(const s32, "core/main", func_001467B8);

extern u16 func_00119700(const u16 *p);

s32 func_00146850(u32 *arg0, u32 *arg1, u32 arg2, u32 *arg3)
{
    u8 *p;
    u32 *list;
    u32 v;
    u32 j;
    s32 found;
    s32 pos;
    s32 cur;

    if (arg0[0] == 4)
    {
        p = (u8 *)arg0[1];
        list = *(u32 **)(p + 0xC);
        if (*(s32 *)(p + 0x10) > 0)
        {
            s32 type = 6;

            pos = 0x10000;
            do
            {
                v = *list;
                if (func_00119700((const u16 *)v) != type)
                {
                    found = 0;
                    for (j = 0; j < *arg3; j++)
                        if (v == arg1[j])
                        {
                            found = 1;
                            break;
                        }
                    if (found == 0)
                    {
                        arg1[*arg3] = v;
                        (*arg3)++;
                        if (*arg3 >= arg2)
                            return 0;
                    }
                }
                cur = pos;
                pos += 0x10000;
                list++;
            } while ((cur >> 16) < *(s32 *)(p + 0x10));
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00146980);

INCLUDE_ASM(const s32, "core/main", func_00146A78);

INCLUDE_ASM(const s32, "core/main", func_00146B40);

INCLUDE_ASM(const s32, "core/main", func_00146C00);

INCLUDE_ASM(const s32, "core/main", func_00146D40);

INCLUDE_ASM(const s32, "core/main", func_00146EB0);

INCLUDE_ASM(const s32, "core/main", func_00147128);

INCLUDE_ASM(const s32, "core/main", func_00147238);

INCLUDE_ASM(const s32, "core/main", func_001473D8);

INCLUDE_ASM(const s32, "core/main", func_001474D0);

extern s32 func_00224B88();

void func_001475D8(s32 arg)
{
    func_00224B88(arg, 0, 0x10);
}

void func_001475E8(u8 *arg0)
{
    func_00224B88(arg0, 0, 0x10);
    *(u32 *)arg0 = 1;
}

void func_00147620(u8 *arg0)
{
    *(u32 *)arg0 = 2;
    *(f32 *)(arg0 + 4) = 5.0f;
    *(f32 *)(arg0 + 8) = 6.0f;
}

void func_00147648(u8 *arg0, s32 arg1)
{
    if (arg1 != 0)
        *(u32 *)arg0 |= 0x100;
    else
        *(u32 *)arg0 &= 0xFFFFFEFF;
}

s32 func_00147678(u8 *p)
{
    return p[0] != 0;
}

s32 func_00147688(const u8 *p) { return *p == 2; }

s32 func_00147698(const u8 *p) { return *p == 1; }

void func_001476A8(u8 *arg0, f32 arg1)
{
    if (func_00147688(arg0) != 0)
        *(f32 *)(arg0 + 4) = arg1;
}

void func_001476E0(u8 *arg0, f32 arg1)
{
    if (func_00147688(arg0) != 0)
        *(f32 *)(arg0 + 8) = arg1;
}

f32 func_00147718(u8 *arg0)
{
    if (func_00147688(arg0) == 0)
        return 0.0f;
    return *(f32 *)(arg0 + 4);
}

f32 func_00147748(u8 *arg0)
{
    if (func_00147688(arg0) == 0)
        return 0.0f;
    return *(f32 *)(arg0 + 8);
}

u32 func_00147778(u8 *arg0)
{
    if (func_00147688(arg0) != 0)
        return *(u8 *)(arg0 + 0xC);
    return 0;
}

void func_001477A8(u8 *arg0, u8 arg1)
{
    if (func_00147688(arg0) != 0)
        *(u8 *)(arg0 + 0xC) = arg1;
}

void func_001477E0(u8 *arg0, s32 arg1)
{
    if (arg1 != 0)
        *(u32 *)arg0 |= 0x200;
    else
        *(u32 *)arg0 &= 0xFFFFFDFF;
}

s32 func_00147810(const s32 *p) { return (p[0] >> 9) & 1; }

void func_00147820(u32 arg0)
{
    if (arg0 < 0x14)
        D_00355298[0] = arg0;
    else
        D_00355298[0] = 0x14;
}

void func_00147848(u32 arg0)
{
    if (arg0 < 0x14)
        D_003552F0[0] = arg0;
    else
        D_003552F0[0] = 0x14;
}

s32 func_00147870(void)
{
    s32 v1[4];
    s32 v3[4];





    func_00119B50(v3);
    func_00119E40(v3[0], 0x45D803F6);
    v1[0] = 0;
    v1[1] = 0;
    v1[2] = 0;
    v1[3] = 0;
    func_00119D78(v3[0], v1);
    *(f32 *)&v1[3] = 255.0f;
    func_00119D68(v3[0], v1);
    return v3[0];
}

extern u8 D_00354200[];

void func_001478D8(void)
{
    func_00224B88((s32)&D_00354200, 0, 0x1144);
    *(s32 *)(D_00354200 + 0x1098) = 4;
    *(s32 *)(D_00354200 + 0x10F0) = 2;
}

s32 func_00147920(void)
{
    u8 *base = D_00354200;
    s32 result = func_00153D30(*(s32 *)(base + 0x10F0), *(s32 *)(base + 0x1098));
    *(s32 *)base = 0;
    *(s32 *)(base + 0x1094) = 0;
    *(s32 *)(base + 0x10EC) = 0;
    return result;
}

void func_00147968(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00147970);

INCLUDE_ASM(const s32, "core/main", func_00147A80);

INCLUDE_ASM(const s32, "core/main", func_00147AE8);

INCLUDE_ASM(const s32, "core/main", func_00147C30);

INCLUDE_ASM(const s32, "core/main", func_00147CF8);

INCLUDE_ASM(const s32, "core/main", func_00147E60);

f32 func_001480C8(s32 a1)
{


    f32 r;

    if ((func_0010C9F0(a1) & 0x200000) != 0)
        return func_00115FF0() * 255.0f;
    r = 255.0f;
    if ((*(s32 *)(a1 + 0x30) & 0x10000) != 0)
        r = *(f32 *)(a1 + 0x3C);
    return r;
}

f32 func_00148130(f32 *arg0, f32 *arg1, f32 arg2, f32 arg3, f32 arg4)
{
    return arg0[2] - arg2 * (arg1[0] * (arg3 - arg0[0]) + arg1[1] * (arg4 - arg0[1]));
}

INCLUDE_ASM(const s32, "core/main", func_00148168);

INCLUDE_ASM(const s32, "core/main", func_001488A8);

INCLUDE_ASM(const s32, "core/main", func_00148BD8);

INCLUDE_ASM(const s32, "core/main", func_00148D20);

INCLUDE_ASM(const s32, "core/main", func_00148EC0);

INCLUDE_ASM(const s32, "core/main", func_00148F88);

INCLUDE_ASM(const s32, "core/main", func_001493C8);

INCLUDE_ASM(const s32, "core/main", func_00149618);

u32 func_00149700(u8 *arg0)
{
    u32 r = 0;

    if (*(u32 *)arg0 == 0x400)
        r = *(u32 *)(arg0 + 4);
    else if ((*(u32 *)arg0 & 8) != 0)
        r = *(u32 *)(*(u32 *)(arg0 + 4));
    return r;
}

u8 *func_00149738(void)
{
    u8 *p = (u8 *)func_00157DE8(D_0039347C, 0x54);

    func_00120D48(p);
    return p;
}

void func_00149770(void)
{
    D_00355448[0] = (s32)func_00149738();
    D_003932C4 = 1;
}

void func_001497A0(void)
{
    func_00120D48(D_00355448[0]);
    D_003932C4 = 1;
    D_00393090 = D_00355448[0];
}

void func_001497D8()
{
    s32 i = D_003932C4;
    if (i != 0x32)
    {
        s32 v = D_00355448[0];
        D_00355448[i] = v;
        D_00393090 = v;
        D_003932C4 = i + 1;
    }
}

void func_00149810(void)
{
    s32 i = D_003932C4;

    if (i != 0)
    {
        D_003932C4 = i - 1;
        D_00393090 = D_00355448[i - 2];
    }
}

void func_00149848(a1)
s32 a1;
{
    s32 i = D_003932C4;
    if (i != 0x32)
    {
        D_00355448[i] = a1;
        D_00393090 = a1;
        D_003932C4 = i + 1;
    }
}

s32 func_00149880(s32 arg1)
{
    return func_0014CBE8(D_0039347C, arg1);
}

void func_001498A8(void)
{
    func_00149880(D_00355448[0]);
}

void func_001498B8(void) { func_00149848(); }

s32 func_001498C0(s32 a1, s32 a2, s32 a3, s32 a4)
{





    f32 v12[4];
    s32 v8;
    f32 v9, v10;

    v8 = func_0020BFD8(a4);
    func_00121278((u32)v12, a1, v8);
    v9 = func_0020BFE0(a4);
    v10 = func_001278D0(a1);
    return func_0020F5A0(a2, a3, (u32)v12, v9 * v10);
}

INCLUDE_ASM(const s32, "core/main", func_00149950);

INCLUDE_ASM(const s32, "core/main", func_00149A90);

s32 func_0014A040(s32 a1, s32 a2, s32 a3, s32 a4)
{
    s16 v[8];
    s32 v8;

    v[0] = 0;
    func_001497D8();
    v8 = func_00149A90(a1, a2, a3, a4, (u32)v);
    func_00149810();
    return v8;
}

INCLUDE_ASM(const s32, "core/main", func_0014A0B0);

INCLUDE_ASM(const s32, "core/main", func_0014A298);

s32 func_0014A5E0(u32 value) { return value != 0; }

void func_0014A5E8(u8 *p, s32 key, s32 value)
{
    *(s32 *)(p + 0x4) = key;
    *(s32 *)p = key ? value : 1;
}

void func_0014A600(void)
{


    s32 n = (u16)D_003932C0 << 16;
    s32 i = 0;
    s32 pos;

    if (n > 0)
    {
        do
        {
            u8 *p = (u8 *)D_00355348[i];
            pos = (i + 1) << 16;
            i = (s16)(pos >> 16);
            *(u32 *)(p + 0x30) &= 0xFFFFFFDEu;
        } while (pos < n);
    }
    D_003932C0 = 0;
    D_003932CC = 0;
    D_003932C2 = 0;
}

s32 func_0014A660(s16 arg0)
{
    if (arg0 < D_003932C0)
        return D_00355348[arg0];
    return 0;
}

void func_0014A698(u8 *a1)
{
    s32 v = *(s32 *)(a1 + 0x30);

    if ((v & 0x20) == 0)
    {
        s16 c = D_003932C0;
        *(s32 *)(a1 + 0x30) = v | 0x21;
        if (c < 0x20)
        {
            u16 next = (u16)D_003932C0 + 1;
            D_00355348[c] = (s32)a1;
            D_003932C0 = (s16)next;
        }
    }
}

INCLUDE_ASM(const s32, "core/main", func_0014A6E8);

INCLUDE_ASM(const s32, "core/main", func_0014A788);

void func_0014A930(u8 *arg0, u8 arg1)
{
    if (arg1 != 0)
        *(u32 *)(arg0 + 0x30) |= 0x20000;
    else
        *(u32 *)(arg0 + 0x30) &= 0xFFFDFFFF;
}

/* Byte-matching: offset-shifted twin of func_001463D0 (+0x14/+0x18/+0x1C). */
s32 func_0014A968(u8 *a1)
{
    s32 result;
    s32 cmp;

    if (*(s32 *)(a1 + 0x14) != 0) { result = *(s32 *)(a1 + 0x18); *(s32 *)(*(s32 *)(a1 + 0x14) + 0x18) = result; }
    if (*(s32 *)(a1 + 0x18) != 0) { result = *(s32 *)(a1 + 0x14); *(s32 *)(*(s32 *)(a1 + 0x18) + 0x14) = result; }
    if (*(s32 *)(a1 + 0x1C) != 0)
    {
        *(s32 *)(*(s32 *)(a1 + 0x1C) + 0x10) -= 1;
        __asm__ volatile("" ::: "memory");
        if (*(s32 *)(*(s32 *)(a1 + 0x1C) + 8) == (s32)a1)
        {
            *(s32 *)(*(s32 *)(a1 + 0x1C) + 8) = *(s32 *)(a1 + 0x14);
            __asm__ volatile("" ::: "memory");
        }
        cmp = *(s32 *)(*(s32 *)(a1 + 0x1C) + 0xC);
        if (cmp == (s32)a1)
        {
            result = *(s32 *)(a1 + 0x18);
            *(s32 *)(*(s32 *)(a1 + 0x1C) + 0xC) = result;
        }
        else
            result = cmp;
        *(s32 *)(a1 + 0x1C) = 0;
    }
    else *(s32 *)(a1 + 0x1C) = 0;
    *(s32 *)(a1 + 0x18) = 0;
    *(s32 *)(a1 + 0x14) = 0;
    return result;
}

/* Byte-matching: variant of func_0014A968 with the first field cached in a
   named local and the final two zero stores in reversed source order. */
s32 func_0014A9E8(u8 *a1)
{
    s32 result;
    s32 cmp;
    s32 v14 = *(s32 *)(a1 + 0x14);

    if (v14 != 0) { result = *(s32 *)(a1 + 0x18); *(s32 *)(v14 + 0x18) = result; }
    if (*(s32 *)(a1 + 0x18) != 0) { result = *(s32 *)(a1 + 0x14); *(s32 *)(*(s32 *)(a1 + 0x18) + 0x14) = result; }
    if (*(s32 *)(a1 + 0x1C) != 0)
    {
        *(s32 *)(*(s32 *)(a1 + 0x1C) + 0x10) -= 1;
        __asm__ volatile("" ::: "memory");
        if (*(s32 *)(*(s32 *)(a1 + 0x1C) + 8) == (s32)a1)
        {
            *(s32 *)(*(s32 *)(a1 + 0x1C) + 8) = *(s32 *)(a1 + 0x14);
            __asm__ volatile("" ::: "memory");
        }
        cmp = *(s32 *)(*(s32 *)(a1 + 0x1C) + 0xC);
        if (cmp == (s32)a1)
        {
            result = *(s32 *)(a1 + 0x18);
            *(s32 *)(*(s32 *)(a1 + 0x1C) + 0xC) = result;
        }
        else
            result = cmp;
        *(s32 *)(a1 + 0x14) = v14;
    }
    else *(s32 *)(a1 + 0x14) = v14;
    *(s32 *)(a1 + 0x1C) = 0;
    *(s32 *)(a1 + 0x18) = 0;
    return result;
}

u8 *func_0014AA68(void)
{
    u8 *p = (u8 *)func_00157DE8(D_0039347C, 0x40);
    u8 *v;

    *(s32 *)(p + 0x08) = 0;
    *(s32 *)(p + 0x0C) = 0;
    *(s32 *)(p + 0x10) = 0;
    *(s32 *)(p + 0x14) = 0;
    *(s32 *)(p + 0x18) = 0;
    *(s32 *)(p + 0x1C) = 0;
    func_0014A5E8(p, 0, 1);
    *(s32 *)(p + 0x34) = 0;
    *(s32 *)(p + 0x38) = 0;
    *(s32 *)(p + 0x2C) = -1;
    v = (u8 *)func_00149738();
    *(s32 *)(p + 0x24) = (s32)v;
    func_00120D48(v);
    func_0014AC68(p);
    *(f32 *)(p + 0x3C) = 255.0f;
    func_0014A930(p, 0);
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0014AB00);

s32 func_0014ABD8(s32 a1) {
bool v2;
s32 result;
v2 = func_0014A5E0(a1) == 0;
result = a1;
if ( !v2 )
return *(s32 *)(a1 + 20);
return result;
}

u8 *func_0014AC08(u8 *arg0, s32 arg1)
{
    u8 *p = *(u8 **)(arg0 + 8);
    s32 i;

    for (i = 0; i < arg1; i++)
        p = *(u8 **)(p + 0x14);
    return p;
}

void func_0014AC40(void)
{
    s32 v = D_003932D0 + 1;

    D_003932D0 = v;
    if (v == -1)
        D_003932D0 = v + 1;
}

void func_0014AC68(u8 *arg0)
{
    if (*(s32 *)(arg0 + 0x28) != -1)
    {
        s32 v = D_003932D0;
        if (v != 0)
            *(s32 *)(arg0 + 0x28) = v - 1;
        else
            *(s32 *)(arg0 + 0x28) = -2;
    }
}

void func_0014AC98(s32 value)
{
    if (value != -1)
        D_003932D0 = value;
    else
        D_003932D0 = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0014ACB8);

void func_0014AD50(u8 *arg0)
{
    u8 buf[0x60];

    func_00121888(buf, *(u32 *)(*(u32 *)(arg0 + 0x1C) + 0x24));
    func_001224F8(*(u32 *)(arg0 + 0x20), buf, *(u32 *)(arg0 + 0x24));
}

INCLUDE_ASM(void, "core/main", func_0014AD90);

INCLUDE_ASM(const s32, "core/main", func_0014AF20);

INCLUDE_ASM(const s32, "core/main", func_0014AF88);

void func_0014B010(u8 *arg0)
{
    u8 *p;

    func_0014ACB8(arg0);
    for (p = *(u8 **)(arg0 + 8); p != 0; p = *(u8 **)(p + 0x14))
        func_0014B010(p);
    func_00149810();
}

void func_0014B050(u8 *arg0)
{
    u8 *p;

    func_0014AC68(arg0);
    for (p = *(u8 **)(arg0 + 8); p != 0; p = *(u8 **)(p + 0x14))
        func_0014B050(p);
}

INCLUDE_ASM(const s32, "core/main", func_0014B090);

void func_0014B100(void) { D_003932D4 = (s32)&D_00355530[0]; }

void func_0014B110(void)
{
    s32 *p = (s32 *)D_003932D4;

    *p = 0x06000000;
    p++;
    D_003932D8 = 0;
    D_003932D4 = (s32)p;
}

void func_0014B130(void)
{
    s32 *p = (s32 *)D_003932D4;

    *p = 0x07000000;
    p++;
    D_003932D8 = 0;
    D_003932D4 = (s32)p;
}

void func_0014B150(void)
{
    s32 *p = (s32 *)D_003932D4;

    *p = 0x05000000;
    p++;
    D_003932D8 = 0;
    D_003932D4 = (s32)p;
}

INCLUDE_ASM(const s32, "core/main", func_0014B170);

INCLUDE_ASM(const s32, "core/main", func_0014B1F8);

INCLUDE_ASM(const s32, "core/main", func_0014B258);

void func_0014B320(s32 arg0)
{
    s32 *p = (s32 *)D_003932D4;
    p[0] = 0x4000000;
    D_003932D4 = (s32)(p + 1);
    p++;
    *p = arg0;
    D_003932D4 = (s32)(p + 1);
}

INCLUDE_ASM(const s32, "core/main", func_0014B348);

INCLUDE_ASM(const s32, "core/main", func_0014B588);

INCLUDE_ASM(const s32, "core/main", func_0014B650);

INCLUDE_ASM(const s32, "core/main", func_0014B7F8);

void func_0014B880(void)
{
    s32 v = D_003932E4 - 1;

    D_003932E4 = v;
    if (v == 0 && D_003932E0 != 0)
    {
        func_0014C130(D_003932E0, 3);
        D_003932E0 = 0;
    }
}

/* Byte-matching; see func_001174C0 for why the result is returned. */
s32 func_0014B8C0(void) { return func_0014BAA0(); }

typedef struct FrameTimer {
    u8 unk00[8];
    f32 interval;
    f32 scaled_interval;
    s32 count;
} FrameTimer;

extern FrameTimer *D_003932E8;

f32 func_0014B8E0(void) { return *(f32 *)((u8 *)D_003932E8 + 0x30); }

s32 func_0014B8F0(void) { return *(s32 *)((u8 *)D_003932E8 + 0x14); }

f32 func_0014B900(void) { return *(f32 *)((u8 *)D_003932E8 + 0x2C); }

f32 func_0014B910() { return *(f32 *)((u8 *)D_003932E8 + 0x8); }

s32 func_0014B920(void) { return *(s32 *)((u8 *)D_003932E8 + 0x28); }

s32 func_0014B930(void) { return *(s32 *)((u8 *)D_003932E8 + 0x24); }

s32 func_0014B940(void)
{
    return func_0014BC50(D_003932E8);
}

s32 func_0014BC60(s32);

s32 func_0014B960(void)
{
    return func_0014BC60(D_003932E8);
}

s32 func_0014BBE8(s32);

s32 func_0014B980(void)
{
    return func_0014BBE8(D_003932E8);
}

s32 func_0014BCD8(s32);

s32 func_0014B9A0(void)
{
    return func_0014BCD8(D_003932E8);
}

INCLUDE_ASM(const s32, "core/main", func_0014B9C0);

s32 func_0014BFA0(s32);

s32 func_0014BA18(void)
{
    return func_0014BFA0(D_003932E8);
}

extern s32 func_0014BBA8(FrameTimer *, f32);

s32 func_0014BA38(f32 interval)
{
    return func_0014BBA8(D_003932E8, interval);
}

s32 func_0014BA58(void)
{
    return func_0014C018(D_003932E8);
}

s32 func_0014BA78();
s32 func_0014BEC8();

s32 func_0014BA78(s32 arg1)
{
    return func_0014BEC8(D_003932E8, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_0014BAA0);

s32 func_0014BB08(s32 a1, s32 a2)
{


    s32 *p = (s32 *)a1;
    s32 r;

    r = ((s32 (*)())func_0014B880)();
    *p = 0;
    if (a2 & 1)
        r = func_00220A58(a1);
    return r;
}

s32 func_0014BB50(u8 *arg0, s32 arg1, f32 farg)
{
    *(s32 *)(arg0 + 0x10) = arg1;
    *(s32 *)arg0 = func_0014B7F8(arg1);
    func_0014BBA8((FrameTimer *)arg0, farg);
    func_0014BBE8(arg0);
    return (s32)arg0;
}

s32 func_0014BBA8(FrameTimer *timer, f32 interval)
{
    s32 count = timer->count;
    f32 scaled_count;

    timer->interval = interval;
    if (count < 0) {
        count = (s32)(((u32)count & 1) | ((u32)count >> 1));
        scaled_count = (f32)count + (f32)count;
    } else {
        scaled_count = (f32)count;
    }
    timer->scaled_interval = scaled_count * interval;
    return count;
}

INCLUDE_ASM(const s32, "core/main", func_0014BBE8);

s32 func_0014BC50(s32 *p) { s32 value = p[5]; p[1] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_0014BC60);

INCLUDE_ASM(const s32, "core/main", func_0014BCD8);

INCLUDE_ASM(const s32, "core/main", func_0014BEC8);

s32 func_0014BFA0(s32 a1)
{
    u8 *p = (u8 *)a1;
    u64 v2, v4;
    u32 v3;
    s32 r;

    v2 = ((u64 (*)(s32))func_0014C1B8)(*(s32 *)p) - *(u64 *)(p + 0x18);
    v3 = ((u32 (*)(s32))func_0014C1A0)(*(s32 *)p);
    v4 = ((u64 (*)(u64, u64))func_0021CDD8)(v2, *(u32 *)(p + 0x10));
    r = (s32)((u64 (*)(u64, u64))func_0021CE38)(v4, v3);
    return *(s32 *)(p + 0x14) + r;
}

/* Byte-matching; returning the callee's result avoids a sibling call. */
s32 func_0014C018(u8 *p) { return func_0014C1B8(*(s32 *)p); }

s32 func_0014C038(void)
{

    *(volatile s64 *)&D_003932F8 = *(volatile s64 *)&D_003932F8 + 1;
    *(volatile u32 *)0x10000010 |= 0xA00;
    __asm__ volatile("sync");
    __asm__ volatile("ei");
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_0014C068);

INCLUDE_ASM(const s32, "core/main", func_0014C130);

/* Returns the sign-extended low half of the 64-bit value at p. */
s32 func_0014C1A0(u8 *p)
{
    return (s32)(*(u64 *)p);
}

INCLUDE_ASM(const s32, "core/main", func_0014C1B8);

/* Byte-matching; returning the callee's result avoids a sibling call. */
s32 func_0014C210(u8 *arg0)
{
    return ((s32 (*)())func_0021CE38)(func_0014C1B8(arg0), *(u64 *)arg0);
}

extern s32 D_003932F8;
extern s32 D_00393300;
extern s32 D_00393308;

void func_0014C240(void)
{
    volatile u32 *p = (volatile u32 *)0x10000010;
    volatile u32 *q = (volatile u32 *)0x10000000;
    s32 v;
    s32 v2;

    *p |= 7;
    *(volatile s64 *)&D_003932F8 = 0;
    v = D_00393300;
    *(volatile s64 *)&D_00393308 = 0;
    *p = v;
    v2 = v | 0x280;
    *q = 0;
    *p = v2;
}

s32 func_001C7308(s32);

void func_0014C280(void)
{
    D_00393314 = func_001C7308(0);
}

/* Stamper asm: cfc2 $2,$vi29 reads a COP2 (VU0) control register, which has
   no C form.  Kept as handwritten assembly under src/asm. */
ASM_FUNC(const s32, func_0014C2A0);

INCLUDE_ASM(const s32, "core/main", func_0014C2B8);

void func_0014C308(s32 arg)
{
    D_00393310 = arg;
    func_001C7658(D_00393314, arg);
}

s32 func_0014C320(void)
{
    return func_001C7BB8(D_00393314, 1, 0) & 0xFF;
}

void func_0014C348(void)
{
    func_001C7BB8(D_00393314, 0, 0);
}

s32 func_0014C358(void)
{
    *(u32 *)D_00393314 |= 0x100;
    return func_001C7C80(D_00393314, D_00393310, 1, 0) & 0xFF;
}

void func_0014C398(u32 address)
{
    __asm__ volatile("ctc2 %0, $vi27" : : "r"(address));
    __asm__ volatile("vnop");
    __asm__ volatile("vnop");
    __asm__ volatile("vcallmsr $vi27");
    __asm__ volatile("nop");
}

void func_0014C3B0(a1)
s32 a1;
{

    if (D_00393318 == 0)
    {
        func_0014C770(func_00223E58, func_00225720, func_00223E80, func_00224A84);
        D_00393318 = func_0014C788(0, a1, 0x80, 5);
        func_0014C770(func_0014C470, func_0014C498, func_0014C4C0, func_00224A84);
    }
}

s32 func_0014CD88(s32);

s32 func_0014C450(void)
{
    return func_0014CD88(D_00393318);
}

s32 func_0014C470();

s32 func_0014C470(s32 arg1)
{
    return func_0014C9A0(D_00393318, arg1);
}

s32 func_0014C498();
s32 func_0014CAC8();

s32 func_0014C498(s32 arg1, s32 arg2)
{
    return func_0014CAC8(D_00393318, arg1, arg2);
}

extern s32 D_00393318;
extern void func_0014CBE8(s32, s32);

void func_0014C4C0(s32 arg)
{
    func_0014CBE8(D_00393318, arg);
}

s32 func_0014C4D0(s32 a1, s32 a2)
{
    s32 result;
    u32 v3;
    result = 0;
    if ((*(u16 *)(a1 + 30) & 2) != 0)
    {
        v3 = *(u32 *)(a1 + 32) + (u32)a2;
        *(u32 *)(a1 + 32) = v3;
        if (*(u32 *)(a1 + 36) < v3)
            *(u32 *)(a1 + 36) = v3;
        return ((s32 (*)(s32))D_0039331C)(a2);
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0014C528);

void func_0014C588(s32 arg0, u8 *arg1)
{
    *(u32 *)(arg1 + 8) = func_0014C528(arg0, *(u32 *)(arg1 + 8));
}

INCLUDE_ASM(const s32, "core/main", func_0014C5B8);

void func_0014C770(s32 a, s32 b, s32 c, s32 d)
{
    D_0039331C = a;
    D_00393320 = b;
    D_00393324 = c;
    D_00393328 = d;
}

INCLUDE_ASM(const s32, "core/main", func_0014C788);

INCLUDE_ASM(const s32, "core/main", func_0014C8A8);

INCLUDE_ASM(const s32, "core/main", func_0014C938);

s32 func_0014C980(u8 *p, u32 addr)
{
    s32 t = *(s32 *)(p + 0x18);
    s32 result = 0;
    if (!(t < addr)) {
        t = (u32)(p + 0x28) < addr;
        result = t;
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0014C9A0);

INCLUDE_ASM(const s32, "core/main", func_0014CAC8);

INCLUDE_ASM(const s32, "core/main", func_0014CBE8);

INCLUDE_ASM(const s32, "core/main", func_0014CD88);

INCLUDE_ASM(const s32, "core/main", func_0014CDD8);

s32 func_0014CE28(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_0014CE30(s32 arg0, u8 *arg1)
{
    if (func_0014C980(arg0, arg1) != 0)
        return *(u32 *)(arg1 - 4) - (s32)arg1;
    return 0;
}

s32 func_0014CE68(u32 *a1, u32 a2)
{
    u32 v = 0;

    if (a1[2] != 0)
        v = a1[0];
    while (v != 0 && (u32)v < a2)
    {
        v = *(u32 *)v;
        if (v == (u32)a1)
            v = 0;
    }
    return v ? (s32)(v + 12) : 0;
}

s32 func_0014CEA8(u32 *a1, u32 *a2) {
s32 v2;
s32 v3;
s32 result;
v2 = *a1;
a2[1] = a1;
*a2 = v2;
v3 = (s32)a1[2];
if ( a1[2] != 0 )
{
*(u32 *)(*a1 + 4) = a2;
v3 = (s32)a1[2];
}
*a1 = a2;
if ( v3 == 0 )
a1[1] = a2;
result = (s32)v3 + 1;
a1[2] = result;
return result;
}

void func_0014CEE8(u8 *arg0, u8 *arg1)
{
    *(u32 *)arg1 = (u32)arg0;
    *(u32 *)(arg1 + 4) = *(u32 *)(arg0 + 4);
    *(u32 *)(*(u32 *)(arg0 + 4)) = (u32)arg1;
    *(u32 *)(arg0 + 4) = (u32)arg1;
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) + 1;
}

void func_0014CF10(s32 *p) { p[0] = (s32)p; p[1] = (s32)p; p[2] = 0; }

void func_0014CF20(u32 *node, u32 *list, u8 flag)
{
    if (node[2]) {
        ((u32 *)list[1])[0] = list[0];
        ((u32 *)list[0])[1] = list[1];
        if (flag) {
            list[0] = 0;
            list[1] = 0;
        }
        node[2]--;
    }
}

void func_0014CF68(u8 *arg0, u8 *arg1, u8 *arg2)
{
    if (arg1 == 0)
    {
        func_0014CEA8(arg0, arg2);
        return;
    }
    *(u32 *)(*(u32 *)arg1 + 4) = (u32)arg2;
    *(u32 *)arg2 = *(u32 *)arg1;
    *(u32 *)arg1 = (u32)arg2;
    *(u32 *)(arg2 + 4) = (u32)arg1;
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) + 1;
}

void func_0014CFA0(u8 *arg0, u8 *arg1, u8 *arg2)
{
    if (arg1 == 0)
    {
        func_0014CEE8(arg0, arg2);
        return;
    }
    *(u32 *)(*(u32 *)(arg1 + 4)) = (u32)arg2;
    *(u32 *)(arg2 + 4) = *(u32 *)(arg1 + 4);
    *(u32 *)(arg1 + 4) = (u32)arg2;
    *(u32 *)arg2 = (u32)arg1;
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) + 1;
}

INCLUDE_ASM(const s32, "core/main", func_0014CFD8);

void func_0014D128(void)
{
    if (D_00393908 == 0)
        func_0014CFD8(((u32)D_0039313C * (u32)D_00393140) >> 6, 0);
    else
        func_0014CFD8(0, ((u32)D_0039313C * (u32)D_00393140) >> 6);
}

INCLUDE_ASM(const s32, "core/main", func_0014D160);

INCLUDE_ASM(const s32, "core/main", func_0014D2B8);

INCLUDE_ASM(const s32, "core/main", func_0014D3D0);

INCLUDE_ASM(const s32, "core/main", func_0014D5B0);

INCLUDE_ASM(const s32, "core/main", func_0014D6C8);

s32 func_0014D7A0(void)
{






    s32 r;

    func_00151570();
    func_00195948();
    func_0014D3D0();
    func_00195948();
    func_0014D5B0();
    func_00195948();
    func_0014D6C8();
    r = func_00195948();
    D_00393358 = 1;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_0014D800);

INCLUDE_ASM(const s32, "core/main", func_0014D8E8);

INCLUDE_ASM(const s32, "core/main", func_0014D990);

extern u8 D_00393358;
void func_0014D800();
void func_0014D8E8();
void func_0014D990();
void func_00196548();
void func_0014F620();
void func_0014DA10(void)
{
    if (D_00393358 != 0) {
        func_0014D800();
        func_0014D8E8();
        func_0014D990();
        func_00196548();
        func_0014F620();
        D_00393358 = 0;
    }
}

s32 func_0014DA58();
s32 func_0016BDC0();

s32 func_0014DA58(s32 arg1)
{
    func_0016BDC0(arg1);
    return arg1;
}

INCLUDE_ASM(const s32, "core/main", func_0014DA80);

s32 func_0014DC28(s32 *src, s32 *dst) { *dst = (s32)src; return 0; }

INCLUDE_ASM(const s32, "core/main", func_0014DC38);

INCLUDE_ASM(const s32, "core/main", func_0014DC98);

INCLUDE_ASM(const s32, "core/main", func_0014DCE0);

INCLUDE_ASM(const s32, "core/main", func_0014DD30);

INCLUDE_ASM(const s32, "core/main", func_0014DEC0);

void func_0014E030(u8 *a1)
{



    s16 v = *(s16 *)(a1 + 0x20);

    if (v < 0x401)
    {
        func_0014DD30(*(s32 *)(a1 + 0x24), (s32)D_0035F690, v);
        func_0014DEC0(*(s32 *)(a1 + 0x1C), (s32)D_0035F690);
    }
}

INCLUDE_ASM(const s32, "core/main", func_0014E098);

INCLUDE_ASM(const s32, "core/main", func_0014E388);

INCLUDE_ASM(const s32, "core/main", func_0014E768);

void func_0014E7D8(u8 *p)
{
    u8 *q = *(u8 **)(p + 0xE0);

    if (q != 0)
        func_0016EFA8(q);
}

u8 *func_0016F028(s32, f32);

void func_0014E7F8(s32 *arg1)
{
    if (arg1[0x38] != 0) {
        func_0016F028(arg1[0x38], *(f32 *)((u8 *)arg1 + 0x10));
    }
}

u8 *func_0016EF58(s32, f32);

void func_0014E818(s32 *arg1)
{
    if (arg1[0x38] != 0) {
        func_0016EF58(arg1[0x38], *(f32 *)((u8 *)arg1 + 0x4));
    }
}

extern s32 D_00393390;
extern s32 D_00393394;

void func_0014E838(void) { D_00393390 = 0; D_00393394 = 0; }

INCLUDE_ASM(const s32, "core/main", func_0014E848);

void func_0014E988(s32 arg0)
{
    if (D_00393388 != 0)
    {
        func_0016EFA8(D_00393388, arg0);
        func_0016EF38(D_00393388, 1);
    }
}

extern s32 D_00393388;
u8 *func_0016EF38(u8 *p, u8 value);

void func_0014E9C8(void)
{
    if (D_00393388 != 0) {
        func_0016EF38((u8 *)D_00393388, 0);
    }
}

void func_0014E9E8(u8 *p)
{
    p[0x40] = 0;
    func_00224B88(p, 0, 0x40);
}

void func_0014EA00(s32 value) { D_00393398 = value; }

INCLUDE_ASM(const s32, "core/main", func_0014EA08);

extern s32 D_0026DFB8[];
extern s32 D_00393098;
extern s32 D_00393398;

void func_0014EB30(u8 *arg0)
{
    s32 i;
    u32 *src;
    u32 *dst;
    arg0[0x40] = 0;
    src = D_0026DFB8;
    dst = (u32 *)arg0;
    i = 0;
    for (i = 0; i < D_00393098; i++, src++) {
        if (((D_00393398 >> i) & 1) && arg0[0x40] < 0x10) {
            *dst++ = *src;
            arg0[0x40]++;
        }
    }
}

INCLUDE_ASM(const s32, "core/main", func_0014EBA8);

void func_0014EC40(void)
{
    func_001755B0(D_003933DC, (s32)func_0014EBA8, 0);
}

INCLUDE_ASM(const s32, "core/main", func_0014EC58);

typedef struct { u32 v; } __attribute__((packed)) S4;
extern u8 D_003933A8[];

s32 func_0014ED50(void)
{
    S4 val;
    s32 v;

    val = *(S4 *)D_003933A8;
    v = func_0016F780();
    func_0016F938(v, &val);
    return v;
}

void func_0014EDA0(void) { func_0016F830(); }

void func_0014EDA8(s32 a1, s32 a2)
{



    s32 v3, v4, v5;

    v3 = func_0016F780(a1, a2);
    v4 = func_0016F950(a2);
    func_0016F938(v3, v4);
    v5 = func_0016F930(a2);
    func_0016F8D8(v3, v5);
}

INCLUDE_ASM(const s32, "core/main", func_0014EE00);

void func_0014EF50(u8 *arg0)
{
    u8 *p;
    u32 v;
    u32 a = 0;

    if (arg0 == 0)
        return;
    v = *(u32 *)(arg0 + 0x20);
    if (v == 0)
        return;
    p = *(u8 **)arg0;
    if (p != 0)
        a = *(u32 *)p;
    func_0016F8D8(v, a);
}

INCLUDE_ASM(const s32, "core/main", func_0014EF80);

INCLUDE_ASM(const s32, "core/main", func_0014F000);

s32 func_0014F170(void)
{

    s32 v;
    if (D_003933A0 == 0)
        D_003933A0 = func_00195BE8(0, 0, 0, 4);
    v = func_00199A30(D_003933A0);
    func_00199BD0(v, 2);
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_0014F1C8);

INCLUDE_ASM(const s32, "core/main", func_0014F2E0);

extern s32 D_00362690[];
s32 func_0014F5A8(s32 a1)
{
    s32 arg = a1;
    s32 v1;
    s32 *v3;
    u32 v4;
    s32 i;
        v1 = 0;
    v3 = D_00362690;
    v4 = 0;
    for (;;) {
        i = *v3;
        if (i != 0) v1 = func_00199CA0(i, arg);
        v4++;
        v3++;
        if (v4 >= 6 || v1 != 0) break;
    }
    return v1;
}

INCLUDE_ASM(const s32, "core/main", func_0014F620);

void func_0014F720(void) { D_003933D8 = 0; }

void func_0014F728(void) { D_003933D8 = 1; }

INCLUDE_ASM(const s32, "core/main", func_0014F738);

INCLUDE_ASM(const s32, "core/main", func_0014F7A8);

INCLUDE_ASM(const s32, "core/main", func_0014F808);

extern s32 D_003933D4;

extern s32 D_0038A048[];

extern f32 D_00392874;

extern s32 D_00394DF0[];

extern f32 D_00394DF4[];

extern f32 D_00394DF8[];

extern f32 D_00394DFC[];

extern s32 D_003D3D30[];

extern s32 D_003D7D80[];

extern s32 D_00393454;

extern s32 D_00393458;

extern s32 D_0039345C;

extern s32 D_00393464;

extern s32 D_00393468;

extern s32 D_0039346C;

void func_0014F8E8(s32 arg1)
{
    D_003933D4 = func_0014C788(0, arg1, 16, 5);
}

void func_0014F918(void)
{
    func_0014C938(&D_003933D4);
    D_003933D4 = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0014F940);

INCLUDE_ASM(const s32, "core/main", func_0014FBD0);

INCLUDE_ASM(const s32, "core/main", func_0014FD18);

INCLUDE_ASM(const s32, "core/main", func_0014FDB0);

INCLUDE_ASM(const s32, "core/main", func_0014FE90);


/* Source: rendering/region_0015.c */
/* func_001500C8 through func_00158A00; retail address range 0x00150000-0x00158B28. */





s32 D_00393130;

extern s32 D_003933EC;

extern s32 D_00393438;

INCLUDE_ASM(const s32, "core/main", func_001500C8);

INCLUDE_ASM(const s32, "core/main", func_00150218);

void func_00150298(s32 *a1)
{
    s32 v2;
    s32 v3;

    v2 = func_00195BE8(8, 8, 32, 1284);
    *a1 = v2;
    v3 = func_00196190(v2, 0, 1);
    func_00224B88(v3, 0x60, 0x100);
    func_001961D0(*a1);
}

s32 func_001502F8(s32 value) { return value; }

s32 func_00150300(s32 value) { return value; }

u8 *func_00150308(u8 *arg1)
{
    *(u32 *)(arg1 + D_003933EC) = 0xFFFFFFFF;
    return arg1;
}

s32 func_00150328(s32 value) { return value; }

s32 func_00150330(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_00150338);

INCLUDE_ASM(const s32, "core/main", func_001503B0);

void func_00150428(u8 *p, s32 value) { *(s32 *)(p + D_003933EC) = value; }

s32 func_00150438(const u8 *p) { return *(const s32 *)(p + D_003933EC); }

INCLUDE_ASM(const s32, "core/main", func_00150448);

INCLUDE_ASM(const s32, "core/main", func_001507B8);

INCLUDE_ASM(const s32, "core/main", func_001509F0);

extern u8 D_00362780[];

u8 *func_001512E8(void) { return D_00362780; }

INCLUDE_ASM(const s32, "core/main", func_001512F8);

INCLUDE_ASM(const s32, "core/main", func_00151418);

INCLUDE_ASM(const s32, "core/main", func_00151570);

INCLUDE_ASM(const s32, "core/main", func_001516A8);

INCLUDE_ASM(const s32, "core/main", func_001517A0);

void func_00151848(void)
{






    if (D_003933F0 != 0) {
        func_00195CE0(D_003933F0);
        D_003933F0 = 0;
    }
    if (D_003933F4 != 0) {
        func_00195CE0(D_003933F4);
        D_003933F4 = 0;
    }
    if (D_003933F8 != 0) {
        func_00195CE0(D_003933F8);
        D_003933F8 = 0;
    }
    func_001517A0();
}

void func_001518A0(u8 *arg0)
{

    if (func_00178BE8(arg0, 0) == 0) {
        func_00211918(D_0038A048, arg0);
        return;
    }
    func_00211918(D_0038A080, arg0);
}

u32 func_001518F0(u32 arg0)
{
    if (func_00178BE8(arg0) == 0)
        func_00211918(D_0038A048, arg0);
    return arg0;
}

void func_00151930(void) { func_00178AE0(); }

s32 func_00151938(s32 arg0)
{
    s32 v = 0;

    func_00178B30(arg0, &v);
    return v;
}

extern s32 D_0039342C;

s32 func_00151960(s32 arg1)
{
    s32 result = 0;

    if (arg1 != 0) {
        result = arg1 == D_0039342C;
    }
    return result;
}

extern s32 D_00378900[];
extern u8 D_0022E820[];

void func_00151980(void)
{
    D_00378900[8] = (s32)D_0022E820;
    D_00378900[10] = (s32)D_0022E820;
}

void func_001519A0(void)
{







    D_00378900[1] = (s32)D_00234650;
    D_00378900[2] = (s32)D_00230B30;
    D_00378900[3] = (s32)D_00230B30;
    D_00378900[5] = (s32)D_0022F910;
    func_00211918((s32)D_0038A0B8);
    D_00393420 = 30.0f;
    D_00393424 = 190.0f;
}

INCLUDE_ASM(const s32, "core/main", func_00151A08);

INCLUDE_ASM(const s32, "core/main", func_00151B38);

INCLUDE_ASM(const s32, "core/main", func_00151C40);

extern s32 D_0039347C;

s32 func_00151CD8(u8 *a1, s32 a2)
{
    s32 v;

    *(s32 *)a1 = 0;
    v = func_00157DE8(D_0039347C, 0x10);
    if (v == 0)
        return 0;
    *(s32 *)a1 = v;
    return func_00151C40(v, 3 * a2);
}

void func_00151D38(s32 *p, s32 value) { p[3] = value; }

void func_00151D40(u32 arg0, f32 *arg1)
{
    f32 *p = (f32 *)D_00394DF0;
    f32 s = D_00392874;

    D_00394DF0[0] = (s32)arg0;
    *(u32 *)(arg0 + 4) = 0;
    p[1] = arg1[0] * s;
    p[2] = arg1[1] * s;
    p[3] = arg1[2] * s;
}

extern s32 D_00394DF0[];

void func_00151D80(void) { D_00394DF0[0] = 0; }

INCLUDE_ASM(const s32, "core/main", func_00151D90);

INCLUDE_ASM(const s32, "core/main", func_00151E70);

INCLUDE_ASM(const s32, "core/main", func_00151F08);

INCLUDE_ASM(const s32, "core/main", func_00151FB8);

s32 func_00152188(s32 *a1, f32 a2)
{
    S4 val;
    s32 v3;

    v3 = func_0016E380((u8 *)a1[6], 0);
    val = *(S4 *)func_0016F950((u8 *)v3);
    ((u8 *)&val)[3] = (s32)(a2 * 255.0f);
    return (s32)func_0016F938(v3, (const u8 *)&val);
}

INCLUDE_ASM(const s32, "core/main", func_00152200);

INCLUDE_ASM(const s32, "core/main", func_00152388);

INCLUDE_ASM(const s32, "core/main", func_00152468);

void func_00152500(void) { D_00393438 = 0; }

INCLUDE_ASM(const s32, "core/main", func_00152508);

INCLUDE_ASM(const s32, "core/main", func_00152928);

INCLUDE_ASM(const s32, "core/main", func_00152AC0);

INCLUDE_ASM(const s32, "core/main", func_001531D8);

INCLUDE_ASM(const s32, "core/main", func_001532F0);

INCLUDE_ASM(const s32, "core/main", func_001533D8);

void func_00153500(void)
{
    func_0018C4D8(D_00393130, 0, 0);
}

INCLUDE_ASM(const s32, "core/main", func_00153510);

INCLUDE_ASM(const s32, "core/main", func_001537B0);

INCLUDE_ASM(const s32, "core/main", func_00153920);

INCLUDE_ASM(const s32, "core/main", func_00153970);

INCLUDE_ASM(const s32, "core/main", func_00153B30);

INCLUDE_ASM(const s32, "core/main", func_00153C58);

INCLUDE_ASM(const s32, "core/main", func_00153D30);

void func_00153DF8(s32 a1, s32 a2)
{





    s32 v;
    func_00199AD0(a2);
    v = func_0018C608(a1);
    func_00195CE0(v);
    v = func_0018C5D8(a1);
    func_0018E6D0(v);
    func_0018C550(a1);
}

INCLUDE_ASM(const s32, "core/main", func_00153E48);

void func_00153EF8(void)
{
    func_00153E48(&D_00393454, &D_00393458, D_0039345C);
    func_00153E48(&D_00393464, &D_00393468, D_0039346C);
}

INCLUDE_ASM(const s32, "core/main", func_00153F30);

INCLUDE_ASM(const s32, "core/main", func_001541A0);

INCLUDE_ASM(const s32, "core/main", func_00154238);

INCLUDE_ASM(const s32, "core/main", func_00154400);

INCLUDE_ASM(const s32, "core/main", func_00154700);

INCLUDE_ASM(const s32, "core/main", func_001547A8);

INCLUDE_ASM(const s32, "core/main", func_00154848);

void func_00154A68(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{


    u8 *p = (u8 *)(a1 + D_00393990);
    s32 v = *(s32 *)(p + 0x3C);
    u32 w;

    if (v == 0) goto out;
    if (*(u8 *)(p + 0x34) == 0) goto out;
    w = *(u32 *)(p + 8);
    func_00154848(*(s32 *)(a1 + 0xC), *(s32 *)(a1 + 0x10), a2,
                  (w >> 14) & 0x3F, (w >> 20) & 0x3F, v);
    return;
out:
    return;
}

INCLUDE_ASM(const s32, "core/main", func_00154AB0);

INCLUDE_ASM(const s32, "core/main", func_00154AF8);

INCLUDE_ASM(const s32, "core/main", func_00154CA8);

INCLUDE_ASM(const s32, "core/main", func_00154F18);

INCLUDE_ASM(const s32, "core/main", func_00155090);

INCLUDE_ASM(const s32, "core/main", func_00155258);

INCLUDE_ASM(const s32, "core/main", func_00155578);

INCLUDE_ASM(const s32, "core/main", func_00155698);

INCLUDE_ASM(const s32, "core/main", func_001557B8);

INCLUDE_ASM(const s32, "core/main", func_001558C8);

void func_00155D00(void)
{
    func_00224B88((s32)D_003700D0, 0, 0x14);
}

INCLUDE_ASM(const s32, "core/main", func_00155D18);

INCLUDE_ASM(const s32, "core/main", func_00155D88);

INCLUDE_ASM(const s32, "core/main", func_00155F28);

INCLUDE_ASM(const s32, "core/main", func_00155F88);

INCLUDE_ASM(const s32, "core/main", func_001567B0);

void func_00156A30(s32 a1, s32 a2)
{


    s32 v;
    func_0019C0E8(a1, a2 << 2, &D_003D3CF0, 1);
    v = a2 * 6;
    func_0019B420(4, &D_003D2A30, v - 2);
    func_0019C198();
}

INCLUDE_ASM(const s32, "core/main", func_00156A80);

INCLUDE_ASM(const s32, "core/main", func_00156D50);

INCLUDE_ASM(const s32, "core/main", func_00156E48);

void func_001570F0(s32 a1, s32 a2)
{


    s32 v;
    func_0019C0E8(a1, a2 << 2, &D_003D3CF0, 1);
    v = a2 * 6;
    func_0019B420(4, &D_003D2A30, v - 2);
    func_0019C198();
}

INCLUDE_ASM(const s32, "core/main", func_00157140);

void func_00157510(s32 a1, s32 a2)
{


    s32 v;
    func_0019C0E8(a1, a2 << 2, &D_003D3CF0, 1);
    v = a2 * 6;
    func_0019B420(4, &D_003D2A30, v - 2);
    func_0019C198();
}

INCLUDE_ASM(const s32, "core/main", func_00157560);

s32 func_001575F0(u32 a1)
{
    switch (a1) {
    case 1024: return 10;
    case 512: return 9;
    case 256: return 8;
    case 128: return 7;
    case 64:  return 6;
    case 32:  return 5;
    case 16:  return 4;
    case 8:   return 3;
    case 4:   return 2;
    case 2:   return 1;
    default:  return 0;
    }
}

INCLUDE_ASM(const s32, "core/main", func_001576D8);

void func_00157710(s32 arg0, s32 arg1, u32 arg2)
{
    u64 buf[1];

    func_001BA580(arg0, (s32 *)((u8 *)buf + 4), (s32 *)buf);
    func_001576D8(buf[0], arg2);
}

/* Byte-matching: u64 bit-mask merge, two field probes and three calls. */
s32 func_00157748(s32 off, s32 unused, s32 arg)
{
    u64 v6;
    u64 v7;

    func_001BA580(off, (s32 *)((u8 *)&v6 + 4), (s32 *)&v6);
    func_001BA580(off, (s32 *)((u8 *)&v7 + 4), (s32 *)&v7);
    v6 = (v6 & 0xFFFFFFFFFFFFC000ULL) | (v7 & 0x3FFF);
    func_001576D8(v6, arg);
    func_001576D8(v6, arg);
    return func_001576D8(v6, arg);
}


void func_001577D0(void) {
}

s32 func_001577D8(s32 a1, s32 a2)
{
    s64 v[2];

    func_001BA580(a1, (s32 *)v + 1, (s32 *)v);
    func_001BA580(a2, (s32 *)(v + 1) + 1, (s32 *)(v + 1));
    return (s64)(s32)(v[0] & 0x3FFF) == (s64)(s32)(v[1] & 0x3FFF);
}

s32 func_00157840(s32 arg0)
{
    u64 buf[1];

    func_001BA580(arg0, (s32 *)((u8 *)buf + 4), (s32 *)buf);
    return (s32)(buf[0] & 0x3FFF);
}

s32 func_00157870(s32 arg0)
{
    u64 buf[1];

    func_001BA580(arg0, (s32 *)((u8 *)buf + 4), (s32 *)buf);
    return (s32)((buf[0] >> 37) & 0x3FFF);
}

INCLUDE_ASM(const s32, "core/main", func_001578A8);

INCLUDE_ASM(const s32, "core/main", func_00157920);

INCLUDE_ASM(const s32, "core/main", func_00157A88);

INCLUDE_ASM(const s32, "core/main", func_00157B50);

u8 func_00157BE8(s32 arg0)
{
    u64 buf[1];

    func_001BA580(arg0, (s32 *)((u8 *)buf + 4), (s32 *)buf);
    return (u8)((buf[0] >> 35) & 3);
}

void func_00157C18(void) { func_0014C3B0(); }

void func_00157C20(s32 arg1)
{
    s32 result = func_0014C788(0, arg1, 4, 5);

    D_00393480 = result;
    D_0039347C = result;
}

extern s32 D_00393480;

void func_00157C50(void)
{
    if (D_0039347C == D_00393480) {
        D_0039347C = 0;
    }
    func_0014C938(&D_00393480);
}

void func_00157C70(s32 arg0, u32 arg1)
{
    s32 result = func_0014C788(0, arg0, 0x80, 5);

    D_0039347C = result;
    D_003D3D30[arg1] = result;
}

void func_00157CC0(s32 arg0)
{
    if (D_0039347C == D_003D3D30[arg0])
        D_0039347C = 0;
    func_0014C938(&D_003D3D30[arg0]);
}

s32 func_0014C788();
s32 func_00157CE8();

s32 func_00157CE8(s32 arg1)
{
    return func_0014C788(0, arg1, 4, 5);
}

void func_00157D10(void) { func_0014C938(); }

s32 func_0014C9A0();

s32 func_00157D18(void)
{
    return func_0014C9A0();
}

void func_00157D38(s32 arg1)
{
    s32 result = func_0014C788(0, arg1, 0x80, 5);

    D_00393484 = result;
    D_0039347C = result;
}

extern s32 D_00393484;

void func_00157D68(void)
{
    if (D_0039347C == D_00393484) {
        D_0039347C = 0;
    }
    func_0014C938(&D_00393484);
}

s32 func_00157D88();

s32 func_00157D88(s32 arg1)
{
    return func_0014C9A0(D_00393484, arg1);
}

extern s32 D_0039347C;
s32 func_00157DB0(u8 *arg0)
{
    s32 v = D_0039347C;
    if (v != 0)
        return func_0014CE30(v, arg0);
    return 0;
}

s32 func_00157DE8(s32 arg0, s32 arg1)
{

    s32 v = func_0014C9A0(arg0, arg1);
    func_00224B88(v, 0, arg1);
    return v;
}

void func_00157E30(u32 *arg0, u32 arg1)
{
    u32 v = *arg0;
    u32 rem = v % arg1;

    if (rem != 0)
        *arg0 = v + (arg1 - rem);
}

s32 func_00157E60(s32 a1, s32 a2, s32 **a3, s32 a4)
{

    s32 v4 = a1 * a2;
    s32 result = func_002249D4(a4, *a3, v4);
    *a3 = (s32 *)((u8 *)*a3 + v4);
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00157EA8);

u8 *func_00157F10(u8 **arg0)
{
    u8 *entry = *arg0;
    u8 *result = entry + 2;

    *arg0 = entry + (*(u16 *)entry + 1) * 2;
    return result;
}

extern u8 D_003D3D60[];
extern u8 D_0038A318[];
extern s32 func_00226310();

s32 func_00157F30(s32 arg0)
{
    s32 p = (s32)D_003D3D60;
    func_00226310(p, (s32)D_0038A318, arg0);
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00157F70);

INCLUDE_ASM(const s32, "core/main", func_001580E0);

void func_00158920(void) { func_002152C8(1); }

extern s32 D_00393C5C;

s32 func_00158928(void)
{
    func_001C0C50(D_00393C5C);
    __asm__ volatile("sync");
    __asm__ volatile("ei");
    return 0;
}

void func_00158958(void)
{





    for (;;) {
        do {
            func_001C0C60(D_00393C5C);
        } while (func_00215600() != 0);
        func_00215508();
    }
}

INCLUDE_ASM(const s32, "core/main", func_00158988);

INCLUDE_ASM(const s32, "core/main", func_00158A00);


/* Source: audio/region_0015.c */
/* func_00158B28 through func_0015D170; retail address range 0x00158B28-0x0015D208. */


INCLUDE_ASM(const s32, "core/main", func_00158B28);

INCLUDE_ASM(const s32, "core/main", func_00158BC8);

INCLUDE_ASM(const s32, "core/main", func_00158C58);

s32 func_00158E60(s32 a1, u16 a2)
{



    s32 v2 = *(s32 *)(**(u32 **)(a1 + 4) + 176);
    s32 v3 = func_00113548(v2, a2, 0);

    if (v3 == 0)
        return -1;
    func_00158C58(**(u32 **)(v3 + 16));
    func_001134C8(v2, v3);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00158EC8);

INCLUDE_ASM(const s32, "core/main", func_00158F98);

INCLUDE_ASM(const s32, "core/main", func_00159230);

s32 func_00159300(const s32 *p) { return *(const s32 *)((const u8 *)p + (16)); }

void func_00159308(u8 *arg0) { func_00158F98(arg0); }

INCLUDE_ASM(const s32, "core/main", func_00159310);

INCLUDE_ASM(const s32, "core/main", func_00159530);

INCLUDE_ASM(const s32, "core/main", func_00159610);

INCLUDE_ASM(const s32, "core/main", func_00159698);

s32 func_00159738(u32 *arg0, u64 *arg1, u64 *arg2)
{
    volatile s32 *p = (volatile s32 *)D_003D7D80;

    if (p[0] == 0)
        return 0;
    *arg0 = p[5];
    *(volatile u64 *)arg1 = p[11];
    *arg2 = p[8];
    return 1;
}

s32 func_00159770(u32 *arg0, u32 *arg1)
{
    u8 *p = (u8 *)D_003D7D80;

    if (D_003D7D80[0] == 0 || *(u32 *)(p + 8) == 0)
        return 0;
    *arg0 = *(u32 *)(p + 0x40);
    *arg1 = *(u32 *)(p + 0xC);
    return 1;
}

void func_001597B0(void)
{
    if (func_00159CE8() != 0)
        func_00159CF8(D_003D7D80);
}

void func_001597F0(void)
{
    D_003D7D80[0] = 0;
    D_003D7D80[0x58 / 4] = 0;
    D_003D7D80[0x5C / 4] = 0;
}

void func_00159808(void) {
}

/* Byte-matching.  Copies selected fields into a fresh descriptor block. */
void func_00159810(u8 *src, u8 *dst)
{
    *(u32 *)(dst + 0x0) = *(u32 *)(src + 0x0);
    *(u32 *)(dst + 0x4) = *(u32 *)(src + 0x4);
    *(u32 *)(dst + 0x8) = *(u32 *)(src + 0x8);
    *(u8 *)(dst + 0xC) = *(u8 *)(src + 0x14);
    *(u32 *)(dst + 0x10) = 1;
    *(u32 *)(dst + 0x14) = *(u32 *)(src + 0x18);
    *(u32 *)(dst + 0x1C) = *(u32 *)(src + 0x24);
    *(u32 *)(dst + 0x28) = *(u32 *)(src + 0x20);
    *(u32 *)(dst + 0x24) = *(u32 *)(src + 0x2C);
}

INCLUDE_ASM(const s32, "core/main", func_00159860);

void func_00159928() {
}

INCLUDE_ASM(const s32, "core/main", func_00159930);

INCLUDE_ASM(const s32, "core/main", func_001599A8);

INCLUDE_ASM(const s32, "core/main", func_00159A70);

INCLUDE_ASM(const s32, "core/main", func_00159C70);

s32 func_00159CE0(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

s32 func_00159CE8(void) { return D_003D7D80[0]; }

INCLUDE_ASM(const s32, "core/main", func_00159CF8);

void func_00160718(void *p);

void func_00159D68(s32 *arg1)
{
    if (arg1[0] != 0) {
        func_00160718((void *)arg1[12]);
    }
}

void func_001606E8(void *p);

void func_00159D88(s32 *arg1)
{
    if (arg1[0] != 0) {
        func_001606E8((void *)arg1[12]);
    }
}

void func_00159DA8() {
}

s32 func_00159DB0(void) { return -1; }

s32 func_00159DB8(void) { return -1; }

s32 func_00159DC0(u8 *p, u8 value) { p[0xC] = value; return 1; }

void func_00159DD0(s32 arg0, u32 arg1)
{
    if (arg0 < D_00393544 && arg0 >= 0)
        *(u32 *)(D_00393C68 + arg0 * 0x74 + 0x60) = arg1;
}

u32 func_00159E00(s32 arg0)
{
    if (arg0 < D_00393544 && arg0 >= 0)
        return *(u32 *)(D_00393C68 + arg0 * 0x74 + 0x60);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00159E38);

void func_00159F00(a1, a2)
s32 a1, a2;
{
    func_00211918((s32)D_0038A440, a1, a2);
    *(s32 *)(D_00393C68 + 116 * a1 + 100) = a2;
}

void func_00159F58(s32 arg0)
{
    *(u32 *)(D_00393C68 + arg0 * 0x74 + 0x64) = 0x7F;
    func_00211918(D_0038A468, arg0, 0x7F);
}

INCLUDE_ASM(const s32, "core/main", func_00159F88);

s32 func_0015A0F0(s32 index)
{
    return *(s32 *)(D_00393C68 + index * 0x74 + 0x64);
}

s32 func_0015A108(s32 a1, s32 a2)
{
    u8 *q = (u8 *)(a2 * 116 + D_00393C68);
    s32 *i = *(s32 **)(q + 0x68);
    s32 found = 0;

    while (i != 0) {
        if (i[1] == a1) {
            found = 1;
            break;
        }
        i = (s32 *)i[2];
    }
    if (found != 0)
        return i[0];
    return 127;
}

INCLUDE_ASM(const s32, "core/main", func_0015A150);

s32 func_0015A2A0(void)
{


    s32 result;
    if (D_003D7DE8[0] < 0x41) {
        result = func_00169B40(0, 0x40);
        *(s32 *)(result + 0x34) = 0;
        *(s32 *)(result + 0x38) = 0;
        *(s32 *)(result + 0x3C) = 0;
    } else {
        func_00169AC8(0x12, (s32)D_00393558, 3);
        return 0;
    }
    return result;
}

void func_0015A300(u8 *arg0)
{
    s32 result;
    s32 cmp;
    u8 *a1 = arg0;

    if (*(s32 *)(a1 + 0x34) != 0) { result = *(s32 *)(a1 + 0x38); *(s32 *)(*(s32 *)(a1 + 0x34) + 0x38) = result; }
    if (*(s32 *)(a1 + 0x38) != 0) { result = *(s32 *)(a1 + 0x34); *(s32 *)(*(s32 *)(a1 + 0x38) + 0x34) = result; }
    if (*(s32 *)(a1 + 0x3C) != 0)
    {
        *(s32 *)(*(s32 *)(a1 + 0x3C) + 8) -= 1;
        __asm__ volatile("" ::: "memory");
        if (**((s32 **)(a1 + 0x3C)) == (s32)a1)
        {
            **(s32 **)(a1 + 0x3C) = *(s32 *)(a1 + 0x34);
            __asm__ volatile("" ::: "memory");
        }
        cmp = (*(s32 **)(a1 + 0x3C))[1];
        if (cmp == (s32)a1)
            (*(s32 **)(a1 + 0x3C))[1] = *(s32 *)(a1 + 0x38);
    }
    *(s32 *)(a1 + 0x34) = 0;
    *(s32 *)(a1 + 0x38) = 0;
    *(s32 *)(a1 + 0x3C) = 0;
    func_00169B78(0, a1);
}

INCLUDE_ASM(const s32, "core/main", func_0015A388);

INCLUDE_ASM(const s32, "core/main", func_0015A648);

INCLUDE_ASM(const s32, "core/main", func_0015A708);

INCLUDE_ASM(const s32, "core/main", func_0015A7E8);

INCLUDE_ASM(const s32, "core/main", func_0015A870);

void func_0015A908(s32 a1)
{

    s32 *i;



    func_0015F748();
    func_0015F770(D_0038A498, 0x2CE);
    i = (s32 *)D_003D7DE0[0];
    while (i != 0)
    {
        if (i[0x28 / 4] == a1)
        {
            func_0015FF18(i[0x18 / 4]);
            break;
        }
        i = (s32 *)i[0x34 / 4];
    }
    func_0015F768();
}

INCLUDE_ASM(const s32, "core/main", func_0015A968);

INCLUDE_ASM(const s32, "core/main", func_0015A9D8);

INCLUDE_ASM(const s32, "core/main", func_0015AA30);

INCLUDE_ASM(const s32, "core/main", func_0015AAA0);

void func_0015AB18(s32 arg0, s32 arg1, u8 *arg2)
{
    *(u32 *)(arg2 + 0x20) |= 0x20;
    arg2 += (*(u32 *)(arg2 + 0x24) << 3) + 0x28;
    *(u32 *)(arg2 + 8) = arg1;
    *(u32 *)(arg2 + 0xC) = arg0;
}

void func_0015AB40(void) {
}

void func_0015AB48(s32 arg0, s32 arg1, s32 arg2)
{
    u8 *p = (u8 *)((u32)arg2 * 0x74 + D_00393C68);

    ((void (*)(s32, s32)) * (u32 *)(p + 0x4C))(arg0, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_0015AB78);

INCLUDE_ASM(const s32, "core/main", func_0015AD40);

INCLUDE_ASM(const s32, "core/main", func_0015B360);

void func_0015B678(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{

    s32 v12[24];

    v12[9] = D_0039353C;
    func_0015AD40(a1, a2, a3, (s32)v12);
    func_0015B360(a2, a3, a4, a5, a6, (s32)v12);
}

INCLUDE_ASM(const s32, "core/main", func_0015B6F8);

s32 func_0015B768(void) { return 0; }

s32 func_0015B770(void) { return 0; }

void func_0015B778(s32 arg0, s32 arg1, u32 *arg2, u32 *arg3, u32 *arg4)
{
    if (arg2 != 0)
        *arg2 = 0x10000;
    if (arg3 != 0)
        *arg3 = 0x10000;
    if (arg4 != 0)
        *arg4 = 0x10000;
}

s32 func_0015B7A8(void) { return 0; }

s32 func_0015B7B0(void) { return 1; }

void func_0015B7B8(s32 arg1, s32 *arg2)
{
    if (arg2 != 0) {
        arg2[0] = 0x10000;
        arg2[1] = 0;
        arg2[2] = 0;
    }
}

void func_0015B7D8(s32 arg1, s32 *arg2)
{
    if (arg2 != 0) {
        arg2[0] = 0;
        arg2[1] = 0x10000;
        arg2[2] = 0;
    }
}

void func_0015B7F8(s32 arg1, s32 *arg2)
{
    if (arg2 != 0) {
        arg2[0] = 0;
        arg2[1] = 0;
        arg2[2] = 0x10000;
    }
}

s32 func_0015B818(void) { return 0; }

u32 func_0015B820(void) { return 0x10000; }

s32 func_0015B828(void) { return 0; }

s32 func_0015B830(void) { return 0; }

u32 func_0015B838(void) { return 0x10000; }

s32 func_0015B840(void) { return 1; }

s32 func_0015B848(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0015B850);

void func_0015B8C0(void)
{
    D_00393544 = 0;
    func_0015CCB8((s32)D_00370238, 0);
}

extern void func_00169B78(s32, s32);

void func_0015B8D8(void)
{
    func_00169B78(0, D_00393C68);
}

INCLUDE_ASM(const s32, "core/main", func_0015B8E8);

void func_0015B928(void)
{
    D_003D7DE0[0] = 0;
    D_003D7DE0[1] = 0;
    D_003D7DE0[2] = 0;
}

void func_0015B940(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0015B948);

s32 func_0015E358(void);

s32 func_0015B9F8(void)
{
    return func_0015E358();
}

INCLUDE_ASM(void, "core/main", func_0015BA18);

s32 func_0015E338(void);

s32 func_0015BAA8(void)
{
    return func_0015E338();
}

extern s32 D_00393550;

void func_0015FC50(void *arg0);

void func_0015FC78(void *arg0);

void func_0015BAC8(s32 arg1)
{
    if (arg1 == 0) {
        D_00393550 = 0;
    } else {
        D_00393550 = 1;
    }
    func_0015FC78((void *)arg1);
}

void func_0015BAE8(void *arg0) { func_0015FC50(arg0); }

INCLUDE_ASM(const s32, "core/main", func_0015BAF0);

INCLUDE_ASM(const s32, "core/main", func_0015BBF0);

INCLUDE_ASM(const s32, "core/main", func_0015BC20);

INCLUDE_ASM(const s32, "core/main", func_0015BCF0);

INCLUDE_ASM(const s32, "core/main", func_0015BD30);

s32 func_0015BF30(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    return func_0015C050(-1, a1, a2, a3, a4, a5);
}

INCLUDE_ASM(const s32, "core/main", func_0015BF70);

INCLUDE_ASM(const s32, "core/main", func_0015C050);

void func_0015C330(void) { func_001603C8(); }

INCLUDE_ASM(const s32, "core/main", func_0015C338);

INCLUDE_ASM(const s32, "core/main", func_0015C3C0);

void func_0015C418(s32 a1, s32 a2, s32 a3)
{




    func_0015F748();
    func_0015F770(D_0038A498, 0x9D7);
    if (a1 != -1)
        *(s32 *)func_0015A708(a1, a2, a3, 0) = 0;
    func_0015F768();
}

INCLUDE_ASM(const s32, "core/main", func_0015C488);

void func_0015C520(u8 *arg0, u8 *arg1)
{



    func_0015F748();
    func_0015F770(D_0038A498, 0xA12);
    func_0015A7E8(arg0, arg1);
    func_00159E38(arg0, arg1);
    func_0015F768();
}

extern s32 D_0038A498[];
void func_0015C578(void)
{

    func_0015F748();
    func_0015F770(D_0038A498, 0xA2A);
    func_0015F768();
}

void func_0015C5A8(u8 *arg0)
{

    func_0015F748();
    func_0015F770(D_0038A498, 0xA40);
    func_0015A968(arg0);
    func_00159F00(arg0, 0x7F);
    func_0015F768();
}

void func_0015C5F0(void)
{




    s32 i;

    func_0015F748();
    func_0015F770(D_0038A498, 0xA54);
    func_0015A9D8();
    for (i = 0; i < D_00393544; i++)
        func_00159F00(i, 127);
    func_0015F768();
}

extern s32 D_0038A498[];

void func_0015C660(void *arg0)
{



    func_0015F748(arg0);
    func_0015F770(D_0038A498, 0xA67);
    func_0015AA30(arg0);
    func_0015F768();
}

INCLUDE_ASM(const s32, "core/main", func_0015C6A0);

INCLUDE_ASM(const s32, "core/main", func_0015C7D0);

INCLUDE_ASM(const s32, "core/main", func_0015C8A0);

INCLUDE_ASM(const s32, "core/main", func_0015C990);

INCLUDE_ASM(const s32, "core/main", func_0015CA40);

INCLUDE_ASM(const s32, "core/main", func_0015CB68);

typedef struct { UnalignedU64 a, b, c, d; } S32;

void func_0015CC58(s32 a1)
{


    u8 *p = (u8 *)(a1 * 0x74 + D_00393C68);

    *(S32 *)(p + 0x34) = *(S32 *)D_00370270;
}

INCLUDE_ASM(const s32, "core/main", func_0015CCB8);

INCLUDE_ASM(const s32, "core/main", func_0015CDD8);

void func_0015CE58(s32 arg0, s32 arg1)
{
    if (arg0 >= 0 && arg0 < D_00393544)
        *(u32 *)(D_00393C68 + arg0 * 0x74 + 0x58) = arg1;
}

INCLUDE_ASM(const s32, "core/main", func_0015CE88);

void func_0015CEE0(s32 arg0, s32 arg1)
{
    if (arg0 >= 0 && arg0 < D_00393544)
        *(u32 *)(D_00393C68 + arg0 * 0x74 + 0x5C) = arg1;
}

s32 func_0015CF10(s32 *p)
{
    return p ? *p : 0x7F;
}

void func_0015CF28(u8 *p, s32 value)
{
    if (p != 0)
    {
        *(s32 *)p = value;
        func_0015BAF0(p);
    }
}

INCLUDE_ASM(const s32, "core/main", func_0015CF40);

INCLUDE_ASM(const s32, "core/main", func_0015CFA0);

INCLUDE_ASM(const s32, "core/main", func_0015D000);

INCLUDE_ASM(const s32, "core/main", func_0015D0B8);

void func_0015D170(s32 a1)
{



    u8 *p, *q;

    if (D_0039354C == 0) return;
    func_0015F748();
    func_0015F770(D_0038A498, 0xC73);
    p = (u8 *)D_003D7DE0[0];
    if (p != 0) {
        q = (u8 *)*(s32 *)(p + 0x20);
        for (;;) {
            if (q != 0) {
                if (*(s32 *)(p + 0xC) == a1)
                    func_00160078(*(s32 **)(p + 0x18));
            }
            p = (u8 *)*(s32 *)(p + 0x34);
            if (p == 0) break;
            q = (u8 *)*(s32 *)(p + 0x20);
        }
    }
    func_0015F768();
}


/* Source: misc/region_0015_tail.c */
/* func_0015D208 through func_0015FFE8; retail address range 0x0015D208-0x00160000. */





extern s32 D_003935AC;

extern s32 D_003935A8;
extern s32 D_00393C6C;
extern s32 D_00393C70;

extern s32 D_00393574;

/* Stored as a 64-bit value; the 4-byte declaration keeps the access GP-relative. */
extern u32 D_00393578[1];

void func_0015D208(s32 a1)
{



    u8 *p, *q;

    if (D_0039354C == 0) return;
    func_0015F748();
    func_0015F770(D_0038A498, 0xC8E);
    p = (u8 *)D_003D7DE0[0];
    if (p != 0) {
        q = (u8 *)*(s32 *)(p + 0x20);
        for (;;) {
            if (q != 0) {
                if (*(s32 *)(p + 0xC) == a1)
                    func_001600B8(*(s32 **)(p + 0x18));
            }
            p = (u8 *)*(s32 *)(p + 0x34);
            if (p == 0) break;
            q = (u8 *)*(s32 *)(p + 0x20);
        }
    }
    func_0015F768();
}

void func_0015D2A0(s32 a1)
{



    u8 *p, *q;

    if (D_0039354C == 0) return;
    func_0015F748();
    func_0015F770(D_0038A498, 0xCAB);
    p = (u8 *)D_003D7DE0[0];
    if (p != 0) {
        q = (u8 *)*(s32 *)(p + 0x20);
        for (;;) {
            if (q != 0) {
                if (*(s32 *)(p + 0xC) != a1)
                    func_00160078(*(s32 **)(p + 0x18));
            }
            p = (u8 *)*(s32 *)(p + 0x34);
            if (p == 0) break;
            q = (u8 *)*(s32 *)(p + 0x20);
        }
    }
    func_0015F768();
}

void func_0015D338(s32 a1)
{



    u8 *p, *q;

    if (D_0039354C == 0) return;
    func_0015F748();
    func_0015F770(D_0038A498, 0xCC5);
    p = (u8 *)D_003D7DE0[0];
    if (p != 0) {
        q = (u8 *)*(s32 *)(p + 0x20);
        for (;;) {
            if (q != 0) {
                if (*(s32 *)(p + 0xC) != a1)
                    func_001600B8(*(s32 **)(p + 0x18));
            }
            p = (u8 *)*(s32 *)(p + 0x34);
            if (p == 0) break;
            q = (u8 *)*(s32 *)(p + 0x20);
        }
    }
    func_0015F768();
}

void func_0015D3D0(void) { func_00160758(); }

void func_0015D3D8(void) { func_00160760(); }

extern s32 D_0039354C;

s32 func_0015D3E0(void)
{
    s32 result;

    if (D_0039354C == 0) goto out;
    if (func_0015F780() == 0) goto out;
    func_00160748();
    func_0015F768();
    return 1;
out:
    result = 0;
    return result;
}

extern s32 D_0039354C;

s32 func_0015D428(void)
{
    s32 result;

    if (D_0039354C == 0) goto out;
    if (func_0015F780() == 0) goto out;
    func_00160750();
    func_0015F768();
    return 1;
out:
    result = 0;
    return result;
}

void func_0015D470(void) { func_0015D3E0(); }

void func_0015D478(void) { func_0015D428(); }

s32 func_001602C0(void);

s32 func_0015D480(void)
{
    return func_001602C0();
}

extern void func_001602B8(u32);

void func_0015D4A0(u8 value)
{
    func_001602B8((signed char)value);
}

void func_0015D4B0() { func_001602E0(); }

s32 func_001602E8(void);

s32 func_0015D4B8()
{
    return func_001602E8();
}

s32 func_00160308(void);

s32 func_0015D4D8()
{
    return func_00160308();
}

void func_0015D4F8(void) { func_00160328(); }

s32 func_00160330(void);

s32 func_0015D500(void)
{
    return func_00160330();
}

void func_0015D520(void) { func_00160870(); }

void func_0015D528(void) { func_00160878(); }

s32 func_00160880(void);

s32 func_0015D530(void)
{
    return func_00160880();
}

s32 func_0015D550(s32 a, s32 b, s32 c)
{
    return func_001608A0(a, b & 0xFFFF, c & 0xFFFF);
}

void func_0015D570(void) { func_001608C0(); }

void func_0015D578(void) { func_001608C8(); }

void func_0015D580(void) { func_001608D0(); }

s32 func_001608D8(void);

s32 func_0015D588(void)
{
    return func_001608D8();
}

s32 func_001608F8(void);

s32 func_0015D5A8(void)
{
    return func_001608F8();
}

s32 func_00160918(void);

s32 func_0015D5C8(void)
{
    return func_00160918();
}

s32 func_00160938(void);

s32 func_0015D5E8(void)
{
    return func_00160938();
}

void func_0015D608(void) { func_00160958(); }

s32 func_0015D610(s32 a1, s32 a2, u8 *a3)
{
    return ((s32 (*)(s32, s32))*(u32 *)(116 * a1 + D_00393C68 + 32))(a2, *(u32 *)(a3 + 32));
}

s32 func_00160AD0(void);

s32 func_0015D648(void)
{
    return func_00160AD0();
}

s32 func_00160AF0(void);

s32 func_0015D668(void)
{
    return func_00160AF0();
}

void func_00160B10(void *arg0);

void func_0015D688(void *arg0) { func_00160B10(arg0); }

extern s32 D_0038A498[];
void func_0015D690(s32 a1, s32 a2, s32 a3)
{




    func_0015F748();
    func_0015F770(D_0038A498, 0xE04);
    func_00159F88(a1, a2, a3);
    func_0015BD30();
    func_0015F768();
}

void func_0015D6F8(u8 *arg0, u8 *arg1)
{

    func_0015F748();
    func_0015F770(D_0038A498, 0xE11);
    func_00159E38(arg0, arg1);
    func_0015BD30();
    func_0015F768();
}

s32 func_0015D750(u8 *arg0, u8 *arg1)
{



    s32 v;

    func_0015F748();
    func_0015F770(D_0038A498, 0xE20);
    v = func_0015A108(arg0, arg1);
    func_0015F768();
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_0015D7A8);

INCLUDE_ASM(const s32, "core/main", func_0015D850);

void func_0015D8F0(u8 *arg0, u8 *arg1)
{

    func_0015F748();
    func_0015F770(D_0038A498, 0xE64);
    func_00159F00(arg0, arg1);
    func_0015BD30();
    func_0015F768();
}

void func_0015F748();
void func_0015F770();
void func_00159F00();
void func_0015BD30();
void func_0015F768();

void func_0015D948(s32 arg0)
{
    func_0015F748(arg0);
    func_0015F770(D_0038A498, 0xE71);
    func_00159F00(arg0, 0x7F);
    func_0015BD30();
    func_0015F768();
}

extern s32 D_0038A498[];

s32 func_0015D990(s32 arg0)
{


    s32 v;

    func_0015F748(arg0);
    func_0015F770((s32)D_0038A498, 0xE80);
    v = func_0015A0F0(arg0);
    func_0015F768();
    return v;
}

void func_0015D9D8(s32 a1, s32 a2)
{
    s32 i = 0;

    func_0015F748();
    func_0015F770(D_0038A498, 0xE95);
    if (D_00393544 > 0) {
        do {
            if (i != a1)
                func_00159F00(i, a2);
            i++;
        } while (i < D_00393544);
    }
    func_0015BD30();
    func_0015F768();
}

INCLUDE_ASM(const s32, "core/main", func_0015DA60);

INCLUDE_ASM(const s32, "core/main", func_0015DAD8);

INCLUDE_ASM(const s32, "core/main", func_0015DB70);

void func_0015DBF8(s32 a1)
{




    s32 i;

    func_0015F748();
    func_0015F770(D_0038A498, 0xEED);
    for (i = 0; i < D_00393544; i++)
        func_00159F00(i, a1);
    func_0015BD30();
    func_0015F768();
}

void func_0015DC68(void)
{
    s32 i = 0;

    func_0015F748();
    func_0015F770(D_0038A498, 0xEFD);
    if (D_00393544 > 0) {
        do {
            func_00159F58(i);
            i++;
        } while (i < D_00393544);
    }
    func_0015BD30();
    func_0015F768();
}

s32 func_0015DCD0(s32 a1, s32 a2, s32 a3)
{





    s32 v0, v1;

    if (D_0039354C != 0) {
        v0 = func_00169B40(0, 28);
        func_00224B88(v0, 0, 28);
        v1 = *(s32 *)(v0 + 0x18);
        *(s32 *)(v0 + 4) = 1;
        *(s32 *)(v0 + 0x18) = v1 | 3;
        *(s32 *)(v0 + 8) = func_00160B40(a1, a2, a3);
        return v0;
    }
    return D_00393530;
}

s32 func_0015DD70(u8 *arg0)
{

    if (D_0039354C != 0) {
        s32 *v = func_00169B40(0, 0x1C);
        func_00224B88(v, 0, 0x1C);
        v[1] = 2;
        *(s32 *)((u8 *)v + 0x18) |= 3;
        v[2] = (s32)arg0;
        return (s32)v;
    }
    return D_00393530;
}

/* Byte-matching: early-out guard + 3-way switch dispatcher with tail calls. */
void func_0015DDE8(u8 *arg0)
{



    s32 v;

    if (arg0 == 0)
        return;
    v = *(s32 *)(arg0 + 4);
    switch (v) {
    case 2:
        func_00169B78(0, (s32)arg0);
        return;
    case 1:
        func_00160B48(*(s32 *)(arg0 + 8));
        func_00169B78(0, (s32)arg0);
        return;
    default:
        func_00169AC0(0x13, &D_00393558[0]);
        return;
    }
}


s32 func_0015DE78(void) { return 0; }

s32 func_0015DE80(void) { return -1; }

s32 func_0015DE88(void) { return 0; }

s32 func_0015DE90(void) { return 0; }

void func_0015DE98(void) {
}

s32 func_0015DEA0(void) { return 0; }

s32 func_0015DEA8(void) { return 0; }

s32 func_0015DEB0(void) { return 0; }

extern s32 D_00393534;
extern s32 D_00393538;

/* Byte-matching.  Clears the flag word and stores arg * 65536 as an integer
   through the FPU (cvt.w.s + swc1). */
void func_0015DEB8(f32 arg)
{
    D_00393534 = 0;
    D_00393538 = (s32)(arg * 65536.0f);
}

s32 func_0015DED8(u8 *arg0)
{
    s32 v = func_002256F0();
    v = func_00160E28(v, 0x7FFFFFFF);
    return func_00160CE0(v, (s32)arg0);
}

/* Byte-matching.  Clears the four flag bits of the first word, zeroes the two
   words at +8 and +0xC and reports success.  The -0xC store comes first in
   retail, so the statements are in that order. */
s32 func_0015DF18(u8 *arg0)
{
    s32 v = *(s32 *)arg0;

    v &= -2;
    v &= -3;
    v &= -5;
    v &= -9;
    *(u32 *)(arg0 + 0xC) = 0;
    *(u32 *)(arg0 + 8) = 0;
    *(s32 *)arg0 = v;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_0015DF50);

INCLUDE_ASM(const s32, "core/main", func_0015DFE0);

INCLUDE_ASM(const s32, "core/main", func_0015E0B0);

INCLUDE_ASM(const s32, "core/main", func_0015E1E0);

INCLUDE_ASM(const s32, "core/main", func_0015E248);

INCLUDE_ASM(const s32, "core/main", func_0015E2A0);

s32 func_0015E2F0(s32 arg)
{
    return func_0015E0B0(D_003D7E10, arg);
}

s32 func_0015E1E0(void *);

s32 func_0015E318(void)
{
    return func_0015E1E0(D_003D7E10);
}

s32 func_0015E338(void)
{
    return func_0015E2A0(D_003D7E10);
}

s32 func_0015E358(void)
{
    return func_0015E248(D_003D7E10);
}

s32 func_0015E378(const s32 *p) { return -(p[0] & 1); }

s32 func_0015E388(const s32 *p) { return (p[0] & 5) == 0; }

/* Byte-matching: reads bit 1 as a signed 1-bit bitfield (0 or -1); the
 * sll+slti+negu sign-extension sequence only comes from a bitfield member. */
typedef struct {
    u32 bit0 : 1;
    s32 bit1 : 1;
    u32 rest : 30;
} BitsAt1;

s32 func_0015E398(u32 *p)
{
    return ((BitsAt1 *)p)->bit1;
}

s32 func_0015E3B0(const s32 *p) { return (p[0] & 0xA) == 0; }

INCLUDE_ASM(const s32, "core/main", func_0015E3C0);

/* Byte-matching: linked-list search returning the matching node pointer. */
extern s32 D_003702D0[];

s32 func_0015E510(s32 target)
{
    s32 *p = (s32 *)D_003702D0[0];
    s32 r = 0;

    while (p != 0) {
        if (*p == target) { r = (s32)p; break; }
        p = *(s32 **)((u8 *)p + 0x30);
    }
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_0015E540);

INCLUDE_ASM(const s32, "core/main", func_0015E698);

s32 func_0015E848(void) { return D_00393574; }

void func_0015E850(s32 value) { D_00393574 = value; }

s32 func_0015E858(void)
{
    return (s32)*(u64 *)D_00393578;
}

/* VU0 status register bit tests. */




/* Starts a VU0 microprogram running at a CSR address. */


/* Reads a word from the VU scratchpad. */

void func_0015E870(u32 value)
{
    *(u64 *)D_00393578 = value;
}

s32 func_0015E880(void) { return D_003702D8[0]; }

s32 func_0015E890(u8 *arg0)
{
    u8 *p = (u8 *)func_0015E510(arg0);
    if (p != 0)
        return *(s32 *)(*(s32 *)(p + 0x1C) + 0xC);
    return 0;
}

void func_0015E8C0(void)
{
    D_003702D0[0] = 0;
    D_003702D0[1] = 0;
    D_003702D0[2] = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0015E8D8);

s32 func_0015E918(u32 a1)
{
    s32 result = 0;
    u32 minus1 = 0xFFFFFFFFu;
    s32 v2;
    u16 lo;

    if (a1 != minus1) {
        lo = (u16)a1;
        v2 = func_0015E510(a1 >> 16);
        if (v2 != 0)
            result = *(s32 *)(*(s32 *)(v2 + 0x1C) + 0x10) + 68 * lo;
    }
    return result;
}

s32 func_0015E980(s32 a1)
{

    s32 v1 = D_003702D0[0];
    if (v1 != 0) {
        do {
            if (a1 == -1) *(u64 *)(v1 + 8) = 0;
            else *(u64 *)(v1 + 8) &= ~(s32)(1 << a1);
            if (*(u64 *)(v1 + 8) == 0) *(u32 *)(v1 + 24) = 3;
            v1 = *(s32 *)(v1 + 48);
        } while (v1 != 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0015E9D8);

INCLUDE_ASM(const s32, "core/main", func_0015ED10);

INCLUDE_ASM(const s32, "core/main", func_0015F080);

s32 func_0015F198(void);

s32 func_0015F178(void)
{
    return func_0015F198();
}

INCLUDE_ASM(const s32, "core/main", func_0015F198);

INCLUDE_ASM(const s32, "core/main", func_0015F2E8);

INCLUDE_ASM(const s32, "core/main", func_0015F3A0);

u32 func_0015F3D0(void) { return 0xFFFFFFFFu; }

s32 func_0015F3E0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_0015F3E8);

s32 func_0015F458(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0015F460);

INCLUDE_ASM(const s32, "core/main", func_0015F4E8);

INCLUDE_ASM(const s32, "core/main", func_0015F568);

INCLUDE_ASM(const s32, "core/main", func_0015F5D0);

void func_0015F648(void)
{
    D_00393C6C = D_00393C6C + 8;
    if (D_00393C6C >= D_00393C70) {
        D_003935A8 = 0;
    }
}

void func_0015F670(void)
{
    D_00393C6C = D_00393C6C + 8;
    if (D_00393C6C >= D_00393C70) {
        D_003935A8 = 0;
    }
}

s32 func_0015F698(void) { return 1; }

void func_0015F6A0(void) {
}

void func_0015F6A8(void) {
}

void func_0015F6B0(void) {
}

void func_0015F6B8(void) {
}

s32 func_0015F6C0(void) { return D_003935A8; }

s32 func_0015F6C8(void) { return 1; }

void func_0015F6D0(void)
{


    s32 x[6];
    s32 v;
    if (D_003935AC < 0) {
        x[0] = 0;
        x[1] = 1;
        x[2] = 0;
        x[5] = (s32)&D_003935AC;
        v = func_001C0C20(x);
        D_003935AC = v;
        func_001C0C40(v);
    }
}

extern s32 D_003935AC;

void func_0015F718(void)
{

    s32 v = D_003935AC;

    if (v >= 0) {
        func_001C0C30(v);
        D_003935AC = -1;
    }
}

void func_0015F748() { func_001C0C60(D_003935AC); }

void func_0015F750(void) { func_001C0C60(D_003935AC); }

void func_0015F758(void) {
}

s32 func_0015F760(void) { return 0; }

void func_0015F768(void) { func_001C0C40(D_003935AC); }

void func_0015F770() {
}

void func_0015F778(void) {
}

s32 func_0015F780(void) { return 0; }

void func_0015F788(void) {
}

void func_0015F790(void) {
}

void func_0015F798(s32 arg, u8 *p)
{
    func_002249D4(arg, (s32)p, *(u32 *)(p + 0x24) * 8 + 0x38);
}

void func_0015F7A8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0015F7B0);

void func_0015F870(u8 *p)
{
    s32 value = *(s32 *)(p + 0x8);

    *(s32 *)(p + 0x10) = 0;
    *(s32 *)(p + 0x8) = value | 1;
}

s32 func_0015F888(u8 *p)
{
    if (*(s32 *)(p + 0x10) != 0) {
        *(s32 *)*(s32 *)(p + 0x10) = 0;
        *(s32 *)(p + 0x10) = 0;
    }
    *(s32 *)(p + 8) |= 1;
    return *(s32 *)(p + 8);
}

void func_0015F8B0(s32 *p) { p[4] = 0; }

INCLUDE_ASM(const s32, "core/main", func_0015F8B8);

INCLUDE_ASM(const s32, "core/main", func_0015FA08);

INCLUDE_ASM(const s32, "core/main", func_0015FAC8);

void func_0015FB60(u8 *a1)
{

    s32 v = *(s32 *)(a1 + 4);

    switch (v) {
    case 1:
        func_00163B88(a1);
        *(s32 *)(a1 + 4) = 0;
        break;
    case 11:
        func_00159928(a1);
        *(s32 *)(a1 + 4) = 0;
        break;
    default:
        *(s32 *)(a1 + 4) = 0;
    }
}

extern u8 D_003D7E48[];

void func_0015FBC0(void)
{
    u8 *p = D_003D7E48;
    u8 *end = p + 0x480;

    do {
        *(u32 *)(p + 8) |= 1;
        p += 0x18;
    } while ((s32)p < (s32)end);
}

s32 func_0015FBF8(s32 a1)
{

    s32 v1[4];






    func_0015FBC0();
    D_00393C74 = func_00163DD8(a1);
    func_00162228(v1);
    func_00164978(v1);
    func_001616B0();
    func_001597F0();
    return 3;
}

/* Byte-matching. */
void func_0015FC50(void *arg0)
{



    func_001621C8(arg0);
    func_00164978(arg0);
}

void func_0015FC78(void *arg0)
{


    func_00162438(arg0);
    func_00163F10(arg0);
}

INCLUDE_ASM(const s32, "core/main", func_0015FCA0);

INCLUDE_ASM(const s32, "core/main", func_0015FE48);

INCLUDE_ASM(const s32, "core/main", func_0015FEB0);

INCLUDE_ASM(const s32, "core/main", func_0015FF18);

INCLUDE_ASM(const s32, "core/main", func_0015FF78);

INCLUDE_ASM(const s32, "core/main", func_0015FFE8);


/* Source: resources/region_0016_before_file_io.c */
/* func_00160078 through func_001661A8; retail address range 0x00160000-0x00166210. */



extern s32 D_00393600;
extern s32 D_00393604;

INCLUDE_ASM(const s32, "core/main", func_00160078);

INCLUDE_ASM(const s32, "core/main", func_001600B8);

void func_001600F8(void *arg0)
{
    void *p = arg0;







    s32 sel = *(s32 *)(*(s32 *)((u8 *)p + 4) + 4);

    /* Retail dispatches through jtbl_0038A720 (an 11-entry table at
     * 0x0038A720, already linked from asm/data/main/rodata.rodata.s).  This
     * tree's linker script puts build/src/core/main.c.o(.rodata*) inside
     * .main, so a `switch` here would emit a *second* 44-byte table and push
     * .main past its retail end (the link fails outright).  An equivalent
     * compare chain keeps the behaviour - sel 1 -> func_00163838, 4 ->
     * func_00162020, 8 -> func_00162B98, 10 -> func_00161300, 11 ->
     * func_00159DA8, everything else -> func_00169AC0(0x15, &D_003935B8) -
     * and needs no table.  See REFERENCE-BEHAVIORMATCH.md for the recipe that
     * would let the table come back as a byte match. */
    if (sel == 1)
        func_00163838(*(s32 *)p);
    else if (sel == 4)
        func_00162020(*(s32 *)p);
    else if (sel == 8)
        func_00162B98(*(s32 *)p);
    else if (sel == 10)
        func_00161300(*(s32 *)p);
    else if (sel == 11)
        func_00159DA8(*(s32 *)p);
    else
        func_00169AC0(0x15, &D_003935B8);
}

INCLUDE_ASM(const s32, "core/main", func_00160168);

INCLUDE_ASM(const s32, "core/main", func_00160210);

void func_001602B8(u32 value) { func_00163D78((u8)value); }

s32 func_00163D80(void);

s32 func_001602C0(void)
{
    return func_00163D80();
}

void func_001602E0(void) { func_00163DA0(); }

s32 func_00163DA8(void);

s32 func_001602E8(void)
{
    return func_00163DA8();
}

s32 func_00159CE8(void);

s32 func_00160308(void)
{
    return func_00159CE8();
}

void func_00160328(void) { func_00163DB0(); }

s32 func_00163DB8(void);

s32 func_00160330(void)
{
    return func_00163DB8();
}

s32 func_00163EC0(void);

s32 func_00160350(void)
{
    return func_00163EC0();
}

extern s32 D_00393C74;
void func_00160370(void)
{



    func_00159808();
    func_00161738();
    if (D_00393C74 == 3)
        func_00163EE0();
}

s32 func_001603A8(void) { return 3; }

s32 func_001603B0(void) { return 1; }

void func_001603B8(void) {
}

void func_001603C0(void) {
}

void func_001603C8(void) { func_00163F08(); }

s32 func_00163F18(void);

s32 func_001603D0(void)
{
    return func_00163F18();
}

void func_001603F0(void) { func_00164130(); }

void func_001603F8(void) { func_00164080(); }

INCLUDE_ASM(const s32, "core/main", func_00160400);

INCLUDE_ASM(const s32, "core/main", func_001604C8);

extern s32 D_003935B8[];
s32 func_00160550(u8 *a1, s32 a2)
{
    s32 v3;
    s32 result;
    v3 = a2;
    if (*(s32 *)(*(s32 *)(a1 + 4) + 4) == 1) {
        result = func_001646A8(*(s32 *)a1, a2);
    } else {
        func_00169AC0(0x15, (s32)D_003935B8);
        result = 0;
    }
    *(s32 *)(a1 + 4) = v3;
    return result;
}

s32 func_001605B8(u8 *a1, s32 a2)
{

    s32 v3 = a2;
    s32 result;

    if (*(s32 *)(*(s32 *)(a1 + 4) + 4) == 1) {
        if (*(s32 *)a1 == -1)
            result = 0;
        else
            result = func_00164708(*(s32 *)a1, a2);
    } else {
        func_00169AC0(0x15, (s32)D_003935B8);
        result = 0;
    }
    *(s32 *)(a1 + 4) = v3;
    return result;
}

extern s32 D_003935B8[];

s32 func_00160630(u8 *arg0, s32 arg1, s32 arg2)
{


    s32 *p = *(s32 **)((u8 *)arg0 + 4);
    s32 result;

    if (p[1] == 1)
        result = func_00164768(*(s32 *)arg0, arg1, arg2);
    else {
        func_00169AC0(0x15, &D_003935B8);
        result = 0;
    }
    return result;
}

void func_00160680(u8 *a1)
{
    s32 neg = -(*(s32 *)(a1 + 8) & 1);

    if (neg != 0)
        goto tail;
    if (*(s32 *)(*(s32 *)(a1 + 4) + 4) == 1)
        func_001647C8((u8 *)*(s32 *)a1);
    else
        func_00169AC0(0x15, &D_003935B8);
tail:
    func_0015F888(a1);
}

void func_00169AC0();
void func_00164878(s32);

extern s32 D_003935B8[];

void func_001606E8(void *arg0)
{
    s32 *p = *(s32 **)((u8 *)arg0 + 4);
    if (p[1] == 1)
        func_00164878(*(s32 *)arg0);
    else
        func_00169AC0(0x15, &D_003935B8);
}

void func_00169AC0();
void func_00164838(s32);

extern s32 D_003935B8[];

void func_00160718(void *arg0)
{
    s32 *p = *(s32 **)((u8 *)arg0 + 4);
    if (p[1] == 1)
        func_00164838(*(s32 *)arg0);
    else
        func_00169AC0(0x15, &D_003935B8);
}

void func_00160748(void) { func_001648B8(); }

void func_00160750(void) { func_001648D8(); }

void func_00160758(void) { func_001648F8(); }

void func_00160760(void) { func_00164900(); }

INCLUDE_ASM(const s32, "core/main", func_00160768);

INCLUDE_ASM(const s32, "core/main", func_001607B8);

void func_00160870(void) { func_00164908(); }

void func_00160878(void) { func_00164910(); }

s32 func_00164918(void);

s32 func_00160880(void)
{
    return func_00164918();
}

s32 func_001608A0(s32 a, s32 b, s32 c)
{
    return func_001641F0(a, b & 0xFFFF, c & 0xFFFF);
}

void func_001608C0(void) { func_001642E8(); }

void func_001608C8(void) { func_00164318(); }

void func_001608D0(void) { func_00164348(); }

s32 func_001643B8();

s32 func_001608D8(void)
{
    return func_001643B8();
}

s32 func_00164408(void);

s32 func_001608F8(void)
{
    return func_00164408();
}

s32 func_001644C0();

s32 func_00160918(void)
{
    return func_001644C0();
}

s32 func_00164510();

s32 func_00160938(void)
{
    return func_00164510();
}

void func_00160958(void) { func_001642B8(); }

INCLUDE_ASM(const s32, "core/main", func_00160960);

s32 func_00160960(void);
void func_00164938(void);

void func_001609F0(void)
{
    func_00160960();
    func_00164938();
}

s32 func_00160A10(u8 *arg0)
{
    if (*(s32 *)(arg0 + 4) == 1)
        return func_00163D20(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00160A48);

s32 func_00164FB0(void);

s32 func_00160AD0(void)
{
    return func_00164FB0();
}

s32 func_00164F90(void);

s32 func_00160AF0(void)
{
    return func_00164F90();
}

/* Byte-matching.  Prepare through func_00164F50, then tail-call
   func_00164F28 with its result and arg0. */
void func_00160B10(void *arg0)
{


    void *r = func_00164F50(arg0);

    func_00164F28(r, arg0);
}

s32 func_00160B40(void) { return 0; }

void func_00160B48(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00160B50);

INCLUDE_ASM(const s32, "core/main", func_00160BF0);

/* Multiplies the 16-bit halves of both arguments. */
s32 func_00160CE0(s32 a, s32 b)
{
    return (a >> 8) * (b >> 8);
}

INCLUDE_ASM(const s32, "core/main", func_00160CF0);

INCLUDE_ASM(const s32, "core/main", func_00160D50);

s32 func_00160D50(void);

s32 func_00160E28()
{
    return func_00160D50();
}

s32 func_00160E48(s32 value)
{
    return value < 0 ? -value : value;
}

void func_00160E60(s32 a, s32 b, s32 c)
{
    s32 buffer[4];
    buffer[0] = a;
    buffer[1] = b;
    buffer[2] = c;
    func_00162EA0(buffer);
}

void func_00160E88(s32 val, s32 *out1, s32 *out2)
{
    if (out1 != 0) {
        *out1 = (val >> 16) << 8;
    }
    if (out2 != 0) {
        *out2 = (val << 16) >> 8;
    }
}

s32 func_00160EB0(s32 arg1, s32 arg2)
{
    return ((arg1 << 8) & 0xFFFF0000) | ((arg2 >> 8) & 0xFFFF);
}

INCLUDE_ASM(const s32, "core/main", func_00160ED0);

INCLUDE_ASM(const s32, "core/main", func_00161060);

INCLUDE_ASM(const s32, "core/main", func_001610A0);

INCLUDE_ASM(const s32, "core/main", func_001611A8);

s32 func_00161230(s32 a1) {
s32 v2;
bool v3;
s32 result;
v2 = a1;
if ( *(u32 *)(a1 + 4) != 0 )
{
v3 = func_0015FEB0((s32 *)*(u32 *)a1) != 0;
result = 1;
if ( v3 )
return result;
v2 = a1;
}
func_00169B78(0, v2);
return 0;
}

extern s32 func_0015FF18();
extern void func_00169B78(s32, s32);

void func_00161280(s32 arg0)
{
    if (*(s32 *)(arg0 + 4) != 0)
        func_0015FF18(*(s32 *)arg0);
    func_00169B78(0, arg0);
}

void func_00160078(void *p);

void func_001612C0(s32 *arg1)
{
    if (arg1[1] != 0) {
        func_00160078((void *)arg1[0]);
    }
}

void func_001600B8(void *p);

void func_001612E0(s32 *arg1)
{
    if (arg1[1] != 0) {
        func_001600B8((void *)arg1[0]);
    }
}

void func_001600F8(void *p);

void func_00161300(s32 *arg1)
{
    if (arg1[1] != 0) {
        func_001600F8((void *)arg1[0]);
    }
}

void func_00161320(void) {
}

s32 func_00161328(u8 *arg0)
{
    if (*(s32 *)(arg0 + 0x4) != 0)
        return func_00160768(*(s32 *)arg0);
    return -1;
}

s32 func_00161358(u8 *arg0)
{
    if (*(s32 *)(arg0 + 0x4) != 0)
        return func_001607B8(*(s32 *)arg0);
    return -1;
}

void func_00161388(u8 *a0, u8 *a1)
{
    *(s32 *)(a1 + 0) = *(s32 *)(a0 + 0);
    *(s32 *)(a1 + 4) = *(s32 *)(a0 + 4);
    *(s32 *)(a1 + 8) = *(s32 *)(a0 + 8);
    *(u8 *)(a1 + 0xC) = *(u8 *)(a0 + 0x14);
    *(s32 *)(a1 + 0x10) = 1;
    *(s32 *)(a1 + 0x14) = *(s32 *)(a0 + 0x18);
    *(s32 *)(a1 + 0x18) = *(s32 *)(a0 + 0x1C);
    *(s32 *)(a1 + 0x1C) = *(s32 *)(a0 + 0x20);
    *(s32 *)(a1 + 0x20) = -2;
    *(s32 *)(a1 + 0x24) = -2;
}

INCLUDE_ASM(const s32, "core/main", func_001613D8);

s32 func_00161468(s32 a1, s32 a2, s32 a3)
{
    s32 v;

    if (a3 != 0)
        v = func_00160ED0(a1);
    else
        v = func_00161060(a1);
    return func_00160A48(v, a2, a3);
}

s32 func_001614C0(u8 *p, u8 value) { p[0xC] = value; return 1; }

s32 func_001614D0(const s32 *p) { return p[9] * 8 + 0x60; }

void func_001614E0(u8 *a1, s32 a2)
{
    s32 v4;
    s32 v5;
    v4 = func_001614D0(a2);
    v5 = func_00169BB0(0, *(s32 *)(a1 + 0x28), v4);
    *(s32 *)(a1 + 0x28) = v5;
    func_002249D4(v5, a2, func_001614D0(a2));
}

void func_00161548(u8 *a1, s32 a2)
{
    s32 v4;
    s32 v5;
    v4 = func_001614D0(a2);
    v5 = func_00169B40(0, v4);
    *(s32 *)(a1 + 0x28) = v5;
    func_002249D4(v5, a2, func_001614D0(a2));
}

void func_001615B0(s32 *p)
{
    func_00169B78(0, *(s32 *)((u8 *)p + 0x28));
}

/* Byte-matching.  Descriptor copy with a negated flag word. */
void func_001615C0(u8 *src, u8 *dst)
{
    *(u32 *)(dst + 0x0) = *(u32 *)(src + 0x0);
    *(u32 *)(dst + 0x4) = *(u32 *)(src + 0x4);
    *(u32 *)(dst + 0x8) = *(u32 *)(src + 0x8);
    *(u8 *)(dst + 0xC) = *(u8 *)(src + 0x14);
    *(u32 *)(dst + 0x10) = 1;
    *(s32 *)(dst + 0x14) = -(s32)(*(u32 *)(src + 0x18) & 1);
    *(u32 *)(dst + 0x24) = *(u32 *)(src + 0x28);
    *(u32 *)(dst + 0x18) = *(u32 *)(src + 0x1C);
    *(u32 *)(dst + 0x1C) = *(u32 *)(src + 0x20);
    *(u32 *)(dst + 0x20) = *(u32 *)(src + 0x24) - 1;
}

INCLUDE_ASM(const s32, "core/main", func_00161620);

INCLUDE_ASM(const s32, "core/main", func_001616B0);

s32 func_00161730(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00161738);

s32 func_00161830(void) { return -1; }

s32 func_00161838(void) { return 0; }

void func_00161840(void) {
}

void func_00161848(void) {
}

void func_00161850(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00161858);

INCLUDE_ASM(const s32, "core/main", func_001618F0);

INCLUDE_ASM(const s32, "core/main", func_00161B78);

INCLUDE_ASM(const s32, "core/main", func_00161E50);

INCLUDE_ASM(const s32, "core/main", func_00161F48);

INCLUDE_ASM(const s32, "core/main", func_00161FB8);

INCLUDE_ASM(const s32, "core/main", func_00162020);

s32 func_00162060(s32 arg0, s32 arg1)
{


    s32 *p = &D_003D82C8[15 * arg0];
    s32 v = -(*p & 1);

    if (v == 0)
        goto zero;
    func_001614E0((u8 *)p, arg1);
    func_00161858(arg0, arg1, 1);
    return -(*p & 1);
zero:
    return 0;
}

s32 func_001620E8(s32 arg0, s32 a2, s32 a3, s32 a4, s32 a5)
{



    func_00161858(arg0, 0, 0);
    return -(D_003D82C8[15 * arg0] & 1);
}

s32 func_00162138(void) { return -1; }

s32 func_00162140(void) { return -1; }

s32 func_00162148(u8 *p, u8 value) { p[0xC] = value; return 1; }

#include "common.h"

#include "common.h"

s32 func_00162158(s32 arg0, s32 arg1)
{



    s32 v = arg0 * 8;
    s32 result;

    D_003935DC = arg0;
    D_003935E0 = v;
    D_003935E4 = arg1;
    if (arg1 < arg0)
        D_003935E4 = 0x7FFFFFFF;
    result = D_003935E4 < v;
    if (!result)
    {
        D_003935E8 = 0;
    }
    else
    {
        s32 r = func_00160E28(v - D_003935E4, v - arg0);
        result = func_00160BF0(0x7F, r);
        D_003935E8 = result;
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001621C8);

void func_00162228(s32 *p)
{
    s32 first = D_003935DC;
    s32 second = D_003935E4;

    p[0] = first;
    p[1] = second;
}

INCLUDE_ASM(const s32, "core/main", func_00162240);

INCLUDE_ASM(const s32, "core/main", func_001622A8);

INCLUDE_ASM(const s32, "core/main", func_00162358);

extern s32 D_003935D8;
void func_00162438(s32 arg0)
{
    if (arg0 != 0)
        D_003935D8 = func_00160E28(0x12C0000, arg0) >> 16;
    else
        D_003935D8 = 0;
}

extern s32 D_003935D8;
s32 func_00162470(void)
{
    s32 v = D_003935D8;
    if (v != 0)
        return func_00160E28(0x12C0000, v << 16);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001624A8);

INCLUDE_ASM(const s32, "core/main", func_001625A8);

INCLUDE_ASM(const s32, "core/main", func_001626A8);

void func_00162810(void) {
}

void func_00162818(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00162820);

INCLUDE_ASM(const s32, "core/main", func_00162950);

s32 func_00162AA8(s32 a1)
{
    s32 v2;
    if (*(s32 *)(a1 + 0x18) != 0) {
        if (func_0015FEB0((s32 *)*(s32 *)a1) != 0) return 1;
    }
    v2 = *(s32 *)(a1 + 0x10);
    if (v2 != 0) ((void (*)(s32))v2)(*(s32 *)(a1 + 0x14));
    func_00169B78(0, a1);
    return 0;
}

void func_00162B08(s32 arg0) {


    void (*f)();
    if (*(s32 *)(arg0 + 0x18) != 0) {
        func_0015FF18(*(s32 *)arg0);
        f = *(void (**)())(arg0 + 0x10);
    } else {
        f = *(void (**)())(arg0 + 0x10);
    }
    if (f != 0) {
        f(*(s32 *)(arg0 + 0x14));
    }
    func_00169B78(0, arg0);
}

void func_00162B58(s32 *arg1)
{
    if (arg1[6] != 0) {
        func_00160078((void *)arg1[0]);
    }
}

void func_00162B78(s32 *arg1)
{
    if (arg1[6] != 0) {
        func_001600B8((void *)arg1[0]);
    }
}

void func_00162B98(s32 *p) { p[4] = 0; }

void func_00162BA0(void) {
}

s32 func_00162BA8(u8 *arg0)
{
    if (*(s32 *)(arg0 + 0x18) != 0)
        return func_00160768(*(s32 *)arg0);
    return -1;
}

s32 func_00162BD8(u8 *arg0)
{
    if (*(s32 *)(arg0 + 0x18) != 0)
        return func_001607B8(*(s32 *)arg0);
    return -1;
}

/* Byte-matching.  Descriptor copy that merges one flag bit into the target. */
void func_00162C08(u8 *src, u8 *dst)
{
    *(u32 *)(dst + 0x0) = *(u32 *)(src + 0x0);
    *(u32 *)(dst + 0x4) = *(u32 *)(src + 0x4);
    *(u32 *)(dst + 0x8) = *(u32 *)(src + 0x8);
    *(u8 *)(dst + 0xC) = *(u8 *)(src + 0x14);
    *(u32 *)(dst + 0x10) = 1;
    *(u32 *)(dst + 0x1C) = *(u32 *)(src + 0x20);
    *(u32 *)(dst + 0x20) = *(u32 *)(src + 0x24);
    *(u32 *)(dst + 0x24) = *(u32 *)(src + 0x28);
    *(u32 *)(dst + 0x28) =
        (*(s32 *)(dst + 0x28) & -2) | (*(u32 *)(src + 0x2C) & 1);
}

INCLUDE_ASM(const s32, "core/main", func_00162C68);

INCLUDE_ASM(const s32, "core/main", func_00162CF8);

s32 func_00162DF8(s32 arg0, s32 arg1, s32 arg2)
{


    s32 v[4];
    s32 r;

    r = func_001626A8(arg0, arg1, v);
    return func_00160A48(r, arg1, arg2);
}

s32 func_00162E40(s32 *arg1)
{
    s32 result = 0;

    if (arg1 != 0) {
        result = arg1[1] == 8;
    }
    return result;
}

s32 func_00162E60(u8 *p, u8 value) { p[0xC] = value; return 1; }

INCLUDE_ASM(const s32, "core/main", func_00162E70);

s64 func_00162EA0(s32 *a1)
{
    s32 a = a1[0];
    s32 b = a1[1];
    s32 c = a1[2];

    if (__builtin_expect(a < 0, 1)) a = -a;
    if (__builtin_expect(b < 0, 1)) b = -b;
    if (__builtin_expect(c < 0, 1)) c = -c;
    return func_00162E70(func_00162E70(a, b), c);
}

s32 func_00162EF8(s32 *a1, s32 *a2)
{
    s32 v4 = func_00160BF0(a1[0], a2[0]);
    s32 v5 = func_00160BF0(a1[1], a2[1]);

    return v4 + v5 + func_00160BF0(a1[2], a2[2]);
}

s32 func_00162F68(s32 *a1, s32 *a2)
{
    s32 v4 = func_00160CE0(a1[0], a2[0]);
    s32 v5 = func_00160CE0(a1[1], a2[1]);

    return v4 + v5 + func_00160CE0(a1[2], a2[2]);
}

s32 func_00162FD8(s32 *a1, s32 *a2, u32 *a3)
{


    s32 v6;
s32 v7;
s32 v8;
s32 result;
v6 = func_00160BF0(a1[1], a2[2]);
*a3 = v6 - func_00160BF0(a1[2], a2[1]);
v7 = func_00160BF0(a1[2], *a2);
a3[1] = v7 - func_00160BF0(*a1, a2[2]);
v8 = func_00160BF0(*a1, a2[1]);
result = func_00160BF0(a1[1], *a2);
a3[2] = v8 - result;
return result;
}

INCLUDE_ASM(const s32, "core/main", func_00163088);

INCLUDE_ASM(const s32, "core/main", func_00163400);

INCLUDE_ASM(const s32, "core/main", func_00163568);

void func_001636D0(u8 *arg0)
{
    s32 result;
    s32 cmp;
    u8 *a1 = arg0;

    if (*(s32 *)(a1 + 0x10) != 0) { result = *(s32 *)(a1 + 0x14); *(s32 *)(*(s32 *)(a1 + 0x10) + 0x14) = result; }
    if (*(s32 *)(a1 + 0x14) != 0) { result = *(s32 *)(a1 + 0x10); *(s32 *)(*(s32 *)(a1 + 0x14) + 0x10) = result; }
    if (*(s32 *)(a1 + 0x18) != 0)
    {
        *(s32 *)(*(s32 *)(a1 + 0x18) + 8) -= 1;
        __asm__ volatile("" ::: "memory");
        if (**((s32 **)(a1 + 0x18)) == (s32)a1)
        {
            **(s32 **)(a1 + 0x18) = *(s32 *)(a1 + 0x10);
            __asm__ volatile("" ::: "memory");
        }
        cmp = (*(s32 **)(a1 + 0x18))[1];
        if (cmp == (s32)a1)
            (*(s32 **)(a1 + 0x18))[1] = *(s32 *)(a1 + 0x14);
    }
    *(s32 *)(a1 + 0x10) = 0;
    *(s32 *)(a1 + 0x14) = 0;
    *(s32 *)(a1 + 0x18) = 0;
    func_00169B78(0, a1);
}

INCLUDE_ASM(const s32, "core/main", func_00163758);

void func_00163838(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 8);
    switch (v) {
    case 1:
        func_001650C0(*(s32 *)(arg0 + 4));
        return;
    case 4:
        func_00167428(*(s32 *)(arg0 + 4));
        return;
    default:
        func_00169AC0(0x1B, &D_003935F8);
    }
}

INCLUDE_ASM(const s32, "core/main", func_00163878);

INCLUDE_ASM(const s32, "core/main", func_00163900);

void func_00163988(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 8);
    switch (v) {
    case 1: func_00165120(*(s32 *)(arg0 + 4)); break;
    case 4: func_00167488(*(s32 *)(arg0 + 4)); break;
    default: func_00169AC0(0x1B, &D_003935F8);
    }
    func_001636D0(arg0);
}

void func_001639F0(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 8);
    switch (v) {
    case 1: func_00165128(*(s32 *)(arg0 + 4)); return;
    case 4: func_00167490(*(s32 *)(arg0 + 4)); return;
    default: func_00169AC0(0x1B, &D_003935F8);
    }
}

void func_00163A30(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 8);
    switch (v) {
    case 1: func_00165130(*(s32 *)(arg0 + 4)); return;
    case 4: func_00167498(*(s32 *)(arg0 + 4)); return;
    default: func_00169AC0(0x1B, &D_003935F8);
    }
}

void func_00163A70(u8 *arg0)
{


    if (*(s32 *)(arg0 + 0x1C) == 1)
        func_001651C0();
    else
        func_00169AC0(0x1B, &D_003935F8);
}

INCLUDE_ASM(const s32, "core/main", func_00163A98);

void func_001651C8();
void func_00167748();
void func_00169AC0();
void func_00163B88(u8 *arg0)
{
    s32 v = *(s32 *)(arg0 + 0x18);
    switch (v) {
    case 1: func_001651C8(); return;
    case 4: func_00167748(); return;
    default: func_00169AC0(0x1B, &D_003935F8);
    }
}

s32 func_00163BC8(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 0x18);
    switch (v) {
    case 1: return func_001651D0();
    case 4: return func_00167750();
    default: func_00169AC0(0x1B, &D_003935F8); return 0;
    }
}

s32 func_00163C20(u8 *arg0)
{


    s32 v = *(s32 *)(arg0 + 0x18);

    switch (v) {
    case 1: return func_00165550();
    case 4: return func_001674A0((s32)arg0);
    default: return 0;
    }
}

s32 func_00163C70(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 8);
    switch (v) {
    case 1: return func_00165428(*(s32 *)(arg0 + 4));
    case 4: return func_001674C0(*(s32 *)(arg0 + 4));
    default: func_00169AC0(0x1B, &D_003935F8); return -1;
    }
}

s32 func_00163CC8(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 8);
    switch (v) {
    case 1: return func_00165448(*(s32 *)(arg0 + 4));
    case 4: return func_001674E0(*(s32 *)(arg0 + 4));
    default: func_00169AC0(0x1B, &D_003935F8); return -1;
    }
}

s32 func_00163D20(u8 *arg0)
{



    s32 v = *(s32 *)(arg0 + 0x1C);
    switch (v) {
    case 1: return func_00165500();
    case 4: return func_00167978();
    default: func_00169AC0(0x1B, &D_003935F8); return 0;
    }
}

void func_00163D78(u32 value) { func_001654C8((u8)value); }

s32 func_001654D0(void);

s32 func_00163D80(void)
{
    return func_001654D0();
}

void func_00163DA0(void) {
}

s32 func_00163DA8(void) { return 0; }

void func_00163DB0(void) { func_00167550(); }

s32 func_001654A0(void);

s32 func_00163DB8(void)
{
    return func_001654A0();
}

INCLUDE_ASM(const s32, "core/main", func_00163DD8);

s32 func_00165158(void);

s32 func_00163EC0(void)
{
    return func_00165158();
}

void func_001675B8(void);
void func_00165178(void);

void func_00163EE0(void)
{
    func_001675B8();
    func_00165178();
}

void func_00163F00(void) {
}

void func_00163F08(void) { func_001654C0(); }

void func_00163F10(void) { func_00165180(); }

INCLUDE_ASM(const s32, "core/main", func_00163F18);

INCLUDE_ASM(const s32, "core/main", func_00164080);

INCLUDE_ASM(const s32, "core/main", func_00164130);

INCLUDE_ASM(const s32, "core/main", func_001641F0);

void func_00164E08();
void func_00165288();

void func_001642B8(s32 *arg0)
{
    if ((s32)arg0 == -1)
        return;
    if (*arg0 == 2) {
        func_00164E08(arg0[1]);
        return;
    }
    func_00165288(arg0[1]);
}

void func_00164E10();
void func_001652D0();

void func_001642E8(s32 *arg0)
{
    if ((s32)arg0 == -1)
        return;
    if (*arg0 == 2) {
        func_00164E10(arg0[1]);
        return;
    }
    func_001652D0(arg0[1]);
}

void func_00164EE0();
void func_00165310();

void func_00164318(s32 *arg0)
{
    if ((s32)arg0 == -1)
        return;
    if (*arg0 == 2) {
        func_00164EE0(arg0[1]);
        return;
    }
    func_00165310(arg0[1]);
}

void func_00164EE8();
void func_00165318();

void func_00164348(s32 *arg0)
{
    if ((s32)arg0 == -1)
        return;
    if (*arg0 == 2) {
        func_00164EE8(arg0[1]);
        return;
    }
    func_00165318(arg0[1]);
}

s32 func_00164378(s32 a1)
{

    if (a1 == -1) return 0;
    if (*(s32 *)a1 != 2) return func_00165320(*(s32 *)(a1 + 4));
    return 0;
}

s32 func_001643B8(arg0)
s32 arg0;
{

    if (arg0 != -1) {
        if (*(s32 *)arg0 == 2)
            return func_00164C40(*(s32 *)((u8 *)arg0 + 4));
        else
            return func_00165340(*(s32 *)((u8 *)arg0 + 4));
    }
    return -1;
}

INCLUDE_ASM(const s32, "core/main", func_00164408);

s32 func_001644C0(arg0)
s32 arg0;
{

    if (arg0 != -1) {
        if (*(s32 *)arg0 == 2)
            return func_00164EF0(*(s32 *)((u8 *)arg0 + 4));
        else
            return func_001653C8(*(s32 *)((u8 *)arg0 + 4));
    }
    return -1;
}

s32 func_00164510(s32 a1, s32 a2)
{


    if (a1 != -1)
        return func_00164CE8(*(s32 *)(a1 + 4), a2);
    return -1;
}

s32 func_00164540(void) { return -1; }

INCLUDE_ASM(const s32, "core/main", func_00164548);

s32 func_00164648(u8 *arg0, s32 arg1)
{


    s32 v = *(s32 *)arg0;
    switch (v) {
    case 1: return func_00166DE0(*(s32 *)(arg0 + 4), arg1);
    case 4: return func_001677E8(*(s32 *)(arg0 + 4), arg1);
    default: func_00169AC0(0x1B, &D_003935F8); return 0;
    }
}

s32 func_001646A8(u8 *arg0, s32 arg1)
{



    s32 v = *(s32 *)arg0;
    switch (v) {
    case 1: return func_00166E70(*(s32 *)(arg0 + 4), arg1);
    case 4: return func_00167820(*(s32 *)(arg0 + 4), arg1);
    default: func_00169AC0(0x1B, &D_003935F8); return 0;
    }
}

s32 func_00164708(u8 *arg0, s32 arg1)
{



    s32 v = *(s32 *)arg0;
    switch (v) {
    case 1: return func_00166FA8(*(s32 *)(arg0 + 4), arg1);
    case 4: return func_00167840(*(s32 *)(arg0 + 4), arg1);
    default: func_00169AC0(0x1B, &D_003935F8); return 0;
    }
}

s32 func_00164768(u8 *arg0, s32 arg1)
{



    s32 v = *(s32 *)arg0;
    switch (v) {
    case 1: return func_00166ED8(*(s32 *)(arg0 + 4), arg1);
    case 4: return func_00167860(*(s32 *)(arg0 + 4), arg1);
    default: func_00169AC0(0x1B, &D_003935F8); return 0;
    }
}

void func_001647C8(u8 *arg0)
{


    s32 v = *(s32 *)arg0;
    switch (v) {
    case 1: func_00166FC8(*(s32 *)(arg0 + 4)); break;
    case 4: func_00167880(*(s32 *)(arg0 + 4)); break;
    default: func_00169AC0(0x1B, &D_003935F8);
    }
    func_00169B78(0, (s32)arg0);
}

void func_00164838(s32 arg0)
{


    u8 *p = (u8 *)arg0;
    s32 v = *(s32 *)p;
    switch (v) {
    case 1: func_00167018(*(s32 *)(p + 4)); return;
    case 4: func_00167888(*(s32 *)(p + 4)); return;
    default: func_00169AC0(0x1B, &D_003935F8);
    }
}

void func_00164878(s32 arg0)
{


    u8 *p = (u8 *)arg0;
    s32 v = *(s32 *)p;
    switch (v) {
    case 1: func_00167058(*(s32 *)(p + 4)); return;
    case 4: func_00167890(*(s32 *)(p + 4)); return;
    default: func_00169AC0(0x1B, &D_003935F8);
    }
}

void func_001648B8(void)
{
    func_001678B8();
    func_00165408();
}

void func_001648D8(void)
{
    func_00165410();
    func_001678C0();
}

void func_001648F8(void) { func_00165418(); }

void func_00164900(void) { func_00165420(); }

void func_00164908(void) { func_00165468(); }

void func_00164910(void) { func_00165470(); }

s32 func_00165478(void);

s32 func_00164918(void)
{
    return func_00165478();
}

void func_00164938(void)
{
    func_001654F0();
    func_001678F8();
}

s32 func_00164958(void) { return -1; }

s32 func_00164960(void) { return 0; }

void func_00164968(void) {
}

void func_00164970(void) {
}

/* Byte-matching.  Same shape as func_0015FC50: prepare, then tail-call. */
void func_00164978(void *arg0)
{



    func_001654F8(arg0);
    func_00167970(arg0);
}

INCLUDE_ASM(const s32, "core/main", func_001649A0);

INCLUDE_ASM(const s32, "core/main", func_00164B48);

INCLUDE_ASM(const s32, "core/main", func_00164C40);

INCLUDE_ASM(const s32, "core/main", func_00164CE8);

void func_00164E08(s32 *p) { func_001642B8(((s32 *)p)[0]); }

INCLUDE_ASM(const s32, "core/main", func_00164E10);

void func_00164EE0(s32 *p) { func_00164318(((s32 *)p)[0]); }

void func_00164EE8(s32 *p) { func_00164348(((s32 *)p)[0]); }

s32 func_00164EF0(const s32 *p) { return *(const s32 *)((const u8 *)p + (52)); }

s32 func_001679B8(void);

s32 func_00164EF8(void)
{
    return func_001679B8() ? 4 : 1;
}

void func_00164F28(void *arg0, void *arg1)
{
    if ((s32)arg0 == 1 || (s32)arg0 == 4)
        return;
    func_00169AC0(0x1B, &D_003935F8);
}

INCLUDE_ASM(const s32, "core/main", func_00164F50);

s32 func_00165510(void);

s32 func_00164F90(void)
{
    return func_00165510();
}

s32 func_00165530(void);

s32 func_00164FB0(void)
{
    return func_00165530();
}

INCLUDE_ASM(const s32, "core/main", func_00164FD0);

s32 func_00165058(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{


    s32 v[4];

    func_00163088(a1, v);
    return func_001670E0(a0, v, a2, a3, a4);
}

void func_001650C0(void) {
}

void func_00163088(s32, s32);

void func_001650C8(s32 arg1, s32 arg2)
{
    s32 buffer[4];

    func_00163088(arg2, (s32)buffer);
    func_001670F0(arg1, (s32)buffer);
}

s32 func_001670F8(void);

s32 func_00165100(void)
{
    return func_001670F8();
}

void func_00165120(void) { func_00167100(); }

void func_00165128(void) { func_00167108(); }

void func_00165130(void) { func_00167110(); }

s32 func_001670A0(void);

s32 func_00165138(void)
{
    return func_001670A0();
}

s32 func_001670D0(void);

s32 func_00165158(void)
{
    return func_001670D0();
}

void func_00165178(void) { func_001670D8(); }

void func_00165180(void) {
}

void func_00165188(void) {
}

void func_00165190(void) {
}

void func_00165198(void) {
}

s32 func_001671B0(void);

s32 func_001651A0(void)
{
    return func_001671B0();
}

void func_001651C0(void) { func_00167368(); }

void func_001651C8(void) { func_001672B0(); }

s32 func_001672B8(void);

s32 func_001651D0(void)
{
    return func_001672B8();
}

s32 func_001651F0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{

    s32 buffer[4];
    s32 b = a2 & 0xFFFF;
    s32 c = a3 & 0xFFFF;
    s32 p;


    if (a5 != 0) {
        func_00163088(a5, buffer);
        p = (s32)buffer;
    } else {
        p = (s32)&D_00393618;
    }
    return func_00167158(a1, b, c, a4, p, a6, a7);
}

void func_00165288(s32 a1, s32 a2)
{


    s32 v[4];
    s32 *p;

    if (a2 != 0) {
        func_00163088(a2, v);
        p = v;
    } else {
        p = 0;
    }
    func_00167160(a1, p);
}

extern s32 D_0038A800[];

void func_001652D0(void *arg0)
{



    func_0015F748(arg0);
    func_0015F770(D_0038A800, 0xDD);
    func_00167168(arg0);
    func_0015F768();
}

void func_00165310(void) { func_00167170(); }

void func_00165318(void) { func_00167178(); }

s32 func_00167180(void);

s32 func_00165320(void)
{
    return func_00167180();
}

s32 func_00167188(void);

s32 func_00165340(void)
{
    return func_00167188();
}

s32 func_00165360(s32 a0, s32 a1, s32 a2, s32 a3)
{



    s32 v[4];
    s32 *p;

    if (a2 != 0) {
        func_00163088(a2, v);
        p = v;
    } else {
        p = &D_00393618;
    }
    return func_00167190(a0, a1, p, a3);
}

s32 func_00167198(void);

s32 func_001653C8(void)
{
    return func_00167198();
}

s32 func_001671A0(void);

s32 func_001653E8(void)
{
    return func_001671A0();
}

void func_00165408(void) { func_001672F8(); }

void func_00165410(void) { func_00167300(); }

void func_00165418(void) { func_00167370(); }

void func_00165420(void) { func_00167378(); }

s32 func_00167118(void);

s32 func_00165428(void)
{
    return func_00167118();
}

s32 func_00167120(void);

s32 func_00165448(void)
{
    return func_00167120();
}

void func_00165468(void) { func_00167308(); }

void func_00165470(void) { func_00167338(); }

s32 func_00167340(void);

s32 func_00165478(void)
{
    return func_00167340();
}

void func_00165498(void) { func_00167380(); }

s32 func_00167390(void);

s32 func_001654A0(void)
{
    return func_00167390();
}

void func_001654C0(void) { func_00167148(); }

void func_001654C8(u32 value) { func_00167388((u8)value); }

s32 func_00167398(void);

s32 func_001654D0(void)
{
    return func_00167398();
}

void func_001654F0(void) { func_00167348(); }

void func_001654F8(void *arg0) {
}

s32 func_00165500(void) { return 1; }

void func_00165508(void) { func_00167358(); }

s32 func_00167350(void);

s32 func_00165510(void)
{
    return func_00167350();
}

s32 func_001673A0(void);

s32 func_00165530(void)
{
    return func_001673A0();
}

s32 func_00167360(void);

s32 func_00165550(void)
{
    return func_00167360();
}

s32 func_00165570(void) { return 0; }

s32 func_00165578(s32 arg1, s32 arg2)
{
    return func_001673A8(arg1, arg2 & 0xFF);
}

s32 func_00165598(s32 left, s32 right) {
    D_00393600 = left;
    __asm__ volatile("" ::: "memory");
    D_00393604 = right;
    __asm__ volatile("" ::: "memory");
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001655A8);

INCLUDE_ASM(const s32, "core/main", func_00165658);

INCLUDE_ASM(const s32, "core/main", func_00165760);

INCLUDE_ASM(const s32, "core/main", func_001657F8);

INCLUDE_ASM(const s32, "core/main", func_00165978);

/* Byte-matching.  The 0x30-byte frame holds only the workspace func_001C0B20
   fills; word +0x18 of it is the result. */
s32 func_00165B20(void)
{

    u8 buf[0x30];

    func_001C0B20(func_001C0B10(), buf);
    return *(s32 *)(buf + 0x18);
}

INCLUDE_ASM(const s32, "core/main", func_00165B50);

extern u8 D_003D9680[];

s32 func_00165C30(u8 *arg0)
{

    s32 stack[4];
    stack[0] = *(s32 *)arg0;
    stack[1] = *(s32 *)(arg0 + 4);
    stack[2] = (s32)D_003D9680;
    stack[3] = 0x4000;
    return func_00165B50(stack, (s32)func_00165978);
}

extern u8 D_003DD680[];
extern s32 D_00165A40[];

s32 func_00165C78(u8 *arg0)
{
    s32 stack[4];
    stack[0] = *(s32 *)arg0;
    stack[1] = *(s32 *)(arg0 + 4);
    stack[2] = (s32)D_003DD680;
    stack[3] = 0x4000;
    return func_00165B50(stack, (s32)D_00165A40);
}

INCLUDE_ASM(const s32, "core/main", func_00165CC0);

s32 func_00165D28()
{
    func_001C0A30();
    return 0;
}

/* Byte-matching.  Releases both of the pair's resources and reports success. */
s32 func_00165D48(u8 *arg0)
{
    func_00165D28(*(s32 *)arg0);
    func_00165D28(*(s32 *)(arg0 + 4));
    return 0;
}

s32 func_00165D80(void)
{
    return func_00165EC0(D_003E16A8);
}

s32 func_00165DA0(void)
{
    return func_00165F28(D_003E16A8);
}

s32 func_00165DC0(void)
{
    return func_00166040(D_003E16A8);
}

s32 func_00165DE0(s32 arg)
{
    return func_00166070(D_003E16A8, arg);
}

s32 func_00165E08(s32 arg)
{
    return func_001660C8(D_003E16A8, arg);
}

s32 func_00165E30(s32 arg)
{
    return func_00166120(D_003E16A8, arg);
}

/* Byte-matching.  Returns -1 unless the type tag says 4, in which case it
   forwards the pointer's first word to func_00165E30. */
s32 func_00165E58(s32 arg0, s32 arg1, u32 *arg2, s32 arg3)
{
    s32 r = -1;

    if (arg3 == 4)
        r = func_00165E30(*(s32 *)arg2);
    return r;
}

extern u32 D_00393C88;

s32 func_00165E88(u8 *out)
{
    u32 *p = &D_00393C88;
    D_00393C88 = (u32)func_00165E58;
    *(u32 *)(out + 0x0) = 0x6B636162u;
    *(u32 *)(out + 0x4) = 3;
    *(u32 *)(out + 0x8) = 1;
    *(u32 *)(out + 0xC) = (u32)p;
    return 0;
}

s32 func_00165EC0(u32 *a1)
{

    u8 *i;
    u8 *end;

    i = (u8 *)a1 + 4;
    end = (u8 *)a1 + 772;
    while (i < end) {
        func_00168BC0((u32 *)i);
        i += 16;
    }
    *a1 |= 1;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00165F28);

INCLUDE_ASM(const s32, "core/main", func_00165F90);

s32 func_00166008(s32 *p, s32 *q)
{
    s32 r = (*p & 1) ^ 1;
    if (r != 0) *q = *q + 1;
    return 0;
}

s32 func_00166030(const s32 *p) { return (p[0] & 1) ^ 1; }

/* Byte-matching.  Passes func_00166008 as the callback and reads back the
   word it wrote. */
s32 func_00166040(s32 arg0)
{

    s32 out = 0;

    func_00165F90(arg0, &out, (void *)func_00166008);
    return out;
}

s32 func_00166070(u8 *arg0, s32 arg1)
{
    u32 minus1 = 0xFFFFFFFFu;
    s32 r = (s32)minus1;
    s32 v = func_00165F90(arg0, 0, func_00166030);

    if (v != 0) r = func_00168BE8(v, arg1);
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001660C8);

INCLUDE_ASM(const s32, "core/main", func_00166120);

u32 func_00166170(u32 base, u32 v)
{
    u32 r = 0;
    u32 t;
    if (v >= base + 4 && base + 0x304 >= v) {
        t = v - 4;
        if (((t - base) & 0xF) == 0) r = v;
    }
    return r;
}

s32 func_001661A8(void)
{

    u8 *base = D_003E19B0;
    s32 r = -1;

    func_00224B88((s32)base, 0, 0x80);
    if (func_00165E88(base) >= 0) {
        if (func_00165598((s32)base, (s32)(base + 0x80)) >= 0)
            r = 0;
    }
    return r;
}


/* Source: file_io/region_0016.c */
/* func_00166210 through func_00169628; retail address range 0x00166210-0x001697A0. */



s32 D_00371428[8];

INCLUDE_ASM(const s32, "core/main", func_00166210);

extern u8 D_0038AA98[];

s32 func_001662C8(s32 arg0, s32 arg1)
{


    func_001C28C8((s32)D_0038AA98);
    return func_00166440(arg0, 4, arg1);
}

void func_00166310(s32 arg0)
{



    s32 local;

    func_00211918(D_0038AAB0, arg0);
    local = arg0;
    func_001657F8(0x20005, 1, 0, 0, &local, 4);
    func_00211918(D_0038AAD0, arg0);
}

s32 func_00166378(s32 arg0, s32 arg1)
{


    func_001C28C8((s32)D_0038AA98);
    return func_00166440(arg0, 2, arg1);
}

/* Byte-matching.  Requests property 0x20003 through func_001657F8. */
void func_001663C0(s32 arg0)
{

    s32 local = arg0;

    func_001657F8(0x20003, 1, 0, 0, &local, 4);
}

extern u8 D_0038AAF0[];

s32 func_001663F8(s32 arg0, s32 arg1)
{


    func_001C28C8((s32)D_0038AAF0);
    return func_00166440(arg0, 6, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_00166440);

INCLUDE_ASM(const s32, "core/main", func_00166518);

INCLUDE_ASM(const s32, "core/main", func_001665C8);

extern s32 D_0038ABA0[];
void func_001669B0(u8 *a1, s32 a2)
{



    s32 *i;
    func_0015F748();
    func_0015F770((s32)D_0038ABA0, 0x106);
    for (i = (s32 *)*(s32 *)(a1 + 0x3C); i != 0; i = (s32 *)*(s32 *)((u8 *)i + 12)) {
        if (i[2] == a2) { func_00166518((s32)i); break; }
    }
    func_001665C8((s32)a1);
    func_0015F768();
}

INCLUDE_ASM(const s32, "core/main", func_00166A18);

INCLUDE_ASM(const s32, "core/main", func_00166B00);

INCLUDE_ASM(const s32, "core/main", func_00166D50);

INCLUDE_ASM(const s32, "core/main", func_00166DE0);

INCLUDE_ASM(const s32, "core/main", func_00166E70);

INCLUDE_ASM(const s32, "core/main", func_00166ED8);

s32 func_00166ED8(s32, s32, s32);

s32 func_00166FA8(s32 arg1, s32 arg2)
{
    return func_00166ED8(arg1, arg2, 0x60000);
}

void func_00166FC8(s32 a1)
{


    func_0015F748();
    func_0015F770(D_0038ABA0, 0x20B);
    if (a1 != -1) {
        *(s32 *)(a1 + 4) |= 1;
        func_00166A18(a1);
    }
    func_0015F768();
}

extern s32 D_0038ABA0[];

void func_00167018(void *arg0)
{



    func_0015F748(arg0);
    func_0015F770(D_0038ABA0, 0x21C);
    func_00164318(*(s32 *)arg0);
    func_0015F768();
}

extern s32 D_0038ABA0[];

void func_00167058(void *arg0)
{



    func_0015F748(arg0);
    func_0015F770(D_0038ABA0, 0x229);
    func_00164348(*(s32 *)arg0);
    func_0015F768();
}

s32 func_00167098(void) { return 3; }

s32 func_001C45B8(void);

s32 func_001670A0(void)
{
    return func_001C45B8() == 0 ? 3 : -1;
}

s32 func_001670D0(void) { return 1; }

void func_001670D8(void) {
}

s32 func_001670E0(void) { return 0; }

void func_001670E8(void) {
}

s32 func_001670F0(void) { return 0; }

s32 func_001670F8(void) { return 0; }

void func_00167100(void) {
}

void func_00167108(void) {
}

void func_00167110(void) {
}

s32 func_00167118(void) { return -1; }

s32 func_00167120(void) { return -1; }

s32 func_00167128(void) { return -1; }

s32 func_00167130(void) { return 0; }

void func_00167138(void) {
}

void func_00167140(void) {
}

void func_00167148(void) {
}

s32 func_00167150(void) { return 0; }

s32 func_00167158(void) { return -1; }

void func_00167160(void) {
}

void func_00167168(void) {
}

void func_00167170(void) {
}

void func_00167178(void) {
}

s32 func_00167180(void) { return 0; }

s32 func_00167188(void) { return 0; }

s32 func_00167190(void) { return -1; }

s32 func_00167198(void) { return -1; }

s32 func_001671A0(void) { return -1; }

s32 func_001671A8(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_001671B0);

void func_001672B0(void) {
}

s32 func_001672B8(void) { return 0; }

s32 func_001672C0(void) { return -1; }

s32 func_001672C8(void) { return 0; }

s32 func_001672D0(void) { return 0; }

s32 func_001672D8(void) { return 0; }

void func_001672E0(void) {
}

void func_001672E8(void) {
}

void func_001672F0(void) {
}

void func_001672F8(void) {
}

void func_00167300(void) {
}

typedef struct { u32 v; s8 b; } __attribute__((packed)) S5;
extern u8 D_00393620[];

void func_00167308(u8 *dst, s32 idx)
{
    if (idx < 5)
        *(S5 *)dst = *(S5 *)D_00393620;
}

void func_00167338(void) {
}

s32 func_00167340(void) { return 0; }

void func_00167348(void) {
}

s32 func_00167350(void) { return 0; }

void func_00167358(void) {
}

s32 func_00167360(void) { return 0; }

void func_00167368(void) {
}

void func_00167370(void) {
}

void func_00167378(void) {
}

void func_00167380(void) {
}

void func_00167388(void) {
}

s32 func_00167390(void) { return 0; }

s32 func_00167398(void) { return 0; }

s32 func_001673A0(void) { return 0; }

s32 func_001673A8(void) { return 0; }

s32 func_001673B0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    s32 buffer[4];


    if (D_00393628 == 0)
        return -1;
    func_00163088(a2, (s32)buffer);
    return func_001679E8(a1, (s32)buffer, a3, a4, a5);
}

void func_00167428(void) { func_00167A68(); }


void func_00167430(s32 arg1, s32 arg2)
{
    s32 buffer[4];

    func_00163088(arg2, (s32)buffer);
    func_00167A70(arg1, (s32)buffer);
}

s32 func_00167A90(void);

s32 func_00167468(void)
{
    return func_00167A90();
}

void func_00167488(void) { func_00167AB0(); }

void func_00167490(void) { func_00167AB8(); }

void func_00167498(void) { func_00167AC0(); }

/* func_00167AC8 is defined below with a prototype; retail calls it without
 * arguments from here, so the call site uses the unprototyped form. */
s32 func_00167AC8();

s32 func_001674A0(void)
{
    return func_00167AC8();
}

s32 func_00167AD8(void);

s32 func_001674C0(void)
{
    return func_00167AD8();
}

s32 func_00167AF8(void);

s32 func_001674E0(void)
{
    return func_00167AF8();
}

void func_00167500(u32 value) { func_00167B00((u8)value); }

s32 func_00167B08(void);

s32 func_00167508(void)
{
    return func_00167B08();
}

void func_00167528(void) { func_00167F98(); }

s32 func_00167FA0(void);

s32 func_00167530(void)
{
    return func_00167FA0();
}

void func_00167550(void) { func_00167B10(); }

s32 func_00167B18(void);

s32 func_00167558(void)
{
    return func_00167B18();
}

s32 func_00167578(void)
{
    D_00393628 = 1;
    return func_00167B20();
}

s64 func_00167B50(void);

s64 func_00167598(void)
{
    return func_00167B50();
}

void func_001675B8(void) { func_00167B78(); }

void func_001675C0(void) { func_00167B80(); }

void func_001675C8(void) {
}

void func_001675D0(void) {
}

void func_001675D8(void) {
}

s32 func_001675E0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    s32 buffer[4];
    s32 b = a2 & 0xFFFF;
    s32 c = a3 & 0xFFFF;


    if (D_00393628 == 0)
        return -1;
    func_00163088(a5, (s32)buffer);
    return func_00167BA0(a1, b, c, a4, (s32)buffer, a6, a7);
}


void func_00167678(s32 arg1, s32 arg2)
{
    s32 buffer[4];

    func_00163088(arg2, (s32)buffer);
    func_00167BA8(arg1, (s32)buffer);
}

void func_001676B0(void) { func_00167BB0(); }

void func_001676B8(void) { func_00167BB8(); }

void func_001676C0(void) { func_00167BC0(); }

s32 func_00167BC8(void);

s32 func_001676C8(void)
{
    return func_00167BC8();
}

s32 func_00167BD0(void);

s32 func_001676E8(void)
{
    return func_00167BD0();
}

s32 func_00167BE0(void);

s32 func_00167708(void)
{
    return func_00167BE0();
}

s32 func_00167E70(void);

s32 func_00167728(void)
{
    return func_00167E70();
}

void func_00167748(void) { func_00167E78(); }

s32 func_00167E80(void);

s32 func_00167750(void)
{
    return func_00167E80();
}

s32 func_00167770(s32 a1, s32 a2, s32 a3, s32 a4, s32 *a5)
{

    s32 v10[4];

    if (D_00393628 == 0)
        return -1;
    func_00163088(a5, v10);
    return func_00167E88(a1, a2, a3, a4, (s32)v10);
}


s32 func_001677E8(s32 arg1, s32 arg2)
{
    s32 buffer[4];

    func_00163088(arg2, (s32)buffer);
    func_00167F10(arg1, (s32)buffer);
}

s32 func_00167F30();

s32 func_00167820(void)
{
    return func_00167F30();
}

s32 func_00167840(s32 arg1, s32 arg2)
{
    return func_00167F58(arg1, arg2, 0x10000);
}

s32 func_00167F58();

s32 func_00167860(void)
{
    return func_00167F58();
}

void func_00167880(void) { func_00167F80(); }

void func_00167888(void) { func_00167F88(); }

void func_00167890(void) { func_00167F90(); }

s32 func_00167FC8(void);

s32 func_00167898(void)
{
    return func_00167FC8();
}

void func_001678B8(void) { func_00167FD0(); }

void func_001678C0(void) { func_00167FD8(); }

void func_001678C8(void) { func_00167FE0(); }

void func_001678D0(void) { func_00167FE8(); }

s32 func_00167FF0(void);

s32 func_001678D8(void)
{
    return func_00167FF0();
}

void func_001678F8(void) { func_00167FC0(); }

extern s32 D_00393628;
extern s32 func_00167FF8();

s32 func_00167900(void)
{
    if (D_00393628 == 0)
        return -1;
    return func_00167FF8();
}

s32 func_00168000(void);

s32 func_00167930(void)
{
    return func_00168000();
}

s32 func_00168008(void);

s32 func_00167950(void)
{
    return func_00168008();
}

void func_00167970(void *arg0) {
}

s32 func_00168010(void);

s32 func_00167978(void)
{
    return func_00168010();
}

s32 func_00168018(void);

s32 func_00167998(void)
{
    return func_00168018();
}

s32 func_001679B8(void) { return 1; }

s32 func_001679C0(s32 arg1, s32 arg2)
{
    return func_00168020(arg1, arg2 & 0xFF);
}

s32 func_001679E0(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001679E8);

void func_00167A68(void) {
}

s32 func_00168188();

s32 func_00167A70(void)
{
    return func_00168188();
}

s32 func_00168160();

s32 func_00167A90(void)
{
    return func_00168160();
}

void func_00167AB0(void) { func_001680A0(); }

void func_00167AB8(void) { func_001681E8(); }

void func_00167AC0(void) { func_001681F8(); }

s32 func_00167AC8(const u8 *p) {
    register s32 value __asm__("$2") = p[0x14];
    __asm__ volatile ("dsrl %0, %0, 7" : "+r"(value));
    return -value;
}

s32 func_00168208();

s32 func_00167AD8(void)
{
    return func_00168208();
}

s32 func_00167AF8(void) { return -1; }

void func_00167B00(void) {
}

s32 func_00167B08(void) { return 0; }

void func_00167B10(void) {
}

s32 func_00167B18(void) { return 0; }

/* Behaviour-faithful.  -1 when the probe fails, otherwise 3.  Retail computes
   the test with slti and folds both arms into movn. */
s32 func_00167B20(void)
{

    s32 r = -1;

    if (func_00168028() >= 0)
        r = 3;
    return r;
}

s64 func_00167B50()
{
  return func_001679E0() != -1;
}

void func_00167B78(void) { func_00168048(); }

void func_00167B80(void) {
}

void func_00167B88(void) {
}

void func_00167B90(void) {
}

void func_00167B98(void) {
}

s32 func_00167BA0(void) { return -1; }

void func_00167BA8(void) {
}

void func_00167BB0(void) {
}

void func_00167BB8(void) {
}

void func_00167BC0(void) {
}

s32 func_00167BC8(void) { return 0; }

s32 func_00167BD0(void) { return -1; }

s32 func_00167BD8(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00167BE0);

s32 func_00167E68(void) { return 0; }

s32 func_00167E70(void) { return 0; }

void func_00167E78(void) {
}

s32 func_00167E80(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00167E88);

s32 func_00168188();

s32 func_00167F10(void)
{
    return func_00168188();
}

s32 func_00167F30(s32 a, s32 b)
{
    return func_001681B0(a, 0, b) != 0;
}

s32 func_00167F58(s32 a, s32 b)
{
    return func_001681B0(a, 0x60000, b) != 0;
}

void func_00167F80(void) { func_001680A0(); }

void func_00167F88(void) { func_001681E8(); }

void func_00167F90(void) { func_001681F8(); }

void func_00167F98(void) { func_00168230(); }

s32 func_00168240(void);

s32 func_00167FA0(void)
{
    return func_00168240();
}

void func_00167FC0(void) { func_00168260(); }

s32 func_00167FC8(void) { return 0; }

void func_00167FD0(void) {
}

void func_00167FD8(void) {
}

void func_00167FE0(void) {
}

void func_00167FE8(void) {
}

s32 func_00167FF0(void) { return 0; }

s32 func_00167FF8(void) { return -1; }

s32 func_00168000(void) { return -1; }

s32 func_00168008(void) { return -1; }

s32 func_00168010(void) { return 0; }

s32 func_00168018(void) { return 16; }

s32 func_00168020(void) { return 0; }

s32 func_00168028(void)
{
    return func_001683D8(D_00371428);
}

s32 func_00168048(void)
{
    return func_00168470(D_00371428);
}

s32 func_00168508();

s32 func_00168068(s32 arg1, s32 arg2, s32 arg3)
{
    return func_00168508(D_00371428, arg1, arg2, arg3);
}

s32 func_001680A0(s32 arg)
{
    return func_00168598(D_00371428, arg);
}

s32 func_001680C8(s32 arg)
{
    return func_00168638(D_00371428, arg);
}

s32 func_00168668();

s32 func_001680F0(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_00168668(D_00371428, arg1, arg2, arg3, arg4, arg5);
}

s32 func_00168138(s32 arg)
{
    return func_001686D8(D_00371428, arg);
}

s32 func_00168160(s32 arg)
{
    return func_001688D0(D_00371428, arg);
}

s32 func_00168188(s32 a, s32 b)
{
    return func_00168938(D_00371428, a, b);
}

s32 func_001689C0();

s32 func_001681B0(s32 arg1, s32 arg2, s32 arg3)
{
    return func_001689C0(D_00371428, arg1, arg2, arg3);
}

void func_00168A10(s32 arg0, s32 arg1);

void func_001681E8(s32 arg)
{
    func_00168A10((s32)D_00371428, arg);
}

void func_00168A40(s32 arg0, s32 arg1);

void func_001681F8(s32 arg)
{
    func_00168A40((s32)D_00371428, arg);
}

s32 func_00168208(s32 arg)
{
    return func_00168A70(D_00371428, arg);
}

void func_00168230(s32 arg)
{
    func_00168AA8((s32)D_00371428, arg);
}

s32 func_00168240(void)
{
    return func_00168AE0(D_00371428);
}

extern void func_00168B38(s32);

void func_00168260(void)
{
    func_00168B38((s32)D_00371428);
}

INCLUDE_ASM(const s32, "core/main", func_00168270);

/* Byte-matching.  Returns a1 when its distance past a0 (plus the 0x10 bias)
   is a whole 0x30 slot, and 0 otherwise. */
s32 func_001682E8(s32 a0, s32 a1)
{
    s32 t = a1 - 0x10;
    u32 r = (u32)(t - a0) % 0x30;

    if (r == 0)
        return a1;
    return 0;
}

s32 func_00168310(s32 *p, s32 *q)
{
    s32 r = (*p & 1) ^ 1;
    if (r != 0) *q = *q + 1;
    return 0;
}

s32 func_00168338(const s32 *p) { return (p[0] & 1) ^ 1; }

s32 func_00168348(s32 arg0, u32 *arg1)
{

    u32 v = 0;
    if (func_00168D70(arg0) >= 0)
        v = (*arg1 != 0);
    *arg1 = v;
    return 0;
}

s32 func_00168390(s32 arg0, u32 *arg1)
{

    u32 v = 0;
    if (func_00168E50(arg0) >= 0)
        v = (*arg1 != 0);
    *arg1 = v;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001683D8);

INCLUDE_ASM(const s32, "core/main", func_00168470);

/* Byte-matching.  Same shape as func_00166040 with func_00168310. */
s32 func_001684D8(s32 arg0)
{

    s32 out = 0;

    func_00168270(arg0, &out, (void *)func_00168310);
    return out;
}

INCLUDE_ASM(const s32, "core/main", func_00168508);

INCLUDE_ASM(const s32, "core/main", func_00168598);

/* Byte-matching.  -1 when the object is missing, else bit 0 of its first
   word. */
s32 func_00168638(void)
{
    void *p = ((s32 (*)())func_001682E8)();
    s32 r = -1;

    if (p != 0)
        r = *(s32 *)p & 1;
    return r;
}

s32 func_00168668(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{

    s32 v9 = func_001682E8(a1, a2);
    s32 v11 = -1;
    if (v9 != 0) v11 = func_001692F0(v9, a3, a4, a5, a6);
    return v11;
}

/* Byte-matching.  -1 when the object is missing, else the result of
   func_00169308 on it. */
s32 func_001686D8(void)
{

    void *p = ((s32 (*)())func_001682E8)();
    s32 r = -1;

    if (p != 0)
        r = func_00169308(p);
    return r;
}

/* Reads the CP0 Count register ($9). */
s32 func_00168710(void)
{
    s32 value;

    __asm__ volatile("mfc0 %0, $9" : "=r"(value));
    return value;
}

void func_00168720(void)
{
    func_00224B88((s32)D_003E1A30, 0, 0xC0);
}

INCLUDE_ASM(const s32, "core/main", func_00168738);

s32 func_001688D0(u8 *a1, s32 a2)
{


    s32 v11 = 0;
    s32 v12 = func_001682E8(a1, a2);
    if (v12 != 0) v11 = func_00169320(v12);
    func_00168738(a1, v12, v11);
    return v11;
}

INCLUDE_ASM(const s32, "core/main", func_00168938);

s32 func_001689C0(s32 a1, s32 a2, s32 a3, s32 a4)
{

    u32 minus1 = 0xFFFFFFFFu;
    s32 r = func_001682E8(a1, a2);
    s32 result = (s32)minus1;

    if (r != 0) result = func_001697A0(r, a3, a4);
    return result;
}

/* Byte-matching.  arg1 is never read: the object in arg0 goes to
   func_001682E8 and its result to func_001698C8 when non-null.  The two
   arguments are settled by the callers func_001681E8/func_001681F8, which
   are matched and set both $a0 (the D_00371428 object) and $a1. */
void func_00168A10(s32 arg0, s32 arg1)
{

    u8 *p = ((s32 (*)())func_001682E8)(arg0);

    if (p != 0)
        func_001698C8(p);
}

/* Byte-matching.  Same as func_00168A10 with func_00169938. */
void func_00168A40(s32 arg0, s32 arg1)
{

    u8 *p = ((s32 (*)())func_001682E8)(arg0);

    if (p != 0)
        func_00169938(p);
}

/* Byte-matching.  Same as func_001686D8 through func_001699B0. */
s32 func_00168A70(void)
{

    void *p = ((s32 (*)())func_001682E8)();
    s32 r = -1;

    if (p != 0)
        r = func_001699B0(p);
    return r;
}

/* Byte-matching.  Requests property 0x1000A through func_001657F8. */
void func_00168AA8(s32 arg0, s32 arg1)
{

    s32 local = arg1;

    func_001657F8(0x1000A, 1, 0, 0, &local, 4);
}

/* Byte-matching.  Reads property 0x1000B back through func_001657F8. */
s32 func_00168AE0(void)
{

    s32 local;

    func_001657F8(0x1000B, 1, (s32)&local, 4, 0, 0);
    return local;
}

s32 func_00168B18();
s32 func_00169320();

s32 func_00168B18()
{
    func_00169320();
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00168B38);

/* Byte-matching.  Clears a flag bit and the three following words. */
s32 func_00168BC0(p)
s32 *p;
{
    s32 *q = p + 1;

    q[0] = 0;
    p[0] &= -2;
    q[1] = 0;
    q[2] = 0;
    return 0;
}

/* Byte-matching.  Marks the record as active and copies the three words that
   follow the flag from the source. */
u8 *func_00168BE8(u8 *dst, u8 *src)
{
    u32 *q = (u32 *)(dst + 4);
    u32 v = *(u32 *)dst | 1;

    *(u32 *)dst = v;
    q[0] = *(u32 *)src;
    q[1] = *(u32 *)(src + 4);
    q[2] = *(u32 *)(src + 8);
    return dst;
}

s32 func_00168BC0();
s32 func_00168C18();

s32 func_00168C18()
{
    func_00168BC0();
    return 0;
}

/* Byte-matching.  Indirect call through the function pointer at +0xC, with the
   words at +4 and +8 as its arguments; -1 when the pointer is null. */
s32 func_00168C38(u8 *arg0)
{
    s32 (*fn)(s32, s32) = *(s32 (**)(s32, s32))(arg0 + 0xC);
    s32 result = -1;

    if (fn != 0)
        result = fn(*(s32 *)(arg0 + 4), *(s32 *)(arg0 + 8));
    return result;
}

s32 func_001C0C60();
s32 func_001C0C40();
s32 func_00168C68(s32 *p)
{
    s32 neg;
    func_001C0C60(p[2]);
    neg = -(*p & 1);
    if (neg != 0) p[1] = 1;
    func_001C0C40(p[2]);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00168CB8);

INCLUDE_ASM(const s32, "core/main", func_00168D70);

INCLUDE_ASM(const s32, "core/main", func_00168E50);

INCLUDE_ASM(const s32, "core/main", func_00168F08);

INCLUDE_ASM(const s32, "core/main", func_001691E0);

s32 func_001692F0(u8 *p, s32 a, s32 b, s32 c, s32 d)
{
    *(s32 *)(p + 0x18) = a;
    *(s32 *)(p + 0x1C) = b;
    *(s32 *)(p + 0x28) = c;
    *(s32 *)(p + 0x2C) = d;
    return 0;
}

s32 func_00169308(u8 *p)
{
    *(s32 *)p &= -3;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00169320);

INCLUDE_ASM(const s32, "core/main", func_00169628);


/* Source: resources/region_0016_tail.c */
/* func_001697A0 through func_0016FD90; retail address range 0x001697A0-0x00170000. */



s32 D_003E1AF0[8];

INCLUDE_ASM(const s32, "core/main", func_001697A0);

INCLUDE_ASM(const s32, "core/main", func_001698C8);

INCLUDE_ASM(const s32, "core/main", func_00169938);

INCLUDE_ASM(const s32, "core/main", func_001699B0);

INCLUDE_ASM(const s32, "core/main", func_00169A40);

void func_00169AC0(void) {
}

void func_00169AC8(void) {
}

void func_00169AD0(void) {
}

void func_00169AD8(void) {
}

void func_00169AE0(void) {
}

s32 func_00169AE8(void) { return 4; }

s32 func_00169AF0(void) { return 4; }

extern void func_00169BD8(s32, s32);
extern void func_00169C20(s32, s32);

void func_00169AF8(s32 unused, s32 value)
{
    func_00169BD8((s32)D_003E1AF0, value);
}

void func_00169B08(s32 unused, s32 value)
{
    func_00169C20((s32)D_003E1AF0, value);
}

s32 func_00169B18(s32 arg0)
{
    return func_00169CB0(D_003E1AF0, 2, arg0);
}

s32 func_00169B40(s32 arg0, s32 arg1)
{
    return func_00169CB0(D_003E1AF0, 2, arg1);
}

void func_00169B68(s32 arg)
{
    func_00169EB8((s32)D_003E1AF0, arg);
}

void func_00169B78(s32 unused, s32 value)
{
    func_00169EB8((s32)D_003E1AF0, value);
}

s32 func_00169B88(s32 a, s32 b)
{
    return func_00169DD0(D_003E1AF0, a, b);
}

s32 func_00169BB0(void)
{
    return func_00169DD0(D_003E1AF0);
}

void func_00169BD0(void) {
}

/* Byte-matching.  Header word, then 0x300 bytes of 3-word records cleared.
   func_001C45B8 takes no arguments, so the tail call leaves $a0 untouched,
   which is what retail does. */
void func_00169BD8(s32 arg0, s32 unused)
{

    s32 *base = (s32 *)arg0;
    s32 *p;

    base[0] = 1;
    base[1] = 0;
    for (p = base + 2; p < base + 0x302; p += 3) {
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }
    func_001C45B8();
}

INCLUDE_ASM(const s32, "core/main", func_00169C20);

INCLUDE_ASM(const s32, "core/main", func_00169CB0);

INCLUDE_ASM(const s32, "core/main", func_00169D90);

INCLUDE_ASM(const s32, "core/main", func_00169DD0);

s32 func_00169EB8(s32 *a1, s32 a2)
{

    s32 *v4;
    v4 = (s32 *)func_00169F18(a1, a2);
    if (v4 != 0) {
        func_00169F50(v4[0], v4[2]);
        a1[1]--;
        v4[0] = 0; v4[1] = 0; v4[2] = 0;
    }
    return -1;
}

u8 *func_00169F18(u8 *arg0, s32 arg1)
{
    u8 *end = arg0 + 0xC08;
    u8 *result = 0;
    u8 *p;

    for (p = arg0 + 8; (u32)p < (u32)end; p += 0xC) {
        if (*(s32 *)(p + 8) == arg1) {
            result = p;
            break;
        }
    }
    return result;
}

/* Byte-matching: a switch with cases 0 and 2 emits retail's beqz (with the
   second comparison constant in its delay slot) and beq, each branching to a
   void tail call that passes arg1 in the delay slot.  An if/else-if chain
   compiles to the same 0x28 bytes with the tests inverted (bne), so use the
   switch form here. */
void func_00169F50(s32 arg0, s32 arg1)
{


    switch (arg0) {
    case 0:
        func_001C46B0(arg1);
        break;
    case 2:
        func_00223E80(arg1);
        break;
    }
}

void func_00169F78(void) { func_0016BF68(); }

void func_00169F80(void) { func_0016C0B8(); }

INCLUDE_ASM(const s32, "core/main", func_00169F88);

INCLUDE_ASM(const s32, "core/main", func_0016A0A8);

INCLUDE_ASM(const s32, "core/main", func_0016A218);

INCLUDE_ASM(const s32, "core/main", func_0016A3A8);

INCLUDE_ASM(const s32, "core/main", func_0016A730);

INCLUDE_ASM(const s32, "core/main", func_0016A8A0);

INCLUDE_ASM(const s32, "core/main", func_0016A9F0);

INCLUDE_ASM(const s32, "core/main", func_0016AAE8);

INCLUDE_ASM(const s32, "core/main", func_0016ABF0);

INCLUDE_ASM(const s32, "core/main", func_0016ACE8);

typedef s32 (*AF78Func)(s32);
u8 *func_0016C6A8(u8 *, s32 *);

/* func_0016AF78: submits the display-list work described by arg1 for the
 * object identified by arg2.  The sequence is:
 *   1. func_0016ACE8 prepares the frame, then two func_001893D0 calls program
 *      VU macro registers 0x10 and 1 (mode 0x304).
 *   2. func_0016BD48 registers func_0016C6A8 as a callback and yields a wait
 *      counter that func_00189728 busy-waits on, followed by func_00197458.
 *   3. func_00189F88/func_00189980 build the state block in ctx.local3; on
 *      failure func_0018A0A0 releases it and the call returns 0.
 *   4. func_0016C380 counts the entries, a vtable allocation (D_003731B8
 *      function table slot 0x12C/4) reserves count*4 bytes, and the circular
 *      list at arg1+0x08 is walked: every not-yet-recorded entry id (node
 *      field at link-0x28) triggers func_0016E028 and is appended.
 *   5. func_00169F88 flushes the allocation.  On success the recorded ids are
 *      replayed through func_0016DFE8, vtable slot 0x130/4 runs the batch, and
 *      0 is returned.  Otherwise the func_0016A218 walk re-initialises each
 *      node with the stack context, the batch is replayed, and
 *      func_0019A1D8(D_00372490, arg2, arg1) re-registers the object; a zero
 *      result clears the returned handle. */

typedef struct AF78Ctx {
    s32 arg2copy;                       /* +0x00: arg2 copy read by func_0016A218 */
    u8 local3[8];                       /* +0x04: state block for func_00189F88 */
    s32 alloc;                          /* +0x0C: vtable allocation result */
    s32 len;                            /* +0x10: recorded entry count */
    s32 flag;                           /* +0x14: set before the func_0016A218 walk */
    u8 unk18[8];                        /* +0x18 */
    s32 buffer0;                        /* +0x20: buffer word read by func_0018DAD0 */
    s32 sysres;                         /* +0x24: func_0018DA08 result */
    u8 unk28[8];                        /* +0x28 */
    s32 local0;                         /* +0x30: func_0016BD48 output */
    s32 local2;                         /* +0x34: wait counter */
    u8 unk38[8];                        /* +0x38 */
} AF78Ctx;

s32 func_0016ACE8(s32, s32);
s32 func_001893D0(s32, s32, s32, s32);
s32 func_0016BD48(s32, s32, s32);
void func_00189728(s32 *, s32);
s32 func_00197458(s32, s32, s32);
s32 func_00189F88(s32 *, s32);
s32 func_00189980(s32 *, s32);
s32 func_0018A0A0(s32 *);
s32 func_0016C380(s32);
s32 func_00169F88(s32 *, s32);
s32 func_0016A218(s32, AF78Ctx *);
s32 func_0019A1D8(s32, s32, s32);
s32 func_0018DA08();
s32 func_0018DAD0(s32 *);

s32 func_0016AF78(s32 arg1, s32 arg2)
{
    AF78Ctx ctx;
    s32 v;
    s32 R;
    s32 *link;
    s32 *sent;
    s32 *cursor;
    s32 id;
    s32 i;
    s32 found;
    s32 node;
    s32 flag;

    v = func_0016ACE8(arg1, arg2);
    if (func_001893D0(arg2, 0x10, v, 0x304) == 0) {
        return 0;
    }
    if (func_001893D0(arg2, 1, 4, 0x304) == 0) {
        return 0;
    }
    ctx.local0 = 0;
    func_0016BD48(arg1, (s32)func_0016C6A8, (s32)&ctx.local0);
    ctx.local2 = ctx.local0;
    func_00189728(&ctx.local2, 4);
    if (func_00197458(arg2, &ctx.local2, 4) == 0) {
        return 0;
    }
    if (func_00189F88(ctx.local3, *(s32 *)(arg1 + 4)) == 0) {
        return 0;
    }
    if (func_00189980(ctx.local3, arg2) == 0) {
        func_0018A0A0(ctx.local3);
        return 0;
    }
    ctx.len = 0;
    R = func_0016C380(arg1);
    ctx.alloc = ((AF78Func *)&D_003731B8)[75](R * 4);
    if (ctx.alloc == 0) {
        ctx.buffer0 = 2;
        ctx.sysres = func_0018DA08(0x80000013, R * 4);
        func_0018DAD0(&ctx.buffer0);
        return 0;
    }
    sent = (s32 *)(arg1 + 8);
    link = *(s32 **)sent;
    cursor = (s32 *)ctx.alloc;
    if (link != sent) {
        do {
            id = link[-10];
            found = 0;
            if (ctx.len > 0) {
                i = 0;
                do {
                    if (((s32 *)ctx.alloc)[i] == id) {
                        found = 1;
                        break;
                    }
                    i++;
                } while (i < ctx.len);
            }
            if (found == 0) {
                func_0016E028(id);
                cursor[ctx.len] = id;
                ctx.len = ctx.len + 1;
            }
            link = (s32 *)link[0];
        } while (link != sent);
    }
    if (func_00169F88(&ctx.alloc, arg2) == 0) {
        if (ctx.len == 0) {
            return 0;
        }
        for (i = 0; i < ctx.len; i++) {
            func_0016DFE8(((s32 *)ctx.alloc)[i]);
        }
        ((AF78Func *)&D_003731B8)[76](ctx.alloc);
        return 0;
    }
    ctx.flag = 1;
    ctx.arg2copy = arg2;
    link = *(s32 **)sent;
    if (link != sent) {
        node = (s32)((s8 *)link - 0x40);
        do {
            if (func_0016A218(node, &ctx) == 0) {
                break;
            }
            link = (s32 *)link[0];
            node = (s32)((s8 *)link - 0x40);
        } while (link != sent);
    }
    if (ctx.len != 0) {
        for (i = 0; i < ctx.len; i++) {
            func_0016DFE8(((s32 *)ctx.alloc)[i]);
        }
        ((AF78Func *)&D_003731B8)[76](ctx.alloc);
    }
    func_0018A0A0(ctx.local3);
    if (ctx.flag == 0) {
        return 0;
    }
    if (func_0019A1D8((s32)D_00372490, arg2, arg1) == 0) {
        arg1 = 0;
    }
    return arg1;
}

INCLUDE_ASM(const s32, "core/main", func_0016B2B0);

INCLUDE_ASM(const s32, "core/main", func_0016BBF8);

INCLUDE_ASM(const s32, "core/main", func_0016BCA0);

INCLUDE_ASM(const s32, "core/main", func_0016BD48);

/* Byte-matching.  Initialises through func_0019A830, sets the byte's bit 0 at
   +3 and returns the object.  Kept as s32 to match the declaration used by
   func_0014DA58. */
s32 func_0016BDC0(s32 arg0)
{


    func_0019A830(arg0);
    *(u8 *)(arg0 + 3) |= 1;
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_0016BDF8);

INCLUDE_ASM(const s32, "core/main", func_0016BE90);

s32 func_0016BF10(s32 arg, u8 *n)
{
    *(s32 *)*(s32 *)(n + 0x44) = *(s32 *)(n + 0x40);
    *(s32 *)(*(s32 *)(n + 0x40) + 4) = *(s32 *)(n + 0x44);
    *(s32 *)(n + 0x3C) = 0;
    return arg;
}

INCLUDE_ASM(const s32, "core/main", func_0016BF38);

INCLUDE_ASM(const s32, "core/main", func_0016BF68);

INCLUDE_ASM(const s32, "core/main", func_0016C020);

s32 func_0016C0B8(s32 a1)
{

    u8 *p = (u8 *)a1;
    s32 *q = (s32 *)a1;

    func_0019A700((s32)&D_00372478, p);
    if (q[5] != 0)
        func_00196788((u8 *)q[5]);
    if (q[6] != 0)
    {
        func_0016DFE8((u8 *)q[6]);
        q[6] = 0;
        if (q[1] != 0)
            func_0018E670((u8 *)q[1]);
    }
    func_0019A890(p);
    ((s32 (*)(s32, s32))D_003731B8[80])(*(s32 *)((s32)D_00393C90 + (u32)D_003731B8), a1);
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0016C150);

INCLUDE_ASM(const s32, "core/main", func_0016C290);

u8 *func_0016C348(u8 *arg0, s32 arg1)
{
    if (arg1 != 0)
        *(s32 *)(arg0 + 0x18) = arg1;
    else
        *(s32 *)(arg0 + 0x18) = (s32)func_0016C7F8;
    return arg0;
}

s32 func_0016C368(u8 *p)
{
    s32 field = *(s32 *)(p + 0x18);

    return field == (s32)func_0016C7F8 ? 0 : field;
}

/* Byte-matching.  Registers func_0016C6A8 as the callback, waits for it and
   returns what it stored. */
s32 func_0016C380(s32 arg0)
{
    s32 out = 0;

    func_0016BD48(arg0, (s32)func_0016C6A8, (s32)&out);
    return out;
}

INCLUDE_ASM(const s32, "core/main", func_0016C3B0);

INCLUDE_ASM(const s32, "core/main", func_0016C400);

s32 func_0016C468(s32 arg1, s32 arg2)
{
    s32 v = 1;
    s32 args[6];
    args[0] = arg2;
    args[5] = v;
    args[2] = 0;
    args[4] = 0;
    return func_0016A218(arg1, (AF78Ctx *)args);
}

INCLUDE_ASM(const s32, "core/main", func_0016C498);

s32 func_0019A490();

s32 func_0016C548(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_00372478, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A490();

s32 func_0016C590(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_00372490, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A198();

s32 func_0016C5D8(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_00372478, arg1, arg2, arg3, arg4);
}

s32 func_0019A198();

s32 func_0016C610(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_00372490, arg1, arg2, arg3, arg4);
}

s32 func_0016C648(s32 arg)
{
    return func_0019A638(D_00372478, arg);
}

s32 func_0016C670(s32 arg)
{
    return func_0019A638(D_00372490, arg);
}

s32 func_0016C698(void) { return 1; }

s32 func_0016C6A0(void) { return 1; }

u8 *func_0016C6A8(u8 *p, s32 *counter)
{
    (*counter)++;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0016C6C0);

struct Off38 { char pad[0x38]; s32 v; };

s32 func_0016C708(u8 *arg0)
{
    s32 v = *(s32 *)(arg0 + 0x6C);
    if (v == 0)
        v = ((struct Off38 *)((u8 *)D_003731B8 + D_003937B0))->v;
    if (func_001A3BA0(v, arg0, 1) == 0)
        return 0;
    return (s32)arg0;
}

INCLUDE_ASM(const s32, "core/main", func_0016C758);

s32 func_0016C7F8(s32 value) { return value; }

s32 func_0016C800();

s32 func_0016C800(s32 arg1)
{
    func_0016C0B8(arg1);
    return arg1;
}

s32 func_0016C828(s32 arg0, s32 *a2)
{
    *a2 += 28;
    *a2 += 12 + func_0019A3A8(D_00372478, arg0);
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_0016C888);

INCLUDE_ASM(const s32, "core/main", func_0016C8D0);

void func_0016CA80(void) { func_0016DFE8(); }

INCLUDE_ASM(const s32, "core/main", func_0016CA88);

INCLUDE_ASM(const s32, "core/main", func_0016CBD0);

INCLUDE_ASM(const s32, "core/main", func_0016CD60);

INCLUDE_ASM(const s32, "core/main", func_0016CE78);

INCLUDE_ASM(const s32, "core/main", func_0016D160);

INCLUDE_ASM(const s32, "core/main", func_0016D618);

INCLUDE_ASM(const s32, "core/main", func_0016DDA8);

INCLUDE_ASM(const s32, "core/main", func_0016DE20);

s32 func_0016DE88(void) { return D_003724B4[0]; }

INCLUDE_ASM(const s32, "core/main", func_0016DE98);

s32 func_0016DF70(f32 a1)
{




    s32 v2;
    s32 v3;
    s32 v4;
    s32 v5;
    s32 result;
    v2 = func_0016CE78(0, 0, 0);
    v3 = v2;
    if (v2 != 0)
    {
        v4 = *(u32 *)(v2 + 64);
        *(f32 *)(v4 + 16) = a1;
        *(u32 *)(v4 + 8) = 0;
        *(u32 *)(v4 + 12) = 0;
        *(u32 *)(v4 + 4) = 0;
    }
    v5 = func_0016CD60(v3) != 0;
    result = v3;
    if (!v5)
    {
        func_0016DFE8(v3);
        return 0;
    }
    return result;
}

/* Byte-matching.  Decrements the counter at +0xA and fetches more work only
   once it has run out. */
s32 func_0016DFE8(u8 *arg0)
{

    s16 *count = (s16 *)(arg0 + 0xA);
    s32 r = 1;

    (*count)--;
    if (*count <= 0)
        r = func_0016C8D0((s32)arg0);
    return r;
}

u8 *func_0016E028(u8 *p)
{
    *(u16 *)(p + 0xA) += 1;
    return p;
}

s32 func_0016E040(const s32 *p) { return *(const s32 *)((const u8 *)p + (24)); }

s32 func_0016E048(const s32 *p) { return *(const s32 *)((const u8 *)p + (28)); }

s32 func_0016E050(const s32 *p) { return *(const s32 *)((const u8 *)p + (16)); }

INCLUDE_ASM(const s32, "core/main", func_0016E058);

s32 func_0016E158(const s32 *p) { return *(const s32 *)((const u8 *)p + (32)); }

u8 *func_0016E160(u8 *p, s32 index)
{
    return *(u8 **)(p + 0x40) + index * 0x1C;
}

/* Byte-matching.  Forwards to func_0016CA88 with the field at +0x1C of the
   object's first member, and returns the object. */
u8 *func_0016E178(u8 *arg0, s32 arg1)
{


    func_0016CA88(arg0, *(s32 *)(*(u32 *)arg0 + 0x1C), arg1);
    return arg0;
}

u8 *func_0016E1B0(u8 *arg0, u8 *arg1)
{
    *(U16 *)(arg0 + 4) = *(U16 *)arg1;
    return arg0;
}

u8 *func_0016E1D8(u8 *p) { return p + 4; }

/* Byte-matching. */
u8 *func_0016E1E0(u8 *arg0)
{
    u8 *p = *(u8 **)(arg0 + 0xC);

    if (p != 0) {
        u8 *q = *(u8 **)p;

        if (q != 0)
            return q + 0xC;
    }
    return 0;
}

s32 func_0016E208(u8 *a1, s32 a2)
{
    s32 v4;
    s32 v5;
    s32 v7;
    v4 = *(s32 *)(a1 + 0x10);
    v5 = 0;
    if (v4 > 0) {
        for (;;) {
            v7 = 4 * v5;
            v5++;
            func_0016F958(*(s32 *)(v7 + *(s32 *)(a1 + 0xC)), a2);
            if (v5 >= v4) break;
        }
    }
    return (s32)a1;
}

s32 func_0016E280(const s32 *p) { return *(const s32 *)((const u8 *)p + (36)); }

s32 func_0016E288(const s32 *p) { return *(const s32 *)((const u8 *)p + (40)); }

s32 func_0016E290(const s32 *p) { return *(const s32 *)((const u8 *)p + (44)); }

s32 func_0016E298(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_0016E2A0(const s32 *p) { return *(const s32 *)((const u8 *)p + (24)); }

/* Byte-matching: callback loop over a linked array. */
s32 func_0016E2A8(s32 a1, s32 (*a2)(s32, s32), s32 a3)
{
    s32 v6 = *(s32 *)(a1 + 16);
    s32 v7 = 0;

    if (v6 > 0)
    {
        s32 *i = *(s32 **)(a1 + 12);
        for (;;)
        {
            s32 v9 = a2(i[v7], a3);
            if (v9 == 0) break;
            v7++;
            if (v7 >= v6) break;
            i = *(s32 **)(a1 + 12);
        }
    }
    return a1;
}


u8 *func_0016E330(u8 *a1, s32 a2)
{

    *(u16 *)(a1 + 8) |= a2;
    if (a2 & 1) {
        s32 *p = *(s32 **)(a1 + 60);
        if (p == 0) return a1;
        func_00170C38(p);
        *(s32 *)(a1 + 60) = 0;
    }
    return a1;
}

s32 func_0016E380(u8 *p, s32 index)
{
    return *(s32 *)(*(u32 *)(p + 0xC) + index * 4);
}

s32 func_0016E398(u8 *a1, s32 a2, s32 a3)
{

    s32 v = *(s32 *)(a1 + 0x3C);

    if (v != 0) {
        if (func_00170DF8(v, a2, a3) != 0) return (s32)a1;
    }
    return 0;
}

u8 *func_0016E3E0(u8 *p, u8 *out, u16 a, u16 b, u16 c)
{
    *(u16 *)out = a;
    *(u16 *)(out + 0x2) = b;
    *(u16 *)(out + 0x4) = c;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0016E3F8);

/* Byte-matching.  Copies the three halfwords at arg1 into whichever of the
   optional outputs were supplied, and returns arg0. */
u8 *func_0016E4E8(u8 *arg0, u8 *arg1, u16 *arg2, u16 *arg3, u16 *arg4)
{
    if (arg2 != 0)
        *arg2 = *(u16 *)(arg1 + 0);
    if (arg3 != 0)
        *arg3 = *(u16 *)(arg1 + 2);
    if (arg4 != 0)
        *arg4 = *(u16 *)(arg1 + 4);
    return arg0;
}

/* Byte-matching: bnel table lookup; s16 index from a2+6, table base from a1+0xC. */
s32 func_0016E520(s32 a1, s32 a2)
{
    s16 v = *(s16 *)((u8 *)a2 + 6);

    if (v == -1)
        return 0;
    return *(s32 *)((v << 2) + *(s32 *)((u8 *)a1 + 12));
}

INCLUDE_ASM(const s32, "core/main", func_0016E548);

INCLUDE_ASM(const s32, "core/main", func_0016E668);

INCLUDE_ASM(const s32, "core/main", func_0016E740);

s32 func_0019A490();

s32 func_0016E7F8(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_003724A8, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A198();

s32 func_0016E840(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_003724A8, arg1, arg2, arg3, arg4);
}

s32 func_0016E878(s32 arg)
{
    return func_0019A638(D_003724A8, arg);
}

s32 func_0016E8A0(void) { return 1; }

void func_0016E8A8(void) { func_0016EEA0(); }

INCLUDE_ASM(const s32, "core/main", func_0016E8B0);

INCLUDE_ASM(const s32, "core/main", func_0016E9F8);

INCLUDE_ASM(const s32, "core/main", func_0016EBB0);

INCLUDE_ASM(const s32, "core/main", func_0016EC98);

INCLUDE_ASM(const s32, "core/main", func_0016ED00);

INCLUDE_ASM(const s32, "core/main", func_0016ED78);

INCLUDE_ASM(const s32, "core/main", func_0016EDC0);

s32 func_0016EEA0(s32 a1)
{
    s32 *p;


    func_0019A700((s32)D_003724C0, a1);
    func_0019A890(a1);
    p = (s32 *)((s32)D_00393CA0 + (u32)D_003731B8);
    ((s32 (*)(s32, s32, s32))D_003731B8[80])(*p, a1, (s32)p);
    return 1;
}

s32 func_0016EF00();
s32 func_0019A830();

s32 func_0016EF00(s32 arg1)
{
    func_0019A830(arg1);
    return arg1;
}

s32 func_0016EF28(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

u8 func_0016EF30(const u8 *p) { return p[1]; }

u8 *func_0016EF38(u8 *p, u8 value) { p[2] = value; return p; }

u8 func_0016EF48(const u8 *p) { return p[2]; }

f32 func_0016EF50(const f32 *p) { return p[5]; }

/* Byte-matching.  The f32 store is unconditional: retail placed it in the
   delay slot of the beqz, so it cannot have been inside the branch. */
u8 *func_0016EF58(s32 arg0, f32 arg1)
{

    u8 *p = (u8 *)arg0;
    s32 r;

    r = func_0016EF28((const s32 *)p);
    *(f32 *)(p + 0x14) = arg1;
    if (r != 0)
        func_0018E670((u8 *)r);
    return p;
}

u8 *func_0016EFA0(u8 *p) { return p + 24; }

typedef struct { UnalignedU64 lo, hi; } Unaligned16;
u8 *func_0016EFA8(u8 *a1, u8 *a2)
{
    f32 v3;
    *(Unaligned16 *)(a1 + 0x18) = *(Unaligned16 *)a2;
    v3 = *(f32 *)(a1 + 0x18);
    if (v3 != *(f32 *)(a1 + 0x1C)) {
        a1[3] = 0;
    } else if (v3 == *(f32 *)(a1 + 0x20)) {
        a1[3] = 1;
    } else {
        a1[3] = 0;
    }
    return a1;
}

s32 func_00217E28(f32 arg);
s32 func_0016F008(u8 *p)
{
    return func_00217E28(-*(f32 *)(p + 0x28));
}

/* Byte-matching.  Retail never touches $f12, but the call sites in this file
   pass a second argument, so it stays in the signature. */
u8 *func_0016F028(s32 arg0, f32 unused)
{
    u8 *p = (u8 *)arg0;
    f32 v = -((f32 (*)(s32))func_00217AC0)(arg0);

    *(f32 *)(p + 0x28) = v;
    return p;
}

s32 func_0016F058(s32 arg0)
{
    return func_0019A3A8(D_003724C0, arg0) + 48;
}

s32 func_0019A490();

s32 func_0016F080(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_003724C0, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A198();

s32 func_0016F0C8(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_003724C0, arg1, arg2, arg3, arg4);
}

s32 func_0016F100(s32 arg)
{
    return func_0019A638(D_003724C0, arg);
}

s32 func_0016F128(void) { return 1; }

s32 func_0016F130(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_0016F138);

INCLUDE_ASM(const s32, "core/main", func_0016F280);

/* Byte-matching: volatile counter keeps a single load at the join point. */
extern s32 D_00393CA8;
extern volatile s32 D_00393CAC;

s32 func_0016F620(s32 arg0)
{
    s32 *p;

    p = (s32 *)((s32)D_00393CA8 + (u32)D_003731B8);
    if (*p != 0) {
        func_001956D0(*p);
        p = (s32 *)((s32)D_00393CA8 + (u32)D_003731B8);
        *p = 0;
    }
    D_00393CAC = D_00393CAC - 1;
    return arg0;
}

s32 func_0016F688(s32 a1, s32 a2)
{
    s32 v3;

    D_00393CA8 = a2;
    v3 = func_00195578(D_003724D8[0], 20, 0);
    *(s32 *)((u8 *)D_003731B8 + D_00393CA8) = v3;
    if (v3 == 0) return 0;
    ++D_00393CAC;
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_0016F6F0);

s32 func_0016F738(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 2;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_00393644;
    __asm__ volatile("" ::: "memory");
    D_00393644++;
    return ret;
}

INCLUDE_ASM(const s32, "core/main", func_0016F780);

s32 func_0016F830(u8 *a1)
{

    s32 v2;
    u32 base;
    u8 *p;
    s32 (*fn)(s32, u8 *);

    v2 = *(u16 *)(a1 + 0x18);
    *(u16 *)(a1 + 0x18) = v2 - 1;
    if ((v2 - 1) << 16 <= 0)
    {
        *(u16 *)(a1 + 0x18) = v2;
        if (v2 << 16 > 0)
        {
            func_0019A700((s32)&D_003724D8, a1);
            func_0016F8D8(a1, 0);
            base = (u32)D_003731B8;
            fn = *(s32 (**)(s32, u8 *))(base + 0x140);
            p = (u8 *)(base + D_00393CA8);
            *(u16 *)(a1 + 0x18) = *(u16 *)(a1 + 0x18) - 1;
            fn(*(s32 *)p, a1);
        }
    }
    return 1;
}

u8 *func_0016F8C0(u8 *p)
{
    *(u16 *)(p + 0x18) += 1;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0016F8D8);

s32 func_0016F930(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

u8 *func_0016F938(u8 *dst, const u8 *src)
{
    __builtin_memcpy(dst + 4, src, 4);
    return dst;
}

u8 *func_0016F950(u8 *p) { return p + 4; }

u8 *func_0016F958(u8 *arg0, u8 *arg1)
{
    *(S12 *)(arg0 + 0xC) = *(S12 *)arg1;
    return arg0;
}

u8 *func_0016F978(u8 *p) { return p + 12; }

s32 func_0019A490();

s32 func_0016F980(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_003724D8, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A198();

s32 func_0016F9C8(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_003724D8, arg1, arg2, arg3, arg4);
}

s32 func_0016FA00(s32 arg)
{
    return func_0019A638(D_003724D8, arg);
}

s32 func_0016FA28(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0016FA30);

INCLUDE_ASM(const s32, "core/main", func_0016FA88);

INCLUDE_ASM(const s32, "core/main", func_0016FB70);

INCLUDE_ASM(const s32, "core/main", func_0016FD90);


/* Source: engine/region_0017.c */
/* func_00170180 through func_0017FF38; retail address range 0x00170000-0x00180000. */



extern s32 D_00393654;
extern s32 D_00393658;
extern s32 D_0039365C;
extern s32 D_00393660;

u8 *func_00170180(u8 *p)
{
    *(s32 *)(p + 0x8) = 0;
    *(s32 *)p = 0;
    *(s32 *)(p + 0x4) = 0;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00170198);

s32 func_00170228(u8 *p, s32 index)
{
    return *(s32 *)(*(u32 *)p + index * 4);
}

INCLUDE_ASM(const s32, "core/main", func_00170240);

INCLUDE_ASM(const s32, "core/main", func_00170358);

INCLUDE_ASM(const s32, "core/main", func_00170460);

INCLUDE_ASM(const s32, "core/main", func_00170498);

INCLUDE_ASM(const s32, "core/main", func_00170558);

INCLUDE_ASM(const s32, "core/main", func_001706A0);

INCLUDE_ASM(const s32, "core/main", func_00170818);

INCLUDE_ASM(const s32, "core/main", func_001709E8);

INCLUDE_ASM(const s32, "core/main", func_00170A88);

INCLUDE_ASM(const s32, "core/main", func_00170AE8);

INCLUDE_ASM(const s32, "core/main", func_00170BD0);

INCLUDE_ASM(const s32, "core/main", func_00170C38);

INCLUDE_ASM(const s32, "core/main", func_00170C90);

INCLUDE_ASM(const s32, "core/main", func_00170DA8);

INCLUDE_ASM(const s32, "core/main", func_00170DF8);

s32 func_00170E88(s32 *p)
{
    s32 t = (*(s32 *)((u8 *)p + 8)) * 4;
    s32 b = (*(u16 *)((u8 *)p + 4)) * 8;
    s32 s = t + 12;
    return b + s;
}

INCLUDE_ASM(const s32, "core/main", func_00170EA8);

INCLUDE_ASM(const s32, "core/main", func_00170FB0);

INCLUDE_ASM(const s32, "core/main", func_00171228);

INCLUDE_ASM(const s32, "core/main", func_001714D8);

INCLUDE_ASM(const s32, "core/main", func_001715F0);

INCLUDE_ASM(const s32, "core/main", func_001719B8);

INCLUDE_ASM(const s32, "core/main", func_00171D98);

INCLUDE_ASM(const s32, "core/main", func_00172230);

INCLUDE_ASM(const s32, "core/main", func_00172C70);

INCLUDE_ASM(const s32, "core/main", func_00173668);

INCLUDE_ASM(const s32, "core/main", func_00173890);

INCLUDE_ASM(const s32, "core/main", func_00173E48);

INCLUDE_ASM(const s32, "core/main", func_001743F8);

INCLUDE_ASM(const s32, "core/main", func_00174530);

INCLUDE_ASM(const s32, "core/main", func_00174658);

INCLUDE_ASM(const s32, "core/main", func_00174860);

INCLUDE_ASM(const s32, "core/main", func_001749D0);

INCLUDE_ASM(const s32, "core/main", func_00174B60);

INCLUDE_ASM(const s32, "core/main", func_00174EB8);

INCLUDE_ASM(const s32, "core/main", func_00175038);

INCLUDE_ASM(const s32, "core/main", func_00175100);

INCLUDE_ASM(const s32, "core/main", func_00175188);

INCLUDE_ASM(const s32, "core/main", func_00175208);

s32 func_00175288(s32 a1)
{
    s32 v1;
    s32 v2;
    s32 v3;
    s32 *p;

    v1 = a1;
    v2 = *(s32 *)(a1 + 0x54);
    v3 = v1;
    if (v2 == 0)
    {
        p = D_003731B8;
        v2 = *(s32 *)(p[1] + 0x70);
        if (v2 == 0)
            v2 = *(s32 *)(D_003937B0 + (s32)p + 0x3C);
        v3 = v1;
    }
    if (func_001A3BA0(v2, v3, 1) == 0)
        return 0;
    return v1;
}

INCLUDE_ASM(const s32, "core/main", func_001752F0);

INCLUDE_ASM(const s32, "core/main", func_00175390);

INCLUDE_ASM(const s32, "core/main", func_00175418);

INCLUDE_ASM(const s32, "core/main", func_001754A0);

/* Byte-matching: twin of func_0016E2A8. */
s32 func_00175528(s32 a1, s32 (*a2)(s32, s32), s32 a3)
{
    s32 v6 = *(s32 *)(a1 + 16);
    s32 v7 = 0;

    if (v6 > 0)
    {
        s32 *i = *(s32 **)(a1 + 12);
        for (;;)
        {
            s32 v9 = a2(i[v7], a3);
            if (v9 == 0) break;
            v7++;
            if (v7 >= v6) break;
            i = *(s32 **)(a1 + 12);
        }
    }
    return a1;
}


INCLUDE_ASM(const s32, "core/main", func_001755B0);

INCLUDE_ASM(const s32, "core/main", func_00175678);

extern s32 D_003731B8[];
extern s32 func_00176FA8();
extern u8 *func_00175A38(u8 *arg0);

s32 func_00175718(s32 arg0)
{
    func_00176FA8(D_003731B8[0], (s32)func_00175A38, arg0);
    return arg0;
}

s32 *func_00175758(s32 *p, s32 value) { p[2] = value; return p; }

s32 func_00175768(const s32 *p) { return *(const s32 *)((const u8 *)p + (8)); }

s32 func_00175770(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 2;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_00393650;
    __asm__ volatile("" ::: "memory");
    D_00393650++;
    return ret;
}

s32 func_001757B8(u8 *p, s32 index)
{
    return *(s32 *)(*(u32 *)(p + 0xC) + index * 4);
}

s32 func_001757D0(const s32 *p) { return *(const s32 *)((const u8 *)p + (16)); }

s32 func_001757D8(const s32 *p) { return *(const s32 *)((const u8 *)p + (28)); }

u8 *func_001757E0(u8 *p) { return p + 72; }

u8 *func_001757E8(u8 *p) { return p + 60; }

u8 *func_001757F0(u8 *arg0, u8 *arg1)
{
    *(S12 *)(arg0 + 0x60) = *(S12 *)arg1;
    return arg0;
}

u8 *func_00175810(u8 *p) { return p + 96; }

s32 *func_00175818(s32 *p, s32 value) { p[0x1B] = value; return p; }

s32 func_00175828(const s32 *p) { return *(const s32 *)((const u8 *)p + (108)); }

INCLUDE_ASM(const s32, "core/main", func_00175830);

u8 *func_00175900(u8 *p, u8 value) { p[2] = value; return p; }

u8 func_00175910(const u8 *p) { return p[2]; }

s32 func_00175918(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    if (func_00188B68((s32 *)(a1 + 12), *(u8 *)(a1 + 2), a2, a3, a4, a5) == 0)
        return 0;
    return a1;
}

s32 func_0019A490();

s32 func_00175968(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_00372518, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A198();

s32 func_001759B0(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_00372518, arg1, arg2, arg3, arg4);
}

s32 func_001759E8(s32 arg)
{
    return func_0019A638(D_00372518, arg);
}

s32 func_00175A10(void) { return 1; }

s32 func_00175A18(s32 val, s32 *p)
{
    if (*(s32 *)p == val) {
        *(s32 *)(p + 1) = 1;
        return 0;
    }
    return val;
}

/* Byte-matching.  Frees the object through func_001743F8 when func_00188D28
   reports it as allocated, and returns it either way. */
u8 *func_00175A38(u8 *arg0)
{



    if (func_00188D28() != 0)
        func_001743F8(arg0);
    return arg0;
}

/* Byte-matching: volatile counter + extra call; see func_0016F620 cluster. */
extern s32 D_00393CB0;
extern volatile s32 D_00393CB4;

s32 func_00175A70(s32 arg0)
{
    s32 *p;

    p = (s32 *)((s32)D_00393CB0 + (u32)D_003731B8);
    if (*p != 0) {
        func_001956D0(*p);
        p = (s32 *)((s32)D_00393CB0 + (u32)D_003731B8);
        *p = 0;
    }
    func_00178640();
    D_00393CB4 = D_00393CB4 - 1;
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_00175AE0);

INCLUDE_ASM(const s32, "core/main", func_00175B70);

INCLUDE_ASM(const s32, "core/main", func_00175DC0);

INCLUDE_ASM(const s32, "core/main", func_00176118);

INCLUDE_ASM(const s32, "core/main", func_00176410);

INCLUDE_ASM(const s32, "core/main", func_00176590);

INCLUDE_ASM(const s32, "core/main", func_001766B0);

INCLUDE_ASM(const s32, "core/main", func_001767E8);

s32 func_001769A0(s32 a1, s32 a2)
{


    u32 *v2 = (u32 *)(a2 + D_00393654);
    if (v2[3] != 0) {
        if (*v2 != 0) ((s32 (*)(s32))D_003732E8[0])(*v2);
        v2[0] = 0;
        v2[1] = 0;
        v2[2] = 0;
        v2[3] = 0;
        return a1;
    }
    return 0;
}

s32 func_00176A10(u8 *arg0, u8 *arg1)
{
    s32 *p = (s32 *)((u8 *)arg1 + D_00393654 + 0xC);
    s32 v = func_0018C5D8(arg1);
    if (v != 0) {
        func_0018E670(v);
        *p = (s32)arg0;
    } else {
        *p = (s32)arg0;
    }
    return (s32)arg0;
}

s32 func_00176A60(const u8 *p) { return *(const s32 *)(p + D_00393654 + 0xC); }

INCLUDE_ASM(const s32, "core/main", func_00176A70);

s32 func_00176B58(s32 a1, u8 *a2)
{

    s32 *v2 = (s32 *)(a2 + D_00393658);
    if (*(s32 *)(a2 + 4) != 0)
        func_0018E670((u8 *)*(s32 *)(a2 + 4));
    *v2 = a1;
    return a1;
}

s32 func_00176BA0(const u8 *p) { return *(const s32 *)(p + D_00393658); }

INCLUDE_ASM(const s32, "core/main", func_00176BB0);

INCLUDE_ASM(const s32, "core/main", func_00176C78);

s32 func_00176D30(const u8 *p) { return *(const s32 *)(p + D_0039365C); }

INCLUDE_ASM(const s32, "core/main", func_00176D40);

INCLUDE_ASM(const s32, "core/main", func_00176DE8);

s32 func_00176EC0(const u8 *p) { return *(const s32 *)(p + D_00393660); }

INCLUDE_ASM(const s32, "core/main", func_00176ED0);

INCLUDE_ASM(const s32, "core/main", func_00176FA8);

INCLUDE_ASM(const s32, "core/main", func_00177028);

INCLUDE_ASM(const s32, "core/main", func_001770E0);

INCLUDE_ASM(const s32, "core/main", func_00177158);

INCLUDE_ASM(const s32, "core/main", func_001771E0);

INCLUDE_ASM(const s32, "core/main", func_00177248);

INCLUDE_ASM(const s32, "core/main", func_001772F8);

INCLUDE_ASM(const s32, "core/main", func_00177388);

s32 func_00176A10();

u8 *func_001773E8(u8 *a1, u8 *a2)
{

    s32 off = D_00393654;
    s32 *p = (s32 *)((u8 *)a1 + off);
    u8 *q = (u8 *)a2 + off;
    s32 v = *(s32 *)(q + 0xC);
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    if (v != 0)
        func_00176A10(v, a1);
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_00177438);

INCLUDE_ASM(const s32, "core/main", func_001774A8);

s32 func_001774E0(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_001774E8);

u8 *func_001775C8(u8 *p)
{
    *(s32 *)(p + D_0039365C) = 0;
    return p;
}

s64 func_001775E0(s64 a1, s32 a2)
{
    s32 *v9 = (s32 *)(a2 + D_0039365C);
    if (*v9 != 0) {
        func_00176BB0(*v9, a1);
        return a1;
    }
    return a1;
}

s32 func_00177620(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_00177628);

s64 func_00177650(s64 a1, s32 a2)
{
    s32 *v3 = (s32 *)(a2 + D_00393660);
    if (*v3 != 0) {
        func_00176D40(*v3, a1);
        return a1;
    }
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_00177690);

s32 func_00176B58();
s32 func_00177738();

s32 func_00177738(s32 arg1, s32 arg2)
{
    func_00176B58(arg2, arg1);
    return arg1;
}

s32 func_00176A70();
s32 func_00177768();

s32 func_00177768(s32 arg1, s32 arg2)
{
    func_00176A70(arg2, arg1);
    return arg1;
}

s32 func_00177798(s32 a0, s32 a1, s32 a2)
{
    return func_001706A0(*(s32 *)(a2 + 0x3C), a2, a0, a2 + 0xC);
}

s32 func_001777C8(s32 a1, s32 a2, s32 a3)
{
    s32 v;

    v = func_00170818(a1, a3, a3 + 0xC);
    *(s32 *)(a3 + 0x3C) = v;
    return v != 0 ? a1 : 0;
}

s32 func_00170E88(s32 *);

s32 func_00177810(s32 *arg1)
{
    return func_00170E88((s32 *)arg1[15]);
}

s32 func_00177830(s32 a0, s32 a1, s32 a2)
{
    s32 v = func_00175830(a2);
    return func_001706A0(*(s32 *)(a2 + 0x50), v, a0, v + 0xC);
}

s32 func_00177878(u8 *arg0, s32 arg1, u8 *arg3)
{
    s32 v = func_00175830(arg3);
    s32 r = func_00170818(arg0, v, v + 0xC);
    *(s32 *)(arg3 + 0x50) = r;
    return r != 0 ? (s32)arg0 : 0;
}

s32 func_00170E88(s32 *);

s32 func_001778D0(s32 *arg1)
{
    return func_00170E88((s32 *)arg1[20]);
}

INCLUDE_ASM(const s32, "core/main", func_001778F0);

INCLUDE_ASM(const s32, "core/main", func_00177930);

INCLUDE_ASM(const s32, "core/main", func_00177968);

INCLUDE_ASM(const s32, "core/main", func_001779F0);

INCLUDE_ASM(const s32, "core/main", func_00177AF0);

INCLUDE_ASM(const s32, "core/main", func_00177C58);

INCLUDE_ASM(const s32, "core/main", func_00178530);

INCLUDE_ASM(const s32, "core/main", func_00178640);

INCLUDE_ASM(const s32, "core/main", func_00178700);

s32 func_001788C8(void) { return 1; }

s32 func_001788D0(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x48);
}

INCLUDE_ASM(const s32, "core/main", func_001788E8);

s32 func_00178920(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x3c);
}

u8 *func_00178938(u8 *p, u8 *out)
{


    s32 stack[2];
    u8 *r;

    if (p != 0) {
        *(s32 *)(p + 0x70) = (s32)out;
        r = p;
    } else {
        stack[0] = 2;
        stack[1] = func_0018DA08(0x80000016);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(stack);
        r = 0;
        __asm__ volatile("nop");
    }
    return r;
}

u8 *func_00178988(u8 *p, u8 *out)
{


    s32 stack[2];
    if (p == 0) goto fallback;
    *(s32 *)out = *(s32 *)(p + 0x70);
    return p;
fallback:
    stack[0] = 2;
    stack[1] = func_0018DA08(0x80000016);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(stack);
    return 0;
}

u8 *func_001789D8(u8 *p, u8 *out)
{


    s32 stack[2];
    u8 *r;

    if (p != 0) {
        *(s32 *)(p + 0x54) = (s32)out;
        r = p;
    } else {
        stack[0] = 2;
        stack[1] = func_0018DA08(0x80000016);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(stack);
        r = 0;
        __asm__ volatile("nop");
    }
    return r;
}

u8 *func_00178A28(u8 *p, u8 *out)
{


    s32 stack[2];
    if (p == 0) goto fallback;
    *(s32 *)out = *(s32 *)(p + 0x54);
    return p;
fallback:
    stack[0] = 2;
    stack[1] = func_0018DA08(0x80000016);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(stack);
    return 0;
}

s32 func_00178A78(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x44);
}

s32 func_00178A90(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x38);
}

INCLUDE_ASM(const s32, "core/main", func_00178AA8);

u8 *func_00178AE0(u8 *p, u8 *out)
{


    s32 stack[2];
    u8 *r;

    if (p != 0) {
        *(s32 *)(p + 0x6C) = (s32)out;
        r = p;
    } else {
        stack[0] = 2;
        stack[1] = func_0018DA08(0x80000016);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(stack);
        r = 0;
        __asm__ volatile("nop");
    }
    return r;
}

u8 *func_00178B30(u8 *p, u8 *out)
{


    s32 stack[2];
    if (p == 0) goto fallback;
    *(s32 *)out = *(s32 *)(p + 0x6C);
    return p;
fallback:
    stack[0] = 2;
    stack[1] = func_0018DA08(0x80000016);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(stack);
    return 0;
}

s32 func_00178B80(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x44);
}

INCLUDE_ASM(const s32, "core/main", func_00178B98);

s32 func_00178BD0(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x40);
}

INCLUDE_ASM(const s32, "core/main", func_00178BE8);

u8 *func_00178C40(u8 *p, u8 *out)
{


    s32 stack[2];
    if (p == 0) goto fallback;
    *(s32 *)out = *(s32 *)(p + 0x8);
    return p;
fallback:
    stack[0] = 2;
    stack[1] = func_0018DA08(0x80000016);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(stack);
    return 0;
}

s32 func_00178C90(u8 *a1, u8 *a2)
{
    s16 v1 = *(s16 *)(a1 + 4);
    s16 v2 = *(s16 *)(a2 + 4);
    u16 v3, v4;

    if (v2 < v1)
        return 1;
    if (v1 < v2)
        return -1;
    v3 = *(u16 *)(a1 + 6);
    v4 = *(u16 *)(a2 + 6);
    if (v4 < v3)
        return 1;
    return (v3 < v4) ? -1 : 0;
}

INCLUDE_ASM(const s32, "core/main", func_00178CD8);

extern u8 D_00372580[];

u8 *func_00178E40(void) { return D_00372580; }

INCLUDE_ASM(const s32, "core/main", func_00178E50);

INCLUDE_ASM(const s32, "core/main", func_00178EC8);

INCLUDE_ASM(const s32, "core/main", func_00178F68);

INCLUDE_ASM(const s32, "core/main", func_001792C8);

INCLUDE_ASM(const s32, "core/main", func_00179410);

INCLUDE_ASM(const s32, "core/main", func_001795F0);

extern u8 D_00372650[];

u8 *func_00179BD0(void) { return D_00372650; }

INCLUDE_ASM(const s32, "core/main", func_00179BE0);

INCLUDE_ASM(const s32, "core/main", func_00179DE8);

INCLUDE_ASM(const s32, "core/main", func_0017A1A0);

INCLUDE_ASM(const s32, "core/main", func_0017A6B8);

INCLUDE_ASM(const s32, "core/main", func_0017ACB0);

INCLUDE_ASM(const s32, "core/main", func_0017B300);

INCLUDE_ASM(const s32, "core/main", func_0017B4D0);

extern u8 D_00372720[];

u8 *func_0017B6E8(void) { return D_00372720; }

INCLUDE_ASM(const s32, "core/main", func_0017B6F8);

extern u8 D_003727F0[];

u8 *func_0017B878(void) { return D_003727F0; }

INCLUDE_ASM(const s32, "core/main", func_0017B888);

extern u8 D_003728A0[];

u8 *func_0017BCC8(void) { return D_003728A0; }

INCLUDE_ASM(const s32, "core/main", func_0017BCD8);

extern u8 D_00372950[];

u8 *func_0017BFB8(void) { return D_00372950; }

INCLUDE_ASM(const s32, "core/main", func_0017BFC8);

extern u8 D_003729E0[];

u8 *func_0017C040(void) { return D_003729E0; }

INCLUDE_ASM(const s32, "core/main", func_0017C050);

INCLUDE_ASM(const s32, "core/main", func_0017C0C8);

INCLUDE_ASM(const s32, "core/main", func_0017C1B0);

INCLUDE_ASM(const s32, "core/main", func_0017C390);

INCLUDE_ASM(const s32, "core/main", func_0017C5B0);

INCLUDE_ASM(const s32, "core/main", func_0017C8F0);

extern u8 D_00372AB8[];

u8 *func_0017CEB8(void) { return D_00372AB8; }

INCLUDE_ASM(const s32, "core/main", func_0017CEC8);

INCLUDE_ASM(const s32, "core/main", func_0017D2C8);

INCLUDE_ASM(const s32, "core/main", func_0017D538);

INCLUDE_ASM(const s32, "core/main", func_0017D6C0);

void func_0017D870(void) { func_001825C8(); }

INCLUDE_ASM(const s32, "core/main", func_0017D878);

s32 func_00181700(void);

s32 func_0017D8D8(void)
{
    return func_00181700();
}

s32 func_00181590(void);

s32 func_0017D8F8(void)
{
    return func_00181590();
}

INCLUDE_ASM(const s32, "core/main", func_0017D918);

INCLUDE_ASM(const s32, "core/main", func_0017DB40);

INCLUDE_ASM(const s32, "core/main", func_0017DD68);

INCLUDE_ASM(const s32, "core/main", func_0017E400);

extern u8 D_00372DE8[];

u8 *func_0017E4B8(void) { return D_00372DE8; }

s32 func_0017E4C8(u8 *arg0, s32 arg1)
{
    s32 result = 0;
    s32 *q;

    if (arg0 != 0 && arg1 != 0 && *(s32 *)(arg0 + 0xC) != 0) {
        q = *(s32 **)(arg0 + 0x14);
        if (q != 0)
            *(s32 *)((u8 *)q + 4) = arg1;
        result = (s32)arg0;
    }
    return result;
}

s32 func_0017E500(s32 a1, s32 a2)
{
    s32 r = 0;

    if (a1 != 0 && *(s32 *)(a1 + 0xC) != 0) {
        s32 p = *(s32 *)(a1 + 0x14);
        if (p != 0)
            *(s32 *)p = a2;
        r = a1;
    }
    return r;
}

s32 func_0017E530(u8 *arg0)
{

    u8 *p = *(u8 **)(arg0 + 0x14);
    func_00224B88((s32)p, 0, 8);
    *(s32 *)(p + 4) = 0;
    *(s32 *)p = 0x1E1;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0017E570);

INCLUDE_ASM(const s32, "core/main", func_0017EB10);

INCLUDE_ASM(const s32, "core/main", func_0017EE40);

INCLUDE_ASM(const s32, "core/main", func_0017F388);

/* ------------------------------------------------------------------------- */
/* func_0017FF38 - build and submit the GIF/DMA packet of a resource batch.   */
/*                                                                            */
/* Behavioural reconstruction from asm/matchings/core/main/func_0017FF38.s.   */
/* The routine owns a packet header inside the buffer it allocates, emits one  */
/* GIFtag per row plus an A+D (register + data) pair per sub-resource, and     */
/* finally hands the finished packet to the renderer that owns the batch mode. */
/* The write-pointer arithmetic, the size walk and the packet encodings below  */
/* are a literal transcription of the retail instruction flow; the struct      */
/* offsets, the argument roles and the constant meanings are recorded in       */
/* docs/REFERENCE-BEHAVIORMATCH.md.                                            */
/* ------------------------------------------------------------------------- */

extern u8 D_003731BC[];
extern u8 *D_003938B4;
extern u32 D_00393880;

void func_00196788(u8 *node);
void func_00196898(u8 *node);
s32 func_001A3CD0(u8 *dst, u32 src, u32 a, u32 b);
u8 *func_001B1EC8(u8 *ctx, s32 size, s32 mode);
void func_00196370(u8 *obj, u8 *holder, s32 size, void (*cb)(u8 *));
s32 func_0017EE40(u8 *list, u8 *pkt, u8 *res, s32 odd, u32 lim, u8 *arg0);
s32 func_0017F388(u8 *list, u8 *pkt, u8 *res, s32 odd, u32 lim, s32 hdr, u8 *arg0);
s32 func_0017EB10(u8 *pkt, u8 *res, s32 odd, u32 lim, s32 hdr, u8 *arg0);
void func_001817E0(u8 *arg0);

/* 0x80-byte block move: retail copies it as eight ldl/ldr+sdl/sdr pairs, which
   is an unaligned 16-byte memmove.  A byte loop is the same transfer. */
#define PB_COPY80(dst, src)                                                      \
    do {                                                                         \
        u8 *_pb_d = (u8 *)(dst);                                                 \
        const u8 *_pb_s = (const u8 *)(src);                                     \
        s32 _pb_i;                                                               \
        for (_pb_i = 0; _pb_i < 0x80; _pb_i++)                                   \
            _pb_d[_pb_i] = _pb_s[_pb_i];                                         \
    } while (0)

s32 func_0017FF38(u8 *arg0, u8 *arg1, u8 *arg2)
{
    u8 hdr[0x80];
    u8 *hull;
    u8 *res;
    u8 *desc;
    u8 *pkt;
    u8 *packet;
    u8 *wp;
    u8 *tagpos;
    s32 mode;
    s32 flag30;
    s32 header;
    s32 size;
    s32 rows;
    s32 strideA;
    s32 strideB;
    s32 limit;
    s32 reusePkt;
    u32 res4;
    s32 i;

    /* arg1 is {resource owner, packet holder}; the holder's +0x00 is the
       packet chain node, the owner's +0x04 is the element/byte count base. */
    res = *(u8 **)arg1;
    hull = *(u8 **)(arg1 + 4);
    mode = *(s32 *)arg0;

    rows = 0;
    strideA = 0;
    strideB = 0;
    limit = 0;
    header = 0;
    size = 0;
    reusePkt = 0;

    /* ---- descriptor and header selection (0x17FF94) ---- */
    if (mode == 0) {
        desc = *(u8 **)(*(u8 **)(arg0 + 4) + 0x50);
        flag30 = (*(u8 **)D_003731BC)[2] & 4;
    } else if (mode == 1) {
        u8 *q = *(u8 **)(*(u8 **)(arg0 + 4) + 0x18);

        header = *(u16 *)(q + 8);
        flag30 = *(u8 *)(q + 2) & 4;
        desc = *(u8 **)(q + 0x3C);
        if ((*(u32 *)(*(u8 **)(arg0 + 4) + 0x4C) & 1) != 0)
            header |= 7;
    } else {
        header = 0x3F;
        flag30 = 0;
        desc = res - 0x10;
    }

    packet = *(u8 **)hull;
    res4 = *(u32 *)(res + 4);

    /* ---- cached packet check (0x18020) ---- */
    if (packet != 0) {
        pkt = packet + 0x18;
        *(u16 *)(arg0 + 0xC) = *(u16 *)(desc + 6);

        if (*(u16 *)(pkt + 0x0C) == *(u16 *)(desc + 6) &&
            *(u16 *)(pkt + 0x10) == (u16)flag30) {
            reusePkt = 1; /* descriptor and flags unchanged: nothing to rebuild */
        } else {
            if (*(u32 *)(pkt + 0x14) == res4 &&
                *(u32 *)(pkt + 0x18) == *(u32 *)desc) {
                /* same source block: the previous size and header stay valid */
                size = *(s32 *)(packet + 8);
                PB_COPY80(hdr, packet + 0x3C);
                strideA = *(s32 *)(pkt + 0x1C);
                strideB = *(s32 *)(pkt + 0x20);
            }
            header = 0x3F;
            func_00196788(packet);
            *(u32 *)hull = 0;
            packet = 0;
        }
    } else {
        header = 0x3F;
    }

    if (reusePkt) {
        /* ---- an up-to-date packet is already installed (0x180E8/0x180F8) ---- */
        if (header == 0) {
            for (i = 0; i < *(s32 *)arg2; i++) {
                u8 *e = *(u8 **)(arg2 + 8 + i * 0x10);

                func_001A3CD0(e, *(u32 *)(*(u32 *)(pkt + 8) + 4 + i * 0x20),
                              *(u32 *)(arg2 + 0x14 + i * 0x10), *(u32 *)(arg2 + 4));
                *(u16 *)e |= 6;
            }
            func_00196898(packet);
            return 1;
        }

        func_00196898(packet);
        if (mode == 0) {
            for (i = 0; i < *(s32 *)arg2; i++) {
                u8 *e = *(u8 **)(arg2 + 8 + i * 0x10);

                func_001A3CD0(e, *(u32 *)(*(u32 *)(pkt + 8) + 4 + i * 0x20),
                              *(u32 *)(arg2 + 0x14 + i * 0x10), *(u32 *)(arg2 + 4));
                *(u16 *)e |= 6;
            }
            return func_0017EE40(arg2, pkt, res, *(u32 *)desc & 1, res4, arg0);
        }
        for (i = 0; i < *(s32 *)arg2; i++) {
            u8 *e = *(u8 **)(arg2 + 8 + i * 0x10);

            func_001A3CD0(e, *(u32 *)(*(u32 *)(pkt + 8) + 4 + i * 0x20),
                          *(u32 *)(arg2 + 0x14 + i * 0x10), *(u32 *)(arg2 + 4));
            *(u16 *)e |= 6;
        }
        return func_0017F388(arg2, pkt, res, *(u32 *)desc & 1, res4, header, arg0);
    }

    /* ---- build path (0x180280) ---- */
    {
        s32 da = *(u8 *)(arg2 + 0xDA);

        if (size == 0) {
            if ((*(u32 *)desc & 1) != 0) {
                /* variant A: stride taken from +0xDC, header block at +0xE4 */
                s32 st = *(s32 *)(arg2 + 0xDC);

                strideA = st;
                rows = (s32)((u32)((s32)res4 + (st - 2) - 3) / (u32)(st - 2));
                strideB = *(s32 *)(arg2 + 0xE0);
                limit = (s32)res4 + (rows << 1) - 2;
                PB_COPY80(hdr, arg2 + 0xE4);
            } else {
                /* variant B: stride taken from +0x164, header block at +0x16C */
                s32 st = *(s32 *)(arg2 + 0x164);

                strideA = st;
                rows = (s32)((u32)((s32)res4 - 1 + st) / (u32)st);
                strideB = *(s32 *)(arg2 + 0x168);
                limit = (s32)res4;
                PB_COPY80(hdr, arg2 + 0x16C);
            }

            /* ---- size of the packet body, in quadwords ---- */
            /* 0x1803B8 dispatches on the batch's 128-byte-alignment flag, not
               on the stride: aligned batches take the layer walk, unaligned
               ones the eight-entry walk below. */
            if (*(u8 *)(arg2 + 0xD8) != 0) {
                /* 0x1803B8: four optional layers, each a strided image */
                /* 0x1803E8 sits in the delay slot of the first test, so the
                   subtraction happens whether or not that layer is present */
                s32 base = strideA * rows - limit;
                s32 layers = 1;
                s32 span = 0;
                s32 h;

                /* Retail stores each layer's span in the branch delay slot of
                   its own test (0x180404/0x180438/0x180464/0x1804CC), so the
                   store happens whether or not that layer is present. */
                h = (s32) * (s16 *)(hdr + 0x08);
                if (h != 0) {
                    /* retail drops `limit` from the base inside this arm only,
                       so a batch whose first layer is absent keeps the full
                       stride*rows base for the remaining layers */
                    layers = 2;
                    size += (limit * 3 + 3) >> 2;
                }
                *(u16 *)(hdr + 0x0A) = 0;
                if (h != 0)
                    span = (base * 3) >> 2;
                h = (s32)(s16) * (u16 *)(hdr + 0x18);
                if (h != 0) {
                    layers++;
                    size += (limit * 2 + 3) >> 2;
                }
                *(u16 *)(hdr + 0x1A) = (u16)span;
                if (h != 0)
                    span += base >> 1;
                h = (s32)(s16) * (u16 *)(hdr + 0x28);
                if (h != 0) {
                    layers++;
                    size += (limit + 3) >> 2;
                }
                *(u16 *)(hdr + 0x2A) = (u16)span;
                if (h != 0)
                    span += base >> 2;
                h = (s32)(s16) * (u16 *)(hdr + 0x38);
                if (h != 0) {
                    s32 t = ((strideA * 2 + strideA + 0xF) >> 4) * (rows - 1);

                    layers++;
                    t += (3 * (limit - strideA * (rows - 1)) + 0xF) >> 4;
                    size += t;
                }
                *(u16 *)(hdr + 0x3A) = (u16)span;

                {
                    s32 q = (s32)((u32)(rows + strideB - 1) / (u32)strideB);

                    size += layers * rows;
                    size += q + q * 8;
                }
            } else {
                /* 0x180500: unaligned batch, an eight-entry layer walk */
                s32 accEnd = (*(u8 *)(arg2 + 0xD9) + 1) * 2 * rows;
                s32 accStart = 0;
                s32 cnt = rows - 1;
                s32 d = limit - strideA * cnt;
                s32 r24 = (3 * strideA + 3) >> 2;
                s32 r16 = (strideA + 1) >> 1;
                s32 r8 = (strideA + 3) >> 2;
                s32 r4 = (3 * strideA + 15) >> 4;
                s32 d24 = (3 * d + 3) >> 2;
                s32 d16 = (d + 1) >> 1;
                s32 d8 = (d + 3) >> 2;
                s32 d4 = (3 * d + 15) >> 4;
                size = 0;
                for (i = 0; i < 8; i++) {
                    /* the layer tag and its size factor are 8 bytes apart */
                    u32 tw = *(u32 *)(arg2 + 0x88 + i * 8);
                    s32 rs;
                    s32 ls;
                    s32 hh;
                    s32 c;

                    if ((tw & 1) == 0)
                        continue;
                    if ((tw & 0x2000) == 0) {
                        /* plain layer: one A+D pair, no strided body */
                        *(u32 *)(hdr + i * 0x10) = res4;
                        *(u16 *)(hdr + i * 0x10 + 0xA) = 0;
                        size += (s32)(((u32)res4 *
                                       *(u32 *)(arg2 + 0x8C + i * 8) + 3u) >> 2);
                        continue;
                    }
                    if (i == 0) {
                        rs = r24;
                        ls = d24;
                        hh = (s32)(s16) * (u16 *)(hdr + 0x08);
                    } else if (i == 1) {
                        rs = r16;
                        ls = d16;
                        hh = (s32) * (s16 *)(hdr + 0x18);
                    } else if (i == 2) {
                        rs = r8;
                        ls = d8;
                        hh = (s32) * (s16 *)(hdr + 0x28);
                    } else if (i == 3) {
                        rs = r4;
                        ls = d4;
                        hh = (s32) * (s16 *)(hdr + 0x38);
                    } else {
                        continue;
                    }
                    c = *(u32 *)(hdr + i * 0x10 + 4) + 1;
                    *(u16 *)(hdr + i * 0x10 + 0xA) = (u16)accStart;
                    accStart += rs - ls;
                    accEnd = c + (rs + hh * cnt);
                }
                accEnd -= accStart;
                size += accEnd;

                /* second walk: chain the plain layers behind the strided ones */
                for (i = 0; i < 8; i++) {
                    u32 tw = *(u32 *)(arg2 + 0x88 + i * 8);

                    if ((tw & 1) == 0)
                        continue;
                    if ((tw & 0x2000) != 0)
                        continue;
                    *(u32 *)(hdr + i * 0x10 + 4) = (u32)accEnd;
                    accEnd += (s32)(((u32)res4 *
                                     *(u32 *)(arg2 + 0x8C + i * 8) + 3u) >> 2);
                }
            }

            /* the 0x12F is the fixed per-packet overhead retail adds */
            size = (size << 4) + 0x12F;
        }

        /* ---- allocate the packet, or chain it into the holder (0x180714) ---- */
        if (*(s32 *)arg0 == 2) {
            u8 *node = func_001B1EC8(D_003938B4, size + 0x18, 2);

            *(u32 *)hull = (u32)node;
            *(u32 *)(node + 4) = 0;
            *(u32 *)(node + 0) = 0;
            *(u32 *)(node + 8) = (u32)size;
            *(u32 *)(node + 0xC) = *(u32 *)(arg0 + 4);
            *(u32 *)(node + 0x14) = 0;
            *(u32 *)(node + 0x10) = 0;
            packet = node;
        } else {
            func_00196370(*(u8 **)(arg0 + 4), hull, size, func_001817E0);
            packet = *(u8 **)hull;
        }

        /* ---- fill the packet header (0x180778) ---- */
        pkt = packet + 0x18;
        *(u16 *)(arg0 + 0xC) = (u16)(*(u16 *)(desc + 6) - 1);
        *(u32 *)(pkt + 0x00) = 0;
        *(u16 *)(pkt + 0x10) = (u16)flag30;
        *(u32 *)(pkt + 0x0C) = *(u16 *)(desc + 6);
        *(u32 *)(pkt + 0x04) = 0;
        *(u32 *)(pkt + 0x14) = res4;
        PB_COPY80(packet + 0x3C, hdr);
        *(u32 *)(pkt + 0x1C) = (u32)strideA;
        *(u32 *)(pkt + 0x20) = (u32)strideB;
        *(u32 *)(pkt + 0x18) = *(u32 *)desc;

        /* the write pointer starts past the 0xB4-byte header, 16-byte aligned
           (128-byte aligned when the batch asks for it) */
        wp = (u8 *)(((u32)pkt + 0xB3) & 0xFFFFFFF0u);
        if (*(u8 *)(arg2 + 0xD8) != 0)
            wp = (u8 *)(((u32)wp + 0x7F) & 0xFFFFFF80u);
        *(u32 *)(pkt + 0x08) = (u32)wp;

        /* copy the per-element source offsets back into the list (0x180828) */
        {
            u8 *d2 = arg2 + 0x10;
            u8 *s2 = pkt + 0x28;

            for (i = 0; i < 8; i++, s2 += 0x10) {
                u32 tw = *(u32 *)(arg2 + 0x88 + i * 8);

                if ((tw & 1) == 0)
                    continue;
                if ((tw & 0x2000) != 0)
                    continue;
                *(u32 *)d2 = *(u32 *)s2;
                d2 += 0x10;
            }
        }
        *(u32 *)(arg2 + 4) = res4;

        /* ---- row loop (0x180878) ---- */
        tagpos = wp;
        {
            s32 reuse = 0;
            s32 span = 0;
            u32 hiBase = 0x01000100u | (u32)da;
            s32 row;

            if (rows > 0) {
                span = strideA * (rows - 1);
                for (row = 0; row < rows; row++) {
                    s32 rowStride = (row < rows - 1) ? strideA : (limit - span);
                    s32 mod = (s32)((u32)row % (u32)strideB);
                    s32 aligned = *(u8 *)(arg2 + 0xD8) != 0;

                    if (mod == 0) {
                        /* close the previous row block with its GIFtag, then
                           start a new block at the current write pointer */
                        u32 qw = (u32)((s32)(wp - tagpos) >> 4);
                        s32 first = (row == 0);

                        /* 0x180924: `movn $3,$0,$14` keeps the tag word only
                           on row 0 - every later boundary writes the NLOOP
                           word with a zeroed tag half. */
                        *(u32 *)(tagpos + 0x0) = (qw - 1) | 0x10000000u;
                        *(u32 *)(tagpos + 0x4) = 0;
                        *(u32 *)(tagpos + 0x8) = first ? 0x13000000u : 0;
                        *(u32 *)(tagpos + 0xC) = first ? 0x06008000u : 0;
                        /* 0x180940 `daddu $18,$11` sits in a delay slot, so
                           tagpos is the old write pointer. 0x180944
                           `beql $14,$0` skips the advance on row 0; the
                           harness's likely-branch model runs delay slots on
                           the taken path, which is what retail's own element
                           counter needs, so both paths advance here. */
                        tagpos = wp;
                        if (!aligned)
                            wp = tagpos + 0x10;
                        else if (first)
                            wp = tagpos + 0x10;
                        else
                            reuse = 1; /* the quad already at the pointer is kept */
                    }

                    /* ---- first element pass: layers without bit 13 ---- */
                    {
                        u8 *tg = (u8 *)(arg2 + 0x88);
                        u8 *ew = pkt + 0x28;
                        u8 *eb = pkt + 0x30;
                        s32 limit2 = strideA - 2;
                        s32 elem;

                        for (elem = 0; elem < 8; elem++) {
                            u32 tw = *(u32 *)(tg + elem * 8);
                            s32 bpp;
                            s32 bytes;
                            u32 addr;
                            u32 mul;
                            u32 w2;

                            if ((tw & 1) == 0)
                                continue;
                            if ((tw & 0x2000) != 0)
                                continue;

                            switch (tw & 0xFF000000u) {
                            case 0x68000000u:
                                bpp = 3;
                                break;
                            case 0x6D000000u:
                                bpp = 2;
                                break;
                            case 0x64000000u:
                                bpp = 2;
                                break;
                            case 0x65000000u:
                                bpp = 1;
                                break;
                            case 0x60000000u:
                                bpp = 1;
                                break;
                            case 0x6E000000u:
                                /* 0x180A18: retail's 0x6E arm lands on bpp = 1 */
                                bpp = 1;
                                break;
                            default:
                                bpp = 4;
                                break;
                            }

                            bytes = (rowStride * bpp + 3) >> 2;
                            addr = *(u32 *)(pkt + 8) +
                                   (*(u32 *)(ew + elem * 0x10) << 4);
                            mul = (u32)(row << 2);
                            mul = mul * (u32)(((*(u32 *)desc & 1) != 0) ? limit2
                                                                       : strideA);
                            mul = mul * (u32)bpp;
                            addr += mul;

                            /* row advance rounded up to the quadword boundary
                               is folded into the transfer count */
                            w2 = (u32)(rowStride +
                                       ((((rowStride * bpp + 3) & ~3) -
                                         rowStride * bpp - 1 + bpp) / bpp));
                            w2 = (w2 << 16) | (tw & 0xFF004000u) |
                                 (u32)(*(u8 *)(eb + elem * 0x10) | 0x8000);

                            /* pcpyld puts the (addr<<32)|bytes word low and
                               the (w2<<32)|hiBase word high */
                            *(u32 *)(tagpos + 0x0) = (u32)bytes | 0x30000000u;
                            *(u32 *)(tagpos + 0x4) = addr;
                            *(u32 *)(tagpos + 0x8) = hiBase;
                            *(u32 *)(tagpos + 0xC) = w2;

                            *(u32 *)(wp + 0x0) = 0x10000000u;
                            *(u32 *)(wp + 0x4) = 0;
                            *(u32 *)(wp + 0x8) = 0;
                            *(u32 *)(wp + 0xC) = 0;

                            tagpos = wp + 0x10;
                            wp = tagpos + 0x10;
                        }
                    }

                    /* ---- second element pass: layers tagged with bit 13 ---- */
                    {
                        u8 *tg = (u8 *)(arg2 + 0x88);
                        u8 *eb = pkt + 0x30;
                        u32 c0 = ((u32)rowStride << 16) | 0x68008000u;
                        u32 c1 = ((u32)rowStride << 16) | 0x64008000u;
                        u32 c2 = ((u32)rowStride << 16) | 0x6E00C000u;
                        u32 c3 = ((u32)rowStride << 16) | 0x6A008000u;
                        s32 q0 = ((3 * rowStride + 3) >> 2) << 4;
                        s32 q1 = ((2 * rowStride + 3) >> 2) << 4;
                        s32 q2 = ((rowStride + 3) >> 2) << 4;
                        s32 q3 = ((3 * rowStride + 15) >> 4) << 4;
                        s32 elem;

                        for (elem = 0; elem < 8; elem++) {
                            u32 tw = *(u32 *)(tg + elem * 8);
                            u32 chan;
                            s32 adv;
                            unsigned long long lo;
                            unsigned long long hi;

                            if ((tw & 1) == 0)
                                continue;
                            if ((tw & 0x2000) == 0)
                                continue;

                            switch (elem) {
                            case 0:
                                chan = c0;
                                adv = q0;
                                break;
                            case 1:
                                chan = c1;
                                adv = q1;
                                break;
                            case 2:
                                chan = c2;
                                adv = q2;
                                break;
                            case 3:
                                chan = c3;
                                adv = q3;
                                break;
                            default:
                                continue;
                            }

                            if (reuse) {
                                lo = ((unsigned long long)*(u32 *)(wp + 4) << 32) |
                                     *(u32 *)(wp + 0);
                                reuse = 0;
                            } else {
                                u32 cnt = D_00393880;

                                lo = ((unsigned long long)((cnt & 0xFFFFu) |
                                                           0x07000000u)) |
                                     0x0500000000000000ULL;
                                D_00393880 = cnt + 1;
                            }
                            hi = ((unsigned long long)(chan |
                                                       *(u8 *)(eb + elem * 0x10)))
                                 << 32;
                            hi |= (unsigned long long)hiBase;

                            *(u32 *)(wp + 0x0) = (u32)lo;
                            *(u32 *)(wp + 0x4) = (u32)(lo >> 32);
                            *(u32 *)(wp + 0x8) = (u32)hi;
                            *(u32 *)(wp + 0xC) = (u32)(hi >> 32);
                            wp += adv;
                            *(u32 *)(wp + 0x0) = 0;
                            *(u32 *)(wp + 0x4) = 0;
                            *(u32 *)(wp + 0x8) = 0;
                            *(u32 *)(wp + 0xC) = 0;
                            wp += 0x10;
                        }
                    }

                    /* ---- per-row A+D size word (0x180D4C) ---- */
                    {
                        u32 d0 = *(u32 *)desc;
                        unsigned long long w;
                        /* 0x180D68 `movz $8,$2,$14` sits in the delay slot of the
                           descriptor test, so the first-row A+D word is picked in
                           every branch, not just the bit-0 one */
                        unsigned long long pre = (row == 0) ? (0xA800ULL << 45)
                                                           : (0xB800ULL << 45);

                        if ((d0 & 1) != 0) {
                            w = (unsigned long long)((u32)rowStride | 0x04000000u);
                        } else if ((d0 & 2) != 0) {
                            /* retail rounds the halving with an arithmetic
                               shift: srl 31, addu, sra 1 */
                            s32 half = (s32)((u32)rowStride +
                                             ((u32)rowStride >> 31)) >>
                                       1;

                            w = (unsigned long long)((u32)half | 0x04000000u);
                        } else {
                            w = (unsigned long long)((u32)((s32)rowStride / 3) |
                                                     0x04000000u);
                        }
                        w |= pre;
                        *(u32 *)(wp + 0x0) = (u32)w;
                        *(u32 *)(wp + 0x4) = (u32)(w >> 32);
                        *(u32 *)(wp + 0x8) = 0;
                        *(u32 *)(wp + 0xC) = 0;
                        wp += 0x10;
                    }
                }
            }
        }

        /* ---- terminate the packet (0x180DD8) ---- */
        {
            u32 qw = (u32)((s32)(wp - tagpos) >> 4);
            /* retail builds the count with 32-bit sign-extending ops and stores
               it as a doubleword, so an empty block stores 0xFFFFFFFF twice */
            u64 word = (u64)(s64)(s32)(qw - 1) | 0x60000000ULL;

            *(u32 *)(wp - 0x8) = 0x11000000u;
            *(u32 *)(wp - 0x4) = 0x06000000u;
            *(u32 *)(tagpos + 0x0) = (u32)word;
            *(u32 *)(tagpos + 0x4) = (u32)(word >> 32);
            /* 0x180E08 `beql $2,$0` guards the `sq $0,8($18)`: in the
               harness's delay-slot model the zeroing runs for the batches
               whose alignment byte is clear */
            if (*(u8 *)(arg2 + 0xD8) == 0) {
                *(u32 *)(tagpos + 0x8) = 0;
                *(u32 *)(tagpos + 0xC) = 0;
            }
        }

        /* ---- clear the dirty bits of the eight sub-entries (0x180E10) ---- */
        if (*(s32 *)arg2 != 0) {
            for (i = 0; i < *(s32 *)arg2; i++) {
                u8 *e = *(u8 **)(arg2 + 8 + i * 0x10);

                func_001A3CD0(e, *(u32 *)(*(u32 *)(pkt + 8) + 4 + i * 0x20),
                              *(u32 *)(arg2 + 0x14 + i * 0x10), *(u32 *)(arg2 + 4));
                *(u16 *)e |= 6;
            }
        }

        /* ---- hand the finished packet to the renderer of the mode ---- */
        if (mode == 0)
            return func_0017EE40(arg2, pkt, res, *(u32 *)desc & 1, res4, arg0);
        if (mode == 1)
            return func_0017F388(arg2, pkt, res, *(u32 *)desc & 1, res4, header, arg0);
        return func_0017EB10(pkt, res, *(u32 *)desc & 1, res4, header, arg0);
    }
}


/* Source: rendering/region_0018.c */
/* func_00180F28 through func_0018FFB0; retail address range 0x00180000-0x00190000. */



extern s32 D_00393748;

INCLUDE_ASM(const s32, "core/main", func_00180F28);

INCLUDE_ASM(const s32, "core/main", func_001813E8);

INCLUDE_ASM(const s32, "core/main", func_00181590);

extern u8 D_00372E78[];

u8 *func_001816F0(void) { return D_00372E78; }

INCLUDE_ASM(const s32, "core/main", func_00181700);

void func_001817E0(u8 *arg0)
{
    volatile u32 *p = (volatile u32 *)(arg0 + 0x18);

    while (*p != 0) {
        func_001AB1C8();
    }
}

INCLUDE_ASM(const s32, "core/main", func_00181820);

INCLUDE_ASM(const s32, "core/main", func_00181CB0);

INCLUDE_ASM(const s32, "core/main", func_001820E0);

INCLUDE_ASM(const s32, "core/main", func_00182238);

extern u8 D_00372F20[];
extern u8 D_00372FA8[];

s32 func_00182578(s32 arg0)
{
    s32 result = 0;

    switch (arg0) {
    case 0:
        result = (s32)&D_00372F20;
        break;
    case 1:
        result = (s32)&D_00372FA8;
        break;
    }
    return result;
}

s32 *func_001825B8(s32 *p, s32 value) { ((s32 *)p[5])[0] = value; return p; }

s32 *func_001825C8(s32 *p, s32 value)
{
    s32 *q = (s32 *)p[5];
    q[1] = value;
    return q;
}

s32 func_001825D8(s32 *arg1)
{
    s32 *target = (s32 *)arg1[5];

    target[1] = (s32)func_001826C8;
    target[0] = 0;
    return 1;
}

s32 func_001825F8(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    s32 v11;
    s32 v12;
    s32 v14;
    s32 v20;
    u16 v21;

    v11 = a6;
    v12 = a5;
    v14 = func_001A3740(a1, 0, a2);
    v20 = func_001A3740(a1, 1, a2);
    func_001A3CD0((u8 *)v14, a3, 16, 1);
    *(u32 *)a4 = v12;
    *(u32 *)(a4 + 4) = v11;
    func_001A3CD0((u8 *)v20, a4, 8, 1);
    v21 = *(u16 *)v20;
    *(u32 *)(v20 + 16) = 1;
    *(u16 *)v20 = v21 | 6;
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_001826C8);

/* Byte-matching.  Sibling of func_00182838: same shape, but tests bit 1 and
   passes a constant 1.0f. */
u8 *func_001827F0(u8 *arg0, u8 *arg1)
{


    if ((*(u8 *)(arg0 + 2) & 2) != 0)
        func_00181820(arg0, 0, *(u32 *)arg1, 1.0f);
    return arg0;
}

u8 *func_00182838(u8 *arg0, u8 *arg1)
{


    if ((*(u8 *)(arg0 + 2) & 1) != 0)
        func_00181820(arg0, arg1 + 0x10, *(volatile u32 *)arg1, *(volatile f32 *)(arg1 + 0x50));
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_00182880);

s32 func_001828F0(s32 a, s32 *b)
{
    return func_00182238(a, *b, 0);
}

s32 func_00182910(s32 a, s32 *b)
{
    return func_00182238(a, *b, 1);
}

INCLUDE_ASM(const s32, "core/main", func_00182930);

INCLUDE_ASM(const s32, "core/main", func_00182A10);

INCLUDE_ASM(const s32, "core/main", func_00182B28);

INCLUDE_ASM(const s32, "core/main", func_00182C88);

INCLUDE_ASM(const s32, "core/main", func_00182DA0);

INCLUDE_ASM(const s32, "core/main", func_00182F00);

INCLUDE_ASM(const s32, "core/main", func_00183040);

INCLUDE_ASM(const s32, "core/main", func_001831E0);

s32 func_00183380(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x50);
}

s32 func_00183398(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x60);
}

s32 func_001833B0(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x5c);
}

s32 func_001833C8(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x54);
}

s32 func_001833E0(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x68);
}

s32 func_001833F8(void)
{
    s32 index = D_003937B0;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x64);
}

INCLUDE_ASM(const s32, "core/main", func_00183410);

INCLUDE_ASM(const s32, "core/main", func_00183468);

INCLUDE_ASM(const s32, "core/main", func_00183510);

s8 *func_001835B8(u8 *p, s32 offset) { *(s32 *)(p + offset) = 0; return (s8 *)p; }

/* Byte-matching.  Releases the entry at arg0+arg1 when it is set, and returns
   arg0. */
u8 *func_001835C8(u8 *arg0, s32 arg1)
{


    if (*(u32 *)(arg0 + arg1) != 0)
        func_00188E50(*(s32 *)(arg0 + arg1));
    return arg0;
}

s8 *func_00183608(u8 *p, s32 unused, s32 offset) { *(s32 *)(p + offset) = 0; return (s8 *)p; }

s32 func_001706A0(s32, s32, s32, s32);

s32 func_00183618(s32 a0, s32 a1, s32 a2)
{
    return func_001706A0(*(s32 *)(a2 + 0x3C), a2, a0, a2 + 0xC);
}

s32 func_00183648(s32 a1, s32 a2, s32 a3)
{
    s32 v;

    v = func_00170818(a1, a3, a3 + 0xC);
    *(s32 *)(a3 + 0x3C) = v;
    return v != 0 ? a1 : 0;
}

s32 func_00170E88(s32 *);

s32 func_00183690(s32 *arg1)
{
    return func_00170E88((s32 *)arg1[15]);
}

s32 func_001836B0(s32 a0, s32 a1, s32 a2)
{
    s32 v = func_00175830(a2);
    return func_001706A0(*(s32 *)(a2 + 0x50), v, a0, v + 0xC);
}

INCLUDE_ASM(const s32, "core/main", func_001836F8);

s32 func_00170E88(s32 *);

s32 func_00183760(s32 *arg1)
{
    return func_00170E88((s32 *)arg1[20]);
}

INCLUDE_ASM(const s32, "core/main", func_00183780);

INCLUDE_ASM(const s32, "core/main", func_00183898);

INCLUDE_ASM(const s32, "core/main", func_001839D8);

s32 func_00183A80(void)
{
    s32 index = D_003937A8;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x34);
}

s32 func_00183A98(void)
{
    s32 index = D_003937A8;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x30);
}

INCLUDE_ASM(const s32, "core/main", func_00183AB0);

INCLUDE_ASM(const s32, "core/main", func_00183B38);

INCLUDE_ASM(const s32, "core/main", func_00183E30);

INCLUDE_ASM(const s32, "core/main", func_001842B0);

INCLUDE_ASM(const s32, "core/main", func_00184428);

INCLUDE_ASM(const s32, "core/main", func_001845B8);

INCLUDE_ASM(const s32, "core/main", func_00184880);

INCLUDE_ASM(const s32, "core/main", func_00184980);

INCLUDE_ASM(const s32, "core/main", func_00184C30);

u8 *func_00185140(u8 *p)
{
    D_00393CCC++;
    return p;
}

u8 *func_00185158(u8 *p)
{
    D_00393CCC--;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00185170);

s32 func_00185238(s32 a1, s32 a2, u32 *a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    s32 v16;
    s32 size;
    u32 v25[4];

    if (func_00189268(a1, 1, (u32)v25, 0) == 0)
        return 0;
    size = 0x18;
    func_00224B88(a2, 0, size);
    v16 = func_001972B8(a1, a2, size);
    if (v16 != size)
        return 0;
    *a3 = v25[0] + 12;
    func_00197630(a1, v25[0] - size);
    func_00189760(a2, size);
    func_001897D0(a2 + 4, 4);
    func_001897D0(a2 + 16, 4);
    func_001897D0(a2 + 20, 4);
    return a2;
}

INCLUDE_ASM(const s32, "core/main", func_00185308);

INCLUDE_ASM(const s32, "core/main", func_001853D0);

INCLUDE_ASM(const s32, "core/main", func_00185C00);

INCLUDE_ASM(const s32, "core/main", func_00185DE0);

INCLUDE_ASM(const s32, "core/main", func_00186420);

INCLUDE_ASM(const s32, "core/main", func_00186AC8);

INCLUDE_ASM(const s32, "core/main", func_00186DA8);

INCLUDE_ASM(const s32, "core/main", func_00186FE0);

INCLUDE_ASM(const s32, "core/main", func_00187180);

INCLUDE_ASM(const s32, "core/main", func_00187358);

INCLUDE_ASM(const s32, "core/main", func_00187520);

INCLUDE_ASM(const s32, "core/main", func_00187A20);

INCLUDE_ASM(const s32, "core/main", func_00187E60);

INCLUDE_ASM(const s32, "core/main", func_001882D0);

INCLUDE_ASM(const s32, "core/main", func_00188628);

u8 *func_00188780(u8 *p)
{
    D_00393CD4--;
    return p;
}

u8 *func_00188798(u8 *p)
{
    D_00393CD4++;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_001887B0);

u8 *func_001889B8(u8 *p)
{
    D_00393CDC++;
    return p;
}

u8 *func_001889D0(u8 *p)
{
    D_00393CDC--;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_001889E8);

INCLUDE_ASM(const s32, "core/main", func_00188A60);

s32 func_00188B28(s32 dummy, f32 a1, s32 a2, s32 a3, u8 *p)
{
    if (*(s32 *)p == 0) {
        *(s32 *)(p + 8) = a2;
        goto store;
    }
    if (*(s32 *)(p + 8) != a2)
        return 0;
    if (a1 < *(f32 *)(p + 4)) {
        *(s32 *)(p + 8) = a2;
        goto store;
    }
    goto done;
store:
    *(f32 *)(p + 4) = a1;
    *(s32 *)p = a3;
done:
    return a3;
}

INCLUDE_ASM(const s32, "core/main", func_00188B68);

u8 *func_00188CA0(u8 *p)
{
    D_00393CE4++;
    return p;
}

u8 *func_00188CB8(u8 *p)
{
    D_00393CE4--;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00188CD0);

u16 func_00188D18(const u16 *p) { return p[46]; }

u16 func_00188D20(const u16 *p) { return p[45]; }

extern u8 D_003731BC[];

s32 func_00188D28(void)
{
    return ((s32 (*)(void))*(u32 *)(*(u32 *)D_003731BC + 0x6C))();
}

s32 func_00188D50(u8 *a1, s32 a2, s32 a3)
{

    s32 v = *(s32 *)(a1 + 0x50);

    if (v != 0) {
        if (func_00170DF8(v, a2, a3) != 0) return (s32)a1;
    }
    return 0;
}

u8 *func_00188D98(u8 *p) { return p + 52; }

s32 func_0019A490();

s32 func_00188DA0(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_00373048, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A198();

s32 func_00188DE8(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_00373048, arg1, arg2, arg3, arg4);
}

s32 func_00188E20(s32 arg)
{
    return func_0019A638(D_00373048, arg);
}

s32 func_00188E48(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00188E50);

INCLUDE_ASM(const s32, "core/main", func_00188ED8);

INCLUDE_ASM(const s32, "core/main", func_00188FD0);

u8 *func_001890C0(u8 *arg0, u8 *arg1)
{
    *(S12 *)(arg0 + 0xC) = *(S12 *)arg1;
    *(S12 *)arg0 = *(S12 *)arg1;
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_001890F8);

/* Byte-matching: 6 float bounds checks, explicit > early-outs. */
s32 func_00189198(f32 *a1, f32 *a2)
{
    if (a1[3] > a2[0]) return 0;
    if (a1[4] > a2[1]) return 0;
    if (a1[5] > a2[2]) return 0;
    if (a2[0] > a1[0]) return 0;
    if (a2[1] > a1[1]) return 0;
    if (a2[2] > a1[2]) return 0;
    return 1;
}


INCLUDE_ASM(const s32, "core/main", func_00189220);

INCLUDE_ASM(const s32, "core/main", func_00189268);

void func_00189728();
s32 func_00197458();

s32 func_001893D0(s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 stack[3];
    stack[0] = a2;
    stack[1] = a3;
    stack[2] = a4;
    func_00189728(stack, 0xC);
    return func_00197458(a1, stack, 0xC);
}

INCLUDE_ASM(const s32, "core/main", func_00189418);

INCLUDE_ASM(const s32, "core/main", func_00189510);

INCLUDE_ASM(const s32, "core/main", func_001895D8);

INCLUDE_ASM(const s32, "core/main", func_00189698);

INCLUDE_ASM(const s32, "core/main", func_00189728);

INCLUDE_ASM(const s32, "core/main", func_00189760);

INCLUDE_ASM(const s32, "core/main", func_00189798);

INCLUDE_ASM(const s32, "core/main", func_001897D0);

INCLUDE_ASM(const s32, "core/main", func_00189808);

INCLUDE_ASM(const s32, "core/main", func_001898D0);

INCLUDE_ASM(const s32, "core/main", func_00189980);

INCLUDE_ASM(const s32, "core/main", func_00189C30);

s32 func_0019A198();

s32 func_00189F50(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_003771B8, arg1, arg2, arg3, arg4);
}

INCLUDE_ASM(const s32, "core/main", func_00189F88);

INCLUDE_ASM(const s32, "core/main", func_0018A060);

extern s32 D_003732E8[];
s32 func_0018A0A0(s32 *arg0)
{
    u8 *p = (u8 *)arg0;
    if (*(s32 *)(p + 4) != 0)
        ((void (*)(u8 *)) D_003732E8[0])(*(u8 **)p);
    return (s32)arg0;
}

INCLUDE_ASM(const s32, "core/main", func_0018A0E0);

s32 func_0018A168(s32 *a1, s32 a2)
{
    s32 *v4;
    s32 v5;

    if (a2 != 0)
    {
        v4 = (s32 *)*a1;
        *v4 = a2;
        v5 = *(s32 *)(a2 + 0xA4);
        *a1 = (s32)(v4 + 1);
        func_0018A168(a1, v5);
        func_0018A168(a1, *(s32 *)(a2 + 0xA0));
    }
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0018A1C0);

INCLUDE_ASM(const s32, "core/main", func_0018A3C0);

INCLUDE_ASM(const s32, "core/main", func_0018A610);

INCLUDE_ASM(const s32, "core/main", func_0018A850);

INCLUDE_ASM(const s32, "core/main", func_0018A998);

extern s32 D_003732E8[];

s32 func_0018AC20(void)
{
    ((s32 (*)(void)) D_003732E8[0])();
    return 1;
}

s32 func_0018AC48(s32 arg0)
{



    s32 (*fp)(s32) = (s32 (*)(s32))D_003732D0[0];
    s32 d = (s32)&D_00393740;
    s32 v = arg0 ? arg0 : d;

    return (fp(v) + 4) & 0xFFFFFFFC;
}

INCLUDE_ASM(const s32, "core/main", func_0018AC80);

s32 func_0019A198();

s32 func_0018AD18(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_00377200, arg1, arg2, arg3, arg4);
}

INCLUDE_ASM(const s32, "core/main", func_0018AD50);

INCLUDE_ASM(const s32, "core/main", func_0018ADF8);

s32 func_0019A198();

s32 func_0018AEC8(s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    return func_0019A198(D_00377218, arg1, arg2, arg3, arg4);
}

s32 func_0018AF00(s32 arg0)
{

    s32 v[2];
    s32 v3;
    s32 t;

    v[0] = 16;
    v[1] = 1;
    func_00199DB0(arg0, func_0018AF78, v);
    if (v[1] == 0)
        return 0;
    v3 = func_0019A3A8(D_00377218, arg0);
    t = v[0] + 12;
    v[0] = t + v3;
    return v[0];
}

/* Byte-matching: early-out r==0 bnel shape. */
s32 func_0018AF78(s32 a1, s32 *a2)
{

    s32 v5[4];
    s32 r;

    r = ((s32 (*)(s32, s32, s32)) D_00373264[0])(v5, a1, 0);
    if (r == 0)
    {
        a2[1] = 0;
        return 0;
    }
    *a2 += 12 + v5[0];
    *a2 += 12 + func_0019A3A8(D_00377200, a1);
    return a1;
}


u8 *func_0018B008(u8 *p, s32 *counter)
{
    (*counter)++;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0018B020);

s32 func_0018B108();
s32 func_00199AD0();

s32 func_0018B108(s32 arg1)
{
    func_00199AD0(arg1);
    return arg1;
}

/* Behavioral reconstruction of func_0018B130.
 *
 * Large (2304-byte / 576-instruction), self-contained leaf function that
 * builds contact-point records for an oriented-box collision.  Only arg0 is
 * used; a1-a3 are overwritten before being read in the reference.
 *
 * All state lives in the pointed-to structure:
 *   +0x04            pointer p to a sub-structure
 *   p+0x50           "q" block: face centre, two half-edges, separating
 *                    vector and a third direction
 *   +0x68/6C/78/7C/80/84  scalar weights (0x78 is negated)
 *   +0x98            "out" contact records (0x20 bytes each)
 *   +0x128           eight Vec3 vertices (0x0C bytes each)
 *
 * Operation order is preserved where it matters for IEEE rounding:
 * multiply-before-add, accumulation order, and the sqrt guard.  Sign bytes
 * are taken from the raw float bit pattern (sra 31 + 1), matching the
 * reference's `lw`/`sra`/`addiu`/`sb` sequence exactly, including -0.0.
 *
 * Behaviour was validated with 800 randomized differential runs (Unicorn)
 * comparing every writable byte against the retail function: identical in
 * all cases.  Not byte-matching (1876/2304 bytes differ) - instruction
 * scheduling and register allocation differ from the original source, and
 * this compiler only emits the retail's inline `sqrt.s` under -ffast-math
 * (see funcs_07 in modules.txt), which is why that flag is set here.
 */

#define F(o)  (*(f32 *)((u8 *)base + (o)))

/* The retail reads the stored floats back as integers to grab their sign
 * bit.  Do the same from a local value via union punning: a direct *(s32*)
 * reload from an f32 store is not reliable under GCC's type-based aliasing. */
#define SB(dst, doff, val) do {                          \
    union { f32 f; s32 i; } _sb;                          \
    _sb.f = (val);                                        \
    *(u8 *)((u8 *)(dst) + (doff)) = (u8)((_sb.i >> 31) + 1); \
} while (0)

void func_0018B130(u8 *arg0)
{
    u8 *base = arg0;
    u8 *p    = (u8 *)(*(u32 *)(base + 0x04));
    u8 *q    = p + 0x50;
    u8 *out  = base + 0x98;
    u8 *v    = base + 0x128;

    f32 negM = -F(0x78);
    f32 w68  = F(0x68);
    f32 w6C  = F(0x6C);
    f32 w7C  = F(0x7C);
    f32 w80  = F(0x80);
    f32 w84  = F(0x84);

    f32 v2x, v2y, v2z;   /* stack 0x40/0x44/0x48 */
    f32 v0x, v0y, v0z;   /* stack 0x00/0x04/0x08 */
    f32 ax, ay, az;      /* stack 0x20/0x24/0x28 */
    f32 bx, by, bz;      /* stack 0x30/0x34/0x38 */

    f32 t0x, t0y, t0z;
    f32 t1x, t1y, t1z;
    f32 t2x, t2y, t2z;
    f32 len, scale;
    f32 nx, ny, nz;      /* normalized contact normal */
    f32 c0, c1, c2;      /* stored centre / negation */
    s32 i;

    /* ---- face-direction vector (stack 0x40) -------------------------- */
    v2x = (*(f32 *)(q + 0x00)) * negM;
    v2y = (*(f32 *)(q + 0x04)) * negM;
    v2z = (*(f32 *)(q + 0x08)) * negM;
    v2x = v2x + (*(f32 *)(q + 0x10)) * w7C;
    v2y = v2y + (*(f32 *)(q + 0x14)) * w7C;
    v2z = v2z + (*(f32 *)(q + 0x18)) * w7C;

    /* ---- centre + two half-edges, and their doubles ------------------ */
    t0x = (*(f32 *)(q + 0x00)) * w68;
    t0y = (*(f32 *)(q + 0x04)) * w68;
    t0z = (*(f32 *)(q + 0x08)) * w68;
    v0x = (*(f32 *)(q + 0x20)) + t0x;
    v0y = (*(f32 *)(q + 0x24)) + t0y;
    v0z = (*(f32 *)(q + 0x28)) + t0z;
    t1x = (*(f32 *)(q + 0x10)) * w6C;
    t1y = (*(f32 *)(q + 0x14)) * w6C;
    t1z = (*(f32 *)(q + 0x18)) * w6C;
    v0x = v0x + t1x;
    v0y = v0y + t1y;
    v0z = v0z + t1z;

    ax = t0x + t0x;  ay = t0y + t0y;  az = t0z + t0z;
    bx = t1x + t1x;  by = t1y + t1y;  bz = t1z + t1z;

    /* ---- four corners of the first face ------------------------------ */
    (*(f32 *)(v + 0x00)) = v0x;
    (*(f32 *)(v + 0x04)) = v0y;
    (*(f32 *)(v + 0x08)) = v0z;

    t2x = v0x - ax;  t2y = v0y - ay;  t2z = v0z - az;
    (*(f32 *)(v + 0x0C)) = t2x;
    (*(f32 *)(v + 0x10)) = t2y;
    (*(f32 *)(v + 0x14)) = t2z;

    t2x = t2x - bx;  t2y = t2y - by;  t2z = t2z - bz;
    (*(f32 *)(v + 0x18)) = t2x;
    (*(f32 *)(v + 0x1C)) = t2y;
    (*(f32 *)(v + 0x20)) = t2z;

    t2x = t2x + ax;  t2y = t2y + ay;  t2z = t2z + az;
    (*(f32 *)(v + 0x24)) = t2x;
    (*(f32 *)(v + 0x28)) = t2y;
    (*(f32 *)(v + 0x2C)) = t2z;

    /* ---- second face: four iterations, two vertices each ------------- */
    for (i = 0; i < 4; i++)
    {
        u8 *a = v + i * 0x0C;
        u8 *b = v + (i + 4) * 0x0C;
        f32 rx = (*(f32 *)(a + 0)) - v2x;
        f32 ry = (*(f32 *)(a + 4)) - v2y;
        f32 rz = (*(f32 *)(a + 8)) - v2z;
        f32 wx = v2x + (*(f32 *)(q + 0x30));
        f32 wy = v2y + (*(f32 *)(q + 0x34));
        f32 wz = v2z + (*(f32 *)(q + 0x38));

        (*(f32 *)(a + 0)) = wx + rx * w80;
        (*(f32 *)(a + 4)) = wy + ry * w80;
        (*(f32 *)(a + 8)) = wz + rz * w80;
        (*(f32 *)(b + 0)) = wx + rx * w84;
        (*(f32 *)(b + 4)) = wy + ry * w84;
        (*(f32 *)(b + 8)) = wz + rz * w84;
    }

    /* ---- contact record 0 -------------------------------------------- */
    c0 = (*(f32 *)(q + 0x20));
    c1 = (*(f32 *)(q + 0x24));
    c2 = (*(f32 *)(q + 0x28));
    (*(f32 *)(out + 0x00)) = c0;
    (*(f32 *)(out + 0x04)) = c1;
    (*(f32 *)(out + 0x08)) = c2;
    (*(f32 *)(out + 0x0C)) = (*(f32 *)(v + 0x30)) * c0
                          + (*(f32 *)(v + 0x34)) * c1
                          + (*(f32 *)(v + 0x38)) * c2;
    SB(out, 0x10, c0);
    SB(out, 0x11, c1);
    SB(out, 0x12, c2);

    /* ---- contact record 1 -------------------------------------------- */
    c0 = -c0;  c1 = -c1;  c2 = -c2;
    (*(f32 *)(out + 0x14)) = c0;
    (*(f32 *)(out + 0x18)) = c1;
    (*(f32 *)(out + 0x1C)) = c2;
    (*(f32 *)(out + 0x20)) = (*(f32 *)(v + 0x00)) * c0
                          + (*(f32 *)(v + 0x04)) * c1
                          + (*(f32 *)(v + 0x08)) * c2;
    SB(out, 0x24, c0);
    SB(out, 0x25, c1);
    SB(out, 0x26, c2);

    /* ---- contact record 2: normal from vertices 1/5/6 ---------------- */
    t0x = (*(f32 *)(v + 0x0C)) - (*(f32 *)(v + 0x3C));
    t0y = (*(f32 *)(v + 0x10)) - (*(f32 *)(v + 0x40));
    t0z = (*(f32 *)(v + 0x14)) - (*(f32 *)(v + 0x44));
    t2x = (*(f32 *)(v + 0x1C)) - (*(f32 *)(v + 0x3C));
    t2y = (*(f32 *)(v + 0x20)) - (*(f32 *)(v + 0x40));
    t2z = (*(f32 *)(v + 0x24)) - (*(f32 *)(v + 0x44));
    nx = t0y * t2z - t0z * t2y;
    ny = t0z * t2x - t0x * t2z;
    nz = t0x * t2y - t0y * t2x;
    (*(f32 *)(out + 0x28)) = nx;
    (*(f32 *)(out + 0x2C)) = ny;
    (*(f32 *)(out + 0x30)) = nz;
    len = (nx * nx + ny * ny) + nz * nz;
    len = __builtin_sqrtf(len);
    scale = (len > 0.0f) ? (1.0f / len) : 0.0f;
    nx = nx * scale;  ny = ny * scale;  nz = nz * scale;
    (*(f32 *)(out + 0x28)) = nx;
    (*(f32 *)(out + 0x2C)) = ny;
    (*(f32 *)(out + 0x30)) = nz;
    (*(f32 *)(out + 0x34)) = (*(f32 *)(v + 0x0C)) * nx
                          + (*(f32 *)(v + 0x10)) * ny
                          + (*(f32 *)(v + 0x14)) * nz;
    SB(out, 0x38, nx);
    SB(out, 0x39, ny);
    SB(out, 0x3A, nz);

    /* ---- contact record 3: normal from vertices 1/4/5 ---------------- */
    t2x = (*(f32 *)(v + 0x30)) - (*(f32 *)(v + 0x3C));
    t2y = (*(f32 *)(v + 0x34)) - (*(f32 *)(v + 0x40));
    t2z = (*(f32 *)(v + 0x38)) - (*(f32 *)(v + 0x44));
    nx = t2y * t0z - t2z * t0y;
    ny = t2z * t0x - t2x * t0z;
    nz = t2x * t0y - t2y * t0x;
    (*(f32 *)(out + 0x3C)) = nx;
    (*(f32 *)(out + 0x40)) = ny;
    (*(f32 *)(out + 0x44)) = nz;
    len = (nx * nx + ny * ny) + nz * nz;
    len = __builtin_sqrtf(len);
    scale = (len > 0.0f) ? (1.0f / len) : 0.0f;
    nx = nx * scale;  ny = ny * scale;  nz = nz * scale;
    (*(f32 *)(out + 0x3C)) = nx;
    (*(f32 *)(out + 0x40)) = ny;
    (*(f32 *)(out + 0x44)) = nz;
    (*(f32 *)(out + 0x48)) = (*(f32 *)(v + 0x0C)) * nx
                          + (*(f32 *)(v + 0x10)) * ny
                          + (*(f32 *)(v + 0x14)) * nz;
    SB(out, 0x4C, nx);
    SB(out, 0x4D, ny);
    SB(out, 0x4E, nz);

    /* ---- contact record 4: normal from vertices 1/2/7 ---------------- */
    t0x = (*(f32 *)(v + 0x0C)) - (*(f32 *)(v + 0x54));
    t0y = (*(f32 *)(v + 0x10)) - (*(f32 *)(v + 0x58));
    t0z = (*(f32 *)(v + 0x14)) - (*(f32 *)(v + 0x5C));
    t2x = (*(f32 *)(v + 0x1C)) - (*(f32 *)(v + 0x54));
    t2y = (*(f32 *)(v + 0x20)) - (*(f32 *)(v + 0x58));
    t2z = (*(f32 *)(v + 0x24)) - (*(f32 *)(v + 0x5C));
    nx = t0y * t2z - t0z * t2y;
    ny = t0z * t2x - t0x * t2z;
    nz = t0x * t2y - t0y * t2x;
    (*(f32 *)(out + 0x50)) = nx;
    (*(f32 *)(out + 0x54)) = ny;
    (*(f32 *)(out + 0x58)) = nz;
    len = (nx * nx + ny * ny) + nz * nz;
    len = __builtin_sqrtf(len);
    scale = (len > 0.0f) ? (1.0f / len) : 0.0f;
    nx = nx * scale;  ny = ny * scale;  nz = nz * scale;
    (*(f32 *)(out + 0x50)) = nx;
    (*(f32 *)(out + 0x54)) = ny;
    (*(f32 *)(out + 0x58)) = nz;
    (*(f32 *)(out + 0x5C)) = (*(f32 *)(v + 0x24)) * nx
                          + (*(f32 *)(v + 0x28)) * ny
                          + (*(f32 *)(v + 0x2C)) * nz;
    SB(out, 0x60, nx);
    SB(out, 0x61, ny);
    SB(out, 0x62, nz);

    /* ---- contact record 5: normal from vertices 1/6/7 ---------------- */
    t2x = (*(f32 *)(v + 0x30)) - (*(f32 *)(v + 0x54));
    t2y = (*(f32 *)(v + 0x34)) - (*(f32 *)(v + 0x58));
    t2z = (*(f32 *)(v + 0x38)) - (*(f32 *)(v + 0x5C));
    nx = t2y * t0z - t2z * t0y;
    ny = t2z * t0x - t2x * t0z;
    nz = t2x * t0y - t2y * t0x;
    (*(f32 *)(out + 0x64)) = nx;
    (*(f32 *)(out + 0x68)) = ny;
    (*(f32 *)(out + 0x6C)) = nz;
    len = (nx * nx + ny * ny) + nz * nz;
    len = __builtin_sqrtf(len);
    scale = (len > 0.0f) ? (1.0f / len) : 0.0f;
    nx = nx * scale;  ny = ny * scale;  nz = nz * scale;
    (*(f32 *)(out + 0x64)) = nx;
    (*(f32 *)(out + 0x68)) = ny;
    (*(f32 *)(out + 0x6C)) = nz;
    (*(f32 *)(out + 0x70)) = (*(f32 *)(v + 0x24)) * nx
                          + (*(f32 *)(v + 0x28)) * ny
                          + (*(f32 *)(v + 0x2C)) * nz;
    SB(out, 0x74, nx);
    SB(out, 0x75, ny);
    SB(out, 0x76, nz);
}

#undef F
#undef SB

INCLUDE_ASM(const s32, "core/main", func_0018BA30);

extern void func_0019AD80(void);

INCLUDE_ASM(const s32, "core/main", func_0018BC08);

INCLUDE_ASM(const s32, "core/main", func_0018BFB0);

INCLUDE_ASM(const s32, "core/main", func_0018C180);

INCLUDE_ASM(const s32, "core/main", func_0018C2D0);

s32 func_0018C450(u8 *p)
{
    return ((s32 (*)(void)) *(u32 *)(p + 0x18))();
}

s32 func_0018C470(u8 *p)
{
    return ((s32 (*)(void)) *(u32 *)(p + 0x1C))();
}

s32 func_0018C490(void) { return D_003731B8[0]; }

extern s32 D_00373254[];
u8 *func_0018C4A0(u8 *arg0)
{
    return ((s32 (*)(void)) D_00373254[0])() ? arg0 : 0;
}

/* Byte-matching.  Releases the resource at +0x60 and reports the object, or
   NULL when the release reported an error. */
u8 *func_0018C4D8(u8 *arg0)
{

    u8 *r = arg0;

    if (func_00196128(*(s32 *)(arg0 + 0x60)) == 0)
        r = 0;
    return r;
}

s32 func_0018C508(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 1;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_00393744;
    __asm__ volatile("" ::: "memory");
    D_00393744++;
    return ret;
}

extern s32 D_00373060[];
extern s32 D_00393CE8;
s32 func_0018C550(s32 a1)
{
    s32 *p;

    func_0019A700((s32)D_00373060, a1);
    func_0019A890(a1);
    p = (s32 *)((s32)D_00393CE8 + (u32)D_003731B8);
    ((s32 (*)(s32, s32, s32))D_003731B8[80])(*p, a1, (s32)p);
    return 1;
}

s32 func_0018C5B0();
s32 func_0019A830();

s32 func_0018C5B0(s32 arg1)
{
    func_0019A830(arg1);
    return arg1;
}

s32 func_0018C5D8(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

s32 *func_0018C5E0(s32 *p, s32 value) { p[0x18] = value; return p; }

s32 func_0018C5F0(const s32 *p) { return *(const s32 *)((const u8 *)p + (96)); }

s32 *func_0018C5F8(s32 *p, s32 value) { p[0x19] = value; return p; }

s32 func_0018C608(const s32 *p) { return *(const s32 *)((const u8 *)p + (100)); }

void func_0018E670();
u8 *func_0018C610(u8 *arg0, u8 *arg1)
{
    u8 *result = arg0;
    s32 v;
    *(UnalignedU64 *)(arg0 + 0x78) = *(UnalignedU64 *)arg1;
    v = *(s32 *)(arg0 + 4);
    if (v != 0) {
        func_0018E670(v);
        return arg0;
    }
    return result;
}

u8 *func_0018C658(u8 *p) { return p + 120; }

INCLUDE_ASM(const s32, "core/main", func_0018C660);

u8 *func_0018C6D8(u8 *p) { return p + 104; }

INCLUDE_ASM(const s32, "core/main", func_0018C6E0);

s32 func_0018C810(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

u8 *func_0018C818(u8 *p) { return p + 32; }

INCLUDE_ASM(const s32, "core/main", func_0018C820);

f32 func_0018C8F8(const f32 *p) { return p[32]; }

INCLUDE_ASM(const s32, "core/main", func_0018C900);

f32 func_0018C9D8(const f32 *p) { return p[33]; }

f32 *func_0018C9E0(f32 *p, f32 value) { p[0x22] = value; return p; }

f32 func_0018C9F0(const f32 *p) { return p[34]; }

s32 func_0019A490();

s32 func_0018C9F8(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_00373060, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0018CA40(s32 arg)
{
    return func_0019A638(D_00373060, arg);
}

s32 func_0018CA68(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0018CA70);

/* Byte-matching: volatile counter keeps a single load at the join point. */
extern s32 D_00393CE8;
extern volatile s32 D_00393CEC;

s32 func_0018CAF8(s32 arg0)
{
    s32 *p;

    p = (s32 *)((s32)D_00393CE8 + (u32)D_003731B8);
    if (*p != 0) {
        func_001956D0(*p);
        p = (s32 *)((s32)D_00393CE8 + (u32)D_003731B8);
        *p = 0;
    }
    D_00393CEC = D_00393CEC - 1;
    return arg0;
}

s32 func_0018CB60(s32 a1, s32 a2)
{
    s32 v3;

    D_00393CE8 = a2;
    v3 = func_00195578(D_00373060[0], 4, 16);
    *(s32 *)((u8 *)D_003731B8 + D_00393CE8) = v3;
    if (v3 == 0) return 0;
    ++D_00393CEC;
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_0018CBC8);

s32 func_0018CC50(s32 a1)
{
    s32 v2;
    s32 result;

    D_003731B8[0] = a1;
    func_001979E8();
    v2 = ((s32 (*)(s32, s32, s32))D_003731B8[0x13])(0, a1, 0);
    result = 0;
    if (v2 != 0) {
        func_0019C6A8(a1);
        return a1;
    }
    return result;
}

s32 func_0018CCB0(s32 a1)
{

    s32 (*f)(s32, s32, s32) = (s32 (*)(s32, s32, s32))D_003731B8[28];
    s32 result = a1;

    if (f(0, a1, 0) == 0)
        goto zero;
    D_003731B8[0] = 0;
    return result;
zero:
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_0018CD08);

INCLUDE_ASM(const s32, "core/main", func_0018CED8);

INCLUDE_ASM(const s32, "core/main", func_0018CF88);

INCLUDE_ASM(const s32, "core/main", func_0018D088);

s32 func_0018D1C8(void) { return 772; }

INCLUDE_ASM(const s32, "core/main", func_0018D1D0);

s32 func_0018D2E8(s32 a1, s32 a2, s32 a3, s32 a4)
{
    return func_0019A490(D_00373078, a1, a2, a3, a4, 0);
}

s32 func_0018D328(s32 arg)
{
    return func_0019A638(D_00373078, arg);
}

INCLUDE_ASM(const s32, "core/main", func_0018D350);

INCLUDE_ASM(const s32, "core/main", func_0018D3D8);

s32 func_0018D428(void)
{

    s32 v[8];

    v[4] = 1;
    if (((s32 (*)(s32, s32 *, s32, s32))D_003731CC[0])(13, &v[4], 0, 0) == 0)
        v[4] = 1;
    return v[4];
}

INCLUDE_ASM(const s32, "core/main", func_0018D478);

INCLUDE_ASM(const s32, "core/main", func_0018D520);

s32 func_0018D560(s32 a1)
{



    s32 stack[2];
    s32 v;

    v = ((s32 (*)(s32, s32, s32, s32)) D_003731CC[0])(16, 0, 0, a1);
    if (v != 0)
        return 1;
    if (a1 == 0)
        return 1;
    stack[0] = 1;
    stack[1] = func_0018DA08(0x17, 0x10);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(stack);
    return 0;
}

/* Byte-matching. The memory barrier pins the store before the jal; the single
   inline nop is a branch hazard this compiler cannot emit from plain C. */
extern s32 D_003731CC[];

s32 func_0018D5D0(void)
{


    s32 v9[4];
    s32 v10[4];
    s32 v7;
    s32 r;

    r = ((s32 (*)(s32, s32 *, s32, s32)) D_003731CC[0])(5, v10, 0, 0);
    if (r != 0)
        v7 = 1;
    else
    {
        v9[0] = 1;
        v9[1] = func_0018DA08(0x17, 5);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(v9);
        v7 = 0;
        __asm__ volatile("nop");
    }
    if (v7 != 0)
        return v10[0];
    return -1;
}

s32 func_0018D638(s32 a1, s32 a2)
{



    s32 stack[2];
    s32 r = a1;
    s32 v7;
    s32 v;

    v = ((s32 (*)(s32, s32, s32, s32)) D_003731CC[0])(6, a1, 0, a2);
    if (v != 0)
        v7 = 1;
    else {
        stack[0] = 1;
        stack[1] = func_0018DA08(0x17, 6);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(stack);
        v7 = 0;
        __asm__ volatile("nop");
    }
    if (v7 == 0)
        r = 0;
    return r;
}

/* Byte-matching: see func_0018D5D0 (constant 0xA). */
s32 func_0018D6A8(void)
{


    s32 v9[4];
    s32 v10[4];
    s32 v7;
    s32 r;

    r = ((s32 (*)(s32, s32 *, s32, s32)) D_003731CC[0])(0xA, v10, 0, 0);
    if (r != 0)
        v7 = 1;
    else
    {
        v9[0] = 1;
        v9[1] = func_0018DA08(0x17, 0xA);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(v9);
        v7 = 0;
        __asm__ volatile("nop");
    }
    if (v7 != 0)
        return v10[0];
    return -1;
}

s32 func_0018D710(s32 a1)
{



    s32 v9[4];
    s32 v7;
    s32 r;

    r = ((s32 (*)(s32, s32, s32, s32)) D_003731CC[0])(7, 0, 0, a1);
    if (r != 0)
        v7 = 1;
    else
    {
        v9[0] = 1;
        v9[1] = func_0018DA08(0x17, 7);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(v9);
        v7 = 0;
        __asm__ volatile("nop");
    }
    return v7;
}

/* Byte-matching: see func_0018D5D0 (constant 0xC). */
s32 func_0018D770(void)
{


    s32 v9[4];
    s32 v10[4];
    s32 v7;
    s32 r;

    r = ((s32 (*)(s32, s32 *, s32, s32)) D_003731CC[0])(0xC, v10, 0, 0);
    if (r != 0)
        v7 = 1;
    else
    {
        v9[0] = 1;
        v9[1] = func_0018DA08(0x17, 0xC);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(v9);
        v7 = 0;
        __asm__ volatile("nop");
    }
    if (v7 != 0)
        return v10[0];
    return -1;
}

/* Byte-matching: see func_0018D5D0 (constant 0x13). */
s32 func_0018D7D8(void)
{


    s32 v9[4];
    s32 v10[4];
    s32 v7;
    s32 r;

    r = ((s32 (*)(s32, s32 *, s32, s32)) D_003731CC[0])(0x13, v10, 0, 0);
    if (r != 0)
        v7 = 1;
    else
    {
        v9[0] = 1;
        v9[1] = func_0018DA08(0x17, 0x13);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(v9);
        v7 = 0;
        __asm__ volatile("nop");
    }
    if (v7 != 0)
        return v10[0];
    return -1;
}

s32 func_0018D840(s32 a1)
{



    s32 v9[4];
    s32 v7;
    s32 r;

    r = ((s32 (*)(s32, s32, s32, s32)) D_003731CC[0])(8, 0, 0, a1);
    if (r != 0)
        v7 = 1;
    else
    {
        v9[0] = 1;
        v9[1] = func_0018DA08(0x17, 8);
        __asm__ volatile("" ::: "memory");
        func_0018DAD0(v9);
        v7 = 0;
        __asm__ volatile("nop");
    }
    return v7;
}

s32 func_0018D8A0(void) { return D_003732FC[0]; }

INCLUDE_ASM(const s32, "core/main", func_0018D8B0);

s32 func_0018D9A0(void) { return D_00393748; }

extern u8 D_003732E4[];

s32 func_0018D9A8(s32 p)
{
    return ((s32 (*)(s32)) *(s32 *)D_003732E4)(*(s32 *)(p + 0x10));
}

extern s32 D_003732E8[];
u8 *func_0018D9D0(u8 *arg0, u8 *arg1)
{
    ((void (*)(u8 *)) D_003732E8[0])(arg1);
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_0018DA08);

extern s32 D_00393CF0;

u8 *func_0018DA48(u8 *arg0, u32 idx)
{
    u32 *p = (u32 *)(idx + (u32)D_003731B8);
    D_00393CF4++;
    p[1] = 0x80000000u;
    D_00393CF0 = (s32)idx;
    p[0] = 0;
    return arg0;
}

u8 *func_0018DA78(u8 *p)
{
    D_00393CF4--;
    return p;
}

extern s32 D_00393CF0;

u8 *func_0018DA90(u8 *dst)
{
    u32 *p = (u32 *)(D_00393CF0 + (u32)D_003731B8);
    *(UnalignedU64 *)dst = *(UnalignedU64 *)p;
    p = (u32 *)(D_00393CF0 + (u32)D_003731B8);
    p[1] = 0x80000000u;
    p[0] = 0;
    return dst;
}

INCLUDE_ASM(const s32, "core/main", func_0018DAD0);

INCLUDE_ASM(const s32, "core/main", func_0018DB30);

INCLUDE_ASM(const s32, "core/main", func_0018DC88);

INCLUDE_ASM(const s32, "core/main", func_0018DD68);

INCLUDE_ASM(const s32, "core/main", func_0018DF20);

INCLUDE_ASM(const s32, "core/main", func_0018DF98);

INCLUDE_ASM(const s32, "core/main", func_0018E018);

INCLUDE_ASM(const s32, "core/main", func_0018E098);

INCLUDE_ASM(const s32, "core/main", func_0018E118);

INCLUDE_ASM(const s32, "core/main", func_0018E198);

INCLUDE_ASM(const s32, "core/main", func_0018E220);

INCLUDE_ASM(const s32, "core/main", func_0018E2D0);

INCLUDE_ASM(const s32, "core/main", func_0018E3A0);

s32 func_0018E4A8(u8 *a1, s32 (*a2)(s32, s32), s32 a3)
{
    u8 *v = *(u8 **)(a1 + 0xA0);
    u8 *next;
    if (v == 0) goto end;
loop:
    next = *(u8 **)(v + 0xA4);
    if (a2((s32)v, a3) == 0) goto end;
    v = next;
    if (v != 0) goto loop;
end:
    return (s32)a1;
}

INCLUDE_ASM(const s32, "core/main", func_0018E518);

s32 func_0018E618(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

s32 func_0018E620(const s32 *p) { return *(const s32 *)((const u8 *)p + (168)); }

s32 func_0018E628(s32 a1)
{

    s32 p = *(s32 *)(a1 + 0xA8);
    s32 q = a1;

    if ((*(u8 *)(p + 3) & 1) != 0)
    {
        func_00197B38(p);
        return q + 0x50;
    }
    return q + 0x50;
}

u8 *func_0018E668(u8 *p) { return p + 16; }

INCLUDE_ASM(const s32, "core/main", func_0018E670);

INCLUDE_ASM(const s32, "core/main", func_0018E6D0);

s32 func_0018E7B0(s32 *p) { return ((u8 *)p[0x2A])[3] & 3; }

s32 func_0018E7C0(u8 *a1)
{
    u8 *p = *(u8 **)(a1 + 0xA0);
    s32 i = 1;

    while (p != 0) {
        i += func_0018E7C0(p);
        p = *(u8 **)(p + 0xA4);
    }
    return i;
}

s32 func_0018E810(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 1;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_0039374C;
    __asm__ volatile("" ::: "memory");
    D_0039374C++;
    return ret;
}

s32 func_0019A490();

s32 func_0018E858(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_003771B8, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0018E8A0(s32 arg)
{
    return func_0019A638(D_003771B8, arg);
}

s32 func_0018E8C8(void) { return 1; }

u8 *func_0018E8D0(u8 *arg0)
{


    u8 *p = (u8 *)func_0018DB30(arg0, 0);
    u8 *r = p;

    if (p != 0) {
        *(u8 *)(p + 3) = 0;
        func_0018E670(p);
        r = p;
    }
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_0018E910);

/* Byte-matching: volatile counter keeps a single load at the join point. */
extern s32 D_00393CF8;
extern volatile s32 D_00393CFC;

s32 func_0018E9A8(s32 arg0)
{
    s32 *p;

    p = (s32 *)((s32)D_00393CF8 + (u32)D_003731B8);
    if (*p != 0) {
        func_001956D0(*p);
        p = (s32 *)((s32)D_00393CF8 + (u32)D_003731B8);
        *p = 0;
    }
    D_00393CFC = D_00393CFC - 1;
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_0018EA10);

INCLUDE_ASM(const s32, "core/main", func_0018EA88);

INCLUDE_ASM(const s32, "core/main", func_0018EAE0);

/* Byte-matching: 10-entry vtable initializer; source stores in ascending
   offset order so cc1 rotates them (??2 rule scaled to 10 stores). */
s32 func_0018EBE8(void)
{











    D_003731B8[49] = (s32)func_0018EC88;
    D_003731B8[50] = (s32)func_002227D8;
    D_003731B8[51] = (s32)func_00222120;
    D_003731B8[52] = (s32)func_00222858;
    D_003731B8[53] = (s32)func_00223768;
    D_003731B8[54] = (s32)func_00222340;
    D_003731B8[55] = (s32)func_00222808;
    D_003731B8[56] = (s32)func_00222228;
    D_003731B8[57] = (s32)func_00222E08;
    D_003731B8[58] = (s32)func_00222238;
    return 1;
}


void func_0018EC70(void) {
}

extern u8 D_0037327C[];

u8 *func_0018EC78(void) { return D_0037327C; }

s32 func_0018EC88(s32 a1)
{

    s32 v = ((s32 (*)(s32, s32))D_003731B8[50])(a1, (s32)&D_00393750);
    s32 r = (v != 0);

    if (v != 0)
        ((void (*)(s32))D_003731B8[51])(v);
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_0018ECE0);

INCLUDE_ASM(const s32, "core/main", func_0018EE48);

INCLUDE_ASM(const s32, "core/main", func_0018EF78);

INCLUDE_ASM(const s32, "core/main", func_0018F160);

INCLUDE_ASM(const s32, "core/main", func_0018F338);

INCLUDE_ASM(const s32, "core/main", func_0018F448);

INCLUDE_ASM(const s32, "core/main", func_0018FA18);

INCLUDE_ASM(const s32, "core/main", func_0018FD00);

INCLUDE_ASM(const s32, "core/main", func_0018FE28);

INCLUDE_ASM(const s32, "core/main", func_0018FFB0);


/* Source: engine/region_0019.c */
/* func_00190170 through func_0019FED8; retail address range 0x00190000-0x001A0000. */


INCLUDE_ASM(const s32, "core/main", func_00190170);

INCLUDE_ASM(const s32, "core/main", func_00190358);

INCLUDE_ASM(const s32, "core/main", func_001905D8);

s32 func_001906E0(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 1;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_00393754;
    __asm__ volatile("" ::: "memory");
    D_00393754++;
    return ret;
}

s32 func_00190728(s32 a1, s32 a2, s32 a3)
{
    s32 *p = (s32 *)((s32)D_00393D00 + (u32)D_003731B8);
    s32 v = ((s32 (*)(s32))D_003731B8[79])(*p);

    if (__builtin_expect(v == 0, 0))
        return 0;
    *(s32 *)(v + 4) = a1;
    *(s32 *)(v + 8) = a2;
    *(s32 *)(v + 0xC) = a3;
    *(s32 *)(v + 0x14) = 0;
    *(s32 *)(v + 0x18) = 0;
    *(s32 *)v = 0;
    func_0019A668((u32)&D_003771D0, v);
    return v;
}

s32 func_001907C0(s32 a1)
{
  s32 *p;
  if ( (*(u32 *)a1 & 1) != 0 )
    func_00190938(a1);
  func_0019A700((u32)&D_003771D0, a1);
  p = (s32 *)((s32)D_00393D00 + (u32)D_003731B8);
  ((s32 (*)(s32, s32, s32))D_003731B8[80])(*p, a1, (s32)p);
  return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00190830);

extern s32 D_003732E8[];

u8 *func_00190938(u8 *a1)
{
    s32 v;
    ((void (*)(s32)) D_003732E8[0]) (*(s32 *)(a1 + 0x14));
    v = *(s32 *)a1;
    *(s32 *)(a1 + 0x14) = 0;
    *(s32 *)(a1 + 0x18) = 0;
    *(s32 *)a1 = v & -2;
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_00190988);

INCLUDE_ASM(const s32, "core/main", func_00190A88);

s32 func_00190B68(void)
{
    s32 index = D_00393D00;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x04);
}

INCLUDE_ASM(const s32, "core/main", func_00190B80);

s32 *func_00190C60(s32 *p, s32 value) { p[4] = value; return p; }

s32 *func_00190C70(s32 *p, s32 value) { p[5] = value; return p; }

s32 *func_00190C80(s32 *p, s32 value) { p[6] = value; return p; }

s32 func_00190C90(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

s32 func_00190C98(const s32 *p) { return *(const s32 *)((const u8 *)p + (8)); }

s32 func_00190CA0(const s32 *p) { return *(const s32 *)((const u8 *)p + (12)); }

s32 func_00190CA8(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_00190CB0(const s32 *p) { return *(const s32 *)((const u8 *)p + (24)); }

extern s32 D_00373208[];
s32 func_00190CB8(s32 arg0, s32 arg1)
{
    s32 stack;
    ((s32 (*)(s32 *, s32, s32)) D_00373208[0])(&stack, arg0, arg1);
    return stack;
}

extern s32 D_0037320C[];
u8 *func_00190CE8(u8 *arg0, s32 arg1)
{
    s32 stack;
    stack = arg1;
    ((s32 (*)(u8 *, s32 *)) D_0037320C[0])(arg0, &stack);
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_00190D20);

f32 func_00190E40(void)
{
    s32 index = D_00393D00;
    u8 *base = (u8 *)D_003731B8;

    return *(f32 *)(base + index + 0x20c);
}

INCLUDE_ASM(const s32, "core/main", func_00190E58);

s32 func_0019A490();

s32 func_00190EB0(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_003771D0, arg1, arg2, arg3, arg4, arg5);
}

s32 func_00190EF8(s32 arg)
{
    return func_0019A638(D_003771D0, arg);
}

s32 func_00190F20(void) { return 1; }

/* Byte-matching.  Runs the callback registered by func_0018FA18 and returns
   the value it left in the local. */
s32 func_00190F28(s32 arg0)
{


    s32 local = 0;

    func_0018FA18(arg0, 0x14, func_001910D8, &local);
    return local;
}

/* Byte-matching.  Size of a buffer: rows times stride plus 0x1C, plus 4<<n
   for a small extra table. */
s32 func_00190F58(u8 *arg0)
{
    s32 n = *(s32 *)(arg0 + 8) * *(s32 *)(arg0 + 0x10);
    s32 shift = *(s32 *)(arg0 + 0xC);
    s32 size = n + 0x1C;

    if (shift < 9)
        size += 4 << shift;
    return size;
}

INCLUDE_ASM(const s32, "core/main", func_00190F88);

INCLUDE_ASM(const s32, "core/main", func_00191000);

INCLUDE_ASM(const s32, "core/main", func_00191078);

INCLUDE_ASM(const s32, "core/main", func_001910D8);

INCLUDE_ASM(const s32, "core/main", func_001911C0);

INCLUDE_ASM(const s32, "core/main", func_00191660);

INCLUDE_ASM(const s32, "core/main", func_00191D80);

INCLUDE_ASM(const s32, "core/main", func_00191EF8);

INCLUDE_ASM(const s32, "core/main", func_00192250);

INCLUDE_ASM(const s32, "core/main", func_001928E0);

INCLUDE_ASM(const s32, "core/main", func_00192A90);

INCLUDE_ASM(const s32, "core/main", func_00192C78);

INCLUDE_ASM(const s32, "core/main", func_00192E78);

s32 func_00192F80(s32 a1, s32 a2)
{



    s32 v = func_00197768(2, 2, a2);

    if (v != 0) {
        func_00192E78(v, 1, *(s32 *)(a1 + 4), *(s32 *)(a1 + 8), a1);
        func_00197810(v, 0);
        return a1;
    }
    return 0;
}

f32 func_00192FF0(void) { return D_003731D0[0]; }

f32 func_00193000(void) { return D_003731D4[0]; }

s32 func_00193010(void)
{
    return ((s32 (*)(void)) D_003731DC[0])();
}

INCLUDE_ASM(const s32, "core/main", func_00193038);

s32 func_00193090(void)
{
    return ((s32 (*)(void)) D_003731E0[0])();
}

s32 func_001930B8(void)
{
    return ((s32 (*)(void)) D_003731E4[0])();
}

s32 func_001930E0(void)
{
    return ((s32 (*)(void)) D_003731E8[0])();
}

s32 func_00193108(void)
{
    return ((s32 (*)(void)) D_003731EC[0])();
}

s32 func_00193130(f32 *a1, f32 *a2, f32 *a3)
{
    a1[0] = (a2[0] * a3[0] + a2[1] * a3[4]) + a2[2] * a3[8];
    a1[1] = (a2[0] * a3[1] + a2[1] * a3[5]) + a2[2] * a3[9];
    a1[2] = (a2[0] * a3[2] + a2[1] * a3[6]) + a2[2] * a3[10];
    a1[4] = (a2[4] * a3[0] + a2[5] * a3[4]) + a2[6] * a3[8];
    a1[5] = (a2[4] * a3[1] + a2[5] * a3[5]) + a2[6] * a3[9];
    a1[6] = (a2[4] * a3[2] + a2[5] * a3[6]) + a2[6] * a3[10];
    a1[8] = (a2[8] * a3[0] + a2[9] * a3[4]) + a2[10] * a3[8];
    a1[9] = (a2[8] * a3[1] + a2[9] * a3[5]) + a2[10] * a3[9];
    a1[10] = (a2[8] * a3[2] + a2[9] * a3[6]) + a2[10] * a3[10];
    a1[12] = ((a2[12] * a3[0] + a2[13] * a3[4]) + a2[14] * a3[8]) + a3[12];
    a1[13] = ((a2[12] * a3[1] + a2[13] * a3[5]) + a2[14] * a3[9]) + a3[13];
    a1[14] = ((a2[12] * a3[2] + a2[13] * a3[6]) + a2[14] * a3[10]) + a3[14];
}

INCLUDE_ASM(const s32, "core/main", func_00193390);

INCLUDE_ASM(const s32, "core/main", func_00193858);

INCLUDE_ASM(const s32, "core/main", func_00193AC8);

INCLUDE_ASM(const s32, "core/main", func_00193C40);

INCLUDE_ASM(const s32, "core/main", func_00193F20);

INCLUDE_ASM(const s32, "core/main", func_00194248);

INCLUDE_ASM(const s32, "core/main", func_001943F8);

INCLUDE_ASM(const s32, "core/main", func_00194638);

INCLUDE_ASM(const s32, "core/main", func_001947F0);

extern s32 D_00393D08;
s32 func_00194AC8(u8 *arg0)
{
    s32 *p = (s32 *)(D_00393D08 + (u32)D_003731B8);
    s32 (*handler)(s32, u8 *, s32) = (s32 (*)(s32, u8 *, s32))D_003731B8[80];
    handler(p[0], arg0, (s32)p);
    return 1;
}

s32 func_00194B00(void)
{
    u8 *p = (u8 *)D_003731B8 + D_00393D08;
    s32 (*fn)(s32 *) = *(s32 (**)(s32 *))((u8 *)D_003731B8 + 0x13C);
    s32 v = fn(*(s32 *)p);

    if (v != 0)
    {
        f32 zero = 0.0f, one = 1.0f;
        *(volatile f32 *)(v + 0x30) = zero;
        *(volatile f32 *)(v + 0x00) = one;
        *(volatile u32 *)(v + 0x0C) = 131075;
        *(volatile f32 *)(v + 0x28) = one;
        *(volatile f32 *)(v + 0x14) = one;
        *(volatile f32 *)(v + 0x10) = zero;
        *(volatile f32 *)(v + 0x08) = zero;
        *(volatile f32 *)(v + 0x04) = zero;
        *(volatile f32 *)(v + 0x24) = zero;
        *(volatile f32 *)(v + 0x20) = zero;
        *(volatile f32 *)(v + 0x18) = zero;
        *(volatile f32 *)(v + 0x38) = zero;
        *(volatile f32 *)(v + 0x34) = zero;
        __asm__ volatile("nop");
    }
    return v;
}

s32 func_00194B88(void)
{


    s32 stack[2];
    s32 v;
    stack[0] = 1;
    stack[1] = func_0018DA08(0x8000001B);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(stack);
    v = D_00393758;
    __asm__ volatile("" ::: "memory");
    D_00393758++;
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_00194BD0);

s32 func_00193390(void);

s32 func_00194D28(void)
{
    return func_00193390();
}

INCLUDE_ASM(const s32, "core/main", func_00194D48);

s32 func_00193C40(void);

s32 func_00194E48(void)
{
    return func_00193C40();
}

s32 *func_00194E68(s32 *arg1)
{
    arg1[3] &= (s32)0xFFFDFFFC;
    return arg1;
}

INCLUDE_ASM(const s32, "core/main", func_00194E88);

INCLUDE_ASM(const s32, "core/main", func_00194F08);

INCLUDE_ASM(const s32, "core/main", func_00194FB0);

INCLUDE_ASM(const s32, "core/main", func_00195048);

s32 func_001950F8(s32 arg0)
{
    u8 *entry = (u8 *)D_003731B8 + D_00393D08;

    *(s32 *)(entry + 4) = arg0;
    return 1;
}

extern s32 D_00393D08;
extern s32 D_003731B8[];
extern s32 func_00193130();

s32 func_00195118(u32 arg0)
{
    u32 v;
    s32 *p;
    v = arg0;
    if (v == 0) v = (u32)func_00193130;
    p = (s32 *)((s8 *)D_003731B8 + D_00393D08);
    p[2] = (s32)v;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00195148);

/* Byte-matching: volatile counter keeps a single load at the join point.
   D_00393D08 is already declared extern s32 in this file. */
extern volatile s32 D_00393D0C;

s32 func_001951C0(s32 arg0)
{
    s32 *p;

    p = (s32 *)((s32)D_00393D08 + (u32)D_003731B8);
    if (*p != 0) {
        func_001956D0(*p);
        p = (s32 *)((s32)D_00393D08 + (u32)D_003731B8);
        *p = 0;
    }
    D_00393D0C = D_00393D0C - 1;
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_00195228);

INCLUDE_ASM(const s32, "core/main", func_001953D0);

extern u8 D_003732E4[];

u8 *func_00195568(void) { return D_003732E4; }

INCLUDE_ASM(const s32, "core/main", func_00195578);

INCLUDE_ASM(const s32, "core/main", func_001956D0);

u8 *func_00195780(u8 *p, s32 value)
{
    s32 *node = *(s32 **)(p + 4);
    s32 *next = node + 1;
    node[1] = value;
    *(s32 **)(p + 4) = next;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00195798);

INCLUDE_ASM(const s32, "core/main", func_00195848);

INCLUDE_ASM(const s32, "core/main", func_00195948);

void func_00195A40(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00195A48);

INCLUDE_ASM(const s32, "core/main", func_00195AC8);

s32 func_00195B18(const s32 *left, const s32 *right) { return *left - *right; }

INCLUDE_ASM(const s32, "core/main", func_00195B28);

INCLUDE_ASM(const s32, "core/main", func_00195BE8);

INCLUDE_ASM(const s32, "core/main", func_00195CE0);

s32 func_00195D50(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 1;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_0039375C;
    __asm__ volatile("" ::: "memory");
    D_0039375C++;
    return ret;
}

s32 func_00195D98(const s32 *p) { return *(const s32 *)((const u8 *)p + (12)); }

s32 func_00195DA0(const s32 *p) { return *(const s32 *)((const u8 *)p + (16)); }

s32 func_00195DA8(const s32 *p) { return *(const s32 *)((const u8 *)p + (24)); }

s32 func_00195DB0(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_00195DB8(u8 *p)
{
    return p[0x23] << 8;
}

s32 func_00195DC8(s32 a1)
{


    s32 result;
    if ((func_00195DB8(a1) & 0x8000) == 0) return 1;
    if (((s32 (*)(s32 *, s32, s32))D_00373270[0])(&result, a1, 0) == 0) return -1;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00195E20);

s32 func_00195EC0(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

extern s32 D_0037324C[];
u8 *func_00195EC8(u8 *arg0, s32 arg1, s32 arg2)
{
    s32 stack[2];
    stack[0] = arg1;
    stack[1] = arg2;
    ((s32 (*)(u8 *, s32 *, s32)) D_0037324C[0])(arg0, stack, 0);
    return arg0;
}

extern s32 D_00373244[];
u8 *func_00195F08(u8 *arg0, s32 arg1, s32 arg2)
{
    s32 stack[2];
    stack[0] = arg1;
    stack[1] = arg2;
    ((s32 (*)(u8 *, s32 *, s32)) D_00373244[0])(arg0, stack, 0);
    return arg0;
}

extern s32 D_00373248[];
u8 *func_00195F48(u8 *arg0, u8 *arg1)
{
    ((s32 (*)(u8 *, u8 *, s32)) D_00373248[0])(arg0, arg1, 0);
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_00195F80);

INCLUDE_ASM(const s32, "core/main", func_00195FF8);

s32 func_00196060(void)
{
    s32 *p = (s32 *)((s8 *)D_003731B8 + D_00393D18);
    s32 i = p[10];
    s32 *q = p + i;
    return *q;
}

INCLUDE_ASM(const s32, "core/main", func_00196088);

extern s32 D_00373234[];
s32 func_001960F8(u8 *arg0, u8 *arg1)
{
    ((s32 (*)(s32, u8 *, u8 *)) D_00373234[0])(0, arg0, arg1);
    return 1;
}

extern s32 D_00373250[];
s32 func_00196128(s32 a1, s32 a2, s32 a3)
{
    s32 (*v5)(s32, s32, s32);
    v5 = (s32 (*)(s32, s32, s32))D_00373250[0];
    func_00196810();
    if (v5(a1, a2, a3) == 0) return 0;
    return a1;
}

extern s32 D_0037323C[];
s32 func_00196190(u8 *arg0, s32 arg1, s32 arg2)
{
    s32 stack;
    if (((s32 (*)(s32 *, u8 *, s32))D_0037323C[0])(&stack, arg0, arg2 + ((arg1 & 0xFF) << 8)) != 0)
        return stack;
    return 0;
}

extern s32 D_00373240[];
u8 *func_001961D0(u8 *arg0)
{
    ((s32 (*)(s32, u8 *, s32)) D_00373240[0])(0, arg0, 0);
    return arg0;
}

extern s32 D_0037325C[];

s32 func_00196210(s32 arg0, s32 arg1)
{
    s32 stack;
    s32 v = ((s32 (*)(s32 *, s32, s32)) D_0037325C[0])(&stack, arg0, arg1);
    if (v != 0)
        return stack;
    return 0;
}

extern s32 D_00373260[];

u8 *func_00196248(u8 *arg0)
{
    ((s32 (*)(s32, u8 *, s32)) D_00373260[0])(0, arg0, 0);
    *(u8 *)(arg0 + 0x22) &= 0xE7;
    return arg0;
}

s32 func_0019A490();

s32 func_00196290(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_003771E8, arg1, arg2, arg3, arg4, arg5);
}

s32 func_001962D8(s32 arg)
{
    return func_0019A638(D_003771E8, arg);
}

s32 func_00196300(void) { return 1; }

/* Byte-matching: volatile counter keeps a single load at the join point.
   D_00393D18 is already declared extern s32 in this file. */
extern volatile s32 D_00393D1C;

s32 func_00196308(s32 arg0)
{
    s32 *p;

    p = (s32 *)((s32)D_00393D18 + (u32)D_003731B8);
    if (p[0x18] != 0) {
        func_001956D0(p[0x18]);
        p = (s32 *)((s32)D_00393D18 + (u32)D_003731B8);
        p[0x18] = 0;
    }
    D_00393D1C = D_00393D1C - 1;
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_00196370);

INCLUDE_ASM(const s32, "core/main", func_00196548);

INCLUDE_ASM(const s32, "core/main", func_00196668);

s32 func_00196720(void)
{
    s32 index = D_00393D20;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x00);
}

INCLUDE_ASM(const s32, "core/main", func_00196738);

INCLUDE_ASM(const s32, "core/main", func_00196788);

INCLUDE_ASM(const s32, "core/main", func_00196810);

INCLUDE_ASM(const s32, "core/main", func_00196898);

INCLUDE_ASM(const s32, "core/main", func_001968E8);

s32 func_00196998(s32 a1)
{




    func_00196548(a1);
    func_00196E88();
    ((void (*)(s32))D_003732E8[0])(D_00393760);
    D_00393760 = 0;
    D_00393D24 = D_00393D24 - 1;
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_001969E8);

INCLUDE_ASM(const s32, "core/main", func_00196B30);

INCLUDE_ASM(const s32, "core/main", func_00196E00);

s32 func_00196E88(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00196E90);

INCLUDE_ASM(const s32, "core/main", func_00196F50);

/* Byte-matching: u32 product + call + conditional writeback. */
s32 func_00197010(s32 a, s32 b)
{


    u32 v = (u32)a * (u32)b;
    s32 r = func_001969E8(v);

    if (r != 0)
        func_00224B88(r, 0, v);
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_00197060);

INCLUDE_ASM(const s32, "core/main", func_001972B8);

INCLUDE_ASM(const s32, "core/main", func_00197458);

INCLUDE_ASM(const s32, "core/main", func_00197630);

INCLUDE_ASM(const s32, "core/main", func_00197768);

INCLUDE_ASM(const s32, "core/main", func_00197810);

s32 func_00197928(u8 *arg0, s32 arg1)
{

    s32 v;
    D_00393D28 = arg1;
    v = func_00195578(0x24, 0xA, 0);
    *(s32 *)(D_00393D28 + (s32)D_003731B8) = v;
    if (v == 0)
        return 0;
    D_00393D2C++;
    return (s32)arg0;
}

s32 func_00197990(u8 *arg0)
{

    s32 *p = (s32 *)(D_00393D28 + (s32)D_003731B8);

    if (*p != 0)
        func_001956D0(*p);
    D_00393D2C--;
    return (s32)arg0;
}

INCLUDE_ASM(const s32, "core/main", func_001979E8);

INCLUDE_ASM(const s32, "core/main", func_00197B38);

/* Byte-matching.  Calls the callback stored at +0x10 and returns the object. */
u8 *func_00197C10(u8 *arg0)
{
    void (*fn)() = *(void (**)())(arg0 + 0x10);

    fn();
    return arg0;
}

INCLUDE_ASM(const s32, "core/main", func_00197C40);

INCLUDE_ASM(const s32, "core/main", func_00197CD0);

INCLUDE_ASM(const s32, "core/main", func_00197D50);

INCLUDE_ASM(const s32, "core/main", func_00197E70);

INCLUDE_ASM(const s32, "core/main", func_00198168);

INCLUDE_ASM(const s32, "core/main", func_00198420);

INCLUDE_ASM(const s32, "core/main", func_001986A8);

INCLUDE_ASM(const s32, "core/main", func_00198C50);

INCLUDE_ASM(const s32, "core/main", func_00198F70);

INCLUDE_ASM(const s32, "core/main", func_001991E0);

INCLUDE_ASM(const s32, "core/main", func_00199390);

s32 func_00199560(s32 arg0)
{
    u8 *entry = (u8 *)D_003731B8 + D_00393D30;

    *(s32 *)(entry + 0x18) = arg0;
    return 1;
}

s32 func_00199580(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x18);
}

s32 func_00199598(s32 arg0)
{
    u8 *entry = (u8 *)D_003731B8 + D_00393D30;

    *(s32 *)(entry + 0x1C) = arg0;
    return 1;
}

s32 func_001995B8(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x1c);
}

s32 func_001995D0(s32 arg0)
{
    u8 *entry = (u8 *)D_003731B8 + D_00393D30;

    *(s32 *)(entry + 0x28) = arg0;
    return 1;
}

s32 func_001995F0(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x28);
}

s32 func_00199608(s32 arg0)
{
    u8 *entry = (u8 *)D_003731B8 + D_00393D30;

    *(s32 *)(entry + 0x2C) = arg0;
    return 1;
}

s32 func_00199628(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x2c);
}

s32 func_00199640(s32 a1, s32 a2, u8 a3)
{
    s32 *p = (s32 *)(D_00393D30 + (u32)D_003731B8);
    s32 (*handler)(s32, s32, u8) = (s32 (*)(s32, s32, u8))p[11];
    if (handler == 0) goto zero;
    return handler(a1, a2, a3);
zero:
    return 0;
}

extern s32 D_00393D30;

s32 func_00199688(void)
{
    s32 *p = (s32 *)(D_00393D30 + (u32)D_003731B8);
    s32 (*handler)(void) = (s32 (*)(void))p[10];

    return handler() != 0;
}

s32 func_001996C0(s32 arg)
{
    s32 *p = (s32 *)((s8 *)D_003731B8 + D_00393D30);
    s32 result = 1;
    p[6] = arg;
    p[7] = arg;
    return result;
}

s32 func_001996E0(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x1c);
}

s32 func_001996F8(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x14);
}

s32 func_00199710(s32 arg0)
{
    u8 *entry = (u8 *)D_003731B8 + D_00393D30;

    *(s32 *)(entry + 0x14) = arg0;
    return 1;
}

u8 *func_00199730(u8 *p) { return p + 16; }

u8 *func_00199738(u8 *p) { return p + 48; }

INCLUDE_ASM(const s32, "core/main", func_00199740);

INCLUDE_ASM(const s32, "core/main", func_001997C8);

s32 func_00199850(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 1;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_00393798;
    __asm__ volatile("" ::: "memory");
    D_00393798++;
    return ret;
}

s32 func_00199898(s32 a1, s32 a2, s32 a3, s32 a4)
{



    s32 v[2];
    s32 ret;

    v[0] = 1;
    v[1] = func_0018DA08(0x8000001B, a2, a3, a4);
    __asm__ volatile("" ::: "memory");
    func_0018DAD0(v);
    ret = D_0039379C;
    __asm__ volatile("" ::: "memory");
    D_0039379C++;
    return ret;
}

INCLUDE_ASM(const s32, "core/main", func_001998E0);

INCLUDE_ASM(const s32, "core/main", func_00199990);

s32 func_00199A30(s32 a1)
{

    s32 (*fn)(s32);
    u32 base;
    s32 *p;
    s32 v2;

    base = (u32)D_003731B8;
    p = (s32 *)(D_00393D30 + base);
    fn = *(s32 (**)(s32))(base + 0x13C);
    v2 = fn(p[2]);
    if (v2 != 0)
    {
        *(u32 *)v2 = a1;
        *(u32 *)(v2 + 4) = 0;
        *(u8 *)(v2 + 16) = 0;
        *(u8 *)(v2 + 48) = 0;
        *(u32 *)(v2 + 80) = 1;
        *(u32 *)(v2 + 84) = 1;
        *(u32 *)(v2 + 88) = 1;
        *(u32 *)(v2 + 92) = 1;
        func_0019A668((u32)&D_00377200, v2);
    }
    return v2;
}

u8 *func_00199AB8(u8 *p)
{
    *(s32 *)(p + 0x50) += 1;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00199AD0);

extern s32 D_00373220[];

s32 func_00199B88(u8 *a1, u32 a2)
{
    s32 r = (s32)a1;
    if (a2 != 0) {
        if (((s32 (*)(u8 *, u32, s32)) D_00373220[0])(a1, a2, 0) == 0)
            r = 0;
    } else {
        *(s32 *)a1 = 0;
    }
    return r;
}

s32 *func_00199BD0(s32 *p, s32 value) { p[0x15] = value; return p; }

s32 *func_00199BE0(s32 *p, s32 value) { p[0x16] = value; p[0x17] = value; return p; }

s32 *func_00199BF0(s32 *p, s32 value) { p[0x16] = value; return p; }

s32 *func_00199C00(s32 *p, s32 value) { p[0x17] = value; return p; }

INCLUDE_ASM(const s32, "core/main", func_00199C10);

s32 *func_00199C68(s32 *p)
{
    volatile s32 *q = (volatile s32 *)p;
    if (q[1] == 0) goto out;
    q[1] = 0;
    *(s32 *)q[3] = q[2];
    *(s32 *)(q[2] + 4) = q[3];
    __asm__ volatile("nop");
out:
    return (s32 *)q;
}

INCLUDE_ASM(const s32, "core/main", func_00199CA0);

s32 func_00199D70(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x10);
}

u8 *func_00199D88(u8 *arg0)
{
    u8 *entry = (u8 *)D_003731B8 + D_00393D30;

    *(s32 *)(entry + 0x10) = (s32)arg0;
    return arg0;
}

s32 func_00199DA8(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

INCLUDE_ASM(const s32, "core/main", func_00199DB0);

s32 func_0019A490();

s32 func_00199E20(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_00377200, arg1, arg2, arg3, arg4, arg5);
}

s32 func_0019A490();

s32 func_00199E68(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    return func_0019A490(D_00377218, arg1, arg2, arg3, arg4, arg5);
}

s32 func_00199EB0(s32 arg)
{
    return func_0019A638(D_00377200, arg);
}

s32 func_00199ED8(s32 arg)
{
    return func_0019A638(D_00377218, arg);
}

s32 func_00199F00(void) { return 1; }

s32 func_00199F08(void) { return 1; }

s32 func_00199F10(s32 a1, s8 *a2, u8 a3)
{


    u8 buf[4];
    s32 v;

    buf[0] = 0x6D;
    if ((u8)(a3 - 1) < 15)
        v = (s8)D_0038BE48[a3];
    else
        v = 0;
    buf[2] = 0;
    buf[1] = v;
    if (v != 0)
    {
        ((s32 (*)(s32, s32))D_003731B8[0x3F])(a1, (s32)buf);
        if (a2 != 0 && *a2 != 0)
            ((s32 (*)(s32, s32))D_003731B8[0x3F])((s32)a2, (s32)buf);
    }
    return 1;
}

s32 func_00199FB0(s32 arg0, s32 arg1)
{


    s32 *p = (s32 *)(D_00393D30 + (u32)D_003731B8);
    if (p[6] != 0)
        return func_001986A8(arg0, arg1);
    return func_00198420(arg0, arg1);
}

u8 *func_00199FF8(u8 *a1)
{


    if (*(s32 *)(a1 + 0x50) >= 2)
    {
        do {
            func_00199AD0(a1);
        } while (*(s32 *)(a1 + 0x50) >= 2);
    }
    func_00199AD0(a1);
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_0019A050);

s32 func_0019A198(s32 *a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    s32 *i = (s32 *)a1[3];

    while (i != 0 && i[2] != a2)
        i = (s32 *)i[10];
    if (i != 0) {
        i[3] = a3;
        i[4] = a4;
        i[5] = a5;
        return i[0];
    }
    return -1;
}

INCLUDE_ASM(const s32, "core/main", func_0019A1D8);

INCLUDE_ASM(const s32, "core/main", func_0019A308);

s32 func_0019A3A8(u8 *a1, s32 a2)
{
    u8 *node = *(u8 **)(a1 + 0xC);
    s32 sum = 0;

    while (node != 0) {
        s32 (*cb)(s32, s32, s32) = *(s32 (**)(s32, s32, s32))(node + 0x14);
        if (cb != 0) {
            s32 r = cb(a2, *(s32 *)(node + 0), *(s32 *)(node + 4));
            if (r > 0) sum += r + 12;
        }
        node = *(u8 **)(node + 0x28);
    }
    return sum;
}

extern s32 D_00393D38;
s32 func_0019A420(void)
{

    s32 v = func_00195578(0x34, 0x14, 0);
    D_00393D38 = v;
    return v != 0;
}

s32 func_0019A450(void)
{



    if (D_00393D38 != 0) {
        func_00195848(D_00393D38, func_0019A7D0, D_00393D38);
        func_001956D0(D_00393D38);
        D_00393D38 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0019A490);

s32 func_0019A638(s32 *a1, s32 a2)
{
    s32 *p = (s32 *)a1[3];

    if (p != 0) {
        for (;;) {
            s32 i = p[2];
            if (i == a2)
                return p[0];
            p = (s32 *)p[10];
            if (p == 0)
                break;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "core/main", func_0019A668);

s32 func_0019A700(u8 *a1, s32 a2)
{
    u8 *v3;

    v3 = (u8 *)*(s32 *)(a1 + 16);
    if (v3 != 0) {
        do {
            ((s32 (*)(s32, s32, s32))*(s32 *)(v3 + 28))(a2, *(s32 *)v3, *(s32 *)(v3 + 4));
            v3 = (u8 *)*(s32 *)(v3 + 44);
        } while (v3 != 0);
    }
    return (s32)a1;
}

s32 func_0019A760(s32 a1, s32 a2, s32 a3)
{
s32 v5;  ((u32)v5) = *(u32 *)(a1 + 12); if ( (u32)v5 != 0 ) { do { (*(void ( **)(s32, s32, u64, u64))(v5 + 32))(a2, a3, *(s32 *)v5, *(s32 *)(v5 + 4)); v5 = *(s32 *)(v5 + 40); } while ( v5 != 0 ); } return a1;
}

s32 func_0019A7D0(s32 a1, s32 a2)
{

    s32 *p = *(s32 **)(a1 + 0x30);
    s32 v;

    if (p[3] != 0)
    {
        v = p[1];
        p[3] = 0;
        p[0] = v;
        p[4] = 0;
    }
    return ((s32 (*)(s32, s32))D_003732F8[0])(a2, a1);
}

s32 func_0019A818(s32 value) { return value; }

s32 func_0019A820(s32 value) { return value; }

s32 func_0019A828(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_0019A830);

/* Byte-matching.  Unlinks the node from its doubly linked list.
   +8 is the next pointer, +0xC the previous one. */
void func_0019A890(u8 *arg0)
{
    if (*(u32 *)(arg0 + 4) == 0)
        return;
    *(u32 *)(*(u32 *)(arg0 + 0xC)) = *(u32 *)(arg0 + 8);
    *(u32 *)(*(u32 *)(arg0 + 8) + 4) = *(u32 *)(arg0 + 0xC);
}

INCLUDE_ASM(const s32, "core/main", func_0019A8C0);

INCLUDE_ASM(const s32, "core/main", func_0019AA20);

INCLUDE_ASM(const s32, "core/main", func_0019ABB0);

f32 func_0019AC80(f32 *v)
{
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/* Byte-matching.  2-D vector length.  Declaring y before x is what makes
   cc1 allocate $f0 to v[0] and schedule both loads before both muls. */
f32 func_0019ACA8(f32 *v)
{
    f32 y = v[1];
    f32 x = v[0];
    return sqrtf(x * x + y * y);
}

INCLUDE_ASM(const s32, "core/main", func_0019ACC8);

extern s32 D_00393D40;

void func_0019AD80(void)
{
    s32 *p = (s32 *)(D_00393D40 + (u32)D_003731B8);
    s32 (*handler)(void) = (s32 (*)(void))p[2];

    handler();
}

extern s32 D_00393D40;

void func_0019ADB0(void)
{
    s32 *p = (s32 *)(D_00393D40 + (u32)D_003731B8);
    s32 (*handler)(void) = (s32 (*)(void))p[3];

    handler();
}

INCLUDE_ASM(const s32, "core/main", func_0019ADE0);

INCLUDE_ASM(const s32, "core/main", func_0019AE30);

INCLUDE_ASM(const s32, "core/main", func_0019AE88);

INCLUDE_ASM(const s32, "core/main", func_0019AF18);

INCLUDE_ASM(const s32, "core/main", func_0019AF90);

s32 func_0019B028(s32 a1, s32 a2)
{


    s32 *p;
    if (a1 == 0)
        a1 = (s32)func_0019B060;
    p = (s32 *)((u8 *)D_003731B8 + D_00393D40);
    p[2] = a1;
    if (a2 == 0)
        a2 = (s32)func_0019B118;
    p[3] = a2;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0019B060);

INCLUDE_ASM(const s32, "core/main", func_0019B118);

INCLUDE_ASM(const s32, "core/main", func_0019B1B8);

void func_0019B2A8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0019B2B0);

INCLUDE_ASM(const s32, "core/main", func_0019B320);

INCLUDE_ASM(const s32, "core/main", func_0019B368);

s8 *func_0019B3B0(s8 *s, s8 c)
{
    s8 *r = 0;

    for (;;)
    {
        s8 ch = *s;
        if (ch == c) { r = s; break; }
        if (ch == 0) break;
        s++;
    }
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_0019B3E8);

INCLUDE_ASM(const s32, "core/main", func_0019B420);

INCLUDE_ASM(const s32, "core/main", func_0019B598);

INCLUDE_ASM(const s32, "core/main", func_0019B920);

INCLUDE_ASM(const s32, "core/main", func_0019BB68);

INCLUDE_ASM(const s32, "core/main", func_0019BCA0);

INCLUDE_ASM(const s32, "core/main", func_0019BDC0);

INCLUDE_ASM(const s32, "core/main", func_0019BE48);

INCLUDE_ASM(const s32, "core/main", func_0019C050);

INCLUDE_ASM(const s32, "core/main", func_0019C0E8);

extern s32 D_003937A8;
extern s32 D_003731B8[];

s32 func_0019C198(void)
{
    s32 *p = (s32 *)(D_003937A8 + (u8 *)D_003731B8);
    if (p[0x13] == 0)
        return 0;
    func_00224B88((s32)(D_003937A8 + ((u8 *)D_003731B8 + 0x48)), 0, 0x3C);
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0019C1E8);

INCLUDE_ASM(const s32, "core/main", func_0019C2E0);

s32 func_0019C3F8(void)
{
    s32 index = D_003937A8;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x00);
}

INCLUDE_ASM(const s32, "core/main", func_0019C410);

s32 func_0019C4F0(void)
{
    s32 index = D_003937A8;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x18);
}

INCLUDE_ASM(const s32, "core/main", func_0019C508);

INCLUDE_ASM(const s32, "core/main", func_0019C5F0);

extern s32 D_003937B0;
extern s32 func_001A3A88();

s32 func_0019C648(s32 arg0, s32 arg1)
{
    s32 r = arg0;
    D_003937B0 = arg1;
    if (func_001A3A88() == 0)
        r = 0;
    return r;
}

s32 func_0019C678();
s32 func_001A3AF0();

s32 func_0019C678(s32 arg1)
{
    func_001A3AF0(arg1);
    return arg1;
}

s32 func_0019C6A0(void) { return 1; }

void func_0019C6A8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0019C6B0);

INCLUDE_ASM(const s32, "core/main", func_0019CDA0);

INCLUDE_ASM(const s32, "core/main", func_0019D480);

extern u8 D_003772F0[];

u8 *func_0019D5A0(void) { return D_003772F0; }

INCLUDE_ASM(const s32, "core/main", func_0019D5B0);

INCLUDE_ASM(const s32, "core/main", func_0019DF58);

INCLUDE_ASM(const s32, "core/main", func_0019E8E8);

extern u8 D_00377400[];

u8 *func_0019EA08(void) { return D_00377400; }

INCLUDE_ASM(const s32, "core/main", func_0019EA18);

INCLUDE_ASM(const s32, "core/main", func_0019ECD0);

extern u8 D_003774F8[];

u8 *func_0019F1C0(void) { return D_003774F8; }

INCLUDE_ASM(const s32, "core/main", func_0019F1D0);

INCLUDE_ASM(const s32, "core/main", func_0019F240);

extern u8 D_003775B0[];

u8 *func_0019F600(void) { return D_003775B0; }

INCLUDE_ASM(const s32, "core/main", func_0019F610);

extern u8 D_00377678[];

u8 *func_0019F840(void) { return D_00377678; }

INCLUDE_ASM(const s32, "core/main", func_0019F850);

extern u8 D_00377740[];

u8 *func_0019FB68(void) { return D_00377740; }

INCLUDE_ASM(const s32, "core/main", func_0019FB78);

extern u8 D_00377838[];

u8 *func_0019FEC8(void) { return D_00377838; }

INCLUDE_ASM(const s32, "core/main", func_0019FED8);


/* Source: engine/region_001A.c */
/* func_001A00C8 through func_001AFE40; retail address range 0x001A0000-0x001B0000. */



extern s32 D_00393824;

extern u8 D_003778E0[];

u8 *func_001A00C8(void) { return D_003778E0; }

INCLUDE_ASM(const s32, "core/main", func_001A00D8);

extern u8 D_003779A0[];

u8 *func_001A0390(void) { return D_003779A0; }

INCLUDE_ASM(const s32, "core/main", func_001A03A0);

extern u8 D_00377A68[];

u8 *func_001A06A0(void) { return D_00377A68; }

INCLUDE_ASM(const s32, "core/main", func_001A06B0);

INCLUDE_ASM(const s32, "core/main", func_001A0D00);

extern u8 D_00377B50[];

u8 *func_001A1350(void) { return D_00377B50; }

s32 func_001A1360(s32 arg0)
{


    u8 *p = (u8 *)D_003731B8[0];
    if (*(s32 *)(p + 0x14) != 1)
        return func_001A06B0(arg0, (s32)p);
    return func_001A0D00(arg0, (s32)p);
}

INCLUDE_ASM(const s32, "core/main", func_001A13A8);

INCLUDE_ASM(const s32, "core/main", func_001A1548);

INCLUDE_ASM(const s32, "core/main", func_001A1698);

INCLUDE_ASM(const s32, "core/main", func_001A1738);

INCLUDE_ASM(const s32, "core/main", func_001A19B8);

INCLUDE_ASM(const s32, "core/main", func_001A1C08);

INCLUDE_ASM(const s32, "core/main", func_001A1E58);

INCLUDE_ASM(const s32, "core/main", func_001A2070);

INCLUDE_ASM(const s32, "core/main", func_001A2298);

INCLUDE_ASM(const s32, "core/main", func_001A24F0);

INCLUDE_ASM(const s32, "core/main", func_001A2748);

INCLUDE_ASM(const s32, "core/main", func_001A2968);

INCLUDE_ASM(const s32, "core/main", func_001A2B90);

/* Byte-matching.  Picks one of two handlers from the third argument and calls
   it with the first two as they arrived. */
void func_001A2DA0(s32 arg0, s32 arg1, s32 arg2)
{


    void (*fn)();

    if (arg2 != 0)
        fn = func_001A1738;
    else
        fn = func_001A2B90;
    fn();
}

INCLUDE_ASM(const s32, "core/main", func_001A2DD8);

INCLUDE_ASM(const s32, "core/main", func_001A2FA8);

INCLUDE_ASM(const s32, "core/main", func_001A3188);

INCLUDE_ASM(const s32, "core/main", func_001A3378);

INCLUDE_ASM(const s32, "core/main", func_001A3558);

INCLUDE_ASM(const s32, "core/main", func_001A3740);

INCLUDE_ASM(const s32, "core/main", func_001A38C0);

INCLUDE_ASM(const s32, "core/main", func_001A3A88);

extern s32 D_00393820;
extern s32 D_00393824;

s32 func_001A3AF0(void)
{
    if (D_00393820 != 0) {
        func_001A8B78(D_00393824);
        D_00393820 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001A3B20);

s32 func_001A3B98(void) { return D_00393824; }

INCLUDE_ASM(const s32, "core/main", func_001A3BA0);

s32 func_001A3C60(u32 *arg0)
{

    u32 *p = (u32 *)D_00377B90[0];
    u32 *v;
    u32 a, b;

    p[5] = 2;
    v = (u32 *)p[6];
    if (v != 0) {
        a = arg0[6];
        b = arg0[3];
        v[2] = a;
        v[4] = b;
        v[3] = arg0[4];
        return (s32)v;
    }
    return 0;
}

/* Byte-matching.  Rejects a null object and anything that is not a nonzero
   u16, otherwise stores the value at +2 and returns the object. */
u8 *func_001A3CA8(u8 *arg0, u32 arg1)
{
    if (arg0 != 0 && arg1 - 1 <= 0xFFFE) {
        *(u16 *)(arg0 + 2) = arg1;
        return arg0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001A3CD0);

INCLUDE_ASM(const s32, "core/main", func_001A3D90);

INCLUDE_ASM(const s32, "core/main", func_001A3E50);

s32 func_001A3F20(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{

    s32 v10;
    s32 v11;
    u64 v13;
    s16 v14;
    s32 v15;
    v10 = func_001A8C18(D_00393824, *(s32 *)(a1 + 4), (u32)a2 * *(u16 *)(a1 + 2), 1, a5, a6, a7, a8);
    v11 = v10;
    if (v10 == 0)
        return 0;
    v13 = *(s32 *)(a1 + 16);
    v14 = *(s16 *)a1 | 1;
    *(u32 *)(a1 + 4) = v11;
    *(s16 *)a1 = v14;
    *(u32 *)(a1 + 8) = v11;
    if (a2 < v13)
        *(u32 *)(a1 + 16) = a2;
    v15 = *(u32 *)(a1 + 4);
    *(u32 *)(a1 + 12) = a2;
    *(u32 *)(a1 + 8) = v15;
    return a1;
}

INCLUDE_ASM(const s32, "core/main", func_001A3FB0);

void func_001A4000(void) {
}

u32 func_001A4008(u8 *arg0)
{
    u8 *p;
    s32 (*handler)(void);

    if (arg0 != 0) {
        p = *(u8 **)arg0;
        handler = *(s32 (**)(void))(p + 0x1C);
        if (handler != 0)
            return (u32)handler();
        return 0;
    }
    return 0xFFFFFFFFu;
}

INCLUDE_ASM(const s32, "core/main", func_001A4048);

INCLUDE_ASM(const s32, "core/main", func_001A4130);

s32 func_001A4180(u8 *p)
{
    if (p == 0)
        return -1;
    return *(s32 *)(p + 0x4);
}

u8 *func_001A4198(u8 *p, s32 value)
{
    u8 *r = 0;

    if (p != 0)
    {
        *(s32 *)(p + 0x4) = value;
        r = p;
    }
    return r;
}

s32 func_001A41B0(u8 *p)
{
    if (p == 0)
        return -1;
    return *(s32 *)(p + 0x18);
}

u8 *func_001A41C8(u8 *p, s32 value)
{
    u8 *r = 0;

    if (p != 0)
    {
        *(s32 *)(p + 0x18) = value;
        r = p;
    }
    return r;
}

/* Behavior-faithful, not byte-matching: paces itself for *(u32*)(arg0+4)
   iterations and does nothing else.  Size exact at 0x3C; retail's loop label
   sits one slot later, which moves the four alignment nops. */
void func_001A41E0(u8 *arg0)
{
    u32 n;
    u32 i;

    if (arg0 == 0)
        return;
    n = *(u32 *)(arg0 + 4);
    if (n == 0)
        return;
    i = 0;
    do {
        i++;
    } while (i < n);
}

INCLUDE_ASM(const s32, "core/main", func_001A4220);

INCLUDE_ASM(const s32, "core/main", func_001A45F8);

INCLUDE_ASM(const s32, "core/main", func_001A47B8);

INCLUDE_ASM(const s32, "core/main", func_001A49B8);

INCLUDE_ASM(const s32, "core/main", func_001A4C40);

INCLUDE_ASM(const s32, "core/main", func_001A4FE0);

INCLUDE_ASM(const s32, "core/main", func_001A51E0);

INCLUDE_ASM(const s32, "core/main", func_001A52D8);

INCLUDE_ASM(const s32, "core/main", func_001A5660);

INCLUDE_ASM(const s32, "core/main", func_001A57E0);

INCLUDE_ASM(const s32, "core/main", func_001A5DE0);

/* Byte-matching.  Bounds-checked element of the array at offset 8. */
u32 *func_001A5E80(u32 *p, u32 i)
{
    if (p != 0 && p[0] != 0 && i < p[1])
        return (u32 *)(p[2] + i * 4);
    return 0;
}

u8 *func_001A5EB8(u8 *p)
{
    if (p != 0 && *(s32 *)p != 0)
        return p;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001A5ED0);

INCLUDE_ASM(const s32, "core/main", func_001A5F70);

INCLUDE_ASM(const s32, "core/main", func_001A6028);

s32 func_001A6108(s32 *p) { return *(s32 *)((u8 *)p[7] + 0xC); }

INCLUDE_ASM(const s32, "core/main", func_001A6118);

INCLUDE_ASM(const s32, "core/main", func_001A61C8);

INCLUDE_ASM(const s32, "core/main", func_001A6278);

/* Byte-matching.  Indexed accessor: arg1 must be below the count at +4, and
   entries are 0x20 bytes each from the base pointer at +8. */
u8 *func_001A6370(u8 *arg0, u32 arg1)
{
    if (arg0 != 0 && arg1 < *(u32 *)(arg0 + 4))
        return *(u8 **)(arg0 + 8) + (arg1 << 5);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001A63A0);

INCLUDE_ASM(const s32, "core/main", func_001A6510);

INCLUDE_ASM(const s32, "core/main", func_001A6588);

INCLUDE_ASM(const s32, "core/main", func_001A65E0);

INCLUDE_ASM(const s32, "core/main", func_001A6670);

INCLUDE_ASM(const s32, "core/main", func_001A66C8);

INCLUDE_ASM(const s32, "core/main", func_001A68D0);

INCLUDE_ASM(const s32, "core/main", func_001A6F18);

INCLUDE_ASM(const s32, "core/main", func_001A73A0);

INCLUDE_ASM(const s32, "core/main", func_001A76D0);

INCLUDE_ASM(const s32, "core/main", func_001A7770);

INCLUDE_ASM(const s32, "core/main", func_001A7CD0);

void func_001A7EF0(u8 *arg0, s32 arg1, u8 **arg2)
{
    s32 *entry = (s32 *)*arg2;
    s32 value = *(s32 *)(arg0 + 8);
    entry[0] = (s32)arg0;
    entry[1] = value;
    *arg2 = (u8 *)(entry + 2);
}

INCLUDE_ASM(const s32, "core/main", func_001A7F10);

INCLUDE_ASM(const s32, "core/main", func_001A8188);

INCLUDE_ASM(const s32, "core/main", func_001A8320);

INCLUDE_ASM(const s32, "core/main", func_001A8548);

INCLUDE_ASM(const s32, "core/main", func_001A8740);

INCLUDE_ASM(const s32, "core/main", func_001A8A80);

INCLUDE_ASM(const s32, "core/main", func_001A8B78);

INCLUDE_ASM(const s32, "core/main", func_001A8C18);

INCLUDE_ASM(const s32, "core/main", func_001A8DA0);

INCLUDE_ASM(const s32, "core/main", func_001A8F00);

INCLUDE_ASM(const s32, "core/main", func_001A90A8);

INCLUDE_ASM(const s32, "core/main", func_001A9108);

s32 func_001A9240(s32 a1)
{



    s32 stack[2];

    if (a1 != 0)
        return ((s32 (*)(s32))D_003732E8[0])(a1);
    stack[0] = 1;
    stack[1] = func_0018DA08(0x80000016);
    return func_0018DAD0(stack);
}

INCLUDE_ASM(const s32, "core/main", func_001A9290);

INCLUDE_ASM(const s32, "core/main", func_001A95B0);

s32 func_001A9670(void) { return 1; }

s32 func_001A9678(void) { return 1; }

void func_001A9680(void) {
}

void func_001A9688(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001A9690);

INCLUDE_ASM(const s32, "core/main", func_001A97A0);

INCLUDE_ASM(const s32, "core/main", func_001A9A78);

INCLUDE_ASM(const s32, "core/main", func_001A9CB8);

INCLUDE_ASM(const s32, "core/main", func_001A9F20);

INCLUDE_ASM(const s32, "core/main", func_001AA418);

INCLUDE_ASM(const s32, "core/main", func_001AA808);

INCLUDE_ASM(const s32, "core/main", func_001AAAB0);

INCLUDE_ASM(const s32, "core/main", func_001AAFC8);

INCLUDE_ASM(const s32, "core/main", func_001AB1C8);

INCLUDE_ASM(const s32, "core/main", func_001AB6E0);

INCLUDE_ASM(const s32, "core/main", func_001AB850);

INCLUDE_ASM(const s32, "core/main", func_001AB960);

INCLUDE_ASM(const s32, "core/main", func_001ABA90);

void func_001ABBC0(void)
{
    while (*(volatile s32 *)&D_0039384C) {
    }
}

INCLUDE_ASM(const s32, "core/main", func_001ABBE8);

void func_001ABD08(s32 arg0)
{




    s32 cap = D_0039386C;
    s32 i = D_00393870;

    if (i == cap)
    {
        s32 (*f)(s32, s32) = (s32 (*)(s32, s32))D_003732EC[0];
        s32 r = f(D_00393868, i * 8);
        if (r == 0)
            return;
        D_00393868 = r;
        D_0039386C = D_0039386C * 2;
        i = D_00393870;
    }
    *(s32 *)(D_00393868 + i * 4) = arg0;
    D_00393870 = i + 1;
}

INCLUDE_ASM(const s32, "core/main", func_001ABD80);

s32 func_001ABE38(s32 a1, s32 a2, s32 a3)
{

    if (D_00393D70 != 0) {
        func_001C28C8((s32)D_0038C848);
        return 0;
    }
    D_00393894 = a2;
    D_00393890 = a1;
    D_00393898 = a3;
    if (a3 != 0 && a1 == 1) {
        func_001C28C8((s32)D_0038C868);
        D_00393898 = 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001ABE98);

s32 func_001ABEE0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_001ABEE8);

void func_001ABF80(void)
{
    func_001C6EE8();
    func_001C6BB8();
    func_001C6900();
    func_001C6460();
    func_001C5FB0();
    func_001BEDE0();
    func_001C7330(1);
}

INCLUDE_ASM(const s32, "core/main", func_001ABFC8);

INCLUDE_ASM(const s32, "core/main", func_001ACA38);

INCLUDE_ASM(const s32, "core/main", func_001ACD88);

INCLUDE_ASM(const s32, "core/main", func_001ACF88);

INCLUDE_ASM(const s32, "core/main", func_001AD8D8);

INCLUDE_ASM(const s32, "core/main", func_001ADC90);

INCLUDE_ASM(const s32, "core/main", func_001AE020);

INCLUDE_ASM(const s32, "core/main", func_001AEE90);

INCLUDE_ASM(const s32, "core/main", func_001AF1A0);

INCLUDE_ASM(const s32, "core/main", func_001AF2E0);

INCLUDE_ASM(const s32, "core/main", func_001AF700);

INCLUDE_ASM(const s32, "core/main", func_001AF870);

INCLUDE_ASM(const s32, "core/main", func_001AFA30);

INCLUDE_ASM(const s32, "core/main", func_001AFB40);

INCLUDE_ASM(const s32, "core/main", func_001AFD08);

INCLUDE_ASM(const s32, "core/main", func_001AFE40);


/* Source: engine/region_001B.c */
/* func_001B0108 through func_001BFE20; retail address range 0x001B0000-0x001C0000. */




extern s32 D_00393968;
extern s32 D_003E358C[8];

extern s32 D_003939C0;

INCLUDE_ASM(const s32, "core/main", func_001B0108);

INCLUDE_ASM(const s32, "core/main", func_001B0CC0);

extern u8 D_00378490[];

u8 *func_001B1040(void) { return D_00378490; }

/* Behavior-faithful, not byte-matching: same bit-length loop with the same -1
   seed and the same size 0x2C, but the increment sits in the loop body here
   where retail has it in the bnez delay slot. */
s32 func_001B1050(u32 arg0)
{
    s32 n = -1;

    if (arg0 != 0) {
        do {
            arg0 >>= 1;
            n++;
        } while (arg0 != 0);
    }
    return n;
}

extern s32 D_00393D8C;
extern s32 D_00393970;

s32 func_001B1080(s32 a1)
{
    s32 v = 0;

    if (D_00393D8C == 0 &&
        (a1 == 0x100 || a1 == 0x140 || a1 == 0x180 || a1 == 0x200 || a1 == 0x280))
        v = 1;
    if (v != 0)
        D_00393970 = a1;
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_001B10D0);

f32 func_001B1128(f32 arg0)
{

    s32 v = func_0021F7A8(arg0 * 16.0f);

    if (v < -2048)
        v = -2048;
    else if (v >= 2048)
        v = 2047;
    D_00393968 |= v & 0xFFF;
    return (f32)v * 0.0625f;
}

void func_001B1190(u32 arg0)
{
    u32 value = arg0;

    if (arg0 >= 4)
        value = 3;
    D_00393968 |= value << 12;
}

extern s32 D_00393968;

f32 func_001B11B0(void)
{
    s32 x = D_00393968;
    s32 v = x & 0xFFF;

    if (x & 0x800)
        v |= -0x1000;
    return (f32)v * 0.0625f;
}

s32 func_001B11E0(void) { return (D_00393968 & 0x3000) >> 12; }

INCLUDE_ASM(const s32, "core/main", func_001B11F0);

INCLUDE_ASM(const s32, "core/main", func_001B1288);

f32 func_001B12D0(u8 *a1)
{
    s32 v1, v2;

    if (a1 == 0)
        goto zero;
    v1 = *(s32 *)a1;
    if (v1 == 0)
        goto zero;
    v2 = *(u16 *)((u8 *)(v1 + D_00393990) + 0x14) & 0xFFF;
    if (v2 & 0x800)
        v2 |= -0x1000;
    return (f32)v2 * 0.0625f;
zero:
    return 0.0f;
}

extern s32 D_00393990;

s32 func_001B1328(void *arg0)
{
    if (arg0 != 0)
    {
        u32 q = *(u32 *)arg0;
        if (q != 0)
        {
            u8 *p = (u8 *)(q + D_00393990);
            u16 v = *(u16 *)(p + 0x14);
            return (v & 0x3000) >> 12;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001B1358);

INCLUDE_ASM(const s32, "core/main", func_001B13A0);

INCLUDE_ASM(const s32, "core/main", func_001B1450);

INCLUDE_ASM(const s32, "core/main", func_001B14C8);

s32 func_001B1520(void) { return 4; }

INCLUDE_ASM(const s32, "core/main", func_001B1528);

INCLUDE_ASM(const s32, "core/main", func_001B15D8);

s32 func_001B1638(s32 *p, s32 value) { p[0] = value; return 1; }

INCLUDE_ASM(const s32, "core/main", func_001B1648);

INCLUDE_ASM(const s32, "core/main", func_001B1720);

INCLUDE_ASM(const s32, "core/main", func_001B17C0);

INCLUDE_ASM(const s32, "core/main", func_001B1818);

INCLUDE_ASM(const s32, "core/main", func_001B1878);

s32 func_001B1908(void) { return 1; }

s32 func_001B1910(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_001B1918);

INCLUDE_ASM(const s32, "core/main", func_001B19B8);

INCLUDE_ASM(const s32, "core/main", func_001B1A48);

INCLUDE_ASM(const s32, "core/main", func_001B1AD0);

INCLUDE_ASM(const s32, "core/main", func_001B1CC8);

INCLUDE_ASM(const s32, "core/main", func_001B1EC8);

INCLUDE_ASM(const s32, "core/main", func_001B1F48);

INCLUDE_ASM(const s32, "core/main", func_001B2030);

INCLUDE_ASM(const s32, "core/main", func_001B20B0);

INCLUDE_ASM(const s32, "core/main", func_001B21C8);

INCLUDE_ASM(const s32, "core/main", func_001B22B0);

INCLUDE_ASM(const s32, "core/main", func_001B22F8);

INCLUDE_ASM(const s32, "core/main", func_001B2518);

INCLUDE_ASM(const s32, "core/main", func_001B2660);

INCLUDE_ASM(const s32, "core/main", func_001B2948);

INCLUDE_ASM(const s32, "core/main", func_001B2A88);

INCLUDE_ASM(const s32, "core/main", func_001B2C70);

INCLUDE_ASM(const s32, "core/main", func_001B2EF8);

INCLUDE_ASM(const s32, "core/main", func_001B3488);

INCLUDE_ASM(const s32, "core/main", func_001B3AF0);

INCLUDE_ASM(const s32, "core/main", func_001B3F20);

extern s32 D_00393DA0;
extern s32 D_00393DA4;
void func_001B3F98(void)
{
    if (D_00393DA0 != 0) {
        func_00195CE0(D_00393DA0);
        D_00393DA0 = 0;
    }
    if (D_00393DA4 != 0) {
        func_00195CE0(D_00393DA4);
        D_00393DA4 = 0;
    }
}

extern s32 D_00393D98;

s32 func_001B3FD8(void)
{
    s32 stack[4];
    s32 p = D_00393D98;
    s32 result = 0;

    if (p != 0) {
        stack[1] = 0;
        stack[0] = 0;
        stack[2] = *(s32 *)(p + 0xC);
        stack[3] = *(s32 *)(p + 0x10);
        result = func_001B2660(0, stack);
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001B4020);

INCLUDE_ASM(const s32, "core/main", func_001B4058);

INCLUDE_ASM(const s32, "core/main", func_001B4AD0);

INCLUDE_ASM(const s32, "core/main", func_001B4CF0);

INCLUDE_ASM(const s32, "core/main", func_001B4EE0);

INCLUDE_ASM(const s32, "core/main", func_001B5370);

INCLUDE_ASM(const s32, "core/main", func_001B5488);

INCLUDE_ASM(const s32, "core/main", func_001B5580);

INCLUDE_ASM(const s32, "core/main", func_001B5670);

INCLUDE_ASM(const s32, "core/main", func_001B57B0);

INCLUDE_ASM(const s32, "core/main", func_001B5978);

INCLUDE_ASM(const s32, "core/main", func_001B5CC0);

INCLUDE_ASM(const s32, "core/main", func_001B6010);

INCLUDE_ASM(const s32, "core/main", func_001B6128);

INCLUDE_ASM(const s32, "core/main", func_001B62A0);

INCLUDE_ASM(const s32, "core/main", func_001B6468);

INCLUDE_ASM(const s32, "core/main", func_001B6F50);

INCLUDE_ASM(const s32, "core/main", func_001B7AB0);

extern s32 func_001B7FF0();
extern s32 func_001B8048();
extern s32 func_001B80A8();
extern s32 func_001B80F8();
extern s32 func_001B7AB0();

void func_001B7DD0(void)
{
    D_003731B8[10] = (s32)func_001B7FF0;
    D_003731B8[11] = (s32)func_001B8048;
    D_003731B8[12] = (s32)func_001B80A8;
    D_003731B8[13] = (s32)func_001B80F8;
    func_001B7AB0();
}

INCLUDE_ASM(const s32, "core/main", func_001B7E10);

void func_001B7FE8(void) {
}

extern u8 D_003939B8;
extern s32 D_0038D150[];
extern s32 D_0038D140[];

s32 func_001B7FF0(s32 a1, s32 a2, s32 a3, s32 a4)
{

    s32 v1 = a1;
    s32 v = (D_003939B8 & 1) ? (s32)&D_0038D150 : (s32)&D_0038D140;

    func_001B6128(v, 0, v1 + (a3 << 6), v1 + (a4 << 6));
    return 1;
}

extern u8 D_003939B8;
extern s32 D_0038D150[];
extern s32 D_0038D140[];

s32 func_001B8048(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    s32 v1 = a1;
    s32 v = (D_003939B8 & 1) ? (s32)&D_0038D150 : (s32)&D_0038D140;

    func_001B62A0(v, 0, v1 + (a3 << 6), v1 + (a4 << 6), v1 + (a5 << 6));
    return 1;
}

extern u8 D_003939B8;
extern s32 D_0038D150[];
extern s32 D_0038D140[];

s32 func_001B80A8(s32 a1, s32 a2, s32 a3)
{

    s32 v1 = a1;
    s32 v3 = a3;
    s32 v = (D_003939B8 & 1) ? (s32)&D_0038D150 : (s32)&D_0038D140;

    func_001B6468(v, 0, v1, a2, v3);
    return 1;
}

extern u8 D_003939B8;
extern s32 D_0038D150[];
extern s32 D_0038D140[];

s32 func_001B80F8(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    s32 v1 = a1;
    s32 v4 = a4;
    s32 v5 = a5;
    s32 v = (D_003939B8 & 1) ? (s32)&D_0038D150 : (s32)&D_0038D140;

    func_001B6F50(v, 0, v1, a2, v4, v5);
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001B8150);

INCLUDE_ASM(const s32, "core/main", func_001B8438);

INCLUDE_ASM(const s32, "core/main", func_001B86F8);

INCLUDE_ASM(const s32, "core/main", func_001B89A8);

INCLUDE_ASM(const s32, "core/main", func_001B8DE8);

INCLUDE_ASM(const s32, "core/main", func_001B8FB8);

INCLUDE_ASM(const s32, "core/main", func_001B9408);

INCLUDE_ASM(const s32, "core/main", func_001B9580);

INCLUDE_ASM(const s32, "core/main", func_001B9788);

INCLUDE_ASM(const s32, "core/main", func_001B9990);

INCLUDE_ASM(const s32, "core/main", func_001B9B08);

INCLUDE_ASM(const s32, "core/main", func_001B9EA8);

INCLUDE_ASM(const s32, "core/main", func_001BA128);

extern s32 D_003E3590[];

void func_001BA1E8(s32 off)
{

    s32 *p = (s32 *)(off + D_00393990);

    if (D_003E3590[0] != 0)
    {
        s32 v = p[0x16];
        if (v != 0)
            func_001BA128(v);
    }
}

INCLUDE_ASM(const s32, "core/main", func_001BA218);

INCLUDE_ASM(const s32, "core/main", func_001BA2B0);

s32 func_001BA370(s32 value) { D_003E358C[0] = value; return 1; }

INCLUDE_ASM(const s32, "core/main", func_001BA380);

INCLUDE_ASM(const s32, "core/main", func_001BA430);

INCLUDE_ASM(const s32, "core/main", func_001BA498);

s32 func_001BA548(s32 index)
{
    return *(s32 *)(index + D_00393990 + 8) & 0x1FFF;
}

s32 func_001BA560(s32 off, s32 a, s32 b)
{
    s32 *p = (s32 *)(off + D_00393990);
    if (p != 0) {
        p[2] = b;
        p[3] = a;
    }
    return off;
}

s32 func_001BA580(s32 off, s32 *a, s32 *b)
{
    s32 *p = (s32 *)(off + D_00393990);
    if (p != 0) {
        *a = p[3];
        *b = p[2];
    }
    return off;
}

s32 func_001BA5A8(s32 off, s32 a, s32 b)
{
    s32 *p = (s32 *)(off + D_00393990);
    if (p != 0) {
        p[6] = b;
        p[7] = a;
    }
    return off;
}

s32 func_001BA5C8(s32 off, s32 *a, s32 *b)
{
    s32 *p = (s32 *)(off + D_00393990);
    if (p != 0) {
        *a = p[7];
        *b = p[6];
    }
    return off;
}

s32 func_001BA5F0(void) { return D_003939C0; }

s32 func_001BA5F8(void)
{
    if (D_003938E4 == 0) {
        D_003939C4 = 0;
        return 1;
    }
    return 0;
}

/* Behavior-faithful, not byte-matching: routes on the low nibble of arg2 and
   always reports 1, size exact at 0x38; gcc keeps the masked value in $6 where
   retail uses $2. */
s32 func_001BA618(s32 arg0, s32 arg1, s32 arg2)
{



    if ((arg2 & 0xF) == 0)
        func_001B86F8(arg0, arg1, arg2);
    else
        func_001B89A8(arg0, arg1, arg2);
    return 1;
}

extern s32 D_003939C4;
extern s32 D_00393990;

s32 func_001BA650(s32 off, u32 arg1)
{
    if (D_003939C4 == 0)
    {
        u8 *p = (u8 *)(off + D_00393990);
        if (p != 0)
        {
            *(u32 *)(p + 0x58) = (arg1 != 0);
            return off;
        }
    }
    return 0;
}

extern u8 D_003E3500[];

s32 func_001BA680(s32 arg0)
{
    u8 *p = (u8 *)D_003E3500;
    if (*(u32 *)(p + 0x90) == 0 && arg0 != 0)
    {
        *(u32 *)(p + 0x94) = arg0;
        return 1;
    }
    return 0;
}

void func_001BA6B0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001BA6B8);

INCLUDE_ASM(const s32, "core/main", func_001BA830);

INCLUDE_ASM(const s32, "core/main", func_001BA9F0);

INCLUDE_ASM(const s32, "core/main", func_001BABA8);

INCLUDE_ASM(const s32, "core/main", func_001BAD80);

INCLUDE_ASM(const s32, "core/main", func_001BAF60);

INCLUDE_ASM(const s32, "core/main", func_001BB120);

INCLUDE_ASM(const s32, "core/main", func_001BB210);

INCLUDE_ASM(const s32, "core/main", func_001BB2B8);

INCLUDE_ASM(const s32, "core/main", func_001BB3B8);

INCLUDE_ASM(const s32, "core/main", func_001BB4A0);

INCLUDE_ASM(const s32, "core/main", func_001BB580);

INCLUDE_ASM(const s32, "core/main", func_001BB660);

INCLUDE_ASM(const s32, "core/main", func_001BB768);

INCLUDE_ASM(const s32, "core/main", func_001BB868);

INCLUDE_ASM(const s32, "core/main", func_001BB9E0);

INCLUDE_ASM(const s32, "core/main", func_001BBB60);

INCLUDE_ASM(const s32, "core/main", func_001BBCE0);

INCLUDE_ASM(const s32, "core/main", func_001BBE68);

INCLUDE_ASM(const s32, "core/main", func_001BBFE0);

INCLUDE_ASM(const s32, "core/main", func_001BC648);

INCLUDE_ASM(const s32, "core/main", func_001BC7A8);

INCLUDE_ASM(const s32, "core/main", func_001BC9E8);

INCLUDE_ASM(const s32, "core/main", func_001BD070);

INCLUDE_ASM(const s32, "core/main", func_001BD208);

INCLUDE_ASM(const s32, "core/main", func_001BD680);

INCLUDE_ASM(const s32, "core/main", func_001BD978);

INCLUDE_ASM(const s32, "core/main", func_001BDA88);

INCLUDE_ASM(const s32, "core/main", func_001BDB38);

INCLUDE_ASM(const s32, "core/main", func_001BDD18);

s32 func_001BDD90(s32 a, u64 b, s32 n)
{
    s32 idx;
    s32 r;
    if (n != 0) goto rec;
    r = ((u8 *)a)[0x18];
    goto done;
rec:
    idx = b & 0xF;
    r = func_001BDD90(*(s32 *)((u32)a + (idx << 2)), b >> 4, n - 1);
done:
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001BDDD8);

s32 func_001BDE78(s8 *a1)
{

    s32 v = 0;

    if (a1[0] == 0x5C || (D_00391541[a1[0]] & 3) != 0 && a1[1] == 0x3A)
        v = 1;
    return v;
}

/* Byte-matching: s64 zero record + two function-pointer calls. */
s32 func_001BDEC0(s32 arg0, s32 arg1)
{

    s64 v6 = 0;
    s32 v4;
    s32 v;

    v4 = func_002220E8(&v6);
    if (v4 != 0) {
        v = ((s32 (*)(s32))D_003731B8[70])(v4);
        if ((u32)v < (u32)(arg1 - 1)) {
            ((s32 (*)(s32, s32))D_003731B8[61])(arg0, v4);
            return arg0;
        }
    }
    return 0;
}


u8 *func_001BDF50(u8 *p)
{
    D_00393DC4++;
    return p;
}

u8 *func_001BDF68(u8 *p)
{
    D_00393DC4--;
    return p;
}

extern s32 D_00373218[];
s32 func_001BDF80(s32 a1, u8 *a2)
{
    s32 v4;
    s32 result;
    v4 = ((s32 (*)(s32, s32, s32))D_00373218[0])(a1, (s32)a2, 0) == 0;
    result = 0;
    if (!v4) {
        if ((*(a2 + 34) & 1) != 0) *(s32 *)a1 |= 2;
        return a1;
    }
    return result;
}

extern s32 D_0037321C[];
s32 func_001BDFE8(s32 a1, u8 *a2)
{
    s32 v4;
    s32 result;
    v4 = ((s32 (*)(s32, s32, s32))D_0037321C[0])(a1, (s32)a2, 0) == 0;
    result = 0;
    if (!v4) {
        if ((*(s32 *)a2 & 2) != 0) *(u8 *)(a1 + 0x22) |= 1;
        return a1;
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001BE050);

INCLUDE_ASM(const s32, "core/main", func_001BE168);

s32 func_001BE228(s32 a1, s32 a2)
{
    s32 v2;
    s32 result;
    s32 v4;
    s32 v5;
    s32 v6;

    v2 = func_0018FFB0(a1, a2);
    result = 0;
    if (v2 != 0)
    {
        v4 = func_00190C90(v2);
        v5 = func_00190C98(v2);
        func_00190CA0(v2);
        v6 = func_00195BE8(v4, v5, 0, 0);
        if (v6 == 0)
        {
            func_001907C0(v2);
            return 0;
        }
        else
        {
            if (((s32 (*)(s32, s32, s32))D_0037321C[0])(v6, v2, 0) != 0 &&
                (*(s32 *)v2 & 2) != 0)
                *(u8 *)(v6 + 0x22) |= 1;
            func_001907C0(v2);
            return v6;
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001BE2F8);

s32 func_001BE398(u8 *a1, s32 a2, s32 a3, s32 a4, f32 *a5)
{

    s32 row_start = a2 >> 16;
    s32 row_end   = a3 >> 16;
    f32 span = (f32)(a3 - a2) * (f32)1.52587890625e-05;
    u8 *p = *(u8 **)(a1 + 0x14) + (a4 >> 16) * *(s32 *)(a1 + 0x10) + row_start * 4;
    f32 scale, w;
    s32 next_row;
    u8 *i;
    f32 r, g, b, a;

    if (row_start == row_end) {
        scale = D_00392908;
        a5[0] = (f32)p[0] * scale * span;
        a5[1] = (f32)p[1] * scale * span;
        a5[2] = (f32)p[2] * scale * span;
        a5[3] = (f32)p[3] * scale * span;
    } else {
        next_row = func_0021F7A8((f32)(row_start + 1) * 65536.0f);
        scale = D_0039290C;
        w = (f32)(next_row - a2) * (f32)1.52587890625e-05;
        a5[0] = (f32)p[0] * scale * w;
        a5[1] = (f32)p[1] * scale * w;
        a5[2] = (f32)p[2] * scale * w;
        a5[3] = (f32)p[3] * scale * w;
        for (i = p + 4; (next_row >> 16) != row_end; i += 4) {
            a5[0] += (f32)i[0] * scale;
            a5[1] += (f32)i[1] * scale;
            a5[2] += (f32)i[2] * scale;
            a5[3] += (f32)i[3] * scale;
            next_row += 0x10000;
        }
        w = (f32)(a3 - next_row) * (f32)1.52587890625e-05;
        scale = D_00392910;
        a5[0] += (f32)i[0] * scale * w;
        a5[1] += (f32)i[1] * scale * w;
        a5[2] += (f32)i[2] * scale * w;
        a5[3] += (f32)i[3] * scale * w;
    }
    r = a5[0] / span; g = a5[1] / span; b = a5[2] / span; a = a5[3] / span;
    a5[0] = r; a5[1] = g; a5[2] = b; a5[3] = a;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001BE6F0);

INCLUDE_ASM(const s32, "core/main", func_001BE990);

INCLUDE_ASM(const s32, "core/main", func_001BEB90);

INCLUDE_ASM(const s32, "core/main", func_001BECC0);

extern u8 D_00378A10[];

u8 *func_001BEDD0(void) { return D_00378A10; }

INCLUDE_ASM(const s32, "core/main", func_001BEDE0);

INCLUDE_ASM(const s32, "core/main", func_001BEE48);

INCLUDE_ASM(const s32, "core/main", func_001BEEE0);

INCLUDE_ASM(const s32, "core/main", func_001BF1F8);

INCLUDE_ASM(const s32, "core/main", func_001BF338);

INCLUDE_ASM(const s32, "core/main", func_001BF9C8);

INCLUDE_ASM(const s32, "core/main", func_001BFA50);

INCLUDE_ASM(const s32, "core/main", func_001BFAD8);

INCLUDE_ASM(const s32, "core/main", func_001BFDC0);

INCLUDE_ASM(const s32, "core/main", func_001BFE20);


/* Source: platform/region_001C.c */
/* func_001C0090 through func_001CFFD0; retail address range 0x001C0000-0x001D0000. */



s32 D_00378AC0[8];
u8 D_00379530[8];
s32 D_0038E278[8];

INCLUDE_ASM(const s32, "core/main", func_001C0090);

INCLUDE_ASM(const s32, "core/main", func_001C0150);

INCLUDE_ASM(const s32, "core/main", func_001C0218);

INCLUDE_ASM(const s32, "core/main", func_001C0400);

INCLUDE_ASM(const s32, "core/main", func_001C0508);

INCLUDE_ASM(const s32, "core/main", func_001C05F0);

s32 func_001C0800(void) {
    register s32 syscall_result __asm__("$2");
    __asm__ volatile (
        "addiu $3, $0, 0\n"
        "syscall"
        : "=r"(syscall_result)
        :
        : "$3", "memory"
    );
    return syscall_result;
}

s32 func_001C0810(void) {
    register s32 syscall_number __asm__("$3") = 0x1;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0820(void) {
    register s32 syscall_number __asm__("$3") = 0x2;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0830(void) {
    register s32 syscall_number __asm__("$3") = 0x3;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

void func_001C0840(s32 status) // Exit (error code: 4) syscall wrapper
{
#ifndef __INTELLISENSE__
    register s32 syscall_status __asm__("$4") = status;
    register s32 syscall_number __asm__("$3") = 4;

    __asm__ volatile (
        "syscall"
        : "+r"(syscall_number)
        : "r"(syscall_status)
        : "memory"
    );
#else
    (void)status;
#endif
}

s32 func_001C0850(void) {
    register s32 syscall_number __asm__("$3") = 0x5;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0860(void) {
    register s32 syscall_number __asm__("$3") = 0x6;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0870(void) {
    register s32 syscall_number __asm__("$3") = 0x7;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0880(void) {
    register s32 syscall_number __asm__("$3") = 0x8;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0890(void) {
    register s32 syscall_number __asm__("$3") = 0x9;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08A0(void) {
    register s32 syscall_number __asm__("$3") = 0xA;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08B0(void) {
    register s32 syscall_number __asm__("$3") = 0xB;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08C0(void) {
    register s32 syscall_number __asm__("$3") = 0xC;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08D0(void) {
    register s32 syscall_number __asm__("$3") = 0xD;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08E0(void) {
    register s32 syscall_number __asm__("$3") = 0xE;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C08F0(void) {
    register s32 syscall_number __asm__("$3") = 0xF;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0900(void)
{
    register s32 syscall_number __asm__("$3") = 0x10;
    register s32 syscall_result __asm__("$2");

    __asm__ volatile (
        "syscall"
        : "=r"(syscall_result), "+r"(syscall_number)
        :
        : "memory"
    );
    return syscall_result;
}

s32 func_001C0910(void) {
    register s32 syscall_number __asm__("$3") = 0x10;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0920(void) {
    register s32 syscall_number __asm__("$3") = 0x11;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0930(void) {
    register s32 syscall_number __asm__("$3") = 0x12;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0940(void) {
    register s32 syscall_number __asm__("$3") = 0x12;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0950() {
    register s32 syscall_number __asm__("$3") = 0x13;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0960(void) {
    register s32 syscall_number __asm__("$3") = 0x14;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0970(void) {
    register s32 syscall_number __asm__("$3") = 0x15;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0980(void) {
    register s32 syscall_number __asm__("$3") = 0x16;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0990(void) {
    register s32 syscall_number __asm__("$3") = 0x17;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09A0(void) {
    register s32 syscall_number __asm__("$3") = 0x18;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09B0(void) {
    register s32 syscall_number __asm__("$3") = 0x19;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09C0(void) {
    register s32 syscall_number __asm__("$3") = -0x1A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09D0(void) {
    register s32 syscall_number __asm__("$3") = -0x1B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09E0(void) {
    register s32 syscall_number __asm__("$3") = -0x1C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C09F0(void) {
    register s32 syscall_number __asm__("$3") = -0x1D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A00() {
    register s32 syscall_number __asm__("$3") = -0x1E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A10(void) {
    register s32 syscall_number __asm__("$3") = -0x1F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A20(void) {
    register s32 syscall_number __asm__("$3") = 0x20;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A30(void) {
    register s32 syscall_number __asm__("$3") = 0x21;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A40(void) {
    register s32 syscall_number __asm__("$3") = 0x22;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A50(void) {
    register s32 syscall_number __asm__("$3") = 0x23;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A60(void) {
    register s32 syscall_number __asm__("$3") = 0x24;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A70(void) {
    register s32 syscall_number __asm__("$3") = 0x25;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A80(void) {
    register s32 syscall_number __asm__("$3") = -0x26;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0A90(void) {
    register s32 syscall_number __asm__("$3") = 0x27;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AA0(void) {
    register s32 syscall_number __asm__("$3") = 0x28;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AB0(void) {
    register s32 syscall_number __asm__("$3") = 0x29;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AC0(void) {
    register s32 syscall_number __asm__("$3") = -0x2A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AD0(void) {
    register s32 syscall_number __asm__("$3") = 0x2B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AE0(void) {
    register s32 syscall_number __asm__("$3") = -0x2C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0AF0(void) {
    register s32 syscall_number __asm__("$3") = 0x2D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B00(void) {
    register s32 syscall_number __asm__("$3") = -0x2E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B10(void) {
    register s32 syscall_number __asm__("$3") = 0x2F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B20() {
    register s32 syscall_number __asm__("$3") = 0x30;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B30(void) {
    register s32 syscall_number __asm__("$3") = -0x31;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

void func_001C0B40(void)
{
#ifndef __INTELLISENSE__
    __asm__ __volatile__("addiu $3, $0, 0x32\n\tsyscall");
#endif
}

s32 func_001C0B50(void) {
    register s32 syscall_number __asm__("$3") = 0x33;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B60(void) {
    register s32 syscall_number __asm__("$3") = -0x34;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B70(void) {
    register s32 syscall_number __asm__("$3") = 0x35;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B80(void) {
    register s32 syscall_number __asm__("$3") = -0x36;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0B90(void) {
    register s32 syscall_number __asm__("$3") = 0x37;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BA0(void) {
    register s32 syscall_number __asm__("$3") = -0x38;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BB0(void) {
    register s32 syscall_number __asm__("$3") = 0x39;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BC0(void) {
    register s32 syscall_number __asm__("$3") = -0x3A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BD0(void) {
    register s32 syscall_number __asm__("$3") = 0x3B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BE0(void) {
    register s32 syscall_number __asm__("$3") = 0x3C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0BF0(void) {
    register s32 syscall_number __asm__("$3") = 0x3D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C00(void) {
    register s32 syscall_number __asm__("$3") = 0x3E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C10(void) {
    register s32 syscall_number __asm__("$3") = 0x3F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C20(void) {
    register s32 syscall_number __asm__("$3") = 0x40;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C30(void) {
    register s32 syscall_number __asm__("$3") = 0x41;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C40(void) {
    register s32 syscall_number __asm__("$3") = 0x42;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C50(void) {
    register s32 syscall_number __asm__("$3") = -0x43;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C60(void) {
    register s32 syscall_number __asm__("$3") = 0x44;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C70(void) {
    register s32 syscall_number __asm__("$3") = 0x45;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C80(void) {
    register s32 syscall_number __asm__("$3") = -0x46;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0C90(void) {
    register s32 syscall_number __asm__("$3") = 0x47;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CA0(void) {
    register s32 syscall_number __asm__("$3") = -0x48;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CB0(void) {
    register s32 syscall_number __asm__("$3") = 0x49;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CC0(void) {
    register s32 syscall_number __asm__("$3") = 0x4A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CD0() {
    register s32 syscall_number __asm__("$3") = 0x4B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CE0(void) {
    register s32 syscall_number __asm__("$3") = 0x4C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0CF0(void) {
    register s32 syscall_number __asm__("$3") = 0x4D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D00(void) {
    register s32 syscall_number __asm__("$3") = 0x4E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D10(void) {
    register s32 syscall_number __asm__("$3") = 0x4F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D20(void) {
    register s32 syscall_number __asm__("$3") = 0x50;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D30(void) {
    register s32 syscall_number __asm__("$3") = 0x51;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D40(void) {
    register s32 syscall_number __asm__("$3") = 0x52;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D50(void) {
    register s32 syscall_number __asm__("$3") = -0x53;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D60(void) {
    register s32 syscall_number __asm__("$3") = 0x54;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D70(void) {
    register s32 syscall_number __asm__("$3") = -0x55;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D80(void) {
    register s32 syscall_number __asm__("$3") = 0x56;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0D90(void) {
    register s32 syscall_number __asm__("$3") = 0x57;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DA0(void) {
    register s32 syscall_number __asm__("$3") = -0x58;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DB0(void) {
    register s32 syscall_number __asm__("$3") = 0x59;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DC0(void) {
    register s32 syscall_number __asm__("$3") = -0x5A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DD0(void) {
    register s32 syscall_number __asm__("$3") = 0x5B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DE0(void) {
    register s32 syscall_number __asm__("$3") = 0x5C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0DF0(void) {
    register s32 syscall_number __asm__("$3") = -0x5C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E00(void) {
    register s32 syscall_number __asm__("$3") = 0x5D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E10(void) {
    register s32 syscall_number __asm__("$3") = -0x5D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E20(void) {
    register s32 syscall_number __asm__("$3") = 0x5E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E30(void) {
    register s32 syscall_number __asm__("$3") = -0x5E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E40(void) {
    register s32 syscall_number __asm__("$3") = 0x5F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E50(void) {
    register s32 syscall_number __asm__("$3") = -0x5F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E60(void) {
    register s32 syscall_number __asm__("$3") = 0x60;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E70(void) {
    register s32 syscall_number __asm__("$3") = 0x61;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E80(void) {
    register s32 syscall_number __asm__("$3") = 0x62;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0E90(void) {
    register s32 syscall_number __asm__("$3") = 0x63;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EA0(void) {
    register s32 syscall_number __asm__("$3") = 0x64;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EB0(void) {
    register s32 syscall_number __asm__("$3") = 0x66;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EC0(void) {
    register s32 syscall_number __asm__("$3") = -0x67;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0ED0(void) {
    register s32 syscall_number __asm__("$3") = -0x68;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EE0(void) {
    register s32 syscall_number __asm__("$3") = -0x6A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0EF0(void) {
    register s32 syscall_number __asm__("$3") = 0x6B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F00(void) {
    register s32 syscall_number __asm__("$3") = 0x6C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F10(void) {
    register s32 syscall_number __asm__("$3") = 0x6D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F20(void) {
    register s32 syscall_number __asm__("$3") = 0x6E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F30(void) {
    register s32 syscall_number __asm__("$3") = 0x6F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F40(void) {
    register s32 syscall_number __asm__("$3") = 0x70;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F50(void) {
    register s32 syscall_number __asm__("$3") = -0x70;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F60(void) {
    register s32 syscall_number __asm__("$3") = 0x71;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F70(void) {
    register s32 syscall_number __asm__("$3") = -0x71;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F80(void) {
    register s32 syscall_number __asm__("$3") = 0x72;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0F90(void) {
    register s32 syscall_number __asm__("$3") = 0x73;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FA0(void) {
    register s32 syscall_number __asm__("$3") = 0x74;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FB0(void) {
    register s32 syscall_number __asm__("$3") = 0x75;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FC0(void) {
    register s32 syscall_number __asm__("$3") = 0x76;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FD0(void) {
    register s32 syscall_number __asm__("$3") = -0x76;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FE0(void) {
    register s32 syscall_number __asm__("$3") = 0x77;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C0FF0(void) {
    register s32 syscall_number __asm__("$3") = -0x77;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1000(void) {
    register s32 syscall_number __asm__("$3") = 0x78;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1010(void) {
    register s32 syscall_number __asm__("$3") = -0x78;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1020() {
    register s32 syscall_number __asm__("$3") = 0x79;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1030(s32 arg0) {
    register s32 syscall_number __asm__("$3") = 0x7A;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1040(void) {
    register s32 syscall_number __asm__("$3") = 0x7B;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1050(s32 arg1, s32 arg2) {
    register s32 syscall_number __asm__("$3") = 0x7C;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1060(void) {
    register s32 syscall_number __asm__("$3") = 0x7D;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1070(void) {
    register s32 syscall_number __asm__("$3") = 0x7E;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1080(void) {
    register s32 syscall_number __asm__("$3") = 0x7F;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

INCLUDE_ASM(const s32, "core/main", func_001C1090);

INCLUDE_ASM(const s32, "core/main", func_001C10D0);

INCLUDE_ASM(const s32, "core/main", func_001C1130);

s32 func_001C11B0(s32 a1, s32 a2, s32 a3)
{

    if (a1 == 0) {
        if (D_00378A60[0] == 0) {
            if (func_001C1E28() == 0) return -1;
            D_00378A60[0] = 1;
        }
        return func_001C1D58(a2, a3);
    }
    return -1;
}

/* Byte-matching.  Stores 5 through the pointer returned by func_00222110 and
   always reports failure. */
s32 func_001C1228()
{
    *(s32 *)(u32)func_00222110() = 5;
    return -1;
}

s32 func_001C1250() { return -1; }

s32 func_001C1258(void) { return -1; }

s32 func_001C1260() { return -1; }

INCLUDE_ASM(const s32, "core/main", func_001C1268);

s32 func_001C1310(void) { return 1; }

s32 func_001C1318(u8 *unused, u8 *p)
{
    *(u64 *)(p + 0x48) = 0;
    *(s32 *)(p + 0x4) = 0x2000;
    return 0;
}

s32 func_001C1330(void) { return 1; }

/* Byte-matching. */
s32 func_001C1338(s32 arg0, s32 arg1)
{
    if (arg0 == 1)
        func_001C0840(arg1);
    return 0;
}

/* Byte-matching.  Same body as func_001C1228. */
s32 func_001C1360(void)
{


    *(s32 *)(u32)func_00222110() = 5;
    return -1;
}

/* Byte-matching.  Same body as func_001C1228 / func_001C1360. */
s32 func_001C1388(void)
{


    *(s32 *)(u32)func_00222110() = 5;
    return -1;
}

INCLUDE_ASM(const s32, "core/main", func_001C13B0);

void func_001C1408(s32 *p)
{
    p[2] = 0;
    p[1] = 0;
    p[0] = 0x200000;
}

INCLUDE_ASM(const s32, "core/main", func_001C1420);

INCLUDE_ASM(const s32, "core/main", func_001C14C8);

void func_001C1540(u32 a, u32 b)
{
    func_001C1420(a & 0xFFFFFFC0, b & 0xFFFFFFC0);
}

INCLUDE_ASM(const s32, "core/main", func_001C1558);

INCLUDE_ASM(const s32, "core/main", func_001C1600);

void func_001C1678(u32 a, u32 b)
{
    func_001C1558(a & 0xFFFFFFC0, b & 0xFFFFFFC0);
}

void func_001C1690(void)
{
    s32 status;
    s32 current;

    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    status = status & 0x10000;
    if (status != 0) {
        do {
            __asm__ volatile("di");
            __asm__ volatile("sync.p");
            __asm__ volatile("mfc0 %0, $12" : "=r"(current));
        } while ((current & 0x10000) != 0);
    }
    func_001C0970();
    __asm__ volatile("sync");
    if (status != 0)
        __asm__ volatile("ei");
}

void func_001C16F8(void)
{
    s32 status;
    s32 current;

    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    status = status & 0x10000;
    if (status != 0) {
        do {
            __asm__ volatile("di");
            __asm__ volatile("sync.p");
            __asm__ volatile("mfc0 %0, $12" : "=r"(current));
        } while ((current & 0x10000) != 0);
    }
    func_001C0960();
    __asm__ volatile("sync");
    if (status != 0)
        __asm__ volatile("ei");
}

s32 func_001C1760()
{
    s32 status;
    s32 current;

    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    status = status & 0x10000;
    if (status != 0) {
        do {
            __asm__ volatile("di");
            __asm__ volatile("sync.p");
            __asm__ volatile("mfc0 %0, $12" : "=r"(current));
        } while ((current & 0x10000) != 0);
    }
    func_001C0990();
    __asm__ volatile("sync");
    if (status != 0)
        __asm__ volatile("ei");
    return;
}

void func_001C17C8(void)
{
    s32 status;
    s32 current;

    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    status = status & 0x10000;
    if (status != 0) {
        do {
            __asm__ volatile("di");
            __asm__ volatile("sync.p");
            __asm__ volatile("mfc0 %0, $12" : "=r"(current));
        } while ((current & 0x10000) != 0);
    }
    func_001C0980();
    __asm__ volatile("sync");
    if (status != 0)
        __asm__ volatile("ei");
}

INCLUDE_ASM(const s32, "core/main", func_001C1830);

void func_001C18A0(void)
{
    s32 status;
    s32 current;

    __asm__ volatile("mfc0 %0, $12" : "=r"(status));
    status = status & 0x10000;
    if (status != 0) {
        do {
            __asm__ volatile("di");
            __asm__ volatile("sync.p");
            __asm__ volatile("mfc0 %0, $12" : "=r"(current));
        } while ((current & 0x10000) != 0);
    }
    func_001C09B0();
    __asm__ volatile("sync");
    if (status != 0)
        __asm__ volatile("ei");
}

/* Byte-matching.  Runs the syscall wrapper then a memory sync.  The nop is
   explicit so the sync does not land in jal's delay slot. */
void func_001C1908(void)
{

    func_001C09C0();
    __asm__ __volatile__(".set noreorder\nsync\n.set reorder" ::: "memory");
}

void func_001C1928(void)
{

    func_001C09D0();
    __asm__ __volatile__(".set noreorder\nsync\n.set reorder" ::: "memory");
}

void func_001C1948(void)
{

    func_001C09E0();
    __asm__ __volatile__(".set noreorder\nsync\n.set reorder" ::: "memory");
}

void func_001C1968(void)
{

    func_001C09F0();
    __asm__ __volatile__(".set noreorder\nsync\n.set reorder" ::: "memory");
}

/* Byte-matching.  The u16 cast puts `andi $4,$4,0xFFFF` in jal's delay slot,
   so here the sync follows the call naturally. */
void func_001C1988(s32 arg)
{
    func_001C0A00((u16)arg);
    __asm__ __volatile__("sync" ::: "memory");
}

void func_001C19A8(void)
{

    func_001C0A10();
    __asm__ __volatile__(".set noreorder\nsync\n.set reorder" ::: "memory");
}

s32 *func_001C19C8(s32 arg)
{
    s32 *base = D_003E3E00;
    base[0] = arg;
    base[1] = 0;
    base[3] = (s32)(base + 4);
    base[2] = (s32)(base + 4);
    return base;
}

/* Byte-matching.  Bumps the count at +4 and the write cursor at +0xC, wrapping
   the cursor to arg0+0x10 once it reaches arg0 + pool offset + 0x10. */
void func_001C19F0(u8 *arg0)
{
    u8 *end;

    (*(s32 *)(arg0 + 4))++;
    (*(u8 **)(arg0 + 0xC))++;
    end = arg0 + (*(s32 *)arg0 + 0x10);
    if (*(u8 **)(arg0 + 0xC) == end)
        *(u8 **)(arg0 + 0xC) = arg0 + 0x10;
}

/* Byte-matching.  Drops the count at +4 and advances the read cursor at +8 with
   the same wrap to arg0+0x10. */
void func_001C1A30(u8 *arg0)
{
    u8 *end;

    (*(s32 *)(arg0 + 4))--;
    (*(u8 **)(arg0 + 8))++;
    end = arg0 + (*(s32 *)arg0 + 0x10);
    if (*(u8 **)(arg0 + 8) == end)
        *(u8 **)(arg0 + 8) = arg0 + 0x10;
}

INCLUDE_ASM(const s32, "core/main", func_001C1A70);

INCLUDE_ASM(const s32, "core/main", func_001C1C08);

INCLUDE_ASM(const s32, "core/main", func_001C1D58);

INCLUDE_ASM(const s32, "core/main", func_001C1E28);

INCLUDE_ASM(const s32, "core/main", func_001C1EE8);

INCLUDE_ASM(const s32, "core/main", func_001C1F20);

void func_001C1FD0(s32 arg0)
{
    if (arg0 == 10) { func_001C1EE8(13); func_001C1EE8(10); return; }
    func_001C1EE8(arg0);
}

INCLUDE_ASM(const s32, "core/main", func_001C2008);

INCLUDE_ASM(const s32, "core/main", func_001C2098);

INCLUDE_ASM(const s32, "core/main", func_001C2160);

INCLUDE_ASM(const s32, "core/main", func_001C22C8);

s32 func_001C2890(void *a1, ...)
{

    char *va = (char *)__builtin_next_arg(a1) - 0x38;

    return func_001C22C8((s32)a1, (s32)va);
}

INCLUDE_ASM(const s32, "core/main", func_001C28C8);

INCLUDE_ASM(const s32, "core/main", func_001C2928);

void func_001C2AE8(void)
{
    func_001C5B50();
    *(s32 *)D_00378A70 = 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C2B10);

void func_001C2BB8(s32 *p)
{
    s32 v = p[4] & 0xFFFFFFFE;
    p[6] = 0;
    p[4] = v;
}

/* Byte-matching.  Advances the cursor modulo the slot count and returns the
   base of the entry it wraps to. */
s32 func_001C2BD8(u8 *arg0)
{
    s32 r = *(s32 *)(arg0 + 0x24) % *(s32 *)(arg0 + 0x18);
    s32 out = *(s32 *)(arg0 + 0x14) + r * 64;

    *(s32 *)(arg0 + 0x24) = r + 1;
    return out;
}

s32 func_001C2C08(s32 *a1, s32 a2)
{
    s32 base;
    if (a2 < 0) return func_001C2BD8(a1);
    if (a2 >= a1[8]) return func_001C2BD8(a1);
    base = a1[7];
    return base + (a2 << 6);
}

INCLUDE_ASM(const s32, "core/main", func_001C2C48);

void func_001C2D00(s32 *a1, s32 a2)
{
    s32 *p = (s32 *)func_001C2BD8(a2);
    s32 v5 = a1[5];
    s32 v4 = a1[7];
    p[5] = v5;
    p[7] = v4;
    p[8] = 0x8000000C;
    func_001C5D88(0x80000008, (u8 *)p, 64, a1[8], a1[9], a1[10]);
}

INCLUDE_ASM(const s32, "core/main", func_001C2D60);

INCLUDE_ASM(const s32, "core/main", func_001C2EB8);

s32 func_001C2F08(u32 *a1, u32 *a2)
{
    u32 v4;
    s32 v5;
    s32 v6;
    v4 = func_001C2BD8(a2);
    v5 = a1[5];
    *(u32 *)(v4 + 28) = a1[7];
    *(u32 *)(v4 + 20) = v5;
    *(u32 *)(v4 + 32) = -2147483639;
    v6 = func_001C2EB8((s32)a1[8], (s32)a2);
    if ( v6 == 0 )
    {
        *(u32 *)(v4 + 36) = 0;
        *(u32 *)(v4 + 40) = 0;
        *(u32 *)(v4 + 44) = 0;
    }
    else
    {
        *(u32 *)(v4 + 36) = v6;
        *(u32 *)(v4 + 40) = *(u32 *)(v6 + 8);
        *(u32 *)(v4 + 44) = *(u32 *)(v6 + 20);
    }
    func_001C5D88(-2147483640, (u8 *)v4, 64, 0, 0, 0);
}

INCLUDE_ASM(const s32, "core/main", func_001C2FB8);

INCLUDE_ASM(const s32, "core/main", func_001C30F8);

INCLUDE_ASM(const s32, "core/main", func_001C3188);

INCLUDE_ASM(const s32, "core/main", func_001C3378);

INCLUDE_ASM(const s32, "core/main", func_001C33B8);

INCLUDE_ASM(const s32, "core/main", func_001C3448);

INCLUDE_ASM(const s32, "core/main", func_001C34D8);

INCLUDE_ASM(const s32, "core/main", func_001C3558);

INCLUDE_ASM(const s32, "core/main", func_001C35E0);

INCLUDE_ASM(const s32, "core/main", func_001C3630);

INCLUDE_ASM(const s32, "core/main", func_001C3810);

INCLUDE_ASM(const s32, "core/main", func_001C3850);

s32 func_001C38C0(s32 idx)
{
    if ((u32)idx >= 0x10) {
        return 0;
    }
    return (s32)(D_003E6790 + idx * 16);
}

void func_001C38E8(void) {
}

void func_001C38F0(void) {
}

s32 func_001C38F8(void) { return 0; }

void func_001C3900(void) {
}

void func_001C3908(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001C3910);

s32 func_001C3A58(void)
{




    s32 r = 0;
    u8 *p = D_003E68B8;

    if (func_00224940(p, &D_00378A5C, 4) != 0)
        r = func_00224940(p, (u8 *)D_00378A7C[0], 4) != 0;
    return r;
}

extern s32 D_00378A74[];
extern u8 D_003E68B8[];
s32 func_001C3AC0(void)
{

    D_00378A74[0] = 0;
    func_00224B88((s32)&D_003E68B8, 0, 4);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C3AF8);

INCLUDE_ASM(const s32, "core/main", func_001C3CA0);

INCLUDE_ASM(const s32, "core/main", func_001C3D98);

INCLUDE_ASM(const s32, "core/main", func_001C3EE8);

INCLUDE_ASM(const s32, "core/main", func_001C3F80);

INCLUDE_ASM(const s32, "core/main", func_001C4128);

INCLUDE_ASM(const s32, "core/main", func_001C4310);

INCLUDE_ASM(const s32, "core/main", func_001C45B8);

s32 func_001C4640(s32 a1)
{




    s32 r;

    if (D_00378A80[0] < 0)
        return 0;
    D_003E6980[0] = a1;
    r = func_001C3188((s32)&D_003E6900[0], 1, 0, (s32)&D_003E6980[0], 4,
                      (s32)&D_003E6940[0], 4, 0, 0);
    if (r < 0)
        return 0;
    return D_003E6940[0];
}

s32 func_001C46B0(s32 a1)
{




    s32 r;

    if (D_00378A80[0] < 0)
        return 0;
    D_003E69C0[0] = a1;
    r = func_001C3188((s32)&D_003E6900[0], 2, 0, (s32)&D_003E69C0[0], 4,
                      (s32)&D_003E6940[0], 4, 0, 0);
    if (r < 0)
        return -1;
    return D_003E6940[0];
}

INCLUDE_ASM(const s32, "core/main", func_001C4728);

INCLUDE_ASM(const s32, "core/main", func_001C4818);

s32 func_001C4918(void)
{




    s32 r = 0;
    u8 *p = D_003E6D28;

    if (func_00224940(p, &D_00378A5C, 4) != 0)
        r = func_00224940(p, (u8 *)D_00378A88[0], 4) != 0;
    return r;
}

extern s32 D_00378A84[];
extern u8 D_003E6D28[];
s32 func_001C4980(void)
{

    D_00378A84[0] = -1;
    func_00224B88((s32)&D_003E6D28, 0, 4);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C49B8);

INCLUDE_ASM(const s32, "core/main", func_001C4BA8);

s32 func_001C4BA8(s32, s32, s32, s32);

s32 func_001C4DA8(s32 arg1, s32 arg2, s32 arg3)
{
    return func_001C4BA8(arg1, arg2, arg3, 0);
}

INCLUDE_ASM(const s32, "core/main", func_001C4DC8);

s32 func_001C4DC8(s32, s32, s32, s32);

s32 func_001C4ED0(s32 arg1, s32 arg2, s32 arg3)
{
    return func_001C4DC8(arg1, arg2, arg3, 1);
}

extern s32 D_0038D9F8[];

s32 func_001C4EF0(s32 arg0, s32 arg1)
{
    return func_001C4DC8(arg0, (s32)D_0038D9F8, arg1, 1);
}

INCLUDE_ASM(const s32, "core/main", func_001C4F18);

INCLUDE_ASM(const s32, "core/main", func_001C5008);

INCLUDE_ASM(const s32, "core/main", func_001C50E8);

/* Byte-matching.  Bit 16 of the value func_001C1030 returns for 4, as a
   boolean.  Written through intermediates because folding it into one
   expression makes gcc emit a shift-and pair instead of lui/and/sltu. */
s32 func_001C5218(void)
{
    u32 v = func_001C1030(4);
    u32 m = v & 0x10000;

    return m != 0;
}

s32 func_001C5240(void)
{

    u32 v = func_001C1030(4);
    u32 m = v & 0x40000;
    if (m != 0) {
        func_001C1020(4, 0x40000);
        func_001C1E28();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C5288);

INCLUDE_ASM(const s32, "core/main", func_001C5388);

INCLUDE_ASM(const s32, "core/main", func_001C5500);

INCLUDE_ASM(const s32, "core/main", func_001C5578);

INCLUDE_ASM(const s32, "core/main", func_001C55D8);

INCLUDE_ASM(const s32, "core/main", func_001C5658);

void func_001C56A0(s32 arg1)
{
    s32 local = arg1;

    func_001C1050(2, (s32)&local);
}

/* Byte-matching.  Queues a two-word event with opcode 3. */
void func_001C56C8(s32 arg0, s8 arg1)
{
    s32 args[2];

    args[0] = arg0;
    args[1] = arg1;
    func_001C1050(3, (s32)args);
}

void func_001C56F8(s32 arg1)
{
    s32 local = arg1;

    func_001C1050(4, (s32)&local);
}

/* Byte-matching.  Queues a three-word event with opcode -5. */
void func_001C5720(s32 arg0, s32 arg1, u16 arg2)
{
    s32 args[3];

    args[0] = arg0;
    args[1] = arg1;
    args[2] = arg2;
    func_001C1050(-5, (s32)args);
}

/* Byte-matching.  Same as func_001C5720 with opcode -6. */
void func_001C5758(s32 arg0, s32 arg1, u16 arg2)
{
    s32 args[3];

    args[0] = arg0;
    args[1] = arg1;
    args[2] = arg2;
    func_001C1050(-6, (s32)args);
}

/* Byte-matching.  Same as func_001C56C8 with opcode -7. */
void func_001C5790(s32 arg0, s8 arg1)
{
    s32 args[2];

    args[0] = arg0;
    args[1] = arg1;
    func_001C1050(-7, (s32)args);
}

void func_001C57C0(s32 arg1)
{
    s32 local = arg1;

    func_001C1050(-8, (s32)&local);
}

void func_001C57E8(s32 arg1)
{
    s32 local = arg1;

    func_001C1050(-9, (s32)&local);
}

void func_001C5810(s32 arg1)
{
    s32 local = arg1;

    func_001C1050(16, (s32)&local);
}

void func_001C5838(s32 *arg1, s32 *arg2)
{
    *(s32 *)(arg2[7] + arg1[4] * 4) = arg1[5];
}

s32 func_001C5858(s32 *src, s32 *dst)
{
    s32 value = src[4];
    dst[2] = value;
    return value;
}

s32 func_001C5868(s32 index)
{
    return D_003E7400[index];
}

s32 func_001C5880(s32 idx, s32 val)
{
    D_003E7400[idx] = val;
    return val;
}

s32 *func_001C58A0(void) { return D_003E72D8; }

INCLUDE_ASM(const s32, "core/main", func_001C58B0);

void func_001C5B50(void)
{


    s32 a = func_001C1760(5);
    func_001C0950(5, D_003E72D4[0]);
    D_00378A90[0] = 0;
    (void)a;
}

s32 func_001C5B88(s32 a, s32 b)
{
    s32 old = D_003E72D8[5];

    D_003E72D8[5] = a;
    D_003E72D8[6] = b;
    return old;
}

s32 func_001C5BA0(s32 a, s32 b)
{
    s32 old = D_003E72D8[3];

    D_003E72D8[3] = a;
    D_003E72D8[4] = b;
    return old;
}

extern s32 D_003E72E4[];
extern s32 D_003E72EC[];

void func_001C5BB8(s32 arg0, u32 a, u32 b)
{
    u32 idx = arg0 << 3;
    if (arg0 < 0) arg0 = D_003E72E4[0];
    else arg0 = D_003E72EC[0];
    idx = idx + arg0;
    *(u32 *)idx = a;
    *(u32 *)(idx + 4) = b;
}

extern s32 D_003E72E4[];
extern s32 D_003E72EC[];

void func_001C5BE8(s32 arg0)
{
    u32 idx = arg0 << 3;
    if (arg0 < 0) arg0 = D_003E72E4[0];
    else arg0 = D_003E72EC[0];
    idx = idx + arg0;
    *(u32 *)idx = 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C5C10);

s32 func_001C5D48(s32 a1, u8 *a2, s64 a3, s32 a4, s32 a5, s64 a6)
{

    return func_001C5C10(a1, 0, a2, a3, a4, a5, a6);
}

s32 func_001C5D88(s32 a1, u8 *a2, s64 a3, s32 a4, s32 a5, s64 a6)
{

    return func_001C5C10(a1, 1, a2, a3, a4, a5, a6);
}

INCLUDE_ASM(const s32, "core/main", func_001C5DC8);

INCLUDE_ASM(const s32, "core/main", func_001C5EE8);

INCLUDE_ASM(const s32, "core/main", func_001C5F94);

s32 func_001C5F98(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi29" : "=r"(value));
    return ((value & 0xF) ^ 1) == 0;
}

void func_001C5FB0(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi28" : "=r"(value));
    value |= 2;
    __asm__ volatile("ctc2 %0, $vi28" : : "r"(value));
}

INCLUDE_ASM(const s32, "core/main", func_001C5FC8);

s32 func_001C6028(void)
{
    s32 result = 0;
    s32 v = func_001C60F8();

    if (v < 4) {
        if (v > 0) {
            result = 1;
            func_001C6138(func_001C6148());
        }
    }
    return result;
}

/* Byte-matching.  Sets or clears bit 2 of the VU0 status register, COP2
   control register $vi28. */
void func_001C6078(s32 arg0)
{
    u32 v;

    __asm__("cfc2 %0,$vi28" : "=r"(v));
    if (arg0 == 1)
        v |= 4;
    else
        v &= ~4;
    __asm__("ctc2 %0,$vi28" : : "r"(v));
}

/* Byte-matching.  Sets or clears bit 3 of the VU0 status register. */
void func_001C60A8(s32 arg0)
{
    u32 v;

    __asm__("cfc2 %0,$vi28" : "=r"(v));
    if (arg0 == 1)
        v |= 8;
    else
        v &= ~8;
    __asm__("ctc2 %0,$vi28" : : "r"(v));
}

s32 func_001C60D8(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi28" : "=r"(value));
    return (value >> 2) & 1;
}

s32 func_001C60E8(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi28" : "=r"(value));
    return (value >> 3) & 1;
}

INCLUDE_ASM(const s32, "core/main", func_001C60F8);

void func_001C6138(u32 address)
{
    __asm__ volatile("ctc2 %0, $vi27" : : "r"(address & 0xFFFF));
    __asm__ volatile("vcallmsr $vi27");
}

/* Reads the VU0 control register vi26. */
s32 func_001C6148(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi26" : "=r"(value));
    return value & 0xFFFF;
}

INCLUDE_ASM(const s32, "core/main", func_001C6158);

INCLUDE_ASM(const s32, "core/main", func_001C6238);

INCLUDE_ASM(const s32, "core/main", func_001C63A8);

INCLUDE_ASM(const s32, "core/main", func_001C63D8);

INCLUDE_ASM(const s32, "core/main", func_001C6418);

void func_001C6460(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi28" : "=r"(value));
    value |= 0x200;
    __asm__ volatile("ctc2 %0, $vi28" : : "r"(value));
}

INCLUDE_ASM(const s32, "core/main", func_001C6478);

s32 func_001C64D8(void)
{
    s32 result = 0;
    s32 v = func_001C65A8();

    if (v < 4) {
        if (v > 0) {
            result = 1;
            func_001C65F0(func_001C6600());
        }
    }
    return result;
}

/* Byte-matching.  Sets or clears bit 10 of the VU0 status register. */
void func_001C6528(s32 arg0)
{
    u32 v;

    __asm__("cfc2 %0,$vi28" : "=r"(v));
    if (arg0 == 1)
        v |= 0x400;
    else
        v &= ~0x400;
    __asm__("ctc2 %0,$vi28" : : "r"(v));
}

/* Byte-matching.  Sets or clears bit 11 of the VU0 status register. */
void func_001C6558(s32 arg0)
{
    u32 v;

    __asm__("cfc2 %0,$vi28" : "=r"(v));
    if (arg0 == 1)
        v |= 0x800;
    else
        v &= ~0x800;
    __asm__("ctc2 %0,$vi28" : : "r"(v));
}

s32 func_001C6588(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi28" : "=r"(value));
    return (value >> 10) & 1;
}

s32 func_001C6598(void)
{
    u32 value;

    __asm__ volatile("cfc2 %0, $vi28" : "=r"(value));
    return (value >> 11) & 1;
}

INCLUDE_ASM(const s32, "core/main", func_001C65A8);

void func_001C65F0(u32 value)
{
    __asm__ volatile("ctc2 %0, $vi31" : : "r"(value & 0xFFFF));
}

s32 func_001C6600(void)
{
    u8 buffer[16];
    func_001C6418(buffer, 0x43A);
    return *(u16 *)buffer;
}

INCLUDE_ASM(const s32, "core/main", func_001C6628);

INCLUDE_ASM(const s32, "core/main", func_001C67B8);

void func_001C6900(void)
{
    *(s32 *)0x10003000 = 1;
}

s32 func_001C6918(void)
{
    *(s32 *)0x10003000 = 8;
    return 1;
}

/* Byte-matching.  Clears the register at 0x10003000 and reports 1 only when
   func_001C6A70 agrees, otherwise 0. */
s32 func_001C6930(void)
{


    if (func_001C6A70() != 1)
        return 0;
    *(volatile u32 *)0x10003000 = 0;
    return 1;
}

s32 func_001C6968(s32 a1)
{
    s32 result;


    if (a1 == 0)
    {
        result = func_001C69D0() & 0xFFFFFFFB;
        *(volatile s32 *)0x10003010 = result;
    }
    else
    {
        result = 1;
        if (a1 == 1)
        {
            result = func_001C69D0() | 4;
            *(volatile s32 *)0x10003010 = result;
        }
    }
    return result;
}

s32 func_001C69D0(void)
{
    return (*(volatile s32 *)0x10003020 & 4) >> 2;
}

s32 func_001C69E8(s32 a1)
{
    s32 result;


    if (a1 == 0)
    {
        result = func_001C69D0() & 0xFFFFFFFE;
        *(volatile s32 *)0x10003010 = result;
    }
    else
    {
        result = 1;
        if (a1 == 1)
        {
            result = func_001C69D0() | 1;
            *(volatile s32 *)0x10003010 = result;
        }
    }
    return 1;
}

s32 func_001C6A50(void)
{
    return func_001C69D0() & 1;
}

/* Byte-matching.  Decodes a video status word into a mode selector. */
s32 func_001C6A70(void)
{
    u32 v = *(volatile u32 *)0x10003020;

    if (v & 8)
        return 1;
    return (v & 0xFC0) ? 2 : 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C6AA0);

INCLUDE_ASM(const s32, "core/main", func_001C6B58);

/* Behavior-faithful, not byte-matching.  Same stores in the same order and
   the same return value; retail computes each address into a general
   register (lui/ori, then sw 0(reg)) where this folds them into the store
   displacement.  Linked size 0x20 == retail. */
s32 func_001C6BB8(void)
{
    *(s32 *)0x10003810 = 1;
    *(s32 *)0x10003820 = 6;
    return 6;
}

INCLUDE_ASM(const s32, "core/main", func_001C6BE0);

/* Byte-matching.  Enables the block at 0x10003810 when func_001C6CB8 reports 1. */
s32 func_001C6C48(void)
{

    if (func_001C6CB8() != 1)
        return 0;
    *(volatile u32 *)0x10003810 = 8;
    return 1;
}

void func_001C6C88(s32 arg0, u32 arg1, u32 arg2)
{
    *(s32 *)0x10003820 = arg0 | (arg1 << 1) | (arg2 << 2);
}

/* Reads a word from the VU scratchpad. */
s32 func_001C6CA8(void)
{
    s32 *p = (s32 *)0x10003820;

    return *p;
}

/* Byte-matching.  Decodes the status word at 0x10003800 into 0..4. */
u32 func_001C6CB8(void)
{
    u32 v = *(volatile u32 *)0x10003800;

    if ((v & 3) == 0)
        return 0;
    if (v & 0x700)
        return 1;
    if (v & 0xC)
        return 2;
    return (v & 0x3000) ? 3 : 4;
}

INCLUDE_ASM(const s32, "core/main", func_001C6D08);

INCLUDE_ASM(const s32, "core/main", func_001C6E88);

INCLUDE_ASM(const s32, "core/main", func_001C6EE8);

INCLUDE_ASM(const s32, "core/main", func_001C6F10);

/* Byte-matching.  Enables the block at 0x10003C10 when func_001C6FE8 reports 1. */
s32 func_001C6F78(void)
{

    if (func_001C6FE8() != 1)
        return 0;
    *(volatile u32 *)0x10003C10 = 8;
    return 1;
}

void func_001C6FB8(s32 arg0, u32 arg1, u32 arg2)
{
    *(s32 *)0x10003C20 = arg0 | (arg1 << 1) | (arg2 << 2);
}

s32 func_001C6FD8(void)
{
    s32 *p = (s32 *)0x10003C20;

    return *p;
}

/* Byte-matching.  Same decode for the status word at 0x10003C00. */
u32 func_001C6FE8(void)
{
    u32 v = *(volatile u32 *)0x10003C00;

    if ((v & 3) == 0)
        return 0;
    if (v & 0x700)
        return 1;
    if (v & 0xC)
        return 2;
    return (v & 0x3000) ? 3 : 4;
}

INCLUDE_ASM(const s32, "core/main", func_001C7038);

INCLUDE_ASM(const s32, "core/main", func_001C7210);

INCLUDE_ASM(const s32, "core/main", func_001C7270);

/* Byte-matching.  Fills arg1 bytes at arg0 with zero. */
void func_001C72D0(u8 *arg0, s32 arg1)
{
    s32 i;

    for (i = arg1 - 1; i != -1; i--)
        *arg0++ = 0;
}

extern s32 D_00378A98[10];

s32 func_001C7308(s32 arg0)
{
    if ((u32)arg0 < 10)
        return D_00378A98[arg0];
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C7330);

s32 func_001C7410(s32 value) { s32 old = D_00378AC0[0]; D_00378AC0[0] = value; return old; }

INCLUDE_ASM(const s32, "core/main", func_001C7420);

typedef struct
{
    UnalignedU64 a, b;
    u32 c;
} S20;

extern u8 D_00378B30[];

u8 *func_001C75F8(u8 *dst)
{
    *(S20 *)dst = *(S20 *)D_00378B30;
    return dst;
}

INCLUDE_ASM(const s32, "core/main", func_001C7630);

INCLUDE_ASM(const s32, "core/main", func_001C7658);

INCLUDE_ASM(const s32, "core/main", func_001C7730);

INCLUDE_ASM(const s32, "core/main", func_001C7818);

INCLUDE_ASM(const s32, "core/main", func_001C7900);

INCLUDE_ASM(const s32, "core/main", func_001C79C8);

INCLUDE_ASM(const s32, "core/main", func_001C7AB8);

extern u8 D_0038DAB0[];

void func_001C7BA8(void)
{
    func_00211918((s32)D_0038DAB0);
}

INCLUDE_ASM(const s32, "core/main", func_001C7BB8);

INCLUDE_ASM(const s32, "core/main", func_001C7C80);

s32 func_001C7D50(void)
{
    func_00211918(D_0038DAD8);
    return 0;
}

extern u8 D_0038DB00[];

void func_001C7D78(void)
{
    func_00211918((s32)D_0038DB00);
}

s32 func_001C7D88(s32 *p)
{
    s32 old = *p;
    s32 bit = ((u32)old >> 8) & 1;
    *p = old & ~0x100;
    return bit;
}

s32 func_001C7DA8(s32 *p)
{
    s32 old = *p;
    s32 bit = ((u32)old >> 8) & 1;
    *p = old & ~0x100;
    return bit;
}

s32 func_001C7DC8(void)
{
    func_00211918(D_0038DB30);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001C7DF0);

INCLUDE_ASM(const s32, "core/main", func_001C7E78);

INCLUDE_ASM(const s32, "core/main", func_001C7FB0);

/* Byte-matching: 9-arg call with absolute globals; bgezl guard. */
s32 func_001C8050(void)
{



    s32 r;
    s32 v;

    D_003E7800[0] = 15;
    r = func_001C3188((s32)&D_003E7480[0], 1, 0, (s32)&D_003E7800[0], 128,
                      (s32)&D_003E7800[0], 128, 0, 0);
    if (r < 0)
        return 0;
    v = D_003E7800[3];
    if (v == 1)
        D_00378B58[0] = 0;
    return v;
}


INCLUDE_ASM(const s32, "core/main", func_001C80D0);

s32 func_001C8228(s32 a1, s32 a2)
{


    s32 r;

    D_003E7800[1] = a1;
    D_003E7800[2] = a2;
    D_003E7800[0] = 14;
    D_003E7800[4] = 1;
    r = func_001C3188((s32)&D_003E7480[0], 1, 0, (s32)&D_003E7800[0], 128,
                      (s32)&D_003E7800[0], 128, 0, 0);
    if (r < 0)
        return 0;
    return D_003E7800[3];
}

INCLUDE_ASM(const s32, "core/main", func_001C82A0);

/* Byte-matching. */
s32 func_001C8300(void)
{


    return *(s32 *)(func_001C82A0() + 0x58);
}

s32 func_001C8320(s32 a1, s32 a2, u8 *a3)
{


    u8 *v4 = func_001C82A0(a1, a2);
    func_002249D4(a3, v4, *(s32 *)(v4 + 0x60));
    return *(s32 *)(v4 + 0x60);
}

INCLUDE_ASM(const s32, "core/main", func_001C8368);

extern u8 D_0038DC48[];
extern s32 D_00378B60[];
extern void func_00226A34(u8 *, s32);

void func_001C83A8(u32 idx, u8 *out)
{
    if (idx < 8)
    {
        func_00226A34(out, D_00378B60[idx]);
    }
    else
    {
        *out = D_0038DC48[0];
    }
}

/* Byte-matching.  Stores the third argument into the byte at +0x71 of the
   object func_001C82A0 returns and reports success. */
s32 func_001C83E0(s32 arg0, s32 arg1, s32 arg2)
{


    *(u8 *)(func_001C82A0() + 0x71) = arg2;
    return 1;
}

/* Byte-matching. */
u32 func_001C8410(void)
{


    return *(u8 *)(func_001C82A0() + 0x71);
}

extern u8 D_0038DC48[];
extern s32 D_00378B80[];
extern void func_00226A34(u8 *, s32);

void func_001C8430(u32 idx, u8 *out)
{
    if (idx < 4)
    {
        func_00226A34(out, D_00378B80[idx]);
    }
    else
    {
        *out = D_0038DC48[0];
    }
}

INCLUDE_ASM(const s32, "core/main", func_001C8468);

INCLUDE_ASM(const s32, "core/main", func_001C8558);

INCLUDE_ASM(const s32, "core/main", func_001C8640);

INCLUDE_ASM(const s32, "core/main", func_001C8748);

INCLUDE_ASM(const s32, "core/main", func_001C8800);

INCLUDE_ASM(const s32, "core/main", func_001C8898);

s32 func_001C8970(s32 a1, s32 a2)
{


    s32 r;

    D_003E7800[1] = a1;
    D_003E7800[2] = a2;
    D_003E7800[0] = 9;
    r = func_001C3188((s32)&D_003E7480[0], 1, 0, (s32)&D_003E7800[0], 128,
                      (s32)&D_003E7800[0], 128, 0, 0);
    if (r < 0)
        return 0;
    return D_003E7800[3];
}

INCLUDE_ASM(const s32, "core/main", func_001C89E0);

/* Byte-matching.  True only when func_001C8970 reports exactly 0x3FFFF. */
u32 func_001C8A90(void)
{

    return (((s32 (*)())func_001C8970)() ^ 0x3FFFF) == 0;
}

s32 func_001C89E0(s32, s32, s32);

s32 func_001C8AC0(s32 arg1, s32 arg2)
{
    return func_001C89E0(arg1, arg2, 0xFFF);
}

s32 func_001C89E0(s32, s32, s32);

s32 func_001C8AE0(s32 arg1, s32 arg2)
{
    return func_001C89E0(arg1, arg2, 0);
}

INCLUDE_ASM(const s32, "core/main", func_001C8B00);

s32 func_001C8BD0(void)
{


    D_003E7800[0] = 12;
    if (func_001C3188(D_003E7480, 1, 0, &D_003E7800[0], 128, &D_003E7800[0], 128, 0, 0) < 0)
        return 0;
    else
        return D_003E7800[3];
}

s32 func_001C8C38(s32 a1)
{


    s32 r;

    D_003E7800[1] = a1;
    D_003E7800[0] = 13;
    r = func_001C3188((s32)&D_003E7480[0], 1, 0, (s32)&D_003E7800[0], 128,
                      (s32)&D_003E7800[0], 128, 0, 0);
    if (r < 0)
        return 0;
    return D_003E7800[3];
}

s32 func_001C8CA0(void)
{


    D_003E7800[0] = 18;
    if (func_001C3188(D_003E7480, 1, 0, &D_003E7800[0], 128, &D_003E7800[0], 128, 0, 0) < 0)
        return 0;
    else
        return D_003E7800[3];
}

s32 func_001C8D08(s32 a1, s32 a2)
{


    s32 r;

    D_003E7800[4] = a1;
    D_003E7800[5] = a2;
    D_003E7800[0] = 19;
    r = func_001C3188((s32)&D_003E7480[0], 1, 0, (s32)&D_003E7800[0], 128,
                      (s32)&D_003E7800[0], 128, 0, 0);
    if (r < 0)
        return 0;
    return D_003E7800[3];
}

void func_001C8D78(s32 *p, s32 value) {
    p[0] = value;
    __asm__ volatile("" ::: "memory");
    p[2] = 0;
    __asm__ volatile("" ::: "memory");
    p[1] = value;
}

s32 func_001C8D88(s32 *p) { s32 value = p[1]; p[2] = 0; p[0] = value; return value; }

typedef struct {
    u32 f0;
    u32 pad4;
    u32 f8;
} S1C8D98Hdr;

u8 *func_001C8D98(S1C8D98Hdr *a)
{
    u8 *p = (u8 *)a->f0;
    u8 *b = (u8 *)a->f8;

    while ((u32)p & 0xC) {
        *(u32 *)p = 0;
        p += 4;
    }
    if (b) {
        s32 n = (s32)(p - b) >> 4;

        n = n - 1;
        *(u32 *)b = *(u32 *)b + n;
    }
    a->f0 = (u32)p;
    a->f8 = 0;
    return p;
}

u32 func_001C8DF0(u8 *p)
{
    return (*(u32 *)p - *(u32 *)(p + 0x4)) >> 4;
}

void func_001C8E08(u32 **a, u64 v)
{
    u32 *p = *a;

    *p = (u32)v;
    p++;
    *p = (u32)(v >> 32);
    *a = p + 1;
}

INCLUDE_ASM(const s32, "core/main", func_001C8E30);

s32 func_001C8F88(s32 *a, s32 *b)
{
    *a = (s32)D_003E8E40;
    *b = (s32)D_00378BB0;
    return (s32)D_003E7880;
}

INCLUDE_ASM(const s32, "core/main", func_001C8FB0);

INCLUDE_ASM(const s32, "core/main", func_001C9040);

INCLUDE_ASM(const s32, "core/main", func_001C90C8);

extern u8 D_00378BB0[];
s32 func_001C91B0(s32 a, s32 b, s32 c)
{

    s32 v = func_001C90C8(a, b, c, 0x40);
    if (v == 0)
        *(s32 *)D_00378BB0 = 0xB;
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_001C91E8);

INCLUDE_ASM(const s32, "core/main", func_001C9278);

INCLUDE_ASM(const s32, "core/main", func_001C9308);

INCLUDE_ASM(const s32, "core/main", func_001C9398);

INCLUDE_ASM(const s32, "core/main", func_001C9470);

INCLUDE_ASM(const s32, "core/main", func_001C95B0);

void func_001C9698(s32 a1)
{



    u32 v = (u32)a1 | 0x20000000u;

    if (D_003E78A8[0] != 0) *(s32 *)D_003E78A8[0] = *(s32 *)v;
    if (D_003E78AC[0] != 0) *(s32 *)D_003E78AC[0] = *(s32 *)(v + 4);
    if (D_003E78B0[0] != 0) *(s32 *)D_003E78B0[0] = *(s32 *)(v + 0x90);
}

INCLUDE_ASM(const s32, "core/main", func_001C96F0);

INCLUDE_ASM(const s32, "core/main", func_001C9828);

/* Byte-matching: hardware-style address inlined at each use -> beql form. */
void func_001C9910(u8 *arg0)
{

    u32 v2;

    if (arg0 != 0) {
        if ((u32)func_00226B48((void *)((u32)D_003E7E40 | 0x20000000u)) < 0x400)
            v2 = func_00226B48((void *)((u32)D_003E7E40 | 0x20000000u));
        else
            v2 = 0x3FF;
        func_002249D4((void *)arg0, (void *)((u32)D_003E7E40 | 0x20000000u), v2);
        *(u8 *)(arg0 + v2) = 0;
    }
}


INCLUDE_ASM(const s32, "core/main", func_001C9998);

INCLUDE_ASM(const s32, "core/main", func_001C9A80);

INCLUDE_ASM(const s32, "core/main", func_001C9B10);

INCLUDE_ASM(const s32, "core/main", func_001C9BE8);

INCLUDE_ASM(const s32, "core/main", func_001C9C78);

INCLUDE_ASM(const s32, "core/main", func_001C9DD0);

INCLUDE_ASM(const s32, "core/main", func_001C9ED0);

INCLUDE_ASM(const s32, "core/main", func_001C9F60);

INCLUDE_ASM(const s32, "core/main", func_001CA018);

extern s8 D_00378BD0[];

s32 func_001CA0B8(void)
{

    if (D_00378BD0[0] == 0)
        func_001CA018();
    return D_00378BD0[4] == 0x54;
}

INCLUDE_ASM(const s32, "core/main", func_001CA0F8);

INCLUDE_ASM(const s32, "core/main", func_001CA158);

extern u8 D_00378BCA[];

s32 func_001CA178(void)
{
    u32 v;

    if (func_001CA0B8() != 0)
        return D_00378BCA[0];
    func_001C0CD0(&v);
    return (v >> 1) & 3;
}

extern u8 D_00378BCD[];

s32 func_001CA1B8(void)
{
    u32 v;

    if (func_001CA0B8() != 0)
        return D_00378BCD[0];
    func_001C0CD0(&v);
    return v & 1;
}

INCLUDE_ASM(const s32, "core/main", func_001CA1F8);

INCLUDE_ASM(const s32, "core/main", func_001CA260);

INCLUDE_ASM(const s32, "core/main", func_001CA2E0);

INCLUDE_ASM(const s32, "core/main", func_001CA360);

u8 func_001CA3E0(u8 arg0)
{

    if (arg0 >= 100)
        func_00222068(D_0038DD78, 0x11B, D_0038DD88);
    return (u8)(6 * (arg0 / 10) + arg0);
}

INCLUDE_ASM(const s32, "core/main", func_001CA448);

s32 func_001CA4A0(s32 a1)
{


    u8 *v1;
s8 v2;
u8 v3;
s8 v4;
u8 v5;
s8 v6;
u8 v7;
s8 v8;
u8 v9;
s8 v10;
u8 v11;
s32 result;
v1 = (u8 *)a1;
if ( a1 == 0 )
func_00222068(D_0038DD78, 308, D_0038DDA0);
v2 = func_001CA448(*(u8 *)(a1 + 7));
v3 = v1[6];
v1[7] = v2;
v4 = func_001CA448(v3);
v5 = v1[5];
v1[6] = v4;
v6 = func_001CA448(v5);
v7 = v1[3];
v1[5] = v6;
v8 = func_001CA448(v7);
v9 = v1[2];
v1[3] = v8;
v10 = func_001CA448(v9);
v11 = v1[1];
v1[2] = v10;
result = func_001CA448(v11);
v1[1] = result;
return result;
}

s32 func_001CA528(s32 a1)
{

    u8 *v1;
s8 v2;
u8 v3;
s8 v4;
u8 v5;
s8 v6;
u8 v7;
s8 v8;
u8 v9;
s8 v10;
u8 v11;
s32 result;
v1 = (u8 *)a1;
if ( a1 == 0 )
func_00222068(D_0038DD78, 323, D_0038DDA0);
v2 = func_001CA3E0(*(u8 *)(a1 + 7));
v3 = v1[6];
v1[7] = v2;
v4 = func_001CA3E0(v3);
v5 = v1[5];
v1[6] = v4;
v6 = func_001CA3E0(v5);
v7 = v1[3];
v1[5] = v6;
v8 = func_001CA3E0(v7);
v9 = v1[2];
v1[3] = v8;
v10 = func_001CA3E0(v9);
v11 = v1[1];
v1[2] = v10;
result = func_001CA3E0(v11);
v1[1] = result;
return result;
}

INCLUDE_ASM(const s32, "core/main", func_001CA5B0);

INCLUDE_ASM(const s32, "core/main", func_001CA698);

void func_001CA770(u8 *a1)
{

    s32 v;
    if (a1 == 0) func_00222068((s32)D_0038DD78, 387, (s32)D_0038DDA0);
    v = a1[3] + 1;
    a1[3] = v;
    if ((u8)v == 24) {
        a1[3] = 0;
        func_001CA5B0((s32)a1);
        return;
    }
}

extern u8 D_0038DD78[];
extern u8 D_0038DDA0[];

void func_001CA7E0(u8 *a1)
{
    s32 v;

    if (a1 == 0)
        func_00222068((s32)D_0038DD78, 0x190, (s32)D_0038DDA0);
    v = *(u8 *)(a1 + 3);
    if (v != 0) {
        v = v - 1;
        *(u8 *)(a1 + 3) = (u8)v;
        return;
    }
    *(u8 *)(a1 + 3) = 23;
    func_001CA698((s32)a1);
}

INCLUDE_ASM(const s32, "core/main", func_001CA848);

extern u8 D_0038DD78[];
extern u8 D_0038DDA0[];

void func_001CA918(s32 arg0)
{

    if (arg0 == 0)
        func_00222068((s32)D_0038DD78, 0x1BE, (s32)D_0038DDA0);
    func_001CA848(arg0, -0x21C);
}

void func_001CA960(s32 a1)
{



    s32 v2 = func_001CA1F8();
    s32 v3 = func_001CA2E0();
    s32 v4 = 60 * v3 - 540;
    s32 v5 = v2 + v4;

    if (a1 == 0)
        func_00222068((s32)D_0038DD78, 0x1C9, (s32)D_0038DDA0);
    func_001CA848(a1, v5);
}

extern s32 D_00378C3C[];

s32 func_001CA9C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{


    if (func_001CC150(1, arg1, arg2, arg3) != 0)
        return 0;
    {
        s32 old = D_00378C3C[0];
        D_00378C3C[0] = arg0;
        return old;
    }
}

INCLUDE_ASM(const s32, "core/main", func_001CAA08);

INCLUDE_ASM(const s32, "core/main", func_001CAAB8);

INCLUDE_ASM(const s32, "core/main", func_001CAC50);

INCLUDE_ASM(const s32, "core/main", func_001CACF0);

INCLUDE_ASM(const s32, "core/main", func_001CAD88);

INCLUDE_ASM(const s32, "core/main", func_001CADF0);

s32 func_001CAEB8(u8 *p)
{
    s32 a = (p[0] >> 4) * 10 + (p[0] & 0xF);
    s32 b = (p[1] >> 4) * 10 + (p[1] & 0xF);
    s32 c = (p[2] >> 4) * 10 + (p[2] & 0xF);

    return (a * 60 + b) * 75 + c - 150;
}

INCLUDE_ASM(const s32, "core/main", func_001CAF18);

INCLUDE_ASM(const s32, "core/main", func_001CB060);

INCLUDE_ASM(const s32, "core/main", func_001CB350);

INCLUDE_ASM(const s32, "core/main", func_001CB4C0);

extern s32 D_00378C40[];
extern u8 D_003E9640[];

s32 func_001CB558(void)
{
    s32 one = 1;

    if (D_00378C40[0] == one)
        return *(s32 *)((u32)D_003E9640 | 0x20000000u);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001CB588);

INCLUDE_ASM(const s32, "core/main", func_001CB878);

INCLUDE_ASM(const s32, "core/main", func_001CBA48);

INCLUDE_ASM(const s32, "core/main", func_001CBB90);

INCLUDE_ASM(const s32, "core/main", func_001CBE20);

INCLUDE_ASM(const s32, "core/main", func_001CBF08);

INCLUDE_ASM(const s32, "core/main", func_001CBFC8);

INCLUDE_ASM(const s32, "core/main", func_001CC090);

INCLUDE_ASM(const s32, "core/main", func_001CC150);

s32 func_001CC1F8(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{




    s32 v8;
    s32 result;

    result = 0;
    if (a1 == 0) {
        if (D_00378BF0[0] > 0)
            func_00211918((s32)D_0038DF88, a2, a3, a4, a5, a6, a7, a8);
        do {
            v8 = func_001C3378((s32)D_003EA2E0) != 0;
        } while (v8);
    } else {
        return func_001C3378((s32)D_003EA2E0);
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001CC260);

INCLUDE_ASM(const s32, "core/main", func_001CC3D0);

INCLUDE_ASM(const s32, "core/main", func_001CC6B8);

INCLUDE_ASM(const s32, "core/main", func_001CC8B0);

INCLUDE_ASM(const s32, "core/main", func_001CC968);

INCLUDE_ASM(const s32, "core/main", func_001CCA00);

INCLUDE_ASM(const s32, "core/main", func_001CCAB8);

INCLUDE_ASM(const s32, "core/main", func_001CCB50);

INCLUDE_ASM(const s32, "core/main", func_001CCC28);

INCLUDE_ASM(const s32, "core/main", func_001CCD10);

INCLUDE_ASM(const s32, "core/main", func_001CCDD0);

INCLUDE_ASM(const s32, "core/main", func_001CCE90);

INCLUDE_ASM(const s32, "core/main", func_001CCF88);

extern s32 D_00378C38[];
extern u8 D_00378C48[];
s32 func_001CD070(s32 a, s32 b, s32 c)
{

    D_00378C38[0] = 0;
    return func_001CD3D0(a, b, c, 5, (s32)&D_00378C48);
}

extern s32 D_00378C38[];
s32 func_001CD0A0(s32 arg0, s32 arg1)
{

    D_00378C38[0] = 1;
    return func_001CD3D0(arg0, 0, 0, 1, arg1);
}

extern u8 D_00378C48[];
s32 func_001CD0D8(s32 a)
{

    return func_001CD3D0(a, 0, 0, 9, (s32)&D_00378C48);
}

extern u8 D_00378C48[];
s32 func_001CD108(s32 a)
{

    return func_001CD3D0(a, 0, 0, 4, (s32)&D_00378C48);
}

extern s32 D_00378C38[];
extern u8 D_00378C48[];
s32 func_001CD138(void)
{

    D_00378C38[0] = 0;
    return func_001CD3D0(0, 0, 0, 3, (s32)&D_00378C48);
}

INCLUDE_ASM(const s32, "core/main", func_001CD170);

s32 func_001CD2E0(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{






    D_00378C38[0] = 0;
    if (D_00378BF0[0] > 0) {
        func_00211918((s32)D_0038E138, a2, a3, a4, a5, a6, a7, a8);
    }
    return func_001CD3D0(0, 0, 0, 7, (s32)&D_00378C48);
}

INCLUDE_ASM(const s32, "core/main", func_001CD330);

s32 func_001CD388(void)
{




    if (D_00378BF0[0] > 0) {
        func_00211918((s32)D_0038E168);
    }
    return func_001CD3D0(0, 0, 0, 6, (s32)&D_00378C48);
}

INCLUDE_ASM(const s32, "core/main", func_001CD3D0);

INCLUDE_ASM(const s32, "core/main", func_001CD530);

INCLUDE_ASM(const s32, "core/main", func_001CD5D8);

s32 func_001CD818(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001CD820);

INCLUDE_ASM(const s32, "core/main", func_001CD858);

INCLUDE_ASM(const s32, "core/main", func_001CD8A0);

INCLUDE_ASM(const s32, "core/main", func_001CD8E8);

void func_001CD938(u8 *p, s32 a, s32 b, s32 c)
{
    s32 *q = *(s32 **)(p + 0x40);

    q[0x94 / 4] = a;
    q[0x98 / 4] = b;
    q[0x9C / 4] = c;
}

void func_001CD950(u8 *p, s32 *a, s32 *b, s32 *c)
{
    u8 *q = *(u8 **)(p + 0x40);
    *a = *(s32 *)(q + 0x94);
    *b = *(s32 *)(q + 0x98);
    *c = *(s32 *)(q + 0x9C);
}

s32 func_001CD970(s32 *p) { return *(s32 *)p[0x10]; }

s32 func_001CD980(s32 *p) { return ((s32 *)p[0x40 / 4])[1] == 0; }

s32 func_001CD990(u8 *arg0)
{



    s32 neg1 = -1;
    u32 *p = *(u32 **)(arg0 + 0x40);

    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    *(u32 *)(arg0 + 8) = 0;
    p[0x20] = (u32)neg1;
    p[0x2B] = 0;
    func_001CE1D8();
    D_00378D84[0] = 0;
    func_001CE0F0();
}

s32 func_001CD9D0(void)
{






    if (D_00378C80[0] != 0) ((s32 *)D_00378C80[0])[10] = 0;
    if (D_00378C8C[0] != 0) ((s32 *)D_00378C8C[0])[10] = 0;
    if (D_00378C98[0] != 0) ((s32 *)D_00378C98[0])[10] = 0;
    if (D_00378C84[0] != 0) ((s32 *)D_00378C84[0])[10] = 0;
    if (D_00378C90[0] != 0) ((s32 *)D_00378C90[0])[10] = 0;
    if (D_00378C9C[0] != 0) ((s32 *)D_00378C9C[0])[10] = 0;
    return 1;
}

void func_001CDA38(u8 *p, s32 idx, s32 a, s32 b)
{
    u8 *base = *(u8 **)(p + 0x40);
    *(s32 *)(base + idx * 8 + 0x10) = a;
    *(s32 *)(base + idx * 8 + 0xC) = b;
}

INCLUDE_ASM(const s32, "core/main", func_001CDA60);

/* Byte-matching.  The 0x20-byte scratch block is what retail reserves: the 1
   goes at offset 0 and the block is handed to func_001CDA60. */
void func_001CDAB0(s32 arg0)
{

    u32 tmp[8];

    tmp[0] = 1;
    func_001CDA60(arg0, tmp);
}

void func_001CDAD8(u8 *p, u64 value)
{
    u8 *q = *(u8 **)(p + 0x40);

    *(s32 *)(q + 0x70) = 1;
    *(u64 *)(q + 0x78) = value;
}

void func_001CDAF0(s32 *p) { s32 *object = (s32 *)p[0x40 / 4]; object[0x70 / 4] = 0; *(unsigned long long *)(object + 0x78 / 4) = 0; }

void func_001CDB00(s32 value)
{
    u8 *q = *(u8 **)(*(u32 *)D_00378C6C + 0x40);

    *(s32 *)(q + 0xD8) = value;
}

s32 func_001CDB18(s32 *p) { return *(s32 *)((u8 *)p[0x10] + 0xCC); }

s32 func_001CDB28(s32 *p) { return *(s32 *)((u8 *)p[0x10] + 0xD0); }

u8 *func_001CDB38(s32 *p) { return (u8 *)p[0x10] + 0xB4; }

u8 *func_001CDB48(s32 *p) { return (u8 *)p[0x10] + 0xB4; }

s32 func_001CDB58(s32 *p, s32 value) { s32 *field = (s32 *)p[0x40 / 4]; s32 old = field[0xE8 / 4]; field[0xE8 / 4] = value; return old; }

void func_001CDB68(u8 *p, u64 value)
{
    u8 *q = *(u8 **)(p + 0x40);

    *(u64 *)(q + 0xF0) = value;
    *(s32 *)(q + 0xF8) = 1;
}

void func_001CDB80(u8 *p, s32 a, s32 b)
{
    *(s32 *)(p + 0x4) = b;
    *(s32 *)p = a;
    *(s32 *)(p + 0x8) = a;
    *(s32 *)(p + 0xC) = a;
}

s32 func_001CDB98(s32 *p) { s32 value = p[2]; p[3] = value; return value; }

s32 func_001CDBA8(s32 *p) { s32 value = p[3]; p[2] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_001CDBB8);

s32 func_001CDC20(s32 *p)
{
    return p[0] + p[1] - p[2];
}

INCLUDE_ASM(const s32, "core/main", func_001CDC38);

INCLUDE_ASM(const s32, "core/main", func_001CDDA8);

extern s32 D_00378E2C[];
s32 func_001CDED0(s32 a, s32 b, s32 c)
{


    if (D_00378E2C[0] != 3)
        return func_001CDF10(a, b, c);
    return func_001CDDA8(a, b, c);
}

INCLUDE_ASM(const s32, "core/main", func_001CDF10);

INCLUDE_ASM(const s32, "core/main", func_001CE080);

void func_001CE0F0(void) { *(s32 *)&D_00379530[0] = 0; func_001D0A58(1); }

INCLUDE_ASM(const s32, "core/main", func_001CE100);

INCLUDE_ASM(const s32, "core/main", func_001CE188);

INCLUDE_ASM(const s32, "core/main", func_001CE1D8);

void func_001CE2A8(s32 arg)
{
    func_00211918((s32)D_0038E278, arg);
}

/* Byte-matching.  Formats into a 0x100-byte stack buffer, then hands it on. */
void func_001CE2B8(s32 arg0, s32 arg1)
{

    u8 buf[0x100];

    func_00226310(buf, arg0, arg1);
    func_001CE2E8(buf);
}

INCLUDE_ASM(const s32, "core/main", func_001CE2E8);

INCLUDE_ASM(const s32, "core/main", func_001CE340);

s32 func_001CE418(u8 *arg0, s32 arg1, s32 arg2)
{
    *(s32 *)(arg0 + 4) = arg1;
    *(s32 *)(arg0 + 8) = arg2;
    *(s32 *)(arg0 + 0xC) = arg1 >> 4;
    *(s32 *)(arg0 + 0x10) = arg2 >> 4;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001CE438);

INCLUDE_ASM(const s32, "core/main", func_001CE590);

INCLUDE_ASM(const s32, "core/main", func_001CE8D8);

INCLUDE_ASM(const s32, "core/main", func_001CE9C0);

INCLUDE_ASM(const s32, "core/main", func_001CEA80);

s32 func_001CEBE8(void)
{







    s32 *p = D_00378DDC;
    s32 *q = D_00378DE0;
    s32 v;

    *p = func_001D2258(3);
    v = func_001D2258(1);
    *q = v;
    if (v != 0)
    {
        p = D_00378DE4;
        *p = func_001D2258(8);
        q = D_00378DE8;
        *q = func_001D2258(8);
        p = D_00378DEC;
        *p = func_001D2258(8);
    }
    p = D_00378DF0;
    *p = func_001D2258(14);
    func_001D2258(1);
    q = D_00378DF4;
    v = func_001D2258(14);
    *q = v;
    return v;
}

extern u8 D_0038E2E8[];
extern u8 D_0038E318[];
extern u8 D_0038E330[];
extern u8 D_0038E368[];
extern void func_001CE2E8(s32);
extern void func_001D3F18(s32);
extern void func_001D4098(s32);

void func_001CEC88(void)
{
    func_001CE2E8((s32)D_0038E2E8);
}

void func_001CEC98(void)
{
    func_001CE2E8((s32)D_0038E318);
}

void func_001CECA8(void)
{
    func_001CE2E8((s32)D_0038E330);
}

void func_001CECB8(void)
{
    func_001CE2E8((s32)D_0038E368);
}

void func_001CECC8(s32 *p)
{
    func_001D3F18((s32)((u8 *)p[0x10] + 0x4C));
}

void func_001CECD8(s32 *p)
{
    func_001D4098((s32)((u8 *)p[0x10] + 0x4C));
}

/* Retail pads this function out to the 16-byte aligned 0x001CECF0 entry. */
__asm__(".space 12, 0");

INCLUDE_ASM(const s32, "core/main", func_001CECF0);

INCLUDE_ASM(const s32, "core/main", func_001CF028);

INCLUDE_ASM(const s32, "core/main", func_001CF6D8);

INCLUDE_ASM(const s32, "core/main", func_001CFAF8);

INCLUDE_ASM(const s32, "core/main", func_001CFC88);

INCLUDE_ASM(const s32, "core/main", func_001CFD00);

INCLUDE_ASM(const s32, "core/main", func_001CFD98);

INCLUDE_ASM(const s32, "core/main", func_001CFE50);

INCLUDE_ASM(const s32, "core/main", func_001CFF20);

INCLUDE_ASM(const s32, "core/main", func_001CFFD0);


/* Source: media/region_001D.c */
/* func_001D0088 through func_001DFF88; retail address range 0x001D0000-0x001E0000. */


INCLUDE_ASM(const s32, "core/main", func_001D0088);

INCLUDE_ASM(const s32, "core/main", func_001D0180);

INCLUDE_ASM(const s32, "core/main", func_001D0280);

INCLUDE_ASM(const s32, "core/main", func_001D0328);

INCLUDE_ASM(const s32, "core/main", func_001D03D8);

INCLUDE_ASM(const s32, "core/main", func_001D04C0);

INCLUDE_ASM(const s32, "core/main", func_001D05A8);

INCLUDE_ASM(const s32, "core/main", func_001D0688);

INCLUDE_ASM(const s32, "core/main", func_001D0758);

INCLUDE_ASM(const s32, "core/main", func_001D0880);

INCLUDE_ASM(const s32, "core/main", func_001D0998);

INCLUDE_ASM(const s32, "core/main", func_001D09F8);

/* Byte-matching.  Read-modify-write of the 0x10002010 register: the low bits
   of arg0 go into bits 23..30, bits 23..30 of the old value are cleared. */
void func_001D0A58(u32 arg0)
{
    *(u32 *)0x10002010 = (*(u32 *)0x10002010 & 0xFF7FFFFF) | (arg0 << 23);
}

INCLUDE_ASM(const s32, "core/main", func_001D0A80);

s32 func_001D1E50(s32);

s32 func_001D0C80(void)
{
    return func_001D1E50(3);
}

INCLUDE_ASM(const s32, "core/main", func_001D0CA0);

INCLUDE_ASM(const s32, "core/main", func_001D0E28);

INCLUDE_ASM(const s32, "core/main", func_001D0F38);

INCLUDE_ASM(const s32, "core/main", func_001D1060);

INCLUDE_ASM(const s32, "core/main", func_001D1188);

INCLUDE_ASM(const s32, "core/main", func_001D1388);

INCLUDE_ASM(const s32, "core/main", func_001D1458);

INCLUDE_ASM(const s32, "core/main", func_001D1980);

INCLUDE_ASM(const s32, "core/main", func_001D1A08);

INCLUDE_ASM(const s32, "core/main", func_001D1B88);

extern u8 D_00379588[];
extern s32 D_00378CA4[];

void func_001D1CC8(u32 arg0)
{
    u32 base;
    *(volatile u32 *)0x10002000 = arg0;
    base = (u32)D_00379588;
    D_00378CA4[0] = *(u32 *)((((arg0 >> 28) << 2)) + base);
}

INCLUDE_ASM(const s32, "core/main", func_001D1CF8);

INCLUDE_ASM(const s32, "core/main", func_001D1DA0);

INCLUDE_ASM(const s32, "core/main", func_001D1E50);

INCLUDE_ASM(const s32, "core/main", func_001D2008);

INCLUDE_ASM(const s32, "core/main", func_001D2140);

INCLUDE_ASM(const s32, "core/main", func_001D2258);

INCLUDE_ASM(const s32, "core/main", func_001D23C0);

INCLUDE_ASM(const s32, "core/main", func_001D2428);

INCLUDE_ASM(const s32, "core/main", func_001D2498);

INCLUDE_ASM(const s32, "core/main", func_001D25C0);

INCLUDE_ASM(const s32, "core/main", func_001D2690);

INCLUDE_ASM(const s32, "core/main", func_001D2740);

/* Byte-matching.  Drains the queue, one unit of work per pending item. */
void func_001D2970(void)
{



    while (func_001D2258(1) != 0)
        func_001D2140(8);
}

INCLUDE_ASM(const s32, "core/main", func_001D29A8);

INCLUDE_ASM(const s32, "core/main", func_001D2A88);

INCLUDE_ASM(const s32, "core/main", func_001D2B70);

INCLUDE_ASM(const s32, "core/main", func_001D2C20);

INCLUDE_ASM(const s32, "core/main", func_001D2D10);

INCLUDE_ASM(const s32, "core/main", func_001D2DC0);

INCLUDE_ASM(const s32, "core/main", func_001D2EA0);

INCLUDE_ASM(const s32, "core/main", func_001D2F58);

INCLUDE_ASM(const s32, "core/main", func_001D3338);

INCLUDE_ASM(const s32, "core/main", func_001D33D8);

s32 func_001D35C8(void)
{


    u8 *q = *(u8 **)(*(u32 *)D_00378C6C + 0x40);
    if (*(s32 *)(q + 8) != 2)
    {
        s32 v = *(s32 *)D_00378D84;
        *(s32 *)(q + 8) = 2;
        *(s32 *)(q + 0xAC) = v;
    }
    *(s32 *)D_00379500 = 1;
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001D3600);

INCLUDE_ASM(const s32, "core/main", func_001D3798);

INCLUDE_ASM(const s32, "core/main", func_001D38B0);

INCLUDE_ASM(const s32, "core/main", func_001D3A70);

INCLUDE_ASM(const s32, "core/main", func_001D3B58);

INCLUDE_ASM(const s32, "core/main", func_001D3C10);

INCLUDE_ASM(const s32, "core/main", func_001D3DC0);

INCLUDE_ASM(const s32, "core/main", func_001D3F18);

INCLUDE_ASM(const s32, "core/main", func_001D4098);

INCLUDE_ASM(const s32, "core/main", func_001D4268);

INCLUDE_ASM(const s32, "core/main", func_001D42D0);

extern s32 D_003939C8;
extern s8 D_003939E9;
extern s32 D_003939F8;
extern s32 D_00393A00;
extern u32 D_003939D4;
extern s32 D_003939DC;
extern f32 D_00393A04;
extern s8 D_003939D0;
extern s8 D_003939E0;
extern s32 D_003939E4;
s32 func_001D4568(void)
{
    s32 result;

    D_003939C8 = 0;
    D_003939E9 = 1;
    D_003939F8 = 0;
    D_00393A00 = 0;
    D_003939D4 = 0;
    D_003939DC = 0;
    D_00393A04 = 0.0f;
    D_003939D0 = 0;
    func_001D4D58();
    func_001D4DC0();
    result = func_001DC6E8();
    D_003939E0 = 0;
    D_003939E4 = 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001D45C0);

INCLUDE_ASM(const s32, "core/main", func_001D46A0);

INCLUDE_ASM(const s32, "core/main", func_001D4C70);

extern u8 D_00380B48[];
extern u8 D_00380C08[];

void func_001D4D58(void)
{
    s32 i;
    u32 base;
    u32 *p;

    base = (u32)D_00380B48;
    p = (u32 *)(base + 0x18);
    i = 5;
    do {
        p[1] = 0;
        i--;
        p[0] = 0;
        p += 8;
    } while (i >= 0);

    base = (u32)D_00380C08;
    p = (u32 *)(base + 0x18);
    i = 5;
    do {
        p[1] = 0;
        i--;
        p[0] = 0;
        p += 8;
    } while (i >= 0);
}

extern u8 D_00380800[];

void func_001D4DC0(void)
{
    u32 base = (u32)D_00380800;
    s32 *p = (s32 *)(base + 0x344);
    s32 i = 0x22;

    do
    {
        *p = 0;
        p = (s32 *)((u8 *)p - 0x18);
    } while (--i >= 0);
}

INCLUDE_ASM(const s32, "core/main", func_001D4DF8);

INCLUDE_ASM(const s32, "core/main", func_001D4E68);

INCLUDE_ASM(const s32, "core/main", func_001D4EC8);

INCLUDE_ASM(const s32, "core/main", func_001D4FA8);

INCLUDE_ASM(const s32, "core/main", func_001D5140);

void func_001D54F0(void)
{






    f32 v1;
    s32 i = 0;

    D_003939CC = (D_003939CC == 0);
    v1 = func_0011A818();
    func_0011B0A0();
    if (D_003939C8 > 0) {
        do { func_001D5568(i); i++; } while (i < D_003939C8);
    }
    func_0011B0C0();
    func_0011A788(v1);
}

/* Proposed behavioural reconstruction of func_001D5568 (5552 bytes, 0x1D5568).
 *
 * Not enabled in src/ yet: as C it compiles to the right size in isolation
 * (symbol size 5552, identical to retail) but the linked image then overflows
 * .main into .main_bss by 16 bytes, so the tree keeps the INCLUDE_ASM stub
 * until that is reduced.  Everything below is transcribed from
 * asm/nonmatchings/core/main/func_001D5568.s; see
 * docs/REFERENCE-BEHAVIORMATCH.md for the field map, the kind/group dispatch
 * and the evidence behind each call's arguments.
 */
/* ===== func_001D5568: one element of the D_003796D0 table =================
 *
 * D_003796D0 is 301 records of 0x60 bytes (28896 bytes, all zero in the image:
 * the table is filled at run time).  arg0 selects the record.  The function
 * advances the record's state machine and submits it for drawing; the draw
 * handle at +0x50 is the gate (a zero handle means "idle, nothing to do").
 *
 * Record fields, as used by this function:
 *   +0x00  flags   bits 0..7  kind (0..0x15)
 *                   bits 24..27 group (selects the draw path)
 *                   bit  24     set here: submitted this frame
 *                   bit  27     add +0x44 to z while drawing, then undo
 *   +0x04  bit 0    single-shot variant: one draw call, no state machine
 *   +0x08  s32      integer parameter (count/level; kind 0x80xxxxxx uses it
 *                   as a float, with the unsigned-conversion sequence)
 *   +0x0C  f32      phase step
 *   +0x10..+0x18    a direction/radius vector (x, y, z)
 *   +0x1C..+0x24    position (x, y, z), also the vector handed to the draws
 *   +0x28..+0x30    direction (x, y, z)
 *   +0x34..+0x3C    up vector (x, y, z)
 *   +0x40, +0x44    two scales (colour/alpha style parameters)
 *   +0x48  f32      scale published to D_00393A2C (except kind 0x0C: 1.0)
 *   +0x50  u32      draw handle (0 = idle)
 *   +0x54  u32      handle used by kind 0x13
 *   +0x58  f32      remaining life (kinds 6/7/8/0xB/0x13); clamped at 0
 *
 * Kinds 7, 8 and 0x0B move the position by a sway offset for the draw and the
 * shared tail at 0x6A40 takes it back out, so the logical position does not
 * change.  The shared tail runs for every kind, hence the kind test there.
 *
 * Callees that do not exist as C yet are declared below with the register
 * usage observed at every call site (first six integers in $4..$9, floats in
 * $f12/$f13 independently of the integer arguments).
 */
typedef struct
{
    u32 flags;          /* +0x00 */
    u32 mode;           /* +0x04 */
    s32 i08;            /* +0x08 */
    f32 f0C;            /* +0x0C */
    f32 f10;            /* +0x10 */
    f32 f14;            /* +0x14 */
    f32 f18;            /* +0x18 */
    f32 f1C;            /* +0x1C */
    f32 f20;            /* +0x20 */
    f32 f24;            /* +0x24 */
    f32 f28;            /* +0x28 */
    f32 f2C;            /* +0x2C */
    f32 f30;            /* +0x30 */
    f32 f34;            /* +0x34 */
    f32 f38;            /* +0x38 */
    f32 f3C;            /* +0x3C */
    f32 f40;            /* +0x40 */
    f32 f44;            /* +0x44 */
    f32 f48;            /* +0x48 */
    f32 f4C;            /* +0x4C */
    u32 h50;            /* +0x50 */
    u32 h54;            /* +0x54 */
    f32 f58;            /* +0x58 */
    f32 f5C;            /* +0x5C */
} Node60;

extern u8 D_003796D0[];
extern u8 D_00252278[];
extern f32 D_00393A2C;
extern f32 D_003939F4;
extern f32 D_003939FC;
extern s32 D_003939CC;
extern f32 D_0039293C, D_00392940, D_00392944, D_00392948, D_0039294C;
extern f32 D_00392950, D_00392954, D_00392958, D_0039295C, D_00392960;
extern f32 D_00392964, D_00392968, D_0039296C, D_00392970, D_00392974;
extern f32 D_00392978, D_0039297C, D_00392980, D_00392984, D_00392988;
extern f32 D_0039298C, D_00392990, D_00392994, D_00392998, D_0039299C;
extern f32 D_003929A0, D_003929A4, D_003929A8, D_003929AC, D_003929B0;

f32 func_0011A788(f32 arg0);
s32 func_002256F0(void);
extern f32 func_00217AC0(f32 arg0);
f32 func_00217CB0(f32 arg0);
s32 func_00156A80(f32 *arg0, u32 arg1, u8 *arg2, s32 arg3, f32 arg4, f32 arg5);
void func_00155698(f32 *arg0, f32 *arg1, u8 *arg2, f32 arg3, f32 arg4);
s32 func_00156E48(f32 *arg0, u32 arg1, u8 *arg2, s32 arg3, f32 arg4, f32 arg5);
void func_001567B0(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, u32 arg4);
s32 func_00157140(f32 *arg0, u32 arg1, u8 *arg2, s32 arg3, f32 *arg4, f32 *arg5,
                  f32 arg6, f32 arg7);
void func_001DC458(f32 *arg0, f32 *arg1, f32 *arg2);
u8 *func_001D46A0(u32 arg0, f32 *arg1, f32 *arg2, u32 arg3, f32 arg4, f32 arg5,
                  f32 arg6);

/* Retail expands `x != 0 ? copysignf(1.0f, x) : 0.0f` inline at four places
   (0x56EC, 0x5728, 0x5820, 0x5854, 0x5D8C, 0x5DCC, 0x5EE0, 0x5F20): it tests
   |x|, then rebuilds 1.0 with x's sign bit.  Kept as one helper. */
/* Retail expands both of these inline at every use (0x56EC, 0x5728, 0x5820,
   0x5854, 0x5D8C, 0x5DCC, 0x5EE0, 0x5F20 for the sign, and the two `lui $1,
   0x3000; mtc1` sequences for the tiny constant).  They have to be macros, not
   helpers: a helper lands at the end of .text, past the last retail function,
   where the per-function padding cannot absorb it and the link overflows into
   .main_bss. */

/* 0.0 for a zero input, otherwise 1.0 carrying the input's sign bit. */
#define SIGN_ONE(x)                                                              ({                                                                               union { f32 f; u32 u; } _s;                                                  _s.f = (x);                                                                  _s.u = (_s.u & 0x7FFFFFFF) ? ((_s.u & 0x80000000) | 0x3F800000) : 0;         _s.f;                                                                    })

/* The float whose bit pattern is `b` (0x30000000 = 4.656613e-10). */
#define BITS_F32(b)                                                              ({                                                                               union { f32 f; u32 u; } _c;                                                  _c.u = (u32)(b);                                                             _c.f;                                                                    })

void func_001D5568(s32 arg0)
{
    Node60 *e;
    f32 *pos;
    f32 *axis;
    f32 *dir;
    f32 *up;
    s32 kind;
    u32 group;
    s32 i;
    f32 f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14;
    f32 f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31;
    f32 zslot;                          /* +0x110($sp): 0.0, kind 8's dz later */
    f32 pz[3];                          /* +0x70 */
    f32 nrm[3];                         /* +0x80 */
    f32 va[3], vb[3], vc[3], vd[3];     /* +0x90, +0xA0, +0xB0, +0xC0 */
    f32 ve[3], vf[3], vg[3], vh[3];     /* +0xD0, +0xE0, +0xF0, +0x100 */

    e = (Node60 *)(D_003796D0 + arg0 * 0x60);
    pos = &e->f1C;
    axis = &e->f10;
    dir = &e->f28;
    up = &e->f34;
    zslot = 0.0f;
    f31 = 0.0f;
    f29 = 0.0f;

    /* 0x001D55E0: single-shot variant (bit 0 of +0x04).  Its own draw first,
       then the record is submitted once and nothing else happens. */
    if ((e->mode & 1) != 0) {
        D_00393A2C = e->f48;
        func_0011A788((e->f58 < 255.0f) ? e->f58 : 255.0f);
        if (func_00156A80(pos, e->h50, D_00252278, e->i08, e->f40, e->f44) != 0)
            e->flags |= 0x01000000;
        return;
    }

    kind = e->flags & 0xFF;

    /* 0x001D5690: kinds 7 and 0x0B sway along a sine of +0x18*(T-+0x0C). */
    if (kind == 7 || kind == 0x0B) {
        f28 = e->f18 * (D_003939F4 - e->f0C);
        f0 = (f32)(s32)(f28 * D_0039293C) * D_00392940;
        f20 = f28 - f0;
        f29 = func_00217AC0(f28);
        f5 = (fabsf(f20) < D_00392944) ? SIGN_ONE(f20) : -SIGN_ONE(f20);
        f2 = f29 * f29;
        f0 = sqrtf(1.0f - f2);
        f31 = f0 * f5;
        f29 = f29 * e->f10;
        f31 = f31 * e->f10;
        f1 = e->f20 + f29;
        f3 = e->f1C + f31;
        f2 = e->f24 + (f31 + f29);
        e->f20 = f1;
        e->f1C = f3;
        e->f24 = f2;
    } else if (kind == 8) {
        /* 0x001D57B0: same sway, phase offset by +0x30. */
        f28 = e->f18 * (D_003939F4 - e->f0C) + e->f30;
        f0 = (f32)(s32)(f28 * D_00392948) * D_0039294C;
        f20 = f28 - f0;
        f2 = func_00217AC0(f28);
        f7 = (fabsf(f20) < D_00392950) ? SIGN_ONE(f20) : -SIGN_ONE(f20);
        f3 = f2 * f2;
        f0 = sqrtf(1.0f - f3) * f7;
        f6 = e->f10 * f2;
        f29 = e->f10 * f2;
        f31 = e->f10 * f0;
        f5 = e->f10 * f0;
        f1 = e->f20 + f29;
        f31 = f31 + f6;
        f2 = e->f24 + f5;
        zslot = f5;
        e->f20 = f1;
        e->f1C = e->f1C + f31;
        e->f24 = f2;
    }

    /* 0x001D58F0: kind 0x0C publishes a fixed 1.0, everything else +0x48. */
    D_00393A2C = (kind == 0x0C) ? 1.0f : e->f48;

    /* 0x001D590C: colour/alpha clamp, then the idle gate. */
    func_0011A788((e->f58 < 255.0f) ? e->f58 : 255.0f);
    if (e->h50 == 0)
        return;

    switch (kind) {
    case 0x11:
        /* 0x001D5958 */
        if (func_00112E38() == 0)
            func_00155698(pos, dir, D_00252278, e->f58, 0.0f);
        e->flags |= 0x01000000;
        break;

    case 0x15:
        /* 0x001D598C: same call, second parameter 255.0. */
        func_00155698(pos, dir, D_00252278, e->f58, 255.0f);
        e->flags |= 0x01000000;
        break;

    case 0x13:
        /* 0x001D5CAC: a chain of up to 40 segments stepping along z, one draw
           per segment, then a ribbon on the second handle (+0x54). */
        f25 = D_00392970;
        /* DIV?MUL by the game's shared 0.1f constant.  Using a literal
           /10.0f makes ee-gcc emit a private .lit4 entry, which (with the
           linker's 16-byte lit4 alignment) overflows .main into .main_bss.
           The reciprocal multiply is the same instruction the original
           division compiled to, so behaviour is identical. */
        f26 = e->f30 * D_00392918;
        f27 = D_00392974;
        f21 = 0.0f;
        f22 = e->f24;
        f24 = e->f58;
        f23 = e->f40;
        if (f25 < e->f28) {
            f0 = f25;
            do {
                f21 = f0;
                f22 = f22 + f26;
                f0 = f21 + f25;
            } while (f0 < e->f28);
        }
        f28 = f24 / 32.0f;
        f0 = (f32)(s32)(f21 * D_00392978) * D_0039297C;
        f20 = f21 - f0;
        f3 = func_00217AC0(f21);
        f6 = (fabsf(f20) < D_00392980) ? SIGN_ONE(f20) : -SIGN_ONE(f20);
        f4 = f3 * f3;
        f7 = sqrtf(1.0f - f4) * f6;
        f5 = f23 * f3;
        f21 = f21 - f25;
        f1 = e->f20 + f5;
        f2 = f22 - f27 * 0.5f;
        f3 = e->f1C + f23 * f7;
        vh[0] = f3;
        vh[1] = f1;
        vh[2] = f2;
        i = 40;
        while (0.0f <= f21) {
            f0 = f21 * D_00392984;
            f22 = f22 - f26;
            vc[0] = vh[0];
            vc[1] = vh[1];
            vc[2] = vh[2];
            f0 = (f32)(s32)f0 * D_00392988;
            f20 = f21 - f0;
            f3 = func_00217AC0(f21);
            f7 = (fabsf(f20) < D_0039298C) ? SIGN_ONE(f20) : -SIGN_ONE(f20);
            f4 = f3 * f3;
            f7 = sqrtf(1.0f - f4) * f7;
            f5 = f23 * f3;
            vh[0] = vc[0];
            vh[1] = vc[1];
            vh[2] = vc[2] - f27;
            f1 = e->f20 + f5;
            f21 = f21 - f25;
            f4 = e->f1C + f23 * f7;
            f24 = f24 - f28;
            va[0] = f4;
            va[1] = f1;
            va[2] = vc[2] - f27;
            vh[2] = f22;
            vh[0] = f4;
            i--;
            func_0011A788(f24);
            func_001567B0(vh, vc, va, vb, e->h50);
            if (!(0.0f <= f21))
                break;
            if (i == 0)
                break;
        }
        /* 0x001D6008 */
        f20 = 0.0f;
        f12 = (f32)func_002256F0() * (e->f58 - f20);
        f12 = f12 * BITS_F32(0x30000000);
        f12 = f12 + f20;
        func_0011A788(f12);
        func_00156A80(vc, e->h54, D_00252278, 1, e->f44, e->f44);
        vd[0] = (vc[0] - e->f1C) * 3.0f;
        vd[1] = (vc[1] - e->f20) * 3.0f;
        vd[2] = 2.0f;
        func_001D46A0(0x4001, vc, vd, e->h54, 0.0f, D_00392990, D_00392990);
        f0 = D_003939FC;
        f21 = f0 * 3.0f;
        f2 = f0 * 0.5f;
        f3 = e->f2C;
        f4 = e->f28;
        f0 = f3 * f0;
        f1 = e->f40;
        f3 = f3 + f21;
        f1 = f1 + f2;
        f4 = f4 + f0;
        e->f2C = f3;
        e->f40 = f1;
        e->f28 = f4;
        break;

    case 0x14:
        /* 0x001D60F8: four vectors straight out of the record. */
        func_001567B0(axis, pos, dir, up, e->h50);
        break;

    default:
        group = e->flags & 0xF0000000;

        if (group == 0x30000000) {
            /* 0x001D59D0: normalise (-+0x2C, -+0x28); a degenerate direction
               retires the record instead. */
            f3 = -e->f2C;
            f5 = e->f28;
            f4 = -f5;
            f0 = f4 * f4 + f3 * f3;
            f1 = sqrtf(f0);
            if (f1 < D_00392954) {
                e->flags |= 0x01000000;
                break;
            }
            f0 = 1.0f / f1;
            va[0] = f3 * f0;
            va[1] = f5 * f0;
            va[2] = 0.0f;
            vb[0] = f4 * f0;
            vb[1] = f3 * f0;
            vb[2] = 0.0f;
            if (func_00157140(pos, e->h50, D_00252278, e->i08,
                              va, vb, e->f40, e->f44) != 0)
                e->flags |= 0x01000000;
            break;
        }

        if (group == 0x80000000) {
            /* 0x001D5A84: a helix.  +0x58 drains by 120*D_003939FC here and by
               1/32 for every segment drawn. */
            f21 = D_003939F4;
            if (e->i08 >= 0) {
                f30 = (f32)e->i08;
            } else {
                u32 u = ((u32)e->i08 >> 1) | ((u32)e->i08 & 1);
                f30 = (f32)(s32)u + (f32)(s32)u;
            }
            f12 = f21 - e->f0C;
            f23 = D_003939FC;
            f5 = e->f1C;
            f6 = e->f20;
            f3 = e->f24;
            f0 = f12 * D_00392958;
            f2 = f23 * 120.0f;
            f0 = f0 + f5;
            f0 = f0 - f6;
            f4 = D_0039295C;
            f1 = e->f58 - f2;
            f24 = 0.0f;
            f3 = f3 * f4;
            e->f58 = f1;
            if (!(f1 > 0.0f)) {
                e->f58 = 0.0f;
                e->flags |= 0x01000000;
            }
            f22 = f0 + f3;
            f21 = f12;
            f20 = f21 + f22;
            f0 = func_00217CB0(f20);
            vc[0] = f30 * f0;
            f0 = func_00217AC0(f21 - f22);
            vc[1] = f30 * f0;
            f0 = func_00217CB0(f21);
            f0 = func_00217AC0(f30 * f22 + f0);
            vc[2] = fabsf(2.0f - f0);
            f0 = func_00217CB0(f21 * D_00392960 + f22);
            f0 = f30 * f0;
            f0 = f0 * D_00392964;
            f0 = f23 * f0;
            e->f1C = e->f1C + f0;
            f0 = func_00217AC0(f21 * D_00392968 + f22 + D_0039296C);
            f0 = f0 * 2.5f;
            f0 = f23 * f0;
            e->f20 = e->f20 + f0;
            f8 = vc[0];
            f9 = vc[1];
            f7 = vc[2];
            f0 = func_00217AC0(f30 * f20);
            f0 = f0 * 4.5f;
            f0 = f0 - 3.5f;
            f0 = f23 * f0;
            f2 = f8 * f8 + f9 * f9;
            f6 = f7 * f7;
            e->f24 = e->f24 + f0;
            f2 = f2 + f6;
            if (f2 == f24)
                break;
            f0 = 1.0f / sqrtf(f2);
            va[0] = f8 * f0;
            va[1] = f9 * f0;
            va[2] = f7 * f0;
            func_001DC458(va, vb, ve);
            func_00157140(pos, e->h50, D_00252278, 1, vb, ve,
                          e->f40, e->f44);
            break;
        }

        if (group == 0x70000000) {
            /* 0x001D6120: cross product of +0x28/+0x2C/+0x30 with
               +0x10/+0x14/+0x18, normalised, then two rings of four points
               around the position, each ring halved into a second quad. */
            f22 = 0.75f;
            f11 = (f32)func_002256F0() * BITS_F32(0x30000000);
            f10 = dir[0];
            f9 = dir[2];
            f8 = dir[1];
            f4 = axis[0];
            f2 = axis[1];
            f12 = f10 * axis[2];
            f1 = f9 * f4;
            f0 = f8 * axis[2];
            f3 = f9 * f2;
            f1 = f1 - f12;
            f2 = f10 * f2;
            f0 = f0 - f3;
            f4 = f8 * f4;
            nrm[1] = f1;
            nrm[0] = f0;
            nrm[2] = f2 - f4;
            f21 = f11 - 0.5f;
            f7 = pos[0] - f10;
            f6 = pos[1] - f8;
            f5 = pos[2] - f9;
            f3 = nrm[0] * nrm[0] + nrm[1] * nrm[1];
            f10 = f10 * f21;
            f8 = f8 * f21;
            f9 = f9 * f21;
            pz[0] = f7 + f10;
            pz[1] = f6 + f8;
            pz[2] = f5 + f9;
            f0 = nrm[2] * nrm[2];
            if (!(D_00392994 < f3 + f0))
                break;
            f0 = 1.0f / sqrtf(f3 + f0);
            for (i = 2; i >= 0; i--)
                nrm[i] = nrm[i] * f0;
            f22 = f22 * D_00392998;
            for (i = 2; i >= 0; i--) {
                f1 = pos[i] + dir[i] * f21;
                f3 = axis[i] * f22;
                f1 = f1 + D_0039299C * nrm[i];
                va[i] = f1 - f3;
                vb[i] = f1 + f3;
                f0 = pz[i] + D_0039299C * nrm[i];
                vc[i] = f0 + f3;
                vd[i] = f0 - f3;
            }
            func_001567B0(vb, vc, vd, va, e->h50);
            for (i = 2; i >= 0; i--) {
                f0 = (vb[i] - va[i]) * 0.5f;
                vf[i] = va[i] - f0;
                vg[i] = vb[i] - f0;
                f0 = (vc[i] - vd[i]) * 0.5f;
                vh[i] = vc[i] - f0;
                ve[i] = vd[i] - f0;
            }
            func_001567B0(vf, vg, vh, ve, e->h50);
            for (i = 2; i >= 0; i--) {
                f1 = pos[i] + dir[i] * f21;
                f3 = axis[i] * f22;
                f1 = f1 - D_0039299C * nrm[i];
                va[i] = f1 - f3;
                vb[i] = f1 + f3;
                f0 = pz[i] - D_0039299C * nrm[i];
                vc[i] = f0 + f3;
                vd[i] = f0 - f3;
            }
            func_001567B0(vb, vc, vd, va, e->h50);
            for (i = 2; i >= 0; i--) {
                f0 = (vb[i] - va[i]) * 0.5f;
                vf[i] = va[i] - f0;
                vg[i] = vb[i] - f0;
                f0 = (vc[i] - vd[i]) * 0.5f;
                vh[i] = vc[i] - f0;
                ve[i] = vd[i] - f0;
            }
            func_001567B0(vf, vg, vh, ve, e->h50);
            break;
        }

        if (group == 0x40000000 || group == 0x50000000 || group == 0x60000000) {
            /* 0x001D65F0: a quad from +0x10/+0x14/+0x18 with a normal built
               from the two half extents.  Group 0x60 has no z extent, group
               0x50 draws a second quad, 0x40 either a third call or a cross
               product depending on D_003939CC. */
            f5 = axis[0];
            f4 = axis[1];
            f3 = axis[2];
            pz[0] = pos[0] - f5;
            pz[1] = pos[1] - f4;
            f6 = (group == 0x60000000) ? 0.0f : (pos[2] - f3);
            pz[2] = f6;
            f7 = pz[0];
            f8 = pz[1];
            f0 = f8 * 0.5f;
            f2 = f7 * 0.5f;
            f1 = f6 * 0.5f;
            nrm[2] = 0.0f;
            va[0] = f5 + f2;
            va[1] = f4 + f0;
            va[2] = f3 + f1;
            nrm[0] = -f8;
            if (group == 0x60000000) {
                nrm[1] = f7;
                f0 = f7 * f7;
                f1 = f8 * f8;
                f2 = f6 * f6;
            } else {
                nrm[1] = f6;
                f0 = f8 * f8;
                f1 = f6 * f6;
                f2 = f7 * f7;
            }
            f5 = f0 + f1;
            if (!(D_003929A0 <= f5))
                break;
            f4 = 1.0f / sqrtf(f5 + f2);
            f9 = f6 * f4;
            f6 = f7 * f4;
            f7 = f8 * f4;
            f5 = 1.0f / sqrtf(f5);
            e->f40 = f2 * 0.5f;
            nrm[1] = nrm[1] * f5;
            nrm[0] = nrm[0] * f5;
            pz[0] = f6;
            pz[1] = f7;
            pz[2] = f9;
            if (group == 0x60000000) {
                func_00157140(va, e->h50, D_00252278, e->i08, nrm, pz,
                              D_003929A4, e->f40);
                break;
            }
            if (group == 0x50000000) {
                func_00157140(va, e->h50, D_00252278, e->i08, nrm, pz,
                              D_003929A8, e->f40);
                func_00157140(va, e->h50, D_00252278, e->i08, ve, pz,
                              D_003929A8, e->f40);
                break;
            }
            if (D_003939CC != 0) {
                func_00157140(va, e->h50, D_00252278, e->i08, nrm, pz,
                              D_003929AC, e->f40);
                break;
            }
            /* 0x001D6858: cross product of nrm and pz, divided by its own
               squared length. */
            ve[0] = nrm[1] * pz[2] - nrm[2] * pz[1];
            ve[1] = nrm[2] * pz[0] - nrm[0] * pz[2];
            ve[2] = nrm[0] * pz[1] - nrm[1] * pz[0];
            f1 = ve[0] * ve[0] + ve[1] * ve[1] + ve[2] * ve[2];
            if (f1 != 0.0f) {
                f0 = 1.0f / f1;
                f4 = ve[0] * f0;
                f3 = ve[1] * f0;
                f2 = ve[2] * f0;
                ve[0] = f4;
                ve[1] = f3;
                ve[2] = f2;
            }
            func_00157140(va, e->h50, D_00252278, e->i08, ve, pz,
                          D_003929B0, e->f40);
            break;
        }

        if (group == 0x20000000) {
            /* 0x001D68E8 */
            func_001DC458(dir, pz, nrm);
            func_00157140(pos, e->h50, D_00252278, e->i08, pz, nrm,
                          e->f40, e->f44);
            break;
        }

        if (group == 0x90000000) {
            /* 0x001D6934: up vector straight up. */
            vb[0] = 0.0f;
            vb[1] = 0.0f;
            vb[2] = 1.0f;
            func_00157140(pos, e->h50, D_00252278, e->i08, up, vb,
                          e->f40, e->f44);
            break;
        }

        if (group == 0x10000000) {
            /* 0x001D6980 */
            if (func_00156E48(pos, e->h50, D_00252278, e->i08,
                              e->f40, e->f44) != 0)
                e->flags |= 0x01000000;
            break;
        }

        /* 0x001D69CC: anything else.  Bit 27 pushes the draw down by +0x44
           and takes it back out afterwards, so the record keeps its z. */
        if ((e->flags & 0x08000000) != 0)
            e->f24 = e->f24 + e->f44;
        if (func_00156A80(pos, e->h50, D_00252278, e->i08,
                          e->f40, e->f44) != 0)
            e->flags |= 0x01000000;
        if ((e->flags & 0x08000000) != 0)
            e->f24 = e->f24 - e->f44;
        break;
    }

    /* 0x001D6A40: take the sway offsets back out, so the position the game
       sees never moved even though the quads were drawn at the offset. */
    kind = e->flags & 0xFF;
    if (kind == 7 || kind == 0x0B) {
        f3 = f29 + f31;
        e->f1C = e->f1C - f31;
        e->f20 = e->f20 - f29;
        e->f24 = e->f24 - f3;
    } else if (kind == 8) {
        e->f1C = e->f1C - f31;
        e->f20 = e->f20 - f29;
        e->f24 = e->f24 - zslot;
    }
}


/* Byte-matching.  arg0[0..2] += arg1[0..2] * arg2, three elements exactly. */
void func_001D6B18(float *arg0, float *arg1, float arg2)
{
    s32 i;

    for (i = 2; i >= 0; i--) {
        *arg0 += *arg1 * arg2;
        arg0++;
        arg1++;
    }
}

/* ===== func_001D6B50: the "state machine" element of D_003796D0 ============
 *
 * Same table as func_001D5568 (301 records of 0x60 bytes, arg0 selects one),
 * but this is the *other* handler: a per-frame state machine that evolves the
 * record (position, direction, two scale slots, life) and only draws through
 * the handle at +0x50 when a draw path supplies one.  There is no `jal` to it
 * anywhere in the image, so it is reached through a function-pointer table.
 *
 * Gate: bit 0x40 of +0x00 freezes the record for this frame (immediate
 * return, no side effect at all).  Otherwise +0x00 & 0xFF selects one of 19
 * per-kind updates (via jtbl_0038E650) and every kind then falls through the
 * shared "life / domain" tail, which also owns the two respawn paths that
 * reference the owner handle D_003939D4.
 *
 * Field roles confirmed by this function (all offsets from the record base):
 *   +0x00  flags   bits 0..7 kind, bit 24 "retire", bit 27 stream respawn,
 *                  bit 19 special stream, bit 11/10/9.. the 0xF00 and 0xF000
 *                  state domains below, bit 6 freeze
 *   +0x04  mode    bit 3 = extra life drain, bit 0 cleared by the respawn
 *   +0x08  param   read as a *float* by kind 0x0C, as an integer elsewhere
 *   +0x0C  f32     phase reference (time stamp of the last domain change)
 *   +0x10/+0x14/+0x18  radius / amplitude vector (kind 6, 8, 9, 0x0A, 0x0C,
 *                      0x0E, 0x12 read or advance it)
 *   +0x1C/+0x20/+0x24  position (the vector handed to the draws)
 *   +0x28/+0x2C/+0x30  direction / velocity
 *   +0x40/+0x44   two scale slots, each reset to 0.01 when it goes <= 0
 *   +0x48  f32     amplitude used by kind 6 and 0x0C (the orbit radius)
 *   +0x4C  f32     inner radius / countdown, clamped by the tail
 *   +0x50  u32     draw handle (only read, never written here)
 *   +0x58  f32     life, driven by the 0xF000 domain switch
 */

/* Every remaining float constant of this function.  Retail reaches all of them
   with `lwc1 %hi/%lo(D)($x)`, i.e. from the literal pool; a local literal would
   add a .lit4 entry and push .main past its retail end (see the build notes for
   func_00136C08).  The literal-pool values are therefore named here. */
extern f32 D_003929B4, D_003929B8, D_003929BC, D_003929C0, D_003929C4;
extern f32 D_003929C8, D_003929CC, D_003929D0, D_003929D4, D_003929D8;
extern f32 D_003929DC, D_003929E0, D_003929E4, D_003929E8, D_003929EC;
extern f32 D_003929F0, D_003929F4, D_003929F8, D_003929FC;
extern f32 D_00392A00, D_00392A04, D_00392A08, D_00392A0C, D_00392A10;
extern f32 D_00392A14, D_00392A18, D_00392A1C, D_00392A20, D_00392A24;
extern f32 D_00392A28, D_00392A2C, D_00392A30, D_00392A34, D_00392A38;
extern f32 D_00392A3C, D_00392A40, D_00392A44, D_00392A48;

void func_001D6B50(s32 arg0)
{
    Node60 *e;
    f32 *axis;
    f32 *pos;
    f32 *dir;
    f32 va[3];
    f32 vb[3];
    f32 f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11;
    f32 s1, c1, s2, c2, s3, c3;
    f32 ph1, ph2, ph3, frac, sgn;
    f32 amp, step, rad, life;
    u32 flags;
    u32 dom;
    s32 kind;
    s32 i;
    s32 hit;

    e = (Node60 *)(D_003796D0 + arg0 * 0x60);
    pos = &e->f1C;
    axis = &e->f10;
    dir = &e->f28;
    flags = e->flags;

    /* 0x001D6B9C: frozen for this frame, nothing at all happens. */
    if ((flags & 0x40) != 0)
        return;

    kind = flags & 0xFF;
    if (kind < 0x13) {
        /* Comparison chain, not a switch: a dense switch makes this compiler
           emit a jump table in .rdata and the image layout has no room for one
           more section (the constraint documented for func_00228510). */

        /* 0x001D6BCC..0x001D6C2C: one shared "impulse then integrate" tail.
           Kind 0x10 takes the 4x gravity kick first, 1 and 4 the plain one,
           5 only the -3.5 kicker, and 3/7/0x0B neither. */
        if (kind == 0x10 || kind == 1 || kind == 4 || kind == 5 ||
            kind == 3 || kind == 7 || kind == 0x0B) {
            if (kind == 0x10)
                e->f30 = e->f30 + (D_003939FC * D_003929B4) * 4.0f;
            if (kind == 0x10 || kind == 1 || kind == 4)
                e->f30 = e->f30 + D_003939FC * D_003929B8;
            if (kind != 3 && kind != 7 && kind != 0x0B)
                e->f30 = e->f30 + D_003939FC * (-3.5f);
            func_001D6B18(pos, dir, D_003939FC);
        }

        /* 0x001D6C44 (kind 0x0E) / 0x001D6CE4 (kind 0x0A): converge every
           component of the position on the direction, 6.5x or 1.5x per frame.
           A component already within 0.1 counts as arrived, and any arrival
           retires the record. */
        else if (kind == 0x0E || kind == 0x0A) {
            f32 tol = (kind == 0x0E) ? D_003929BC : D_003929C0;
            f32 rate = (kind == 0x0E) ? 6.5f : 1.5f;

            hit = 0;
            for (i = 0; i <= 2; i++) {
                f0 = dir[i] - pos[i];
                f3 = fabsf(f0);
                if (!(f3 <= tol)) {
                    axis[i] = pos[i];
                    f1 = (f0 * rate) * D_003939FC;
                    if (f3 <= fabsf(f1)) {
                        pos[i] = dir[i];
                        hit++;
                    } else {
                        pos[i] = pos[i] + f1;
                    }
                } else {
                    hit++;
                }
            }
            if (hit != 0)
                e->flags |= 0x01000000;
        }

        /* 0x001D6D84: the radius vector collapses onto the position, and the
           record is retired once +0x4C passes 19 frames. */
        else if (kind == 0x12) {
            for (i = 0; i <= 2; i++)
                pos[i] = pos[i] + axis[i];
            e->f4C = e->f4C + 1.0f;
            if (19.0f <= e->f4C)
                e->flags |= 0x01000000;
        }

        /* 0x001D6DD4: the direction drifts by dt * -1.6 on all three axes,
           then the position integrates it. */
        else if (kind == 9) {
            f1 = D_003939FC * D_003929C4;
            dir[0] = dir[0] + f1;
            dir[1] = dir[1] + f1;
            dir[2] = dir[2] + f1;
            func_001D6B18(pos, dir, D_003939FC);
        }

        /* 0x001D6E20: the two radii drift by a rate pair chosen from the
           current +0x10 (grow while small, shrink once large). */
        else if (kind == 8) {
            f1 = e->f10;
            if (f1 < 0.25f) {
                f0 = f1 * D_003929C8;
                f2 = D_003929CC;
            } else if (f1 < 0.5f) {
                f0 = f1 * D_003929D0;
                f2 = D_003929D4;
            } else {
                f0 = f1 * D_003929D8;
                f2 = D_003929DC;
            }
            e->f10 = f0;
            e->f14 = e->f14 * f2;
        }

        /* 0x001D6E90: an orbiting particle.  +0x4C is the radius and shrinks
           by dt per frame; +0x08 is advanced as a *float* phase and feeds
           +0x48, which is the phase of the sine/cosine pair.  The position is
           rebuilt from a basis supplied by func_001DC458, so the orbit plane
           follows +0x28. */
        else if (kind == 0x0C) {
            f32 phs = *(f32 *)&e->i08;

            step = D_003939FC;
            amp = e->f48;
            f2 = (step * 6.5f) * phs;
            f0 = step * D_003929E0;
            e->f4C = e->f4C - step;
            phs = phs + f0;
            amp = amp + f2;
            *(f32 *)&e->i08 = phs;
            e->f48 = amp;
            if (e->f4C <= D_003929E4)
                e->flags |= 0x01000000;

            s1 = func_00217AC0(amp);
            frac = amp - (f32)(s32)(amp * D_003929E8) * D_003929EC;
            sgn = (fabsf(frac) < D_003929F0) ? SIGN_ONE(frac) : -SIGN_ONE(frac);
            c1 = sqrtf(1.0f - s1 * s1) * sgn;

            rad = e->f4C;
            f3 = s1 * rad;
            f4 = c1 * rad;
            func_001DC458(dir, va, vb);
            pos[0] = (e->f10 + va[0] * f4) + vb[0] * f3;
            pos[1] = (e->f14 + va[1] * f4) + vb[1] * f3;
            pos[2] = (e->f18 + va[2] * f4) + vb[2] * f3;
        }

        /* 0x001D7048: three phases built from the time stamp and the three
           amplitude/frequency slots, combined into an orbit contribution that
           is added to the direction while +0x48 decays as the amplitude.
           +0x4C drains by 4.5*dt per frame and retires the record at 0.01. */
        else if (kind == 6) {
            f32 t = D_003939F4;

            ph1 = e->f10 * t;
            ph3 = e->f18 * t;
            ph2 = e->f14 * t;
            amp = e->f48;
            life = e->f4C;

            frac = ph1 - (f32)(s32)(ph1 * D_003929F4) * D_003929F8;
            s1 = func_00217AC0(ph1);
            sgn = (fabsf(frac) < D_00392A00) ? SIGN_ONE(frac) : -SIGN_ONE(frac);
            c1 = sqrtf(1.0f - s1 * s1) * sgn;

            frac = ph2 - (f32)(s32)(ph2 * D_00392A04) * D_00392A08;
            s2 = func_00217AC0(ph2);
            sgn = (fabsf(frac) < D_00392A0C) ? SIGN_ONE(frac) : -SIGN_ONE(frac);
            c2 = sqrtf(1.0f - s2 * s2) * sgn;

            frac = ph3 - (f32)(s32)(ph3 * D_00392A10) * D_00392A14;
            s3 = func_00217AC0(ph3);
            sgn = (fabsf(frac) < D_00392A18) ? SIGN_ONE(frac) : -SIGN_ONE(frac);
            c3 = sqrtf(1.0f - s3 * s3) * sgn;

            f7 = s1 * s3;
            f5 = (-c2) * s1;
            f10 = c1 * s3;
            f5 = f5 * s3;
            f2 = -c3;
            f6 = s1 * c3;
            f1 = c1 * c3;
            f2 = f2 * c2;
            f6 = f5 - f6;
            f9 = c1 * c2;
            f4 = f2 * c1;
            f3 = s2 * s3;
            f2 = f2 * s1;
            f4 = f4 + f7;
            f0 = s2 * c3;
            f5 = f5 + f1;
            f2 = f2 - f10;
            f7 = s1 * s2;
            f3 = f3 * D_003929FC;
            f6 = f6 * 0.5f;
            f0 = f0 * D_003929FC;
            f4 = f4 * 0.5f;
            f1 = c2 * D_003929FC;
            f9 = f9 * 0.5f;
            f3 = f3 + f6;
            f0 = f0 + f4;
            f1 = f1 + f9;
            f7 = f7 * 0.5f;
            f5 = f5 * 0.5f;
            f2 = f2 * 0.5f;
            f1 = f1 + f7;
            f3 = f3 + f5;
            f0 = f0 + f2;
            dir[0] = dir[0] + amp * f3;
            dir[1] = dir[1] + amp * f0;
            dir[2] = dir[2] + amp * f1;
            life = amp - D_003939FC * 4.5f;
            e->f4C = life;
            if (life <= D_00392A1C)
                e->flags |= 0x01000000;
        }
    }

    /* ---- 0x001D73EC: the two "id 7" stream respawns ----------------------
       +0x20000 marks a record owned by the remembered spawner: when the
       position reaches the inner radius (+0x4C) on the z axis it is re-armed
       as a fresh 0x10003100 record.  The 0x2000000 variant only fires with a
       50% chance and re-arms as 0x10004100 instead. */
    flags = e->flags;
    if ((flags & 0x20000) == 0 || D_003939D4 == 0) {
        if ((flags & 0x2000000) != 0 && D_003939D4 != 0 &&
            (flags & 0x80000) == 0 && e->f24 <= e->f4C) {
            if (0.5f < (f32)func_002256F0() * BITS_F32(0x30000000)) {
                e->f24 = e->f4C;
                e->flags = 0x10004100;
                e->f44 = D_00392A24;
                e->f0C = D_003939F4;
                e->f58 = 150.0f;
                e->h50 = D_003939D4;
                e->mode = 0;
                e->f40 = D_00392A24;
                flags = 0x10004100;
            } else {
                flags = e->flags | 0x01000000;
                e->flags = flags;
            }
        }
    } else if ((flags & 0x80000) == 0 && e->f24 <= e->f4C) {
        e->f24 = e->f4C;
        e->flags = 0x10003100;
        e->f44 = D_00392A20;
        e->f0C = D_003939F4;
        e->f58 = 250.0f;
        e->h50 = D_003939D4;
        e->f40 = D_00392A20;
        flags = 0x10003100;
    }

    /* ---- 0x001D7538: the 0xF00 scale-site domain -------------------------
       Most domains just add the same per-frame amount to both scale slots;
       0x900 is asymmetric and 0xA00/0xB00/0xD00 are a three-step climb that
       ends by retiring the record. */
    dom = flags & 0xF00;
    f5 = 0.0f;
    switch (dom) {
    case 0x100:
        f5 = D_00392A30;
        break;
    case 0x200:
        f5 = D_00392A34;
        break;
    case 0x300:
        f5 = 6.5f;
        break;
    case 0x400:
        f5 = 9.5f;
        break;
    case 0x500:
        f5 = -0.5f;
        break;
    case 0x600:
        f5 = D_00392A38;
        break;
    case 0x700:
        f5 = -3.5f;
        break;
    case 0x800:
        f5 = D_00392A2C;
        break;
    case 0x900:
        e->f40 = e->f40 + D_003939FC * D_00392A28;
        e->f44 = e->f44 + D_003939FC * 20.5f;
        break;
    case 0xA00:
        e->f40 = e->f40 + D_003939FC * 60.0f;
        e->f44 = e->f44 - D_003939FC * 7.5f;
        if (5.0f <= e->f40) {
            flags = (e->flags & ~0xF000) | 0xB00;
            e->flags = flags;
        }
        break;
    case 0xB00:
        e->f40 = e->f40 - D_003939FC * 60.0f;
        if (e->f40 <= 1.0f) {
            e->f40 = 2.0f;
            flags = (e->flags & ~0xF000) | 0xD00;
            e->flags = flags;
        }
        break;
    case 0xD00:
        e->f44 = e->f44 + D_003939FC * 61.5f;
        e->f40 = e->f40 - D_003939FC * 16.5f;
        if (10.0f <= e->f44) {
            flags = e->flags | 0x01000000;
            e->flags = flags;
        }
        break;
    default:
        break;
    }

    if (f5 != 0.0f) {
        f1 = f5 * D_003939FC;
        e->f40 = e->f40 + f1;
        e->f44 = e->f44 + f1;
    }

    /* 0x001D77C0: the 0xE00 domain replaces both scale slots with a random
       0..1 value every frame. */
    if ((flags & 0xF00) == 0xE00) {
        f0 = (f32)func_002256F0() * BITS_F32(0x30000000) + 0.0f;
        flags = e->flags;
        e->f44 = f0;
        e->f40 = f0;
    }

    /* 0x001D77FC: a scale slot that fell to zero is reset to 0.01 and the
       record is retired. */
    if (e->f40 <= 0.0f) {
        e->f40 = D_00392A3C;
        flags = flags | 0x01000000;
        e->flags = flags;
    }
    if (e->f44 <= 0.0f) {
        e->f44 = D_00392A40;
        flags = flags | 0x01000000;
        e->flags = flags;
    }

    /* ---- 0x001D7848: the 0xF000 life domain ------------------------------
       Each domain drains +0x58 at its own rate; 0x6000 is the only one that
       can retire on its own. */
    switch (flags & 0xF000) {
    case 0x1000:
        e->f58 = e->f58 - D_003939FC * 100.0f;
        break;
    case 0x2000:
        e->f58 = e->f58 - D_003939FC * 160.0f;
        break;
    case 0x3000:
        e->f58 = e->f58 - D_003939FC * 260.0f;
        break;
    case 0x4000:
        e->f58 = e->f58 - D_003939FC * 350.0f;
        break;
    case 0x5000:
        e->f58 = e->f58 - D_003939FC * D_00392A44;
        break;
    case 0x6000:
        e->f58 = e->f58 - D_003939FC * D_00392A48;
        if (e->f58 < 5.0f) {
            e->f58 = 0.0f;
            flags = flags | 0x01000000;
            e->flags = flags;
        }
        break;
    case 0x7000:
        e->f58 = e->f58 + D_003939FC * 350.0f;
        if (255.0f <= e->f58) {
            e->f58 = 255.0f;
            e->f0C = D_003939F4;
            flags = (e->flags & ~0xF000) | 0x8000;
            e->flags = flags;
        }
        break;
    case 0x8000:
        /* 0x001D78EC: the fade finishes one +0x4C after +0x0C elapsed. */
        if (e->f4C < D_003939F4 - e->f0C) {
            flags = (e->flags & ~0xF000) | 0x3000;
            e->flags = flags;
        }
        break;
    case 0x9000:
        e->f58 = (f32)func_002256F0() * 155.0f * BITS_F32(0x30000000) + 100.0f;
        break;
    case 0xA000:
        e->f58 = e->f58 + D_003939FC * 100.0f;
        break;
    case 0xB000:
        e->f58 = e->f58 - D_003939FC * 500.0f;
        break;
    default:
        break;
    }

    /* 0x001D7AA8: mode bit 3 adds a constant drain on top of the domain. */
    if ((e->mode & 8) != 0)
        e->f58 = e->f58 - D_003939FC * 20.0f;

    /* 0x001D7ACC: negative life retires the record, except in domain 0x7000
       (that one is climbing and is allowed to start below zero). */
    if (e->f58 < 0.0f) {
        flags = e->flags;
        if ((flags & 0xF000) != 0x7000) {
            e->f58 = 0.0f;
            e->flags = flags | 0x01000000;
        }
    }
}

INCLUDE_ASM(const s32, "core/main", func_001D7B38);

INCLUDE_ASM(const s32, "core/main", func_001D7DB8);

INCLUDE_ASM(const s32, "core/main", func_001D7FD0);

INCLUDE_ASM(const s32, "core/main", func_001D81B0);

INCLUDE_ASM(const s32, "core/main", func_001D8318);

INCLUDE_ASM(const s32, "core/main", func_001D84B0);

INCLUDE_ASM(const s32, "core/main", func_001D8770);

INCLUDE_ASM(const s32, "core/main", func_001D8908);

INCLUDE_ASM(const s32, "core/main", func_001D8AD8);

INCLUDE_ASM(const s32, "core/main", func_001D8CD8);

INCLUDE_ASM(const s32, "core/main", func_001D8EE8);

INCLUDE_ASM(const s32, "core/main", func_001D9170);

INCLUDE_ASM(const s32, "core/main", func_001D92C8);

INCLUDE_ASM(const s32, "core/main", func_001D9430);

INCLUDE_ASM(const s32, "core/main", func_001D95B8);

void func_001D9618(f32 *a1)
{
    f32 x = a1[0];
    f32 y = a1[1];
    f32 sum = x * x + y * y;
    f32 r;

    if (sum == 0.0f)
        return;
    __asm__ volatile("li.s $f0, 1.0\n\trsqrt.s $f0, $f0, %1" : "=f"(r) : "f"(sum));
    a1[0] = x * r;
    a1[1] = y * r;
}

INCLUDE_ASM(const s32, "core/main", func_001D9660);

INCLUDE_ASM(const s32, "core/main", func_001D9890);

INCLUDE_ASM(const s32, "core/main", func_001D9E70);

INCLUDE_ASM(const s32, "core/main", func_001DA138);

INCLUDE_ASM(const s32, "core/main", func_001DA360);


void func_001DA360(f32 *pos, f32 *dir);
void func_001DB8E0(s32 id, f32 *a, f32 *b, u32 owner, f32 scale);
/* The ten specialised spawners all take (a, b, owner, scale): every call site
   loads $4 = a, $5 = b, $6 = owner and $f12 = scale, in that order. */
void func_001D7B38(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D7DB8(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D7FD0(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D81B0(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D8318(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D84B0(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D8770(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D8AD8(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001D8EE8(f32 *a, f32 *b, u32 owner, f32 scale);
void func_001DA138(f32 *a, f32 *b, u32 owner, f32 scale);

extern f32 D_00392B9C, D_00392BA0, D_00392BA4, D_00392BA8, D_00392BAC, D_00392BB0;
extern f32 D_00392BB4, D_00392BB8, D_00392BBC, D_00392BC0, D_00392BC4, D_00392BC8;
extern f32 D_00392BCC, D_00392BD0, D_00392BD4, D_00392BD8, D_00392BDC, D_00392BE0;
extern f32 D_00392BE4, D_00392BE8, D_00392BEC, D_00392BF0, D_00392BF4, D_00392BF8;
extern f32 D_00392BFC, D_00392C00, D_00392C04, D_00392C08, D_00392C0C, D_00392C10;
extern f32 D_00392C14, D_00392C18, D_00392C1C, D_00392C20, D_00392C24, D_00392C28;
extern f32 D_00392C2C, D_00392C30, D_00392C34, D_00392C38, D_00392C3C, D_00392C40;
extern f32 D_00392C44, D_00392C48, D_00392C4C, D_00392C50, D_00392C54, D_00392C58;
extern f32 D_00392C5C, D_00392C60, D_00392C64, D_00392C68;
extern u32 D_003939D4;          /* owner remembered by id 7  */
extern u32 D_003939D8;          /* owner remembered by id 50 */
extern s8  D_003939E9;          /* spawn-id selector, id 7   */
extern f32 D_00393A04, D_00393A1C, D_00393A20, D_00393A24;
extern u32 D_00393A18;          /* 5-slot divider for id 175  */
extern f32 D_003807E0[3];
extern f32 D_003807F0[3];

void func_001DA3F0(s32 id, f32 *a, f32 *b, u32 owner, f32 scale)
{
    f32 pos[3];                 /* +0x00 */
    f32 dir[3];                 /* +0x10 */
    f32 tan1[3];                /* +0x20 */
    f32 tan2[3];                /* +0x30 */
    f32 rnd[3];                 /* +0x40 */
    f32 org[3];                 /* +0x50 */
    u8 *e;
    f32 now, s, d, span, base, sum, inv, sn, cs, ang, r1, r2;
    f32 tiny;                   /* 2^-31, the RNG scale */
    f32 half, two;
    s32 i, n, cnt, k;

    tiny = BITS_F32(0x30000000);
    half = 0.5f;
    two = 2.0f;
    D_00393A2C = 1.0f;
    pos[0] = a[0];
    pos[1] = a[1];
    pos[2] = a[2];
    dir[0] = b[0];
    dir[1] = b[1];
    dir[2] = b[2];

    /* ---- 175: rate-limited radial burst around *b ----------------------
       Counter D_00393A18 increments and returns while < 4, so the burst
       fires on the 5th call of every 5. */
    if (id == 175) {
        if (D_00393A18 < 4) {
            D_00393A18 = D_00393A18 + 1;
            return;
        }
        D_00393A18 = 0;
        if (scale == 0.0f)
            return;
        scale = scale * half;

        /* org = *b; dir = *b - *a (then normalised) */
        sum = 0.0f;
        for (i = 2; i >= 0; i--) {
            org[i] = dir[i];
            d = dir[i] - pos[i];
            dir[i] = d;
            sum = sum + d * d;
        }
        if (sum == 0.0f)
            return;
        inv = 1.0f / sqrtf(sum);
        for (i = 2; i >= 0; i--)
            dir[i] = dir[i] * inv;

        /* two tangents of that direction */
        func_001DC458(dir, tan1, tan2);

        for (n = 0; n < 12; n++) {
            sum = 0.0f;
            for (i = 2; i >= 0; i--) {
                r1 = (f32)func_002256F0() * 16.0f * tiny + (-8.0f);
                r2 = (f32)func_002256F0() * 16.0f * tiny + (-8.0f);
                d = tan1[i] * r1 + tan2[i] * r2;
                rnd[i] = d;
                sum = sum + d * d;
            }
            if (sum != 0.0f) {
                f32 f = (1.0f / sqrtf(sum)) * scale;

                for (i = 2; i >= 0; i--)
                    rnd[i] = org[i] + rnd[i] * f;
            }
            s = (f32)func_002256F0() * D_00392B9C * tiny + D_00392BA0;
            /* position is the computed point, direction is the caller's *a */
            e = func_001D46A0(0x12, rnd, pos, owner, two, s, s);
            if (e != 0)
                *(s32 *)(e + 0x04) = 1;
        }
        return;
    }

    /* ---- 1: one spawn, both small scales are scale + 1 ------------------ */
    if (id == 1) {
        s = scale + 1.0f;
        func_001D46A0(0x10000, pos, 0, owner, two, s, s);
        return;
    }

    /* ---- delegated to the specialised spawners ------------------------- */
    if (id == 5) {
        func_001D7B38(a, b, owner, scale);
        return;
    }
    if (id == 8) {
        func_001D7DB8(a, b, owner, scale);
        return;
    }
    if (id == 15) {
        func_001D8770(a, b, owner, scale);
        return;
    }
    if (id == 21) {
        func_001D8AD8(a, b, owner, scale);
        return;
    }
    if (id == 28) {
        func_001D8EE8(a, b, owner, scale);
        return;
    }
    if (id == 35) {
        func_001D84B0(a, b, owner, scale);
        return;
    }
    if (id == 52) {
        func_001D7FD0(a, b, owner, scale);
        return;
    }
    if (id == 60) {
        func_001D81B0(a, b, owner, scale);
        return;
    }
    if (id == 62) {
        func_001D8318(a, b, owner, scale);
        return;
    }
    if (id == 67) {
        func_001DA138(a, b, owner, scale);
        return;
    }

    /* ---- 7: 0.2 s timer, one 0x10003x00 spawn --------------------------- */
    if (id == 7) {
        D_003939D4 = owner;
        now = func_0014B8E0();
        /* the sample store sits in the branch delay slot: always executed */
        D_003939F4 = now;
        if (D_00393A04 == 0.0f)
            D_00393A04 = now;
        if (now < D_00393A04)
            return;
        D_00393A04 = now + D_00392BA4;
        /* 300.0, not 150.0: `lui $1, 0x43960000` at 0x001DA7A0 */
        if (300.0f < dir[0])
            dir[0] = 250.0f;
        if (D_003939E9 != 0)
            func_001D46A0(0x10003100, pos, dir, owner, dir[0], dir[2], dir[2]);
        else
            func_001D46A0(0x10003300, pos, dir, owner, dir[0], dir[2], dir[2]);
        return;
    }

    /* ---- 9: one 0x4500 then eight 0xB001 ------------------------------- */
    if (id == 9) {
        s = (f32)func_002256F0() * D_00392BA8 * tiny + D_00392BA8;
        e = func_001D46A0(0x4500, pos, 0, owner, scale, s, s);
        if (e != 0)
            *(s32 *)(e + 0x04) = 1;

        for (n = 0; n < 8; n++) {
            dir[0] = (f32)func_002256F0() * 4.0f * tiny + (-two);
            dir[1] = (f32)func_002256F0() * 4.0f * tiny + (-two);
            dir[2] = (f32)func_002256F0() * D_00392BAC * tiny + D_00392BB0;
            s = (f32)func_002256F0() * D_00392BB4 * tiny + D_00392BB4;
            e = func_001D46A0(0xB001, pos, dir, owner, scale, s, s);
            if (e != 0)
                *(s32 *)(e + 0x04) = 1;
        }
        return;
    }

    /* ---- 10: 0.3 s timer, one 0xB501 with a zero direction -------------- */
    if (id == 10) {
        now = func_0014B8E0();
        D_003939F4 = now;
        if (D_00393A1C == 0.0f)
            D_00393A1C = now;
        if (now < D_00393A1C)
            return;
        D_00393A1C = now + D_00392BB8;
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        dir[2] = 0.0f;
        func_001D46A0(0xB501, pos, dir, owner, scale, D_00392BBC, D_00392BBC);
        return;
    }

    /* ---- 11: "circle" emitter, count = (s32)b[1] ------------------------
       The angle step is an **integer** division (i / cnt), so every
       component lands on the same angle; kept exactly as retail encodes it.
       pos[] is rebuilt from *a on each iteration, not accumulated. */
    if (id == 11) {
        cnt = (s32)b[1];
        if (cnt <= 0)
            return;
        for (i = 0; i < cnt; i++) {
            d = (f32)(i / cnt) * D_00392BC0;
            k = (s32)(d * D_00392BC4);
            ang = d - (f32)k * D_00392BC8;

            sn = func_00217AC0(d);
            cs = sqrtf(1.0f - sn * sn);
            cs = cs * ((fabsf(ang) < D_00392BCC) ? SIGN_ONE(ang) : -SIGN_ONE(ang));

            r1 = (f32)func_002256F0() * D_00392BD4 * tiny;

            pos[0] = a[0] + b[0] * sn;
            pos[1] = a[1] + b[0] * cs;
            pos[2] = a[2];
            dir[0] = b[0] * sn;
            dir[1] = b[0] * cs;
            dir[2] = D_00392BD8;            /* 1.53, a constant */
            D_00393A2C = r1 + D_00392BD0;

            s = (f32)func_002256F0() * tiny + D_00392BD0;
            func_001D46A0(0x3303, pos, dir, owner, scale, s, s);
        }
        return;
    }

    /* ---- 19 / 20: normalise *b in place, then one spawn ----------------- */
    if (id == 19) {
        sum = b[0] * b[0] + b[1] * b[1] + b[2] * b[2];
        if (sum <= D_00392BDC)
            return;
        inv = 1.0f / sqrtf(sum);
        d = b[2] * inv;
        b[2] = d;
        d = b[0] * inv;
        b[0] = d;
        d = b[1] * inv;
        b[1] = d;
        dir[0] = b[0];
        dir[1] = b[1];
        dir[2] = b[2];
        e = func_001D46A0(0x20004200, pos, dir, owner, scale,
                          D_00392BE0, D_00392BE0);
        if (e != 0)
            *(f32 *)(e + 0x58) = 170.0f;
        return;
    }

    if (id == 20) {
        sum = b[0] * b[0] + b[1] * b[1] + b[2] * b[2];
        if (sum <= D_00392BE4)
            return;
        inv = 1.0f / sqrtf(sum);
        d = b[2] * inv;
        b[2] = d;
        d = b[0] * inv;
        b[0] = d;
        d = b[1] * inv;
        b[1] = d;
        dir[0] = b[0];
        dir[1] = b[1];
        dir[2] = b[2];
        func_001D46A0(0x20005400, pos, dir, owner, scale, half, half);
        return;
    }

    /* ---- 23: rotate, jitter, one 0x0E ---------------------------------- */
    if (id == 23) {
        func_001DA360(pos, dir);
        dir[0] = a[0];
        dir[1] = a[1];
        dir[2] = a[2];
        s = (f32)func_002256F0() * D_00392BE8 * tiny + D_00392BEC;
        for (i = 2; i >= 0; i--) {
            d = (f32)func_002256F0();
            pos[i] = dir[i] + (d + d) * tiny + (-1.0f);
        }
        func_001D46A0(0x0E, pos, dir, owner, scale, s, s);
        return;
    }

    /* ---- 26: 0.5 s timer, up to three 0x40807 ---------------------------
       The gate and the ladder both run on the **position** copy (sp+0x08 is
       pos[2], not dir[2]), and dir[2] is 2r*2^-15 + 2.0. */
    if (id == 26) {
        now = func_0014B8E0();
        D_003939F4 = now;
        if (D_00393A20 == 0.0f)
            D_00393A20 = now;
        if (now < D_00393A20)
            return;
        D_00393A20 = D_003939F4 + (f32)func_002256F0() * half * tiny;
        if (scale < pos[2])
            return;

        s = (f32)func_002256F0() * D_00392BF0 * tiny + D_00392BF4;
        dir[0] = (f32)func_002256F0() * two * tiny + (-1.0f);
        dir[1] = (f32)func_002256F0() * two * tiny + (-1.0f);
        dir[2] = (f32)func_002256F0() * two * tiny + two;
        func_001D46A0(0x40807, pos, dir, owner, scale, s, s);

        pos[2] = pos[2] + D_00392BF8;
        pos[1] = pos[1] + D_00392BF8;
        if (pos[2] < scale)
            func_001D46A0(0x40807, pos, dir, owner, scale, s, s);

        pos[2] = pos[2] + D_00392BFC;
        pos[1] = pos[1] - half;
        if (pos[2] < scale)
            func_001D46A0(0x40807, pos, dir, owner, scale, s, s);
        return;
    }

    /* ---- 27: 0.5 s timer, scale clamped to 7.0, three 0x10807 ----------- */
    if (id == 27) {
        now = func_0014B8E0();
        D_003939F4 = now;
        if (D_00393A24 == 0.0f)
            D_00393A24 = now;
        if (now < D_00393A24)
            return;
        D_00393A24 = D_003939F4 + (f32)func_002256F0() * half * tiny;
        if (7.0f < scale)
            scale = 7.0f;

        s = (f32)func_002256F0() * D_00392C00 * tiny + D_00392C04;
        dir[0] = (f32)func_002256F0() * two * tiny + (-1.0f);
        dir[1] = (f32)func_002256F0() * two * tiny + (-1.0f);
        dir[2] = (f32)func_002256F0() * two * tiny + two;
        func_001D46A0(0x10807, pos, dir, owner, scale, s, s);

        pos[2] = pos[2] + D_00392C08;
        pos[1] = pos[1] + D_00392C08;
        func_001D46A0(0x10807, pos, dir, owner, scale, s, s);

        pos[1] = pos[1] - half;
        pos[2] = pos[2] + D_00392C0C;
        func_001D46A0(0x10807, pos, dir, owner, scale, s, s);
        return;
    }

    /* ---- 30: one 0x4500 then (s32)scale x 0x3001 ------------------------ */
    if (id == 30) {
        n = (s32)scale;
        s = (f32)func_002256F0() * D_00392C10 * tiny + half;
        func_001D46A0(0x4500, pos, 0, owner, scale, s, s);
        if (n == 0)
            return;
        do {
            dir[0] = (f32)func_002256F0() * 4.0f * tiny + (-two);
            dir[1] = (f32)func_002256F0() * 4.0f * tiny + (-two);
            dir[2] = (f32)func_002256F0() * D_00392C14 * tiny + D_00392C18;
            s = (f32)func_002256F0() * D_00392C1C * tiny + D_00392C20;
            func_001D46A0(0x3001, pos, dir, owner, scale, s, s);
            n--;
        } while (n != 0);
        return;
    }

    /* ---- 33: one 0x3000, offset rotated by *b around z ------------------ */
    if (id == 33) {
        d = (f32)func_002256F0() * (scale + scale) * tiny - scale;
        pos[1] = pos[1] + b[0] * d;
        pos[0] = pos[0] - b[1] * d;
        func_001D46A0(0x3000, pos, 0, owner, two, two, two);
        return;
    }

    /* ---- 39: one 0x08001100 -------------------------------------------- */
    if (id == 39) {
        s = b[1] * half;
        func_001D46A0(0x08001100, pos, 0, owner, D_00392C24, s, s);
        return;
    }

    /* ---- 42: one 0x103 with a randomised D_00393A2C --------------------- */
    if (id == 42) {
        D_00393A2C = (f32)func_002256F0() * D_00392C28 * tiny + D_00392C2C;
        s = (f32)func_002256F0() * D_00392C38 * tiny + D_00392C3C;
        pos[0] = pos[0] + ((f32)func_002256F0() * two * tiny + (-1.0f));
        pos[1] = pos[1] + ((f32)func_002256F0() * two * tiny + (-1.0f));
        dir[0] = (f32)func_002256F0() * D_00392C30 * tiny + D_00392C34;
        dir[1] = (f32)func_002256F0() * D_00392C30 * tiny + D_00392C34;
        dir[2] = (f32)func_002256F0() * tiny + 1.0f;
        func_001D46A0(0x103, pos, dir, owner, D_00392C40, s, s);
        return;
    }

    /* ---- 45: six 0x2501, every span scaled by `scale` ------------------- */
    if (id == 45) {
        base = scale * (-two);
        span = (scale + scale) - base;
        for (n = 0; n < 6; n++) {
            dir[0] = (f32)func_002256F0() * span * tiny + base;
            dir[1] = (f32)func_002256F0() * span * tiny + base;
            d = scale * D_00392C44;
            span = (scale * 6.0f) - d;
            dir[2] = (f32)func_002256F0() * span * tiny + d;
            d = scale * D_00392C4C;
            span = (scale * D_00392C48) - d;
            s = (f32)func_002256F0() * span * tiny + d;
            func_001D46A0(0x2501, pos, dir, owner, scale, s, s);
        }
        return;
    }

    /* ---- 46: two 0x100 with D_00393A2C 0.7 then 1.25 -------------------- */
    if (id == 46) {
        s = scale * half;
        D_00393A2C = D_00392C54;
        func_001D46A0(0x100, pos, 0, owner, D_00392C50, s, s);

        base = scale * (-two);
        span = (scale + scale) - base;
        pos[0] = pos[0] + ((f32)func_002256F0() * span * tiny + base);
        D_00393A2C = 1.25f;
        pos[1] = pos[1] + ((f32)func_002256F0() * span * tiny + base);
        func_001D46A0(0x100, pos, 0, owner, D_00392C50, s, s);
        return;
    }

    /* ---- 47 / 49: handled, but nothing to do --------------------------- */
    if (id == 47 || id == 49) {
        return;
    }

    /* ---- 48: one of two spawn ids around scale 11.0 --------------------- */
    if (id == 48) {
        if (11.0f <= scale) {
            s = scale * D_00392C58;
            func_001D46A0(0x10002400, pos, 0, owner, D_00392C5C, s, s);
        } else {
            s = scale * D_00392C60;
            func_001D46A0(0x10003400, pos, 0, owner, D_00392C64, s, s);
        }
        return;
    }

    /* ---- 50: remember this frame, spawn nothing ------------------------ */
    if (id == 50) {
        D_003807E0[0] = pos[0];
        D_003807E0[1] = pos[1];
        D_003807E0[2] = pos[2];
        D_003939D8 = owner;
        D_003807F0[0] = dir[0];
        D_003807F0[1] = dir[1];
        D_003807F0[2] = dir[2];
        return;
    }

    /* ---- 51: one 0x50005000 carrying *b at +0x10 ----------------------- */
    if (id == 51) {
        e = func_001D46A0(0x50005000, pos, 0, owner, D_00392C68, 1.0f, 1.0f);
        if (e != 0) {
            for (i = 2; i >= 0; i--)
                *(f32 *)(e + 0x10 + i * 4) = dir[i];
        }
        return;
    }

    /* ---- 55: (s32)b[1] + 1 random directions, f13/f14 = b[0] ----------- */
    if (id == 55) {
        cnt = (s32)b[1] + 1;
        s = dir[0];
        if (cnt == 0)
            return;
        do {
            for (i = 2; i >= 0; i--)
                dir[i] = (f32)func_002256F0() * (scale + scale) * tiny - scale;
            e = func_001D46A0(0x3003, pos, dir, owner, 1.0f, s, s);
            if (e != 0)
                *(s32 *)(e + 0x04) = 1;
            cnt--;
        } while (cnt != 0);
        return;
    }

    /* ---- everything else ---------------------------------------------- */
    func_001DB8E0(id, a, b, owner, scale);
    return;
}

INCLUDE_ASM(const s32, "core/main", func_001DB8E0);

INCLUDE_ASM(const s32, "core/main", func_001DC458);

void func_001DC5B0(u8 *dst, u8 *src)
{
    s32 i = 0;

    if (*src != 0) {
        do {
            dst[i] = src[i];
        } while (src[++i] != 0);
    }
    dst[i] = 0;
}

void func_001DC5F8(u8 *dst, s8 *src)
{
    s32 i = 0;

    if (*src != 0) {
        do {
            dst[i] = (u8)src[i];
        } while (src[++i] != 0);
    }
    dst[i] = 0;
}

void func_001DC640(void *dst, s32 src)
{
    s32 i = 0;

    if (((u8 *)src)[0] != 0) {
        do {
            ((u8 *)dst)[i] = ((u8 *)src)[i];
        } while (((u8 *)src)[++i] != 0);
    }
    ((u8 *)dst)[i] = 0;
}

/* Byte-matching.  String length with boundary re-tested each step. */
s32 func_001DC688(u8 *arg0)
{
    s32 n = 0;

    if (arg0[0] != 0) {
        do {
            n++;
        } while (arg0[n] != 0);
    }
    return n;
}

/* Byte-matching.  Builds a 0x260-byte record on the stack, fills it, then hands
   it to func_002220A0. */
void func_001DC6C0(s32 arg0)
{


    u8 buf[0x260];

    func_001DC640(buf, arg0);
    func_002220A0(buf);
}

INCLUDE_ASM(const s32, "core/main", func_001DC6E8);

extern s32 D_00380CD8[];

s32 func_001DC728(void)
{
    u32 base = (u32)D_00380CD8;
    s32 i = 0x14;
    s32 *p = (s32 *)(base + 0x370);

    do {
        if (*p == 0)
            return i;
        i++;
        p = (s32 *)((u8 *)p + 0x2C);
    } while (i < 0x28);
    return -1;
}

extern s32 D_00380CD8[];

void func_001DC760(void)
{
    u32 base = (u32)D_00380CD8;
    s32 one = 1;
    s32 i = 0x13;
    u8 *p = (u8 *)(base + 0x380);

    do {
        *p = one;
        i--;
        p += 0x2C;
    } while (i >= 0);
}

extern s32 D_00380CD8[];

void func_001DC7A0(void)
{
    u32 base = (u32)D_00380CD8;
    s32 one = 0xFF;
    s32 i = 0x13;
    u8 *p = (u8 *)(base + 0x380);

    do {
        *p = one;
        i--;
        p += 0x2C;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "core/main", func_001DC7E0);

INCLUDE_ASM(const s32, "core/main", func_001DC850);

INCLUDE_ASM(const s32, "core/main", func_001DC8C8);

extern s32 D_00380CD8[];

void func_001DCA40(s32 idx, u8 *p, s32 val)
{
    if (idx < 0x28)
    {
        s32 v = D_00380CD8[idx * 11];
        if (v != 0)
            *(u8 *)(v + (u32)p) = val;
    }
}

INCLUDE_ASM(const s32, "core/main", func_001DCA78);

s32 func_001DCBF0(s32 c)
{
    /* Lower-case letters map to upper case, everything else is unchanged. */
    return (u32)(c - 0x61) <= 0x19 ? c - 0x20 : c;
}

INCLUDE_ASM(const s32, "core/main", func_001DCC08);

INCLUDE_ASM(const s32, "core/main", func_001DCD20);

INCLUDE_ASM(const s32, "core/main", func_001DD048);

INCLUDE_ASM(const s32, "core/main", func_001DD6A0);

extern s32 D_0039347C;

u8 *func_001DD710(void)
{
    u8 *p = (u8 *)func_00157DE8(D_0039347C, 0x14);

    *(s32 *)(p + 0x0) = 0;
    *(s32 *)(p + 0x4) = 0;
    *(s32 *)(p + 0x8) = 0;
    *(s32 *)(p + 0xC) = 0;
    *(u8 *)(p + 0x10) = 0;
    return p;
}

void func_001DD748(s32 arg1)
{
    D_00393AE6 = 0x20;
    D_00393AE7 = 1;
}

s32 func_001DFB48();

s32 func_001DD760(void *arg0)
{
    u8 *a1 = (u8 *)arg0;
    u8 *p = *(u8 **)(a1 + 4);
    u8 *q = *(u8 **)(p + 0xC);
    u8 *r = *(u8 **)q;
    s32 v = *(s32 *)(r + 4);

    *(u8 *)(r + 0x10) = 1;
    if (v != 0)
        return func_001DFB48(a1, v);
    return 0;
}

s32 func_001DD7B0(u8 *arg0)
{
    s32 A = *(s32 *)(arg0 + 4);
    s32 B = *(s32 *)(A + 0xC);
    s32 C = *(s32 *)B;
    s32 D = *(s32 *)(C + 8);
    *(u8 *)(C + 0x10) = 0;
    if (D != 0)
        return func_001DFB48(arg0, D);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001DD7F8);

INCLUDE_ASM(const s32, "core/main", func_001DD980);

INCLUDE_ASM(const s32, "core/main", func_001DDAD8);

INCLUDE_ASM(const s32, "core/main", func_001DDC90);

INCLUDE_ASM(const s32, "core/main", func_001DDD48);

INCLUDE_ASM(const s32, "core/main", func_001DDEB8);

void func_001DDF78(void) {
}

void func_001DDF80(a1)
s32 *a1;
{
    func_001F2F70(0, 0);
    if (a1 != 0)
    {
        if (a1[1] != 0)
            func_001DFC00((u8 *)a1[1]);
        if (a1[2] != 0)
            func_001DFC00((u8 *)a1[2]);
    }
}

s32 func_001DDFE0(s32 a1) {
if ( a1 != 0 )
{
func_001DDF80(a1);
func_001DD980(a1);
}
}

void func_001DE020() { func_001DDF80(); }

void func_001DE028(s32 a1, u8 a2)
{

    if (a1 != 0)
    {
        func_001DE020(a1, a2);
        func_001DD7F8(a1, a2);
    }
}

s32 func_001DE080(s32 *a1, s32 a2)
{
    s32 v = *a1;
    *a1 = v + a2;
    func_00224B88(v, 0, a2);
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_001DE0C8);

s32 func_001DE148(s32 *a1, u8 a2)
{

    u32 v2;
    s32 v4;

    v2 = 104 * a2;
    v4 = *a1;
    func_00224B88(v4, 0, v2);
    *a1 += v2;
    return v4;
}

INCLUDE_ASM(const s32, "core/main", func_001DE1A8);

s32 func_001DE258(s32 *a1, u32 a2, u8 a3)
{


    s32 v4;
v4 = 0;
if ( *(u32 *)(*(u32 *)a2 + 4) != 0 )
{
v4 = *a1;
func_00224B88(v4, 0, 0x18u);
*a1 += 24;
*(u32 *)v4 = a2;
*(u32 *)(v4 + 20) = *(u32 *)(*(u32 *)a2 + 8);
*(u32 *)(v4 + 16) = func_001DE1A8(a1, a3);
}
return v4;
}

INCLUDE_ASM(const s32, "core/main", func_001DE2F0);

INCLUDE_ASM(const s32, "core/main", func_001DE4D0);

void func_001DE550(float *src, u8 *unused, u8 *dst)
{
    *(f32 *)dst = *src;
    dst[0xC] = 8;
}

void func_001DE568(u8 *arg0, u8 *unused, u8 *out)
{
    s32 *p = *(s32 **)arg0;
    s32 v;

    if (p != 0) {
        v = 0;
        if (*p == 2)
            v = p[1];
    } else {
        v = 0;
    }
    *(s32 *)out = v;
    out[0xC] = 0xB;
}

void func_001DE5A0(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x1e;
}

/* Byte-matching.  Retail copies the payload as one unaligned 64-bit move
   (ldl/ldr) plus a word move, which is what a 12-byte struct assignment
   between byte pointers produces.  arg1 is unused, as in the neighbours. */
typedef struct
{
    u32 a;
    u32 b;
    u32 c;
} Payload12;

void func_001DE5B8(u8 *src, u8 *unused, u8 *out)
{
    *(Payload12 *)out = *(Payload12 *)src;
    out[0xC] = 0x15;
}

void func_001DE5E0(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x2c;
}

void func_001DE5F8(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x2d;
}

void func_001DE610(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x0a;
}

void func_001DE628(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x18;
}

void func_001DE640(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x22;
}

void func_001DE658(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x24;
}

s32 func_001DE670(s32 value, s32 unused, u8 *p) {
    register s32 result __asm__("$2") = 0xC;

    *(s32 *)p = value;
    p[0xC] = result;
    return result;
}

void func_001DE680(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s8 *)src;
    out[0xC] = 0x06;
}

void func_001DE698(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s8 *)src;
    out[0xC] = 0x06;
}

void func_001DE6B0(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(u8 *)src;
    out[0xC] = 0x06;
}

void func_001DE6C8(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s16 *)src;
    out[0xC] = 0x06;
}

void func_001DE6E0(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(u16 *)src;
    out[0xC] = 0x06;
}

void func_001DE6F8(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x06;
}

void func_001DE710(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x06;
}

void func_001DE728(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x26;
}

void func_001DE740(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x27;
}

void func_001DE758(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x21;
}

void func_001DE770(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x2e;
}

void func_001DE788(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x16;
}

void func_001DE7A0(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x19;
}

INCLUDE_ASM(const s32, "core/main", func_001DE7B8);

INCLUDE_ASM(const s32, "core/main", func_001DE818);

/* Byte-matching.  Copies the value at arg2 into *arg0, converting from an int
   when the tag byte at +0xC is not 8. */
void func_001DE880(f32 *arg0, s32 unused, u8 *arg2)
{
    f32 v;

    if (*(u8 *)(arg2 + 0xC) == 8)
        v = *(f32 *)arg2;
    else
        v = (f32)*(s32 *)arg2;
    *arg0 = v;
}

void func_001DE8A8(s32 *out, s32 unused, s32 *list)
{
    s32 v = *list;

    if (v != 0) {
        *out = *(s32 *)(*(s32 *)(v + 4) + 0x18);
    } else {
        *out = 0;
    }
}

void func_001DE8D0(s32 *out, s32 unused, s32 *list)
{
    s32 v = *list;

    if (v != 0) {
        *out = *(s32 *)(*(s32 *)(v + 4) + 0x18);
    } else {
        *out = 0;
    }
}

s32 func_001DE8F8(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DE908(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

typedef struct { u64 v; } __attribute__((packed)) PK8;
void func_001DE918(u8 *dst, s32 unused, u8 *src)
{
    s32 v32 = *(s32 *)(src + 8);
    ((PK8 *)dst)->v = ((PK8 *)src)->v;
    *(s32 *)(dst + 8) = v32;
}

s32 func_001DE938(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DE948(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DE958(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DE968(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_001DE978);

/* Byte-matching.  arg0 receives a copy of the u8 at arg2 once func_001E0F78 has
   worked on it; arg1 is not used. */
void func_001DE9B0(u8 *arg0, s32 arg1, u8 *arg2)
{


    func_001E0F78((s32)arg2);
    *arg0 = *arg2;
}

/* Byte-matching.  Same u8 copy as func_001DE9B0. */
void func_001DE9F0(u8 *arg0, s32 arg1, u8 *arg2)
{


    func_001E0F78((s32)arg2);
    *arg0 = *arg2;
}

/* Byte-matching.  Same u8 copy as func_001DE9B0. */
void func_001DEA30(u8 *arg0, s32 arg1, u8 *arg2)
{


    func_001E0F78((s32)arg2);
    *arg0 = *arg2;
}

/* Byte-matching.  Halfword copy behind func_001E0F78. */
void func_001DEA70(u16 *arg0, s32 arg1, u16 *arg2)
{


    func_001E0F78((s32)arg2);
    *arg0 = *arg2;
}

/* Byte-matching.  Halfword copy behind func_001E0F78. */
void func_001DEAB0(u16 *arg0, s32 arg1, u16 *arg2)
{


    func_001E0F78((s32)arg2);
    *arg0 = *arg2;
}

/* Byte-matching.  Word copy behind func_001E0F78. */
void func_001DEAF0(u32 *arg0, s32 arg1, u32 *arg2)
{


    func_001E0F78((s32)arg2);
    *arg0 = *arg2;
}

void func_001DEB30(u32 *arg0, s32 arg1, u32 *arg2)
{


    func_001E0F78((s32)arg2);
    *arg0 = *arg2;
}

s32 func_001DEB70(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DEB80(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DEB90(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DEBA0(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DEBB0(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

s32 func_001DEBC0(s32 *dst, s32 arg2, s32 *src) { s32 value = *src; *dst = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_001DEBD0);

INCLUDE_ASM(const s32, "core/main", func_001DEC30);

INCLUDE_ASM(const s32, "core/main", func_001DEC98);

INCLUDE_ASM(const s32, "core/main", func_001DED18);

INCLUDE_ASM(const s32, "core/main", func_001DED90);

u8 *func_001DEF00(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x28;
    return src + 8;
}

u8 *func_001DEF18(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x29;
    return src + 8;
}

/* Byte-matching.  Reports the word at arg1 as a 16-bit value and returns the
   block that follows it. */
u8 *func_001DEF30(s32 arg0, u8 *arg1)
{


    func_00200FA0(*(u16 *)arg1, arg0);
    return arg1 + 8;
}

u8 *func_001DEF60(u8 *arg0, u8 *arg1, s32 arg2)
{

    func_001DE818(arg1[0], 0, *(s32 *)*(s32 **)((u8 *)*(s32 **)(arg0 + 4) + 0xC), arg2);
    return arg1 + 8;
}

u8 *func_001DEFA0(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x06;
    return src + 8;
}

u8 *func_001DEFB8(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x19;
    return src + 8;
}

u8 *func_001DEFD0(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x1e;
    return src + 8;
}

u8 *func_001DEFE8(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x1d;
    return src + 8;
}

u8 *func_001DF000(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x0b;
    return src + 8;
}

u8 *func_001DF018(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x18;
    return src + 8;
}

u8 *func_001DF030(u8 *unused, u8 *src, u8 *out)
{
    *(f32 *)out = *(f32 *)src;
    out[0xC] = 0x08;
    return src + 8;
}

INCLUDE_ASM(const s32, "core/main", func_001DF048);

u8 *func_001DF128(u8 *unused, u8 *src, u8 *out)
{
    u32 *p = *(u32 **)src;
    *(S12 *)out = *(S12 *)p;
    out[0xC] = 0x15;
    return src + 8;
}

u8 *func_001DF158(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x0a;
    return src + 8;
}

u8 *func_001DF170(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x16;
    return src + 8;
}

u8 *func_001DF188(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x17;
    return src + 8;
}

u8 *func_001DF1A0(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(u16 *)src;
    out[0xC] = 0x1a;
    return src + 8;
}

u8 *func_001DF1B8(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x09;
    return src + 8;
}

u8 *func_001DF1D0(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x21;
    return src + 8;
}

u8 *func_001DF1E8(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x20;
    return src + 8;
}

u8 *func_001DF200(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = 0x10;
    out[0xC] = 0x14;
    return src;
}

u8 *func_001DF218(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x14;
    return base + 8;
}

u8 *func_001DF230(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x14;
    return base + 8;
}

u8 *func_001DF248(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x22;
    return src + 8;
}

u8 *func_001DF260(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x24;
    return src + 8;
}

u8 *func_001DF278(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x2e;
    return src + 8;
}

u8 *func_001DF290(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x26;
    return src + 8;
}

u8 *func_001DF2A8(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x27;
    return src + 8;
}

u8 *func_001DF2C0(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x2b;
    return src + 8;
}

u8 *func_001DF2D8(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x2c;
    return src + 8;
}

u8 *func_001DF2F0(u8 *unused, u8 *src, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x2d;
    return src + 8;
}

s32 func_001DF308(s32 unused, s32 value) { return value + 8; }

extern s32 D_00382E48[];

u8 *func_001DF310(u8 *arg0, u16 *arg1)
{
    s32 (*handler)(u8 *, u8 *);

    handler = (s32 (*)(u8 *, u8 *))D_00382E48[*arg1];
    return (u8 *)handler(arg0, (u8 *)arg1 + 8);
}

s32 func_001DF348(s32 a1, u16 *a2, u32 a3)
{

    s32 v7[4];
    s32 result, b;
    v7[0] = 0;
    result = ((s32 (*)(s32, s32, s32))D_003825C8[*a2])(a1, (s32)(a2 + 4), (s32)v7);
    b = v7[0] != 0;
    *(u8 *)(a3 + 12) = 6;
    *(u32 *)a3 = b;
    return result;
}

extern s32 D_003828F8[];

u8 *func_001DF3A8(u8 *arg0, u16 *arg1)
{
    s32 (*handler)(u8 *, u8 *);

    handler = (s32 (*)(u8 *, u8 *))D_003828F8[*arg1];
    return (u8 *)handler(arg0, (u8 *)arg1 + 8);
}

extern s32 D_00382EC0[];

u8 *func_001DF3E0(u8 *arg0, u16 *arg1)
{
    s32 (*handler)(u8 *, u8 *);

    handler = (s32 (*)(u8 *, u8 *))D_00382EC0[*arg1];
    return (u8 *)handler(arg0, (u8 *)arg1 + 8);
}

s64 func_001DF418(s64 a1, u16 *a2, u8 *a3)
{


    s32 v4 = *a2;
    s64 (*fp)(s64, s64);
    s64 result;
    u8 v6;

    D_00393A30 = 0;
    fp = (s64 (*)(s64, s64))D_00382F30[v4];
    result = fp(a1, (s64)(a2 + 4));
    v6 = D_00393A30;
    a3[0xC] = 0x14;
    *(s32 *)a3 = v6 & 0xFB;
    return result;
}

u8 *func_001DF470(u8 *a1)
{
    u8 v = *(u8 *)(a1 + 6);
    u8 *i = a1 + 8;

    if (*(u8 *)(i + 6) >= v && *(u8 *)(i + 7) != 8) {
        do {
            i += 8;
        } while (*(u8 *)(i + 6) >= v && *(u8 *)(i + 7) != 8);
    }
    return i;
}

u8 *func_001DF4C0(u8 *a1)
{
    u8 v = *(u8 *)(a1 - 2);
    u8 *i = a1;

    if (v < *(u8 *)(i + 6) && *(u8 *)(i + 7) != 8) {
        do {
            i += 8;
        } while (v < *(u8 *)(i + 6) && *(u8 *)(i + 7) != 8);
    }
    return i;
}

u8 *func_001DF510(u8 *a1)
{
    u8 v = *(u8 *)(a1 + 6);
    u8 *i = a1 + 8;

    if (v < *(u8 *)(i + 6) && *(u8 *)(i + 7) != 8) {
        do {
            i += 8;
        } while (v < *(u8 *)(i + 6) && *(u8 *)(i + 7) != 8);
    }
    return i;
}

INCLUDE_ASM(const s32, "core/main", func_001DF560);

/* Byte-matching: indirect dispatch through D_003824D8[5*u16_index], then a
   second call whose result is stored to *a4 plus a type byte at +0xC. */
extern s32 D_003824D8[];

s32 func_001DF598(s32 a1, s32 a2, u16 a3, u8 *a4)
{

    s32 v = ((s32 (*)(void)) D_003824D8[5 * a3])();

    *(s32 *)a4 = func_001DF560(a1, a3);
    *(u8 *)(a4 + 0xC) = 0x14;
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_001DF618);

extern u32 D_003813B8[];

s32 func_001DF760(a, b)
s32 a, b;
{
    u8 *p = (u8 *)b;
    s32 (*handler)(void) = (s32 (*)(void))D_003813B8[p[7]];

    return handler();
}

INCLUDE_ASM(const s32, "core/main", func_001DF798);

INCLUDE_ASM(const s32, "core/main", func_001DF7F0);

INCLUDE_ASM(const s32, "core/main", func_001DF8D8);

INCLUDE_ASM(const s32, "core/main", func_001DFA10);

INCLUDE_ASM(const s32, "core/main", func_001DFAE8);

INCLUDE_ASM(const s32, "core/main", func_001DFB48);

/* Byte-matching.  Advances the stream and mirrors +8 into +0xC. */
void func_001DFC00(u8 *arg0)
{

    ((void (*)(u8 *, s32))func_001DFC30)(arg0, *(s32 *)(arg0 + 0x14));
    *(u32 *)(arg0 + 0xC) = *(u32 *)(arg0 + 8);
}

s32 func_001DFC30(u32 *a1, s32 a2) {
s32 v4;
s32 result;
v4 = a1[2];
a1[2] = a2;
a1[3] = v4;
func_00200D50((s32)a1);
result = *(s32 *)(a2 + 4);
a1[1] = result;
return result;
}

u8 func_001DFC78(u8 *a1)
{
    u8 v1 = a1[-2];
    u8 v2 = 0;
    if (v1 < a1[6] && a1[7] != 8) {
        while (1) {
            if (a1[6] == v1 + 1) v2 = (u8)(v2 + 1);
            a1 += 8;
            if (!(v1 < a1[6] && a1[7] != 8)) break;
        }
    }
    return v2;
}

void func_001DFCD8(void)
{
    s32 *p = (s32 *)func_00157DE8(D_0039347C, 0xC);

    if (p != 0) {
        p[1] = 0;
        p[2] = 0;
        p[0] = 0;
    }
}

INCLUDE_ASM(const s32, "core/main", func_001DFD08);

INCLUDE_ASM(const s32, "core/main", func_001DFDD8);

s32 func_001DFED8(s32 *arg0, s32 arg1, s32 arg2)
{
    s32 *i = *arg0;

    while (i != 0) {
        if (i[3] == arg1) {
            i[4] = arg2;
            i[5] = arg2;
            return 0;
        }
        i = *i;
    }
    return -1;
}

INCLUDE_ASM(const s32, "core/main", func_001DFF08);

INCLUDE_ASM(const s32, "core/main", func_001DFF88);


/* Source: gameplay/region_001E.c */
/* func_001E0010 through func_001EFF50; retail address range 0x001E0000-0x001F0000. */



extern u16 D_0038395C[4];
extern u16 D_0038395E[4];
extern f32 D_00383960[2];
extern f32 D_00383964[2];
extern f32 D_00383968[2];
extern s32 D_0038396C[2];

INCLUDE_ASM(const s32, "core/main", func_001E0010);

INCLUDE_ASM(const s32, "core/main", func_001E0090);

INCLUDE_ASM(const s32, "core/main", func_001E0110);

INCLUDE_ASM(const s32, "core/main", func_001E0190);

INCLUDE_ASM(const s32, "core/main", func_001E01F0);

INCLUDE_ASM(const s32, "core/main", func_001E0648);

/* Byte-matching.  Walks arg1 links of the list, reports the word at +0x10 of
   the node it lands on through arg2 and returns the word at +0xC. */
s32 func_001E0AD0(u8 *arg0, s32 arg1, u32 *arg2)
{
    u8 *p = *(u8 **)arg0;
    s32 n = arg1;

    if (n > 0) {
        do {
            n--;
            p = *(u8 **)p;
        } while (n != 0);
    }
    *arg2 = *(u32 *)(p + 0x10);
    return *(s32 *)(p + 0xC);
}

INCLUDE_ASM(const s32, "core/main", func_001E0B08);

void func_001E0B80(u8 *p)
{


    if (p != 0) {
        func_00201F80(*(s32 *)(p + 0x18));
        func_0014CBE8(D_0039347C, (s32)p);
        __asm__ volatile("" ::: "memory");
    }
}

/* Byte-matching.  The third and fourth arguments are deliberately swapped for
   the tail call; that is what retail passes. */
void func_001E0BB8(u8 *arg0, s32 arg1, s32 arg2, s32 arg3)
{


    if (arg0 != 0 && arg1 != 0)
        func_00202050(*(u8 **)(arg0 + 0x18), arg1, arg3, arg2);
}

s32 func_001E0BE0(u8 *p)
{
    return p ? *(s32 *)(p + 0x18) : 0;
}

s32 func_001E0BF8(s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 result;

    if (a1 == 0)
        return -1;
    result = -1;
    if (a2 != 0)
        return func_00202180(*(s32 *)(a1 + 0x18), a2, a3, a4);
    return result;
}

s32 func_001E0C30(s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 result;

    if (a1 == 0)
        return -1;
    result = -1;
    if (a2 != 0)
        return func_00202218(*(s32 *)(a1 + 0x18), a2, a3, a4);
    return result;
}

s32 func_001E0C68(s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 result;

    if (a1 == 0)
        return -1;
    result = -1;
    if (a2 != 0)
        return func_00202258(*(s32 *)(a1 + 0x18), a2, a3, a4);
    return result;
}

s32 func_001E0CA0(s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 result;

    if (a1 == 0)
        return -1;
    result = -1;
    if (a2 != 0)
        return func_00202298(*(s32 *)(a1 + 0x18), a2, a3, a4);
    return result;
}

s32 func_001E0CD8(u8 *a1)
{
    s32 v = a1[12];
    s32 result = 0;

    if (v == 0x24) {
        result = func_00115AA8(*(s32 *)a1);
    } else if (v == 9) {
        result = *(s32 *)a1;
    }
    return result;
}

void func_001E0D20(u8 *arg0, u8 *arg1)
{

    u32 flag;
    u8 *p;

    flag = *(u32 *)(arg0 + 0x38);
    if (flag == 0)
    {
        p = *(u8 **)(arg0 + 0x24);
        *(S12 *)arg1 = *(S12 *)(p + 0x30);
        return;
    }
    func_00121278(arg1, *(u32 *)(arg0 + 0x24), flag);
}

INCLUDE_ASM(const s32, "core/main", func_001E0D60);

INCLUDE_ASM(const s32, "core/main", func_001E0DD8);

INCLUDE_ASM(const s32, "core/main", func_001E0E98);

s32 func_001E0EF0(u8 *p)
{
    s32 v = *(s32 *)(*(s32 *)(p + 4) + 8);
    if (v == 0) {
        return 0;
    }
    return *(s32 *)(*(s32 *)(v + 0x1C) + 0x148);
}

/* Byte-matching.  Wraps b into the half-open range [a, a + step): the
   difference b - a is stepped up while negative and stepped down while it
   reaches a whole step, then a is added back. */
float func_001E0F10(float a, float step, float b)
{
    float d = b - a;

    while (d < 0.0f)
        d += step;
    while (step <= d)
        d -= step;
    return d + a;
}

INCLUDE_ASM(const s32, "core/main", func_001E0F78);

INCLUDE_ASM(const s32, "core/main", func_001E0FB8);

s32 func_001E1010(u8 *arg0)
{
    s32 *p = *(s32 **)(arg0 + 4);
    s32 *q = *(s32 **)((u8 *)p + 8);
    if (q != 0) { func_0012DC18(*(s32 *)((u8 *)q + 0x1C)); return 1; }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001E1040);

INCLUDE_ASM(const s32, "core/main", func_001E1158);

INCLUDE_ASM(const s32, "core/main", func_001E11D0);

INCLUDE_ASM(const s32, "core/main", func_001E1538);

INCLUDE_ASM(const s32, "core/main", func_001E1628);

INCLUDE_ASM(const s32, "core/main", func_001E16E0);

INCLUDE_ASM(const s32, "core/main", func_001E1898);

INCLUDE_ASM(const s32, "core/main", func_001E19E0);

INCLUDE_ASM(const s32, "core/main", func_001E1DE0);

INCLUDE_ASM(const s32, "core/main", func_001E1F18);

INCLUDE_ASM(const s32, "core/main", func_001E2148);

INCLUDE_ASM(const s32, "core/main", func_001E2258);

INCLUDE_ASM(const s32, "core/main", func_001E22F8);

INCLUDE_ASM(const s32, "core/main", func_001E2478);

INCLUDE_ASM(const s32, "core/main", func_001E2680);

INCLUDE_ASM(const s32, "core/main", func_001E28A8);

INCLUDE_ASM(const s32, "core/main", func_001E2948);

INCLUDE_ASM(const s32, "core/main", func_001E2B30);

INCLUDE_ASM(const s32, "core/main", func_001E2CC8);

INCLUDE_ASM(const s32, "core/main", func_001E2EB0);

INCLUDE_ASM(const s32, "core/main", func_001E2EF0);

INCLUDE_ASM(const s32, "core/main", func_001E3020);

INCLUDE_ASM(const s32, "core/main", func_001E3088);

s32 func_001E3608(s32 a1, s32 a2)
{


    u8 *p;
    s32 result;
    p = (u8 *)D_00393BEC;
    D_00393BEC = (s32)(p + 0x10);
    result = func_001DF760(a1, a2, (s32)p);
    *(u8 *)D_0025362C = *p;
    D_00393BEC = (s32)p;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001E3648);

INCLUDE_ASM(const s32, "core/main", func_001E3700);

INCLUDE_ASM(const s32, "core/main", func_001E37B8);

INCLUDE_ASM(const s32, "core/main", func_001E38C8);

s32 func_001E3978(u8 *a1, f32 *a2)
{

    u8 *p1, *p2, *p3;
    f32 v6;
    if (a2 != 0) *a2 = 0.0f;
    p1 = *(u8 **)(*(u32 *)(a1 + 4) + 8);
    if (p1 == 0) return 0;
    p2 = *(u8 **)(p1 + 0x1C);
    if (p2 == 0) return 0;
    p3 = *(u8 **)(p2 + 0x148);
    if (p3 == 0) return 0;
    v6 = *(f32 *)(p3 + 0x120);
    if (D_00253F30[0] < v6) {
        if (a2 != 0) *a2 = v6;
        return 1;
    }
    return 0;
}

s32 func_001E39E0(u8 *a1, u8 *a2)
{
    u8 *p2, *v2, *v3;
    p2 = *(u8 **)(*(u32 *)(a1 + 4) + 8);
    if (p2 == 0) return 0;
    v2 = *(u8 **)(p2 + 0x1C);
    if (v2 == 0) return 0;
    v3 = *(u8 **)(v2 + 0x148);
    if (v3 == 0) return 0;
    if ((*(u32 *)(v3 + 4) & 1) == 0) return 0;
    *(S12 *)a2 = *(S12 *)(v3 + 0x34);
    return 1;
}

/* Byte-matching.  Follows three links, then checks the flag word. */
s32 func_001E3A40(s32 a)
{
    u8 *p;
    u8 *q;
    u8 *r;

    if (a == 0)
        return 0;
    p = *(u8 **)(a + 4);
    q = *(u8 **)(p + 8);
    if (q == 0)
        return 0;
    r = *(u8 **)(q + 0x1C);
    if (r == 0)
        return 0;
    if ((*(u32 *)(r + 0xC) & 4) == 0)
        return 0;
    return *(u32 *)(r + 0x27C);
}

INCLUDE_ASM(const s32, "core/main", func_001E3A88);

INCLUDE_ASM(const s32, "core/main", func_001E3B20);

INCLUDE_ASM(const s32, "core/main", func_001E3D68);

INCLUDE_ASM(const s32, "core/main", func_001E3DC8);

void func_001E3E48(s32 a1, f32 *a2, f32 *a3, f32 *a4, f32 *a5, f32 *a6)
{




    *a4 = func_00127BB8(a2, a3, a6);
    *a5 = func_00127E10((u32)a2, (u32)a3, a6);
    func_001E3DC8(a1, a4, a5);
}

INCLUDE_ASM(const s32, "core/main", func_001E3ED8);

INCLUDE_ASM(const s32, "core/main", func_001E3F18);

INCLUDE_ASM(const s32, "core/main", func_001E3F58);

INCLUDE_ASM(const s32, "core/main", func_001E4070);

INCLUDE_ASM(const s32, "core/main", func_001E41B8);

INCLUDE_ASM(const s32, "core/main", func_001E45D0);

INCLUDE_ASM(const s32, "core/main", func_001E4778);

/* Byte-matching.  1 when arg1 appears in the list at +0x28 of the object at
   +4, 0 otherwise. */
s32 func_001E4A58(u8 *arg0, s32 arg1)
{
    u8 *p = *(u8 **)(arg0 + 4);
    s32 n = *(s32 *)(p + 0x2C);
    u32 *list = *(u32 **)(p + 0x28);

    while (n > 0) {
        if (*list == arg1)
            return 1;
        n--;
        list++;
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001E4A90);

INCLUDE_ASM(const s32, "core/main", func_001E4B90);

INCLUDE_ASM(const s32, "core/main", func_001E4F58);

INCLUDE_ASM(const s32, "core/main", func_001E5288);

INCLUDE_ASM(const s32, "core/main", func_001E5308);

INCLUDE_ASM(const s32, "core/main", func_001E53F8);

INCLUDE_ASM(const s32, "core/main", func_001E5468);

INCLUDE_ASM(const s32, "core/main", func_001E5570);

INCLUDE_ASM(const s32, "core/main", func_001E55F8);

INCLUDE_ASM(const s32, "core/main", func_001E57D8);

/* Byte-matching.  -1, 0 or 1 by comparing the floats at +0xC. */
s32 func_001E5920(u8 *arg0, u8 *arg1)
{
    float a = *(float *)(arg0 + 0xC);
    float b = *(float *)(arg1 + 0xC);
    s32 r;

    if (a < b)
        r = -1;
    else if (b == a)
        r = 0;
    else
        r = 1;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001E5958);

INCLUDE_ASM(const s32, "core/main", func_001E5F80);

INCLUDE_ASM(const s32, "core/main", func_001E60F0);

INCLUDE_ASM(const s32, "core/main", func_001E6190);

INCLUDE_ASM(const s32, "core/main", func_001E6218);

INCLUDE_ASM(const s32, "core/main", func_001E6368);

INCLUDE_ASM(const s32, "core/main", func_001E66F8);

INCLUDE_ASM(const s32, "core/main", func_001E67B0);

s32 func_001E6870(u8 *arg0, s32 arg1)
{

    s32 old;
    s32 result;

    old = *(s32 *)(arg0 + 0xC);
    *(s32 *)(arg0 + 0xC) = *(s32 *)(arg0 + 0x10);
    result = func_001E67B0(arg0, arg1);
    *(s32 *)(arg0 + 0xC) = old;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001E68B0);

INCLUDE_ASM(const s32, "core/main", func_001E6A88);

INCLUDE_ASM(const s32, "core/main", func_001E6BC0);

INCLUDE_ASM(const s32, "core/main", func_001E6CF0);

/* Byte-matching.  Returns the address of record `arg` in the 20-byte table. */
s32 func_001E6E00(u16 arg)
{

    return (s32)(D_003824E0 + arg * 20);
}

s32 func_001E6E20(void) { return 148; }

/* Byte-matching.  Field +4 of a 12-byte record; typing the table as an array
   of 12-byte records keeps `lw $2,4($3)` instead of folding +4 into the base. */
s32 func_001E6E28(u16 arg)
{
    return D_00382818[arg].b;
}

/* Byte-matching.  Field +8 of the same 12-byte record table. */
s32 func_001E6E48(u16 arg)
{
    return D_00382818[arg].c;
}

s32 func_001E6E68(void) { return 30; }

s32 func_001E6E70(void) { return 532; }

/* Byte-matching.  Byte at +0 of a 12-byte record, zero-extended (lbu). */
s32 func_001E6E78(u8 arg)
{

    return *(u8 *)(D_00383780 + arg * 12);
}

s32 func_001E6E98(void) { return 3; }

s32 func_001E6EA0(void) { return 0; }

s32 func_001E6EA8(s32 arg1)
{
    return D_00383900[arg1 & 0xFF];
}

s32 func_001E6EC8(void) { return 4; }

s32 func_001E6ED0(void) { return 2; }

u8 func_001E6ED8(u32 index)
{
    return ((u8 *)D_00383908)[index & 0xFF];
}

void func_001E6EF0(void) {
}

void func_001E6EF8(void) {
}

void func_001E6F00(s32 p, s32 out)
{
    u8 *a = *(u8 **)(p + 4);
    u8 *b = *(u8 **)(a + 0xC);
    u8 *c = *(u8 **)(b + 0);
    u8 *d = *(u8 **)(c + 4);
    *(s32 *)(out + 0) = *(s32 *)(d + 0xC);
    *(u8 *)(out + 0xC) = 0x22;
}

void func_001E6F28(s32 p, s32 out)
{
    u8 *a = *(u8 **)(p + 4);
    u8 *b = *(u8 **)(a + 0xC);
    u8 *c = *(u8 **)(b + 0);
    u8 *d = *(u8 **)(c + 8);
    *(s32 *)(out + 0) = *(s32 *)(d + 0xC);
    *(u8 *)(out + 0xC) = 0x22;
}

void func_001E6F50(u8 *arg0, u8 *dst)
{
    u8 *src = *(u8 **)(arg0 + 0x24);

    *(S12 *)dst = *(S12 *)(src + 0x30);
    *(u8 *)(dst + 0xC) = 0x15;
}

void func_001E6F78(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001E6F80);

INCLUDE_ASM(const s32, "core/main", func_001E7028);

INCLUDE_ASM(const s32, "core/main", func_001E7078);

extern u8 D_00383950[];

void func_001E70C0(u32 arg0, S12 *arg1)
{
    *arg1 = *(S12 *)D_00383950;
    *(u8 *)((u8 *)arg1 + 0xC) = 0x15;
}

void func_001E70F0(u32 arg0, S12 *arg1)
{
    *(S12 *)D_00383950 = *arg1;
}

void func_001E7118(u8 *unused, u8 *out)
{
    *(s32 *)out = (s16)D_0038395C[0];
    out[0xC] = 0x06;
}

void func_001E7130(s32 unused, const u16 *p) { D_0038395C[0] = p[0]; }

void func_001E7140(u8 *unused, u8 *out)
{
    *(s32 *)out = (s16)D_0038395E[0];
    out[0xC] = 0x06;
}

void func_001E7158(s32 unused, const u16 *p) { D_0038395E[0] = p[0]; }

void func_001E7168(u8 *unused, u8 *out)
{
    *(f32 *)out = *(f32 *)D_00383960;
    out[0xC] = 0x08;
}

void func_001E7180(s32 unused, const f32 *p) { D_00383960[0] = p[0]; }

void func_001E7190(u8 *unused, u8 *out)
{
    *(f32 *)out = D_00383964[0];
    out[0xC] = 0x08;
}

void func_001E71A8(s32 unused, const f32 *p) { D_00383964[0] = p[0]; }

void func_001E71B8(u8 *unused, u8 *out)
{
    *(f32 *)out = D_00383968[0];
    out[0xC] = 0x08;
}

void func_001E71D0(s32 unused, const f32 *p) { D_00383968[0] = p[0]; }

void func_001E71E0(u8 *unused, u8 *out)
{
    *(s32 *)out = D_0038396C[0];
    out[0xC] = 0x06;
}

void func_001E71F8(s32 unused, const s32 *p) { D_0038396C[0] = p[0]; }

INCLUDE_ASM(const s32, "core/main", func_001E7208);

INCLUDE_ASM(const s32, "core/main", func_001E75A0);

INCLUDE_ASM(const s32, "core/main", func_001E7758);

INCLUDE_ASM(const s32, "core/main", func_001E7980);

INCLUDE_ASM(const s32, "core/main", func_001E7B48);

INCLUDE_ASM(const s32, "core/main", func_001E7D90);

INCLUDE_ASM(const s32, "core/main", func_001E8198);

INCLUDE_ASM(const s32, "core/main", func_001E8318);

s32 func_001E83A8(s32 a1, s32 a2, s32 *a3)
{
    f32 v4;
    s32 v5;

    v4 = 0.0f;
    v5 = func_001DF760(a1, a2, a3);
    if (*(s32 *)a3 != 0) v4 = *(f32 *)((u8 *)*(s32 *)a3 + 12);
    *(f32 *)a3 = v4;
    *(u8 *)((u8 *)a3 + 12) = 8;
    return v5;
}

INCLUDE_ASM(const s32, "core/main", func_001E83F8);

void func_001E8450(s32 arg0)
{

    u8 *p = (u8 *)D_00393A64;

    if (p == 0)
        return;
    for (;;) {
        s32 i = *(s32 *)(p + 0x25C);
        if (i == arg0) {
            *p = 0;
            *(s32 *)(p + 0x25C) = 0;
        }
        p = *(u8 **)(p + 0x260);
        if (p == 0)
            break;
    }
}

INCLUDE_ASM(const s32, "core/main", func_001E8488);

INCLUDE_ASM(const s32, "core/main", func_001E8510);

INCLUDE_ASM(const s32, "core/main", func_001E85D0);

INCLUDE_ASM(const s32, "core/main", func_001E8F28);

INCLUDE_ASM(const s32, "core/main", func_001E97A0);

INCLUDE_ASM(const s32, "core/main", func_001E9928);

INCLUDE_ASM(const s32, "core/main", func_001E9B00);

INCLUDE_ASM(const s32, "core/main", func_001E9BE8);

INCLUDE_ASM(const s32, "core/main", func_001E9E48);

INCLUDE_ASM(const s32, "core/main", func_001EA2B0);

INCLUDE_ASM(const s32, "core/main", func_001EAAF8);

INCLUDE_ASM(const s32, "core/main", func_001EACE8);

INCLUDE_ASM(const s32, "core/main", func_001EAED0);

INCLUDE_ASM(const s32, "core/main", func_001EB1E0);

INCLUDE_ASM(const s32, "core/main", func_001EB6A0);

INCLUDE_ASM(const s32, "core/main", func_001EB9A0);

INCLUDE_ASM(const s32, "core/main", func_001EBA28);

s32 func_001EBBF8(s32 a1, s32 a2, s32 *a3)
{
    s32 v5;
    s32 *v7;

    func_001F2F88(a1, &v7);
    v5 = 0;
    if (*v7 == 2) v5 = v7[1];
    *a3 = func_00213918((s32 *)v5);
    *(u8 *)((u8 *)a3 + 12) = 44;
    return a2;
}

u8 *func_001EBC60(u8 *arg0, u8 *arg1, u8 *arg2)
{

    s32 v = func_001DF760(arg0, arg1);
    *(s32 *)arg2 = 1 << *arg2;
    *(u8 *)(arg2 + 0xC) = 0x2C;
    return (u8 *)v;
}

INCLUDE_ASM(const s32, "core/main", func_001EBCA0);

s32 func_001EBD10(s32 a1, s32 a2, u32 a3) {
s32 v6[4];
func_001F2F88(a1, v6);
*(u32 *)a3 = func_0010EDF8(v6[0]);
*(u8 *)(a3 + 12) = 6;
return a2;
}

INCLUDE_ASM(const s32, "core/main", func_001EBD60);

INCLUDE_ASM(const s32, "core/main", func_001EBEF8);

u8 *func_001EBF88(u8 *arg0, u8 *arg1, u8 *arg2)
{

    *(s32 *)arg2 = func_001012A0(arg0);
    *(u8 *)(arg2 + 0xC) = 6;
    return arg1;
}

s32 func_001EBFC8(s32 a1, s32 a2, u8 *a3)
{
    s32 *p;
    s32 v;
    u8 b;

    func_001F2F88(a1, &p);
    v = 0;
    if (*p == 2)
        v = p[1];
    b = *(u8 *)(*(s32 *)(v + 4) + 0x28);
    a3[12] = 3;
    *a3 = b;
    return a2;
}

INCLUDE_ASM(const s32, "core/main", func_001EC030);

INCLUDE_ASM(const s32, "core/main", func_001EC128);

INCLUDE_ASM(const s32, "core/main", func_001EC200);

INCLUDE_ASM(const s32, "core/main", func_001EC2C8);

u8 *func_001EC350(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x06;
    return base;
}

INCLUDE_ASM(const s32, "core/main", func_001EC368);

INCLUDE_ASM(const s32, "core/main", func_001EC7B0);

INCLUDE_ASM(const s32, "core/main", func_001EC828);

s32 func_001ECA60(s32 a1, s32 a2, u8 *a3)
{
    f32 v = func_0014B900();

    a3[12] = 8;
    *(f32 *)a3 = v * 1000.0f;
    return a2;
}

INCLUDE_ASM(const s32, "core/main", func_001ECAB0);

INCLUDE_ASM(const s32, "core/main", func_001ECE08);

INCLUDE_ASM(const s32, "core/main", func_001ED080);

INCLUDE_ASM(const s32, "core/main", func_001ED0F8);

INCLUDE_ASM(const s32, "core/main", func_001ED370);

s32 func_001ED4C0(s32 a1)
{



    s32 r = 0;

    if (a1 == 0)
        return 0;
    {
        s32 v1 = *(s32 *)(a1 + 0x20);
        if (func_0011DAA8(v1) != 0) {
            s32 v4 = func_0011DAD8(v1);
            if (v4 != 0 && (u32)v4 != 0xFFFFFFFFu)
                return func_0011DB80(v4);
        }
    }
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001ED530);

s32 func_001ED8F8(s32 a1, s32 a2, u8 *a3)
{


    s32 v4;
    s32 v5;
    s32 v6;

    v4 = func_001DF760(a1, a2);
    v5 = func_00101908(*(u32 *)(*(u32 *)(*(u32 *)a3 + 4) + 24));
    if (v5 != 0) {
        v6 = 0;
        if (*(u32 *)v5 == 2)
            v6 = *(u32 *)(v5 + 4);
    } else {
        v6 = 0;
    }
    *(u32 *)a3 = v6;
    a3[12] = 11;
    return v4;
}

INCLUDE_ASM(const s32, "core/main", func_001ED970);

INCLUDE_ASM(const s32, "core/main", func_001EDC20);

INCLUDE_ASM(const s32, "core/main", func_001EDE50);

INCLUDE_ASM(const s32, "core/main", func_001EDF78);

INCLUDE_ASM(const s32, "core/main", func_001EE460);

INCLUDE_ASM(const s32, "core/main", func_001EE610);

INCLUDE_ASM(const s32, "core/main", func_001EE6A0);

INCLUDE_ASM(const s32, "core/main", func_001EE7B8);

INCLUDE_ASM(const s32, "core/main", func_001EE880);

INCLUDE_ASM(const s32, "core/main", func_001EEB00);

u8 *func_001EECE0(u8 *arg0, u8 *arg1, u8 *arg2)
{


    s32 v = func_001DF760(arg0, arg1);
    *(s32 *)arg2 = func_00120708(*(s32 *)arg2);
    *(u8 *)(arg2 + 0xC) = 9;
    return (u8 *)v;
}

s32 func_001EED28(s32 a, s32 b, u8 *p)
{
    u8 flag = D_00254029[0];
    p[0xC] = 6;
    *(u32 *)p = flag;
    return b;
}

s32 func_001EED48(s32 a, s32 b, u8 *p)
{
    u8 flag = D_00253628[0];
    p[0xC] = 6;
    *(u32 *)p = flag;
    return b;
}

s32 func_001EED68(s32 a, s32 b, u8 *p)
{
    u8 flag = D_00254028[0];
    p[0xC] = 6;
    *(u32 *)p = flag;
    return b;
}

s32 func_001DF760();

s32 func_001EED88(void)
{
    return func_001DF760();
}

u8 *func_001EEDA8(u8 *a1, u8 *a2, u8 *a3)
{


    s32 v = func_001DF760(a1, a2);
    u8 *p = (u8 *)func_001E0CD8(a3);

    *(s32 *)a3 = func_002220A0(p);
    a3[12] = 6;
    return (u8 *)v;
}

INCLUDE_ASM(const s32, "core/main", func_001EEDF8);

s32 func_001EEE90(s32 unused, s32 value) { return value; }

s32 func_001EEE98(s32 unused, s32 value) { return value; }

s32 func_001EEEA0(s32 unused, s32 value) { return value; }

s32 func_001DF760();

s32 func_001EEEA8(void)
{
    return func_001DF760();
}

extern u8 D_003EAE50[];
u8 *func_001EEEC8(u8 *arg0, u8 *arg1, u8 *arg2)
{

    s32 v = func_001DF760(arg0, arg1);
    *(u8 *)(arg2 + 0xC) = 9;
    *(s32 *)arg2 = (s32)&D_003EAE50;
    return (u8 *)v;
}

s32 func_001EEF00(s32 unused, s32 value) { return value; }

u8 *func_001EEF08(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 1;
    out[0xC] = 0x06;
    return base;
}

INCLUDE_ASM(const s32, "core/main", func_001EEF20);

INCLUDE_ASM(const s32, "core/main", func_001EEFC8);

INCLUDE_ASM(const s32, "core/main", func_001EF080);

INCLUDE_ASM(const s32, "core/main", func_001EF0F8);

/* Byte-matching. */
s32 func_001EF1E8(s32 arg0, s32 arg1, u8 *arg2)
{
    s32 result = func_001DF760(arg0, arg1);
    *(u32 *)arg2 = ~*(u32 *)arg2;
    *(u8 *)(arg2 + 0xC) = 7;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001EF220);

INCLUDE_ASM(const s32, "core/main", func_001EF420);

INCLUDE_ASM(const s32, "core/main", func_001EF4C0);

INCLUDE_ASM(const s32, "core/main", func_001EF5D8);

s32 func_001EF6C0(s32 a1, s32 a2, u8 *a3)
{
    s32 v8[4];
    s32 *v6;

    func_001F2F88(a1, v8);
    v6 = (s32 *)(*(s32 *)v8 + 0x24);
    v6 = (s32 *)*v6;
    *(S12 *)a3 = *(S12 *)((u8 *)v6 + 0x30);
    *(a3 + 12) = 21;
    return a2;
}

/* Behavior-faithful, not byte-matching: same 8+4 byte copy from
   (*(arg0+0x24))+0x30 and +0x38 into arg2, same 0x15 tag and same return of
   arg1, size exact at 0x2C; only the register allocation differs. */
u8 *func_001EF720(u8 *arg0, u8 *arg1, u8 *arg2)
{
    u8 *src = *(u8 **)(arg0 + 0x24);

    *(S12 *)arg2 = *(S12 *)(src + 0x30);
    *(u8 *)(arg2 + 0xC) = 0x15;
    return arg1;
}

INCLUDE_ASM(const s32, "core/main", func_001EF750);

INCLUDE_ASM(const s32, "core/main", func_001EF7E8);

INCLUDE_ASM(const s32, "core/main", func_001EF898);

INCLUDE_ASM(const s32, "core/main", func_001EF988);

INCLUDE_ASM(const s32, "core/main", func_001EFC20);

INCLUDE_ASM(const s32, "core/main", func_001EFDC8);

INCLUDE_ASM(const s32, "core/main", func_001EFEF0);

INCLUDE_ASM(const s32, "core/main", func_001EFF50);


/* Source: gameplay/region_001F_before_audio.c */
/* func_001F02F8 through func_001F4EF0; retail address range 0x001F0000-0x001F51F8. */


INCLUDE_ASM(const s32, "core/main", func_001F02F8);

INCLUDE_ASM(const s32, "core/main", func_001F0368);

INCLUDE_ASM(const s32, "core/main", func_001F03E8);

INCLUDE_ASM(const s32, "core/main", func_001F0460);

INCLUDE_ASM(const s32, "core/main", func_001F0A08);

s32 func_001F0B70(s32 a1, s32 a2, u8 *a3)
{
    s32 result;
    u8 v;
    result = func_001DF760(a1, a2);
    v = *(u8 *)(*(s32 *)(*(s32 *)a3 + 4) + 0xC);
    a3[0xC] = 6;
    *(s32 *)a3 = v & 0x7F;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001F0BB0);

INCLUDE_ASM(const s32, "core/main", func_001F0D88);

INCLUDE_ASM(const s32, "core/main", func_001F0E28);

s32 func_001F0F18(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_001F0F20);

INCLUDE_ASM(const s32, "core/main", func_001F1100);

INCLUDE_ASM(const s32, "core/main", func_001F1210);

INCLUDE_ASM(const s32, "core/main", func_001F1388);

/* Byte-matching. */
s32 func_001F1420(s32 arg0, s32 arg1, u8 *arg2)
{

    u8 local;
    s32 result = func_001F1388(arg0, arg1, &local);

    *arg2 = local;
    return result;
}

s32 func_001F1450(s32 arg0, s32 arg1, u8 *arg2)
{

    u8 local;
    s32 result = func_001F1388(arg0, arg1, &local);

    arg2[1] = local;
    return result;
}

s32 func_001F1480(s32 arg0, s32 arg1, u8 *arg2)
{

    u8 local;
    s32 result = func_001F1388(arg0, arg1, &local);

    arg2[2] = local;
    return result;
}

s32 func_001F14B0(s32 arg0, s32 arg1, u8 *arg2)
{

    u8 local;
    s32 result = func_001F1388(arg0, arg1, &local);

    arg2[3] = local;
    return result;
}

s32 func_001F14E0(s32 a1, s32 a2, u8 *a3)
{



    s32 v5, v6, result;

    a3[3] = 0;
    v5 = func_001F1420(a1, a2, a3);
    v6 = func_001F1450(a1, v5, a3);
    result = func_001F1480(a1, v6, a3);
    a3[12] = 40;
    return result;
}

s32 func_001F1540(s32 arg0, s32 arg1, u8 *arg2)
{

    s32 v = func_001F14E0(arg0, arg1, arg2);
    return func_001F14B0(arg0, v, arg2);
}

INCLUDE_ASM(const s32, "core/main", func_001F1580);

INCLUDE_ASM(const s32, "core/main", func_001F1670);

INCLUDE_ASM(const s32, "core/main", func_001F1988);

u8 *func_001F1A18(u8 *arg0, u8 *arg1, u8 *arg2)
{
    f32 v = func_0014B910(arg0);
    *(f32 *)arg2 = v;
    *(u8 *)(arg2 + 0xC) = 8;
    return arg1;
}

s32 func_001F1A58(u32 *a1)
{


    s32 v1;
s32 i;
if ( *a1 == 2 )
{
if ( *(u32 *)(a1[1] + 12) != 0 )
func_001DE028(**(s32 **)(a1[1] + 12), 0);
}
else
{
v1 = (s32)a1[2];
for ( i = func_0014ABD8((s32)a1[2]); v1 != 0; i = func_0014ABD8(i) )
{
func_001F1A58(v1);
v1 = i;
}
}
}

s32 func_001F1AE0(s32 a1, s32 a2, u32 a3)
{








    s32 v5;
    u32 v6;
    s32 v7;
    u32 v8;
    s32 v9;
    u8 v11[64];
    if (*(u16 *)(a2 - 8) == 103) {
        v5 = func_001124E0();
        func_00226A34((u32)v11, v5);
        v6 = func_00112610();
        if (func_00226580((u8 *)v6, (u8 *)D_00393A98) != 0) {
            func_00211A20((u32)v11, (u32)D_00393AA0);
            v7 = func_00112610();
            func_00211A20((u32)v11, v7);
        }
        v8 = func_00112620();
        if (func_00226580((u8 *)v8, (u8 *)D_00393A98) != 0) {
            func_00211A20((u32)v11, (u32)D_00393AA8);
            v9 = func_00112620();
            func_00211A20((u32)v11, v9);
        }
        *(u32 *)a3 = func_00111568(v11);
        *(u8 *)(a3 + 12) = 6;
    }
    return a2;
}

u8 *func_001F1BD0(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x18;
    return base;
}

/* Byte-matching. */
s32 func_001F1BE8(u32 *arg0, s32 arg1, u8 *arg2)
{
    s32 v = 0;

    if (arg0[0] == 2)
        v = arg0[1];
    *(s32 *)arg2 = v;
    *(u8 *)(arg2 + 0xC) = 0xB;
    return arg1;
}

extern u32 D_00252398[];

s32 func_001F1C10(s32 arg0, s32 arg1, u8 *arg2)
{
    s32 two = 2;
    u32 *p = (u32 *)D_00252398[0];
    s32 v = 0;

    if (p[0] == two)
        v = p[1];
    *(s32 *)arg2 = v;
    *(u8 *)(arg2 + 0xC) = 0xB;
    return arg1;
}

u8 *func_001F1C40(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x1e;
    return base;
}

u8 *func_001F1C58(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x0b;
    return base;
}

u8 *func_001F1C70(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x0a;
    return base;
}

u8 *func_001F1C88(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = -1;
    out[0xC] = 0x24;
    return base;
}

u8 *func_001F1CA0(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x2c;
    return base;
}

u8 *func_001F1CB8(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x2d;
    return base;
}

u8 *func_001F1CD0(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x26;
    return base;
}

u8 *func_001F1CE8(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x27;
    return base;
}

u8 *func_001F1D00(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x21;
    return base;
}

u8 *func_001F1D18(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x16;
    return base;
}

u8 *func_001F1D30(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x19;
    return base;
}

s32 func_001DF7F0(void);

s32 func_001F1D48(void)
{
    return func_001DF7F0();
}

u8 *func_001F1D68(u8 *unused, u8 *base)
{
    base -= 8;
    return base + *(u16 *)(base + 0x4) * 8;
}

INCLUDE_ASM(const s32, "core/main", func_001F1D80);

INCLUDE_ASM(const s32, "core/main", func_001F1E90);

s32 func_001DF7F0(void);

s32 func_001F1F30(void)
{
    return func_001DF7F0();
}

INCLUDE_ASM(const s32, "core/main", func_001F1F50);

INCLUDE_ASM(const s32, "core/main", func_001F1FE0);

INCLUDE_ASM(const s32, "core/main", func_001F2048);

INCLUDE_ASM(const s32, "core/main", func_001F2298);

s32 func_001F24B0(s32 a1, s32 a2, s32 *a3)
{


    s32 old = D_00393BEC;
    s32 *out = a3;
    s32 result;
    s32 v;

    D_00393BEC = old + 0x10;
    result = func_001DF760(a1, a2, old);
    v = *(s32 *)(*(s32 *)(*(s32 *)old + 4) + 0x18);
    D_00393BEC = old;
    *out = v;
    return result;
}

void func_001F2500(u32 value, u32 unused0, u32 unused1) { func_00200FA0((u16)value); }

void func_001F2508(u32 value, u32 unused0, u32 unused1) { func_00200E60((u16)value); }

void func_001F2510(u32 a, u32 b, u8 *ctx, u32 unused0)
{
    u8 *p = *(u8 **)(ctx + 0x4);

    func_001DE818(a & 0xFF, b & 0xFF, *(s32 *)(*(u32 *)(p + 0xC)));
}

void func_001F2528(u32 a, u32 b, u8 *ctx, u32 unused0)
{
    u8 *p = *(u8 **)(ctx + 0x4);

    func_001DEC30(a & 0xFF, b & 0xFF, *(s32 *)(*(u32 *)(p + 0xC)));
}

INCLUDE_ASM(const s32, "core/main", func_001F2540);

INCLUDE_ASM(const s32, "core/main", func_001F25C8);

INCLUDE_ASM(const s32, "core/main", func_001F2690);

void func_001F2728(s32 a1, u8 a2, u8 a3, u8 a4, u16 a5, s32 a6)
{

    if (a2 == 11)
    {
        func_001F2510(a3, a4, a1, a6);
        return;
    }
    if (a2 == 9)
    {
        func_001F2500(a5, a1, a6);
        return;
    }
    return 9;
}

void func_001F2778(s32 a1, u8 a2, u8 a3, u8 a4, u16 a5, s32 a6)
{

    if (a2 == 11)
    {
        func_001F2528(a3, a4, a1, a6);
        return;
    }
    if (a2 == 9)
    {
        func_001F2508(a5, a1, a6);
        return;
    }
    return 9;
}

INCLUDE_ASM(const s32, "core/main", func_001F27C8);

INCLUDE_ASM(const s32, "core/main", func_001F2820);

INCLUDE_ASM(const s32, "core/main", func_001F2860);

void func_001F2F70(s32 a, s32 b)
{
    if (a == b)
        D_00393AC8 = 0;
    else
        D_00393AC8 = b;
}

void func_001F2F88(s32 def, s32 *out)
{
    s32 v = D_00393AC8;
    if (v != 0) {
        *out = v;
        D_00393AC8 = 0;
    } else {
        *out = def;
    }
}

s32 func_001F2FA8(u8 *arg0, s32 arg1, s32 arg2)
{
    s32 v[4];
    s32 r = func_001F24B0(arg0, arg1, v);
    func_001F2F70(arg0, v[0]);
    return func_001DF760(arg0, r, arg2);
}

INCLUDE_ASM(const s32, "core/main", func_001F3008);

INCLUDE_ASM(const s32, "core/main", func_001F3080);

INCLUDE_ASM(const s32, "core/main", func_001F32F0);

INCLUDE_ASM(const s32, "core/main", func_001F3358);

INCLUDE_ASM(const s32, "core/main", func_001F33C0);

INCLUDE_ASM(const s32, "core/main", func_001F3428);

INCLUDE_ASM(const s32, "core/main", func_001F3490);

INCLUDE_ASM(const s32, "core/main", func_001F34F8);

INCLUDE_ASM(const s32, "core/main", func_001F3560);

INCLUDE_ASM(const s32, "core/main", func_001F36D0);

INCLUDE_ASM(const s32, "core/main", func_001F37D0);

INCLUDE_ASM(const s32, "core/main", func_001F3940);

INCLUDE_ASM(const s32, "core/main", func_001F3C28);

INCLUDE_ASM(const s32, "core/main", func_001F3CC0);

INCLUDE_ASM(const s32, "core/main", func_001F3D90);

INCLUDE_ASM(const s32, "core/main", func_001F3E28);

INCLUDE_ASM(const s32, "core/main", func_001F3F30);

INCLUDE_ASM(const s32, "core/main", func_001F4000);

INCLUDE_ASM(const s32, "core/main", func_001F4138);

INCLUDE_ASM(const s32, "core/main", func_001F4570);

INCLUDE_ASM(const s32, "core/main", func_001F4680);

INCLUDE_ASM(const s32, "core/main", func_001F4720);

INCLUDE_ASM(const s32, "core/main", func_001F48C8);

INCLUDE_ASM(const s32, "core/main", func_001F49C8);

INCLUDE_ASM(const s32, "core/main", func_001F4B78);

INCLUDE_ASM(const s32, "core/main", func_001F4C10);

INCLUDE_ASM(const s32, "core/main", func_001F4EF0);


/* Source: audio/animation_sound_events.c */
/* func_001F51F8 through func_001F51F8; retail address range 0x001F51F8-0x001F5508. */


INCLUDE_ASM(const s32, "core/main", func_001F51F8);


/* Source: gameplay/region_001F_tail.c */
/* func_001F5508 through func_001FFFE8; retail address range 0x001F5508-0x00200000. */


INCLUDE_ASM(const s32, "core/main", func_001F5508);

s32 func_001F5580(s32 unused, s32 value) { return value; }

s32 func_001F5588(u32 *a1, s32 a2, u8 a3)
{


    s32 r = a1[2];
    u8 flag = a3;

    if (r != a2) {
        s32 eight = 8;
        r = func_001DFC30(a1, a2);
        if (flag == 0) {
            D_00393AE0 = 0;
            D_00393A30 = eight;
            r = eight;
        }
    }
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001F55D0);

INCLUDE_ASM(const s32, "core/main", func_001F5668);

s32 func_001F5700(u8 *a1, u8 *a2)
{

    u8 *p = *(u8 **)(a1 + 4);
    u8 *q = *(u8 **)(p + 12);
    u8 *r = *(u8 **)q;
    s32 v5;
    s32 v;
    v = func_001DF760(a1, a2, &v5);
    func_001F5588(*(s32 *)(r + 4), v5, r[16] == 0);
    return v;
}

s32 func_001F5758(u8 *a1, u8 *a2)
{

    u8 *p = *(u8 **)(a1 + 4);
    u8 *q = *(u8 **)(p + 12);
    u8 *r = *(u8 **)q;
    s32 v5;
    s32 v;
    v = func_001DF760(a1, a2, &v5);
    func_001F5588(*(s32 *)(r + 8), v5, r[16]);
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_001F57A8);

INCLUDE_ASM(const s32, "core/main", func_001F5838);

INCLUDE_ASM(const s32, "core/main", func_001F5910);

INCLUDE_ASM(const s32, "core/main", func_001F5A80);

INCLUDE_ASM(const s32, "core/main", func_001F5BC8);

INCLUDE_ASM(const s32, "core/main", func_001F5C40);

INCLUDE_ASM(const s32, "core/main", func_001F5CF8);

void func_001F5EE0(u32 *a1, s32 a2, s32 a3, u8 a4)
{
  s32 v6; 
  s32 v7; 

  v6 = a4;
  func_0014AD50(a1);
  if ( v6 != 0 && a2 != 0 )
  {
    v7 = *(u32 *)(a1[1] + 8);
    if ( v7 != 0 )
      func_0012D750(*(u32 *)(v7 + 28), (f32 *)a2);
  }
}

INCLUDE_ASM(const s32, "core/main", func_001F5F58);

INCLUDE_ASM(const s32, "core/main", func_001F6C40);

/* ---------------------------------------------------------------------------
 * func_001F6EA8 - entity command executor ("comport op" apply/commit).
 *
 * Behavioural reconstruction from asm/nonmatchings/core/main/func_001F6EA8.s.
 * $4 = evaluation context, $5 = first operand node of the command; the opcode
 * is the u16 stored 8 bytes before that node.  Returns the operand cursor the
 * caller continues from.
 *
 * Two passes over the same opcode: pass 1 walks the operand list through
 * func_001DF760 (which fills the shared value slot at D_00393BEC and returns
 * the next operand), pass 2 applies the decoded values to ctx->[0x24]: rotate,
 * look-at, copy another entity, scale, or commit a position.
 *
 * Retail frame offsets map onto these locals:
 *   sp+0x00 -> frame   0x54-byte snapshot of the entry entity
 *   sp+0x60 -> v60     decoded value (vector / position)
 *   sp+0x70 -> v70     second vector / accumulation helper
 *   sp+0x80 -> v80     third vector
 *   sp+0x90 -> v90     scratch vector handed to func_00127330
 *   sp+0xA0/0xB0/0xC0 -> rowA/rowB/rowC   rotation rows for func_00126868
 *   sp+0xD0 -> mtx     0x54-byte node built by func_001275C0
 *   sp+0x130/0x140/0x150 -> basis/vecV/vecU   look-at basis vectors
 * ------------------------------------------------------------------------- */

typedef struct
{
    f32 x, y, z;
} CmdVec3;

/* The 0x54-byte transform node: 3x3 basis (rows 0x10 apart), pos at +0x30,
   1.0 at +0x3C, scale at +0x40, kind at +0x50.  Only copied whole here. */
typedef struct
{
    u32 w[0x54 / 4];
} CmdNode54;

/* One 0x10-byte entry of the operand value pool (D_00393BEC). */
typedef struct
{
    union
    {
        f32 f;    /* +0x00 float value */
        s32 i;    /* +0x00 integer value */
        f32 v[3]; /* +0x00 vector value */
        u8 *ptr;  /* +0x00 reference value (opcode 0x96) */
        u8 b[12]; /* +0x00 raw bytes (object name, did-work byte) */
    } p;
    u8 kind; /* +0x0C: 8 = float, otherwise s32 / pointer */
    u8 pad0D[3];
} CmdVal;

/* a*b - c*d, fused like retail's mula.s / msub.s pair. */
#define F_MULSUB(a, b, c, d)                                       \
    ({                                                             \
        float r_;                                                  \
        __asm__ volatile("mula.s %1,%2\n\t"                        \
                         "msub.s %0,%3,%4"                         \
                         : "=&f"(r_)                               \
                         : "f"(a), "f"(b), "f"(c), "f"(d));        \
        r_;                                                        \
    })

/* Bit pattern of a float, through the FPU move rather than a memory round
   trip (retail uses mfc1/mtc1 for every one of these tests). */
#define F_BITS(x)                                                  \
    ({                                                             \
        u32 b_;                                                    \
        __asm__("mfc1 %0,%1" : "=r"(b_) : "f"(x));                \
        b_;                                                        \
    })

/* Opcode groups: computed once in pass 1, reused by pass 2 so neither pass
   walks the whole tag chain (retail does the same with its jump tables). */
#define GRP_ROT_VEC   0 /* D7, 9F: vector + negative angle + vector */
#define GRP_ROT       1 /* D6, 9E: vector + negative angle */
#define GRP_CTX       2 /* 96: switch evaluation context, copy the object */
#define GRP_VEC       3 /* A5, DD: vector only (aim / scale) */
#define GRP_ANGLE_VEC 4 /* A0-A2, D8-DA: positive angle + vector */
#define GRP_ANGLE     5 /* 9B-9D, D3-D5: angle only */
#define GRP_LOOKAT    6 /* 99, 9A, CF, D2: build a look-at basis */
#define GRP_FRAME     7 /* 98, D1: convert the vector into the node frame */
#define GRP_SCALE     8 /* D0: uniform scale */
#define GRP_POS       9 /* 95: commit a position */
#define GRP_NONE     10 /* no-op tag */

/* 1.0f with the sign of the given bit pattern. */
#define SIGN1_BITS(b)                                              \
    ({                                                             \
        f32 r_;                                                    \
        u32 v_ = ((b) & 0x80000000u) | 0x3F800000u;                 \
        __asm__("mtc1 %1,%0" : "=f"(r_) : "r"(v_));               \
        r_;                                                        \
    })

/* copysign(1.0f, x): retail keeps the 1.0 exponent and copies x's sign. */
#define CMD_SIGN1(x) SIGN1_BITS(F_BITS(x))

/* t = ctx->[4]->[0]->[0x88], entry idx, first word: the named-object lookup
   repeated five times in the listing. */
#define CMD_LOOKUP(dst, c, idx)                                     \
    ({                                                             \
        u8 *tbl_ = *(u8 **)(*(u8 **)((c) + 4));                     \
        tbl_ = *(u8 **)(tbl_ + 0x88);                               \
        (dst) = (tbl_ != 0) ? *(u8 **)(tbl_ + (u32)(idx) * 0x20) : 0; \
    })

/* The value of the current operand slot: 8 means float, anything else is an
   s32 (lwc1 + cvt.s.w in retail). */
#define CMD_NUM(s) (((s)->kind == 8) ? (s)->p.f : (f32)(s)->p.i)

/* 0x54-byte node copy, word by word (retail uses ldl/ldr/sdl/sdr pairs). */
#define CMD_COPY54(dst, src)                                       \
    ({                                                             \
        u32 i_;                                                    \
        for (i_ = 0; i_ < 0x54 / 4; i_++)                           \
            ((u32 *)(dst))[i_] = ((u32 *)(src))[i_];                \
    })

u8 *func_001F6EA8(u8 *arg0, u8 *arg1)
{










    /* 0x10C9F0 is a 4-instruction accessor: ctx->[4]->[4]->[0x2C].  It reads
       $a0 even though it takes nothing that looks like an argument, and retail
       leaves the context there (0x1F7EE8 reloads it and 0x1F7EFC does not
       re-set $a0 before the call) ??? do not call it empty. */


    CmdVal *slot;
    CmdVec3 v60;
    CmdVec3 v70;
    CmdVec3 v80;
    CmdVec3 v90; /* sp+0x90: scratch vector handed to func_00127330 */
    CmdVec3 rowA;
    CmdVec3 rowB;
    CmdVec3 rowC;
    CmdVec3 basis;
    CmdVec3 vecV;
    CmdVec3 vecU;
    CmdVec3 cvec;
    u32 frame[0x54 / 4];
    u32 mtx[0x54 / 4];
    u8 *cur;
    u8 *echo;
    u8 *ctx;
    u8 *other;
    u8 *saved;
    u8 *w;
    u8 *ent;
    u8 *entryEnt;
    u8 *p;
    u32 nameByte;
    s32 grp;
    s32 tag;
    s32 flagA;
    s32 flagB;
    s32 didWork;
    s32 n;
    s32 r;
    f32 f20;
    f32 f0;
    f32 f1;
    f32 cosv;
    f32 sinv;
    f32 wrapped;

    /* ---------------- entry (0x1F6EA8) ---------------- */
    slot = (CmdVal *)D_00393BEC;
    D_00393BEC = (s32)((u8 *)slot + 0x10);

    nameByte = 0xFF;
    didWork = 0;
    other = 0;
    echo = arg0;
    cur = arg1;
    f20 = 0.0f;
    flagA = 0;
    flagB = 0;

    tag = *(u16 *)(cur - 8);
    ctx = arg0;
    func_001F2F88((s32)arg0, (s32 *)&ctx);
    saved = ctx;
    entryEnt = *(u8 **)(ctx + 0x24);
    w = *(u8 **)(*(u8 **)(ctx + 4) + 8);
    cur = (u8 *)func_001DF760(echo, cur, slot);

    /* ---------------- pass 1: decode the operand list ---------------- */
    grp = GRP_NONE;
    if (tag < 0xCF)
    {
        if (tag < 0xA5)
        {
            if (tag < 0x9B)
            {
                if (tag < 0x98)                     /* 95, 96, 97 */
                {
                    if (tag == 0x96)
                    {
                        /* Switch the evaluation context to slot->[0]->[4]->[0x18]
                           and resolve a name there.  On a miss, tail-return the
                           next cursor from func_001DF470 (see below). */
                        p = *(u8 **)slot->p.b;
                        echo = *(u8 **)(*(u8 **)(p + 4) + 0x18);
                        cur = (u8 *)func_001DF760(echo, cur, slot);
                        nameByte = slot->p.b[0];
                        CMD_LOOKUP(other, echo, nameByte);
                        if (other == 0)
                        {
                            /* retail tail-returns func_001DF470's result ($v0 from
                               the jal); it branches to .L001F7FC8, skipping the
                               daddu $2,$21 at 0x1F7FC0 that would reload $v0 with
                               the cursor. */
                            D_00393BEC = (s32)slot;
                            return func_001DF470(cur);
                        }
                        nameByte = 0xFF;
                        flagA = 1;
                        didWork = 1;
                        grp = GRP_CTX;
                    }
                    else if (tag == 0x95)
                    {
                        flagA = 1;
                        v60 = *(CmdVec3 *)slot->p.v;
                        didWork = 1;
                        grp = GRP_POS;
                    }
                }
                else if (tag == 0x98)
                {
                    v60 = *(CmdVec3 *)slot->p.v;
                    cur = (u8 *)func_001DF760(echo, cur, slot);
                    flagA = 1;
                    didWork = slot->p.b[0];
                    grp = GRP_FRAME;
                }
                else                                /* 99, 9A */
                {
                    v60 = *(CmdVec3 *)slot->p.v;
                    didWork = 1;
                    grp = GRP_LOOKAT;
                    if (tag == 0x9A)
                    {
                        flagA = 1;
                        cur = (u8 *)func_001DF760(echo, cur, slot);
                        v80 = *(CmdVec3 *)slot->p.v;
                    }
                }
            }
            else if (tag < 0x9E)                    /* 9B, 9C, 9D */
            {
                f20 = CMD_NUM(slot) * D_00392DE8;
                didWork = 1;
                grp = GRP_ANGLE;
            }
            else if (tag < 0xA0)                    /* 9E, 9F */
            {
                v60 = *(CmdVec3 *)slot->p.v;
                cur = (u8 *)func_001DF760(echo, cur, slot);
                if (tag == 0x9F)
                {
                    f20 = -CMD_NUM(slot) * D_00392DDC;
                    flagA = 1;
                    cur = (u8 *)func_001DF760(echo, cur, slot);
                    v80 = *(CmdVec3 *)slot->p.v;
                    grp = GRP_ROT_VEC;
                }
                else
                {
                    f20 = -CMD_NUM(slot) * D_00392DE0;
                    grp = GRP_ROT;
                }
                didWork = 1;
            }
            else if (tag < 0xA3)                    /* A0, A1, A2 */
            {
                f20 = CMD_NUM(slot) * D_00392DE4;
                flagA = 1;
                didWork = 1;
                cur = (u8 *)func_001DF760(echo, cur, slot);
                v60 = *(CmdVec3 *)slot->p.v;
                grp = GRP_ANGLE_VEC;
            }
            /* 0xA3, 0xA4: no operation */
        }
        else if (tag == 0xA5)
        {
            v60 = *(CmdVec3 *)slot->p.v;
            didWork = 1;
            grp = GRP_VEC;
        }
        /* 0xA6..0xCE: no operation */
    }
    else if (tag < 0xD3)                            /* CF..D2: named variants */
    {
        v60 = *(CmdVec3 *)slot->p.v;
        cur = (u8 *)func_001DF760(echo, cur, slot);
        nameByte = slot->p.b[0];
        CMD_LOOKUP(other, ctx, nameByte);
        cur = (u8 *)func_001DF760(echo, cur, slot);
        didWork = slot->p.b[0];
        if (tag == 0xD0)
            grp = GRP_SCALE;
        else if (tag == 0xD1)
            grp = GRP_FRAME;
        else
        {
            grp = GRP_LOOKAT;
            if (tag == 0xD2)
            {
                flagA = 1;
                cur = (u8 *)func_001DF760(echo, cur, slot);
                v80 = *(CmdVec3 *)slot->p.v;
            }
        }
    }
    else if (tag < 0xD6)                            /* D3..D5: named angle */
    {
        nameByte = slot->p.b[0];
        CMD_LOOKUP(other, ctx, nameByte);
        cur = (u8 *)func_001DF760(echo, cur, slot);
        f20 = CMD_NUM(slot) * D_00392DE8;
        didWork = 1;
        grp = GRP_ANGLE;
    }
    else if (tag < 0xD8)                            /* D6, D7: named rotation */
    {
        nameByte = slot->p.b[0];
        CMD_LOOKUP(other, ctx, nameByte);
        cur = (u8 *)func_001DF760(echo, cur, slot);
        v60 = *(CmdVec3 *)slot->p.v;
        cur = (u8 *)func_001DF760(echo, cur, slot);
        if (tag == 0xD7)
        {
            f20 = -CMD_NUM(slot) * D_00392DDC;
            flagA = 1;
            cur = (u8 *)func_001DF760(echo, cur, slot);
            v80 = *(CmdVec3 *)slot->p.v;
            grp = GRP_ROT_VEC;
        }
        else
        {
            f20 = -CMD_NUM(slot) * D_00392DE0;
            grp = GRP_ROT;
        }
        didWork = 1;
    }
    else if (tag < 0xDB)                            /* D8..DA: named angle+vec */
    {
        nameByte = slot->p.b[0];
        CMD_LOOKUP(other, ctx, nameByte);
        cur = (u8 *)func_001DF760(echo, cur, slot);
        f20 = CMD_NUM(slot) * D_00392DE4;
        flagA = 1;
        didWork = 1;
        cur = (u8 *)func_001DF760(echo, cur, slot);
        v60 = *(CmdVec3 *)slot->p.v;
        grp = GRP_ANGLE_VEC;
    }
    else if (tag == 0xDD)                           /* DD: named vector */
    {
        nameByte = slot->p.b[0];
        CMD_LOOKUP(other, ctx, nameByte);
        cur = (u8 *)func_001DF760(echo, cur, slot);
        v60 = *(CmdVec3 *)slot->p.v;
        didWork = 1;
        grp = GRP_VEC;
    }
    /* 0xDB, 0xDC: no operation */

    /* ---------------- join (0x1F7480) ---------------- */
    if (nameByte != 0xFF)
    {
        if (other == 0)
        {
            D_00393BEC = (s32)slot;
            return cur;
        }
        ctx = other;
    }
    ent = *(u8 **)(ctx + 0x24);
    if (didWork == 0)
        CMD_COPY54(frame, entryEnt);

    /* ---------------- pass 2: apply (0x1F7574) ---------------- */
    if (grp == GRP_NONE)
        goto L7FA0;

    if (grp < GRP_CTX)                              /* ROT_VEC, ROT */
    {
        if (grp == GRP_ROT_VEC)
            func_00121480(&v70.x, ent, &v80.x);

        /* 0x1F75AC: normalise the axis, then rotate the entity about it */
        f0 = F_LEN2(v60.x, v60.y, v60.z);
        f1 = sqrtf(f0);
        if (f1 != 0.0f)
        {
            f0 = 1.0f / f1;
            v60.z = f0 * v60.z;
            v60.x = f0 * v60.x;
            v60.y = f0 * v60.y;
        }
        func_001255C8(ent, &v60.x, f20);
        if (flagA == 0)
            goto L7ED4;
        func_00121480(&v60.x, ent, &v80.x);
        goto L7988;
    }

    if (grp == GRP_CTX)
    {
        /* copy the resolved object's whole transform block */
        CMD_COPY54(ent, *(u8 **)(other + 0x24));
        goto L7ED4;
    }

    if (grp == GRP_VEC)                             /* A5, DD: aim / scale */
    {
        if (tag == 0xA5)
            flagB = 1;
        if ((*(u32 *)(ctx + 0x30) & 8) != 0)
        {
            f0 = v60.x;
            v60.z = f0;
            v60.y = f0;
        }
        if (!(nameByte == 0xFF && w != 0))
        {
            v90 = v60;
            func_00127330(ent, &v90.x);
        }
        goto L7ED4;
    }

    if (grp < GRP_LOOKAT)                           /* ANGLE_VEC, ANGLE */
    {
        if (grp == GRP_ANGLE_VEC)
            func_00121480(&v70.x, ent, &v60.x);

        /* 0x1F7780: cos / sin of the angle, then the rotation rows */
        f0 = f20 * D_00392DEC;
        n = (s32)f0;
        wrapped = f20 - (f32)n * D_00392DF0;
        cosv = func_00217AC0(f20);
        f0 = (wrapped < 0.0f) ? -1.0f : 1.0f;
        f1 = fabsf(wrapped);
        if (!(f1 < D_00392DF4))
            f0 = -f0;
        if (f1 == 0.0f)
            f0 = 0.0f;
        sinv = sqrtf(1.0f - cosv * cosv) * f0;

        if (tag == 0x9B || tag == 0xA0 || tag == 0xD8 || tag == 0xD3)
        {
            rowA.x = 1.0f;
            rowA.y = 0.0f;
            rowA.z = 0.0f;
            rowB.x = 0.0f;
            rowB.y = cosv;
            rowB.z = sinv;
            rowC.x = 0.0f;
            rowC.y = -sinv;
            rowC.z = cosv;
        }
        else if (tag == 0x9C || tag == 0xA1 || tag == 0xD9 || tag == 0xD4)
        {
            rowA.x = cosv;
            rowA.y = 0.0f;
            rowA.z = sinv;
            rowB.x = 0.0f;
            rowB.y = 1.0f;
            rowB.z = 0.0f;
            rowC.x = -sinv;
            rowC.y = 0.0f;
            rowC.z = cosv;
        }
        else
        {
            rowA.x = cosv;
            rowA.y = sinv;
            rowA.z = 0.0f;
            rowB.x = -sinv;
            rowB.y = cosv;
            rowB.z = 0.0f;
            rowC.x = 0.0f;
            rowC.y = 0.0f;
            rowC.z = 1.0f;
        }

        /* 0x1F7914: rotate the entity's own matrix by the rows just built */
        func_001275C0((f32 *)mtx, ent);
        func_00121480(&rowA.x, (u8 *)mtx, &rowA.x);
        func_00121480(&rowB.x, (u8 *)mtx, &rowB.x);
        func_00121480(&rowC.x, (u8 *)mtx, &rowC.x);
        func_00126868(ent, &rowA.x, &rowB.x, &rowC.x);
        if (flagA == 0)
            goto L7ED4;
        func_00121480(&v60.x, ent, &v60.x);
        goto L7988;
    }

    if (grp == GRP_LOOKAT)                          /* 99, 9A, CF, D2 */
    {
        if (flagA != 0)
            func_00121480(slot->p.v, ent, &v80.x);

        /* 0x1F79D4: build a basis whose first vector points at -v60 */
        func_001262F8(ent, &basis.x, &v70.x, &vecU.x);

        if (v60.x == 0.0f && v60.y == 0.0f && v60.z == 0.0f)
        {
            vecV = v70;
        }
        else
        {
            v60.x = -v60.x;
            v60.y = -v60.y;
            v60.z = -v60.z;
            if (didWork == 0)
                func_00121480(&v60.x, (u8 *)frame, &v60.x);
            f0 = F_LEN2(v60.x, v60.y, v60.z);
            f1 = sqrtf(f0);
            if (f1 == 0.0f)
            {
                vecV = v60;
            }
            else
            {
                f0 = 1.0f / f1;
                vecV.x = f0 * v60.x;
                vecV.y = f0 * v60.y;
                vecV.z = f0 * v60.z;
            }
        }

        /* 0x1F7AEC: the up vector; reuse the previous one when both
           horizontal components of V and W are degenerate. */
        if (fabsf(vecV.x) < D_00392DF8 && fabsf(vecV.y) < D_00392DF8)
        {
            if (fabsf(v70.x) < D_00392DF8 && fabsf(v70.y) < D_00392DF8)
                v70 = vecU;
        }
        else
        {
            v70.x = -vecV.x;
            v70.y = -vecV.y;
            v70.z = vecV.z + 2.0f;
        }

        /* 0x1F7BA8: secondary vector, perpendicular to V with z = 0.
           The reference direction for the handedness test is *not* vecV: retail
           reads 0x158($sp), i.e. the third row func_001262F8 wrote into vecU
           (still intact here, before the cross products overwrite it). */
        n = (vecU.z > 0.0f);

        cvec.x = F_MULSUB(v70.y, vecV.z, v70.z, vecV.y);
        cvec.y = F_MULSUB(v70.z, vecV.x, v70.x, vecV.z);
        cvec.z = 0.0f;
        vecU.x = cvec.y * vecV.z;
        vecU.y = -(cvec.x * vecV.z);
        vecU.z = F_MULSUB(cvec.x, vecV.y, cvec.y, vecV.x);

        if ((vecU.z > 0.0f) != n)
        {
            vecU.x = -vecU.x;
            vecU.y = -vecU.y;
            vecU.z = -vecU.z;
        }

        basis.x = F_MULSUB(vecV.y, vecU.z, vecV.z, vecU.y);
        basis.y = F_MULSUB(vecV.z, vecU.x, vecV.x, vecU.z);
        basis.z = F_MULSUB(vecV.x, vecU.y, vecV.y, vecU.x);

        f0 = F_LEN2(basis.x, basis.y, basis.z);
        f1 = sqrtf(f0);
        if (f1 != 0.0f)
        {
            f0 = 1.0f / f1;
            basis.x = f0 * basis.x;
            basis.y = f0 * basis.y;
            basis.z = f0 * basis.z;
        }
        f0 = F_LEN2(vecU.x, vecU.y, vecU.z);
        f1 = sqrtf(f0);
        if (f1 != 0.0f)
        {
            f0 = 1.0f / f1;
            vecU.x = f0 * vecU.x;
            vecU.y = f0 * vecU.y;
            vecU.z = f0 * vecU.z;
        }

        func_00126868(ent, &basis.x, &vecV.x, &vecU.x);

        /* Retail branches to .L001F7ED4 (the tail) at 0x1F7D98 when flagA is
           clear, i.e. for the plain 0x99/0xCF look-at: it has just written the
           basis into ent and must NOT commit v60 as a new position.  Only the
           0x9A/0xD2 variants carry a second vector and fall through to the
           commit.  Going to L7DDC unconditionally here moves the entity by the
           look vector instead of just orienting it. */
        if (flagA == 0)
            goto L7ED4;

        func_00121480(&v80.x, ent, &v80.x);
        v60.x = slot->p.v[0] - v80.x;
        v60.y = slot->p.v[1] - v80.y;
        v60.z = slot->p.v[2] - v80.z;
        goto L7DDC;
    }

    if (grp < GRP_SCALE)                            /* FRAME: 98, D1 */
        goto L7DDC;

    if (grp == GRP_SCALE)                           /* D0 */
    {
        if (didWork == 0)
            func_00121278(&v60.x, (u8 *)frame, &v60.x);
        goto L7E50;
    }

    goto L7E50;                                     /* POS: 95 */

    /* ---------------- shared tails ---------------- */
    /* 0x1F7988 is a single call site in retail whose arguments are set by the
       caller (rotate: v60 = ent*v80, rotation rows: v60 = ent*v60).  Each path
       above performs that call itself, so the label starts after it. */
L7988:
    v60.x = v70.x - v60.x;
    v60.y = v70.y - v60.y;
    v60.z = v70.z - v60.z;

L7DDC:
    if (didWork == 0)
        func_00121480(&v60.x, (u8 *)frame, &v60.x);
    v70 = *(CmdVec3 *)(ent + 0x30);
    v60.x += v70.x;
    v60.y += v70.y;
    v60.z += v70.z;

L7E50:
    if (*(u8 *)(ent + 0x50) == 1)
        *(u8 *)(ent + 0x50) = 2;
    *(CmdVec3 *)(ent + 0x30) = v60;
    *(f32 *)(ent + 0x3C) = 1.0f;
    if ((tag == 0x95 || tag == 0x98) && w != 0)
        func_0012D900(*(u8 **)(w + 0x1C), &v60.x);

L7ED4:
    *(u32 *)(ctx + 0x28) = D_003932D0;
    func_0014AD50(ctx);
    if (*(s32 *)ctx == 2)
    {
        r = func_0010C9F0(ctx);
        if ((r & 2) != 0 && (w == 0 || (r & 0x8000) != 0))
            func_0014AF20(ctx);
        else
            func_0014B050(ctx);
    }
    else
    {
        func_0014AF20(ctx);
    }

    if (nameByte != 0xFF)
        goto L7FAC;
    if (w != 0)
    {
        p = *(u8 **)(w + 0x1C);
        if (flagB != 0 && p != 0)
        {
            *(u32 *)(p + 0x0C) |= 0x1000;
            *(f32 *)(p + 0x48) = v60.x;
            *(f32 *)(p + 0x4C) = v60.y;
            *(f32 *)(p + 0x50) = v60.z;
        }
        func_0012D750(*(u8 **)(w + 0x1C), ent);
    }
    goto L7FBC;

L7FA0:
    if (nameByte == 0xFF)
        goto L7FBC;
L7FAC:
    *(u8 *)(*(u8 **)(*(u8 **)(saved + 4)) + 0xF0) = 1;

L7FBC:
    D_00393BEC = (s32)slot;
    return cur;
}

#undef F_MULSUB
#undef F_BITS
#undef CMD_SIGN1
#undef CMD_LOOKUP
#undef CMD_NUM
#undef CMD_COPY54

INCLUDE_ASM(const s32, "core/main", func_001F8000);

INCLUDE_ASM(const s32, "core/main", func_001F81F8);

INCLUDE_ASM(const s32, "core/main", func_001F82E8);

INCLUDE_ASM(const s32, "core/main", func_001F8528);

INCLUDE_ASM(const s32, "core/main", func_001F8600);

INCLUDE_ASM(const s32, "core/main", func_001F8818);

INCLUDE_ASM(const s32, "core/main", func_001F8BD8);

INCLUDE_ASM(const s32, "core/main", func_001F8CC0);

INCLUDE_ASM(const s32, "core/main", func_001F8E40);

INCLUDE_ASM(const s32, "core/main", func_001F8EF8);

INCLUDE_ASM(const s32, "core/main", func_001F9090);

INCLUDE_ASM(const s32, "core/main", func_001F9310);

INCLUDE_ASM(const s32, "core/main", func_001F9D88);

INCLUDE_ASM(const s32, "core/main", func_001F9E28);

INCLUDE_ASM(const s32, "core/main", func_001F9ED0);

INCLUDE_ASM(const s32, "core/main", func_001F9FA0);

INCLUDE_ASM(const s32, "core/main", func_001FA170);

INCLUDE_ASM(const s32, "core/main", func_001FA268);

INCLUDE_ASM(const s32, "core/main", func_001FA2F0);

extern s32 D_00393BEC;
extern f32 D_00393198;
s32 func_001FA3F8(s32 a1, s32 a2)
{
    s32 v5[4];
    s32 v;
    s32 result;

    v = D_00393BEC;
    D_00393BEC = v + 0x10;
    func_001F2F88(a1, v5);
    result = func_001DF760(a1, a2, v);
    D_00393198 = *(f32 *)v;
    D_00393BEC = v;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001FA460);

INCLUDE_ASM(const s32, "core/main", func_001FA5F0);

INCLUDE_ASM(const s32, "core/main", func_001FA7B8);

INCLUDE_ASM(const s32, "core/main", func_001FA8D8);

INCLUDE_ASM(const s32, "core/main", func_001FA988);

INCLUDE_ASM(const s32, "core/main", func_001FAA08);

INCLUDE_ASM(const s32, "core/main", func_001FAAA0);

/* ------------------------------------------------------------------------
 * func_001FB9E0 -- script-driven 2D aim / interpolation step for one slot of
 * the D_00383950 record table (0x40 bytes per entry, indexed by an s16 key).
 *
 * Retail reads eight operands out of the inline script through
 * func_001DF760 (value word at +0x00, type byte at +0x0C, 8 = float):
 *   0  s16 key                    5  mode   (0 = clear, 2 = basis path)
 *   1  f32 min length             6  flag   (raw word, compared with 1)
 *   2  f32 angle threshold (deg)  7  state  (raw word, 0 leaves it alone)
 *   3  f32 rate                    (2 is converted deg -> rad here)
 *   4  f32 curve input
 * The record keeps: +0x20 vector, +0x2C/+0x2E the two s16 values taken from
 * the objects D_00330D78[key][2] and [3] (their +0x10 floats truncated),
 * +0x30 length, +0x34 the initial length, +0x38 the signed angle in
 * degrees, +0x3C the 0..4 facing state.  Returns the advanced script.
 * --------------------------------------------------------------------- */

typedef union UB9Val { f32 f; s32 i; } UB9Val;

/* Operand slot written by func_001DF760 (0x10 bytes). */
typedef struct UB9Op { UB9Val v; u32 pad[2]; u8 type; u8 pad2[3]; } UB9Op;

#define U_B9F(o) ((o)->type == 8 ? (o)->v.f : (f32)(o)->v.i)
#define U_B9I(o) ((o)->type == 8 ? (s32)(o)->v.f : (o)->v.i)

extern f32 D_0038FB98[3];              /* the (0,0,1) frame axis          */
extern f32 D_00392E18;                 /* pi/180                          */
extern f32 D_00392E1C;                 /* 0.3  (powf exponent, row 1)     */
extern f32 D_00392E20;                 /* 0.3  (powf exponent, row 3)     */
extern f32 D_00392E24;                 /* 0.8                             */
extern f32 D_00392E28;                 /* 1e-4                            */
extern f32 D_00392E2C;                 /* 180/pi                          */
extern f32 D_00393AD4;
extern f32 D_00393AD8;

f32 func_00218050(f32, f32);           /* powf                            */
s32 func_001208C0(s16, s16);
f32 func_0011EEA0(u8 *);
void func_001262F8(u8 *, f32 *, f32 *, f32 *);
void func_00126510(u8 *, f32 *);
void func_001265E0(u8 *, f32 *);
void func_00125C80(f32, u8 *);
void func_001232E8(u8 *, u8 *, u8 *);
void func_0012D750(s32, u8 *);

/* copysign(1, x) with the zero case forced to 0, as the retail sign tests do
 * (they look at bits & 0x7FFFFFFF).  Spelling it out keeps this file free of
 * an extra out-of-line helper: the module has no room for one. */
#define U_B9SIGN(x) ((x) == 0.0f ? 0.0f : ((x) < 0.0f ? -1.0f : 1.0f))

u8 *func_001FB9E0(u8 *obj, u8 *script)
{
    UB9Op op;
    u8 *tgt;                           /* resolved target object          */
    u8 *rec;                           /* D_00383950 + key * 0x40         */
    f32 v[3];                          /* the 2D vector being driven      */
    f32 dir[3];                        /* reference direction             */
    f32 axis[3];                       /* scratch basis vector            */
    f32 up[3];                         /* -target Y axis, flattened       */
    f32 ax1[3], ax2[3], ax3[3];        /* this object's basis rows        */
    f32 bx1[3], bx2[3], bx3[3];        /* target object's basis rows      */
    f32 cst[3];                        /* copy of D_0038FB98              */
    f32 crs[3];                        /* cross(dir, cst)                 */
    f32 w[3];                          /* vector before the frame change  */
    u32 rot[0x18];                     /* rotation transform buffer       */
    f32 f20, f21, f22, f23 = 0.0f, f24, f25 = 0.0f, f26;
    f32 prev, len, inv, k, d, ang, m, x, c, a, b;
    s32 key, n30 = 0, n23 = 0, mode, flag, st = 0, ms, sel, nz;
    u8 *o2, *o3, *ent, *tr, *child;

    func_001F2F88((s32)obj, (s32 *)&tgt);

    if (*(u16 *)(script - 8) == 0x15D) {
        script = (u8 *)func_001DF760(obj, script, &op);
        key = (s16)U_B9I(&op);
    } else {
        key = 0;
    }

    script = (u8 *)func_001DF760(obj, script, &op);
    f22 = U_B9F(&op);                  /* minimum length                  */
    script = (u8 *)func_001DF760(obj, script, &op);
    f26 = U_B9F(&op) * D_00392E18;     /* angle threshold, radians        */
    script = (u8 *)func_001DF760(obj, script, &op);
    f24 = U_B9F(&op);                  /* rate                            */
    script = (u8 *)func_001DF760(obj, script, &op);
    f20 = U_B9F(&op);                  /* curve input                     */
    script = (u8 *)func_001DF760(obj, script, &op);
    mode = U_B9I(&op);
    script = (u8 *)func_001DF760(obj, script, &op);
    flag = op.v.i;
    script = (u8 *)func_001DF760(obj, script, &op);
    st = op.v.i;

    rec = D_00383950 + key * 0x40;

    if (mode == 0) {
        n30 = 0;
        n23 = 0;
        f21 = 0.0f;
        f23 = 0.0f;
        f25 = 0.0f;
        st = 0;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
        goto store;
    }

    ms = (s32)(func_0014B900() * 1000.0f);     /* frame time in ms        */
    sel = func_0014B930();                     /* interpolation selector  */
    o2 = (u8 *)func_001208C0((s16)key, 2);
    o3 = (u8 *)func_001208C0((s16)key, 3);
    n30 = o2 != 0 ? (s32)func_0011EEA0(o2) : 0;
    n23 = o3 != 0 ? (s32)func_0011EEA0(o3) : 0;

    D_00393AD8 = 0.0f;
    D_00393AD4 = 0.0f;

    if (mode == 2) {
        f24 = 0.0f;
        flag = 0;
    }

    v[0] = (f32)(-n30);
    if (mode == 2) {
        v[1] = 0.0f;
        v[2] = (f32)(-n23);
    } else {
        v[2] = 0.0f;
        v[1] = (f32)n23;
    }

    f21 = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    prev = *(f32 *)(rec + 0x30);

    if (f21 <= f22) {
        f21 = 0.0f;
        flag = 0;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }

    if (0.0f < f20) {
        switch (sel) {
        case 1:
            k = f20;
            break;
        case 2:
            k = f20 * f20;
            break;
        case 3:
            k = f20 * f20 * f20;
            break;
        default:                       /* 0 and >= 4 both use the quartic  */
            k = f20 * f20;
            k = k * k;
            break;
        }
        f23 = (prev - f21) * k + f21;
        if (f22 < f23 - f21) {
            flag = 0;
            v[0] = 0.0f;
            v[1] = 0.0f;
            v[2] = 0.0f;
        }
    } else {
        f23 = f21;
    }

    if (!(f22 <= f23))
        goto plain_store;

    if (mode == 2) {
        /* Rescale the vector to the interpolated length, then rebuild it out
         * of this object's first and third basis rows, each weighted by how
         * well it lines up with the target object's matching row. */
        if (v[0] != 0.0f || v[1] != 0.0f || v[2] != 0.0f) {
            len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
            if (len != 0.0f) {
                inv = 1.0f / len;
                v[0] = inv * v[0];
                v[1] = inv * v[1];
                v[2] = inv * v[2];
            }
            v[0] = f23 * v[0];
            v[1] = f23 * v[1];
            v[2] = f23 * v[2];
        }

        func_001262F8(*(u8 **)(obj + 0x24), ax1, ax2, ax3);
        func_001262F8(*(u8 **)(tgt + 0x24), bx1, bx2, bx3);

        d = ax1[0] * bx1[0] + ax1[1] * bx1[1] + ax1[2] * bx1[2];
        k = 1.5f * func_00218050(fabsf(d), D_00392E1C);
        if (k > 1.0f)
            k = 1.0f;
        a = U_B9SIGN(d) * k;

        d = ax3[0] * bx3[0] + ax3[1] * bx3[1] + ax3[2] * bx3[2];
        k = 1.5f * func_00218050(fabsf(d), D_00392E20);
        if (k > 1.0f)
            k = 1.0f;
        b = U_B9SIGN(d) * k;

        a = a * v[0];
        b = b * v[2];
        v[0] = ax1[0] * a + ax3[0] * b;
        v[1] = ax1[1] * a + ax3[1] * b;
        v[2] = ax1[2] * a + ax3[2] * b;
        goto classify;
    }

    /* ---- mode != 2 ---------------------------------------------------- */
    cst[0] = D_0038FB98[0];
    cst[1] = D_0038FB98[1];
    cst[2] = D_0038FB98[2];

    up[0] = *(f32 *)(*(u8 **)(tgt + 0x24) + 0x30) -
            *(f32 *)(*(u8 **)(obj + 0x24) + 0x30);
    up[1] = *(f32 *)(*(u8 **)(tgt + 0x24) + 0x34) -
            *(f32 *)(*(u8 **)(obj + 0x24) + 0x34);
    up[2] = *(f32 *)(*(u8 **)(tgt + 0x24) + 0x38) -
            *(f32 *)(*(u8 **)(obj + 0x24) + 0x38);
    dir[0] = up[0];
    dir[1] = up[1];
    dir[2] = up[2];
    nz = up[0] == 0.0f && up[1] == 0.0f && up[2] == 0.0f ? 0 : 1;

    if (nz) {
        len = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
        if (len != 0.0f) {
            inv = 1.0f / len;
            dir[0] = inv * dir[0];
            dir[1] = inv * dir[1];
            dir[2] = inv * dir[2];
        }
    }

    if (nz == 0)
        goto use_axis;
    if (n23 < 0)
        goto use_axis;
    if (D_00392E24 < fabsf(dir[2]))
        goto use_axis;
    dir[2] = 0.0f;
    goto shortcut;

use_axis:
    func_00126510(*(u8 **)(tgt + 0x24), dir);
    dir[2] = 0.0f;
    if (dir[0] == 0.0f && dir[1] == 0.0f) {
        func_001265E0(*(u8 **)(tgt + 0x24), dir);
        dir[2] = 0.0f;
    }
    {
        len = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
        if (len != 0.0f) {
            inv = 1.0f / len;
            dir[0] = inv * dir[0];
            dir[1] = inv * dir[1];
            dir[2] = inv * dir[2];
        }
    }
    goto frame;

shortcut:
    if (dir[0] == 0.0f && dir[1] == 0.0f) {
        func_001265E0(*(u8 **)(tgt + 0x24), dir);
        dir[2] = 0.0f;
        if (dir[0] == 0.0f && dir[1] == 0.0f)
            goto store;
    }
    {
        len = sqrtf(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
        if (len != 0.0f) {
            inv = 1.0f / len;
            dir[0] = inv * dir[0];
            dir[1] = inv * dir[1];
            dir[2] = inv * dir[2];
        }
    }

frame:
    crs[0] = dir[1] * cst[2] - dir[2] * cst[1];
    crs[1] = dir[2] * cst[0] - dir[0] * cst[2];
    crs[2] = dir[0] * cst[1] - dir[1] * cst[0];

    w[0] = v[0];
    w[1] = v[1];
    w[2] = v[2];
    v[0] = crs[0] * w[0] + dir[0] * w[1] + cst[0] * w[2];
    v[1] = crs[1] * w[0] + dir[1] * w[1] + cst[1] * w[2];
    v[2] = crs[2] * w[0] + dir[2] * w[1] + cst[2] * w[2];

    if (v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f)
        goto store;

    len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len != 0.0f) {
        inv = 1.0f / len;
        v[0] = inv * v[0];
        v[1] = inv * v[1];
        v[2] = inv * v[2];
    }

    func_00126510(*(u8 **)(obj + 0x24), axis);
    up[0] = -axis[0];
    up[1] = -axis[1];
    up[2] = 0.0f;
    if (up[0] == 0.0f && up[1] == 0.0f)
        goto store;

    len = sqrtf(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
    if (len != 0.0f) {
        inv = 1.0f / len;
        up[0] = inv * up[0];
        up[1] = inv * up[1];
        up[2] = inv * up[2];
    }

    d = up[0] * v[0] + up[1] * v[1] + up[2] * v[2];
    if (d > 1.0f)
        d = 1.0f;
    else if (d < -1.0f)
        d = -1.0f;
    /* acos: func_00217E28 is declared as returning s32 earlier in this file
     * (func_0016F008 reads that result as an integer), so the float-returning
     * prototype cannot be repeated; go through a cast to read $f0. */
    ang = ((f32 (*)(f32))func_00217E28)(d);
    if (up[0] * v[1] - up[1] * v[0] < 0.0f)
        ang = -ang;
    f25 = ang;

    v[0] = f23 * v[0];
    v[1] = f23 * v[1];
    v[2] = f23 * v[2];

    if (flag != 0 && f24 != 0.0f) {
        m = fabsf(f25);
        x = (f25 * f23) * (f24 * ((f32)ms * D_00392E28));
        if (m < fabsf(x)) {
            x = f25;
            c = m;
        } else {
            c = fabsf(x);
        }
        if (!(c <= f26) && f21 != 0.0f && flag == 1) {
            tr = *(u8 **)(obj + 0x24);
            if (tr != 0) {
                child = *(u8 **)(*(u8 **)(obj + 4) + 8);
                func_00125C80(x, (u8 *)rot);
                func_001232E8(tr, (u8 *)rot, tr);
                func_0014AD50(obj);
                if (child != 0)
                    func_0012D750(*(s32 *)(child + 0x1C), tr);
            }
        }
    }

    f25 = f25 * D_00392E2C;            /* radians -> degrees              */

classify:
    if (st != 0) {
        if (mode == 2) {
            if (n23 < n30)
                st = -n23 < n30 ? 2 : 1;
            else
                st = -n23 < n30 ? 3 : 4;
        } else {
            d = fabsf(f25);
            if (d <= 45.0f)
                st = 1;
            else if (135.0f <= d)
                st = 3;
            else
                st = 0.0f < f25 ? 4 : 2;
        }
    }
    goto store;

plain_store:
    n30 = 0;
    n23 = 0;
    st = 0;
    f25 = 0.0f;
    v[0] = 0.0f;
    v[1] = 0.0f;
    v[2] = 0.0f;

store:
    *(s16 *)(rec + 0x2C) = (s16)n30;
    *(s16 *)(rec + 0x2E) = (s16)n23;
    *(f32 *)(rec + 0x20) = v[0];
    *(f32 *)(rec + 0x24) = v[1];
    *(f32 *)(rec + 0x28) = v[2];
    *(s32 *)(rec + 0x3C) = st;
    *(f32 *)(rec + 0x30) = f23;
    *(f32 *)(rec + 0x34) = f21;
    *(f32 *)(rec + 0x38) = f25;
    return script;
}

INCLUDE_ASM(const s32, "core/main", func_001FC930);

INCLUDE_ASM(const s32, "core/main", func_001FCAA8);

INCLUDE_ASM(const s32, "core/main", func_001FCB20);

s32 func_001FCC50(s32 a1, s32 a2)
{

    f32 tmp;
    f32 f1;
    f32 f2;
    f32 f3;
    s32 v2;
    s32 v4;
    s32 v5;

    v2 = func_001DF760(a1, a2, &tmp);
    f1 = tmp;
    v4 = func_001DF760(a1, v2, &tmp);
    f2 = tmp;
    v5 = func_001DF760(a1, v4, &tmp);
    f3 = tmp;
    func_00104A10(f1, f2, f3);
    return v5;
}

void func_001FCCC8(s32 arg1, s32 arg2)
{
    s32 buffer[4];

    func_001DF760(arg1, arg2, (s32)buffer);
}

void func_001FCCE8(s32 arg1, s32 arg2)
{
    s32 buffer[4];

    func_001DF760(arg1, arg2, (s32)buffer);
}

void func_001FCD08(s32 arg1, s32 arg2)
{
    s32 buffer[4];

    func_001DF760(arg1, arg2, (s32)buffer);
}

/* Byte-matching.  Reads a byte property and applies it with func_0015D4B0,
   returning whatever func_001DF760 reported. */
u8 *func_001FCD28(s32 arg0, s32 arg1)
{
    u8 local;
    u8 *p;

    p = (u8 *)func_001DF760(arg0, arg1, &local);
    func_0015D4B0(local);
    return p;
}

/* Byte-matching.  Same shape as func_001FCD28 with a float property and
   func_0015DEB8. */
u8 *func_001FCD60(s32 arg0, s32 arg1)
{

    float local;
    u8 *p;

    p = (u8 *)func_001DF760(arg0, arg1, &local);
    func_0015DEB8(local);
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_001FCD98);

INCLUDE_ASM(const s32, "core/main", func_001FCE30);

s32 func_001FCED8(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_001FCEE0);

/* Byte-matching.  Calls func_00120C58 with arg0 and returns arg1 unchanged. */
s32 func_001FD190(s32 arg0, s32 arg1)
{


    func_00120C58(arg0);
    return arg1;
}

void func_001FD1B8(s32 a, s32 b)
{
    u8 buffer[16];
    func_001DF760(a, b, buffer);
    D_00393ADC = buffer[0];
}

s32 func_001FD1E0(s32 a1, s32 a2)
{


    s32 v7, v5, v2, result;
    v2 = func_001DF760(a1, a2, &v7);
    v5 = v7;
    result = func_001DF760(a1, v2, &v7);
    if (v5 >= 10) v5 = 9;
    D_00383DD8[v5] = v7;
    return result;
}

s32 func_001FD240(s32 a1, s32 a2)
{


    union { s32 i; f32 f; } v7;
    s32 v5, v2, result;
    v2 = func_001DF760(a1, a2, &v7);
    v5 = v7.i;
    result = func_001DF760(a1, v2, &v7);
    if (v5 >= 10) v5 = 9;
    D_00383E00[v5] = v7.f;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001FD2A0);

INCLUDE_ASM(const s32, "core/main", func_001FD390);

INCLUDE_ASM(const s32, "core/main", func_001FD4B8);

INCLUDE_ASM(const s32, "core/main", func_001FD5B8);

INCLUDE_ASM(const s32, "core/main", func_001FD738);

INCLUDE_ASM(const s32, "core/main", func_001FD8D0);

/* Byte-matching.  Same shape as func_001FCD28 with a word property and
   func_001E0190. */
u8 *func_001FD9F8(s32 arg0, s32 arg1)
{

    s32 local;
    u8 *p;

    p = (u8 *)func_001DF760(arg0, arg1, &local);
    func_001E0190(local);
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_001FDA30);

INCLUDE_ASM(const s32, "core/main", func_001FDBD0);

INCLUDE_ASM(const s32, "core/main", func_001FDC70);

INCLUDE_ASM(const s32, "core/main", func_001FDF80);

INCLUDE_ASM(const s32, "core/main", func_001FE028);

INCLUDE_ASM(const s32, "core/main", func_001FE1A8);

INCLUDE_ASM(const s32, "core/main", func_001FE240);

INCLUDE_ASM(const s32, "core/main", func_001FE478);

INCLUDE_ASM(const s32, "core/main", func_001FE4E8);

s32 func_001FE680(s32 *a, s32 *b)
{
    s32 vb = *b;
    s32 va = *a;
    if (vb >= va)
        return vb != va;
    return -1;
}

s32 func_001FE6A8(s32 *a, s32 *b)
{
    s32 vb = *b;
    s32 va = *a;
    if (va >= vb)
        return vb != va;
    return -1;
}

/* Byte-matching.  -1, 0 or 1 comparing the floats at offset 0.  The second
   argument's float is loaded first, which is what fixes the register order. */
s32 func_001FE6D0(float *arg0, float *arg1)
{
    float b = *arg1;
    float a = *arg0;
    s32 r;

    if (a < b)
        r = -1;
    else if (b == a)
        r = 0;
    else
        r = 1;
    return r;
}

/* Byte-matching.  Same compare with the operands the other way round. */
s32 func_001FE708(float *arg0, float *arg1)
{
    float b = *arg1;
    float a = *arg0;
    s32 r;

    if (b < a)
        r = -1;
    else if (b == a)
        r = 0;
    else
        r = 1;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_001FE740);

s32 func_001FE878(s32 a1, s32 a2)
{
    s32 v3;



    v3 = *(u16 *)(a2 - 8);
    if (v3 == 421) goto c421;
    if (v3 == 422) goto c422;
    return a2;
c421:
    func_001203A8();
    func_00120428();
    return a2;
c422:
    func_001203A8();
    return a2;
}

INCLUDE_ASM(const s32, "core/main", func_001FE8E0);

INCLUDE_ASM(const s32, "core/main", func_001FE9E0);

s32 func_001FEAC0(s32 a1, s32 a2)
{

    f32 f;
    s32 v;

    v = func_001DF760(a1, a2, &f);
    func_0015BAC8((s32)(f * 65536.0f));
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_001FEB10);

INCLUDE_ASM(const s32, "core/main", func_001FEB98);

INCLUDE_ASM(const s32, "core/main", func_001FECD0);

INCLUDE_ASM(const s32, "core/main", func_001FED50);

/* Byte-matching.  Same shape as func_001FCD28 with func_0011B4F0. */
u8 *func_001FEDF8(s32 arg0, s32 arg1)
{

    float local;
    u8 *p;

    p = (u8 *)func_001DF760(arg0, arg1, &local);
    func_0011B4F0(local);
    return p;
}

s32 func_001FEE30(s32 a1, s32 a2) {
if ( *(u16 *)(a2 - 8) == 445 )
func_0011B4F8();
else
func_0011B5B8();
return a2;
}

INCLUDE_ASM(const s32, "core/main", func_001FEE80);

INCLUDE_ASM(const s32, "core/main", func_001FEF40);

INCLUDE_ASM(const s32, "core/main", func_001FF098);

INCLUDE_ASM(const s32, "core/main", func_001FF298);

INCLUDE_ASM(const s32, "core/main", func_001FF380);

s32 func_001FF410(s32 a1, s32 a2) {
s32 v4;
u32 v5[4];
func_001F2F88(a1, v5);
v4 = *(u32 *)(v5[0] + 44) | 8;
*(u32 *)(**(u32 **)(v5[0] + 4) + 220) = v4;
*(u32 *)(v5[0] + 44) = v4;
return a2;
}

INCLUDE_ASM(const s32, "core/main", func_001FF460);

INCLUDE_ASM(const s32, "core/main", func_001FF748);

INCLUDE_ASM(const s32, "core/main", func_001FF9C8);

s32 func_001FFA80(s32 a1, s32 a2)
{


    S12 src;
    S12 dst;
    f32 scale;
    s32 result;

    result = func_001DF760(a1, a2, (s32)&src);
    dst = src;
    scale = D_00392E34;
    ((f32 *)&dst)[0] *= scale;
    ((f32 *)&dst)[1] *= scale;
    ((f32 *)&dst)[2] *= scale;
    func_0011B320((s32)&dst);
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_001FFAF8);

INCLUDE_ASM(const s32, "core/main", func_001FFBB0);

INCLUDE_ASM(const s32, "core/main", func_001FFC78);

INCLUDE_ASM(const s32, "core/main", func_001FFD48);

INCLUDE_ASM(const s32, "core/main", func_001FFE18);

INCLUDE_ASM(const s32, "core/main", func_001FFF00);

INCLUDE_ASM(const s32, "core/main", func_001FFF90);

INCLUDE_ASM(const s32, "core/main", func_001FFFE8);


/* Source: gameplay/region_0020.c */
/* func_00200048 through func_0020FAA0; retail address range 0x00200000-0x00210000. */


s32 func_00200048(s32 a1, s32 a2)
{
    s32 v7;
    u8 v3;
    u8 i;
    s32 result;

    v3 = *(u8 *)(a2 - 2);
    func_001F2F88(a1, &v7);
    for (i = *(u8 *)(a2 + 6); ; i = *(u8 *)(a2 + 6)) {
        if (v3 >= i) {
            result = a2;
            break;
        }
        a2 = func_001DF510(a2);
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_002000A0);

/* Byte-matching. */
s32 func_002001A8(s32 arg0, s32 arg1)
{
    s32 out;
    s32 result = func_001DF760(arg0, arg1, &out);

    *(u8 *)(out + 0x164) &= 0xFE;
    return result;
}

s32 func_002001D8(s32 arg0, s32 arg1) {
    L l;
    s32 result;
    func_001F2F88(arg0, l.buf);
    result = func_001DF760(arg0, arg1, &l.out);
    *(u8 *)(l.out + 0x164) &= 0xFE;
    return result;
}

/* Byte-matching. */
s32 func_00200230(s32 arg0, s32 arg1)
{
    s32 out;
    s32 result = func_001DF760(arg0, arg1, &out);

    *(u8 *)(out + 0x164) |= 1;
    return result;
}

s32 func_00200260(s32 arg0, s32 arg1) {
    L l;
    s32 result;
    func_001F2F88(arg0, l.buf);
    result = func_001DF760(arg0, arg1, &l.out);
    *(u8 *)(l.out + 0x164) |= 1;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_002002B8);

s32 func_00200358(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_00200360);

INCLUDE_ASM(const s32, "core/main", func_00200550);

s32 func_002006F0(void) { return 4; }

s32 func_002006F8(u8 a1, s32 a2, f32 a3)
{
    s32 v3;
    s32 v4;
    s32 result;
    v3 = **(u32 **)(*(u32 *)(a2 + 4) + 12);
    if (*(u8 *)(v3 + 16) != 0)
        v4 = *(u32 *)(v3 + 4);
    else
        v4 = *(u32 *)(v3 + 8);
    result = 104 * *(u8 *)(*(u32 *)(v4 + 16) + 6) + **(u32 **)(v4 + 16);
    *(f32 *)(12 * a1 + result) = a3;
    return result;
}

f32 func_00200748(u8 a1, s32 a2)
{
    s32 v2;
    s32 v3;
    v2 = **(u32 **)(*(u32 *)(a2 + 4) + 12);
    if (*(u8 *)(v2 + 16) != 0)
        v3 = *(u32 *)(v2 + 4);
    else
        v3 = *(u32 *)(v2 + 8);
    return *(f32 *)(104 * *(u8 *)(*(u32 *)(v3 + 16) + 6) + **(u32 **)(v3 + 16) + 12 * a1);
}

s32 func_00200798(u8 a1, s32 a2, s32 a3) {
s32 v3;
s32 v4;
s32 result;
v3 = **(u32 **)(*(u32 *)(a2 + 4) + 12);
if ( *(u8 *)(v3 + 16) != 0 )
v4 = *(u32 *)(v3 + 4);
else
v4 = *(u32 *)(v3 + 8);
result = 104 * *(u8 *)(*(u32 *)(v4 + 16) + 6) + **(u32 **)(v4 + 16);
*(u32 *)(12 * a1 + result) = a3;
return result;
}

s32 func_002007F0(u8 a1, s32 a2)
{
    s32 v2;
    s32 v3;
    v2 = **(u32 **)(*(u32 *)(a2 + 4) + 12);
    if (*(u8 *)(v2 + 16) != 0)
        v3 = *(u32 *)(v2 + 4);
    else
        v3 = *(u32 *)(v2 + 8);
    return *(s32 *)(104 * *(u8 *)(*(u32 *)(v3 + 16) + 6) + **(u32 **)(v3 + 16) + 12 * a1);
}

s32 func_00200840(u8 a1, s32 a2, s32 a3) {
s32 v3;
s32 v4;
s32 result;
v3 = **(u32 **)(*(u32 *)(a2 + 4) + 12);
if ( *(u8 *)(v3 + 16) != 0 )
v4 = *(u32 *)(v3 + 4);
else
v4 = *(u32 *)(v3 + 8);
result = 104 * *(u8 *)(*(u32 *)(v4 + 16) + 6) + **(u32 **)(v4 + 16);
*(u32 *)(12 * a1 + result) = a3;
return result;
}

s32 func_00200898(u8 a1, s32 a2)
{
    s32 v2;
    s32 v3;
    v2 = **(u32 **)(*(u32 *)(a2 + 4) + 12);
    if (*(u8 *)(v2 + 16) != 0)
        v3 = *(u32 *)(v2 + 4);
    else
        v3 = *(u32 *)(v2 + 8);
    return *(s32 *)(104 * *(u8 *)(*(u32 *)(v3 + 16) + 6) + **(u32 **)(v3 + 16) + 12 * a1);
}

INCLUDE_ASM(const s32, "core/main", func_002008E8);

s32 func_00200950(u8 a1, s32 a2)
{
    s32 v2;
    s32 v3;
    v2 = **(u32 **)(*(u32 *)(a2 + 4) + 12);
    if (*(u8 *)(v2 + 16) != 0)
        v3 = *(u32 *)(v2 + 4);
    else
        v3 = *(u32 *)(v2 + 8);
    return **(u32 **)(v3 + 16) + 104 * *(u8 *)(*(u32 *)(v3 + 16) + 6) + 12 * a1;
}

INCLUDE_ASM(const s32, "core/main", func_002009A0);

INCLUDE_ASM(const s32, "core/main", func_002009F8);

INCLUDE_ASM(const s32, "core/main", func_00200A58);

INCLUDE_ASM(const s32, "core/main", func_00200AA8);

INCLUDE_ASM(const s32, "core/main", func_00200B28);

s32 func_00200B90(s32 a1)
{
    s32 result;
    s32 v2;
    s32 v4;
    s32 v5;


    result = 104;
    v2 = *(s32 *)(a1 + 0x10);
    result = result * *(u8 *)(v2 + 6) + *(s32 *)v2;
    if (*(u8 *)(result + 100) != 0)
    {
        func_00200AA8(a1, *(u8 *)(v2 + 6), 0, 0, 0);
        v4 = *(s32 *)(a1 + 0x10);
        v5 = *(u8 *)(v4 + 5) - 1;
        *(u8 *)(v4 + 5) = v5;
        result = v5;
    }
    return result;
}

s32 func_00200BF8(s32 a1, u8 a2)
{
    u32 v3;
    s32 result;
    u64 v5;
    s32 v6;
    s32 v7;
    s32 v8;
    v3 = *(u32 *)(a1 + 16);
    result = *(u8 *)(v3 + 5);
    v5 = a2;
    if (*(u8 *)(v3 + 5) != 0) {
        v6 = *(u8 *)(v3 + 4);
        v7 = 0;
        v8 = *(u32 *)v3;
        if (*(u8 *)(v3 + 4) != 0) {
            do {
                if (*(u8 *)(v8 + 100) != 0) {
                    if (v5 < *(u8 *)(v8 + 101)) {
                        func_00200AA8(a1, (u8)v7, 0, 0, 0);
                        --*(u8 *)(*(u32 *)(a1 + 16) + 5);
                    }
                    v7 = v7 + 1;
                } else {
                    v7 = v7 + 1;
                }
                result = v7 < v6;
                v8 += 104;
            } while (v7 < v6);
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00200CA8);

s32 func_00200D50(s32 a1)
{


    s32 result;
    s32 v3;
    s32 v4;
    *(u8 *)(*(u32 *)(a1 + 16) + 5) = 0;
    *(u8 *)(*(u32 *)(a1 + 16) + 6) = 0;
    result = *(s32 *)(a1 + 16);
    v3 = *(u8 *)(result + 4);
    v4 = 0;
    if (v3 != 0)
    {
        do
        {
            func_00200AA8(a1, (u8)v4, 0, 0, 0);
            v4 = v4 + 1;
            result = v4 < v3;
        }
        while (v4 < v3);
    }
    return result;
}

u32 func_00200DD0(u8 *a1, u8 idx)
{
    u32 v = idx * 104;

    D_00393AE7 = *(u8 *)(v + (u32)**(u32 **)(a1 + 0x10) + 0x66);
    D_00393AE6 = *(u8 *)(v + (u32)**(u32 **)(a1 + 0x10) + 0x67);
    return *(u32 *)(v + (u32)**(u32 **)(a1 + 0x10) + 0x60);
}

void func_00200E18(void) {
}

void func_00200E20(void) {
}

/* Byte-matching.  Converts the int at +0 of the record in place once the tag
   byte at +0xC is set to 8. */
void func_00200E28(u8 *arg0)
{
    if (*(u8 *)(arg0 + 0xC) != 8) {
        f32 v = (f32)*(s32 *)arg0;

        *(u8 *)(arg0 + 0xC) = 8;
        *(f32 *)arg0 = v;
    }
}

void func_00200E50(void) { func_001E0F78(); }

void func_00200E58(void) { func_001E0F78(); }

INCLUDE_ASM(const s32, "core/main", func_00200E60);

void func_00200EE8(void) {
}

void func_00200EF0(void) {
}

/* Byte-matching.  Identical to func_00200E28. */
void func_00200EF8(u8 *arg0)
{
    if (*(u8 *)(arg0 + 0xC) != 8) {
        f32 v = (f32)*(s32 *)arg0;

        *(u8 *)(arg0 + 0xC) = 8;
        *(f32 *)arg0 = v;
    }
}

INCLUDE_ASM(const s32, "core/main", func_00200F20);

INCLUDE_ASM(const s32, "core/main", func_00200F60);

INCLUDE_ASM(const s32, "core/main", func_00200FA0);

void func_00201048(u8 *arg0, s32 arg1)
{
    u32 offset = 4 * (u8)(*arg0)++;
    *(s32 *)(((u32)arg0 + offset) + 4) = arg1;
}

INCLUDE_ASM(const s32, "core/main", func_00201068);

INCLUDE_ASM(const s32, "core/main", func_002010F0);

INCLUDE_ASM(const s32, "core/main", func_00201150);

INCLUDE_ASM(const s32, "core/main", func_002011D0);

INCLUDE_ASM(const s32, "core/main", func_00201250);

INCLUDE_ASM(const s32, "core/main", func_00201328);

INCLUDE_ASM(const s32, "core/main", func_002013A8);

INCLUDE_ASM(const s32, "core/main", func_00201478);

INCLUDE_ASM(const s32, "core/main", func_00201560);

INCLUDE_ASM(const s32, "core/main", func_00201690);

INCLUDE_ASM(const s32, "core/main", func_00201748);

INCLUDE_ASM(const s32, "core/main", func_00201830);

INCLUDE_ASM(const s32, "core/main", func_00201918);

INCLUDE_ASM(const s32, "core/main", func_002019B0);

INCLUDE_ASM(const s32, "core/main", func_00201A48);

extern u8 D_00383EA0[];
extern s32 func_00115620();

s32 func_00201AE0(s32 arg0, u8 arg1)
{
    u16 v[8];
    v[0] = 1;
    func_00115620(arg0, arg1, (s32)D_00383EA0, v);
    return v[0] != 0;
}

s32 func_00201B18(s32 a1, u8 a2, u16 a3)
{


    u16 v4;
    v4 = 1;
    func_001146D8(a1, a2, (s32)D_00383EA0, &v4, a3);
    return v4 != 0;
}

INCLUDE_ASM(const s32, "core/main", func_00201B58);

INCLUDE_ASM(const s32, "core/main", func_00201BB0);

INCLUDE_ASM(const s32, "core/main", func_00201C88);

void func_00201F50(void)
{
    s32 *p = (s32 *)func_00157DE8(D_0039347C, 0xC);

    if (p != 0) {
        p[1] = 0;
        p[2] = 0;
        p[0] = 0;
    }
}

INCLUDE_ASM(const s32, "core/main", func_00201F80);

INCLUDE_ASM(const s32, "core/main", func_00202050);

INCLUDE_ASM(const s32, "core/main", func_00202180);

s32 func_00202218(s32 *a1, s32 a2, s32 *a3)
{
    s32 *i;

    if (a1 != 0 && a2 != 0) {
        for (i = (s32 *)*a1; i != 0; i = (s32 *)*i) {
            if (i[3] == a2) {
                *a3 = i[4];
                return 0;
            }
        }
    }
    return -1;
}

s32 func_00202258(s32 *a1, s32 a2, s32 a3)
{
    s32 *i;

    if (a1 != 0 && a2 != 0) {
        for (i = (s32 *)*a1; i != 0; i = (s32 *)*i) {
            if (i[3] == a2) {
                i[6] = a3;
                return 0;
            }
        }
    }
    return -1;
}

s32 func_00202298(s32 *a1, s32 a2, s32 *a3)
{
    s32 *i;

    if (a1 != 0 && a2 != 0) {
        for (i = (s32 *)*a1; i != 0; i = (s32 *)*i) {
            if (i[3] == a2) {
                *a3 = i[6];
                return 0;
            }
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "core/main", func_002022D8);

void func_00202340(s32 arg)
{
    s32 dummy = 0;
    if (arg != 0)
        func_0014CBE8(D_0039347C, arg);
    dummy = 1;
}

void func_00202368(void) {
}

void func_00202370(u8 *src, u8 *dst)
{
    s32 v32 = *(s32 *)(src + 8);
    ((PK8 *)dst)->v = ((PK8 *)src)->v;
    *(s32 *)(dst + 8) = v32;
}

f32 func_00202390(const f32 *p) { return p[3]; }

void func_00202398(void) {
}

void func_002023A0(u8 *a1, u32 a2)
{

    S12 tmp;

    if (*(s32 *)(a1 + 0x10) != 0) {
        *(S12 *)&tmp = *(S12 *)a1;
        func_00121278(a2, *(s32 *)(*(s32 *)(a1 + 0x10) + 0x24), (s32)&tmp);
    } else {
        *(S12 *)a2 = *(S12 *)a1;
    }
}

void func_00202408(void) {
}

s32 func_00202410(s32 a1, s32 a2)
{


    s32 v3;
s32 v4;
s32 v5;
s32 v6;
v3 = func_00200550(a1, a2);
v4 = func_00200898(0, a1);
v5 = v4;
*(u32 *)(**(u32 **)(a1 + 4) + 8) = 0;
v6 = func_0010E970(a1, v4, 0, 0, 0);
func_00200798(0, a1, v6);
func_00200840(1u, a1, v5);
return v3;
}

/* Byte-matching: 4-call chain; the final ternary must be written
   `v != 0 ? 4 : 1` so GCC emits movz (not movn) with 1 in $4 and 4 in $3. */
s32 func_002024A0(s32 a1)
{


    s32 v2 = func_00200898(1, a1);

    if (func_002007F0(0, a1) == 0 || func_0010EF58(a1, v2) == 0)
        return 4;
    return func_0010F2E8(a1) != 0 ? 4 : 1;
}

s32 func_0010F2E8(void);

s32 func_00202520(void)
{
    return func_0010F2E8() ? 4 : 1;
}

s32 func_00202550(void)
{
    return func_0010F318() ? 4 : 1;
}

s32 func_00202580(s32 a1, s32 a2)
{


    s32 v3 = func_00200550(a1, a2);
    s32 v4 = func_0014B8F0();
    s32 v5 = func_002007F0(0, a1);
    func_00200798(0, a1, v4 + v5);
    return v3;
}

s32 func_002025E0(s32 a1)
{
    u64 v2;
    s64 v3;
    s32 r;
    v2 = func_0014B8F0();
    v3 = v2 < func_002007F0(0, a1);
    r = 4;
    if (v3 != 0)
        r = 1;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_00202630);

INCLUDE_ASM(const s32, "core/main", func_00202868);

INCLUDE_ASM(const s32, "core/main", func_00202AB8);

INCLUDE_ASM(const s32, "core/main", func_00202F28);

INCLUDE_ASM(const s32, "core/main", func_002030C8);

s32 func_002033D8(s32 a1)
{
u32 v2;
  s32 v3;
  s32 v4;
  s32 result;

  v2 = func_0014B8F0();
  v3 = func_002007F0(3u, a1);
  v4 = v2 < (s32)(v3 + func_002007F0(2u, a1));
  result = 1;
  if ( v4 == 0 )
  {
    *(u32 *)(*(u32 *)(*(u32 *)(a1 + 4) + 4) + 44) &= ~0x8000u;
    return 4;
  }
  return result;
}

INCLUDE_ASM(const s32, "core/main", func_00203460);

INCLUDE_ASM(const s32, "core/main", func_002035E8);

INCLUDE_ASM(const s32, "core/main", func_00203780);

INCLUDE_ASM(const s32, "core/main", func_00203918);

INCLUDE_ASM(const s32, "core/main", func_00203AA8);

INCLUDE_ASM(const s32, "core/main", func_00203C38);

INCLUDE_ASM(const s32, "core/main", func_00203EF8);

INCLUDE_ASM(const s32, "core/main", func_002040A8);

INCLUDE_ASM(const s32, "core/main", func_002041C8);

INCLUDE_ASM(const s32, "core/main", func_00204300);

INCLUDE_ASM(const s32, "core/main", func_002044A0);

INCLUDE_ASM(const s32, "core/main", func_00204590);

INCLUDE_ASM(const s32, "core/main", func_00204690);

INCLUDE_ASM(const s32, "core/main", func_00204998);

s32 func_00204BA0(u8 *arg0, u8 *arg1)
{
    u8 *p = *(u8 **)(arg0 + 0xC);
    f32 v = *(f32 *)(arg1 + 8);
    f32 lo = *(f32 *)(p + 0x64);
    f32 hi;

    if (lo < v) {
        *(f32 *)(arg1 + 8) = lo;
        __asm__ volatile("" ::: "memory");
        p = *(u8 **)(arg0 + 0xC);
    }
    hi = *(f32 *)(p + 0x60);
    if (v < hi)
        *(f32 *)(arg1 + 8) = hi;
    return (s32)p;
}

INCLUDE_ASM(const s32, "core/main", func_00204BE0);

INCLUDE_ASM(const s32, "core/main", func_00204F50);

INCLUDE_ASM(const s32, "core/main", func_00204FC0);

INCLUDE_ASM(const s32, "core/main", func_002055A0);

INCLUDE_ASM(const s32, "core/main", func_00205810);

f32 func_00205A60(u8 *a1)
{


    f32 v1 = D_00253F78[0] + D_00253F78[0];
    if (*(f32 *)(a1 + 0x7C) > 0.0f)
        v1 = -v1;
    return func_001E3F18(v1 + *(f32 *)(a1 + 0x78));
}

INCLUDE_ASM(const s32, "core/main", func_00205AA8);

void func_00205CA0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00205CA8);

extern s32 func_001E3A40(s32);

void func_00205DA0(s32 *p)
{
    func_001E3A40(*(s32 *)((u8 *)p[3] + 0x68));
}

INCLUDE_ASM(const s32, "core/main", func_00205DB0);

INCLUDE_ASM(const s32, "core/main", func_002061E0);

f32 func_002063F8(s32 a1, f32 *a2, f32 *a3, f32 *a4, f32 *a5, s32 a6)
{
    if (a6 == 1)
    {
        if ((*(u8 *)(a1 + 0x71) & 1) != 0)
            return *a4;
        return *a3;
    }
    if (a6 == -1)
        return *a2;
    if ((*(u8 *)(a1 + 0x72) & 2) != 0)
        return *a5;
    return *a3;
}

s32 func_00206450(u8 *arg0, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    s32 flag;

    if (a6 == 1) {
        flag = arg0[0x71] & 1;
        if (flag == 0)
            return a3;
        return a4;
    } else if (a6 == -1) {
        return a2;
    } else {
        flag = arg0[0x72] & 2;
        if (flag != 0)
            return a5;
        return a3;
    }
}

INCLUDE_ASM(const s32, "core/main", func_00206490);

INCLUDE_ASM(const s32, "core/main", func_002065C0);

INCLUDE_ASM(const s32, "core/main", func_00206980);

INCLUDE_ASM(const s32, "core/main", func_00206A60);

s32 func_00206B20(s32 a1, s32 a2, u32 a3) {
u32 v6[4];
func_001F2F88(a1, v6);
*(u32 *)a3 = func_00146740(**(u32 **)(*(u32 *)(v6[0] + 4) + 32));
*(u8 *)(a3 + 12) = 6;
return a2;
}

INCLUDE_ASM(const s32, "core/main", func_00206B78);

INCLUDE_ASM(const s32, "core/main", func_00206C50);

INCLUDE_ASM(const s32, "core/main", func_00206D30);

INCLUDE_ASM(const s32, "core/main", func_00206D98);

INCLUDE_ASM(const s32, "core/main", func_00206F00);

INCLUDE_ASM(const s32, "core/main", func_00206F68);

INCLUDE_ASM(const s32, "core/main", func_00206FA8);

INCLUDE_ASM(const s32, "core/main", func_00207238);

INCLUDE_ASM(const s32, "core/main", func_00207340);

INCLUDE_ASM(const s32, "core/main", func_00207420);

INCLUDE_ASM(const s32, "core/main", func_00207560);

INCLUDE_ASM(const s32, "core/main", func_00207888);

INCLUDE_ASM(const s32, "core/main", func_00207948);

INCLUDE_ASM(const s32, "core/main", func_00207A10);

INCLUDE_ASM(const s32, "core/main", func_00207B88);

INCLUDE_ASM(const s32, "core/main", func_00207CF8);

INCLUDE_ASM(const s32, "core/main", func_00207DA0);

INCLUDE_ASM(const s32, "core/main", func_00207E88);

INCLUDE_ASM(const s32, "core/main", func_00207F50);

INCLUDE_ASM(const s32, "core/main", func_002081B8);

INCLUDE_ASM(const s32, "core/main", func_002082D0);

INCLUDE_ASM(const s32, "core/main", func_002083A8);

s32 func_00208480(s32 a1, s32 a2, s32 *a3)
{
    s32 *p;
    s32 v;

    func_001F2F88(a1, &p);
    v = 0;
    if (*p == 2)
        v = p[1];
    *a3 = (*(u8 *)(*(s32 *)(v + 4) + 0x34) >> 1) & 1;
    return a2;
}

INCLUDE_ASM(const s32, "core/main", func_002084E8);

INCLUDE_ASM(const s32, "core/main", func_00208560);

INCLUDE_ASM(const s32, "core/main", func_002086B8);

INCLUDE_ASM(const s32, "core/main", func_00208758);

INCLUDE_ASM(const s32, "core/main", func_00208878);

INCLUDE_ASM(const s32, "core/main", func_00208990);

INCLUDE_ASM(const s32, "core/main", func_00208AF8);

INCLUDE_ASM(const s32, "core/main", func_00208B90);

INCLUDE_ASM(const s32, "core/main", func_00208E28);

INCLUDE_ASM(const s32, "core/main", func_00208EF8);

INCLUDE_ASM(const s32, "core/main", func_00208FE8);

INCLUDE_ASM(const s32, "core/main", func_00209188);

INCLUDE_ASM(const s32, "core/main", func_002091F8);

INCLUDE_ASM(const s32, "core/main", func_002092B0);

s32 func_00209398(s32 unused, s32 value) { return value; }

s32 func_002093A0(s32 unused, s32 value) { return value; }

/* Byte-matching.  Passes the result of func_001207E8 back through arg2 and
   returns arg1. */
s32 func_002093A8(s32 arg0, s32 arg1, s32 *arg2)
{


    *arg2 = func_001207E8(arg0);
    return arg1;
}

u8 *func_002093E0(u8 *unused, u8 *ret, u8 *out)
{
    *(s32 *)out = D_00253627;
    return ret;
}

/* Byte-matching.  Same as func_002093A8 through func_0015D4B8. */
s32 func_002093F8(s32 arg0, s32 arg1, s32 *arg2)
{
    *arg2 = func_0015D4B8(arg0);
    return arg1;
}

/* Byte-matching.  Same as func_002093A8 through func_0015D4D8. */
s32 func_00209430(s32 arg0, s32 arg1, s32 *arg2)
{
    *arg2 = func_0015D4D8(arg0);
    return arg1;
}

s32 func_001DF760();
s32 func_00209468(s32 a1, s32 a2, s32 *out)
{
    s32 stack[2];
    s32 v = func_001DF760(a1, a2, stack);
    s32 *p = *(s32 **)stack;
    s32 flag = 0;
    if (p != 0) flag = *p != 0;
    *out = flag;
    return v;
}

s32 func_002094B0(s32 a1, s32 a2, s32 *a3)
{
  s32 v5;
  u32 v7[4];
  func_001F2F88(a1, v7);
  if (v7[0] == D_00252398[0])
  {
    *a3 = 0;
  }
  else
  {
    v5 = *(u32 *)(*(u32 *)(v7[0] + 4) + 4);
    if (v5 != 0)
    {
      if ((*(u32 *)(v5 + 44) & 0x4C0000) != 0)
        *a3 = *(u8 *)(v5 + 52) >> 7;
      else
        *a3 = 0;
    }
  }
  return a2;
}

INCLUDE_ASM(const s32, "core/main", func_00209528);

INCLUDE_ASM(const s32, "core/main", func_002095C8);

INCLUDE_ASM(const s32, "core/main", func_00209648);

void func_002096C8(f32 *a1, f32 *a2, f32 *a3)
{
    a1[0]  = a2[0];
    a1[1]  = a2[1];
    a1[2]  = a2[2];
    a1[3]  = a2[0];
    a1[4]  = a2[1];
    a1[5]  = a3[2];
    a1[6]  = a2[0];
    a1[7]  = a3[1];
    a1[8]  = a2[2];
    a1[9]  = a2[0];
    a1[10] = a3[1];
    a1[11] = a3[2];
    a1[12] = a3[0];
    a1[13] = a2[1];
    a1[14] = a2[2];
    a1[15] = a3[0];
    a1[16] = a2[1];
    a1[17] = a3[2];
    a1[18] = a3[0];
    a1[19] = a3[1];
    a1[20] = a2[2];
    a1[21] = a3[0];
    a1[22] = a3[1];
    a1[23] = a3[2];
}

void func_00209790(f32 *lo, f32 *hi, f32 *c, f32 r)
{
    lo[0] = c[0] - r;
    lo[1] = c[1] - r;
    lo[2] = c[2] - r;
    hi[0] = c[0] + r;
    hi[1] = c[1] + r;
    hi[2] = c[2] + r;
}

INCLUDE_ASM(const s32, "core/main", func_002097E0);

/* Byte-matching: 6-condition float bound with nested final return. */
s32 func_00209860(f32 *a1, f32 *a2, f32 *a3, f32 *a4)
{
    if (a3[0] > a2[0] && a4[0] < a1[0] && a3[1] > a2[1] && a4[1] < a1[1] && a3[2] > a2[2]) {
        if (a4[2] < a1[2]) return 1;
    }
    return 0;
}


extern s32 D_00393AF4;
extern s32 D_00393AF8;

void func_002098E8(s32 value, s32 unused) { D_00393AF4 = value; D_00393AF8 = 0; }

INCLUDE_ASM(const s32, "core/main", func_002098F8);

INCLUDE_ASM(const s32, "core/main", func_00209AF8);

extern u8 D_00383F28[];
s32 func_00209BE0(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4)
{
    s32 r;
    ((void (*)())func_002098E8)((s32)&D_00383F28);
    func_0020AF68(0, p0);
    func_002098F8(p1, p2, p3);
    r = ((s32 *)D_00383F28)[9];
    *(s32 *)p4 = r;
    return r;
}

extern u8 D_00383F28[];
s32 func_00209C70(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4)
{
    s32 r;
    ((void (*)())func_002098E8)((s32)&D_00383F28);
    func_0020B398(0, p0);
    func_002098F8(p1, p2, p3);
    r = ((s32 *)D_00383F28)[9];
    *(s32 *)p4 = r;
    return r;
}

extern u8 D_00383F50[];
s32 func_00209D00(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4)
{
    s32 r;
    ((void (*)())func_002098E8)((s32)&D_00383F50);
    func_0020B398(0, p0);
    func_002098F8(p1, p2, p3);
    r = ((s32 *)D_00383F50)[9];
    *(s32 *)p4 = r;
    return r;
}

extern u8 D_00383F28[];
s32 func_00209D90(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4)
{
    s32 r;
    ((void (*)())func_002098E8)((s32)&D_00383F28);
    func_0020B6B8(0, p0);
    func_002098F8(p1, p2, p3);
    r = ((s32 *)D_00383F28)[9];
    *(s32 *)p4 = r;
    return r;
}

extern u8 D_00383F50[];
s32 func_00209E20(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4)
{
    s32 r;
    ((void (*)())func_002098E8)((s32)&D_00383F50);
    func_0020B6B8(0, p0);
    func_002098F8(p1, p2, p3);
    r = ((s32 *)D_00383F50)[9];
    *(s32 *)p4 = r;
    return r;
}

extern u8 D_00383F28[];
s32 func_00209EB0(s32 arg0)
{

    ((void (*)())func_002098E8)((s32)&D_00383F28);
    return func_0020B530(0, arg0);
}

INCLUDE_ASM(const s32, "core/main", func_00209EE8);

INCLUDE_ASM(const s32, "core/main", func_0020A098);

INCLUDE_ASM(const s32, "core/main", func_0020A520);

INCLUDE_ASM(const s32, "core/main", func_0020A9B0);

INCLUDE_ASM(const s32, "core/main", func_0020AE40);

INCLUDE_ASM(const s32, "core/main", func_0020AEF0);

INCLUDE_ASM(const s32, "core/main", func_0020AF68);

INCLUDE_ASM(const s32, "core/main", func_0020B098);

INCLUDE_ASM(const s32, "core/main", func_0020B0F8);

INCLUDE_ASM(const s32, "core/main", func_0020B160);

INCLUDE_ASM(const s32, "core/main", func_0020B1C8);

INCLUDE_ASM(const s32, "core/main", func_0020B398);

/* Byte-matching.  Sets up with both arguments, then tail-calls the handler
   with a zero first argument (the second parameter of func_002098E8 is never
   read by its body). */
void func_0020B500(s32 arg0, s32 arg1)
{


    func_002098E8(arg0, arg1);
    func_0020B398(0, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_0020B530);

INCLUDE_ASM(const s32, "core/main", func_0020B6B8);

/* Byte-matching.  Word at +4 of a 0xC-byte table entry, reached through two
   pointer indirections. */
s32 func_0020B810(u8 *arg0, s16 arg1, s16 arg2)
{
    u32 *list = *(u32 **)(arg0 + 0xC);
    u32 base = *(u32 *)list[arg1];

    return *(s32 *)(base + arg2 * 0xC + 4);
}

s32 func_0020B848(u8 *a, s32 b)
{
    return *(s16 *)(*(s32 *)(((b << 16) >> 14) + *(s32 *)(a + 12)) + 4);
}

/* Byte-matching.  Stores a float into the same table entry addressing. */
void func_0020B868(u8 *arg0, s16 arg1, s16 arg2, float arg3)
{
    u32 *list = *(u32 **)(arg0 + 0xC);
    u32 base = *(u32 *)list[arg1];

    *(float *)(base + arg2 * 0xC) = arg3;
}

INCLUDE_ASM(const s32, "core/main", func_0020B8A0);

/* Behavior-faithful, not byte-matching: same double indirection with the
   0x20-byte entry stride and the same size 0x2C; only $3/$6 are swapped. */
s32 func_0020B8F0(u8 *arg0, s16 arg1, s16 arg2)
{
    u32 *base = *(u32 **)(arg0 + 0xC);
    u32 *tbl = *(u32 **)(base + arg1);
    u32 v = *tbl;

    return *(s32 *)(((u32)arg2 << 3) + v + 4);
}

s32 func_0020B920(u8 *a, s32 b)
{
    return *(s16 *)(*(s32 *)(((b << 16) >> 14) + *(s32 *)(a + 12)) + 4);
}

INCLUDE_ASM(const s32, "core/main", func_0020B940);

s32 func_0020BD68(u8 *p, s32 index)
{
    return *(s16 *)(*(u32 *)(p + 0x8) + (s16)index * 2);
}

/* Byte-matching.  Copies one 12-byte record into the table entry at
   base->f4 + index * 12. */
void func_0020BD80(u8 *arg0, u8 *arg1, s16 arg2)
{
    *(Struct12 *)(*(u32 *)(arg0 + 4) + arg2 * 12) = *(Struct12 *)arg1;
}

/* Byte-matching.  Copies the table entry at base->f4 + index * 12 into the
   destination record. */
void func_0020BDB8(u8 *arg0, u8 *arg1, s16 arg2)
{
    *(Struct12 *)arg1 = *(Struct12 *)(*(u32 *)(arg0 + 4) + arg2 * 12);
}

s16 func_0020BDF0(const s16 *p) { return p[1]; }

INCLUDE_ASM(const s32, "core/main", func_0020BDF8);

s32 func_0020BF80(s32 value) { return value; }

u8 *func_0020BF88(u8 *p) { return p + 12; }

void func_0020BF90(Struct12 *dst, f32 *out, s32 *a2)
{
    s32 *t = *(s32 **)a2[3];
    s32 *e = *(s32 **)t;
    s16 idx = *(s16 *)((u8 *)e + 8);
    Struct12 *src = (Struct12 *)(a2[1] + idx * 12);

    *dst = *src;
    *out = *(f32 *)e;
}

s32 func_0020BFD8(s32 value) { return value; }

f32 func_0020BFE0(const f32 *p) { return p[3]; }

INCLUDE_ASM(const s32, "core/main", func_0020BFE8);

INCLUDE_ASM(const s32, "core/main", func_0020C0D0);

INCLUDE_ASM(const s32, "core/main", func_0020C360);

void func_0020C430(u8 *arg0)
{
    S12 tmp;

    if (arg0[0x244] != 0) {
        f32 v = *(f32 *)(arg0 + 0x20);
        *(f32 *)(arg0 + 0x20) = *(f32 *)(arg0 + 0x240);
        *(f32 *)(arg0 + 0x240) = v;
        *(S12 *)&tmp = *(S12 *)(arg0 + 0x14);
        *(S12 *)(arg0 + 0x14) = *(S12 *)(arg0 + 0x22C);
        *(S12 *)(arg0 + 0x22C) = *(S12 *)&tmp;
        arg0[0x244] = 0;
    }
}

void func_0020C4A0(u8 *arg0)
{
    S12 tmp;

    if (arg0[0x244] == 0) {
        f32 v = *(f32 *)(arg0 + 0x20);
        *(f32 *)(arg0 + 0x20) = *(f32 *)(arg0 + 0x240);
        *(f32 *)(arg0 + 0x240) = v;
        *(S12 *)&tmp = *(S12 *)(arg0 + 0x14);
        *(S12 *)(arg0 + 0x14) = *(S12 *)(arg0 + 0x22C);
        *(S12 *)(arg0 + 0x22C) = *(S12 *)&tmp;
        arg0[0x244] = 1;
    }
}

void func_0020C518(void)
{






    func_0014C280();
    func_0014C2B8(D_00250C30);
    while (func_0014C320() != 0) {}
    D_00393B34 = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0020C558);

INCLUDE_ASM(const s32, "core/main", func_0020C660);

INCLUDE_ASM(const s32, "core/main", func_0020C998);

INCLUDE_ASM(const s32, "core/main", func_0020CAD8);

INCLUDE_ASM(const s32, "core/main", func_0020CC68);

INCLUDE_ASM(const s32, "core/main", func_0020D030);

INCLUDE_ASM(const s32, "core/main", func_0020D1C8);

INCLUDE_ASM(const s32, "core/main", func_0020D558);

INCLUDE_ASM(const s32, "core/main", func_0020D6B8);

INCLUDE_ASM(const s32, "core/main", func_0020D798);

INCLUDE_ASM(const s32, "core/main", func_0020D818);

INCLUDE_ASM(const s32, "core/main", func_0020D8E8);

INCLUDE_ASM(const s32, "core/main", func_0020D940);

s32 func_0020D9E0(f32 *a1, f32 *a2, f32 *a3)
{
    if (*(f32 *)((u8 *)a1 + 0) < *(f32 *)((u8 *)a2 + 0)) return 0;
    if (*(f32 *)((u8 *)a3 + 0) < *(f32 *)((u8 *)a1 + 0)) return 0;
    if (*(f32 *)((u8 *)a1 + 0 + 4) < *(f32 *)((u8 *)a2 + 0 + 4)) return 0;
    if (*(f32 *)((u8 *)a3 + 0 + 4) < *(f32 *)((u8 *)a1 + 0 + 4)) return 0;
    return 1;
}

s32 func_0020DA40(f32 *a1, f32 *a2, f32 *a3)
{
    if (*(f32 *)((u8 *)a1 + 4) < *(f32 *)((u8 *)a2 + 4)) return 0;
    if (*(f32 *)((u8 *)a3 + 4) < *(f32 *)((u8 *)a1 + 4)) return 0;
    if (*(f32 *)((u8 *)a1 + 4 + 4) < *(f32 *)((u8 *)a2 + 4 + 4)) return 0;
    if (*(f32 *)((u8 *)a3 + 4 + 4) < *(f32 *)((u8 *)a1 + 4 + 4)) return 0;
    return 1;
}

s32 func_0020DAA0(f32 *a1, f32 *a2, f32 *a3)
{
    if (*a1 < *a2) return 0;
    if (*a3 < *a1) return 0;
    if (a1[2] < a2[2]) return 0;
    if (a3[2] < a1[2]) return 0;
    return 1;
}

/* Byte-matching: 6 float bounds checks, explicit < early-outs. */
s32 func_0020DB00(f32 *a1, f32 *a2, f32 *a3)
{
    if (a1[0] < a2[0]) return 0;
    if (a3[0] < a1[0]) return 0;
    if (a1[1] < a2[1]) return 0;
    if (a3[1] < a1[1]) return 0;
    if (a1[2] < a2[2]) return 0;
    if (a3[2] < a1[2]) return 0;
    return 1;
}


INCLUDE_ASM(const s32, "core/main", func_0020DB88);

INCLUDE_ASM(const s32, "core/main", func_0020DD68);

INCLUDE_ASM(const s32, "core/main", func_0020E058);

INCLUDE_ASM(const s32, "core/main", func_0020E4A0);

INCLUDE_ASM(const s32, "core/main", func_0020E670);

INCLUDE_ASM(const s32, "core/main", func_0020EA38);

INCLUDE_ASM(const s32, "core/main", func_0020EBE8);

INCLUDE_ASM(const s32, "core/main", func_0020EEA0);

INCLUDE_ASM(const s32, "core/main", func_0020EFF8);

INCLUDE_ASM(const s32, "core/main", func_0020F0F0);

INCLUDE_ASM(const s32, "core/main", func_0020F1E8);

INCLUDE_ASM(const s32, "core/main", func_0020F238);

INCLUDE_ASM(const s32, "core/main", func_0020F398);

INCLUDE_ASM(const s32, "core/main", func_0020F5A0);

INCLUDE_ASM(const s32, "core/main", func_0020F800);

INCLUDE_ASM(const s32, "core/main", func_0020F958);

INCLUDE_ASM(const s32, "core/main", func_0020FAA0);


/* Source: file_io/region_0021.c */
/* func_002102D0 through func_0021FF88; retail address range 0x00210000-0x00220000. */




extern s32 D_00387534[8];

extern s32 D_00393B5C;

INCLUDE_ASM(const s32, "core/main", func_002102D0);

INCLUDE_ASM(const s32, "core/main", func_002108E8);

INCLUDE_ASM(const s32, "core/main", func_00210940);

INCLUDE_ASM(const s32, "core/main", func_00210BF8);

INCLUDE_ASM(const s32, "core/main", func_002114A0);

void func_00211630(s32 a, s32 b, s32 c, s32 d, s32 *out)
{
    *(s16 *)out = 0;

    if (a != 0) {
        a = *(s32 *)a;

        if (a != 0) {
            func_002114A0(a, b, c, d, out);
        }
    }
}

INCLUDE_ASM(const s32, "core/main", func_00211658);

INCLUDE_ASM(const s32, "core/main", func_00211718);

void func_002118C8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 *out)
{
    *(s16 *)out = 0;

    if (a != 0) {
        a = *(s32 *)(u32)a;

        if (a != 0) {
            func_00211718(a, b, c, d, e, out);
        }
    }
}

s32 func_002118F0(s32 a0, s32 a1, ...)
{
    return 0;
}

/* Byte-matching.  Variadic function whose body is just the o32 save area. */
s32 func_00211918(s32 a0, ...)
{
    return 0;
}

/* Byte-matching.  Delay loop of arg0*0x3999 iterations, always reports 0. */
s32 func_00211948(u32 arg0)
{
    u32 n = arg0 * 0x3999;
    u32 i;

    for (i = 0; i < n; i++)
        ;
    return 0;
}

u8 *func_00211980(u8 *s, s32 c)
{
    s8 ch;
    u8 b;
    u8 *p = s;

    ch = *(s8 *)p;
    b = *p;
    if (ch != 0 && ch != (s8)c) {
        for (;;) {
            p++;
            ch = *(s8 *)p;
            b = *p;
            if (ch == 0 || ch == (s8)c)
                break;
        }
    }
    if (b != 0)
        return p;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_002119C0);

INCLUDE_ASM(const s32, "core/main", func_00211A20);

INCLUDE_ASM(const s32, "core/main", func_00211A78);

void func_00211B68(void)
{
    func_002256E0(0);
    func_00144DA8();
}

INCLUDE_ASM(const s32, "core/main", func_00211B88);

INCLUDE_ASM(const s32, "core/main", func_00211C38);

void func_00211D00(void)
{

    s32 result = D_00393B58;

    if (result != 0) {
        result = D_003864C8[1];
        if (result != 0) {
            func_0014CBE8(D_0039347C, D_003864C8[0]);
            D_003864C8[0] = 0;
            D_003864C8[1] = 0;
        }
    }
}

s32 func_00211D50(void)
{

    s32 five = 5;
    u8 v = D_00393B58;
    if (v == 0) {
        if (func_00211B88() != 0)
            D_00393B58 = 1;
        D_00393B5C = five;
        return five;
    }
    return v;
}

extern s32 D_003864D4[];
extern s32 D_00393B60[];
void func_00211D00();
void func_00112408();
void func_00112D80();
void func_00211D88(void)
{
    if (D_00393B58 != 0) {
        func_00211D00();
        D_00393B58 = 0;
        func_00112408(D_003864D4);
        func_00112D80(D_00393B60);
        D_00393B5C = 70;
    }
}

INCLUDE_ASM(const s32, "core/main", func_00211DD0);

INCLUDE_ASM(const s32, "core/main", func_00211EB8);

s32 func_00211F18(void) { return D_00393B5C; }

extern s32 D_00386D18[];
extern s32 D_0038FF48[];
extern s32 D_0038FF58[];
extern s32 func_00224B88();
extern s32 func_00211F58();

void func_00211F20(void)
{
    func_00224B88((s32)D_00386D18, 0, 0xF0);
    func_00211F58((s32)D_0038FF48, (s32)D_0038FF58);
}

s32 func_002128A0(void);

s32 func_00211F58(void)
{
    return func_002128A0();
}

s32 func_00211F78(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{


    s32 r = func_00212CA8(a1, a2, a3, a4, a5, a6, a7, a8);
    if (r == 0) return func_00213578(a1, a2, a3);
    return r;
}

s32 func_00211FD0(u8 *arg0, u8 *arg1) {

    u32 v;
    v = func_00212950();
    if (v >= 15) v = func_00212E28(arg0, arg1);
    return v;
}

s32 func_00212028(s32 a1)
{
    s32 v = func_00212970(a1);

    if ((u32)v >= 0xF) {
        v = func_00213510(a1);
    }
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_00212070);

INCLUDE_ASM(const s32, "core/main", func_00212100);

INCLUDE_ASM(const s32, "core/main", func_00212158);

INCLUDE_ASM(const s32, "core/main", func_00212188);

INCLUDE_ASM(const s32, "core/main", func_00212220);

INCLUDE_ASM(const s32, "core/main", func_002122B8);

INCLUDE_ASM(const s32, "core/main", func_00212348);

INCLUDE_ASM(const s32, "core/main", func_002123E8);

INCLUDE_ASM(const s32, "core/main", func_00212480);

INCLUDE_ASM(const s32, "core/main", func_002124E0);

INCLUDE_ASM(const s32, "core/main", func_002125D0);

INCLUDE_ASM(const s32, "core/main", func_00212720);

INCLUDE_ASM(const s32, "core/main", func_002127C8);

extern u8 D_00390198[];

s32 func_00212860(s32 a0)
{
    u32 v = func_00226B48(a0);
    u32 limit = 0x3F3;
    if (limit < v) {
        func_00211918((s32)D_00390198);
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_002128A0);

s32 func_00212970(void);

s32 func_00212950(void)
{
    return func_00212970();
}

INCLUDE_ASM(const s32, "core/main", func_00212970);

s32 func_00212B10(s32 arg0)
{
    s32 *p = &D_00386D18[4 * arg0];
    func_00216978(*p);
    *p = 0;
    return func_002138B0(arg0);
}

INCLUDE_ASM(const s32, "core/main", func_00212B60);

INCLUDE_ASM(const s32, "core/main", func_00212BC0);

extern s32 D_00386D18[];

s32 func_00212C18(s32 arg0)
{
    return func_00216AC8(D_00386D18[4 * arg0]);
}

extern s32 D_00386D18[];

s32 func_00212C48(s32 arg0)
{
    return func_00216AE0(D_00386D18[4 * arg0]);
}

extern s32 D_00386D18[];

s64 func_00212C78(s32 arg0)
{
    s32 v1 = D_00386D18[4 * arg0];
    s64 a = *(s64 *)((u8 *)v1 + 0x10);
    s64 b = (s64) * (s32 *)((u8 *)v1 + 0x18);
    s64 av = (s64)(s32)a;

    if (av < b)
        return b;
    return av;
}

INCLUDE_ASM(const s32, "core/main", func_00212CA8);

INCLUDE_ASM(const s32, "core/main", func_00212DD0);

INCLUDE_ASM(const s32, "core/main", func_00212E28);

INCLUDE_ASM(const s32, "core/main", func_00212FA0);

void func_00213090(void)
{
    func_001CC150();
    func_001CC6B8(0);
}

INCLUDE_ASM(const s32, "core/main", func_002130B0);

INCLUDE_ASM(const s32, "core/main", func_00213178);

INCLUDE_ASM(const s32, "core/main", func_00213258);

s32 func_002133F0(s32 index)
{
    return *(s32 *)((u8 *)D_00386D18 + index * 0x10 + 0x4);
}

INCLUDE_ASM(const s32, "core/main", func_00213408);

s32 func_002134A0(u32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8)
{
    func_001CC6B8(0, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    func_00223E80(D_00386D18[4 * arg1]);
    func_002138B0(arg1);
    return 0;
}

s32 func_002134F0(s32 a)
{
    return func_00213408(a, 0, 2);
}

s32 func_00212E28(s32, s32);

s32 func_00213510(s32 arg1)
{
    return func_00212E28(arg1, 1);
}

extern u8 D_00390550[];

s32 func_00213530(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    s32 *p = &D_00386D18[4 * a1];
    if (p == 0)
        func_00211918((s32)D_00390550, a2, a3, a4, a5, a6, a7, a8);
    return *(s32 *)((u8 *)*p + 4);
}

INCLUDE_ASM(const s32, "core/main", func_00213578);

INCLUDE_ASM(const s32, "core/main", func_00213690);

s32 func_00213848(s32 arg)
{
    return *(s32 *)D_00386D18[arg * 4];
}

extern s32 D_00386D18[];

s32 func_00213868(void)
{
    s32 v0 = 0;
    s32 *p = D_00386D18;
    if (p[3] != 0)
    {
        s32 *v1 = p + 4;
        do
        {
            v0++;
            if (v1[3] == 0)
                break;
            v1 += 4;
        } while (v0 < 15);
    }
    if (v0 <= 14)
        return v0;
    return -1;
}

INCLUDE_ASM(const s32, "core/main", func_002138B0);

s32 *func_00213908(s32 *p, s32 value) { s32 *q = (s32 *)p[1]; q[9] = value; return q; }

s32 func_00213918(s32 *p) { return ((s32 *)p[1])[9]; }

void func_00213928(u8 *p, u32 bits)
{
    u8 *q = *(u8 **)(p + 0x4);
    *(s32 *)(q + 0x24) = *(s32 *)(q + 0x24) | bits;
}

void func_00213940(u8 *p, u32 mask)
{
    u8 *q = *(u8 **)(p + 0x4);
    u32 old = *(u32 *)(q + 0x24);

    *(u32 *)(q + 0x24) = old - (old & mask);
}

INCLUDE_ASM(const s32, "core/main", func_00213958);

INCLUDE_ASM(const s32, "core/main", func_002139C8);

s32 func_00213AA8(u8 *a1, s32 a2, u8 *a3, s32 a4, s32 a5)
{
    s32 *p = *(s32 **)(a3 + 4);
    s32 v = *(s32 *)((u8 *)p + 4);

    if (v != 0) {
        s32 x = *(u8 *)(a1 + 0x30) & 8;
        s32 y = *(s32 *)(a3 + 0x24);
        return func_002175B0(a2, a4, a5, v, y, (s32)a3, 1,
                             *(u8 *)(a3 + 0x30) & 4, x);
    }
    return (s32)p;
}

INCLUDE_ASM(const s32, "core/main", func_00213B00);

INCLUDE_ASM(const s32, "core/main", func_00213DA8);

INCLUDE_ASM(const s32, "core/main", func_00214138);

s32 func_002142C0(s32 *p) { return *((u8 *)p[1] + 0x29); }

u8 *func_002142D0(s32 *p, u8 value) { u8 *q = (u8 *)p[1]; q[0x40] = value; return q; }

u8 *func_002142E0(s32 *p, u8 value) { u8 *q = (u8 *)p[1]; q[0x29] = value; return q; }

u8 func_002142F0(s32 *p) { u8 *object = (u8 *)p[1]; u8 value = object[0x2A]; object[0x29] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_00214300);

INCLUDE_ASM(const s32, "core/main", func_00214330);

s32 func_00214358(s32 *p) { return *((u8 *)p[1] + 0x2A); }

u8 *func_00214368(s32 *p, u8 value) { u8 *q = (u8 *)p[1]; q[0x2A] = value; return q; }

u8 func_00214378(s32 *p) { u8 *object = (u8 *)p[1]; u8 value = object[0x2B]; object[0x2A] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_00214388);

INCLUDE_ASM(const s32, "core/main", func_002143B8);

/* Byte-matching.  Two pointer hops from arg0+4, then a flag test at +0xD6
   before forwarding to func_00106D28 with a zero second argument. */
void func_00214408(u8 *arg0)
{

    u8 *p;

    p = *(u8 **)(*(u8 **)(arg0 + 4));
    if (*(u8 *)(p + 0xD6) != 0)
        func_00106D28(p, 0);
}

INCLUDE_ASM(const s32, "core/main", func_00214430);

u8 *func_002144A0(s16 a1)
{


    u8 *v = (u8 *)func_00157DE8(D_0039347C, 8);
    *(s32 *)v = (s32)func_00157DE8(D_0039347C, 16 * a1);
    *(s16 *)(v + 4) = a1;
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_002144F8);

INCLUDE_ASM(const s32, "core/main", func_00214548);

void func_002145A0(u8 *a, s32 b, s32 val)
{
    *(s32 *)(*(s32 *)(((b << 16) >> 14) + *(s32 *)(a + 4))) = val;
}

void func_002145C0(u8 *a, s32 b, s32 *out)
{
    *out = *(s32 *)*(s32 *)(((b << 16) >> 14) + *(s32 *)(a + 4));
}

INCLUDE_ASM(const s32, "core/main", func_002145E0);

INCLUDE_ASM(const s32, "core/main", func_00214628);

s32 func_002146B8(u8 *p) { return p[8] & 3; }

INCLUDE_ASM(const s32, "core/main", func_002146C8);

INCLUDE_ASM(const s32, "core/main", func_00214930);

INCLUDE_ASM(const s32, "core/main", func_00214AF0);

INCLUDE_ASM(const s32, "core/main", func_00214C68);

INCLUDE_ASM(const s32, "core/main", func_00214E38);

void func_00214EF8(u8 *arg, u8 *guard)
{
    if (guard != 0)
        func_0011AA40(arg);
}

s32 func_00214F10(s32 *p)
{
    return *(u16 *)((u8 *)p[3] + 2);
}

s32 func_00214F20(s32 *p)
{
    return *(s32 *)((u8 *)p[3] + 4);
}

void func_00214F30(s32 arg)
{
    func_0014CBE8(D_0039347C, arg);
}

s32 func_00214F40(s32 arg0)
{

    s32 v = func_0014C9A0(D_0039347C, arg0);
    func_00224B88(v, 0, arg0);
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_00214F90);

INCLUDE_ASM(const s32, "core/main", func_002152C8);

INCLUDE_ASM(const s32, "core/main", func_002153A8);

f32 func_002154D8(f32 a1, f32 a2, f32 a3)
{
    f32 result = a2;

    if (a1 < a3)
        a3 = a1;
    if (result < a3)
        return a3;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00215508);

INCLUDE_ASM(const s32, "core/main", func_00215600);

INCLUDE_ASM(const s32, "core/main", func_00215690);

void func_00215810(void)
{







    func_001CD990(D_00386EF0);
    if (D_00393BE0 != 0)
        func_00223E80(D_00393BE0);
    if (D_00393BC0 != 0) {
        func_00195CE0(D_00393BC0);
        D_00393BC0 = 0;
    }
}

INCLUDE_ASM(const s32, "core/main", func_00215860);

void func_00215B78(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    func_00215BB8(a, b, 0x400, c, d, e, f);
}

void func_00215B98(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    func_00215BB8(a, b, 0, c, d, e, f);
}

INCLUDE_ASM(const s32, "core/main", func_00215BB8);

INCLUDE_ASM(const s32, "core/main", func_00215E28);

void func_00215E78(s32 *a1, s64 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8)
{
    u64 m = 0xFFFFFFFFu;

    a1[3] = (s32)(a2 >> 32);
    a1[1] = (a6 << 14) | (a5 << 15) | (a4 << 26) | (a3 << 28);
    a1[2] = (s32)(a2 & m);
    a1[0] = (a7 << 15) | a8;
}

s32 func_00215EC8(s32 a1, s32 a2, s64 a3)
{
    u64 m = 0xFFFFFFFFu;
    s32 lo = (s32)(a3 & m);
    s32 hi = (s32)(a3 >> 32);

    *(s32 *)(a1 + 8) = a2;
    *(s32 *)(a1 + 0xC) = 0;
    *(s32 *)(a1 + 4) = hi;
    *(s32 *)(a1 + 0) = lo;
    return;
}

s32 func_00215EC8(s32, s32, s64);
void func_00215EF8(s32 a, s32 b, s32 c, s32 d)
{
    s64 hi = ((s64)d << 56) | ((s64)c << 48);
    func_00215EC8(a, 0x50, hi | ((s64)b << 32));
}

void func_00215F18(s32 a, s32 b, s32 c, s32 d)
{
    s64 hi = ((s64)b << 59) | ((s64)d << 48);
    func_00215EC8(a, 0x51, hi | ((s64)c << 32));
}

/* Assembly callers pass the 64-bit packet value as low/high words. */
void func_00215F38(u32 address, u32 low, u32 high)
{
    func_00215EC8(address, 0x52, ((u64)high << 32) | low);
}

void func_00215F50(s32 arg, u32 value)
{
    func_00215EC8(arg, 0x53, (u64)value);
}

/* Byte-matching.  Programs the three display registers from a masked value. */
void func_00215F60(u32 arg0)
{
    *(volatile u32 *)0x1000A030 = arg0 & 0xFFFFFFF;
    *(volatile u32 *)0x1000A020 = 0;
    *(volatile u32 *)0x1000A000 = 0x105;
}

void func_00215FA0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00215FA8);

void func_002160A0(s32 arg0)
{





    func_001497D8(arg0);
    func_0014AC40();
    func_00119150(1);
    func_00215FA8(arg0);
    func_00149810();
}

INCLUDE_ASM(const s32, "core/main", func_002160E0);

INCLUDE_ASM(const s32, "core/main", func_00216170);

INCLUDE_ASM(const s32, "core/main", func_002162E8);

INCLUDE_ASM(const s32, "core/main", func_002163D8);

s32 func_00216468(s32 flag)
{
    s32 r;
    if (flag != 0) {
        r = func_00216488(flag);
    }
    return r;
}

/* Byte-matching.  Releases the two objects and then the object itself. */
void func_00216488(u8 *arg0)
{



    func_00216BE8(*(s32 *)arg0);
    *(u32 *)(arg0 + 4) = 0;
    func_00212348(*(s32 *)(arg0 + 8));
    func_00223E80((s32)arg0);
}

INCLUDE_ASM(const s32, "core/main", func_002164C0);

INCLUDE_ASM(const s32, "core/main", func_00216588);

s32 func_00216648(s32 *arg0)
{
    s32 v2, v3, v4;
    u8 buf[128];
    v2 = *arg0;
    *arg0 = v2 + 1;
    v3 = 0;
    if (*(s8 *)(v2 + 1) != 44) {
        do {
            v4 = *arg0;
            buf[v3++] = *(u8 *)(*arg0)++;
        } while (*(s8 *)(v4 + 1) != 44);
    }
    buf[v3] = 0;
    return func_002220C8((s32)buf);
}

s32 func_002166B8(s32 *arg0)
{
    s32 v2, v3, v4;
    u8 buf[128];
    v2 = *arg0;
    *arg0 = v2 + 1;
    v3 = 0;
    if (*(s8 *)(v2 + 1) != 44) {
        do {
            v4 = *arg0;
            buf[v3++] = *(u8 *)(*arg0)++;
        } while (*(s8 *)(v4 + 1) != 44);
    }
    buf[v3] = 0;
    return func_002220C8((s32)buf);
}

/* Byte-matching: string-copy-to-buffer with 0x5D delimiter (twin of 00216648). */
s32 func_00216728(a1)
s32 *a1;
{
    u8 buf[128];
    s32 p;
    s32 i = 0;
    u8 c;

    p = *a1;
    *a1 = p + 1;
    if (*(s8 *)(p + 1) != 93)
    {
        s32 q;
        do
        {
            q = *a1;
            c = *(u8 *)q;
            *a1 = q + 1;
            buf[i++] = c;
        } while (*(s8 *)(q + 1) != 93);
    }
    buf[i] = 0;
    return func_002220C8((s32)buf);
}

INCLUDE_ASM(const s32, "core/main", func_00216798);

s32 func_00216DF0(s32);

s32 func_00216918(s32 *arg1)
{
    return func_00216DF0(arg1[0]);
}

s32 func_00216938(s32 *a1)
{
    s32 v = func_00216918(a1);
    a1[1] = v;
    if (v != 0) {
        *(s32 *)(v + 0x28) = (s32)a1;
        *(s64 *)(a1[1] + 0x20) = 0;
    }
    __asm__ volatile("" ::: "memory");
    return a1[1];
}

s32 func_00216978(s32 *p) { *(u64 *)((u8 *)p + 0x20) = 0; return 0; }

INCLUDE_ASM(const s32, "core/main", func_00216988);

INCLUDE_ASM(const s32, "core/main", func_00216A50);

s32 func_00216AC8(const u8 *p)
{
    return (s32)*(const s64 *)(p + 0x20);
}

s32 func_00216AE0(u8 *p)
{
    return (s32)(*(s64 *)(p + 0x10) - *(s64 *)(p + 0x20));
}

INCLUDE_ASM(const s32, "core/main", func_00216B00);

s32 func_00216BA0(s32 a1, s32 a2)
{
    s32 *p = (s32 *)func_00223E58(0x10);
    p[1] = a1;
    p[0] = a2;
    p[2] = 0;
    p[3] = 0;
    return (s32)p;
}

INCLUDE_ASM(const s32, "core/main", func_00216BE8);

INCLUDE_ASM(const s32, "core/main", func_00216C90);

INCLUDE_ASM(const s32, "core/main", func_00216D30);

s32 func_00216DD0(s32 cur)
{
    s32 next;
retry:
    next = *(s32 *)(cur + 4);
    if (next != 0) {
        cur = next;
        goto retry;
    }
    return cur;
}

INCLUDE_ASM(const s32, "core/main", func_00216DF0);

INCLUDE_ASM(const s32, "core/main", func_00216EE0);

s32 func_00217000(s32 a1, s32 a2, s64 a3, s64 a4, s64 a5)
{
    s32 result = func_00223E58(48);

    *(s32 *)(result + 0) = a1;
    *(s32 *)(result + 4) = a2;
    *(s64 *)(result + 8) = a3;
    *(s64 *)(result + 16) = a4;
    *(s64 *)(result + 24) = a5;
    *(s64 *)(result + 32) = 0;
    *(s32 *)(result + 40) = 0;
    return result;
}

/* Byte-matching.  Frees the object at +4, then the object itself, both through
   func_00223E80. */
void func_00217078(u8 *arg0)
{


    func_00223E80(*(s32 *)(arg0 + 4));
    func_00223E80((s32)arg0);
}

s32 func_00216DD0(s32);

s32 func_002170A8(s32 *arg1)
{
    return func_00216DD0(arg1[0]);
}

/* Byte-matching.  Releases the object at +4 and reports the object, or NULL
   when the release reported an error.  Mirror of func_0018C4D8, whose movz
   becomes movn here because the sense of the test flips. */
u8 *func_002170C8(u8 *arg0)
{

    u8 *r = arg0;

    if (func_002268F0(*(s32 *)(arg0 + 4)) != 0)
        r = 0;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_002170F8);

INCLUDE_ASM(const s32, "core/main", func_00217318);

INCLUDE_ASM(const s32, "core/main", func_00217388);

INCLUDE_ASM(const s32, "core/main", func_002174C0);

s32 func_002175B0(u8 *a1, s32 a2, s32 a3, u8 *a4, s32 a5, s32 a6, s16 a7, u8 a8, u8 a9)
{



    u8 *p = D_00385FF0;

    D_00393B28 = 1;
    p[0x68] = a8;
    p[0x69] = a9;
    *(s32 *)(p + 0x70) = a3;
    *(s32 *)(p + 0x74) = a2;
    *(s32 *)(p + 0x24) = a5;
    *(s32 *)(p + 0x58) = 0x1FFFFFF;
    *(s32 *)(p + 0x48) = a6;
    *(s16 *)(p + 0x4C) = a7;
    *(s32 *)(p + 0x6C) = (s32)a1;
    *(s32 *)(p + 0x50) = (s32)a4;
    if (a1 == 0 || a4 == 0) return;
    if (*(s16 *)a1 == 0) return;
    if (*(s16 *)a4 == 0) return;
    if (*(s16 *)(a1 + 2) == 0) return;
    if (*(s16 *)(a4 + 2) == 0) return;
    func_002174C0(p);
}

INCLUDE_ASM(const s32, "core/main", func_00217640);

INCLUDE_ASM(const s32, "core/main", func_00217818);

INCLUDE_ASM(const s32, "core/main", func_00217AC0);

f32 func_00217BA8(f32 arg0)
{
    return __builtin_fabsf(arg0);
}

INCLUDE_ASM(const s32, "core/main", func_00217BC8);

INCLUDE_ASM(const s32, "core/main", func_00217CB0);

INCLUDE_ASM(const s32, "core/main", func_00217DA0);

INCLUDE_ASM(const s32, "core/main", func_00217E28);

INCLUDE_ASM(const s32, "core/main", func_00217F28);

INCLUDE_ASM(const s32, "core/main", func_00218050);

INCLUDE_ASM(const s32, "core/main", func_00218558);

INCLUDE_ASM(const s32, "core/main", func_00218670);

INCLUDE_ASM(const s32, "core/main", func_00218AA0);

INCLUDE_ASM(const s32, "core/main", func_00218D88);

INCLUDE_ASM(const s32, "core/main", func_00219618);

INCLUDE_ASM(const s32, "core/main", func_002199F8);

INCLUDE_ASM(const s32, "core/main", func_00219B30);

INCLUDE_ASM(const s32, "core/main", func_00219C88);

INCLUDE_ASM(const s32, "core/main", func_0021A5D8);

INCLUDE_ASM(const s32, "core/main", func_0021A6E0);

/* Byte-matching. */
s32 func_0021A978(u8 *arg0)
{

    u64 v = *(u64 *)(arg0 + 8);

    func_0021E340(v, v);
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_0021A9A0);

/* Behavior-faithful, not byte-matching.  Identical arithmetic on the same
   bits (returns 1 unless the argument is inf or NaN); only the register
   allocation differs.  Linked size 0x24 == retail. */
u32 func_0021ABA0(f32 arg0)
{
    u32 v = *(u32 *)&arg0;

    return ((v & 0x7FFFFFFF) + 0x80800000) >> 31;
}

/* Behavior-faithful, not byte-matching.  Identical arithmetic on the same
   bits (returns 1 only for NaN); only the register allocation differs.
   Linked size 0x24 == retail. */
u32 func_0021ABC8(f32 arg0)
{
    u32 v = *(u32 *)&arg0;

    return (0x7F800000 - (v & 0x7FFFFFFF)) >> 31;
}

INCLUDE_ASM(const s32, "core/main", func_0021ABF0);

INCLUDE_ASM(const s32, "core/main", func_0021AD50);

INCLUDE_ASM(const s32, "core/main", func_0021AD80);

INCLUDE_ASM(const s32, "core/main", func_0021ADD8);

void func_0021AE88(void)
{
    if (D_00410CC0[0] == 0) {
        D_00410CC0[0] = 1;
        func_0021ADD8();
    }
}

INCLUDE_ASM(const s32, "core/main", func_0021AEA8);

/* Stamper asm: calls the never-returning abort() wrapper func_00222048.  C
   tail-calls it and emits an epilogue, so the frame+jal shape is kept in
   src/asm/func_0021B598.s. */
ASM_FUNC(const s32, func_0021B598);

/* Byte-matching: function-pointer wrapper; the void (*)(void) typedef is
   required so the base lands in $2 and the loaded pointer in $3 (jalr $3). */
s32 func_0021B5A8(void)
{

    ((void (*)(void)) D_00386F3C[0])();
}

/* Byte-matching.  Returns arg2 unless the comparison found something. */
s32 func_0021B5D0(s32 arg0, s32 arg1, s32 arg2)
{

    s32 r = func_002268F0(arg0, arg1);

    if (r != 0)
        arg2 = 0;
    return arg2;
}

void func_0021B600(void) {
}

u8 *func_0021B608(void)
{


    u8 *v = (u8 *)func_00223E58(0x18);
    if (v == 0) func_0021B5A8();
    func_00224B88((s32)v, 0, 0x18);
    *(s32 *)(v + 4) = (s32)(v + 16);
    return v;
}

extern s32 D_00386F40[];

s32 func_0021B660(void)
{
    return ((s32 (*)(void)) D_00386F40[0])();
}

u8 *func_0021B688(void)
{
    return (u8 *)((s32 (*)(void)) D_00386F40[0])() + 8;
}

extern s32 func_0021B6D8();

extern s32 func_0021B6D8();

s32 func_0021B6B0(void)
{
    D_00386F40[0] = (s32)func_0021B6D8;
    return ((s32 (*)(void)) D_00386F40[0])();
}

extern s32 D_00410CD8[];
extern u8 D_00410CC8[];
extern u8 D_00410CE0[];

s32 func_0021B6D8(void)
{


    if (D_00410CD8[0] == 0) {
        D_00410CD8[0] = 1;
        func_00224B88((s32)&D_00410CC8, 0, 0x10);
        *(s32 *)(D_00410CC8 + 4) = (s32)&D_00410CE0;
    }
    return (s32)&D_00410CC8;
}

s32 func_0021B740(void)
{
    return ((s32 (*)(void)) D_00386F40[0])() + 4;
}

INCLUDE_ASM(const s32, "core/main", func_0021B768);

s16 func_0021BA40(const s16 *p) { return p[3]; }

s16 func_0021BA48(const s16 *p) { return p[2]; }

INCLUDE_ASM(const s32, "core/main", func_0021BA50);

INCLUDE_ASM(const s32, "core/main", func_0021BB28);

INCLUDE_ASM(const s32, "core/main", func_0021BC70);

INCLUDE_ASM(const s32, "core/main", func_0021BCF8);

INCLUDE_ASM(const s32, "core/main", func_0021BD78);

void func_0021BE08(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0021BE10);

INCLUDE_ASM(const s32, "core/main", func_0021C140);

INCLUDE_ASM(const s32, "core/main", func_0021C320);

INCLUDE_ASM(const s32, "core/main", func_0021C508);

INCLUDE_ASM(const s32, "core/main", func_0021C5F8);

INCLUDE_ASM(const s32, "core/main", func_0021C690);

INCLUDE_ASM(const s32, "core/main", func_0021C770);

INCLUDE_ASM(const s32, "core/main", func_0021CDD8);

INCLUDE_ASM(const s32, "core/main", func_0021CE38);

INCLUDE_ASM(const s32, "core/main", func_0021D408);

INCLUDE_ASM(const s32, "core/main", func_0021D948);

INCLUDE_ASM(const s32, "core/main", func_0021DA78);

INCLUDE_ASM(const s32, "core/main", func_0021DB18);

INCLUDE_ASM(const s32, "core/main", func_0021DD58);

u64 func_0021DDB0(u64 a1, u64 a2)
{



    Stk s;

    s.v8 = a1;
    s.v9 = a2;
    func_0021DA78(&s.v8, s.v4);
    func_0021DA78(&s.v9, &s.v5);
    s.v6 ^= 1;
    return func_0021D948(func_0021DB18(s.v4, &s.v5, s.v7));
}

INCLUDE_ASM(const s32, "core/main", func_0021DE18);

INCLUDE_ASM(const s32, "core/main", func_0021E0C0);

INCLUDE_ASM(const s32, "core/main", func_0021E228);

s32 func_0021E340(u64 a1, u64 a2)
{

    s32 v3[8], v4[8];
    u64 v5 = a1, v6 = a2;

    func_0021DA78(&v5, v3);
    func_0021DA78(&v6, v4);
    func_0021E228(v3, v4);
    return;
}

INCLUDE_ASM(const s32, "core/main", func_0021E390);

INCLUDE_ASM(const s32, "core/main", func_0021E448);

INCLUDE_ASM(const s32, "core/main", func_0021E4E0);

/* Byte-matching.  Builds a one-value block from the 64-bit argument, converts
   it through func_0021DA78, normalises the flag at +4 and hands it over.  The
   argument is 64-bit, which is why retail stores it with sd. */
void func_0021E580(u64 arg0)
{


    s32 out[8];
    u64 src;

    src = arg0;
    func_0021DA78(&src, out);
    out[1] = (out[1] == 0);
    func_0021D948(out);
}

/* Byte-matching.  Packs the four arguments into one block and hands it over;
   the 64-bit member sits at +0x10, matching retail's 8-byte store there. */
void func_0021E5B8(s32 arg0, s32 arg1, s32 arg2, u64 arg3)
{

    ArgBlock blk;

    blk.a = arg0;
    blk.b = arg1;
    blk.c = arg2;
    blk.d = arg3;
    func_0021D948(&blk);
}

INCLUDE_ASM(const s32, "core/main", func_0021E5E8);

INCLUDE_ASM(const s32, "core/main", func_0021E640);

s32 func_0021E678(s32 value) { s32 old = D_00387534[0]; D_00387534[0] = value; return old; }

INCLUDE_ASM(const s32, "core/main", func_0021E688);

s32 func_0021E6C8(void)
{
    return *(s32 *)(s32)func_0021B688();
}

/* Byte-matching.  Sets the count at +0x14 to 1, bumps the 64-bit counter at
   +0x20 and returns the object. */
u8 *func_0021E6E8(void)
{
    u8 *p = *(u8 **)func_0021B688();

    *(u32 *)(p + 0x14) = 1;
    *(u64 *)(p + 0x20) = *(u64 *)(p + 0x20) + 1;
    return p;
}

/* Behavior-faithful, not byte-matching: same test and same callee, but gcc
   turns the final call into a sibling call (j) where retail emits jal + jr
   before its epilogue.  Same linked size 0x28. */
void func_0021E720(void)
{



    if (func_00223E58() == 0)
        func_0021E640();
}

s32 func_00223E80();

s32 func_0021E748(void)
{
    return func_00223E80();
}

INCLUDE_ASM(const s32, "core/main", func_0021E768);

INCLUDE_ASM(const s32, "core/main", func_0021E7D8);

INCLUDE_ASM(const s32, "core/main", func_0021E868);

void func_0021E930(void)
{
    s32 v = *(s32 *)func_0021B688();
    if (v == 0) v = func_0021E640();
    *(s32 *)(v + 20) = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0021E960);

/* Byte-matching.  Calls the function pointer at +4 of the object arg0 points
   at, passing arg0, then reads the word the callee returned. */
s32 func_0021EC50(u8 *arg0)
{
    u8 *p = *(u8 **)arg0;
    Fn fn = *(Fn *)(p + 4);

    return *(s32 *)fn(arg0);
}

s32 func_0021EC78(s32 *arg0, s32 arg1)
{
    s32 *p = D_003910E8;

    *arg0 = (s32)p;
    if (arg1 & 1)
        return func_00220A58();
    return (s32)p;
}

u8 *func_0021ECA8(u8 *p)
{
    *(s32 *)p = (s32)D_003910D0;
    return p;
}

s32 *func_0021ED58(void);
s32 func_00220C90(s32, s32, s32);

s32 *func_0021ECC0(void)
{
    s32 *base = (s32 *)D_00410D18;

    if (*base == 0) {
        func_0021ED58();
        func_00220C90((s32)base, D_00391100, D_00393DD0);
    }
    return base;
}

s32 func_0021ED10(s32 *arg0, s32 arg1)
{
    s32 *p = D_003910E8;

    *arg0 = (s32)p;
    if (arg1 & 1)
        return func_00220A58();
    return (s32)p;
}

u8 *func_0021ED40(u8 *p)
{
    *(s32 *)p = (s32)D_003910E8;
    return p;
}

s32 *func_0021ED58(void)
{




    if (D_00393DD0[0] == 0)
        func_00220CB0(D_00393DD0, D_00391110);
    return D_00393DD0;
}

INCLUDE_ASM(const s32, "core/main", func_0021ED98);

INCLUDE_ASM(const s32, "core/main", func_0021EEA8);

INCLUDE_ASM(const s32, "core/main", func_0021EF38);

INCLUDE_ASM(const s32, "core/main", func_0021F170);

s32 func_0021F1C8(f32 a1, f32 a2)
{
    StkAlt s;
    s32 v2;

    s.v8 = a1;
    s.v9 = a2;
    func_0021EEA8((s32 *)&s.v8, s.v4);
    func_0021EEA8((s32 *)&s.v9, &s.v5);
    s.v6 ^= 1;
    v2 = func_0021EF38(s.v4, &s.v5, s.v7);
    return ((s32 (*)(u8 *))func_0021ED98)(v2);
}

INCLUDE_ASM(const s32, "core/main", func_0021F230);

INCLUDE_ASM(const s32, "core/main", func_0021F428);

INCLUDE_ASM(const s32, "core/main", func_0021F588);

extern void func_0021EEA8();
s32 func_0021F588();

s32 func_0021F6A0(f32 a1, f32 a2)
{
    s32 v3[4];
    s32 v4[4];
    f32 v[2];
    v[0] = a1;
    v[1] = a2;
    func_0021EEA8(&v[0], v3);
    func_0021EEA8(&v[1], v4);
    return func_0021F588(v3, v4);
}

INCLUDE_ASM(const s32, "core/main", func_0021F6F0);

INCLUDE_ASM(const s32, "core/main", func_0021F7A8);

INCLUDE_ASM(const s32, "core/main", func_0021F838);

/* Byte-matching. */
float func_0021F8D0(float arg0)
{
    float src = arg0;
    s32 out[4];

    func_0021EEA8(&src, out);
    out[1] = (out[1] == 0);
    return ((float (*)(void *))func_0021ED98)(out);
}

/* Byte-matching.  Same shape as func_0021E5B8 with four 32-bit words. */
void func_0021F908(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 args[4];

    args[0] = arg0;
    args[1] = arg1;
    args[2] = arg2;
    args[3] = arg3;
    func_0021ED98(args);
}

void func_0021F938(f32 a1)
{
    s32 v2[4];
    f32 v3[4];
    f32 *p;
    __asm__ volatile("" ::: "memory");
    p = v3;
    __asm__ volatile("" : : "r"(p) : "memory");
    *p = a1;
    func_0021EEA8(p, v2);
    func_0021E5B8(v2[0], v2[1], v2[2], (u64)v2[3] << 32 >> 2);
}

u8 *func_0021F978(u8 *p, u32 *out)
{
    u32 sh = 0;
    u32 c = *p++;
    u32 v = c & 0x7F;

    while (c & 0x80) {
        c = *p++;
        sh += 7;
        v |= (c & 0x7F) << sh;
    }
    *out = v;
    return p;
}

/* Byte-matching.  LEB128-style decode with sign extension on the last byte. */
u8 *func_0021F9C0(u8 *p, u32 *out)
{
    u32 shift = 0;
    u32 value = 0;
    u8 b;

    do
    {
        b = *p++;
        value |= (u32)(b & 0x7F) << shift;
        shift += 7;
    } while (b & 0x80);
    if (shift < 32)
    {
        if (b & 0x40)
            value |= (u32)(-1 << shift);
    }
    *out = value;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0021FA10);

INCLUDE_ASM(const s32, "core/main", func_0021FB10);

s32 func_0021FD78(u8 *a1)
{
    s32 v1 = *(s32 *)a1;
    s32 count = 0;
    s32 i;
    u8 *v4;

    if (*(s32 *)a1 != 0)
    {
        i = *(s32 *)(a1 + 4);
        for (;;)
        {
            if (i != 0 && *(s32 *)(a1 + 8) != 0)
                count++;
            v4 = a1 + v1;
            a1 = v4 + 4;
            if (*(s32 *)(v4 + 4) == 0)
                break;
            v1 = *(s32 *)(v4 + 4);
            __asm__ volatile("" ::: "memory");
            i = *(s32 *)(a1 + 4);
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "core/main", func_0021FDC0);

INCLUDE_ASM(const s32, "core/main", func_0021FE48);

INCLUDE_ASM(const s32, "core/main", func_0021FF88);


/* Source: misc/region_0022.c */
/* func_00220070 through func_0022E7B8; retail address range 0x00220000-0x00230000. */



extern s32 D_003878CC[8];
extern u8 D_0038857C[8];

INCLUDE_ASM(const s32, "core/main", func_00220070);

INCLUDE_ASM(const s32, "core/main", func_00220178);

extern s32 D_00410D10[];

void func_00220510(u8 *arg0, u8 *arg1)
{
    s32 *p = (s32 *)arg1;
    s32 v = D_00410D10[0];

    p[2] = (s32)arg0;
    p[5] = v;
    D_00410D10[0] = (s32)p;
    p[0] = 0;
    p[3] = 0;
    p[4] = 0;
    p[1] = 0;
}

/* Byte-matching.  Allocates 0x18 bytes and hands them, with arg0, to
   func_00220510 as a tail call. */
void func_00220538(u8 *arg0)
{



    func_00220510(arg0, func_00223E58(0x18));
}

void func_00220568(u8 *arg0, u8 *arg1)
{
    s32 *p = (s32 *)arg1;
    s32 v = D_00410D10[0];

    p[3] = (s32)arg0;
    p[5] = v;
    D_00410D10[0] = (s32)p;
    p[2] = (s32)arg0;
    p[0] = 0;
    p[4] = 0;
    p[1] = 0;
}

/* Byte-matching.  Same as func_00220538 but hands the block to
   func_00220568. */
void func_00220590(u8 *arg0)
{



    func_00220568(arg0, func_00223E58(0x18));
}

INCLUDE_ASM(const s32, "core/main", func_002205C0);

void func_00220638(void)
{
    func_00223E80(func_002205C0());
}

INCLUDE_ASM(const s32, "core/main", func_00220658);

INCLUDE_ASM(const s32, "core/main", func_00220830);

s32 func_002209A8(s32 value) { s32 old = D_003878CC[0]; D_003878CC[0] = value; return old; }

s32 func_002209B8(s32 unused, s32 value) { return value; }

s32 func_002209C0(s32 unused, s32 value) { return value; }

extern u8 D_003911F0[];

u8 *func_002209C8(void) { return D_003911F0; }

s32 *func_0021ED58(void);
s32 func_00220C90(s32, s32, s32);

s32 *func_002209D8(void)
{
    s32 *base = (s32 *)D_00410D28;

    if (*base == 0) {
        func_0021ED58();
        func_00220C90((s32)base, D_00391220, D_00393DD0);
    }
    return base;
}

s32 func_00220A28(s32 *arg0, s32 arg1)
{
    s32 *p = D_003910E8;

    *arg0 = (s32)p;
    if (arg1 & 1)
        return func_00220A58();
    return (s32)p;
}

INCLUDE_ASM(const s32, "core/main", func_00220A58);

INCLUDE_ASM(const s32, "core/main", func_00220AD0);

s32 func_00220BF8(a, flag)
s32 a, flag;
{
    s32 *p = D_003912D8;

    *(s32 *)(a + 4) = (s32)p;
    if (flag & 1)
        return func_00220A58();
    return (s32)p;
}

s32 func_00220C28(a1, a2, a3)
s32 a1;
s32 a2;
s32 a3;
{
    s32 v = 0;

    if (a2 == a1) goto set1;
    if (func_002268F0(*(s32 *)a1, *(s32 *)a2) != 0) goto out;
set1:
    v = 1;
out:
    return v;
}

void func_00220C68(u8 *arg0, s32 a, s32 b, s32 c)
{
    if (arg0 == 0) return;
    *(s32 *)(arg0 + 0) = a;
    *(s32 *)(arg0 + 4) = (s32)D_00391260;
    *(s32 *)(arg0 + 8) = b;
    *(s32 *)(arg0 + 0xC) = c;
}

s32 func_00220C90(s32 p, s32 a, s32 b)
{
    if (p != 0) {
        *(s32 *)(p + 8) = b;
        *(s32 *)(p + 0) = a;
        *(s32 *)(p + 4) = (s32)D_00391278;
    }
}

void func_00220CB0(s32 *arg1, s32 arg2)
{
    if (arg1 != 0) {
        arg1[0] = arg2;
        arg1[1] = (s32)D_00391290;
    }
}

/* Byte-matching. */
s32 func_00220CD0(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{

    s32 r = func_00220C28(arg0, arg1, arg2);

    if (r == 0)
        arg3 = 0;
    return arg3;
}

s32 func_00220D00(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{

    s32 result;

    if (func_00220C28(a1, a2) != 0) {
        result = a4;
    } else {
        s32 *p = *(s32 **)(a1 + 8);
        s32 *q = *(s32 **)((u8 *)p + 4);
        s16 off = *(s16 *)((u8 *)q + 0x10);
        s32 (*handler)(s32, s32, s32, s32, s32, s32) =
            *(s32 (**)(s32, s32, s32, s32, s32, s32))((u8 *)q + 0x14);
        result = handler((s32)((u8 *)p + off), a2, a3, a4, a5, a6);
    }
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00220D98);

s32 *func_00220F70(s32 *p, s32 a, s32 b, s32 c)
{
    p[0] = a;
    p[1] = (s32)D_00391260;
    p[2] = b;
    p[3] = c;
    return p;
}

s32 func_00220C90(s32, s32, s32);

s32 *func_00220F90(void)
{
    s32 *base = (s32 *)D_00410D78;

    if (*base == 0) {
        func_002210A8();
        func_00220C90((s32)base, D_003912E8, D_00410D58);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00220FE0(void)
{
    return func_00220BF8();
}

s32 *func_00221000(s32 *p, s32 a, s32 b)
{
    p[0] = a;
    p[1] = (s32)D_00391278;
    p[2] = b;
    return p;
}

s32 func_00220C90(s32, s32, s32);

s32 *func_00221020(void)
{
    s32 *base = (s32 *)D_00410D68;

    if (*base == 0) {
        func_002210A8();
        func_00220C90((s32)base, D_00391300, D_00410D58);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00221070(void)
{
    return func_00220BF8();
}

u8 *func_00221090(u8 *p, s32 value)
{
    *(s32 *)p = value;
    *(s32 *)(p + 0x4) = (s32)D_00391290;
    return p;
}

s32 *func_00221288();
s32 func_00220C90(s32, s32, s32);

s32 *func_002210A8(void)
{
    s32 *base = (s32 *)D_00410D58;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_00391318, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_002210F8(void)
{
    return func_00220BF8();
}

s32 func_00221118(s32 *arg0, s32 arg1)
{
    s32 *p = D_003910E8;

    *arg0 = (s32)p;
    if (arg1 & 1)
        return func_00220A58();
    return (s32)p;
}

u8 *func_00221148(u8 *p)
{
    *(s32 *)p = (s32)D_003912A8;
    return p;
}

s32 *func_0021ED58(void);
s32 func_00220C90(s32, s32, s32);

s32 *func_00221160(void)
{
    s32 *base = (s32 *)D_00410D38;

    if (*base == 0) {
        func_0021ED58();
        func_00220C90((s32)base, D_00391330, D_00393DD0);
    }
    return base;
}

s32 func_002211B0(s32 *arg0, s32 arg1)
{
    s32 *p = D_003910E8;

    *arg0 = (s32)p;
    if (arg1 & 1)
        return func_00220A58();
    return (s32)p;
}

u8 *func_002211E0(u8 *p)
{
    *(s32 *)p = (s32)D_003912C0;
    return p;
}

s32 *func_0021ED58(void);
s32 func_00220C90(s32, s32, s32);

s32 *func_002211F8(void)
{
    s32 *base = (s32 *)D_00410D48;

    if (*base == 0) {
        func_0021ED58();
        func_00220C90((s32)base, D_00391340, D_00393DD0);
    }
    return base;
}

s32 func_00221248(void)
{
    return func_00220C28() ^ 1;
}

s32 func_00221268(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

u8 *func_00221270(u8 *p, s32 value)
{
    *(s32 *)p = value;
    *(s32 *)(p + 0x4) = (s32)D_003912D8;
    return p;
}

s32 *func_00221288(void)
{



    if (D_00393DD8[0] == 0)
        func_00220CB0(D_00393DD8, D_00391350);
    return D_00393DD8;
}

/* Byte-matching.  Boolean test on the sign bit of the comparator result. */
u32 func_002212C8(s32 *arg0, s32 *arg1)
{


    return (u32)func_002268F0(*arg0, *arg1) >> 31;
}

INCLUDE_ASM(const s32, "core/main", func_002212F0);

s32 func_002217B8(s32 a1)
{
  s32 v1;

  v1 = 0;
  if ( a1 != 0 )
    v1 = func_002218D0(
           (s32 (*)(void))*(u32 *)(*(u32 *)(a1 + 4) + 4),
           func_00221FD8,
           1,
           (s32)a1 + **(s16 **)(a1 + 4),
           func_00221288,
           a1);
  return v1 != 0;
}

void func_00221808(s32 *p, s32 a, s32 b)
{
    if (p != 0) {
        p[2] = b;
        p[0] = a;
        p[1] = (s32)D_00391458;
    }
}

void func_00221828(u8 *arg0, s32 a, s32 b, s32 c)
{
    if (arg0 == 0) return;
    *(s32 *)(arg0 + 0) = a;
    *(s32 *)(arg0 + 4) = (s32)D_00391448;
    *(s32 *)(arg0 + 8) = c;
    *(s32 *)(arg0 + 0xC) = b;
}

void func_00221850(s32 *arg1, s32 arg2)
{
    if (arg1 != 0) {
        arg1[0] = arg2;
        arg1[1] = (s32)D_00391428;
    }
}

void func_00221870(s32 *arg1, s32 arg2)
{
    if (arg1 != 0) {
        arg1[0] = arg2;
        arg1[1] = (s32)D_00391418;
    }
}

void func_00221890(s32 *arg1, s32 arg2)
{
    if (arg1 != 0) {
        arg1[0] = arg2;
        arg1[1] = (s32)D_00391408;
    }
}

void func_002218B0(s32 *arg1, s32 arg2)
{
    if (arg1 != 0) {
        arg1[0] = arg2;
        arg1[1] = (s32)D_003913F8;
    }
}

INCLUDE_ASM(const s32, "core/main", func_002218D0);

INCLUDE_ASM(const s32, "core/main", func_00221980);

INCLUDE_ASM(const s32, "core/main", func_002219B8);

INCLUDE_ASM(const s32, "core/main", func_002219F0);

INCLUDE_ASM(const s32, "core/main", func_00221A28);

INCLUDE_ASM(const s32, "core/main", func_00221A60);

INCLUDE_ASM(const s32, "core/main", func_00221A98);

INCLUDE_ASM(const s32, "core/main", func_00221AD0);

INCLUDE_ASM(const s32, "core/main", func_00221B08);

INCLUDE_ASM(const s32, "core/main", func_00221B40);

INCLUDE_ASM(const s32, "core/main", func_00221B78);

INCLUDE_ASM(const s32, "core/main", func_00221BB0);

INCLUDE_ASM(const s32, "core/main", func_00221BE8);

INCLUDE_ASM(const s32, "core/main", func_00221C20);

INCLUDE_ASM(const s32, "core/main", func_00221C58);

INCLUDE_ASM(const s32, "core/main", func_00221C90);

INCLUDE_ASM(const s32, "core/main", func_00221CC8);

INCLUDE_ASM(const s32, "core/main", func_00221D00);

s32 *func_00221288();
s32 func_00220C90(s32, s32, s32);

s32 *func_00221D38(void)
{
    s32 *base = (s32 *)D_00410DB8;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_00391468, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00221D88(void)
{
    return func_00220BF8();
}


s32 *func_00221DA8(void)
{
    s32 *base = (s32 *)D_410DD8;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_00391480, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00221DF8(void)
{
    return func_00220BF8();
}


s32 *func_00221E18(void)
{
    s32 *base = (s32 *)D_00410DA8;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_00391498, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00221E68(void)
{
    return func_00220BF8();
}


s32 *func_00221E88(void)
{
    s32 *base = (s32 *)D_00410D88;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_003914B0, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00221ED8(void)
{
    return func_00220BF8();
}


s32 *func_00221EF8(void)
{
    s32 *base = (s32 *)D_410DE8;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_003914C8, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00221F48(void)
{
    return func_00220BF8();
}


s32 *func_00221F68(void)
{
    s32 *base = (s32 *)D_00410D98;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_003914E0, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00221FB8(void)
{
    return func_00220BF8();
}


s32 *func_00221FD8(void)
{
    s32 *base = (s32 *)D_410DC8;

    if (*base == 0) {
        func_00221288();
        func_00220C90((s32)base, D_003914F8, D_00393DD8);
    }
    return base;
}

s32 func_00220BF8();

s32 func_00222028(void)
{
    return func_00220BF8();
}

void func_00222048(void)
{
    for (;;) {
        func_00226180(6);
        func_001000B0(1);
    }
}

extern s32 D_00391510[];
void func_00222068(s32 a1, s32 a2, s32 a3)
{
    func_002226C0(*(s32 *)(*(s32 *)D_0038857C + 0xC), D_00391510, a3, a1, a2);
    func_00222048();
}

/* Byte-matching.  Forwards arg0 to func_002275C0 with a zero and a 0xA, and
   narrows its 64-bit return to 32 bits (the dsll32/dsra32 pair). */
s32 func_002220A0(s32 arg0)
{

    return (s32)((s64 (*)(s32, s32, s32))func_002275C0)(arg0, 0, 0xA);
}

s32 func_002220C8(s32 a)
{
    return func_002275C0(a, 0, 0xA);
}

s32 func_002220E8(void)
{
    return func_0022AA90(func_00223810());
}

s32 func_00222110(void) { return *(s32 *)D_0038857C; }

INCLUDE_ASM(const s32, "core/main", func_00222120);

s32 func_00222228(const u16 *p) { return (p[6] >> 5) & 1; }

INCLUDE_ASM(const s32, "core/main", func_00222238);

INCLUDE_ASM(const s32, "core/main", func_00222340);

/* Byte-matching.  Fills in an object header with its callback table. */
void func_00226410(void);
s64 func_00226478(s32 a1, s32 *a2, s32 a3);
void func_002264F8(void);
s32 func_00226560(u8 *p);

typedef struct {
    u32 f0;
    u32 f4;
    u32 f8;
    s16 fC;
    s16 fE;
    u32 f10;
    u32 pad14;
    u32 f18;
    u32 f1C;
    u32 f20;
    u32 f24;
    u32 f28;
    u32 f2C;
    u8 pad30[0x24];
    u32 f54;
} S222468;

void func_00222468(S222468 *s, s16 a, s16 b, u32 c)
{
    s->f54 = c;
    s->fC = a;
    s->fE = b;
    s->f20 = (u32)func_00226410;
    s->f24 = (u32)func_00226478;
    s->f28 = (u32)func_002264F8;
    s->f2C = (u32)func_00226560;
    s->f0 = 0;
    s->f4 = 0;
    s->f8 = 0;
    s->f10 = 0;
    s->f18 = 0;
    __asm__ volatile("" ::: "memory");
    s->f1C = (u32)s;
}

u8 *func_002224C0(u8 *a1, s32 a2)
{

    u8 *v = (u8 *)func_002240E8(a1, 88 * a2 + 12);
    if (v == 0) return 0;
    *(s32 *)(v + 4) = a2;
    *(s32 *)v = 0;
    *(s32 *)(v + 8) = (s32)(v + 12);
    func_00224B88((s32)(v + 12), 0, 88 * a2);
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_00222530);

extern s32 func_00222238();
extern void func_002236D0(s32, s32);

void func_00222610(s32 arg)
{
    func_002236D0(arg, (s32)func_00222238);
}

void func_00222620(void)
{
    func_00222610(*(s32 *)D_0038857C);
}

s32 func_00222630(u32 *a1)
{


    u32 *v2;
v2 = a1 + 121;
a1[15] = func_00222610;
a1[14] = 1;
func_00222468((u32)(a1 + 121), 4, 0, (s32)a1);
func_00222468((u32)(a1 + 143), 9, 1, (s32)a1);
func_00222468((u32)(a1 + 165), 10, 2, (s32)a1);
a1[120] = v2;
a1[119] = 3;
a1[118] = 0;
return 3;
}

s32 func_002226C0(void *a1, void *a2, ...)
{

    char *va = (char *)__builtin_next_arg(a2) - 0x30;

    return func_00227750((s32)a1, (s32)a2, (s32)va);
}

INCLUDE_ASM(const s32, "core/main", func_002226F8);

s32 func_002227D8(s32 arg0, s32 arg1)
{
    return func_002226F8(*(s32 *)D_0038857C, arg0, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_00222808);

INCLUDE_ASM(const s32, "core/main", func_00222858);

INCLUDE_ASM(const s32, "core/main", func_00222978);

INCLUDE_ASM(const s32, "core/main", func_00222C98);

INCLUDE_ASM(const s32, "core/main", func_00222E08);

s32 func_00223298(s32 *a1, s32 a2, s32 a3)
{

    s32 v;
    D_00393DE0[0] = 0;
    v = func_001C1318(a2, a3);
    if (v == -1 && D_00393DE0[0] != 0) *a1 = D_00393DE0[0];
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_002232F8);

INCLUDE_ASM(const s32, "core/main", func_002236D0);

INCLUDE_ASM(const s32, "core/main", func_00223768);

s32 func_00226760(void);

s32 func_002237F0(void)
{
    return func_00226760();
}

s32 func_00223810(s32 arg0)
{
    return func_00223838(arg0, *(u32 *)D_0038857C + 0x7C);
}

INCLUDE_ASM(const s32, "core/main", func_00223838);

INCLUDE_ASM(const s32, "core/main", func_00223B08);

s32 func_00223C58(s32 arg0, s32 arg1)
{
    return func_00223C88(*(s32 *)D_0038857C, arg0, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_00223C88);

s32 func_00223E58(s32 arg0)
{
    return func_002240E8(*(s32 *)D_0038857C, arg0);
}

s32 func_00223E80(s32 arg)
{
    func_00222978(*(s32 *)D_0038857C, arg);
}

INCLUDE_ASM(const s32, "core/main", func_00223E90);

INCLUDE_ASM(const s32, "core/main", func_002240E8);

INCLUDE_ASM(const s32, "core/main", func_00224860);

INCLUDE_ASM(const s32, "core/main", func_00224940);

INCLUDE_ASM(const s32, "core/main", func_002249D4);

INCLUDE_ASM(const s32, "core/main", func_00224A84);

INCLUDE_ASM(const s32, "core/main", func_00224B88);

void func_00224C48(void) {
}

void func_00224C50(void) {
}

extern s32 D_00393DE0[];
s32 func_00224C58(s32 *a1, s32 a2, s32 a3, s32 a4)
{
    s32 v;
    D_00393DE0[0] = 0;
    v = func_001C1228(a2, a3, a4);
    if (v == -1 && D_00393DE0[0] != 0) *a1 = D_00393DE0[0];
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_00224CB8);

void func_002256E0(s32 value) { ((s32 *)*(u8 **)D_0038857C)[0x58 / 4] = value; }

s32 func_002256F0(void)
{
  s32 v0;
  v0 = 1103515245 * ((s32 *) *(u8 **)D_0038857C)[0x58 / 4] + 12345;
  ((s32 *) *(u8 **)D_0038857C)[0x58 / 4] = v0;
  return v0 & 0x7FFFFFFF;
}

s32 func_00225720(s32 arg0, s32 arg1)
{
    return func_00225750(*(s32 *)D_0038857C, arg0, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_00225750);

s32 func_00222238(void);

s32 func_00225CC0(void)
{
    return func_00222238();
}

INCLUDE_ASM(const s32, "core/main", func_00225CE0);

s32 func_00227200();

s32 func_00225E58(void)
{
    return func_00227200();
}

s32 func_00225E78(s32 *a1, s32 a2)
{

    s32 *g = D_00393DE0;
    s32 v;
    g[0] = 0;
    v = func_001C1268(a2);
    if ((u32)v == 0xFFFFFFFFu && g[0] != 0)
        *a1 = g[0];
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_00225ED8);

INCLUDE_ASM(const s32, "core/main", func_00225F48);

INCLUDE_ASM(const s32, "core/main", func_00225FD8);

INCLUDE_ASM(const s32, "core/main", func_002260C8);

s32 func_00226180(s32 arg0)
{
    return func_00225FD8(*(s32 *)D_0038857C, arg0);
}

s32 func_002261A8(s32 arg0, s32 arg1)
{
    return func_00225F48(*(s32 *)D_0038857C, arg0, arg1);
}

s32 func_002261D8(void)
{
    return func_00225ED8(*(s32 *)D_0038857C);
}

s32 func_00226200(s32 arg0)
{
    return func_002260C8(*(s32 *)D_0038857C, arg0);
}

extern s32 D_00393DE0[];
s32 func_00226228(s32 *a1, s32 a2, s32 a3)
{
    s32 v;
    D_00393DE0[0] = 0;
    v = func_001C1338(a2, a3);
    if (v == -1 && D_00393DE0[0] != 0) *a1 = D_00393DE0[0];
    return v;
}

s32 func_001C1330(void);

s32 func_00226288(void)
{
    return func_001C1330();
}

s32 func_002262A8(s32 a1, s32 a2, u8 *a3, ...)
{

    Blk b;
    char *va;
    s32 r;

    b.v8 = a1;
    b.v5 = 520;
    b.v4[0] = a2;
    b.v4[2] = 0x7FFFFFFF;
    b.v6 = a2;
    b.v7 = 0x7FFFFFFF;
    va = (char *)__builtin_next_arg(a3) - 0x28;
    r = func_00228498((s32)b.v4, (s32)a3, (s32)va);
    *(u8 *)b.v4[0] = 0;
    return r;
}

INCLUDE_ASM(const s32, "core/main", func_00226310);

s32 func_00226380(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00226388);

INCLUDE_ASM(const s32, "core/main", func_00226410);

/* Byte-matching: u64 cast chain for post-epilogue sign extension. */
s64 func_00226478(s32 a1, s32 *a2, s32 a3)
{
    s16 v6;
    s32 *v7;
    u16 flags;
    u64 v;
    flags = *(u16 *)((u8 *)a1 + 0xC);
    if ((flags & 0x100) != 0)
        func_0022C130(*(s32 **)((u8 *)a1 + 0x54), *(s16 *)((u8 *)a1 + 0xE), 0, 2);
    v6 = *(s16 *)((u8 *)a1 + 0xE);
    *(u16 *)((u8 *)a1 + 0xC) &= ~0x1000;
    v7 = *(s32 **)((u8 *)a1 + 0x54);
    v = (u64)func_0022A920(v7, v6, (s32)a2, a3);
    
    return (s64)(s32)v;
}


INCLUDE_ASM(const s32, "core/main", func_002264F8);

s32 func_00226560(u8 *p)
{
    return func_0022AB58(*(s32 *)(p + 0x54), *(s16 *)(p + 0xE));
}

INCLUDE_ASM(const s32, "core/main", func_00226580);

INCLUDE_ASM(const s32, "core/main", func_00226634);

INCLUDE_ASM(const s32, "core/main", func_00226760);

INCLUDE_ASM(const s32, "core/main", func_002268F0);

INCLUDE_ASM(const s32, "core/main", func_00226A34);

INCLUDE_ASM(const s32, "core/main", func_00226B48);

INCLUDE_ASM(const s32, "core/main", func_00226C80);

INCLUDE_ASM(const s32, "core/main", func_00226CD8);

INCLUDE_ASM(const s32, "core/main", func_00226E88);

INCLUDE_ASM(const s32, "core/main", func_00227040);

s32 func_00227200(s8 *s, int c)
{
    s8 ch = (s8)c;
    s8 *p = s;
    s8 *found;

    found = 0;
    while (*p != 0) {
        if (*p == ch)
            found = p;
        p++;
    }
    if (*p == ch)
        return (s32)p;
    return (s32)found;
}

INCLUDE_ASM(const s32, "core/main", func_00227250);

s32 func_002272C0(s32 arg0, s32 arg1)
{
    return func_002272E8(arg0, arg1, *(u32 *)D_0038857C + 0x5C);
}

INCLUDE_ASM(const s32, "core/main", func_002272E8);

INCLUDE_ASM(const s32, "core/main", func_00227388);

s32 func_002275C0(s32 arg0, s32 arg1, s32 arg2)
{
    return func_00227388(*(s32 *)D_0038857C, arg0, arg1, arg2);
}

INCLUDE_ASM(const s32, "core/main", func_002275F8);

s32 func_00227650(s32 a1, s32 *a2)
{
    s32 result;

    if (a2[2] != 0) goto call;
    result = 0;
    a2[1] = 0;
    return result;
call:
    result = func_002232F8(a1, a2);
    a2[2] = 0;
    a2[1] = 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_00227698);

INCLUDE_ASM(const s32, "core/main", func_00227750);

INCLUDE_ASM(const s32, "core/main", func_002277C8);

s32 func_00228398(s32 a1, s32 *a2)
{
    s32 result;

    if (a2[2] != 0) goto call;
    result = 0;
    a2[1] = 0;
    return result;
call:
    result = func_002232F8(a1, a2);
    a2[2] = 0;
    a2[1] = 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_002283E0);

INCLUDE_ASM(const s32, "core/main", func_00228498);

/* ==========================================================================
   func_00228510 - formatter core of the printf family

   Retail 0x228510, 0x1518 bytes.  Frame 0x2D0.  The routine never writes to
   the stream itself: it queues (text, length) pieces into an eight-slot list
   on its own frame and hands that list to func_00228398 whenever it fills up
   (and once per conversion).  Fields and buffers confirmed from the retail
   listing:

     sp+0x000  char[16]   exponent suffix buffer, filled by func_00229BD8
     sp+0x010  FmtPieceList  { first = sp+0x20, count, pending total }
     sp+0x020  FmtPiece[8]   the queue
     sp+0x060  char[0x15C]   digits, written backwards from sp+0x1BC
     sp+0x1C0  char[2]       two-character prefix ("0x", "0o", "d.")
     sp+0x1D0  s8            float sign written by func_00229A28
     sp+0x1D1  s8            sign character for the current conversion
     sp+0x1D4  u32           wide character produced by the scan
     sp+0x1DC  s32           decimal exponent
     sp+0x1E0  s32           number of significant digits
     sp+0x1E4  void *        locale (first argument, only used by 0x229A28)
     sp+0x1E8  stream *      the stream context
     sp+0x1EC  char *        format cursor
     sp+0x1F0  s32           characters produced so far
     sp+0x1F4  s32           field width
     sp+0x1F8  char *        decimal point string of the current locale
     sp+0x200  u64           raw bits of the floating point argument
     sp+0x208  s32           length of the exponent suffix
     sp+0x20C  s32           precision actually used (0 until a numeric
                             conversion copies the parsed one)
     sp+0x210  s32           printed body length including sign and prefix
     sp+0x214  char *        hex digit table of the current conversion

   Stream context fields read here (see FmtStream): +0x0C u16 flags,
   +0x0E s16 limit, +0x10 pointer.  Bit 0x40 of +0x0C is the error flag: the
   routine propagates it to the return value (see the epilogue).
   ========================================================================== */

typedef struct
{
    const char *text;
    s32 length;
} FmtPiece;

typedef struct
{
    FmtPiece *first;
    s32 count;
    s32 total;
} FmtPieceList;

/* One stretch of the current conversion, in output order.  `chunk` limits how
   many bytes go into one vector (0 = the whole stretch is one piece); pad runs
   use 16, which is the size of the D_003918B0/D_003918C0 tables. */
typedef struct
{
    const char *src;
    s32 length;
    s32 chunk;
} FmtSeg;

typedef struct
{
    u8 gap00[0x0C];
    u16 flags;   /* +0x0C */
    s16 limit;   /* +0x0E */
    u8 *link10;  /* +0x10 */
} FmtStream;

#define FMT_SLOTS 8
#define FMT_SEGS 10
extern u8 D_003889D8[8];
extern u8 D_0038857C[8];  /* _impure_ptr / _REENT holder        */
extern u8 D_003918B0[];  /* sixteen spaces                 */
extern u8 D_003918C0[];  /* sixteen zeroes                 */
extern u8 D_003918D0[];  /* "Inf"                          */
extern u8 D_003918D8[];  /* "NaN"                          */
extern u8 D_003918E0[];  /* "0123456789abcdef"             */
extern u8 D_003918F8[];  /* "(null)"                       */
extern u8 D_00391900[];  /* "0123456789ABCDEF"             */
extern u8 D_00391918[];  /* "bug in vfprintf: bad base"     */
extern u8 D_00391938[];  /* "0"                            */
extern u8 D_00391940[];  /* "."                            */

s32 func_0022A980(FmtStream *stream);
s32 func_00228398();   /* definition takes raw s32 pointers; see func_00228398 body */
s32 func_002283E0(FmtStream *stream, const char *fmt, u64 *args);
u8 *func_0022C108(void);
s32 func_0022C190(void *locale, u32 *out, const char *text, void *left,
                  u32 *state);
s8 *func_00229A28(void *locale, u64 value, s32 prec, u32 flags, s8 *signOut,
                  s32 *expOut, s32 conv, s32 *lenOut);
s32 func_00229BD8(char *buffer, s32 exponent, s32 conv);
u32 func_0022D250(u64 value);
s32 func_00226B48(const char *text);
const char *func_00224860(const char *text, s32 ch, s32 count);
u64 func_0021D408(u64 value, u64 base);
extern u64 func_0021CE38();


s32 func_00228510(void *locale, FmtStream *stream, const char *fmt, u64 *args)
{

    FmtPiece pieces[FMT_SLOTS];
    FmtPieceList list;
    FmtSeg segment[FMT_SEGS];
    char digits[0x15C];
    char exponent[16];
    char prefix[2];
    FmtPiece *piece;
    const char *point;
    const char *pace;
    const char *start;
    const char *text;
    const char *table;
    char *wall;
    u64 value;
    u32 wide;
    u32 mbstate;
    u32 flags;
    u32 mode;
    s32 kind;
    s32 n;
    s32 segCount;
    s32 segIndex;
    s32 span;
    s32 litLen;
    s32 written;
    s32 width;
    s32 prec;
    s32 precUsed;
    s32 bodyLen;
    s32 padFlags;
    s32 fExp;
    s32 fLen;
    s32 expLen;
    s32 c;
    s8 sign;
    s8 fSign;

/* Queue one piece; drain the list (and restart at slot 0) when it is full. */
#define EMIT(p, l)                                                            \
    do {                                                                      \
        piece->text = (const char *)(p);                                       \
        piece->length = (l);                                                   \
        piece++;                                                               \
        list.total += (l);                                                     \
        if (++list.count >= FMT_SLOTS) {                                        \
            if (func_00228398(stream, &list) != 0)                             \
                goto done;                                                     \
            piece = pieces;                                                    \
        }                                                                      \
    } while (0)

/* Append one output stretch; `ch` is the per-vector step (0 = one piece). */
#define SEG(p, l, ch)                                                         \
    do {                                                                      \
        segment[segCount].src = (const char *)(p);                            \
        segment[segCount].length = (l);                                       \
        segment[segCount].chunk = (ch);                                       \
        segCount++;                                                           \
    } while (0)

    piece = pieces;
    list.first = pieces;
    list.count = 0;
    list.total = 0;
    segCount = 0;
    written = 0;
    flags = 0;
    mbstate = 0;
    sign = 0;
    pace = fmt;
    point = *(const char **)func_0022C108();

    /* 0x22855C: the stream only skips func_0022A980 when it is already open;
       the flags are re-read after that call on both paths. */
    if ((stream->flags & 0x08) == 0 || stream->link10 == 0) {
        if (func_0022A980(stream) != 0)
            return -1;
        mode = stream->flags & 0x1A;
    } else {
        mode = stream->flags & 0x1A;
    }

    if (mode == 0x0A && stream->limit >= 0)
        return func_002283E0(stream, fmt, args);

next:
    segCount = 0;
    /* Literal run: advance to the next '%' (or the end of the format). */
    start = pace;
    for (;;) {
        n = func_0022C190(*(void **)D_0038857C, &wide, pace,
                          *(void **)D_003889D8, &mbstate);
        if (n <= 0)
            break;
        pace += n;
        if (wide == '%') {
            pace -= 1;
            break;
        }
    }
    litLen = (s32)(pace - start);
    if (litLen != 0) {
        EMIT(start, litLen);
        written += litLen;
    }
    if (n <= 0)
        goto end_of_format;

    flags = 0;
    prec = -1;
    width = 0;
    precUsed = 0;
    sign = 0;
    pace++;

    for (;;) {
        c = (s32)(s8)*pace++;

    dispatch:
        /* Retail dispatches this through jtbl_00391950; the chain below
           selects the same cases.  A switch here would make gcc build a
           second table of its own, and those bytes are not part of the
           function's retail slot. */
        if (c == ' ') {
            if (sign == 0)
                sign = ' ';
            continue;
        }
        if (c == '#') {
            flags |= 1;
            continue;
        }
        if (c == '*') {
            width = (s32)(s64)*(s32 *)args;
            args++;
            if (width < 0) {
                width = -width;
                flags |= 4;
            }
            continue;
        }
        if (c == '+') {
            sign = '+';
            continue;
        }
        if (c == '-') {
            flags |= 4;
            continue;
        }
        if (c == '.') {
            c = (s32)(s8)*pace++;
            if (c == '*') {
                prec = (s32)(s64)*(s32 *)args;
                args++;
                if (prec <= -2)
                    prec = -1;
                continue;
            }
            {
                s32 parsed = 0;

                if ((u32)(c - '0') < 10) {
                    do {
                        parsed = parsed * 10 + (c - '0');
                        c = (s32)(s8)*pace++;
                    } while ((u32)(c - '0') < 10);
                }
                prec = (parsed > -2) ? parsed : -1;
            }
            goto dispatch;
        }
        if (c == '0') {
            flags |= 0x80;
            continue;
        }
        if (c >= '1' && c <= '9') {
            s32 parsed = 0;

            do {
                parsed = parsed * 10 + (c - '0');
                c = (s32)(s8)*pace++;
            } while ((u32)(c - '0') < 10);
            width = parsed;
            goto dispatch;
        }
        if (c == 'L') {
            flags |= 8;
            continue;
        }
        if (c == 'h') {
            flags |= 0x40;
            continue;
        }
        if (c == 'l') {
            if (*pace == 'l') {
                pace++;
                flags |= 0x20;
            } else {
                flags |= 0x10;
            }
            continue;
        }
        if (c == 'q') {
            flags |= 0x20;
            continue;
        }
        if (c == 'n') {
            if ((flags & 0x10) != 0) {
                *(u64 *)(u32)*args = (u64)written;
            } else if ((flags & 0x40) != 0) {
                *(u16 *)(u32)*args = (u16)written;
            } else {
                *(u32 *)(u32)*args = (u32)written;
            }
            args++;
            goto next;
        }
        if (c == 'c') {
            digits[0] = (char)*(u8 *)args;
            args++;
            text = digits;
            span = 1;
            sign = 0;
            goto have_digits;
        }
        if (c == 's') {
            /* 0x228B80 loads the slot with `lw`: the pointer is the low 32
               bits, sign-extended, and the top half of the va slot is not
               part of it.  Reading 8 bytes here turns a slot whose upper half
               holds stale data into an unrelated address. */
            text = (const char *)(u32)*(s32 *)args;
            args++;
            if (text == 0)
                text = (const char *)D_003918F8;
            if (prec < 0) {
                span = func_00226B48(text);
            } else {
                const char *stop;

                stop = func_00224860(text, 0, prec);
                if (stop != 0) {
                    span = (s32)(stop - text);
                    if (prec < span)
                        span = prec;
                } else {
                    span = prec;
                }
            }
            sign = 0;
            goto have_digits;
        }
        if (c == 'd' || c == 'i' || c == 'D') {
            if (c == 'D')
                flags |= 0x10;
            if ((flags & 0x10) != 0)
                value = *args;
            else if ((flags & 0x40) != 0)
                value = (u64)(s64)*(s16 *)args;
            else
                value = (u64)(s64)*(s32 *)args;
            args++;
            kind = 1;
            if ((s64)value < 0) {
                value = -value;
                sign = '-';
            }
            goto numeric;
        }
        if (c == 'o' || c == 'O') {
            if (c == 'O')
                flags |= 0x10;
            if ((flags & 0x10) != 0)
                value = *args;
            else if ((flags & 0x40) != 0)
                value = (u64)*(u16 *)args;
            else
                value = (u64)*(u32 *)args;
            args++;
            sign = 0;
            kind = 0;
            goto numeric;
        }
        if (c == 'u' || c == 'U') {
            if (c == 'U')
                flags |= 0x10;
            if ((flags & 0x10) != 0)
                value = *args;
            else if ((flags & 0x40) != 0)
                value = (u64)*(u16 *)args;
            else
                value = (u64)*(u32 *)args;
            args++;
            sign = 0;
            kind = 1;
            goto numeric;
        }
        if (c == 'x' || c == 'X') {
            table = (const char *)(c == 'X' ? D_00391900 : D_003918E0);
            if ((flags & 0x10) != 0)
                value = *args;
            else if ((flags & 0x40) != 0)
                value = (u64)*(u16 *)args;
            else
                value = (u64)*(u32 *)args;
            args++;
            sign = 0;
            kind = 2;
            if ((flags & 1) != 0 && value != 0)
                flags |= 2;
            goto numeric;
        }
        if (c == 'p') {
            table = (const char *)D_003918E0;
            value = (u64)(s64)*(s32 *)args;
            args++;
            flags |= 2;
            c = 'x';
            sign = 0;
            kind = 2;
            goto numeric;
        }
        if (c == 'e' || c == 'E' || c == 'f' || c == 'g' || c == 'G') {
            if (prec == -1) {
                prec = 6;
            } else if ((c == 'g' || c == 'G') && prec == 0) {
                prec = 1;
            }
            value = *args;
            args++;
            if (func_0022D250(value) != 0) {
                /* infinity */
                if (func_0021E340(value, 0) < 0)
                    sign = '-';
                span = 3;
                text = (const char *)D_003918D0;
                goto have_digits;
            }
            if (func_0022D298(value) != 0) {
                /* not a number */
                span = 3;
                text = (const char *)D_003918D8;
                goto have_digits;
            }
            flags |= 0x100;
            text = func_00229A28(locale, value, prec, flags, &fSign, &fExp, c,
                                 &fLen);
            if (c == 'g' || c == 'G') {
                s32 upper = (c != 'g');

                if (fExp < -3 || prec < fExp)
                    c = upper ? 'E' : 'e';
                else
                    c = 'g';
            }
            if (c < 'f') {
                /* Scientific: one leading digit, then the remainder and the
                   exponent suffix built by func_00229BD8. */
                fExp -= 1;
                expLen = func_00229BD8(exponent, fExp, c);
                span = expLen + fLen;
                if (fLen >= 2 || (flags & 1) != 0)
                    span += 1;
            } else if (c == 'f') {
                if (fExp <= 0)
                    span = prec + 2;
                else if (prec != 0)
                    span = fExp + 1 + prec;
                else if ((flags & 1) != 0)
                    span = fExp + 1;
                else
                    span = fExp;
            } else {
                if (fExp < fLen) {
                    if (fExp > 0)
                        span = fLen + 1;
                    else
                        span = fLen + 2 - fExp;
                } else {
                    span = fExp + (flags & 1);
                }
            }
            if (fSign != 0)
                sign = '-';
            goto have_digits;
        }

        /* default: end of the format string, or the character itself. */
        if (c == 0)
            goto end_of_format;
        digits[0] = (char)c;
        text = digits;
        span = 1;
        sign = 0;
        goto have_digits;
    }

numeric:
    /* 0x228C64: `dprec = prec` sits in the delay slot of `bltz $18`, so it
       runs unconditionally (dprec becomes -1 when no precision is given);
       only the '0'-flag clear is guarded. */
    precUsed = prec;
    if (prec >= 0)
        flags &= ~0x80u;
    wall = digits + 0x15C;
    if (value == 0 && precUsed == 0) {
        text = wall;
        span = 0;
        goto have_digits;
    }
    switch (kind) {
    case 0:
        do {
            *--wall = (char)((value & 7) + '0');
            value >>= 3;
        } while (value != 0);
        if ((flags & 1) != 0 && wall[0] != '0')
            *--wall = '0';
        break;
    case 1:
        if (value < 10) {
            *--wall = (char)(value + '0');
        } else {
            do {
                *--wall = (char)(func_0021D408(value, 10) + '0');
                value = func_0021CE38(value, 10);
            } while (value >= 10);
            *--wall = (char)(value + '0');
        }
        break;
    case 2:
        do {
            *--wall = table[value & 0xF];
            value >>= 4;
        } while (value != 0);
        break;
    default:
        /* Unreachable: no conversion above selects kind 3. */
        text = (const char *)D_00391918;
        span = func_00226B48(text);
        goto have_digits;
    }
    text = wall;
    span = (digits + 0x15C) - wall;

have_digits:
    padFlags = flags & 0x84;
    bodyLen = span;
    if (precUsed > span)
        bodyLen = precUsed;
    if (sign != 0)
        bodyLen++;
    else
        bodyLen += (flags & 2);

    /* Everything this conversion prints is collected here in output order and
       handed over by the single loop at `flush`.  The sink only writes the
       bytes in sequence, so where the vector boundaries fall cannot be
       observed; pad runs keep retail's 16-byte step through `chunk`. */
    segCount = 0;

    /* Left padding is skipped by both '-' and a leading zero. */
    if (padFlags == 0 && width > bodyLen)
        SEG(D_003918B0, width - bodyLen, 16);

    if (sign != 0)
        SEG(&sign, 1, 0);

    if ((flags & 2) != 0) {
        prefix[0] = '0';
        prefix[1] = (char)c;
        SEG(prefix, 2, 0);
    }

    /* '0' padding goes between the sign/prefix and the digits. */
    if (padFlags == 0x80 && width > bodyLen)
        SEG(D_003918C0, width - bodyLen, 16);

    /* Precision digits are zeroes appended in front of the value. */
    if (precUsed > span)
        SEG(D_003918C0, precUsed - span, 16);

    if ((flags & 0x100) == 0) {
        SEG(text, span, 0);
    } else if (c < 'f') {
        /* Scientific notation. */
        if (fLen >= 2 || (flags & 1) != 0) {
            prefix[0] = text[0];
            prefix[1] = '.';
            SEG(prefix, 2, 0);
            text++;
        } else {
            SEG(text, 1, 0);
        }
        if (func_0021E340(value, 0) != 0)
            SEG(text, fLen - 1, 0);
        else
            SEG(D_003918C0, fLen - 1, 16);
        SEG(exponent, expLen, 0);
    } else if (func_0021E340(value, 0) == 0) {
        /* Zero: a leading '0', then the point and the requested zeroes. */
        SEG(D_00391938, 1, 0);
        if (fExp < fLen) {
            SEG(point, 1, 0);
            SEG(D_003918C0, fLen - 1, 16);
        } else if ((flags & 1) != 0) {
            SEG(point, 1, 0);
        }
    } else if (fExp > 0) {
        if (fExp < fLen) {
            SEG(text, fExp, 0);
            SEG(D_00391940, 1, 0);
            text += fExp;
            SEG(text, fLen - fExp, 0);
        } else {
            SEG(text, fLen, 0);
            SEG(D_003918C0, fExp - fLen, 16);
            if ((flags & 1) != 0)
                SEG(D_00391940, 1, 0);
        }
    } else {
        SEG(D_00391938, 1, 0);
        SEG(point, 1, 0);
        SEG(D_003918C0, -fExp, 16);
        SEG(text, fLen, 0);
    }

    if ((flags & 4) != 0 && width > bodyLen)
        SEG(D_003918B0, width - bodyLen, 16);

    written += (bodyLen > width) ? bodyLen : width;

flush:
    /* The one place that fills the vector queue. */
    for (segIndex = 0; segIndex < segCount; segIndex++) {
        s32 left = segment[segIndex].length;
        const char *src = segment[segIndex].src;
        s32 step = segment[segIndex].chunk;

        while (left > 0) {
            s32 part = (step != 0 && left > step) ? step : left;

            EMIT(src, part);
            src += part;
            left -= part;
        }
    }
    segCount = 0;
    if (list.total != 0) {
        if (func_00228398(stream, &list) != 0)
            goto done;
    }
    list.count = 0;
    piece = pieces;
    if (n > 0)
        goto next;
    goto done;

end_of_format:
    /* The format string ended: flush what the literal run queued and return. */
    n = 0;
    goto flush;

done:
    /* 0x2299E4: the error flag of the stream turns the count into -1. */
    return ((stream->flags & 0x40) == 0) ? written : -1;

#undef EMIT
#undef PAD
}


INCLUDE_ASM(const s32, "core/main", func_00229A28);

INCLUDE_ASM(const s32, "core/main", func_00229BD8);

INCLUDE_ASM(const s32, "core/main", func_00229CB8);

INCLUDE_ASM(const s32, "core/main", func_0022A7C0);

s32 func_0022A8C8(s32 arg0, s32 arg1, s32 arg2) {

    Blk b;
    s32 r;

    b.v5 = 520;
    b.v4[0] = arg0;
    b.v4[2] = 0x7FFFFFFF;
    b.v6 = arg0;
    b.v7 = 0x7FFFFFFF;
    b.v8 = *(s32 *)D_0038857C;
    r = func_00228498((s32)b.v4, (s32)arg1, (s32)arg2);
    *(u8 *)b.v4[0] = 0;
    return r;
}

extern s32 D_00393DE0[];
s32 func_0022A920(s32 *a1, s32 a2, s32 a3, s32 a4)
{
    s32 v;
    D_00393DE0[0] = 0;
    v = func_001C1130(a2, a3, a4);
    if (v == -1 && D_00393DE0[0] != 0) *a1 = D_00393DE0[0];
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_0022A980);

s32 func_0022AA90(s32 arg0)
{
    return func_0022AAB8(arg0, *(u32 *)D_0038857C + 0x60);
}

INCLUDE_ASM(const s32, "core/main", func_0022AAB8);

extern s32 func_0022E1D0();

s32 func_0022AB38(s32 arg1)
{
    return func_0022E1D0(arg1, 0);
}

extern s32 D_00393DE0[];
s32 func_0022AB58(s32 *a1, s32 a2)
{
    s32 v;
    D_00393DE0[0] = 0;
    v = func_001C1250(a2);
    if (v == -1 && D_00393DE0[0] != 0) *a1 = D_00393DE0[0];
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_0022ABB0);

/* ---------------------------------------------------------------------
 * func_0022ADC8 - newlib _dtoa_r(): double -> decimal digits (4556 bytes).
 *
 * Behavioural reconstruction from asm/matchings/core/main/func_0022ADC8.s.
 * The EE core has no double-precision FPU, so a double travels in one
 * 64-bit GPR and every arithmetic step is a soft-float helper call:
 *   func_0021DDB0 = a - b     func_0021DD58 = a + b
 *   func_0021DE18 = a * b     func_0021E0C0 = a / b
 *   func_0021E340 = compare   func_0021E390 = (double)(s32)
 *   func_0021E448 = (s32)double
 * The body is newlib's dtoa.c block for block, including the shared
 * one_digit / no_digits / bump_up / round_9_up tails that the fast path
 * jumps into.
 *
 * Confirmed from the listing: bs+0x40 = block handed back by the previous
 * call (freed at entry after its _k/_maxwds are repaired from bs+0x44),
 * bs+0x44 = Balloc size class, bs+0x4C = freelist.  A Bigint is
 * {next, k, maxwds, sign, wds, w[1]}: w at +0x14, wds at +0x10.
 *
 * Deliberate deviation, behaviourally irrelevant for printf: retail's
 * register allocator keeps the fast path's saved d2 in the register it
 * also uses for the hoisted `k + 1`, so its `d = d2` at fast_failed
 * (0x22B678) reloads k+1 whenever the Steele & White loop leaves through
 * its digit budget instead of a margin test.  (Retail also leaves
 * `spec_case` at sp+0x40 unwritten for modes >= 2, where dtoa.c wants 0.)
 * Both read back as zero for every mode printf uses (0..3); this code
 * keeps the dtoa.c form, i.e. restores the real d2 and initialises
 * spec_case.
 * --------------------------------------------------------------------- */

#define DTOA_HI(x) ((u32)((x) >> 32))
#define DTOA_DBL(hi) ((u64)(u32)(hi) << 32)

extern u64 D_00391FC0;    /* 0.289529654602168   */
extern u64 D_00391FC8;    /* 0.1760912590558     */
extern u64 D_00391FD0;    /* 0.301029995663981   */
extern u64 D_00392050[];  /* tens[]:    1e0 .. 1e22  */
extern u64 D_00392118[];  /* bigtens[]: 1e16 .. 1e128 */
extern u64 D_00392138;    /* 1e256, a private copy of bigtens[4] */
extern u8 D_00391FA0[];   /* "Infinity" */
extern u8 D_00391FB0[];   /* "NaN"      */
extern u8 D_00391FB8[];   /* "0"        */


u8 *func_0022ADC8(u8 *bs, u64 d, s32 mode, s32 ndigits, s32 *decpt,
                  s32 *sign, u8 **rve)
{














    s32 bbits, b2, b5, be, dig, i, ilim, ilim0, ilim1, j, j1, k, k0;
    s32 k_check, leftright, m2, m5, s2, s5, spec_case, try_quick, denorm;
    u64 d2, eps, ds;
    u32 x;
    u8 *b, *S, *mlo, *mhi, *delta, *s, *s0;

    /* 0x22ADE8: the string of the previous call was written over the
       header of the block handed back, so repair k/maxwds before Bfree. */
    if (*(u8 **)(bs + 0x40) != 0) {
        u8 *old = *(u8 **)(bs + 0x40);

        *(s32 *)(old + 0x04) = *(s32 *)(bs + 0x44);
        *(s32 *)(old + 0x08) = 1 << *(s32 *)(bs + 0x44);
        func_0022C278(bs, (u32 *)old);
        *(u32 *)(bs + 0x40) = 0;
    }

    /* 0x22AE3C: sign, cleared from d before anything else is looked at. */
    if ((s32)DTOA_HI(d) < 0) {
        *sign = 1;
        d = (d & 0xFFFFFFFF) | DTOA_DBL(DTOA_HI(d) & 0x7FFFFFFF);
    } else {
        *sign = 0;
    }

    /* 0x22AE7C: Infinity / NaN, returned as pointers to rodata. */
    if ((DTOA_HI(d) & 0x7FF00000) == 0x7FF00000) {
        *decpt = 9999;
        s = D_00391FA0;
        if ((d & (((u64)0xFFFFF << 32) | 0xFFFFFFFF)) != 0)
            s = D_00391FB0;
        if (rve != 0)
            *rve = (s[3] != 0) ? s + 8 : s + 3;
        return s;
    }

    /* 0x22AEE8: zero. */
    if (func_0021E340(d, 0) == 0) {
        *decpt = 1;
        s = D_00391FB8;
        if (rve != 0)
            *rve = s + 1;
        return s;
    }

    /* 0x22AF28: split d into a normalised mantissa and a binary exponent. */
    b = func_0022CF40(bs, d, &be, &bbits);
    i = (DTOA_HI(d) >> 20) & 0x7FF;
    if (i != 0) {
        /* normalised: force the exponent to 1 so d2 lands in [1,2) */
        d2 = (d & 0xFFFFFFFF) | DTOA_DBL((DTOA_HI(d) & 0xFFFFF) | 0x3FF00000);
        i -= 0x3FF;
        denorm = 0;
    } else {
        /* 0x22AF98: denormalised, assemble the integer mantissa */
        i = bbits + be + 0x432;
        if (i > 32)
            x = (DTOA_HI(d) << (64 - i)) | ((u32)d >> (i - 32));
        else
            x = (u32)d << (32 - i);
        d2 = func_0021E390((s32)x);
        if ((s32)x < 0)
            d2 = func_0021DD58(d2, DTOA_DBL(0x41F00000));  /* + 2^32 */
        d2 -= DTOA_DBL(0x01F00000);                        /* exp -= 31 */
        i -= 0x433;
        denorm = 1;
    }

    /* 0x22B030: k = floor(log10(d)) from the Taylor estimate */
    d2 = func_0021DDB0(d2, DTOA_DBL(0x3FF80000));          /* d2 - 1.5   */
    d2 = func_0021DE18(d2, D_00391FC0);                    /* * 1/(1.5ln10) */
    ds = func_0021DD58(d2, D_00391FC8);                    /* + log10 1.5 */
    ds = func_0021DD58(ds, func_0021DE18(func_0021E390(i), D_00391FD0));
    k = func_0021E448(ds);                                 /* (int)ds     */
    if (func_0021E340(ds, 0) < 0 &&
        func_0021E340(ds, func_0021E390(k)) != 0)
        --k;                                               /* want floor  */

    /* 0x22B0D0: correct k against the table */
    k_check = 1;
    if ((u32)k < 0x17) {
        k_check = 0;
        if (func_0021E340(d, D_00392050[k]) < 0)
            --k;
    }

    /* 0x22B108: powers of 2 and 5 to apply to b and S */
    j = bbits - i - 1;
    if (j >= 0) {
        b2 = 0;
        s2 = j;
    } else {
        b2 = -j;
        s2 = 0;
    }
    if (k >= 0) {
        b5 = 0;
        s5 = k;
        s2 += k;
    } else {
        b2 -= k;
        b5 = -k;
        s5 = 0;
    }

    if (mode < 0 || mode > 9)
        mode = 0;
    try_quick = 1;
    if (mode > 5) {
        mode -= 4;
        try_quick = 0;
    }
    leftright = 1;
    ilim = ilim1 = -1;
    if (mode < 2) {
        /* mode 0, 1: shortest string */
        i = 18;
        ndigits = 0;
    } else if (mode == 2 || mode == 4) {
        /* ecvt style: max(1, ndigits) significant digits */
        if (mode == 2)
            leftright = 0;
        if (ndigits <= 0)
            ndigits = 1;
        ilim = ilim1 = i = ndigits;
    } else {
        /* mode 3, 5: fcvt style, ndigits past the point */
        if (mode == 3)
            leftright = 0;
        i = ndigits + k + 1;
        ilim = i;
        ilim1 = i - 1;
        if (i <= 0)
            i = 1;
    }

    /* 0x22B21C: Balloc size class for the output block.  Retail only
       resets bs->k inside the >= 24 arm, so `i < 24` keeps the previous
       class; the entry repair uses the same field, so the two agree. */
    if (i >= 24) {
        *(s32 *)(bs + 0x44) = 0;
        for (j = 4; i >= j + 0x14; j <<= 1)
            ++*(s32 *)(bs + 0x44);
    }
    b = func_0022C1D0(bs, *(s32 *)(bs + 0x44));
    s = s0 = b;
    *(u8 **)(bs + 0x40) = b;

    if (ilim >= 0 && ilim <= 14 && try_quick) {
        /* ---- fast path: digits straight out of the double ---------- */
        i = 0;
        d2 = d;
        k0 = k;
        ilim0 = ilim;
        m5 = 2;                                  /* ieps, conservative  */
        if (k > 0) {
            ds = D_00392050[k & 0xF];
            j = k >> 4;
            if (j & 0x10) {                      /* Bletch: avoid overflow */
                j &= 0xF;
                d = func_0021E0C0(d, D_00392138);
                ++m5;
            }
            for (; j != 0; j >>= 1, ++i)
                if (j & 1) {
                    ++m5;
                    ds = func_0021DE18(ds, D_00392118[i]);
                }
            d = func_0021E0C0(d, ds);
        } else if ((j1 = -k) != 0) {
            d = func_0021DE18(d, D_00392050[j1 & 0xF]);
            for (j = j1 >> 4; j != 0; j >>= 1, ++i)
                if (j & 1) {
                    ++m5;
                    d = func_0021DE18(d, D_00392118[i]);
                }
        }
        if (k_check && func_0021E340(d, DTOA_DBL(0x3FF00000)) < 0 && ilim > 0) {
            if (ilim1 <= 0)
                goto fast_failed;
            ilim = ilim1;
            --k;
            d = func_0021DE18(DTOA_DBL(0x40240000), d);   /* d *= 10. */
            ++m5;
        }
        /* eps = (m5 * d + 7) * 2^-52 */
        eps = func_0021DD58(
            func_0021DE18(func_0021E390(m5), d), DTOA_DBL(0x401C0000));
        eps -= DTOA_DBL(0x03400000);

        if (ilim == 0) {
            /* no digits wanted, only a rounding decision */
            S = 0;
            mhi = 0;
            ds = func_0021DDB0(d, DTOA_DBL(0x40140000));  /* d - 5. */
            if (func_0021E340(ds, eps) > 0)
                goto one_digit;
            if (func_0021E340(ds, func_0021DDB0(0, eps)) < 0)
                goto no_digits;
            goto fast_failed;
        }

        if (leftright) {
            /* 0x22B494: Steele & White */
            eps = func_0021DDB0(
                func_0021E0C0(DTOA_DBL(0x3FE00000), D_00392050[ilim - 1]),
                eps);
            for (i = 0;;) {
                dig = func_0021E448(d);
                d = func_0021DDB0(d, func_0021E390(dig));
                *s++ = (u8)(0x30 + dig);
                if (func_0021E340(d, eps) < 0)
                    goto ret1;
                if (func_0021E340(func_0021DDB0(DTOA_DBL(0x3FF00000), d),
                                  eps) < 0)
                    goto bump_up;
                if (++i >= ilim)
                    break;
                eps = func_0021DE18(DTOA_DBL(0x40240000), eps);
                d = func_0021DE18(DTOA_DBL(0x40240000), d);
            }
        } else {
            /* 0x22B580: ilim digits, then fix them up */
            eps = func_0021DE18(D_00392050[ilim - 1], eps);
            for (i = 1;; ++i, d = func_0021DE18(DTOA_DBL(0x40240000), d)) {
                dig = func_0021E448(d);
                d = func_0021DDB0(d, func_0021E390(dig));
                *s++ = (u8)(0x30 + dig);
                if (i == ilim) {
                    if (func_0021E340(
                            d, func_0021DD58(DTOA_DBL(0x3FE00000), eps)) > 0)
                        goto bump_up;
                    if (func_0021E340(
                            d, func_0021DDB0(DTOA_DBL(0x3FE00000), eps)) < 0) {
                        while (*--s == 0x30)
                            ;
                        ++s;
                        goto ret1;
                    }
                    break;
                }
            }
        }

    fast_failed:
        s = s0;
        d = d2;
        k = k0;
        ilim = ilim0;
    }

    /* 0x22B688: do we have a "small" integer? */
    if (be >= 0 && k <= 14) {
        ds = D_00392050[k];
        if (ndigits < 0 && ilim <= 0) {
            S = 0;
            mhi = 0;
            if (ilim < 0 ||
                func_0021E340(d, func_0021DE18(DTOA_DBL(0x40140000), ds)) <= 0)
                goto no_digits;
            goto one_digit;
        }
        for (i = 1;; ++i) {
            u64 t = func_0021E0C0(d, ds);

            dig = func_0021E448(t);
            d = func_0021DDB0(d, func_0021DE18(func_0021E390(dig), ds));
            *s++ = (u8)(0x30 + dig);
            if (i == ilim) {
                d = func_0021DD58(d, d);
                if (func_0021E340(d, ds) > 0 ||
                    (func_0021E340(d, ds) == 0 && (dig & 1))) {
                bump_up:
                    while (*--s == 0x39) {
                        if (s == s0) {
                            ++k;
                            *s = 0x30;
                            break;
                        }
                    }
                    ++*s;
                    ++s;
                }
                break;
            }
            d = func_0021DE18(DTOA_DBL(0x40240000), d);
            if (func_0021E340(d, 0) == 0)
                break;
        }
        goto ret1;
    }

    /* ---- 0x22B808: Bigint path ----------------------------------- */
    m2 = b2;
    m5 = b5;
    mhi = 0;
    mlo = 0;
    if (leftright) {
        if (mode < 2) {
            if (denorm)
                i = be + 0x433;          /* Bias + P - 1 - 1 + 1 */
            else
                i = 0x36 - bbits;        /* 1 + P - bbits */
        } else {
            j = ilim - 1;
            if (m5 >= j) {
                m5 -= j;
            } else {
                j -= m5;
                s5 += j;
                b5 += j;
                m5 = 0;
            }
            if ((i = ilim) < 0) {
                m2 -= i;
                i = 0;
            }
        }
        b2 += i;
        s2 += i;
        mhi = func_0022C650(bs, 1);       /* i2b(1) */
    }

    /* 0x22B8D0: spend the common powers of 2 on both sides */
    if (m2 > 0 && s2 > 0) {
        i = (m2 < s2) ? m2 : s2;
        b2 -= i;
        m2 -= i;
        s2 -= i;
    }

    /* 0x22B900: 5^b5 */
    if (b5 > 0) {
        if (leftright) {
            if (m5 > 0) {
                u8 *b1;

                mhi = func_0022C8B8(bs, mhi, m5);       /* pow5mult */
                b1 = func_0022C688(bs, mhi, b);         /* mult     */
                func_0022C278(bs, (u32 *)b);
                b = b1;
            }
            if ((j = b5 - m5) != 0)
                b = func_0022C8B8(bs, b, j);
        } else {
            b = func_0022C8B8(bs, b, b5);
        }
    }

    /* 0x22B980: S = 2^s2 * 5^s5 */
    S = func_0022C650(bs, 1);
    if (s5 > 0)
        S = func_0022C8B8(bs, S, s5);

    /* 0x22B9AC: d is an exact power of 2 -> upper margin is doubled */
    spec_case = 0;
    if (mode < 2) {
        if ((d & (((u64)0xFFFFF << 32) | 0xFFFFFFFF)) == 0 &&
            (DTOA_HI(d) & 0x7FF00000) != 0) {
            b2 += 1;                     /* Log2P == 1 */
            s2 += 1;
            spec_case = 1;
        }
    }

    /* 0x22BA04: align the divisor so it has 4 leading zero bits */
    if (s5 != 0)
        i = s2 - func_0022C508(
                      *(u32 *)(S + 0x14 + (*(s32 *)(S + 0x10) - 1) * 4));
    else
        i = s2 + 1;
    i &= 0x1F;
    if (i != 0)
        i = 0x20 - i;
    if (i > 4) {
        i -= 4;
        b2 += i;
        m2 += i;
        s2 += i;
    } else if (i < 4) {
        i += 0x1C;
        b2 += i;
        m2 += i;
        s2 += i;
    }

    if (b2 > 0)
        b = func_0022C9B8(bs, b, b2);     /* lshift */
    if (s2 > 0)
        S = func_0022C9B8(bs, S, s2);

    /* 0x22BAD4: the k estimate was one too large */
    if (k_check) {
        if (func_0022CB30(b, S) < 0) {
            --k;
            b = func_0022C2A8(bs, b, 10, 0);
            if (leftright)
                mhi = func_0022C2A8(bs, mhi, 10, 0);
            ilim = ilim1;
        }
    }

    if (ilim <= 0 && mode > 2) {
        if (ilim < 0 ||
            func_0022CB30(b, S = func_0022C2A8(bs, S, 5, 0)) <= 0) {
        no_digits:
            k = -1 - ndigits;
            goto ret;
        }
    one_digit:
        *s++ = 0x31;
        ++k;
        goto ret;
    }

    /* 0x22BBA4: digit generation */
    if (leftright) {
        if (m2 > 0)
            mhi = func_0022C9B8(bs, mhi, m2);
        mlo = mhi;
        if (spec_case) {
            mhi = func_0022C1D0(bs, *(s32 *)(mlo + 0x04));   /* Balloc(mlo->k) */
            func_002249D4(mhi + 0x0C, mlo + 0x0C,
                          *(s32 *)(mlo + 0x10) * 4 + 8);     /* Bcopy */
            mhi = func_0022C9B8(bs, mhi, 1);
        }
        for (i = 1;; ++i) {
            dig = func_0022ABB0(b, S) + 0x30;    /* quorem */
            j = func_0022CB30(b, mlo);
            delta = func_0022CB98(bs, S, mhi);
            j1 = *(s32 *)(delta + 0x0C) ? 1 : func_0022CB30(b, delta);
            func_0022C278(bs, (u32 *)delta);
            if (j1 == 0 && mode == 0 && ((u32)d & 1) == 0) {
                /* shortest string that rounds to d */
                if (dig == 0x39)
                    goto round_9_up;
                if (j > 0)
                    ++dig;
                *s++ = (u8)dig;
                goto ret;
            }
            if (j < 0 || (j == 0 && mode == 0 && ((u32)d & 1) == 0)) {
                if (j1 > 0) {
                    b = func_0022C9B8(bs, b, 1);
                    j1 = func_0022CB30(b, S);
                    if (j1 > 0 || (j1 == 0 && (dig & 1))) {
                        if (dig++ == 0x39)
                            goto round_9_up;
                    }
                }
                *s++ = (u8)dig;
                goto ret;
            }
            if (j1 > 0) {
                if (dig == 0x39) {
                round_9_up:
                    *s++ = 0x39;
                    goto roundoff;
                }
                *s++ = (u8)(dig + 1);
                goto ret;
            }
            *s++ = (u8)dig;
            if (i == ilim)
                break;
            b = func_0022C2A8(bs, b, 10, 0);
            if (mlo == mhi) {
                mhi = func_0022C2A8(bs, mhi, 10, 0);
                mlo = mhi;
            } else {
                mlo = func_0022C2A8(bs, mlo, 10, 0);
                mhi = func_0022C2A8(bs, mhi, 10, 0);
            }
        }
    } else {
        for (i = 1;; ++i) {
            dig = func_0022ABB0(b, S) + 0x30;
            *s++ = (u8)dig;
            if (i >= ilim)
                break;
            b = func_0022C2A8(bs, b, 10, 0);
        }
    }

    /* 0x22BE4C: round off the last digit */
    b = func_0022C9B8(bs, b, 1);
    j = func_0022CB30(b, S);
    if (j > 0 || (j == 0 && (dig & 1))) {
    roundoff:
        while (*--s == 0x39) {
            if (s == s0) {
                ++k;
                *s++ = 0x31;
                goto ret;
            }
        }
        ++*s;
        ++s;
    } else {
        while (*--s == 0x30)
            ;
        ++s;
    }

ret:
    func_0022C278(bs, (u32 *)S);
    if (mhi != 0) {
        if (mlo != 0 && mlo != mhi)
            func_0022C278(bs, (u32 *)mlo);
        func_0022C278(bs, (u32 *)mhi);
    }
ret1:
    func_0022C278(bs, (u32 *)b);
    *s = 0;
    *decpt = k + 1;
    if (rve != 0)
        *rve = s;
    return s0;
}

#undef DTOA_HI
#undef DTOA_DBL

INCLUDE_ASM(const s32, "core/main", func_0022BF98);

INCLUDE_ASM(const s32, "core/main", func_0022C040);

extern u8 D_00391FF8[];

u8 *func_0022C0C8() { return D_00391FF8; }

s32 func_0022C0D8(s32 arg0, s32 arg1)
{
    return func_0022C040(*(s32 *)D_0038857C, arg0, arg1);
}

u8 *func_0022C108(void)
{
    return func_0022C0C8(*(s32 *)D_0038857C);
}

extern s32 D_00393DE0[];
s32 func_0022C130(s32 *a1, s32 a2, s32 a3, s32 a4)
{
    s32 v;
    D_00393DE0[0] = 0;
    v = func_001C1260(a2, a3, a4);
    if (v == -1 && D_00393DE0[0] != 0) *a1 = D_00393DE0[0];
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_0022C190);

INCLUDE_ASM(const s32, "core/main", func_0022C1D0);

void func_0022C278(u8 *arg0, u32 *arg1)
{
    u32 *bucket;
    u32 idx;

    if (arg1 == 0)
        return;
    idx = arg1[1];
    bucket = (u32 *)((idx << 2) + *(u32 *)(arg0 + 0x4C));
    arg1[0] = *bucket;
    *bucket = (u32)arg1;
}

INCLUDE_ASM(const s32, "core/main", func_0022C2A8);

INCLUDE_ASM(const s32, "core/main", func_0022C3C0);

s32 func_0022C508(s32 x)
{
    s32 v = 0;
    if ((x & 0xFFFF0000u) == 0) { v = 16; x <<= 16; }
    if ((x & 0xFF000000u) == 0) { v += 8; x <<= 8; }
    if ((x & 0xF0000000u) == 0) { v += 4; x <<= 4; }
    if ((x & 0xC0000000u) == 0) { v += 2; x <<= 2; }
    if (x < 0) goto done;
    v += 1;
    if ((x & 0x40000000u) == 0) return 32;
done:
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_0022C590);

u8 *func_0022C650(u8 *arg0, s32 arg1)
{

    u8 *p = func_0022C1D0(arg0, 1);
    *(s32 *)(p + 0x14) = arg1;
    *(s32 *)(p + 0x10) = 1;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0022C688);

INCLUDE_ASM(const s32, "core/main", func_0022C8B8);

INCLUDE_ASM(const s32, "core/main", func_0022C9B8);

INCLUDE_ASM(const s32, "core/main", func_0022CB30);

INCLUDE_ASM(const s32, "core/main", func_0022CB98);

INCLUDE_ASM(const s32, "core/main", func_0022CD28);

INCLUDE_ASM(const s32, "core/main", func_0022CDC0);

INCLUDE_ASM(const s32, "core/main", func_0022CF40);

INCLUDE_ASM(const s32, "core/main", func_0022D0C0);

INCLUDE_ASM(const s32, "core/main", func_0022D180);

extern s32 D_00393DE0[];
s32 func_0022D1F0(s32 *a1, s32 a2, s32 a3, s32 a4)
{
    s32 v;
    D_00393DE0[0] = 0;
    v = func_001C11B0(a2, a3, a4);
    if (v == -1 && D_00393DE0[0] != 0) *a1 = D_00393DE0[0];
    return v;
}

INCLUDE_ASM(const s32, "core/main", func_0022D250);

/* Behavior-faithful, not byte-matching: 1 when the double's exponent field is
   saturated (inf/NaN), computed as (0x7FF00000 - masked) >> 31 with the low
   word folded into bit 0; size exact at 0x38, registers shifted by one. */
u32 func_0022D298(u64 arg0)
{
    u32 lo = (s32)arg0;
    u32 hi = (u32)(arg0 >> 32);
    u32 n = (hi & 0x7FFFFFFF) | ((lo | -lo) >> 31);

    return (0x7FF00000 - n) >> 31;
}

INCLUDE_ASM(const s32, "core/main", func_0022D2D0);

s32 func_0022E1D0(s32 arg0, s32 arg1)
{
    return func_0022D2D0(*(s32 *)D_0038857C, arg0, arg1);
}

s32 func_0022E200(void)
{

    return func_0021E5E8(((s32 (*)())func_0022E1D0)());
}

INCLUDE_ASM(const s32, "core/main", func_0022E228);

s32 func_0022E448(s32 arg0, s32 arg1, s32 arg2)
{
    return func_0022E228(*(s32 *)D_0038857C, arg0, arg1, arg2);
}

INCLUDE_ASM(const s32, "core/main", func_0022E480);

INCLUDE_ASM(const s32, "core/main", func_0022E570);

INCLUDE_ASM(const s32, "core/main", func_0022E6F8);

INCLUDE_ASM(const s32, "core/main", func_0022E7B8);
