/* libcuss © 2019 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include "libcuss.h"

void con_clear_to_eol(void)
{
    if (screeny >= SCREENHEIGHT)
            return;

    #if defined LIBCUSS_CLEAREOL
        cpm_printstring0(LIBCUSS_CLEAREOL);
    #else
    {
        uint16_t i = SCREENWIDTH - screenx;
        if ((i != 0) && (screeny == (SCREENHEIGHT-1)))
            i--;

        while (i--)
            cpm_bios_conout(' ');
        con_goto(screenx, screeny);
    }
    #endif
}

