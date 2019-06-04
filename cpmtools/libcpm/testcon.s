.globl _cpm_get_console_status
_cpm_get_console_status:
    ld c, #11
    jp 0x0005
    