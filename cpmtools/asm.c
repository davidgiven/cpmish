#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "libcpm.h"

typedef int token_t;

enum
{
	TOKEN_EOF = 26,
	TOKEN_NL = '\n',

	TOKEN_IDENTIFIER = -1,
	TOKEN_NUMBER = -2,
	TOKEN_STRING = -3,
};

#define asm_fcb cpm_fcb
FCB hex_fcb;
FCB prn_fcb;
int pass;

uint8_t input_buffer_read_count;
uint8_t token_length;
uint8_t token_buffer[64];
uint16_t token_number;

void printn(const char* s, unsigned len)
{
	while (len--)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        putchar(b);
    }
}

void print(const char* s) __z88dk_fastcall
{
    for (;;)
    {
        uint8_t b = *s++;
        if (!b)
            return;
        putchar(b);
    }
}

void crlf(void)
{
    print("\r\n");
}

void printx(const char* s) __z88dk_fastcall
{
    print(s);
    crlf();
}

void printhex4(uint8_t nibble) __z88dk_fastcall
{
        nibble &= 0x0f;
    if (nibble < 10)
        nibble += '0';
    else
        nibble += 'a' - 10;
    putchar(nibble);
}

void printhex8(uint8_t b) __z88dk_fastcall
{
    printhex4(b >> 4);
    printhex4(b);
}

void printhex16(uint16_t b) __z88dk_fastcall
{
    printhex8(b >> 8);
    printhex8(b);
}

void fatal(const char* s) __z88dk_fastcall
{
	printx(s);
	cpm_exit();
}

static uint8_t get_drive_or_default(uint8_t dr)
{
	if (dr == ' ')
		return 0;
	return dr - '@';
}

static void open_output_file(FCB* fcb)
{
	if (fcb->dr > 16)
		return;
	
	cpm_delete_file(fcb);
	if (cpm_make_file(fcb) == 0xff)
		fatal("Cannot create output file");
}

static uint8_t read_byte(void)
{
	if (input_buffer_read_count == 0x80)
	{
		cpm_set_dma(cpm_default_dma);
		if (cpm_read_sequential(&asm_fcb) != 0)
			memset(cpm_default_dma, 26, 128);
		input_buffer_read_count = 0;
	}

	return cpm_default_dma[input_buffer_read_count++];
}

static void unread_byte(uint8_t b)
{
	cpm_default_dma[--input_buffer_read_count] = b;
}

static bool isident(uint8_t c)
{
	return isalnum(c) || (c == '_');
}

static void check_token_buffer_size(void)
{
	if (token_length == sizeof(token_buffer))
		fatal("token too long");
}

static token_t read_token(void)
{
	uint8_t c;

	do
		c = read_byte();
	while ((c == ' ') || (c == '\t') || (c == '\r') || (c == '\f') || (c == '\v'));

	if (c == ';')
	{
		do
			c = read_byte();
		while ((c != '!') || (c == '\n') || (c == 26));
	}

	c = toupper(c);
	token_length = 0;
	if (isdigit(c))
	{
		for (;;)
		{
			check_token_buffer_size();
			token_buffer[token_length++] = c;

			c = toupper(read_byte());
			if (c == '$')
				continue;
			if (!isalnum(c))
				break;
		}
		unread_byte(c);

		int base = 10;
		c = token_buffer[--token_length];
		switch (c)
		{
			case 'B': base = 2; break;
			case 'O': case 'Q': base = 8; break;
			case 'D': base = 10; break;
			case 'H': base = 16; break;
			default:
				if (isdigit(c))
					token_length++;
		}

		token_number = 0;
		for (unsigned i=0; i<token_length; i++)
		{
			c = token_buffer[i];
			if (c >= 'A')
				c = c - 'A' + 10;
			else
				c = c - '0';
			if (c >= base)
				fatal("invalid digit in character constant");

			token_number = (token_number * base) + c;
		}

		return TOKEN_NUMBER;
	}
	else if (isupper(c))
	{
		for (;;)
		{
			check_token_buffer_size();
			token_buffer[token_length++] = c;

			c = toupper(read_byte());
			if (c == '$')
				continue;
			if (!isident(c))
				break;
		}
		unread_byte(c);

		return TOKEN_IDENTIFIER;
	}
	else if (c == '\'')
	{
		for (;;)
		{
			c = read_byte();
			if (iscntrl(c))
				fatal("unterminated string constant");
			if (c == '\'')
			{
				c = read_byte();
				if (c != '\'')
					break;
			}

			check_token_buffer_size();
			token_buffer[token_length++] = c;
		}
		unread_byte(c);

		return TOKEN_STRING;
	}
	else if (c == '!')
		c = TOKEN_NL;
	else if (c == 0)
		c = TOKEN_EOF;

	/* Everything else is a single-character token */
	return c;
}

void main(void)
{
	memcpy(&hex_fcb, &cpm_fcb, sizeof(FCB));
	hex_fcb.dr = get_drive_or_default(cpm_fcb.f[9]);
	memcpy(&hex_fcb.f[8], "HEX", 3);

	memcpy(&prn_fcb, &cpm_fcb, sizeof(FCB));
	prn_fcb.dr = get_drive_or_default(cpm_fcb.f[10]);
	memcpy(&prn_fcb.f[8], "PRN", 3);

	asm_fcb.dr = get_drive_or_default(cpm_fcb.f[8]);
	memcpy(&asm_fcb.f[8], "ASM", 3);

	open_output_file(&hex_fcb);
	open_output_file(&prn_fcb);

	if (cpm_open_file(&asm_fcb) == 0xff)
		fatal("Cannot open input file");

	for (pass=0; pass<2; pass++)
	{
		asm_fcb.ex = asm_fcb.cr = 0;
		input_buffer_read_count = 0x80;

		for (unsigned i=0; i<10; i++)
		{
			printhex16(read_token());
			putchar(' ');
			printn(token_buffer, token_length);
			putchar(' ');
			printhex16(token_number);
			crlf();
		}
		crlf();
	}
}

