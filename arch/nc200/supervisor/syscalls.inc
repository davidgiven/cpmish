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
    jp (hl)

syscall_exit:
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
    
    call cache_evict_all
    jp warmboot
    ; does not return

    if SWBOOT_DEBUG
    .1:
        db "[WBOOT]", 0
    endif

; Read console: returns char in A.

label SCONIN
    call cache_flush_unwritten
    ld hl, disk_activity_flag
    ld (hl), 0

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
    ld hl, disk_activity_flag
    bit 0, (hl)
    ld (hl), 0
    jr nz, .1                   ; only flush buffers if there *hasn't* been activity
    call cache_flush_unwritten
.1:
    call tty_draw_cursor
    call kbd_test_pending_key
    ld l, 0
    jr z, syscall_exit
    call cache_flush_unwritten  ; or if the user pressed a key
    ld l, 0xff
    jr syscall_exit

; Read 128-byte CP/M sector.
label SREAD
    ld a, (BDISK)
    or a
    jr nz, general_purpose_failure

    if SREAD_DEBUG
        ld a, '{'
        call tty_putc
    endif
    
    ld hl, disk_activity_flag
    ld (hl), 1
    ld hl, BTRACK
    ld d, (hl)

    if SREAD_DEBUG
        ld a, d
        push de
        call tty_puthex8
        ld a, ' '
        call tty_putc
        pop de
    endif

    ld hl, BSECTOR
    ld e, (hl)

    if SREAD_DEBUG
        ld a, e
        push de
        call tty_puthex8
        ld a, ' '
        call tty_putc
        pop de
    endif

    ld hl, (BDMA)

    if SREAD_DEBUG
        push hl
        push de
        call tty_puthex16
        pop de
        pop hl
    endif
    
    call cache_read128

    if SREAD_DEBUG
        push af
        ld a, '}'
        call tty_putc
        pop af
    endif

    ld l, 0
    jr z, general_purpose_failure
    jp syscall_exit

general_purpose_failure:
    ld l, 1
    jp syscall_exit
    
; Write 128-byte CP/M sector.
label SWRITE
    ld a, (BDISK)
    or a
    jr nz, general_purpose_failure

    ld hl, disk_activity_flag
    ld (hl), 1
    ld hl, BTRACK
    ld d, (hl)
    ld hl, BSECTOR
    ld e, (hl)
    ld hl, (BDMA)
    call cache_write128
    ld l, 0
    jr z, general_purpose_failure
    jp syscall_exit

; Miscellaneous unsupported system calls.
label SLIST
label SLISTST
label SPUNCH
label SREADER
    ld l, 0
    jp syscall_exit

disk_activity_flag:
    db 0
