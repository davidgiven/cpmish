; The bootstrap has just loaded us and run us. We're at 0x0000, interrupts are
; disabled, and the world is ours to play with.

startup:
    ld sp, .data.supervisor_stack_end
    call init_interrupts
    call tty_init
    ld hl, .str.banner
    call tty_puts

    ;call fd765_stop_polling
outerloop:
    ;ld a, 0x1f ; bit is active low
    ;out (BAUDCONTROL_PORT), a       ; drive on
    ;call fd765_sense_drive_state
    ;ld a, 0x3f ; bit is active low
    ;out (BAUDCONTROL_PORT), a       ; drive on
    di
    ld hl, .data.fd765_disk_changed
    ld b, (hl)
    xor a
    ld (hl), a
    ei

    ld a, b
    or a
    jr z, outerloop

    ld a, (.data.fd765_old_state)
    call tty_puthex8
    ld a, ' '
    call tty_putc
    jr outerloop

current_value:
    db 0

;outerloop:
;    call kbd_get_next_key
;
;    cp 'm'
;    push af
;    call z, motor_on_and_wait
;    pop af
;
;    cp 'h'
;    push af
;    call z, recalibrate_and_report
;    pop af
;
;    cp 's'
;    push af
;    call z, seek_and_report
;    pop af
;
;    cp 'r'
;    push af
;    call z, read_and_report
;    pop af
;
;    jr outerloop
;
;recalibrate_and_report:
;    call fd765_recalibrate_twice
;    jr report
;
;seek_and_report:
;    ld b, 79
;    call fd765_seek
;    jr report
;
;read_and_report:
;    ld b, 0
;    ld hl, VIDEORAM_BASE
;    call fd765_read_track
;    jr report
;
;motor_on_and_wait:
;    call fd765_wait_until_drive_ready
;report:
;    ld a, 'Y'
;    jr nz, skip
;    ld a, 'N'
;skip:
;    call tty_putc
;    ld a, (.data.fd765_status)
;    call tty_puthex8
;    ld a, (.data.fd765_status+1)
;    call tty_puthex8
;    ret
;
.str.banner: db "NC200 CP/M Supervisor (C) 2018 David Given", 10, 0

.data.supervisor_stack:
    ds 48
.data.supervisor_stack_end:
