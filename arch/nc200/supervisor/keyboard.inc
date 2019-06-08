MODIFIER_BIT_SHIFT = 0
MODIFIER_BIT_CTRL = 1

data.keyboard_bitmap:
        ds 10, 0x55
data.keyboard_buffer:
        ds 8, 0x55
data.keyboard_rdptr:
        db 0
data.keyboard_wrptr:
        db 0
data.approximate_time:
        dw 0

; --- Processes an incoming keyboard event ----------------------------------

; Given a keyboard event in A, processes it and pushes a key into the buffer.
; Called from an interrupt state.

process_keyboard_event:
		ld b, a
		and 0x7f
		cp 0x09                         ; real control key
		jr z, ctrl_change
		cp 0x10                         ; caps lock key
		jr z, ctrl_change
		and 0x7e
		jr z, shift_change

		ld a, b
		sla a
		ret nc                          ; ignore keyups
		srl a

		ld b, 0
		ld c, a
		ld a, (keyboard_modifiers)
		ld hl, keyboard_normal_map
		bit 0, a
		jr z, .2
		ld hl, keyboard_shifted_map
.2:
		add hl, bc
		ld a, (hl)

		ld hl, keyboard_modifiers
		bit MODIFIER_BIT_CTRL, (hl)
		jr z, .1
		and 31
.1:
		or a
		ret z                           ; ignore ignored keys
		jp queue_key                    ; ...and push the keypress

shift_change:
		ld hl, keyboard_modifiers
		res MODIFIER_BIT_SHIFT, (hl)
		bit 7, b
		ret nz
		set MODIFIER_BIT_SHIFT, (hl)
		ret

ctrl_change:
		ld hl, keyboard_modifiers
		res MODIFIER_BIT_CTRL, (hl)
		bit 7, b
		ret nz
		set MODIFIER_BIT_CTRL, (hl)
		ret

keyboard_modifiers:
		db 0
    
; --- Pushes a key into the buffer ------------------------------------------

; Pushes A into the keyboard buffer.
; Interrupts must be DISABLED for this to work safely.

queue_key:
		ld c, a
		
		; Compute the keyboard buffer address in DE.

		ld a, (data.keyboard_rdptr)
		dec a
		and 7
		ld hl, data.keyboard_wrptr
		cp (hl)
		ret z                   ; keyboard buffer full

		ld l, (hl)
		ld h, 0
		ld de, data.keyboard_buffer
		add hl, de

		ld (hl), c              ; write key

		; Increment write pointer.

		ld hl, data.keyboard_wrptr
		ld a, (hl)
		inc a
		and 7
		ld (hl), a

		ret

; --- nz if there is a pending keypress -------------------------------------

kbd_test_pending_key:
		ld de, data.keyboard_rdptr
		ld hl, data.keyboard_wrptr
		di
		ld a, (de)
		cp (hl)
		ei
		ret

; --- Returns the next keypress, blocking -----------------------------------

; (Event returned in A.)
kbd_get_next_key:
		call kbd_test_pending_key
		jr z, kbd_get_next_key

		ld hl, data.keyboard_rdptr
		ld de, data.keyboard_buffer
		di
		ld a, (hl)
		add a, e
		ld e, a
		ld a, 0
		add a, d
		ld d, a

		ld a, (hl)
		inc a
		and 7
		ld (hl), a

		ld a, (de)
		ei
		ret

.include "keytab.inc"

