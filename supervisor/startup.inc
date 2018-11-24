; The bootstrap has just loaded us and run us. We're at 0x0000, interrupts are
; disabled, and the world is ours to play with.

startup:
    ld sp, .data.supervisor_stack_end
    call init_interrupts
    call tty_init
    ld hl, .str.banner
    call tty_puts

outerloop:
    call kbd_get_next_key

    cp 'm'
    push af
    call z, motor_on_and_wait
    pop af

    cp 'r'
    push af
    call z, recalibrate_and_report
    pop af

    cp 's'
    push af
    call z, seek_and_report
    pop af

    jr outerloop

recalibrate_and_report:
    call fd765_recalibrate_twice
    jr report

seek_and_report:
    ld a, 79
    call fd765_seek
    jr report

motor_on_and_wait:
    call fd765_wait_until_drive_ready
report:
    ld a, 'Y'
    jr nz, skip
    ld a, 'N'
skip:
    call tty_putc
    ret

.str.banner: db "NC200 CP/M Supervisor (C) 2018 David Given", 10, 0

.data.supervisor_stack:
    ds 48
.data.supervisor_stack_end:
