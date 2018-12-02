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

MODIFIER_BIT_SHIFT = 0
MODIFIER_BIT_CTRL = 1

kbd_get_next_key:
    call tty_draw_cursor
kbd_get_next_key_again:
    call kbd_get_next_event
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
    jr nc, kbd_get_next_key_again   ; ignore keyups
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

    ld hl, .data.modifiers
    bit MODIFIER_BIT_CTRL, (hl)
    jr z, 1f
    and 31
1:
    or a
    jr z, 1b
    call tty_draw_cursor
    ret

shift_change:
    ld hl, .data.modifiers
    res MODIFIER_BIT_SHIFT, (hl)
    bit 7, b
    jr nz, kbd_get_next_key_again
    set MODIFIER_BIT_SHIFT, (hl)
    jr kbd_get_next_key_again

ctrl_change:
    ld hl, .data.modifiers
    res MODIFIER_BIT_CTRL, (hl)
    bit 7, b
    jr nz, kbd_get_next_key_again
    set MODIFIER_BIT_CTRL, (hl)
    jr kbd_get_next_key_again

.data.modifiers:
    db 0

.include ".obj/keyboard.inc"
