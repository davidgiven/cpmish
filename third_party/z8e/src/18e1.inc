;-----------------------------------------------------------------------------
;		HD64189/Z180 debugger with one terminal
;-----------------------------------------------------------------------------

; Setting Z180 to TRUE enables Hitachi HD64180/Z180 support. Set it to FALSE
; for a Z80-only version of Z8E.

z180	equ	true		; Enable Z180/HD64180 support

; Define the console terminal

conterm	equ	ansi

; Define the auxiliary terminal

auxterm equ	none

; If you are running under CP/M Plus then you can set CPM3 to true in which
; case the auxiliary input status routine will be a simple BDOS call.

CPM3	equ	true
