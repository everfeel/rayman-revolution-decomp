
/* Entry-point padding; func_00100008 starts at 0x00100008. */
nop
nop
.align 3
/* CRT entry point: clear .bss, start the main thread, initialize the heap,
   enable interrupts, run main, and exit with its status. */
glabel func_00100008
    /* Clear .bss from D_00393C00 up to _end. */
    lui $2, %hi(D_00393C00)
    lui $3, %hi(D_410DF4)
    addiu $2, $2, %lo(D_00393C00)
    addiu $3, $3, %lo(D_410DF4)
1:
    nop
    nop
    sq $0, 0x0($2)
    sltu $1, $2, $3
    nop
    bnez $1, 1b
    addiu $2, $2, 0x10

    /* SetupThread(gp, _stack, _stack_size, &args, ExitThread). */
    lui $4, %hi(D_0039A3F0)
    lui $5, %hi(0x01FF0000)
    lui $6, %hi(0x00010000)
    lui $7, %hi(D_00394640)
    lui $8, %hi(func_001000B8)
    addiu $4, $4, %lo(D_0039A3F0)
    addiu $5, $5, %lo(0x01FF0000)
    addiu $6, $6, %lo(0x00010000)
    addiu $7, $7, %lo(D_00394640)
    addiu $8, $8, %lo(func_001000B8)
    daddu $28, $4, $0
    addiu $3, $0, 0x3C
    syscall
    daddu $29, $2, $0

    /* SetupHeap(_end, -1). */
    lui $4, %hi(D_410DF4)
    lui $5, 0
    addiu $4, $4, %lo(D_410DF4)
    addiu $5, $5, -0x1
    addiu $3, $0, 0x3D
    syscall

    /* FlushCache(0), then main(args.argc, args.argv). */
    jal func_001C0EA0
    daddu $4, $0, $0
    ei
    lui $2, %hi(D_00394640)
    addiu $2, $2, %lo(D_00394640)
    lw $4, 0x0($2)
    jal func_001002A0
    addiu $5, $2, 0x4

    j func_001C0840
    move $4, $2
endlabel func_00100008
