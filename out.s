
.data


.text

main:
    addiu $29, $29, -16
    ori   $8, $0, 0x1
    ori   $9, $0, 0x2
    sw    $10, 4($29)
    ori   $10, $0, 0x3
    lw    $10, 4($29)
    sw    $10, 4($29)
    sw    $10, 8($29)
    ori   $10, $0, 0x4
    lw    $10, 8($29)
    sw    $10, 8($29)
    sw    $10, 12($29)
    ori   $10, $0, 0x5
    lw    $10, 12($29)
    addu  $10, $10, $10
    lw    $10, 8($29)
    addu  $10, $10, $10
    lw    $10, 4($29)
    addu  $9, $9, $10
    lw    $9, 0($29)
    addu  $8, $8, $10
    lw    $8, -4($29)
    sw    $10, 0($29)
    addiu $29, $29, 16
    ori   $2, $0, 0xa
    syscall
