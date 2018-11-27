CONTROL_PORT = 0x30
FD_POWER_PORT = 0x20
FD_ST_PORT = 0xe0
FD_DT_PORT = 0xe1


.section AUTOPRG

; On entry, 0x0000 contains bank 0x40 and 0x4000 contains (usually) 0x47. This
; means we can write to 0x0000 without stepping on ourselves.

main:
    ; Interrupts *off*! We want to kill the NC200 OS ASAP.

    di

    ; Now, load the entire first track to 0x0000. The 765 has just loaded
    ; this boot block so we know it's correctly seeked and spun up. We
    ; just need to issue the command and we're ready.

    ; Send the read command to the 765.

    ld hl, .data.readtrack
    ld b, .data.readtrack.end - .data.readtrack
    ld c, FD_DT_PORT
cmd_loop:
	in a, (FD_ST_PORT)
	rla							; RQM...
	jr nc, cmd_loop             ; ...low, keep waiting
    outi                        ; port(C) = (HL++), B++
    jr nz, cmd_loop

    ; Now read bytes from the 765 until we're done.

	ld hl, 0                   ; destination address
	; ld c, FD_DT               ; still in C from previously
read_loop:
	in a, (FD_ST_PORT)
	rla							; RQM...
	jr nc, read_loop      		; ...low, keep waiting
	rla							; DIO (ignore)
	rla							; EXM...
	jr nc, read_finished		; ...low, transfer complete
	ini							; (HL++) = port(C), B--
    jr read_loop
read_finished:

    ; We've finished a command, so nudge the FDC TC line to tell it
    ; we're done.

	ld a, 0x83
	out (FD_POWER_PORT), a
	dec a
	out (FD_POWER_PORT), a

    ; Read and then discard the status.

read_status_loop:
	in a, (FD_ST_PORT)
	rla 						; RQM...
	jr nc, read_status_loop 	; ...low, keep waiting 
	rla							; DIO...
    jr nc, finished             ; ...low, no more data
    in a, (FD_DT_PORT)
	jr read_status_loop

    ; We're done.

finished:
    ; Jump to the newly loaded code.
    jp 0

; The baked command to read a track.

.data.readtrack:
    .db 0xc2     ; 0: READ TRACK MULTITRACK
    .db 0        ; 1: physical head 0, drive 0
    .db 0        ; 2: track 0
    .db 0        ; 3: logical head 0
    .db 0        ; 4: unused
    .db 2        ; 5: bytes per sector: 512
    .db 9        ; 6: last sector for one track (*inclusive*)
    .db 27       ; 7: gap 3 length (27 is standard for 3.5" drives)
    .db 0        ; 8: sector length (unused)
.data.readtrack.end:
