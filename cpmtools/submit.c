#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "libcpm.h"

static int lineno = 0;
static uint8_t input_buffer[128];
static char* parameter[10];
static uint8_t parameter_length[10];

static uint8_t* record_ptr = cpm_ram;
static uint8_t record_fill = 0;

static FCB out_fcb = {
    1, /* dr; drive A */
    "$$$     SUB"
};

void printn(const char* s, unsigned len)
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

static void printi(uint16_t v) 
{
	bool zerosup = true;
	uint16_t precision = 10000;
    while (precision)
    {
        uint8_t d = v / precision;
        v %= precision;
        precision /= 10;
        if ((d != 0) || (precision == 0) || !zerosup)
        {
            zerosup = false;
            putchar('0' + d);
        }
    }
}

static void fatal(const char* s)
{
    if (lineno != 0)
    {
        print("Error at line ");
        printi(lineno);
        print(": ");
    }
    else
        print("Error: ");

    printx(s);
    cpm_delete_file(&out_fcb);
    cpm_fastexit();
}

static void parse_parameters(void)
{
    char* inp = cpm_cmdline;
    char* const maxp = cpm_cmdline + cpm_cmdlinelen;
    uint8_t param = 0;

    while (inp != maxp)
    {
        while ((inp != maxp) && (*inp == ' '))
            inp++;
        parameter[param] = inp;

        while ((inp != maxp) && (*inp != ' '))
            inp++;
        parameter_length[param] = inp - parameter[param];
        param++;
    }

    while (param < 10)
    {
        parameter[param] = 0;
        parameter_length[param] = 0;
        param++;
    }
}

static void process_byte(uint8_t b)
{
    static bool escaped = false;
    static bool control = false;
    if (!escaped && (b == '$'))
    {
        escaped = true;
        return;
    }
    if (!control && (b == '^'))
    {
        control = true;
        return;
    }

    if (b == '\r')
    {
        record_ptr[0] = record_fill;
        while (record_fill != 127)
            record_ptr[1 + record_fill++] = '\0';

        record_ptr += 128;
        record_fill = 0;
    }
    else if (b != '\n')
    {
        if (escaped)
        {
            if (b == '$')
            {
                /* Just emit a $. */
            }
            else if (isdigit(b))
            {
                uint8_t p = b - '0';
                memcpy(&record_ptr[1 + record_fill], parameter[p], parameter_length[p]);
                record_fill += parameter_length[p];
                goto exit;
            }
            else
                fatal("bad escape character");
        }

        if (control)
        {
            if (!isalpha(b))
                fatal("bad control character");
            b = toupper(b) - '@';
        }

        record_ptr[1 + record_fill++] = b;
    }

exit:
    if (record_fill >= 127)
        fatal("line too long");

    escaped = false;
    control = false;
}

void main(void)
{
    parse_parameters();

    memcpy(&cpm_fcb.f[8], "SUB", 3);
    if (cpm_open_file(&cpm_fcb) == 0xff)
        fatal("could not open input file");

    lineno = 1;
    for (;;)
    {
        uint8_t i;

        cpm_set_dma(&input_buffer);
        i = cpm_read_sequential(&cpm_fcb);
        if (i == 1) /* EOF */
            goto eof;
        if (i != 0)
            fatal("read error");

        for (i=0; i<128; i++)
        {
            uint8_t b = input_buffer[i];
            if (b == 26)
                goto eof;
            process_byte(b);
        }
    }
eof:

    cpm_delete_file(&out_fcb);
    if (cpm_make_file(&out_fcb) == 0xff)
        fatal("could not open output file");

    while (record_ptr != cpm_ram)
    {
        record_ptr -= 128;
        cpm_set_dma(record_ptr);
        if (cpm_write_sequential(&out_fcb) == 0xff)
            fatal("error writing output file");
    }

    if (cpm_close_file(&out_fcb) == 0xff)
        fatal("error writing output file");

    /* Force a CP/M restart so the file gets invoked */
    cpm_exit();
}

