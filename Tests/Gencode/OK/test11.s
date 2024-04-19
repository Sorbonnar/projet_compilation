
.data

.asciiz "\na in while: "
.asciiz "\ni in for: "
.asciiz "\nb in do-while: "
.asciiz "\n"

.text

main:
    addiu $29, $29, -12
    ori   $8, $0, 0x3
    sw    $8, 0($29)
    ori   $8, $0, 0xa
    sw    $8, 4($29)
_L1:
    lw    $8, 0($29)
    ori   $9, $0, 0x8
    slt   $8, $8, $9
    beq   $8, $0, _L2
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 0($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 0($29)
    j     _L1
_L2:
    ori   $8, $0, 0x0
    sw    $8, 8($29)
_L3:
    lw    $8, 8($29)
    ori   $9, $0, 0x5
    slt   $8, $8, $9
    beq   $8, $0, _L4
    lui   $4, 0x1001
    ori   $4, $4, 0xe
    ori   $2, $0, 0x4
    syscall
    lw    $4, 8($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 8($29)
    ori   $9, $0, 0x1
    addu  $8, $8, $9
    sw    $8, 8($29)
    j     _L3
_L4:
_L5:
    lw    $8, 4($29)
    ori   $9, $0, 0x1
    subu  $8, $8, $9
    sw    $8, 4($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x1a
    ori   $2, $0, 0x4
    syscall
    lw    $4, 4($29)
    ori   $2, $0, 0x1
    syscall
    lw    $8, 4($29)
    ori   $9, $0, 0x5
    slt   $8, $9, $8
    bne   $8, $0, _L5
    lui   $4, 0x1001
    ori   $4, $4, 0x2b
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 12
    ori   $2, $0, 0xa
    syscall
