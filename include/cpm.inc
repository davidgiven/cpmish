IOBYTE   = 0x0003
CDISK    = 0x0004
BDOSCALL = 0x0005

BBASE = 0xfe00 ; BIOS entrypoint
FBASE = 0xf000 ; BDOS entrypoint
CBASE = 0xe800 ; CCP entrypoint

; BIOS entrypoints.

BOOT =      BBASE +  0*3
WBOOT =     BBASE +  1*3
CONST =     BBASE +  2*3
CONIN =     BBASE +  3*3
CONOUT =    BBASE +  4*3
LIST =      BBASE +  5*3
PUNCH =     BBASE +  6*3
READER =    BBASE +  7*3
HOME =      BBASE +  8*3
SELDSK =    BBASE +  9*3
SETTRK =    BBASE + 10*3
SETSEC =    BBASE + 11*3
SETDMA =    BBASE + 12*3
READ =      BBASE + 13*3
WRITE =     BBASE + 14*3
LISTST =    BBASE + 15*3
SECTRAN =   BBASE + 16*3
INTERRUPT = BBASE + 17*3
