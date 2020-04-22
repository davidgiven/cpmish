;-----------------------------------------------------------------------------
;	    HD64189/Z180 debugger with auxiliary debug terminal
;-----------------------------------------------------------------------------

; Setting Z180 to TRUE enables Hitachi HD64180/Z180 support. Set it to FALSE
; for a Z80-only version of Z8E.

z180	equ	true		; Enable Z180/HD64180 support

; Define the console terminal type

conterm	equ	ansi

; Define the auxiliary terminal type

auxterm	equ	adm3a

; If you are running CP/M Plus then you can set CPM3 true in which case the
; auxiliary input status routine becomes a simple BDOS call, otherwise you
; may need to provide some hardware-specific code.

CPM3	equ	true
