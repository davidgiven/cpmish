include "include/cpm.inc"
include "include/supervisor.inc"

.section BIOS

; Boot vectors.

BIOS:
    jp BOOTE
    jp WBOOTE
    jp CONSTE
    jp CONINE
    jp CONOUTE
    jp LISTE
    jp PUNCHE
    jp READERE
    jp HOMEE
    jp SELDSKE
    jp SETTRKE
    jp SETSECE
    jp SETDMAE
    jp READE
    jp WRITEE
    jp LISTSTE
    jp SECTRANE
    jp INTERRUPTE

BOOTE:
    ld a, 'Q'
    call CONOUT
    jr .

WBOOTE:
    ld a, SYS_WBOOT
    jp syscall

CONSTE:
CONINE:
CONOUTE:
LISTE:
PUNCHE:
READERE:
HOMEE:
SELDSKE:
SETTRKE:
SETSECE:
SETDMAE:
READE:
WRITEE:
LISTSTE:
SECTRANE:
INTERRUPTE:
    jr .

syscall:
    di
    push af
    ld a, SUPERVISOR_BANK
    out (0x10), a
    pop af
    ld hl, syscall_return
    jp 0
syscall_return:
    ld a, USER0_BANK
    ld c, 0x10
    ld b, 3
1:
    out (c), a
    inc a
    inc c
    djnz 1b
    ei
    ret
