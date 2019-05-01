.area _CODE
__entry:
    ld (oldstack), sp
    ld sp, #stack_end
    jp gsinit

.area _GSINIT
gsinit:
    ld bc, #l__INITIALIZER
    ld a, b
    or a, c
    jr Z, gsinit_next
    ld de, #s__INITIALIZED
    ld hl, #s__INITIALIZER
    ldir
gsinit_next:
.area _GSFINAL
    call _main
    ld sp, (oldstack)
    ret

.area _INITIALIZER

.area _DATA
.area _INITIALIZED
.area _BSEG
.area _BSS
.area _HEAP

.area _DATA
oldstack: .ds 2
stack: .ds 64
stack_end:
