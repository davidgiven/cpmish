.globl _cpm_delete_file
_cpm_delete_file:
    ld d, h
    ld e, l
    ld c, #19
    jp 0x0005
