#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "libcpm.h"

static uint8_t accumulator[4];
static uint8_t ibp = 0;
static DPB* dpb;
static uint16_t bpb; /* bytes per block */

static const uint8_t devices[] = "CON:RDR:PUN:LST:DEV:VAL:USR:DSK:";

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

static void printip(uint16_t v, bool pad, uint16_t precision)
{
    bool zerosup = true;
    while (precision)
    {
        uint8_t d = v / precision;
        v %= precision;
        precision /= 10;
        if (precision && zerosup && !d)
        {
            if (pad)
                putchar(' ');
        }
        else
        {
            zerosup = false;
            putchar('0' + d);
        }
    }
}

static void printi(uint16_t v)
{
    printip(v, false, 10000);
}

/* Compares the accumulator with an array of uint8_t[4] words. Returns the
 * matching index plus one or 0. */
static uint8_t compare_accumulator(const uint8_t* list, uint8_t length)
{
    uint8_t match = 1;
    while (length--)
    {
        uint8_t j;
        for (j=0; j<4; j++)
            if (*list++ != accumulator[j])
                break;
        if (j == 4)
        {
            /* Matched! */
            return match;
        }
        match++;
    }
    return 0;
}

static void select_disk(uint8_t d)
{
    cpm_select_drive(d);
    dpb = cpm_get_dpb();
    bpb = 1<<(dpb->bsh+7);
}

static void select_fcb_disk(void)
{
    if (fcb.dr)
        select_disk(fcb.dr-1);
}

static uint16_t count_space(bool all)
{
    uint8_t* alloca = cpm_get_allocation_vector();
    uint16_t blocks = 0;
    for (uint16_t i=0; i<=dpb->dsm; i++)
    {
        bool bit = false;
        if (!all)
            bit = alloca[i >> 3] & (0x80 >> (i & 7));
        if (!bit)
            blocks++;
    }
    return blocks;
}

static void print_free_space(void)
{
    uint16_t blocks = count_space(false);
    printi(blocks << (dpb->bsh - 3));
    print("/");
    printi((dpb->dsm+1) << (dpb->bsh - 3));
    print("kB");
}

/* Show status of all drives. */
static void print_drive_status(void)
{
    uint16_t login = cpm_get_login_vector();
    uint16_t rodisk = cpm_get_rodisk_vector();

    uint8_t d = 0;
    while (login)
    {
        if (login & 1)
        {
            select_disk(d);
            putchar('A' + d);
            print(": R/");
            putchar((rodisk & 1) ? 'O' : 'W');
            print(", space: ");
            print_free_space();
            crlf();
        }

        login >>= 1;
        rodisk >>= 1;
        d++;
    }
}

/* Reads the next input token into the 4-byte accumulator. */
static void scan(void)
{
    /* Skip whitespace. */

    while (cmdline[ibp] == ' ')
        ibp++;

    uint8_t obp = 0;
    while (obp < 4)
    {
        uint8_t b = cmdline[ibp];
        accumulator[obp++] = (b>1) ? b : ' ';
        switch (b)
        {
            case 0:
            case 1:
            case ',':
            case ':':
            case '*':
            case '.':
            case '>':
            case '<':
            case '=':
                /* Blanks the current char and does not advance,
                 * so the rest of the accumulator is filled with
                 * spaces. */
                cmdline[ibp] = 1;
                break;

            default:
                ibp++;
        }
    }
    ibp++;
}

static void printipadded(uint16_t value)
{
    printip(value, true, 10000);
}

static void get_detailed_drive_status(void)
{
    print("    ");
    putchar(cpm_get_current_drive() + 'A');
    putchar(':');
    printx(" Drive Characteristics");

    uint16_t rpb = 1<<dpb->bsh;
    uint16_t rpd = (dpb->dsm+1) * rpb;
    if ((rpd == 0) && (rpb != 0))
        print("65536");
    else
        printipadded(rpd);
    printx(": 128 byte record capacity");

    printipadded(count_space(false));
    printx(": kilobyte drive capacity");

    printipadded(dpb->drm+1);
    printx(": 32 byte directory entries");

    printipadded(dpb->cks);
    printx(": checked directory entries");

    printipadded((dpb->exm+1)*128);
    printx(": records per extent");

    printipadded(dpb->spt);
    printx(": sectors per track");

    printipadded(dpb->off);
    printx(": reserved tracks");
}

static void getfile(void)
{
    printx("unsupported getfile");
}

static void set_drive_status(void)
{
    scan(); /* drive is already in fcb.dr */
    scan(); /* read = */
    if (accumulator[0] == '=')
    {
        scan(); /* get assignment */
        if (compare_accumulator("R/O ", 1))
        {
            select_fcb_disk();
            cpm_write_protect_drive();
        }
        else
            printx("Invalid disk assignment (only R/O is supported)");
    }
    else
    {
        /* Not a disk assignment --- the user must be trying to stat a file. */

        select_fcb_disk();

        if (compare_accumulator("DSK:", 1))
            get_detailed_drive_status();
        else
            getfile();
    }
}

static bool devreq(void)
{
    printx("unsupported devreq");
    return false;
}

void main(void)
{
    if (!fcb.dr && (fcb.f[0] == ' '))
        print_drive_status();
    else if (fcb.dr)
        set_drive_status();
    else if (!devreq())
        getfile();
}
