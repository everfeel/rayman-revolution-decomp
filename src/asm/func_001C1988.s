.align 3

glabel func_001C1988
    /* C1988 001C1988 F0FFBD27 */  addiu      $29, $29, -0x10
    /* C198C 001C198C 0000BFFF */  sd         $31, 0x0($29)
    /* C1990 001C1990 8002070C */  jal        func_001C0A00
    /* C1994 001C1994 FFFF8430 */   andi      $4, $4, 0xFFFF
    /* C1998 001C1998 0F000000 */  sync
    /* C199C 001C199C 0000BFDF */  ld         $31, 0x0($29)
    /* C19A0 001C19A0 0800E003 */  jr         $31
    /* C19A4 001C19A4 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C1988
