textout:     equ 0xb81e
diskservice: equ 0xba5e
kmwaitkbd:   equ 0xb80f

org 0x4000

main:
    ld c, 0x30
    call diskservice
loop:
    ld hl, .str.hello_world
    call textout
    call kmwaitkbd
    ret

.str.hello_world: db "Hello, world! ", 0
