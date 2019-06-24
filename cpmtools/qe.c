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

#define FILENAME_LEN 16 /* 0:12345678.abc\n */
char filename[FILENAME_LEN];

uint16_t screenx, screeny;
uint16_t status_line_length;

uint8_t* buffer_start;
uint8_t* gap_start;
uint8_t* gap_end;
uint8_t* buffer_end;
uint16_t dirty;

uint8_t* first_line; /* <= gap_start */
uint8_t* current_line; /* <= gap_start */
uint8_t* old_current_line;
uint16_t current_line_y;
uint8_t display_height[HEIGHT];
uint16_t line_length[HEIGHT];

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

#define buffer ((char*)cpm_default_dma)

extern void colon(uint16_t count);
extern void goto_line(uint16_t lineno);

/* ======================================================================= */
/*                                SCREEN DRAWING                           */
/* ======================================================================= */

void con_goto(uint8_t x, uint8_t y)
{
	if (!x && !y)
		bios_conout(30);
	else
	{
		static uint8_t gotoseq[] = "\033=xx";
		gotoseq[2] = y + ' ';
		gotoseq[3] = x + ' ';
		cpm_printstring0((char*) gotoseq);
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
	while ((first_line != buffer_start) && (total_height <= (HEIGHT/2)))
	{
		const uint8_t* line_end = first_line--;
		while ((first_line != buffer_start) && (first_line[-1] != '\n'))
			first_line--;

		total_height += (compute_length(first_line, line_end, NULL) / WIDTH) + 1;
	}
	if (total_height > (HEIGHT/2))
		first_line = current_line;

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
	dirty = true;
}

void load_file(const char* filename)
{
	new_file();
	if (filename[0])
		insert_file(filename);

	dirty = false;
	goto_line(1);
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
		uint16_t c = bios_conin();
		if (c == 27)
			break;
		else if (c == 8)
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
	dirty = true;
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

void replace_char(uint16_t count)
{
	uint16_t c = bios_conin();

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
	if (!filename[0])
	{
		set_status_line("No filename set");
		return;
	}
	if (save_file(filename))
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
}

void enter_zed_mode(uint16_t count)
{
	bindings = &zed_bindings;
}

const char normal_keys[] = "^$hjkl\010\012\013\014bwiAGxJOorR:\022dZ";
command_t* const normal_cbs[] =
{
	cursor_home,
	cursor_end,	
	cursor_left,
	cursor_down,
	cursor_up,		
	cursor_right,
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
	strncpy(filename, f, sizeof(filename));
	filename[sizeof(filename)-1] = '\0';
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

void colon(uint16_t count)
{
	set_status_line("");

	for (;;)
	{
		char* w;
		char* arg;

		goto_status_line();
		cpm_conout(':');
		buffer[0] = 126;
		buffer[1] = 0;
		cpm_readline((uint8_t*) buffer);
		cpm_printstring0("\r\n");

		buffer[buffer[1]+2] = '\0';

		w = strtok(buffer+2, " ");
		if (!w)
			break;
		arg = strtok(NULL, " ");
		if (*w == 'w')
		{
			bool quitting = w[1] == 'q';
			if (arg)
				set_current_filename(arg);
			if (!filename[0])
				print_no_filename();
			else if (save_file(filename))
			{
				if (quitting)
					quit();
			}
		}
		else if (*w == 'r')
		{
			if (arg)
			{
				/* insert_file uses buffer, which is where arg is pointed. */
				char argcopy[FILENAME_LEN];
				strcpy(argcopy, arg);
				insert_file(argcopy);
			}
			else
				print_no_filename();
		}
		else if (*w == 'e')
		{
			if (!arg)
				print_no_filename();
			else if (dirty && (w[1] != '!'))
				print_document_not_saved();
			else
			{
				set_current_filename(arg);
				load_file(filename);
			}
		}
		else if (*w == 'n')
		{
			if (dirty && (w[1] != '!'))
				print_document_not_saved();
			else
			{
				new_file();
				filename[0] = 0; /* no filename */
			}
		}
		else if (*w == 'q')
		{
			if (!dirty || (w[1] == '!'))
				quit();
			else
				print_document_not_saved();
		}
		else
			cpm_printstring0("Unknown command\r\n");
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
		cpm_printstring0("Syntax: qe [<filename>]\r\n");
		cpm_exit();
	}
	else if (argc == 2)
		set_current_filename(argv[1]);
	else
		filename[0] = '\0';

	cpm_overwrite_ccp();
	con_clear();

	buffer_start = cpm_ram;
	buffer_end = cpm_ramtop-1;
	*buffer_end = '\n';
	cpm_ram = buffer_start;

	itoa((uint16_t)(buffer_end - buffer_start), buffer, 10);
	strcat(buffer, " bytes free");
	set_status_line(buffer);

	load_file(filename);

	con_goto(0, 0);
	render_screen(first_line);
	old_current_line = current_line;
	bindings = &normal_bindings;

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
			if (command_count == 0)
			{
				if (cmd == goto_line)
					command_count = UINT_MAX;
				else
					command_count = 1;
			}

			bindings = &normal_bindings;
			cmd(command_count);
			if (bindings->name)
				set_status_line(bindings->name);
		}
		else
		{
			set_status_line("Unknown key");
			bindings = &normal_bindings;
		}
	}
}
