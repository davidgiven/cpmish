/* Amstrad NC200 cpmish BIOS © 2019 David Given
 * This file is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <stdio.h>

static unsigned char normaltab[80] = {};
static unsigned char shiftedtab[80] = {};

static void key(int keycode, unsigned char normal, unsigned char shifted)
{
    normaltab[keycode] = normal;
    shiftedtab[keycode] = shifted;
}

int main(int argc, const char* argv[])
{
    key(0x0a, 27,  27);
    key(0x12, '1', '!');
    key(0x19, '2', '"');
    key(0x18, '3', '#');
    key(0x02, '4', '$');
    key(0x14, '5', '%');
    key(0x16, '6', '^');
    key(0x21, '7', '&');
    key(0x20, '8', '*');
    key(0x0f, '9', '(');
    key(0x49, '0', ')');
    key(0x41, '-', '_');
    key(0x39, '=', '+');
    key(0x32, 127, 127);
    key(0x4a, 8  , 8);

    key(0x13, 9,   9);
    key(0x1a, 'q', 'Q');
    key(0x1b, 'w', 'W');
    key(0x1c, 'e', 'E');
    key(0x26, 'r', 'R');
    key(0x2c, 't', 'T');
    key(0x2d, 'y', 'Y');
    key(0x3d, 'u', 'U');
    key(0x45, 'i', 'I');
    key(0x4e, 'o', 'O');
    key(0x4b, 'p', 'P');
    key(0x43, '[', '{');
    key(0x42, ']', '}');

    key(0x10, 0,   0); // caps lock/control
    key(0x24, 'a', 'A');
    key(0x1e, 's', 'S');
    key(0x1f, 'd', 'D');
    key(0x27, 'f', 'F');
    key(0x2e, 'g', 'G');
    key(0x36, 'h', 'H');
    key(0x46, 'j', 'J');
    key(0x3f, 'k', 'K');
    key(0x4d, 'l', 'L');
    key(0x4c, ';', ':');
    key(0x44, 39,  '@');
    key(0x34, '#', '~');
    key(0x04, 13,  13);

    key(0x00, 0,   0); // left shift
    key(0x22, 'z', 'Z');
    key(0x23, 'x', 'X');
    key(0x2f, 'c', 'C');
    key(0x2b, 'v', 'V');
    key(0x2a, 'b', 'B');
    key(0x37, 'n', 'N');
    key(0x3e, 'm', 'M');
    key(0x47, ',', '<');
    key(0x4f, '.', '>');
    key(0x35, '/', '?');
    key(0x01, 0,   0); // right shift
    key(0x3b, 1,   1); // cursor up

    key(0x08, 0,   0); // yellow
    key(0x09, 0,   0); // ctrl
    key(0x11, 0,   0); // symbol
    key(0x0b, ' ', ' ');
    key(0x3a, 92, '|');
    key(0x3c, 0,   0); // menu
    key(0x03, 1,   1); // cursor left
    key(0x33, 1,   1); // cursor right
    key(0x31, 1,   1); // cursor down

    printf("keyboard_normal_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", normaltab[i]);
    printf("keyboard_shifted_map:\n");
    for (int i=0; i<sizeof(normaltab); i++)
        printf("\t.db 0x%02x\n", shiftedtab[i]);

    return 0;
}
