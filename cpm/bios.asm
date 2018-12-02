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
    ld hl, sys_list
    jr syscall

PUNCHE:
    ld hl, sys_punch
    jr syscall
    
READERE:
    ld hl, sys_reader
    jr syscall

HOMEE:
    ld hl, sys_home
    jr syscall

; Selects a drive, returning the address of the DPH in HL (or 0x0000 on
; error).
SELDSKE:
    ld a, c
    ld (bios_selected_disk), a
    or a
    jr z, select_drive_a
    ld hl, 0
    ret
select_drive_a:
    ld hl, drive_a_dph
    ret

SETTRKE:
    ld a, c
    ld (bios_selected_track), a
    ret

SETSECE:
    ld a, c
    ld (bios_selected_sector), a
    ret

SETDMAE:
    ld hl, bios_selected_dma
    ld (hl), c
    inc hl
    ld (hl), b
    ret

READE:
    ld hl, sys_read128
    jr syscall

WRITEE:
    ld hl, sys_write128
    jr syscall

LISTSTE:
    ld hl, sys_listst
    jr syscall

SECTRANE:
    ld h, b
    ld l, c
    ret

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

drive_a_dph:
    dw 0            ; Sector translation vector
    dw 0, 0, 0      ; BDOS scratchpad
    dw dirbuf       ; Directory scratchpad
    dw drive_a_dpb  ; Drive parameter block
    dw drive_a_check_vector ; Disk change check vector
    dw drive_a_bitmap ; Allocation bitmap

DRIVE_A_SIZE = 720
DRIVE_A_BLOCKS = (DRIVE_A_SIZE - (2*9)) / 2

drive_a_dpb:
    dw 18*4         ; Number of CP/M sectors per track
    db 4, 15        ; BSH/BLM for 2048-byte blocks
    db 0            ; EXM for 2048-byte allocation units and >255 blocks
    dw DRIVE_A_BLOCKS-1 ; DSM
    dw 127          ; DRM, one less than the number of directory entries
    db 0xc0, 0x00   ; Initial allocation vector for two directory blocks
    dw 32           ; Size of disk change check vector: (DRM+1)/4
    dw 2            ; Number of reserved tracks

drive_a_bitmap:
    ds (DRIVE_A_BLOCKS+7) / 8
drive_a_check_vector:
    ds 32
dirbuf:
    ds 128

label bios_selected_disk
    db 0
label bios_selected_track
    db 0
label bios_selected_sector
    db 0
label bios_selected_dma
    dw 0
