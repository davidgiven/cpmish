#include <stdio.h>
#include <stdbool.h>
#include <cpm.h>

enum
{
    CISTPL_VERS_1 = 0x15,
    CISTPL_FUNCID = 0x21,
    CISTPL_FUNCE = 0x22
};

extern uint8_t read_attr_byte(uint16_t offset);

uint16_t attrptr = 0;
uint8_t buffer[512];

void print(const char* s)
{
    for (;;)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        cpm_conout(b);
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
    cpm_conout(nibble);
}

void printhex8(uint8_t b)
{
    printhex4(b >> 4);
    printhex4(b);
}

bool read_attr(void)
{
    uint8_t link;
    uint8_t* outptr;

    buffer[0] = read_attr_byte(attrptr+0);
    link = buffer[1] = read_attr_byte(attrptr+1);
    if ((buffer[0] == 0xff) || (link == 0xff))
        return false;
    attrptr += 2;

    outptr = buffer+2;
    while (link--)
        *outptr++ = read_attr_byte(attrptr++);
    return true;
}

bool find_attr(uint8_t code)
{
    for (;;)
    {
        if (!read_attr())
            return false;

        if (buffer[0] == code)
            return true;
    }
}

void bad_card(void)
{
    printx("No ATA card found");
    cpm_exit();
}

void main(void)
{
    /* Find and print the name, if there is one. */

    if (find_attr(CISTPL_VERS_1))
    {
        print("Found: ");
        print((char*)buffer + 4);
        print(" V");
        printhex8(buffer[2]);
        cpm_conout('.');
        printhex8(buffer[3]);
        crlf();
    }

    /* Check this is a disk card. */

    attrptr = 0;
    if (!find_attr(CISTPL_FUNCID))
        bad_card();
    if (buffer[2] != 0x04 /* fixed disk */)
        bad_card();

    /* Now find a FUNCE indicating this is a ATA card. */

    for (;;)
    {
        if (!read_attr())
            bad_card();
        if (buffer[0] == CISTPL_FUNCID)
            bad_card(); /* Multi-function cards don't work */
        if ((buffer[0] == CISTPL_FUNCE) && ((buffer[2] == 0x01) || (buffer[2] == 0x02)))
            break;
    }
    printx("This is an ATA flash card.");
}
