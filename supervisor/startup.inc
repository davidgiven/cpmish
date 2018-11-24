; The bootstrap has just loaded us and run us. We're at 0x0000, interrupts are
; disabled, and the world is ours to play with.

startup:
    ld sp, .data.supervisor_stack_end
    call init_interrupts
    call tty_init
    ld hl, .str.banner
    call tty_puts

outerloop:
    call tty_draw_cursor
    call kbd_get_next_key
    push af
    call tty_draw_cursor
    pop af
    call tty_putc
    jr outerloop

.str.banner: db "NC200 CP/M Supervisor (C) 2018 David Given", 10, 0

.data.supervisor_stack:
    ds 48
.data.supervisor_stack_end:
