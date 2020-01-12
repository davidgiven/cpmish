; Amstrad NC200 cpmish BIOS © 2020 David Given
; This file is distributable under the terms of the 2-clause BSD license.
; See COPYING.cpmish in the distribution root directory for more information.

; Prints the character in A (waiting first until the printer is ready).
lst_print:
    push af
.1:
    in a, (PORT_PRINTER_STATUS)
    bit 0, a
    jr nz, .1           ; spin until ready
    pop af

    out (PORT_PRINTER_DATA), a

    di
    ld a, (PBAUD)
    res 6, a
    out (PORT_BAUDRATE), a
    set 6, a
    out (PORT_BAUDRATE), a
    ei
    
    ret

; Tests the printer status, returning Z if the printer is busy or NZ if it's ready.
lst_test_busy:
    in a, (PORT_PRINTER_STATUS)
    and 1
    xor 1                ; bit is 1 if busy, 0 if ready
    ret
    