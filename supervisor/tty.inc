VIDEORAM_BASE  = 0x2000
VIDEORAM_SIZE  = 0x2000
FONT_HEIGHT    = 8
SCREEN_WIDTH   = 480 / 6
SCREEN_HEIGHT  = 128 / FONT_HEIGHT

cursorx:       db 0
cursory:       db 0
cursor_shown:  db 0

commandlen:    db 0
commandgot:    db 0
commandbuf:    ds 3

tty_init:
    ld a, VIDEORAM_BASE>>8
    out (PORT_DISPLAY_MEMORY_ADDR), a ; set base address of video RAM
    call tty_clear_screen
    ret

; --- Prints a zero-terminated string in HL ---------------------------------

; Prints a zero-terminated string in hl.
tty_puts:
    ld a, (hl)
    or a
    ret z
    inc hl
    push hl
    call tty_putc
    pop hl
    jr tty_puts

; --- Prints the hex bytes in HL or A ---------------------------------------

; prints HL
tty_puthex16:
    ld a, h
    call tty_puthex8
    ld a, l
tty_puthex8:
    ld c, a
    rra
    rra
    rra
    rra
    call tty_puthex8_conv
    ld a, c
tty_puthex8_conv:
    and 15
    add a, 0x90
    daa
    adc a, 0x40
    daa
    push hl
    push bc
    call tty_putc
    pop bc
    pop hl
    ret

; --- Calculates the address of the cursor ----------------------------------

; Sets cursor_screen_address to the address of the current char.
; Also sets current_pixel_decoder to the pixel decoder routine.

tty_calculate_screen_address:
    ld a, (cursory)
    add a, a            ; a = cursory*2
    ld h, a             ; address will be cursory*512

    ; We want to calculate (cursorx*6/8) to find which horizontal byte our
    ; character is at; this is the same as cursorx*3/4, which will also
    ; fit in a byte (80*3 == 240).
    
    ld a, (cursorx)
    ld b, a
    add a, a            ; a = cursorx*2
    add a, b            ; a = cursorx*2 + cursorx = cursorx*3
    ld b, a
    rra
    rra
    and 0x3f            ; divide by 8
    ld l, a             ; lower byte of address

    ld de, VIDEORAM_BASE
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

; --- Draw (or undraw) the cursor -------------------------------------------

tty_draw_cursor:
    push hl
    ld hl, cursor_shown
    bit 0, (hl)
    jr nz, 1f
    set 0, (hl)
    call tty_xor_draw_cursor
1:
    pop hl
    ret

tty_undraw_cursor:
    push hl
    ld hl, cursor_shown
    bit 0, (hl)
    jr z, 1f
    res 0, (hl)
    call tty_xor_draw_cursor
1:
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

    ld b, 8
    ld de, 63
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

; --- Clears (and initialises) the screen -----------------------------------

tty_clear_screen:
    call tty_home_cursor
    jr tty_clear_to_eos
    
tty_home_cursor:
    xor a
    ld (cursorx), a
    ld (cursory), a
    ret

; --- Screen clearing -------------------------------------------------------

tty_clear_to_eol:
    call tty_calculate_screen_address
    ld b, FONT_HEIGHT
    ld hl, (L_cursor_address)
1:
    ld (hl), 0
    inc hl
    ld a, 63
    and l
    jr nz, 1b

    ld hl, (L_cursor_address)
    ld de, 64
    add hl, de
    ld (L_cursor_address), hl
    djnz 1b
    ret

tty_clear_to_eos:
    ; Compute the start of the area to clear.

    call tty_calculate_screen_address
    ld hl, (L_cursor_address)
    ld a, 0xc0
    and l
    ld l, a                     ; get address of start of line
    ld bc, 64
    add hl, bc                  ; get address of start of *next* line

    ; Compute the *size* of the area to clear.

    ex de, hl                   ; stash start address in DE
    ld hl, VIDEORAM_BASE + VIDEORAM_SIZE
    and a                       ; clear carry flag
    sbc hl, de                  ; HL is amount to clear
    ld a, h
    or l
    ret z                       ; give up if nothing
    dec hl                      ; need one *fewer* than the size to clear
    ld b, h
    ld c, l
    ex de, hl

    ld d, h
    ld e, l
    inc de
    ld (hl), 0
    ldir

    jr tty_clear_to_eol         ; we haven't cleared the rest of this line

; --- Performs a carriage return --------------------------------------------

tty_carriagereturn:
    xor a
    ld (cursorx), a
    ret

; --- Move the cursor -------------------------------------------------------

tty_cursor_left:
    ld hl, cursorx
    dec (hl)
    ret p
    inc (hl)
    ret

tty_cursor_up:
    ld hl, cursory
    dec (hl)
    ret p
    inc (hl)
    ret

tty_cursor_right:
    ld hl, cursorx
    ld a, (hl)
    inc a
    cp SCREEN_WIDTH
    ret z
    ld (hl), a
    ret

tty_cursor_down:
    ld hl, cursory
    ld a, (hl)
    inc a
    cp SCREEN_HEIGHT
    ret z
    ld (hl), a
    ret

; Move to (B, C).
tty_goto_xy:
    ld a, b
    cp SCREEN_WIDTH
    ret nc
    ld (cursorx), a

    ld a, c
    cp SCREEN_HEIGHT
    ret nc
    ld (cursory), a
    ret

; --- Prints the character in A ---------------------------------------------
; (also tty_newline)

; Helper routine: deal with command bytes (passed in C).
queue_command_byte:
    ; Write the byte to the buffer.

    ld hl, commandgot
    ld d, 0
    ld e, (hl)
    inc (hl)

    ld hl, commandbuf
    add hl, de
    ld (hl), c

    ; Have we reached the end of the buffer?

    ld hl, commandlen
    ld a, (commandgot)
    cp (hl)
    ret nz              ; no, go back for more bytes.
    xor a
    ld (hl), a          ; end of command

    ; Process a command.

    ld a, (commandbuf+0)
    cp 'A'
    jr z, tty_cursor_up
    cp 'B'
    jr z, tty_cursor_down
    cp 'C'
    jr z, tty_cursor_right
    cp 'D'
    jr z, tty_cursor_left
    cp 'H'
    jp z, tty_home_cursor
    cp 'J'
    jp z, tty_clear_to_eos
    cp 'K'
    jp z, tty_clear_to_eol
    cp 'Y'
    ret nz ; unknown command
    ld a, (commandgot)  ; Y takes parameters
    cp 3                ; do we have enough bytes?
    jr z, 1f            ; yes, execute command
    ld a, 3             ; not enough bytes read yet
    ld (commandlen), a
    ret
1:
    ld hl, commandbuf+1 ; got enough bytes; process command
    ld a, (hl)
    sub 32
    ld c, a             ; Y
    inc hl
    ld a, (hl)
    sub 32
    ld b, a             ; X
    jr tty_goto_xy

negchar:
    add a, 32
    push af
    ld a, '['
    call tty_putc
    pop af
    call tty_puthex8
    ld a, ']'
    call tty_putc
    ret
    
tty_putc:
    ; Check to see if there's a pending command.

    ld c, a
    ld a, (commandlen)
    or a
    jr nz, queue_command_byte

    ; Handle special characters.

    ld a, c
    cp 8
    jp z, tty_cursor_left
    cp 10
    jr z, tty_newline
    cp 13
    jp z, tty_carriagereturn
    cp 27
    jp nz, 1f
        ; Escape characters mean commands.
        xor a
        ld (commandgot), a
        inc a
        ld (commandlen), a
        ret
    1:

    ; Calculate screen address and which pixel decoder to use.

    push af
    call tty_calculate_screen_address
    pop af

    ; Get the pointer to the character data.

    and 0x7f
    sub 32
    jp m, negchar
    ld l, a
    ld h, 0
    add hl, hl          ; hl = a*2
    add hl, hl          ; hl = a*4
    ld e, a
    ld d, 0
    add hl, de          ; hl = a*4 + a = a*5
    ld de, .data.font
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
    call draw_single_scanline

    ld a, (hl)              ; ???XXXXX
    add a, a
    add a, a
    add a, a
    call draw_single_scanline

    ; Now we've drawn a character, advance the cursor.

    ld hl, cursorx
    ld a, (hl)
    inc a
    ld (hl), a
    cp SCREEN_WIDTH
    ret nz
tty_newline:
    xor a
    ld (cursorx), a
    ld hl, cursory
    ld a, (hl)
    inc a
    ld (hl), a
    cp SCREEN_HEIGHT
    ret nz
    dec (hl)                ; oops, don't go the next line after all
    jr tty_scroll

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
    ld (scanline_shift_amount), a
scanline_shift_amount = . + 1
    jr .+2              ; offset based on start of *next* instruction
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
    ld hl, VIDEORAM_BASE + 512
    ld bc, VIDEORAM_SIZE - 511
    ldir
    ld h, d
    ld l, e
    inc de
    ld bc, 511
    ld (hl), 0
    ldir
    ret
