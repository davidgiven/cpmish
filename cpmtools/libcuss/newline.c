/* libcuss © 2019 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include "libcuss.h"

void con_newline(void)
{
    if (screeny >= SCREENHEIGHT)
            return;

    cpm_printstring0(LIBCUSS_NEWLINE);
    screenx = 0;
    screeny++;
}
