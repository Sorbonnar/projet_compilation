
.data

num: .word 10
.asciiz "! = "
.asciiz "\n"

.text

main:
    addiu $29, $29, -8
    ori   $8, $0, 0x1
    sw    $8, 0($29)
    ori   $8, $0, 0x1
    sw    $8, 4($29)
_L1:
    lw    $8, 4($29)
    lui   $9, 0x1001
    lw    $9, 0($9)
    slt   $8, $9, $8
    xori  $8, $8, 0x1
    beq   $8, $0, _L2
    lw    $8, 0($29)
    lw    $9, 4($29)
    mult  $8, $9
    mflo  $8
    sw    $8, 0($29)
    lw    $8, 4($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 4($29)
    j     _L1
_L2:
    lui   $4, 0x1001
    lw    $4, 0($4)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x4
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x9
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 8
    ori   $2, $0, 0xa
    syscall
