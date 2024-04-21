
.data

.asciiz "\n"

.text

main:
    addiu $29, $29, -16
    ori   $8, $0, 0x8
    sw    $8, 0($29)
    ori   $8, $0, 0x9
    sw    $8, 4($29)
    ori   $8, $0, 0xa
    sw    $8, 8($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 8($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 0($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 4($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 8($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 0($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 4($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 8($29)
    mult  $8, $9
    mflo  $8
    lw    $9, 0($29)
    mult  $8, $9
    mflo  $8
    sw    $8, 12($29)
    lw    $4, 12($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 16
    ori   $2, $0, 0xa
    syscall
