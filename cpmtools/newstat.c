#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "libcpm.h"

static uint8_t ibp = 1;
static DPB* dpb;
static uint16_t bpb; /* bytes per block */

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

static void printi(uint16_t v)
{
    uint16_t k = 10000;
    bool zero = false;
    while (k)
    {
        uint8_t d = v / k;
        v %= k;
        k /= 10;
        if (zero || !k || d)
        {
            zero = true;
            putchar('0' + d);
        }
    }
}

static void crlf(void)
{
    print("\r\n");
}

static void select_disk(uint8_t d)
{
    cpm_select_disk(d);
    dpb = cpm_get_dpb();
    bpb = 1<<(dpb->bsh+7);
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

static void setdrivestatus(void)
{
    puts("unsupported setdrivestatus");
}

static bool devreq(void)
{
    puts("unsupported devreq");
    return false;
}

static void getfile(void)
{
    puts("unsupported getfile");
}

void main(void)
{
    if (!fcb.dr && (fcb.f[0] == ' '))
        print_drive_status();
    else if (fcb.dr)
        setdrivestatus();
    else if (!devreq())
        getfile();
}
