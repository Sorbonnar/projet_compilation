
.data

.asciiz "Max: "
.asciiz "\n"
.asciiz "Min: "
.asciiz "\n"
.asciiz "Overflow: "
.asciiz "\n"
.asciiz "Underflow: "
.asciiz "\n"

.text

main:
    addiu $29, $29, -16
    addiu $8, $0, -1
    sw    $8, 0($29)
    addiu $8, $0, -1
    subu  $8, $0, $8
    sw    $8, 4($29)
    lw    $8, 0($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 8($29)
    lw    $8, 4($29)
    ori   $9, $0, 0x1
    subu  $8, $8, $9
    sw    $8, 12($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x6
    ori   $2, $0, 0x4
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x8
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xe
    ori   $2, $0, 0x4
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x10
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x1b
    ori   $2, $0, 0x4
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x1d
    ori   $2, $0, 0x4
    syscall
    lw    $4, 12($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x29
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 16
    ori   $2, $0, 0xa
    syscall
