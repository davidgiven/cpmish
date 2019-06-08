.globl _putchar
_putchar:
    pop bc
    pop de
    push de
    push bc
    push de
    ld c, #2
    call 0x0005
    pop hl
    ret
