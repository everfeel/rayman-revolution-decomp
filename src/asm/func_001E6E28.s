.align 3

glabel func_001E6E28
    /* E6E28 001E6E28 0C000224 */  addiu      $2, $0, 0xC
    /* E6E2C 001E6E2C FFFF8430 */  andi       $4, $4, 0xFFFF
    /* E6E30 001E6E30 18208200 */  mult       $4, $4, $2
    /* E6E34 001E6E34 3800033C */  lui        $3, %hi(D_00382818)
    /* E6E38 001E6E38 18286324 */  addiu      $3, $3, %lo(D_00382818)
    /* E6E3C 001E6E3C 21186400 */  addu       $3, $3, $4
    /* E6E40 001E6E40 0800E003 */  jr         $31
    /* E6E44 001E6E44 0400628C */   lw        $2, 0x4($3)
endlabel func_001E6E28
