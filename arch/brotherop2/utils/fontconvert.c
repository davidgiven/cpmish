/* Amstrad Brother OP2 cpmish BIOS © 2019 David Given
 * This file is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 7

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

    int widthpixels, heightpixels, depth;
    unsigned char* data = stbi_load(argv[1], &widthpixels, &heightpixels, &depth, 1);

    if (depth != 1)
        fatal("image must be greyscale");
    if ((widthpixels % 6) != 0)
        fatal("image must be a multiple of 6 pixels across");
    int charswidth = widthpixels / CHAR_WIDTH;
    if ((heightpixels % CHAR_HEIGHT) != 0)
        fatal("image must be a multiple of %d pixels high", CHAR_HEIGHT);
    int charsheight = heightpixels / CHAR_HEIGHT;
    printf("\t; font is %dx%d pixels, containing %d characters\n",
        widthpixels, heightpixels, charswidth*charsheight);

    int c = 32;
    for (int y=0; y<charsheight; y++)
    {
        for (int x=0; x<charswidth; x++)
        {
            unsigned char* p = data + x*CHAR_WIDTH + y*widthpixels*CHAR_HEIGHT;

			printf("\tdb ");
            int yy = 0;
            while (yy < CHAR_HEIGHT)
            {
				if (yy != 0)
					printf(", ");

				uint8_t mask = 0;
                for (int xx=0; xx<CHAR_WIDTH; xx++)
                    mask = (mask << 1) | (p[xx] == 0);
				printf("0x%02x", mask);
                p += widthpixels;
                yy++;
            }

			printf(" ; char %d\n", c);
            c++;
        }
    }

    stbi_image_free(data);

    return 0;
}
