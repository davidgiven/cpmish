#include <stdio.h>
#include <string.h>
#include <cpm.h>

#define ESC         0x1b
#define BELL        0x07
#define BACKSPACE   0x08
#define TAB         0x09
#define CR          0x0d
#define LF          0x0a
#define VT          0x0b
#define FF          0x0c
#define LOCAL_CMD   0x11   // ctrl+q
#define SOH         0x01
#define EOT         0x04
#define ACK         0x06
#define DLE         0x10
#define XON         0x11
#define XOFF        0x13
#define NAK         0x15
#define SYN         0x16
#define CAN         0x18
#define SUB         0x1a

// UART functions written in assembly
extern uint8_t uart_rx_avail(void);
extern uint8_t uart_getc(void);
extern void uart_putc_raw(uint8_t c);
extern uint8_t uart_getbaud(void);
extern void uart_setbaud_raw(uint8_t b);

uint8_t local_echo;
uint8_t baudrate;

static FCB xmodem_file;
static uint8_t xmodem_buffer[128]; 

void print(const char* s)
{
    for(;;) {
        uint8_t b = *s++;
        if(!b) return;
        cpm_conout(b);
    }
}


void crlf(void) 
{
    print("\r\n");
}

void printx(const char* s) {
    print(s);
    crlf();
}

void printhex4(uint8_t nibble) 
{
    nibble &= 0x0f;
    if(nibble < 10)
        nibble += '0';
    else
        nibble += 'a'-10;
    cpm_conout(nibble);
}

void printhex8(uint8_t b)
{
    printhex4(b>>4);
    printhex4(b);
}

/*uint8_t dummy(uint8_t b) {
    uint8_t x;
    x=b;
    return x;
}*/
// Wrapper which is only used since I for the life of me can't figure out
// the correct calling convention
/*void uart_putc(uint8_t b) {
    dummy(b);    
    uart_putc_raw(b);   
}

void uart_setbaud(uint8_t b) {
    if(b>5) b=5;
    
    dummy(b);
    uart_setbaud_raw(b);
}
*/

void print_settings(void) {
    printx("Current settings");    
    print("Local echo: ");
    if(!local_echo) {
        printx("OFF");
    } else { 
        printx("ON");
    }

    print("Baudrate: ");
    switch(baudrate) {
        case 0:
            print("4800");
            break;
        case 1:
            print("9600");
            break;
        case 2:
            print("19200") ;
            break;
        case 3:
            print("38400");
            break;
        case 4:
            print("57600");
            break;
        case 5:
            print("115200");
            break;
        default:
            print("Undefined");
            break;
    }
    crlf();
    crlf();
}

void set_baudrate(void) {
    uint8_t b;
    crlf();
    printx("Select new baudrate");    
    printx("1: 4800");    
    printx("2: 9600");    
    printx("3: 19200");   
    printx("4: 38400");  
    printx("5: 57600");    
    printx("6: 115200");
    b = cpm_conin();
    
    b=b-'1';
    if(b>5) {
        printx("Invalid selection");
        return;        
    }
    uart_setbaud_raw(b);
    baudrate = uart_getbaud();
    crlf();
    print_settings();
}

static uint8_t getblockchar(uint8_t *data) {
    uint8_t data_available;
    uint16_t i;
    for(i=0; i<30000; i++) {
        data_available = uart_rx_avail();
     
        if(data_available) {
            *data = uart_getc();
            break;        
        }
    }
    return data_available;
}

static void xmodem_receive(void) {
    char filename_input[14];
    uint8_t block_cnt;
    uint8_t block_exp = 1;
    uint8_t pos = 0;
    uint8_t checksum;
    uint8_t inp;
    uint8_t outp;
    uint8_t data_available;
    
    print("X modem receive");
    crlf();
    print("Enter filename: ");

    filename_input[0]=13;
    filename_input[1]=0;    
    cpm_readline((uint8_t *)filename_input);
    crlf();

    // Reset FCB
    memset(&xmodem_file, 0, sizeof(xmodem_file));
    
    // Parse filename
    cpm_parse_filename(&xmodem_file,&filename_input[2]);

    // Create file
    cpm_make_file(&xmodem_file);

    print("Waiting for sender");
    crlf();
    print("Press any key to cancel");
    crlf(); 
    outp = NAK;
    // Transmission
    while(1) {
        uart_putc_raw(outp);
        if(getblockchar(&inp)) {
            if(inp == EOT) {
                crlf();
                print("Transmission done");
                crlf();
                cpm_close_file(&xmodem_file);
                return;
            }
            if(inp == CAN) {
                crlf();
                print("Transmission cancelled");
                crlf();
                cpm_close_file(&xmodem_file);
                return;
            }
            if(inp == SOH) {
                // Got header, get package
                outp = NAK;
                checksum = 0;
                getblockchar(&inp);
                block_cnt = inp;
                getblockchar(&inp);
                if((block_cnt == (inp ^ 0xFF)) && (block_cnt == block_exp)) {
                    // Get block, otherwise retry
                    for(pos=0; pos<128; pos++) {
                        getblockchar(&inp);
                        xmodem_buffer[pos]=inp;
                        checksum += inp;                    
                    }
                    // Verify checksum
                    getblockchar(&inp);
                    if(checksum == inp) {
                        outp = ACK;
                        cpm_set_dma(&xmodem_buffer);
                        cpm_write_sequential(&xmodem_file);
                        //printi(block_cnt);    
                    }
                    block_exp++;
                }
            } 
        }
        cpm_conout('.');
        if(cpm_const()) {
            cpm_close_file(&xmodem_file);
            return;
        }
    }

}

static void xmodem_send_block(uint8_t block_cnt) {
    uint8_t i;
    uint8_t checksum;
    uint8_t data;
    
    // Print block number
    //printi(block_cnt);
    print(".");

    // Send header
    uart_putc_raw(SOH);
    uart_putc_raw(block_cnt);
    uart_putc_raw(block_cnt ^ 0xFF);

    checksum = 0;
    // Send data
    for(i=0; i<128; i++) {
        data = xmodem_buffer[i];
        checksum += data;
        uart_putc_raw(data);
    }

    // Send checksum
    uart_putc_raw(checksum);    
}

static void xmodem_send(void) {
    char filename_input[14];
    uint8_t block_cnt = 1;
    uint8_t pos = 0;
    uint8_t delay;
    uint8_t inp;
    uint8_t outp;
    uint8_t nak_cnt = 0;
    
    print("X modem send");
    crlf();
    print("Enter filename: ");

    filename_input[0]=13;
    filename_input[1]=0;    
    cpm_readline((uint8_t *)filename_input);
    crlf();

    // Reset FCB
    memset(&xmodem_file, 0, sizeof(xmodem_file));
    
    // Parse filename
    cpm_parse_filename(&xmodem_file,&filename_input[2]);
   
    // Open file
    if(cpm_open_file( &xmodem_file)) {
        print("Error opening file\r\n");
        return;
    }

    // Load first block
    cpm_set_dma(&xmodem_buffer);
    cpm_read_sequential(&xmodem_file);

    print("Waiting for receiver...");
    crlf();
    print("Press any key to cancel");
    crlf();

    while(1) {
        if(getblockchar(&inp)) {
            if(inp == NAK) {
                // Resend block
                xmodem_send_block(block_cnt);
                nak_cnt++;
                if(nak_cnt == 11) {
                    print("Too many NAKs, aborting");
                    crlf();
                    uart_putc_raw(CAN);
                    cpm_close_file(&xmodem_file);
                    return;
                }
            }
            if(inp == ACK) {
                cpm_conout('.');
                // Load next block
                cpm_set_dma(&xmodem_buffer);
                if(cpm_read_sequential(&xmodem_file)) {
                    crlf();
                    print("Transmission done");
                    crlf();
                    uart_putc_raw(EOT);
                    cpm_close_file(&xmodem_file);
                    return;
                }
                block_cnt++;
                // Send next block
                xmodem_send_block(block_cnt);
            }    
        }
        if(cpm_const()) {
            // Cancel due to keypress
            cpm_close_file(&xmodem_file);
            uart_putc_raw(CAN);
            return;
        }
    }
}


int main(void) {
    uint8_t run=1;
    uint8_t console_data;
    uint8_t uart_data;
    
    local_echo = 0;
    baudrate = uart_getbaud();

    printx("Nanoterm for the nano-z80");
    printx("Press ctrl-q + h for help");
    crlf();
    print_settings();

    while(run) {
        if(cpm_bios_const()) {
            console_data = cpm_bios_conin(); // No echo...
            if(console_data == LOCAL_CMD) {
                console_data = cpm_conin();
                switch(console_data) {
                    case 'q':
                    case 'Q':
                        // Quit
                        crlf();
                        printx("Goodbye!");                       
                        cpm_warmboot();
                        break;
                    case 'e':
                    case 'E':
                        // Toggle local echo
                        crlf();                        
                        print("Local echo ");
                        if(!local_echo) {
                            local_echo = 1;
                            printx("ON");
                        } else { 
                            local_echo = 0;
                            printx("OFF");
                        }
                        break;
                    case 'b':
                    case 'B':
                        set_baudrate();
                        break;
                    case 'p':
                    case 'P':
                        print_settings();
                        break;
                    case 's':
                    case 'S':
                        xmodem_send();
                        break;
                    case 'r':
                    case 'R':
                        xmodem_receive();
                        break;
                    case LOCAL_CMD:
                        uart_putc_raw(LOCAL_CMD);
                        break;
                    case 'h':
                    case 'H':
                        // Print help
                        crlf();                        
                        printx("Available commands:");
                        printx("Ctrl-q + q:      Quit");
                        printx("Ctrl-q + e:      Toggle local echo");
                        printx("Ctrl-q + b:      Set baudrate");
                        printx("Ctrl-q + p:      Print current settings");
                        printx("Ctrl-q + s:      Send file with X-modem");
                        printx("Ctrl-q + r:      Receive file with X-modem");
                        printx("Ctrl-q + ctrl+q: Send ctrl-q   ");
                        break;
                    default:
                    break; 
                }           
            } else {
                if(local_echo == 1) {
                    cpm_conout(console_data);
                }
              
                uart_putc_raw(console_data);
            }
        }
        if(uart_rx_avail()) {
                uart_data = uart_getc();
                cpm_conout(uart_data);  
        }

    }
}   
