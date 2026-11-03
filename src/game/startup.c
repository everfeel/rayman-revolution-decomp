#include "common.h"

/* func_00100008 through func_00102298; retail address range 0x00100000-0x001022E8. */

/* Entry and syscall wrappers. */

INCLUDE_ASM(const s32, "core/main", func_00100008);

void func_001000B0(void) // Exit syscall wrapper of the wrapper
{
    func_001C0840(0);
}

/* The syscall exits the thread; this compiler otherwise appends a return. */
#ifndef __INTELLISENSE__
__asm__(
    "glabel func_001000B8\n"
    "addiu $3, $0, 0x23\n"
    "syscall\n"
    "endlabel func_001000B8\n"
);
#else
void func_001000B8(void);
#endif

void func_001000C0(void) {
    func_001415E8();
}

void func_001000C8(void) {
    func_00102548();
}

void func_001000D0(void) {
}

void func_001000D8(void) {
    func_00100460();
}

void func_001000E0(void) {
    func_00100728();
}

/* Main-loop and lifecycle helpers. */

extern u8 D_00252230[9];
void func_001007F8(void);
extern void func_00100CD0(void);

s32 func_001000E8(void)
{
    return func_00100360();
}

s32 func_00100108(void) { return func_00100378(); }

void func_00100128(void) { func_001007F8(); }

extern int D_00392F00;

void func_00100130(void)
{
    while (func_001000E8() == 0)
    {
        func_001000D8();

        D_00392F00 = 0;

        while (func_00100108() == 0)
        {
            func_00100128();
        }

        func_001000E0();
    }
}

INCLUDE_ASM(const s32, "core/main", func_00100188);

INCLUDE_ASM(const s32, "core/main", func_001001C0);

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

INCLUDE_ASM(const s32, "core/main", func_001002A0);

s32 func_00100360(void)
{
    return D_00252230[0] == 2;
}

/* Process-state predicates. */

s32 func_00100378(void)
{
    u8 state = D_00252230[0];
    return state == 2 || state == 6 || state == 8;
}

/* Startup routines. */

INCLUDE_ASM(const s32, "core/main", func_001003B0);

INCLUDE_ASM(const s32, "core/main", func_00100460);

INCLUDE_ASM(const s32, "core/main", func_00100728);

void func_001007F8(void)
{
    register s32 state __asm__("$3");
    register s32 active_state __asm__("$4");

    if (func_00100360() == 0) {
        func_00100CD0();
        active_state = 2;
        state = D_00252230[0];
        if (state != active_state) {
            func_001010F8();
            func_001028C8();
        }
    }
}

INCLUDE_ASM(const s32, "core/main", func_00100848);

INCLUDE_ASM(const s32, "core/main", func_00100A38);

INCLUDE_ASM(const s32, "core/main", func_00100BD0);

INCLUDE_ASM(const s32, "core/main", func_00100CD0);

extern s32 D_00392F24;
extern s32 func_0010CA50(void *);
extern void func_0014AD90(void *, s32, s32);

void func_00100F58(void *arg)
{
    s32 result = func_0010CA50(arg);
    func_0014AD90(arg, D_00392F24, result);
}

INCLUDE_ASM(const s32, "core/main", func_00100F88);

INCLUDE_ASM(const s32, "core/main", func_00101068);

INCLUDE_ASM(const s32, "core/main", func_001010F8);

typedef struct StartupCountState {
    u8 unk00[0x34];
    u8 flags;
} StartupCountState;

typedef struct StartupCountComponent {
    u32 unk00;
    StartupCountState *state;
} StartupCountComponent;

typedef struct StartupCountObject {
    u32 unk00;
    StartupCountComponent *component;
} StartupCountObject;

typedef struct StartupCountNode {
    StartupCountObject *object;
    struct StartupCountNode *next;
} StartupCountNode;

s32 func_00101260(void *entity)
{
    StartupCountNode *node =
        *(StartupCountNode **)(*(void **)((u8 *)entity + 4));
    s32 total = 0;

    while (node != 0) {
        StartupCountState *state = node->object->component->state;
        total += (state->flags >> 1) & 1;
        node = node->next;
    }

    return total;
}

typedef struct StartupItemList {
    u8 unk00[0x30];
    void **items;
    s32 count;
} StartupItemList;

typedef struct StartupNode {
    u8 unk00[0x20];
    struct StartupEntity **entity;
} StartupNode;

typedef struct StartupEntity {
    u8 unk00[4];
    StartupItemList *items;
} StartupEntity;

typedef struct StartupManager {
    u8 unk00[4];
    StartupNode *node;
} StartupManager;

extern u8 D_00252398[];

s32 func_001012A0(void)
{
    StartupManager *manager;
    StartupEntity *entity;
    StartupItemList *items;
    s32 total;
    s32 count;
    void **item;

    manager = *(StartupManager **)D_00252398;
    entity = *manager->node->entity;
    total = func_00101260(entity);
    items = entity->items;
    count = items->count;
    item = items->items;
    while (count > 0) {
        total += func_00101260(*item);
        count--;
        item++;
    }

    return total;
}

INCLUDE_ASM(const s32, "core/main", func_00101320);

INCLUDE_ASM(const s32, "core/main", func_00101380);

INCLUDE_ASM(const s32, "core/main", func_00101438);

INCLUDE_ASM(const s32, "core/main", func_001015B8);

INCLUDE_ASM(const s32, "core/main", func_001016B0);

INCLUDE_ASM(const s32, "core/main", func_00101828);

void func_001018B8(void) {
}

typedef struct StartupDevice {
    u8 unk00[0x148];
    void *object;
} StartupDevice;

typedef struct StartupContainer {
    u8 unk00[0x1C];
    StartupDevice *device;
} StartupContainer;

typedef struct StartupContext {
    u8 unk00[8];
    StartupContainer *container;
} StartupContext;

typedef struct StartupRoot {
    u8 unk00[4];
    StartupContext *context;
} StartupRoot;

typedef struct StartupDeviceObject {
    u32 unk00;
    u32 flags;
    u8 unk08[0x9C];
    s32 value;
} StartupDeviceObject;

void func_001018C0(StartupRoot *root, s32 value)
{
    StartupContainer *container = root->context->container;
    StartupDevice *device;
    StartupDeviceObject *object;

    if (container == 0) {
        return;
    }
    device = container->device;
    object = (StartupDeviceObject *)device->object;
    if (object == 0) {
        return;
    }
    object->value = value;
    if (value == 0) {
        object->flags &= 0xFFFEFFFF;
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

s32 func_001019A0(void *a0)
{
    void *v1 = *(void **)((u8 *)a0 + 4);
    void *v2 = *(void **)(v1);
    void *v3 = *(void **)((u8 *)v2 + 4);
    if (v3 == 0)
        return 0;
    return *(s32 *)((u8 *)v3 + 0x18);
}

INCLUDE_ASM(const s32, "core/main", func_001019C8);

INCLUDE_ASM(const s32, "core/main", func_00101A78);

INCLUDE_ASM(const s32, "core/main", func_00101B20);

INCLUDE_ASM(const s32, "core/main", func_00101BF0);

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

INCLUDE_ASM(const s32, "core/main", func_00101CF0);

INCLUDE_ASM(const s32, "core/main", func_00101D90);

INCLUDE_ASM(const s32, "core/main", func_00101E30);

extern void func_00112120(void);
extern void func_001D4568(void);

void func_00102138(void)
{
    func_00101068();
    func_00112120();
    func_001D4568();
}

INCLUDE_ASM(const s32, "core/main", func_00102160);

typedef struct StartupTreeNode {
    u32 unk00[2];
    struct StartupTreeNode *child;
    u32 unk0C[2];
    struct StartupTreeNode *next;
    u32 unk18[5];
    u32 flags;
} StartupTreeNode;

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
