; AST control/event block structure

A.LNK	equ	0		; link to next item in list (2 bytes)
A.SIZ	equ	A.LNK + 2	; size of data area (1 byte)
A.TYP	equ	A.SIZ + 1	; event type (1 byte)
A.ADDR	equ	A.TYP + 1	; AST entry address (2 bytes)
A.DAT	equ	A.ADDR + 2	; start of data area (variable length)

; AST control block size = AST event header size

ACBSZ	equ	A.DAT

; AST types

AST.IO	equ	1		; I/O completion
AST.UC	equ	2		; unsolicited char from attached terminal
AST.RD	equ	3		; data received
AST.CK	equ	4		; clock (mark time)
AST.ST	equ	5		; status (from offspring)
AST.AB	equ	6		; abort/exit
AST.CL	equ	7		; command line (CLIs only)

