
.data

.asciiz "x = "
.asciiz "\ny = "
.asciiz "\nadd = "
.asciiz "\nsub = "
.asciiz "\nmul = "
.asciiz "\ndiv = "
.asciiz "\n"

.text

main:
    addiu $29, $29, -24
    ori   $8, $0, 0x1
    sw    $8, 0($29)
    lui   $8, 0xf
    ori   $8, $8, 0xffff
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
    ori   $10, $0, 0x1
    addu  $9, $9, $10
    div   $8, $9
    teq $9, $0
    mflo  $8
    sw    $8, 20($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x5
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xb
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x13
    ori   $2, $0, 0x4
    syscall
    lw    $4, 12($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x1b
    ori   $2, $0, 0x4
    syscall
    lw    $4, 16($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x23
    ori   $2, $0, 0x4
    syscall
    lw    $4, 20($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x2b
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 24
    ori   $2, $0, 0xa
    syscall
