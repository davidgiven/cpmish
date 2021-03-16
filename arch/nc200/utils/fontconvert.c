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

		printf("\tdb ");
		int yy = 0;
		const uint8_t* p = glyph->data;
		while (yy < LINEHEIGHT)
		{
			if (yy != 0)
				printf(", ");
			printf("0x%02x", *p);
			p++;
			yy++;
		}
		printf(" ; char %d\n", c);
    }

    return 0;
}
