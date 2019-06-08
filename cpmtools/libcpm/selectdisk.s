.globl _cpm_select_drive
_cpm_select_drive:
    ld e, l
    ld c, #14
    jp 0x0005

