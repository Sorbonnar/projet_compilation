
.data

.asciiz "a: "
.asciiz "\nb: "
.asciiz "\n"

.text

main:
    addiu $29, $29, -8
    ori   $8, $0, 0x1
    ori   $9, $0, 0x2
    addu  $8, $8, $9
    ori   $9, $0, 0x3
    addu  $8, $8, $9
    ori   $9, $0, 0x4
    addu  $8, $8, $9
    ori   $9, $0, 0x5
    addu  $8, $8, $9
    ori   $9, $0, 0x6
    addu  $8, $8, $9
    ori   $9, $0, 0x7
    addu  $8, $8, $9
    sw    $8, 0($29)
    ori   $8, $0, 0x1
    ori   $9, $0, 0x2
    ori   $10, $0, 0x3
    ori   $11, $0, 0x4
    ori   $12, $0, 0x5
    ori   $13, $0, 0x6
    ori   $14, $0, 0x7
    addu  $13, $13, $14
    addu  $12, $12, $13
    addu  $11, $11, $12
    addu  $10, $10, $11
    addu  $9, $9, $10
    addu  $8, $8, $9
    sw    $8, 4($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x4
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x9
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 8
    ori   $2, $0, 0xa
    syscall
