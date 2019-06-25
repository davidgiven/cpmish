/* libcuss © 2019 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include "libcuss.h"

void con_putc(uint16_t c)
{
    if (screeny >= SCREENHEIGHT)
            return;

    if (c < 32)
    {
            con_putc('^');
            c += '@';
    }

    bios_conout(c);
    screenx++;
    if (screenx == SCREENWIDTH)
    {
            screenx = 0;
            screeny++;
    }
}

