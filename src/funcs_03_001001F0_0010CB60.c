#include "common.h"

typedef struct __attribute__((packed)) {
    u64 value;
} UnalignedU64;

/* 12-byte record copied as a whole: packed doubleword + aligned word. The
   aggregate copy keeps ldl/ldr/lw together before sdl/sdr/sw, as retail does. */
typedef struct
{
    UnalignedU64 v;
    u32 w;
} Unaligned12;
/* Plain 12-byte record; the codegen for copying it is the ldl/ldr + sdl/sdr
   doubleword pair plus a trailing word, which is what retail does. */
typedef struct {
    u32 a;
    u32 b;
    u32 c;
} Struct12;

extern s32 D_00392F24;
extern s32 D_0039347C;
extern s32 D_00393178;
extern s32 D_00393174;
extern u32 D_00330990[];
extern f32 D_003930F8;
extern u8 D_0026F1E0[];
extern u8 D_0026F220[];
extern s32 D_002536F8[];
extern s32 D_00254018[];
extern s32 D_00393028[];
extern s16 D_003947A8[];
void func_00119640(u8 *, f32, f32);
extern u8 D_0034A380[];
extern u8 D_0034A300[];
extern u8 D_00393C20;
extern f32 D_00393050;
extern s32 D_0025236C[];
extern u8 D_003931E9;
extern s32 D_00393294;
extern s32 D_00254030[];
extern s32 D_00254034[];
extern f32 D_003924C4;
extern s32 D_0026DFB8[];
extern u8 D_0026BEE8[];
extern u8 D_00254028[];
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

s32 func_001001F0(void)
{
    StartupConfig config;
    u8 *startup_data = D_00252274;

    if (func_00116F00(&config, startup_data) != 0) {
        StartupState *startup_state = (StartupState *)(startup_data - 0x44);

        config.unk70 = 0;
        config.unk68 = 0;
        config.unk74 = 1000;
        config.unk50 = 100;
        config.unk6C = 1000;
        config.unk54 = 100;
        config.unk78 = 0;
        config.unk7C = 0;

        if (func_00117078(
                startup_state->unk44, (u8 *)&config + 0x20, startup_data + 2
            ) != 0) {
            func_00117A60(
                startup_state->unk44, startup_state->unk46, &D_00392F10
            );
        }
    }

    return 1;
}

void func_00100288(void)
{
    while (1) {
        func_001C0B40();
    }
}

extern u32 D_00393E20[];
extern u32 D_0039A3F0[];

int func_001002A0(void)
{
    u32 thread_context[12];
    int thread;
    u32 table_value;

    func_0021AE88();
    func_00224B88(thread_context, 48, 0);
    table_value = D_0039A3F0[0];
    thread_context[1] = func_00100288;
    thread_context[2] = (u32*)&D_00393E20;
    thread_context[3] = 2048;
    thread_context[5] = 63;
    thread_context[4] = table_value;

    thread = func_001C0A20(thread_context);
    func_001C0AB0(thread, 127);
    thread = func_001C0B10();
    func_001C0AB0(thread, 63);

    func_001000C0();
    func_001000C8();

    if (func_001001F0())
    {
        func_001000D0();
        func_00100130();
    }

    return 0;
}

s32 func_00100360(void)
{
    return D_00252230.state == 2;
}

/* Process-state predicates. */

s32 func_00100378(void)
{
    u8 state = D_00252230.state;
    return state == 2 || state == 6 || state == 8;
}

/* Startup routines. */

INCLUDE_ASM(void, "core/main", func_001003B0);

void func_00112AA0(void);
void func_0011E0E0(void);
s32 func_00120950(s32);
s32 func_0014B9C0(f32);
s32 func_00120AD8(void);
void func_00211D50(void);
void func_00120BA8(s32, s32);
void func_00110E88(void);

void func_00100460(void)
{
    s32 state = D_00252230.state;
    s32 toggle;
    s32 threshold;
    s32 ad8;
    s32 ba18;
    s32 r2;
    s32 i;
    u8 buffer[0x20];

    if (state == 3) {
        goto main;
    }
    if (state < 4) {
        if (state == 1) {
            func_001022E8();
            func_001126D0();
            func_001123F8(3);
            goto main;
        }
        return;
    }
    if (state == 5) {
        func_00120C58();
        func_00112AA0();
        func_00102570();
        func_001123F8(9);
        func_00110E88();
        func_001003B0();
        func_0014B980();
        return;
    }
    if (state == 7) {
        func_00101E30();
        func_00110E88();
        func_001123F8(9);
        return;
    }
    return;
main:
    func_00120C38();
    for (i = 16; --i >= 0;) {
        func_0014BA78(0xF);
        func_0011E0E0();
    }
    toggle = func_00120950(0);
    *(s64 *)buffer = *(s64 *)D_00388B00;
    buffer[8] = D_00388B00[8];
    *(s64 *)(buffer + 0x10) = *(s64 *)D_00388B10;
    *(u16 *)(buffer + 0x18) = *(u16 *)(D_00388B10 + 8);
    buffer[0x1A] = D_00388B10[0xA];
    func_001533D8((u32 *)(toggle ? buffer : buffer + 0x10), D_0039313C, D_00393140, 1);
    func_001533D8((u32 *)(toggle ? buffer + 0x10 : buffer), D_0039313C, D_00393140, 0);
    func_0014B980();
    D_00252230.unk13F7 = 0;
    for (;;) {
        ba18 = func_0014BA18();
        r2 = func_00211F18();
        threshold = ba18 + func_0014B9C0((f32)(u32)r2);
        for (;;) {
            if (func_00120950(0) != toggle) {
                func_00153500();
                toggle = (toggle ^ 1) & 0xFF;
            }
            func_0014BA78(0xF);
            func_0011ECC0();
            ad8 = func_00120AD8();
            ba18 = func_0014BA18();
            if ((u32)ba18 >= (u32)threshold) {
                break;
            }
            if (ad8 == 0) {
                break;
            }
        }
        if (ad8 != 0) {
            func_00211D50();
            D_00252230.unk13F7 = D_00393B58;
        } else {
            /* NOTE: retail passes the register leftover of the preceding call
             * as the first argument here; 0 is used as a deterministic
             * substitute. */
            func_00120BA8(0, 0);
            func_001126D0();
        }
        if (D_00252230.unk13F7 != 0) {
            break;
        }
        if (ad8 == 0) {
            break;
        }
    }
    func_00101CF0();
    func_001123F8(9);
    func_00110E88();
    func_001003B0();
    func_001126D0();
    func_00144DA8();
    func_0014B980();
}

extern u8 D_00253627;

s32 func_00100728(void)
{
    s32 result;
    s32 unk1;

    if (D_00252230.state == 6)
    {
        if (D_00253627 != 0)
        {
            D_00253627 = 0;
            func_00102698();
            return func_001123F8(3);
        }
        else
        {
            func_00102698();
            unk1 = func_001126C0();
            func_00112408(unk1);
            return func_001123F8(5);
        }
    }
    else
    {
        result = 2;

        if (D_00252230.state >= 7)
        {
            result = 8;

            if (D_00252230.state == 8)
            {
                func_00102138();
                return func_001123F8(7);
            }
        }
        else if (D_00252230.state == 2)
        {
            func_00102698();
            return func_00101CB8();
        }
    }

    return result;
}

extern const void func_00100CD0();
extern const void func_001010F8();
extern const void func_001028C8();

void func_001007F8(void)
{
    if (func_00100360() == 0) {
        func_00100CD0();

        if (D_00252230.state != 2) {
            func_001010F8();
            func_001028C8();
        }
    }
}

INCLUDE_ASM(const s32, "core/main", func_00100848);

INCLUDE_ASM(const s32, "core/main", func_00100A38);

INCLUDE_ASM(const s32, "core/main", func_00100BD0);

INCLUDE_ASM(const void, "core/main", func_00100CD0);

extern s32 D_00392F24;
extern u8 func_0010CA50(s32 *);
extern void func_0014AD90(void *, s32, s32);

void func_00100F58(void *arg1)
{
    s32 result = func_0010CA50(arg1);
    func_0014AD90(arg1, D_00392F24, result);
}

INCLUDE_ASM(const s32, "core/main", func_00100F88);

extern u8 D_00252398[];
extern s32 D_00392F24; 

int func_00101068(void)
{
  int unk1;
  int unk2;
  int unk3;

  unk1 = *(int *)(*(u8 **)D_00252398 + 8);
  for (unk2 = func_0014ABD8(unk1); unk1 != 0; unk2 = func_0014ABD8(unk2))
  {
    if (*(s32 *)unk1 == 2)
    {
      unk3 = func_0010CA50(unk1);
      func_0014AD90(unk1, D_00392F24, unk3);
    }
    unk1 = unk2;
  }
  func_00100F88(D_00392F24);
}

INCLUDE_ASM(const void, "core/main", func_001010F8);

/* Sums bit 1 of the entity flags over every actor of one list. */
s32 func_00101260(ActorManager *manager)
{
    ActorListEntry *node = manager->scene->head;
    s32 total = 0;

    while (node != 0) {
        total += (node->value->component->entity->flags >> 1) & 1;
        node = node->next;
    }

    return total;
}

/* Sums the same count over the current world and all of its groups. */
s32 func_001012A0(void)
{
    ActorRoot *root = *(ActorRoot **)D_00252398;
    ActorManager *manager = *root->context->world;
    ActorScene *scene;
    s32 total;
    s32 count;
    void **item;

    total = func_00101260(manager);
    scene = manager->scene;
    count = scene->groupCount;
    item = (void **)scene->groups;
    while (count > 0) {
        total += func_00101260((ActorManager *)*item);
        count--;
        item++;
    }

    return total;
}

/* Pointers to the two primary worlds; func_0010B860 points them at the
 * static structures at D_00394788 and D_00394798. */
extern void *D_00392FDC;
extern void *D_00392FE0;

/* Reports bit 2 of the entity flags, but only for kind-2 nodes whose entity
 * belongs to one of the two primary worlds.  Every other node yields 0. */
s32 func_00101320(ActorNode *node)
{
    ActorComponent *component = node->component;
    ActorEntity *entity = component->entity;
    s32 result = 0;

    if (entity->world == D_00392FDC || entity->world == D_00392FE0) {
        /* Retail clears the component pointer for nodes of any other kind and
         * then still loads through it, so that behaviour is kept here. */
        component = node->kind == 2 ? component : 0;
        result = (component->entity->flags >> 2) & 1;
    }

    return result;
}

/* Entry in a world's object list: the next entry at +0x00, the object it
 * belongs to at +0x18, and the entry's own state flags at +0x34. */
typedef struct WorldEntry {
    struct WorldEntry *next;
    u8 unk04[0x14];
    struct ActorNode *object;
    u8 unk1C[0x18];
    u8 flags;
} WorldEntry;

/* Each primary world starts with the head of its object list. */
typedef struct ActorWorld {
    struct WorldEntry *entries;
} ActorWorld;

extern void func_001DED18(ActorNode *object);
extern void func_0010E638(ActorNode *object, ActorComponent *component);
extern void func_0015C520(ActorNode *object, s32 arg2);
extern s32 D_00392F88;

/* Walks the second world's object list.  Every entry whose object's entity has
 * flag bit 2 set is refreshed and has that bit cleared. */
void func_00101380(void)
{
    WorldEntry *entry = ((ActorWorld *)D_00392FE0)->entries;

    while (entry != 0) {
        ActorNode *object = entry->object;
        ActorComponent *component = object->component;
        ActorEntity *entity = component->entity;

        if (entity != 0 && ((entity->flags >> 2) & 1) == 1) {
            if ((entry->flags & 1) == 0)
                func_001DED18(object);
            func_0010E638(object, object->component);
            object->component->entity->flags &= ~4;
            func_0015C520(object, D_00392F88);
        }

        entry = entry->next;
    }
}

/* List header: last entry at +0x08, first entry at +0x0C, entry count at
 * +0x10. */
typedef struct ObjectList {
    u8 unk00[8];
    struct ActorNode *tail;
    struct ActorNode *head;
    s32 count;
} ObjectList;

/* Unlinks node from whatever list owns it and re-links it at the tail or the
 * head of the global object list, depending on the entity byte reported by
 * func_0010CA50.  Returns the list's new entry count. */
s32 func_00101438(ActorNode *node)
{
    ObjectList *list = (ObjectList *)D_00392F24;
    s32 append = func_0010CA50((s32 *)node);

    /* Unlink from the current owner. */
    if (node->prev != 0)
        node->prev->next = node->next;
    if (node->next != 0)
        node->next->prev = node->prev;
    if (node->owner != 0) {
        node->owner->count--;
        if (node->owner->tail == node)
            node->owner->tail = node->prev;
        if (node->owner->head == node)
            node->owner->head = node->next;
    }

    node->prev = 0;
    node->next = 0;
    node->owner = list;

    if (append != 0) {
        /* Append at the tail. */
        node->prev = list->tail;
        if (list->head != 0)
            list->tail->next = node;
        else
            list->head = node;
        list->tail = node;
    } else {
        /* Prepend at the head. */
        node->next = list->head;
        if (list->tail != 0)
            list->head->prev = node;
        else
            list->tail = node;
        list->head = node;
    }

    list->count++;
    return list->count;
}

extern s32 func_00106400(ActorNode *node);
extern void func_0010BC08(ActorEntity *entity);

/* Detaches the entity of a node that is sitting in an object list and marks it
 * as detached.  Nodes whose component sub-object is live, that are not in a
 * list at all, or whose entity flags do not line up are left untouched. */
void func_001015B8(ActorNode *node)
{
    ActorComponent *component = node->component;
    ActorEntity *entity = component->entity;
    s32 other_world = entity->world != D_00392FE0;
    u8 flags;

    /* Kind-2 nodes with a live component sub-object are handled elsewhere. */
    if (func_00106400(node) != 0)
        return;

    /* Not linked into a list, so there is nothing to detach. */
    if (node->owner == 0)
        return;

    entity = component->entity;
    flags = entity->flags;

    if (((flags >> 1) & 1) != 1)
        return;

    /* Entities of the second world are only detached when their sub-type is
     * at least 2; entities of any other world always are. */
    if ((flags & 1) != 0 && (!other_world || entity->unk1D >= 2))
        return;

    if (entity->world == D_00392FDC)
        return;

    func_0010BC08(entity);

    component->entity->flags |= 4;

    if (other_world)
        entity->flags2 |= 0x800;
}

/* Table used by the last pass: a count, an array of 0x40-byte nodes and a
 * parallel cursor array.  D_00252224 and D_0025222C are its +0x14/+0x1C
 * fields. */
typedef struct ActorNodeTable {
    s32 count;
    u8 unk04[0x10];
    u8 *nodes;
    u8 unk18[4];
    u32 *cursor;
} ActorNodeTable;

extern ActorNodeTable D_00252210;
extern s16 D_003932C0;

extern void *func_001118C0(void);
extern s32 func_00111600(const s32 *p);
extern void *func_001118D0(void *arg1);
extern void *func_0014A660(s32 index);

/* Detaches every actor the manager knows about: the main list, each group
 * list, the animation lists, the node table and the fixed-size node pool. */
void func_001016B0(ActorManager *manager)
{
    ActorScene *scene = manager->scene;
    ActorListEntry *entry;
    ActorGroup **group;
    u8 *node;
    u32 *cursor;
    u32 *end;
    void *list;
    s32 count;
    s32 i;

    entry = scene->head;
    while (entry != 0) {
        func_001015B8((ActorNode *)entry->value);
        entry = entry->next;
    }

    scene = manager->scene;
    group = scene->groups;
    count = scene->groupCount;
    while (count > 0) {
        entry = (*group)->list->head;
        while (entry != 0) {
            func_001015B8((ActorNode *)entry->value);
            entry = entry->next;
        }
        group++;
        count--;
    }

    for (list = func_001118C0(); list != 0; list = func_001118D0(list))
        func_001015B8((ActorNode *)func_00111600(list));

    cursor = D_00252210.cursor;
    end = cursor + D_00252210.count;
    node = D_00252210.nodes;
    while (cursor < end) {
        if (*(u32 *)(*(u32 *)(node + 4) + 4) != 0)
            func_001015B8((ActorNode *)node);
        cursor++;
        node += 0x40;
    }

    for (i = 0; i < D_003932C0; i++)
        func_001015B8((ActorNode *)func_0014A660(i));
}

extern void func_0010C028(void);
extern void func_0010BBC8(void);
extern void func_0010BA78(void);
extern void *D_00392F30;
extern void *D_00392F34;

/* Activates the world held in the current context: the two primary worlds are
 * swapped, the previous world is remembered in D_00392F34 and everything that
 * belonged to it is detached.  Nothing happens when it is already current. */
void func_00101828(void)
{
    ActorRoot *root = *(ActorRoot **)D_00252398;
    ActorManager *current = *root->context->world;
    void *worldA;
    void *worldB;
    void *previous;

    if (current == D_00392F30)
        return;

    func_0010C028();
    func_0010BBC8();

    worldA = D_00392FDC;
    worldB = D_00392FE0;
    previous = D_00392F30;

    D_00392F30 = current;
    D_00392FDC = worldB;
    D_00392FE0 = worldA;
    D_00392F34 = previous;

    func_001016B0(current);
    func_00101380();
    func_0010BA78();
    func_0010C028();
}

void func_001018B8(void) {
}

/* Stores value on the event object of the node's actor; clearing it to zero
 * also drops bit 16 of the event flags. */
void func_001018C0(ActorNode *node, s32 value)
{
    ActorComponent *component = node->component;
    ActorAnimState *anim;
    ActorEvent *event;

    anim = component->anim;
    if (anim == 0) {
        return;
    }
    event = anim->actor->event;
    if (event == 0) {
        return;
    }
    event->value = value;
    if (value == 0) {
        event->flags &= 0xFFFEFFFF;
    }
}

typedef struct StartupStatus {
    s32 type;
    struct StartupStatusItem *item;
    u8 unk08[0x14];
    struct StartupStatus *next;
} StartupStatus;

typedef struct StartupStatusItem {
    u32 unk00;
    s32 enabled;
} StartupStatusItem;

typedef struct StartupStatusEntry {
    u8 unk00[0xA4];
    StartupStatus *status;
} StartupStatusEntry;

typedef struct StartupStatusContext {
    u8 unk00[0x1C];
    struct StartupStatusContext *next;
    u8 unk20[0x128];
    StartupStatusEntry *entry;
} StartupStatusContext;

typedef struct StartupStatusContainer {
    u8 unk00[8];
    StartupStatusContext *context;
} StartupStatusContainer;

typedef struct StartupStatusRoot {
    u32 unk00;
    StartupStatusContainer *container;
} StartupStatusRoot;

StartupStatus *func_00101908(StartupStatusRoot *root)
{
    StartupStatusContext *context = root->container->context;
    StartupStatusEntry *entry;
    StartupStatus *status;

    if (context == 0) {
        return 0;
    }

    context = context->next;
    entry = context->entry;
    if (entry == 0) {
        return 0;
    }

    status = entry->status;
    if (status != 0 && status->type != 2) {
        do {
            status = status->next;
        } while (status != 0 && status->type != 2);
    }

    if (status != 0 && status->item->enabled == 0) {
        entry->status = 0;
        status = 0;
    }

    return status;
}

extern void func_0012A628(void);
extern void func_0012BA18(void);

void func_00101980(void **ptr)
{
    ptr[0] = func_0012A628;
    ptr[1] = func_0012BA18;
}

/* Handle of the script the node is currently running, 0 when it has none. */
s32 func_001019A0(ActorNode *node)
{
    ActorScriptHolder *holder = node->component->owner->scripts;

    if (holder == 0)
        return 0;
    return holder->current;
}

/* Motion sample of one animation step, stored as the 8-byte chunk at +0x54 and
 * the extra word at +0x5C of the entity. */
typedef struct MotionSample {
    u32 low;    /* +0x00 */
    u32 high;   /* +0x04 */
    s32 extra;  /* +0x08 */
} MotionSample;

extern s32 func_0010EF70(void *node, s32 index, void *out);
extern s32 func_0010F0F0(void *node, s32 index, void *out);
extern void func_0012D888(void *entity, void *values);

/* Copies the animation sample and the pose values for index into the actor of
 * the node's animation state. */
void func_001019C8(ActorNode *node, s16 index)
{
    ActorComponent *component = node->component;
    MotionSample sample;
    u8 values[0x30];
    ActorEntity *actor;
    s32 hasSample;
    s32 hasValues;

    if (component->anim == 0)
        return;

    actor = component->anim->actor;

    hasSample = func_0010EF70(node, index, &sample);
    hasValues = func_0010F0F0(node, index, values);

    if (hasSample != 0) {
        actor->motionLow = sample.low;
        actor->motionHigh = sample.high;
        actor->motionExtra = sample.extra;
    }

    if (hasValues != 0)
        func_0012D888(actor, values);
}

extern void func_00106E60(void *node);
extern s32 func_00106EA8(void *node);
extern s32 func_001DD7B0(void *node);
extern void func_001DD760(void *node);
extern u8 D_00392F50;

/* Advances the animation of a node: bumps the frame counter of its state owner,
 * publishes it through D_00392F50 and runs the follow-up step when the node has
 * a live script. */
void func_00101A78(ActorNode *node)
{
    ActorComponent *component = node->component;
    ActorAnimOwner *owner = component->owner;
    u16 value;

    func_00106E60(node);

    if (func_00106EA8(node) == 0)
        return;

    component = node->component;
    if (component->script == 0)
        return;

    if (owner != 0)
        value = ++owner->frame;
    else
        value = 0;

    D_00392F50 = (u8)value;

    if (func_001DD7B0(node) == 0) {
        component = node->component;
        if (component->script != 0)
            func_001DD760(node);
    }
}

typedef void (*ScriptCallback)(void *runner, void *node, void *owner,
                               void *script, f32 value);

extern ScriptCallback D_00392F48[];
extern void func_0012ACF0(void *runner, void *script);
extern s32 func_0012B698(const s32 *p);
extern f32 func_0014B900(void);

/* Runs the callback registered for the node's current script. */
void func_00101B20(ActorNode *node)
{
    ActorComponent *component = node->component;
    ActorEntity *actor;
    ScriptCallback *slot;
    void *script;
    f32 value;

    if (component->anim == 0)
        return;

    actor = component->anim->actor;
    script = (void *)func_001019A0(node);

    if (actor->script != script)
        func_0012ACF0(actor, script);

    if (script == 0)
        return;

    slot = &D_00392F48[func_0012B698((const s32 *)script)];
    value = func_0014B900();

    component = node->component;
    (*slot)(actor, node, component->anim, script, value);

    if ((actor->flags0C & 0x4000) != 0) {
        ActorEvent *event = actor->event;
        event->head = event->flags;
    }
}

/* Decides whether the node's script callback has to run this frame. */
void func_00101BF0(ActorNode *node)
{
    ActorComponent *component = node->component;
    ActorEntity *entity = component->entity;
    u32 flags;

    flags = entity->flags2;

    if ((flags & 0x1800) == 0x1800)
        return;

    if ((flags & 0x10000) == 0
        && ((flags & 0x80000) == 0 || (entity->flags >> 7) == 0)) {
        func_00101A78(node);
        component = node->component;
        entity = component->entity;
        if (entity == 0)
            return;
        flags = entity->flags2;
    }

    if ((flags & 0x8000) != 0)
        return;

    if ((flags & 0x400000) != 0 && (entity->flags >> 7) != 0)
        return;

    func_00101B20(node);
}

extern void func_00112E18(void);
extern void func_00112B38(void);
extern void func_00119140(void);
extern void func_0015BA18(void);
extern void func_00113798(void);

void func_00101CB8(void)
{
    func_00112E18();
    func_00112B38();
    func_00119140();
    func_0015BA18();
    func_00113798();
}

/* Node of the level tree walked by func_00102298. */
typedef struct StartupTreeNode {
    u32 unk00[2];
    struct StartupTreeNode *child;
    u32 unk0C[2];
    struct StartupTreeNode *next;
    u32 unk18[5];
    u32 flags;
} StartupTreeNode;

void func_00102298(StartupTreeNode *node);

/* A 32-byte block of a scene record. */
typedef struct SceneBlock32 {
    u64 w0;
    u64 w1;
    u64 w2;
    u64 w3;
} SceneBlock32;

/* Tail of a scene record: two quadwords followed by a word (0x14 bytes). */
typedef struct SceneBlock20 {
    u64 w0;
    u64 w1;
    u32 w2;
} SceneBlock20;

extern void func_00112E20(void);
extern s32 func_0014B980(void);
extern s32 D_00392F80[];
extern s32 *D_00392F44;
extern u8 D_00388B88[];
extern s32 D_0039313C;
extern s32 D_00393140;
extern void func_0014AC98(s32 value);
extern s32 func_001126E0(void);
extern s8 *func_001126C0(void);
extern s32 func_00112408(s8 *handle);
extern void func_001126D0(void);
extern void func_00103FE8(void);
extern s32 func_00144C90(s32 mode);
extern void func_001123F8(u8 state);
extern void func_00105EB0(void);
extern void func_00102570(void);

/* Starts the transition to state 5 (world teardown) and reloads the actors. */
void func_00101CF0(void)
{
    u8 previous = D_00252230.state;

    func_00112E20();
    func_0014B980();
    func_0014AC98(0);

    if (func_001126E0() != 0) {
        func_00112408(func_001126C0());
        func_001126D0();
    }

    func_00103FE8();
    func_00144C90(1);

    D_00252230.active = D_00252230.saved;

    func_001123F8(5);
    func_00102570();
    func_001123F8(previous);
    func_00105EB0();
}

extern void func_001126F0(s32 arg1, s32 arg2);
extern s32 func_00117B18(s32 unused, s32 value);
extern u8 *func_001124E0(void);
extern u8 *func_00112610(void);
extern u8 *func_00112620(void);
extern void func_001580E0(u8 *arg1, u8 *arg2, u8 *arg3);
extern s32 func_0014C450(void);
extern void func_00157D38(s32 value);
extern void func_00103FF0(void);
extern void func_00106380(void);
extern void func_00101E30(void);

/* Starts the transition to state 2 and rebuilds the scene. */
void func_00101D90(void)
{
    u8 *first;
    u8 *second;
    u8 *third;

    func_001126F0(0, 1);
    func_00117B18(0, 0);

    first = func_001124E0();
    second = func_00112610();
    third = func_00112620();
    func_001580E0(first, second, third);

    func_00157D38(func_0014C450() - 1024);
    func_00103FF0();
    func_00106380();
    func_00144C90(2);

    D_00252230.active = D_00252230.saved;

    func_00101E30();
}

extern void func_001417F8(void);
extern s32 func_00106428(s32 value);
extern void func_00104720(void);
extern void func_00120300(void);
extern void func_00102160(void);
extern s32 func_00144CB8(s32 handle, s32 index);
extern void func_0011B148(void);
extern s32 func_00103AF0(s32 index);
extern void func_0015BF30(s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void func_0015BD30(void);
extern void *D_00392F28;
extern u8 D_00392F58;
extern s32 D_00392F9C;
extern u8 D_00393060;

/* Startup step run every frame: refreshes the scene records and the state. */
void func_00101E30(void)
{
    StartupBlock *state = &D_00252230;
    void *node = 0;
    u8 *dest;
    SceneBlock32 *out;
    const SceneBlock32 *src;
    const SceneBlock32 *end;
    s32 result;

    state->unk13FA = 0;
    state->unk13FC = 0;
    func_0014B980();
    func_001417F8();
    func_00102298((StartupTreeNode *)D_00392F28);
    func_00106428(1);

    if (state->state != 5 && state->state != 3) {
        node = (void *)state->active;
        if (*(u8 *)(*(u32 *)(*(u32 *)((u8 *)node + 4) + 4) + 0x29) != 0) {
            func_00144C90(0x20);
            state->active = state->saved;
            func_00104720();
            func_00120300();
        } else {
            func_00102160();
        }
    }

    if (state->flag != 0) {
        if (node != 0) {
            dest = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)((u8 *)node + 4)) + 0x18, 50);
            out = (SceneBlock32 *)dest;
            src = (const SceneBlock32 *)state->recordA;
            end = (const SceneBlock32 *)(state->recordA + 0x40);
            while (src != end)
                *out++ = *src++;
            *(SceneBlock20 *)out = *(const SceneBlock20 *)src;
        }
        if (state->pending != 0) {
            dest = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)(state->pending + 4)) + 0x18, 50);
            out = (SceneBlock32 *)dest;
            src = (const SceneBlock32 *)state->recordB;
            end = (const SceneBlock32 *)(state->recordB + 0x40);
            while (src != end)
                *out++ = *src++;
            *(SceneBlock20 *)out = *(const SceneBlock20 *)src;
        }
    }

    D_00392F30 = 0;
    state->flag = 0;
    func_0011B148();
    D_00393060 = 0;

    func_00100F58((void *)state->active);

    if (state->state == 7 && D_00392F58 != 0) {
        result = func_00103AF0(0);
        func_0015BF30(-1, D_00392F9C, result, 0, 0);
        func_0015BD30();
    }
}

extern void func_00112120(void);
extern void func_001D4568(void);

void func_00102138(void)
{
    func_00101068();
    func_00112120();
    func_001D4568();
}

extern s32 func_00120C58(void);
extern void func_0015C5A8(s32 handle);
extern void func_00120D48(u32 record);
extern void func_00112120(void);
extern s32 func_00101068(void);
extern void func_00104058(void);
extern void func_00103AD8(s32 value, s32 index);
extern void func_00159230(u32 value);
extern void func_0014A600(void);
extern void func_001E83F8(s32 mode);
extern void func_00116ED0(s16 arg1, s16 arg2);
extern u32 D_00354200[];
extern s32 D_00393AE0;

/* Drops the current actors and selects the scene layout of the level. */
void func_00102160(void)
{
    u32 *world;
    func_00120C58();
    func_0015C5A8(D_00392F80[2]);
    func_0015C5A8(D_00392F80[3]);
    func_0015C5A8(D_00392F80[4]);
    func_0015C5A8(D_00392F80[5]);
    func_00120D48((u32)D_00252230.recordA);
    func_00120D48((u32)D_00252230.recordB);
    D_00252230.flag = 0;
    func_00112120();
    func_00101068();
    func_00144C90(0x10);

    D_00252230.active = D_00252230.saved;

    func_00104058();
    func_00102298((StartupTreeNode *)D_00392F28);
    func_00106428(1);
    func_0011B148();

    if (D_00392F58 != 0)
        func_00103AD8(D_00392F44[0], 0);
    else
        func_00103AD8(0, 0);

    func_00159230(D_00392F24);
    D_00393060 = 0;
    func_0014A600();
    D_00392F30 = 0;
    D_00393AE0 = 0;
    func_00120300();
    func_00105EB0();
    func_001E83F8(1);
    func_001D4568();
    func_00116ED0(D_00252230.unk44, D_00252230.unk46);
    func_0014B980();

    world = D_00354200;
    world[0] = 0;
    *(u32 *)((u8 *)world + 0x10EC) = 0;
    *(u32 *)((u8 *)world + 0x1094) = 0;
}

void func_00102298(StartupTreeNode *node)
{
    StartupTreeNode *child;

    node->flags |= 0xEFFFFFFF;
    child = node->child;
    while (child != 0) {
        func_00102298(child);
        child = child->next;
    }
}


/* Source: gameplay/initialization.c */
/* func_001022E8 through func_001022E8; retail address range 0x001022E8-0x00102548. */


extern void func_0010B860(void);
extern void func_00112B10(void);
extern void func_0010CA70(void);
extern void func_001DD748(s32 arg1);
extern s32 func_0015B948(void *out);
extern s32 func_0015B9F8(void);
extern void func_00211918();
extern void func_0015BAC8(s32 value);
extern void func_00144DA8(void);
extern void func_00110F20(void);
extern void func_00103760(void);
extern void func_0010A778(void);
extern void func_00120D18(void);
extern void func_0010DCE8(void);
extern void func_00149770(void);
extern void func_00157F70(void);
extern void func_001533D8(u32 *dst, s32 width, s32 height, s32 depth);
extern s32 func_001CA0F8(void);
extern s32 func_001201B8(s32 arg1);
extern s32 D_00392F64;
extern s32 D_00392F68;
extern u32 D_00392F60;
extern u8 D_00254029;
extern u8 D_00388B20[];
extern u8 D_00388B38[];
extern u8 D_00388B50[];

/* The jump table jtbl_00388B70 in rodata.rodata.s still names the retail case
 * bodies; the C rewrite dispatches without a table, so bind the labels to the
 * retail addresses to keep the table bytes unchanged. */
__asm__(".globl .L001024D0\n.equ .L001024D0, 0x001024D0");
__asm__(".globl .L001024E4\n.equ .L001024E4, 0x001024E4");
__asm__(".globl .L001024EC\n.equ .L001024EC, 0x001024EC");
__asm__(".globl .L001024F4\n.equ .L001024F4, 0x001024F4");
__asm__(".globl .L0010250C\n.equ .L0010250C, 0x0010250C");

/* Resets the actor system before a new level is set up. */
void func_001022E8(void)
{
    StartupBlock *state = &D_00252230;
    s32 status;
    s32 layout;
    s32 scratch[4];

    state->unk0140 = 0;
    state->unk13F8 = 0xFF;
    state->unk0160 = 0;
    state->unk0144 = 0;
    state->unk0148 = 0;
    state->active = 0;
    state->unk016C = 0;
    state->saved = 0;
    state->unk0158 = 0;
    state->unk015C = 0;
    state->unk13F4 = 0;
    state->unk13F5 = 0;
    state->unk13F6 = 0;

    func_0010B860();
    func_00112B10();
    func_0010CA70();
    func_001109D8();
    func_001DD748(0);
    D_00392F64 = 1;

    status = func_0015B948(scratch);
    D_00392F68 = status;
    while (status == 1)
        status = D_00392F68 = func_0015B9F8();

    if (D_00392F68 == 3)
        func_00211918(D_00388B20);
    else if (D_00392F68 == -1)
        func_00211918(D_00388B38);
    else
        func_00211918(D_00388B50);

    func_0015BAC8(0);
    func_00144DA8();
    func_00110F20();
    func_00103760();
    func_0010A778();
    func_00120D18();
    func_0010DCE8();
    func_00120D48((u32)state->recordA);
    func_00120D48((u32)state->recordB);
    state->flag = 0;
    func_00149770();
    func_00104A10(1.0f, 2.0f, 0.0f);
    func_00157F70();
    func_001533D8(&D_00392F60, D_0039313C, D_00393140, 1);

    /* Layers 1..5 select run mode 0..4; every layer above 4 uses mode 3 and
     * the unknown layers fall back to mode 0. */
    layout = func_001CA0F8();
    if (layout >= 1 && layout <= 5) {
        D_00254029 = (u8)(layout - 1);
        state->unk13F8 = (layout >= 5) ? 3 : (u8)(layout - 1);
    } else {
        D_00254029 = 0;
        state->unk13F8 = 3;
    }

    func_001201B8(0);
}
extern void func_00144C80(void);

/* func_00102548 through func_00102548; retail address range 0x00102548-0x00102570. */
void func_00102548(void)
{
    func_00112E30();
    D_00252230.state = 1;
    func_00144C80();
}


/* Source: audio/mpeg_thread.c */
/* func_00102570 through func_00102570; retail address range 0x00102570-0x00102698. */


/* Data block filled in by the thread-create syscall wrapper func_001C0B20. */
typedef struct ThreadInfo {
    u8 unk00[0x18];
    u32 value;                              /* +0x18 */
    u8 unk1C[0x34];
} ThreadInfo;

extern s32 func_001C0B10(void);
extern s32 func_001C0B20();
extern void func_00158A00(u8 value);
extern void func_0014D7A0(void);
extern void func_001417A8(void);
extern void func_00159DD0(s32 arg1, s32 arg2);
extern void func_00120270(void);
extern void func_0010DCC8(void);
extern void func_0011ECC0(void);
extern void func_00158B28(u8 value);
extern void func_001516A8(void);
extern void func_00147920(void);
extern void func_00120B70(s32 arg1);
extern s32 func_00120B38(s32 arg1);
extern u8 D_00393080;
extern s32 D_003930AC;
extern u8 D_00393AE4;

/* Full scene reload performed when a level starts. */
void func_00102570(void)
{
    ThreadInfo info;
    s32 thread;

    thread = func_001C0B10();
    func_001C0B20(thread, &info);
    func_00158A00((u8)info.value);
    D_00393080 = 0;
    D_003930AC = 0;
    D_00393AE4 = 0;
    func_00101D90();
    func_0014D7A0();
    func_001417A8();

    if ((D_00252230.state == 5 || D_00252230.state == 3) && D_00392F58 != 0) {
        func_00103AD8(D_00392F44[0], 0);
        func_00159DD0(D_00392F9C, 0);
    }

    func_00101980((void **)D_00392F48);
    func_00120270();
    func_0010DCC8();
    D_00393060 = 0;
    func_0011B148();
    func_00120300();
    func_0011ECC0();
    func_0011ECC0();
    D_00392F30 = 0;
    D_00392F34 = 0;
    func_001D4568();
    func_001E83F8(0);
    func_00158B28((u8)info.value);
    thread = func_001C0B10();
    func_00211918(&D_00388B88, thread, (s32)info.value);
    func_001516A8();
    func_00147920();
    func_00120B70(0);
    func_00120B38(0);
}


/* Source: misc/early_runtime.c */
/* func_00102698 through func_00103750; retail address range 0x00102698-0x00103760. */



s32 D_00392F80[8];

INCLUDE_ASM(const s32, "core/main", func_00102698);

void func_001028C0(void) {
}

INCLUDE_ASM(void, "core/main", func_001028C8);

INCLUDE_ASM(const s32, "core/main", func_00102A00);

INCLUDE_ASM(const s32, "core/main", func_00102A68);

INCLUDE_ASM(const s32, "core/main", func_00102AD0);

INCLUDE_ASM(const s32, "core/main", func_00102B38);

INCLUDE_ASM(const s32, "core/main", func_00102BB0);

INCLUDE_ASM(const s32, "core/main", func_00102C28);

s32 func_00102CA0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00102CA8);

INCLUDE_ASM(const s32, "core/main", func_00102DA0);

INCLUDE_ASM(const s32, "core/main", func_00102E60);

INCLUDE_ASM(const s32, "core/main", func_00102F00);

s32 func_00103010(void) { return 0; }

s32 func_00103018(void) { return 0; }

s32 func_00103020(void) { return 0; }

s32 func_00103028(void) { return 0; }

s32 func_00103030(void) { return 0; }

s32 func_00103038(void) { return 0; }

s32 func_00103040(void) { return 0; }

void func_00103048(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4)
{
    if (arg4 != 0)
    {
        u8 *p = (u8 *)func_0010A858(arg0, arg1, arg2, arg3);

        if (p != 0)
        {
            s32 probe = *(volatile s32 *)p;

            (void)probe;
        }
        *arg4 = 0x10000;
    }
}

s32 func_00103088(void) { return 0; }

s32 func_00103090(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00103098);

INCLUDE_ASM(const s32, "core/main", func_00103150);

INCLUDE_ASM(const s32, "core/main", func_00103208);

INCLUDE_ASM(const s32, "core/main", func_001032C0);

INCLUDE_ASM(const s32, "core/main", func_00103370);

INCLUDE_ASM(const s32, "core/main", func_00103420);

s32 func_001034D0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_001034D8);

s32 func_00103660(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00103668);

extern void func_00227040(void *, void *);
void func_00103750(void *arg1, void *arg2)
{
    func_00227040(arg2, D_00392F80);
}

INCLUDE_ASM(const s32, "core/main", func_00103760);

extern s32 D_00392FD8;
extern s32 D_00392F24;
extern s32 D_002536C0[];

void func_00103AD8(s32 value, s32 index)
{
    D_002536C0[index] = value;
}

s32 func_00103AF0(s32 index)
{
    return D_002536C0[index];
}

INCLUDE_ASM(const s32, "core/main", func_00103B08);

INCLUDE_ASM(const s32, "core/main", func_00103B70);

INCLUDE_ASM(const s32, "core/main", func_00103C10);

INCLUDE_ASM(const s32, "core/main", func_00103C98);

INCLUDE_ASM(const s32, "core/main", func_00103DB8);

INCLUDE_ASM(const s32, "core/main", func_00103E48);

INCLUDE_ASM(const s32, "core/main", func_00103F20);

void func_00103FE8(void) {
}

extern void func_001497A0(void);
extern void func_00104748(s32, s32);
extern void func_00111950(s32);

void func_00103FF0(void)
{
    func_001497A0();
    func_00104748(1, D_00392F24);
    func_00111950(1);
}

void func_00104020(s32 arg1)
{
    func_00104748((u8)arg1, D_00392F24);
}

extern void func_00104020(s32);

void func_00104030(void)
{
    func_001497A0();
    func_00104020(3);
    func_00111950(3);
}

extern void func_001062F8(void);
extern void func_001003B0(void);

void func_00104058(void)
{
    func_001497A0();
    func_00104020(2);
    func_00111950(2);
    func_001062F8();
    func_001003B0();
}

INCLUDE_ASM(const s32, "core/main", func_00104090);

extern s32 D_00392F8C;
extern s32 D_00392F90;
extern s32 D_00392F94;

/* A 0x54-byte scene record.  Keeping the alignment at four bytes makes the
 * compiler check the joint eight-byte alignment for the block move while still
 * copying the trailing word with a plain lw/sw. */
typedef struct SceneQuad {
    u32 w0;
    u32 w1;
} SceneQuad;

typedef struct SceneRecord {
    SceneQuad quad[10];                     /* +0x00..+0x4F */
    u32 tail;                               /* +0x50 */
} SceneRecord;

/* Copies a 0x54-byte scene record. */
#define COPY_SCENE_RECORD(dst, src) \
    (*(SceneRecord *)(dst) = *(const SceneRecord *)(src))

/* Swaps the scene records of the active and pending roots with the caller's
 * saved copies, then refreshes the four scene handles. */
void func_001040F0(u8 *arg0, u8 *arg1)
{
    StartupBlock *state = &D_00252230;
    void *active = (void *)state->active;
    u8 *dst;
    u8 *src;

    state->flag = 0;

    if (arg0 != 0) {
        if (active != 0) {
            src = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)((u8 *)active + 4)) + 0x18, 50);
            COPY_SCENE_RECORD(state->recordA, src);

            dst = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)((u8 *)active + 4)) + 0x18, 50);
            src = (u8 *)*(u32 *)(arg0 + 0x24);
            COPY_SCENE_RECORD(dst, src);
            state->flag = 1;
        }
    } else {
        if (active != 0) {
            src = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)((u8 *)active + 4)) + 0x18, 50);
            COPY_SCENE_RECORD(state->recordA, src);
            state->flag = 1;
        }
    }

    if (arg1 != 0) {
        if (state->pending != 0) {
            src = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)((u8 *)state->pending + 4)) + 0x18, 50);
            COPY_SCENE_RECORD(state->recordB, src);

            dst = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)((u8 *)state->pending + 4)) + 0x18, 50);
            src = (u8 *)*(u32 *)(arg1 + 0x24);
            COPY_SCENE_RECORD(dst, src);
            state->flag = 1;
        }
    } else {
        if (state->pending != 0) {
            src = (u8 *)func_00144CB8(
                *(s32 *)(*(u32 *)((u8 *)state->pending + 4)) + 0x18, 50);
            COPY_SCENE_RECORD(state->recordB, src);
            state->flag = 1;
        }
    }

    func_0015C5A8(D_00392F88);
    func_0015C5A8(D_00392F8C);
    func_0015C5A8(D_00392F90);
    func_0015C5A8(D_00392F94);
    func_001123F8(8);
}

extern void func_00104020(s32);

void func_00104720(void)
{
    func_001497A0();
    func_00104020(4);
    func_00111950(4);
}

INCLUDE_ASM(void, "core/main", func_00104748);

extern float D_002536E8[4];

void func_00104A10(float arg1, float arg2, float arg3)
{
    D_002536E8[0] = 0.0f;
    if (D_002536E8[0] <= arg2)
        D_002536E8[1] = arg2;
    if (D_002536E8[0] <= arg3)
        D_002536E8[2] = arg3;
    return func_00106068();
}

INCLUDE_ASM(const s32, "core/main", func_00104A48);

INCLUDE_ASM(const s32, "core/main", func_00104AD0);

INCLUDE_ASM(const s32, "core/main", func_00104E30);

INCLUDE_ASM(const s32, "core/main", func_00104F98);

INCLUDE_ASM(const s32, "core/main", func_00105510);

extern void func_00112E98(s32 *buf, s32 count);

void func_00105768(u8 *arg0, u16 arg1)
{
    s32 buf[4];
    s32 value;

    *(s32 *)(arg0 + 0x8C) = arg1;
    func_00112E98(buf, arg1);
    value = buf[0];
    *(s32 *)(arg0 + 0x88) = value;
    *(s32 *)(arg0 + 0x90) = value;
}

INCLUDE_ASM(const s32, "core/main", func_001057A8);

INCLUDE_ASM(const s32, "core/main", func_00105A40);

INCLUDE_ASM(const s32, "core/main", func_00105CE8);

INCLUDE_ASM(const s32, "core/main", func_00105EB0);

s32 func_00105F40(s32 arg0)
{
    u8 i;

    for (i = 0; i < 30; i++)
    {
        if (arg0 == D_002536F8[i])
            return i;
    }
    return i;
}

INCLUDE_ASM(const s32, "core/main", func_00105F80);

INCLUDE_ASM(const s32, "core/main", func_00105FC8);

extern float D_00253A40[256];

void func_00106068(float arg1)
{
    int i = 255;

    do
    {
        D_00253A40[i] = arg1;
        i--;
    }
    while (i >= 0);
}

INCLUDE_ASM(const s32, "core/main", func_001060A0);

INCLUDE_ASM(const s32, "core/main", func_00106188);

s32 *func_00106278(s32 arg0)
{
    if (arg0 >= 3)
        return 0;
    if (arg0 != 0)
        return (s32 *)(D_0025236C[0] + (arg0 << 4) - 0x10);
    return 0;
}

extern s32 *func_00106278(s32);

s32 func_001062A8(s32 index)
{
    return *func_00106278(index);
}

void func_001062C8(s32 arg0, s32 arg1)
{
    s32 *p = func_00106278(arg0);

    *p = arg1;
    ((u8 *)p)[0xC] = 1;
}

INCLUDE_ASM(void, "core/main", func_001062F8);

INCLUDE_ASM(const s32, "core/main", func_00106380);

/* Kind-2 actors whose component carries a non-zero payload belong to the
 * script system and are skipped by the list walkers. */
s32 func_00106400(ActorNode *node)
{
    if (node->kind != 2) {
        return 0;
    }

    return node->component->value != 0;
}

INCLUDE_ASM(const s32, "core/main", func_00106428);

INCLUDE_ASM(const s32, "core/main", func_00106680);

void func_00106748(s32 *arg0)
{
    arg0[0] = func_00106770();
}

INCLUDE_ASM(const s32, "core/main", func_00106770);

INCLUDE_ASM(const s32, "core/main", func_00106868);

INCLUDE_ASM(const s32, "core/main", func_00106B98);

void func_00106C00(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00106C08);

INCLUDE_ASM(const s32, "core/main", func_00106D28);

INCLUDE_ASM(const s32, "core/main", func_00106DA0);

void func_00106E40(u8 *arg0, u8 arg1)
{
    arg0[0xF2] = arg1;
    if ((s8)arg0[0xF3] < arg1)
        arg0[0xF3] = 0;
}

/* Byte-matching.  Advances a byte counter, wrapping it back to zero when it reaches the limit. */
void func_00106E60(void *node)
{
    u8 *a = (u8 *)node;
    u8 *q;
    u8 *n;
    int v;

    if (a == 0)
        return;
    q = *(u8 **)(a + 4);
    n = *(u8 **)q;
    if (n == 0)
        return;
    v = n[0xF3] + 1;
    n[0xF3] = v;
    if (!((s8)v < n[0xF2]))
        n[0xF3] = 0;
}

s32 func_00106EA8(void *node)
{
    u8 *p;

    if (node == 0)
        return 1;
    p = *(u8 **)((u8 *)node + 4);
    p = *(u8 **)p;
    if (p == 0)
        return 1;
    return (s8)p[0xF3] == 0;
}

s32 func_00106ED8(u8 *arg0, s32 arg1, u8 *arg2)
{
    u8 mask = (1 << arg1) & 0xFF;

    if (arg2 != 0)
        *(Unaligned12 *)arg2 = *(Unaligned12 *)(arg0 + 0xE4);
    return arg0[0xF1] & mask;
}

/* Byte-matching.  Sets or clears bit arg1 in the flags at +0xF1 and, when arg3
   is given, copies its 12-byte record into +0xE4. */
void func_00106F10(u8 *arg0, s32 arg1, s32 arg2, Struct12 *arg3)
{
    u8 mask = 1 << arg1;

    if (arg2 != 0)
        *(u8 *)(arg0 + 0xF1) |= mask;
    else
        *(u8 *)(arg0 + 0xF1) &= ~mask;
    if (arg3 != 0)
        *(Struct12 *)(arg0 + 0xE4) = *arg3;
}

INCLUDE_ASM(const s32, "core/main", func_00106F60);

void func_00106FD0(u8 *arg0)
{
    s32 value = *(s32 *)(arg0 + 0x94);

    if (value != 0)
        func_0014CBE8(D_0039347C, value);
    *(s32 *)(arg0 + 0x94) = 0;
    *(s16 *)(arg0 + 0x7A) = 0;
}

void func_00107010(s32 *arg0)
{
    arg0[3] = func_00107038();
}

u8 *func_00107038(void)
{
    u8 *p = func_00157DE8(D_0039347C, 8);

    func_00107070(p);
    return p;
}

void func_00107070(u8 *arg0)
{
    *(s32 *)arg0 = func_001DD710();
    *(s32 *)(arg0 + 4) = (s32)0xCDCDCDCD;
}

INCLUDE_ASM(const s32, "core/main", func_001070A8);

void func_00107160(void) {
}

void func_00107168(u8 *a, u8 *b)
{
    func_001DE2F0(*(s32 *)(*(u32 *)(b + 0xC)), *(s32 *)(*(u32 *)(a + 0xC)));
}

INCLUDE_ASM(const s32, "core/main", func_00107180);

s32 func_00107288(void)
{
    return func_00157DE8(D_0039347C, 0x250);
}

INCLUDE_ASM(const s32, "core/main", func_001072A8);

s32 func_001074E0(void)
{
    return func_00157DE8(D_0039347C, 0x78);
}

INCLUDE_ASM(const s32, "core/main", func_00107500);

INCLUDE_ASM(const s32, "core/main", func_00107690);

INCLUDE_ASM(const s32, "core/main", func_00107760);

void func_001077F8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00107800);

INCLUDE_ASM(const s32, "core/main", func_001078E8);

INCLUDE_ASM(const s32, "core/main", func_001079E0);

INCLUDE_ASM(const s32, "core/main", func_00107AC8);

INCLUDE_ASM(const s32, "core/main", func_00107B98);

INCLUDE_ASM(const s32, "core/main", func_00107C60);

INCLUDE_ASM(const s32, "core/main", func_00107DB8);

INCLUDE_ASM(const s32, "core/main", func_00107F10);

/* Byte-matching.  Clears the byte at +0x10 of each 0x14-stride record in the
   table that starts at +0x17C, for the count held at +0x244. */
void func_00107F58(u8 *arg0)
{
    s32 n = *(s16 *)(arg0 + 0x244);
    u8 *p = arg0 + 0x17C;
    u8 *end = p + n * 0x14;

    while (p < end) {
        *(u8 *)(p + 0x10) = 0;
        p += 0x14;
    }
}

INCLUDE_ASM(const s32, "core/main", func_00107FA0);

INCLUDE_ASM(const s32, "core/main", func_00108058);

INCLUDE_ASM(const s32, "core/main", func_00108150);

INCLUDE_ASM(const s32, "core/main", func_001082A8);

void func_00108308(s32 *arg0)
{
    arg0[5] = func_00108330();
}

INCLUDE_ASM(const s32, "core/main", func_00108330);

INCLUDE_ASM(const s32, "core/main", func_00108398);

void func_00108428(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00108430);

INCLUDE_ASM(const s32, "core/main", func_001084E8);

INCLUDE_ASM(const s32, "core/main", func_00108568);

/* Byte-matching.  Returns the per-case field of arg1 for the masked selector. */
s32 func_001085B0(s32 arg0, u8 *arg1)
{
    switch (arg0 & 0xFF) {
    case 0:
        return *(s32 *)(arg1 + 0x10);
    case 2:
        return *(s32 *)(arg1 + 0x14);
    case 1:
        return *(s32 *)(arg1 + 0x18);
    case 3:
        return *(s32 *)(arg1 + 0x1C);
    }
    return 0;
}

s32 func_00108618(u8 *arg0)
{
    u8 *p = 0;

    if (arg0 != 0)
    {
        p = *(u8 **)(arg0 + 4);
        p = *(u8 **)(p + 0x14);
    }
    if (p == 0)
        return 0;
    return p[0x42] & 0x10;
}

s32 func_00108648(u8 *arg0)
{
    u8 *entry;

    if (arg0 == 0) {
        entry = 0;
    } else {
        entry = *(u8 **)(arg0 + 4);
        entry = *(u8 **)(entry + 0x14);
    }
    if (entry != 0)
        return entry[0x40];
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_00108670);

INCLUDE_ASM(const s32, "core/main", func_00108700);

INCLUDE_ASM(const s32, "core/main", func_00108760);

INCLUDE_ASM(const s32, "core/main", func_001087C8);

INCLUDE_ASM(const s32, "core/main", func_00108830);

INCLUDE_ASM(const s32, "core/main", func_00108890);

INCLUDE_ASM(const s32, "core/main", func_00108950);

INCLUDE_ASM(const s32, "core/main", func_001089F0);

s32 func_00108A38(void)
{
    return func_00157DE8(D_0039347C, 0x20);
}

INCLUDE_ASM(const s32, "core/main", func_00108A58);

INCLUDE_ASM(const s32, "core/main", func_00108C30);

INCLUDE_ASM(const s32, "core/main", func_00108CC0);

INCLUDE_ASM(const s32, "core/main", func_00108D20);

INCLUDE_ASM(const s32, "core/main", func_00108D70);

void func_00108F18(s32 *p) { func_0012A740(((s32 *)p)[7]); }

/* Actor reached through the node's animation state, 0 when it has none. */
void *func_00108F20(ActorNode *node)
{
    void *res = 0;
    ActorComponent *component = node->component;
    if (component != 0) {
        ActorAnimState *anim = component->anim;
        if (anim != 0) {
            res = anim->actor;
        }
    }
    return res;
}

u8 *func_00108F40(u8 *p) { return p + 44; }

extern void func_001192D0(void *, s32);
void func_00108F48(void *p, s32 val)
{
    func_001192D0(func_00108F40(p), val);
}

f32 func_00108F78(s32 arg0)
{
    s32 a;
    f32 b;

    func_00119668(func_00109438((const s32 *)arg0), &a, (s32 *)&b);
    return b;
}

void func_00108FA8(s32 *p, s32 value) { p[11] = value; }

f32 func_00108FB0(const f32 *p) { return p[7]; }

void func_00108FB8(f32 *p, f32 value) { p[10] = value; }

f32 func_00108FC0(const f32 *p) { return p[10]; }

f32 func_00108FC8(const f32 *p) { return p[8]; }

void func_00108FD0(f32 *p, f32 value) { p[8] = value; }

f32 func_00108FD8(const f32 *p) { return p[9]; }

void func_00108FE0(f32 *p, f32 x, f32 y, f32 z)
{
    p[1] = x;
    p[2] = y;
    p[3] = z;
}

f32 func_00108FF0(const f32 *p) { return p[1]; }

f32 func_00108FF8(const f32 *p) { return p[2]; }

f32 func_00109000(const f32 *p) { return p[3]; }

extern void *func_00109030(void);
void func_00109008(s32 *p)
{
    p[7] = (s32)func_00109030();
}

extern s32 D_0039347C;
extern void *func_00157DE8(s32, s32);
extern void func_00108FB8(f32 *, f32);
extern void func_00108FE0(f32 *, f32, f32, f32);
extern void func_00108F48(void *, s32);
extern s32 func_00146028(void);

void *func_00109030(void)
{
    float zero = 0.0f;
    void *p = func_00157DE8(D_0039347C, 0x34);
    func_00108FB8((f32 *)p, zero);
    func_00108FE0((f32 *)p, zero, zero, zero);
    func_00108F48(p, 1);
    *(s32 *)((u8 *)p + 0x30) = func_00146028();
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_001090A8);

void func_00109140(void) {}

void func_00109148(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00109150);

s32 func_00109248(const s32 *p) { return *(const s32 *)((const u8 *)p + (48)); }

void func_00109250(s32 *p, s32 value) { p[12] = value; }

void func_00109258(s32 arg0, u16 arg1)
{
    func_00119708((s16 *)func_00109438((const s32 *)arg0), arg1);
}

u8 func_00109288(s32 arg0)
{
    u8 out;

    func_00119508((const u8 *)func_00109438((const s32 *)arg0), (s32 *)&out);
    return out;
}

void func_001092B8(s32 arg0, u8 arg1)
{
    func_001194F8((u8 *)func_00109438((const s32 *)arg0), arg1);
}

void func_001092E8(s32 arg0, f32 arg1, f32 arg2)
{
    func_00119640((u8 *)func_00109438((const s32 *)arg0), arg1, arg2);
}

INCLUDE_ASM(const s32, "core/main", func_00109328);

INCLUDE_ASM(const s32, "core/main", func_00109380);

void func_001093D8(s32 arg0, s32 arg1)
{
    s32 value = func_00109438((const s32 *)arg0);

    func_00119518(value, arg1);
}

void func_00109408(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4)
{
    f32 values[4];

    values[0] = arg1;
    values[1] = arg2;
    values[2] = arg3;
    values[3] = arg4;
    func_001093D8(arg0, (s32)values);
}

s32 func_00109438(const s32 *p) { return *(const s32 *)((const u8 *)p + (44)); }

INCLUDE_ASM(const s32, "core/main", func_00109440);

u8 func_00109490(const u8 *p) { return p[0]; }

void func_00109498(u8 *p, u8 value) { p[1] = value; }

u8 func_001094A0(const u8 *p) { return p[1]; }

/* Stores a float at +0x1C and a byte at +0x2 of p. */
extern s32 D_00254008[];
extern s32 D_00252270[];
extern s32 D_002522EC[];
extern s32 D_0026E038[];
extern s32 D_00330E78[];
extern s32 D_003700D0[];
extern s32 D_00370238[];
extern s32 D_003E1A30[];
extern s32 D_00393310;
extern s32 D_0039331C;
extern s32 D_00393320;
extern s32 D_00393324;
extern s32 D_00393328;
extern s32 D_003933DC;
extern s32 D_00393544;
extern s32 D_003935DC;
extern s32 D_003935E4;
extern void func_00112708();
extern void func_00117900();
extern void func_00120D48();
extern void func_001C7658();
extern void func_001755B0();
extern void func_0015CCB8();
extern void func_0014EBA8();

extern s32 D_003910D0[];
extern s32 D_003910E8[];
extern s32 D_003912A8[];
extern s32 D_003912C0[];
extern s32 D_003731B8[];
extern s32 D_003732FC[];
extern s32 D_003724B4[];
extern f32 D_003731D0[];
extern f32 D_003731D4[];
extern s32 D_003937B0;
extern s32 D_003937A8;
extern s32 D_00393D00;
extern s32 D_00393D20;
extern s32 D_00393D30;
extern s32 D_0026F210[];
extern f32 D_0026F290[];
extern f32 D_0026F280[];
extern f32 D_00393258;
extern f32 D_0039325C;
extern f32 D_00393260;
extern f32 D_00393264;
extern f32 D_00393268;
extern s32 D_003D7D80[];
extern s32 D_003D7DE0[];
extern s32 D_003702D0[];
extern s32 D_003702D8[];
extern s32 D_00393C68;
extern void func_00118DA0();
extern void func_00224CB8();
extern void func_0015BAF0();
extern s32 func_0016C7F8(s32 value);
extern s32 D_003E7400[];
extern s32 D_003E72D8[];
extern s32 D_00383908[];
extern s32 D_00391290[];
extern s32 D_003912D8[];
extern s32 D_00386D18[];
extern s32 D_00330A18[];
extern s32 D_00258038[];
extern s32 D_003932D0;
extern s32 D_003932D4;
extern s32 D_003932D8;
extern s32 D_00393098;
extern s32 D_00393AC8;
extern void func_001DE818();
extern void func_001DEC30();
extern void func_0011AA40();
extern void func_0011E1E8();
extern void func_00139228();
extern void func_0010DBB8();
extern void func_00119518();
extern void func_0010DC28();
extern s32 func_0011BC68(void);
extern s32 func_0011D0C0(void);
extern s32 func_0014BAA0(void);
extern void func_0014C1B8();
extern void func_001456F0();
extern void func_0016EFA8();
extern s32 D_00393CCC;
extern s32 D_00393CD4;
extern s32 D_00393CDC;
extern s32 D_00393CE4;
extern s32 D_00393CF4;
extern s32 D_00393DC4;
extern s32 D_0039365C;
extern s32 D_00393990;
extern s32 D_00378C6C[];
extern u8 D_00393AE6;
extern u8 D_00393AE7;
extern void func_001C1420();
extern void func_001C1558();

void func_001094A8(u8 *p, s32 value, float f)
{
    p[0x2] = value;
    *(f32 *)(p + 0x1C) = f;
}

u8 func_001094B8(const u8 *p) { return p[2]; }

void func_001094C0(u8 *p, s32 value, float a, float b)
{
    p[0x3] = value;
    *(f32 *)(p + 0x20) = a;
    *(f32 *)(p + 0x24) = b;
}

u8 func_001094D0(const u8 *p) { return p[3]; }

u8 *func_001094D8(u8 *p) { return p + 4; }

void func_001094E0(f32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4)
{
    *arg2 = arg1[4];
    *arg3 = arg1[5];
    *arg4 = arg1[6];
}

INCLUDE_ASM(const s32, "core/main", func_00109500);

INCLUDE_ASM(const s32, "core/main", func_001095D8);

INCLUDE_ASM(const s32, "core/main", func_00109AC0);

INCLUDE_ASM(const s32, "core/main", func_00109B20);

u8 *func_00109BA8(void)
{
    u8 *p = func_00157DE8(D_0039347C, 8);

    *(s32 *)p = func_00149738();
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00109BE0);

void func_00109C30(void) {
}

/* Unaligned quadword copied through the pointer fields at +0x24. */

extern s32 D_00392FA8;

extern s32 D_00392FA4;
extern s32 D_00392FA0;
extern s32 D_0034A490[];
extern s32 D_0034A48C[];
extern s32 D_0034A494[];
extern void func_0015CF28(u8 *p, s32 value);
extern s32 D_00393C00;
extern u8 D_003D7E10[];
extern s32 D_003938E4;
extern s32 D_003939C4;
extern u16 D_00383900[];
extern u8 D_00378BC8[];
extern s32 D_00391278[];
extern s32 D_00391428[];
extern s32 D_00391418[];
extern s32 D_00391408[];
extern s32 D_003913F8[];


void func_00109C38(s32 *arg1, s32 *arg2)
{
    *(UnalignedU64 *)(void *)arg1[9] = *(UnalignedU64 *)(void *)arg2[9];
}

u8 func_00109C58(const u8 *p) { return p[4]; }

s32 func_00109C60(s32 arg0)
{
    u8 *p = (u8 *)D_00254008[0];

    while (p != 0)
    {
        if (*(s32 *)(p + 0xC) == arg0)
            return *(s32 *)(p + 0x10);
        p = *(u8 **)p;
    }
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_00109C98);

INCLUDE_ASM(const s32, "core/main", func_00109D18);

INCLUDE_ASM(const s32, "core/main", func_00109D98);

INCLUDE_ASM(const s32, "core/main", func_00109E18);

INCLUDE_ASM(const s32, "core/main", func_00109F00);

INCLUDE_ASM(const s32, "core/main", func_00109FE8);

INCLUDE_ASM(const s32, "core/main", func_0010A0D0);

INCLUDE_ASM(const s32, "core/main", func_0010A198);

INCLUDE_ASM(const s32, "core/main", func_0010A260);

INCLUDE_ASM(const s32, "core/main", func_0010A328);

INCLUDE_ASM(const s32, "core/main", func_0010A3F0);

INCLUDE_ASM(const s32, "core/main", func_0010A4B8);

s32 func_0010A580(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_0010A588);

INCLUDE_ASM(const s32, "core/main", func_0010A6A0);

INCLUDE_ASM(const s32, "core/main", func_0010A778);

extern f32 D_003930A0;
extern f32 D_0039309C;
extern s32 D_003930A4;

s32 func_0010A858(void)
{
    s32 v = D_00254008[0];
    if (v != 0) {
        return *(s32 *)(v + 0x10);
    }
    return 0;
}

void func_0010A878(u8 *p)
{
    *(s32 *)(*(u32 *)D_00254008 + 0x10) = *(s32 *)(p + 0x4);
}

void func_0010A890(s32 *arg0)
{
    arg0[10] = func_0010A8B8();
}

s32 func_0010A8B8(void)
{
    return func_00157DE8(D_0039347C, 0x8);
}

void func_0010A8D8(s32 *arg1, s32 arg2)
{
    if ((arg2 & 0xFF) == 1) {
        func_0010A978((void *)arg1[10]);
    }
}

void func_0010A8F8(void) {
}

/* Unaligned quadword copied through the pointer fields at +0x28. */
void func_0010A900(s32 *arg1, s32 *arg2)
{
    *(UnalignedU64 *)(void *)arg1[10] = *(UnalignedU64 *)(void *)arg2[10];
}

s32 func_0010A920(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

void func_0010A928(s32 *p, s32 value) { p[0] = value; }

s32 func_0010A930(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

void func_0010A938(s32 *p, s32 value) { p[1] = value; }

s32 func_0010A940(s32 arg0, s32 *arg1)
{
    u8 *p;
    u8 *node;

    if ((u32)(arg0 + 1) < 2)
        return 0;
    p = *(u8 **)(arg0 + 4);
    node = *(u8 **)(p + 0x28);
    if (node == 0)
        return 0;
    arg1[0] = *(s32 *)node;
    arg1[1] = *(s32 *)(node + 4);
    return 1;
}

void func_0010A978(void) {
}

void func_0010A980(s32 *arg0)
{
    arg0[6] = func_0010A9A8();
}

u8 *func_0010A9A8(void)
{
    u8 *p = func_00157DE8(D_0039347C, 0xC);

    func_0010AA48(p);
    return p;
}

void func_0010A9E0(s32 *p) { func_0010AA48(((s32 *)p)[6]); }

void func_0010A9E8(s32 *p) { func_0010AA50(((s32 *)p)[6]); }

INCLUDE_ASM(const s32, "core/main", func_0010A9F0);

void func_0010AA48(void) { func_0010AA50(); }

/* Byte-matching.  Releases the id held at +0 when there is one, then clears the
   record: state byte at +8, the -1 at +4 and the id itself. */
void func_0010AA50(u8 *arg0)
{
    void func_001DFD08(s32 id);
    s32 id;

    if (arg0 == 0)
        return;
    id = *(s32 *)arg0;
    if (id != 0)
        func_001DFD08(id);
    *(s32 *)arg0 = 0;
    *(s32 *)(arg0 + 4) = -1;
    *(u8 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0010AA98);

s32 func_0010AB00(s32 *arg1)
{
    s32 result = -1;

    if (arg1 != 0) {
        s32 value = arg1[0];

        if (value != 0) {
            result = *(s32 *)(value + 8);
        }
    }
    return result;
}

s32 func_0010AB20(u8 *arg0)
{
    if (arg0 != 0 && *(s32 *)arg0 != 0)
        return *(s32 *)(arg0 + 4);
    return -1;
}

s32 func_0010AB48(s32 **arg0)
{
    s32 *entry;

    if (arg0 != 0 && (entry = *arg0) != 0)
        return entry[2] - 1;
    return -1;
}

s32 func_0010AB70(s32 *p)
{
    s32 result = -1;

    if (p != 0 && *p != 0)
        return 0;
    return result;
}

INCLUDE_ASM(const s32, "core/main", func_0010AB88);

INCLUDE_ASM(const s32, "core/main", func_0010AC00);

INCLUDE_ASM(const s32, "core/main", func_0010AC50);

INCLUDE_ASM(const s32, "core/main", func_0010ACD0);

INCLUDE_ASM(const s32, "core/main", func_0010AD70);

INCLUDE_ASM(const s32, "core/main", func_0010AE38);

INCLUDE_ASM(const s32, "core/main", func_0010AF18);

INCLUDE_ASM(const s32, "core/main", func_0010AFE0);

void func_0010B0B0(s32 *arg0)
{
    arg0[8] = func_0010B0D8();
}

u8 *func_0010B0D8(void)
{
    u8 *p = func_00157DE8(D_0039347C, 0x14);

    *(s32 *)(p + 4) = func_00145FF0();
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0010B110);

void func_0010B258(u8 *arg0, s32 arg1)
{
    u8 *p;
    u8 *q;

    arg1 &= 0xFF;
    if (arg1 >= 6)
        return;
    if (arg1 == 0)
        return;
    p = *(u8 **)(arg0 + 4);
    q = *(u8 **)(p + 0x18);
    if (q == 0)
        return;
    func_0010B498(q);
}

void func_0010B290(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0010B298);

INCLUDE_ASM(const s32, "core/main", func_0010B340);

INCLUDE_ASM(const s32, "core/main", func_0010B3D8);

INCLUDE_ASM(const s32, "core/main", func_0010B498);

s32 func_0010B4F0(u8 *arg0)
{
    u8 *p;

    if (*(s32 *)arg0 != 2)
        return 0;
    p = *(u8 **)(arg0 + 4);
    p = *(u8 **)(p + 0x20);
    if (p == 0)
        return 0;
    return *(s32 *)p;
}

INCLUDE_ASM(const s32, "core/main", func_0010B520);

INCLUDE_ASM(const s32, "core/main", func_0010B5A0);

void func_0010B828(void)
{
    D_00392FD8 = -1;
    func_0010BF80();
    func_0010C028();
    func_0010B8A8();
}

s32 func_0010B850(void) { return D_00392FD8 != -1; }

INCLUDE_ASM(const s32, "core/main", func_0010B860);

INCLUDE_ASM(const s32, "core/main", func_0010B8A8);

INCLUDE_ASM(const s32, "core/main", func_0010B950);

INCLUDE_ASM(const s32, "core/main", func_0010BA78);

INCLUDE_ASM(const s32, "core/main", func_0010BAC8);

INCLUDE_ASM(const s32, "core/main", func_0010BBC8);

INCLUDE_ASM(const s32, "core/main", func_0010BC08);

INCLUDE_ASM(const s32, "core/main", func_0010BF80);

INCLUDE_ASM(const s32, "core/main", func_0010C028);

INCLUDE_ASM(const s32, "core/main", func_0010C0D8);

INCLUDE_ASM(const s32, "core/main", func_0010C480);

void func_0010C568(u8 *arg0)
{
    s32 value = func_0010C598();

    *(s32 *)(arg0 + 4) = value;
    func_0010C8C8(value);
}

INCLUDE_ASM(const s32, "core/main", func_0010C598);

INCLUDE_ASM(const s32, "core/main", func_0010C648);

void func_0010C898(s32 *arg1, s32 arg2)
{
    if ((arg2 & 0xFF) < 6 && (arg2 & 0xFF) != 0) {
        u8 *p = (u8 *)arg1[1];

        p[0x34] &= 0xFD;
    }
}

void func_0010C8C8(u8 *arg0)
{
    arg0[0x40] = arg0[0x29];
    arg0[0x41] = arg0[0x2A];
    *(s32 *)(arg0 + 0x38) = *(s32 *)(arg0 + 0x2C);
    *(s32 *)(arg0 + 0x3C) = *(s32 *)(arg0 + 0x30);
    arg0[0x42] = arg0[0x34] & 7;
}

void func_0010C8F8(void) {
}

void func_0010C900(void) {
}

void func_0010C908(void) {
}

void func_0010C910(void) {
}

u8 func_0010C918(s32 *p) { u8 *object = (u8 *)p[1]; u8 value = object[0x40]; object[0x29] = value; return value; }

u8 func_0010C928(s32 *p) { u8 *object = (u8 *)p[1]; u8 value = object[0x41]; object[0x2A] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_0010C938);

s32 func_0010C9F0(p) s32 *p; { return ((s32 *)((s32 *)p[1])[1])[0x2C / 4]; }

void func_0010CA00(s32 *p, s32 value) { ((s32 *)((s32 *)p[1])[1])[0x2C / 4] = value; }

s32 func_0010CA10(u8 *p)
{
    s32 v = *(s32 *)(*(s32 *)(p + 4) + 4);
    if (v != 0) {
        return *(u8 *)(v + 0x35);
    }
    return 0x46;
}

s32 func_0010CA30(u8 *p)
{
    s32 v = *(s32 *)(*(s32 *)(p + 4) + 4);
    if (v != 0) {
        return *(u8 *)(v + 0x36);
    }
    return 0x50;
}

u8 func_0010CA50(s32 *p) { return ((u8 *)((s32 *)p[1])[1])[0xD]; }

s32 func_0010CA60(s32 *p) { return ((s32 *)p[1])[1] != 0; }

void func_0010CA70(void)
{
    u8 *p = D_00254028;

    p[0] = 0;
    p[1] = 0xFF;
    *(u16 *)(p + 2) = 0;
    *(s32 *)(p + 4) = 0;
    *(s32 *)(p + 8) = 0;
    *(s32 *)(p + 0xC) = 0;
}

void func_0010CA98(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0010CAA0);

INCLUDE_ASM(const s32, "core/main", func_0010CB00);

INCLUDE_ASM(const s32, "core/main", func_0010CB60);