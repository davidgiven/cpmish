WAITSTATE_PORT:   equ 0x20
BAUDCONTROL_PORT: equ 0x30
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
		ld a, 0x3f 
		out (BAUDCONTROL_PORT), a
		ret

data.fd765_motor_on_time:
		dw FD_MOTOR_ON_TIME ; motor is running on power on

; --- Turn the motor on (it turns off by itself) ----------------------------

fd765_motor_on:
		di
		ld a, 0x1f ; bit is active low
		out (BAUDCONTROL_PORT), a
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

 ; Seeks to track B.
 ; Returns nz on success, z on failure.

 fd765_seek:
		push bc
		call fd765_wait_until_drive_ready
		pop bc
		ret z

		ld a, 15                    ; SEEK
		call fd765_tx
		xor a                       ; head 0, drive 0
		call fd765_tx
		ld a, b                     ; track number
		call fd765_tx
		jr fd765_wait_for_seek_ending

; --- Reads or writes a track -----------------------------------------------

; Seeks to track B and reads up to DE bytes from it into the buffer at HL.
; This can transfer up to NINE KILOBYTES of data.
; Returns nz on success, z on failure.

fd765_read_complete_track:
		ld de, 9*1024
fd765_read_track:
		ld a, 9
		ld (data.readwritecommand+6), a
		ld a, 0xc6 ; READ SECTORS MULTITRACK
		call fd765_setup_read_or_write
		ret z
		jr fd765_read_bytes_from_controller

; Read de bytes from the 765 into HL.
fd765_read_bytes_from_controller:
		ld c, FD_DT_PORT
		di ; terribly important
fd765_read_loop:
		in a, (FD_ST_PORT)
		rla							; RQM...
		jr nc, fd765_read_loop      ; ...low, keep waiting
		rla							; DIO (ignore)
		rla							; EXM...
		jr nc, fd765_read_loop_finished ; ...low, transfer complete
		ini							; (HL++) = port(C), B--

		dec de
		ld a, d
		or e
		jr nz, fd765_read_loop
fd765_read_loop_finished:
		ei
		jr fd765_complete_transfer

; Seeks to track B and writes a complete track to it from the buffer at HL.
; This will write NINE KILOBYTES of data.
; Returns nz on success, z on failure.

fd765_write_complete_track:
		ld de, 9*1024
		ld a, 9
		ld (data.readwritecommand+6), a
		ld a, 0xc5 ; WRITE SECTORS MULTITRACK
		call fd765_setup_read_or_write
		ret z
		jr fd765_write_bytes_to_controller

; Writes de bytes to the 765 from HL.
fd765_write_bytes_to_controller:
		ld c, FD_DT_PORT
		di ; terribly important
.1:
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
.2:
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
		ld hl, (data.fd765_status_codes)
		ld b, 0
		ld c, a
		add hl, bc
		ld a, (hl)
		or a
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

; On entry, A contains the opcode, B contains the track. HL is preserved.
; Returns nz on success, z on failure.
fd765_setup_read_or_write:
		push hl
		ld (data.readwritecommand+0), a
		ld hl, data.readwritecommand+2
		ld (hl), b
		call fd765_seek
		pop hl
		ret z

		push hl
		ld hl, data.readwritecommand
		ld b, data.readwritecommand.end - data.readwritecommand
		ld c, FD_DT_PORT
fd765_setup_read_or_write_loop:
		in a, (FD_ST_PORT)
		rla							; RQM...
		jr nc, fd765_setup_read_or_write_loop ; ...low, keep waiting
		outi                        ; port(C) = (HL++), B++
		jr nz, fd765_setup_read_or_write_loop
		pop hl
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
		db 9        ; 6: last sector for one track (*inclusive*)
		db 27       ; 7: gap 3 length (27 is standard for 3.5" drives)
		db 0        ; 8: sector length (unused)
data.readwritecommand.end:

