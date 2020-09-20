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

IDE_STATUS_BIT_ERR     = 0
IDE_STATUS_BIT_IDX     = 1
IDE_STATUS_BIT_CORR    = 2
IDE_STATUS_BIT_DRQ     = 3
IDE_STATUS_BIT_SRV     = 4
IDE_STATUS_BIT_DF      = 5
IDE_STATUS_BIT_RDY     = 6
IDE_STATUS_BIT_BSY     = 7

IDE_CMD_READ_SECTOR    = 0x20
IDE_CMD_WRITE_SECTOR   = 0x30
IDE_CMD_IDENTIFY       = 0xEC

pcmcia_init:
    ld a, CARD0_BANK
    out (PORT_BANK3), a     ; map card to 0xc000
    ld hl, PBAUD
    di
    res 7, (hl)             ; read attribute space
    ld a, (hl)
    out (PORT_BAUDRATE), a
    ei

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
    ld (ATABUF+DBS_NUM_TRACKS), hl
    ld hl, str.no_card
    jp tty_puts

L_bad_card:
    ; If we get here, a bad card was found.
    ld hl, 0
    ld (ATABUF+DBS_NUM_TRACKS), hl
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
    ld hl, PBAUD
    di
    set 7, (hl)
    ld a, (hl)
    out (PORT_BAUDRATE), a  ; access common space (with the IDE registers)
    ei

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
    ld a, 0x02
    ld (IDEREG_FEATURES), a ; 8-bit mode

    ld a, IDE_STATUS_READY
    call ide_wait
    ld a, 0x82
    ld (IDEREG_FEATURES), a ; disable disk cache

    ; Query the card capacity (and verify it is a card).

    ld a, IDE_STATUS_READY
    call ide_wait
    ld a, 0xe0
    ld (IDEREG_DEVLBA3), a  ; select master
    ld a, IDE_CMD_IDENTIFY
    ld (IDEREG_COMMAND), a
    ld a, (IDEREG_STATUS)
    or a
    jp z, L_bad_card

    ld a, IDE_STATUS_DATAREQUEST
    call ide_wait
	ld hl, IDEREG_DATABLOCK
	ld de, ATABUF+DBS_BUFFER
	ld bc, 512
	ldir

    ld hl, str.capacity1
    call tty_puts

    ; buffer+120 contains the 32-bit number of 512-byte sectors in the disk.
    ; We've made sure that there are 256 512-byte sectors per track, so to
    ; get the track number we just want the top three bytes.

    ld hl, 0xffff           ; maximum number of tracks
    ld a, (ATABUF+DBS_BUFFER+123)      ; top byte
    or a
    jr nz, .1               ; too many tracks? just use hl
    ld hl, (ATABUF+DBS_BUFFER+121)
.1
    ld (ATABUF+DBS_NUM_TRACKS), hl

    ; There is 128kB on each track. To get megabytes, divide by 8.
    ld a, l
    srl h
    rra
    srl h
    rra
    srl h
    rra
    ld l, a
    call tty_putdec16
    ld hl, str.capacity2
    call tty_puts

    ld hl, 0xffff
    ld (ATABUF+DBS_CUR_SECTOR), hl
    ld (ATABUF+DBS_CUR_TRACK), hl
    xor a
    ld (ATABUF+DBS_DIRTY), a
    ret

; Waits for a specific IDE status, in A (or failure). Sets z on error.
ide_wait:
    ld c, a                 ; c = mask
.1
    ld a, (IDEREG_STATUS)   ; a = IDE status

    bit IDE_STATUS_BIT_BSY, a ; wait until not busy
    jr nz, .1

    bit IDE_STATUS_BIT_ERR, a ; did an error occur?
    jr nz, .2

    and c
    cp c                    ; response the user asked for?
    jr nz, .1

    or 1                    ; set nz (success)
    ret
.2
    ld hl, str.ide_error
    call tty_puts
    ld a, (IDEREG_ERROR)
    call tty_puthex8
    call tty_newline

    or a                    ; set z (failed)
    ret

; Reads/writes a CP/M 128-byte sector, with deblocking etc.
;
; On entry:
;   BC = zero-based sector to load
;   DE = zero-based track to load
;
; Returns nz on success, z on error.

ata_read128:
    ld ix, ATABUF
    jp generic_read128

ata_write128:
    ld ix, ATABUF
    jp generic_write128

label ATA_R512
    call setup_ata_read_or_write
    ret z

    ld a, IDE_CMD_READ_SECTOR
    ld (IDEREG_COMMAND), a

    ld a, IDE_STATUS_DATAREQUEST
    call ide_wait
    ret z

    ; Copy the block from the IDE interface into the ATA buffer.

    ld hl, DBS_BUFFER
    ld b, ixh
    ld c, ixl
    add hl, bc
    ex de, hl               ; DE = destination address
    ld hl, IDEREG_DATABLOCK ; HL = source address
    ld bc, 512
    ldir

    or 1                    ; set nz (success)
    ret

label ATA_W512
    call setup_ata_read_or_write
    ret z

    ld a, IDE_CMD_WRITE_SECTOR
    ld (IDEREG_COMMAND), a

    ld a, IDE_STATUS_DATAREQUEST
    call ide_wait
    ret z

    ; Copy the block from the ATA buffer interface into the IDE interface.

    ld hl, DBS_BUFFER
    ld b, ixh
    ld c, ixl
    add hl, bc              ; HL = source address
    ld de, IDEREG_DATABLOCK ; DE = destination address
    ld bc, 512
    ldir

    ld a, IDE_STATUS_READY
    call ide_wait           ; check for disk error
    ret

; Prepares to read and write the CURRENT block.
setup_ata_read_or_write:
    ; Page in the ATA drive and set up the command.

    ld a, CARD0_BANK
    out (PORT_BANK3), a     ; map card to 0xc000
    di
    ld hl, PBAUD
    set 7, (hl)
    ld a, (hl)
    out (PORT_BAUDRATE), a  ; read common space
    ei

    ld a, IDE_STATUS_READY
    call ide_wait
    ret z

    ld hl, IDEREG_LBA0

    ld a, (ix+DBS_CUR_SECTOR+0)
    ld (hl), a
    inc hl

    ld a, (ix+DBS_CUR_TRACK+0)
    ld (hl), a
    inc hl

    ld a, (ix+DBS_CUR_TRACK+1)
    ld (hl), a
    inc hl

    ld (hl), 0xe0           ; top LBA byte is 0 (plus drive selector)

    ld a, IDE_STATUS_READY
    call ide_wait
    ret z

    ld a, 1
    ld (IDEREG_SECCOUNT), a ; number of sectors to transfer

    or 1                    ; set nz
    ret

str.capacity1: db "Card is ATA with ", 0
str.capacity2: db " MB", 13, 10, 0
str.ide_error: db "IDE error: ", 0
str.ata_card:  db "PCMCIA card found: ", 0
str.not_ata:   db "Card is not ATA", 13, 10, 0
str.no_card:   db "No PCMCIA card found", 13, 10, 0

; vim: sw=4 ts=4 expandtab ft=asm

