; Amstrad NC200 cpmish BIOS © 2019 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.

SWBOOT_DEBUG:   equ 0
SREAD_DEBUG:    equ 0

; --- System call entry -----------------------------------------------------

; The BIOS calls here when it wants to execute a system call. Interrupts are
; disabled.
;
; On entry:
;   HL = system call handler address
;   BC = system call parameter
;   SP = unusable, due to not being mapped
;
; On exit, the system call jumps to syscall_exit with the result in HL.

label SYSIN
    ld sp, SYSSTK
    ei
    push ix
    push iy
    jp (hl)

syscall_exit:
    pop iy
    pop ix
    di
    ld a, USER3_BANK
    out (PORT_BANK3), a
    jp SYSOUT

; Warm boot: reload the CCP, BDOS and BIOS.

label SWBOOT
    if SWBOOT_DEBUG
        ld hl, .1
        call tty_puts
    endif
    
    jp warmboot
    ; does not return

    if SWBOOT_DEBUG
    .1:
        db "[WBOOT]", 0
    endif

; Read console: returns char in A.

label SCONIN
    call tty_draw_cursor
    call kbd_get_next_key
    ld l, a
    call tty_undraw_cursor
    jr syscall_exit

; Write console: writes char in A.

label SCONOUT
    call tty_undraw_cursor

    ld a, c
    call tty_putc
    jr syscall_exit

; Test console: returns 0xff in A if console has characters to read.
label SCONST
    call tty_draw_cursor
    call kbd_test_pending_key
    ld l, 0
    jr z, syscall_exit
    ld l, 0xff
    jr syscall_exit

; Read 128-byte CP/M sector.
label SREAD
    call setup_read_or_write

    ld hl, exit_read_or_write
    push hl
    ld a, (BDISK)
    or a
    jp z, fd_read128            ; drive 0
    dec a
    jp z, ata_read128           ; drive 1
    jr general_purpose_failure

; Write 128-byte CP/M sector.
label SWRITE
    call setup_read_or_write
    
    ld hl, exit_read_or_write
    push hl
    ld a, (BDISK)
    or a
    jp z, fd_write128           ; drive 0
    dec a
    jp z, ata_write128          ; drive 1
    ; fall through
general_purpose_failure:
    ld l, 1
    jp syscall_exit

exit_read_or_write:
    ld l, 0
    jr z, general_purpose_failure
    jp syscall_exit

setup_read_or_write:
    ld de, (BTRACK)             ; copy the transfer data into supervisor storage
    ld (current_track), de
    ld bc, (BSECTOR)
    ld (current_sector), bc
    ld hl, (BDMA)
    ld (current_dma), hl
    ld a, (BDEBLOCK)
    ld (deblock_flag), a
    ld a, (BDISK)
    ret

; Miscellaneous unsupported system calls.
label SLIST
label SLISTST
label SPUNCH
label SREADER
    ld l, 0
    jp syscall_exit

disk_activity_flag: db 0
current_sector:     dw 0
current_track:      dw 0
buffer_address:     dw 0
current_dma:        dw 0
deblock_flag:       db 0
