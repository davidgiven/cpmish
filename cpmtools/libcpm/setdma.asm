.globl _cpm_set_dma
_cpm_set_dma:
    ld d, h
    ld e, l
    ld c, #26
    jp 0x0005
    