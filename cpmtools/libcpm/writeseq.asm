.globl _cpm_write_sequential
_cpm_write_sequential:
    ld d, h
    ld e, l
    ld c, #21
    jp 0x0005

