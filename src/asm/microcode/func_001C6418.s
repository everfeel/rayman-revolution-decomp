.align 3
/* Handwritten VU0 vector load: vlqi.xyzw + qmfc2 with vi1 as the source index. */

glabel func_001C6418
    /* C6418 001C6418 00084648 */  cfc2.ni    $6, $vi1
    /* C641C 001C641C 00082348 */  qmfc2.ni   $3, $vf1
    /* C6420 001C6420 F0FFBD27 */  addiu      $29, $29, -0x10
    /* C6424 001C6424 FFFFA530 */  andi       $5, $5, 0xFFFF
    /* C6428 001C6428 0008C548 */  ctc2.ni    $5, $vi1
    /* C642C 001C642C 7C0BE14B */  vlqi.xyzw  $vf1, ($vi1++)
    /* C6430 001C6430 00082248 */  qmfc2.ni   $2, $vf1
    /* C6434 001C6434 0008C648 */  ctc2.ni    $6, $vi1
    /* C6438 001C6438 0008A348 */  qmtc2.ni   $3, $vf1
    /* C643C 001C643C 0000A27F */  sq         $2, 0x0($29)
    /* C6440 001C6440 2D108000 */  daddu      $2, $4, $0
    /* C6444 001C6444 0000A3DF */  ld         $3, 0x0($29)
    /* C6448 001C6448 0800A5DF */  ld         $5, 0x8($29)
    /* C644C 001C644C 000083FC */  sd         $3, 0x0($4)
    /* C6450 001C6450 080085FC */  sd         $5, 0x8($4)
    /* C6454 001C6454 0800E003 */  jr         $31
    /* C6458 001C6458 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C6418
    /* C645C 001C645C 00000000 */  nop
