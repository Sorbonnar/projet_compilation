
.data

a: .word 5
b: .word 10
.asciiz "Total: "
.asciiz "\n"

.text

main:
    addiu $29, $29, -8
    ori   $8, $0, 0xf
    sw    $8, 0($29)
    lui   $8, 0x1001
    lw    $8, 0($8)
    lui   $9, 0x1001
    lw    $9, 4($9)
    addu  $8, $8, $9
    lw    $9, 0($29)
    addu  $8, $8, $9
    sw    $8, 4($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x8
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x10
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 8
    ori   $2, $0, 0xa
    syscall
