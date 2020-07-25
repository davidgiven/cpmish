/* libcuss © 2019 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

#include <cpm.h>
#include <stdio.h>
#include "libcuss.h"

void con_goto(uint16_t x, uint16_t y)
{
    #if defined LIBCUSS_ADM3
        if (!x && !y)
            cpm_bios_conout(30);
        else
        {
            static uint8_t gotoseq[] = "\033=xx";
            gotoseq[2] = y + ' ';
            gotoseq[3] = x + ' ';
            cpm_printstring0((char*) gotoseq);
        }
	#elif defined LIBCUSS_VT52
		if (!x && !y)
			cpm_printstring0("\033H");
		else
		{
            static uint8_t gotoseq[] = "\033Yxx";
            gotoseq[2] = y + ' ';
            gotoseq[3] = x + ' ';
            cpm_printstring0((char*) gotoseq);
		}
	#elif defined LIBCUSS_ANSI
		if (!x && !y)
			cpm_printstring0("\033[H");
		else
		{
			static char buffer[10];
			cpm_printstring0("\033[");
			itoa(y, buffer, 10);
			cpm_printstring0(buffer);
			cpm_bios_conout(';');
			itoa(x, buffer, 10);
			cpm_printstring0(buffer);
			cpm_bios_conout('H');
		}
	#else
        #error "No libcuss configuration specified."
    #endif

	screenx = x;
	screeny = y;
}

