#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "libcpm.h"

void print(const char* s)
{
    for (;;)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        putchar(b);
    }
}

void crlf(void)
{
    print("\r\n");
}

void printx(const char* s)
{
    print(s);
    crlf();
}

void printhex4(uint8_t nibble)
{
	nibble &= 0x0f;
    if (nibble < 10)
        nibble += '0';
    else
        nibble += 'a' - 10;
    putchar(nibble);
}

void printhex8(uint8_t b)
{
    printhex4(b >> 4);
    printhex4(b);
}

void printhex24(uint32_t n)
{
    printhex8(n >> 16);
    printhex8(n >> 8);
    printhex8(n);
}

void main(void)
{
    uint32_t address;

    cpm_fcb.cr = 0;
    if (cpm_open_file(&cpm_fcb) == 0xff)
    {
        printx("Cannot open source file");
        return;
    }

    address = 0;
    for (;;)
    {
        const uint8_t* data;
        uint8_t i;
        uint8_t j;
        uint8_t b;

        if (cpm_read_sequential(&cpm_fcb) != 0)
            break;
        
        data = (const uint8_t*)0x80;
        for (i=0; i<8; i++)
        {
			printhex24(address);
			print(" : ");
            address += 16;

            for (j=0; j<16; j++)
            {
                b = *data++;
                printhex8(b);
                putchar(' ');
            }
            data -= 16;

            print(": ");
            for (j=0; j<16; j++)
            {
                b = *data++;
                putchar(isprint(b) ? b : '.');
            }

            crlf();
            if (cpm_get_console_status())
                return;
        }
    }
}
