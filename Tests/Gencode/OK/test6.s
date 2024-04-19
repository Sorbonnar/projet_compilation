
.data

count: .word 5
.asciiz "Count: "
.asciiz "\n"
.asciiz "Count: "
.asciiz "\n"

.text

main:
    addiu $29, $29, 0
_L1:
    lui   $8, 0x1001
    lw    $8, 0($8)
    ori   $9, $0, 0x1
    slt   $8, $9, $8
    beq   $8, $0, _L2
    lui   $4, 0x1001
    ori   $4, $4, 0x4
    ori   $2, $0, 0x4
    syscall
    lui   $4, 0x1001
    lw    $4, 0($4)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xc
    ori   $2, $0, 0x4
    syscall
    lui   $8, 0x1001
    lw    $8, 0($8)
    ori   $9, $0, 0x1
    subu  $8, $8, $9
    lui   $9, 0x1001
    sw    $8, 0($9)
    j     _L1
_L2:
    lui   $8, 0x1001
    lw    $8, 0($8)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    lui   $9, 0x1001
    sw    $8, 0($9)
    lui   $4, 0x1001
    ori   $4, $4, 0xe
    ori   $2, $0, 0x4
    syscall
    lui   $4, 0x1001
    lw    $4, 0($4)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x16
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 0
    ori   $2, $0, 0xa
    syscall
