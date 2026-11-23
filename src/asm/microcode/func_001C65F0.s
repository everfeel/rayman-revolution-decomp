.align 3
/* Handwritten VU0 microcode: write the microprogram address to COP2 control
   register vi31.  ctc2 has no C form. */
glabel func_001C65F0
    /* C65F0 001C65F0 FFFF8430 */  andi       $4, $4, 0xFFFF
    /* C65F4 001C65F4 00F8C448 */  ctc2.ni    $4, $vi31
    /* C65F8 001C65F8 0800E003 */  jr         $31
    /* C65FC 001C65FC 00000000 */   nop
endlabel func_001C65F0
