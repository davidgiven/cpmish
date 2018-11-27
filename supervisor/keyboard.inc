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
    call kbd_get_next_event
    ld b, a
    and 0x7e
    jr z, kbd_get_next_key_shift_pressed_or_released
    ld a, b
    sla a
    jr nc, kbd_get_next_key
    srl a
    ld b, 0
    ld c, a
    ld a, (.data.modifiers)
    ld hl, keyboard_normal_map
    bit 0, a
    jr z, kbd_get_next_key_lookup_key_in_map
    ld hl, keyboard_shifted_map
kbd_get_next_key_lookup_key_in_map:
    add hl, bc
    ld a, (hl)
    or a
    jr z, kbd_get_next_key
    ret

; On entry, the key is in B.
kbd_get_next_key_shift_pressed_or_released:
    ld hl, .data.modifiers
    res 0, (hl)
    bit 7, b
    jr nz, kbd_get_next_key
    set 0, (hl)
    jr kbd_get_next_key

.data.modifiers:
    db 0

.include ".obj/keyboard.inc"
