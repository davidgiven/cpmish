/* mkfs © 2019 David Given
 * This program is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static DPH* dph;
static DPB* dpb;
static uint16_t spt;
static uint16_t reservedtracks;
static uint16_t directoryblocks;
static uint16_t blocksize;
static uint16_t blockcount;

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

void main(void)
{
    if (!cpm_fcb.dr || (cpm_fcb.f[0] != ' '))
        fatal("syntax: mkfs <drive>");

    cpm_overwrite_ccp(); /* to use cpm_bios_seldsk */
    dph = cpm_bios_seldsk(cpm_fcb.dr - 1);
    if (!dph)
        fatal("that drive does not exist");
    dpb = (DPB*) U16(dph->dpb);
    spt = U16(dpb->spt);
    blocksize = 1<<(dpb->bsh+7);
    blockcount = U16(dpb->dsm) + 1;
    reservedtracks = U16(dpb->off);
    directoryblocks = (U16(dpb->drm)+1) * 32 / blocksize;

    print("Drive ");
    cpm_conout(cpm_fcb.dr + '@');
    printx(":");
    print("  Sectors per track:          ");
    printi(spt);
    crlf();
    print("  Number of reserved tracks:  ");
    printi(reservedtracks);
    crlf();
    print("  Number of directory blocks: ");
    printi(directoryblocks);
    crlf();
    print("  Size of block:              ");
    printi(blocksize);
    printx(" bytes");
    print("  Number of blocks:           ");
    printi(blockcount);
    crlf();
    print("  Total disk size:            ");
    printi((uint32_t)blockcount * (uint32_t)blocksize/1024 + reservedtracks*spt/8);
    printx(" kB");
    crlf();

    print("About to create a filesystem on drive ");
    cpm_conout(cpm_fcb.dr + '@');
    printx(", destroying everything on it.");
    printx("Press Y to proceed, anything else to cancel:");
    if (cpm_conio(0xfd) != 'y')
        fatal("Aborted.");

    printx("Formatting now...");

    cpm_bios_setdma(cpm_default_dma);
    memset(cpm_default_dma, 0xe5, 128);

    {
        uint16_t track = reservedtracks;
        uint16_t sector = 0;

        while (directoryblocks--)
        {
            uint8_t i;
            for (i=0; i<blocksize/128; i++)
            {
                cpm_bios_settrk(track);
                cpm_bios_setsec(sector);
                if (cpm_bios_write(0))
                    fatal("Disk error");

                sector++;
                if (sector == spt)
                {
                    sector = 0;
                    track++;
                }
            }
        }
    }

    printx("Done.");
}
