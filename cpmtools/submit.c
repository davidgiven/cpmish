#include <cpm.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

static int lineno = 0;
static uint8_t buffer[128];
static uint8_t gargc;
static char** gargv;

static uint8_t* record_ptr;
static uint8_t record_fill;

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
        cpm_conout(b);
    }
}

static void print(const char* s)
{
    for (;;)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        cpm_conout(b);
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
            cpm_conout('0' + d);
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
    cpm_exit();
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
                uint8_t p = b - '0' + 1;
                if (p < gargc)
                {
                    char* param = gargv[p];
                    uint8_t len = strlen(param);
                    memcpy(&record_ptr[1 + record_fill], param, len);
                    record_fill += len;
                }
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

void main(int argc, const char* argv[])
{
    gargc = argc;
    gargv = argv;

    memcpy(&cpm_fcb.f[8], "SUB", 3);
    if (cpm_open_file(&cpm_fcb) == 0xff)
        fatal("could not open input file");

    record_ptr = cpm_ram;
    record_fill = 0;
    lineno = 1;
    for (;;)
    {
        uint8_t i;

        cpm_set_dma(&buffer);
        i = cpm_read_sequential(&cpm_fcb);
        if (i == 1) /* EOF */
            goto eof;
        if (i != 0)
            fatal("read error");

        for (i=0; i<128; i++)
        {
            uint8_t b = buffer[i];
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
    cpm_warmboot();
}

