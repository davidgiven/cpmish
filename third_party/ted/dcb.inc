; Device Control Block structure, one per loaded device

D.LNK	equ	0		; link to next DCB in device list (2 bytes)
D.ST	equ	D.LNK + 2	; status (1 byte)
D.TCNT	equ	D.ST + 1	; timeout counter (2 bytes)
D.NAME	equ	D.TCNT + 2	; device name (2 bytes)
D.UNITS	equ	D.NAME + 2	; number of units (1 byte)
D.UCBL	equ	D.UNITS + 1	; link to list of UCBs (2 bytes)
D.BANK	equ	D.UCBL + 2	; device driver page (BBR) (1 byte)
D.START	equ	D.BANK + 1	; device driver start address (2 bytes)
D.END	equ	D.START + 2	; device driver end address (2 bytes)
D.EPT	equ	D.END + 2	; device driver jump table entry point (2 bytes)

; DCB size

DCBSZ	equ	D.EPT + 2

; Status bit numbers

DS.RES	equ	0		; resident (unloadable)

; Unit Control Block structure, one per device unit (statically allocated)

U.LNK	equ	0		; link to next UCB in list (2 bytes)
U.UNIT	equ	U.LNK + 2	; physical unit number (1 byte)
U.ST	equ	U.UNIT + 1	; status bits (1 byte)
U.DCB	equ	U.ST + 1	; pointer to the corresponding DCB (2 bytes)
U.SCB	equ	U.DCB + 2	; pointer to the related SCB (2 bytes)
U.RED	equ	U.SCB + 2	; redirect pointer (2 bytes)
U.CTL	equ	U.RED + 2	; control bits (1 byte)
U.CW	equ	U.CTL + 1	; characteristics word (2 bytes)
U.ATT	equ	U.CW + 2	; TCB of attached task (0=detached) (2 bytes)
U.ACP	equ	U.ATT + 2	; TCB of ACP task (0=no ACP) (2 bytes)
U.LCB	equ	U.ACP + 2	; LCB of owner (2 bytes)

; Unit control bit numbers

UC.ATT	equ	0		; send attach/detach notifications

; UCB min size

UCBSZ	equ	U.LCB + 2

; UCB status bit numbers

US.OFL	equ	0		; unit is offline
US.BSY	equ	1		; unit busy
US.MNT	equ	2		; unit mounted
US.PUB	equ	3		; unit is public
US.DMO	equ	4		; unit is being dismounted (file access denied)
US.RED	equ	5		; unit can be redirected
US.AST	equ	6		; generate AST on I/O completion for the attached task

; Device/Unit Characteristics Word bit numbers: Lo-byte

DV.REC	equ	0		; record-oriented (block) device
DV.TTY	equ	1		; terminal (character) device
DV.DIR	equ	2		; directory device
DV.MNT	equ	3		; device mountable
DV.PSE	equ	4		; pseudo-device

; Device/Unit Characteristics Word bit numbers: Hi-byte (device-dependent)
; Terminal characteristics bits

TC.TTS	equ	0		; misc terminal operation sync bit
TC.BIN	equ	1		; binary mode (pass-all)
TC.NEC	equ	2		; no-echo mode
TC.SCP	equ	3		; scope mode
TC.SLV	equ	4		; slave mode
TC.SMR	equ	5		; uppercase conversion on input disabled
TC.NBR	equ	6		; not receiving broadcast messages
TC.ANS	equ	7		; ANSI terminal

; Status Control Block, one per controller (statically allocated)

S.ST	equ	0		; controller status (1 byte)
S.PKTL	equ	S.ST + 1	; link to I/O packet queue (2 bytes)
S.CPKT	equ	S.PKTL + 2	; address of current I/O packet (2 bytes)

; SCB min size

SCBSZ	equ	S.CPKT + 2

; Logical device assignment structure, one per logical device

N.LNK	equ	0		; link to next item in list (2 bytes)
N.TYPE	equ	N.LNK + 2	; assignment type (1 byte)
N.TI	equ	N.TYPE + 1	; UCB of terminal (2 bytes)
N.NAME	equ	N.TI + 2	; logical device name (2 bytes)
N.UNIT	equ	N.NAME + 2	; logical device unit (1 byte)
N.UCB	equ	N.UNIT + 1	; UCB of physical device (2 bytes)

; Logical Assignment structure size

LASZ	equ	N.UCB + 2

; Assignment types

N.LCL	equ	0		; local
N.LGN	equ	1		; login
N.GBL	equ	2		; global

