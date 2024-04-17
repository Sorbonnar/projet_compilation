
.data

.asciiz "a: "
.asciiz "\nb: "

.text

main:
    addiu $29, $29, -24
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
    sw    $10, 8($29)
    ori   $10, $0, 0x4
    sw    $10, 12($29)
    ori   $10, $0, 0x5
    sw    $10, 16($29)
    ori   $10, $0, 0x6
    sw    $10, 20($29)
    ori   $10, $0, 0x7
    lw    $11, 20($29)
    addu  $10, $11, $10
    lw    $11, 16($29)
    addu  $10, $11, $10
    lw    $11, 12($29)
    addu  $10, $11, $10
    lw    $11, 8($29)
    addu  $10, $11, $10
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
    addiu $29, $29, 24
    ori   $2, $0, 0xa
    syscall
