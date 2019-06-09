#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "libcpm.h"

uint8_t* cmdline_cursor;
uint8_t* cmdline_limit;
uint8_t* buffer_start;
bool erase_destination = false;
bool only_one_record = false;

FCB src_fcb;
FCB dest_fcb;

static void printn(const char* s, unsigned len)
{
	while (len--)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        putchar(b);
    }
}

static void print(const char* s) 
{
    for (;;)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        putchar(b);
    }
}

static void crlf(void)
{
    print("\r\n");
}

static void printx(const char* s) 
{
    print(s);
    crlf();
}

void fatal(const char* s) 
{
    print("Error: ");
	printx(s);
	cpm_exit();
}

void syntax_error(void)
{
    fatal("syntax error");
}

void cant_use_wildcards(void)
{
    fatal("you can't use wildcards in this mode");
}

void abort(void)
{
    fatal("user cancel");
}

void help(void)
{
    printx("Syntax: copy [/uf] [<inputspec...>] <outputspec>");
    printx("Options:");
    printx("  F: overwrite output");
    printx("  U: unbuffered");
    printx("<inputspec> may contain wildcards. <outputspec> may be just a drive.");
    cpm_exit();
}

void skip_whitespace(void)
{
    while (cmdline_cursor != cmdline_limit)
    {
        uint8_t c = *cmdline_cursor;
        if (c != ' ')
            return;
        cmdline_cursor++;
    }
}

void parse_options(void)
{
    uint8_t b;

    skip_whitespace();
    b = *cmdline_cursor;
    if ((b != '-') && (b != '/'))
        return;
    cmdline_cursor++;

    while (cmdline_cursor != cmdline_limit)
    {
        b = *cmdline_cursor;
        if (b == ' ')
            return;
        cmdline_cursor++;

        switch (b)
        {
            case 'F': erase_destination = true; break;
            case 'U': only_one_record = true; break;
            case '?': case 'H': help();
            default:
                fatal("invalid option");
        }
    }
}

void print_fcb(const FCB* fcb)
{
    uint8_t i;
    const uint8_t* p;

    putchar('@' + fcb->dr);
    putchar(':');

    p = fcb->f;
    for (i=0; i<8; i++)
    {
        uint8_t b = *p++;
        if (b == ' ')
            break;
        putchar(b);
    }

    if (fcb->f[8] == ' ')
        return;

    putchar('.');
    p = fcb->f+8;
    for (i=0; i<3; i++)
    {
        uint8_t b = *p++;
        if (b == ' ')
            break;
        putchar(b);
    }
}

const uint8_t* read_fcb(const uint8_t* inp, FCB* fcb)
{
    uint8_t* outp;
    uint8_t b;

    memset(fcb, 0, sizeof(FCB));
    memset(fcb->f, ' ', sizeof(fcb->f));
    if (inp[1] == ':')
    {
        /* There's a drive letter. */
        fcb->dr = inp[0] - '@';
        inp += 2;
        if (inp >= cmdline_limit)
            return inp;
    }
    else
        fcb->dr = cpm_get_current_drive() + 1;

    /* Read left side of filename. */

    outp = fcb->f;
    for (;;)
    {
        if (inp == cmdline_limit)
            break;
        
        b = *inp++;
        if (b == ' ') 
            return inp;
        if (b == '.')
            break;
        if (b == '*')
        {
            while (outp != (fcb->f + 8))
                *outp++ = '?';
        }
        if (outp != (fcb->f + 8))
            *outp++ = b;
    }

    /* Read right side of filename. */

    outp = fcb->f + 8;
    for (;;)
    {
        if (inp == cmdline_limit)
            break;

        b = *inp++;
        if (b == ' ')
            return inp;
        if (b == '*')
        {
            while (outp != (fcb->f + 11))
                *outp++ = '?';
        }
        if (outp != (fcb->f + 11))
            *outp++ = b;
    }

    return inp;
}

void parse_destination(void)
{
    uint8_t* p = cmdline_limit - 1;
    for (;;)
    {
        uint8_t b = *p;
        if (b == ' ')
            break;
        p--;
        if (p == cmdline_cursor)
            syntax_error();
    }
    p++;

    read_fcb(p, &dest_fcb);
    cmdline_limit = p;
}

bool does_fcb_have_wildcards(const FCB* fcb)
{
    uint8_t i = 11;
    while (--i)
    {
        if (fcb->f[i] == '?')
            return true;
    }
    return false;
}

void copy_one_file(FCB* src, FCB* dest)
{
    bool destexists;
    uint8_t* maxp;
    uint8_t* readp;
    uint8_t* writep;
    bool eof = false;
    uint8_t i;
    
    maxp = buffer_start + (only_one_record ? 128 : ((cpm_ramtop - buffer_start + 1) & ~127));

    print_fcb(src);
    print(" -> ");
    print_fcb(dest);
    print(": ");

    destexists = cpm_findfirst(dest) != 0xff;
    if (destexists)
    {
        if (erase_destination)
        {
            if (cpm_findfirst(dest) != 0xff)
            {
                if (cpm_delete_file(dest) == 0xff)
                    fatal("cannot erase destination file");
            }
        }
        else
            fatal("destination file exists");
    }
    
    if (cpm_open_file(src) == 0xff)
        fatal("cannot open source file");
    src->cr = 0;
    if (cpm_make_file(dest) == 0xff)
        fatal("cannot open destination file");

    while (!eof)
    {
        uint8_t count;

        /* Read phase. */

        readp = buffer_start;
        count = 0;
        while (readp != maxp)
        {
            cpm_set_dma(readp);
            i = cpm_read_sequential(src);
            if (i == 1) /* EOF */
            {
                eof = true;
                break;
            }
            if (i != 0)
                fatal("error on read");

            readp += 128;
            if ((count & 3) == 0)
                putchar('r');
            count++;

            if (cpm_get_console_status())
                abort();
        }

        /* Write phase */

        writep = buffer_start;
        count = 0;
        while (writep != readp)
        {
            cpm_set_dma(writep);
            i = cpm_write_sequential(dest);
            if (i != 0)
                fatal("error on write");

            writep += 128;

            if ((count & 3) == 0)
                putchar('w');
            count++;

            if (cpm_get_console_status())
                abort();
        }
    }

    crlf();
    if (cpm_close_file(dest) == 0xff)
        fatal("failed to close output file");
}

void multicopy(void)
{
    DIRE* dmabuf = (DIRE*) buffer_start;
    FCB* const fcbtab_start = (FCB*) (buffer_start + 128);
    FCB* fcbtab_max = fcbtab_start;
    FCB* fcbtab_ptr = fcbtab_start;
    uint8_t i;

    for (;;)
    {
        skip_whitespace();
        if (cmdline_cursor == cmdline_limit)
            break;

        cmdline_cursor = read_fcb(cmdline_cursor, &cpm_fcb);
        cpm_set_dma(dmabuf);
        i = cpm_findfirst(&cpm_fcb);
        while (i != 0xff)
        {
            DIRE* de = &dmabuf[i];
            memset(fcbtab_max, 0, sizeof(FCB));
            fcbtab_max->dr = cpm_fcb.dr;
            for (i=0; i<11; i++)
                fcbtab_max->f[i] = de->f[i] & 0x7f;
            fcbtab_max++;
            i = cpm_findnext(&cpm_fcb);
        }
    }

    if (fcbtab_max == fcbtab_start)
        fatal("nothing to copy");

    buffer_start = (uint8_t*) fcbtab_max;
    while (fcbtab_ptr != fcbtab_max)
    {
        memcpy(&cpm_fcb, &dest_fcb, sizeof(FCB));
        memcpy(cpm_fcb.f, fcbtab_ptr->f, sizeof(cpm_fcb.f));
        copy_one_file(fcbtab_ptr, &cpm_fcb);
        fcbtab_ptr++;
    }
}

void singlecopy(void)
{
    skip_whitespace();
    cmdline_cursor = read_fcb(cmdline_cursor, &src_fcb);
    skip_whitespace();
    if (cmdline_cursor != cmdline_limit)
        fatal("only one source allowed if a filename is specified as destination");

    if (does_fcb_have_wildcards(&src_fcb) || does_fcb_have_wildcards(&dest_fcb))
        cant_use_wildcards();

    copy_one_file(&src_fcb, &dest_fcb);
}

void main(void)
{
    cmdline_limit = cpm_cmdline + cpm_cmdlinelen;
    cmdline_cursor = cpm_cmdline;
    buffer_start = cpm_ram;

    parse_options();
    if (*cmdline_cursor == '-')
        parse_options();
    skip_whitespace();

    parse_destination();
    if (dest_fcb.f[0] == ' ')
    {
        /* Destination is a drive spec. */
        multicopy();
    }
    else
    {
        /* Copy a single file. */
        singlecopy();
    }

    cpm_exit();
}
