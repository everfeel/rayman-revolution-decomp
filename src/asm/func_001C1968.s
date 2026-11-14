.align 3

glabel func_001C1968
    /* C1968 001C1968 F0FFBD27 */  addiu      $29, $29, -0x10
    /* C196C 001C196C 0000BFFF */  sd         $31, 0x0($29)
    /* C1970 001C1970 7C02070C */  jal        func_001C09F0
    /* C1974 001C1974 00000000 */   nop
    /* C1978 001C1978 0F000000 */  sync
    /* C197C 001C197C 0000BFDF */  ld         $31, 0x0($29)
    /* C1980 001C1980 0800E003 */  jr         $31
    /* C1984 001C1984 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C1968
