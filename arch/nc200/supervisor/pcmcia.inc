; Amstrad NC200 cpmish BIOS © 2019 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.

IDEREG_DATA            = 0xc000
IDEREG_ERROR           = 0xc001
IDEREG_FEATURES        = 0xc001
IDEREG_SECCOUNT        = 0xc002
IDEREG_LBA0            = 0xc003
IDEREG_LBA1            = 0xc004
IDEREG_LBA2            = 0xc005
IDEREG_DEVLBA3         = 0xc006
IDEREG_STATUS          = 0xc007
IDEREG_COMMAND         = 0xc007
IDEREG_CONTROL         = 0xc00e
IDEREG_DATABLOCK       = 0xc400

IDE_STATUS_ERROR       = 0x01
IDE_STATUS_DATAREQUEST = 0x08
IDE_STATUS_READY       = 0x40
IDE_STATUS_BUSY        = 0x80

IDE_CMD_READ_SECTOR    = 0x20
IDE_CMD_WRITE_SECTOR   = 0x30
IDE_CMD_IDENTIFY       = 0xEC

pcmcia_init:
    ld a, CARD0_BANK
    out (PORT_BANK3), a     ; map card to 0xc000
    xor a
    out (PORT_BAUDRATE), a  ; read attribute space

    ; IX is our pointer to the attribute space.
    ld ix, 0xc000
L_again:
    ld a, (ix+0)
    cp 0xff                 ; end of chain
    jr z, L_bad_card
    cp 0x15                 ; CISTPL_VERS_1
    jr z, L_print_ata_banner
    cp 0x21                 ; CISTPL_FUNCID
    jr nz, L_continue
    ld a, (ix+4)
    cp 0x04                 ; fixed disk
    jr z, L_found_ata_card
L_continue:
    ld c, (ix+2)
    ld b, 0
    add ix, bc
    add ix, bc
    ld bc, 4
    add ix, bc
    jr L_again

L_no_card:
    ; If we get here, no card was found.
    ld hl, 0
    ld (ata_blocks), hl
    ld hl, str.no_card
    jp tty_puts

L_bad_card:
    ; If we get here, a bad card was found.
    ld hl, 0
    ld (ata_blocks), hl
    ld hl, str.not_ata
    jp tty_puts

L_print_ata_banner:
    ld hl, str.ata_card
    call tty_puts
    push ix
    push ix
    pop hl
    ld bc, 8
    add hl, bc              ; advance hl to banner text
.1
    ld a, (hl)
    or a
    jr z, .2
    push hl
    call tty_putc
    pop hl
    inc hl
    inc hl
    jr .1
.2
    pop ix
    call tty_newline
    jr L_continue

L_found_ata_card:
    ld a, 0x80
    out (PORT_BAUDRATE), a  ; access common space (with the IDE registers)

    ; Reset the card.

    ld a, 0xe0
    ld (IDEREG_DEVLBA3), a  ; select master
    ld a, 0x06
    ld (IDEREG_CONTROL), a  ; reset, no interrupts
    halt
    halt
    halt                    ; ~30ms delay
    ld a, 0x02
    ld (IDEREG_CONTROL), a  ; release reset, no interrupts
    halt
    halt
    halt

    ld a, IDE_STATUS_READY
    call ide_wait
    ld a, 0xe0
    ld (IDEREG_DEVLBA3), a  ; select master
    ld a, 0x02
    ld (IDEREG_FEATURES), a ; 8-bit mode

    ; Query the card capacity (and verify it is a card).

    ld a, IDE_STATUS_READY
    call ide_wait
    ld a, 0xe0
    ld (IDEREG_DEVLBA3), a  ; select master
    ld a, IDE_CMD_IDENTIFY
    ld (IDEREG_COMMAND), a
    ld a, (IDEREG_STATUS)
    or a
    jr z, L_bad_card

    ld a, IDE_STATUS_DATAREQUEST
    call ide_wait
	ld hl, IDEREG_DATABLOCK
	ld de, ATABUF
	ld bc, 512
	ldir

    ld hl, str.capacity1
    call tty_puts

    ; ATABUF+120 contains the 32-bit number of 512-byte sectors in the disk.
    ; The BIOS wants to know the number of 16384-byte blocks. As there are
    ; 32 physical sectors per block, we need to shift this right by five
    ; bits. Oh, boy.

    ld b, 5
.1
    xor a
    ld hl, ATABUF+123
    rr (hl)
    dec hl
    rr (hl)
    dec hl
    rr (hl)
    dec hl
    rr (hl)
    djnz .1

    ld de, DRIVE_B_BLOCKS   ; maximum number of blocks
    ld hl, (ATABUF+122)     ; high word of LBA size
    ld a, h
    or l
    jr nz, .2               ; disk is too big
    ld hl, (ATABUF+120)     ; low word of LBA size
    ld a, d
    cp h                    ; compare with high byte of maximum number of blocks
    jr nc, .2               ; disk is still too big
    xchg                    ; computed number of blocks is valid; de->hl
.2
    ld (ata_blocks), hl     ; save the number of blocks
    ; There are 64 blocks per megabyte. 6-bit right shift from
    ; http://www.chilliant.com/z80shift.html:
    xor a
    add hl, hl
    rla
    add hl, hl
    rla
    ld l, h
    ld h, a
    call tty_putdec16
    ld hl, str.capacity2
    jp tty_puts

ide_wait:
    ld c, a                 ; c = mask
.1
    ld a, (IDEREG_STATUS)
    ld b, a                 ; b = IDE status

    ld a, c
    or IDE_STATUS_BUSY|IDE_STATUS_ERROR
    and b
    cp c
    jr z, .2                ; successful exit

    ld a, IDE_STATUS_BUSY|IDE_STATUS_ERROR
    and b
    cp IDE_STATUS_ERROR
    jr nz, .1               ; retry if busy but no error

    push bc
    ld hl, str.ide_error
    call tty_puts
    pop af
    call tty_puthex8
    call tty_newline

    cp a                    ; set z (failed)
    ret
.2
    or a                    ; set nz (success)
    ret

; Reads/writes a CP/M 128-byte sector, with deblocking etc.
;
; On entry:
;   BC = zero-based track to load
;   DE = zero-based sector to load
;   HL = userspace address for data
;
; Returns nz on success, z on error.

ata_read128:
    call setup_ata_read_or_write
    ret z

    ld a, IDE_CMD_READ_SECTOR
    ld (IDEREG_COMMAND), a

    ld a, IDE_STATUS_DATAREQUEST
    call ide_wait
    ret z

    ; Copy the block from the IDE interface into the ATA buffer.

    ld de, ATABUF           ; DE = destination address
    ld hl, IDEREG_DATABLOCK ; HL = source address
    ld bc, 512
    ldir

    ; Now copy the CP/M sector from the ATA buffer into userspace.

    ld hl, (current_dma)
    call map_userspace_dma
    ex de, hl               ; DE = destination address

    ld a, (current_sector)
    and 3                   ; get 0, 1, 2, 3 offset
    rra                     ; HL = A<<7
    ld h, a
    ld l, 0
    rr l
    ld bc, ATABUF
    add hl, bc              ; HL = address of sector

    ld bc, 128
    ldir

    or 1                    ; set nz
    ret

ata_write128:
    call setup_ata_read_or_write
    ret z
    cp a                    ; set z
    ret

setup_ata_read_or_write:
    ld a, (current_sector+1) ; check that the sector count <0x400
    cp 4
    jp nc, out_of_bounds

    ld a, CARD0_BANK
    out (PORT_BANK3), a     ; map card to 0xc000
    ld a, 0x80
    out (PORT_BAUDRATE), a  ; read common space

    ld a, 0xe0              ; top LBA byte is 0 (plus drive sector)
    ld (IDEREG_DEVLBA3), a

    ld hl, (current_track)  ; load the track number
    ld (IDEREG_LBA1), hl

    ld hl, (current_sector) ; load the sector number / 4
    srl h
    rr l
    srl h
    rr l
    ld a, l
    ld (IDEREG_LBA0), a     ; corrupts DEVLBA1, but we fix that next

    ld a, IDE_STATUS_READY
    call ide_wait
    ret z

    ld a, 1
    ld (IDEREG_SECCOUNT), a ; number of sectors to transfer

    or 1                    ; set nz
    ret

ata_blocks: dw 0

str.capacity1: db "Card is ATA with ", 0
str.capacity2: db " MB", 13, 10, 0
str.ide_error: db "IDE error: ", 0
str.ata_card:  db "PCMCIA card found: ", 0
str.not_ata:   db "Card is not ATA", 13, 10, 0
str.no_card:   db "No PCMCIA card found", 13, 10, 0
