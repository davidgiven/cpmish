.globl _cpm_findfirst
_cpm_findfirst:
    ld d, h
    ld e, l
    ld c, #17
    jp 0x0005
    