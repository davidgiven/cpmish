#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>
#include <cpm.h>

#define WIDTH 80
#define HEIGHT 23

char filename[16]; /* 0:12345678.abc\n */

uint8_t screenx, screeny;
uint8_t status_line_length;

uint8_t* buffer_start;
uint8_t* gap_start;
uint8_t* gap_end;
uint8_t* buffer_end;
bool dirty;

uint8_t* first_line; /* <= gap_start */
uint8_t* current_line; /* <= gap_start */
uint8_t* old_current_line;
unsigned current_line_y;
uint8_t display_height[HEIGHT];
uint16_t line_length[HEIGHT];

struct command
{
	void (*callback)(uint16_t);
	uint16_t default_count;
};

#define buffer ((char*)cpm_default_dma)

extern void colon(uint16_t count);

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
	itoa(i, buffer, 10);
	con_puts(buffer);
}

void goto_status_line(void)
{
	bios_conout(27);
	bios_conout('=');
	bios_conout(HEIGHT + ' ');
	bios_conout(0 + ' ');
}

void set_status_line(const char* message)
{
	uint16_t length = 0;

	goto_status_line();
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

uint8_t* draw_line(uint8_t* startp)
{
	uint16_t xo = 0;
	uint16_t c;
	uint16_t starty = screeny;
	uint8_t* inp = startp;

	while (screeny != HEIGHT)
	{
		if (inp == gap_start)
		{
			inp = gap_end;
			startp += (gap_end - gap_start);
		}
		if (inp == buffer_end)
		{
			if (xo == 0)
				con_putc('~');
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

	display_height[starty] = (xo / WIDTH) + 1;
	line_length[starty] = inp - startp;

	return inp;
}

/* inp <= gap_start */
void render_screen(uint8_t* inp)
{
	unsigned i;
	for (i=screeny; i != HEIGHT; i++)
		display_height[i] = 0;

	while (screeny < HEIGHT)
	{
		if (inp == current_line)
			current_line_y = screeny;
		inp = draw_line(inp);
	}
}

void adjust_scroll_position(void)
{
	uint16_t total_height = 0;

	first_line = current_line;
	while ((first_line != buffer_start) && (total_height < (HEIGHT/2)))
	{
		const uint8_t* line_end = first_line--;
		while ((first_line != buffer_start) && (first_line[-1] != '\n'))
			first_line--;

		total_height += (compute_length(first_line, line_end, NULL) / WIDTH) + 1;
	}

	con_goto(0, 0);
	render_screen(first_line);
}

void recompute_screen_position(void)
{
	const uint8_t* inp;
	uint16_t length;

	if (current_line < first_line)
		adjust_scroll_position();
	
	for (;;)
	{
		inp = first_line;
		current_line_y = 0;
		while (current_line_y < HEIGHT)
		{
			uint16_t height;

			if (inp == current_line)
				break;

			height = display_height[current_line_y];
			inp += line_length[current_line_y];

			current_line_y += height;
		}

		if ((current_line_y >= HEIGHT) ||
			((current_line_y + display_height[current_line_y]) > HEIGHT))
		{
			adjust_scroll_position();
		}
		else
			break;
	}

	length = compute_length(current_line, gap_start, NULL);
	con_goto(length % WIDTH, current_line_y + (length / WIDTH));
}

void redraw_current_line(void)
{
	uint8_t* nextp;
	uint16_t oldheight;
	
	oldheight = display_height[current_line_y];
	con_goto(0, current_line_y);
	nextp = draw_line(current_line);
	if (oldheight != display_height[current_line_y])
		render_screen(nextp);

	recompute_screen_position();
}

/* ======================================================================= */
/*                                LIFECYCLE                                */
/* ======================================================================= */

void insert_file(const char* filename)
{
	int fd;
	
	strcpy(buffer, "Reading ");
	strcat(buffer, filename);
	set_status_line(buffer);

	fd = open(filename, O_RDONLY);
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
					goto done;
				}
				*gap_start++ = c;
			}
		}
	}

	set_status_line("");
done:
	close(fd);
}

bool save_file(const char* filename)
{
	int fd;
	const uint8_t* inp;
	uint8_t* outp;
	uint16_t pushed;

	strcpy(buffer, "Writing ");
	strcat(buffer, filename);
	set_status_line(buffer);

	fd = open(filename, O_WRONLY|O_CREAT|O_TRUNC);
	if (fd == -1)
	{
		set_status_line("Could not open output file");
		return false;
	}

	inp = buffer_start;
	outp = cpm_default_dma;
	pushed = 0;
	while ((inp != buffer_end) || (outp != cpm_default_dma) || pushed)
	{
		uint16_t c;

		if (pushed)
		{
			c = pushed;
			pushed = 0;
		}
		else
		{
			if (inp == gap_start)
				inp = gap_end;
			c = (inp != buffer_end) ? *inp++ : 26;

			if (c == '\n')
			{
				pushed = '\n';
				c = '\r';
			}
		}
		
		*outp++ = c;

		if (outp == (cpm_default_dma+128))
		{
			if (write(fd, cpm_default_dma, 128) != 128)
			{
				set_status_line("Error writing output file");
				goto error;
			}
			outp = cpm_default_dma;
		}
	}
	
	set_status_line("");
	dirty = false;
	close(fd);
	return true;

error:
	close(fd);
	return false;
}

void quit(void)
{
	cpm_printstring("\032Goodbye!\r\n$");
	cpm_exit();
}

/* ======================================================================= */
/*                            EDITOR OPERATIONS                            */
/* ======================================================================= */

void cursor_home(uint16_t count)
{
	while (gap_start != current_line)
		*--gap_end = *--gap_start;
}

void cursor_end(uint16_t count)
{
	while ((gap_end != buffer_end) && (gap_end[0] != '\n'))
		*gap_start++ = *gap_end++;
}

void cursor_left(uint16_t count)
{
	while (count--)
	{
		if ((gap_start != buffer_start) && (gap_start[-1] != '\n'))
			*--gap_end = *--gap_start;
	}
}

void cursor_right(uint16_t count)
{
	while (count--)
	{
		if ((gap_end != buffer_end) && (gap_end[0] != '\n'))
			*gap_start++ = *gap_end++;
	}
}

void cursor_down(uint16_t count)
{
	while (count--)
	{
		uint16_t offset = gap_start - current_line;
		cursor_end(1);
		if (gap_end == buffer_end)
			return;
			
		*gap_start++ = *gap_end++;
		current_line = gap_start;
		cursor_right(offset);
	}
}

void cursor_up(uint16_t count)
{
	while (count--)
	{
		uint16_t offset = gap_start - current_line;

		cursor_home(1);
		if (gap_start == buffer_start)
			return;

		do
			*--gap_end = *--gap_start;
		while ((gap_start != buffer_start) && (gap_start[-1] != '\n'));

		current_line = gap_start;
		cursor_right(offset);
	}
}

bool word_boundary(uint16_t left, uint16_t right)
{
	if (!isalnum(left) && isalnum(right))
		return 1;
	if (isspace(left) && !isspace(right))
		return 1;
	return 0;
}

void cursor_wordleft(uint16_t count)
{
	while (count--)
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
}

void cursor_wordright(uint16_t count)
{
	while (count--)
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
}

void insert_text(uint16_t count)
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
		else if (c == 13)
		{
			if (gap_start != gap_end)
			{
				*gap_start++ = '\n';
				con_goto(0, current_line_y);
				current_line = draw_line(current_line);
				current_line_y = screeny;
				display_height[current_line_y] = 0;
			}
		}
		else if (gap_start != gap_end)
			*gap_start++ = c;
		
		redraw_current_line();
	}

	set_status_line("");
	dirty = true;
}

void append_text(uint16_t count)
{
	cursor_end(1);
	recompute_screen_position();
	insert_text(count);
}

void goto_line(uint16_t lineno)
{
	while (gap_start != buffer_start)
		*--gap_end = *--gap_start;
	current_line = buffer_start;

	while ((gap_end != buffer_end) && --lineno)
	{
		while (gap_end != buffer_end)
		{
			uint16_t c = *gap_start++ = *gap_end++;
			if (c == '\n')
			{
				current_line = gap_start;
				break;
			}
		}
	}
}

void delete_right(uint16_t count)
{
	while (count--)
	{
		if (gap_end == buffer_end)
			break;
		gap_end++;
	}

	redraw_current_line();
	dirty = true;
}

void delete_rest_of_line(uint16_t count)
{
	while ((gap_end != buffer_end) && (*++gap_end != '\n'))
		;

	if (count != 0)
		redraw_current_line();
	dirty = true;
}

void delete_multi(uint16_t count)
{
	uint16_t c;
	set_status_line("Delete?");
	c = bios_conin();
	set_status_line("");

	while (count--)
	{
		switch (c)
		{
			case 'w': /* Delete word */
			{
				uint16_t left = (gap_start == buffer_start) ? '\n' : gap_start[-1];

				while (gap_end != buffer_end)
				{
					uint16_t right = *++gap_end;

					if ((gap_end == buffer_end) || (right == '\n'))
						break;
					if (word_boundary(left, right))
						break;

					left = right;
				}
				dirty = true;
				break;
			}

			case 'd': /* Delete line */
			{
				cursor_home(1);
				delete_rest_of_line(0);
				if (gap_end != buffer_end)
				{
					gap_end++;
					display_height[current_line_y] = 0;
				}
				break;
			}

			case '$': /* Delete rest of line */
			{
				delete_rest_of_line(0);
				break;
			}

			default:
				set_status_line("Invalid delete modifier");
				return;
		}
	}

	redraw_current_line();
}

void join(uint16_t count)
{
	while (count--)
	{
		uint8_t* ptr = gap_end;
		while ((ptr != buffer_end) && (*ptr != '\n'))
			ptr++;

		if (ptr != buffer_end)
			*ptr = ' ';
	}

	con_goto(0, current_line_y);
	render_screen(current_line);
	dirty = true;
}

void open_above(uint16_t count)
{
	if (gap_start == gap_end)
		return;

	cursor_home(1);
	*--gap_end = '\n';

	recompute_screen_position();
	con_goto(0, current_line_y);
	render_screen(current_line);
	recompute_screen_position();

	insert_text(count);
}

void open_below(uint16_t count)
{
	cursor_down(1);
	open_above(1);
}

void zed(uint16_t count)
{
	uint16_t c;
	set_status_line(dirty ?
		"Save (document has changed)?" : "Save (document has not changed)?");

	c = bios_conin();
	set_status_line("");

	switch (c)
	{
		case 'Z':
			if (!dirty)
				quit();
			if (!filename[0])
			{
				set_status_line("No filename set");
				break;
			}
			if (save_file(filename))
				quit();
			break;

		case 'Q':
			quit();
			break;
	}
}

void redraw_screen(uint16_t count)
{
	con_clear();
	render_screen(first_line);
}

const char editor_keys[] = "^$hjklbwiAGxdJOoZ:\014";
const struct command editor_cb[] =
{
	{ cursor_home,		1 },
	{ cursor_end,		1 },
	{ cursor_left,		1 },
	{ cursor_down,		1 },
	{ cursor_up,		1 },
	{ cursor_right,		1 },
	{ cursor_wordleft,	1 },
	{ cursor_wordright,	1 },
	{ insert_text,		1 },
	{ append_text,		1 },
	{ goto_line,		UINT_MAX },
	{ delete_right,		1 },
	{ delete_multi,		1 },
	{ join,             1 },
	{ open_above,       1 },
	{ open_below,       1 },
	{ zed,              1 },
	{ colon,            1 },
	{ redraw_screen,	1 },
};

/* ======================================================================= */
/*                             COLON COMMANDS                              */
/* ======================================================================= */

void colon(uint16_t count)
{
	set_status_line("");

	for (;;)
	{
		char* w;

		goto_status_line();
		cpm_conout(':');
		buffer[0] = 126;
		buffer[1] = 0;
		cpm_readline((uint8_t*) buffer);
		cpm_printstring("\r\n$");

		buffer[buffer[1]+2] = '\0';

		w = strtok(buffer+2, " ");
		if (!w)
			break;
		if (*w == 'w')
		{
			char* arg = strtok(NULL, " ");
			if (arg)
			{
				strncpy(filename, arg, sizeof(filename));
				filename[sizeof(filename)-1] = '\0';
				dirty = true;
			}
			if (!filename[0])
				cpm_printstring("No filename\r\n$");
			else if (save_file(filename))
			{
				if (w[1] == 'q')
					quit();
			}
		}
		else if (*w == 'q')
		{
			if (!dirty || (w[1] == '!'))
				quit();
			else
				cpm_printstring("Document not saved\r\n$");
		}
		else
			cpm_printstring("Unknown command\r\n$");
	}

	con_clear();
	render_screen(first_line);
}

/* ======================================================================= */
/*                            EDITOR OPERATIONS                            */
/* ======================================================================= */

void main(int argc, const char* argv[])
{
	if (argc > 2)
	{
		cpm_printstring("Syntax: qe [<filename>]\r\n$");
		cpm_exit();
	}
	else if (argc == 2)
	{
		strncpy(filename, argv[1], sizeof(filename));
		filename[sizeof(filename)-1] = '\0';
	}
	else
		filename[0] = '\0';

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
	if (filename[0])
		insert_file(filename);

	dirty = false;
	goto_line(1);

	con_goto(0, 0);
	render_screen(first_line);
	old_current_line = current_line;

	for (;;)
	{
		const char* cmdp;
		uint16_t length;
		unsigned c;
		uint16_t command_count = 0;

		recompute_screen_position();
		old_current_line = current_line;

		for (;;)
		{
			c = bios_conin();
			if (isdigit(c))
			{
				command_count = (command_count*10) + (c-'0');
				itoa(command_count, (char*)cpm_default_dma, 10);
				strcat((char*)cpm_default_dma, " repeat");
				set_status_line((char*)cpm_default_dma);
			}
			else
			{
				set_status_line("");
				break;
			}
		}
			
		cmdp = strchr(editor_keys, c);
		if (cmdp)
		{
			const struct command* cmd = &editor_cb[cmdp - editor_keys];
			if (command_count == 0)
				command_count = cmd->default_count;
			cmd->callback(command_count);
		}
	}
}

