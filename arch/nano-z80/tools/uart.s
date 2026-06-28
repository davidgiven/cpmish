.sect .text
.sect .rom
.sect .data
.sect .bss


IOBANK              = 0x7f
IO_SELECT_UART      = 0x05
UART_B_TX_DATA      = 0x04
UART_B_TX_READY     = 0x05
UART_B_RX_DATA      = 0x06
UART_B_RX_AVAIL     = 0x07
UART_B_BAUD         = 0x08

! Check if there is RX data available, return 1 if there is, 0 if not
.sect .text
.define _uart_rx_avail
_uart_rx_avail:
    mvi a, IO_SELECT_UART 
	out IOBANK
    in UART_B_RX_AVAIL
    mvi d, 0
    mov e, a
    ret
        
! Get a byte from the serial port
.sect .text
.define _uart_getc
_uart_getc:
    mvi a, IO_SELECT_UART 
	out IOBANK
    in UART_B_RX_DATA
    mvi d,0
    mov e,a
    ret
       
! Send a byte to the serial port, blocking
.sect .text
.define _uart_putc_raw
_uart_putc_raw:
    pop d                       ! pop return address
    pop h                       ! pop character to send
    
    !xthl                        ! L = character to send
    push h
    push d
    mvi a, IO_SELECT_UART 
	out IOBANK
uart_send_loop:
    in UART_B_TX_READY
    cpi 0
    jz uart_send_loop
    mov a,l
    out UART_B_TX_DATA
    ret

! Get the current baudrate
.sect .text
.define _uart_getbaud
_uart_getbaud:
    mvi a, IO_SELECT_UART 
	out IOBANK
    in UART_B_BAUD
    mvi d,0
    mov e,a
    ret

 ! Set the baudrate
.sect .text
.define _uart_setbaud_raw
_uart_setbaud_raw:
    pop d                       ! pop return address
    pop h                       ! pop setting
    
    !xthl                        ! L = setting
    push h
    push d
    mvi a, IO_SELECT_UART 
	out IOBANK
    mov a,l
    out UART_B_BAUD
    ret


