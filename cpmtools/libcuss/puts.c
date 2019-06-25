/* libcuss © 2019 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include "libcuss.h"

void con_puts(const char* s)
{
	for (;;)
	{
		uint16_t c = *s++;
		if (!c)
			break;
		con_putc(c);
	}
}

