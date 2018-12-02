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

label call_supervisor_from_bios
    ld sp, .data.supervisor_stack_end
    ei
    jp (hl)

syscall_exit:
    di
    ld a, USER3_BANK
    out (PORT_BANK3), a
    jp return_to_bios_from_supervisor

; Warm boot: reload the CCP, BDOS and BIOS.

label sys_wboot
    call cache_evict_all
    jp warmboot
    ; does not return

; Read console: returns char in A.

label sys_conin
    call kbd_get_next_key
    ld l, a
    jr syscall_exit

; Write console: writes char in A.

label sys_conout
    push bc
    call cache_flush_unwritten
    pop bc
    
    ld a, c
    call tty_putc
    jr syscall_exit

; Test console: returns 0xff in A if console has characters to read.
label sys_const
    ld l, 0
    jr syscall_exit

; Home disk (only drive A).
label sys_home
;    ld a, (bios_selected_disk)
;    or a
;    jr nz, 1f
;    call fd765_recalibrate
;1:
    jr syscall_exit

; Read 128-byte CP/M sector.
label sys_read128
    ld a, (bios_selected_disk)
    or a
    jr nz, general_purpose_failure

    ld hl, bios_selected_track
    ld d, (hl)
    ld hl, bios_selected_sector
    ld e, (hl)
    ld hl, (bios_selected_dma)
    call cache_read128
    ld l, 0
    jr z, general_purpose_failure
    jr syscall_exit

general_purpose_failure:
    ld l, 1
    jr syscall_exit
    
; Write 128-byte CP/M sector.
label sys_write128
    ld a, (bios_selected_disk)
    or a
    jr nz, general_purpose_failure

    ld hl, bios_selected_track
    ld d, (hl)
    ld hl, bios_selected_sector
    ld e, (hl)
    ld hl, (bios_selected_dma)
    call cache_write128
    ld l, 0
    jr z, general_purpose_failure
    jr syscall_exit

; Miscellaneous unsupported system calls.
label sys_list
label sys_listst
label sys_punch
label sys_reader
    ld l, 0
    jr syscall_exit
