
.data

int1: .word 10
.asciiz "Sum:"
.asciiz "true"
.asciiz "false"
.asciiz "a:"
.asciiz "i:"
.asciiz "b:"

.text

main:
    addiu $29, $29, -20
    ori   $8, $0, 0x5
    sw    $8, 0($29)
    lui   $8, 0x1001
    lw    $8, 0($8)
    sw    $8, 4($29)
    ori   $8, $0, 0x1
    sw    $8, 12($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    addu  $8, $8, $9
    sw    $8, 16($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x4
    ori   $2, $0, 0x4
    syscall
    lw    $4, 16($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 12($29)
    beq   $8, $0, _L1
    lui   $4, 0x1001
    ori   $4, $4, 0x9
    ori   $2, $0, 0x4
    syscall
    j     _L2
_L1:
    lui   $4, 0x1001
    ori   $4, $4, 0xe
    ori   $2, $0, 0x4
    syscall
_L2:
_L3:
    lw    $8, 0($29)
    ori   $9, $0, 0x8
    slt   $8, $8, $9
    beq   $8, $0, _L4
    lui   $4, 0x1001
    ori   $4, $4, 0x14
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 0($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 0($29)
    j     _L3
_L4:
    ori   $8, $0, 0x0
    sw    $8, 8($29)
_L5:
    lw    $8, 8($29)
    ori   $9, $0, 0x5
    slt   $8, $8, $9
    beq   $8, $0, _L6
    lui   $4, 0x1001
    ori   $4, $4, 0x17
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 8($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 8($29)
    j     _L5
_L6:
_L7:
    lw    $8, 4($29)
    ori   $9, $0, 0x1
    subu  $8, $8, $9
    sw    $8, 4($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x1a
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 4($29)
    ori   $9, $0, 0x5
    slt   $8, $9, $8
    bne   $8, $0, _L7
    addiu $29, $29, 20
    ori   $2, $0, 0xa
    syscall
