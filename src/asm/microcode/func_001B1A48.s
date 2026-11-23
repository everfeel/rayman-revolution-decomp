.align 3
/* Handwritten function */
/* Handwritten VU0 matrix-multiply microcode (lqc2/qmtc2/vmadd*) - companion to func_001B1918 and func_001B19B8. */

glabel func_001B1A48
    /* B1A48 001B1A48 0000E1D8 */  lqc2       $vf1, 0x0($7)
    /* B1A4C 001B1A4C 1000E2D8 */  lqc2       $vf2, 0x10($7)
    /* B1A50 001B1A50 2000E3D8 */  lqc2       $vf3, 0x20($7)
    /* B1A54 001B1A54 0000B89C */  lwu        $24, 0x0($5)
    /* B1A58 001B1A58 0400AE9C */  lwu        $14, 0x4($5)
    /* B1A5C 001B1A5C 0800AF9C */  lwu        $15, 0x8($5)
    /* B1A60 001B1A60 3C700E00 */  dsll32     $14, $14, 0
    /* B1A64 001B1A64 2570D801 */  or         $14, $14, $24
    /* B1A68 001B1A68 897BEE71 */  pcpyld     $15, $15, $14
    /* B1A6C 001B1A6C 0028AF48 */  qmtc2.ni   $15, $vf5
.align 2
  .L001B1A70:
    /* B1A70 001B1A70 1400C018 */  blez       $6, .L001B1AC4
    /* B1A74 001B1A74 0C00A520 */   addi      $5, $5, 0xC /* handwritten instruction */
    /* B1A78 001B1A78 BC09C54B */  vmulax.xyz ACC, $vf1, $vf5x
    /* B1A7C 001B1A7C BD10C54B */  vmadday.xyz ACC, $vf2, $vf5y
    /* B1A80 001B1A80 8A19C54B */  vmaddz.xyz $vf6, $vf3, $vf5z
    /* B1A84 001B1A84 0000B89C */  lwu        $24, 0x0($5)
    /* B1A88 001B1A88 0400AE9C */  lwu        $14, 0x4($5)
    /* B1A8C 001B1A8C 0800AF9C */  lwu        $15, 0x8($5)
    /* B1A90 001B1A90 FFFFC620 */  addi       $6, $6, -0x1 /* handwritten instruction */
    /* B1A94 001B1A94 3C700E00 */  dsll32     $14, $14, 0
    /* B1A98 001B1A98 2570D801 */  or         $14, $14, $24
    /* B1A9C 001B1A9C 897BEE71 */  pcpyld     $15, $15, $14
    /* B1AA0 001B1AA0 0028AF48 */  qmtc2.ni   $15, $vf5
    /* B1AA4 001B1AA4 00303948 */  qmfc2.ni   $25, $vf6
    /* B1AA8 001B1AA8 3E101900 */  dsrl32     $2, $25, 0
    /* B1AAC 001B1AAC A91B3973 */  pcpyud     $3, $25, $25
    /* B1AB0 001B1AB0 000099AC */  sw         $25, 0x0($4)
    /* B1AB4 001B1AB4 040082AC */  sw         $2, 0x4($4)
    /* B1AB8 001B1AB8 080083AC */  sw         $3, 0x8($4)
    /* B1ABC 001B1ABC ECFF0010 */  b          .L001B1A70
    /* B1AC0 001B1AC0 0C008420 */   addi      $4, $4, 0xC /* handwritten instruction */
.align 2
  .L001B1AC4:
    /* B1AC4 001B1AC4 0800E003 */  jr         $31
    /* B1AC8 001B1AC8 2D108000 */   daddu     $2, $4, $0
endlabel func_001B1A48
    /* B1ACC 001B1ACC 00000000 */  nop
