
.data

.asciiz "BAND: "
.asciiz "BOR: "
.asciiz "BXOR: "
.asciiz "!C: "
.asciiz "-A: "
.asciiz "~A: "
.asciiz "Shift Left: "
.asciiz "Shift Right: "

.text

main:
    addiu $29, $29, -44
    ori   $8, $0, 0x5
    sw    $8, 0($29)
    ori   $8, $0, 0xa
    sw    $8, 4($29)
    ori   $8, $0, 0x1
    sw    $8, 8($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    and   $8, $8, $9
    sw    $8, 12($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    or    $8, $8, $9
    sw    $8, 16($29)
    lw    $8, 0($29)
    lw    $9, 4($29)
    xor   $8, $8, $9
    sw    $8, 20($29)
    lw    $8, 8($29)
    xori  $8, $8, 0x1
    sw    $8, 24($29)
    lw    $8, 0($29)
    subu  $8, $0, $8
    sw    $8, 28($29)
    lw    $8, 0($29)
    nor   $8, $0, $8
    sw    $8, 32($29)
    lw    $8, 0($29)
    ori   $9, $0, 0x2
    sllv  $8, $8, $9
    sw    $8, 36($29)
    lw    $8, 4($29)
    ori   $9, $0, 0x2
    srav  $8, $8, $9
    sw    $8, 40($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x0
    ori   $2, $0, 0x4
    syscall
    lw    $4, 12($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x7
    ori   $2, $0, 0x4
    syscall
    lw    $4, 16($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xd
    ori   $2, $0, 0x4
    syscall
    lw    $4, 20($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x14
    ori   $2, $0, 0x4
    syscall
    lw    $4, 24($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x19
    ori   $2, $0, 0x4
    syscall
    lw    $4, 28($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x1e
    ori   $2, $0, 0x4
    syscall
    lw    $4, 32($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x23
    ori   $2, $0, 0x4
    syscall
    lw    $4, 36($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x30
    ori   $2, $0, 0x4
    syscall
    lw    $4, 40($29)
    ori   $2, $0, 0x1
    syscall
    addiu $29, $29, 44
    ori   $2, $0, 0xa
    syscall
