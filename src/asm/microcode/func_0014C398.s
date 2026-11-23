.align 3
/* Handwritten VU0 microcode: load the microprogram address into COP2
   control register vi27, stall the VU pipeline, then call it with
   vcallmsr.  ctc2/vcallmsr have no C form. */
glabel func_0014C398
    /* 4C398 0014C398 00D8C448 */  ctc2.ni    $4, $vi27
    /* 4C39C 0014C39C FF02004A */  vnop
    /* 4C3A0 0014C3A0 FF02004A */  vnop
    /* 4C3A4 0014C3A4 39D8004A */  vcallmsr   $vi27
    /* 4C3A8 0014C3A8 0800E003 */  jr         $31
    /* 4C3AC 0014C3AC 00000000 */   nop
endlabel func_0014C398
