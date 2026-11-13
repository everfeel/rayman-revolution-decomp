#include "common.h"

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

INCLUDE_ASM(const s32, "core/main", func_00100460);

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

INCLUDE_ASM(const s32, "core/main", func_00107288);

INCLUDE_ASM(const s32, "core/main", func_001072A8);

INCLUDE_ASM(const s32, "core/main", func_001074E0);

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

INCLUDE_ASM(const s32, "core/main", func_00108A38);

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

INCLUDE_ASM(const s32, "core/main", func_00109C38);

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

INCLUDE_ASM(const s32, "core/main", func_0010A858);

void func_0010A878(u8 *p)
{
    *(s32 *)(*(u32 *)D_00254008 + 0x10) = *(s32 *)(p + 0x4);
}

INCLUDE_ASM(const s32, "core/main", func_0010A890);

INCLUDE_ASM(const s32, "core/main", func_0010A8B8);

INCLUDE_ASM(const s32, "core/main", func_0010A8D8);

void func_0010A8F8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0010A900);

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

INCLUDE_ASM(const s32, "core/main", func_0010AB00);

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

INCLUDE_ASM(const s32, "core/main", func_0010C898);

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

s32 func_0010C9F0(s32 *p) { return ((s32 *)((s32 *)p[1])[1])[0x2C / 4]; }

void func_0010CA00(s32 *p, s32 value) { ((s32 *)((s32 *)p[1])[1])[0x2C / 4] = value; }

INCLUDE_ASM(const s32, "core/main", func_0010CA10);

INCLUDE_ASM(const s32, "core/main", func_0010CA30);

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

INCLUDE_ASM(const s32, "core/main", func_0010DCA8);

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

s32 func_0010F318(u8 *p)
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
extern void func_00149880();

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

INCLUDE_ASM(const s32, "core/main", func_00111548);

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
extern void func_00215EC8();
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

INCLUDE_ASM(const s32, "core/main", func_00113120);

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

INCLUDE_ASM(const s32, "core/main", func_001146B8);

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

INCLUDE_ASM(const s32, "core/main", func_0011A2A0);

s32 func_0011A2C0(u8 *p)
{
    return *(u16 *)(p + 0x26) & 2;
}

INCLUDE_ASM(const s32, "core/main", func_0011A2D0);

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

INCLUDE_ASM(const s32, "core/main", func_0011B0A0);

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

INCLUDE_ASM(const s32, "core/main", func_0011C660);

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

INCLUDE_ASM(const s32, "core/main", func_0011E1E8);

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

INCLUDE_ASM(const s32, "core/main", func_0011EE80);

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

INCLUDE_ASM(const s32, "core/main", func_00120748);

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

INCLUDE_ASM(const s32, "core/main", func_0012B668);

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

INCLUDE_ASM(const s32, "core/main", func_001346E8);

INCLUDE_ASM(const s32, "core/main", func_00135530);

INCLUDE_ASM(const s32, "core/main", func_00135C10);

INCLUDE_ASM(const s32, "core/main", func_00135CF8);

INCLUDE_ASM(const s32, "core/main", func_00136970);

INCLUDE_ASM(const s32, "core/main", func_00136C08);

INCLUDE_ASM(const s32, "core/main", func_00137208);

INCLUDE_ASM(const s32, "core/main", func_001378C0);

INCLUDE_ASM(const s32, "core/main", func_001379E0);

INCLUDE_ASM(const s32, "core/main", func_00137C68);

INCLUDE_ASM(const s32, "core/main", func_00137CB8);

INCLUDE_ASM(const s32, "core/main", func_00137E10);

INCLUDE_ASM(const s32, "core/main", func_00138030);

INCLUDE_ASM(const s32, "core/main", func_00138170);

INCLUDE_ASM(const s32, "core/main", func_00138390);

INCLUDE_ASM(const s32, "core/main", func_001385D0);

INCLUDE_ASM(const s32, "core/main", func_001387F0);

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

INCLUDE_ASM(const s32, "core/main", func_00139228);

INCLUDE_ASM(const s32, "core/main", func_00139250);

void func_00139288(u8 *p)
{
    *(u32 *)(p + 0x8) &= 0xBFFFFFFF;
    func_00139228(p);
}

INCLUDE_ASM(const s32, "core/main", func_001392A8);

INCLUDE_ASM(const s32, "core/main", func_001396A0);

INCLUDE_ASM(const s32, "core/main", func_00139708);

INCLUDE_ASM(const s32, "core/main", func_00139760);

INCLUDE_ASM(const s32, "core/main", func_001397C0);

INCLUDE_ASM(const s32, "core/main", func_00139800);

INCLUDE_ASM(const s32, "core/main", func_00139828);

INCLUDE_ASM(const s32, "core/main", func_00139858);

INCLUDE_ASM(const s32, "core/main", func_00139888);

INCLUDE_ASM(const s32, "core/main", func_001399D8);

INCLUDE_ASM(const s32, "core/main", func_00139BA0);

INCLUDE_ASM(const s32, "core/main", func_00139CE0);

INCLUDE_ASM(const s32, "core/main", func_0013A148);

INCLUDE_ASM(const s32, "core/main", func_0013A1C8);

INCLUDE_ASM(const s32, "core/main", func_0013A468);

INCLUDE_ASM(const s32, "core/main", func_0013A4A8);

void func_0013A5D0(u8 *p) { func_001241D8((u8 *)p + 0xCC); }

INCLUDE_ASM(const s32, "core/main", func_0013A5D8);

INCLUDE_ASM(const s32, "core/main", func_0013A610);

INCLUDE_ASM(const s32, "core/main", func_0013A7C0);

INCLUDE_ASM(const s32, "core/main", func_0013A7F8);

INCLUDE_ASM(const s32, "core/main", func_0013A858);

INCLUDE_ASM(const s32, "core/main", func_0013A910);

INCLUDE_ASM(const s32, "core/main", func_0013AB00);

INCLUDE_ASM(const s32, "core/main", func_0013AC60);

INCLUDE_ASM(const s32, "core/main", func_0013AD90);

INCLUDE_ASM(const s32, "core/main", func_0013AFB8);

INCLUDE_ASM(const s32, "core/main", func_0013B060);

INCLUDE_ASM(const s32, "core/main", func_0013B258);

INCLUDE_ASM(const s32, "core/main", func_0013B2E8);

void func_0013B320(u8 *p) { func_001241D8((u8 *)p + 0xCC); }

INCLUDE_ASM(const s32, "core/main", func_0013B328);

INCLUDE_ASM(const s32, "core/main", func_0013B388);

INCLUDE_ASM(const s32, "core/main", func_0013B420);

INCLUDE_ASM(const s32, "core/main", func_0013B440);

INCLUDE_ASM(const s32, "core/main", func_0013B500);

INCLUDE_ASM(const s32, "core/main", func_0013B5E0);

INCLUDE_ASM(const s32, "core/main", func_0013B6B8);

INCLUDE_ASM(const s32, "core/main", func_0013B750);

INCLUDE_ASM(const s32, "core/main", func_0013B870);

INCLUDE_ASM(const s32, "core/main", func_0013BA88);

INCLUDE_ASM(const s32, "core/main", func_0013BAD0);

INCLUDE_ASM(const s32, "core/main", func_0013BD70);

INCLUDE_ASM(const s32, "core/main", func_0013BDC8);

INCLUDE_ASM(const s32, "core/main", func_0013C138);

INCLUDE_ASM(const s32, "core/main", func_0013C590);

INCLUDE_ASM(const s32, "core/main", func_0013C9D8);

INCLUDE_ASM(const s32, "core/main", func_0013CCC0);

INCLUDE_ASM(const s32, "core/main", func_0013CD48);

INCLUDE_ASM(const s32, "core/main", func_0013CEB8);

INCLUDE_ASM(const s32, "core/main", func_0013CF00);

INCLUDE_ASM(const s32, "core/main", func_0013CF50);

INCLUDE_ASM(const s32, "core/main", func_0013D038);

INCLUDE_ASM(const s32, "core/main", func_0013D1B0);

INCLUDE_ASM(const s32, "core/main", func_0013D930);

INCLUDE_ASM(const s32, "core/main", func_0013DE78);

INCLUDE_ASM(const s32, "core/main", func_0013E038);

INCLUDE_ASM(const s32, "core/main", func_0013E260);

INCLUDE_ASM(const s32, "core/main", func_0013E4E0);

INCLUDE_ASM(const s32, "core/main", func_0013E568);

INCLUDE_ASM(const s32, "core/main", func_0013E708);

INCLUDE_ASM(const s32, "core/main", func_0013E8A8);

INCLUDE_ASM(const s32, "core/main", func_0013EA20);

INCLUDE_ASM(const s32, "core/main", func_0013EA68);

INCLUDE_ASM(const s32, "core/main", func_0013EB08);

INCLUDE_ASM(const s32, "core/main", func_0013EB40);

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

INCLUDE_ASM(const s32, "core/main", func_0013F910);

INCLUDE_ASM(const s32, "core/main", func_0013F968);

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

INCLUDE_ASM(const s32, "core/main", func_001413C8);

INCLUDE_ASM(const s32, "core/main", func_001413E8);

INCLUDE_ASM(const s32, "core/main", func_001415E8);

INCLUDE_ASM(const s32, "core/main", func_001417A8);

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

INCLUDE_ASM(const s32, "core/main", func_00144830);

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

INCLUDE_ASM(const s32, "core/main", func_00144C90);

INCLUDE_ASM(const s32, "core/main", func_00144CB8);

u32 func_00144D98(void) { return 0x9D7C; }

u32 func_00144DA0(void) { return 0x9D7C; }

INCLUDE_ASM(const s32, "core/main", func_00144DA8);

INCLUDE_ASM(const s32, "core/main", func_00144DD0);

INCLUDE_ASM(const s32, "core/main", func_00144E40);

INCLUDE_ASM(const s32, "core/main", func_00144EB8);

INCLUDE_ASM(const s32, "core/main", func_00144F28);

INCLUDE_ASM(const s32, "core/main", func_00144F98);

void func_00144FE8(s32 arg)
{
    func_002249D4(arg + (s32)D_0034C3DC);
}

void func_00144FF8(s32 arg, s32 other)
{
    func_002249D4(other, arg + (s32)D_0034C3DC);
}

INCLUDE_ASM(const s32, "core/main", func_00145010);

u8 *func_00145048(u8 *p)
{
    return p ? p + 0x4B : 0;
}

u16 func_00145060(u8 *p)
{
    return p ? *(u16 *)(p + 0x48) : 0;
}

u8 *func_00145078(u8 *p)
{
    return p ? p + 0x5C : 0;
}

s32 func_00145090(u8 *p)
{
    return p ? *(s32 *)(p + 0x40) : 0;
}

INCLUDE_ASM(const s32, "core/main", func_001450A8);

INCLUDE_ASM(const s32, "core/main", func_00145168);

INCLUDE_ASM(const s32, "core/main", func_001451C0);

INCLUDE_ASM(const s32, "core/main", func_00145210);

INCLUDE_ASM(const s32, "core/main", func_00145378);

INCLUDE_ASM(const s32, "core/main", func_001453B8);

void func_001453E0(s32 arg, s32 value)
{
    func_00226310(arg, (s32)D_00389AD0, value);
}

void func_001453F0(s32 arg, s32 value) { func_001453E0(arg, value); }

INCLUDE_ASM(const s32, "core/main", func_001453F8);

INCLUDE_ASM(const s32, "core/main", func_00145438);

INCLUDE_ASM(const s32, "core/main", func_00145460);

INCLUDE_ASM(const s32, "core/main", func_001454A0);

INCLUDE_ASM(const s32, "core/main", func_001454C8);

INCLUDE_ASM(const s32, "core/main", func_001454F8);

INCLUDE_ASM(const s32, "core/main", func_00145560);

INCLUDE_ASM(const s32, "core/main", func_001455E0);

INCLUDE_ASM(const s32, "core/main", func_001456F0);

void func_00145790(s32 a, s32 b, s32 c)
{
    func_001456F0(a, b, c, c);
}

INCLUDE_ASM(const s32, "core/main", func_001457B0);

INCLUDE_ASM(const s32, "core/main", func_00145808);

INCLUDE_ASM(const s32, "core/main", func_00145840);

INCLUDE_ASM(const s32, "core/main", func_00145958);

INCLUDE_ASM(const s32, "core/main", func_001459C0);

INCLUDE_ASM(const s32, "core/main", func_00145A00);

INCLUDE_ASM(const s32, "core/main", func_00145A28);

INCLUDE_ASM(const s32, "core/main", func_00145A50);

INCLUDE_ASM(const s32, "core/main", func_00145A90);

INCLUDE_ASM(const s32, "core/main", func_00145BB8);

INCLUDE_ASM(const s32, "core/main", func_00145C10);

INCLUDE_ASM(const s32, "core/main", func_00145C50);

INCLUDE_ASM(const s32, "core/main", func_00145EC0);

INCLUDE_ASM(const s32, "core/main", func_00145F60);

INCLUDE_ASM(const s32, "core/main", func_00145F80);

INCLUDE_ASM(const s32, "core/main", func_00145FA0);

INCLUDE_ASM(const s32, "core/main", func_00145FF0);

void func_00146020(s32 *p, s32 value) { p[0] = value; }

INCLUDE_ASM(const s32, "core/main", func_00146028);

void func_00146058(s32 *p, s32 value) { p[0] = value; }

INCLUDE_ASM(const s32, "core/main", func_00146060);

INCLUDE_ASM(const s32, "core/main", func_00146118);

INCLUDE_ASM(const s32, "core/main", func_001462A8);

INCLUDE_ASM(const s32, "core/main", func_001463D0);

INCLUDE_ASM(const s32, "core/main", func_00146450);

INCLUDE_ASM(const s32, "core/main", func_00146518);

INCLUDE_ASM(const s32, "core/main", func_001465E0);

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

INCLUDE_ASM(const s32, "core/main", func_00146780);

INCLUDE_ASM(const s32, "core/main", func_001467B8);

INCLUDE_ASM(const s32, "core/main", func_00146850);

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

extern void func_00224B88();

void func_001475D8(s32 arg)
{
    func_00224B88(arg, 0, 0x10);
}

INCLUDE_ASM(const s32, "core/main", func_001475E8);

INCLUDE_ASM(const s32, "core/main", func_00147620);

INCLUDE_ASM(const s32, "core/main", func_00147648);

s32 func_00147678(u8 *p)
{
    return p[0] != 0;
}

s32 func_00147688(const u8 *p) { return *p == 2; }

s32 func_00147698(const u8 *p) { return *p == 1; }

INCLUDE_ASM(const s32, "core/main", func_001476A8);

INCLUDE_ASM(const s32, "core/main", func_001476E0);

INCLUDE_ASM(const s32, "core/main", func_00147718);

INCLUDE_ASM(const s32, "core/main", func_00147748);

INCLUDE_ASM(const s32, "core/main", func_00147778);

INCLUDE_ASM(const s32, "core/main", func_001477A8);

INCLUDE_ASM(const s32, "core/main", func_001477E0);

s32 func_00147810(const s32 *p) { return (p[0] >> 9) & 1; }

INCLUDE_ASM(const s32, "core/main", func_00147820);

INCLUDE_ASM(const s32, "core/main", func_00147848);

INCLUDE_ASM(const s32, "core/main", func_00147870);

INCLUDE_ASM(const s32, "core/main", func_001478D8);

INCLUDE_ASM(const s32, "core/main", func_00147920);

void func_00147968(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00147970);

INCLUDE_ASM(const s32, "core/main", func_00147A80);

INCLUDE_ASM(const s32, "core/main", func_00147AE8);

INCLUDE_ASM(const s32, "core/main", func_00147C30);

INCLUDE_ASM(const s32, "core/main", func_00147CF8);

INCLUDE_ASM(const s32, "core/main", func_00147E60);

INCLUDE_ASM(const s32, "core/main", func_001480C8);

INCLUDE_ASM(const s32, "core/main", func_00148130);

INCLUDE_ASM(const s32, "core/main", func_00148168);

INCLUDE_ASM(const s32, "core/main", func_001488A8);

INCLUDE_ASM(const s32, "core/main", func_00148BD8);

INCLUDE_ASM(const s32, "core/main", func_00148D20);

INCLUDE_ASM(const s32, "core/main", func_00148EC0);

INCLUDE_ASM(const s32, "core/main", func_00148F88);

INCLUDE_ASM(const s32, "core/main", func_001493C8);

INCLUDE_ASM(const s32, "core/main", func_00149618);

INCLUDE_ASM(const s32, "core/main", func_00149700);

INCLUDE_ASM(const s32, "core/main", func_00149738);

INCLUDE_ASM(const s32, "core/main", func_00149770);

INCLUDE_ASM(void, "core/main", func_001497A0);

INCLUDE_ASM(const s32, "core/main", func_001497D8);

INCLUDE_ASM(const s32, "core/main", func_00149810);

INCLUDE_ASM(const s32, "core/main", func_00149848);

INCLUDE_ASM(const s32, "core/main", func_00149880);

void func_001498A8(void)
{
    func_00149880(D_00355448[0]);
}

void func_001498B8(void) { func_00149848(); }

INCLUDE_ASM(const s32, "core/main", func_001498C0);

INCLUDE_ASM(const s32, "core/main", func_00149950);

INCLUDE_ASM(const s32, "core/main", func_00149A90);

INCLUDE_ASM(const s32, "core/main", func_0014A040);

INCLUDE_ASM(const s32, "core/main", func_0014A0B0);

INCLUDE_ASM(const s32, "core/main", func_0014A298);

s32 func_0014A5E0(u32 value) { return value != 0; }

void func_0014A5E8(u8 *p, s32 key, s32 value)
{
    *(s32 *)(p + 0x4) = key;
    *(s32 *)p = key ? value : 1;
}

INCLUDE_ASM(const s32, "core/main", func_0014A600);

INCLUDE_ASM(const s32, "core/main", func_0014A660);

INCLUDE_ASM(const s32, "core/main", func_0014A698);

INCLUDE_ASM(const s32, "core/main", func_0014A6E8);

INCLUDE_ASM(const s32, "core/main", func_0014A788);

INCLUDE_ASM(const s32, "core/main", func_0014A930);

INCLUDE_ASM(const s32, "core/main", func_0014A968);

INCLUDE_ASM(const s32, "core/main", func_0014A9E8);

INCLUDE_ASM(const s32, "core/main", func_0014AA68);

INCLUDE_ASM(const s32, "core/main", func_0014AB00);

INCLUDE_ASM(const s32, "core/main", func_0014ABD8);

INCLUDE_ASM(const s32, "core/main", func_0014AC08);

INCLUDE_ASM(const s32, "core/main", func_0014AC40);

INCLUDE_ASM(const s32, "core/main", func_0014AC68);

void func_0014AC98(s32 value)
{
    if (value == -1)
        D_003932D0 = 0;
    else
        D_003932D0 = value;
}

INCLUDE_ASM(const s32, "core/main", func_0014ACB8);

INCLUDE_ASM(const s32, "core/main", func_0014AD50);

INCLUDE_ASM(void, "core/main", func_0014AD90);

INCLUDE_ASM(const s32, "core/main", func_0014AF20);

INCLUDE_ASM(const s32, "core/main", func_0014AF88);

INCLUDE_ASM(const s32, "core/main", func_0014B010);

INCLUDE_ASM(const s32, "core/main", func_0014B050);

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

INCLUDE_ASM(const s32, "core/main", func_0014B320);

INCLUDE_ASM(const s32, "core/main", func_0014B348);

INCLUDE_ASM(const s32, "core/main", func_0014B588);

INCLUDE_ASM(const s32, "core/main", func_0014B650);

INCLUDE_ASM(const s32, "core/main", func_0014B7F8);

INCLUDE_ASM(const s32, "core/main", func_0014B880);

void func_0014B8C0(void) { func_0014BAA0(); }

INCLUDE_ASM(const s32, "core/main", func_0014B8E0);

INCLUDE_ASM(const s32, "core/main", func_0014B8F0);

INCLUDE_ASM(const s32, "core/main", func_0014B900);

INCLUDE_ASM(const s32, "core/main", func_0014B910);

INCLUDE_ASM(const s32, "core/main", func_0014B920);

INCLUDE_ASM(const s32, "core/main", func_0014B930);

INCLUDE_ASM(const s32, "core/main", func_0014B940);

INCLUDE_ASM(const s32, "core/main", func_0014B960);

INCLUDE_ASM(const s32, "core/main", func_0014B980);

INCLUDE_ASM(const s32, "core/main", func_0014B9A0);

INCLUDE_ASM(const s32, "core/main", func_0014B9C0);

INCLUDE_ASM(const s32, "core/main", func_0014BA18);

typedef struct FrameTimer {
    u8 unk00[8];
    f32 interval;
    f32 scaled_interval;
    s32 count;
} FrameTimer;

extern FrameTimer *D_003932E8;
extern s32 func_0014BBA8(FrameTimer *, f32);

s32 func_0014BA38(f32 interval)
{
    return func_0014BBA8(D_003932E8, interval);
}

INCLUDE_ASM(const s32, "core/main", func_0014BA58);

INCLUDE_ASM(const s32, "core/main", func_0014BA78);

INCLUDE_ASM(const s32, "core/main", func_0014BAA0);

INCLUDE_ASM(const s32, "core/main", func_0014BB08);

INCLUDE_ASM(const s32, "core/main", func_0014BB50);

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

INCLUDE_ASM(const s32, "core/main", func_0014BC50);

INCLUDE_ASM(const s32, "core/main", func_0014BC60);

INCLUDE_ASM(const s32, "core/main", func_0014BCD8);

INCLUDE_ASM(const s32, "core/main", func_0014BEC8);

INCLUDE_ASM(const s32, "core/main", func_0014BFA0);

void func_0014C018(u8 *p) { func_0014C1B8(*(s32 *)p); }

INCLUDE_ASM(const s32, "core/main", func_0014C038);

INCLUDE_ASM(const s32, "core/main", func_0014C068);

INCLUDE_ASM(const s32, "core/main", func_0014C130);

/* Returns the sign-extended low half of the 64-bit value at p. */
s32 func_0014C1A0(u8 *p)
{
    return (s32)(*(u64 *)p);
}

INCLUDE_ASM(const s32, "core/main", func_0014C1B8);

INCLUDE_ASM(const s32, "core/main", func_0014C210);

INCLUDE_ASM(const s32, "core/main", func_0014C240);

INCLUDE_ASM(const s32, "core/main", func_0014C280);

INCLUDE_ASM(const s32, "core/main", func_0014C2A0);

INCLUDE_ASM(const s32, "core/main", func_0014C2B8);

void func_0014C308(s32 arg)
{
    D_00393310 = arg;
    func_001C7658(D_00393314, arg);
}

INCLUDE_ASM(const s32, "core/main", func_0014C320);

void func_0014C348(void)
{
    func_001C7BB8(D_00393314, 0, 0);
}

INCLUDE_ASM(const s32, "core/main", func_0014C358);

void func_0014C398(u32 address)
{
    __asm__ volatile("ctc2 %0, $vi27" : : "r"(address));
    __asm__ volatile("vnop");
    __asm__ volatile("vnop");
    __asm__ volatile("vcallmsr $vi27");
}

INCLUDE_ASM(const s32, "core/main", func_0014C3B0);

INCLUDE_ASM(const s32, "core/main", func_0014C450);

INCLUDE_ASM(const s32, "core/main", func_0014C470);

INCLUDE_ASM(const s32, "core/main", func_0014C498);

INCLUDE_ASM(const s32, "core/main", func_0014C4C0);

INCLUDE_ASM(const s32, "core/main", func_0014C4D0);

INCLUDE_ASM(const s32, "core/main", func_0014C528);

INCLUDE_ASM(const s32, "core/main", func_0014C588);

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

INCLUDE_ASM(const s32, "core/main", func_0014C980);

INCLUDE_ASM(const s32, "core/main", func_0014C9A0);

INCLUDE_ASM(const s32, "core/main", func_0014CAC8);

INCLUDE_ASM(const s32, "core/main", func_0014CBE8);

INCLUDE_ASM(const s32, "core/main", func_0014CD88);

INCLUDE_ASM(const s32, "core/main", func_0014CDD8);

s32 func_0014CE28(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

INCLUDE_ASM(const s32, "core/main", func_0014CE30);

INCLUDE_ASM(const s32, "core/main", func_0014CE68);

INCLUDE_ASM(const s32, "core/main", func_0014CEA8);

INCLUDE_ASM(const s32, "core/main", func_0014CEE8);

void func_0014CF10(s32 *p) { p[0] = (s32)p; p[1] = (s32)p; p[2] = 0; }

INCLUDE_ASM(const s32, "core/main", func_0014CF20);

INCLUDE_ASM(const s32, "core/main", func_0014CF68);

INCLUDE_ASM(const s32, "core/main", func_0014CFA0);

INCLUDE_ASM(const s32, "core/main", func_0014CFD8);

INCLUDE_ASM(const s32, "core/main", func_0014D128);

INCLUDE_ASM(const s32, "core/main", func_0014D160);

INCLUDE_ASM(const s32, "core/main", func_0014D2B8);

INCLUDE_ASM(const s32, "core/main", func_0014D3D0);

INCLUDE_ASM(const s32, "core/main", func_0014D5B0);

INCLUDE_ASM(const s32, "core/main", func_0014D6C8);

INCLUDE_ASM(const s32, "core/main", func_0014D7A0);

INCLUDE_ASM(const s32, "core/main", func_0014D800);

INCLUDE_ASM(const s32, "core/main", func_0014D8E8);

INCLUDE_ASM(const s32, "core/main", func_0014D990);

INCLUDE_ASM(const s32, "core/main", func_0014DA10);

INCLUDE_ASM(const s32, "core/main", func_0014DA58);

INCLUDE_ASM(const s32, "core/main", func_0014DA80);

INCLUDE_ASM(const s32, "core/main", func_0014DC28);

INCLUDE_ASM(const s32, "core/main", func_0014DC38);

INCLUDE_ASM(const s32, "core/main", func_0014DC98);

INCLUDE_ASM(const s32, "core/main", func_0014DCE0);

INCLUDE_ASM(const s32, "core/main", func_0014DD30);

INCLUDE_ASM(const s32, "core/main", func_0014DEC0);

INCLUDE_ASM(const s32, "core/main", func_0014E030);

INCLUDE_ASM(const s32, "core/main", func_0014E098);

INCLUDE_ASM(const s32, "core/main", func_0014E388);

INCLUDE_ASM(const s32, "core/main", func_0014E768);

void func_0014E7D8(u8 *p)
{
    u8 *q = *(u8 **)(p + 0xE0);

    if (q != 0)
        func_0016EFA8(q);
}

INCLUDE_ASM(const s32, "core/main", func_0014E7F8);

INCLUDE_ASM(const s32, "core/main", func_0014E818);

INCLUDE_ASM(const s32, "core/main", func_0014E838);

INCLUDE_ASM(const s32, "core/main", func_0014E848);

INCLUDE_ASM(const s32, "core/main", func_0014E988);

INCLUDE_ASM(const s32, "core/main", func_0014E9C8);

void func_0014E9E8(u8 *p)
{
    p[0x40] = 0;
    func_00224B88(p, 0, 0x40);
}

void func_0014EA00(s32 value) { D_00393398 = value; }

INCLUDE_ASM(const s32, "core/main", func_0014EA08);

INCLUDE_ASM(const s32, "core/main", func_0014EB30);

INCLUDE_ASM(const s32, "core/main", func_0014EBA8);

void func_0014EC40(void)
{
    func_001755B0(D_003933DC, (s32)func_0014EBA8, 0);
}

INCLUDE_ASM(const s32, "core/main", func_0014EC58);

INCLUDE_ASM(const s32, "core/main", func_0014ED50);

void func_0014EDA0(void) { func_0016F830(); }

INCLUDE_ASM(const s32, "core/main", func_0014EDA8);

INCLUDE_ASM(const s32, "core/main", func_0014EE00);

INCLUDE_ASM(const s32, "core/main", func_0014EF50);

INCLUDE_ASM(const s32, "core/main", func_0014EF80);

INCLUDE_ASM(const s32, "core/main", func_0014F000);

INCLUDE_ASM(const s32, "core/main", func_0014F170);

INCLUDE_ASM(const s32, "core/main", func_0014F1C8);

INCLUDE_ASM(const s32, "core/main", func_0014F2E0);

INCLUDE_ASM(const s32, "core/main", func_0014F5A8);

INCLUDE_ASM(const s32, "core/main", func_0014F620);

void func_0014F720(void) { D_003933D8 = 0; }

INCLUDE_ASM(const s32, "core/main", func_0014F728);

INCLUDE_ASM(const s32, "core/main", func_0014F738);

INCLUDE_ASM(const s32, "core/main", func_0014F7A8);

INCLUDE_ASM(const s32, "core/main", func_0014F808);

INCLUDE_ASM(const s32, "core/main", func_0014F8E8);

INCLUDE_ASM(const s32, "core/main", func_0014F918);

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

INCLUDE_ASM(const s32, "core/main", func_00150298);

s32 func_001502F8(s32 value) { return value; }

s32 func_00150300(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_00150308);

s32 func_00150328(s32 value) { return value; }

s32 func_00150330(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_00150338);

INCLUDE_ASM(const s32, "core/main", func_001503B0);

void func_00150428(u8 *p, s32 value) { *(s32 *)(p + D_003933EC) = value; }

s32 func_00150438(const u8 *p) { return *(const s32 *)(p + D_003933EC); }

INCLUDE_ASM(const s32, "core/main", func_00150448);

INCLUDE_ASM(const s32, "core/main", func_001507B8);

INCLUDE_ASM(const s32, "core/main", func_001509F0);

INCLUDE_ASM(const s32, "core/main", func_001512E8);

INCLUDE_ASM(const s32, "core/main", func_001512F8);

INCLUDE_ASM(const s32, "core/main", func_00151418);

INCLUDE_ASM(const s32, "core/main", func_00151570);

INCLUDE_ASM(const s32, "core/main", func_001516A8);

INCLUDE_ASM(const s32, "core/main", func_001517A0);

INCLUDE_ASM(const s32, "core/main", func_00151848);

INCLUDE_ASM(const s32, "core/main", func_001518A0);

INCLUDE_ASM(const s32, "core/main", func_001518F0);

void func_00151930(void) { func_00178AE0(); }

INCLUDE_ASM(const s32, "core/main", func_00151938);

INCLUDE_ASM(const s32, "core/main", func_00151960);

INCLUDE_ASM(const s32, "core/main", func_00151980);

INCLUDE_ASM(const s32, "core/main", func_001519A0);

INCLUDE_ASM(const s32, "core/main", func_00151A08);

INCLUDE_ASM(const s32, "core/main", func_00151B38);

INCLUDE_ASM(const s32, "core/main", func_00151C40);

INCLUDE_ASM(const s32, "core/main", func_00151CD8);

void func_00151D38(s32 *p, s32 value) { p[3] = value; }

INCLUDE_ASM(const s32, "core/main", func_00151D40);

INCLUDE_ASM(const s32, "core/main", func_00151D80);

INCLUDE_ASM(const s32, "core/main", func_00151D90);

INCLUDE_ASM(const s32, "core/main", func_00151E70);

INCLUDE_ASM(const s32, "core/main", func_00151F08);

INCLUDE_ASM(const s32, "core/main", func_00151FB8);

INCLUDE_ASM(const s32, "core/main", func_00152188);

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

INCLUDE_ASM(const s32, "core/main", func_00153DF8);

INCLUDE_ASM(const s32, "core/main", func_00153E48);

INCLUDE_ASM(const s32, "core/main", func_00153EF8);

INCLUDE_ASM(const s32, "core/main", func_00153F30);

INCLUDE_ASM(const s32, "core/main", func_001541A0);

INCLUDE_ASM(const s32, "core/main", func_00154238);

INCLUDE_ASM(const s32, "core/main", func_00154400);

INCLUDE_ASM(const s32, "core/main", func_00154700);

INCLUDE_ASM(const s32, "core/main", func_001547A8);

INCLUDE_ASM(const s32, "core/main", func_00154848);

INCLUDE_ASM(const s32, "core/main", func_00154A68);

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

INCLUDE_ASM(const s32, "core/main", func_00156A30);

INCLUDE_ASM(const s32, "core/main", func_00156A80);

INCLUDE_ASM(const s32, "core/main", func_00156D50);

INCLUDE_ASM(const s32, "core/main", func_00156E48);

INCLUDE_ASM(const s32, "core/main", func_001570F0);

INCLUDE_ASM(const s32, "core/main", func_00157140);

INCLUDE_ASM(const s32, "core/main", func_00157510);

INCLUDE_ASM(const s32, "core/main", func_00157560);

INCLUDE_ASM(const s32, "core/main", func_001575F0);

INCLUDE_ASM(const s32, "core/main", func_001576D8);

INCLUDE_ASM(const s32, "core/main", func_00157710);

INCLUDE_ASM(const s32, "core/main", func_00157748);

void func_001577D0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001577D8);

INCLUDE_ASM(const s32, "core/main", func_00157840);

INCLUDE_ASM(const s32, "core/main", func_00157870);

INCLUDE_ASM(const s32, "core/main", func_001578A8);

INCLUDE_ASM(const s32, "core/main", func_00157920);

INCLUDE_ASM(const s32, "core/main", func_00157A88);

INCLUDE_ASM(const s32, "core/main", func_00157B50);

INCLUDE_ASM(const s32, "core/main", func_00157BE8);

void func_00157C18(void) { func_0014C3B0(); }

INCLUDE_ASM(const s32, "core/main", func_00157C20);

INCLUDE_ASM(const s32, "core/main", func_00157C50);

INCLUDE_ASM(const s32, "core/main", func_00157C70);

INCLUDE_ASM(const s32, "core/main", func_00157CC0);

INCLUDE_ASM(const s32, "core/main", func_00157CE8);

void func_00157D10(void) { func_0014C938(); }

INCLUDE_ASM(const s32, "core/main", func_00157D18);

INCLUDE_ASM(const s32, "core/main", func_00157D38);

INCLUDE_ASM(const s32, "core/main", func_00157D68);

INCLUDE_ASM(const s32, "core/main", func_00157D88);

INCLUDE_ASM(const s32, "core/main", func_00157DB0);

INCLUDE_ASM(const s32, "core/main", func_00157DE8);

INCLUDE_ASM(const s32, "core/main", func_00157E30);

INCLUDE_ASM(const s32, "core/main", func_00157E60);

INCLUDE_ASM(const s32, "core/main", func_00157EA8);

INCLUDE_ASM(const s32, "core/main", func_00157F10);

INCLUDE_ASM(const s32, "core/main", func_00157F30);

INCLUDE_ASM(const s32, "core/main", func_00157F70);

INCLUDE_ASM(const s32, "core/main", func_001580E0);

void func_00158920(void) { func_002152C8(1); }

INCLUDE_ASM(const s32, "core/main", func_00158928);

INCLUDE_ASM(const s32, "core/main", func_00158958);

INCLUDE_ASM(const s32, "core/main", func_00158988);

INCLUDE_ASM(const s32, "core/main", func_00158A00);


/* Source: audio/region_0015.c */
/* func_00158B28 through func_0015D170; retail address range 0x00158B28-0x0015D208. */


INCLUDE_ASM(const s32, "core/main", func_00158B28);

INCLUDE_ASM(const s32, "core/main", func_00158BC8);

INCLUDE_ASM(const s32, "core/main", func_00158C58);

INCLUDE_ASM(const s32, "core/main", func_00158E60);

INCLUDE_ASM(const s32, "core/main", func_00158EC8);

INCLUDE_ASM(const s32, "core/main", func_00158F98);

INCLUDE_ASM(const s32, "core/main", func_00159230);

s32 func_00159300(const s32 *p) { return *(const s32 *)((const u8 *)p + (16)); }

void func_00159308(void) { func_00158F98(); }

INCLUDE_ASM(const s32, "core/main", func_00159310);

INCLUDE_ASM(const s32, "core/main", func_00159530);

INCLUDE_ASM(const s32, "core/main", func_00159610);

INCLUDE_ASM(const s32, "core/main", func_00159698);

INCLUDE_ASM(const s32, "core/main", func_00159738);

INCLUDE_ASM(const s32, "core/main", func_00159770);

INCLUDE_ASM(const s32, "core/main", func_001597B0);

void func_001597F0(void)
{
    D_003D7D80[0] = 0;
    D_003D7D80[0x58 / 4] = 0;
    D_003D7D80[0x5C / 4] = 0;
}

void func_00159808(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00159810);

INCLUDE_ASM(const s32, "core/main", func_00159860);

void func_00159928(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00159930);

INCLUDE_ASM(const s32, "core/main", func_001599A8);

INCLUDE_ASM(const s32, "core/main", func_00159A70);

INCLUDE_ASM(const s32, "core/main", func_00159C70);

s32 func_00159CE0(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

INCLUDE_ASM(const s32, "core/main", func_00159CE8);

INCLUDE_ASM(const s32, "core/main", func_00159CF8);

INCLUDE_ASM(const s32, "core/main", func_00159D68);

INCLUDE_ASM(const s32, "core/main", func_00159D88);

void func_00159DA8(void) {
}

s32 func_00159DB0(void) { return -1; }

s32 func_00159DB8(void) { return -1; }

INCLUDE_ASM(const s32, "core/main", func_00159DC0);

INCLUDE_ASM(const s32, "core/main", func_00159DD0);

INCLUDE_ASM(const s32, "core/main", func_00159E00);

INCLUDE_ASM(const s32, "core/main", func_00159E38);

INCLUDE_ASM(const s32, "core/main", func_00159F00);

INCLUDE_ASM(const s32, "core/main", func_00159F58);

INCLUDE_ASM(const s32, "core/main", func_00159F88);

s32 func_0015A0F0(s32 index)
{
    return *(s32 *)(D_00393C68 + index * 0x74 + 0x64);
}

INCLUDE_ASM(const s32, "core/main", func_0015A108);

INCLUDE_ASM(const s32, "core/main", func_0015A150);

INCLUDE_ASM(const s32, "core/main", func_0015A2A0);

INCLUDE_ASM(const s32, "core/main", func_0015A300);

INCLUDE_ASM(const s32, "core/main", func_0015A388);

INCLUDE_ASM(const s32, "core/main", func_0015A648);

INCLUDE_ASM(const s32, "core/main", func_0015A708);

INCLUDE_ASM(const s32, "core/main", func_0015A7E8);

INCLUDE_ASM(const s32, "core/main", func_0015A870);

INCLUDE_ASM(const s32, "core/main", func_0015A908);

INCLUDE_ASM(const s32, "core/main", func_0015A968);

INCLUDE_ASM(const s32, "core/main", func_0015A9D8);

INCLUDE_ASM(const s32, "core/main", func_0015AA30);

INCLUDE_ASM(const s32, "core/main", func_0015AAA0);

INCLUDE_ASM(const s32, "core/main", func_0015AB18);

void func_0015AB40(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0015AB48);

INCLUDE_ASM(const s32, "core/main", func_0015AB78);

INCLUDE_ASM(const s32, "core/main", func_0015AD40);

INCLUDE_ASM(const s32, "core/main", func_0015B360);

INCLUDE_ASM(const s32, "core/main", func_0015B678);

INCLUDE_ASM(const s32, "core/main", func_0015B6F8);

s32 func_0015B768(void) { return 0; }

s32 func_0015B770(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_0015B778);

s32 func_0015B7A8(void) { return 0; }

s32 func_0015B7B0(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0015B7B8);

INCLUDE_ASM(const s32, "core/main", func_0015B7D8);

INCLUDE_ASM(const s32, "core/main", func_0015B7F8);

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

INCLUDE_ASM(const s32, "core/main", func_0015B8D8);

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

INCLUDE_ASM(const s32, "core/main", func_0015B9F8);

INCLUDE_ASM(void, "core/main", func_0015BA18);

INCLUDE_ASM(const s32, "core/main", func_0015BAA8);

INCLUDE_ASM(const s32, "core/main", func_0015BAC8);

void func_0015BAE8(void) { func_0015FC50(); }

INCLUDE_ASM(const s32, "core/main", func_0015BAF0);

INCLUDE_ASM(const s32, "core/main", func_0015BBF0);

INCLUDE_ASM(const s32, "core/main", func_0015BC20);

INCLUDE_ASM(const s32, "core/main", func_0015BCF0);

INCLUDE_ASM(const s32, "core/main", func_0015BD30);

INCLUDE_ASM(const s32, "core/main", func_0015BF30);

INCLUDE_ASM(const s32, "core/main", func_0015BF70);

INCLUDE_ASM(const s32, "core/main", func_0015C050);

void func_0015C330(void) { func_001603C8(); }

INCLUDE_ASM(const s32, "core/main", func_0015C338);

INCLUDE_ASM(const s32, "core/main", func_0015C3C0);

INCLUDE_ASM(const s32, "core/main", func_0015C418);

INCLUDE_ASM(const s32, "core/main", func_0015C488);

INCLUDE_ASM(const s32, "core/main", func_0015C520);

INCLUDE_ASM(const s32, "core/main", func_0015C578);

INCLUDE_ASM(const s32, "core/main", func_0015C5A8);

INCLUDE_ASM(const s32, "core/main", func_0015C5F0);

INCLUDE_ASM(const s32, "core/main", func_0015C660);

INCLUDE_ASM(const s32, "core/main", func_0015C6A0);

INCLUDE_ASM(const s32, "core/main", func_0015C7D0);

INCLUDE_ASM(const s32, "core/main", func_0015C8A0);

INCLUDE_ASM(const s32, "core/main", func_0015C990);

INCLUDE_ASM(const s32, "core/main", func_0015CA40);

INCLUDE_ASM(const s32, "core/main", func_0015CB68);

INCLUDE_ASM(const s32, "core/main", func_0015CC58);

INCLUDE_ASM(const s32, "core/main", func_0015CCB8);

INCLUDE_ASM(const s32, "core/main", func_0015CDD8);

INCLUDE_ASM(const s32, "core/main", func_0015CE58);

INCLUDE_ASM(const s32, "core/main", func_0015CE88);

INCLUDE_ASM(const s32, "core/main", func_0015CEE0);

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

INCLUDE_ASM(const s32, "core/main", func_0015D170);


/* Source: misc/region_0015_tail.c */
/* func_0015D208 through func_0015FFE8; retail address range 0x0015D208-0x00160000. */





extern s32 D_003935AC;

extern s32 D_003935A8;

extern s32 D_00393574;

INCLUDE_ASM(const s32, "core/main", func_0015D208);

INCLUDE_ASM(const s32, "core/main", func_0015D2A0);

INCLUDE_ASM(const s32, "core/main", func_0015D338);

void func_0015D3D0(void) { func_00160758(); }

void func_0015D3D8(void) { func_00160760(); }

INCLUDE_ASM(const s32, "core/main", func_0015D3E0);

INCLUDE_ASM(const s32, "core/main", func_0015D428);

void func_0015D470(void) { func_0015D3E0(); }

void func_0015D478(void) { func_0015D428(); }

INCLUDE_ASM(const s32, "core/main", func_0015D480);

INCLUDE_ASM(const s32, "core/main", func_0015D4A0);

void func_0015D4B0(void) { func_001602E0(); }

INCLUDE_ASM(const s32, "core/main", func_0015D4B8);

INCLUDE_ASM(const s32, "core/main", func_0015D4D8);

void func_0015D4F8(void) { func_00160328(); }

INCLUDE_ASM(const s32, "core/main", func_0015D500);

void func_0015D520(void) { func_00160870(); }

void func_0015D528(void) { func_00160878(); }

INCLUDE_ASM(const s32, "core/main", func_0015D530);

INCLUDE_ASM(const s32, "core/main", func_0015D550);

void func_0015D570(void) { func_001608C0(); }

void func_0015D578(void) { func_001608C8(); }

void func_0015D580(void) { func_001608D0(); }

INCLUDE_ASM(const s32, "core/main", func_0015D588);

INCLUDE_ASM(const s32, "core/main", func_0015D5A8);

INCLUDE_ASM(const s32, "core/main", func_0015D5C8);

INCLUDE_ASM(const s32, "core/main", func_0015D5E8);

void func_0015D608(void) { func_00160958(); }

INCLUDE_ASM(const s32, "core/main", func_0015D610);

INCLUDE_ASM(const s32, "core/main", func_0015D648);

INCLUDE_ASM(const s32, "core/main", func_0015D668);

void func_0015D688(void) { func_00160B10(); }

INCLUDE_ASM(const s32, "core/main", func_0015D690);

INCLUDE_ASM(const s32, "core/main", func_0015D6F8);

INCLUDE_ASM(const s32, "core/main", func_0015D750);

INCLUDE_ASM(const s32, "core/main", func_0015D7A8);

INCLUDE_ASM(const s32, "core/main", func_0015D850);

INCLUDE_ASM(const s32, "core/main", func_0015D8F0);

INCLUDE_ASM(const s32, "core/main", func_0015D948);

INCLUDE_ASM(const s32, "core/main", func_0015D990);

INCLUDE_ASM(const s32, "core/main", func_0015D9D8);

INCLUDE_ASM(const s32, "core/main", func_0015DA60);

INCLUDE_ASM(const s32, "core/main", func_0015DAD8);

INCLUDE_ASM(const s32, "core/main", func_0015DB70);

INCLUDE_ASM(const s32, "core/main", func_0015DBF8);

INCLUDE_ASM(const s32, "core/main", func_0015DC68);

INCLUDE_ASM(const s32, "core/main", func_0015DCD0);

INCLUDE_ASM(const s32, "core/main", func_0015DD70);

INCLUDE_ASM(const s32, "core/main", func_0015DDE8);

s32 func_0015DE78(void) { return 0; }

s32 func_0015DE80(void) { return -1; }

s32 func_0015DE88(void) { return 0; }

s32 func_0015DE90(void) { return 0; }

void func_0015DE98(void) {
}

s32 func_0015DEA0(void) { return 0; }

s32 func_0015DEA8(void) { return 0; }

s32 func_0015DEB0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_0015DEB8);

INCLUDE_ASM(const s32, "core/main", func_0015DED8);

INCLUDE_ASM(const s32, "core/main", func_0015DF18);

INCLUDE_ASM(const s32, "core/main", func_0015DF50);

INCLUDE_ASM(const s32, "core/main", func_0015DFE0);

INCLUDE_ASM(const s32, "core/main", func_0015E0B0);

INCLUDE_ASM(const s32, "core/main", func_0015E1E0);

INCLUDE_ASM(const s32, "core/main", func_0015E248);

INCLUDE_ASM(const s32, "core/main", func_0015E2A0);

INCLUDE_ASM(const s32, "core/main", func_0015E2F0);

INCLUDE_ASM(const s32, "core/main", func_0015E318);

INCLUDE_ASM(const s32, "core/main", func_0015E338);

INCLUDE_ASM(const s32, "core/main", func_0015E358);

s32 func_0015E378(const s32 *p) { return -(p[0] & 1); }

s32 func_0015E388(const s32 *p) { return (p[0] & 5) == 0; }

s32 func_0015E398(u32 *p)
{
    return -((*p >> 1) & 1);
}

s32 func_0015E3B0(const s32 *p) { return (p[0] & 0xA) == 0; }

INCLUDE_ASM(const s32, "core/main", func_0015E3C0);

INCLUDE_ASM(const s32, "core/main", func_0015E510);

INCLUDE_ASM(const s32, "core/main", func_0015E540);

INCLUDE_ASM(const s32, "core/main", func_0015E698);

s32 func_0015E848(void) { return D_00393574; }

void func_0015E850(s32 value) { D_00393574 = value; }

INCLUDE_ASM(const s32, "core/main", func_0015E858);

/* VU0 status register bit tests. */




/* Starts a VU0 microprogram running at a CSR address. */


/* Reads a word from the VU scratchpad. */

INCLUDE_ASM(const s32, "core/main", func_0015E870);

INCLUDE_ASM(const s32, "core/main", func_0015E880);

INCLUDE_ASM(const s32, "core/main", func_0015E890);

void func_0015E8C0(void)
{
    D_003702D0[0] = 0;
    D_003702D0[1] = 0;
    D_003702D0[2] = 0;
}

INCLUDE_ASM(const s32, "core/main", func_0015E8D8);

INCLUDE_ASM(const s32, "core/main", func_0015E918);

INCLUDE_ASM(const s32, "core/main", func_0015E980);

INCLUDE_ASM(const s32, "core/main", func_0015E9D8);

INCLUDE_ASM(const s32, "core/main", func_0015ED10);

INCLUDE_ASM(const s32, "core/main", func_0015F080);

INCLUDE_ASM(const s32, "core/main", func_0015F178);

INCLUDE_ASM(const s32, "core/main", func_0015F198);

INCLUDE_ASM(const s32, "core/main", func_0015F2E8);

INCLUDE_ASM(const s32, "core/main", func_0015F3A0);

INCLUDE_ASM(const s32, "core/main", func_0015F3D0);

s32 func_0015F3E0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_0015F3E8);

s32 func_0015F458(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0015F460);

INCLUDE_ASM(const s32, "core/main", func_0015F4E8);

INCLUDE_ASM(const s32, "core/main", func_0015F568);

INCLUDE_ASM(const s32, "core/main", func_0015F5D0);

INCLUDE_ASM(const s32, "core/main", func_0015F648);

INCLUDE_ASM(const s32, "core/main", func_0015F670);

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

INCLUDE_ASM(const s32, "core/main", func_0015F6D0);

INCLUDE_ASM(const s32, "core/main", func_0015F718);

void func_0015F748(void) { func_001C0C60(D_003935AC); }

void func_0015F750(void) { func_001C0C60(D_003935AC); }

void func_0015F758(void) {
}

s32 func_0015F760(void) { return 0; }

void func_0015F768(void) { func_001C0C40(D_003935AC); }

void func_0015F770(void) {
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

INCLUDE_ASM(const s32, "core/main", func_0015F888);

void func_0015F8B0(s32 *p) { p[4] = 0; }

INCLUDE_ASM(const s32, "core/main", func_0015F8B8);

INCLUDE_ASM(const s32, "core/main", func_0015FA08);

INCLUDE_ASM(const s32, "core/main", func_0015FAC8);

INCLUDE_ASM(const s32, "core/main", func_0015FB60);

INCLUDE_ASM(const s32, "core/main", func_0015FBC0);

INCLUDE_ASM(const s32, "core/main", func_0015FBF8);

INCLUDE_ASM(const s32, "core/main", func_0015FC50);

INCLUDE_ASM(const s32, "core/main", func_0015FC78);

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

INCLUDE_ASM(const s32, "core/main", func_001600F8);

INCLUDE_ASM(const s32, "core/main", func_00160168);

INCLUDE_ASM(const s32, "core/main", func_00160210);

void func_001602B8(u32 value) { func_00163D78((u8)value); }

INCLUDE_ASM(const s32, "core/main", func_001602C0);

void func_001602E0(void) { func_00163DA0(); }

INCLUDE_ASM(const s32, "core/main", func_001602E8);

INCLUDE_ASM(const s32, "core/main", func_00160308);

void func_00160328(void) { func_00163DB0(); }

INCLUDE_ASM(const s32, "core/main", func_00160330);

INCLUDE_ASM(const s32, "core/main", func_00160350);

INCLUDE_ASM(const s32, "core/main", func_00160370);

s32 func_001603A8(void) { return 3; }

s32 func_001603B0(void) { return 1; }

void func_001603B8(void) {
}

void func_001603C0(void) {
}

void func_001603C8(void) { func_00163F08(); }

INCLUDE_ASM(const s32, "core/main", func_001603D0);

void func_001603F0(void) { func_00164130(); }

void func_001603F8(void) { func_00164080(); }

INCLUDE_ASM(const s32, "core/main", func_00160400);

INCLUDE_ASM(const s32, "core/main", func_001604C8);

INCLUDE_ASM(const s32, "core/main", func_00160550);

INCLUDE_ASM(const s32, "core/main", func_001605B8);

INCLUDE_ASM(const s32, "core/main", func_00160630);

INCLUDE_ASM(const s32, "core/main", func_00160680);

INCLUDE_ASM(const s32, "core/main", func_001606E8);

INCLUDE_ASM(const s32, "core/main", func_00160718);

void func_00160748(void) { func_001648B8(); }

void func_00160750(void) { func_001648D8(); }

void func_00160758(void) { func_001648F8(); }

void func_00160760(void) { func_00164900(); }

INCLUDE_ASM(const s32, "core/main", func_00160768);

INCLUDE_ASM(const s32, "core/main", func_001607B8);

void func_00160870(void) { func_00164908(); }

void func_00160878(void) { func_00164910(); }

INCLUDE_ASM(const s32, "core/main", func_00160880);

INCLUDE_ASM(const s32, "core/main", func_001608A0);

void func_001608C0(void) { func_001642E8(); }

void func_001608C8(void) { func_00164318(); }

void func_001608D0(void) { func_00164348(); }

INCLUDE_ASM(const s32, "core/main", func_001608D8);

INCLUDE_ASM(const s32, "core/main", func_001608F8);

INCLUDE_ASM(const s32, "core/main", func_00160918);

INCLUDE_ASM(const s32, "core/main", func_00160938);

void func_00160958(void) { func_001642B8(); }

INCLUDE_ASM(const s32, "core/main", func_00160960);

INCLUDE_ASM(const s32, "core/main", func_001609F0);

INCLUDE_ASM(const s32, "core/main", func_00160A10);

INCLUDE_ASM(const s32, "core/main", func_00160A48);

INCLUDE_ASM(const s32, "core/main", func_00160AD0);

INCLUDE_ASM(const s32, "core/main", func_00160AF0);

INCLUDE_ASM(const s32, "core/main", func_00160B10);

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

INCLUDE_ASM(const s32, "core/main", func_00160E28);

s32 func_00160E48(s32 value)
{
    return value < 0 ? -value : value;
}

INCLUDE_ASM(const s32, "core/main", func_00160E60);

INCLUDE_ASM(const s32, "core/main", func_00160E88);

INCLUDE_ASM(const s32, "core/main", func_00160EB0);

INCLUDE_ASM(const s32, "core/main", func_00160ED0);

INCLUDE_ASM(const s32, "core/main", func_00161060);

INCLUDE_ASM(const s32, "core/main", func_001610A0);

INCLUDE_ASM(const s32, "core/main", func_001611A8);

INCLUDE_ASM(const s32, "core/main", func_00161230);

INCLUDE_ASM(const s32, "core/main", func_00161280);

INCLUDE_ASM(const s32, "core/main", func_001612C0);

INCLUDE_ASM(const s32, "core/main", func_001612E0);

INCLUDE_ASM(const s32, "core/main", func_00161300);

void func_00161320(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00161328);

INCLUDE_ASM(const s32, "core/main", func_00161358);

INCLUDE_ASM(const s32, "core/main", func_00161388);

INCLUDE_ASM(const s32, "core/main", func_001613D8);

INCLUDE_ASM(const s32, "core/main", func_00161468);

INCLUDE_ASM(const s32, "core/main", func_001614C0);

s32 func_001614D0(const s32 *p) { return p[9] * 8 + 0x60; }

INCLUDE_ASM(const s32, "core/main", func_001614E0);

INCLUDE_ASM(const s32, "core/main", func_00161548);

INCLUDE_ASM(const s32, "core/main", func_001615B0);

INCLUDE_ASM(const s32, "core/main", func_001615C0);

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

INCLUDE_ASM(const s32, "core/main", func_00162060);

INCLUDE_ASM(const s32, "core/main", func_001620E8);

s32 func_00162138(void) { return -1; }

s32 func_00162140(void) { return -1; }

INCLUDE_ASM(const s32, "core/main", func_00162148);

INCLUDE_ASM(const s32, "core/main", func_00162158);

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

INCLUDE_ASM(const s32, "core/main", func_00162438);

INCLUDE_ASM(const s32, "core/main", func_00162470);

INCLUDE_ASM(const s32, "core/main", func_001624A8);

INCLUDE_ASM(const s32, "core/main", func_001625A8);

INCLUDE_ASM(const s32, "core/main", func_001626A8);

void func_00162810(void) {
}

void func_00162818(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00162820);

INCLUDE_ASM(const s32, "core/main", func_00162950);

INCLUDE_ASM(const s32, "core/main", func_00162AA8);

INCLUDE_ASM(const s32, "core/main", func_00162B08);

INCLUDE_ASM(const s32, "core/main", func_00162B58);

INCLUDE_ASM(const s32, "core/main", func_00162B78);

void func_00162B98(s32 *p) { p[4] = 0; }

void func_00162BA0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00162BA8);

INCLUDE_ASM(const s32, "core/main", func_00162BD8);

INCLUDE_ASM(const s32, "core/main", func_00162C08);

INCLUDE_ASM(const s32, "core/main", func_00162C68);

INCLUDE_ASM(const s32, "core/main", func_00162CF8);

INCLUDE_ASM(const s32, "core/main", func_00162DF8);

INCLUDE_ASM(const s32, "core/main", func_00162E40);

INCLUDE_ASM(const s32, "core/main", func_00162E60);

INCLUDE_ASM(const s32, "core/main", func_00162E70);

INCLUDE_ASM(const s32, "core/main", func_00162EA0);

INCLUDE_ASM(const s32, "core/main", func_00162EF8);

INCLUDE_ASM(const s32, "core/main", func_00162F68);

INCLUDE_ASM(const s32, "core/main", func_00162FD8);

INCLUDE_ASM(const s32, "core/main", func_00163088);

INCLUDE_ASM(const s32, "core/main", func_00163400);

INCLUDE_ASM(const s32, "core/main", func_00163568);

INCLUDE_ASM(const s32, "core/main", func_001636D0);

INCLUDE_ASM(const s32, "core/main", func_00163758);

INCLUDE_ASM(const s32, "core/main", func_00163838);

INCLUDE_ASM(const s32, "core/main", func_00163878);

INCLUDE_ASM(const s32, "core/main", func_00163900);

INCLUDE_ASM(const s32, "core/main", func_00163988);

INCLUDE_ASM(const s32, "core/main", func_001639F0);

INCLUDE_ASM(const s32, "core/main", func_00163A30);

INCLUDE_ASM(const s32, "core/main", func_00163A70);

INCLUDE_ASM(const s32, "core/main", func_00163A98);

INCLUDE_ASM(const s32, "core/main", func_00163B88);

INCLUDE_ASM(const s32, "core/main", func_00163BC8);

INCLUDE_ASM(const s32, "core/main", func_00163C20);

INCLUDE_ASM(const s32, "core/main", func_00163C70);

INCLUDE_ASM(const s32, "core/main", func_00163CC8);

INCLUDE_ASM(const s32, "core/main", func_00163D20);

void func_00163D78(u32 value) { func_001654C8((u8)value); }

INCLUDE_ASM(const s32, "core/main", func_00163D80);

void func_00163DA0(void) {
}

s32 func_00163DA8(void) { return 0; }

void func_00163DB0(void) { func_00167550(); }

INCLUDE_ASM(const s32, "core/main", func_00163DB8);

INCLUDE_ASM(const s32, "core/main", func_00163DD8);

INCLUDE_ASM(const s32, "core/main", func_00163EC0);

INCLUDE_ASM(const s32, "core/main", func_00163EE0);

void func_00163F00(void) {
}

void func_00163F08(void) { func_001654C0(); }

void func_00163F10(void) { func_00165180(); }

INCLUDE_ASM(const s32, "core/main", func_00163F18);

INCLUDE_ASM(const s32, "core/main", func_00164080);

INCLUDE_ASM(const s32, "core/main", func_00164130);

INCLUDE_ASM(const s32, "core/main", func_001641F0);

INCLUDE_ASM(const s32, "core/main", func_001642B8);

INCLUDE_ASM(const s32, "core/main", func_001642E8);

INCLUDE_ASM(const s32, "core/main", func_00164318);

INCLUDE_ASM(const s32, "core/main", func_00164348);

INCLUDE_ASM(const s32, "core/main", func_00164378);

INCLUDE_ASM(const s32, "core/main", func_001643B8);

INCLUDE_ASM(const s32, "core/main", func_00164408);

INCLUDE_ASM(const s32, "core/main", func_001644C0);

INCLUDE_ASM(const s32, "core/main", func_00164510);

s32 func_00164540(void) { return -1; }

INCLUDE_ASM(const s32, "core/main", func_00164548);

INCLUDE_ASM(const s32, "core/main", func_00164648);

INCLUDE_ASM(const s32, "core/main", func_001646A8);

INCLUDE_ASM(const s32, "core/main", func_00164708);

INCLUDE_ASM(const s32, "core/main", func_00164768);

INCLUDE_ASM(const s32, "core/main", func_001647C8);

INCLUDE_ASM(const s32, "core/main", func_00164838);

INCLUDE_ASM(const s32, "core/main", func_00164878);

INCLUDE_ASM(const s32, "core/main", func_001648B8);

INCLUDE_ASM(const s32, "core/main", func_001648D8);

void func_001648F8(void) { func_00165418(); }

void func_00164900(void) { func_00165420(); }

void func_00164908(void) { func_00165468(); }

void func_00164910(void) { func_00165470(); }

INCLUDE_ASM(const s32, "core/main", func_00164918);

INCLUDE_ASM(const s32, "core/main", func_00164938);

s32 func_00164958(void) { return -1; }

s32 func_00164960(void) { return 0; }

void func_00164968(void) {
}

void func_00164970(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00164978);

INCLUDE_ASM(const s32, "core/main", func_001649A0);

INCLUDE_ASM(const s32, "core/main", func_00164B48);

INCLUDE_ASM(const s32, "core/main", func_00164C40);

INCLUDE_ASM(const s32, "core/main", func_00164CE8);

void func_00164E08(s32 *p) { func_001642B8(((s32 *)p)[0]); }

INCLUDE_ASM(const s32, "core/main", func_00164E10);

void func_00164EE0(s32 *p) { func_00164318(((s32 *)p)[0]); }

void func_00164EE8(s32 *p) { func_00164348(((s32 *)p)[0]); }

s32 func_00164EF0(const s32 *p) { return *(const s32 *)((const u8 *)p + (52)); }

INCLUDE_ASM(const s32, "core/main", func_00164EF8);

INCLUDE_ASM(const s32, "core/main", func_00164F28);

INCLUDE_ASM(const s32, "core/main", func_00164F50);

INCLUDE_ASM(const s32, "core/main", func_00164F90);

INCLUDE_ASM(const s32, "core/main", func_00164FB0);

INCLUDE_ASM(const s32, "core/main", func_00164FD0);

INCLUDE_ASM(const s32, "core/main", func_00165058);

void func_001650C0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001650C8);

INCLUDE_ASM(const s32, "core/main", func_00165100);

void func_00165120(void) { func_00167100(); }

void func_00165128(void) { func_00167108(); }

void func_00165130(void) { func_00167110(); }

INCLUDE_ASM(const s32, "core/main", func_00165138);

INCLUDE_ASM(const s32, "core/main", func_00165158);

void func_00165178(void) { func_001670D8(); }

void func_00165180(void) {
}

void func_00165188(void) {
}

void func_00165190(void) {
}

void func_00165198(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001651A0);

void func_001651C0(void) { func_00167368(); }

void func_001651C8(void) { func_001672B0(); }

INCLUDE_ASM(const s32, "core/main", func_001651D0);

INCLUDE_ASM(const s32, "core/main", func_001651F0);

INCLUDE_ASM(const s32, "core/main", func_00165288);

INCLUDE_ASM(const s32, "core/main", func_001652D0);

void func_00165310(void) { func_00167170(); }

void func_00165318(void) { func_00167178(); }

INCLUDE_ASM(const s32, "core/main", func_00165320);

INCLUDE_ASM(const s32, "core/main", func_00165340);

INCLUDE_ASM(const s32, "core/main", func_00165360);

INCLUDE_ASM(const s32, "core/main", func_001653C8);

INCLUDE_ASM(const s32, "core/main", func_001653E8);

void func_00165408(void) { func_001672F8(); }

void func_00165410(void) { func_00167300(); }

void func_00165418(void) { func_00167370(); }

void func_00165420(void) { func_00167378(); }

INCLUDE_ASM(const s32, "core/main", func_00165428);

INCLUDE_ASM(const s32, "core/main", func_00165448);

void func_00165468(void) { func_00167308(); }

void func_00165470(void) { func_00167338(); }

INCLUDE_ASM(const s32, "core/main", func_00165478);

void func_00165498(void) { func_00167380(); }

INCLUDE_ASM(const s32, "core/main", func_001654A0);

void func_001654C0(void) { func_00167148(); }

void func_001654C8(u32 value) { func_00167388((u8)value); }

INCLUDE_ASM(const s32, "core/main", func_001654D0);

void func_001654F0(void) { func_00167348(); }

void func_001654F8(void) {
}

s32 func_00165500(void) { return 1; }

void func_00165508(void) { func_00167358(); }

INCLUDE_ASM(const s32, "core/main", func_00165510);

INCLUDE_ASM(const s32, "core/main", func_00165530);

INCLUDE_ASM(const s32, "core/main", func_00165550);

s32 func_00165570(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00165578);

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

INCLUDE_ASM(const s32, "core/main", func_00165B20);

INCLUDE_ASM(const s32, "core/main", func_00165B50);

INCLUDE_ASM(const s32, "core/main", func_00165C30);

INCLUDE_ASM(const s32, "core/main", func_00165C78);

INCLUDE_ASM(const s32, "core/main", func_00165CC0);

INCLUDE_ASM(const s32, "core/main", func_00165D28);

INCLUDE_ASM(const s32, "core/main", func_00165D48);

INCLUDE_ASM(const s32, "core/main", func_00165D80);

INCLUDE_ASM(const s32, "core/main", func_00165DA0);

INCLUDE_ASM(const s32, "core/main", func_00165DC0);

INCLUDE_ASM(const s32, "core/main", func_00165DE0);

INCLUDE_ASM(const s32, "core/main", func_00165E08);

INCLUDE_ASM(const s32, "core/main", func_00165E30);

INCLUDE_ASM(const s32, "core/main", func_00165E58);

INCLUDE_ASM(const s32, "core/main", func_00165E88);

INCLUDE_ASM(const s32, "core/main", func_00165EC0);

INCLUDE_ASM(const s32, "core/main", func_00165F28);

INCLUDE_ASM(const s32, "core/main", func_00165F90);

INCLUDE_ASM(const s32, "core/main", func_00166008);

s32 func_00166030(const s32 *p) { return (p[0] & 1) ^ 1; }

INCLUDE_ASM(const s32, "core/main", func_00166040);

INCLUDE_ASM(const s32, "core/main", func_00166070);

INCLUDE_ASM(const s32, "core/main", func_001660C8);

INCLUDE_ASM(const s32, "core/main", func_00166120);

INCLUDE_ASM(const s32, "core/main", func_00166170);

INCLUDE_ASM(const s32, "core/main", func_001661A8);


/* Source: file_io/region_0016.c */
/* func_00166210 through func_00169628; retail address range 0x00166210-0x001697A0. */



s32 D_00371428[8];

INCLUDE_ASM(const s32, "core/main", func_00166210);

INCLUDE_ASM(const s32, "core/main", func_001662C8);

INCLUDE_ASM(const s32, "core/main", func_00166310);

INCLUDE_ASM(const s32, "core/main", func_00166378);

INCLUDE_ASM(const s32, "core/main", func_001663C0);

INCLUDE_ASM(const s32, "core/main", func_001663F8);

INCLUDE_ASM(const s32, "core/main", func_00166440);

INCLUDE_ASM(const s32, "core/main", func_00166518);

INCLUDE_ASM(const s32, "core/main", func_001665C8);

INCLUDE_ASM(const s32, "core/main", func_001669B0);

INCLUDE_ASM(const s32, "core/main", func_00166A18);

INCLUDE_ASM(const s32, "core/main", func_00166B00);

INCLUDE_ASM(const s32, "core/main", func_00166D50);

INCLUDE_ASM(const s32, "core/main", func_00166DE0);

INCLUDE_ASM(const s32, "core/main", func_00166E70);

INCLUDE_ASM(const s32, "core/main", func_00166ED8);

INCLUDE_ASM(const s32, "core/main", func_00166FA8);

INCLUDE_ASM(const s32, "core/main", func_00166FC8);

INCLUDE_ASM(const s32, "core/main", func_00167018);

INCLUDE_ASM(const s32, "core/main", func_00167058);

s32 func_00167098(void) { return 3; }

INCLUDE_ASM(const s32, "core/main", func_001670A0);

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

INCLUDE_ASM(const s32, "core/main", func_00167308);

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

INCLUDE_ASM(const s32, "core/main", func_001673B0);

void func_00167428(void) { func_00167A68(); }

INCLUDE_ASM(const s32, "core/main", func_00167430);

INCLUDE_ASM(const s32, "core/main", func_00167468);

void func_00167488(void) { func_00167AB0(); }

void func_00167490(void) { func_00167AB8(); }

void func_00167498(void) { func_00167AC0(); }

INCLUDE_ASM(const s32, "core/main", func_001674A0);

INCLUDE_ASM(const s32, "core/main", func_001674C0);

INCLUDE_ASM(const s32, "core/main", func_001674E0);

void func_00167500(u32 value) { func_00167B00((u8)value); }

INCLUDE_ASM(const s32, "core/main", func_00167508);

void func_00167528(void) { func_00167F98(); }

INCLUDE_ASM(const s32, "core/main", func_00167530);

void func_00167550(void) { func_00167B10(); }

INCLUDE_ASM(const s32, "core/main", func_00167558);

INCLUDE_ASM(const s32, "core/main", func_00167578);

INCLUDE_ASM(const s32, "core/main", func_00167598);

void func_001675B8(void) { func_00167B78(); }

void func_001675C0(void) { func_00167B80(); }

void func_001675C8(void) {
}

void func_001675D0(void) {
}

void func_001675D8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001675E0);

INCLUDE_ASM(const s32, "core/main", func_00167678);

void func_001676B0(void) { func_00167BB0(); }

void func_001676B8(void) { func_00167BB8(); }

void func_001676C0(void) { func_00167BC0(); }

INCLUDE_ASM(const s32, "core/main", func_001676C8);

INCLUDE_ASM(const s32, "core/main", func_001676E8);

INCLUDE_ASM(const s32, "core/main", func_00167708);

INCLUDE_ASM(const s32, "core/main", func_00167728);

void func_00167748(void) { func_00167E78(); }

INCLUDE_ASM(const s32, "core/main", func_00167750);

INCLUDE_ASM(const s32, "core/main", func_00167770);

INCLUDE_ASM(const s32, "core/main", func_001677E8);

INCLUDE_ASM(const s32, "core/main", func_00167820);

INCLUDE_ASM(const s32, "core/main", func_00167840);

INCLUDE_ASM(const s32, "core/main", func_00167860);

void func_00167880(void) { func_00167F80(); }

void func_00167888(void) { func_00167F88(); }

void func_00167890(void) { func_00167F90(); }

INCLUDE_ASM(const s32, "core/main", func_00167898);

void func_001678B8(void) { func_00167FD0(); }

void func_001678C0(void) { func_00167FD8(); }

void func_001678C8(void) { func_00167FE0(); }

void func_001678D0(void) { func_00167FE8(); }

INCLUDE_ASM(const s32, "core/main", func_001678D8);

void func_001678F8(void) { func_00167FC0(); }

INCLUDE_ASM(const s32, "core/main", func_00167900);

INCLUDE_ASM(const s32, "core/main", func_00167930);

INCLUDE_ASM(const s32, "core/main", func_00167950);

void func_00167970(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00167978);

INCLUDE_ASM(const s32, "core/main", func_00167998);

s32 func_001679B8(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001679C0);

s32 func_001679E0(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001679E8);

void func_00167A68(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00167A70);

INCLUDE_ASM(const s32, "core/main", func_00167A90);

void func_00167AB0(void) { func_001680A0(); }

void func_00167AB8(void) { func_001681E8(); }

void func_00167AC0(void) { func_001681F8(); }

s32 func_00167AC8(const u8 *p) {
    register s32 value __asm__("$2") = p[0x14];
    __asm__ volatile ("dsrl %0, %0, 7" : "+r"(value));
    return -value;
}

INCLUDE_ASM(const s32, "core/main", func_00167AD8);

s32 func_00167AF8(void) { return -1; }

void func_00167B00(void) {
}

s32 func_00167B08(void) { return 0; }

void func_00167B10(void) {
}

s32 func_00167B18(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00167B20);

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

INCLUDE_ASM(const s32, "core/main", func_00167F10);

INCLUDE_ASM(const s32, "core/main", func_00167F30);

INCLUDE_ASM(const s32, "core/main", func_00167F58);

void func_00167F80(void) { func_001680A0(); }

void func_00167F88(void) { func_001681E8(); }

void func_00167F90(void) { func_001681F8(); }

void func_00167F98(void) { func_00168230(); }

INCLUDE_ASM(const s32, "core/main", func_00167FA0);

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

INCLUDE_ASM(const s32, "core/main", func_00168028);

INCLUDE_ASM(const s32, "core/main", func_00168048);

INCLUDE_ASM(const s32, "core/main", func_00168068);

INCLUDE_ASM(const s32, "core/main", func_001680A0);

INCLUDE_ASM(const s32, "core/main", func_001680C8);

INCLUDE_ASM(const s32, "core/main", func_001680F0);

INCLUDE_ASM(const s32, "core/main", func_00168138);

INCLUDE_ASM(const s32, "core/main", func_00168160);

INCLUDE_ASM(const s32, "core/main", func_00168188);

INCLUDE_ASM(const s32, "core/main", func_001681B0);

void func_001681E8(s32 arg)
{
    func_00168A10((s32)D_00371428, arg);
}

void func_001681F8(s32 arg)
{
    func_00168A40((s32)D_00371428, arg);
}

INCLUDE_ASM(const s32, "core/main", func_00168208);

void func_00168230(s32 arg)
{
    func_00168AA8((s32)D_00371428, arg);
}

INCLUDE_ASM(const s32, "core/main", func_00168240);

INCLUDE_ASM(const s32, "core/main", func_00168260);

INCLUDE_ASM(const s32, "core/main", func_00168270);

INCLUDE_ASM(const s32, "core/main", func_001682E8);

INCLUDE_ASM(const s32, "core/main", func_00168310);

s32 func_00168338(const s32 *p) { return (p[0] & 1) ^ 1; }

INCLUDE_ASM(const s32, "core/main", func_00168348);

INCLUDE_ASM(const s32, "core/main", func_00168390);

INCLUDE_ASM(const s32, "core/main", func_001683D8);

INCLUDE_ASM(const s32, "core/main", func_00168470);

INCLUDE_ASM(const s32, "core/main", func_001684D8);

INCLUDE_ASM(const s32, "core/main", func_00168508);

INCLUDE_ASM(const s32, "core/main", func_00168598);

INCLUDE_ASM(const s32, "core/main", func_00168638);

INCLUDE_ASM(const s32, "core/main", func_00168668);

INCLUDE_ASM(const s32, "core/main", func_001686D8);

INCLUDE_ASM(const s32, "core/main", func_00168710);

void func_00168720(void)
{
    func_00224B88((s32)D_003E1A30, 0, 0xC0);
}

INCLUDE_ASM(const s32, "core/main", func_00168738);

INCLUDE_ASM(const s32, "core/main", func_001688D0);

INCLUDE_ASM(const s32, "core/main", func_00168938);

INCLUDE_ASM(const s32, "core/main", func_001689C0);

INCLUDE_ASM(const s32, "core/main", func_00168A10);

INCLUDE_ASM(const s32, "core/main", func_00168A40);

INCLUDE_ASM(const s32, "core/main", func_00168A70);

INCLUDE_ASM(const s32, "core/main", func_00168AA8);

INCLUDE_ASM(const s32, "core/main", func_00168AE0);

INCLUDE_ASM(const s32, "core/main", func_00168B18);

INCLUDE_ASM(const s32, "core/main", func_00168B38);

INCLUDE_ASM(const s32, "core/main", func_00168BC0);

INCLUDE_ASM(const s32, "core/main", func_00168BE8);

INCLUDE_ASM(const s32, "core/main", func_00168C18);

INCLUDE_ASM(const s32, "core/main", func_00168C38);

INCLUDE_ASM(const s32, "core/main", func_00168C68);

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

INCLUDE_ASM(const s32, "core/main", func_00169AF8);

INCLUDE_ASM(const s32, "core/main", func_00169B08);

INCLUDE_ASM(const s32, "core/main", func_00169B18);

INCLUDE_ASM(const s32, "core/main", func_00169B40);

void func_00169B68(s32 arg)
{
    func_00169EB8((s32)D_003E1AF0, arg);
}

INCLUDE_ASM(const s32, "core/main", func_00169B78);

INCLUDE_ASM(const s32, "core/main", func_00169B88);

INCLUDE_ASM(const s32, "core/main", func_00169BB0);

void func_00169BD0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00169BD8);

INCLUDE_ASM(const s32, "core/main", func_00169C20);

INCLUDE_ASM(const s32, "core/main", func_00169CB0);

INCLUDE_ASM(const s32, "core/main", func_00169D90);

INCLUDE_ASM(const s32, "core/main", func_00169DD0);

INCLUDE_ASM(const s32, "core/main", func_00169EB8);

INCLUDE_ASM(const s32, "core/main", func_00169F18);

INCLUDE_ASM(const s32, "core/main", func_00169F50);

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

INCLUDE_ASM(const s32, "core/main", func_0016AF78);

INCLUDE_ASM(const s32, "core/main", func_0016B2B0);

INCLUDE_ASM(const s32, "core/main", func_0016BBF8);

INCLUDE_ASM(const s32, "core/main", func_0016BCA0);

INCLUDE_ASM(const s32, "core/main", func_0016BD48);

INCLUDE_ASM(const s32, "core/main", func_0016BDC0);

INCLUDE_ASM(const s32, "core/main", func_0016BDF8);

INCLUDE_ASM(const s32, "core/main", func_0016BE90);

INCLUDE_ASM(const s32, "core/main", func_0016BF10);

INCLUDE_ASM(const s32, "core/main", func_0016BF38);

INCLUDE_ASM(const s32, "core/main", func_0016BF68);

INCLUDE_ASM(const s32, "core/main", func_0016C020);

INCLUDE_ASM(const s32, "core/main", func_0016C0B8);

INCLUDE_ASM(const s32, "core/main", func_0016C150);

INCLUDE_ASM(const s32, "core/main", func_0016C290);

INCLUDE_ASM(const s32, "core/main", func_0016C348);

s32 func_0016C368(u8 *p)
{
    s32 field = *(s32 *)(p + 0x18);

    return field == (s32)func_0016C7F8 ? 0 : field;
}

INCLUDE_ASM(const s32, "core/main", func_0016C380);

INCLUDE_ASM(const s32, "core/main", func_0016C3B0);

INCLUDE_ASM(const s32, "core/main", func_0016C400);

INCLUDE_ASM(const s32, "core/main", func_0016C468);

INCLUDE_ASM(const s32, "core/main", func_0016C498);

INCLUDE_ASM(const s32, "core/main", func_0016C548);

INCLUDE_ASM(const s32, "core/main", func_0016C590);

INCLUDE_ASM(const s32, "core/main", func_0016C5D8);

INCLUDE_ASM(const s32, "core/main", func_0016C610);

INCLUDE_ASM(const s32, "core/main", func_0016C648);

INCLUDE_ASM(const s32, "core/main", func_0016C670);

s32 func_0016C698(void) { return 1; }

s32 func_0016C6A0(void) { return 1; }

u8 *func_0016C6A8(u8 *p, s32 *counter)
{
    (*counter)++;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0016C6C0);

INCLUDE_ASM(const s32, "core/main", func_0016C708);

INCLUDE_ASM(const s32, "core/main", func_0016C758);

s32 func_0016C7F8(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_0016C800);

INCLUDE_ASM(const s32, "core/main", func_0016C828);

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

INCLUDE_ASM(const s32, "core/main", func_0016DE88);

INCLUDE_ASM(const s32, "core/main", func_0016DE98);

INCLUDE_ASM(const s32, "core/main", func_0016DF70);

INCLUDE_ASM(const s32, "core/main", func_0016DFE8);

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

INCLUDE_ASM(const s32, "core/main", func_0016E178);

INCLUDE_ASM(const s32, "core/main", func_0016E1B0);

u8 *func_0016E1D8(u8 *p) { return p + 4; }

INCLUDE_ASM(const s32, "core/main", func_0016E1E0);

INCLUDE_ASM(const s32, "core/main", func_0016E208);

s32 func_0016E280(const s32 *p) { return *(const s32 *)((const u8 *)p + (36)); }

s32 func_0016E288(const s32 *p) { return *(const s32 *)((const u8 *)p + (40)); }

s32 func_0016E290(const s32 *p) { return *(const s32 *)((const u8 *)p + (44)); }

s32 func_0016E298(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_0016E2A0(const s32 *p) { return *(const s32 *)((const u8 *)p + (24)); }

INCLUDE_ASM(const s32, "core/main", func_0016E2A8);

INCLUDE_ASM(const s32, "core/main", func_0016E330);

s32 func_0016E380(u8 *p, s32 index)
{
    return *(s32 *)(*(u32 *)(p + 0xC) + index * 4);
}

INCLUDE_ASM(const s32, "core/main", func_0016E398);

u8 *func_0016E3E0(u8 *p, u8 *out, u16 a, u16 b, u16 c)
{
    *(u16 *)out = a;
    *(u16 *)(out + 0x2) = b;
    *(u16 *)(out + 0x4) = c;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0016E3F8);

INCLUDE_ASM(const s32, "core/main", func_0016E4E8);

INCLUDE_ASM(const s32, "core/main", func_0016E520);

INCLUDE_ASM(const s32, "core/main", func_0016E548);

INCLUDE_ASM(const s32, "core/main", func_0016E668);

INCLUDE_ASM(const s32, "core/main", func_0016E740);

INCLUDE_ASM(const s32, "core/main", func_0016E7F8);

INCLUDE_ASM(const s32, "core/main", func_0016E840);

INCLUDE_ASM(const s32, "core/main", func_0016E878);

s32 func_0016E8A0(void) { return 1; }

void func_0016E8A8(void) { func_0016EEA0(); }

INCLUDE_ASM(const s32, "core/main", func_0016E8B0);

INCLUDE_ASM(const s32, "core/main", func_0016E9F8);

INCLUDE_ASM(const s32, "core/main", func_0016EBB0);

INCLUDE_ASM(const s32, "core/main", func_0016EC98);

INCLUDE_ASM(const s32, "core/main", func_0016ED00);

INCLUDE_ASM(const s32, "core/main", func_0016ED78);

INCLUDE_ASM(const s32, "core/main", func_0016EDC0);

INCLUDE_ASM(const s32, "core/main", func_0016EEA0);

INCLUDE_ASM(const s32, "core/main", func_0016EF00);

s32 func_0016EF28(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

u8 func_0016EF30(const u8 *p) { return p[1]; }

INCLUDE_ASM(const s32, "core/main", func_0016EF38);

u8 func_0016EF48(const u8 *p) { return p[2]; }

f32 func_0016EF50(const f32 *p) { return p[5]; }

INCLUDE_ASM(const s32, "core/main", func_0016EF58);

u8 *func_0016EFA0(u8 *p) { return p + 24; }

INCLUDE_ASM(const s32, "core/main", func_0016EFA8);

INCLUDE_ASM(const s32, "core/main", func_0016F008);

INCLUDE_ASM(const s32, "core/main", func_0016F028);

INCLUDE_ASM(const s32, "core/main", func_0016F058);

INCLUDE_ASM(const s32, "core/main", func_0016F080);

INCLUDE_ASM(const s32, "core/main", func_0016F0C8);

INCLUDE_ASM(const s32, "core/main", func_0016F100);

s32 func_0016F128(void) { return 1; }

s32 func_0016F130(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_0016F138);

INCLUDE_ASM(const s32, "core/main", func_0016F280);

INCLUDE_ASM(const s32, "core/main", func_0016F620);

INCLUDE_ASM(const s32, "core/main", func_0016F688);

INCLUDE_ASM(const s32, "core/main", func_0016F6F0);

INCLUDE_ASM(const s32, "core/main", func_0016F738);

INCLUDE_ASM(const s32, "core/main", func_0016F780);

INCLUDE_ASM(const s32, "core/main", func_0016F830);

u8 *func_0016F8C0(u8 *p)
{
    *(u16 *)(p + 0x18) += 1;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0016F8D8);

s32 func_0016F930(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

INCLUDE_ASM(const s32, "core/main", func_0016F938);

u8 *func_0016F950(u8 *p) { return p + 4; }

INCLUDE_ASM(const s32, "core/main", func_0016F958);

u8 *func_0016F978(u8 *p) { return p + 12; }

INCLUDE_ASM(const s32, "core/main", func_0016F980);

INCLUDE_ASM(const s32, "core/main", func_0016F9C8);

INCLUDE_ASM(const s32, "core/main", func_0016FA00);

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

INCLUDE_ASM(const s32, "core/main", func_00170E88);

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

INCLUDE_ASM(const s32, "core/main", func_00175288);

INCLUDE_ASM(const s32, "core/main", func_001752F0);

INCLUDE_ASM(const s32, "core/main", func_00175390);

INCLUDE_ASM(const s32, "core/main", func_00175418);

INCLUDE_ASM(const s32, "core/main", func_001754A0);

INCLUDE_ASM(const s32, "core/main", func_00175528);

INCLUDE_ASM(const s32, "core/main", func_001755B0);

INCLUDE_ASM(const s32, "core/main", func_00175678);

INCLUDE_ASM(const s32, "core/main", func_00175718);

INCLUDE_ASM(const s32, "core/main", func_00175758);

s32 func_00175768(const s32 *p) { return *(const s32 *)((const u8 *)p + (8)); }

INCLUDE_ASM(const s32, "core/main", func_00175770);

s32 func_001757B8(u8 *p, s32 index)
{
    return *(s32 *)(*(u32 *)(p + 0xC) + index * 4);
}

s32 func_001757D0(const s32 *p) { return *(const s32 *)((const u8 *)p + (16)); }

s32 func_001757D8(const s32 *p) { return *(const s32 *)((const u8 *)p + (28)); }

u8 *func_001757E0(u8 *p) { return p + 72; }

u8 *func_001757E8(u8 *p) { return p + 60; }

INCLUDE_ASM(const s32, "core/main", func_001757F0);

u8 *func_00175810(u8 *p) { return p + 96; }

INCLUDE_ASM(const s32, "core/main", func_00175818);

s32 func_00175828(const s32 *p) { return *(const s32 *)((const u8 *)p + (108)); }

INCLUDE_ASM(const s32, "core/main", func_00175830);

INCLUDE_ASM(const s32, "core/main", func_00175900);

u8 func_00175910(const u8 *p) { return p[2]; }

INCLUDE_ASM(const s32, "core/main", func_00175918);

INCLUDE_ASM(const s32, "core/main", func_00175968);

INCLUDE_ASM(const s32, "core/main", func_001759B0);

INCLUDE_ASM(const s32, "core/main", func_001759E8);

s32 func_00175A10(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00175A18);

INCLUDE_ASM(const s32, "core/main", func_00175A38);

INCLUDE_ASM(const s32, "core/main", func_00175A70);

INCLUDE_ASM(const s32, "core/main", func_00175AE0);

INCLUDE_ASM(const s32, "core/main", func_00175B70);

INCLUDE_ASM(const s32, "core/main", func_00175DC0);

INCLUDE_ASM(const s32, "core/main", func_00176118);

INCLUDE_ASM(const s32, "core/main", func_00176410);

INCLUDE_ASM(const s32, "core/main", func_00176590);

INCLUDE_ASM(const s32, "core/main", func_001766B0);

INCLUDE_ASM(const s32, "core/main", func_001767E8);

INCLUDE_ASM(const s32, "core/main", func_001769A0);

INCLUDE_ASM(const s32, "core/main", func_00176A10);

s32 func_00176A60(const u8 *p) { return *(const s32 *)(p + D_00393654 + 0xC); }

INCLUDE_ASM(const s32, "core/main", func_00176A70);

INCLUDE_ASM(const s32, "core/main", func_00176B58);

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

INCLUDE_ASM(const s32, "core/main", func_001773E8);

INCLUDE_ASM(const s32, "core/main", func_00177438);

INCLUDE_ASM(const s32, "core/main", func_001774A8);

s32 func_001774E0(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_001774E8);

u8 *func_001775C8(u8 *p)
{
    *(s32 *)(p + D_0039365C) = 0;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_001775E0);

s32 func_00177620(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_00177628);

INCLUDE_ASM(const s32, "core/main", func_00177650);

INCLUDE_ASM(const s32, "core/main", func_00177690);

INCLUDE_ASM(const s32, "core/main", func_00177738);

INCLUDE_ASM(const s32, "core/main", func_00177768);

INCLUDE_ASM(const s32, "core/main", func_00177798);

INCLUDE_ASM(const s32, "core/main", func_001777C8);

INCLUDE_ASM(const s32, "core/main", func_00177810);

INCLUDE_ASM(const s32, "core/main", func_00177830);

INCLUDE_ASM(const s32, "core/main", func_00177878);

INCLUDE_ASM(const s32, "core/main", func_001778D0);

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

INCLUDE_ASM(const s32, "core/main", func_00178938);

INCLUDE_ASM(const s32, "core/main", func_00178988);

INCLUDE_ASM(const s32, "core/main", func_001789D8);

INCLUDE_ASM(const s32, "core/main", func_00178A28);

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

INCLUDE_ASM(const s32, "core/main", func_00178AE0);

INCLUDE_ASM(const s32, "core/main", func_00178B30);

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

INCLUDE_ASM(const s32, "core/main", func_00178C40);

INCLUDE_ASM(const s32, "core/main", func_00178C90);

INCLUDE_ASM(const s32, "core/main", func_00178CD8);

INCLUDE_ASM(const s32, "core/main", func_00178E40);

INCLUDE_ASM(const s32, "core/main", func_00178E50);

INCLUDE_ASM(const s32, "core/main", func_00178EC8);

INCLUDE_ASM(const s32, "core/main", func_00178F68);

INCLUDE_ASM(const s32, "core/main", func_001792C8);

INCLUDE_ASM(const s32, "core/main", func_00179410);

INCLUDE_ASM(const s32, "core/main", func_001795F0);

INCLUDE_ASM(const s32, "core/main", func_00179BD0);

INCLUDE_ASM(const s32, "core/main", func_00179BE0);

INCLUDE_ASM(const s32, "core/main", func_00179DE8);

INCLUDE_ASM(const s32, "core/main", func_0017A1A0);

INCLUDE_ASM(const s32, "core/main", func_0017A6B8);

INCLUDE_ASM(const s32, "core/main", func_0017ACB0);

INCLUDE_ASM(const s32, "core/main", func_0017B300);

INCLUDE_ASM(const s32, "core/main", func_0017B4D0);

INCLUDE_ASM(const s32, "core/main", func_0017B6E8);

INCLUDE_ASM(const s32, "core/main", func_0017B6F8);

INCLUDE_ASM(const s32, "core/main", func_0017B878);

INCLUDE_ASM(const s32, "core/main", func_0017B888);

INCLUDE_ASM(const s32, "core/main", func_0017BCC8);

INCLUDE_ASM(const s32, "core/main", func_0017BCD8);

INCLUDE_ASM(const s32, "core/main", func_0017BFB8);

INCLUDE_ASM(const s32, "core/main", func_0017BFC8);

INCLUDE_ASM(const s32, "core/main", func_0017C040);

INCLUDE_ASM(const s32, "core/main", func_0017C050);

INCLUDE_ASM(const s32, "core/main", func_0017C0C8);

INCLUDE_ASM(const s32, "core/main", func_0017C1B0);

INCLUDE_ASM(const s32, "core/main", func_0017C390);

INCLUDE_ASM(const s32, "core/main", func_0017C5B0);

INCLUDE_ASM(const s32, "core/main", func_0017C8F0);

INCLUDE_ASM(const s32, "core/main", func_0017CEB8);

INCLUDE_ASM(const s32, "core/main", func_0017CEC8);

INCLUDE_ASM(const s32, "core/main", func_0017D2C8);

INCLUDE_ASM(const s32, "core/main", func_0017D538);

INCLUDE_ASM(const s32, "core/main", func_0017D6C0);

void func_0017D870(void) { func_001825C8(); }

INCLUDE_ASM(const s32, "core/main", func_0017D878);

INCLUDE_ASM(const s32, "core/main", func_0017D8D8);

INCLUDE_ASM(const s32, "core/main", func_0017D8F8);

INCLUDE_ASM(const s32, "core/main", func_0017D918);

INCLUDE_ASM(const s32, "core/main", func_0017DB40);

INCLUDE_ASM(const s32, "core/main", func_0017DD68);

INCLUDE_ASM(const s32, "core/main", func_0017E400);

INCLUDE_ASM(const s32, "core/main", func_0017E4B8);

INCLUDE_ASM(const s32, "core/main", func_0017E4C8);

INCLUDE_ASM(const s32, "core/main", func_0017E500);

INCLUDE_ASM(const s32, "core/main", func_0017E530);

INCLUDE_ASM(const s32, "core/main", func_0017E570);

INCLUDE_ASM(const s32, "core/main", func_0017EB10);

INCLUDE_ASM(const s32, "core/main", func_0017EE40);

INCLUDE_ASM(const s32, "core/main", func_0017F388);

INCLUDE_ASM(const s32, "core/main", func_0017FF38);


/* Source: rendering/region_0018.c */
/* func_00180F28 through func_0018FFB0; retail address range 0x00180000-0x00190000. */



extern s32 D_00393748;

INCLUDE_ASM(const s32, "core/main", func_00180F28);

INCLUDE_ASM(const s32, "core/main", func_001813E8);

INCLUDE_ASM(const s32, "core/main", func_00181590);

INCLUDE_ASM(const s32, "core/main", func_001816F0);

INCLUDE_ASM(const s32, "core/main", func_00181700);

INCLUDE_ASM(const s32, "core/main", func_001817E0);

INCLUDE_ASM(const s32, "core/main", func_00181820);

INCLUDE_ASM(const s32, "core/main", func_00181CB0);

INCLUDE_ASM(const s32, "core/main", func_001820E0);

INCLUDE_ASM(const s32, "core/main", func_00182238);

INCLUDE_ASM(const s32, "core/main", func_00182578);

s32 *func_001825B8(s32 *p, s32 value) { ((s32 *)p[5])[0] = value; return p; }

INCLUDE_ASM(const s32, "core/main", func_001825C8);

INCLUDE_ASM(const s32, "core/main", func_001825D8);

INCLUDE_ASM(const s32, "core/main", func_001825F8);

INCLUDE_ASM(const s32, "core/main", func_001826C8);

INCLUDE_ASM(const s32, "core/main", func_001827F0);

INCLUDE_ASM(const s32, "core/main", func_00182838);

INCLUDE_ASM(const s32, "core/main", func_00182880);

INCLUDE_ASM(const s32, "core/main", func_001828F0);

INCLUDE_ASM(const s32, "core/main", func_00182910);

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

INCLUDE_ASM(const s32, "core/main", func_001835C8);

s8 *func_00183608(u8 *p, s32 unused, s32 offset) { *(s32 *)(p + offset) = 0; return (s8 *)p; }

INCLUDE_ASM(const s32, "core/main", func_00183618);

INCLUDE_ASM(const s32, "core/main", func_00183648);

INCLUDE_ASM(const s32, "core/main", func_00183690);

INCLUDE_ASM(const s32, "core/main", func_001836B0);

INCLUDE_ASM(const s32, "core/main", func_001836F8);

INCLUDE_ASM(const s32, "core/main", func_00183760);

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

INCLUDE_ASM(const s32, "core/main", func_00185238);

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

INCLUDE_ASM(const s32, "core/main", func_00188B28);

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

INCLUDE_ASM(const s32, "core/main", func_00188D28);

INCLUDE_ASM(const s32, "core/main", func_00188D50);

u8 *func_00188D98(u8 *p) { return p + 52; }

INCLUDE_ASM(const s32, "core/main", func_00188DA0);

INCLUDE_ASM(const s32, "core/main", func_00188DE8);

INCLUDE_ASM(const s32, "core/main", func_00188E20);

s32 func_00188E48(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00188E50);

INCLUDE_ASM(const s32, "core/main", func_00188ED8);

INCLUDE_ASM(const s32, "core/main", func_00188FD0);

INCLUDE_ASM(const s32, "core/main", func_001890C0);

INCLUDE_ASM(const s32, "core/main", func_001890F8);

INCLUDE_ASM(const s32, "core/main", func_00189198);

INCLUDE_ASM(const s32, "core/main", func_00189220);

INCLUDE_ASM(const s32, "core/main", func_00189268);

INCLUDE_ASM(const s32, "core/main", func_001893D0);

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

INCLUDE_ASM(const s32, "core/main", func_00189F50);

INCLUDE_ASM(const s32, "core/main", func_00189F88);

INCLUDE_ASM(const s32, "core/main", func_0018A060);

INCLUDE_ASM(const s32, "core/main", func_0018A0A0);

INCLUDE_ASM(const s32, "core/main", func_0018A0E0);

INCLUDE_ASM(const s32, "core/main", func_0018A168);

INCLUDE_ASM(const s32, "core/main", func_0018A1C0);

INCLUDE_ASM(const s32, "core/main", func_0018A3C0);

INCLUDE_ASM(const s32, "core/main", func_0018A610);

INCLUDE_ASM(const s32, "core/main", func_0018A850);

INCLUDE_ASM(const s32, "core/main", func_0018A998);

INCLUDE_ASM(const s32, "core/main", func_0018AC20);

INCLUDE_ASM(const s32, "core/main", func_0018AC48);

INCLUDE_ASM(const s32, "core/main", func_0018AC80);

INCLUDE_ASM(const s32, "core/main", func_0018AD18);

INCLUDE_ASM(const s32, "core/main", func_0018AD50);

INCLUDE_ASM(const s32, "core/main", func_0018ADF8);

INCLUDE_ASM(const s32, "core/main", func_0018AEC8);

INCLUDE_ASM(const s32, "core/main", func_0018AF00);

INCLUDE_ASM(const s32, "core/main", func_0018AF78);

u8 *func_0018B008(u8 *p, s32 *counter)
{
    (*counter)++;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0018B020);

INCLUDE_ASM(const s32, "core/main", func_0018B108);

INCLUDE_ASM(const s32, "core/main", func_0018B130);

INCLUDE_ASM(const s32, "core/main", func_0018BA30);

INCLUDE_ASM(const s32, "core/main", func_0018BC08);

INCLUDE_ASM(const s32, "core/main", func_0018BFB0);

INCLUDE_ASM(const s32, "core/main", func_0018C180);

INCLUDE_ASM(const s32, "core/main", func_0018C2D0);

INCLUDE_ASM(const s32, "core/main", func_0018C450);

INCLUDE_ASM(const s32, "core/main", func_0018C470);

INCLUDE_ASM(const s32, "core/main", func_0018C490);

INCLUDE_ASM(const s32, "core/main", func_0018C4A0);

INCLUDE_ASM(const s32, "core/main", func_0018C4D8);

INCLUDE_ASM(const s32, "core/main", func_0018C508);

INCLUDE_ASM(const s32, "core/main", func_0018C550);

INCLUDE_ASM(const s32, "core/main", func_0018C5B0);

s32 func_0018C5D8(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

INCLUDE_ASM(const s32, "core/main", func_0018C5E0);

s32 func_0018C5F0(const s32 *p) { return *(const s32 *)((const u8 *)p + (96)); }

INCLUDE_ASM(const s32, "core/main", func_0018C5F8);

s32 func_0018C608(const s32 *p) { return *(const s32 *)((const u8 *)p + (100)); }

INCLUDE_ASM(const s32, "core/main", func_0018C610);

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

INCLUDE_ASM(const s32, "core/main", func_0018C9E0);

f32 func_0018C9F0(const f32 *p) { return p[34]; }

INCLUDE_ASM(const s32, "core/main", func_0018C9F8);

INCLUDE_ASM(const s32, "core/main", func_0018CA40);

s32 func_0018CA68(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0018CA70);

INCLUDE_ASM(const s32, "core/main", func_0018CAF8);

INCLUDE_ASM(const s32, "core/main", func_0018CB60);

INCLUDE_ASM(const s32, "core/main", func_0018CBC8);

INCLUDE_ASM(const s32, "core/main", func_0018CC50);

INCLUDE_ASM(const s32, "core/main", func_0018CCB0);

INCLUDE_ASM(const s32, "core/main", func_0018CD08);

INCLUDE_ASM(const s32, "core/main", func_0018CED8);

INCLUDE_ASM(const s32, "core/main", func_0018CF88);

INCLUDE_ASM(const s32, "core/main", func_0018D088);

s32 func_0018D1C8(void) { return 772; }

INCLUDE_ASM(const s32, "core/main", func_0018D1D0);

INCLUDE_ASM(const s32, "core/main", func_0018D2E8);

INCLUDE_ASM(const s32, "core/main", func_0018D328);

INCLUDE_ASM(const s32, "core/main", func_0018D350);

INCLUDE_ASM(const s32, "core/main", func_0018D3D8);

INCLUDE_ASM(const s32, "core/main", func_0018D428);

INCLUDE_ASM(const s32, "core/main", func_0018D478);

INCLUDE_ASM(const s32, "core/main", func_0018D520);

INCLUDE_ASM(const s32, "core/main", func_0018D560);

INCLUDE_ASM(const s32, "core/main", func_0018D5D0);

INCLUDE_ASM(const s32, "core/main", func_0018D638);

INCLUDE_ASM(const s32, "core/main", func_0018D6A8);

INCLUDE_ASM(const s32, "core/main", func_0018D710);

INCLUDE_ASM(const s32, "core/main", func_0018D770);

INCLUDE_ASM(const s32, "core/main", func_0018D7D8);

INCLUDE_ASM(const s32, "core/main", func_0018D840);

INCLUDE_ASM(const s32, "core/main", func_0018D8A0);

INCLUDE_ASM(const s32, "core/main", func_0018D8B0);

s32 func_0018D9A0(void) { return D_00393748; }

INCLUDE_ASM(const s32, "core/main", func_0018D9A8);

INCLUDE_ASM(const s32, "core/main", func_0018D9D0);

INCLUDE_ASM(const s32, "core/main", func_0018DA08);

INCLUDE_ASM(const s32, "core/main", func_0018DA48);

u8 *func_0018DA78(u8 *p)
{
    D_00393CF4--;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0018DA90);

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

INCLUDE_ASM(const s32, "core/main", func_0018E4A8);

INCLUDE_ASM(const s32, "core/main", func_0018E518);

s32 func_0018E618(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

s32 func_0018E620(const s32 *p) { return *(const s32 *)((const u8 *)p + (168)); }

INCLUDE_ASM(const s32, "core/main", func_0018E628);

u8 *func_0018E668(u8 *p) { return p + 16; }

INCLUDE_ASM(const s32, "core/main", func_0018E670);

INCLUDE_ASM(const s32, "core/main", func_0018E6D0);

s32 func_0018E7B0(s32 *p) { return ((u8 *)p[0x2A])[3] & 3; }

INCLUDE_ASM(const s32, "core/main", func_0018E7C0);

INCLUDE_ASM(const s32, "core/main", func_0018E810);

INCLUDE_ASM(const s32, "core/main", func_0018E858);

INCLUDE_ASM(const s32, "core/main", func_0018E8A0);

s32 func_0018E8C8(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_0018E8D0);

INCLUDE_ASM(const s32, "core/main", func_0018E910);

INCLUDE_ASM(const s32, "core/main", func_0018E9A8);

INCLUDE_ASM(const s32, "core/main", func_0018EA10);

INCLUDE_ASM(const s32, "core/main", func_0018EA88);

INCLUDE_ASM(const s32, "core/main", func_0018EAE0);

INCLUDE_ASM(const s32, "core/main", func_0018EBE8);

void func_0018EC70(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0018EC78);

INCLUDE_ASM(const s32, "core/main", func_0018EC88);

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

INCLUDE_ASM(const s32, "core/main", func_001906E0);

INCLUDE_ASM(const s32, "core/main", func_00190728);

INCLUDE_ASM(const s32, "core/main", func_001907C0);

INCLUDE_ASM(const s32, "core/main", func_00190830);

INCLUDE_ASM(const s32, "core/main", func_00190938);

INCLUDE_ASM(const s32, "core/main", func_00190988);

INCLUDE_ASM(const s32, "core/main", func_00190A88);

s32 func_00190B68(void)
{
    s32 index = D_00393D00;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x04);
}

INCLUDE_ASM(const s32, "core/main", func_00190B80);

INCLUDE_ASM(const s32, "core/main", func_00190C60);

INCLUDE_ASM(const s32, "core/main", func_00190C70);

INCLUDE_ASM(const s32, "core/main", func_00190C80);

s32 func_00190C90(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

s32 func_00190C98(const s32 *p) { return *(const s32 *)((const u8 *)p + (8)); }

s32 func_00190CA0(const s32 *p) { return *(const s32 *)((const u8 *)p + (12)); }

s32 func_00190CA8(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

s32 func_00190CB0(const s32 *p) { return *(const s32 *)((const u8 *)p + (24)); }

INCLUDE_ASM(const s32, "core/main", func_00190CB8);

INCLUDE_ASM(const s32, "core/main", func_00190CE8);

INCLUDE_ASM(const s32, "core/main", func_00190D20);

f32 func_00190E40(void)
{
    s32 index = D_00393D00;
    u8 *base = (u8 *)D_003731B8;

    return *(f32 *)(base + index + 0x20c);
}

INCLUDE_ASM(const s32, "core/main", func_00190E58);

INCLUDE_ASM(const s32, "core/main", func_00190EB0);

INCLUDE_ASM(const s32, "core/main", func_00190EF8);

s32 func_00190F20(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00190F28);

INCLUDE_ASM(const s32, "core/main", func_00190F58);

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

INCLUDE_ASM(const s32, "core/main", func_00192F80);

INCLUDE_ASM(const s32, "core/main", func_00192FF0);

INCLUDE_ASM(const s32, "core/main", func_00193000);

INCLUDE_ASM(const s32, "core/main", func_00193010);

INCLUDE_ASM(const s32, "core/main", func_00193038);

INCLUDE_ASM(const s32, "core/main", func_00193090);

INCLUDE_ASM(const s32, "core/main", func_001930B8);

INCLUDE_ASM(const s32, "core/main", func_001930E0);

INCLUDE_ASM(const s32, "core/main", func_00193108);

INCLUDE_ASM(const s32, "core/main", func_00193130);

INCLUDE_ASM(const s32, "core/main", func_00193390);

INCLUDE_ASM(const s32, "core/main", func_00193858);

INCLUDE_ASM(const s32, "core/main", func_00193AC8);

INCLUDE_ASM(const s32, "core/main", func_00193C40);

INCLUDE_ASM(const s32, "core/main", func_00193F20);

INCLUDE_ASM(const s32, "core/main", func_00194248);

INCLUDE_ASM(const s32, "core/main", func_001943F8);

INCLUDE_ASM(const s32, "core/main", func_00194638);

INCLUDE_ASM(const s32, "core/main", func_001947F0);

INCLUDE_ASM(const s32, "core/main", func_00194AC8);

INCLUDE_ASM(const s32, "core/main", func_00194B00);

INCLUDE_ASM(const s32, "core/main", func_00194B88);

INCLUDE_ASM(const s32, "core/main", func_00194BD0);

INCLUDE_ASM(const s32, "core/main", func_00194D28);

INCLUDE_ASM(const s32, "core/main", func_00194D48);

INCLUDE_ASM(const s32, "core/main", func_00194E48);

INCLUDE_ASM(const s32, "core/main", func_00194E68);

INCLUDE_ASM(const s32, "core/main", func_00194E88);

INCLUDE_ASM(const s32, "core/main", func_00194F08);

INCLUDE_ASM(const s32, "core/main", func_00194FB0);

INCLUDE_ASM(const s32, "core/main", func_00195048);

INCLUDE_ASM(const s32, "core/main", func_001950F8);

INCLUDE_ASM(const s32, "core/main", func_00195118);

INCLUDE_ASM(const s32, "core/main", func_00195148);

INCLUDE_ASM(const s32, "core/main", func_001951C0);

INCLUDE_ASM(const s32, "core/main", func_00195228);

INCLUDE_ASM(const s32, "core/main", func_001953D0);

INCLUDE_ASM(const s32, "core/main", func_00195568);

INCLUDE_ASM(const s32, "core/main", func_00195578);

INCLUDE_ASM(const s32, "core/main", func_001956D0);

u8 *func_00195780(u8 *p, s32 value)
{
    u8 *node = *(u8 **)(p + 0x4);

    *(s32 *)(node + 0x4) = value;
    *(u32 *)(p + 0x4) = (u32)(node + 0x4);
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

INCLUDE_ASM(const s32, "core/main", func_00195D50);

s32 func_00195D98(const s32 *p) { return *(const s32 *)((const u8 *)p + (12)); }

s32 func_00195DA0(const s32 *p) { return *(const s32 *)((const u8 *)p + (16)); }

s32 func_00195DA8(const s32 *p) { return *(const s32 *)((const u8 *)p + (24)); }

s32 func_00195DB0(const s32 *p) { return *(const s32 *)((const u8 *)p + (20)); }

INCLUDE_ASM(const s32, "core/main", func_00195DB8);

INCLUDE_ASM(const s32, "core/main", func_00195DC8);

INCLUDE_ASM(const s32, "core/main", func_00195E20);

s32 func_00195EC0(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

INCLUDE_ASM(const s32, "core/main", func_00195EC8);

INCLUDE_ASM(const s32, "core/main", func_00195F08);

INCLUDE_ASM(const s32, "core/main", func_00195F48);

INCLUDE_ASM(const s32, "core/main", func_00195F80);

INCLUDE_ASM(const s32, "core/main", func_00195FF8);

INCLUDE_ASM(const s32, "core/main", func_00196060);

INCLUDE_ASM(const s32, "core/main", func_00196088);

INCLUDE_ASM(const s32, "core/main", func_001960F8);

INCLUDE_ASM(const s32, "core/main", func_00196128);

INCLUDE_ASM(const s32, "core/main", func_00196190);

INCLUDE_ASM(const s32, "core/main", func_001961D0);

INCLUDE_ASM(const s32, "core/main", func_00196210);

INCLUDE_ASM(const s32, "core/main", func_00196248);

INCLUDE_ASM(const s32, "core/main", func_00196290);

INCLUDE_ASM(const s32, "core/main", func_001962D8);

s32 func_00196300(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00196308);

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

INCLUDE_ASM(const s32, "core/main", func_00196998);

INCLUDE_ASM(const s32, "core/main", func_001969E8);

INCLUDE_ASM(const s32, "core/main", func_00196B30);

INCLUDE_ASM(const s32, "core/main", func_00196E00);

s32 func_00196E88(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00196E90);

INCLUDE_ASM(const s32, "core/main", func_00196F50);

INCLUDE_ASM(const s32, "core/main", func_00197010);

INCLUDE_ASM(const s32, "core/main", func_00197060);

INCLUDE_ASM(const s32, "core/main", func_001972B8);

INCLUDE_ASM(const s32, "core/main", func_00197458);

INCLUDE_ASM(const s32, "core/main", func_00197630);

INCLUDE_ASM(const s32, "core/main", func_00197768);

INCLUDE_ASM(const s32, "core/main", func_00197810);

INCLUDE_ASM(const s32, "core/main", func_00197928);

INCLUDE_ASM(const s32, "core/main", func_00197990);

INCLUDE_ASM(const s32, "core/main", func_001979E8);

INCLUDE_ASM(const s32, "core/main", func_00197B38);

INCLUDE_ASM(const s32, "core/main", func_00197C10);

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

INCLUDE_ASM(const s32, "core/main", func_00199560);

s32 func_00199580(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x18);
}

INCLUDE_ASM(const s32, "core/main", func_00199598);

s32 func_001995B8(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x1c);
}

INCLUDE_ASM(const s32, "core/main", func_001995D0);

s32 func_001995F0(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x28);
}

INCLUDE_ASM(const s32, "core/main", func_00199608);

s32 func_00199628(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x2c);
}

INCLUDE_ASM(const s32, "core/main", func_00199640);

INCLUDE_ASM(const s32, "core/main", func_00199688);

INCLUDE_ASM(const s32, "core/main", func_001996C0);

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

INCLUDE_ASM(const s32, "core/main", func_00199710);

u8 *func_00199730(u8 *p) { return p + 16; }

u8 *func_00199738(u8 *p) { return p + 48; }

INCLUDE_ASM(const s32, "core/main", func_00199740);

INCLUDE_ASM(const s32, "core/main", func_001997C8);

INCLUDE_ASM(const s32, "core/main", func_00199850);

INCLUDE_ASM(const s32, "core/main", func_00199898);

INCLUDE_ASM(const s32, "core/main", func_001998E0);

INCLUDE_ASM(const s32, "core/main", func_00199990);

INCLUDE_ASM(const s32, "core/main", func_00199A30);

u8 *func_00199AB8(u8 *p)
{
    *(s32 *)(p + 0x50) += 1;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00199AD0);

INCLUDE_ASM(const s32, "core/main", func_00199B88);

INCLUDE_ASM(const s32, "core/main", func_00199BD0);

s32 *func_00199BE0(s32 *p, s32 value) { p[0x16] = value; p[0x17] = value; return p; }

INCLUDE_ASM(const s32, "core/main", func_00199BF0);

INCLUDE_ASM(const s32, "core/main", func_00199C00);

INCLUDE_ASM(const s32, "core/main", func_00199C10);

INCLUDE_ASM(const s32, "core/main", func_00199C68);

INCLUDE_ASM(const s32, "core/main", func_00199CA0);

s32 func_00199D70(void)
{
    s32 index = D_00393D30;
    u8 *base = (u8 *)D_003731B8;

    return *(s32 *)(base + index + 0x10);
}

INCLUDE_ASM(const s32, "core/main", func_00199D88);

s32 func_00199DA8(const s32 *p) { return *(const s32 *)((const u8 *)p + (4)); }

INCLUDE_ASM(const s32, "core/main", func_00199DB0);

INCLUDE_ASM(const s32, "core/main", func_00199E20);

INCLUDE_ASM(const s32, "core/main", func_00199E68);

INCLUDE_ASM(const s32, "core/main", func_00199EB0);

INCLUDE_ASM(const s32, "core/main", func_00199ED8);

s32 func_00199F00(void) { return 1; }

s32 func_00199F08(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_00199F10);

INCLUDE_ASM(const s32, "core/main", func_00199FB0);

INCLUDE_ASM(const s32, "core/main", func_00199FF8);

INCLUDE_ASM(const s32, "core/main", func_0019A050);

INCLUDE_ASM(const s32, "core/main", func_0019A198);

INCLUDE_ASM(const s32, "core/main", func_0019A1D8);

INCLUDE_ASM(const s32, "core/main", func_0019A308);

INCLUDE_ASM(const s32, "core/main", func_0019A3A8);

INCLUDE_ASM(const s32, "core/main", func_0019A420);

INCLUDE_ASM(const s32, "core/main", func_0019A450);

INCLUDE_ASM(const s32, "core/main", func_0019A490);

INCLUDE_ASM(const s32, "core/main", func_0019A638);

INCLUDE_ASM(const s32, "core/main", func_0019A668);

INCLUDE_ASM(const s32, "core/main", func_0019A700);

INCLUDE_ASM(const s32, "core/main", func_0019A760);

INCLUDE_ASM(const s32, "core/main", func_0019A7D0);

s32 func_0019A818(s32 value) { return value; }

s32 func_0019A820(s32 value) { return value; }

s32 func_0019A828(s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_0019A830);

INCLUDE_ASM(const s32, "core/main", func_0019A890);

INCLUDE_ASM(const s32, "core/main", func_0019A8C0);

INCLUDE_ASM(const s32, "core/main", func_0019AA20);

INCLUDE_ASM(const s32, "core/main", func_0019ABB0);

INCLUDE_ASM(const s32, "core/main", func_0019AC80);

INCLUDE_ASM(const s32, "core/main", func_0019ACA8);

INCLUDE_ASM(const s32, "core/main", func_0019ACC8);

INCLUDE_ASM(const s32, "core/main", func_0019AD80);

INCLUDE_ASM(const s32, "core/main", func_0019ADB0);

INCLUDE_ASM(const s32, "core/main", func_0019ADE0);

INCLUDE_ASM(const s32, "core/main", func_0019AE30);

INCLUDE_ASM(const s32, "core/main", func_0019AE88);

INCLUDE_ASM(const s32, "core/main", func_0019AF18);

INCLUDE_ASM(const s32, "core/main", func_0019AF90);

INCLUDE_ASM(const s32, "core/main", func_0019B028);

INCLUDE_ASM(const s32, "core/main", func_0019B060);

INCLUDE_ASM(const s32, "core/main", func_0019B118);

INCLUDE_ASM(const s32, "core/main", func_0019B1B8);

void func_0019B2A8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0019B2B0);

INCLUDE_ASM(const s32, "core/main", func_0019B320);

INCLUDE_ASM(const s32, "core/main", func_0019B368);

INCLUDE_ASM(const s32, "core/main", func_0019B3B0);

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

INCLUDE_ASM(const s32, "core/main", func_0019C198);

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

INCLUDE_ASM(const s32, "core/main", func_0019C648);

INCLUDE_ASM(const s32, "core/main", func_0019C678);

s32 func_0019C6A0(void) { return 1; }

void func_0019C6A8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0019C6B0);

INCLUDE_ASM(const s32, "core/main", func_0019CDA0);

INCLUDE_ASM(const s32, "core/main", func_0019D480);

INCLUDE_ASM(const s32, "core/main", func_0019D5A0);

INCLUDE_ASM(const s32, "core/main", func_0019D5B0);

INCLUDE_ASM(const s32, "core/main", func_0019DF58);

INCLUDE_ASM(const s32, "core/main", func_0019E8E8);

INCLUDE_ASM(const s32, "core/main", func_0019EA08);

INCLUDE_ASM(const s32, "core/main", func_0019EA18);

INCLUDE_ASM(const s32, "core/main", func_0019ECD0);

INCLUDE_ASM(const s32, "core/main", func_0019F1C0);

INCLUDE_ASM(const s32, "core/main", func_0019F1D0);

INCLUDE_ASM(const s32, "core/main", func_0019F240);

INCLUDE_ASM(const s32, "core/main", func_0019F600);

INCLUDE_ASM(const s32, "core/main", func_0019F610);

INCLUDE_ASM(const s32, "core/main", func_0019F840);

INCLUDE_ASM(const s32, "core/main", func_0019F850);

INCLUDE_ASM(const s32, "core/main", func_0019FB68);

INCLUDE_ASM(const s32, "core/main", func_0019FB78);

INCLUDE_ASM(const s32, "core/main", func_0019FEC8);

INCLUDE_ASM(const s32, "core/main", func_0019FED8);


/* Source: engine/region_001A.c */
/* func_001A00C8 through func_001AFE40; retail address range 0x001A0000-0x001B0000. */



extern s32 D_00393824;

INCLUDE_ASM(const s32, "core/main", func_001A00C8);

INCLUDE_ASM(const s32, "core/main", func_001A00D8);

INCLUDE_ASM(const s32, "core/main", func_001A0390);

INCLUDE_ASM(const s32, "core/main", func_001A03A0);

INCLUDE_ASM(const s32, "core/main", func_001A06A0);

INCLUDE_ASM(const s32, "core/main", func_001A06B0);

INCLUDE_ASM(const s32, "core/main", func_001A0D00);

INCLUDE_ASM(const s32, "core/main", func_001A1350);

INCLUDE_ASM(const s32, "core/main", func_001A1360);

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

INCLUDE_ASM(const s32, "core/main", func_001A2DA0);

INCLUDE_ASM(const s32, "core/main", func_001A2DD8);

INCLUDE_ASM(const s32, "core/main", func_001A2FA8);

INCLUDE_ASM(const s32, "core/main", func_001A3188);

INCLUDE_ASM(const s32, "core/main", func_001A3378);

INCLUDE_ASM(const s32, "core/main", func_001A3558);

INCLUDE_ASM(const s32, "core/main", func_001A3740);

INCLUDE_ASM(const s32, "core/main", func_001A38C0);

INCLUDE_ASM(const s32, "core/main", func_001A3A88);

INCLUDE_ASM(const s32, "core/main", func_001A3AF0);

INCLUDE_ASM(const s32, "core/main", func_001A3B20);

s32 func_001A3B98(void) { return D_00393824; }

INCLUDE_ASM(const s32, "core/main", func_001A3BA0);

INCLUDE_ASM(const s32, "core/main", func_001A3C60);

INCLUDE_ASM(const s32, "core/main", func_001A3CA8);

INCLUDE_ASM(const s32, "core/main", func_001A3CD0);

INCLUDE_ASM(const s32, "core/main", func_001A3D90);

INCLUDE_ASM(const s32, "core/main", func_001A3E50);

INCLUDE_ASM(const s32, "core/main", func_001A3F20);

INCLUDE_ASM(const s32, "core/main", func_001A3FB0);

void func_001A4000(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001A4008);

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

INCLUDE_ASM(const s32, "core/main", func_001A41E0);

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

INCLUDE_ASM(const s32, "core/main", func_001A5E80);

u8 *func_001A5EB8(u8 *p)
{
    if (p != 0 && *(s32 *)p != 0)
        return p;
    return 0;
}

INCLUDE_ASM(const s32, "core/main", func_001A5ED0);

INCLUDE_ASM(const s32, "core/main", func_001A5F70);

INCLUDE_ASM(const s32, "core/main", func_001A6028);

INCLUDE_ASM(const s32, "core/main", func_001A6108);

INCLUDE_ASM(const s32, "core/main", func_001A6118);

INCLUDE_ASM(const s32, "core/main", func_001A61C8);

INCLUDE_ASM(const s32, "core/main", func_001A6278);

INCLUDE_ASM(const s32, "core/main", func_001A6370);

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

INCLUDE_ASM(const s32, "core/main", func_001A7EF0);

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

INCLUDE_ASM(const s32, "core/main", func_001A9240);

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

INCLUDE_ASM(const s32, "core/main", func_001ABBC0);

INCLUDE_ASM(const s32, "core/main", func_001ABBE8);

INCLUDE_ASM(const s32, "core/main", func_001ABD08);

INCLUDE_ASM(const s32, "core/main", func_001ABD80);

INCLUDE_ASM(const s32, "core/main", func_001ABE38);

INCLUDE_ASM(const s32, "core/main", func_001ABE98);

s32 func_001ABEE0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_001ABEE8);

INCLUDE_ASM(const s32, "core/main", func_001ABF80);

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

INCLUDE_ASM(const s32, "core/main", func_001B1040);

INCLUDE_ASM(const s32, "core/main", func_001B1050);

INCLUDE_ASM(const s32, "core/main", func_001B1080);

INCLUDE_ASM(const s32, "core/main", func_001B10D0);

INCLUDE_ASM(const s32, "core/main", func_001B1128);

INCLUDE_ASM(const s32, "core/main", func_001B1190);

INCLUDE_ASM(const s32, "core/main", func_001B11B0);

s32 func_001B11E0(void) { return (D_00393968 & 0x3000) >> 12; }

INCLUDE_ASM(const s32, "core/main", func_001B11F0);

INCLUDE_ASM(const s32, "core/main", func_001B1288);

INCLUDE_ASM(const s32, "core/main", func_001B12D0);

INCLUDE_ASM(const s32, "core/main", func_001B1328);

INCLUDE_ASM(const s32, "core/main", func_001B1358);

INCLUDE_ASM(const s32, "core/main", func_001B13A0);

INCLUDE_ASM(const s32, "core/main", func_001B1450);

INCLUDE_ASM(const s32, "core/main", func_001B14C8);

s32 func_001B1520(void) { return 4; }

INCLUDE_ASM(const s32, "core/main", func_001B1528);

INCLUDE_ASM(const s32, "core/main", func_001B15D8);

INCLUDE_ASM(const s32, "core/main", func_001B1638);

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

INCLUDE_ASM(const s32, "core/main", func_001B3F98);

INCLUDE_ASM(const s32, "core/main", func_001B3FD8);

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

INCLUDE_ASM(const s32, "core/main", func_001B7DD0);

INCLUDE_ASM(const s32, "core/main", func_001B7E10);

void func_001B7FE8(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001B7FF0);

INCLUDE_ASM(const s32, "core/main", func_001B8048);

INCLUDE_ASM(const s32, "core/main", func_001B80A8);

INCLUDE_ASM(const s32, "core/main", func_001B80F8);

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

INCLUDE_ASM(const s32, "core/main", func_001BA1E8);

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

INCLUDE_ASM(const s32, "core/main", func_001BA560);

INCLUDE_ASM(const s32, "core/main", func_001BA580);

INCLUDE_ASM(const s32, "core/main", func_001BA5A8);

INCLUDE_ASM(const s32, "core/main", func_001BA5C8);

s32 func_001BA5F0(void) { return D_003939C0; }

INCLUDE_ASM(const s32, "core/main", func_001BA5F8);

INCLUDE_ASM(const s32, "core/main", func_001BA618);

INCLUDE_ASM(const s32, "core/main", func_001BA650);

INCLUDE_ASM(const s32, "core/main", func_001BA680);

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

INCLUDE_ASM(const s32, "core/main", func_001BDD90);

INCLUDE_ASM(const s32, "core/main", func_001BDDD8);

INCLUDE_ASM(const s32, "core/main", func_001BDE78);

INCLUDE_ASM(const s32, "core/main", func_001BDEC0);

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

INCLUDE_ASM(const s32, "core/main", func_001BDF80);

INCLUDE_ASM(const s32, "core/main", func_001BDFE8);

INCLUDE_ASM(const s32, "core/main", func_001BE050);

INCLUDE_ASM(const s32, "core/main", func_001BE168);

INCLUDE_ASM(const s32, "core/main", func_001BE228);

INCLUDE_ASM(const s32, "core/main", func_001BE2F8);

INCLUDE_ASM(const s32, "core/main", func_001BE398);

INCLUDE_ASM(const s32, "core/main", func_001BE6F0);

INCLUDE_ASM(const s32, "core/main", func_001BE990);

INCLUDE_ASM(const s32, "core/main", func_001BEB90);

INCLUDE_ASM(const s32, "core/main", func_001BECC0);

INCLUDE_ASM(const s32, "core/main", func_001BEDD0);

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

s32 func_001C0950(void) {
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

s32 func_001C0A00(void) {
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

s32 func_001C0CD0(void) {
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

s32 func_001C1020(void) {
    register s32 syscall_number __asm__("$3") = 0x79;
    register s32 syscall_result __asm__("$2");
    __asm__ volatile ("syscall" : "=r"(syscall_result), "+r"(syscall_number) : : "memory");
    return syscall_result;
}

s32 func_001C1030(void) {
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

s32 func_001C1050(void) {
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

INCLUDE_ASM(const s32, "core/main", func_001C11B0);

INCLUDE_ASM(const s32, "core/main", func_001C1228);

s32 func_001C1250(void) { return -1; }

s32 func_001C1258(void) { return -1; }

s32 func_001C1260(void) { return -1; }

INCLUDE_ASM(const s32, "core/main", func_001C1268);

s32 func_001C1310(void) { return 1; }

s32 func_001C1318(u8 *unused, u8 *p)
{
    *(u64 *)(p + 0x48) = 0;
    *(s32 *)(p + 0x4) = 0x2000;
    return 0;
}

s32 func_001C1330(void) { return 1; }

INCLUDE_ASM(const s32, "core/main", func_001C1338);

INCLUDE_ASM(const s32, "core/main", func_001C1360);

INCLUDE_ASM(const s32, "core/main", func_001C1388);

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

INCLUDE_ASM(const s32, "core/main", func_001C1690);

INCLUDE_ASM(const s32, "core/main", func_001C16F8);

INCLUDE_ASM(const s32, "core/main", func_001C1760);

INCLUDE_ASM(const s32, "core/main", func_001C17C8);

INCLUDE_ASM(const s32, "core/main", func_001C1830);

INCLUDE_ASM(const s32, "core/main", func_001C18A0);

INCLUDE_ASM(const s32, "core/main", func_001C1908);

INCLUDE_ASM(const s32, "core/main", func_001C1928);

INCLUDE_ASM(const s32, "core/main", func_001C1948);

INCLUDE_ASM(const s32, "core/main", func_001C1968);

INCLUDE_ASM(const s32, "core/main", func_001C1988);

INCLUDE_ASM(const s32, "core/main", func_001C19A8);

INCLUDE_ASM(const s32, "core/main", func_001C19C8);

INCLUDE_ASM(const s32, "core/main", func_001C19F0);

INCLUDE_ASM(const s32, "core/main", func_001C1A30);

INCLUDE_ASM(const s32, "core/main", func_001C1A70);

INCLUDE_ASM(const s32, "core/main", func_001C1C08);

INCLUDE_ASM(const s32, "core/main", func_001C1D58);

INCLUDE_ASM(const s32, "core/main", func_001C1E28);

INCLUDE_ASM(const s32, "core/main", func_001C1EE8);

INCLUDE_ASM(const s32, "core/main", func_001C1F20);

INCLUDE_ASM(const s32, "core/main", func_001C1FD0);

INCLUDE_ASM(const s32, "core/main", func_001C2008);

INCLUDE_ASM(const s32, "core/main", func_001C2098);

INCLUDE_ASM(const s32, "core/main", func_001C2160);

INCLUDE_ASM(const s32, "core/main", func_001C22C8);

INCLUDE_ASM(const s32, "core/main", func_001C2890);

INCLUDE_ASM(const s32, "core/main", func_001C28C8);

INCLUDE_ASM(const s32, "core/main", func_001C2928);

INCLUDE_ASM(const s32, "core/main", func_001C2AE8);

INCLUDE_ASM(const s32, "core/main", func_001C2B10);

INCLUDE_ASM(const s32, "core/main", func_001C2BB8);

INCLUDE_ASM(const s32, "core/main", func_001C2BD8);

INCLUDE_ASM(const s32, "core/main", func_001C2C08);

INCLUDE_ASM(const s32, "core/main", func_001C2C48);

INCLUDE_ASM(const s32, "core/main", func_001C2D00);

INCLUDE_ASM(const s32, "core/main", func_001C2D60);

INCLUDE_ASM(const s32, "core/main", func_001C2EB8);

INCLUDE_ASM(const s32, "core/main", func_001C2F08);

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

INCLUDE_ASM(const s32, "core/main", func_001C38C0);

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

INCLUDE_ASM(const s32, "core/main", func_001C3A58);

INCLUDE_ASM(const s32, "core/main", func_001C3AC0);

INCLUDE_ASM(const s32, "core/main", func_001C3AF8);

INCLUDE_ASM(const s32, "core/main", func_001C3CA0);

INCLUDE_ASM(const s32, "core/main", func_001C3D98);

INCLUDE_ASM(const s32, "core/main", func_001C3EE8);

INCLUDE_ASM(const s32, "core/main", func_001C3F80);

INCLUDE_ASM(const s32, "core/main", func_001C4128);

INCLUDE_ASM(const s32, "core/main", func_001C4310);

INCLUDE_ASM(const s32, "core/main", func_001C45B8);

INCLUDE_ASM(const s32, "core/main", func_001C4640);

INCLUDE_ASM(const s32, "core/main", func_001C46B0);

INCLUDE_ASM(const s32, "core/main", func_001C4728);

INCLUDE_ASM(const s32, "core/main", func_001C4818);

INCLUDE_ASM(const s32, "core/main", func_001C4918);

INCLUDE_ASM(const s32, "core/main", func_001C4980);

INCLUDE_ASM(const s32, "core/main", func_001C49B8);

INCLUDE_ASM(const s32, "core/main", func_001C4BA8);

INCLUDE_ASM(const s32, "core/main", func_001C4DA8);

INCLUDE_ASM(const s32, "core/main", func_001C4DC8);

INCLUDE_ASM(const s32, "core/main", func_001C4ED0);

INCLUDE_ASM(const s32, "core/main", func_001C4EF0);

INCLUDE_ASM(const s32, "core/main", func_001C4F18);

INCLUDE_ASM(const s32, "core/main", func_001C5008);

INCLUDE_ASM(const s32, "core/main", func_001C50E8);

INCLUDE_ASM(const s32, "core/main", func_001C5218);

INCLUDE_ASM(const s32, "core/main", func_001C5240);

INCLUDE_ASM(const s32, "core/main", func_001C5288);

INCLUDE_ASM(const s32, "core/main", func_001C5388);

INCLUDE_ASM(const s32, "core/main", func_001C5500);

INCLUDE_ASM(const s32, "core/main", func_001C5578);

INCLUDE_ASM(const s32, "core/main", func_001C55D8);

INCLUDE_ASM(const s32, "core/main", func_001C5658);

INCLUDE_ASM(const s32, "core/main", func_001C56A0);

INCLUDE_ASM(const s32, "core/main", func_001C56C8);

INCLUDE_ASM(const s32, "core/main", func_001C56F8);

INCLUDE_ASM(const s32, "core/main", func_001C5720);

INCLUDE_ASM(const s32, "core/main", func_001C5758);

INCLUDE_ASM(const s32, "core/main", func_001C5790);

INCLUDE_ASM(const s32, "core/main", func_001C57C0);

INCLUDE_ASM(const s32, "core/main", func_001C57E8);

INCLUDE_ASM(const s32, "core/main", func_001C5810);

INCLUDE_ASM(const s32, "core/main", func_001C5838);

INCLUDE_ASM(const s32, "core/main", func_001C5858);

s32 func_001C5868(s32 index)
{
    return D_003E7400[index];
}

INCLUDE_ASM(const s32, "core/main", func_001C5880);

INCLUDE_ASM(const s32, "core/main", func_001C58A0);

INCLUDE_ASM(const s32, "core/main", func_001C58B0);

INCLUDE_ASM(const s32, "core/main", func_001C5B50);

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

INCLUDE_ASM(const s32, "core/main", func_001C5BB8);

INCLUDE_ASM(const s32, "core/main", func_001C5BE8);

INCLUDE_ASM(const s32, "core/main", func_001C5C10);

INCLUDE_ASM(const s32, "core/main", func_001C5D48);

INCLUDE_ASM(const s32, "core/main", func_001C5D88);

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

INCLUDE_ASM(const s32, "core/main", func_001C6028);

INCLUDE_ASM(const s32, "core/main", func_001C6078);

INCLUDE_ASM(const s32, "core/main", func_001C60A8);

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

INCLUDE_ASM(const s32, "core/main", func_001C6148);

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

INCLUDE_ASM(const s32, "core/main", func_001C64D8);

INCLUDE_ASM(const s32, "core/main", func_001C6528);

INCLUDE_ASM(const s32, "core/main", func_001C6558);

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

INCLUDE_ASM(const s32, "core/main", func_001C6600);

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

INCLUDE_ASM(const s32, "core/main", func_001C6930);

INCLUDE_ASM(const s32, "core/main", func_001C6968);

s32 func_001C69D0(void)
{
    return (*(volatile s32 *)0x10003020 & 4) >> 2;
}

INCLUDE_ASM(const s32, "core/main", func_001C69E8);

INCLUDE_ASM(const s32, "core/main", func_001C6A50);

INCLUDE_ASM(const s32, "core/main", func_001C6A70);

INCLUDE_ASM(const s32, "core/main", func_001C6AA0);

INCLUDE_ASM(const s32, "core/main", func_001C6B58);

INCLUDE_ASM(const s32, "core/main", func_001C6BB8);

INCLUDE_ASM(const s32, "core/main", func_001C6BE0);

INCLUDE_ASM(const s32, "core/main", func_001C6C48);

INCLUDE_ASM(const s32, "core/main", func_001C6C88);

/* Reads a word from the VU scratchpad. */
s32 func_001C6CA8(void)
{
    return *(s32 *)0x10003820;
}

INCLUDE_ASM(const s32, "core/main", func_001C6CB8);

INCLUDE_ASM(const s32, "core/main", func_001C6D08);

INCLUDE_ASM(const s32, "core/main", func_001C6E88);

INCLUDE_ASM(const s32, "core/main", func_001C6EE8);

INCLUDE_ASM(const s32, "core/main", func_001C6F10);

INCLUDE_ASM(const s32, "core/main", func_001C6F78);

INCLUDE_ASM(const s32, "core/main", func_001C6FB8);

s32 func_001C6FD8(void)
{
    return *(s32 *)0x10003C20;
}

INCLUDE_ASM(const s32, "core/main", func_001C6FE8);

INCLUDE_ASM(const s32, "core/main", func_001C7038);

INCLUDE_ASM(const s32, "core/main", func_001C7210);

INCLUDE_ASM(const s32, "core/main", func_001C7270);

INCLUDE_ASM(const s32, "core/main", func_001C72D0);

INCLUDE_ASM(const s32, "core/main", func_001C7308);

INCLUDE_ASM(const s32, "core/main", func_001C7330);

s32 func_001C7410(s32 value) { s32 old = D_00378AC0[0]; D_00378AC0[0] = value; return old; }

INCLUDE_ASM(const s32, "core/main", func_001C7420);

INCLUDE_ASM(const s32, "core/main", func_001C75F8);

INCLUDE_ASM(const s32, "core/main", func_001C7630);

INCLUDE_ASM(const s32, "core/main", func_001C7658);

INCLUDE_ASM(const s32, "core/main", func_001C7730);

INCLUDE_ASM(const s32, "core/main", func_001C7818);

INCLUDE_ASM(const s32, "core/main", func_001C7900);

INCLUDE_ASM(const s32, "core/main", func_001C79C8);

INCLUDE_ASM(const s32, "core/main", func_001C7AB8);

INCLUDE_ASM(const s32, "core/main", func_001C7BA8);

INCLUDE_ASM(const s32, "core/main", func_001C7BB8);

INCLUDE_ASM(const s32, "core/main", func_001C7C80);

INCLUDE_ASM(const s32, "core/main", func_001C7D50);

INCLUDE_ASM(const s32, "core/main", func_001C7D78);

INCLUDE_ASM(const s32, "core/main", func_001C7D88);

INCLUDE_ASM(const s32, "core/main", func_001C7DA8);

INCLUDE_ASM(const s32, "core/main", func_001C7DC8);

INCLUDE_ASM(const s32, "core/main", func_001C7DF0);

INCLUDE_ASM(const s32, "core/main", func_001C7E78);

INCLUDE_ASM(const s32, "core/main", func_001C7FB0);

INCLUDE_ASM(const s32, "core/main", func_001C8050);

INCLUDE_ASM(const s32, "core/main", func_001C80D0);

INCLUDE_ASM(const s32, "core/main", func_001C8228);

INCLUDE_ASM(const s32, "core/main", func_001C82A0);

INCLUDE_ASM(const s32, "core/main", func_001C8300);

INCLUDE_ASM(const s32, "core/main", func_001C8320);

INCLUDE_ASM(const s32, "core/main", func_001C8368);

INCLUDE_ASM(const s32, "core/main", func_001C83A8);

INCLUDE_ASM(const s32, "core/main", func_001C83E0);

INCLUDE_ASM(const s32, "core/main", func_001C8410);

INCLUDE_ASM(const s32, "core/main", func_001C8430);

INCLUDE_ASM(const s32, "core/main", func_001C8468);

INCLUDE_ASM(const s32, "core/main", func_001C8558);

INCLUDE_ASM(const s32, "core/main", func_001C8640);

INCLUDE_ASM(const s32, "core/main", func_001C8748);

INCLUDE_ASM(const s32, "core/main", func_001C8800);

INCLUDE_ASM(const s32, "core/main", func_001C8898);

INCLUDE_ASM(const s32, "core/main", func_001C8970);

INCLUDE_ASM(const s32, "core/main", func_001C89E0);

INCLUDE_ASM(const s32, "core/main", func_001C8A90);

INCLUDE_ASM(const s32, "core/main", func_001C8AC0);

INCLUDE_ASM(const s32, "core/main", func_001C8AE0);

INCLUDE_ASM(const s32, "core/main", func_001C8B00);

INCLUDE_ASM(const s32, "core/main", func_001C8BD0);

INCLUDE_ASM(const s32, "core/main", func_001C8C38);

INCLUDE_ASM(const s32, "core/main", func_001C8CA0);

INCLUDE_ASM(const s32, "core/main", func_001C8D08);

void func_001C8D78(s32 *p, s32 value) {
    p[0] = value;
    __asm__ volatile("" ::: "memory");
    p[2] = 0;
    __asm__ volatile("" ::: "memory");
    p[1] = value;
}

s32 func_001C8D88(s32 *p) { s32 value = p[1]; p[2] = 0; p[0] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_001C8D98);

u32 func_001C8DF0(u8 *p)
{
    return (*(u32 *)p - *(u32 *)(p + 0x4)) >> 4;
}

INCLUDE_ASM(const s32, "core/main", func_001C8E08);

INCLUDE_ASM(const s32, "core/main", func_001C8E30);

INCLUDE_ASM(const s32, "core/main", func_001C8F88);

INCLUDE_ASM(const s32, "core/main", func_001C8FB0);

INCLUDE_ASM(const s32, "core/main", func_001C9040);

INCLUDE_ASM(const s32, "core/main", func_001C90C8);

INCLUDE_ASM(const s32, "core/main", func_001C91B0);

INCLUDE_ASM(const s32, "core/main", func_001C91E8);

INCLUDE_ASM(const s32, "core/main", func_001C9278);

INCLUDE_ASM(const s32, "core/main", func_001C9308);

INCLUDE_ASM(const s32, "core/main", func_001C9398);

INCLUDE_ASM(const s32, "core/main", func_001C9470);

INCLUDE_ASM(const s32, "core/main", func_001C95B0);

INCLUDE_ASM(const s32, "core/main", func_001C9698);

INCLUDE_ASM(const s32, "core/main", func_001C96F0);

INCLUDE_ASM(const s32, "core/main", func_001C9828);

INCLUDE_ASM(const s32, "core/main", func_001C9910);

INCLUDE_ASM(const s32, "core/main", func_001C9998);

INCLUDE_ASM(const s32, "core/main", func_001C9A80);

INCLUDE_ASM(const s32, "core/main", func_001C9B10);

INCLUDE_ASM(const s32, "core/main", func_001C9BE8);

INCLUDE_ASM(const s32, "core/main", func_001C9C78);

INCLUDE_ASM(const s32, "core/main", func_001C9DD0);

INCLUDE_ASM(const s32, "core/main", func_001C9ED0);

INCLUDE_ASM(const s32, "core/main", func_001C9F60);

INCLUDE_ASM(const s32, "core/main", func_001CA018);

INCLUDE_ASM(const s32, "core/main", func_001CA0B8);

INCLUDE_ASM(const s32, "core/main", func_001CA0F8);

INCLUDE_ASM(const s32, "core/main", func_001CA158);

INCLUDE_ASM(const s32, "core/main", func_001CA178);

INCLUDE_ASM(const s32, "core/main", func_001CA1B8);

INCLUDE_ASM(const s32, "core/main", func_001CA1F8);

INCLUDE_ASM(const s32, "core/main", func_001CA260);

INCLUDE_ASM(const s32, "core/main", func_001CA2E0);

INCLUDE_ASM(const s32, "core/main", func_001CA360);

INCLUDE_ASM(const s32, "core/main", func_001CA3E0);

INCLUDE_ASM(const s32, "core/main", func_001CA448);

INCLUDE_ASM(const s32, "core/main", func_001CA4A0);

INCLUDE_ASM(const s32, "core/main", func_001CA528);

INCLUDE_ASM(const s32, "core/main", func_001CA5B0);

INCLUDE_ASM(const s32, "core/main", func_001CA698);

INCLUDE_ASM(const s32, "core/main", func_001CA770);

INCLUDE_ASM(const s32, "core/main", func_001CA7E0);

INCLUDE_ASM(const s32, "core/main", func_001CA848);

INCLUDE_ASM(const s32, "core/main", func_001CA918);

INCLUDE_ASM(const s32, "core/main", func_001CA960);

INCLUDE_ASM(const s32, "core/main", func_001CA9C8);

INCLUDE_ASM(const s32, "core/main", func_001CAA08);

INCLUDE_ASM(const s32, "core/main", func_001CAAB8);

INCLUDE_ASM(const s32, "core/main", func_001CAC50);

INCLUDE_ASM(const s32, "core/main", func_001CACF0);

INCLUDE_ASM(const s32, "core/main", func_001CAD88);

INCLUDE_ASM(const s32, "core/main", func_001CADF0);

INCLUDE_ASM(const s32, "core/main", func_001CAEB8);

INCLUDE_ASM(const s32, "core/main", func_001CAF18);

INCLUDE_ASM(const s32, "core/main", func_001CB060);

INCLUDE_ASM(const s32, "core/main", func_001CB350);

INCLUDE_ASM(const s32, "core/main", func_001CB4C0);

INCLUDE_ASM(const s32, "core/main", func_001CB558);

INCLUDE_ASM(const s32, "core/main", func_001CB588);

INCLUDE_ASM(const s32, "core/main", func_001CB878);

INCLUDE_ASM(const s32, "core/main", func_001CBA48);

INCLUDE_ASM(const s32, "core/main", func_001CBB90);

INCLUDE_ASM(const s32, "core/main", func_001CBE20);

INCLUDE_ASM(const s32, "core/main", func_001CBF08);

INCLUDE_ASM(const s32, "core/main", func_001CBFC8);

INCLUDE_ASM(const s32, "core/main", func_001CC090);

INCLUDE_ASM(const s32, "core/main", func_001CC150);

INCLUDE_ASM(const s32, "core/main", func_001CC1F8);

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

INCLUDE_ASM(const s32, "core/main", func_001CD070);

INCLUDE_ASM(const s32, "core/main", func_001CD0A0);

INCLUDE_ASM(const s32, "core/main", func_001CD0D8);

INCLUDE_ASM(const s32, "core/main", func_001CD108);

INCLUDE_ASM(const s32, "core/main", func_001CD138);

INCLUDE_ASM(const s32, "core/main", func_001CD170);

INCLUDE_ASM(const s32, "core/main", func_001CD2E0);

INCLUDE_ASM(const s32, "core/main", func_001CD330);

INCLUDE_ASM(const s32, "core/main", func_001CD388);

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

INCLUDE_ASM(const s32, "core/main", func_001CD950);

INCLUDE_ASM(const s32, "core/main", func_001CD970);

s32 func_001CD980(s32 *p) { return ((s32 *)p[0x40 / 4])[1] == 0; }

INCLUDE_ASM(const s32, "core/main", func_001CD990);

INCLUDE_ASM(const s32, "core/main", func_001CD9D0);

INCLUDE_ASM(const s32, "core/main", func_001CDA38);

INCLUDE_ASM(const s32, "core/main", func_001CDA60);

INCLUDE_ASM(const s32, "core/main", func_001CDAB0);

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

INCLUDE_ASM(const s32, "core/main", func_001CDB18);

INCLUDE_ASM(const s32, "core/main", func_001CDB28);

INCLUDE_ASM(const s32, "core/main", func_001CDB38);

INCLUDE_ASM(const s32, "core/main", func_001CDB48);

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

INCLUDE_ASM(const s32, "core/main", func_001CDB98);

INCLUDE_ASM(const s32, "core/main", func_001CDBA8);

INCLUDE_ASM(const s32, "core/main", func_001CDBB8);

s32 func_001CDC20(s32 *p)
{
    return p[0] + p[1] - p[2];
}

INCLUDE_ASM(const s32, "core/main", func_001CDC38);

INCLUDE_ASM(const s32, "core/main", func_001CDDA8);

INCLUDE_ASM(const s32, "core/main", func_001CDED0);

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

INCLUDE_ASM(const s32, "core/main", func_001CE2B8);

INCLUDE_ASM(const s32, "core/main", func_001CE2E8);

INCLUDE_ASM(const s32, "core/main", func_001CE340);

INCLUDE_ASM(const s32, "core/main", func_001CE418);

INCLUDE_ASM(const s32, "core/main", func_001CE438);

INCLUDE_ASM(const s32, "core/main", func_001CE590);

INCLUDE_ASM(const s32, "core/main", func_001CE8D8);

INCLUDE_ASM(const s32, "core/main", func_001CE9C0);

INCLUDE_ASM(const s32, "core/main", func_001CEA80);

INCLUDE_ASM(const s32, "core/main", func_001CEBE8);

INCLUDE_ASM(const s32, "core/main", func_001CEC88);

INCLUDE_ASM(const s32, "core/main", func_001CEC98);

INCLUDE_ASM(const s32, "core/main", func_001CECA8);

INCLUDE_ASM(const s32, "core/main", func_001CECB8);

INCLUDE_ASM(const s32, "core/main", func_001CECC8);

INCLUDE_ASM(const s32, "core/main", func_001CECD8);

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

INCLUDE_ASM(const s32, "core/main", func_001D0A58);

INCLUDE_ASM(const s32, "core/main", func_001D0A80);

INCLUDE_ASM(const s32, "core/main", func_001D0C80);

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

INCLUDE_ASM(const s32, "core/main", func_001D1CC8);

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

INCLUDE_ASM(const s32, "core/main", func_001D2970);

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

INCLUDE_ASM(const s32, "core/main", func_001D35C8);

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

INCLUDE_ASM(void, "core/main", func_001D4568);

INCLUDE_ASM(const s32, "core/main", func_001D45C0);

INCLUDE_ASM(const s32, "core/main", func_001D46A0);

INCLUDE_ASM(const s32, "core/main", func_001D4C70);

INCLUDE_ASM(const s32, "core/main", func_001D4D58);

INCLUDE_ASM(const s32, "core/main", func_001D4DC0);

INCLUDE_ASM(const s32, "core/main", func_001D4DF8);

INCLUDE_ASM(const s32, "core/main", func_001D4E68);

INCLUDE_ASM(const s32, "core/main", func_001D4EC8);

INCLUDE_ASM(const s32, "core/main", func_001D4FA8);

INCLUDE_ASM(const s32, "core/main", func_001D5140);

INCLUDE_ASM(const s32, "core/main", func_001D54F0);

INCLUDE_ASM(const s32, "core/main", func_001D5568);

INCLUDE_ASM(const s32, "core/main", func_001D6B18);

INCLUDE_ASM(const s32, "core/main", func_001D6B50);

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

INCLUDE_ASM(const s32, "core/main", func_001D9618);

INCLUDE_ASM(const s32, "core/main", func_001D9660);

INCLUDE_ASM(const s32, "core/main", func_001D9890);

INCLUDE_ASM(const s32, "core/main", func_001D9E70);

INCLUDE_ASM(const s32, "core/main", func_001DA138);

INCLUDE_ASM(const s32, "core/main", func_001DA360);

INCLUDE_ASM(const s32, "core/main", func_001DA3F0);

INCLUDE_ASM(const s32, "core/main", func_001DB8E0);

INCLUDE_ASM(const s32, "core/main", func_001DC458);

INCLUDE_ASM(const s32, "core/main", func_001DC5B0);

INCLUDE_ASM(const s32, "core/main", func_001DC5F8);

INCLUDE_ASM(const s32, "core/main", func_001DC640);

INCLUDE_ASM(const s32, "core/main", func_001DC688);

INCLUDE_ASM(const s32, "core/main", func_001DC6C0);

INCLUDE_ASM(const s32, "core/main", func_001DC6E8);

INCLUDE_ASM(const s32, "core/main", func_001DC728);

INCLUDE_ASM(const s32, "core/main", func_001DC760);

INCLUDE_ASM(const s32, "core/main", func_001DC7A0);

INCLUDE_ASM(const s32, "core/main", func_001DC7E0);

INCLUDE_ASM(const s32, "core/main", func_001DC850);

INCLUDE_ASM(const s32, "core/main", func_001DC8C8);

INCLUDE_ASM(const s32, "core/main", func_001DCA40);

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

INCLUDE_ASM(const s32, "core/main", func_001DD710);

void func_001DD748(s32 arg1)
{
    D_00393AE6 = 0x20;
    D_00393AE7 = 1;
}

INCLUDE_ASM(const s32, "core/main", func_001DD760);

INCLUDE_ASM(const s32, "core/main", func_001DD7B0);

INCLUDE_ASM(const s32, "core/main", func_001DD7F8);

INCLUDE_ASM(const s32, "core/main", func_001DD980);

INCLUDE_ASM(const s32, "core/main", func_001DDAD8);

INCLUDE_ASM(const s32, "core/main", func_001DDC90);

INCLUDE_ASM(const s32, "core/main", func_001DDD48);

INCLUDE_ASM(const s32, "core/main", func_001DDEB8);

void func_001DDF78(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001DDF80);

INCLUDE_ASM(const s32, "core/main", func_001DDFE0);

void func_001DE020(void) { func_001DDF80(); }

INCLUDE_ASM(const s32, "core/main", func_001DE028);

INCLUDE_ASM(const s32, "core/main", func_001DE080);

INCLUDE_ASM(const s32, "core/main", func_001DE0C8);

INCLUDE_ASM(const s32, "core/main", func_001DE148);

INCLUDE_ASM(const s32, "core/main", func_001DE1A8);

INCLUDE_ASM(const s32, "core/main", func_001DE258);

INCLUDE_ASM(const s32, "core/main", func_001DE2F0);

INCLUDE_ASM(const s32, "core/main", func_001DE4D0);

void func_001DE550(float *src, u8 *unused, u8 *dst)
{
    *(f32 *)dst = *src;
    dst[0xC] = 8;
}

INCLUDE_ASM(const s32, "core/main", func_001DE568);

void func_001DE5A0(u8 *src, u8 *unused, u8 *out)
{
    *(s32 *)out = *(s32 *)src;
    out[0xC] = 0x1e;
}

INCLUDE_ASM(const s32, "core/main", func_001DE5B8);

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

INCLUDE_ASM(const s32, "core/main", func_001DE880);

INCLUDE_ASM(const s32, "core/main", func_001DE8A8);

INCLUDE_ASM(const s32, "core/main", func_001DE8D0);

INCLUDE_ASM(const s32, "core/main", func_001DE8F8);

INCLUDE_ASM(const s32, "core/main", func_001DE908);

INCLUDE_ASM(const s32, "core/main", func_001DE918);

INCLUDE_ASM(const s32, "core/main", func_001DE938);

INCLUDE_ASM(const s32, "core/main", func_001DE948);

INCLUDE_ASM(const s32, "core/main", func_001DE958);

INCLUDE_ASM(const s32, "core/main", func_001DE968);

INCLUDE_ASM(const s32, "core/main", func_001DE978);

INCLUDE_ASM(const s32, "core/main", func_001DE9B0);

INCLUDE_ASM(const s32, "core/main", func_001DE9F0);

INCLUDE_ASM(const s32, "core/main", func_001DEA30);

INCLUDE_ASM(const s32, "core/main", func_001DEA70);

INCLUDE_ASM(const s32, "core/main", func_001DEAB0);

INCLUDE_ASM(const s32, "core/main", func_001DEAF0);

INCLUDE_ASM(const s32, "core/main", func_001DEB30);

INCLUDE_ASM(const s32, "core/main", func_001DEB70);

INCLUDE_ASM(const s32, "core/main", func_001DEB80);

INCLUDE_ASM(const s32, "core/main", func_001DEB90);

INCLUDE_ASM(const s32, "core/main", func_001DEBA0);

INCLUDE_ASM(const s32, "core/main", func_001DEBB0);

INCLUDE_ASM(const s32, "core/main", func_001DEBC0);

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

INCLUDE_ASM(const s32, "core/main", func_001DEF30);

INCLUDE_ASM(const s32, "core/main", func_001DEF60);

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

INCLUDE_ASM(const s32, "core/main", func_001DF128);

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

INCLUDE_ASM(const s32, "core/main", func_001DF310);

INCLUDE_ASM(const s32, "core/main", func_001DF348);

INCLUDE_ASM(const s32, "core/main", func_001DF3A8);

INCLUDE_ASM(const s32, "core/main", func_001DF3E0);

INCLUDE_ASM(const s32, "core/main", func_001DF418);

INCLUDE_ASM(const s32, "core/main", func_001DF470);

INCLUDE_ASM(const s32, "core/main", func_001DF4C0);

INCLUDE_ASM(const s32, "core/main", func_001DF510);

INCLUDE_ASM(const s32, "core/main", func_001DF560);

INCLUDE_ASM(const s32, "core/main", func_001DF598);

INCLUDE_ASM(const s32, "core/main", func_001DF618);

INCLUDE_ASM(const s32, "core/main", func_001DF760);

INCLUDE_ASM(const s32, "core/main", func_001DF798);

INCLUDE_ASM(const s32, "core/main", func_001DF7F0);

INCLUDE_ASM(const s32, "core/main", func_001DF8D8);

INCLUDE_ASM(const s32, "core/main", func_001DFA10);

INCLUDE_ASM(const s32, "core/main", func_001DFAE8);

INCLUDE_ASM(const s32, "core/main", func_001DFB48);

INCLUDE_ASM(const s32, "core/main", func_001DFC00);

INCLUDE_ASM(const s32, "core/main", func_001DFC30);

INCLUDE_ASM(const s32, "core/main", func_001DFC78);

INCLUDE_ASM(const s32, "core/main", func_001DFCD8);

INCLUDE_ASM(const s32, "core/main", func_001DFD08);

INCLUDE_ASM(const s32, "core/main", func_001DFDD8);

INCLUDE_ASM(const s32, "core/main", func_001DFED8);

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

INCLUDE_ASM(const s32, "core/main", func_001E0AD0);

INCLUDE_ASM(const s32, "core/main", func_001E0B08);

INCLUDE_ASM(const s32, "core/main", func_001E0B80);

INCLUDE_ASM(const s32, "core/main", func_001E0BB8);

s32 func_001E0BE0(u8 *p)
{
    return p ? *(s32 *)(p + 0x18) : 0;
}

INCLUDE_ASM(const s32, "core/main", func_001E0BF8);

INCLUDE_ASM(const s32, "core/main", func_001E0C30);

INCLUDE_ASM(const s32, "core/main", func_001E0C68);

INCLUDE_ASM(const s32, "core/main", func_001E0CA0);

INCLUDE_ASM(const s32, "core/main", func_001E0CD8);

INCLUDE_ASM(const s32, "core/main", func_001E0D20);

INCLUDE_ASM(const s32, "core/main", func_001E0D60);

INCLUDE_ASM(const s32, "core/main", func_001E0DD8);

INCLUDE_ASM(const s32, "core/main", func_001E0E98);

INCLUDE_ASM(const s32, "core/main", func_001E0EF0);

INCLUDE_ASM(const s32, "core/main", func_001E0F10);

INCLUDE_ASM(const s32, "core/main", func_001E0F78);

INCLUDE_ASM(const s32, "core/main", func_001E0FB8);

INCLUDE_ASM(const s32, "core/main", func_001E1010);

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

INCLUDE_ASM(const s32, "core/main", func_001E3608);

INCLUDE_ASM(const s32, "core/main", func_001E3648);

INCLUDE_ASM(const s32, "core/main", func_001E3700);

INCLUDE_ASM(const s32, "core/main", func_001E37B8);

INCLUDE_ASM(const s32, "core/main", func_001E38C8);

INCLUDE_ASM(const s32, "core/main", func_001E3978);

INCLUDE_ASM(const s32, "core/main", func_001E39E0);

INCLUDE_ASM(const s32, "core/main", func_001E3A40);

INCLUDE_ASM(const s32, "core/main", func_001E3A88);

INCLUDE_ASM(const s32, "core/main", func_001E3B20);

INCLUDE_ASM(const s32, "core/main", func_001E3D68);

INCLUDE_ASM(const s32, "core/main", func_001E3DC8);

INCLUDE_ASM(const s32, "core/main", func_001E3E48);

INCLUDE_ASM(const s32, "core/main", func_001E3ED8);

INCLUDE_ASM(const s32, "core/main", func_001E3F18);

INCLUDE_ASM(const s32, "core/main", func_001E3F58);

INCLUDE_ASM(const s32, "core/main", func_001E4070);

INCLUDE_ASM(const s32, "core/main", func_001E41B8);

INCLUDE_ASM(const s32, "core/main", func_001E45D0);

INCLUDE_ASM(const s32, "core/main", func_001E4778);

INCLUDE_ASM(const s32, "core/main", func_001E4A58);

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

INCLUDE_ASM(const s32, "core/main", func_001E5920);

INCLUDE_ASM(const s32, "core/main", func_001E5958);

INCLUDE_ASM(const s32, "core/main", func_001E5F80);

INCLUDE_ASM(const s32, "core/main", func_001E60F0);

INCLUDE_ASM(const s32, "core/main", func_001E6190);

INCLUDE_ASM(const s32, "core/main", func_001E6218);

INCLUDE_ASM(const s32, "core/main", func_001E6368);

INCLUDE_ASM(const s32, "core/main", func_001E66F8);

INCLUDE_ASM(const s32, "core/main", func_001E67B0);

INCLUDE_ASM(const s32, "core/main", func_001E6870);

INCLUDE_ASM(const s32, "core/main", func_001E68B0);

INCLUDE_ASM(const s32, "core/main", func_001E6A88);

INCLUDE_ASM(const s32, "core/main", func_001E6BC0);

INCLUDE_ASM(const s32, "core/main", func_001E6CF0);

INCLUDE_ASM(const s32, "core/main", func_001E6E00);

s32 func_001E6E20(void) { return 148; }

INCLUDE_ASM(const s32, "core/main", func_001E6E28);

INCLUDE_ASM(const s32, "core/main", func_001E6E48);

s32 func_001E6E68(void) { return 30; }

s32 func_001E6E70(void) { return 532; }

INCLUDE_ASM(const s32, "core/main", func_001E6E78);

s32 func_001E6E98(void) { return 3; }

s32 func_001E6EA0(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_001E6EA8);

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

INCLUDE_ASM(const s32, "core/main", func_001E6F00);

INCLUDE_ASM(const s32, "core/main", func_001E6F28);

INCLUDE_ASM(const s32, "core/main", func_001E6F50);

void func_001E6F78(void) {
}

INCLUDE_ASM(const s32, "core/main", func_001E6F80);

INCLUDE_ASM(const s32, "core/main", func_001E7028);

INCLUDE_ASM(const s32, "core/main", func_001E7078);

INCLUDE_ASM(const s32, "core/main", func_001E70C0);

INCLUDE_ASM(const s32, "core/main", func_001E70F0);

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

INCLUDE_ASM(const s32, "core/main", func_001E83A8);

INCLUDE_ASM(const s32, "core/main", func_001E83F8);

INCLUDE_ASM(const s32, "core/main", func_001E8450);

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

INCLUDE_ASM(const s32, "core/main", func_001EBBF8);

INCLUDE_ASM(const s32, "core/main", func_001EBC60);

INCLUDE_ASM(const s32, "core/main", func_001EBCA0);

INCLUDE_ASM(const s32, "core/main", func_001EBD10);

INCLUDE_ASM(const s32, "core/main", func_001EBD60);

INCLUDE_ASM(const s32, "core/main", func_001EBEF8);

INCLUDE_ASM(const s32, "core/main", func_001EBF88);

INCLUDE_ASM(const s32, "core/main", func_001EBFC8);

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

INCLUDE_ASM(const s32, "core/main", func_001ECA60);

INCLUDE_ASM(const s32, "core/main", func_001ECAB0);

INCLUDE_ASM(const s32, "core/main", func_001ECE08);

INCLUDE_ASM(const s32, "core/main", func_001ED080);

INCLUDE_ASM(const s32, "core/main", func_001ED0F8);

INCLUDE_ASM(const s32, "core/main", func_001ED370);

INCLUDE_ASM(const s32, "core/main", func_001ED4C0);

INCLUDE_ASM(const s32, "core/main", func_001ED530);

INCLUDE_ASM(const s32, "core/main", func_001ED8F8);

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

INCLUDE_ASM(const s32, "core/main", func_001EECE0);

INCLUDE_ASM(const s32, "core/main", func_001EED28);

INCLUDE_ASM(const s32, "core/main", func_001EED48);

INCLUDE_ASM(const s32, "core/main", func_001EED68);

INCLUDE_ASM(const s32, "core/main", func_001EED88);

INCLUDE_ASM(const s32, "core/main", func_001EEDA8);

INCLUDE_ASM(const s32, "core/main", func_001EEDF8);

s32 func_001EEE90(s32 unused, s32 value) { return value; }

s32 func_001EEE98(s32 unused, s32 value) { return value; }

s32 func_001EEEA0(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_001EEEA8);

INCLUDE_ASM(const s32, "core/main", func_001EEEC8);

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

INCLUDE_ASM(const s32, "core/main", func_001EF1E8);

INCLUDE_ASM(const s32, "core/main", func_001EF220);

INCLUDE_ASM(const s32, "core/main", func_001EF420);

INCLUDE_ASM(const s32, "core/main", func_001EF4C0);

INCLUDE_ASM(const s32, "core/main", func_001EF5D8);

INCLUDE_ASM(const s32, "core/main", func_001EF6C0);

INCLUDE_ASM(const s32, "core/main", func_001EF720);

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

INCLUDE_ASM(const s32, "core/main", func_001F0B70);

INCLUDE_ASM(const s32, "core/main", func_001F0BB0);

INCLUDE_ASM(const s32, "core/main", func_001F0D88);

INCLUDE_ASM(const s32, "core/main", func_001F0E28);

s32 func_001F0F18(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_001F0F20);

INCLUDE_ASM(const s32, "core/main", func_001F1100);

INCLUDE_ASM(const s32, "core/main", func_001F1210);

INCLUDE_ASM(const s32, "core/main", func_001F1388);

INCLUDE_ASM(const s32, "core/main", func_001F1420);

INCLUDE_ASM(const s32, "core/main", func_001F1450);

INCLUDE_ASM(const s32, "core/main", func_001F1480);

INCLUDE_ASM(const s32, "core/main", func_001F14B0);

INCLUDE_ASM(const s32, "core/main", func_001F14E0);

INCLUDE_ASM(const s32, "core/main", func_001F1540);

INCLUDE_ASM(const s32, "core/main", func_001F1580);

INCLUDE_ASM(const s32, "core/main", func_001F1670);

INCLUDE_ASM(const s32, "core/main", func_001F1988);

INCLUDE_ASM(const s32, "core/main", func_001F1A18);

INCLUDE_ASM(const s32, "core/main", func_001F1A58);

INCLUDE_ASM(const s32, "core/main", func_001F1AE0);

u8 *func_001F1BD0(u8 *unused, u8 *base, u8 *out)
{
    *(s32 *)out = 0;
    out[0xC] = 0x18;
    return base;
}

INCLUDE_ASM(const s32, "core/main", func_001F1BE8);

INCLUDE_ASM(const s32, "core/main", func_001F1C10);

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

INCLUDE_ASM(const s32, "core/main", func_001F1D48);

u8 *func_001F1D68(u8 *unused, u8 *base)
{
    base -= 8;
    return base + *(u16 *)(base + 0x4) * 8;
}

INCLUDE_ASM(const s32, "core/main", func_001F1D80);

INCLUDE_ASM(const s32, "core/main", func_001F1E90);

INCLUDE_ASM(const s32, "core/main", func_001F1F30);

INCLUDE_ASM(const s32, "core/main", func_001F1F50);

INCLUDE_ASM(const s32, "core/main", func_001F1FE0);

INCLUDE_ASM(const s32, "core/main", func_001F2048);

INCLUDE_ASM(const s32, "core/main", func_001F2298);

INCLUDE_ASM(const s32, "core/main", func_001F24B0);

void func_001F2500(u32 value) { func_00200FA0((u16)value); }

void func_001F2508(u32 value) { func_00200E60((u16)value); }

void func_001F2510(u32 a, u32 b, u8 *ctx)
{
    u8 *p = *(u8 **)(ctx + 0x4);

    func_001DE818(a & 0xFF, b & 0xFF, *(s32 *)(*(u32 *)(p + 0xC)));
}

void func_001F2528(u32 a, u32 b, u8 *ctx)
{
    u8 *p = *(u8 **)(ctx + 0x4);

    func_001DEC30(a & 0xFF, b & 0xFF, *(s32 *)(*(u32 *)(p + 0xC)));
}

INCLUDE_ASM(const s32, "core/main", func_001F2540);

INCLUDE_ASM(const s32, "core/main", func_001F25C8);

INCLUDE_ASM(const s32, "core/main", func_001F2690);

INCLUDE_ASM(const s32, "core/main", func_001F2728);

INCLUDE_ASM(const s32, "core/main", func_001F2778);

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

INCLUDE_ASM(const s32, "core/main", func_001F2F88);

INCLUDE_ASM(const s32, "core/main", func_001F2FA8);

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

INCLUDE_ASM(const s32, "core/main", func_001F5588);

INCLUDE_ASM(const s32, "core/main", func_001F55D0);

INCLUDE_ASM(const s32, "core/main", func_001F5668);

INCLUDE_ASM(const s32, "core/main", func_001F5700);

INCLUDE_ASM(const s32, "core/main", func_001F5758);

INCLUDE_ASM(const s32, "core/main", func_001F57A8);

INCLUDE_ASM(const s32, "core/main", func_001F5838);

INCLUDE_ASM(const s32, "core/main", func_001F5910);

INCLUDE_ASM(const s32, "core/main", func_001F5A80);

INCLUDE_ASM(const s32, "core/main", func_001F5BC8);

INCLUDE_ASM(const s32, "core/main", func_001F5C40);

INCLUDE_ASM(const s32, "core/main", func_001F5CF8);

INCLUDE_ASM(const s32, "core/main", func_001F5EE0);

INCLUDE_ASM(const s32, "core/main", func_001F5F58);

INCLUDE_ASM(const s32, "core/main", func_001F6C40);

INCLUDE_ASM(const s32, "core/main", func_001F6EA8);

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

INCLUDE_ASM(const s32, "core/main", func_001FA3F8);

INCLUDE_ASM(const s32, "core/main", func_001FA460);

INCLUDE_ASM(const s32, "core/main", func_001FA5F0);

INCLUDE_ASM(const s32, "core/main", func_001FA7B8);

INCLUDE_ASM(const s32, "core/main", func_001FA8D8);

INCLUDE_ASM(const s32, "core/main", func_001FA988);

INCLUDE_ASM(const s32, "core/main", func_001FAA08);

INCLUDE_ASM(const s32, "core/main", func_001FAAA0);

INCLUDE_ASM(const s32, "core/main", func_001FB9E0);

INCLUDE_ASM(const s32, "core/main", func_001FC930);

INCLUDE_ASM(const s32, "core/main", func_001FCAA8);

INCLUDE_ASM(const s32, "core/main", func_001FCB20);

INCLUDE_ASM(const s32, "core/main", func_001FCC50);

INCLUDE_ASM(const s32, "core/main", func_001FCCC8);

INCLUDE_ASM(const s32, "core/main", func_001FCCE8);

INCLUDE_ASM(const s32, "core/main", func_001FCD08);

INCLUDE_ASM(const s32, "core/main", func_001FCD28);

INCLUDE_ASM(const s32, "core/main", func_001FCD60);

INCLUDE_ASM(const s32, "core/main", func_001FCD98);

INCLUDE_ASM(const s32, "core/main", func_001FCE30);

s32 func_001FCED8(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_001FCEE0);

INCLUDE_ASM(const s32, "core/main", func_001FD190);

INCLUDE_ASM(const s32, "core/main", func_001FD1B8);

INCLUDE_ASM(const s32, "core/main", func_001FD1E0);

INCLUDE_ASM(const s32, "core/main", func_001FD240);

INCLUDE_ASM(const s32, "core/main", func_001FD2A0);

INCLUDE_ASM(const s32, "core/main", func_001FD390);

INCLUDE_ASM(const s32, "core/main", func_001FD4B8);

INCLUDE_ASM(const s32, "core/main", func_001FD5B8);

INCLUDE_ASM(const s32, "core/main", func_001FD738);

INCLUDE_ASM(const s32, "core/main", func_001FD8D0);

INCLUDE_ASM(const s32, "core/main", func_001FD9F8);

INCLUDE_ASM(const s32, "core/main", func_001FDA30);

INCLUDE_ASM(const s32, "core/main", func_001FDBD0);

INCLUDE_ASM(const s32, "core/main", func_001FDC70);

INCLUDE_ASM(const s32, "core/main", func_001FDF80);

INCLUDE_ASM(const s32, "core/main", func_001FE028);

INCLUDE_ASM(const s32, "core/main", func_001FE1A8);

INCLUDE_ASM(const s32, "core/main", func_001FE240);

INCLUDE_ASM(const s32, "core/main", func_001FE478);

INCLUDE_ASM(const s32, "core/main", func_001FE4E8);

INCLUDE_ASM(const s32, "core/main", func_001FE680);

INCLUDE_ASM(const s32, "core/main", func_001FE6A8);

INCLUDE_ASM(const s32, "core/main", func_001FE6D0);

INCLUDE_ASM(const s32, "core/main", func_001FE708);

INCLUDE_ASM(const s32, "core/main", func_001FE740);

INCLUDE_ASM(const s32, "core/main", func_001FE878);

INCLUDE_ASM(const s32, "core/main", func_001FE8E0);

INCLUDE_ASM(const s32, "core/main", func_001FE9E0);

INCLUDE_ASM(const s32, "core/main", func_001FEAC0);

INCLUDE_ASM(const s32, "core/main", func_001FEB10);

INCLUDE_ASM(const s32, "core/main", func_001FEB98);

INCLUDE_ASM(const s32, "core/main", func_001FECD0);

INCLUDE_ASM(const s32, "core/main", func_001FED50);

INCLUDE_ASM(const s32, "core/main", func_001FEDF8);

INCLUDE_ASM(const s32, "core/main", func_001FEE30);

INCLUDE_ASM(const s32, "core/main", func_001FEE80);

INCLUDE_ASM(const s32, "core/main", func_001FEF40);

INCLUDE_ASM(const s32, "core/main", func_001FF098);

INCLUDE_ASM(const s32, "core/main", func_001FF298);

INCLUDE_ASM(const s32, "core/main", func_001FF380);

INCLUDE_ASM(const s32, "core/main", func_001FF410);

INCLUDE_ASM(const s32, "core/main", func_001FF460);

INCLUDE_ASM(const s32, "core/main", func_001FF748);

INCLUDE_ASM(const s32, "core/main", func_001FF9C8);

INCLUDE_ASM(const s32, "core/main", func_001FFA80);

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


INCLUDE_ASM(const s32, "core/main", func_00200048);

INCLUDE_ASM(const s32, "core/main", func_002000A0);

INCLUDE_ASM(const s32, "core/main", func_002001A8);

INCLUDE_ASM(const s32, "core/main", func_002001D8);

INCLUDE_ASM(const s32, "core/main", func_00200230);

INCLUDE_ASM(const s32, "core/main", func_00200260);

INCLUDE_ASM(const s32, "core/main", func_002002B8);

s32 func_00200358(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_00200360);

INCLUDE_ASM(const s32, "core/main", func_00200550);

s32 func_002006F0(void) { return 4; }

INCLUDE_ASM(const s32, "core/main", func_002006F8);

INCLUDE_ASM(const s32, "core/main", func_00200748);

INCLUDE_ASM(const s32, "core/main", func_00200798);

INCLUDE_ASM(const s32, "core/main", func_002007F0);

INCLUDE_ASM(const s32, "core/main", func_00200840);

INCLUDE_ASM(const s32, "core/main", func_00200898);

INCLUDE_ASM(const s32, "core/main", func_002008E8);

INCLUDE_ASM(const s32, "core/main", func_00200950);

INCLUDE_ASM(const s32, "core/main", func_002009A0);

INCLUDE_ASM(const s32, "core/main", func_002009F8);

INCLUDE_ASM(const s32, "core/main", func_00200A58);

INCLUDE_ASM(const s32, "core/main", func_00200AA8);

INCLUDE_ASM(const s32, "core/main", func_00200B28);

INCLUDE_ASM(const s32, "core/main", func_00200B90);

INCLUDE_ASM(const s32, "core/main", func_00200BF8);

INCLUDE_ASM(const s32, "core/main", func_00200CA8);

INCLUDE_ASM(const s32, "core/main", func_00200D50);

INCLUDE_ASM(const s32, "core/main", func_00200DD0);

void func_00200E18(void) {
}

void func_00200E20(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00200E28);

void func_00200E50(void) { func_001E0F78(); }

void func_00200E58(void) { func_001E0F78(); }

INCLUDE_ASM(const s32, "core/main", func_00200E60);

void func_00200EE8(void) {
}

void func_00200EF0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00200EF8);

INCLUDE_ASM(const s32, "core/main", func_00200F20);

INCLUDE_ASM(const s32, "core/main", func_00200F60);

INCLUDE_ASM(const s32, "core/main", func_00200FA0);

INCLUDE_ASM(const s32, "core/main", func_00201048);

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

INCLUDE_ASM(const s32, "core/main", func_00201AE0);

INCLUDE_ASM(const s32, "core/main", func_00201B18);

INCLUDE_ASM(const s32, "core/main", func_00201B58);

INCLUDE_ASM(const s32, "core/main", func_00201BB0);

INCLUDE_ASM(const s32, "core/main", func_00201C88);

INCLUDE_ASM(const s32, "core/main", func_00201F50);

INCLUDE_ASM(const s32, "core/main", func_00201F80);

INCLUDE_ASM(const s32, "core/main", func_00202050);

INCLUDE_ASM(const s32, "core/main", func_00202180);

INCLUDE_ASM(const s32, "core/main", func_00202218);

INCLUDE_ASM(const s32, "core/main", func_00202258);

INCLUDE_ASM(const s32, "core/main", func_00202298);

INCLUDE_ASM(const s32, "core/main", func_002022D8);

INCLUDE_ASM(const s32, "core/main", func_00202340);

void func_00202368(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00202370);

f32 func_00202390(const f32 *p) { return p[3]; }

void func_00202398(void) {
}

INCLUDE_ASM(const s32, "core/main", func_002023A0);

void func_00202408(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00202410);

INCLUDE_ASM(const s32, "core/main", func_002024A0);

INCLUDE_ASM(const s32, "core/main", func_00202520);

INCLUDE_ASM(const s32, "core/main", func_00202550);

INCLUDE_ASM(const s32, "core/main", func_00202580);

INCLUDE_ASM(const s32, "core/main", func_002025E0);

INCLUDE_ASM(const s32, "core/main", func_00202630);

INCLUDE_ASM(const s32, "core/main", func_00202868);

INCLUDE_ASM(const s32, "core/main", func_00202AB8);

INCLUDE_ASM(const s32, "core/main", func_00202F28);

INCLUDE_ASM(const s32, "core/main", func_002030C8);

INCLUDE_ASM(const s32, "core/main", func_002033D8);

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

INCLUDE_ASM(const s32, "core/main", func_00204BA0);

INCLUDE_ASM(const s32, "core/main", func_00204BE0);

INCLUDE_ASM(const s32, "core/main", func_00204F50);

INCLUDE_ASM(const s32, "core/main", func_00204FC0);

INCLUDE_ASM(const s32, "core/main", func_002055A0);

INCLUDE_ASM(const s32, "core/main", func_00205810);

INCLUDE_ASM(const s32, "core/main", func_00205A60);

INCLUDE_ASM(const s32, "core/main", func_00205AA8);

void func_00205CA0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00205CA8);

INCLUDE_ASM(const s32, "core/main", func_00205DA0);

INCLUDE_ASM(const s32, "core/main", func_00205DB0);

INCLUDE_ASM(const s32, "core/main", func_002061E0);

INCLUDE_ASM(const s32, "core/main", func_002063F8);

INCLUDE_ASM(const s32, "core/main", func_00206450);

INCLUDE_ASM(const s32, "core/main", func_00206490);

INCLUDE_ASM(const s32, "core/main", func_002065C0);

INCLUDE_ASM(const s32, "core/main", func_00206980);

INCLUDE_ASM(const s32, "core/main", func_00206A60);

INCLUDE_ASM(const s32, "core/main", func_00206B20);

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

INCLUDE_ASM(const s32, "core/main", func_00208480);

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

INCLUDE_ASM(const s32, "core/main", func_002093A8);

u8 *func_002093E0(u8 *unused, u8 *ret, u8 *out)
{
    *(s32 *)out = D_00253627;
    return ret;
}

INCLUDE_ASM(const s32, "core/main", func_002093F8);

INCLUDE_ASM(const s32, "core/main", func_00209430);

INCLUDE_ASM(const s32, "core/main", func_00209468);

INCLUDE_ASM(const s32, "core/main", func_002094B0);

INCLUDE_ASM(const s32, "core/main", func_00209528);

INCLUDE_ASM(const s32, "core/main", func_002095C8);

INCLUDE_ASM(const s32, "core/main", func_00209648);

INCLUDE_ASM(const s32, "core/main", func_002096C8);

INCLUDE_ASM(const s32, "core/main", func_00209790);

INCLUDE_ASM(const s32, "core/main", func_002097E0);

INCLUDE_ASM(const s32, "core/main", func_00209860);

INCLUDE_ASM(const s32, "core/main", func_002098E8);

INCLUDE_ASM(const s32, "core/main", func_002098F8);

INCLUDE_ASM(const s32, "core/main", func_00209AF8);

INCLUDE_ASM(const s32, "core/main", func_00209BE0);

INCLUDE_ASM(const s32, "core/main", func_00209C70);

INCLUDE_ASM(const s32, "core/main", func_00209D00);

INCLUDE_ASM(const s32, "core/main", func_00209D90);

INCLUDE_ASM(const s32, "core/main", func_00209E20);

INCLUDE_ASM(const s32, "core/main", func_00209EB0);

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

INCLUDE_ASM(const s32, "core/main", func_0020B500);

INCLUDE_ASM(const s32, "core/main", func_0020B530);

INCLUDE_ASM(const s32, "core/main", func_0020B6B8);

INCLUDE_ASM(const s32, "core/main", func_0020B810);

INCLUDE_ASM(const s32, "core/main", func_0020B848);

INCLUDE_ASM(const s32, "core/main", func_0020B868);

INCLUDE_ASM(const s32, "core/main", func_0020B8A0);

INCLUDE_ASM(const s32, "core/main", func_0020B8F0);

INCLUDE_ASM(const s32, "core/main", func_0020B920);

INCLUDE_ASM(const s32, "core/main", func_0020B940);

s32 func_0020BD68(u8 *p, s32 index)
{
    return *(s16 *)(*(u32 *)(p + 0x8) + (s16)index * 2);
}

INCLUDE_ASM(const s32, "core/main", func_0020BD80);

INCLUDE_ASM(const s32, "core/main", func_0020BDB8);

s16 func_0020BDF0(const s16 *p) { return p[1]; }

INCLUDE_ASM(const s32, "core/main", func_0020BDF8);

s32 func_0020BF80(s32 value) { return value; }

u8 *func_0020BF88(u8 *p) { return p + 12; }

INCLUDE_ASM(const s32, "core/main", func_0020BF90);

s32 func_0020BFD8(s32 value) { return value; }

f32 func_0020BFE0(const f32 *p) { return p[3]; }

INCLUDE_ASM(const s32, "core/main", func_0020BFE8);

INCLUDE_ASM(const s32, "core/main", func_0020C0D0);

INCLUDE_ASM(const s32, "core/main", func_0020C360);

INCLUDE_ASM(const s32, "core/main", func_0020C430);

INCLUDE_ASM(const s32, "core/main", func_0020C4A0);

INCLUDE_ASM(const s32, "core/main", func_0020C518);

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

INCLUDE_ASM(const s32, "core/main", func_0020D9E0);

INCLUDE_ASM(const s32, "core/main", func_0020DA40);

INCLUDE_ASM(const s32, "core/main", func_0020DAA0);

INCLUDE_ASM(const s32, "core/main", func_0020DB00);

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

INCLUDE_ASM(const s32, "core/main", func_00211630);

INCLUDE_ASM(const s32, "core/main", func_00211658);

INCLUDE_ASM(const s32, "core/main", func_00211718);

INCLUDE_ASM(const s32, "core/main", func_002118C8);

INCLUDE_ASM(const s32, "core/main", func_002118F0);

INCLUDE_ASM(const s32, "core/main", func_00211918);

INCLUDE_ASM(const s32, "core/main", func_00211948);

INCLUDE_ASM(const s32, "core/main", func_00211980);

INCLUDE_ASM(const s32, "core/main", func_002119C0);

INCLUDE_ASM(const s32, "core/main", func_00211A20);

INCLUDE_ASM(const s32, "core/main", func_00211A78);

INCLUDE_ASM(const s32, "core/main", func_00211B68);

INCLUDE_ASM(const s32, "core/main", func_00211B88);

INCLUDE_ASM(const s32, "core/main", func_00211C38);

INCLUDE_ASM(const s32, "core/main", func_00211D00);

INCLUDE_ASM(const s32, "core/main", func_00211D50);

INCLUDE_ASM(const s32, "core/main", func_00211D88);

INCLUDE_ASM(const s32, "core/main", func_00211DD0);

INCLUDE_ASM(const s32, "core/main", func_00211EB8);

s32 func_00211F18(void) { return D_00393B5C; }

INCLUDE_ASM(const s32, "core/main", func_00211F20);

INCLUDE_ASM(const s32, "core/main", func_00211F58);

INCLUDE_ASM(const s32, "core/main", func_00211F78);

INCLUDE_ASM(const s32, "core/main", func_00211FD0);

INCLUDE_ASM(const s32, "core/main", func_00212028);

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

INCLUDE_ASM(const s32, "core/main", func_00212860);

INCLUDE_ASM(const s32, "core/main", func_002128A0);

INCLUDE_ASM(const s32, "core/main", func_00212950);

INCLUDE_ASM(const s32, "core/main", func_00212970);

INCLUDE_ASM(const s32, "core/main", func_00212B10);

INCLUDE_ASM(const s32, "core/main", func_00212B60);

INCLUDE_ASM(const s32, "core/main", func_00212BC0);

INCLUDE_ASM(const s32, "core/main", func_00212C18);

INCLUDE_ASM(const s32, "core/main", func_00212C48);

INCLUDE_ASM(const s32, "core/main", func_00212C78);

INCLUDE_ASM(const s32, "core/main", func_00212CA8);

INCLUDE_ASM(const s32, "core/main", func_00212DD0);

INCLUDE_ASM(const s32, "core/main", func_00212E28);

INCLUDE_ASM(const s32, "core/main", func_00212FA0);

INCLUDE_ASM(const s32, "core/main", func_00213090);

INCLUDE_ASM(const s32, "core/main", func_002130B0);

INCLUDE_ASM(const s32, "core/main", func_00213178);

INCLUDE_ASM(const s32, "core/main", func_00213258);

s32 func_002133F0(s32 index)
{
    return *(s32 *)((u8 *)D_00386D18 + index * 0x10 + 0x4);
}

INCLUDE_ASM(const s32, "core/main", func_00213408);

INCLUDE_ASM(const s32, "core/main", func_002134A0);

INCLUDE_ASM(const s32, "core/main", func_002134F0);

INCLUDE_ASM(const s32, "core/main", func_00213510);

INCLUDE_ASM(const s32, "core/main", func_00213530);

INCLUDE_ASM(const s32, "core/main", func_00213578);

INCLUDE_ASM(const s32, "core/main", func_00213690);

INCLUDE_ASM(const s32, "core/main", func_00213848);

INCLUDE_ASM(const s32, "core/main", func_00213868);

INCLUDE_ASM(const s32, "core/main", func_002138B0);

INCLUDE_ASM(const s32, "core/main", func_00213908);

INCLUDE_ASM(const s32, "core/main", func_00213918);

void func_00213928(u8 *p, u32 bits)
{
    u8 *q = *(u8 **)(p + 0x4);
    *(s32 *)(q + 0x24) = *(s32 *)(q + 0x24) | bits;
}

void func_00213940(u8 *p, u32 mask)
{
    u8 *q = *(u8 **)(p + 0x4);

    *(u32 *)(q + 0x24) &= ~mask;
}

INCLUDE_ASM(const s32, "core/main", func_00213958);

INCLUDE_ASM(const s32, "core/main", func_002139C8);

INCLUDE_ASM(const s32, "core/main", func_00213AA8);

INCLUDE_ASM(const s32, "core/main", func_00213B00);

INCLUDE_ASM(const s32, "core/main", func_00213DA8);

INCLUDE_ASM(const s32, "core/main", func_00214138);

INCLUDE_ASM(const s32, "core/main", func_002142C0);

INCLUDE_ASM(const s32, "core/main", func_002142D0);

INCLUDE_ASM(const s32, "core/main", func_002142E0);

u8 func_002142F0(s32 *p) { u8 *object = (u8 *)p[1]; u8 value = object[0x2A]; object[0x29] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_00214300);

INCLUDE_ASM(const s32, "core/main", func_00214330);

INCLUDE_ASM(const s32, "core/main", func_00214358);

INCLUDE_ASM(const s32, "core/main", func_00214368);

u8 func_00214378(s32 *p) { u8 *object = (u8 *)p[1]; u8 value = object[0x2B]; object[0x2A] = value; return value; }

INCLUDE_ASM(const s32, "core/main", func_00214388);

INCLUDE_ASM(const s32, "core/main", func_002143B8);

INCLUDE_ASM(const s32, "core/main", func_00214408);

INCLUDE_ASM(const s32, "core/main", func_00214430);

INCLUDE_ASM(const s32, "core/main", func_002144A0);

INCLUDE_ASM(const s32, "core/main", func_002144F8);

INCLUDE_ASM(const s32, "core/main", func_00214548);

INCLUDE_ASM(const s32, "core/main", func_002145A0);

INCLUDE_ASM(const s32, "core/main", func_002145C0);

INCLUDE_ASM(const s32, "core/main", func_002145E0);

INCLUDE_ASM(const s32, "core/main", func_00214628);

INCLUDE_ASM(const s32, "core/main", func_002146B8);

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

INCLUDE_ASM(const s32, "core/main", func_00214F10);

INCLUDE_ASM(const s32, "core/main", func_00214F20);

INCLUDE_ASM(const s32, "core/main", func_00214F30);

INCLUDE_ASM(const s32, "core/main", func_00214F40);

INCLUDE_ASM(const s32, "core/main", func_00214F90);

INCLUDE_ASM(const s32, "core/main", func_002152C8);

INCLUDE_ASM(const s32, "core/main", func_002153A8);

INCLUDE_ASM(const s32, "core/main", func_002154D8);

INCLUDE_ASM(const s32, "core/main", func_00215508);

INCLUDE_ASM(const s32, "core/main", func_00215600);

INCLUDE_ASM(const s32, "core/main", func_00215690);

INCLUDE_ASM(const s32, "core/main", func_00215810);

INCLUDE_ASM(const s32, "core/main", func_00215860);

INCLUDE_ASM(const s32, "core/main", func_00215B78);

INCLUDE_ASM(const s32, "core/main", func_00215B98);

INCLUDE_ASM(const s32, "core/main", func_00215BB8);

INCLUDE_ASM(const s32, "core/main", func_00215E28);

INCLUDE_ASM(const s32, "core/main", func_00215E78);

INCLUDE_ASM(const s32, "core/main", func_00215EC8);

INCLUDE_ASM(const s32, "core/main", func_00215EF8);

INCLUDE_ASM(const s32, "core/main", func_00215F18);

void func_00215F38(u32 a, u32 b)
{
    func_00215EC8(a, 0x52, ((u64)b << 32) | a);
}

void func_00215F50(s32 arg, u32 value)
{
    func_00215EC8(arg, 0x53, (u64)value);
}

INCLUDE_ASM(const s32, "core/main", func_00215F60);

void func_00215FA0(void) {
}

INCLUDE_ASM(const s32, "core/main", func_00215FA8);

INCLUDE_ASM(const s32, "core/main", func_002160A0);

INCLUDE_ASM(const s32, "core/main", func_002160E0);

INCLUDE_ASM(const s32, "core/main", func_00216170);

INCLUDE_ASM(const s32, "core/main", func_002162E8);

INCLUDE_ASM(const s32, "core/main", func_002163D8);

INCLUDE_ASM(const s32, "core/main", func_00216468);

INCLUDE_ASM(const s32, "core/main", func_00216488);

INCLUDE_ASM(const s32, "core/main", func_002164C0);

INCLUDE_ASM(const s32, "core/main", func_00216588);

INCLUDE_ASM(const s32, "core/main", func_00216648);

INCLUDE_ASM(const s32, "core/main", func_002166B8);

INCLUDE_ASM(const s32, "core/main", func_00216728);

INCLUDE_ASM(const s32, "core/main", func_00216798);

INCLUDE_ASM(const s32, "core/main", func_00216918);

INCLUDE_ASM(const s32, "core/main", func_00216938);

INCLUDE_ASM(const s32, "core/main", func_00216978);

INCLUDE_ASM(const s32, "core/main", func_00216988);

INCLUDE_ASM(const s32, "core/main", func_00216A50);

INCLUDE_ASM(const s32, "core/main", func_00216AC8);

INCLUDE_ASM(const s32, "core/main", func_00216AE0);

INCLUDE_ASM(const s32, "core/main", func_00216B00);

INCLUDE_ASM(const s32, "core/main", func_00216BA0);

INCLUDE_ASM(const s32, "core/main", func_00216BE8);

INCLUDE_ASM(const s32, "core/main", func_00216C90);

INCLUDE_ASM(const s32, "core/main", func_00216D30);

INCLUDE_ASM(const s32, "core/main", func_00216DD0);

INCLUDE_ASM(const s32, "core/main", func_00216DF0);

INCLUDE_ASM(const s32, "core/main", func_00216EE0);

INCLUDE_ASM(const s32, "core/main", func_00217000);

INCLUDE_ASM(const s32, "core/main", func_00217078);

INCLUDE_ASM(const s32, "core/main", func_002170A8);

INCLUDE_ASM(const s32, "core/main", func_002170C8);

INCLUDE_ASM(const s32, "core/main", func_002170F8);

INCLUDE_ASM(const s32, "core/main", func_00217318);

INCLUDE_ASM(const s32, "core/main", func_00217388);

INCLUDE_ASM(const s32, "core/main", func_002174C0);

INCLUDE_ASM(const s32, "core/main", func_002175B0);

INCLUDE_ASM(const s32, "core/main", func_00217640);

INCLUDE_ASM(const s32, "core/main", func_00217818);

INCLUDE_ASM(const s32, "core/main", func_00217AC0);

INCLUDE_ASM(const s32, "core/main", func_00217BA8);

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

INCLUDE_ASM(const s32, "core/main", func_0021A978);

INCLUDE_ASM(const s32, "core/main", func_0021A9A0);

INCLUDE_ASM(const s32, "core/main", func_0021ABA0);

INCLUDE_ASM(const s32, "core/main", func_0021ABC8);

INCLUDE_ASM(const s32, "core/main", func_0021ABF0);

INCLUDE_ASM(const s32, "core/main", func_0021AD50);

INCLUDE_ASM(const s32, "core/main", func_0021AD80);

INCLUDE_ASM(const s32, "core/main", func_0021ADD8);

INCLUDE_ASM(const s32, "core/main", func_0021AE88);

INCLUDE_ASM(const s32, "core/main", func_0021AEA8);

INCLUDE_ASM(const s32, "core/main", func_0021B598);

INCLUDE_ASM(const s32, "core/main", func_0021B5A8);

INCLUDE_ASM(const s32, "core/main", func_0021B5D0);

void func_0021B600(void) {
}

INCLUDE_ASM(const s32, "core/main", func_0021B608);

INCLUDE_ASM(const s32, "core/main", func_0021B660);

INCLUDE_ASM(const s32, "core/main", func_0021B688);

INCLUDE_ASM(const s32, "core/main", func_0021B6B0);

INCLUDE_ASM(const s32, "core/main", func_0021B6D8);

INCLUDE_ASM(const s32, "core/main", func_0021B740);

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

INCLUDE_ASM(const s32, "core/main", func_0021DDB0);

INCLUDE_ASM(const s32, "core/main", func_0021DE18);

INCLUDE_ASM(const s32, "core/main", func_0021E0C0);

INCLUDE_ASM(const s32, "core/main", func_0021E228);

INCLUDE_ASM(const s32, "core/main", func_0021E340);

INCLUDE_ASM(const s32, "core/main", func_0021E390);

INCLUDE_ASM(const s32, "core/main", func_0021E448);

INCLUDE_ASM(const s32, "core/main", func_0021E4E0);

INCLUDE_ASM(const s32, "core/main", func_0021E580);

INCLUDE_ASM(const s32, "core/main", func_0021E5B8);

INCLUDE_ASM(const s32, "core/main", func_0021E5E8);

INCLUDE_ASM(const s32, "core/main", func_0021E640);

s32 func_0021E678(s32 value) { s32 old = D_00387534[0]; D_00387534[0] = value; return old; }

INCLUDE_ASM(const s32, "core/main", func_0021E688);

INCLUDE_ASM(const s32, "core/main", func_0021E6C8);

INCLUDE_ASM(const s32, "core/main", func_0021E6E8);

INCLUDE_ASM(const s32, "core/main", func_0021E720);

INCLUDE_ASM(const s32, "core/main", func_0021E748);

INCLUDE_ASM(const s32, "core/main", func_0021E768);

INCLUDE_ASM(const s32, "core/main", func_0021E7D8);

INCLUDE_ASM(const s32, "core/main", func_0021E868);

INCLUDE_ASM(const s32, "core/main", func_0021E930);

INCLUDE_ASM(const s32, "core/main", func_0021E960);

INCLUDE_ASM(const s32, "core/main", func_0021EC50);

INCLUDE_ASM(const s32, "core/main", func_0021EC78);

u8 *func_0021ECA8(u8 *p)
{
    *(s32 *)p = (s32)D_003910D0;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0021ECC0);

INCLUDE_ASM(const s32, "core/main", func_0021ED10);

u8 *func_0021ED40(u8 *p)
{
    *(s32 *)p = (s32)D_003910E8;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_0021ED58);

INCLUDE_ASM(const s32, "core/main", func_0021ED98);

INCLUDE_ASM(const s32, "core/main", func_0021EEA8);

INCLUDE_ASM(const s32, "core/main", func_0021EF38);

INCLUDE_ASM(const s32, "core/main", func_0021F170);

INCLUDE_ASM(const s32, "core/main", func_0021F1C8);

INCLUDE_ASM(const s32, "core/main", func_0021F230);

INCLUDE_ASM(const s32, "core/main", func_0021F428);

INCLUDE_ASM(const s32, "core/main", func_0021F588);

INCLUDE_ASM(const s32, "core/main", func_0021F6A0);

INCLUDE_ASM(const s32, "core/main", func_0021F6F0);

INCLUDE_ASM(const s32, "core/main", func_0021F7A8);

INCLUDE_ASM(const s32, "core/main", func_0021F838);

INCLUDE_ASM(const s32, "core/main", func_0021F8D0);

INCLUDE_ASM(const s32, "core/main", func_0021F908);

INCLUDE_ASM(const s32, "core/main", func_0021F938);

INCLUDE_ASM(const s32, "core/main", func_0021F978);

INCLUDE_ASM(const s32, "core/main", func_0021F9C0);

INCLUDE_ASM(const s32, "core/main", func_0021FA10);

INCLUDE_ASM(const s32, "core/main", func_0021FB10);

INCLUDE_ASM(const s32, "core/main", func_0021FD78);

INCLUDE_ASM(const s32, "core/main", func_0021FDC0);

INCLUDE_ASM(const s32, "core/main", func_0021FE48);

INCLUDE_ASM(const s32, "core/main", func_0021FF88);


/* Source: misc/region_0022.c */
/* func_00220070 through func_0022E7B8; retail address range 0x00220000-0x00230000. */



extern s32 D_003878CC[8];
extern u8 D_0038857C[8];

INCLUDE_ASM(const s32, "core/main", func_00220070);

INCLUDE_ASM(const s32, "core/main", func_00220178);

INCLUDE_ASM(const s32, "core/main", func_00220510);

INCLUDE_ASM(const s32, "core/main", func_00220538);

INCLUDE_ASM(const s32, "core/main", func_00220568);

INCLUDE_ASM(const s32, "core/main", func_00220590);

INCLUDE_ASM(const s32, "core/main", func_002205C0);

INCLUDE_ASM(const s32, "core/main", func_00220638);

INCLUDE_ASM(const s32, "core/main", func_00220658);

INCLUDE_ASM(const s32, "core/main", func_00220830);

s32 func_002209A8(s32 value) { s32 old = D_003878CC[0]; D_003878CC[0] = value; return old; }

s32 func_002209B8(s32 unused, s32 value) { return value; }

s32 func_002209C0(s32 unused, s32 value) { return value; }

INCLUDE_ASM(const s32, "core/main", func_002209C8);

INCLUDE_ASM(const s32, "core/main", func_002209D8);

INCLUDE_ASM(const s32, "core/main", func_00220A28);

INCLUDE_ASM(const s32, "core/main", func_00220A58);

INCLUDE_ASM(const s32, "core/main", func_00220AD0);

INCLUDE_ASM(const s32, "core/main", func_00220BF8);

INCLUDE_ASM(const s32, "core/main", func_00220C28);

INCLUDE_ASM(const s32, "core/main", func_00220C68);

INCLUDE_ASM(const s32, "core/main", func_00220C90);

INCLUDE_ASM(const s32, "core/main", func_00220CB0);

INCLUDE_ASM(const s32, "core/main", func_00220CD0);

INCLUDE_ASM(const s32, "core/main", func_00220D00);

INCLUDE_ASM(const s32, "core/main", func_00220D98);

INCLUDE_ASM(const s32, "core/main", func_00220F70);

INCLUDE_ASM(const s32, "core/main", func_00220F90);

INCLUDE_ASM(const s32, "core/main", func_00220FE0);

INCLUDE_ASM(const s32, "core/main", func_00221000);

INCLUDE_ASM(const s32, "core/main", func_00221020);

INCLUDE_ASM(const s32, "core/main", func_00221070);

u8 *func_00221090(u8 *p, s32 value)
{
    *(s32 *)p = value;
    *(s32 *)(p + 0x4) = (s32)D_00391290;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_002210A8);

INCLUDE_ASM(const s32, "core/main", func_002210F8);

INCLUDE_ASM(const s32, "core/main", func_00221118);

u8 *func_00221148(u8 *p)
{
    *(s32 *)p = (s32)D_003912A8;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00221160);

INCLUDE_ASM(const s32, "core/main", func_002211B0);

u8 *func_002211E0(u8 *p)
{
    *(s32 *)p = (s32)D_003912C0;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_002211F8);

INCLUDE_ASM(const s32, "core/main", func_00221248);

s32 func_00221268(const s32 *p) { return *(const s32 *)((const u8 *)p + (0)); }

u8 *func_00221270(u8 *p, s32 value)
{
    *(s32 *)p = value;
    *(s32 *)(p + 0x4) = (s32)D_003912D8;
    return p;
}

INCLUDE_ASM(const s32, "core/main", func_00221288);

INCLUDE_ASM(const s32, "core/main", func_002212C8);

INCLUDE_ASM(const s32, "core/main", func_002212F0);

INCLUDE_ASM(const s32, "core/main", func_002217B8);

INCLUDE_ASM(const s32, "core/main", func_00221808);

INCLUDE_ASM(const s32, "core/main", func_00221828);

INCLUDE_ASM(const s32, "core/main", func_00221850);

INCLUDE_ASM(const s32, "core/main", func_00221870);

INCLUDE_ASM(const s32, "core/main", func_00221890);

INCLUDE_ASM(const s32, "core/main", func_002218B0);

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

INCLUDE_ASM(const s32, "core/main", func_00221D38);

INCLUDE_ASM(const s32, "core/main", func_00221D88);

INCLUDE_ASM(const s32, "core/main", func_00221DA8);

INCLUDE_ASM(const s32, "core/main", func_00221DF8);

INCLUDE_ASM(const s32, "core/main", func_00221E18);

INCLUDE_ASM(const s32, "core/main", func_00221E68);

INCLUDE_ASM(const s32, "core/main", func_00221E88);

INCLUDE_ASM(const s32, "core/main", func_00221ED8);

INCLUDE_ASM(const s32, "core/main", func_00221EF8);

INCLUDE_ASM(const s32, "core/main", func_00221F48);

INCLUDE_ASM(const s32, "core/main", func_00221F68);

INCLUDE_ASM(const s32, "core/main", func_00221FB8);

INCLUDE_ASM(const s32, "core/main", func_00221FD8);

INCLUDE_ASM(const s32, "core/main", func_00222028);

INCLUDE_ASM(const s32, "core/main", func_00222048);

INCLUDE_ASM(const s32, "core/main", func_00222068);

INCLUDE_ASM(const s32, "core/main", func_002220A0);

INCLUDE_ASM(const s32, "core/main", func_002220C8);

INCLUDE_ASM(const s32, "core/main", func_002220E8);

INCLUDE_ASM(const s32, "core/main", func_00222110);

INCLUDE_ASM(const s32, "core/main", func_00222120);

s32 func_00222228(const u16 *p) { return (p[6] >> 5) & 1; }

INCLUDE_ASM(const s32, "core/main", func_00222238);

INCLUDE_ASM(const s32, "core/main", func_00222340);

INCLUDE_ASM(const s32, "core/main", func_00222468);

INCLUDE_ASM(const s32, "core/main", func_002224C0);

INCLUDE_ASM(const s32, "core/main", func_00222530);

INCLUDE_ASM(const s32, "core/main", func_00222610);

INCLUDE_ASM(const s32, "core/main", func_00222620);

INCLUDE_ASM(const s32, "core/main", func_00222630);

INCLUDE_ASM(const s32, "core/main", func_002226C0);

INCLUDE_ASM(const s32, "core/main", func_002226F8);

INCLUDE_ASM(const s32, "core/main", func_002227D8);

INCLUDE_ASM(const s32, "core/main", func_00222808);

INCLUDE_ASM(const s32, "core/main", func_00222858);

INCLUDE_ASM(const s32, "core/main", func_00222978);

INCLUDE_ASM(const s32, "core/main", func_00222C98);

INCLUDE_ASM(const s32, "core/main", func_00222E08);

INCLUDE_ASM(const s32, "core/main", func_00223298);

INCLUDE_ASM(const s32, "core/main", func_002232F8);

INCLUDE_ASM(const s32, "core/main", func_002236D0);

INCLUDE_ASM(const s32, "core/main", func_00223768);

INCLUDE_ASM(const s32, "core/main", func_002237F0);

INCLUDE_ASM(const s32, "core/main", func_00223810);

INCLUDE_ASM(const s32, "core/main", func_00223838);

INCLUDE_ASM(const s32, "core/main", func_00223B08);

INCLUDE_ASM(const s32, "core/main", func_00223C58);

INCLUDE_ASM(const s32, "core/main", func_00223C88);

INCLUDE_ASM(const s32, "core/main", func_00223E58);

void func_00223E80(s32 arg)
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

INCLUDE_ASM(const s32, "core/main", func_00224C58);

INCLUDE_ASM(const s32, "core/main", func_00224CB8);

void func_002256E0(s32 value) { ((s32 *)*(u8 **)D_0038857C)[0x58 / 4] = value; }

INCLUDE_ASM(const s32, "core/main", func_002256F0);

INCLUDE_ASM(const s32, "core/main", func_00225720);

INCLUDE_ASM(const s32, "core/main", func_00225750);

INCLUDE_ASM(const s32, "core/main", func_00225CC0);

INCLUDE_ASM(const s32, "core/main", func_00225CE0);

INCLUDE_ASM(const s32, "core/main", func_00225E58);

INCLUDE_ASM(const s32, "core/main", func_00225E78);

INCLUDE_ASM(const s32, "core/main", func_00225ED8);

INCLUDE_ASM(const s32, "core/main", func_00225F48);

INCLUDE_ASM(const s32, "core/main", func_00225FD8);

INCLUDE_ASM(const s32, "core/main", func_002260C8);

INCLUDE_ASM(const s32, "core/main", func_00226180);

INCLUDE_ASM(const s32, "core/main", func_002261A8);

INCLUDE_ASM(const s32, "core/main", func_002261D8);

INCLUDE_ASM(const s32, "core/main", func_00226200);

INCLUDE_ASM(const s32, "core/main", func_00226228);

INCLUDE_ASM(const s32, "core/main", func_00226288);

INCLUDE_ASM(const s32, "core/main", func_002262A8);

INCLUDE_ASM(const s32, "core/main", func_00226310);

s32 func_00226380(void) { return 0; }

INCLUDE_ASM(const s32, "core/main", func_00226388);

INCLUDE_ASM(const s32, "core/main", func_00226410);

INCLUDE_ASM(const s32, "core/main", func_00226478);

INCLUDE_ASM(const s32, "core/main", func_002264F8);

INCLUDE_ASM(const s32, "core/main", func_00226560);

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

INCLUDE_ASM(const s32, "core/main", func_00227200);

INCLUDE_ASM(const s32, "core/main", func_00227250);

INCLUDE_ASM(const s32, "core/main", func_002272C0);

INCLUDE_ASM(const s32, "core/main", func_002272E8);

INCLUDE_ASM(const s32, "core/main", func_00227388);

INCLUDE_ASM(const s32, "core/main", func_002275C0);

INCLUDE_ASM(const s32, "core/main", func_002275F8);

INCLUDE_ASM(const s32, "core/main", func_00227650);

INCLUDE_ASM(const s32, "core/main", func_00227698);

INCLUDE_ASM(const s32, "core/main", func_00227750);

INCLUDE_ASM(const s32, "core/main", func_002277C8);

INCLUDE_ASM(const s32, "core/main", func_00228398);

INCLUDE_ASM(const s32, "core/main", func_002283E0);

INCLUDE_ASM(const s32, "core/main", func_00228498);

INCLUDE_ASM(const s32, "core/main", func_00228510);

INCLUDE_ASM(const s32, "core/main", func_00229A28);

INCLUDE_ASM(const s32, "core/main", func_00229BD8);

INCLUDE_ASM(const s32, "core/main", func_00229CB8);

INCLUDE_ASM(const s32, "core/main", func_0022A7C0);

INCLUDE_ASM(const s32, "core/main", func_0022A8C8);

INCLUDE_ASM(const s32, "core/main", func_0022A920);

INCLUDE_ASM(const s32, "core/main", func_0022A980);

INCLUDE_ASM(const s32, "core/main", func_0022AA90);

INCLUDE_ASM(const s32, "core/main", func_0022AAB8);

INCLUDE_ASM(const s32, "core/main", func_0022AB38);

INCLUDE_ASM(const s32, "core/main", func_0022AB58);

INCLUDE_ASM(const s32, "core/main", func_0022ABB0);

INCLUDE_ASM(const s32, "core/main", func_0022ADC8);

INCLUDE_ASM(const s32, "core/main", func_0022BF98);

INCLUDE_ASM(const s32, "core/main", func_0022C040);

INCLUDE_ASM(const s32, "core/main", func_0022C0C8);

INCLUDE_ASM(const s32, "core/main", func_0022C0D8);

INCLUDE_ASM(const s32, "core/main", func_0022C108);

INCLUDE_ASM(const s32, "core/main", func_0022C130);

INCLUDE_ASM(const s32, "core/main", func_0022C190);

INCLUDE_ASM(const s32, "core/main", func_0022C1D0);

INCLUDE_ASM(const s32, "core/main", func_0022C278);

INCLUDE_ASM(const s32, "core/main", func_0022C2A8);

INCLUDE_ASM(const s32, "core/main", func_0022C3C0);

INCLUDE_ASM(const s32, "core/main", func_0022C508);

INCLUDE_ASM(const s32, "core/main", func_0022C590);

INCLUDE_ASM(const s32, "core/main", func_0022C650);

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

INCLUDE_ASM(const s32, "core/main", func_0022D1F0);

INCLUDE_ASM(const s32, "core/main", func_0022D250);

INCLUDE_ASM(const s32, "core/main", func_0022D298);

INCLUDE_ASM(const s32, "core/main", func_0022D2D0);

INCLUDE_ASM(const s32, "core/main", func_0022E1D0);

INCLUDE_ASM(const s32, "core/main", func_0022E200);

INCLUDE_ASM(const s32, "core/main", func_0022E228);

INCLUDE_ASM(const s32, "core/main", func_0022E448);

INCLUDE_ASM(const s32, "core/main", func_0022E480);

INCLUDE_ASM(const s32, "core/main", func_0022E570);

INCLUDE_ASM(const s32, "core/main", func_0022E6F8);

INCLUDE_ASM(const s32, "core/main", func_0022E7B8);
