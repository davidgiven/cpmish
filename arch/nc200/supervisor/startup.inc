; Amstrad NC200 cpmish BIOS © 2019 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.

; The bootstrap has just loaded us and run us. We're at 0x0000, interrupts are
; disabled, and the world is ours to play with.

label STARTUP
    ld sp, SYSSTK
    call init_interrupts
    call tty_init
    ld hl, str.banner
    call tty_puts
    call pcmcia_init
    ; fall through
coldboot:
    ; Initialise CP/M-y things in the base of userspace.

    ld a, USER0_BANK
    out (PORT_BANK1), a         ; Map user space 0x0000 to 0x4000.
    xor a
    ld (0x4000 + IOBYTE), a     ; reset iobyte
    ld (0x4000 + CDISK), a      ; reset current disk
warmboot:
    ; Reload the CCP, BDOS and BIOS.

    call fd765_recalibrate_twice
    jr z, bootfailure
    ld a, USER3_BANK            ; Map user space 0xc000 to 0xc000.
    out (PORT_BANK3), a
    ld b, 1
    ld de, 0xffff - CBASE       ; amount to read
    inc de                      ; ideally we'd use 0x10000 in the previous line but
                                ; the assembler doesn't like it
    ld hl, CBASE
    call fd765_read_track
    jr z, bootfailure

    ; Initialise the page zero CP/M-y things.

    ld a, USER0_BANK
    out (PORT_BANK1), a         ; Map user space 0x0000 to 0x4000.
    ld a, 0xc3                  ; JP instruction

    ld (0x4000 + 0x0000), a     ; For warm boot
    ld hl, BBASE + 3
    ld (0x4000 + 0x0001), hl

    ld (0x4000 + BDOSCALL+0), a ; BDOS entrypoint
    ld hl, FBASE + 6
    ld (0x4000 + BDOSCALL+1), hl

    ld (0x4000 + 0x0038), a     ; Interrupt handler
    ld hl, BIRQ
    ld (0x4000 + 0x0039), hl

    ; Initialise BIOS-y things.

    ld hl, 0x0080
    ld (BDMA), hl

    ld hl, (ata_blocks)
    dec hl                      ; actually want maximum block number
    ld (BHDSIZE), hl
    
    ; Now jump to the BIOS so it can page in the rest of userspace
    ; and start the CCP.

    jp BOOT

bootfailure:
    ld hl, str.insert_disk
    call tty_puts
    call kbd_get_next_key
    jr warmboot
    
str.insert_disk:
	db "[Cannot load CP/M, please insert a system disk]", 10, 0

str.banner:
    cpmish_banner "Amstrad NC200"
    db 0

supervisor_stack:
		ds 64
label SYSSTK
