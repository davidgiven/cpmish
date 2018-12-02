VIDEORAM_BASE: equ 0x2000
VIDEORAM_SIZE: equ 0x2000
SCREEN_WIDTH:  equ 480 / 6
SCREEN_HEIGHT: equ 128 / 8

cursorx: db 0
cursory: db 0

tty_init:
    call tty_clear_screen
    ld a, VIDEORAM_BASE>>8
    out (0x00), a           ; set base address of video RAM
    ret

; --- Clears (and initialises) the screen -----------------------------------

tty_clear_screen:
    ld hl, VIDEORAM_BASE
    ld de, VIDEORAM_BASE+1
    ld bc, VIDEORAM_SIZE-1
    ld (hl), 0
    ldir
tty_home_cursor:
    xor a
    ld (cursorx), a
    ld (cursory), a
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
    and 0x03
    add a, a            ; calculate remainder (0, 2, 4, 6)
    ld b, a
    ld a, 10
    sub b               ; calculate LEFT shift
    ld (L_char_shift), a

    ; Calculate the mask.

    ld hl, 0xffc0       ; 1111.1111 1100.0000
    ld b, a
1:
    scf
    rl l
    rl h
    djnz 1b
    ld (L_char_mask), hl ; H is on the LEFT
    ret
L_cursor_address:
    dw 0
L_char_shift:
    db 0
L_char_mask:
	dw 0

; --- Draw (or undraw) the cursor -------------------------------------------

tty_draw_cursor:
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

; --- Performs a carriage return --------------------------------------------

tty_carriagereturn:
    xor a
    ld (cursorx), a
    ret

; --- Prints the character in A ---------------------------------------------
; (also tty_newline)

tty_putc:
    ; Handle special characters.

    cp 10
    jp z, tty_newline
    cp 13
    jp z, tty_carriagereturn

    ; Calculate screen address and which pixel decoder to use.

    push af
    call tty_calculate_screen_address
    pop af

    ; Get the pointer to the character data.

    and 0x7f
    sub 32
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
    sub 3               ; because our data is already shifted left three bits
1:
    add hl, hl
    dec a
    jr nz, 1b
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

L_chardata:
    ds 5

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
