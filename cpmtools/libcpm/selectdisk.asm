.globl _cpm_select_disk
_cpm_select_disk:
    ld e, l
    ld c, #14
    jp 0x0005

