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
.globl _cpm_fastexit
_cpm_fastexit:
    ld sp, (oldstack)
    ret

.area _INITIALIZER

.area _DATA
.area _INITIALIZED
.area _BSEG
.area _BSS
.area _HEAP
.globl _cpm_ram
_cpm_ram = .

.area _BSS
oldstack: .ds 2
stack: .ds 128
stack_end:
