.globl _cpm_seek_to_end
_cpm_seek_to_end:
    ld d, h
    ld e, l
    ld c, #35
    jp 0x0005
    
