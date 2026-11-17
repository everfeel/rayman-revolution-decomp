#include "common.h"
extern s32 D_00330ED0;
extern s32 func_00135C10(s32, s32);

/* func_00135CF8 - literal behavioral translation */
extern u8  D_00330ED4[];
extern u8  D_00330F3C[];
extern u8  D_00330FA4[];
extern u8  D_0033100C[];
extern u8  D_00331074[];
extern u8  D_003310DC[];
extern s32 func_00135C10(s32, s32);

typedef struct { u64 v; } __attribute__((packed)) CPW;
#define CP8(d,s)  (((CPW*)(d))[0].v=((CPW*)(s))[0].v)
#define CP20(d,s) do { s32 _k; for(_k=0;_k<4;_k++) ((CPW*)(d))[_k].v=((CPW*)(s))[_k].v; } while(0)

s32 func_00135CF8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 r2,r2b,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16,r17,r18,r19,r20,r21,r22,r23,r24,r25,r30;
    f32 f0,f1,f2,f3,f4,f5,f6;
    s32 sp00,sp04,sp08,sp10,sp14,sp18,sp20,sp30;
    u64 lo;
    s32 *D = (s32 *)0x00330ED0;

    r16=a0; r6=a2; sp10=a1; sp30=a3; sp14=a4;
    f0=1.0f; sp08=*(s32*)&f0; f6=2.0f;
    r12=r20=r19=r18=r17=r4=r25=r24=r15=r13=r14=r9=r21=0;
    sp18=0; sp20=0; sp00=0; sp04=0;
    if (r6<=0) goto L36870;
    r8=(s32)0x00330FA4; r21=(s32)0x003310DC; r3=sp30;

L35D98:
    r2=0x68; lo=(u32)r3;
    r5=(s32)((u64)((u32)r9*(u32)r2)+lo);
    r2=(s16)*(u16*)((u8*)r5+0x2E); r2^=0x63;
    if (r2==0) goto L36858;
    r3=*(u16*)((u8*)r5+0x2E);
    r7=sp14; r2=(r7<r9)?1:0;
    f0=*(f32*)((u8*)r5+0x0); /* beqz delay slot: always executes */
    if (r2==0) goto L35DD4;
    if (!(f6==f0)) { r30=1; goto L3684C; } /* bc1fl: delay only if taken */
L35DD4:
    r2=r3-7; r22=sp20; f6=f0; r10=((u32)r2<2);
    r11=r5+4; /* bnez $22 delay slot at 35DE4: always executes */
    if (r22!=0) goto L35EC4;
    r7=(s32)0x00330ED4; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L35E68;
    r9+=1; r2=r5+0x60;
L35E0C: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L35E0C;
    r30=1; /* delay: always executes */

    goto L35EA8;
L35E68: r9+=1; r2=r5+0x60;
L35E70: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L35E70;
    r30=1;
L35EA8:
    r12=r10; sp20=r30; CP8((u8*)r7,(u8*)r3);
    goto L35FA0;
L35EC4:
    if (r12==0) { r9+=1; goto L35FA0; } /* beql: delay only if taken */
    if (r10!=0) { r9+=1; goto L35FA0; } /* bnel: delay only if taken */
    r7=(s32)0x00330ED4; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L35F50;
    r9+=1; r2=r5+0x60;
L35EF4: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L35EF4;
    goto L35F8C;
L35F50: r9+=1; r2=r5+0x60;
L35F58: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L35F58;
L35F8C: CP8((u8*)r7,(u8*)r3); r12=0;
L35FA0:
    r3=*(u32*)((u8*)r5+0x28); r2=r3&0x2;
    if (r2!=0) goto L35FBC;
    r3&=0x9; r2=9;
    if (r3!=r2) { r2=*(u32*)((u8*)r5+0x20); goto L36164; } /* bnel */
L35FBC:
    if (r4!=0) goto L3608C;
    r7=(s32)0x0033100C; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L36038;
    r2=r5+0x60;
L35FDC: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L35FDC;
    goto L36070;
L36038: r2=r5+0x60;
L3603C: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L3603C;
L36070: CP8((u8*)r7,(u8*)r3); r13=r10; r4=1; goto L36160;
L3608C:
    if (r13==0) { r2=*(u32*)((u8*)r5+0x20); goto L36164; } /* beql */
    if (r10!=0) { r2=*(u32*)((u8*)r5+0x20); goto L36164; } /* bnel */
    r7=(s32)0x0033100C; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L36114;
    r2=r5+0x60;
L360B8: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L360B8;
    goto L3614C;
L36114: r2=r5+0x60;
L36118: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36118;
L3614C: CP8((u8*)r7,(u8*)r3); r13=0;
L36160: r2=*(u32*)((u8*)r5+0x20);
L36164:
    r7=*(u16*)((u8*)r2+0x06); r3=r7&0x4000;
    if (r3!=0) {
        r3=*(u32*)((u8*)r5+0x1C); r2=*(u16*)((u8*)r3+0x06); r2&=0x4000;
        if (r2!=0) goto L36198;
    }
    r2=r7&0x8000; /* 36170/36184 delay: always */
    if (r2==0) goto L36340;
    r2=r7&0x1F88; /* 3618C delay: always */
    if (r2==0) goto L36344; /* 36190 beqz */
    f0=*(f32*)&sp08; /* 36194 delay: always */
L36198:
    if (r18!=0) goto L36268; /* bnez: delay always */
    r7=(s32)0x00331074; /* delay */
    r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L36214;
    r2=r5+0x60;
L361B8: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L361B8;
    r14=r10; /* delay: always executes */

    goto L36250;
L36214: r2=r5+0x60;
L36218: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36218;
    r14=r10;
L36250:
    r18=1; CP8((u8*)r7,(u8*)r3); goto L3685C;
L36268:
    if (r14==0) goto L36860; /* beqz: delay always */
    r2=(r9<r6)?1:0; /* delay */
    if (r10!=0) goto L36860;
    r7=(s32)0x00331074; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L362F0;
    r2=r5+0x60;
L36294: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36294;
    goto L36328;
L362F0: r2=r5+0x60;
L362F4: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L362F4;
L36328: CP8((u8*)r7,(u8*)r3); r14=0; goto L3685C;

L36340:
    f0=*(f32*)&sp08;
L36344:
    f5=*(f32*)((u8*)r11+0x08); f4=*(f32*)((u8*)r11+0x00);
    f3=*(f32*)&sp00; f2=*(f32*)((u8*)r11+0x04); f1=*(f32*)&sp04;
    /* F_DOT3(f4,f3,f2,f1,f5,f0) = f4*f3+f2*f1+f5*f0 */
    { float a; __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6":"=&f"(a):"f"(f4),"f"(f3),"f"(f2),"f"(f1),"f"(f5),"f"(f0)); f4=a; }
    f0=*(f32*)((u8*)r16+0x18);
    if (!(f0<f4)) goto L36518; /* bc1f */
    /* threshold < dot -> D_00330F3C */
    if (r19!=0) goto L36444;
    r7=(s32)0x00330F3C; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L363F0;
    r2=r5+0x60;
L36394: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36394;
    goto L36428;
L363F0: r2=r5+0x60;
L363F4: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L363F4;
L36428: CP8((u8*)r7,(u8*)r3); r15=r10; r19=1; goto L3685C;
L36444:
    if (r15==0) goto L36860;
    r2=(r9<r6)?1:0;
    if (r10!=0) goto L36860;
    r7=(s32)0x00330F3C; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L364C8;
    r2=r5+0x60;
L3646C: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L3646C;
    goto L36500;
L364C8: r2=r5+0x60;
L364CC: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L364CC;
L36500: CP8((u8*)r7,(u8*)r3); r15=0; goto L3685C;
L36518:
    f0=-f0; /* neg.s */
    if (!(f0<f4)) goto L366B4; /* bc1f */
    if (r20!=0) goto L365E4;
    r7=r8; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L365A0;
    r2=r5+0x60;
L36544: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36544;
    r24=r10; /* delay: always executes */

    goto L365DC;
L365A0: r2=r5+0x60;
L365A4: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L365A4;
    r24=r10;
L365DC: r20=1; /* delay: always executes */
 goto L3676C;
L365E4:
    if (r24==0) goto L36860;
    r2=(r9<r6)?1:0;
    if (r10!=0) goto L36860;
    r7=r8; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L36664;
    r2=r5+0x60;
L36608: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36608;
    goto L3669C;
L36664: r2=r5+0x60;
L36668: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36668;
L3669C: CP8((u8*)r7,(u8*)r3); r24=0; goto L3685C;
L366B4:
    if (r17!=0) goto L36780;
    r7=r21; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L3672C;
    r2=r5+0x60;
L366D0: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L366D0;
    r25=r10; /* delay: always executes */

    goto L36768;
L3672C: r2=r5+0x60;
L36730: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36730;
    r25=r10;
L36768: r17=1; /* delay: always executes */
 goto L3685C;
L3676C: CP8((u8*)r7,(u8*)r3); goto L3685C;
L36780:
    if (r25==0) goto L36860;
    r2=(r9<r6)?1:0;
    if (r10!=0) goto L36860;
    r7=r21; r2=(r5|r7)&0x7;
    r3=r5; /* beqz delay slot: always executes */
    if (r2==0) goto L36800;
    r2=r5+0x60;
L367A4: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L367A4;
    goto L36834;
L36800: r2=r5+0x60;
L36804: CP20((u8*)r7,(u8*)r3); r3+=0x20; r7+=0x20; if(r3!=r2) goto L36804;
L36834: CP8((u8*)r7,(u8*)r3); r25=0; goto L3685C;

L3684C: r9+=1; sp18=r30; goto L3685C;
L36858: r9+=1; goto L3685C;
L3685C:
    r2=(r9<r6)?1:0;
    if (r2==0) goto L36870;
    r2=sp18;
    r3=sp30; /* delay slot of beqz at 36868: always executes */
    if (r2==0) goto L35D98;
    goto L36870; /* sp18 set -> exit loop */
L36860:
    goto L3685C; /* terminal check failed -> continue loop */

L36870:
    if (r4!=0) {
        D[0] |= 0x10000;
        func_00135C10(sp10, (s32)((u8*)D + 0x13C));
    }
L36898:
    if (r18!=0) D[0] |= 0x40;
L368AC:
    if (r19!=0) {
        D[0] |= 0x1;
        if ((*(u32*)((u8*)r16+0xC) & 0x2)!=0) {
            CP8((u8*)r16+0x1B8, (u8*)D+0x70);
            *(u32*)((u8*)r16+0x1C0) = *(u32*)((u8*)D+0x78);
        }
    }
L368EC:
    if (r20!=0) {
        D[0] |= 0x4;
        if ((*(u32*)((u8*)r16+0xC) & 0x2)!=0) {
            CP8((u8*)r16+0x1C4, (u8*)D+0xD8);
            *(u32*)((u8*)r16+0x1CC) = *(u32*)((u8*)D+0xE0);
        }
    }
L3692C:
    if (r17!=0) D[0] |= 0x10;
    return 0;
}
