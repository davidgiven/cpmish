VIDEORAM_BASE: equ 0x2000
VIDEORAM_SIZE: equ 0x2000
SCREEN_WIDTH:  equ 480 / 8
SCREEN_HEIGHT: equ 160 / 8

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
    ld bc, VIDEORAM_SIZE
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

    ld a, (cursorx)
    inc a
    ld (cursorx), a
    cp SCREEN_WIDTH
    jr z, tty_newline
    ret

; --- Performs a newline ----------------------------------------------------

tty_newline:
    xor a
    ld (cursorx), a
    ld a, (cursory)
    inc a
    ld (cursory), a
    cp SCREEN_HEIGHT
    call z, tty_scroll
    ret

; --- Scrolls the screen by one line ----------------------------------------

tty_scroll:
    ld de, VIDEORAM_BASE
    ld hl, VIDEORAM_BASE + 512
    ld bc, VIDEORAM_SIZE - 512
    ldir
    ld h, d
    ld l, e
    inc de
    ld bc, 512
    ld (hl), 0
    ldir
    ret