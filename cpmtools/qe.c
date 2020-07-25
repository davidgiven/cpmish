/* qe © 2019 David Given
 * This library is distributable under the terms of the 2-clause BSD license.
 * See COPYING.cpmish in the distribution root directory for more information.
 */

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
#include "libcuss.h"

#define WIDTH SCREENWIDTH
#define HEIGHT (SCREENHEIGHT-1)

uint16_t screenx, screeny;
uint16_t status_line_length;
void (*print_status)(const char*);

uint8_t* buffer_start;
uint8_t* gap_start;
uint8_t* gap_end;
uint8_t* buffer_end;
uint16_t dirty;

uint8_t* first_line; /* <= gap_start */
uint8_t* current_line; /* <= gap_start */
uint16_t current_line_y;
uint8_t display_height[HEIGHT];
uint16_t line_length[HEIGHT];

uint16_t command_count;
typedef void command_t(uint16_t);

struct bindings
{
	const char* name;
	const char* keys;
	command_t* const* callbacks;
};

const struct bindings* bindings;

extern const struct bindings delete_bindings;
extern const struct bindings zed_bindings;
extern const struct bindings change_bindings;

#define buffer ((char*)cpm_default_dma)

extern void colon(uint16_t count);
extern void goto_line(uint16_t lineno);

/* ======================================================================= */
/*                                MISCELLANEOUS                            */
/* ======================================================================= */

void print_newline(void)
{
	cpm_printstring0("\r\n");
}

/* Appends a string representation of the FCB to buffer. */
void render_fcb(FCB* fcb)
{
	const uint8_t* inp;
	char* outp = buffer;

	while (*outp)
		outp++;

	if (fcb->dr)
	{
		*outp++ = '@' + fcb->dr;
		*outp++ = ':';
	}

	inp = &fcb->f[0];
	while (inp != &fcb->f[12])
	{
		uint8_t c;
		if (inp == &fcb->f[8])
			*outp++ = '.';
		c = *inp++;
		if (c != ' ')
			*outp++ = c;
	}

	*outp++ = '\0';
}

/* ======================================================================= */
/*                                SCREEN DRAWING                           */
/* ======================================================================= */

void con_puti(int i)
{
	itoa(i, buffer, 10);
	con_puts(buffer);
}

void goto_status_line(void)
{
	con_goto(0, HEIGHT);
}

void set_status_line(const char* message)
{
	uint16_t length = 0;

	goto_status_line();
	con_revon();
	for (;;)
	{
		uint16_t c = *message++;
		if (!c)
			break;
		con_putc(c);
		length++;
	}
	con_revoff();
	while (length < status_line_length)
	{
		cpm_bios_conout(' ');
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
	dirty = true;
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
			con_newline();
			break;
		}

		c = *inp++;
		if (c == '\n')
		{
			con_clear_to_eol();
			con_newline();
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
	while (first_line != buffer_start)
	{
		uint8_t* line_start = first_line;
		const uint8_t* line_end = line_start--;
		while ((line_start != buffer_start) && (line_start[-1] != '\n'))
			line_start--;

		total_height += (compute_length(line_start, line_end, NULL) / WIDTH) + 1;
		if (total_height > (HEIGHT/2))
			break;
		first_line = line_start;
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


void insert_file(void)
{
	strcpy(buffer, "Reading ");
	render_fcb(&cpm_fcb);
	print_status(buffer);

	cpm_fcb.ex = cpm_fcb.s1 = cpm_fcb.s2 = cpm_fcb.rc = 0;
	if (cpm_open_file(&cpm_fcb) == 0xff)
		goto error;

	for (;;)
	{
		uint8_t* inptr;

		int i = cpm_read_sequential(&cpm_fcb);
		if (i == 1) /* EOF */
			goto done;
		if (i != 0)
			goto error;

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
					print_status("Out of memory");
					goto done;
				}
				*gap_start++ = c;
			}
		}
	}

error:
	print_status("Could not read file");
done:
	cpm_close_file(&cpm_fcb);
	dirty = true;
	return;
}

void load_file(void)
{
	new_file();
	if (cpm_fcb.f[0])
		insert_file();

	dirty = false;
	goto_line(1);
}

bool really_save_file(FCB* fcb)
{
	const uint8_t* inp;
	uint8_t* outp;
	static uint16_t pushed;

	strcpy(buffer, "Writing ");
	render_fcb(fcb);
	print_status(buffer);

	fcb->ex = fcb->s1 = fcb->s2 = fcb->rc = 0;
	if (cpm_make_file(fcb) == 0xff)
		return false;
	fcb->cr = 0;

	inp = buffer_start;
	outp = cpm_default_dma;
	pushed = 0;
	while ((inp != buffer_end) || (outp != cpm_default_dma) || pushed)
	{
		static uint16_t c;

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
			if (cpm_write_sequential(fcb) == 0xff)
				goto error;
			outp = cpm_default_dma;
		}
	}
	
	dirty = false;
	return cpm_close_file(fcb) != 0xff;

error:
	cpm_close_file(fcb);
	return false;
}

bool save_file(void)
{
	static FCB tempfcb;

	if (cpm_open_file(&cpm_fcb) == 0xff)
	{
		/* The file does not exist. */
		if (really_save_file(&cpm_fcb))
		{
			dirty = false;
			return true;
		}
		else
		{
			print_status("Failed to save file");
			return false;
		}
	}

	/* Write to a temporary file. */

	strcpy((char*)tempfcb.f, "QETEMP  $$$");
	tempfcb.dr = cpm_fcb.dr;
	if (really_save_file(&tempfcb) == 0xff)
		goto tempfile;

	strcpy(buffer, "Renaming ");
	render_fcb(&tempfcb);
	strcat(buffer, " to ");
	render_fcb(&cpm_fcb);
	print_status(buffer);

	if (cpm_delete_file(&cpm_fcb) == 0xff)
		goto commit;
	memcpy(((uint8_t*) &tempfcb) + 16, &cpm_fcb, 16);
	if (cpm_rename_file((RCB*) &tempfcb) == 0xff)
		goto commit;
	return true;

tempfile:
	print_status("Cannot create QETEMP.$$$ file (it may exist)");
	return false;

commit:
	print_status("Cannot commit file; your data may be in QETEMP.$$$");
	return false;
}

void quit(void)
{
	cpm_printstring0("\032Goodbye!\r\n");
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

void insert_newline(void)
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

void insert_mode(bool replacing)
{
	set_status_line(replacing ? "Replace mode" : "Insert mode");

	for (;;)
	{
		uint16_t oldheight;
		uint8_t* nextp;
		uint16_t length;
		uint16_t c = con_getc();
		if (c == 27)
			break;

		dirty = true;
		if (c == 8)
		{
			if (gap_start != current_line)
				gap_start--;
		}
		else if (gap_start == gap_end)
		{
			/* Do nothing, out of memory */
		}
		else
		{
			if (replacing && (gap_end != buffer_end) && (*gap_end != '\n'))
				gap_end++;

			if (c == 13)
				insert_newline();
			else
				*gap_start++ = c;
		}
		
		redraw_current_line();
	}

	set_status_line("");
}

void insert_text(uint16_t count)
{
	insert_mode(false);
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

void delete_line(uint16_t count)
{
	while (count--)
	{
		cursor_home(1);
		delete_rest_of_line(0);
		if (gap_end != buffer_end)
		{
			gap_end++;
			display_height[current_line_y] = 0;
		}
	}

	redraw_current_line();
	dirty = true;
}

void delete_word(uint16_t count)
{
	while (count--)
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
	}

	redraw_current_line();
	dirty = true;
}

void change_word(uint16_t count)
{
	delete_word(1);
	insert_text(count);
}

void change_rest_of_line(uint16_t count)
{
	delete_rest_of_line(1);
	insert_text(count);
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
	open_above(count);
}

void replace_char(uint16_t count)
{
	uint16_t c = con_getc();

	if (gap_end == buffer_end)
		return;
	if (c == '\n')
	{
		gap_end++;
		/* The cursor ends up *after* the newline. */
		insert_newline();
	}
	else if (isprint(c))
	{
		*gap_end = c;
		/* The cursor ends on *on* the replace character. */
		redraw_current_line();
	}
}

void replace_line(uint16_t count)
{
	insert_mode(true);
}

void zed_save_and_quit(uint16_t count)
{
	if (!dirty)
		quit();
	if (!cpm_fcb.f[0])
	{
		set_status_line("No filename set");
		return;
	}
	if (save_file())
		quit();
}

void zed_force_quit(uint16_t count)
{
	quit();
}

void redraw_screen(uint16_t count)
{
	con_clear();
	render_screen(first_line);
}

void enter_delete_mode(uint16_t count)
{
	bindings = &delete_bindings;
	command_count = count;
}

void enter_zed_mode(uint16_t count)
{
	bindings = &zed_bindings;
	command_count = count;
}

void enter_change_mode(uint16_t count)
{
	bindings = &change_bindings;
	command_count = count;
}

const char normal_keys[] =
	LIBCUSS_KEY_LEFT LIBCUSS_KEY_DOWN LIBCUSS_KEY_UP LIBCUSS_KEY_RIGHT
	"^$hjklbwiAGxJOorR:\022dZc";

command_t* const normal_cbs[] =
{
	cursor_left,
	cursor_down,	
	cursor_up,		
	cursor_right,		
	cursor_home,
	cursor_end,	
	cursor_left,
	cursor_down,
	cursor_up,		
	cursor_right,
	cursor_wordleft,
	cursor_wordright,
	insert_text,
	append_text,	
	goto_line,	
	delete_right,	
	join,      
	open_above, 
	open_below,  
	replace_char, 
	replace_line,  
	colon,         
	redraw_screen,	
	enter_delete_mode,
	enter_zed_mode,
	enter_change_mode,
};

const struct bindings normal_bindings =
{
	NULL,
	normal_keys,
	normal_cbs
};

const char delete_keys[] = "dw$";
command_t* const delete_cbs[] =
{
	delete_line,   	  
	delete_word,         
	delete_rest_of_line,
};

const struct bindings delete_bindings =
{
	"Delete",
	delete_keys,
	delete_cbs
};

const char change_keys[] = "w$";
command_t* const change_cbs[] =
{
	change_word,         
	change_rest_of_line,
};

const struct bindings change_bindings =
{
	"Change",
	change_keys,
	change_cbs
};

const char zed_keys[] = "ZQ";
command_t* const zed_cbs[] =
{
	zed_save_and_quit, 
	zed_force_quit,	
};

const struct bindings zed_bindings =
{
	"Zed",
	zed_keys,
	zed_cbs
};

/* ======================================================================= */
/*                             COLON COMMANDS                              */
/* ======================================================================= */

void set_current_filename(const char* f)
{
	cpm_parse_filename(&cpm_fcb, f);
	dirty = true;
}

void print_no_filename(void)
{
	cpm_printstring0("No filename set\r\n");
}

void print_document_not_saved(void)
{
	cpm_printstring0("Document not saved (use ! to confirm)\r\n");
}

void print_colon_status(const char* s)
{
	cpm_printstring0(s);
	print_newline();
}

void colon(uint16_t count)
{
	print_status = print_colon_status;

	for (;;)
	{
		char* w;
		char* arg;

		goto_status_line();
		cpm_conout(':');
		buffer[0] = 126;
		buffer[1] = 0;
		cpm_readline((uint8_t*) buffer);
		print_newline();

		buffer[buffer[1]+2] = '\0';

		w = strtok(buffer+2, " ");
		if (!w)
			break;
		arg = strtok(NULL, " ");
		switch (*w)
		{
			case 'w':
			{
				bool quitting = w[1] == 'q';
				if (arg)
					set_current_filename(arg);
				if (!cpm_fcb.f[0])
					print_no_filename();
				else if (save_file())
				{
					if (quitting)
						quit();
				}
				break;
			}

			case 'r':
			{
				if (arg)
				{
					FCB backupfcb;

					memcpy(&backupfcb, &cpm_fcb, sizeof(FCB));
					cpm_parse_filename(&cpm_fcb, arg);
					insert_file();
					memcpy(&cpm_fcb, &backupfcb, sizeof(FCB));
				}
				else
					print_no_filename();
				break;
			}

			case 'e':
			{
				if (!arg)
					print_no_filename();
				else if (dirty && (w[1] != '!'))
					print_document_not_saved();
				else
				{
					set_current_filename(arg);
					load_file();
				}
				break;
			}

			case 'n':
			{
				if (dirty && (w[1] != '!'))
					print_document_not_saved();
				else
				{
					new_file();
					cpm_fcb.f[0] = 0; /* no filename */
				}
				break;
			}

			case 'q':
			{
				if (!dirty || (w[1] == '!'))
					quit();
				else
					print_document_not_saved();
				break;
			}

			default:
				cpm_printstring0("Unknown command\r\n");
		}
	}

	con_clear();
	print_status = set_status_line;
	render_screen(first_line);
}

/* ======================================================================= */
/*                            EDITOR OPERATIONS                            */
/* ======================================================================= */

void main(int argc, const char* argv[])
{
	if (cpm_fcb.f[0] == ' ')
		cpm_fcb.f[0] = 0;

	cpm_overwrite_ccp();
	con_clear();

	buffer_start = cpm_ram;
	buffer_end = cpm_ramtop-1;
	*buffer_end = '\n';
	cpm_ram = buffer_start;
	print_status = set_status_line;

	itoa((uint16_t)(buffer_end - buffer_start), buffer, 10);
	strcat(buffer, " bytes free");
	print_status(buffer);

	load_file();

	con_goto(0, 0);
	render_screen(first_line);
	bindings = &normal_bindings;

	command_count = 0;
	for (;;)
	{
		const char* cmdp;
		uint16_t length;
		unsigned c;

		recompute_screen_position();

		for (;;)
		{
			c = con_getc();
			if (isdigit(c))
			{
				command_count = (command_count*10) + (c-'0');
				itoa(command_count, buffer, 10);
				strcat(buffer, " repeat");
				set_status_line(buffer);
			}
			else
			{
				set_status_line("");
				break;
			}
		}
			
		cmdp = strchr(bindings->keys, c);
		if (cmdp)
		{
			command_t* cmd = bindings->callbacks[cmdp - bindings->keys];
			uint16_t count = command_count;
			if (count == 0)
			{
				if (cmd == goto_line)
					count = UINT_MAX;
				else
					count = 1;
			}
			command_count = 0;

			bindings = &normal_bindings;
			set_status_line("");
			cmd(count);
			if (bindings->name)
				set_status_line(bindings->name);
		}
		else
		{
			set_status_line("Unknown key");
			bindings = &normal_bindings;
			command_count = 0;
		}
	}
}
