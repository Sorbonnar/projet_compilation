
.data

limit: .word 3
.asciiz "("
.asciiz ", "
.asciiz ")\n"
.asciiz "a: "
.asciiz "\n"
.asciiz "a: "

.text

main:
    addiu $29, $29, -16
    ori   $8, $0, 0x3
    sw    $8, 8($29)
    ori   $8, $0, 0x4
    sw    $8, 12($29)
    ori   $8, $0, 0x0
    sw    $8, 0($29)
_L1:
    lw    $8, 0($29)
    lui   $9, 0x1001
    lw    $9, 0($9)
    slt   $8, $9, $8
    xori  $8, $8, 0x1
    beq   $8, $0, _L2
    ori   $8, $0, 0x0
    sw    $8, 4($29)
_L3:
    lw    $8, 4($29)
    lui   $9, 0x1001
    lw    $9, 0($9)
    ori   $10, $0, 0x1
    addu  $9, $9, $10
    slt   $8, $8, $9
    beq   $8, $0, _L4
    lui   $4, 0x1001
    ori   $4, $4, 0x4
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x6
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x9
    ori   $2, $0, 0x4
    syscall
    lw    $8, 0($29)
    ori   $9, $0, 0x2
    mult  $8, $9
    mflo  $8
    lw    $9, 4($29)
    lw    $10, 12($29)
    subu  $9, $9, $10
    addu  $8, $8, $9
    sw    $8, 8($29)
    lui   $4, 0x1001
    ori   $4, $4, 0xc
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x10
    ori   $2, $0, 0x4
    syscall
    lw    $8, 4($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 4($29)
    j     _L3
_L4:
    lw    $8, 0($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 0($29)
    j     _L1
_L2:
    lui   $4, 0x1001
    ori   $4, $4, 0x12
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    addiu $29, $29, 16
    ori   $2, $0, 0xa
    syscall
