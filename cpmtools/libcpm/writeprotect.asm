.globl _cpm_write_protect_drive
_cpm_write_protect_drive:
    ld c, #28
    jp 0x0005
    