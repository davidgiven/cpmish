.globl _cpm_read_sequential
_cpm_read_sequential:
    ld d, h
    ld e, l
    ld c, #20
    jp 0x0005

