; The bootstrap has just loaded us and run us. We're at 0x0000, interrupts are
; disabled, and the world is ours to play with.

startup:
    call tty_init
    ld hl, .str.banner
    call tty_puts
    call tty_draw_cursor
    jr $

.str.banner: db "NC200 CP/M Supervisor (C) 2018 David Given", 10
db "Good luck!", 10, 0
