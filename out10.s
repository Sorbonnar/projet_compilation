
.data

.asciiz "True"
.asciiz "False"
.asciiz "a == b"
.asciiz "a != b"
.asciiz "a < b"
.asciiz "a > b"
.asciiz "a <= b"
.asciiz "a >= b"

.text

main:
    addiu $29, $29, -12
    ori   $8, $0, 0x64
    sw    $8, 0($29)
    ori   $8, $0, 0xc8
    sw    $8, 4($29)
    ori   $8, $0, 0x1
    sw    $8, 8($29)
    lw    $8, 8($29)
    beq   $8, $0, _L1
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    j     _L2
_L1:
    lui   $4, 0x1001
    ori   $4, $4, 0x5
    ori   $2, $0, 0x4
    syscall
_L2:
    lw    $8, 0($29)
    lw    $9, 4($29)
    xor   $8, $8, $9
    sltiu $8, $8, 1
    beq   $8, $0, _L3
    lui   $4, 0x1001
    ori   $4, $4, 0xb
    ori   $2, $0, 0x4
    syscall
    j     _L4
_L3:
    lw    $8, 0($29)
    lw    $9, 4($29)
    xor   $8, $8, $9
    sltu  $8, $0, $8
    beq   $8, $0, _L5
    lui   $4, 0x1001
    ori   $4, $4, 0x12
    ori   $2, $0, 0x4
    syscall
_L5:
_L4:
    lw    $8, 0($29)
    lw    $9, 4($29)
    slt   $8, $8, $9
    beq   $8, $0, _L6
    lui   $4, 0x1001
    ori   $4, $4, 0x19
    ori   $2, $0, 0x4
    syscall
    j     _L7
_L6:
    lw    $8, 0($29)
    lw    $9, 4($29)
    slt   $8, $9, $8
    beq   $8, $0, _L8
    lui   $4, 0x1001
    ori   $4, $4, 0x1f
    ori   $2, $0, 0x4
    syscall
_L8:
_L7:
    lw    $8, 0($29)
    lw    $9, 4($29)
    slt   $8, $9, $8
    xori  $8, $8, 0x1
    beq   $8, $0, _L9
    lui   $4, 0x1001
    ori   $4, $4, 0x25
    ori   $2, $0, 0x4
    syscall
    j     _L10
_L9:
    lw    $8, 0($29)
    lw    $9, 4($29)
    slt   $8, $8, $9
    xori  $8, $8, 0x1
    beq   $8, $0, _L11
    lui   $4, 0x1001
    ori   $4, $4, 0x2c
    ori   $2, $0, 0x4
    syscall
_L11:
_L10:
    addiu $29, $29, 12
    ori   $2, $0, 0xa
    syscall
