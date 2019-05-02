.globl _cpm_get_current_drive
_cpm_get_current_drive:
    ld c, #25
    jp 0x0005
    