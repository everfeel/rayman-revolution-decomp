.align 3

glabel func_0019ACA8
    /* 9ACA8 0019ACA8 000080C4 */  lwc1       $f0, 0x0($4)
    /* 9ACAC 0019ACAC 040081C4 */  lwc1       $f1, 0x4($4)
    /* 9ACB0 0019ACB0 02000046 */  mul.s      $f0, $f0, $f0
    /* 9ACB4 0019ACB4 42080146 */  mul.s      $f1, $f1, $f1
    /* 9ACB8 0019ACB8 00000146 */  add.s      $f0, $f0, $f1
    /* 9ACBC 0019ACBC 0800E003 */  jr         $31
    /* 9ACC0 0019ACC0 04000046 */   c1        0x4
endlabel func_0019ACA8
    /* 9ACC4 0019ACC4 00000000 */  nop
