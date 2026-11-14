.align 3

glabel func_001C1928
    /* C1928 001C1928 F0FFBD27 */  addiu      $29, $29, -0x10
    /* C192C 001C192C 0000BFFF */  sd         $31, 0x0($29)
    /* C1930 001C1930 7402070C */  jal        func_001C09D0
    /* C1934 001C1934 00000000 */   nop
    /* C1938 001C1938 0F000000 */  sync
    /* C193C 001C193C 0000BFDF */  ld         $31, 0x0($29)
    /* C1940 001C1940 0800E003 */  jr         $31
    /* C1944 001C1944 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C1928
