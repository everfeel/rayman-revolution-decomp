.align 3
/* Handwritten VU0 microcode: transform a list of packed 12-byte vectors
   through a 4x4 matrix held in VU0 registers.  lqc2/qmtc2/vmaddw/qmfc2 and
   the packed EE ops (pcpyld/pcpyud) have no C form. */
glabel func_001B19B8
    /* B19B8 001B19B8 0000E1D8 */  lqc2       $vf1, 0x0($7)
    /* B19BC 001B19BC 1000E2D8 */  lqc2       $vf2, 0x10($7)
    /* B19C0 001B19C0 2000E3D8 */  lqc2       $vf3, 0x20($7)
    /* B19C4 001B19C4 3000E4D8 */  lqc2       $vf4, 0x30($7)
    /* B19C8 001B19C8 0000AD9C */  lwu        $13, 0x0($5)
    /* B19CC 001B19CC 0400AB9C */  lwu        $11, 0x4($5)
    /* B19D0 001B19D0 0800AC9C */  lwu        $12, 0x8($5)
    /* B19D4 001B19D4 3C580B00 */  dsll32     $11, $11, 0
    /* B19D8 001B19D8 25586D01 */  or         $11, $11, $13
    /* B19DC 001B19DC 89638B71 */  pcpyld     $12, $12, $11
    /* B19E0 001B19E0 0028AC48 */  qmtc2.ni   $12, $vf5
.align 2
  .L001B19E4:
    /* B19E4 001B19E4 1500C018 */  blez       $6, .L001B1A3C
    /* B19E8 001B19E8 0C00A520 */   addi      $5, $5, 0xC
    /* B19EC 001B19EC BC09C54B */  vmulax.xyz ACC, $vf1, $vf5x
    /* B19F0 001B19F0 BD10C54B */  vmadday.xyz ACC, $vf2, $vf5y
    /* B19F4 001B19F4 BE18C54B */  vmaddaz.xyz ACC, $vf3, $vf5z
    /* B19F8 001B19F8 8B21C04B */  vmaddw.xyz $vf6, $vf4, $vf0w
    /* B19FC 001B19FC 0000AD9C */  lwu        $13, 0x0($5)
    /* B1A00 001B1A00 0400AB9C */  lwu        $11, 0x4($5)
    /* B1A04 001B1A04 0800AC9C */  lwu        $12, 0x8($5)
    /* B1A08 001B1A08 FFFFC620 */  addi       $6, $6, -0x1
    /* B1A0C 001B1A0C 3C580B00 */  dsll32     $11, $11, 0
    /* B1A10 001B1A10 25586D01 */  or         $11, $11, $13
    /* B1A14 001B1A14 89638B71 */  pcpyld     $12, $12, $11
    /* B1A18 001B1A18 0028AC48 */  qmtc2.ni   $12, $vf5
    /* B1A1C 001B1A1C 00303948 */  qmfc2.ni   $25, $vf6
    /* B1A20 001B1A20 3E101900 */  dsrl32     $2, $25, 0
    /* B1A24 001B1A24 A91B3973 */  pcpyud     $3, $25, $25
    /* B1A28 001B1A28 000099AC */  sw         $25, 0x0($4)
    /* B1A2C 001B1A2C 040082AC */  sw         $2, 0x4($4)
    /* B1A30 001B1A30 080083AC */  sw         $3, 0x8($4)
    /* B1A34 001B1A34 EBFF0010 */  b          .L001B19E4
    /* B1A38 001B1A38 0C008420 */   addi      $4, $4, 0xC
.align 2
  .L001B1A3C:
    /* B1A3C 001B1A3C 0800E003 */  jr         $31
    /* B1A40 001B1A40 2D108000 */   daddu     $2, $4, $0
endlabel func_001B19B8
