.sect .text
.sect .rom
.sect .data
.sect .bss


IOBANK              = 0x7f
IO_SELECT_VIDEO     = 0x04
VID_MODE            = 0x20
PIX_Y               = 0x21
PIX_X               = 0x22
PIX_DATA            = 0x24
VID_PAGE            = 0x25
PAL_COLOR           = 0x26
PAL_R               = 0x27
PAL_G               = 0x28
PAL_B               = 0x29

! Set the video mode
.sect .text
.define _set_vid_mode
_set_vid_mode:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out VID_MODE
    ret

! Set pixel X
.sect .text
.define _set_pixel_x
_set_pixel_x:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out PIX_X
    ret

! Set pixel Y
.sect .text
.define _set_pixel_y
_set_pixel_y:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out PIX_Y
    ret

! Set pixel data
.sect .text
.define _set_pixel_data
_set_pixel_data:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out PIX_DATA
    ret

! Set video page
.sect .text
.define _set_vid_page
_set_vid_page:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out VID_PAGE
    ret

! Set palette color
.sect .text
.define _set_pal_color
_set_pal_color:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out PAL_COLOR
    ret

! Set palette R
.sect .text
.define _set_pal_r
_set_pal_r:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out PAL_R
    ret

! Set palette G
.sect .text
.define _set_pal_g
_set_pal_g:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out PAL_G
    ret

! Set palette B
.sect .text
.define _set_pal_b
_set_pal_b:
    pop d
    pop h
    push h
    push d
    mvi a, IO_SELECT_VIDEO
    out IOBANK
    mov a,l
    out PAL_B
    ret

