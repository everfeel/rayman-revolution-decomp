.align 3
/* Handwritten VU0 microcode: start the microprogram whose address is in
   COP2 control register vi27.  vcallmsr sits in the jr delay slot. */
glabel func_001C6138
    /* C6138 001C6138 FFFF8430 */  andi       $4, $4, 0xFFFF
    /* C613C 001C613C 00D8C448 */  ctc2.ni    $4, $vi27
    /* C6140 001C6140 0800E003 */  jr         $31
    /* C6144 001C6144 39D8004A */   vcallmsr  $vi27
endlabel func_001C6138
