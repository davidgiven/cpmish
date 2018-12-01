; --- !z if there is a pending keyboard event -------------------------------

kbd_test_pending_event:
    ld de, .data.keyboard_rdptr
    ld hl, .data.keyboard_wrptr
    di
    ld a, (de)
    cp (hl)
    ei
    ret

; --- Returns the next keyboard event, blocking -----------------------------

; (Event returned in A.)
kbd_get_next_event:
    call kbd_test_pending_event
    jr z, kbd_get_next_event

    di
    ld a, (de)
    ld hl, .data.keyboard_buffer
    add a, l
    ld l, a

    ld a, (de)
    inc a
    and 7
    ld (de), a

    ld a, (hl)
    ei
    ret

; --- Returns the next key, blocking ----------------------------------------

kbd_get_next_key:
    call tty_draw_cursor
1:
    call kbd_get_next_event
    ld b, a
    and 0x7e
    jr nz, 2f

    ; Shift is being pressed or released.
    ld hl, .data.modifiers
    res 0, (hl)
    bit 7, b
    jr nz, 1b
    set 0, (hl)
    jr 1b

2:
    ld a, b
    sla a
    jr nc, 1b
    srl a
    ld b, 0
    ld c, a
    ld a, (.data.modifiers)
    ld hl, keyboard_normal_map
    bit 0, a
    jr z, 2f
    ld hl, keyboard_shifted_map
2:
    add hl, bc
    ld a, (hl)
    or a
    jr z, 1b
    call tty_draw_cursor
    ret

.data.modifiers:
    db 0

.include ".obj/keyboard.inc"
