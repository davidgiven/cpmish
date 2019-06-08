.globl _cpm_make_file
_cpm_make_file:
    ld d, h
    ld e, l
    ld c, #22
    jp 0x0005
