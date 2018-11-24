; im 1 interrupt handler.

IRQ_FDC:         equ 5
IRQ_POWER:       equ 4
IRQ_KEYBOARD:    equ 3
IRQ_SERIAL:      equ 2
IRQ_PARALLEL:    equ 0

IRQ_MASK_PORT:   equ 0x60
POWER_PORT:      equ 0x70
IRQ_STATUS_PORT: equ 0x90
KBD_SCAN_PORT:   equ 0xb0

; WARNING: registers must be preserved!
safeorg 0x0038
interrupt_handler:
    push af

    ; Status port bits are *zero* to indicate a pending interrupt.

    in a, (IRQ_STATUS_PORT)
    out (IRQ_STATUS_PORT), a        ; clear pending interrupts
    bit IRQ_POWER, a
    call z, power_interrupt_handler
    bit IRQ_KEYBOARD, a
    call z, keyboard_interrupt_handler
    
    pop af
    ei
    ret

power_interrupt_handler:
    xor a
    out (0x70), a
    ; does not return

keyboard_interrupt_handler:
    push hl
    push bc
    push de
    ld hl, .data.keyboard_bitmap
    ld c, KBD_SCAN_PORT
keyboard_read_loop:
    in a, (c)
    ld b, a
    xor (hl)
    jr z, keyboard_byte_hasnt_changed_status

        ; This byte has changed status. a tells us which.
        push hl
        ld (hl), b
        ld h, a             ; h = change bitset
        ld l, b             ; l = pressed bitset
        ld b, 8
    keyboard_read_bit_loop:
            add hl, hl          ; shift h and l left one bit
            jr nc, keyboard_read_bit_loop_end

            ; Compute the keyboard buffer address in DE.

            ld a, (.data.keyboard_rdptr)
            dec a
            and 7
            ld d, a
            ld a, (.data.keyboard_wrptr)
            cp d
            jr z, keyboard_read_bit_loop_end ; keyboard buffer full
            ld (.data.keyboard_wrptr), a
            ld de, .data.keyboard_buffer
            add a, e
            ld e, a

            ; Compute the keycode.

            ld a, c
            sub KBD_SCAN_PORT
            add a, a
            add a, a
            add a, a
            add a, b
            dec a

            bit 0, h            ; c = key pressed
            jr nz, keyboard_read_key_not_pressed
            or 0x80
        keyboard_read_key_not_pressed:
            ld (de), a

            ; Increment write pointer.

            ld de, .data.keyboard_wrptr
            ld a, (de)
            inc a
            and 7
            ld (de), a

        keyboard_read_bit_loop_end:
            djnz keyboard_read_bit_loop
        pop hl

keyboard_byte_hasnt_changed_status:
    inc hl
    inc c
    ld a, KBD_SCAN_PORT+10
    cp c
    jr nz, keyboard_read_loop
    pop de
    pop bc
    pop hl
    ret

init_interrupts:
    ld a, 1<<IRQ_KEYBOARD | 1<<IRQ_POWER
    out (IRQ_MASK_PORT), a
    im 1
    in a, (KBD_SCAN_PORT+9)     ; trigger a keyboard scan
    ei
    ret