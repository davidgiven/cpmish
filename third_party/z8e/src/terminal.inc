;-----------------------------------------------------------------------------
; Terminal types.
;-----------------------------------------------------------------------------

 if	termdef			; Once only table definition

;-----------------------------------------------------------------------------
; This just assigns a number to each terminal type.  The numbers themselves
; don't matter just so long as there are no duplicates.  If you add a terminal
; then make up a new entry for it here and add an entry to the two inclusion
; lists.  (Remember that the file names in the inclusion lists must be in
; upper case for them to work with all versions of M80.)
;
; Any synonyms should be defined in this section.  For example, the ADM3A,
; Soroc and and Televideo terminals use the same control sequences so they
; should be given the same number.  When soroc is called for, the ADM3A
; definitions will be used.
;-----------------------------------------------------------------------------

none	equ	0
adm3a	equ	1
ansi	equ	2
d8227	equ	3
vt52	equ	4
soroc	equ	4		; Soroc is a synonym for adm3a
tvi	equ	4		; Televideo also

;-----------------------------------------------------------------------------

termdef	defl	false		; Don't touch this

 else	; termdef

;-----------------------------------------------------------------------------
; Include console terminal code or data upon request
;-----------------------------------------------------------------------------

  if	slr or zasmb
    if	terminal eq console

      if	conterm eq adm3a
	include	ADM3A.TTY
      endif

      if	conterm eq ansi
	include	ANSI.TTY
      endif

      if	conterm eq d8227
	include	D8227.TTY
      endif

      if	conterm eq vt52
	include	VT52.TTY
      endif

    endif ; console
;-----------------------------------------------------------------------------
; Include auxiliary terminal code or data upon request
;-----------------------------------------------------------------------------

    if terminal eq auxiliary
      if	auxterm eq adm3a
	include	ADM3A.TTY
      endif

      if	auxterm eq ansi
	include	ANSI.TTY
      endif

      if	auxterm eq d8227
	include	D8227.TTY
      endif

      if	auxterm eq vt52
	include	VT52.TTY
      endif
    endif ;terminal eq auxiliary
  endif	  ; slr or zasmb

;-----------------------------------------------------------------------------
; M80 does not allow nested includes.  To import the correct file for each
; terminal I resorted to this artifice.  It is rather ugly but the idea is
; to keep the list of terminals in this file where it is easier to maintain
; than it would be if it were in the main source file.
;
; It would be nice to think of a more uniform approach.
;-----------------------------------------------------------------------------

  if	m80
    if	terminal eq console
      if	conterm eq adm3a
conincl	macro
	include	ADM3A.TTY
	endm
      endif

      if	conterm eq ansi
conincl	macro
	include	ANSI.TTY
	endm
      endif

      if	conterm eq d8227
conincl	macro
	include	D8227.TTY
	endm
      endif

      if	conterm eq vt52
conincl	macro
	include	VT52.TTY
	endm
      endif
    endif ; terminal eq console

    if terminal eq auxiliary
      if	auxterm eq adm3a
auxincl	macro
	include	ADM3A.TTY
	endm
      endif

      if	auxterm eq ansi
auxincl	macro
	include	ANSI.TTY
	endm
      endif

      if	auxterm eq d8227
auxincl	macro
	include	D8227.TTY
	endm
      endif

      if	auxterm eq vt52
auxincl	macro
	include	VT52.TTY
	endm
      endif
    endif ; terminal eq auxiliary
  endif ; M80
 endif ; termdef
