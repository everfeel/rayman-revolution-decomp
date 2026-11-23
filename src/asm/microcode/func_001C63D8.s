.align 3
/* Handwritten VU0 vector store: qmtc2 + vsqi.xyzw with vi1 as the destination index. */

glabel func_001C63D8
    /* C63D8 001C63D8 0800A2DC */  ld         $2, 0x8($5)
    /* C63DC 001C63DC F0FFBD27 */  addiu      $29, $29, -0x10
    /* C63E0 001C63E0 0000A3DC */  ld         $3, 0x0($5)
    /* C63E4 001C63E4 FFFF8430 */  andi       $4, $4, 0xFFFF
    /* C63E8 001C63E8 0800A2FF */  sd         $2, 0x8($29)
    /* C63EC 001C63EC 00084648 */  cfc2.ni    $6, $vi1
    /* C63F0 001C63F0 0000A3FF */  sd         $3, 0x0($29)
    /* C63F4 001C63F4 00082548 */  qmfc2.ni   $5, $vf1
    /* C63F8 001C63F8 0000A27B */  lq         $2, 0x0($29)
    /* C63FC 001C63FC 0008A248 */  qmtc2.ni   $2, $vf1
    /* C6400 001C6400 0008C448 */  ctc2.ni    $4, $vi1
    /* C6404 001C6404 7D0BE14B */  vsqi.xyzw  $vf1, ($vi1++)
    /* C6408 001C6408 0008C648 */  ctc2.ni    $6, $vi1
    /* C640C 001C640C 0008A548 */  qmtc2.ni   $5, $vf1
    /* C6410 001C6410 0800E003 */  jr         $31
    /* C6414 001C6414 1000BD27 */   addiu     $29, $29, 0x10
endlabel func_001C63D8
