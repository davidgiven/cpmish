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
    jp (hl)

syscall_exit:
    di
    ld a, USER3_BANK
    out (PORT_BANK3), a
    jp return_to_bios_from_supervisor

; Warm boot: reload the CCP, BDOS and BIOS.

label sys_wboot
    ei
    jp warmboot
    ; does not return

; Read console: returns char in A.

label sys_conin
    ei
    call kbd_get_next_key
    ld l, a
    jr syscall_exit

; Write console: writes char in A.

label sys_conout
    ei
    ld a, c
    call tty_putc
    jr syscall_exit

; Test console: returns 0xff in A if console has characters to read.
label sys_const
    ld l, 0
    jr syscall_exit
