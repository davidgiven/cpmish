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
    cp 0x80
    jr z, kbd_get_next_key_shift_released
    cp 0x81
    jr z, kbd_get_next_key_shift_released
    cp 0
    jr z, kbd_get_next_key_shift_pressed
    cp 1
    jr z, kbd_get_next_key_shift_pressed
    sla a
    jr nc, kbd_get_next_key
    srl a
    ld hl, .data.keyboard_normal_map
    ld b, 0
    ld c, a
    add hl, bc
    ld a, (hl)
    or a
    jr z, kbd_get_next_key
    ret
kbd_get_next_key_shift_pressed:
    ld hl, .data.modifiers
    set 0, (hl)
    jr kbd_get_next_key
kbd_get_next_key_shift_released:
    ld hl, .data.modifiers
    res 0, (hl)
    jr kbd_get_next_key

.data.modifiers:
    db 0
.data.keyboard_normal_map:
    db 0,   0,   0,   13,   128|'D', '4',  0,       0
    db '9', 0,   0,   0,    ' ',     27,   0,       0
    db 0,   '6', 0,   '5',  9,       '1',  0,       0
    db 'd', 's', 0,   'e',  'w',     'q',  '2',     '3'
    db 'f', 'r', 0,   'a',  'x',     'z',  '7',     '8'
    db 'c', 'g', 'y', 't',  'v',     'b',  0,       0
    db 'n', 'h', '/', '#',  128|'C', 127,  128|'B', '6'
    db 'k', 'm', 'u', 0,    128|'A', '\\', '=',     0
    db ',', 'j', 'i', 39,   '[',     ']',  '-',     0
    db '.', 'o', 'l', ';',  'p',     8,    '0',     0  

.data.keyboard_shifted_map:
    db 0,   0,   0,   13,   128|'D', '$',  0,       0
    db '(', 0,   0,   0,    ' ',     27,   0,       0
    db 0,   '^', 0,   '%',  9,       '!',  0,       0
    db 'D', 'S', 0,   'E',  'W',     'Q',  '"',     '#'
    db 'F', 'R', 0,   'A',  'X',     'Z',  '&',     '*'
    db 'C', 'G', 'Y', 'T',  'V',     'B',  0,       0
    db 'N', 'H', '?', '~',  128|'C', 127,  128|'B', '^'
    db 'K', 'M', 'U', 0,    128|'A', '|',  '+',     0
    db '<', 'J', 'I', '@',  '{',     '}',  '_',     0
    db '>', 'O', 'L', ':',  'P',     8,    ')',     0  
