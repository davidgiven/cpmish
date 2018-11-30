; --- Initialisation --------------------------------------------------------

syscalls_init:
    ld hl, (syscall_entry)
    ld (1), hl                  ; update the jump statement at 0x0000
    ret

; --- System call entry -----------------------------------------------------

; The BIOS calls here when it wants to execute a system call. Interrupts are
; disabled.
;
; On entry:
;   C = system call number
;   A, B, DE = system call parameters
;   HL = return address (in the BIOS)
;   SP = unusable, due to not being mapped, but must be preserved

syscall_entry:
    ld (L_syscall_saved_stack), sp
    ld (L_syscall_return_address), hl
    ld hl, L_syscall_saved_a
    ld (hl), a
    inc hl
    ld (hl), b
    inc hl
    ld (hl), e
    inc hl
    ld (hl), d
    ld sp, .data.supervisor_stack_end

    ld hl, syscall_jump_table
    ld b, 0
    add hl, bc
    add hl, bc
    ld a, (hl)
    inc hl
    ld h, (hl)
    ld l, a
    jp (hl)

sys_interrupt:
    call interrupt_handler
    ; Fall through
syscall_return:
    ld sp, (L_syscall_saved_stack)
    ld hl, (L_syscall_return_address)
    jp (hl)

; Read console: returns char in A.

sys_conin:
    ei
    call kbd_get_next_key
    di
    jr syscall_return

; Write console: writes char in A.

sys_conout:
    ei
    call tty_putc
    di
    jr syscall_return

; Test console: returns 0xff in A if console has characters to read.
sys_const:
    xor a
    jr syscall_return

syscall_jump_table:
    dw sys_interrupt
    dw sys_conin
    dw sys_conout
    dw sys_const

L_syscall_saved_a:
    db 0
L_syscall_saved_b:
    db 0
L_syscall_saved_de:
    dw 0
L_syscall_return_address:
    dw 0
L_syscall_saved_stack:
    dw 0
