.globl _cpm_findnext
_cpm_findnext:
    ld d, h
    ld e, l
    ld c, #18
    jp 0x0005
    
