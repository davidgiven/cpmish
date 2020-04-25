; Amstrad NC200 cpmish BIOS © 2019 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.

WAITSTATE_PORT:   equ 0x20
FD_ST_PORT:       equ 0xe0
FD_DT_PORT:       equ 0xe1

FD_MOTOR_ON_TIME: equ 5000 / 10; tick every 10ms
FD_DRIVE_READY_COUNTS: equ 100 ; very approximately 100 10ms ticks

; --- FDD power timer -------------------------------------------------------

; Called by the keyboard handler every approximately 10ms.
fd765_motor_interrupt_handler:
		ld hl, (data.fd765_motor_on_time)
		ld a, h
		or l
		jr z, fd765_irq_motor_off
		dec hl
		ld (data.fd765_motor_on_time), hl
		ret
fd765_irq_motor_off:
		ld hl, PBAUD
		set 5, (hl)
		ld a, (hl)
		out (PORT_BAUDRATE), a
		ret

data.fd765_motor_on_time:
		dw FD_MOTOR_ON_TIME ; motor is running on power on

; --- Turn the motor on (it turns off by itself) ----------------------------

fd765_motor_on:
		di
		ld hl, PBAUD
		res 5, (hl)			; bit is active low
		ld a, (hl)
		out (PORT_BAUDRATE), a
		ld hl, FD_MOTOR_ON_TIME
		ld (data.fd765_motor_on_time), hl
		ei
		ret

; --- Twiddle the Terminal Count line to the FDC ----------------------------

fd765_nudge_tc:
		ld a, 0x83
		out (WAITSTATE_PORT), a
		dec a
		out (WAITSTATE_PORT), a
		ret

; --- Writes A to the FDC data register -------------------------------------

fd765_tx:
		push af
fd765_tx_loop:
		in a, (FD_ST_PORT)
		rla							; RQM...
		jr nc, fd765_tx_loop		; ...low, keep waiting

		pop af
		out (FD_DT_PORT), a
		ret

; --- Reads status from the FDC data register -------------------------------

; Reads bytes from the FDC data register until the FDC tells us to stop (by
; lowering DIO in the status register).

fd765_read_status:
		ld hl, data.fd765_status
		ld c, FD_DT_PORT
read_status_loop:
		in a, (FD_ST_PORT)
		rla 						; RQM...
		jr nc, read_status_loop 	; ...low, keep waiting 
		rla							; DIO...
		ret nc						; ...low, no more data
		ini							; (hl)++ = port(c); b--
		jr read_status_loop
data.fd765_status:
		ds 8						; 8 bytes of status data

; --- Wait for the drive to become ready -----------------------------------

; Returns nz on success, z on failure
fd765_wait_until_drive_ready:
		call fd765_motor_on
		ld hl, FD_DRIVE_READY_COUNTS
fd765_wait_until_drive_ready_loop:
		push hl
		call fd765_sense_drive_state
		pop hl
		bit 5, a
		ret nz
		dec hl
		halt ; wait a bit, probably about 10ms
		ld a, h
		or l
		jr nz, fd765_wait_until_drive_ready_loop
		; zero flag set on exit
		ret

; --- Does SENSE DRIVE STATE ------------------------------------------------

; Performs the SENSE DRIVE STATE command, returning S3 in A.

fd765_sense_drive_state:
		ld a, 4                         ; SENSE DRIVE STATE
		call fd765_tx
		xor a                           ; head 0, drive 0
		call fd765_tx
		call fd765_read_status
		ld a, (data.fd765_status)
		ret

; --- Does RECALIBRATE ------------------------------------------------------

; Returns nz on success, z on failure.
; Note: only does a single recalibrate (more may be necessary).

fd765_recalibrate:
		call fd765_wait_until_drive_ready
		ld a, 7                     ; RECALIBRATE
		call fd765_tx
		xor a                       ; head 0, drive 0
		call fd765_tx
		; falls through
; Waits for a SEEK or RECALIBRATE command to finish by polling SENSE INTERRUPT STATUS.
fd765_wait_for_seek_ending:
		ld a, 8	    				; SENSE INTERRUPT STATUS
		call fd765_tx
		call fd765_read_status

		ld a, (data.fd765_status)
		bit 5, a					; SE, seek end
		jr z, fd765_wait_for_seek_ending
		ret
 
 ; Recalibrates twice (to get the entire 80 track range).
 ; Returns nz on success, z on failure.
 fd765_recalibrate_twice:
		call fd765_recalibrate
		ret z
		jr fd765_recalibrate

 ; --- Does SEEK ------------------------------------------------------------

 ; Seeks to track in deblock buffer in IX.
 ; Returns nz on success, z on failure.

 fd765_seek:
		call fd765_wait_until_drive_ready
		ret z

		ld a, 15                    ; SEEK
		call fd765_tx
		xor a                       ; head 0, drive 0
		call fd765_tx
		ld a, (ix+DBS_CUR_TRACK)    ; track number
		call fd765_tx
		jr fd765_wait_for_seek_ending

; --- Reads or writes a sector ----------------------------------------------

; Reads/writes a CP/M 128-byte sector, with deblocking etc.
;
; On entry:
;   BC = zero-based track to load
;   DE = zero-based sector to load
;
; Returns nz on success, z on error.

fd_read128:
    ld ix, FDBUF
	jp generic_read128

fd_write128:
	ld ix, FDBUF
	jp generic_write128

; Given a deblocker structure in IX, reads the described sector into the
; buffer. This transfers 512 bytes of data.
; Returns nz on success, z on failure.

label FD_R512
		ld a, 0x66 ; READ SECTORS
		call fd765_setup_read_or_write ; sets HL and DE
		ret z
		; fall through
; Read DE bytes from the 765 into HL.
fd765_read_bytes_from_controller:
		ld c, FD_DT_PORT
		di ; terribly important
.1
		in a, (FD_ST_PORT)
		rla							; RQM...
		jr nc, .1                   ; ...low, keep waiting
		rla							; DIO (ignore)
		rla							; EXM...
		jr nc, .2                   ; ...low, transfer complete
		ini							; (HL++) = port(C), B--

		dec de
		ld a, d
		or e
		jr nz, .1
.2
		ei
		jr fd765_complete_transfer

; Given a deblocker structure in IX, writes the described sector out of the
; buffer. This transfers 512 bytes of data.
; Returns nz on success, z on failure.

label FD_W512
		ld a, 0x65 ; WRITE SECTORS MULTITRACK
		call fd765_setup_read_or_write ; sets HL or DE
		ret z
		; fall through
; Writes de bytes to the 765 from HL.
fd765_write_bytes_to_controller:
		ld c, FD_DT_PORT
		di ; terribly important
.1
		in a, (FD_ST_PORT)
		rla							; RQM...
		jr nc, .1                   ; ...low, keep waiting
		rla							; DIO (ignore)
		rla							; EXM...
		jr nc, .2                   ; ...low, transfer complete
		outi						; port(C) = (HL++), B--

		dec de
		ld a, d
		or e
		jr nz, .1
.2
		ei
		jr fd765_complete_transfer

; Finish up a read or write transfer.
; Returns nz on success, z on failure.
fd765_complete_transfer:
		call fd765_nudge_tc
		call fd765_read_status
		; Parsing the status code is fiddly, because we're allowed a readfail if
		; EN is set.
		ld a, (data.fd765_status+1)
		rla                         ; EN->C
		ld a, (data.fd765_status+0)
		rla                         ; IC6->b7, IC7->C, EN->b0
		rla                         ; IC6->C, IC7->b0, EN->b1
		rla                         ; IC6->b0, IC7->b1, EN->b2
		and 7                       ; clip off stray bits
		ld hl, data.fd765_status_codes
		ld b, 0
		ld c, a
		add hl, bc
		ld a, (hl)
		or a						; return nz on success, z on failure
		ret

data.fd765_status_codes:
		; EN, IC7, IC6
		db 1    ; OK
		db 0    ; readfail
		db 0    ; unknown command
		db 0    ; disk removed
		db 1    ; OK
		db 1    ; reached end of track
		db 0    ; unknown command
		db 0    ; disk removed

; On entry, A contains the opcode, and IX is the deblocker structure.
; Returns nz on success, z on failure.
fd765_setup_read_or_write:
		ld (data.readwritecommand+0), a
		xor a
		ld (data.readwritecommand+1), a ; physical head (in bit 2)
		ld (data.readwritecommand+3), a	; logical head
		ld a, (ix+DBS_CUR_SECTOR)
		cp 9						; sectors 9-17 are on the other head
		jr c, .1					; sectors 0-8

		push af
		ld a, 4
		ld (data.readwritecommand+1), a ; physical head (in bit 2)
		ld a, 1
		ld (data.readwritecommand+3), a ; logical head
		pop af
		
		sub 9						; ...and adjust sector numbering
.1
		inc a						; cpmish uses 0-based IDs
		ld (data.readwritecommand+4), a
		ld (data.readwritecommand+6), a
		ld a, (ix+DBS_CUR_TRACK)
		ld (data.readwritecommand+2), a

		call fd765_seek
		ret z

		ld hl, data.readwritecommand
		ld b, data.readwritecommand.end - data.readwritecommand
		ld c, FD_DT_PORT
.3
		in a, (FD_ST_PORT)
		rla							; RQM...
		jr nc, .3                   ; ...low, keep waiting
		outi                        ; port(C) = (HL++), B++
		jr nz, .3

		ld d, ixh
		ld e, ixl
		ex de, hl
		ld de, DBS_BUFFER
		add hl, de					; HL is address of buffer
		ld de, 512					; DE is count

		or 1                        ; set nz
		ret

; The baked command to read or write a track.

data.readwritecommand:
		db 0        ; 0: opcode, filled in later
		db 0        ; 1: physical head 0, drive 0
		db 0        ; 2: track, filled in later
		db 0        ; 3: logical head 0
		db 1        ; 4: start sector, always 1
		db 2        ; 5: bytes per sector: 512
		db 9        ; 6: last sector (*inclusive*)
		db 27       ; 7: gap 3 length (27 is standard for 3.5" drives)
		db 0        ; 8: sector length (unused)
data.readwritecommand.end:

