
.data

.asciiz "Even\n"
.asciiz "Odd\n"

.text

main:
    addiu $29, $29, -4
    ori   $8, $0, 0x0
    sw    $8, 0($29)
_L1:
    lw    $8, 0($29)
    ori   $9, $0, 0x3
    slt   $8, $8, $9
    beq   $8, $0, _L2
    lw    $8, 0($29)
    ori   $9, $0, 0x2
    div   $8, $9
    teq $9, $0
    mfhi  $8
    ori   $9, $0, 0x0
    xor   $8, $8, $9
    sltiu $8, $8, 1
    beq   $8, $0, _L3
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    j     _L4
_L3:
    lui   $4, 0x1001
    ori   $4, $4, 0x6
    ori   $2, $0, 0x4
    syscall
_L4:
    lw    $8, 0($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 0($29)
    j     _L1
_L2:
    addiu $29, $29, 4
    ori   $2, $0, 0xa
    syscall
