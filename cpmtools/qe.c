#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cpm.h>

#define WIDTH 80
#define HEIGHT 23

typedef uint8_t uc;

uc physical_screen[HEIGHT][WIDTH];
uc logical_screen[HEIGHT][WIDTH];
#define logical_screen_start (logical_screen[0])
#define logical_screen_end (logical_screen[HEIGHT])
uc cursorx, cursory;
uc status_line_length;

uc* buffer_start;
uc* gap_start;
uc* gap_end;
uc* buffer_end;

uc* first_line;
uc* cursor;
uc* current_line;
uc current_line_y;

void con_clear(void)
{
	memset(logical_screen, ' ', sizeof(logical_screen));
	cursorx = cursory = 0;
}

void con_init(void)
{
	con_clear();
	memset(physical_screen, ' ', sizeof(physical_screen));
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

void con_puts(const char* s)
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
	static uc* lptr;
	static uc* pptr;

	lptr = logical_screen[0];
	pptr = physical_screen[0];
	while (lptr != logical_screen_end)
	{
		uint16_t offset;
		uc x, y;

		for (;;)
		{
			if (*lptr != *pptr)
				break;
			lptr++;
			pptr++;
			if (lptr == logical_screen_end)
				goto done;
		}

		offset = lptr - logical_screen[0];
		y = offset / WIDTH;
		x = offset % WIDTH;

		scr_goto(x, y);
		for (;;)
		{
			uc c = *lptr;
			if (c == *pptr)
				break;
			lptr++;
			*pptr++ = c;

			cpm_conout(c);
		}
	}
done:;

	scr_goto(cursorx, cursory);
}

void set_status_line(const char* message)
{
	uc length = 0;
	scr_goto(0, HEIGHT);
	for (;;)
	{
		uc c = *message++;
		if (!c)
			break;
		cpm_conout(c);
		length++;
	}
	while (length < status_line_length)
	{
		cpm_conout(' ');
		length++;
	}
	status_line_length = length;
	scr_goto(cursorx, cursory);
}

void new_file(void)
{
	gap_start = buffer_start;
	gap_end = buffer_end;

	first_line = buffer_start;
	cursor = first_line;
	current_line = first_line;
	current_line_y = 0;
}
	
void insert_file(const char* filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
		return;

	for (;;)
	{
		const uc* inptr;
		if (read(fd, &cpm_default_dma, 128) != 128)
			break;

		inptr = cpm_default_dma;
		while (inptr != (cpm_default_dma+128))
		{
			uc c = *inptr++;
			if (c == 26) /* EOF */
				break;
			if (c != '\r')
			{
				if (gap_start == gap_end)
				{
					set_status_line("Out of memory");
					break;
				}
				*gap_start++ = c;
			}
		}
	}
	
	close(fd);
}

void render_screen(void)
{
	uc* inp = first_line;
	uc screenx = 0;
	uc screeny = 0;
	uc xo = 0;

	con_clear();
	while (cursory != HEIGHT)
	{
		uc c;

		if (inp == gap_start)
			inp = gap_end;
		if (inp == buffer_end)
		{
			con_newline();
			con_puts("<<EOF>>");
			break;
		}

		if (inp == cursor)
		{
			screenx = cursorx;
			screeny = cursory;
		}
		c = *inp++;
		if (c == '\n')
		{
			con_newline();
			xo = 0;

			if (inp == current_line)
				current_line_y = cursory;
		}
		else if (c == '\t')
		{
			do
			{
				con_putc(' ');
				xo++;
			}
			while (xo & 7);
		}
		else
		{
			con_putc(c);
			xo++;
		}
	}

	con_goto(screenx, screeny);
	con_refresh();
}

void main(int argc, const char* argv[])
{
	cpm_overwrite_ccp();
	con_init();

	buffer_start = cpm_ram;
	buffer_end = cpm_ramtop;
	cpm_ram = buffer_start;

	itoa((uint16_t)(buffer_end - buffer_start), cpm_default_dma, 10);
	strcat(cpm_default_dma, " bytes free");
	set_status_line(cpm_default_dma);

	new_file();
	insert_file("ccp.asm");

	for (;;)
	{
		uc c;
		render_screen();
		c = cpm_conin();
		break;
	}
}

