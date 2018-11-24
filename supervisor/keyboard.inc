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
    ld hl, .data.keyboard_normal_map
    bit 0, a
    jr z, kbd_get_next_key_lookup_key_in_map
    ld hl, .data.keyboard_shifted_map
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

.data.keyboard_normal_map:
org .data.keyboard_normal_map + 80
.data.keyboard_shifted_map:

macro key, code, normal, shifted
    org .data.keyboard_normal_map + code
    db normal
    org .data.keyboard_shifted_map + code
    db shifted
endm

key 0x0a, 27,  27
key 0x12, '1', '!'
key 0x19, '2', '"'
key 0x18, '3', '#'
key 0x02, '4', '$'
key 0x14, '5', '%'
key 0x16, '6', '^'
key 0x21, '7', '&'
key 0x20, '8', '*'
key 0x0f, '9', '('
key 0x49, '0', ')'
key 0x41, '-', '_'
key 0x39, '=', '+'
key 0x32, 8,   8
key 0x4a, 127, 127

key 0x13, 9,   9
key 0x1a, 'q', 'Q'
key 0x1b, 'w', 'W'
key 0x1c, 'e', 'E'
key 0x26, 'r', 'R'
key 0x2c, 't', 'T'
key 0x2d, 'y', 'Y'
key 0x3d, 'u', 'U'
key 0x45, 'i', 'I'
key 0x4e, 'o', 'O'
key 0x4b, 'p', 'P'
key 0x43, '[', '{'
key 0x42, ']', '}'

key 0x10, 0,   0  ; caps lock/control
key 0x24, 'a', 'A'
key 0x1e, 's', 'S'
key 0x1f, 'd', 'D'
key 0x27, 'f', 'F'
key 0x2e, 'g', 'G'
key 0x36, 'h', 'H'
key 0x46, 'j', 'J'
key 0x3f, 'k', 'K'
key 0x4d, 'l', 'L'
key 0x4c, ';', ':'
key 0x44, 39,  '@'
key 0x34, '#', '~'
key 0x04, 10,  10

key 0x00, 0,   0  ; left shift
key 0x22, 'z', 'Z'
key 0x23, 'x', 'X'
key 0x2f, 'c', 'C'
key 0x2b, 'v', 'V'
key 0x2a, 'b', 'B'
key 0x37, 'n', 'N'
key 0x3e, 'm', 'M'
key 0x47, ',', '<'
key 0x4f, '.', '>'
key 0x35, '/', '?'
key 0x01, 0,   0  ; right shift
key 0x3b, 128|'A', 128|'A' ; cursor up

key 0x08, 0,   0  ; yellow
key 0x09, 0,   0  ; ctrl
key 0x11, 0,   0  ; symbol
key 0x0b, ' ', ' '
key 0x3a, 92, '|'
key 0x3c, 0,   0  ; menu
key 0x03, 128|'D', 128|'D'
key 0x33, 128|'C', 128|'C'
key 0x31, 128|'B', 128|'B'

org .data.keyboard_shifted_map + 80