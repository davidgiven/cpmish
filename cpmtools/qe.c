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
uc screenx, screeny;
uc* screenptr;
uc status_line_length;

uc* buffer_start;
uc* gap_start;
uc* gap_end;
uc* buffer_end;

uint16_t first_line;
uint16_t cursor;
uc cursorx, cursory;
uint16_t current_line;
uc current_line_y;

/* ======================================================================= */
/*                                SCREEN REFRESH                           */
/* ======================================================================= */

void con_goto(uc x, uc y)
{
	screenx = x;
	screeny = y;
	screenptr = &logical_screen[screeny][screenx];
}

void con_clear(void)
{
	memset(logical_screen, ' ', sizeof(logical_screen));
	con_goto(0, 0);
}

void con_init(void)
{
	con_clear();
	memset(physical_screen, ' ', sizeof(physical_screen));
	bios_conout(26);
}

void con_newline(void)
{
	screenx = 0;
	screeny++;
	con_goto(screenx, screeny);
}

void con_putc(uc c)
{
	if (screeny >= HEIGHT)
		return;

	if (c < 32)
	{
		con_putc('^');
		c += '@';
	}

	*screenptr++ = c;
	screenx++;
	if (screenx == WIDTH)
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
		bios_conout(30);
	else
	{
		bios_conout(27);
		bios_conout('=');
		bios_conout(y + ' ');
		bios_conout(x + ' ');
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
		static uint16_t offset;
		static uc x, y;

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

			bios_conout(c);
		}
	}
done:;

	scr_goto(screenx, screeny);
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
		bios_conout(c);
		length++;
	}
	while (length < status_line_length)
	{
		bios_conout(' ');
		length++;
	}
	status_line_length = length;
	scr_goto(screenx, screeny);
}

/* ======================================================================= */
/*                              BUFFER MANAGEMENT                          */
/* ======================================================================= */

uc* ptr(uint16_t index)
{
	uc* p = buffer_start + index;
	if (p < gap_start)
		return p;
	return gap_end + (gap_start - buffer_start);
}

uint16_t pos(const uc* ptr)
{
	if (ptr < gap_start)
		return ptr - buffer_start;
	return (gap_start - buffer_start) + (ptr - gap_end);
}

void new_file(void)
{
	gap_start = buffer_start;
	gap_end = buffer_end;

	first_line = 0;
	cursor = 0;
	current_line = 0;
	current_line_y = 0;
}
	
void render_screen(void)
{
	static uc* inp;
	static uc* cursorp;
	static uc* currentlinep;
	static uc xo;

	inp = ptr(first_line);
	cursorp = ptr(cursor);
	currentlinep = ptr(current_line);
	xo = 0;

	con_clear();
	while (screeny != HEIGHT)
	{
		static uc c;

		if (inp == gap_start)
			inp = gap_end;
		if (inp == buffer_end)
		{
			con_newline();
			con_puts("<<EOF>>");
			break;
		}

		if (inp == cursorp)
		{
			cursorx = screenx;
			cursory = screeny;
		}
		c = *inp++;
		if (c == '\n')
		{
			con_newline();
			xo = 0;

			if (inp == currentlinep)
				current_line_y = screeny;
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

	con_goto(cursorx, cursory);
	con_refresh();
}

/* ======================================================================= */
/*                            EDITOR OPERATIONS                            */
/* ======================================================================= */

void cursor_right(void)
{
	if (cursor != pos(buffer_end))
		cursor++;
}

const char editor_keys[] = "l";
void (*const editor_cb[])(void) =
{
	cursor_right,
};

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

void main(int argc, const char* argv[])
{
	cpm_overwrite_ccp();
	con_init();

	buffer_start = cpm_ram;
	buffer_end = cpm_ramtop;
	cpm_ram = buffer_start;

	itoa((uint16_t)(buffer_end - buffer_start), (char*)cpm_default_dma, 10);
	strcat((char*)cpm_default_dma, " bytes free");
	set_status_line((char*) cpm_default_dma);

	new_file();
	insert_file("ccp.asm");

	for (;;)
	{
		char c;
		char* cmdptr;
		render_screen();
		for (;;) {
		c = bios_conin();
		con_putc(c);
		con_refresh();
		}

		#if 0
		cmdptr = strchr(editor_keys, c);
		if (cmdptr)
			editor_cb[cmdptr - editor_keys]();
		#endif
	}
}

