#include "common.h"

/* func_00103AD8 through func_0010FC40; retail address range 0x00103AD8-0x00110000. */



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

INCLUDE_ASM(const s32, "core/main", func_00104020);

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

INCLUDE_ASM(const s32, "core/main", func_00104748);

INCLUDE_ASM(const s32, "core/main", func_00104A10);

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

INCLUDE_ASM(const s32, "core/main", func_00106068);

INCLUDE_ASM(const s32, "core/main", func_001060A0);

INCLUDE_ASM(const s32, "core/main", func_00106188);

INCLUDE_ASM(const s32, "core/main", func_00106278);

extern s32 *func_00106278(s32);

s32 func_001062A8(s32 index)
{
    return *func_00106278(index);
}

INCLUDE_ASM(const s32, "core/main", func_001062C8);

INCLUDE_ASM(const s32, "core/main", func_001062F8);

INCLUDE_ASM(const s32, "core/main", func_00106380);

typedef struct RuntimeFlagValue {
    u8 unk00[0x10];
    s32 value;
} RuntimeFlagValue;

typedef struct RuntimeFlagRecord {
    s32 type;
    RuntimeFlagValue *value;
} RuntimeFlagRecord;

s32 func_00106400(RuntimeFlagRecord *record)
{
    if (record->type != 2) {
        return 0;
    }

    return record->value->value != 0;
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

INCLUDE_ASM(const s32, "core/main", func_00107168);

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

INCLUDE_ASM(const s32, "core/main", func_00108F20);

u8 *func_00108F40(u8 *p) { return p + 44; }

INCLUDE_ASM(const s32, "core/main", func_00108F48);

INCLUDE_ASM(const s32, "core/main", func_00108F78);

void func_00108FA8(s32 *p, s32 value) { p[11] = value; }

f32 func_00108FB0(const f32 *p) { return p[7]; }

void func_00108FB8(f32 *p, f32 value) { p[10] = value; }

f32 func_00108FC0(const f32 *p) { return p[10]; }

f32 func_00108FC8(const f32 *p) { return p[8]; }

void func_00108FD0(f32 *p, f32 value) { p[8] = value; }

f32 func_00108FD8(const f32 *p) { return p[9]; }

INCLUDE_ASM(const s32, "core/main", func_00108FE0);

f32 func_00108FF0(const f32 *p) { return p[1]; }

f32 func_00108FF8(const f32 *p) { return p[2]; }

f32 func_00109000(const f32 *p) { return p[3]; }

INCLUDE_ASM(const s32, "core/main", func_00109008);

INCLUDE_ASM(const s32, "core/main", func_00109030);

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

INCLUDE_ASM(const s32, "core/main", func_001094A8);

u8 func_001094B8(const u8 *p) { return p[2]; }

INCLUDE_ASM(const s32, "core/main", func_001094C0);

u8 func_001094D0(const u8 *p) { return p[3]; }

u8 *func_001094D8(u8 *p) { return p + 4; }

INCLUDE_ASM(const s32, "core/main", func_001094E0);

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

INCLUDE_ASM(const s32, "core/main", func_0010A878);

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

INCLUDE_ASM(const s32, "core/main", func_0010AB70);

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

INCLUDE_ASM(const s32, "core/main", func_0010DCC8);

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

INCLUDE_ASM(const s32, "core/main", func_0010EDF8);

INCLUDE_ASM(const s32, "core/main", func_0010EE10);

INCLUDE_ASM(const s32, "core/main", func_0010EED8);

INCLUDE_ASM(const s32, "core/main", func_0010EF58);

INCLUDE_ASM(const s32, "core/main", func_0010EF70);

INCLUDE_ASM(const s32, "core/main", func_0010F0F0);

INCLUDE_ASM(const s32, "core/main", func_0010F2E8);

INCLUDE_ASM(const s32, "core/main", func_0010F318);

INCLUDE_ASM(const s32, "core/main", func_0010F330);

INCLUDE_ASM(const s32, "core/main", func_0010F448);

INCLUDE_ASM(const s32, "core/main", func_0010F6D0);

INCLUDE_ASM(const s32, "core/main", func_0010F8E0);

INCLUDE_ASM(const s32, "core/main", func_0010FAA8);

INCLUDE_ASM(const s32, "core/main", func_0010FC40);
