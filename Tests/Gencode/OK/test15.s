
.data

.asciiz "0"

.text

main:
    addiu $29, $29, -8
    ori   $8, $0, 0x1
    sw    $8, 0($29)
    lw    $8, 0($29)
    beq   $8, $0, _L1
    ori   $8, $0, 0x0
    sw    $8, 4($29)
_L1:
    lw    $8, 0($29)
    beq   $8, $0, _L2
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    j     _L3
_L2:
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
_L3:
    addiu $29, $29, 8
    ori   $2, $0, 0xa
    syscall
