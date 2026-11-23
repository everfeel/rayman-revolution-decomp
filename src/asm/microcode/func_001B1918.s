.align 3
/* Handwritten VU0 microcode: 4x4 matrix multiply in macro mode.  Loads two
   4x4 matrices with lqc2, multiplies with vmulax/vmadday/vmaddz, and stores
   the result with sd/sw.  No C form. */
glabel func_001B1918
    /* B1918 001B1918 0000A1D8 */  lqc2       $vf1, 0x0($5)
    /* B191C 001B191C 1000A2D8 */  lqc2       $vf2, 0x10($5)
    /* B1920 001B1920 2000A3D8 */  lqc2       $vf3, 0x20($5)
    /* B1924 001B1924 3000A4D8 */  lqc2       $vf4, 0x30($5)
    /* B1928 001B1928 0000C5D8 */  lqc2       $vf5, 0x0($6)
    /* B192C 001B192C 1000C6D8 */  lqc2       $vf6, 0x10($6)
    /* B1930 001B1930 2000C7D8 */  lqc2       $vf7, 0x20($6)
    /* B1934 001B1934 3000C8D8 */  lqc2       $vf8, 0x30($6)
    /* B1938 001B1938 BC29C14B */  vmulax.xyz ACC, $vf5, $vf1x
    /* B193C 001B193C BD30C14B */  vmadday.xyz ACC, $vf6, $vf1y
    /* B1940 001B1940 4A3AC14B */  vmaddz.xyz $vf9, $vf7, $vf1z
    /* B1944 001B1944 BC29C24B */  vmulax.xyz ACC, $vf5, $vf2x
    /* B1948 001B1948 BD30C24B */  vmadday.xyz ACC, $vf6, $vf2y
    /* B194C 001B194C 8A3AC24B */  vmaddz.xyz $vf10, $vf7, $vf2z
    /* B1950 001B1950 BC29C34B */  vmulax.xyz ACC, $vf5, $vf3x
    /* B1954 001B1954 BD30C34B */  vmadday.xyz ACC, $vf6, $vf3y
    /* B1958 001B1958 CA3AC34B */  vmaddz.xyz $vf11, $vf7, $vf3z
    /* B195C 001B195C BC29C44B */  vmulax.xyz ACC, $vf5, $vf4x
    /* B1960 001B1960 BD30C44B */  vmadday.xyz ACC, $vf6, $vf4y
    /* B1964 001B1964 BE38C44B */  vmaddaz.xyz ACC, $vf7, $vf4z
    /* B1968 001B1968 0B43C04B */  vmaddw.xyz $vf12, $vf8, $vf0w
    /* B196C 001B196C 00482248 */  qmfc2.ni   $2, $vf9
    /* B1970 001B1970 00502348 */  qmfc2.ni   $3, $vf10
    /* B1974 001B1974 00583848 */  qmfc2.ni   $24, $vf11
    /* B1978 001B1978 00603948 */  qmfc2.ni   $25, $vf12
    /* B197C 001B197C 000082FC */  sd         $2, 0x0($4)
    /* B1980 001B1980 100083FC */  sd         $3, 0x10($4)
    /* B1984 001B1984 200098FC */  sd         $24, 0x20($4)
    /* B1988 001B1988 300099FC */  sd         $25, 0x30($4)
    /* B198C 001B198C A9134270 */  pcpyud     $2, $2, $2
    /* B1990 001B1990 A91B6370 */  pcpyud     $3, $3, $3
    /* B1994 001B1994 A9C31873 */  pcpyud     $24, $24, $24
    /* B1998 001B1998 A9CB3973 */  pcpyud     $25, $25, $25
    /* B199C 001B199C 080082AC */  sw         $2, 0x8($4)
    /* B19A0 001B19A0 180083AC */  sw         $3, 0x18($4)
    /* B19A4 001B19A4 280098AC */  sw         $24, 0x28($4)
    /* B19A8 001B19A8 380099AC */  sw         $25, 0x38($4)
    /* B19AC 001B19AC 0800E003 */  jr         $31
    /* B19B0 001B19B0 00000000 */   nop
endlabel func_001B1918
