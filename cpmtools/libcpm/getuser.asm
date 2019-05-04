.globl _cpm_get_current_user
_cpm_get_current_user:
    ld c, #32
    ld e, #0xff
    jp 0x0005
    