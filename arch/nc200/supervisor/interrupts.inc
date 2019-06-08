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

; This routine actually does the work; it's called by the supervisor's own
; interrupt handler and also the one proxied through the BIOS.

label SIRQ
		push af
		push hl
		push bc
		push de

		; Status port bits are *zero* to indicate a pending interrupt.

		in a, (IRQ_STATUS_PORT)
		out (IRQ_STATUS_PORT), a        ; clear pending interrupts

		bit IRQ_POWER, a
		push af
		call z, power_interrupt_handler
		pop af

		bit IRQ_KEYBOARD, a
		jr nz, .1
		call keyboard_interrupt_handler
		call fd765_motor_interrupt_handler
.1:
		pop de
		pop bc
		pop hl
		pop af
		ret
    
power_interrupt_handler:
		xor a
		out (0x70), a
		; does not return

keyboard_interrupt_handler:
		; The keyboard handler is also the timer interrupt, so increment that.
		ld hl, (data.approximate_time)
		inc hl
		ld (data.approximate_time), hl

		ld hl, data.keyboard_bitmap
		ld c, KBD_SCAN_PORT
.2:
		in a, (c)
		ld b, a
		xor (hl)
		jr z, .3                ; skip loop if no bits changed state

        ; This byte has changed status. a tells us which.
        push hl
        ld (hl), b
        ld h, a             ; h = change bitset
        ld l, b             ; l = pressed bitset
        ld b, 8
        .1:
            add hl, hl          ; shift h and l left one bit
            jr nc, .4
                ; Compute the keycode.

                ld a, c
                sub KBD_SCAN_PORT
                add a, a
                add a, a
                add a, a
                add a, b
                dec a

                bit 0, h            ; set z if key pressed
                jr nz, .5
                or 0x80
            .5:
                push bc
                push hl
                call process_keyboard_event
                pop hl
                pop bc
            .4:
        djnz .1
        pop hl

.3:
		inc hl
		inc c
		ld a, KBD_SCAN_PORT+10
		cp c
		jr nz, .2
		ret

init_interrupts:
		ld a, 1<<IRQ_KEYBOARD | 1<<IRQ_POWER
		out (IRQ_MASK_PORT), a
		im 1
		call keyboard_interrupt_handler ; scan the keyboard now
		xor a
		ld (data.keyboard_rdptr), a ; flush the keyboard event buffer
		ld (data.keyboard_wrptr), a
		ei
		ret
