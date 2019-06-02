/* asm.c
 * © 2019 David Given
 * This software is redistributable under the terms of the 2-clause BSD
 * licensed. See the LICENSE file in the source of the repository for the
 * full text.
 *
 * This is a reimplementation of DR's venerable asm.com, in C.
 *
 * It's a simple 8080 assembler which honours the original asm.com syntax,
 * complete with the non-well defined grammar and poor error handling of the
 * original.
 *
 * Unlike the original, it emits .bin files rather than .hex files, so in the
 * common use case (producing .com files) you don't need the loader at all ---
 * just run the assembler and rename the output. Also, it was easier that way.
 *
 * See http://www.gaby.de/cpm/randyfiles/DRI/ASM.pdf for the original
 * documentation.
 *
 * This version differs as follows:
 *
 *  - the aforesaid .hex / .bin difference
 *
 *  - error messages are more human readable, but no error recovery is done
 *    and assembly stops at the first error (this is to be fixed)
 *
 *  - the .prn file is always empty currently (this is to be fixed)
 *
 *  - if...endif supports else (but still doesn't support nesting)
 *  - bugs
 */

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

struct symbol
{
	uint8_t namelen;
	uint16_t value;
	void (*callback)(void);
	struct symbol* next;
	const uint8_t name[]; /* not zero terminated */
};

struct operator
{
	uint16_t (*callback)(uint16_t left, uint16_t right);
	uint8_t precedence;
	uint8_t binary;
};

uint16_t add_cb(uint16_t left, uint16_t right) { return left + right; }
uint16_t and_cb(uint16_t left, uint16_t right) { return left & right; }
uint16_t div_cb(uint16_t left, uint16_t right) { return left / right; }
uint16_t mod_cb(uint16_t left, uint16_t right) { return left % right; }
uint16_t mul_cb(uint16_t left, uint16_t right) { return left * right; }
uint16_t neg_cb(uint16_t left, uint16_t right) { (void)right; return -left; }
uint16_t not_cb(uint16_t left, uint16_t right) { (void)right; return ~left; }
uint16_t or_cb(uint16_t left, uint16_t right)  { return left | right; }
uint16_t shl_cb(uint16_t left, uint16_t right) { return left << right; }
uint16_t shr_cb(uint16_t left, uint16_t right) { return left >> right; }
uint16_t sub_cb(uint16_t left, uint16_t right) { return left - right; }
uint16_t xor_cb(uint16_t left, uint16_t right) { return left ^ right; }

enum
{
	OP_ADD,
	OP_AND,
	OP_DIV,
	OP_MOD,
	OP_MUL,
	OP_NEG,
	OP_NOT,
	OP_OR,
	OP_SHL,
	OP_SHR,
	OP_SUB,
	OP_XOR,
	OP_PAR,
};

const struct operator operators[] =
{
	{ add_cb, 2, true },
	{ and_cb, 4, true },
	{ div_cb, 1, true },
	{ mod_cb, 1, true },
	{ mul_cb, 1, true },
	{ neg_cb, 0, false },
	{ not_cb, 3, false },
	{ or_cb,  5, true},
	{ shl_cb, 1, true },
	{ shr_cb, 1, true },
	{ sub_cb, 2, true },
	{ xor_cb, 2, true },
	{ NULL,   0xff, false },
};

#define INSN(id, name, value, cb, next) \
	extern void cb(void); \
	const struct symbol id = { sizeof(name)-1, value, cb, next, name }

#define VALUE(id, name, value, next) \
	const struct symbol id = { sizeof(name)-1, value, equlabel_cb, next, name }

extern void operator_cb(void);
extern void undeflabel_cb(void);
extern void setlabel_cb(void);
extern void equlabel_cb(void);

INSN(and_symbol,   "AND",   OP_AND, operator_cb, NULL);
VALUE(a_symbol,    "A",     7,                   &and_symbol);
INSN(ana_symbol,   "ANA",   0xa0,   alusrc_cb,   &a_symbol);
INSN(add_symbol,   "ADD",   0x80,   alusrc_cb,   &ana_symbol);
INSN(adc_symbol,   "ADC",   0x88,   alusrc_cb,   &add_symbol);
INSN(ani_symbol,   "ANI",   0xe6,   simple2b_cb, &adc_symbol);
INSN(adi_symbol,   "ADI",   0xc6,   simple2b_cb, &ani_symbol);
INSN(aci_symbol,   "ACI",   0xce,   simple2b_cb, &adi_symbol);

VALUE(b_symbol,    "B",     0,                   NULL);

VALUE(c_symbol,    "C",     1,                   NULL);
INSN(call_symbol,  "CALL",  0xcd,   simple3b_cb, &c_symbol);
INSN(cmp_symbol,   "CMP",   0xb8,   alusrc_cb,   &call_symbol);
INSN(cpi_symbol,   "CPI",   0xfe,   simple2b_cb, &cmp_symbol);
INSN(cnz_symbol,   "CNZ",   0xc4,   simple3b_cb, &cpi_symbol);
INSN(cz_symbol,    "CZ",    0xcc,   simple3b_cb, &cnz_symbol);
INSN(cnc_symbol,   "CNC",   0xd4,   simple3b_cb, &cz_symbol);
INSN(cc_symbol,    "CC",    0xdc,   simple3b_cb, &cnc_symbol);
INSN(cpo_symbol,   "CPO",   0xe4,   simple3b_cb, &cc_symbol);
INSN(cpe_symbol,   "CPE",   0xec,   simple3b_cb, &cpo_symbol);
INSN(cp_symbol,    "CP",    0xf4,   simple3b_cb, &cpe_symbol);
INSN(cm_symbol,    "CM",    0xfc,   simple3b_cb, &cp_symbol);
INSN(cma_symbol,   "CMA",   0x2f,   simple1b_cb, &cm_symbol);
INSN(cmc_symbol,   "CMC",   0x3f,   simple1b_cb, &cma_symbol);

INSN(db_symbol,    "DB",    0,      db_cb,       NULL);
INSN(ds_symbol,    "DS",    0,      ds_cb,       &db_symbol);
INSN(dw_symbol,    "DW",    0,      dw_cb,       &ds_symbol);
VALUE(d_symbol,    "D",     2,                   &dw_symbol);
INSN(dcx_symbol,   "DCX",   0x0b,   rp_cb,       &d_symbol);
INSN(dad_symbol,   "DAD",   0x09,   rp_cb,       &dcx_symbol);
INSN(dcr_symbol,   "DCR",   0x05,   aludst_cb,   &dad_symbol);
INSN(daa_symbol,   "DAA",   0x27,   simple1b_cb, &dcr_symbol);
INSN(di_symbol,    "DI",    0xf3,   simple1b_cb, &daa_symbol);

INSN(equ_symbol,   "EQU",   0,      equ_cb,      NULL);
INSN(else_symbol,  "ELSE",  0,      else_cb,     &equ_symbol);
INSN(endif_symbol, "ENDIF", 0,      endif_cb,    &else_symbol);
VALUE(e_symbol,    "E",     3,                   &endif_symbol);
INSN(end_symbol,   "END",   0,      end_cb,      &e_symbol);
INSN(ei_symbol,    "EI",    0xfb,   simple1b_cb, &end_symbol);

VALUE(h_symbol,    "H",     4,                   NULL);
INSN(hlt_symbol,   "HLT",   0x76,   simple1b_cb, &h_symbol);

INSN(if_symbol,    "IF",    0,      if_cb,       NULL);
INSN(inx_symbol,   "INX",   0x03,   rp_cb,       &if_symbol);
INSN(inr_symbol,   "INR",   0x04,   aludst_cb,   &inx_symbol);
INSN(in_symbol,    "IN",    0xdb,   simple1b_cb, &inr_symbol);

INSN(jmp_symbol,   "JMP",   0xc3,   simple3b_cb, NULL);
INSN(jnz_symbol,   "JNZ",   0xc2,   simple3b_cb, &jmp_symbol);
INSN(jz_symbol,    "JZ",    0xca,   simple3b_cb, &jnz_symbol);
INSN(jnc_symbol,   "JNC",   0xd2,   simple3b_cb, &jz_symbol);
INSN(jc_symbol,    "JC",    0xda,   simple3b_cb, &jnc_symbol);
INSN(jpo_symbol,   "JPO",   0xe2,   simple3b_cb, &jc_symbol);
INSN(jpe_symbol,   "JPE",   0xea,   simple3b_cb, &jpo_symbol);
INSN(jp_symbol,    "JP",    0xf2,   simple3b_cb, &jpe_symbol);
INSN(jm_symbol,    "JM",    0xfa,   simple3b_cb, &jp_symbol);

VALUE(l_symbol,    "L",     5,                   NULL);
INSN(ldax_symbol,  "LDAX",  0x0a,   rp_cb,       &l_symbol);
INSN(lda_symbol,   "LDA",   0x3a,   simple3b_cb, &ldax_symbol);
INSN(lxi_symbol,   "LXI",   0,      lxi_cb,      &lda_symbol);
INSN(lhld_symbol,  "LHLD",  0x2a,   simple3b_cb, &lxi_symbol);

INSN(mod_symbol,   "MOD",   OP_MOD, operator_cb, NULL);
VALUE(m_symbol,    "M",     6,                   &mod_symbol);
INSN(mov_symbol,   "MOV",   0,      mov_cb,      &m_symbol);
INSN(mvi_symbol,   "MVI",   0,      mvi_cb,      &mov_symbol);

INSN(not_symbol,   "NOT",   OP_NOT, operator_cb, NULL);
INSN(nop_symbol,   "NOP",   0x00,   simple1b_cb, &not_symbol);

INSN(or_symbol,    "OR",    OP_OR,  operator_cb, NULL);
INSN(org_symbol,   "ORG",   0,      org_cb,      &or_symbol);
INSN(ora_symbol,   "ORA",   0xb0,   alusrc_cb,   &org_symbol);
INSN(ori_symbol,   "ORI",   0xf6,   simple2b_cb, &ora_symbol);
INSN(out_symbol,   "OUT",   0xd3,   simple1b_cb, &ori_symbol);

VALUE(psw_symbol,  "PSW",   6,                   NULL);
INSN(push_symbol,  "PUSH",  0xc5,   rp_cb,       &psw_symbol);
INSN(pop_symbol,   "POP",   0xc1,   rp_cb,       &push_symbol);
INSN(pchl_symbol,  "PCHL",  0xe9,   simple1b_cb, &pop_symbol);

INSN(ret_symbol,   "RET",   0xc9,   simple1b_cb, NULL);
INSN(rnz_symbol,   "RNZ",   0xc0,   simple1b_cb, &ret_symbol);
INSN(rz_symbol,    "RZ",    0xc8,   simple1b_cb, &rnz_symbol);
INSN(rnc_symbol,   "RNC",   0xd0,   simple1b_cb, &rz_symbol);
INSN(rc_symbol,    "RC",    0xd8,   simple1b_cb, &rnc_symbol);
INSN(rpo_symbol,   "RPO",   0xe0,   simple1b_cb, &rc_symbol);
INSN(rpe_symbol,   "RPE",   0xe8,   simple1b_cb, &rpo_symbol);
INSN(rp_symbol,    "RP",    0xf0,   simple1b_cb, &rpe_symbol);
INSN(rm_symbol,    "RM",    0xf8,   simple1b_cb, &rp_symbol);
INSN(rst_symbol,   "RST",   0xc7,   aludst_cb,   &rm_symbol);
INSN(ral_symbol,   "RAL",   0x17,   simple1b_cb, &rst_symbol);
INSN(rar_symbol,   "RAR",   0x1f,   simple1b_cb, &ral_symbol);
INSN(rlc_symbol,   "RLC",   0x07,   simple1b_cb, &rar_symbol);
INSN(rrc_symbol,   "RRC",   0x0f,   simple1b_cb, &rlc_symbol);

INSN(shl_symbol,   "SHL",   OP_SHL, operator_cb, NULL);
INSN(shr_symbol,   "SHR",   OP_SHR, operator_cb, &shl_symbol);
INSN(set_symbol,   "SET",   0,      set_cb,      &shr_symbol);
VALUE(sp_symbol,   "SP",    6,                   &set_symbol);
INSN(stax_symbol,  "STAX",  0x02,   rp_cb,       &sp_symbol);
INSN(sta_symbol,   "STA",   0x32,   simple3b_cb, &stax_symbol);
INSN(sub_symbol,   "SUB",   0x90,   alusrc_cb,   &sta_symbol);
INSN(sbb_symbol,   "SBB",   0x98,   alusrc_cb,   &sub_symbol);
INSN(sbi_symbol,   "SBI",   0xde,   simple2b_cb, &sbb_symbol);
INSN(sui_symbol,   "SUI",   0xd6,   simple2b_cb, &sbi_symbol);
INSN(shld_symbol,  "SHLD",  0x22,   simple3b_cb, &sui_symbol);
INSN(sphl_symbol,  "SPHL",  0xf9,   simple1b_cb, &shld_symbol);
INSN(stc_symbol,   "STC",   0x37,   simple1b_cb, &sphl_symbol);

INSN(title_symbol, "TITLE", 0,      title_cb, NULL);

INSN(xor_symbol,   "XOR",   OP_XOR, operator_cb, NULL);
INSN(xra_symbol,   "XRA",   0xa8,   alusrc_cb,   &xor_symbol);
INSN(xri_symbol,   "XRI",   0xee,   simple2b_cb, &xra_symbol);
INSN(xchg_symbol,  "XCHG",  0xeb,   simple1b_cb, &xri_symbol);
INSN(xthl_symbol,  "XTHL",  0xe3,   simple1b_cb, &xchg_symbol);

struct symbol* hashtable[32] =
{
	0, /* @ */
	&aci_symbol,
	&b_symbol,
	&cmc_symbol,
	&di_symbol,
	&ei_symbol,
	0, /* F */
	0, /* G */
	&hlt_symbol,
	&in_symbol,
	&jm_symbol,
	0, /* K */
	&lhld_symbol,
	&mvi_symbol,
	&nop_symbol,
	&out_symbol,
	&pchl_symbol,
	0, /* Q */
	&rrc_symbol,
	&stc_symbol,
	&title_symbol,
	0, /* U */
	0, /* V */
	0, /* W */
	&xthl_symbol,
	0, /* Y */
	0, /* Z */
	0, /* [ */
	0, /* \ */
	0, /* ] */
	0, /* ^ */
	0, /* _ */
};

#define asm_fcb cpm_fcb
FCB bin_fcb;
FCB prn_fcb;
int pass;
bool eol;
uint16_t lineno;
uint16_t program_counter;
uint8_t* heapptr = cpm_ram;
bool db_string_constant_hack = false;

bool bin_buffer_fixed;
uint16_t bin_buffer_base;
uint8_t bin_buffer_write_count;
uint8_t bin_buffer[128];

uint8_t input_buffer_read_count;
uint8_t token_length;
uint8_t token_buffer[64];
uint16_t token_number;
struct symbol* token_symbol;

struct symbol* current_label;
struct symbol* current_insn;

extern token_t read_expression(void);

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

void printi(uint16_t v)
{
	bool zerosup = true;
	uint16_t precision = 10000;
    while (precision)
    {
        uint8_t d = v / precision;
        v %= precision;
        precision /= 10;
        if ((d != 0) || (precision == 0) || !zerosup)
        {
            zerosup = false;
            putchar('0' + d);
        }
    }
}

void fatal(const char* s) __z88dk_fastcall
{
	printi(lineno);
	print(": ");
	printx(s);
	cpm_exit();
}

uint8_t* allocmem(uint8_t len)
{
	uint8_t* ptr = heapptr;
	heapptr += len;
	return ptr;
}

uint8_t get_drive_or_default(uint8_t dr)
{
	if (dr == ' ')
		return 0;
	return dr - '@';
}

void open_output_file(FCB* fcb)
{
	if (fcb->dr > 16)
		return;
	
	cpm_delete_file(fcb);
	if (cpm_make_file(fcb) == 0xff)
		fatal("Cannot create output file");
}

void close_output_file(FCB* fcb)
{
	if (cpm_close_file(fcb) == 0xff)
		fatal("Cannot close output file");
}

uint8_t read_byte(void)
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

void unread_byte(uint8_t b)
{
	cpm_default_dma[--input_buffer_read_count] = b;
}

void emit_flush(void)
{
	if (bin_buffer_write_count != 0)
	{
		cpm_set_dma(bin_buffer);
		if (cpm_write_sequential(&bin_fcb) != 0)
			fatal("Error writing output file");
	}
}

void emit8_raw(uint8_t b)
{
	bin_buffer[bin_buffer_write_count++] = b;
	if (bin_buffer_write_count == sizeof(bin_buffer))
	{
		emit_flush();
		bin_buffer_base += sizeof(bin_buffer);
		bin_buffer_write_count = 0;
	}
}

void emit8(uint8_t b)
{
	if (pass == 1)
	{
		if (!bin_buffer_fixed)
		{
			bin_buffer_fixed = true;
			bin_buffer_base = program_counter;
		}

		uint16_t delta = program_counter - (bin_buffer_base + bin_buffer_write_count);
		while (delta--)
			emit8_raw(0);
		emit8_raw(b);
	}

	program_counter++;
}

void emit16(uint16_t w)
{
	emit8(w);
	emit8(w >> 8);
}

bool isident(uint8_t c)
{
	return isalnum(c) || (c == '_');
}

void check_token_buffer_size(void)
{
	if (token_length == sizeof(token_buffer))
		fatal("token too long");
}

token_t read_token(void)
{
	uint8_t c;

	if (eol)
	{
		lineno++;
		eol = false;
	}

	do
		c = read_byte();
	while ((c == ' ') || (c == '\t') || (c == '\r') || (c == '\f') || (c == '\v'));

	if (c == ';')
	{
		do
			c = read_byte();
		while ((c != '!') && (c != '\n') && (c != 26));
	}

	c = toupper(c);
	token_length = 0;
	if (isdigit(c))
	{
		for (;;)
		{
			check_token_buffer_size();
			token_buffer[token_length++] = c;

			do
				c = toupper(read_byte());
			while (c == '$');
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

			do
				c = toupper(read_byte());
			while (c == '$');
			if (!isident(c))
				break;
		}
		unread_byte(c);

		/* Search for this identifier in the symbol table. */

		uint8_t slot = token_buffer[0] & 0x1f;
		token_symbol = hashtable[slot];
		while (token_symbol)
		{
			if (token_symbol->namelen == token_length)
			{
				if (memcmp(token_symbol->name, token_buffer, token_length) == 0)
					return TOKEN_IDENTIFIER;
			}
			token_symbol = token_symbol->next;
		}

		token_symbol = (struct symbol*) allocmem(sizeof(struct symbol) + token_length);
		token_symbol->value = 0;
		token_symbol->callback = undeflabel_cb;
		token_symbol->namelen = token_length;
		memcpy(token_symbol->name, token_buffer, token_length);
		token_symbol->next = hashtable[slot];
		hashtable[slot] = token_symbol;

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
	else if (c == '\n')
		eol = true;
	else if (c == '!')
		c = TOKEN_NL;
	else if (c == 0)
		c = TOKEN_EOF;

	/* Everything else is a single-character token */
	return c;
}

/* Tests if the current symbol is a label or not. */
bool islabel(void)
{
	return (token_symbol->callback == undeflabel_cb)
		|| (token_symbol->callback == equlabel_cb)
		|| (token_symbol->callback == setlabel_cb);
}

void syntax_error(void)
{
	fatal("syntax error");
}

void expect(token_t wanted)
{
	if (read_token() != wanted)
		syntax_error();
}

#define STACK_DEPTH 32
uint16_t value_stack[STACK_DEPTH];
uint8_t operator_stack[STACK_DEPTH];
uint8_t value_sp = 1;
uint8_t operator_sp = 1;

void push_value(uint16_t value)
{
	if (value_sp == STACK_DEPTH)
		fatal("expression stack overflow");
	value_stack[value_sp++] = value;
}

uint16_t pop_value(void)
{
	if (value_sp == 0)
		fatal("expression stack underflow");
	return value_stack[--value_sp];
}

void apply_operator(uint8_t opid)
{
	const struct operator* op = &operators[opid];

	#if defined EXPR_DEBUG
		print("applying ");
		printhex8(opid);
		crlf();
	#endif

	uint16_t right = pop_value();
	if (op->binary)
	{
		uint16_t left = pop_value();
		push_value(op->callback(left, right));
	}
	else
		push_value(op->callback(right, 0));
}

void push_operator(uint8_t opid)
{
	if (operator_sp == STACK_DEPTH)
		fatal("operator stack overflow");
	operator_stack[operator_sp++] = opid;
}

void push_and_apply_operator(uint8_t opid)
{
	const struct operator* op = &operators[opid];
	while (operator_sp != 0)
	{
		uint8_t topopid = operator_stack[operator_sp-1];
		const struct operator* topop = &operators[topopid];
		if (topop->precedence <= op->precedence)
		{
			apply_operator(topopid);
			operator_sp--;
		}
		else
			break;
	}

	push_operator(opid);
}

#if defined EXPR_DEBUG
	void printstacks(void)
	{
		print("v: ");
		for (unsigned i=0; i<value_sp; i++)
		{
			printhex16(value_stack[i]);
			putchar(' ');
		}
		crlf();

		print("o: ");
		for (unsigned i=0; i<operator_sp; i++)
		{
			printhex8(operator_stack[i]);
			putchar(' ');
		}
		crlf();
	}
#endif

/* Includes terminator (newline or comma).
 * The value is written to token_number. */
token_t read_expression(void)
{
	token_t t;
	uint16_t v;

	#if defined EXPR_DEBUG
		printx("read expression");
	#endif

	value_sp = operator_sp = 0;
	bool seenvalue = false;
	for (;;)
	{
		#if defined EXPR_DEBUG
			printstacks();
		#endif

		t = read_token();

		#if defined EXPR_DEBUG
			print("token: ");
			printhex16(t);
			crlf();
		#endif

		switch (t)
		{
			case '$':
				if (seenvalue)
					syntax_error();

				push_value(program_counter);
				seenvalue = true;
				break;

			case TOKEN_NUMBER:
				if (seenvalue)
					syntax_error();

				push_value(token_number);
				seenvalue = true;
				break;
			
			case TOKEN_STRING:
				if (seenvalue)
					syntax_error();

				/* Special hack for db */
				if (db_string_constant_hack && (value_sp == 0) && (operator_sp == 0))
					return t;

				v = token_buffer[0];
				if (token_length == 2)
					v = (v<<8) | token_buffer[1];
				if ((token_length != 1) && (token_length != 2))
					fatal("invalid character constant");

				push_value(v);
				seenvalue = true;
				break;

			case '+':
				if (seenvalue)
					push_and_apply_operator(OP_ADD);
				seenvalue = false;
				break;

			case '-':
				if (seenvalue)
					push_and_apply_operator(OP_SUB);
				else
					push_and_apply_operator(OP_NEG);
				seenvalue = false;
				break;

			case '*':
				if (!seenvalue)
					syntax_error();
				push_and_apply_operator(OP_MUL);
				seenvalue = false;
				break;

			case '/':
				if (!seenvalue)
					syntax_error();
				push_and_apply_operator(OP_DIV);
				seenvalue = false;
				break;

			case '(':
				if (seenvalue)
					syntax_error();

				push_operator(OP_PAR);
				seenvalue = false;
				break;

			case ')':
				if (!seenvalue)
					syntax_error();

				for (;;)
				{
					if (operator_sp == 0)
						fatal("unbalanced parentheses");
					uint8_t opid = operator_stack[--operator_sp];
					if (opid == OP_PAR)
						break;
					apply_operator(opid);
				}
				break;

			case TOKEN_IDENTIFIER:
				if (token_symbol->callback == operator_cb)
				{
					const struct operator* op = &operators[token_symbol->value];
					if (op->binary != seenvalue)
						syntax_error();
						
					push_and_apply_operator(token_symbol->value);
					seenvalue = false;
				}
				else if (islabel())
				{
					if (seenvalue)
						syntax_error();

					push_value(token_symbol->value);
					seenvalue = true;
				}
				else
					syntax_error();
				break;

			default:
				goto terminate;
		}
	}
terminate:

	#if defined EXPR_DEBUG
		printx("unstacking");
	#endif

	while (operator_sp != 0)
	{
		#if defined EXPR_DEBUG
			printstacks();
		#endif

		uint8_t opid = operator_stack[--operator_sp];
		apply_operator(opid);
	}

	token_number = value_stack[0];

	#if defined EXPR_DEBUG
		print("result: ");
		printhex16(token_number);
		crlf();
	#endif

	return t;
}

void expect_expression(void)
{
	if (read_expression() != TOKEN_NL)
		syntax_error();
}

void operator_cb(void)   { fatal("operators are not instructions"); }
void setlabel_cb(void)   { fatal("values are not instructions"); }
void equlabel_cb(void)   { setlabel_cb(); }
void undeflabel_cb(void) { fatal("unrecognised instruction"); }

void set_implicit_label(void)
{
	if (!current_label)
		return;

	if ((current_label->value != program_counter) &&
	    (current_label->callback != undeflabel_cb))
		fatal("label already defined");

	current_label->value = program_counter;
	current_label->callback = equlabel_cb;
}

void title_cb(void)
{
	expect(TOKEN_STRING);
	if (pass == 0)
	{
		print("Title: ");
		printn(token_buffer, token_length);
		crlf();
	}

	expect(TOKEN_NL);
}

void equ_cb(void)
{
	if (!current_label)
		syntax_error();
	expect_expression();

	if ((current_label->value != token_number) &&
	    (current_label->callback != undeflabel_cb))
		fatal("label already defined");

	current_label->value = token_number;
	current_label->callback = equlabel_cb;
}

void set_cb(void)
{
	if (!current_label)
		syntax_error();

	if (current_label->callback == equlabel_cb)
		fatal("label already defined");

	expect_expression();
	current_label->value = token_number;
	current_label->callback = setlabel_cb;
}

void if_cb(void)
{
	expect_expression();

	if (token_number)
	{
		/* true case; do nothing */
	}
	else
	{
		for (;;)
		{
			token_t t = read_token();
			if (t == TOKEN_EOF)
				fatal("unexpected end of file");
			if ((t == TOKEN_IDENTIFIER) &&
				((token_symbol->callback == endif_cb) || (token_symbol->callback == else_cb)))
				break;
		}
		expect(TOKEN_NL);
	}
}

void else_cb(void)
{
	/* If this pseudoop actually gets executed, then we've been executing the
	 * true branch of the if...endif. Skip to the end. */

	for (;;)
	{
		token_t t = read_token();
		if (t == TOKEN_EOF)
			fatal("unexpected end of file");
		if ((t == TOKEN_IDENTIFIER) && (token_symbol->callback == endif_cb))
			break;
	}
	expect(TOKEN_NL);
}

void endif_cb(void)
{
	expect(TOKEN_NL);
}

void org_cb(void)
{
	expect_expression();
	if (token_number < program_counter)
		fatal("program counter moved backwards");

	program_counter = token_number;
}

void db_cb(void)
{
	db_string_constant_hack = true;

	token_t t;
	do
	{
		t = read_expression();
		if (t == TOKEN_STRING)
		{
			for (unsigned i=0; i<token_length; i++)
				emit8(token_buffer[i]);
			t = read_token();
			if ((t != TOKEN_NL) && (t != ','))
				syntax_error();
		}
		else
			emit8(token_number);
	}
	while (t == ',');

	db_string_constant_hack = false;
}

void dw_cb(void)
{
	token_t t;
	do
	{
		t = read_expression();
		emit16(token_number);
	}
	while (t == ',');
}

void ds_cb(void)
{
	expect_expression();
	program_counter += token_number;
}

void simple1b_cb(void)
{
	expect_expression();
	emit8(current_insn->value);
}

void simple2b_cb(void)
{
	expect_expression();
	emit8(current_insn->value);
	emit8(token_number);
}

void simple3b_cb(void)
{
	expect_expression();
	emit8(current_insn->value);
	emit16(token_number);
}

void alusrc_cb(void)
{
	expect_expression();
	emit8(current_insn->value | token_number);
}

void aludst_cb(void)
{
	expect_expression();
	emit8(current_insn->value | (token_number << 3));
}

void rp_cb(void)
{
	expect_expression();
	emit8(current_insn->value | ((token_number & 6) << 3));
}

void mov_cb(void)
{
	if (read_expression() != ',')
		syntax_error();
	uint16_t dest = token_number;
	expect_expression();
	uint16_t src = token_number;

	emit8(0x40 | (dest<<3) | src);
}

void lxi_cb(void)
{
	if (read_expression() != ',')
		syntax_error();
	emit8(0x01 | ((token_number & 6) << 3));

	expect_expression();
	emit16(token_number);
}

void mvi_cb(void)
{
	if (read_expression() != ',')
		syntax_error();
	uint16_t dest = token_number;
	expect_expression();

	emit8(0x06 | (dest<<3));
	emit8(token_number);
}

void end_cb(void) {}

void main(void)
{
	memcpy(&bin_fcb, &cpm_fcb, sizeof(FCB));
	bin_fcb.dr = get_drive_or_default(cpm_fcb.f[9]);
	memcpy(&bin_fcb.f[8], "BIN", 3);

	memcpy(&prn_fcb, &cpm_fcb, sizeof(FCB));
	prn_fcb.dr = get_drive_or_default(cpm_fcb.f[10]);
	memcpy(&prn_fcb.f[8], "PRN", 3);

	asm_fcb.dr = get_drive_or_default(cpm_fcb.f[8]);
	memcpy(&asm_fcb.f[8], "ASM", 3);

	open_output_file(&bin_fcb);
	bin_buffer_write_count = 0;
	bin_buffer_base = 0;
	bin_buffer_fixed = false;

	open_output_file(&prn_fcb);

	if (cpm_open_file(&asm_fcb) == 0xff)
		fatal("Cannot open input file");

	for (pass=0; pass<2; pass++)
	{
		asm_fcb.ex = asm_fcb.cr = 0;
		input_buffer_read_count = 0x80;
		program_counter = 0;
		eol = true;
		lineno = 0;

		print("Pass ");
		printi(pass);
		crlf();

		/* Each statement consists of:
		 * LABEL [:] INSTRUCTION [operands...]
		 *
		 * foo: mvi a, 1
		 * foo mvi a, 1
		 * mvi a, 1
		 * foo
		 *
		 * ...are all valid.
		 */

		for (;;)
		{
			token_t t = read_token();
			if (t == TOKEN_EOF)
				break;
			if (t == TOKEN_NL)
				continue;

			if (t != TOKEN_IDENTIFIER)
				fatal("expected an identifier");
			if (token_symbol->callback == end_cb)
				break;

			current_label = NULL;
			current_insn = NULL;

			if (islabel())
			{
				current_label = token_symbol;
				t = read_token();
				if (t == ':')
					t = read_token();
			}

			if (t != TOKEN_NL)
			{
				if (t != TOKEN_IDENTIFIER)
					fatal("expected an identifier");

				current_insn = token_symbol;
			}

			if (current_insn)
			{
				void (*cb)(void) = current_insn->callback;
				if ((cb != set_cb) && (cb != equ_cb))
					set_implicit_label();
				cb();
			}
			else
				set_implicit_label();
		}
	}

	if (bin_buffer_write_count != 0)
	{
		while (bin_buffer_write_count != 0)
			emit8_raw(0);
	}

	close_output_file(&bin_fcb);
	close_output_file(&prn_fcb);
}

