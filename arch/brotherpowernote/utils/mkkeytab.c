/* Amstrad NC200 cpmish BIOS © 2019 David Given
 * This file is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <stdio.h>

static unsigned char normaltab[8*9] = {};
static unsigned char shiftedtab[8*9] = {};

static void key(int keycode, unsigned char normal, unsigned char shifted)
{
    normaltab[keycode] = normal;
    shiftedtab[keycode] = shifted;
}

int main(int argc, const char* argv[])
{
    key(0x43, '`', '~');
    key(0x44, '1', '!');
    key(0x3c, '2', '@');
    key(0x32, '3', '#');
    key(0x34, '4', '$');
    key(0x33, '5', '%');
    key(0x1b, '6', '^');
    key(0x1c, '7', '&');
    key(0x1a, '8', '*');
    key(0x13, '9', '(');
    key(0x0b, '0', ')');
    key(0x0c, '-', '_');
    key(0x0a, '=', '+');
    key(0x03, 127, 127);

    key(0x3b, 9,   9);
    key(0x42, 'q', 'Q');
    key(0x3a, 'w', 'W');
    key(0x35, 'e', 'E');
    key(0x2c, 'r', 'R');
    key(0x2b, 't', 'T');
    key(0x23, 'y', 'Y');
    key(0x24, 'u', 'U');
    key(0x1d, 'i', 'I');
    key(0x14, 'o', 'O');
    key(0x0d, 'p', 'P');
	key(0x09, ']', ']');
	key(0x04, 13,  13); // return

    key(0x45, 0,   0); // caps lock
    key(0x41, 'a', 'A');
    key(0x3d, 's', 'S');
    key(0x31, 'd', 'D');
    key(0x2a, 'f', 'F');
    key(0x2d, 'g', 'G');
    key(0x25, 'h', 'H');
    key(0x22, 'j', 'J');
    key(0x19, 'k', 'K');
    key(0x12, 'l', 'L');
	key(0x16, ';', ':');
    key(0x11, '\'', '"');

    key(0x00, 0,   0); // left shift
    key(0x46, 'z', 'Z');
    key(0x39, 'x', 'X');
    key(0x36, 'c', 'C');
    key(0x2e, 'v', 'V');
    key(0x29, 'b', 'B');
    key(0x21, 'n', 'N');
    key(0x26, 'm', 'M');
    key(0x1e, ',', '<');
    key(0x15, '.', '>');
    key(0x0e, '/', '?');

    key(0x47, 0,   0); // ctrl
    key(0x37, 0,   0); // menu
    key(0x30, 27,  27); // escape
    key(0x07, ' ', ' '); // space
    key(0x01, 8,   8); // cursor left
    key(0x06, 11,  11); // cursor up
    key(0x10, 10,  10); // cursor down
	key(0x17, 12,  12); // cursor right

    printf("keyboard_normal_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", normaltab[i]);
    printf("keyboard_shifted_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", shiftedtab[i]);

    return 0;
}
