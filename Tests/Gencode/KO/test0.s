
.data


.text

main:
    addiu $29, $29, -12
    ori   $8, $0, 0x5
    sw    $8, 0($29)
    ori   $8, $0, 0x0
    sw    $8, 4($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    div   $8, $9
    teq $9, $0
    mflo  $8
    sw    $8, 8($29)
    addiu $29, $29, 12
    ori   $2, $0, 0xa
    syscall
