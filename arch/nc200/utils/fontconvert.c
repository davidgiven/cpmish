/* Amstrad NC200 cpmish BIOS © 2019 David Given
 * This file is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "libbdf.h"
#define LINEHEIGHT 7

static void fatal(const char* msg, ...)
{
    va_list ap;
    va_start(ap, msg);

    fprintf(stderr, "error: ");
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");

    exit(1);
}

int main(int argc, const char* argv[])
{
    if (argc != 2)
        fatal("fontconvert <inputfile>");

    BDF* bdf = bdf_load(argv[1]);
    if (bdf->height != LINEHEIGHT)
        fatal("font is not 6x7");

    for (int c=32; c<128; c++)
    {
        Glyph* glyph = bdf->glyphs[c];

        /* The glyph data is a 7-element array of bytes. Each byte contains
         * one scanline, left justified. */

        uint64_t mask = 0;
        const uint8_t* p = glyph->data;

        int yy = 0;
        while (yy < LINEHEIGHT)
        {
            /* We assume the right-most column is blank, and so only store five bits. */

            mask = (mask << 5) | ((*p >> 3) & 0x1f);
            p++;
            yy++;
        }

        /* The encoding expects 8 5-bit values in five bytes, *left* justified. */
        while (yy < 8)
        {
            mask <<= 5;
            yy++;
        }

        printf("\tdb 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x ; char %d\n",
            (uint32_t)((mask >> 32) & 0xff),
            (uint32_t)((mask >> 24) & 0xff),
            (uint32_t)((mask >> 16) & 0xff),
            (uint32_t)((mask >> 8) & 0xff),
            (uint32_t)(mask & 0xff),
            c);
    }

    return 0;
}

// vim: ts=4 sw=4 et

