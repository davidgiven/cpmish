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

uint8_t screenx, screeny;
uint8_t status_line_length;

uint8_t* buffer_start;
uint8_t* gap_start;
uint8_t* gap_end;
uint8_t* buffer_end;

uint8_t* first_line; /* <= gap_start */
uint8_t* current_line; /* <= gap_start */
uint8_t* old_current_line;
unsigned current_line_y;
uint8_t display_length[HEIGHT];
uint8_t line_length[HEIGHT];

/* ======================================================================= */
/*                                SCREEN DRAWING                           */
/* ======================================================================= */

void con_goto(uint8_t x, uint8_t y)
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
	screenx = x;
	screeny = y;
}

void con_clear(void)
{
	bios_conout(26);
	screenx = screeny = 0;
}

/* Leaves cursor at the beginning of the *next* line. */
void con_clear_to_eol(void)
{
	if (screeny >= HEIGHT)
		return;

	while (screenx != WIDTH)
	{
		bios_conout(' ');
		screenx++;
	}
	screenx = 0;
	screeny++;
}

void con_clear_to_eos(void)
{
	while (screeny < HEIGHT)
		con_clear_to_eol();
}

void con_newline(void)
{
	if (screeny >= HEIGHT)
		return;

	bios_conout('\n');
	screenx = 0;
	screeny++;
}

void con_putc(uint8_t c)
{
	if (screeny >= HEIGHT)
		return;

	if (c < 32)
	{
		con_putc('^');
		c += '@';
	}

	bios_conout(c);
	screenx++;
	if (screenx == WIDTH)
	{
		screenx = 0;
		screeny++;
	}
}

void con_puts(const char* s)
{
	for (;;)
	{
		char c = *s++;
		if (!c)
			break;
		con_putc((uint8_t) c);
	}
}

void con_puti(long i)
{
	itoa(i, (char*)cpm_default_dma, 10);
	con_puts((char*) cpm_default_dma);
}

void set_status_line(const char* message)
{
	uint16_t length = 0;

	bios_conout(27);
	bios_conout('=');
	bios_conout(HEIGHT + ' ');
	bios_conout(0 + ' ');

	for (;;)
	{
		uint16_t c = *message++;
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
	con_goto(screenx, screeny);
}

/* ======================================================================= */
/*                              BUFFER MANAGEMENT                          */
/* ======================================================================= */

void new_file(void)
{
	gap_start = buffer_start;
	gap_end = buffer_end;

	first_line = current_line = buffer_start;
}
	
uint16_t compute_length(const uint8_t* inp, const uint8_t* endp, const uint8_t** nextp)
{
	static uint16_t xo;
	static uint16_t c;

	xo = 0;
	for (;;)
	{
		if (inp == endp)
			break;
		if (inp == gap_start)
			inp = gap_end;

		c = *inp++;
		if (c == '\n')
			break;
		if (c == '\t')
			xo = (xo + 8) & ~7;
		else if (c < 32)
			xo += 2;
		else
			xo++;
	}

	if (nextp)
		*nextp = inp;
	return xo;
}

const uint8_t* draw_line(const uint8_t* startp)
{
	uint16_t xo = 0;
	uint16_t c;
	uint16_t starty = screeny;
	const uint8_t* inp = startp;

	while (screeny != HEIGHT)
	{
		if (inp == gap_start)
		{
			inp = gap_end;
			startp += (gap_end - gap_start);
		}
		if (inp == buffer_end)
		{
			con_puts("<<EOF>>");
			con_clear_to_eol();
			break;
		}

		c = *inp++;
		if (c == '\n')
		{
			con_clear_to_eol();
			break;
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

	display_length[starty] = xo;
	line_length[starty] = inp - startp;
	return inp;
}

/* inp <= gap_start */
void render_screen(const uint8_t* inp)
{
	unsigned i;
	for (i=screeny; i<HEIGHT; i++)
		display_length[i] = 0;

	while (screeny != HEIGHT)
	{
		if (inp == current_line)
			current_line_y = screeny;
		inp = draw_line(inp);
		if (inp == buffer_end)
		{
			con_clear_to_eos();
			break;
		}
	}
}

void recompute_screen_position(void)
{
	const uint8_t* inp = first_line;

	current_line_y = 0;
	while (current_line_y != HEIGHT)
	{
		uint16_t length;

		if (inp == current_line)
			break;

		length = display_length[current_line_y];
		inp += line_length[current_line_y];

		current_line_y += (length / WIDTH) + 1;
	}
}

/* ======================================================================= */
/*                            EDITOR OPERATIONS                            */
/* ======================================================================= */

void cursor_home(void)
{
	while (gap_start != current_line)
		*--gap_end = *--gap_start;
}

void cursor_end(void)
{
	while ((gap_end != buffer_end) && (gap_end[0] != '\n'))
		*gap_start++ = *gap_end++;
}

void cursor_left(void)
{
	if ((gap_start != buffer_start) && (gap_start[-1] != '\n'))
		*--gap_end = *--gap_start;
}

void cursor_right(void)
{
	if ((gap_end != buffer_end) && (gap_end[0] != '\n'))
		*gap_start++ = *gap_end++;
}

void cursor_down(void)
{
	uint16_t offset = gap_start - current_line;
	cursor_end();
	if (gap_end == buffer_end)
		return;
		
	*gap_start++ = *gap_end++;
	current_line = gap_start;
	while (offset--)
		cursor_right();
}

void cursor_up(void)
{
	uint16_t offset = gap_start - current_line;

	cursor_home();
	if (gap_start == buffer_start)
		return;

	do
		*--gap_end = *--gap_start;
	while ((gap_start != buffer_start) && (gap_start[-1] != '\n'));

	current_line = gap_start;
	while (offset--)
		cursor_right();
}

bool word_boundary(uint16_t left, uint16_t right)
{
	if (!isalnum(left) && isalnum(right))
		return 1;
	if (isspace(left) && !isspace(right))
		return 1;
	return 0;
}

void cursor_wordleft(void)
{
	bool linechanged = false;

	while (gap_start != buffer_start)
	{
		uint16_t right = *--gap_start = *--gap_end;
		uint16_t left = gap_start[-1];
		if (right == '\n')
			linechanged = true;

		if (word_boundary(left, right))
			break;
	}

	if (linechanged)
	{
		current_line = gap_start;
		while ((current_line != buffer_start) && (current_line[-1] != '\n'))
			current_line--;
	}
}

void cursor_wordright(void)
{
	while (gap_end != buffer_end)
	{
		uint16_t left = *gap_start++ = *gap_end++;
		uint16_t right = *gap_end;
		if (left == '\n')
			current_line = gap_start;

		if (word_boundary(left, right))
			break;
	}
}

void insert_text(void)
{
	set_status_line("Insert mode");

	for (;;)
	{
		uint16_t oldheight;
		uint8_t* nextp;
		uint16_t length;
		uint16_t c = bios_conin();
		if (c == 27)
			break;
		else if (c == 8)
		{
			if (gap_start != current_line)
				gap_start--;
		}
		else
			*gap_start++ = c;
		
		oldheight = display_length[current_line_y] / WIDTH;
		con_goto(0, current_line_y);
		nextp = draw_line(current_line);
		if (oldheight != (display_length[current_line_y] / WIDTH))
			render_screen(nextp);

		length = compute_length(current_line, gap_start, NULL);
		con_goto(length % WIDTH, current_line_y + (length / WIDTH));
	}
}

void goto_line(int lineno)
{
	while (gap_start != buffer_start)
		*--gap_end = *--gap_start;
	current_line = buffer_start;
}

const char editor_keys[] = "^$hjklbwi";
void (*const editor_cb[])(void) =
{
	cursor_home,
	cursor_end,
	cursor_left,
	cursor_down,
	cursor_up,
	cursor_right,
	cursor_wordleft,
	cursor_wordright,
	insert_text,
};

void insert_file(const char* filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
		return;

	for (;;)
	{
		const uint8_t* inptr;
		if (read(fd, &cpm_default_dma, 128) != 128)
			break;

		inptr = cpm_default_dma;
		while (inptr != (cpm_default_dma+128))
		{
			uint8_t c = *inptr++;
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
	con_clear();

	buffer_start = cpm_ram;
	buffer_end = cpm_ramtop-1;
	*buffer_end = '\n';
	cpm_ram = buffer_start;

	itoa((uint16_t)(buffer_end - buffer_start), (char*)cpm_default_dma, 10);
	strcat((char*)cpm_default_dma, " bytes free");
	set_status_line((char*) cpm_default_dma);

	new_file();
	insert_file("ccp.asm");
	goto_line(0);

	con_goto(0, 0);
	render_screen(first_line);
	old_current_line = current_line;
	for (;;)
	{
		const char* cmdp;
		uint16_t length;

		#if 0
		itoa((uint16_t)(gap_start - buffer_start), (char*)cpm_default_dma, 10);
		strcat((char*)cpm_default_dma, " ");
		itoa((uint16_t)(buffer_end - gap_end), (char*)cpm_default_dma + strlen(cpm_default_dma), 10);
		set_status_line((char*) cpm_default_dma);
		#endif

		if (current_line != old_current_line)
			recompute_screen_position();
		old_current_line = current_line;

		length = compute_length(current_line, gap_start, NULL);
		con_goto(length % WIDTH, current_line_y + (length / WIDTH));

		cmdp = strchr(editor_keys, bios_conin());
		if (cmdp)
			editor_cb[cmdp - editor_keys]();
	}
}

