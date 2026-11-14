.align 3

glabel func_0015DEB8
    /* 5DEB8 0015DEB8 8047013C */  lui        $1, (0x47800000 >> 16)
    /* 5DEBC 0015DEBC 00008144 */  mtc1       $1, $f0
    /* 5DEC0 0015DEC0 449180AF */  sw $0, D_00393534
    /* 5DEC4 0015DEC4 02630046 */  mul.s      $f12, $f12, $f0
    /* 5DEC8 0015DEC8 24600046 */  .word      0x46006024                    # cvt.w.s    $f0, $f12 # 00000000 <InstrIdType: CPU_COP1_FPUS>
    /* 5DECC 0015DECC 489180E7 */  swc1 $f0, D_00393538
    /* 5DED0 0015DED0 0800E003 */  jr         $31
    /* 5DED4 0015DED4 00000000 */   nop
endlabel func_0015DEB8
