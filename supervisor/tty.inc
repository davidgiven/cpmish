VIDEORAM_BASE: equ 0x2000
VIDEORAM_SIZE: equ 0x2000
SCREEN_WIDTH:  equ 480 / 8
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

tty_calculate_screen_address:
    ld a, (cursory)
    add a, a            ; a = cursory * 2
    ld h, a
    ld a, (cursorx)
    ld l, a             ; hl = cursory*512 + cursorx
    ld de, VIDEORAM_BASE
    add hl, de
    ret

; --- Draw (or undraw) the cursor -------------------------------------------

tty_draw_cursor:
    call tty_calculate_screen_address
    ld b, 8
    ld de, 64
tty_draw_cursor_loop:
    ld a, -1
    xor (hl)
    ld (hl), a
    add hl, de
    djnz tty_draw_cursor_loop
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

    ; Calculate HL = address of the char data in the font.

    and 0x7f
    sub 32
    ld l, a
    ld h, 0
    add hl, hl          ; hl = a * 2
    add hl, hl          ; hl = a * 4
    add hl, hl          ; hl = a * 8
    ld bc, .data.font
    add hl, bc

    ; Calculate DE = screen address to write to.

    push hl
    call tty_calculate_screen_address
    ex de, hl
    pop hl

    ld a, 8
tty_putc_loop:
    ldi                 ; (DE++) = (HL++)
    ex de, hl
    ld bc, 63
    add hl, bc
    ex de, hl
    dec a
    jr nz, tty_putc_loop

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