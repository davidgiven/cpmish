.globl _cpm_close_file
_cpm_close_file:
    ld d, h
    ld e, l
    ld c, #16
    jp 0x0005
