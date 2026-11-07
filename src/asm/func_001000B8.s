.include "include/macro.inc"

.align 3
/* ExitThread syscall; control does not return. */
glabel func_001000B8
    addiu $3, $0, 0x23
    syscall
endlabel func_001000B8
