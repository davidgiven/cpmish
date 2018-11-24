WAITSTATE_PORT: equ 0x20
BAUDCONTROL_PORT: equ 0x30
FD_ST_PORT: equ	0xe0
FD_DT_PORT: equ	0xe1

FD_MOTOR_ON_TIME: equ 5000 / 10; tick every 10ms
FD_DRIVE_READY_COUNTS: equ 100 ; very approximately 100 10ms ticks

; --- FDD power timer -------------------------------------------------------

; Called by the keyboard handler every approximately 10ms.
fd765_motor_interrupt_handler:
    ld hl, (.data.fd765_motor_on_time)
    ld a, h
    or l
    jr z, fd765_irq_motor_off
    dec hl
    ld (.data.fd765_motor_on_time), hl
    ret
fd765_irq_motor_off:
    ld a, 0x3f 
    out (BAUDCONTROL_PORT), a
    ret

.data.fd765_motor_on_time:
    dw FD_MOTOR_ON_TIME ; motor is running on power on

; --- Turn the motor on (it turns off by itself) ----------------------------

fd765_motor_on:
    di
    ld a, 0x1f ; bit is active low
    out (BAUDCONTROL_PORT), a
    ld hl, FD_MOTOR_ON_TIME
    ld (.data.fd765_motor_on_time), hl
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
	ld hl, .data.fd765_status
	ld c, FD_DT_PORT
read_status_loop:
	in a, (FD_ST_PORT)
	rla 						; RQM...
	jr nc, read_status_loop 	; ...low, keep waiting 
	rla							; DIO...
	ret nc						; ...low, no more data
	ini							; (hl)++ = port(c); b--
	jr read_status_loop
.data.fd765_status:
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
    ld a, 4                     ; SENSE DRIVE STATE
    call fd765_tx
    xor a                       ; head 0, drive 0
    call fd765_tx
    call fd765_read_status
    ld a, (.data.fd765_status)
    ret

; --- Does RECALIBRATE ------------------------------------------------------

; Returns nz on success, z on failure.
; Note: only does a single recalibrate (more may be necessary).

fd765_recalibrate:
    call fd765_wait_until_drive_ready
    ret z
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

	ld a, (.data.fd765_status)
	bit 5, a					; SE, seek end
	jr z, fd765_wait_for_seek_ending
    ; returns nz
	ret
 
 ; Recalibrates twice (to get the entire 80 track range).
 ; Returns nz on success, z on failure.
 fd765_recalibrate_twice:
    call fd765_recalibrate
    ret z
    jr fd765_recalibrate
    
 ; --- Does SEEK ------------------------------------------------------------

 ; Seeks to track A.
 ; Returns nz on success, z on failure.

 fd765_seek:
    ld b, a
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
    