FALSE	equ	0
TRUE	equ	not FALSE

CR	equ	0Dh
LF	equ	0Ah
TAB	equ	09h
ESC	equ	1Bh
SSPC	equ	' '+80h

BLKSIZ	equ	512		; disk block size
BUFSIZ	equ	BLKSIZ+4	; memory buffer size (516 bytes)

MAXLEN	equ	162		; max line length
