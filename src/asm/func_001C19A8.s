.align 3

glabel func_001C19A8
    /* C19A8 001C19A8 F0FFBD27 */  addiu      $29, $29, -0x10
    /* C19AC 001C19AC 0000BFFF */  sd         $31, 0x0($29)
    /* C19B0 001C19B0 8402070C */  jal        func_001C0A10
    /* C19B4 001C19B4 00000000 */   nop
    /* C19B8 001C19B8 0F000000 */  sync
    /* C19BC 001C19BC 0000BFDF */  ld         $31, 0x0($29)
    /* C19C0 001C19C0 0800E003 */  jr         $31
    /* C19C4 001C19C4 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C19A8
