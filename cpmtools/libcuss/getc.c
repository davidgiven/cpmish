/* libcuss © 2020 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include "libcuss.h"

uint8_t con_getc(void)
{
	#if defined LIBCUSS_VT52
		uint16_t i;
		uint8_t c = cpm_bios_conin();
		if (c != 27)
			return c;

		i = 1000;
		while (--i)
		{
			c = cpm_bios_const();
			if (c == 0xff)
				break;
		}
		if (c == 0)
			return 27;
		return cpm_bios_conin() | 0x80;
	#else
		return cpm_bios_conin();
	#endif
}


