#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <cpm.h>

#define WIDTH 80
#define HEIGHT 23

typedef uint8_t uc;

uc physical_screen[HEIGHT][WIDTH];
uc logical_screen[HEIGHT][WIDTH];
uc cursorx, cursory;

void con_init(void)
{
	memset(physical_screen, ' ', sizeof(physical_screen));
	memset(logical_screen, ' ', sizeof(logical_screen));
	cpm_conout(26);
}

void con_newline(void)
{
	cursorx = 0;
	cursory++;
}

void con_goto(uc x, uc y)
{
	cursorx = x;
	cursory = y;
}

void con_putc(uc c)
{
	if (cursory >= HEIGHT)
		return;

	if (c < 32)
	{
		con_putc('^');
		c += '@';
	}

	logical_screen[cursory][cursorx] = c;
	cursorx++;
	if (cursorx == WIDTH)
		con_newline();
}

void con_puts(const uc* s)
{
	for (;;)
	{
		uc c = *s++;
		if (!c)
			break;
		con_putc(c);
	}
}

void scr_goto(uc x, uc y)
{
	if (!x && !y)
		cpm_conout(30);
	else
	{
		cpm_conout(27);
		cpm_conout('=');
		cpm_conout(y + ' ');
		cpm_conout(x + ' ');
	}
}

void con_refresh(void)
{
	uc x, y;
	for (y=0; y<HEIGHT; y++)
	{
		uc* pptr = physical_screen[y];
		const uc* lptr = logical_screen[y];
		bool drawing = false;

		x = 0;
		while (x<WIDTH)
		{
			uc lc = *lptr;
			uc pc = *pptr;
			bool different = (lc != pc);

			if (different && !drawing)
				scr_goto(x, y);
			drawing = different;

			if (drawing)
			{
				*pptr = lc;
				cpm_conout(lc);
			}

			lptr++;
			pptr++;
			x++;
		}
	}
}

void main(int argc, const char* argv[])
{
	con_init();
	con_puts("Foo bar");
	con_refresh();
	cpm_conin();
	con_goto(40, 20);
	con_puts("(40, 20)");
	con_refresh();
	cpm_conin();
}

