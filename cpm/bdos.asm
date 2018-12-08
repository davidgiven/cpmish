;**************************************************************
;*
;*             C P / M   version   2 . 2
;*
;*   Reconstructed from memory image on February 27, 1981
;*
;*                by Clark A. Calkins
;*
;**************************************************************
;
.section BDOS
;
IOBYTE	EQU	3		;i/o definition byte.
TDRIVE	EQU	4		;current drive name and user number.
ENTRY	EQU	5		;entry point for the cp/m bdos.
TFCB	EQU	5CH		;default file control block.
TBUFF	EQU	80H		;i/o buffer and command line storage.
TBASE	EQU	100H		;transiant program storage area.
;
;   Set control character equates.
;
CNTRLC	EQU	3		;control-c
CNTRLE	EQU	05H		;control-e
BS	EQU	08H		;backspace
TAB	EQU	09H		;tab
LF	EQU	0AH		;line feed
FF	EQU	0CH		;form feed
CR	EQU	0DH		;carriage return
CNTRLP	EQU	10H		;control-p
CNTRLR	EQU	12H		;control-r
CNTRLS	EQU	13H		;control-s
CNTRLU	EQU	15H		;control-u
CNTRLX	EQU	18H		;control-x
CNTRLZ	EQU	1AH		;control-z (end-of-file mark)
DEL	EQU	7FH		;rubout

; BIOS entrypoints.

BIOS = FBASE + 0x0e00
BOOT:	EQU BIOS + 0*3
WBOOT:	EQU BIOS + 1*3
CONST:	EQU BIOS + 2*3
CONIN:	EQU BIOS + 3*3
CONOUT:	EQU BIOS + 4*3
LIST:	EQU BIOS + 5*3
PUNCH:	EQU BIOS + 6*3
READER:	EQU BIOS + 7*3
HOME:	EQU BIOS + 8*3
SELDSK:	EQU BIOS + 9*3
SETTRK:	EQU BIOS + 10*3
SETSEC:	EQU BIOS + 11*3
SETDMA:	EQU BIOS + 12*3
READ:	EQU BIOS + 13*3
WRITE:	EQU BIOS + 14*3
PRSTAT:	EQU BIOS + 15*3
SECTRN:	EQU BIOS + 16*3

;
;**************************************************************
;*
;*                    B D O S   E N T R Y
;*
;**************************************************************
;
FBASE:
PATTRN2: DB 0,22,0,0,0,0
	JP	FBASE1
;
;   Bdos error table.
;
BADSCTR:DEFW	ERROR1		;bad sector on read or write.
BADSLCT:DEFW	ERROR2		;bad disk select.
RODISK:	DEFW	ERROR3		;disk is read only.
ROFILE:	DEFW	ERROR4		;file is read only.
;
;   Entry into bdos. (DE) or (E) are the parameters passed. The
; function number desired is in register (C).
;
FBASE1:	EX	DE,HL		;save the (DE) parameters.
	LD	(PARAMS),HL
	EX	DE,HL
	LD	A,E		;and save register (E) in particular.
	LD	(EPARAM),A
	LD	HL,0
	LD	(STATUS),HL	;clear return status.
	ADD	HL,SP
	LD	(USRSTACK),HL	;save users stack pointer.
	LD	SP,STKAREA	;and set our own.
	XOR	A		;clear auto select storage space.
	LD	(AUTOFLAG),A
	LD	(AUTO),A
	LD	HL,GOBACK	;set return address.
	PUSH	HL
	LD	A,C		;get function number.
	CP	NFUNCTS		;valid function number?
	RET	NC
	LD	C,E		;keep single register function here.
	LD	HL,FUNCTNS	;now look thru the function table.
	LD	E,A
	LD	D,0		;(DE)=function number.
	ADD	HL,DE
	ADD	HL,DE		;(HL)=(start of table)+2*(function number).
	LD	E,(HL)
	INC	HL
	LD	D,(HL)		;now (DE)=address for this function.
	LD	HL,(PARAMS)	;retrieve parameters.
	EX	DE,HL		;now (DE) has the original parameters.
	JP	(HL)		;execute desired function.
;
;   BDOS function jump table.
;
NFUNCTS EQU	41		;number of functions in followin table.
;
FUNCTNS:DEFW	WBOOT,GETCON,OUTCON,GETRDR,PUNCH,LIST,DIRCIO,GETIOB
	DEFW	SETIOB,PRTSTR,RDBUFF,GETCSTS,GETVER,RSTDSK,SETDSK,OPENFIL
	DEFW	CLOSEFIL,GETFST,GETNXT,DELFILE,READSEQ,WRTSEQ,FCREATE
	DEFW	RENFILE,GETLOG,GETCRNT,PUTDMA,GETALOC,WRTPRTD,GETROV,SETATTR
	DEFW	GETPARM,GETUSER,RDRANDOM,WTRANDOM,FILESIZE,SETRAN,LOGOFF,RTN
	DEFW	RTN,WTSPECL
;
;   Bdos error message section.
;
ERROR1:	LD	HL,BADSEC	;bad sector message.
	CALL	PRTERR		;print it and get a 1 char responce.
	CP	CNTRLC		;re-boot request (control-c)?
	JP	Z,0		;yes.
	RET			;no, return to retry i/o function.
;
ERROR2:	LD	HL,BADSEL	;bad drive selected.
	JP	ERROR5
;
ERROR3:	LD	HL,DISKRO	;disk is read only.
	JP	ERROR5
;
ERROR4:	LD	HL,FILERO	;file is read only.
;
ERROR5:	CALL	PRTERR
	JP	0		;always reboot on these errors.
;
BDOSERR:DEFB	'Bdos Err On '
BDOSDRV:DEFB	' : $'
BADSEC:	DEFB	'Bad Sector$'
BADSEL:	DEFB	'Select$'
FILERO:	DEFB	'File '
DISKRO:	DEFB	'R/O$'
;
;   Print bdos error message.
;
PRTERR:	PUSH	HL		;save second message pointer.
	CALL	OUTCRLF		;send (cr)(lf).
	LD	A,(ACTIVE)	;get active drive.
	ADD	A,'A'		;make ascii.
	LD	(BDOSDRV),A	;and put in message.
	LD	BC,BDOSERR	;and print it.
	CALL	PRTMESG
	POP	BC		;print second message line now.
	CALL	PRTMESG
;
;   Get an input character. We will check our 1 character
; buffer first. This may be set by the console status routine.
;
GETCHAR:LD	HL,CHARBUF	;check character buffer.
	LD	A,(HL)		;anything present already?
	LD	(HL),0		;...either case clear it.
	OR	A
	RET	NZ		;yes, use it.
	JP	CONIN		;nope, go get a character responce.
;
;   Input and echo a character.
;
GETECHO:CALL	GETCHAR		;input a character.
	CALL	CHKCHAR		;carriage control?
	RET	C		;no, a regular control char so don't echo.
	PUSH	AF		;ok, save character now.
	LD	C,A
	CALL	OUTCON		;and echo it.
	POP	AF		;get character and return.
	RET	
;
;   Check character in (A). Set the zero flag on a carriage
; control character and the carry flag on any other control
; character.
;
CHKCHAR:CP	CR		;check for carriage return, line feed, backspace,
	RET	Z		;or a tab.
	CP	LF
	RET	Z
	CP	TAB
	RET	Z
	CP	BS
	RET	Z
	CP	' '		;other control char? Set carry flag.
	RET	
;
;   Check the console during output. Halt on a control-s, then
; reboot on a control-c. If anything else is ready, clear the
; zero flag and return (the calling routine may want to do
; something).
;
CKCONSOL: LD	A,(CHARBUF)	;check buffer.
	OR	A		;if anything, just return without checking.
	JP	NZ,CKCON2
	CALL	CONST		;nothing in buffer. Check console.
	AND	01H		;look at bit 0.
	RET	Z		;return if nothing.
	CALL	CONIN		;ok, get it.
	CP	CNTRLS		;if not control-s, return with zero cleared.
	JP	NZ,CKCON1
	CALL	CONIN		;halt processing until another char
	CP	CNTRLC		;is typed. Control-c?
	JP	Z,0		;yes, reboot now.
	XOR	A		;no, just pretend nothing was ever ready.
	RET	
CKCON1:	LD	(CHARBUF),A	;save character in buffer for later processing.
CKCON2:	LD	A,1		;set (A) to non zero to mean something is ready.
	RET	
;
;   Output (C) to the screen. If the printer flip-flop flag
; is set, we will send character to printer also. The console
; will be checked in the process.
;
OUTCHAR:LD	A,(OUTFLAG)	;check output flag.
	OR	A		;anything and we won't generate output.
	JP	NZ,OUTCHR1
	PUSH	BC
	CALL	CKCONSOL	;check console (we don't care whats there).
	POP	BC
	PUSH	BC
	CALL	CONOUT		;output (C) to the screen.
	POP	BC
	PUSH	BC
	LD	A,(PRTFLAG)	;check printer flip-flop flag.
	OR	A
	CALL	NZ,LIST		;print it also if non-zero.
	POP	BC
OUTCHR1:LD	A,C		;update cursors position.
	LD	HL,CURPOS
	CP	DEL		;rubouts don't do anything here.
	RET	Z
	INC	(HL)		;bump line pointer.
	CP	' '		;and return if a normal character.
	RET	NC
	DEC	(HL)		;restore and check for the start of the line.
	LD	A,(HL)
	OR	A
	RET	Z		;ingnore control characters at the start of the line.
	LD	A,C
	CP	BS		;is it a backspace?
	JP	NZ,OUTCHR2
	DEC	(HL)		;yes, backup pointer.
	RET	
OUTCHR2:CP	LF		;is it a line feed?
	RET	NZ		;ignore anything else.
	LD	(HL),0		;reset pointer to start of line.
	RET	
;
;   Output (A) to the screen. If it is a control character
; (other than carriage control), use ^x format.
;
SHOWIT:	LD	A,C
	CALL	CHKCHAR		;check character.
	JP	NC,OUTCON	;not a control, use normal output.
	PUSH	AF
	LD	C,'^'		;for a control character, preceed it with '^'.
	CALL	OUTCHAR
	POP	AF
	OR	'@'		;and then use the letter equivelant.
	LD	C,A
;
;   Function to output (C) to the console device and expand tabs
; if necessary.
;
OUTCON:	LD	A,C
	CP	TAB		;is it a tab?
	JP	NZ,OUTCHAR	;use regular output.
OUTCON1:LD	C,' '		;yes it is, use spaces instead.
	CALL	OUTCHAR
	LD	A,(CURPOS)	;go until the cursor is at a multiple of 8

	AND	07H		;position.
	JP	NZ,OUTCON1
	RET	
;
;   Echo a backspace character. Erase the prevoius character
; on the screen.
;
BACKUP:	CALL	BACKUP1		;backup the screen 1 place.
	LD	C,' '		;then blank that character.
	CALL	CONOUT
BACKUP1:LD	C,BS		;then back space once more.
	JP	CONOUT
;
;   Signal a deleted line. Print a '#' at the end and start
; over.
;
NEWLINE:LD	C,'#'
	CALL	OUTCHAR		;print this.
	CALL	OUTCRLF		;start new line.
NEWLN1:	LD	A,(CURPOS)	;move the cursor to the starting position.
	LD	HL,STARTING
	CP	(HL)
	RET	NC		;there yet?
	LD	C,' '
	CALL	OUTCHAR		;nope, keep going.
	JP	NEWLN1
;
;   Output a (cr) (lf) to the console device (screen).
;
OUTCRLF:LD	C,CR
	CALL	OUTCHAR
	LD	C,LF
	JP	OUTCHAR
;
;   Print message pointed to by (BC). It will end with a '$'.
;
PRTMESG:LD	A,(BC)		;check for terminating character.
	CP	'$'
	RET	Z
	INC	BC
	PUSH	BC		;otherwise, bump pointer and print it.
	LD	C,A
	CALL	OUTCON
	POP	BC
	JP	PRTMESG
;
;   Function to execute a buffered read.
;
RDBUFF:	LD	A,(CURPOS)	;use present location as starting one.
	LD	(STARTING),A
	LD	HL,(PARAMS)	;get the maximum buffer space.
	LD	C,(HL)
	INC	HL		;point to first available space.
	PUSH	HL		;and save.
	LD	B,0		;keep a character count.
RDBUF1:	PUSH	BC
	PUSH	HL
RDBUF2:	CALL	GETCHAR		;get the next input character.
	AND	7FH		;strip bit 7.
	POP	HL		;reset registers.
	POP	BC
	CP	CR		;en of the line?
	JP	Z,RDBUF17
	CP	LF
	JP	Z,RDBUF17
	CP	BS		;how about a backspace?
	JP	NZ,RDBUF3
	LD	A,B		;yes, but ignore at the beginning of the line.
	OR	A
	JP	Z,RDBUF1
	DEC	B		;ok, update counter.
	LD	A,(CURPOS)	;if we backspace to the start of the line,
	LD	(OUTFLAG),A	;treat as a cancel (control-x).
	JP	RDBUF10
RDBUF3:	CP	DEL		;user typed a rubout?
	JP	NZ,RDBUF4
	LD	A,B		;ignore at the start of the line.
	OR	A
	JP	Z,RDBUF1
	LD	A,(HL)		;ok, echo the prevoius character.
	DEC	B		;and reset pointers (counters).
	DEC	HL
	JP	RDBUF15
RDBUF4:	CP	CNTRLE		;physical end of line?
	JP	NZ,RDBUF5
	PUSH	BC		;yes, do it.
	PUSH	HL
	CALL	OUTCRLF
	XOR	A		;and update starting position.
	LD	(STARTING),A
	JP	RDBUF2
RDBUF5:	CP	CNTRLP		;control-p?
	JP	NZ,RDBUF6
	PUSH	HL		;yes, flip the print flag filp-flop byte.
	LD	HL,PRTFLAG
	LD	A,1		;PRTFLAG=1-PRTFLAG
	SUB	(HL)
	LD	(HL),A
	POP	HL
	JP	RDBUF1
RDBUF6:	CP	CNTRLX		;control-x (cancel)?
	JP	NZ,RDBUF8
	POP	HL
RDBUF7:	LD	A,(STARTING)	;yes, backup the cursor to here.
	LD	HL,CURPOS
	CP	(HL)
	JP	NC,RDBUFF	;done yet?
	DEC	(HL)		;no, decrement pointer and output back up one space.
	CALL	BACKUP
	JP	RDBUF7
RDBUF8:	CP	CNTRLU		;cntrol-u (cancel line)?
	JP	NZ,RDBUF9
	CALL	NEWLINE		;start a new line.
	POP	HL
	JP	RDBUFF
RDBUF9:	CP	CNTRLR		;control-r?
	JP	NZ,RDBUF14
RDBUF10:PUSH	BC		;yes, start a new line and retype the old one.
	CALL	NEWLINE
	POP	BC
	POP	HL
	PUSH	HL
	PUSH	BC
RDBUF11:LD	A,B		;done whole line yet?
	OR	A
	JP	Z,RDBUF12
	INC	HL		;nope, get next character.
	LD	C,(HL)
	DEC	B		;count it.
	PUSH	BC
	PUSH	HL
	CALL	SHOWIT		;and display it.
	POP	HL
	POP	BC
	JP	RDBUF11
RDBUF12:PUSH	HL		;done with line. If we were displaying
	LD	A,(OUTFLAG)	;then update cursor position.
	OR	A
	JP	Z,RDBUF2
	LD	HL,CURPOS	;because this line is shorter, we must
	SUB	(HL)		;back up the cursor (not the screen however)
	LD	(OUTFLAG),A	;some number of positions.
RDBUF13:CALL	BACKUP		;note that as long as (OUTFLAG) is non
	LD	HL,OUTFLAG	;zero, the screen will not be changed.
	DEC	(HL)
	JP	NZ,RDBUF13
	JP	RDBUF2		;now just get the next character.
;
;   Just a normal character, put this in our buffer and echo.
;
RDBUF14:INC	HL
	LD	(HL),A		;store character.
	INC	B		;and count it.
RDBUF15:PUSH	BC
	PUSH	HL
	LD	C,A		;echo it now.
	CALL	SHOWIT
	POP	HL
	POP	BC
	LD	A,(HL)		;was it an abort request?
	CP	CNTRLC		;control-c abort?
	LD	A,B
	JP	NZ,RDBUF16
	CP	1		;only if at start of line.
	JP	Z,0
RDBUF16:CP	C		;nope, have we filled the buffer?
	JP	C,RDBUF1
RDBUF17:POP	HL		;yes end the line and return.
	LD	(HL),B
	LD	C,CR
	JP	OUTCHAR		;output (cr) and return.
;
;   Function to get a character from the console device.
;
GETCON:	CALL	GETECHO		;get and echo.
	JP	SETSTAT		;save status and return.
;
;   Function to get a character from the tape reader device.
;
GETRDR:	CALL	READER		;get a character from reader, set status and return.
	JP	SETSTAT
;
;  Function to perform direct console i/o. If (C) contains (FF)
; then this is an input request. If (C) contains (FE) then
; this is a status request. Otherwise we are to output (C).
;
DIRCIO:	LD	A,C		;test for (FF).
	INC	A
	JP	Z,DIRC1
	INC	A		;test for (FE).
	JP	Z,CONST
	JP	CONOUT		;just output (C).
DIRC1:	CALL	CONST		;this is an input request.
	OR	A
	JP	Z,GOBACK1	;not ready? Just return (directly).
	CALL	CONIN		;yes, get character.
	JP	SETSTAT		;set status and return.
;
;   Function to return the i/o byte.
;
GETIOB:	LD	A,(IOBYTE)
	JP	SETSTAT
;
;   Function to set the i/o byte.
;
SETIOB:	LD	HL,IOBYTE
	LD	(HL),C
	RET	
;
;   Function to print the character string pointed to by (DE)
; on the console device. The string ends with a '$'.
;
PRTSTR:	EX	DE,HL
	LD	C,L
	LD	B,H		;now (BC) points to it.
	JP	PRTMESG
;
;   Function to interigate the console device.
;
GETCSTS:CALL	CKCONSOL
;
;   Get here to set the status and return to the cleanup
; section. Then back to the user.
;
SETSTAT:LD	(STATUS),A
RTN:	RET	
;
;   Set the status to 1 (read or write error code).
;
IOERR1:	LD	A,1
	JP	SETSTAT
;
OUTFLAG:DEFB	0		;output flag (non zero means no output).
STARTING: DEFB	2		;starting position for cursor.
CURPOS:	DEFB	0		;cursor position (0=start of line).
PRTFLAG:DEFB	0		;printer flag (control-p toggle). List if non zero.
CHARBUF:DEFB	0		;single input character buffer.
;
;   Stack area for BDOS calls.
;
USRSTACK: DEFW	0		;save users stack pointer here.
;
	DEFB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	DEFB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
STKAREA EQU	$		;end of stack area.
;
USERNO:	DEFB	0		;current user number.
ACTIVE:	DEFB	0		;currently active drive.
PARAMS:	DEFW	0		;save (DE) parameters here on entry.
STATUS:	DEFW	0		;status returned from bdos function.
;
;   Select error occured, jump to error routine.
;
SLCTERR:LD	HL,BADSLCT
;
;   Jump to (HL) indirectly.
;
JUMPHL:	LD	E,(HL)
	INC	HL
	LD	D,(HL)		;now (DE) contain the desired address.
	EX	DE,HL
	JP	(HL)
;
;   Block move. (DE) to (HL), (C) bytes total.
;
DE2HL:	INC	C		;is count down to zero?
DE2HL1:	DEC	C
	RET	Z		;yes, we are done.
	LD	A,(DE)		;no, move one more byte.
	LD	(HL),A
	INC	DE
	INC	HL
	JP	DE2HL1		;and repeat.
;
;   Select the desired drive.
;
SELECT:	LD	A,(ACTIVE)	;get active disk.
	LD	C,A
	CALL	SELDSK		;select it.
	LD	A,H		;valid drive?
	OR	L		;valid drive?
	RET	Z		;return if not.
;
;   Here, the BIOS returned the address of the parameter block
; in (HL). We will extract the necessary pointers and save them.
;
	LD	E,(HL)		;yes, get address of translation table into (DE).
	INC	HL
	LD	D,(HL)
	INC	HL
	LD	(SCRATCH1),HL	;save pointers to scratch areas.
	INC	HL
	INC	HL
	LD	(SCRATCH2),HL	;ditto.
	INC	HL
	INC	HL
	LD	(SCRATCH3),HL	;ditto.
	INC	HL
	INC	HL
	EX	DE,HL		;now save the translation table address.
	LD	(XLATE),HL
	LD	HL,DIRBUF	;put the next 8 bytes here.
	LD	C,8		;they consist of the directory buffer
	CALL	DE2HL		;pointer, parameter block pointer,
	LD	HL,(DISKPB)	;check and allocation vectors.
	EX	DE,HL
	LD	HL,SECTORS	;move parameter block into our ram.
	LD	C,15		;it is 15 bytes long.
	CALL	DE2HL
	LD	HL,(DSKSIZE)	;check disk size.
	LD	A,H		;more than 256 blocks on this?
	LD	HL,BIGDISK
	LD	(HL),0FFH	;set to samll.
	OR	A
	JP	Z,SELECT1
	LD	(HL),0		;wrong, set to large.
SELECT1:LD	A,0FFH		;clear the zero flag.
	OR	A
	RET	
;
;   Routine to home the disk track head and clear pointers.
;
HOMEDRV:CALL	HOME		;home the head.
	XOR	A
	LD	HL,(SCRATCH2)	;set our track pointer also.
	LD	(HL),A
	INC	HL
	LD	(HL),A
	LD	HL,(SCRATCH3)	;and our sector pointer.
	LD	(HL),A
	INC	HL
	LD	(HL),A
	RET	
;
;   Do the actual disk read and check the error return status.
;
DOREAD:	CALL	READ
	JP	IORET
;
;   Do the actual disk write and handle any bios error.
;
DOWRITE:CALL	WRITE
IORET:	OR	A
	RET	Z		;return unless an error occured.
	LD	HL,BADSCTR	;bad read/write on this sector.
	JP	JUMPHL
;
;   Routine to select the track and sector that the desired
; block number falls in.
;
TRKSEC:	LD	HL,(FILEPOS)	;get position of last accessed file
	LD	C,2		;in directory and compute sector #.
	CALL	SHIFTR		;sector #=file-position/4.
	LD	(BLKNMBR),HL	;save this as the block number of interest.
	LD	(CKSUMTBL),HL	;what's it doing here too?
;
;   if the sector number has already been set (BLKNMBR), enter
; at this point.
;
TRKSEC1:LD	HL,BLKNMBR
	LD	C,(HL)		;move sector number into (BC).
	INC	HL
	LD	B,(HL)
	LD	HL,(SCRATCH3)	;get current sector number and
	LD	E,(HL)		;move this into (DE).
	INC	HL
	LD	D,(HL)
	LD	HL,(SCRATCH2)	;get current track number.
	LD	A,(HL)		;and this into (HL).
	INC	HL
	LD	H,(HL)
	LD	L,A
TRKSEC2:LD	A,C		;is desired sector before current one?
	SUB	E
	LD	A,B
	SBC	A,D
	JP	NC,TRKSEC3
	PUSH	HL		;yes, decrement sectors by one track.
	LD	HL,(SECTORS)	;get sectors per track.
	LD	A,E
	SUB	L
	LD	E,A
	LD	A,D
	SBC	A,H
	LD	D,A		;now we have backed up one full track.
	POP	HL
	DEC	HL		;adjust track counter.
	JP	TRKSEC2
TRKSEC3:PUSH	HL		;desired sector is after current one.
	LD	HL,(SECTORS)	;get sectors per track.
	ADD	HL,DE		;bump sector pointer to next track.
	JP	C,TRKSEC4
	LD	A,C		;is desired sector now before current one?
	SUB	L
	LD	A,B
	SBC	A,H
	JP	C,TRKSEC4
	EX	DE,HL		;not yes, increment track counter
	POP	HL		;and continue until it is.
	INC	HL
	JP	TRKSEC3
;
;   here we have determined the track number that contains the
; desired sector.
;
TRKSEC4:POP	HL		;get track number (HL).
	PUSH	BC
	PUSH	DE
	PUSH	HL
	EX	DE,HL
	LD	HL,(OFFSET)	;adjust for first track offset.
	ADD	HL,DE
	LD	B,H
	LD	C,L
	CALL	SETTRK		;select this track.
	POP	DE		;reset current track pointer.
	LD	HL,(SCRATCH2)
	LD	(HL),E
	INC	HL
	LD	(HL),D
	POP	DE
	LD	HL,(SCRATCH3)	;reset the first sector on this track.
	LD	(HL),E
	INC	HL
	LD	(HL),D
	POP	BC
	LD	A,C		;now subtract the desired one.
	SUB	E		;to make it relative (1-# sectors/track).
	LD	C,A
	LD	A,B
	SBC	A,D
	LD	B,A
	LD	HL,(XLATE)	;translate this sector according to this table.
	EX	DE,HL
	CALL	SECTRN		;let the bios translate it.
	LD	C,L
	LD	B,H
	JP	SETSEC		;and select it.
;
;   Compute block number from record number (SAVNREC) and
; extent number (SAVEXT).
;
GETBLOCK: LD	HL,BLKSHFT	;get logical to physical conversion.
	LD	C,(HL)		;note that this is base 2 log of ratio.
	LD	A,(SAVNREC)	;get record number.
GETBLK1:OR	A		;compute (A)=(A)/2^BLKSHFT.
	RRA	
	DEC	C
	JP	NZ,GETBLK1
	LD	B,A		;save result in (B).
	LD	A,8
	SUB	(HL)
	LD	C,A		;compute (C)=8-BLKSHFT.
	LD	A,(SAVEXT)
GETBLK2:DEC	C		;compute (A)=SAVEXT*2^(8-BLKSHFT).
	JP	Z,GETBLK3
	OR	A
	RLA	
	JP	GETBLK2
GETBLK3:ADD	A,B
	RET	
;
;   Routine to extract the (BC) block byte from the fcb pointed
; to by (PARAMS). If this is a big-disk, then these are 16 bit
; block numbers, else they are 8 bit numbers.
; Number is returned in (HL).
;
EXTBLK:	LD	HL,(PARAMS)	;get fcb address.
	LD	DE,16		;block numbers start 16 bytes into fcb.
	ADD	HL,DE
	ADD	HL,BC
	LD	A,(BIGDISK)	;are we using a big-disk?
	OR	A
	JP	Z,EXTBLK1
	LD	L,(HL)		;no, extract an 8 bit number from the fcb.
	LD	H,0
	RET	
EXTBLK1:ADD	HL,BC		;yes, extract a 16 bit number.
	LD	E,(HL)
	INC	HL
	LD	D,(HL)
	EX	DE,HL		;return in (HL).
	RET	
;
;   Compute block number.
;
COMBLK:	CALL	GETBLOCK
	LD	C,A
	LD	B,0
	CALL	EXTBLK
	LD	(BLKNMBR),HL
	RET	
;
;   Check for a zero block number (unused).
;
CHKBLK:	LD	HL,(BLKNMBR)
	LD	A,L		;is it zero?
	OR	H
	RET	
;
;   Adjust physical block (BLKNMBR) and convert to logical
; sector (LOGSECT). This is the starting sector of this block.
; The actual sector of interest is then added to this and the
; resulting sector number is stored back in (BLKNMBR). This
; will still have to be adjusted for the track number.
;
LOGICAL:LD	A,(BLKSHFT)	;get log2(physical/logical sectors).
	LD	HL,(BLKNMBR)	;get physical sector desired.
LOGICL1:ADD	HL,HL		;compute logical sector number.
	DEC	A		;note logical sectors are 128 bytes long.
	JP	NZ,LOGICL1
	LD	(LOGSECT),HL	;save logical sector.
	LD	A,(BLKMASK)	;get block mask.
	LD	C,A
	LD	A,(SAVNREC)	;get next sector to access.
	AND	C		;extract the relative position within physical block.
	OR	L		;and add it too logical sector.
	LD	L,A
	LD	(BLKNMBR),HL	;and store.
	RET	
;
;   Set (HL) to point to extent byte in fcb.
;
SETEXT:	LD	HL,(PARAMS)
	LD	DE,12		;it is the twelth byte.
	ADD	HL,DE
	RET	
;
;   Set (HL) to point to record count byte in fcb and (DE) to
; next record number byte.
;
SETHLDE:LD	HL,(PARAMS)
	LD	DE,15		;record count byte (#15).
	ADD	HL,DE
	EX	DE,HL
	LD	HL,17		;next record number (#32).
	ADD	HL,DE
	RET	
;
;   Save current file data from fcb.
;
STRDATA:CALL	SETHLDE
	LD	A,(HL)		;get and store record count byte.
	LD	(SAVNREC),A
	EX	DE,HL
	LD	A,(HL)		;get and store next record number byte.
	LD	(SAVNXT),A
	CALL	SETEXT		;point to extent byte.
	LD	A,(EXTMASK)	;get extent mask.
	AND	(HL)
	LD	(SAVEXT),A	;and save extent here.
	RET	
;
;   Set the next record to access. If (MODE) is set to 2, then
; the last record byte (SAVNREC) has the correct number to access.
; For sequential access, (MODE) will be equal to 1.
;
SETNREC:CALL	SETHLDE
	LD	A,(MODE)	;get sequential flag (=1).
	CP	2		;a 2 indicates that no adder is needed.
	JP	NZ,STNREC1
	XOR	A		;clear adder (random access?).
STNREC1:LD	C,A
	LD	A,(SAVNREC)	;get last record number.
	ADD	A,C		;increment record count.
	LD	(HL),A		;and set fcb's next record byte.
	EX	DE,HL
	LD	A,(SAVNXT)	;get next record byte from storage.
	LD	(HL),A		;and put this into fcb as number of records used.
	RET	
;
;   Shift (HL) right (C) bits.
;
SHIFTR:	INC	C
SHIFTR1:DEC	C
	RET	Z
	LD	A,H
	OR	A
	RRA	
	LD	H,A
	LD	A,L
	RRA	
	LD	L,A
	JP	SHIFTR1
;
;   Compute the check-sum for the directory buffer. Return
; integer sum in (A).
;
CHECKSUM: LD	C,128		;length of buffer.
	LD	HL,(DIRBUF)	;get its location.
	XOR	A		;clear summation byte.
CHKSUM1:ADD	A,(HL) ; find compute sum ignoring carries.
	INC	HL
	DEC	C
	JP	NZ,CHKSUM1
	RET	
;
;   Shift (HL) left (C) bits.
;
SHIFTL:	INC	C
SHIFTL1:DEC	C
	RET	Z
	ADD	HL,HL		;shift left 1 bit.
	JP	SHIFTL1
;
;   Routine to set a bit in a 16 bit value contained in (BC).
; The bit set depends on the current drive selection.
;
SETBIT:	PUSH	BC		;save 16 bit word.
	LD	A,(ACTIVE)	;get active drive.
	LD	C,A
	LD	HL,1
	CALL	SHIFTL		;shift bit 0 into place.
	POP	BC		;now 'or' this with the original word.
	LD	A,C
	OR	L
	LD	L,A		;low byte done, do high byte.
	LD	A,B
	OR	H
	LD	H,A
	RET	
;
;   Extract the write protect status bit for the current drive.
; The result is returned in (A), bit 0.
;
GETWPRT:LD	HL,(WRTPRT)	;get status bytes.
	LD	A,(ACTIVE)	;which drive is current?
	LD	C,A
	CALL	SHIFTR		;shift status such that bit 0 is the
	LD	A,L		;one of interest for this drive.
	AND	01H		;and isolate it.
	RET	
;
;   Function to write protect the current disk.
;
WRTPRTD:LD	HL,WRTPRT	;point to status word.
	LD	C,(HL)		;set (BC) equal to the status.
	INC	HL
	LD	B,(HL)
	CALL	SETBIT		;and set this bit according to current drive.
	LD	(WRTPRT),HL	;then save.
	LD	HL,(DIRSIZE)	;now save directory size limit.
	INC	HL		;remember the last one.
	EX	DE,HL
	LD	HL,(SCRATCH1)	;and store it here.
	LD	(HL),E		;put low byte.
	INC	HL
	LD	(HL),D		;then high byte.
	RET	
;
;   Check for a read only file.
;
CHKROFL:CALL	FCB2HL		;set (HL) to file entry in directory buffer.
CKROF1:	LD	DE,9		;look at bit 7 of the ninth byte.
	ADD	HL,DE
	LD	A,(HL)
	RLA	
	RET	NC		;return if ok.
	LD	HL,ROFILE	;else, print error message and terminate.
	JP	JUMPHL
;
;   Check the write protect status of the active disk.
;
CHKWPRT:CALL	GETWPRT
	RET	Z		;return if ok.
	LD	HL,RODISK	;else print message and terminate.
	JP	JUMPHL
;
;   Routine to set (HL) pointing to the proper entry in the
; directory buffer.
;
FCB2HL:	LD	HL,(DIRBUF)	;get address of buffer.
	LD	A,(FCBPOS)	;relative position of file.
;
;   Routine to add (A) to (HL).
;
ADDA2HL:ADD	A,L
	LD	L,A
	RET	NC
	INC	H		;take care of any carry.
	RET	
;
;   Routine to get the 's2' byte from the fcb supplied in
; the initial parameter specification.
;
GETS2:	LD	HL,(PARAMS)	;get address of fcb.
	LD	DE,14		;relative position of 's2'.
	ADD	HL,DE
	LD	A,(HL)		;extract this byte.
	RET	
;
;   Clear the 's2' byte in the fcb.
;
CLEARS2:CALL	GETS2		;this sets (HL) pointing to it.
	LD	(HL),0		;now clear it.
	RET	
;
;   Set bit 7 in the 's2' byte of the fcb.
;
SETS2B7:CALL	GETS2		;get the byte.
	OR	80H		;and set bit 7.
	LD	(HL),A		;then store.
	RET	
;
;   Compare (FILEPOS) with (SCRATCH1) and set flags based on
; the difference. This checks to see if there are more file
; names in the directory. We are at (FILEPOS) and there are
; (SCRATCH1) of them to check.
;
MOREFLS:LD	HL,(FILEPOS)	;we are here.
	EX	DE,HL
	LD	HL,(SCRATCH1)	;and don't go past here.
	LD	A,E		;compute difference but don't keep.
	SUB	(HL)
	INC	HL
	LD	A,D
	SBC	A,(HL) ; set carry if no more names.
	RET	
;
;   Call this routine to prevent (SCRATCH1) from being greater
; than (FILEPOS).
;
CHKNMBR:CALL	MOREFLS		;SCRATCH1 too big?
	RET	C
	INC	DE		;yes, reset it to (FILEPOS).
	LD	(HL),D
	DEC	HL
	LD	(HL),E
	RET	
;
;   Compute (HL)=(DE)-(HL)
;
SUBHL:	LD	A,E		;compute difference.
	SUB	L
	LD	L,A		;store low byte.
	LD	A,D
	SBC	A,H
	LD	H,A		;and then high byte.
	RET	
;
;   Set the directory checksum byte.
;
SETDIR:	LD	C,0FFH
;
;   Routine to set or compare the directory checksum byte. If
; (C)=0ffh, then this will set the checksum byte. Else the byte
; will be checked. If the check fails (the disk has been changed),
; then this disk will be write protected.
;
CHECKDIR: LD	HL,(CKSUMTBL)
	EX	DE,HL
	LD	HL,(ALLOC1)
	CALL	SUBHL
	RET	NC		;ok if (CKSUMTBL) > (ALLOC1), so return.
	PUSH	BC
	CALL	CHECKSUM	;else compute checksum.
	LD	HL,(CHKVECT)	;get address of checksum table.
	EX	DE,HL
	LD	HL,(CKSUMTBL)
	ADD	HL,DE		;set (HL) to point to byte for this drive.
	POP	BC
	INC	C		;set or check ?
	JP	Z,CHKDIR1
	CP	(HL)		;check them.
	RET	Z		;return if they are the same.
	CALL	MOREFLS		;not the same, do we care?
	RET	NC
	CALL	WRTPRTD		;yes, mark this as write protected.
	RET	
CHKDIR1:LD	(HL),A		;just set the byte.
	RET	
;
;   Do a write to the directory of the current disk.
;
DIRWRITE: CALL	SETDIR		;set checksum byte.
	CALL	DIRDMA		;set directory dma address.
	LD	C,1		;tell the bios to actually write.
	CALL	DOWRITE		;then do the write.
	JP	DEFDMA
;
;   Read from the directory.
;
DIRREAD:CALL	DIRDMA		;set the directory dma address.
	CALL	DOREAD		;and read it.
;
;   Routine to set the dma address to the users choice.
;
DEFDMA:	LD	HL,USERDMA	;reset the default dma address and return.
	JP	DIRDMA1
;
;   Routine to set the dma address for directory work.
;
DIRDMA:	LD	HL,DIRBUF
;
;   Set the dma address. On entry, (HL) points to
; word containing the desired dma address.
;
DIRDMA1:LD	C,(HL)
	INC	HL
	LD	B,(HL)		;setup (BC) and go to the bios to set it.
	JP	SETDMA
;
;   Move the directory buffer into user's dma space.
;
MOVEDIR:LD	HL,(DIRBUF)	;buffer is located here, and
	EX	DE,HL
	LD	HL,(USERDMA)	; put it here.
	LD	C,128		;this is its length.
	JP	DE2HL		;move it now and return.
;
;   Check (FILEPOS) and set the zero flag if it equals 0ffffh.
;
CKFILPOS: LD	HL,FILEPOS
	LD	A,(HL)
	INC	HL
	CP	(HL)		;are both bytes the same?
	RET	NZ
	INC	A		;yes, but are they each 0ffh?
	RET	
;
;   Set location (FILEPOS) to 0ffffh.
;
STFILPOS: LD	HL,0FFFFH
	LD	(FILEPOS),HL
	RET	
;
;   Move on to the next file position within the current
; directory buffer. If no more exist, set pointer to 0ffffh
; and the calling routine will check for this. Enter with (C)
; equal to 0ffh to cause the checksum byte to be set, else we
; will check this disk and set write protect if checksums are
; not the same (applies only if another directory sector must
; be read).
;
NXENTRY:LD	HL,(DIRSIZE)	;get directory entry size limit.
	EX	DE,HL
	LD	HL,(FILEPOS)	;get current count.
	INC	HL		;go on to the next one.
	LD	(FILEPOS),HL
	CALL	SUBHL		;(HL)=(DIRSIZE)-(FILEPOS)
	JP	NC,NXENT1	;is there more room left?
	JP	STFILPOS	;no. Set this flag and return.
NXENT1:	LD	A,(FILEPOS)	;get file position within directory.
	AND	03H		;only look within this sector (only 4 entries fit).
	LD	B,5		;convert to relative position (32 bytes each).
NXENT2:	ADD	A,A		;note that this is not efficient code.
	DEC	B		;5 'ADD A's would be better.
	JP	NZ,NXENT2
	LD	(FCBPOS),A	;save it as position of fcb.
	OR	A
	RET	NZ		;return if we are within buffer.
	PUSH	BC
	CALL	TRKSEC		;we need the next directory sector.
	CALL	DIRREAD
	POP	BC
	JP	CHECKDIR
;
;   Routine to to get a bit from the disk space allocation
; map. It is returned in (A), bit position 0. On entry to here,
; set (BC) to the block number on the disk to check.
; On return, (D) will contain the original bit position for
; this block number and (HL) will point to the address for it.
;
CKBITMAP: LD	A,C		;determine bit number of interest.
	AND	07H		;compute (D)=(E)=(C and 7)+1.
	INC	A
	LD	E,A		;save particular bit number.
	LD	D,A
;
;   compute (BC)=(BC)/8.
;
	LD	A,C
	RRCA			;now shift right 3 bits.
	RRCA	
	RRCA	
	AND	1FH		;and clear bits 7,6,5.
	LD	C,A
	LD	A,B
	ADD	A,A		;now shift (B) into bits 7,6,5.
	ADD	A,A
	ADD	A,A
	ADD	A,A
	ADD	A,A
	OR	C		;and add in (C).
	LD	C,A		;ok, (C) ha been completed.
	LD	A,B		;is there a better way of doing this?
	RRCA	
	RRCA	
	RRCA	
	AND	1FH
	LD	B,A		;and now (B) is completed.
;
;   use this as an offset into the disk space allocation
; table.
;
	LD	HL,(ALOCVECT)
	ADD	HL,BC
	LD	A,(HL)		;now get correct byte.
CKBMAP1:RLCA			;get correct bit into position 0.
	DEC	E
	JP	NZ,CKBMAP1
	RET	
;
;   Set or clear the bit map such that block number (BC) will be marked
; as used. On entry, if (E)=0 then this bit will be cleared, if it equals
; 1 then it will be set (don't use anyother values).
;
STBITMAP: PUSH	DE
	CALL	CKBITMAP	;get the byte of interest.
	AND	0FEH		;clear the affected bit.
	POP	BC
	OR	C		;and now set it acording to (C).
;
;  entry to restore the original bit position and then store
; in table. (A) contains the value, (D) contains the bit
; position (1-8), and (HL) points to the address within the
; space allocation table for this byte.
;
STBMAP1:RRCA			;restore original bit position.
	DEC	D
	JP	NZ,STBMAP1
	LD	(HL),A		;and stor byte in table.
	RET	
;
;   Set/clear space used bits in allocation map for this file.
; On entry, (C)=1 to set the map and (C)=0 to clear it.
;
SETFILE:CALL	FCB2HL		;get address of fcb
	LD	DE,16
	ADD	HL,DE		;get to block number bytes.
	PUSH	BC
	LD	C,17		;check all 17 bytes (max) of table.
SETFL1:	POP	DE
	DEC	C		;done all bytes yet?
	RET	Z
	PUSH	DE
	LD	A,(BIGDISK)	;check disk size for 16 bit block numbers.
	OR	A
	JP	Z,SETFL2
	PUSH	BC		;only 8 bit numbers. set (BC) to this one.
	PUSH	HL
	LD	C,(HL)		;get low byte from table, always
	LD	B,0		;set high byte to zero.
	JP	SETFL3
SETFL2:	DEC	C		;for 16 bit block numbers, adjust counter.
	PUSH	BC
	LD	C,(HL)		;now get both the low and high bytes.
	INC	HL
	LD	B,(HL)
	PUSH	HL
SETFL3:	LD	A,C		;block used?
	OR	B
	JP	Z,SETFL4
	LD	HL,(DSKSIZE)	;is this block number within the
	LD	A,L		;space on the disk?
	SUB	C
	LD	A,H
	SBC	A,B
	CALL	NC,STBITMAP	;yes, set the proper bit.
SETFL4:	POP	HL		;point to next block number in fcb.
	INC	HL
	POP	BC
	JP	SETFL1
;
;   Construct the space used allocation bit map for the active
; drive. If a file name starts with '$' and it is under the
; current user number, then (STATUS) is set to minus 1. Otherwise
; it is not set at all.
;
BITMAP:	LD	HL,(DSKSIZE)	;compute size of allocation table.
	LD	C,3
	CALL	SHIFTR		;(HL)=(HL)/8.
	INC	HL		;at lease 1 byte.
	LD	B,H
	LD	C,L		;set (BC) to the allocation table length.
;
;   Initialize the bitmap for this drive. Right now, the first
; two bytes are specified by the disk parameter block. However
; a patch could be entered here if it were necessary to setup
; this table in a special mannor. For example, the bios could
; determine locations of 'bad blocks' and set them as already
; 'used' in the map.
;
	LD	HL,(ALOCVECT)	;now zero out the table now.
BITMAP1:LD	(HL),0
	INC	HL
	DEC	BC
	LD	A,B
	OR	C
	JP	NZ,BITMAP1
	LD	HL,(ALLOC0)	;get initial space used by directory.
	EX	DE,HL
	LD	HL,(ALOCVECT)	;and put this into map.
	LD	(HL),E
	INC	HL
	LD	(HL),D
;
;   End of initialization portion.
;
	CALL	HOMEDRV		;now home the drive.
	LD	HL,(SCRATCH1)
	LD	(HL),3		;force next directory request to read
	INC	HL		;in a sector.
	LD	(HL),0
	CALL	STFILPOS	;clear initial file position also.
BITMAP2:LD	C,0FFH		;read next file name in directory
	CALL	NXENTRY		;and set checksum byte.
	CALL	CKFILPOS	;is there another file?
	RET	Z
	CALL	FCB2HL		;yes, get its address.
	LD	A,0E5H
	CP	(HL)		;empty file entry?
	JP	Z,BITMAP2
	LD	A,(USERNO)	;no, correct user number?
	CP	(HL)
	JP	NZ,BITMAP3
	INC	HL
	LD	A,(HL)		;yes, does name start with a '$'?
	SUB	'$'
	JP	NZ,BITMAP3
	DEC	A		;yes, set atatus to minus one.
	LD	(STATUS),A
BITMAP3:LD	C,1		;now set this file's space as used in bit map.
	CALL	SETFILE
	CALL	CHKNMBR		;keep (SCRATCH1) in bounds.
	JP	BITMAP2
;
;   Set the status (STATUS) and return.
;
STSTATUS: LD	A,(FNDSTAT)
	JP	SETSTAT
;
;   Check extents in (A) and (C). Set the zero flag if they
; are the same. The number of 16k chunks of disk space that
; the directory extent covers is expressad is (EXTMASK+1).
; No registers are modified.
;
SAMEXT:	PUSH	BC
	PUSH	AF
	LD	A,(EXTMASK)	;get extent mask and use it to
	CPL			;to compare both extent numbers.
	LD	B,A		;save resulting mask here.
	LD	A,C		;mask first extent and save in (C).
	AND	B
	LD	C,A
	POP	AF		;now mask second extent and compare
	AND	B		;with the first one.
	SUB	C
	AND	1FH		;(* only check buts 0-4 *)
	POP	BC		;the zero flag is set if they are the same.
	RET			;restore (BC) and return.
;
;   Search for the first occurence of a file name. On entry,
; register (C) should contain the number of bytes of the fcb
; that must match.
;
FINDFST:LD	A,0FFH
	LD	(FNDSTAT),A
	LD	HL,COUNTER	;save character count.
	LD	(HL),C
	LD	HL,(PARAMS)	;get filename to match.
	LD	(SAVEFCB),HL	;and save.
	CALL	STFILPOS	;clear initial file position (set to 0ffffh).
	CALL	HOMEDRV		;home the drive.
;
;   Entry to locate the next occurence of a filename within the
; directory. The disk is not expected to have been changed. If
; it was, then it will be write protected.
;
FINDNXT:LD	C,0		;write protect the disk if changed.
	CALL	NXENTRY		;get next filename entry in directory.
	CALL	CKFILPOS	;is file position = 0ffffh?
	JP	Z,FNDNXT6	;yes, exit now then.
	LD	HL,(SAVEFCB)	;set (DE) pointing to filename to match.
	EX	DE,HL
	LD	A,(DE)
	CP	0E5H		;empty directory entry?
	JP	Z,FNDNXT1	;(* are we trying to reserect erased entries? *)
	PUSH	DE
	CALL	MOREFLS		;more files in directory?
	POP	DE
	JP	NC,FNDNXT6	;no more. Exit now.
FNDNXT1:CALL	FCB2HL		;get address of this fcb in directory.
	LD	A,(COUNTER)	;get number of bytes (characters) to check.
	LD	C,A
	LD	B,0		;initialize byte position counter.
FNDNXT2:LD	A,C		;are we done with the compare?
	OR	A
	JP	Z,FNDNXT5
	LD	A,(DE)		;no, check next byte.
	CP	'?'		;don't care about this character?
	JP	Z,FNDNXT4
	LD	A,B		;get bytes position in fcb.
	CP	13		;don't care about the thirteenth byte either.
	JP	Z,FNDNXT4
	CP	12		;extent byte?
	LD	A,(DE)
	JP	Z,FNDNXT3
	SUB	(HL)		;otherwise compare characters.
	AND	7FH
	JP	NZ,FINDNXT	;not the same, check next entry.
	JP	FNDNXT4		;so far so good, keep checking.
FNDNXT3:PUSH	BC		;check the extent byte here.
	LD	C,(HL)
	CALL	SAMEXT
	POP	BC
	JP	NZ,FINDNXT	;not the same, look some more.
;
;   So far the names compare. Bump pointers to the next byte
; and continue until all (C) characters have been checked.
;
FNDNXT4:INC	DE		;bump pointers.
	INC	HL
	INC	B
	DEC	C		;adjust character counter.
	JP	FNDNXT2
FNDNXT5:LD	A,(FILEPOS)	;return the position of this entry.
	AND	03H
	LD	(STATUS),A
	LD	HL,FNDSTAT
	LD	A,(HL)
	RLA	
	RET	NC
	XOR	A
	LD	(HL),A
	RET	
;
;   Filename was not found. Set appropriate status.
;
FNDNXT6:CALL	STFILPOS	;set (FILEPOS) to 0ffffh.
	LD	A,0FFH		;say not located.
	JP	SETSTAT
;
;   Erase files from the directory. Only the first byte of the
; fcb will be affected. It is set to (E5).
;
ERAFILE:CALL	CHKWPRT		;is disk write protected?
	LD	C,12		;only compare file names.
	CALL	FINDFST		;get first file name.
ERAFIL1:CALL	CKFILPOS	;any found?
	RET	Z		;nope, we must be done.
	CALL	CHKROFL		;is file read only?
	CALL	FCB2HL		;nope, get address of fcb and
	LD	(HL),0E5H	;set first byte to 'empty'.
	LD	C,0		;clear the space from the bit map.
	CALL	SETFILE
	CALL	DIRWRITE	;now write the directory sector back out.
	CALL	FINDNXT		;find the next file name.
	JP	ERAFIL1		;and repeat process.
;
;   Look through the space allocation map (bit map) for the
; next available block. Start searching at block number (BC-1).
; The search procedure is to look for an empty block that is
; before the starting block. If not empty, look at a later
; block number. In this way, we return the closest empty block
; on either side of the 'target' block number. This will speed
; access on random devices. For serial devices, this should be
; changed to look in the forward direction first and then start
; at the front and search some more.
;
;   On return, (DE)= block number that is empty and (HL) =0
; if no empry block was found.
;
FNDSPACE: LD	D,B		;set (DE) as the block that is checked.
	LD	E,C
;
;   Look before target block. Registers (BC) are used as the lower
; pointer and (DE) as the upper pointer.
;
FNDSPA1:LD	A,C		;is block 0 specified?
	OR	B
	JP	Z,FNDSPA2
	DEC	BC		;nope, check previous block.
	PUSH	DE
	PUSH	BC
	CALL	CKBITMAP
	RRA			;is this block empty?
	JP	NC,FNDSPA3	;yes. use this.
;
;   Note that the above logic gets the first block that it finds
; that is empty. Thus a file could be written 'backward' making
; it very slow to access. This could be changed to look for the
; first empty block and then continue until the start of this
; empty space is located and then used that starting block.
; This should help speed up access to some files especially on
; a well used disk with lots of fairly small 'holes'.
;
	POP	BC		;nope, check some more.
	POP	DE
;
;   Now look after target block.
;
FNDSPA2:LD	HL,(DSKSIZE)	;is block (DE) within disk limits?
	LD	A,E
	SUB	L
	LD	A,D
	SBC	A,H
	JP	NC,FNDSPA4
	INC	DE		;yes, move on to next one.
	PUSH	BC
	PUSH	DE
	LD	B,D
	LD	C,E
	CALL	CKBITMAP	;check it.
	RRA			;empty?
	JP	NC,FNDSPA3
	POP	DE		;nope, continue searching.
	POP	BC
	JP	FNDSPA1
;
;   Empty block found. Set it as used and return with (HL)
; pointing to it (true?).
;
FNDSPA3:RLA			;reset byte.
	INC	A		;and set bit 0.
	CALL	STBMAP1		;update bit map.
	POP	HL		;set return registers.
	POP	DE
	RET	
;
;   Free block was not found. If (BC) is not zero, then we have
; not checked all of the disk space.
;
FNDSPA4:LD	A,C
	OR	B
	JP	NZ,FNDSPA1
	LD	HL,0		;set 'not found' status.
	RET	
;
;   Move a complete fcb entry into the directory and write it.
;
FCBSET:	LD	C,0
	LD	E,32		;length of each entry.
;
;   Move (E) bytes from the fcb pointed to by (PARAMS) into
; fcb in directory starting at relative byte (C). This updated
; directory buffer is then written to the disk.
;
UPDATE:	PUSH	DE
	LD	B,0		;set (BC) to relative byte position.
	LD	HL,(PARAMS)	;get address of fcb.
	ADD	HL,BC		;compute starting byte.
	EX	DE,HL
	CALL	FCB2HL		;get address of fcb to update in directory.
	POP	BC		;set (C) to number of bytes to change.
	CALL	DE2HL
UPDATE1:CALL	TRKSEC		;determine the track and sector affected.
	JP	DIRWRITE	;then write this sector out.
;
;   Routine to change the name of all files on the disk with a
; specified name. The fcb contains the current name as the
; first 12 characters and the new name 16 bytes into the fcb.
;
CHGNAMES: CALL	CHKWPRT		;check for a write protected disk.
	LD	C,12		;match first 12 bytes of fcb only.
	CALL	FINDFST		;get first name.
	LD	HL,(PARAMS)	;get address of fcb.
	LD	A,(HL)		;get user number.
	LD	DE,16		;move over to desired name.
	ADD	HL,DE
	LD	(HL),A		;keep same user number.
CHGNAM1:CALL	CKFILPOS	;any matching file found?
	RET	Z		;no, we must be done.
	CALL	CHKROFL		;check for read only file.
	LD	C,16		;start 16 bytes into fcb.
	LD	E,12		;and update the first 12 bytes of directory.
	CALL	UPDATE
	CALL	FINDNXT		;get te next file name.
	JP	CHGNAM1		;and continue.
;
;   Update a files attributes. The procedure is to search for
; every file with the same name as shown in fcb (ignoring bit 7)
; and then to update it (which includes bit 7). No other changes
; are made.
;
SAVEATTR: LD	C,12		;match first 12 bytes.
	CALL	FINDFST		;look for first filename.
SAVATR1:CALL	CKFILPOS	;was one found?
	RET	Z		;nope, we must be done.
	LD	C,0		;yes, update the first 12 bytes now.
	LD	E,12
	CALL	UPDATE		;update filename and write directory.
	CALL	FINDNXT		;and get the next file.
	JP	SAVATR1		;then continue until done.
;
;  Open a file (name specified in fcb).
;
OPENIT:	LD	C,15		;compare the first 15 bytes.
	CALL	FINDFST		;get the first one in directory.
	CALL	CKFILPOS	;any at all?
	RET	Z
OPENIT1:CALL	SETEXT		;point to extent byte within users fcb.
	LD	A,(HL)		;and get it.
	PUSH	AF		;save it and address.
	PUSH	HL
	CALL	FCB2HL		;point to fcb in directory.
	EX	DE,HL
	LD	HL,(PARAMS)	;this is the users copy.
	LD	C,32		;move it into users space.
	PUSH	DE
	CALL	DE2HL
	CALL	SETS2B7		;set bit 7 in 's2' byte (unmodified).
	POP	DE		;now get the extent byte from this fcb.
	LD	HL,12
	ADD	HL,DE
	LD	C,(HL)		;into (C).
	LD	HL,15		;now get the record count byte into (B).
	ADD	HL,DE
	LD	B,(HL)
	POP	HL		;keep the same extent as the user had originally.
	POP	AF
	LD	(HL),A
	LD	A,C		;is it the same as in the directory fcb?
	CP	(HL)
	LD	A,B		;if yes, then use the same record count.
	JP	Z,OPENIT2
	LD	A,0		;if the user specified an extent greater than
	JP	C,OPENIT2	;the one in the directory, then set record count to 0.
	LD	A,128		;otherwise set to maximum.
OPENIT2:LD	HL,(PARAMS)	;set record count in users fcb to (A).
	LD	DE,15
	ADD	HL,DE		;compute relative position.
	LD	(HL),A		;and set the record count.
	RET	
;
;   Move two bytes from (DE) to (HL) if (and only if) (HL)
; point to a zero value (16 bit).
;   Return with zero flag set it (DE) was moved. Registers (DE)
; and (HL) are not changed. However (A) is.
;
MOVEWORD: LD	A,(HL)		;check for a zero word.
	INC	HL
	OR	(HL)		;both bytes zero?
	DEC	HL
	RET	NZ		;nope, just return.
	LD	A,(DE)		;yes, move two bytes from (DE) into
	LD	(HL),A		;this zero space.
	INC	DE
	INC	HL
	LD	A,(DE)
	LD	(HL),A
	DEC	DE		;don't disturb these registers.
	DEC	HL
	RET	
;
;   Get here to close a file specified by (fcb).
;
CLOSEIT:XOR	A		;clear status and file position bytes.
	LD	(STATUS),A
	LD	(FILEPOS),A
	LD	(FILEPOS+1),A
	CALL	GETWPRT		;get write protect bit for this drive.
	RET	NZ		;just return if it is set.
	CALL	GETS2		;else get the 's2' byte.
	AND	80H		;and look at bit 7 (file unmodified?).
	RET	NZ		;just return if set.
	LD	C,15		;else look up this file in directory.
	CALL	FINDFST
	CALL	CKFILPOS	;was it found?
	RET	Z		;just return if not.
	LD	BC,16		;set (HL) pointing to records used section.
	CALL	FCB2HL
	ADD	HL,BC
	EX	DE,HL
	LD	HL,(PARAMS)	;do the same for users specified fcb.
	ADD	HL,BC
	LD	C,16		;this many bytes are present in this extent.
CLOSEIT1: LD	A,(BIGDISK)	;8 or 16 bit record numbers?
	OR	A
	JP	Z,CLOSEIT4
	LD	A,(HL)		;just 8 bit. Get one from users fcb.
	OR	A
	LD	A,(DE)		;now get one from directory fcb.
	JP	NZ,CLOSEIT2
	LD	(HL),A		;users byte was zero. Update from directory.
CLOSEIT2: OR	A
	JP	NZ,CLOSEIT3
	LD	A,(HL)		;directories byte was zero, update from users fcb.
	LD	(DE),A
CLOSEIT3: CP	(HL)		;if neither one of these bytes were zero,
	JP	NZ,CLOSEIT7	;then close error if they are not the same.
	JP	CLOSEIT5	;ok so far, get to next byte in fcbs.
CLOSEIT4: CALL	MOVEWORD	;update users fcb if it is zero.
	EX	DE,HL
	CALL	MOVEWORD	;update directories fcb if it is zero.
	EX	DE,HL
	LD	A,(DE)		;if these two values are no different,
	CP	(HL)		;then a close error occured.
	JP	NZ,CLOSEIT7
	INC	DE		;check second byte.
	INC	HL
	LD	A,(DE)
	CP	(HL)
	JP	NZ,CLOSEIT7
	DEC	C		;remember 16 bit values.
CLOSEIT5: INC	DE		;bump to next item in table.
	INC	HL
	DEC	C		;there are 16 entries only.
	JP	NZ,CLOSEIT1	;continue if more to do.
	LD	BC,0FFECH	;backup 20 places (extent byte).
	ADD	HL,BC
	EX	DE,HL
	ADD	HL,BC
	LD	A,(DE)
	CP	(HL)		;directory's extent already greater than the
	JP	C,CLOSEIT6	;users extent?
	LD	(HL),A		;no, update directory extent.
	LD	BC,3		;and update the record count byte in
	ADD	HL,BC		;directories fcb.
	EX	DE,HL
	ADD	HL,BC
	LD	A,(HL)		;get from user.
	LD	(DE),A		;and put in directory.
CLOSEIT6: LD	A,0FFH		;set 'was open and is now closed' byte.
	LD	(CLOSEFLG),A
	JP	UPDATE1		;update the directory now.
CLOSEIT7: LD	HL,STATUS	;set return status and then return.
	DEC	(HL)
	RET	
;
;   Routine to get the next empty space in the directory. It
; will then be cleared for use.
;
GETEMPTY: CALL	CHKWPRT		;make sure disk is not write protected.
	LD	HL,(PARAMS)	;save current parameters (fcb).
	PUSH	HL
	LD	HL,EMPTYFCB	;use special one for empty space.
	LD	(PARAMS),HL
	LD	C,1		;search for first empty spot in directory.
	CALL	FINDFST		;(* only check first byte *)
	CALL	CKFILPOS	;none?
	POP	HL
	LD	(PARAMS),HL	;restore original fcb address.
	RET	Z		;return if no more space.
	EX	DE,HL
	LD	HL,15		;point to number of records for this file.
	ADD	HL,DE
	LD	C,17		;and clear all of this space.
	XOR	A
GETMT1:	LD	(HL),A
	INC	HL
	DEC	C
	JP	NZ,GETMT1
	LD	HL,13		;clear the 's1' byte also.
	ADD	HL,DE
	LD	(HL),A
	CALL	CHKNMBR		;keep (SCRATCH1) within bounds.
	CALL	FCBSET		;write out this fcb entry to directory.
	JP	SETS2B7		;set 's2' byte bit 7 (unmodified at present).
;
;   Routine to close the current extent and open the next one
; for reading.
;
GETNEXT:XOR	A
	LD	(CLOSEFLG),A	;clear close flag.
	CALL	CLOSEIT		;close this extent.
	CALL	CKFILPOS
	RET	Z		;not there???
	LD	HL,(PARAMS)	;get extent byte.
	LD	BC,12
	ADD	HL,BC
	LD	A,(HL)		;and increment it.
	INC	A
	AND	1FH		;keep within range 0-31.
	LD	(HL),A
	JP	Z,GTNEXT1	;overflow?
	LD	B,A		;mask extent byte.
	LD	A,(EXTMASK)
	AND	B
	LD	HL,CLOSEFLG	;check close flag (0ffh is ok).
	AND	(HL)
	JP	Z,GTNEXT2	;if zero, we must read in next extent.
	JP	GTNEXT3		;else, it is already in memory.
GTNEXT1:LD	BC,2		;Point to the 's2' byte.
	ADD	HL,BC
	INC	(HL)		;and bump it.
	LD	A,(HL)		;too many extents?
	AND	0FH
	JP	Z,GTNEXT5	;yes, set error code.
;
;   Get here to open the next extent.
;
GTNEXT2:LD	C,15		;set to check first 15 bytes of fcb.
	CALL	FINDFST		;find the first one.
	CALL	CKFILPOS	;none available?
	JP	NZ,GTNEXT3
	LD	A,(RDWRTFLG)	;no extent present. Can we open an empty one?
	INC	A		;0ffh means reading (so not possible).
	JP	Z,GTNEXT5	;or an error.
	CALL	GETEMPTY	;we are writing, get an empty entry.
	CALL	CKFILPOS	;none?
	JP	Z,GTNEXT5	;error if true.
	JP	GTNEXT4		;else we are almost done.
GTNEXT3:CALL	OPENIT1		;open this extent.
GTNEXT4:CALL	STRDATA		;move in updated data (rec #, extent #, etc.)
	XOR	A		;clear status and return.
	JP	SETSTAT
;
;   Error in extending the file. Too many extents were needed
; or not enough space on the disk.
;
GTNEXT5:CALL	IOERR1		;set error code, clear bit 7 of 's2'
	JP	SETS2B7		;so this is not written on a close.
;
;   Read a sequential file.
;
RDSEQ:	LD	A,1		;set sequential access mode.
	LD	(MODE),A
RDSEQ1:	LD	A,0FFH		;don't allow reading unwritten space.
	LD	(RDWRTFLG),A
	CALL	STRDATA		;put rec# and ext# into fcb.
	LD	A,(SAVNREC)	;get next record to read.
	LD	HL,SAVNXT	;get number of records in extent.
	CP	(HL)		;within this extent?
	JP	C,RDSEQ2
	CP	128		;no. Is this extent fully used?
	JP	NZ,RDSEQ3	;no. End-of-file.
	CALL	GETNEXT		;yes, open the next one.
	XOR	A		;reset next record to read.
	LD	(SAVNREC),A
	LD	A,(STATUS)	;check on open, successful?
	OR	A
	JP	NZ,RDSEQ3	;no, error.
RDSEQ2:	CALL	COMBLK		;ok. compute block number to read.
	CALL	CHKBLK		;check it. Within bounds?
	JP	Z,RDSEQ3	;no, error.
	CALL	LOGICAL		;convert (BLKNMBR) to logical sector (128 byte).
	CALL	TRKSEC1		;set the track and sector for this block #.
	CALL	DOREAD		;and read it.
	JP	SETNREC		;and set the next record to be accessed.
;
;   Read error occured. Set status and return.
;
RDSEQ3:	JP	IOERR1
;
;   Write the next sequential record.
;
WTSEQ:	LD	A,1		;set sequential access mode.
	LD	(MODE),A
WTSEQ1:	LD	A,0		;allow an addition empty extent to be opened.
	LD	(RDWRTFLG),A
	CALL	CHKWPRT		;check write protect status.
	LD	HL,(PARAMS)
	CALL	CKROF1		;check for read only file, (HL) already set to fcb.
	CALL	STRDATA		;put updated data into fcb.
	LD	A,(SAVNREC)	;get record number to write.
	CP	128		;within range?
	JP	NC,IOERR1	;no, error(?).
	CALL	COMBLK		;compute block number.
	CALL	CHKBLK		;check number.
	LD	C,0		;is there one to write to?
	JP	NZ,WTSEQ6	;yes, go do it.
	CALL	GETBLOCK	;get next block number within fcb to use.
	LD	(RELBLOCK),A	;and save.
	LD	BC,0		;start looking for space from the start
	OR	A		;if none allocated as yet.
	JP	Z,WTSEQ2
	LD	C,A		;extract previous block number from fcb
	DEC	BC		;so we can be closest to it.
	CALL	EXTBLK
	LD	B,H
	LD	C,L
WTSEQ2:	CALL	FNDSPACE	;find the next empty block nearest number (BC).
	LD	A,L		;check for a zero number.
	OR	H
	JP	NZ,WTSEQ3
	LD	A,2		;no more space?
	JP	SETSTAT
WTSEQ3:	LD	(BLKNMBR),HL	;save block number to access.
	EX	DE,HL		;put block number into (DE).
	LD	HL,(PARAMS)	;now we must update the fcb for this
	LD	BC,16		;newly allocated block.
	ADD	HL,BC
	LD	A,(BIGDISK)	;8 or 16 bit block numbers?
	OR	A
	LD	A,(RELBLOCK)	;(* update this entry *)
	JP	Z,WTSEQ4	;zero means 16 bit ones.
	CALL	ADDA2HL		;(HL)=(HL)+(A)
	LD	(HL),E		;store new block number.
	JP	WTSEQ5
WTSEQ4:	LD	C,A		;compute spot in this 16 bit table.
	LD	B,0
	ADD	HL,BC
	ADD	HL,BC
	LD	(HL),E		;stuff block number (DE) there.
	INC	HL
	LD	(HL),D
WTSEQ5:	LD	C,2		;set (C) to indicate writing to un-used disk space.
WTSEQ6:	LD	A,(STATUS)	;are we ok so far?
	OR	A
	RET	NZ
	PUSH	BC		;yes, save write flag for bios (register C).
	CALL	LOGICAL		;convert (BLKNMBR) over to loical sectors.
	LD	A,(MODE)	;get access mode flag (1=sequential,
	DEC	A		;0=random, 2=special?).
	DEC	A
	JP	NZ,WTSEQ9
;
;   Special random i/o from function #40. Maybe for M/PM, but the
; current block, if it has not been written to, will be zeroed
; out and then written (reason?).
;
	POP	BC
	PUSH	BC
	LD	A,C		;get write status flag (2=writing unused space).
	DEC	A
	DEC	A
	JP	NZ,WTSEQ9
	PUSH	HL
	LD	HL,(DIRBUF)	;zero out the directory buffer.
	LD	D,A		;note that (A) is zero here.
WTSEQ7:	LD	(HL),A
	INC	HL
	INC	D		;do 128 bytes.
	JP	P,WTSEQ7
	CALL	DIRDMA		;tell the bios the dma address for directory access.
	LD	HL,(LOGSECT)	;get sector that starts current block.
	LD	C,2		;set 'writing to unused space' flag.
WTSEQ8:	LD	(BLKNMBR),HL	;save sector to write.
	PUSH	BC
	CALL	TRKSEC1		;determine its track and sector numbers.
	POP	BC
	CALL	DOWRITE		;now write out 128 bytes of zeros.
	LD	HL,(BLKNMBR)	;get sector number.
	LD	C,0		;set normal write flag.
	LD	A,(BLKMASK)	;determine if we have written the entire
	LD	B,A		;physical block.
	AND	L
	CP	B
	INC	HL		;prepare for the next one.
	JP	NZ,WTSEQ8	;continue until (BLKMASK+1) sectors written.
	POP	HL		;reset next sector number.
	LD	(BLKNMBR),HL
	CALL	DEFDMA		;and reset dma address.
;
;   Normal disk write. Set the desired track and sector then
; do the actual write.
;
WTSEQ9:	CALL	TRKSEC1		;determine track and sector for this write.
	POP	BC		;get write status flag.
	PUSH	BC
	CALL	DOWRITE		;and write this out.
	POP	BC
	LD	A,(SAVNREC)	;get number of records in file.
	LD	HL,SAVNXT	;get last record written.
	CP	(HL)
	JP	C,WTSEQ10
	LD	(HL),A		;we have to update record count.
	INC	(HL)
	LD	C,2
;
;*   This area has been patched to correct disk update problem
;* when using blocking and de-blocking in the BIOS.
;
WTSEQ10:NOP			;was 'dcr c'
	NOP			;was 'dcr c'
	LD	HL,0		;was 'jnz wtseq99'
;
; *   End of patch.
;
	PUSH	AF
	CALL	GETS2		;set 'extent written to' flag.
	AND	7FH		;(* clear bit 7 *)
	LD	(HL),A
	POP	AF		;get record count for this extent.
WTSEQ99:CP	127		;is it full?
	JP	NZ,WTSEQ12
	LD	A,(MODE)	;yes, are we in sequential mode?
	CP	1
	JP	NZ,WTSEQ12
	CALL	SETNREC		;yes, set next record number.
	CALL	GETNEXT		;and get next empty space in directory.
	LD	HL,STATUS	;ok?
	LD	A,(HL)
	OR	A
	JP	NZ,WTSEQ11
	DEC	A		;yes, set record count to -1.
	LD	(SAVNREC),A
WTSEQ11:LD	(HL),0		;clear status.
WTSEQ12:JP	SETNREC		;set next record to access.
;
;   For random i/o, set the fcb for the desired record number
; based on the 'r0,r1,r2' bytes. These bytes in the fcb are
; used as follows:
;
;       fcb+35            fcb+34            fcb+33
;  |     'r-2'      |      'r-1'      |      'r-0'     |
;  |7             0 | 7             0 | 7             0|
;  |0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0|
;  |    overflow   | | extra |  extent   |   record #  |
;  | ______________| |_extent|__number___|_____________|
;                     also 's2'
;
;   On entry, register (C) contains 0ffh if this is a read
; and thus we can not access unwritten disk space. Otherwise,
; another extent will be opened (for writing) if required.
;
POSITION: XOR	A		;set random i/o flag.
	LD	(MODE),A
;
;   Special entry (function #40). M/PM ?
;
POSITN1:PUSH	BC		;save read/write flag.
	LD	HL,(PARAMS)	;get address of fcb.
	EX	DE,HL
	LD	HL,33		;now get byte 'r0'.
	ADD	HL,DE
	LD	A,(HL)
	AND	7FH		;keep bits 0-6 for the record number to access.
	PUSH	AF
	LD	A,(HL)		;now get bit 7 of 'r0' and bits 0-3 of 'r1'.
	RLA	
	INC	HL
	LD	A,(HL)
	RLA	
	AND	1FH		;and save this in bits 0-4 of (C).
	LD	C,A		;this is the extent byte.
	LD	A,(HL)		;now get the extra extent byte.
	RRA	
	RRA	
	RRA	
	RRA	
	AND	0FH
	LD	B,A		;and save it in (B).
	POP	AF		;get record number back to (A).
	INC	HL		;check overflow byte 'r2'.
	LD	L,(HL)
	INC	L
	DEC	L
	LD	L,6		;prepare for error.
	JP	NZ,POSITN5	;out of disk space error.
	LD	HL,32		;store record number into fcb.
	ADD	HL,DE
	LD	(HL),A
	LD	HL,12		;and now check the extent byte.
	ADD	HL,DE
	LD	A,C
	SUB	(HL)		;same extent as before?
	JP	NZ,POSITN2
	LD	HL,14		;yes, check extra extent byte 's2' also.
	ADD	HL,DE
	LD	A,B
	SUB	(HL)
	AND	7FH
	JP	Z,POSITN3	;same, we are almost done then.
;
;  Get here when another extent is required.
;
POSITN2:PUSH	BC
	PUSH	DE
	CALL	CLOSEIT		;close current extent.
	POP	DE
	POP	BC
	LD	L,3		;prepare for error.
	LD	A,(STATUS)
	INC	A
	JP	Z,POSITN4	;close error.
	LD	HL,12		;put desired extent into fcb now.
	ADD	HL,DE
	LD	(HL),C
	LD	HL,14		;and store extra extent byte 's2'.
	ADD	HL,DE
	LD	(HL),B
	CALL	OPENIT		;try and get this extent.
	LD	A,(STATUS)	;was it there?
	INC	A
	JP	NZ,POSITN3
	POP	BC		;no. can we create a new one (writing?).
	PUSH	BC
	LD	L,4		;prepare for error.
	INC	C
	JP	Z,POSITN4	;nope, reading unwritten space error.
	CALL	GETEMPTY	;yes we can, try to find space.
	LD	L,5		;prepare for error.
	LD	A,(STATUS)
	INC	A
	JP	Z,POSITN4	;out of space?
;
;   Normal return location. Clear error code and return.
;
POSITN3:POP	BC		;restore stack.
	XOR	A		;and clear error code byte.
	JP	SETSTAT
;
;   Error. Set the 's2' byte to indicate this (why?).
;
POSITN4:PUSH	HL
	CALL	GETS2
	LD	(HL),0C0H
	POP	HL
;
;   Return with error code (presently in L).
;
POSITN5:POP	BC
	LD	A,L		;get error code.
	LD	(STATUS),A
	JP	SETS2B7
;
;   Read a random record.
;
READRAN:LD	C,0FFH		;set 'read' status.
	CALL	POSITION	;position the file to proper record.
	CALL	Z,RDSEQ1	;and read it as usual (if no errors).
	RET	
;
;   Write to a random record.
;
WRITERAN: LD	C,0		;set 'writing' flag.
	CALL	POSITION	;position the file to proper record.
	CALL	Z,WTSEQ1	;and write as usual (if no errors).
	RET	
;
;   Compute the random record number. Enter with (HL) pointing
; to a fcb an (DE) contains a relative location of a record
; number. On exit, (C) contains the 'r0' byte, (B) the 'r1'
; byte, and (A) the 'r2' byte.
;
;   On return, the zero flag is set if the record is within
; bounds. Otherwise, an overflow occured.
;
COMPRAND: EX	DE,HL		;save fcb pointer in (DE).
	ADD	HL,DE		;compute relative position of record #.
	LD	C,(HL)		;get record number into (BC).
	LD	B,0
	LD	HL,12		;now get extent.
	ADD	HL,DE
	LD	A,(HL)		;compute (BC)=(record #)+(extent)*128.
	RRCA			;move lower bit into bit 7.
	AND	80H		;and ignore all other bits.
	ADD	A,C		;add to our record number.
	LD	C,A
	LD	A,0		;take care of any carry.
	ADC	A,B
	LD	B,A
	LD	A,(HL)		;now get the upper bits of extent into
	RRCA			;bit positions 0-3.
	AND	0FH		;and ignore all others.
	ADD	A,B		;add this in to 'r1' byte.
	LD	B,A
	LD	HL,14		;get the 's2' byte (extra extent).
	ADD	HL,DE
	LD	A,(HL)
	ADD	A,A		;and shift it left 4 bits (bits 4-7).
	ADD	A,A
	ADD	A,A
	ADD	A,A
	PUSH	AF		;save carry flag (bit 0 of flag byte).
	ADD	A,B		;now add extra extent into 'r1'.
	LD	B,A
	PUSH	AF		;and save carry (overflow byte 'r2').
	POP	HL		;bit 0 of (L) is the overflow indicator.
	LD	A,L
	POP	HL		;and same for first carry flag.
	OR	L		;either one of these set?
	AND	01H		;only check the carry flags.
	RET	
;
;   Routine to setup the fcb (bytes 'r0', 'r1', 'r2') to
; reflect the last record used for a random (or other) file.
; This reads the directory and looks at all extents computing
; the largerst record number for each and keeping the maximum
; value only. Then 'r0', 'r1', and 'r2' will reflect this
; maximum record number. This is used to compute the space used
; by a random file.
;
RANSIZE:LD	C,12		;look thru directory for first entry with
	CALL	FINDFST		;this name.
	LD	HL,(PARAMS)	;zero out the 'r0, r1, r2' bytes.
	LD	DE,33
	ADD	HL,DE
	PUSH	HL
	LD	(HL),D		;note that (D)=0.
	INC	HL
	LD	(HL),D
	INC	HL
	LD	(HL),D
RANSIZ1:CALL	CKFILPOS	;is there an extent to process?
	JP	Z,RANSIZ3	;no, we are done.
	CALL	FCB2HL		;set (HL) pointing to proper fcb in dir.
	LD	DE,15		;point to last record in extent.
	CALL	COMPRAND	;and compute random parameters.
	POP	HL
	PUSH	HL		;now check these values against those
	LD	E,A		;already in fcb.
	LD	A,C		;the carry flag will be set if those
	SUB	(HL)		;in the fcb represent a larger size than
	INC	HL		;this extent does.
	LD	A,B
	SBC	A,(HL)
	INC	HL
	LD	A,E
	SBC	A,(HL)
	JP	C,RANSIZ2
	LD	(HL),E		;we found a larger (in size) extent.
	DEC	HL		;stuff these values into fcb.
	LD	(HL),B
	DEC	HL
	LD	(HL),C
RANSIZ2:CALL	FINDNXT		;now get the next extent.
	JP	RANSIZ1		;continue til all done.
RANSIZ3:POP	HL		;we are done, restore the stack and
	RET			;return.
;
;   Function to return the random record position of a given
; file which has been read in sequential mode up to now.
;
SETRAN:	LD	HL,(PARAMS)	;point to fcb.
	LD	DE,32		;and to last used record.
	CALL	COMPRAND	;compute random position.
	LD	HL,33		;now stuff these values into fcb.
	ADD	HL,DE
	LD	(HL),C		;move 'r0'.
	INC	HL
	LD	(HL),B		;and 'r1'.
	INC	HL
	LD	(HL),A		;and lastly 'r2'.
	RET	
;
;   This routine select the drive specified in (ACTIVE) and
; update the login vector and bitmap table if this drive was
; not already active.
;
LOGINDRV: LD	HL,(LOGIN)	;get the login vector.
	LD	A,(ACTIVE)	;get the default drive.
	LD	C,A
	CALL	SHIFTR		;position active bit for this drive
	PUSH	HL		;into bit 0.
	EX	DE,HL
	CALL	SELECT		;select this drive.
	POP	HL
	CALL	Z,SLCTERR	;valid drive?
	LD	A,L		;is this a newly activated drive?
	RRA	
	RET	C
	LD	HL,(LOGIN)	;yes, update the login vector.
	LD	C,L
	LD	B,H
	CALL	SETBIT
	LD	(LOGIN),HL	;and save.
	JP	BITMAP		;now update the bitmap.
;
;   Function to set the active disk number.
;
SETDSK:	LD	A,(EPARAM)	;get parameter passed and see if this
	LD	HL,ACTIVE	;represents a change in drives.
	CP	(HL)
	RET	Z
	LD	(HL),A		;yes it does, log it in.
	JP	LOGINDRV
;
;   This is the 'auto disk select' routine. The firsst byte
; of the fcb is examined for a drive specification. If non
; zero then the drive will be selected and loged in.
;
AUTOSEL:LD	A,0FFH		;say 'auto-select activated'.
	LD	(AUTO),A
	LD	HL,(PARAMS)	;get drive specified.
	LD	A,(HL)
	AND	1FH		;look at lower 5 bits.
	DEC	A		;adjust for (1=A, 2=B) etc.
	LD	(EPARAM),A	;and save for the select routine.
	CP	1EH		;check for 'no change' condition.
	JP	NC,AUTOSL1	;yes, don't change.
	LD	A,(ACTIVE)	;we must change, save currently active
	LD	(OLDDRV),A	;drive.
	LD	A,(HL)		;and save first byte of fcb also.
	LD	(AUTOFLAG),A	;this must be non-zero.
	AND	0E0H		;whats this for (bits 6,7 are used for
	LD	(HL),A		;something)?
	CALL	SETDSK		;select and log in this drive.
AUTOSL1:LD	A,(USERNO)	;move user number into fcb.
	LD	HL,(PARAMS)	;(* upper half of first byte *)
	OR	(HL)
	LD	(HL),A
	RET			;and return (all done).
;
;   Function to return the current cp/m version number.
;
GETVER:	LD	A,022H		;version 2.2
	JP	SETSTAT
;
;   Function to reset the disk system.
;
RSTDSK:	LD	HL,0		;clear write protect status and log
	LD	(WRTPRT),HL	;in vector.
	LD	(LOGIN),HL
	XOR	A		;select drive 'A'.
	LD	(ACTIVE),A
	LD	HL,TBUFF	;setup default dma address.
	LD	(USERDMA),HL
	CALL	DEFDMA
	JP	LOGINDRV	;now log in drive 'A'.
;
;   Function to open a specified file.
;
OPENFIL:CALL	CLEARS2		;clear 's2' byte.
	CALL	AUTOSEL		;select proper disk.
	JP	OPENIT		;and open the file.
;
;   Function to close a specified file.
;
CLOSEFIL: CALL	AUTOSEL		;select proper disk.
	JP	CLOSEIT		;and close the file.
;
;   Function to return the first occurence of a specified file
; name. If the first byte of the fcb is '?' then the name will
; not be checked (get the first entry no matter what).
;
GETFST:	LD	C,0		;prepare for special search.
	EX	DE,HL
	LD	A,(HL)		;is first byte a '?'?
	CP	'?'
	JP	Z,GETFST1	;yes, just get very first entry (zero length match).
	CALL	SETEXT		;get the extension byte from fcb.
	LD	A,(HL)		;is it '?'? if yes, then we want
	CP	'?'		;an entry with a specific 's2' byte.
	CALL	NZ,CLEARS2	;otherwise, look for a zero 's2' byte.
	CALL	AUTOSEL		;select proper drive.
	LD	C,15		;compare bytes 0-14 in fcb (12&13 excluded).
GETFST1:CALL	FINDFST		;find an entry and then move it into
	JP	MOVEDIR		;the users dma space.
;
;   Function to return the next occurence of a file name.
;
GETNXT:	LD	HL,(SAVEFCB)	;restore pointers. note that no
	LD	(PARAMS),HL	;other dbos calls are allowed.
	CALL	AUTOSEL		;no error will be returned, but the
	CALL	FINDNXT		;results will be wrong.
	JP	MOVEDIR
;
;   Function to delete a file by name.
;
DELFILE:CALL	AUTOSEL		;select proper drive.
	CALL	ERAFILE		;erase the file.
	JP	STSTATUS	;set status and return.
;
;   Function to execute a sequential read of the specified
; record number.
;
READSEQ:CALL	AUTOSEL		;select proper drive then read.
	JP	RDSEQ
;
;   Function to write the net sequential record.
;
WRTSEQ:	CALL	AUTOSEL		;select proper drive then write.
	JP	WTSEQ
;
;   Create a file function.
;
FCREATE:CALL	CLEARS2		;clear the 's2' byte on all creates.
	CALL	AUTOSEL		;select proper drive and get the next
	JP	GETEMPTY	;empty directory space.
;
;   Function to rename a file.
;
RENFILE:CALL	AUTOSEL		;select proper drive and then switch
	CALL	CHGNAMES	;file names.
	JP	STSTATUS
;
;   Function to return the login vector.
;
GETLOG:	LD	HL,(LOGIN)
	JP	GETPRM1
;
;   Function to return the current disk assignment.
;
GETCRNT:LD	A,(ACTIVE)
	JP	SETSTAT
;
;   Function to set the dma address.
;
PUTDMA:	EX	DE,HL
	LD	(USERDMA),HL	;save in our space and then get to
	JP	DEFDMA		;the bios with this also.
;
;   Function to return the allocation vector.
;
GETALOC:LD	HL,(ALOCVECT)
	JP	GETPRM1
;
;   Function to return the read-only status vector.
;
GETROV:	LD	HL,(WRTPRT)
	JP	GETPRM1
;
;   Function to set the file attributes (read-only, system).
;
SETATTR:CALL	AUTOSEL		;select proper drive then save attributes.
	CALL	SAVEATTR
	JP	STSTATUS
;
;   Function to return the address of the disk parameter block
; for the current drive.
;
GETPARM:LD	HL,(DISKPB)
GETPRM1:LD	(STATUS),HL
	RET	
;
;   Function to get or set the user number. If (E) was (FF)
; then this is a request to return the current user number.
; Else set the user number from (E).
;
GETUSER:LD	A,(EPARAM)	;get parameter.
	CP	0FFH		;get user number?
	JP	NZ,SETUSER
	LD	A,(USERNO)	;yes, just do it.
	JP	SETSTAT
SETUSER:AND	1FH		;no, we should set it instead. keep low
	LD	(USERNO),A	;bits (0-4) only.
	RET	
;
;   Function to read a random record from a file.
;
RDRANDOM: CALL	AUTOSEL		;select proper drive and read.
	JP	READRAN
;
;   Function to compute the file size for random files.
;
WTRANDOM: CALL	AUTOSEL		;select proper drive and write.
	JP	WRITERAN
;
;   Function to compute the size of a random file.
;
FILESIZE: CALL	AUTOSEL		;select proper drive and check file length
	JP	RANSIZE
;
;   Function #37. This allows a program to log off any drives.
; On entry, set (DE) to contain a word with bits set for those
; drives that are to be logged off. The log-in vector and the
; write protect vector will be updated. This must be a M/PM
; special function.
;
LOGOFF:	LD	HL,(PARAMS)	;get drives to log off.
	LD	A,L		;for each bit that is set, we want
	CPL			;to clear that bit in (LOGIN)
	LD	E,A		;and (WRTPRT).
	LD	A,H
	CPL	
	LD	HL,(LOGIN)	;reset the login vector.
	AND	H
	LD	D,A
	LD	A,L
	AND	E
	LD	E,A
	LD	HL,(WRTPRT)
	EX	DE,HL
	LD	(LOGIN),HL	;and save.
	LD	A,L		;now do the write protect vector.
	AND	E
	LD	L,A
	LD	A,H
	AND	D
	LD	H,A
	LD	(WRTPRT),HL	;and save. all done.
	RET	
;
;   Get here to return to the user.
;
GOBACK:	LD	A,(AUTO)	;was auto select activated?
	OR	A
	JP	Z,GOBACK1
	LD	HL,(PARAMS)	;yes, but was a change made?
	LD	(HL),0		;(* reset first byte of fcb *)
	LD	A,(AUTOFLAG)
	OR	A
	JP	Z,GOBACK1
	LD	(HL),A		;yes, reset first byte properly.
	LD	A,(OLDDRV)	;and get the old drive and select it.
	LD	(EPARAM),A
	CALL	SETDSK
GOBACK1:LD	HL,(USRSTACK)	;reset the users stack pointer.
	LD	SP,HL
	LD	HL,(STATUS)	;get return status.
	LD	A,L		;force version 1.4 compatability.
	LD	B,H
	RET			;and go back to user.
;
;   Function #40. This is a special entry to do random i/o.
; For the case where we are writing to unused disk space, this
; space will be zeroed out first. This must be a M/PM special
; purpose function, because why would any normal program even
; care about the previous contents of a sector about to be
; written over.
;
WTSPECL:CALL	AUTOSEL		;select proper drive.
	LD	A,2		;use special write mode.
	LD	(MODE),A
	LD	C,0		;set write indicator.
	CALL	POSITN1		;position the file.
	CALL	Z,WTSEQ1	;and write (if no errors).
	RET	
;
;**************************************************************
;*
;*     BDOS data storage pool.
;*
;**************************************************************
;
EMPTYFCB: DEFB	0E5H		;empty directory segment indicator.
WRTPRT:	DEFW	0		;write protect status for all 16 drives.
LOGIN:	DEFW	0		;drive active word (1 bit per drive).
USERDMA:DEFW	080H		;user's dma address (defaults to 80h).
;
;   Scratch areas from parameter block.
;
SCRATCH1: DEFW	0		;relative position within dir segment for file (0-3).
SCRATCH2: DEFW	0		;last selected track number.
SCRATCH3: DEFW	0		;last selected sector number.
;
;   Disk storage areas from parameter block.
;
DIRBUF:	DEFW	0		;address of directory buffer to use.
DISKPB:	DEFW	0		;contains address of disk parameter block.
CHKVECT:DEFW	0		;address of check vector.
ALOCVECT: DEFW	0		;address of allocation vector (bit map).
;
;   Parameter block returned from the bios.
;
SECTORS:DEFW	0		;sectors per track from bios.
BLKSHFT:DEFB	0		;block shift.
BLKMASK:DEFB	0		;block mask.
EXTMASK:DEFB	0		;extent mask.
DSKSIZE:DEFW	0		;disk size from bios (number of blocks-1).
DIRSIZE:DEFW	0		;directory size.
ALLOC0:	DEFW	0		;storage for first bytes of bit map (dir space used).
ALLOC1:	DEFW	0
OFFSET:	DEFW	0		;first usable track number.
XLATE:	DEFW	0		;sector translation table address.
;
;
CLOSEFLG: DEFB	0		;close flag (=0ffh is extent written ok).
RDWRTFLG: DEFB	0		;read/write flag (0ffh=read, 0=write).
FNDSTAT:DEFB	0		;filename found status (0=found first entry).
MODE:	DEFB	0		;I/o mode select (0=random, 1=sequential, 2=special random).
EPARAM:	DEFB	0		;storage for register (E) on entry to bdos.
RELBLOCK: DEFB	0		;relative position within fcb of block number written.
COUNTER:DEFB	0		;byte counter for directory name searches.
SAVEFCB:DEFW	0,0		;save space for address of fcb (for directory searches).
BIGDISK:DEFB	0		;if =0 then disk is > 256 blocks long.
AUTO:	DEFB	0		;if non-zero, then auto select activated.
OLDDRV:	DEFB	0		;on auto select, storage for previous drive.
AUTOFLAG: DEFB	0		;if non-zero, then auto select changed drives.
SAVNXT:	DEFB	0		;storage for next record number to access.
SAVEXT:	DEFB	0		;storage for extent number of file.
SAVNREC:DEFW	0		;storage for number of records in file.
BLKNMBR:DEFW	0		;block number (physical sector) used within a file or logical sect
LOGSECT:DEFW	0		;starting logical (128 byte) sector of block (physical sector).
FCBPOS:	DEFB	0		;relative position within buffer for fcb of file of interest.
FILEPOS:DEFW	0		;files position within directory (0 to max entries -1).
;
;   Disk directory buffer checksum bytes. One for each of the
; 16 possible drives.
;
CKSUMTBL: DEFB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
