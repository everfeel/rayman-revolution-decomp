.align 3

glabel func_001C1948
    /* C1948 001C1948 F0FFBD27 */  addiu      $29, $29, -0x10
    /* C194C 001C194C 0000BFFF */  sd         $31, 0x0($29)
    /* C1950 001C1950 7802070C */  jal        func_001C09E0
    /* C1954 001C1954 00000000 */   nop
    /* C1958 001C1958 0F000000 */  sync
    /* C195C 001C195C 0000BFDF */  ld         $31, 0x0($29)
    /* C1960 001C1960 0800E003 */  jr         $31
    /* C1964 001C1964 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C1948
