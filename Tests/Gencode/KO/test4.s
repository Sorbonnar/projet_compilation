
.data


.text

main:
    addiu $29, $29, -4
    ori   $8, $0, 0x2
    ori   $9, $0, 0x0
    div   $8, $9
    teq $9, $0
    mfhi  $8
    sw    $8, 0($29)
    addiu $29, $29, 4
    ori   $2, $0, 0xa
    syscall
