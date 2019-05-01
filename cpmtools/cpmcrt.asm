.area _CODE
__entry:
    ld sp, #__stack_end
    jp _main

.area _DATA
__stack: .ds 64
__stack_end:
