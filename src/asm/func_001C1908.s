.align 3

glabel func_001C1908
    /* C1908 001C1908 F0FFBD27 */  addiu      $29, $29, -0x10
    /* C190C 001C190C 0000BFFF */  sd         $31, 0x0($29)
    /* C1910 001C1910 7002070C */  jal        func_001C09C0
    /* C1914 001C1914 00000000 */   nop
    /* C1918 001C1918 0F000000 */  sync
    /* C191C 001C191C 0000BFDF */  ld         $31, 0x0($29)
    /* C1920 001C1920 0800E003 */  jr         $31
    /* C1924 001C1924 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C1908
