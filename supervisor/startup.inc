; The bootstrap has just loaded us and run us. We're at 0x0000, interrupts are
; disabled, and the world is ours to play with.

startup:
    ld sp, .data.supervisor_stack_end
    call syscalls_init
    call init_interrupts
    call tty_init
    ld hl, .str.banner
    call tty_puts
    ; fall through
coldboot:
    ; Initialise CP/M-y things in the base of userspace.

    ld a, USER0_BANK
    out (0x11), a               ; Map user space 0x0000 to 0x4000.
    xor a
    ld (0x4000 + IOBYTE), a     ; reset iobyte
    ld (0x4000 + CDISK), a      ; reset current disk
warmboot:
    ; Reload the CCP, BDOS and BIOS.

    call fd765_recalibrate_twice
    jr z, bootfailure
    ld a, USER3_BANK            ; Map user space 0xc000 to 0xc000.
    out (0x13), a
    ld b, 1
    ld de, 6*1024
    ld hl, 0x10000 - (6*1024)
    call fd765_read_track
    jr z, bootfailure

    ; Initialise the page zero CP/M-y things.

    ld a, USER0_BANK
    out (0x11), a               ; Map user space 0x0000 to 0x4000.
    ld a, 0xc3                  ; JP instruction

    ld (0x4000 + 0x0000), a     ; For warm boot
    ld hl, BBASE + 3
    ld (0x4000 + 0x0001), hl

    ld (0x4000 + BDOSCALL+0), a ; BDOS entrypoint
    ld hl, FBASE
    ld (0x4000 + BDOSCALL+1), hl

    ld (0x4000 + 0x0038), a     ; Interrupt handler
    ld hl, INTERRUPT
    ld (0x4000 + 0x0039), hl

    ; Now jump to the BIOS so it can page in the reset of userspace
    ; and start the CCP.

    jp BOOT

bootfailure:
    ld hl, .str.insert_disk
    call tty_puts
    call kbd_get_next_key
    jr warmboot
    
.str.insert_disk: db "[Cannot load CP/M, please insert a system disk]", 10, 0

.str.banner: db "NC200 CP/M Supervisor (C) 2018 David Given", 10, 0

.data.supervisor_stack:
    ds 48
.data.supervisor_stack_end:
