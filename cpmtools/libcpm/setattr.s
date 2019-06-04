.globl _cpm_set_file_attributes
_cpm_set_file_attributes:
    ld d, h
    ld e, l
    ld c, #30
    jp 0x0005
    

