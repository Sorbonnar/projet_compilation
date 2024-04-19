
.data

.asciiz "Addition: "
.asciiz "\nSubtraction: "
.asciiz "\nMultiplication: "
.asciiz "\nDivision 1: "
.asciiz "\nDivision 2: "
.asciiz "\nModulo: "
.asciiz "\n"

.text

main:
    addiu $29, $29, -32
    ori   $8, $0, 0x6
    sw    $8, 0($29)
    ori   $8, $0, 0x2
    sw    $8, 4($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    addu  $8, $8, $9
    sw    $8, 8($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    subu  $8, $8, $9
    sw    $8, 12($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    mult  $8, $9
    mflo  $8
    sw    $8, 16($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    div   $8, $9
    teq $9, $0
    mflo  $8
    sw    $8, 20($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    div   $8, $9
    teq $9, $0
    mflo  $8
    sw    $8, 24($29)
    lw    $8, 4($29)
    lw    $9, 0($29)
    div   $8, $9
    teq $9, $0
    mfhi  $8
    sw    $8, 28($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xb
    ori   $2, $0, 0x4
    syscall
    lw    $4, 12($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x1a
    ori   $2, $0, 0x4
    syscall
    lw    $4, 16($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x2c
    ori   $2, $0, 0x4
    syscall
    lw    $4, 20($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x3a
    ori   $2, $0, 0x4
    syscall
    lw    $4, 24($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x48
    ori   $2, $0, 0x4
    syscall
    lw    $4, 28($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x52
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 32
    ori   $2, $0, 0xa
    syscall
