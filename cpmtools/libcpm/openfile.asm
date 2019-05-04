.globl _cpm_open_file
_cpm_open_file:
    ld d, h
    ld e, l
    ld c, #15
    jp 0x0005
