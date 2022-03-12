/* Amstrad NC200 cpmish BIOS © 2019 David Given
 * This file is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <stdio.h>

static unsigned char normaltab[0x80] = {};
static unsigned char shiftedtab[0x80] = {};

static void key(int keycode, unsigned char normal, unsigned char shifted)
{
    normaltab[keycode] = normal;
    shiftedtab[keycode] = shifted;
}

int main(int argc, const char* argv[])
{
	key(0x31, '1', '!');
	key(0x32, '2', '"');
	key(0x33, '3', '#');
	key(0x34, '4', '$');
	key(0x35, '5', '%');
	key(0x36, '6', '^');
	key(0x37, '7', '&');
	key(0x38, '8', '*');
	key(0x39, '9', '(');
	key(0x30, '0', ')');
	key(0x2d, '-', '_');
	key(0x3d, '=', '+');
	key(0x1a, '\\', '|');

	key(0x0f, 9,   9);
	key(0x71, 'q', 'Q');
	key(0x77, 'w', 'W');
	key(0x65, 'e', 'E');
	key(0x72, 'r', 'R');
	key(0x74, 't', 'T');
	key(0x79, 'y', 'Y');
	key(0x75, 'u', 'U');
	key(0x69, 'i', 'I');
	key(0x6f, 'o', 'O');
	key(0x70, 'p', 'P');
	key(0x7c, '[', '{');
	key(0x5d, ']', '}');
	key(0x08, 127, 127);

	key(0x61, 'a', 'A');
	key(0x73, 's', 'S');
	key(0x64, 'd', 'D');
	key(0x66, 'f', 'F');
	key(0x67, 'g', 'G');
	key(0x68, 'h', 'H');
	key(0x6a, 'j', 'J');
	key(0x6b, 'k', 'K');
	key(0x6c, 'l', 'L');
	key(0x3b, ';', ':');
	key(0x27, '\'', '@');
	key(0x5c, '#', '~');
	key(0x0d, 13,  13);

	key(0x7a, 'z', 'Z');
	key(0x78, 'x', 'X');
	key(0x63, 'c', 'C');
	key(0x76, 'v', 'V');
	key(0x62, 'b', 'B');
	key(0x6e, 'n', 'N');
	key(0x6d, 'm', 'M');
	key(0x2c, ',', '<');
	key(0x2e, '.', '>');
	key(0x2f, '/', '?');

	key(0x20, ' ', ' ');
	key(0x7f, 127, 127);

	key(0x10, 128, 138); /* function keys */
	key(0x11, 129, 139);
	key(0x12, 130, 140);
	key(0x13, 131, 141);
	key(0x14, 132, 142);
	key(0x15, 133, 143);
	key(0x16, 134, 144);
	key(0x17, 135, 145);
	key(0x18, 136, 146);

	key(0x0c, 0,   0); /* nav cluster */
	key(0x0b, 0,   0);
	key(0x07, 0,   0);
	key(0x09, 0,   0);
	key(0x0e, 0,   0);
	key(0x0a, 0,   0);

	key(0x1e, 11,  11); /* up */
	key(0x1d, 8,   8); /* left */
	key(0x1f, 10,  10); /* down */
	key(0x1c, 12,  12); /* right */

	key(0x03, 27,  27); /* left row */
	key(0x04, 137, 147);
	key(0x05, 148, 150);
	key(0x06, 149, 151);

    printf("keyboard_normal_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", normaltab[i]);
    printf("keyboard_shifted_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", shiftedtab[i]);

    return 0;
}
