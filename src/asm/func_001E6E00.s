.align 3

glabel func_001E6E00
    /* E6E00 001E6E00 14000224 */  addiu      $2, $0, 0x14
    /* E6E04 001E6E04 FFFF8430 */  andi       $4, $4, 0xFFFF
    /* E6E08 001E6E08 18208200 */  mult       $4, $4, $2
    /* E6E0C 001E6E0C 3800023C */  lui        $2, %hi(D_003824E0)
    /* E6E10 001E6E10 E0244224 */  addiu      $2, $2, %lo(D_003824E0)
    /* E6E14 001E6E14 0800E003 */  jr         $31
    /* E6E18 001E6E18 21108200 */   addu      $2, $4, $2
endlabel func_001E6E00
    /* E6E1C 001E6E1C 00000000 */  nop
