.align 3

glabel func_001E6E48
    /* E6E48 001E6E48 0C000224 */  addiu      $2, $0, 0xC
    /* E6E4C 001E6E4C FFFF8430 */  andi       $4, $4, 0xFFFF
    /* E6E50 001E6E50 18208200 */  mult       $4, $4, $2
    /* E6E54 001E6E54 3800033C */  lui        $3, %hi(D_00382818)
    /* E6E58 001E6E58 18286324 */  addiu      $3, $3, %lo(D_00382818)
    /* E6E5C 001E6E5C 21186400 */  addu       $3, $3, $4
    /* E6E60 001E6E60 0800E003 */  jr         $31
    /* E6E64 001E6E64 0800628C */   lw        $2, 0x8($3)
endlabel func_001E6E48
