; Amstrad NC200 cpmish BIOS © 2019 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.

VIDEORAM_BASE      equ 0x2000
VIDEORAM_SIZE      equ 0x2000
FONT_HEIGHT        equ 7
SCREEN_WIDTH       equ 480 / 6
SCREEN_HEIGHT      equ 128 / FONT_HEIGHT
BYTES_PER_SCANLINE equ 64
BYTES_PER_LINE     equ BYTES_PER_SCANLINE * FONT_HEIGHT
LAST_LINE          equ VIDEORAM_BASE + (SCREEN_HEIGHT-1) * BYTES_PER_LINE
VIDEORAM_END       equ LAST_LINE + BYTES_PER_LINE

cursor_shown:  db 0

video_init:
    ld a, VIDEORAM_BASE>>8
    out (PORT_DISPLAY_MEMORY_ADDR), a ; set base address of video RAM
	jp tty_init
    ret

EMULATE_CLEAR_TO_EOL = 1
EMULATE_CLEAR_TO_EOS = 0
	maclib tty

; --- Calculates the address of the cursor ----------------------------------

; Sets cursor_screen_address to the address of the current char.

tty_calculate_screen_address:
    ld a, (tty_cursory)
    add a, a
    ld de, line_address_table
    ld h, 0
    ld l, a
    add hl, de
    ld a, (hl)
    inc hl
    ld h, (hl)
    ld l, a

    ; We want to calculate (cursorx*6/8) to find which horizontal byte our
    ; character is at; this is the same as cursorx*3/4, which will also
    ; fit in a byte (80*3 == 240).
    
    ld a, (tty_cursorx)
    ld b, a
    add a, a            ; a = cursorx*2
    add a, b            ; a = cursorx*2 + cursorx = cursorx*3
    ld b, a
    rra
    rra
    and 0x3f            ; divide by 8
    ld e, a             ; lower byte of address
    ld d, 0

    add hl, de
    ld (L_cursor_address), hl

	; Calculate the font data shift.

    ld a, b
    and 0x03            ; calculate remainder of cursorx*3/4 (0, 1, 2, 3)
    add a, a            ; calculate remainder of cursorx*6/8 (0, 2, 4, 6)
    ld (L_char_shift), a

    ; Calculate the mask.

    ld hl, char_mask_table
    ld d, 0
    ld e, a
    add hl, de
    ld a, (hl)
    inc hl
    ld h, (hl)
    ld l, a
    ld (L_char_mask), hl ; H is on the LEFT
    ret
L_cursor_address:
    dw 0
L_char_shift:
    db 0
L_char_mask:
	dw 0

char_mask_table:
    dw 0x03ff           ; 0000.0011 1111.1111
    dw 0xc0ff           ; 1100.0000 1111.1111
    dw 0xf03f           ; 1111.0000 0011.1111
    dw 0xfc0f           ; 1111.1100.0000.1111

line_address_table:
	line_no = 0
    rept SCREEN_HEIGHT
        dw VIDEORAM_BASE + line_no
        line_no = line_no + BYTES_PER_LINE
    endm

; As above, but calculates the address of the beginning of the current line.
; Returns the address in HL (as well as updating L_cursor_address).
tty_calculate_screen_address_of_line:
    call tty_calculate_screen_address
    ld hl, (L_cursor_address)
    ld a, 0xc0
    and l
    ld l, a                     ; get address of start of line
    ld (L_cursor_address), hl
    ret

; As above, but calculates the address of the beginning of the *next* line.
; Returns the address in HL (as well as updating L_cursor_address).
; Returns z if we're on the last line of the screen.
tty_calculate_screen_address_of_next_line:
    call tty_calculate_screen_address_of_line
    ld bc, 64
    add hl, bc                  ; get address of start of *next* line
    ld (L_cursor_address), hl
    ld bc, VIDEORAM_END
    or a
    sbc hl, bc
    add hl, bc
    ret

; --- Draw (or undraw) the cursor -------------------------------------------

tty_draw_cursor:
    push hl
    ld hl, cursor_shown
    bit 0, (hl)
    jr nz, .1
    set 0, (hl)
    call tty_xor_draw_cursor
.1:
    pop hl
    ret

tty_undraw_cursor:
    push hl
    ld hl, cursor_shown
    bit 0, (hl)
    jr z, .1
    res 0, (hl)
    call tty_xor_draw_cursor
.1:
    pop hl
    ret

tty_xor_draw_cursor:
	push af
	push bc
	push de
	push hl
    call tty_calculate_screen_address
    ld hl, L_char_mask
    ld a, (hl)
    xor 0xff
    ld (hl), a
    inc hl
    ld a, (hl)
    xor 0xff
    ld (hl), a

    ld b, FONT_HEIGHT
    ld de, BYTES_PER_SCANLINE - 1
    ld hl, (L_cursor_address)
tty_draw_cursor_loop:
	ld a, (L_char_mask+1)
	xor (hl)
	ld (hl), a
	inc hl
	ld a, (L_char_mask+0)
	xor (hl)
	ld (hl), a
    add hl, de
    djnz tty_draw_cursor_loop
    pop hl
    pop de
    pop bc
    pop af
    ret

; --- Screen clearing -------------------------------------------------------

tty_clear_to_eos:
    ; Compute the start of the area to clear.

    call tty_calculate_screen_address_of_next_line
    ret z

    ; Compute the *size* of the area to clear.

    ex de, hl                   ; stash start address in DE
    ld hl, VIDEORAM_BASE + SCREEN_HEIGHT*BYTES_PER_LINE
    and a                       ; clear carry flag
    sbc hl, de                  ; HL is amount to clear
    dec hl                      ; need one *fewer* than the size to clear
    ld b, h
    ld c, l
    ex de, hl

    ld d, h
    ld e, l
    inc de
    ld (hl), 0
    ldir

    jp tty_clear_to_eol         ; we haven't cleared the rest of this line

; --- Line insertion/removal ------------------------------------------------

tty_insert_line:
    call tty_calculate_screen_address_of_next_line
    jr z, _clear_current_line   ; skip the copy if we're on the last line.
    ; HL is address of the *next* line of text.

    ; Compute the size of the area to move
    call calculate_insert_delete_size

    ; We want to copy in reverse, which means HL and BC are fixed.
    
    ld hl, LAST_LINE - 1        ; source
    ld de, VIDEORAM_END - 1     ; dest
    ; BC is size-1
    lddr

_clear_current_line:
    ; Clear the current line.

    call tty_calculate_screen_address_of_line
    ld d, h
    ld e, l
    inc de
    ld bc, BYTES_PER_LINE - 1
    ld (hl), 0
    ldir
    ret

tty_delete_line:
    call tty_calculate_screen_address_of_next_line
    jr z, _clear_current_line   ; if we're on the last line, just clear it

    ; Compute the size of the area to move
    call calculate_insert_delete_size

    ; We want to copy forwards.

    push hl
    ld de, -BYTES_PER_LINE
    add hl, de                  ; Start of *current* line, source
    pop de                      ; Start of *next* line, dest
    ex de, hl                   ; Copy from next line to current line
    ldir

    ; Clear the last line.

    ld hl, LAST_LINE
    ld de, LAST_LINE + 1
    ld bc, BYTES_PER_LINE - 1
    ld (hl), 0
    ldir
    ret
    
; On entry: HL is the start of the NEXT line.
; On exit: HL is preserved; BC contains the amount to move.
calculate_insert_delete_size:
    ex de, hl                   ; stash start address in DE
    ld hl, VIDEORAM_END - 1
    and a                       ; clear carry flag
    sbc hl, de                  ; HL is amount to clear
    ld b, h
    ld c, l
    ex de, hl
    ret

; Writes A to the current cursor location, without advancing the cursor.

tty_rawwrite:
    push af
    call tty_calculate_screen_address
    pop af

    ; Get the pointer to the character data.

    and 0x7f
    sub 32
    ret m
    ld l, a
    ld h, 0
    add hl, hl          ; hl = a*2
    add hl, hl          ; hl = a*4
    ld e, a
    ld d, 0
    add hl, de          ; hl = a*4 + a = a*5
    ld de, FONT
    add hl, de
    ; hl points at font data

    ; We are now *finally* ready to start drawing.

    ld a, (hl)              ; XXXXX???.????????
    call draw_single_scanline

    ld b, (hl)              ; ?????XXX.XX??????
    inc hl
    ld a, (hl)
    srl b
    rra
    srl b
    rra
    srl b
    rra
    call draw_single_scanline

    ld a, (hl)              ; ??XXXXX?
    add a, a
    add a, a
    call draw_single_scanline

    ld a, (hl)              ; ???????X.XXXX????
    srl a
    inc hl
    ld a, (hl)  
    rra
    call draw_single_scanline

    ld a, (hl)              ; ????XXXX.X???????
    inc hl
    ld b, (hl)
    sll b
    rla
    add a, a
    add a, a
    add a, a
    call draw_single_scanline

    ld a, (hl)              ; ?XXXXX??
    add a, a
    call draw_single_scanline

    ld b, (hl)              ; ??????XX.XXX?????
    inc hl
    ld a, (hl)
    srl b
    rra
    srl b
    rra
    ; fall through

; On entry, the font data is in A, left justified.
; Font pointer is in HL.
draw_single_scanline:
    ex de, hl           ; save font pointer in DE
    and 0xf8
    ld l, a
    ld h, 0

    ; Rotate the font data to the right offset.

    ld a, (L_char_shift)
    ; Self modifying code ahoy! A can be 0, 2, 4 or 6. Our data is already
    ; left-justified, so it's already shifted left three bits, so we want
    ; to shift by 7, 5, 3 or 1 bits respectively.
    ld (scanline_shift_amount+1), a

scanline_shift_amount:
    jr $+2              ; offset based on start of *next* instruction

    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, hl
    ld b, h
    ld c, l             ; put the adjusted data in BC. Remember, B is on the LEFT.

    ; Actually adjust the screen.

    ld hl, (L_cursor_address)
    ld a, (L_char_mask+1)
    and (hl)
    or b
    ld (hl), a
    inc hl              ; note, HL changes by 1
    ld a, (L_char_mask+0)
    and (hl)
    or c
    ld (hl), a

    ; Advance to the next scanline.

    ld bc, 63           ; advance address to next scanline; remember HL has changed
    add hl, bc
    ld (L_cursor_address), hl

    ex de, hl           ; put font pointer back in HL
    ret

; --- Scrolls the screen by one line ----------------------------------------

tty_scroll:
    ld de, VIDEORAM_BASE
    ld hl, VIDEORAM_BASE + BYTES_PER_LINE
    ld bc, LAST_LINE - VIDEORAM_BASE
    ldir
    ld h, d
    ld l, e
    inc de
    ld bc, BYTES_PER_LINE - 1
    ld (hl), 0
    ldir
    ret
