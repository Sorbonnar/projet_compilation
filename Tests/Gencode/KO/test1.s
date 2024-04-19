
.data

.asciiz "b: "
.asciiz "\n"

.text

main:
    addiu $29, $29, -8
    lw    $8, 0($29)
    ori   $9, $0, 0xa
    addu  $8, $8, $9
    sw    $8, 4($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x4
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 8
    ori   $2, $0, 0xa
    syscall
