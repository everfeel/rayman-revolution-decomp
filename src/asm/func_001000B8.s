
.align 3
/* ExitThread syscall (0x23), used as the entry-thread's return address by
   func_00100008.  Control never returns, so unlike a C syscall wrapper this
   has no epilogue. */
glabel func_001000B8
    addiu $3, $0, 0x23
    syscall
endlabel func_001000B8
