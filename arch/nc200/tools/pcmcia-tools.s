! Declare segments (the order is important).

.sect .text
.sect .rom
.sect .data
.sect .bss

! Keep this in sync with ../supervisor/banking.inc and ../include/nc200.lib.

PORT_BANK3      = 0x13 ! port number
PORT_BAUDRATE   = 0x30 ! port number

USER3_BANK      = 0x47 ! bank number
CARD0_BANK      = 0x80 ! bank number

.sect .text
read_byte:
    ! HL = card offset, B = common/attribute
    
    dad h                   ! hl = hl * 2
    lxi d, 0xc000
    dad d                   ! hl = absolute address

    di
    mvi a, CARD0_BANK
    out PORT_BANK3          ! 0xc000 is now pointing at card memory
    mov a, b
    out PORT_BAUDRATE       ! PCMCIA reading attribute memory
    mov e, m                ! read byte
    mvi d, 0
    mvi a, USER3_BANK
    out PORT_BANK3          ! 0xc000 is now pointing at user space
    ei

    ret

.sect .text
.define _read_attr_byte
_read_attr_byte:
    pop d                   ! pop return address
    pop h                   ! hl = card offset
    push h
    push d
    push b                  ! save frame pointer

    mvi b, 0x00
    call read_byte

    pop b
    ret

.sect .text
.define _read_common_byte
_read_common_byte:
    pop d                   ! pop return address
    pop h                   ! hl = card offset
    push h
    push d
    push b                  ! save frame pointer

    mvi b, 0x80
    call read_byte

    pop b
    ret
