
.data

intos: .word 10
boolustre: .word 0
.asciiz "Sum: "
.asciiz "Difference: "
.asciiz "Product: "
.asciiz "Quotient: "
.asciiz "Remainder: "
.asciiz "Logical True"
.asciiz "Logical False"
.asciiz "a equals b"
.asciiz "a not equal b"
.asciiz "a is less than b"
.asciiz "a is greater than b"
.asciiz "a is less than or equal to b"
.asciiz "a is greater than or equal to b"
.asciiz "Bitwise AND: "
.asciiz "Bitwise OR: "
.asciiz "Bitwise XOR: "
.asciiz "Not C: "
.asciiz "Negative A: "
.asciiz "Bitwise Not A: "
.asciiz "Shift Left: "
.asciiz "Shift Right: "
.asciiz "a in while: "
.asciiz "i in for: "
.asciiz "b in do-while: "

.text

main:
    addiu $29, $29, -68
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
    lw    $8, 0($29)
    lw    $9, 4($29)
    subu  $8, $8, $9
    sw    $8, 20($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    mult  $8, $9
    mflo  $8
    sw    $8, 24($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    div   $8, $9
    teq $9, $0
    mflo  $8
    sw    $8, 28($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    div   $8, $9
    teq $9, $0
    mfhi  $8
    sw    $8, 32($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    and   $8, $8, $9
    sw    $8, 36($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    or    $8, $8, $9
    sw    $8, 40($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    xor   $8, $8, $9
    sw    $8, 44($29)
    lw    $8, 12($29)
    xori  $8, $8, 0x1
    sw    $8, 48($29)
    lw    $8, 0($29)
    subu  $8, $0, $8
    sw    $8, 52($29)
    lw    $8, 0($29)
    nor   $8, $0, $8
    sw    $8, 56($29)
    lw    $8, 0($29)
    ori   $9, $0, 0x2
    sllv  $8, $8, $9
    sw    $8, 60($29)
    lw    $8, 4($29)
    ori   $9, $0, 0x2
    srav  $8, $8, $9
    sw    $8, 64($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x8
    ori   $2, $0, 0x4
    syscall
    lw    $4, 16($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xe
    ori   $2, $0, 0x4
    syscall
    lw    $4, 20($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x1b
    ori   $2, $0, 0x4
    syscall
    lw    $4, 24($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x25
    ori   $2, $0, 0x4
    syscall
    lw    $4, 28($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x30
    ori   $2, $0, 0x4
    syscall
    lw    $4, 32($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 12($29)
    beq   $8, $0, _L1
    lui   $4, 0x1001
    ori   $4, $4, 0x3c
    ori   $2, $0, 0x4
    syscall
    j     _L2
_L1:
    lui   $4, 0x1001
    ori   $4, $4, 0x49
    ori   $2, $0, 0x4
    syscall
_L2:
    lw    $8, 0($29)
    lw    $9, 4($29)
    xor   $8, $8, $9
    sltiu $8, $8, 1
    beq   $8, $0, _L3
    lui   $4, 0x1001
    ori   $4, $4, 0x57
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
    ori   $4, $4, 0x62
    ori   $2, $0, 0x4
    syscall
_L5:
_L4:
    lw    $8, 0($29)
    lw    $9, 4($29)
    slt   $8, $8, $9
    beq   $8, $0, _L6
    lui   $4, 0x1001
    ori   $4, $4, 0x70
    ori   $2, $0, 0x4
    syscall
    j     _L7
_L6:
    lw    $8, 0($29)
    lw    $9, 4($29)
    slt   $8, $9, $8
    beq   $8, $0, _L8
    lui   $4, 0x1001
    ori   $4, $4, 0x81
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
    ori   $4, $4, 0x95
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
    ori   $4, $4, 0xb2
    ori   $2, $0, 0x4
    syscall
_L11:
_L10:
    lui   $4, 0x1001
    ori   $4, $4, 0xd2
    ori   $2, $0, 0x4
    syscall
    lw    $4, 36($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xe0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 40($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xed
    ori   $2, $0, 0x4
    syscall
    lw    $4, 44($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xfb
    ori   $2, $0, 0x4
    syscall
    lw    $4, 48($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x103
    ori   $2, $0, 0x4
    syscall
    lw    $4, 52($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x110
    ori   $2, $0, 0x4
    syscall
    lw    $4, 56($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x120
    ori   $2, $0, 0x4
    syscall
    lw    $4, 60($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x12d
    ori   $2, $0, 0x4
    syscall
    lw    $4, 64($29)
    ori   $2, $0, 0x1
    syscall
_L12:
    lw    $8, 0($29)
    ori   $9, $0, 0x8
    slt   $8, $8, $9
    beq   $8, $0, _L13
    lui   $4, 0x1001
    ori   $4, $4, 0x13b
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 0($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 0($29)
    j     _L12
_L13:
    ori   $8, $0, 0x0
    sw    $8, 8($29)
_L14:
    lw    $8, 8($29)
    ori   $9, $0, 0x5
    slt   $8, $8, $9
    beq   $8, $0, _L15
    lui   $4, 0x1001
    ori   $4, $4, 0x148
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 8($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 8($29)
    j     _L14
_L15:
_L16:
    lw    $8, 4($29)
    ori   $9, $0, 0x1
    subu  $8, $8, $9
    sw    $8, 4($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x153
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 4($29)
    ori   $9, $0, 0x5
    slt   $8, $9, $8
    bne   $8, $0, _L16
    addiu $29, $29, 68
    ori   $2, $0, 0xa
    syscall
