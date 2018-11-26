#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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
    int charswidth = widthpixels / 6;
    if ((heightpixels % 8) != 0)
        fatal("image must be a multiple of 8 pixels high");
    int charsheight = heightpixels / 8;
    printf("; font is %dx%d pixels, containing %d characters\n",
        widthpixels, heightpixels, charswidth*charsheight);

    int c = 32;
    for (int y=0; y<charsheight; y++)
    {
        for (int x=0; x<charswidth; x++)
        {
            unsigned char* p = data + x*6 + y*widthpixels*8;

            /* We assume the left column is blank, and so only store five bits. */
            p++;

            uint64_t mask = 0;

            for (int yy=0; yy<8; yy++)
            {
                for (int xx=0; xx<5; xx++)
                    mask = (mask << 1) | (p[xx] == 0);
                p += widthpixels;
            }

            printf("db 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x ; char %d\n",
                (mask >> 32) & 0xff,
                (mask >> 24) & 0xff,
                (mask >> 16) & 0xff,
                (mask >> 8) & 0xff,
                mask & 0xff,
                c);
            c++;
        }
    }

    stbi_image_free(data);

    return 0;
}
