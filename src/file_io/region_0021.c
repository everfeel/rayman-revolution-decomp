#include "common.h"

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

INCLUDE_ASM(const s32, "core/main", func_002133F0);

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

INCLUDE_ASM(const s32, "core/main", func_00213928);

INCLUDE_ASM(const s32, "core/main", func_00213940);

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

INCLUDE_ASM(const s32, "core/main", func_00214EF8);

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

INCLUDE_ASM(const s32, "core/main", func_00215F38);

INCLUDE_ASM(const s32, "core/main", func_00215F50);

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

INCLUDE_ASM(const s32, "core/main", func_0021ECA8);

INCLUDE_ASM(const s32, "core/main", func_0021ECC0);

INCLUDE_ASM(const s32, "core/main", func_0021ED10);

INCLUDE_ASM(const s32, "core/main", func_0021ED40);

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
