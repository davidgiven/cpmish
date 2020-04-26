/* libcuss © 2020 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include "libcuss.h"

uint8_t con_getc(void)
{
	return cpm_bios_conin();
}


