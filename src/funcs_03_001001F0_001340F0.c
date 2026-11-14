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

    func_0021AE88();
    func_00224B88(thread_context, 48, 0);
    thread_context[1] = func_00100288;
    thread_context[2] = (u32*)&D_00393E20;
    thread_context[3] = 2048;
    thread_context[5] = 63;
    thread_context[4] = D_0039A3F0[0];

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
extern void func_00144C90(s32 mode);
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

INCLUDE_ASM(const s32, "core/main", func_00103048);

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

INCLUDE_ASM(const s32, "core/main", func_001040F0);

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

INCLUDE_ASM(const s32, "core/main", func_00105768);

INCLUDE_ASM(const s32, "core/main", func_001057A8);

INCLUDE_ASM(const s32, "core/main", func_00105A40);

INCLUDE_ASM(const s32, "core/main", func_00105CE8);

INCLUDE_ASM(const s32, "core/main", func_00105EB0);

INCLUDE_ASM(const s32, "core/main", func_00105F40);

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

INCLUDE_ASM(s32 *, "core/main", func_00106278);

extern s32 *func_00106278(s32);

s32 func_001062A8(s32 index)
{
    return *func_00106278(index);
}

INCLUDE_ASM(const s32, "core/main", func_001062C8);

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

INCLUDE_ASM(const s32, "core/main", func_00106748);

INCLUDE_ASM(const s32, "core/main", func_00106770);

INCLUDE_ASM(const s32, "core/main", func_00106868);

INCLUDE_ASM(const s32, "core/main", func_00106B98);

void func_00106C00(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00106C08);

INCLUDE_ASM(const s32, "core/main", func_00106D28);

INCLUDE_ASM(const s32, "core/main", func_00106DA0);

INCLUDE_ASM(const s32, "core/main", func_00106E40);

INCLUDE_ASM(const s32, "core/main", func_00106E60);

INCLUDE_ASM(const s32, "core/main", func_00106EA8);

INCLUDE_ASM(const s32, "core/main", func_00106ED8);

INCLUDE_ASM(const s32, "core/main", func_00106F10);

INCLUDE_ASM(const s32, "core/main", func_00106F60);

INCLUDE_ASM(const s32, "core/main", func_00106FD0);

INCLUDE_ASM(const s32, "core/main", func_00107010);

INCLUDE_ASM(const s32, "core/main", func_00107038);

INCLUDE_ASM(const s32, "core/main", func_00107070);

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

INCLUDE_ASM(const s32, "core/main", func_00107F58);

INCLUDE_ASM(const s32, "core/main", func_00107FA0);

INCLUDE_ASM(const s32, "core/main", func_00108058);

INCLUDE_ASM(const s32, "core/main", func_00108150);

INCLUDE_ASM(const s32, "core/main", func_001082A8);

INCLUDE_ASM(const s32, "core/main", func_00108308);

INCLUDE_ASM(const s32, "core/main", func_00108330);

INCLUDE_ASM(const s32, "core/main", func_00108398);

void func_00108428(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00108430);

INCLUDE_ASM(const s32, "core/main", func_001084E8);

INCLUDE_ASM(const s32, "core/main", func_00108568);

INCLUDE_ASM(const s32, "core/main", func_001085B0);

INCLUDE_ASM(const s32, "core/main", func_00108618);

INCLUDE_ASM(const s32, "core/main", func_00108648);

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

INCLUDE_ASM(const s32, "core/main", func_00108F78);

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

INCLUDE_ASM(const s32, "core/main", func_00109258);

INCLUDE_ASM(const s32, "core/main", func_00109288);

INCLUDE_ASM(const s32, "core/main", func_001092B8);

INCLUDE_ASM(const s32, "core/main", func_001092E8);

INCLUDE_ASM(const s32, "core/main", func_00109328);

INCLUDE_ASM(const s32, "core/main", func_00109380);

INCLUDE_ASM(const s32, "core/main", func_001093D8);

INCLUDE_ASM(const s32, "core/main", func_00109408);

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
extern void func_0010DC28();
extern void func_0011BC68();
extern void func_0011D0C0();
extern void func_0014BAA0();
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

INCLUDE_ASM(const s32, "core/main", func_00109BA8);

INCLUDE_ASM(const s32, "core/main", func_00109BE0);

void func_00109C30(void) {
}

/* Unaligned quadword copied through the pointer fields at +0x24. */
typedef struct __attribute__((packed)) {
    u64 value;
} UnalignedU64;

extern s32 D_00392FA8;
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

INCLUDE_ASM(const s32, "core/main", func_00109C60);

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

INCLUDE_ASM(const s32, "core/main", func_0010A890);

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

INCLUDE_ASM(const s32, "core/main", func_0010A940);

void func_0010A978(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0010A980);

INCLUDE_ASM(const s32, "core/main", func_0010A9A8);

void func_0010A9E0(s32 *p) { func_0010AA48(((s32 *)p)[6]); }

void func_0010A9E8(s32 *p) { func_0010AA50(((s32 *)p)[6]); }

INCLUDE_ASM(const s32, "core/main", func_0010A9F0);

void func_0010AA48(void) { func_0010AA50(); }

INCLUDE_ASM(const s32, "core/main", func_0010AA50);

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

INCLUDE_ASM(const s32, "core/main", func_0010AB20);

INCLUDE_ASM(const s32, "core/main", func_0010AB48);

s32 func_0010AB70(s32 *p)
{
    s32 result = -1;

    if (p != 0 && *p != 0)
        result = 0;
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

INCLUDE_ASM(const s32, "core/main", func_0010B0B0);

INCLUDE_ASM(const s32, "core/main", func_0010B0D8);

INCLUDE_ASM(const s32, "core/main", func_0010B110);

INCLUDE_ASM(const s32, "core/main", func_0010B258);

void func_0010B290(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0010B298);

INCLUDE_ASM(const s32, "core/main", func_0010B340);

INCLUDE_ASM(const s32, "core/main", func_0010B3D8);

INCLUDE_ASM(const s32, "core/main", func_0010B498);

INCLUDE_ASM(const s32, "core/main", func_0010B4F0);

INCLUDE_ASM(const s32, "core/main", func_0010B520);

INCLUDE_ASM(const s32, "core/main", func_0010B5A0);

INCLUDE_ASM(const s32, "core/main", func_0010B828);

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

INCLUDE_ASM(const s32, "core/main", func_0010C568);

INCLUDE_ASM(const s32, "core/main", func_0010C598);

INCLUDE_ASM(const s32, "core/main", func_0010C648);

void func_0010C898(s32 *arg1, s32 arg2)
{
    if ((arg2 & 0xFF) < 6 && (arg2 & 0xFF) != 0) {
        u8 *p = (u8 *)arg1[1];

        p[0x34] &= 0xFD;
    }
}

INCLUDE_ASM(const s32, "core/main", func_0010C8C8);

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

INCLUDE_ASM(const s32, "core/main", func_0010CA70);

void func_0010CA98(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0010CAA0);

INCLUDE_ASM(const s32, "core/main", func_0010CB00);

INCLUDE_ASM(const s32, "core/main", func_0010CB60);

INCLUDE_ASM(const s32, "core/main", func_0010CC30);

INCLUDE_ASM(const s32, "core/main", func_0010D408);

INCLUDE_ASM(const s32, "core/main", func_0010D5C8);

INCLUDE_ASM(const s32, "core/main", func_0010DA20);

INCLUDE_ASM(const s32, "core/main", func_0010DBB8);

INCLUDE_ASM(const s32, "core/main", func_0010DC00);

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

INCLUDE_ASM(const s32, "core/main", func_0010DCE8);

INCLUDE_ASM(const s32, "core/main", func_0010DD18);

INCLUDE_ASM(const s32, "core/main", func_0010DE30);

INCLUDE_ASM(const s32, "core/main", func_0010DE70);

INCLUDE_ASM(const s32, "core/main", func_0010DED0);

INCLUDE_ASM(const s32, "core/main", func_0010E638);

INCLUDE_ASM(const s32, "core/main", func_0010E8B0);

INCLUDE_ASM(const s32, "core/main", func_0010E920);

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

INCLUDE_ASM(const s32, "core/main", func_0010F2E8);

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


INCLUDE_ASM(const s32, "core/main", func_00110340);

INCLUDE_ASM(const s32, "core/main", func_00110398);

INCLUDE_ASM(const s32, "core/main", func_001104A0);

INCLUDE_ASM(const s32, "core/main", func_001106D0);

INCLUDE_ASM(const s32, "core/main", func_001108F8);

INCLUDE_ASM(const s32, "core/main", func_00110950);

void func_001109D8(void) { func_00115AD8(); }

INCLUDE_ASM(const s32, "core/main", func_001109E0);

INCLUDE_ASM(const s32, "core/main", func_00110E88);

INCLUDE_ASM(const s32, "core/main", func_00110EC0);

INCLUDE_ASM(const s32, "core/main", func_00110EE0);

INCLUDE_ASM(const s32, "core/main", func_00110F00);


/* Source: player/rayman_init.c */
/* func_00110F20 through func_00110F20; retail address range 0x00110F20-0x00110F78. */


INCLUDE_ASM(const s32, "core/main", func_00110F20);


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

INCLUDE_ASM(const s32, "core/main", func_00110F78);

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
extern void func_00117AD8();
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

INCLUDE_ASM(const s32, "core/main", func_00111018);

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

INCLUDE_ASM(const s32, "core/main", func_00111128);

INCLUDE_ASM(const s32, "core/main", func_001111C0);

INCLUDE_ASM(const s32, "core/main", func_001111F8);

INCLUDE_ASM(const s32, "core/main", func_001112C8);

INCLUDE_ASM(const s32, "core/main", func_00111330);

INCLUDE_ASM(const s32, "core/main", func_00111358);

INCLUDE_ASM(const s32, "core/main", func_001113F0);

INCLUDE_ASM(const s32, "core/main", func_00111428);

INCLUDE_ASM(const s32, "core/main", func_00111470);

s32 func_001114A0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_001114A8);

INCLUDE_ASM(const s32, "core/main", func_001114E0);

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

INCLUDE_ASM(const s32, "core/main", func_001118D0);

INCLUDE_ASM(const s32, "core/main", func_00111910);

INCLUDE_ASM(void, "core/main", func_00111950);

INCLUDE_ASM(const s32, "core/main", func_001119A0);

INCLUDE_ASM(const s32, "core/main", func_001119F8);

INCLUDE_ASM(const s32, "core/main", func_00111FC8);

INCLUDE_ASM(void, "core/main", func_00112120);

INCLUDE_ASM(const s32, "core/main", func_00112200);

INCLUDE_ASM(const s32, "core/main", func_00112270);

INCLUDE_ASM(const s32, "core/main", func_00112370);

INCLUDE_ASM(const s32, "core/main", func_001123B0);

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

INCLUDE_ASM(const s32, "core/main", func_001125B0);

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

INCLUDE_ASM(const s32, "core/main", func_00112640);

INCLUDE_ASM(const s32, "core/main", func_00112680);

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

INCLUDE_ASM(const s32, "core/main", func_00112B10);

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

INCLUDE_ASM(const s32, "core/main", func_00112D28);

INCLUDE_ASM(const s32, "core/main", func_00112D80);

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

INCLUDE_ASM(const s32, "core/main", func_00112E48);

INCLUDE_ASM(const s32, "core/main", func_00112E70);

INCLUDE_ASM(const s32, "core/main", func_00112E98);

INCLUDE_ASM(const s32, "core/main", func_00112F10);

INCLUDE_ASM(const s32, "core/main", func_00112F88);

INCLUDE_ASM(const s32, "core/main", func_00112FF8);

INCLUDE_ASM(const s32, "core/main", func_00113030);

INCLUDE_ASM(const s32, "core/main", func_001130F0);

s32 func_00113120();

s32 func_00113120(s32 arg1)
{
    return func_0014CBE8(D_0039347C, arg1);
}

INCLUDE_ASM(const s32, "core/main", func_00113148);

INCLUDE_ASM(const s32, "core/main", func_00113198);

INCLUDE_ASM(const s32, "core/main", func_00113280);

INCLUDE_ASM(const s32, "core/main", func_001134C8);

INCLUDE_ASM(const s32, "core/main", func_00113548);

INCLUDE_ASM(const s32, "core/main", func_00113760);

INCLUDE_ASM(void, "core/main", func_00113798);

s32 func_00113838(const s32 *p) { return *(const s32 *)((const u8 *)p + (32)); }

s32 func_00113840(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_00113848(u8 *p, s32 index)
{
    return *(s32 *)(p + 0x18) + (s16)index * 0xC;
}

INCLUDE_ASM(const s32, "core/main", func_00113868);

void func_001138A0(void) {
}

s32 func_001138A8(s32 *p, s32 value) { p[6] = value; p[7] = 1; return 1; }

INCLUDE_ASM(const s32, "core/main", func_001138B8);

INCLUDE_ASM(const s32, "core/main", func_00113998);

u16 func_001139E8(const u16 *p) { return p[4]; }

INCLUDE_ASM(const s32, "core/main", func_001139F0);

void func_00113A70(u8 *p)
{
    func_002249D4(*(s32 *)p, *(s32 *)(p + 0x4), *(u16 *)(p + 0x8) * 0x14);
}

s32 func_00113A90(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

INCLUDE_ASM(const s32, "core/main", func_00113A98);

INCLUDE_ASM(const s32, "core/main", func_00113AD8);

INCLUDE_ASM(const s32, "core/main", func_00113B28);

INCLUDE_ASM(const s32, "core/main", func_00113C68);

INCLUDE_ASM(const s32, "core/main", func_00113DF8);

INCLUDE_ASM(const s32, "core/main", func_00113F80);

INCLUDE_ASM(const s32, "core/main", func_00114110);

INCLUDE_ASM(const s32, "core/main", func_00114208);

INCLUDE_ASM(const s32, "core/main", func_001143C8);

INCLUDE_ASM(const s32, "core/main", func_00114448);

INCLUDE_ASM(const s32, "core/main", func_00114480);

s32 func_001145D8(const u16 *left, const u16 *right) { return left[2] - right[2]; }

void func_001145E8(u8 *p)
{
    func_00224CB8(*(s32 *)p, *(s32 *)(p + 0x4), 0x8, (s32)func_001145D8);
}

INCLUDE_ASM(const s32, "core/main", func_00114600);

u16 func_00114670(const u16 *p) { return p[0]; }

INCLUDE_ASM(const s32, "core/main", func_00114678);

s32 func_001146B8(void)
{
    return func_00157DE8(D_0039347C, 0x8);
}

INCLUDE_ASM(const s32, "core/main", func_001146D8);

INCLUDE_ASM(const s32, "core/main", func_00114768);

INCLUDE_ASM(const s32, "core/main", func_001148F0);

INCLUDE_ASM(const s32, "core/main", func_001149A0);

INCLUDE_ASM(const s32, "core/main", func_00114CE8);

INCLUDE_ASM(const s32, "core/main", func_00114D38);

INCLUDE_ASM(const s32, "core/main", func_00114D98);

INCLUDE_ASM(const s32, "core/main", func_00114E28);

INCLUDE_ASM(const s32, "core/main", func_00114FF0);

INCLUDE_ASM(const s32, "core/main", func_00115290);

INCLUDE_ASM(const s32, "core/main", func_001152E0);

INCLUDE_ASM(const s32, "core/main", func_00115438);

INCLUDE_ASM(const s32, "core/main", func_00115620);

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

INCLUDE_ASM(const s32, "core/main", func_00115900);

INCLUDE_ASM(const s32, "core/main", func_00115940);

INCLUDE_ASM(const s32, "core/main", func_00115978);

INCLUDE_ASM(const s32, "core/main", func_001159A0);

INCLUDE_ASM(const s32, "core/main", func_001159D0);

INCLUDE_ASM(const s32, "core/main", func_00115AA8);

INCLUDE_ASM(const s32, "core/main", func_00115AD8);

INCLUDE_ASM(const s32, "core/main", func_00115BE0);

INCLUDE_ASM(const s32, "core/main", func_00115C38);

INCLUDE_ASM(const s32, "core/main", func_00115C60);

INCLUDE_ASM(const s32, "core/main", func_00115D10);

f32 func_00115FA0(void) { return D_00393048; }

void func_00115FA8(f32 value) { D_00393048 = value; }

f32 func_00115FB0(void) { return D_0039304C; }

INCLUDE_ASM(const s32, "core/main", func_00115FB8);

f32 func_00115FF0(void) { return D_00393054; }

INCLUDE_ASM(const s32, "core/main", func_00115FF8);

INCLUDE_ASM(const s32, "core/main", func_00116308);

INCLUDE_ASM(const s32, "core/main", func_001163F0);

INCLUDE_ASM(const s32, "core/main", func_00116470);

void func_00116A90(void)
{
    func_00117AD8(0, 0);
}

INCLUDE_ASM(const s32, "core/main", func_00116AA0);

INCLUDE_ASM(const s32, "core/main", func_00116AD0);

INCLUDE_ASM(const s32, "core/main", func_00116D58);

INCLUDE_ASM(const s32, "core/main", func_00116DB0);

INCLUDE_ASM(const s32, "core/main", func_00116E28);

s32 func_00116E58(s32 index)
{
    return *(s32 *)((u8 *)D_00258038 + (s16)index * 4);
}

INCLUDE_ASM(const s32, "core/main", func_00116E78);

INCLUDE_ASM(const s32, "core/main", func_00116EA0);

INCLUDE_ASM(const s32, "core/main", func_00116ED0);

INCLUDE_ASM(const s32, "core/main", func_00116F00);

INCLUDE_ASM(const s32, "core/main", func_00117000);

INCLUDE_ASM(const s32, "core/main", func_00117078);

INCLUDE_ASM(const s32, "core/main", func_00117350);

s64 func_001174A0(void)
{
  return func_0011BBE8();
}

void func_001174C0(void) { func_0011BC68(); }

INCLUDE_ASM(const s32, "core/main", func_001174E0);

INCLUDE_ASM(const s32, "core/main", func_00117658);

s64 func_001176A0(void)
{
  func_0011C5A8();
  return 1;
}

INCLUDE_ASM(const s32, "core/main", func_001176C0);

INCLUDE_ASM(const s32, "core/main", func_00117900);

INCLUDE_ASM(const s32, "core/main", func_001179A0);

INCLUDE_ASM(const s32, "core/main", func_00117A60);

INCLUDE_ASM(const s32, "core/main", func_00117AD8);

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

INCLUDE_ASM(const s32, "core/main", func_00118B88);

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

INCLUDE_ASM(const s32, "core/main", func_00118E18);

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

INCLUDE_ASM(const s32, "core/main", func_00119230);

void func_00119260(s32 unused, s32 value)
{
    D_00393098 -= value;
    if (D_00393098 < 0)
        D_00393098 = 0;
}

void func_00119280(void) { D_00393098 = 0; }

INCLUDE_ASM(const s32, "core/main", func_00119288);

INCLUDE_ASM(const s32, "core/main", func_001192D0);

INCLUDE_ASM(const s32, "core/main", func_001193B0);

INCLUDE_ASM(const s32, "core/main", func_00119460);

void func_001194F8(u8 *p, u32 value)
{
    p[0] = value != 0;
}

s32 func_00119508(const u8 *source, s32 *destination) { s32 value = *source != 0; *destination = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_00119518);

INCLUDE_ASM(const s32, "core/main", func_00119548);

INCLUDE_ASM(const s32, "core/main", func_00119570);

INCLUDE_ASM(const s32, "core/main", func_00119640);

void func_00119668(u8 *p, float *a, float *b)
{
    *a = *(f32 *)(p + 0x8);
    *b = *(f32 *)(p + 0x4);
}

INCLUDE_ASM(const s32, "core/main", func_00119680);

void func_001196E8(u8 *p, float *a, float *b)
{
    *a = *(f32 *)(p + 0xC);
    *b = *(f32 *)(p + 0x10);
}

u16 func_00119700(const u16 *p) { return p[1]; }

void func_00119708(s16 *p, s16 value) { p[1] = value; }

void func_00119710(u8 *p)
{
    p[0x86] = 1;
}

void func_00119720(u8 *p, u8 value) { p[135] = value; }

void func_00119728(u8 *p)
{
    p[0x87] = 1;
}

INCLUDE_ASM(const s32, "core/main", func_00119738);

INCLUDE_ASM(const s32, "core/main", func_00119760);

void func_00119788(u8 *p, float a, float b)
{
    *(f32 *)(p + 0xC8) = a;
    *(f32 *)(p + 0xCC) = b;
}

INCLUDE_ASM(const s32, "core/main", func_00119798);

INCLUDE_ASM(const s32, "core/main", func_001197E8);

INCLUDE_ASM(const s32, "core/main", func_00119838);

INCLUDE_ASM(const s32, "core/main", func_00119888);

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

INCLUDE_ASM(const s32, "core/main", func_00119958);

INCLUDE_ASM(const s32, "core/main", func_00119980);

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

INCLUDE_ASM(const s32, "core/main", func_00119E48);

s32 func_00119E78(u32 *p)
{
    return (((s32)*p >> 26) ^ 1) & 1;
}

INCLUDE_ASM(const s32, "core/main", func_00119E90);

void func_00119F30(u8 *src, s32 *dst)
{
    *dst = *(s32 *)(src + 0x28);
}

INCLUDE_ASM(const s32, "core/main", func_00119F40);

INCLUDE_ASM(const s32, "core/main", func_00119F90);

INCLUDE_ASM(const s32, "core/main", func_00119FD8);

INCLUDE_ASM(const s32, "core/main", func_0011A020);

INCLUDE_ASM(const s32, "core/main", func_0011A0A8);

INCLUDE_ASM(const s32, "core/main", func_0011A0F0);

u16 func_0011A1E0(const u16 *p) { return p[18]; }

INCLUDE_ASM(const s32, "core/main", func_0011A1E8);

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

INCLUDE_ASM(const s32, "core/main", func_0011A518);

INCLUDE_ASM(const s32, "core/main", func_0011A5A0);

INCLUDE_ASM(const s32, "core/main", func_0011A630);

INCLUDE_ASM(const s32, "core/main", func_0011A660);

void func_0011A778(void)
{
    D_00393094 = 0;
    D_00393090 = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0011A788);

INCLUDE_ASM(const s32, "core/main", func_0011A7E0);

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

INCLUDE_ASM(const s32, "core/main", func_0011B0E8);

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

INCLUDE_ASM(const s32, "core/main", func_0011B320);

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

INCLUDE_ASM(const s32, "core/main", func_0011BB78);

INCLUDE_ASM(const s32, "core/main", func_0011BBE8);

INCLUDE_ASM(const s32, "core/main", func_0011BC68);

INCLUDE_ASM(const s32, "core/main", func_0011BC90);

INCLUDE_ASM(const s32, "core/main", func_0011C090);

INCLUDE_ASM(const s32, "core/main", func_0011C130);

INCLUDE_ASM(const s32, "core/main", func_0011C420);

INCLUDE_ASM(const s32, "core/main", func_0011C498);

INCLUDE_ASM(const s32, "core/main", func_0011C4D8);

INCLUDE_ASM(const s32, "core/main", func_0011C558);

void func_0011C5A0(void) { func_0011C660(1); }

INCLUDE_ASM(const s32, "core/main", func_0011C5A8);

INCLUDE_ASM(const s32, "core/main", func_0011C5F8);

INCLUDE_ASM(const s32, "core/main", func_0011C638);

s32 func_0011CA80(void);

s32 func_0011C660(void)
{
    func_0011CA80();
    return 1;
}

INCLUDE_ASM(const s32, "core/main", func_0011C680);

INCLUDE_ASM(const s32, "core/main", func_0011C768);

INCLUDE_ASM(const s32, "core/main", func_0011C7B0);

INCLUDE_ASM(const s32, "core/main", func_0011C990);

INCLUDE_ASM(const s32, "core/main", func_0011CA80);

INCLUDE_ASM(const s32, "core/main", func_0011CAA8);

INCLUDE_ASM(const s32, "core/main", func_0011CC68);

INCLUDE_ASM(const s32, "core/main", func_0011CE10);

void func_0011CEC0(s32 value) { D_00393130 = value; }

INCLUDE_ASM(const s32, "core/main", func_0011CEC8);

INCLUDE_ASM(const s32, "core/main", func_0011CF50);

INCLUDE_ASM(const s32, "core/main", func_0011CFC8);

INCLUDE_ASM(const s32, "core/main", func_0011D0C0);

void func_0011D290(void) { func_0011D0C0(); }

INCLUDE_ASM(const s32, "core/main", func_0011D2B0);

INCLUDE_ASM(const s32, "core/main", func_0011D320);

INCLUDE_ASM(const s32, "core/main", func_0011D380);

INCLUDE_ASM(const s32, "core/main", func_0011D4C0);

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

INCLUDE_ASM(const s32, "core/main", func_0011DA30);

INCLUDE_ASM(const s32, "core/main", func_0011DA68);

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

INCLUDE_ASM(const s32, "core/main", func_0011DAE0);

INCLUDE_ASM(const s32, "core/main", func_0011DB20);

void func_0011DB78(s16 *p, s16 value) { p[0] = value; }

u16 func_0011DB80(const u16 *p) { return p[1]; }

void func_0011DB88(s16 *p, s16 value) { p[1] = value; }

INCLUDE_ASM(const s32, "core/main", func_0011DB90);

INCLUDE_ASM(const s32, "core/main", func_0011DBC8);

void func_0011DBF8(s16 *p) { p[0] = 0; }

INCLUDE_ASM(const s32, "core/main", func_0011DC00);

INCLUDE_ASM(const s32, "core/main", func_0011DC30);

INCLUDE_ASM(const s32, "core/main", func_0011DD00);

INCLUDE_ASM(const s32, "core/main", func_0011DD60);

INCLUDE_ASM(const s32, "core/main", func_0011DDE8);

INCLUDE_ASM(const s32, "core/main", func_0011DE30);

INCLUDE_ASM(const s32, "core/main", func_0011DFE8);

INCLUDE_ASM(const s32, "core/main", func_0011E0A8);

INCLUDE_ASM(const s32, "core/main", func_0011E0E0);

s32 func_0011EC58();
void func_0011E1E8(u8 *p)
{
    if (*(u16 *)p == 1) {
        func_0011EC58(p);
    }
}

INCLUDE_ASM(const s32, "core/main", func_0011E208);

INCLUDE_ASM(const s32, "core/main", func_0011E330);

INCLUDE_ASM(const s32, "core/main", func_0011E368);

INCLUDE_ASM(const s32, "core/main", func_0011E3A0);

INCLUDE_ASM(const s32, "core/main", func_0011E4A0);

INCLUDE_ASM(const s32, "core/main", func_0011E910);

INCLUDE_ASM(const s32, "core/main", func_0011E940);

INCLUDE_ASM(const s32, "core/main", func_0011EA20);

void func_0011EA50(u8 *unused, u8 *p)
{
    *(s32 *)(p + 0x10) = 0x40;
    *(s32 *)p = 0x40;
    *(s32 *)(p + 0x4) = 0x40;
    *(s32 *)(p + 0xC) = 0x40;
}

INCLUDE_ASM(const s32, "core/main", func_0011EA70);

INCLUDE_ASM(const s32, "core/main", func_0011EB20);

INCLUDE_ASM(const s32, "core/main", func_0011EC58);

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

INCLUDE_ASM(const s32, "core/main", func_0011F2A8);

INCLUDE_ASM(const s32, "core/main", func_0011F2E0);

INCLUDE_ASM(const s32, "core/main", func_0011F318);

INCLUDE_ASM(const s32, "core/main", func_0011F360);

INCLUDE_ASM(const s32, "core/main", func_0011F3B0);

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

INCLUDE_ASM(const s32, "core/main", func_0011FF20);

INCLUDE_ASM(const s32, "core/main", func_0011FF60);

INCLUDE_ASM(const s32, "core/main", func_0011FFA0);

INCLUDE_ASM(const s32, "core/main", func_0011FFE0);


/* Source: math/region_0012.c */
/* func_00120020 through func_0012F8D8; retail address range 0x00120000-0x00130000. */




extern s32 D_00393184;

extern s32 D_00393180;
extern s32 D_00393194;

INCLUDE_ASM(const s32, "core/main", func_00120020);

INCLUDE_ASM(const s32, "core/main", func_001200B0);

INCLUDE_ASM(const s32, "core/main", func_00120130);

INCLUDE_ASM(const s32, "core/main", func_001201B8);

INCLUDE_ASM(const s32, "core/main", func_00120270);

s32 func_001202B8(s32 *p)
{
    return p ? p[2] : 0;
}

INCLUDE_ASM(const s32, "core/main", func_001202D0);

INCLUDE_ASM(const s32, "core/main", func_00120300);

void func_00120368(u8 *p)
{
    if (p != 0)
    {
        *(s32 *)(p + 0xC) = -2;
        p[0x14] = 1;
    }
}

s32 func_00120388(u8 *p)
{
    s32 result = -2;

    if (p != 0)
    {
        p[0x14] = 0;
        *(s32 *)(p + 0xC) = -2;
    }
    return result;
}

void func_001203A0(s32 *p, s32 value) { p[3] = value; }

INCLUDE_ASM(const s32, "core/main", func_001203A8);

INCLUDE_ASM(const s32, "core/main", func_00120428);

INCLUDE_ASM(const s32, "core/main", func_001204A8);

INCLUDE_ASM(const s32, "core/main", func_00120538);

INCLUDE_ASM(const s32, "core/main", func_00120588);

INCLUDE_ASM(const s32, "core/main", func_00120708);

s32 func_00120748(void)
{
    return func_00157DE8(D_0039347C, 0x10);
}

INCLUDE_ASM(const s32, "core/main", func_00120768);

INCLUDE_ASM(const s32, "core/main", func_001207E8);

INCLUDE_ASM(const s32, "core/main", func_001208C0);

/* Sign-extends the low 16 bits of the argument. */
s32 func_00120940(s32 value)
{
    return (s16)value;
}

INCLUDE_ASM(const s32, "core/main", func_00120950);

INCLUDE_ASM(const s32, "core/main", func_00120978);

INCLUDE_ASM(const s32, "core/main", func_001209C0);

INCLUDE_ASM(const s32, "core/main", func_001209F0);

INCLUDE_ASM(const s32, "core/main", func_00120A50);

INCLUDE_ASM(const s32, "core/main", func_00120AD8);

INCLUDE_ASM(const s32, "core/main", func_00120B38);

INCLUDE_ASM(const s32, "core/main", func_00120B70);

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

void func_00120C68(s32 index)
{
    func_0011E1E8(*(s32 *)((u8 *)D_00330A18 + (index + 2) * 4));
}

INCLUDE_ASM(const s32, "core/main", func_00120C88);

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

INCLUDE_ASM(const s32, "core/main", func_00121888);

INCLUDE_ASM(const s32, "core/main", func_001224F8);

INCLUDE_ASM(const s32, "core/main", func_001232E8);

INCLUDE_ASM(const s32, "core/main", func_001241D8);

INCLUDE_ASM(const s32, "core/main", func_00124260);

INCLUDE_ASM(const s32, "core/main", func_001246A8);

INCLUDE_ASM(const s32, "core/main", func_00124738);

INCLUDE_ASM(const s32, "core/main", func_00124988);

INCLUDE_ASM(const s32, "core/main", func_00124BB0);

INCLUDE_ASM(const s32, "core/main", func_00124EA8);

INCLUDE_ASM(const s32, "core/main", func_001250C8);

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

INCLUDE_ASM(const s32, "core/main", func_00127868);

INCLUDE_ASM(const s32, "core/main", func_001278D0);

INCLUDE_ASM(const s32, "core/main", func_001279E8);

INCLUDE_ASM(const s32, "core/main", func_00127A68);

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

INCLUDE_ASM(const s32, "core/main", func_00129038);

INCLUDE_ASM(const s32, "core/main", func_001290B8);

INCLUDE_ASM(const s32, "core/main", func_0012A108);

INCLUDE_ASM(const s32, "core/main", func_0012A468);

INCLUDE_ASM(void, "core/main", func_0012A628);

INCLUDE_ASM(const s32, "core/main", func_0012A740);

void func_0012A860(void) { func_0012A740(); }

INCLUDE_ASM(const s32, "core/main", func_0012A868);

INCLUDE_ASM(const s32, "core/main", func_0012ACF0);

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

INCLUDE_ASM(const s32, "core/main", func_0012B6A0);

INCLUDE_ASM(const s32, "core/main", func_0012B700);

INCLUDE_ASM(const s32, "core/main", func_0012B800);

INCLUDE_ASM(const s32, "core/main", func_0012B908);

INCLUDE_ASM(void, "core/main", func_0012BA18);

INCLUDE_ASM(const s32, "core/main", func_0012D750);

INCLUDE_ASM(const s32, "core/main", func_0012D810);

INCLUDE_ASM(const s32, "core/main", func_0012D888);

INCLUDE_ASM(const s32, "core/main", func_0012D900);

INCLUDE_ASM(const s32, "core/main", func_0012D988);

INCLUDE_ASM(const s32, "core/main", func_0012DC18);

INCLUDE_ASM(const s32, "core/main", func_0012DE80);

INCLUDE_ASM(const s32, "core/main", func_0012E110);

INCLUDE_ASM(const s32, "core/main", func_0012E138);

INCLUDE_ASM(const s32, "core/main", func_0012E260);

INCLUDE_ASM(const s32, "core/main", func_0012F180);

INCLUDE_ASM(const s32, "core/main", func_0012F518);

INCLUDE_ASM(const s32, "core/main", func_0012F6F0);

INCLUDE_ASM(const s32, "core/main", func_0012F8D8);


/* Source: math/region_0013.c */
/* func_00130888 through func_0013FFA0; retail address range 0x00130000-0x00140000. */



extern s32 D_003931C4;

INCLUDE_ASM(const s32, "core/main", func_00130888);

INCLUDE_ASM(const s32, "core/main", func_00131D78);

INCLUDE_ASM(const s32, "core/main", func_00133F40);

INCLUDE_ASM(const s32, "core/main", func_001340F0);

s32 func_00108648(s32);
s32 func_00135530();
s32 func_00139708(s32, s32);
s32 func_00131D78(s32, s32, f32);
s32 func_00135CF8(s32, s32, s32, s32, s32);
f32 func_00139828(s32, s32);
f32 func_00139858(s32, s32);
s32 func_00136C08(s32 *, s32, s32, s32, s32);
s32 func_0013A148(s32, f32);

/* Draft for func_001346E8 - standalone test version. */

extern s32 D_003931B0;          /* object entry array base (pointer value) */
extern s32 D_003931B8;
extern f32 D_003931B4;
extern f32 D_00392718;
extern f32 D_0039271C;
extern f32 D_00392720;
extern f32 D_00392724;
extern f32 D_00392728;
extern f32 D_0039272C;
extern f32 D_00392730;
extern s32 D_00330ED0;

