; ===============================================
; CamelForth for the Zilog Z80
; Copyright (c) 1994,1995 Bradford J. Rodriguez
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

; Commercial inquiries should be directed to the author at 
; 115 First St., #105, Collingwood, Ontario L9Y 4W3 Canada
; or via email to bj@camelforth.com
;
; ===============================================
; CAMEL80.AZM: Code Primitives
;   Source code is for the Z80MR macro assembler.
;   Forth words are documented as follows:
;x   NAME     stack -- stack    description
;   where x=C for ANS Forth Core words, X for ANS
;   Extensions, Z for internal or private words.
;
; Direct-Threaded Forth model for Zilog Z80
; 16 bit cell, 8 bit char, 8 bit (byte) adrs unit
;    Z80 BC = Forth TOS (top Param Stack item)
;        HL =       W    working register
;        DE =       IP   Interpreter Pointer
;        SP =       PSP  Param Stack Pointer
;        IX =       RSP  Return Stack Pointer
;        IY =       UP   User area Pointer
;    A, alternate register set = temporaries
;
; Macros to define Forth headers
; HEAD  label,length,name,action
; IMMED label,length,name,action
;    label  = assembler name for this word
;             (special characters not allowed)
;    length = length of name field
;    name   = Forth's name for this word
;    action = code routine for this word, e.g.
;             DOCOLON, or DOCODE for code words
; IMMED defines a header for an IMMEDIATE word.
;
DOCODE  EQU 0      ; flag to indicate CODE words
link    DEFL 0     ; link to previous Forth word

head    MACRO   #label,#length,#name,#action
        DW link
        DB 0
link    DEFL $
        DB #length,'#name'
#label:
        IF  #action<>DOCODE
        call #action
        ENDIF
        ENDM

immed   MACRO   #label,#length,#name,#action
        DW link
        DB 1
link    DEFL $
        DB #length,'#name'
#label:
        IF  #action<>DOCODE
        call #action
        ENDIF
        ENDM

; The NEXT macro (7 bytes) assembles the 'next'
; code in-line in every Z80 CamelForth CODE word.
next    MACRO
        ex de,hl
        ld e,(hl)
        inc hl
        ld d,(hl)
        inc hl
        ex de,hl
        jp (hl)
        ENDM

; NEXTHL is used when the IP is already in HL.
nexthl  MACRO
        ld e,(hl)
        inc hl
        ld d,(hl)
        inc hl
        ex de,hl
        jp (hl)
        ENDM

