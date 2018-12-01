include "include/cpm.inc"
include "include/supervisor.inc"
include "include/nc200.inc"

.section BIOS

; BIOS jump table.

label BOOT
    jp BOOTE
label WBOOT
    jp WBOOTE
label CONST
    jp CONSTE
label CONIN
    jp CONINE
label CONOUT
    jp CONOUTE
label LIST
    jp LISTE
label PUNCH
    jp PUNCHE
label READER
    jp READERE
label HOME
    jp HOMEE
label SELDSK
    jp SELDSKE
label SETTRK
    jp SETTRKE
label SETSEC
    jp SETSECE
label SETDMA
    jp SETDMAE
label READ
    jp READE
label WRITE
    jp WRITEE
label LISTST
    jp LISTSTE
label SECTRAN
    jp SECTRANE
label INTERRUPT
    jp INTERRUPTE

; BIOS interrupt handler. This is only invoked when the USER0 is mapped, and
; we can assume the stack is valid.

label bios_interrupt_handler
    push af
    ld a, SUPERVISOR_BANK
    out (PORT_BANK0), a
    call supervisor_interrupt_handler
    ld a, USER0_BANK
    out (PORT_BANK0), a
    pop af
    ei
    ret

; Actual BIOS entrypoints.
;
; The BIOS calls typically use a simple calling convention where the parameter
; is in BC and the result is returned in A and HL. The docs don't mention
; anything about other registers so we'll assume they can be corrupted. In
; addition, our syscall stuff assumes that on return A = L, just like in the
; BDOS, so we have a single parameter and a single response --- all very simple.

BOOTE:
    ; Map userspace.
    ld a, USER0_BANK
    out (PORT_BANK0), a
    inc a
    out (PORT_BANK1), a
    inc a
    out (PORT_BANK2), a

    ld a, (CDISK)
    ld c, a
    jp CBASE                ; enter the CCP
    
WBOOTE:
    ld hl, sys_wboot
    jr syscall

CONSTE:
    ld hl, sys_const
    jr syscall

CONINE:
    ld hl, sys_conin
    jr syscall

CONOUTE:
    ld hl, sys_conout
    jr syscall

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
    ret

INTERRUPTE:
    ld e, SYS_INTERRUPT
    ; fall through

; On entry, HL contains the system call handler address and BC the argument.
syscall:
    di
    ld (saved_stack), sp
    
    ; Map the supervisor in. At this point, our stack becomes inaccessible
    ; (because it might not be mapped).

    ld a, SUPERVISOR_BANK
    out (PORT_BANK0), a
    jp call_supervisor_from_bios

; On entry, HL contains the result.
label return_to_bios_from_supervisor
    ; Map user space back in.

    ld a, USER0_BANK
    out (PORT_BANK0), a
    inc a
    out (PORT_BANK1), a
    inc a
    out (PORT_BANK2), a

    ; The stack is now accessible. Load the result and return.

    ld sp, (saved_stack)
    ld a, l
    ei
    ret

saved_stack:
    dw 0
