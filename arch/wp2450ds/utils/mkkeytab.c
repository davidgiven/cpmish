/* Amstrad NC200 cpmish BIOS © 2019 David Given
 * This file is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <stdio.h>

static unsigned char normaltab[64] = {};
static unsigned char shiftedtab[64] = {};

static void key(int keycode, unsigned char normal, unsigned char shifted)
{
    normaltab[keycode] = normal;
    shiftedtab[keycode] = shifted;
}

int main(int argc, const char* argv[])
{
    key(0x36, '1', '!');
    key(0x35, '2', '"');
    key(0x25, '3', '#');
    key(0x26, '4', '$');
    key(0x1e, '5', '%');
    key(0x1d, '6', '^');
    key(0x15, '7', '&');
    key(0x16, '8', '*');
    key(0x0e, '9', '(');
    key(0x05, '0', ')');
    key(0x06, '-', '_');
    key(0x2e, '=', '+');
    key(0x3c, 127, 127);

    key(0x20, 9,   9);
    key(0x37, 'q', 'Q');
    key(0x27, 'w', 'W');
    key(0x24, 'e', 'E');
    key(0x1f, 'r', 'R');
    key(0x1c, 't', 'T');
    key(0x17, 'y', 'Y');
    key(0x0c, 'u', 'U');
    key(0x0f, 'i', 'I');
    key(0x04, 'o', 'O');
    key(0x07, 'p', 'P');

    key(0x30, 0,   0); // caps lock/control
    key(0x31, 'a', 'A');
    key(0x32, 's', 'S');
    key(0x21, 'd', 'D');
    key(0x1a, 'f', 'F');
    key(0x11, 'g', 'G');
    key(0x14, 'h', 'H');
    key(0x0d, 'j', 'J');
    key(0x2c, 'k', 'K');
    key(0x2d, 'l', 'L');
    key(0x2b, 39,  '@');
    key(0x3f, 13,  13); // return

    key(0x38, 0,   0); // left shift
    key(0x34, 'z', 'Z');
    key(0x22, 'x', 'X');
    key(0x19, 'c', 'C');
    key(0x12, 'v', 'V');
    key(0x09, 'b', 'B');
    key(0x0a, 'n', 'N');
    key(0x01, 'm', 'M');
    key(0x02, ',', '<');
    key(0x29, '.', '>');
    key(0x2a, '/', '?');

    key(0x18, 11,  11); // cursor up
    key(0x00, 8,   8); // cursor left
	key(0x08, 12,  12); // cursor right
    key(0x00, 10,  10); // cursor down

	key(0x3d, 8,   8); // delete
	key(0x3b, ' ', ' '); // space

    printf("keyboard_normal_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", normaltab[i]);
    printf("keyboard_shifted_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", shiftedtab[i]);

    return 0;
}
