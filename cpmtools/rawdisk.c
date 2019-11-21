/* rawdisk © 2019 David Given
 * This program is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static const char** gargv;
static uint8_t drive;
static uint8_t mode;
static uint32_t startsector;
static uint32_t sectorcount;
static DPH* dph;
static DPB* dpb;
static uint16_t spt;
static uint8_t olddrive;

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

void printfcb(FCB* fcb)
{
    const uint8_t* inp;

    if (fcb->dr)
    {
        cpm_conout('@' + fcb->dr);
        cpm_conout(':');
    }

    inp = &fcb->f[0];
    while (inp != &fcb->f[12])
    {
        uint8_t c;
        if (inp == &fcb->f[8])
            cpm_conout('.');
        c = *inp++;
        if (c != ' ')
            cpm_conout(c);
    }
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

void printhex16(uint16_t n)
{
    printhex8(n >> 8);
    printhex8(n);
}

/* 
 * Prints a 32-bit decimal number with optional left padding and configurable
 * precision. *.
 */
void printip(uint32_t v, bool pad, uint32_t precision)
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
                cpm_conout(' ');
        }
        else
        {
            zerosup = false;
            cpm_conout('0' + d);
        }
    }
}

void printi(uint32_t v)
{
    printip(v, false, 1000000000LU);
}

void fatal(const char* s) 
{
    print("Error: ");
	printx(s);
	cpm_exit();
}

void help(void)
{
    fatal("syntax: rawdisk <file> <drive> [r|w] <startsector> <count>");
}

void parse_options(void)
{
    cpm_parse_filename(&cpm_fcb, gargv[1]);
    if (cpm_fcb.f[0] == ' ')
        help();

    drive = gargv[2][0] - 'A';
    if (drive >= 15)
        help();

    mode = gargv[3][0];
    if ((mode != 'R') && (mode != 'W'))
        help();

    startsector = strtoul(gargv[4], NULL, 0);
    sectorcount = strtoul(gargv[5], NULL, 0);
}

DPH* begin_seldsk(uint8_t biosdrive)
{
    olddrive = cpm_get_current_drive();
    return cpm_bios_seldsk(biosdrive);
}

void end_seldsk(void)
{
    cpm_bios_seldsk(olddrive);
}

void main(int argc, const char* argv[])
{
    uint8_t r;

    if (argc != 6)
        help();

    gargv = argv;
    parse_options();

    print((mode == 'R') ? "Reading" : "Writing");
    print(" drive ");
    cpm_conout('A' + drive);
    print(" to/from ");
    printfcb(&cpm_fcb);
    crlf();
    print("Start sector: ");
    printi(startsector);
    crlf();
    print("Sector count: ");
    printi(sectorcount);
    crlf();

    dph = begin_seldsk(drive);
    end_seldsk();
    if (!dph)
        fatal("that drive does not exist");
    dpb = (DPB*) U16(dph->dpb);
    spt = U16(dpb->spt);

    if (mode == 'R')
    {
        cpm_delete_file(&cpm_fcb);
        r = cpm_make_file(&cpm_fcb);
    }
    else
        r = cpm_open_file(&cpm_fcb);
    if (r == 0xff)
        fatal("cannot open file");

    {
        static uint16_t track;
        static uint16_t sector;

        track = startsector / spt;
        sector = startsector % spt;

        r = 0;
        while (sectorcount--)
        {
            if (mode == 'W')
            {
                cpm_set_dma(cpm_default_dma);
                r = cpm_read_sequential(&cpm_fcb);
                if (r)
                    break;
            }

            begin_seldsk(drive);
            cpm_bios_setdma(cpm_default_dma);
            cpm_bios_settrk(track);
            cpm_bios_setsec(sector);
            r = (mode == 'R') ? cpm_bios_read() : cpm_bios_write();
            end_seldsk();
            if (r)
                break;

            if (mode == 'R')
            {
                cpm_set_dma(cpm_default_dma);
                r = cpm_write_sequential(&cpm_fcb);
                if (r)
                    break;
            }

            sector++;
            if (sector == spt)
            {
                track++;
                sector = 0;
            }
        }

        if (r)
            printx("Disk error");
    }

    cpm_close_file(&cpm_fcb);
}
