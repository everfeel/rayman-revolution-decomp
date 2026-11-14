.align 3

glabel func_001E6E78
    /* E6E78 001E6E78 FF008430 */  andi       $4, $4, 0xFF
    /* E6E7C 001E6E7C 0C000324 */  addiu      $3, $0, 0xC
    /* E6E80 001E6E80 18208300 */  mult       $4, $4, $3
    /* E6E84 001E6E84 3800023C */  lui        $2, %hi(D_00383780)
    /* E6E88 001E6E88 80374224 */  addiu      $2, $2, %lo(D_00383780)
    /* E6E8C 001E6E8C 21208200 */  addu       $4, $4, $2
    /* E6E90 001E6E90 0800E003 */  jr         $31
    /* E6E94 001E6E94 00008290 */   lbu       $2, 0x0($4)
endlabel func_001E6E78
