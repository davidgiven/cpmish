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
    call cache_evict_all
    jp warmboot
    ; does not return

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

    ld hl, disk_activity_flag
    ld (hl), 1
    ld hl, BTRACK
    ld d, (hl)
    ld hl, BSECTOR
    ld e, (hl)
    ld hl, (BDMA)
    call cache_read128
    ld l, 0
    jr z, general_purpose_failure
    jr syscall_exit

general_purpose_failure:
    ld l, 1
    jr syscall_exit
    
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
