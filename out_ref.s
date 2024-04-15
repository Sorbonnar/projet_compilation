
.data


.text

main:
    addiu $29, $29, -12
    ori   $8, $0, 0x1
    ori   $9, $0, 0x2
    ori   $10, $0, 0x3
    sw    $10, 4($29)
    ori   $10, $0, 0x4
    sw    $10, 8($29)
    ori   $10, $0, 0x5
    lw    $11, 8($29)
    addu  $10, $11, $10
    lw    $11, 4($29)
    addu  $10, $11, $10
    addu  $9, $9, $10
    addu  $8, $8, $9
    sw    $8, 0($29)
    addiu $29, $29, 12
    ori   $2, $0, 0xa
    syscall
