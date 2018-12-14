/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "third_party/zmac/zmac.y" /* yacc.c:339  */

// GWP - keep track of version via hand-maintained date stamp.
#define VERSION "5dec2018"

/*
 *  zmac -- macro cross-assembler for the Zilog Z80 microprocessor
 *
 *  Bruce Norskog	4/78
 *
 *  Last modification  1-18-87 by cdk
 *  This assembler is modeled after the Intel 8080 macro cross-assembler
 *  for the Intel 8080 by Ken Borgendale.  The major features are:
 *	1.  Full macro capabilities
 *	2.  Conditional assembly
 *	3.  A very flexible set of listing options and pseudo-ops
 *	4.  Symbol table output
 *	5.  Error report
 *	6.  Elimination of sequential searching
 *	7.  Commenting of source
 *	8.  Facilities for system definiton files
 *
 * Revision History:
 *
 * jrp	3-8-82	Converted to run on Vax, updated syntax to conform better
 *		to the Zilog standard.
 *
 * jrp	3-15-82	Added underscore as a character type in the lex table
 *		'numpart' (0x5F).
 *
 *		Changed maximum number of characters in a label to 15
 *		from 7. Note that 'putsymtab' uses this value inside
 *		of a quoted string, so we use 15.
 *
 * jrp	2-15-83	Fixed 'getlocal' to return better local labels. It used
 *		to crash after 6 invocations.
 *
 * jrp	6-7-83	Fixed bug in the ADD IX,... instruction.
 *
 * jrp	5-11-84	Added code to print unused labels out with the symbol table
 *		Also sped up the macro processor by using stdio.
 *
 * jrp 5-22-84	Added include files ala ormac
 *
 * jrp 8-27-84	Added PHASE/DEPHASE commands
 *
 * cdk 9-20-86	Converted to run on a Pyramid.  This meant changing yylval
 *		to be a %union, and then putting in the appropriate
 *		typecasts where ints are pointers are used interchangeably.
 *		The current version still probably won't run on machines where
 *		sizeof(int) != sizeof(char *).
 *		Also changed emit() to use varargs, and got rid of the
 *		old style = in front of yacc action code.
 *			-Colin Kelley  vu-vlsi!colin
 *
 * cdk 10-2-86	Added some more typecasts to keep lint a little happier.
 *		Removed several unused variables.  Changed most vars
 *		declared as char to int, since many of them were being
 *		compared with -1!  I still don't know what's going on with
 *		est[][] being malloc'd and free'd everywhere...it looks pretty
 *		fishy...
 *
 * cdk 1-18-87  Added MIO code to emulate 'mfile' using malloc()'d memory.
 *		This was needed to get the code to work when compiled under
 *		MSC 4.0 on a PC, and it's probably faster anyway.
 *
 * cdk 2-5-87	Added 'cmp' as a synonym for 'cp', 'jmp' as a synonym for
 *		'jp', and added tolerance of accumulator specification for arithmetic
 *		and logical instructions.  (For example, 'or a,12' is now accepted,
 *		same as 'or 12'.)
 *
 * gwp 12-29-08	Changes to allow compilation with modern C compiler and using bison
 *		as the .y to .c converter.  assert, tstate pseudo-ops.
 *		t(), tilo(), tihi() functions.  ==, <=, >=, !=, !, <, > operators.
 *		-c to turn cycle counts off in listing.  Usage, -h and version.
 *
 * gwp 9-26-10	Add ocf() and setocf to track and set op code fetch counts.
 *		Add sett as an alias for tstate
 *
 * gwp 12-30-11	Add undocumented instructions sl1, pfix, pfiy, in (c), out (c),0
 *		bit/set/res (ixy+d),reg and ld/inc/dec ixylh.
 *
 * gwp 2-8-12   Increase MAXIFS massively due to massive lowt macro
 *
 * gwp 2-11-12  Support 32 bit constants.  '%' alias for MOD.  Add defd, dword.
 *		lo(x) and hi(x) for easy low and high byte extraction.  Allow
 *		filenames longer than 15 characters.  All output to "zout" subdirectory
 *		of source file.
 *
 * gwp 2-15-12	Perform multiple passes while equates are changing.  Support
 *		.label for temporary label definitions and _label for file
 *		scoped labels.  Allow '.' within labels.  Assert listing bugfix.
 *
 * gwp 4-27-12	Implement $ prefixed hex constants and double-quoted strings.
 *
 * gwp 6-30-12	Minor changes to allow compilation with gcc.
 *
 * gwp 9-05-12	incbin added.
 *
 * gwp 11-24-12	Fix macro expansion bug when symbol larger than MAXSYMBOLSIZE
 *		due to file name prepending when symbol starts with '_'.
 *
 * gwp 12-04-12	Optional JR promotion and JP demotion errors.  Output a warning
 *		if no execute address given.  Output ".bds" file to enable easy
 *		simple source level debugging.
 *
 * gwp 4-14-13	Parens in expressions, else, .pseudo, full set of C operators
 *		with conventional precedence and various aliases and code
 *		changes to make source similar to zmac 1.3 on internet.
 *
 * gwp 5-5-13	.cmd,.cas,.lcas,.bin output.  dc (both MACRO-80 and EDAS types).
 *		lo, hi renamed to low, high and make unary operators.  Allow
 *		label::, placeholder public, extern declarations.  Bug fixes
 *		in defs, t, ocf, tihi, tilo in phase mode.  Initial support
 *		for -I include directories. 0x hex constants. --mras flag for
 *		limited MRAS compatibility (allows $ in labels, $? to start
 *		labels).
 *
 * gwp 4-6-13	--rel for .rel (Microsoft linker) output and extern, public,
 *		aseg, cseg, dseg in support (big emit + expression revamp).
 *		-I follows C preprocessor convention, output relative to
 *		current directory.  8080 mnemonics, .z80, .8080, -z, -8.
 *		Change .bin to .cim.  Warn on labels not in first column.
 *
 * gwp 8-11-13	Allow $ to start identifiers and do '$' dropping when macro
 *              parsed so we no longer need to drop '$' in identifiers. 
 *              Even $FCB allowed, with warning.  Add --zmac for backwards
 *		compatibility with zmac.  ` now used for joining in macros.
 *		Most reserved words can be used as labels or variables.
 *		Free-form title, name, comment, subttl parsing.  Allow #arg
 *		for macro arguments (in --mras mode).  Support <CR> delimited
 *		files.  Add .ams output.  Integrate documentation (--doc).
 *
 * gwp 3-12-14	Emit jr even if out of range.  z80.lib support.
 *		Warning and bug fixes from Mark Galanger.
 *		Macros can override built-ins and are no longer listed
 *		in symbol table.  A, B, C, D, E, H, L, M, PSW, SP are
 *		pre-defined values which can be used in data statements
 *		(db, dw, dd).  Reserved words can be equated but are only
 *		accessbile in data.  SET can be used in place of DEFL
 *		(MAC and MACRO-80 agree on this).  '=' can be used in place
 *		of EQU. 'maclib file' includes 'file.lib'.  Bug fix in "dw 0,$".
 *		Removed error flagging in expressions which could cause parse
 *		to fail from that point onwards.
 *		expression(ix) equivalent to (ix + expression).
 *		Macro expanded lines didn't go through the line analyzer.
 *		Empty macro arguments (e.g., mac 1,,2)
 *		Implemented rept, irp, irpc, exitm.  Add more detail on phase
 *		errors. '' is an empty string in db/ascii/etc, 0 otherwise.
 *		Almost any name can be used as a macro parameter name.
 *		Allow 'if' in first column.
 *		Fix .rel mode bug in dc, incbin.
 *		Emit .bds output for dc, incbin.
 *		Allow assembly to wrap past end of memory.
 *		"pragma bds" for raw output to .bds file.  Also output equates
 *		to .bds file.
 *		Macro bug fix from Sergey Erokhin.
 *
 * gwp 9-5-16	Allow ' on identifiers for manual prime register tracking.
 *		Improve MRAS support with *LIST, *INCLUDE and Pk=n parameters.
 *
 * gwp 20-9-16	Big MRAS compatibility fixes.  Can do MRAS operator precedence,
 *		proper .operator. tokenization and agressive macro parameter
 *		substituion.  Change Pk=n to -Pk=n.  Add ++, += and variants
 *		for more compact variable adjustment than defl.  First crack
 *		at .tap output for ZX Spectrum support.
 *
 * gwp 13-8-17	Add "import" for simple once-only inclusion of files.
 *		Track full path so relative includes work properly.
 *		Allow push af', pop af' for notational convenience.
 *		Add "bytes" as alias for "dc".  Fix --rel output bugs in
 *		low(), high(), div and mod.
 *
 * gwp 12-3-18	250 baud .cas output and .wav format.  Common blocks.
 *		--oo, --xo, --od to control output.  Delete output on fail.
 *
 * gwp 2-6-18	1000 baud .cas ouput and .mds (MAME/MESS debug script) output.
 *
 * gwp 28-7-18	Double free of output files bug fix from Pedro Gimeno.  Don't
 *		output SYSTEM files if no entry point thanks to Tim Halloran.
 *
 * gwp 6-10-18	Stop quoted inclued file crash on for OSX (and linux?). Thanks
 *		to Paulo Silva. Added "dm" as "ascii" alias for Blair Robins.
 *
 * gwp 5-12-18	Peter Wilson ran into quoted include crash.  Also suggested
 *		import be allowed in column 0 and noted that keywords like
 *		IF and LIST could not be labels even if given colons.
 */

#if defined(__GNUC__)
#pragma GCC diagnostic error "-Wreturn-type"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>

#ifdef WIN32
#include <windows.h>	// just for colouring the output
#include <direct.h>		// _mkdir
#ifdef _MSC_VER
#define strdup _strdup
#define unlink _unlink
#endif
#endif

#if defined(__APPLE__) || defined(__linux__)
#include <unistd.h>	// just for unlink
#endif

#include "zi80dis.h"

#ifdef vax11c
#define unlink(filename) delete(filename)
#endif

#include "mio.h"

/*
 * DEBUG turns on pass reporting.
 * DBUG enables -d to allow yacc/bison yydebug increment (but must be passed
 *	on the command line)
 * Macro debug and Token debug enables.
#define	DEBUG
#define	M_DEBUG	
#define	T_DEBUG
 */

#ifdef DBUG
#define YYDEBUG 1
#endif

#define ITEMTABLESIZE	100000
#define TEMPBUFSIZE	(1000+MAXSYMBOLSIZE)
#define LINEBUFFERSIZE	1000
#define EMITBUFFERSIZE	200
#define MAXSYMBOLSIZE	40
#define IFSTACKSIZE	20
// GWP - I use lots of if's with my lowt macro
#define MAXIFS		65536
#define TITLELEN	50
#define BINPERLINE	16
#define	PARMMAX		25
#define MAXEXP		25
#define SYMMAJIC	07203
#define	NEST_IN		32
#define MAXPASS		32
#define MAXINCPATH	32

int iflist();
int yylex();
int phaseaddr(int addr);
int nextchar();
int getcol();
int skipline(int ac);
int tokenofitem(int deftoken, int keyexclude, int keyinclude);
int getm();
int counterr();
int countwarn();
int convert(char *buf, char *bufend, int *overflow);

void yyerror(char *err)
{}		/* we will do our own error printing */

struct argparse {
	char *arg;	// output buffer for argument
	int argsize;	// size of output buffer
	int (*getch)(struct argparse *); // get next character
	int *peek;	// pointer single item pushback buffer
	int macarg;	// working on macro arguments

	char *user_ptr;	// state for getch
	int user_int;	// state for getch
	int user_peek;	// state for getch

	int didarg;	// internal parsing state
	int numarg;	// internal parsing state
};

int getarg(struct argparse *ap);

struct	item	{
	char	*i_string;
	int	i_value;
	int	i_token;
	int	i_uses;
	int	i_scope;
	int	i_chain;
	int	i_pass;
};

void itemcpy(struct item *dst, struct item *src);
struct item *keyword(char *name);

#define SCOPE_NONE	(0)
#define SCOPE_PROGRAM	(1)
#define SCOPE_DATA	(2)
#define SCOPE_COMMON	(3)
#define SCOPE_PUBLIC	(4)
#define SCOPE_EXTERNAL	(8)
#define SCOPE_NORELOC	(16)
#define SCOPE_BUILTIN	(32)	/* abuse */
#define SCOPE_COMBLOCK	(64)

#define SCOPE_SEGMASK	(3)
#define SCOPE_SEG(s)	((s) & SCOPE_SEGMASK)

struct expr {
	int e_value;
	int e_scope;
	int e_token;
	struct item *e_item;
	struct expr *e_left;
	struct expr *e_right;
};

#define EXPR_SEG(e)	SCOPE_SEG(e->e_scope)

FILE	*fout,
	*fbuf,
	*fbds,
	*fcmd,
	*fcas,
	*flcas,
	*flnwcas,
	*ftcas,
	*fcim,
	*fams,
	*frel,
	*ftap,
	*fmds,
	*f1500wav,
	*f1000wav,
	*f500wav,
	*f250wav,
	*fin[NEST_IN],
	*now_file ;

char *output_dir = "zout";

struct OutputFile {
	char *suffix;
	char *mode;
	FILE **fpp;
	int system; // A cassette SYSTEM file
	int no_open;
	int wanted; // output file has been explicitly selected
	char *filename;
	int temp;
}
outf[] = {
	{ "lst",	"w",	&fout		},
	{ "hex", 	"w",	&fbuf		},
	{ "bds",	"w",	&fbds		},
	{ "cmd",	"wb",	&fcmd		},
	{ "1500.wav",	"wb",	&f1500wav, 1	}, // must be 1 before 1500.cas
	{ "1500.cas",	"w+b",	&fcas,	   1	},
	{ "1000.wav",	"wb",	&f1000wav, 1	}, // must be 1 before 1000.cas
	{ "1000.cas",	"w+b",	&flnwcas,  1	},
	{ "500.wav",	"wb",	&f500wav,  1	}, // must be 1 before 500.cas
	{ "500.cas",	"w+b",	&flcas,    1	},
	{ "250.wav",	"wb",	&f250wav	}, // must be 1 before 250.cas
	{ "250.cas",	"w+b",	&ftcas		},
	{ "cim",	"wb",	&fcim		},
	{ "ams",	"wb",	&fams		},
	{ "rel",	"wb",	&frel,	0, 1	},
	{ "tap",	"wb",	&ftap		},
	{ "mds",	"w",	&fmds		},
};
#define CNT_OUTF (sizeof outf / sizeof outf[0])

int getoptarg(int argc, char *argv[], int i);
void stop_all_outf();
void clean_outf();
void clean_outf_temp();
void suffix_list(char *sfx_lst, int no_open);

int	pass2;	/*set when pass one completed*/
int	outpass; 	// set when we decide to stop doing passes */
int	passfail;	// set when an error means passes should not continue
int	passretry;	// set when an inconsistency will require another pass
int	dollarsign ;	/* location counter */
int	olddollar ;	/* kept to put out binary */
int	oldothdollar;	// output address of next .cmd/.cas/.lcas block
int	emit_addr;	// where code and data are being placed in memory
int	tstates;	// cumulative T-states
int	ocf;		// cumulative op code fetches
int	llseq;		// local label sequence number
int	mras;		// MRAS semi-compatibility mode
int	trueval = 1;	// Value returned for boolean true
int	zcompat;	// Original zmac compatibility mode
char	modstr[8];	// Replacement string for '?' in labels when MRAS compatible
int	relopt;		// Only output .rel files
int relsymlen = 6; // Length of external symbols
char	progname[8];	// Program name for .rel output
int	note_depend;	// Print names of files included
int	firstcol;
int	logcol;
int	coloncnt;
int	full_exprs;	// expression parsing mode allowing almost any identifier
struct item *label, pristine_label; // 
int	list_dollarsign;// flag used to change list output for some operations
int	list_addr;	// address to display for operation if !list_dollarsign

// Include file search path
char	*incpath[MAXINCPATH];
int	incpath_cnt;

/* program counter save for PHASE/DEPHASE */
int	phdollar, phbegin, phaseflag ;

char	*src_name[NEST_IN] ;
int	linein[NEST_IN] ;
int linepeek[NEST_IN];
int	now_in ;


// These first 5 errors are singled out in lsterr1() for reasons I don't
// quite understand.
#define bflag	0	/* balance error */
#define eflag	1	/* expression error */
#define fflag	2	/* syntax error */
#define iflag	3	/* bad digits */
#define mflag	4	/* multiply defined */

#define pflag	5	/* phase error */
#define uflag	6	/* undeclared used */
#define vflag	7	/* value out of range */
#define oflag	8	/* phase/dephase error */
#define aflag	9	/* assert failed */
#define jflag	10	/* JP could be JR */
#define rflag	11	/* expression not relocatable */
#define gflag	12	/* incorrect register */
#define zflag	13	/* Z-80 instruction */

#define FIRSTWARN	warn_hex
#define	warn_hex	14
#define warn_notimpl	15
#define warn_general	16

#define FLAGS	17	/* number of errors and warnings */

char	err[FLAGS];
int	keeperr[FLAGS];
char	errlet[FLAGS]="BEFIMPUVOAJRGZHNW";
char	*errname[FLAGS]={
	"Balance",
	"Expression",
	"Syntax",
	"Digit",
	"Mult. def.",
	"Phase",
	"Undeclared",
	"Value",
	"Phase/Dephase",
	"Assertion failure",
	"Use JR",
	"Not relocatable",
	"Register usage",
	"Z-80 instruction in 8080 mode",
	"$hex constant interpreted as symbol",
	"Not implemented",
	"General"
};
char	errdetail[FLAGS][1024];
char	detail[1024];


unsigned char inpbuf[LINEBUFFERSIZE];
unsigned char inpbuf_insert[LINEBUFFERSIZE];
unsigned char *inpptr;

char	linebuf[LINEBUFFERSIZE];
char	*lineptr;
char	*linemax = linebuf+LINEBUFFERSIZE;

char	outbin[BINPERLINE];
char	*outbinp = outbin;
char	*outbinm = outbin+BINPERLINE;

char	outoth[256];
int	outoth_cnt = 0;

unsigned char	emitbuf[EMITBUFFERSIZE];
unsigned char	*emitptr;

char	ifstack[IFSTACKSIZE];
char	*ifptr;
char	*ifstmax = ifstack+IFSTACKSIZE-1;


char	hexadec[] = "0123456789ABCDEF" ;


int	nitems;
int	linecnt;
int	nbytes;
int	invented;
int	npass;
int	njrpromo;


char	tempbuf[TEMPBUFSIZE];
char	*tempmax = tempbuf+TEMPBUFSIZE-1;

char	arg_flag;
struct argparse arg_state;
void	arg_start();
void	arg_reset();
int	str_getch(struct argparse *ap);

int	mras_undecl_ok;
int	mras_param[10];

char	inmlex;
char	mlex_list_on;
int	parm_number;
int	exp_number;
char	symlong[] = "Symbol/number too long";
int	raw;

int	disp;

int	param_parse;
#define PARAMTABSIZE (PARMMAX * 2)
struct item paramtab[PARAMTABSIZE];

#define FLOC	PARMMAX
#define TEMPNUM	PARMMAX+1
#define REPNUM	PARMMAX+2
#define MSTART	PARMMAX+3
#define MSTR	PARMMAX+4
#define MARGP	PARMMAX+5
#define MIF	PARMMAX+6

#define PAREXT	7

union exprec {
	char *param;
	int value;
	struct argparse *ap;
};
union exprec	*est;
union exprec	*est2;
union exprec	*expstack[MAXEXP];
int	expptr;

int	floc;
int	mfptr;
FILE	*mfile;

char	*writesyms;


char	*title;
char	titlespace[TITLELEN];
char	*timp;
char	*sourcef;
/* changed to cope with filenames longer than 14 chars -rjm 1998-12-15 */
char	src[1024];
char	bin[1024];
char	listf[1024];
char	oth[1024];

char	copt = 1,	/* cycle counts in listings by default */
	edef = 1,
	eopt = 1,
	fdef = 0,
	fopt = 0,
	gdef = 1,
	gopt = 1,
	iopt = 0 ,	/* list include files */
	jopt = 0,
	JPopt = 0,
	lstoff = 0,
	lston = 0,	/* flag to force listing on */
	mdef = 0,
	mopt = 0,
	nopt = 1 ,	/* line numbers on as default */
	popt = 1,	/* form feed as default page eject */
	sopt = 0,	/* turn on symbol table listing */
	topt = 1,	/* terse, only error count to terminal */
	printer_output = 0, // GWP - printer style output
	z80,
	saveopt;

char default_jopt, default_JPopt, default_z80 = 1;

char	xeq_flag = 0;
int	xeq;

time_t	now;
int	line;
int	page = 1;

struct stab {
	char	t_name[MAXSYMBOLSIZE+1];
	int	t_value;
	int	t_token;
};

// GWP - support for cycle count tracking (and opens door for efficient .cmd, etc. output)

unsigned char memory[1 << 16];
char memflag[1 << 16];
enum {
	MEM_DATA = 1,
	MEM_INST = 2,
	MEM_T_SET = 4
};
int tstatesum[1 << 16];
int ocfsum[1 << 16];

// GWP - expression handling extensions for .rel output.
void advance_segment(int step);
void expr_reloc_check(struct expr *ex);
void expr_number_check(struct expr *ex);
void expr_scope_same(struct expr *ex1, struct expr *ex2);
void expr_word_check(struct expr *ex);
int is_number(struct expr *ex);
int is_external(struct expr *ex);
struct expr *expr_num(int value);
struct expr *expr_alloc(void);
struct expr *expr_var(struct item *var);
struct expr *expr_mk(struct expr *left, int token, struct expr *right);
struct expr *expr_op(struct expr *left, int token, struct expr *right, int value);
struct expr *expr_op_sc(struct expr *left, int token, struct expr *right, int value);
void expr_free(struct expr *ex);
int can_extend_link(struct expr *ex);
void extend_link(struct expr *ex);
void putrelop(int op);
#define RELOP_BYTE	(1)
#define RELOP_WORD	(2)
#define RELOP_HIGH	(3)
#define RELOP_LOW	(4)
#define RELOP_NOT	(5)
#define RELOP_NEG	(6)
#define RELOP_SUB	(7)
#define RELOP_ADD	(8)
#define RELOP_MUL	(9)
#define RELOP_DIV	(10)
#define RELOP_MOD	(11)
struct item *item_lookup(char *name, struct item *table, int table_size);
struct item *item_substr_lookup(char *name, int token, struct item *table, int table_size);
struct item *locate(char *name);
// Data descriptions for emit()
#define E_CODE		(0)
#define E_DATA		(1)
#define E_CODE8		(2)
#define E_CODE16	(3)
int segment;
#define SEGCHAR(s) " '\"!"[s]
#define SEG_ABS		(0)
#define SEG_CODE	(1)
#define SEG_DATA	(2)
#define SEG_COMMON	(3)
int seg_pos[4]; // may eventually support SEG_COMMON
int seg_size[4];
int rel_main;
int segchange;
struct item *cur_common;
void putout(int value);
int outrec;
int outlen;
unsigned char outbuf[1024 * 1024];


/*
 *  push back character
 */
#define NOPEEK (EOF - 1)
int	peekc;
int	nextline_peek;

/* function prototypes */
void error(char *as);
void usage(char *msg, char *param);
void help();
void list_out(int optarg, char *line_str, char type);
void erreport();
void errorprt(int errnum);
void errwarn(int errnum, char *message);
void mlex(char *look);
void popsi();
void suffix(char *str, char *suff);
char *basename(char *filename);
char *getsuffix(char *str);
void outpath(char *out, char *src, char *suff);
void casname(char *out, char *src, int maxlen);
void putm(int c);
void insymtab(char *name);
void outsymtab(char *name);
void compactsymtab();
void putsymtab();
void clear();
void setmem(int addr, int value, int type);
void setvars();
void flushbin();
void flushoth();
void lineout();
void puthex(int byte, FILE *buf);
void putcas(int byte);
void putrelbits(int count, int bits);
void putrel(int byte);
void putrelname(char *str);
void putrelextaddr(int extaddr);
void putrelcmd(int cmd);
void putrelsegref(int scope, int addr);
void flushrel(void);
void lsterr1();
void lsterr2(int lst);
void copyname(char *st1, char *st2);
void next_source(char *sp, int always);
void incbin(char *filename);
void dc(int count, int value);
char *getmraslocal();
void write_tap(int len, int org, unsigned char *data);
void write_250(int low, int high);
void writewavs(int pad250, int pad500, int pad1500);
void reset_import();
int imported(char *filename);

#define RELCMD_PUBLIC	(0)
#define RELCMD_COMMON	(1)
#define RELCMD_PROGNAME	(2)
#define RELCMD_LIBLOOK	(3)
#define RELCMD_EXTLINK	(4)
#define RELCMD_COMSIZE	(5)
#define RELCMD_EXTCHAIN	(6)
#define RELCMD_PUBVALUE	(7)
#define RELCMD_EXTMINUS	(8)
#define RELCMD_EXTPLUS	(9)
#define RELCMD_DATASIZE	(10)
#define RELCMD_SETLOC	(11)
#define RELCMD_CODESIZE	(13)
#define RELCMD_ENDMOD	(14)
#define RELCMD_ENDPROG	(15)

/*
 *  add a character to the output line buffer
 */
void addtoline(int ac)
{
	/* check for EOF from stdio */
	if (ac == -1)
		ac = 0 ;
	if (lineptr >= linemax)
		error("line buffer overflow");
	*lineptr++ = ac;
}

int get_tstates(unsigned char *inst, int *low, int *high, int *fetch)
{
	int len;

	if (z80)
		len = zi_tstates(inst, low, high, fetch, 0, 0);
	else
		len = zi_tstates(inst, 0, 0, fetch, low, high);

	return len;
}

/*
 *  put values in buffer for outputing
 */

void emit(int bytes, int desc, struct expr *data, ...)
{
	int type, i, args, dsize;
	va_list ap;

	if (relopt && segchange) {
		segchange = 0;
		putrelcmd(RELCMD_SETLOC);
		putrelsegref(segment, seg_pos[segment]);
	}

	// External references only supported in .rel output.
	if (!relopt && data && (data->e_scope & SCOPE_EXTERNAL)) {
		sprintf(detail, "External references only allowed in .rel output\n");
		errwarn(uflag, detail);
	}

	va_start(ap, data);

	type = desc == E_DATA ? MEM_DATA : MEM_INST;

	// Check emit is not adding instructions to the buffer.
	if (desc != E_DATA && emitptr != emitbuf)
		fprintf(stderr, "internal inconsistency in t-state counting\n");

	dsize = 0;
	args = bytes;
	if (desc == E_DATA) {
		args = 0;
		dsize = bytes;
	}
	else if (desc == E_CODE16)
		dsize = 2;
	else if (desc == E_CODE8)
		dsize = 1;

	for (i = 0; i < args; i++)
	{
		if (emitptr >= &emitbuf[EMITBUFFERSIZE])
			error("emit buffer overflow");
		else {
			int addr = (emit_addr + (emitptr - emitbuf)) & 0xffff;
			*emitptr = va_arg(ap, int);
			if (segment == SEG_CODE) 
				setmem(addr, *emitptr, type);
			putrel(*emitptr);
			putout(*emitptr);
			emitptr++;
		}
	}

	va_end(ap);

	for (i = 0; i < dsize; i++) {
		int addr = (emit_addr + (emitptr - emitbuf)) & 0xffff;
		*emitptr = data->e_value >> (i * 8);
		if (segment == SEG_CODE)
			setmem(addr, *emitptr, type);
		putout(*emitptr);
		emitptr++;
	}

	if (desc != E_DATA)
	{
		int eaddr = emit_addr, low, fetch, addr_after;

		// emitbuf is OK since this only happens with single emits

		if (!z80) {
			// Check for invalid 8080 instructions.
			int op = emitbuf[0] & 0xff;
			if (op == 0x08 || op == 0x10 || op == 0x18 ||
			    op == 0x20 || op == 0x28 || op == 0x30 ||
			    op == 0x38 || op == 0xCB || op == 0xD9 ||
			    op == 0xDD || op == 0xED || op == 0xFD)
			{
				err[zflag]++;
			}
		}

		get_tstates(emitbuf, &low, 0, &fetch);

		// Sanity check
		if (low <= 0)
		{
			fprintf(stderr, "undefined instruction on %02x %02x (assembler or diassembler broken)\n",
				emitbuf[0], emitbuf[1]);
		}

		// Special case to catch promotion of djnz to DEC B JP NZ
		// Even update the tstatesum[] counter though that seems to
		// me to be above and beyond.
		if (emitbuf[0] == 5 && args == 2) {
			tstatesum[eaddr] = tstates;
			ocfsum[eaddr] = ocf;
			memflag[eaddr] |= MEM_T_SET;
			eaddr++;
			tstates += low;
			ocf += fetch;
			low = 10;
			// still 1 fetch
		}

		// Double setting of both sides of tstatesum[] seems like too
		// much, but must be done in the isolated instruction case:
		// org x ; inc a ; org y

		tstatesum[eaddr] = tstates;
		ocfsum[eaddr] = ocf;
		memflag[eaddr] |= MEM_T_SET;

		// Well, OK, should we default to high or low???
		// Guess it should be whatever makes sense for you
		// to get here which, generally, is the low.

		// low it is.

		tstates += low;
		ocf += fetch;

		addr_after = (emit_addr + (emitptr - emitbuf)) & 0xffff;

		tstatesum[addr_after] = tstates;
		ocfsum[addr_after] = ocf;
		memflag[addr_after] |= MEM_T_SET;
	}

	if (relopt && outpass && dsize > 0) {
		if (dsize == 1) {
			if (is_number(data))
				putrel(data->e_value);
			else if (can_extend_link(data)) {
				extend_link(data);
				putrelop(RELOP_BYTE);
				putrel(0);
			}
			else {
				err[rflag]++;

				putrel(0);
			}
		}
		else if (dsize == 2) {
			int handled = 0;
			if (data->e_scope & SCOPE_EXTERNAL) {
				struct item *var = 0;
				int offset = 0;
				// Simple external reference.
				if (is_external(data))
					var = data->e_item;
				else if (is_external(data->e_left) &&
					data->e_token == '+' &&
					is_number(data->e_right))
				{
					var = data->e_left->e_item;
					offset = data->e_right->e_value;
				}
				else if (is_number(data->e_left) &&
					data->e_token == '+' &&
					is_external(data->e_right))
				{
					offset = data->e_left->e_value;
					var = data->e_right->e_item;
				}
				else if (is_external(data->e_left) &&
					data->e_token == '-' &&
					is_number(data->e_right))
				{
					var = data->e_left->e_item;
					offset = data->e_right->e_value;
				}

				if (var && offset) {
					putrelcmd(data->e_token == '-' ?
						RELCMD_EXTMINUS : RELCMD_EXTPLUS);
					// Theoretically we could put a
					// program or data relative value here...
					putrelsegref(SEG_ABS, offset);
				}

				if (var) {
					if (var->i_chain == 0) {
						putrel(0);
						putrel(0);
					}
					else {
						putrelbits(1, 1);
						putrelextaddr(var->i_chain);
					}
					var->i_chain = (segment << 16) |
						((dollarsign + args) & 0xffff);
					handled = 1;
				}
			}
			else if ((data->e_scope & ~SCOPE_PUBLIC) == 0) {
				// nice constant value
				putrel(data->e_value);
				putrel(data->e_value >> 8);
				handled = 1;
			}
			else if (!(data->e_scope & SCOPE_NORELOC)) {
				// relocatable value
				putrelbits(1, 1);
				putrelbits(2, data->e_scope);
				putrelbits(8, data->e_value);
				putrelbits(8, data->e_value >> 8);
				handled = 1;
			}

			if (!handled) {
				if (can_extend_link(data)) {
					extend_link(data);
					putrelop(RELOP_WORD);
					putrel(0);
					putrel(0);
				}
				else {
					err[rflag]++;
					putrel(data->e_value);
					putrel(data->e_value >> 8);
				}
			}
		}
		else if (dsize == 4) {
			// Only numbers are allowed.
			if (data->e_scope != 0) {
				err[vflag]++;
				if (data->e_scope & SCOPE_NORELOC)
					err[rflag]++;
			}
			for (i = 0; i < dsize; i++)
				putrel(data->e_value >> (i * 8));
		}
		else
			error("internal dsize error");
	}
}

#define ET_NOARG_DISP	(0)
#define ET_NOARG	(1)
#define ET_BYTE		(2)
#define ET_WORD		(5)

void emit1(int opcode, int regvalh, struct expr *data, int type)
{
	if (type == ET_BYTE && (data->e_value < -128 || data->e_value > 255))
		err[vflag]++;

	if (regvalh & 0x10000) {
		switch (type) {
		case ET_NOARG_DISP:
			emit(2, E_CODE, 0, regvalh >> 8, opcode);
			break;
		case ET_BYTE:
			emit(2, E_CODE8, data, regvalh >> 8, opcode);
			break;
		}
	}
	else if (regvalh & 0x8000) {
		switch (type) {
		case ET_NOARG_DISP:
			if (opcode & 0x8000)
				emit(4, E_CODE, 0, regvalh >> 8, opcode >> 8, disp, opcode);
			else
				emit(3, E_CODE, 0, regvalh >> 8, opcode, disp);
			break;
		case ET_NOARG:
			emit(2, E_CODE, 0, regvalh >> 8, opcode);
			break;
		case ET_BYTE:
			emit(3, E_CODE8, data, regvalh >> 8, opcode, disp);
			break;
		case ET_WORD:
			emit(2, E_CODE16, data, regvalh >> 8, opcode);
		}
	} else
		switch(type) {
		case ET_NOARG_DISP:
		case ET_NOARG:
			if (opcode & 0100000)
				emit(2, E_CODE, 0, opcode >> 8, opcode);
			else
				emit(1, E_CODE, 0, opcode);
			break;
		case ET_BYTE:
			emit(1, E_CODE8, data, opcode);
			break;
		case ET_WORD:
			if (opcode & 0100000)
				emit(2, E_CODE16, data, opcode >> 8, opcode);
			else
				emit(1, E_CODE16, data, opcode);
		}
}




void emitdad(int rp1,int rp2)
{
	if (rp1 & 0x8000)
		emit(2, E_CODE, 0, rp1 >> 8, rp2 + 9);
	else
		emit(1, E_CODE, 0, rp2 + 9);
}


void emitjr(int opcode, struct expr *dest)
{
	int disp = dest->e_value - dollarsign - 2;

	if (dest->e_scope & SCOPE_NORELOC)
		err[rflag]++;

	// Can't relative jump to other segments or an external
	// However, without .rel output we default to the code segment
	// for various reasons thus we let "jr 0" (and such) be acceptable
	// in those cases.
	if (((relopt && (dest->e_scope & SCOPE_SEGMASK) != segment) ||
		(dest->e_scope & SCOPE_EXTERNAL) ||
		disp > 127 || disp < -128) && z80)
	{
		if (jopt) {
			njrpromo++;
			switch (opcode) {
			case 0x10: // DJNZ
				emit(2, E_CODE16, dest, 0x05, 0xC2); // DEC B, JP NZ
				break;
			case 0x18: // JR
				emit(1, E_CODE16, dest, 0xC3); // JP
				break;
			case 0x20: // JR NZ
				emit(1, E_CODE16, dest, 0xC2); // JP NZ
				break;
			case 0x28: // JR Z
				emit(1, E_CODE16, dest, 0xCA); // JP Z
				break;
			case 0x30: // JR NC
				emit(1, E_CODE16, dest, 0xD2); // JP NC
				break;
			case 0x38: // JR C
				emit(1, E_CODE16, dest, 0xDA); // JP C
				break;
			default:
				err[vflag]++;	// shouldn't happen!
				expr_free(dest);
				break;
			}
		}
		else {
			emit(2, E_CODE, 0, opcode, -2);  // branch to itself
			err[vflag]++;
			expr_free(dest);
		}
	}
	else {
		emit(2, E_CODE, 0, opcode, disp);
		expr_free(dest);
	}
}

void checkjp(int op, struct expr *dest)
{
	op &= 0x030;
	// Only applies to Z-80 output and if JP optimization checking is on.
	// JR only has z, nz, nc, c
	// A jump to the current segment might have been optimizable
	if (z80 && JPopt && (op == 0 || op == 010 || op == 020 || op == 030) &&
		(dest->e_scope & (SCOPE_SEGMASK | SCOPE_EXTERNAL)) == segment)
	{
		int disp = dest->e_value - dollarsign - 2;
		if (disp >= -128 && disp <= 127)
			err[jflag]++;
	}
}

/*
 *  put out a byte of binary 
 */
void putbin(int v)
{
	if(!outpass) return;
	*outbinp++ = v;
	if (outbinp >= outbinm) flushbin();

	outoth[outoth_cnt++] = v;
	if (outoth_cnt == 256) flushoth();
}



/*
 *  output one line of binary in INTEL standard form
 */
void flushbin()
{
	char *p;
	int check=outbinp-outbin;

	if (!outpass)
		return;
	nbytes += check;
	if (check) {
		if (fbuf) {
			putc(':', fbuf);
			puthex(check, fbuf);
			puthex(olddollar>>8, fbuf);
			puthex(olddollar, fbuf);
			puthex(0, fbuf);
			check += (olddollar >> 8) + olddollar;
			olddollar += (outbinp-outbin);
			for (p=outbin; p<outbinp; p++) {
				puthex(*p, fbuf);
				check += *p;
			}
			puthex(256-check, fbuf);
			putc('\n', fbuf);
		}
		outbinp = outbin;
	}
}



/*
 *  put out one byte of hex
 */
void puthex(int byte, FILE *buf)
{
	putc(hexadec[(byte >> 4) & 017], buf);
	putc(hexadec[byte & 017], buf);
}

// Case-independent string comparisons.

int ci_strcompare(char *s1, char *s2, int len)
{
	int c1, c2;
	do {
		if (len == 0)
			return 0;

		c1 = *s1++;
		if (c1 >= 'A' && c1 <= 'Z') c1 += 'a' - 'A';
		c2 = *s2++;
		if (c2 >= 'A' && c2 <= 'Z') c2 += 'a' - 'A';
		if (c1 != c2)
			return c1 - c2;

		if (len > 0)
			if (--len == 0)
				return 0;

	} while (c1 && c2);

	return 0;
}

int ci_strcmp(char *s1, char *s2)
{
	return ci_strcompare(s1, s2, -1);
}

void flushoth()
{
	int i, checksum;

	if (!outpass || outoth_cnt == 0)
		return;

	if (fcmd) {
		fprintf(fcmd, "%c%c%c%c", 1, outoth_cnt + 2, oldothdollar, oldothdollar >> 8);
		fwrite(outoth, outoth_cnt, 1, fcmd);
	}

	putcas(0x3c);
	putcas(outoth_cnt);
	putcas(oldothdollar);
	putcas(oldothdollar >> 8);
	checksum = oldothdollar + (oldothdollar >> 8);
	for (i = 0; i < outoth_cnt; i++) {
		putcas(outoth[i]);
		checksum += outoth[i];
		if (fmds)
			fprintf(fmds, "b@$%04x=$%02x\n", oldothdollar + i, outoth[i] & 0xff);
	}
	putcas(checksum);

	oldothdollar += outoth_cnt;
	outoth_cnt = 0;
}

int casbit, casbitcnt = 0;

void putcas(int byte)
{
	if (flcas)
		fputc(byte, flcas);

	if (flnwcas)
		fputc(byte, flnwcas);

	if (fcas) {
		// Buffer 0 stop bit and the 8 data bits.
		casbit = (casbit << 9) | (byte & 0xff);
		casbitcnt += 9;
		while (casbitcnt >= 8) {
			casbitcnt -= 8;
			fputc(casbit >> casbitcnt, fcas);
		}
	}
}

void casname(char *out, char *src, int maxlen)
{
	char *base = basename(src);
	int i;

	out[0] = 'N';
	for (i = 1; i < maxlen; i++)
		out[i] = ' ';

	for (i = 0; *base && i < maxlen; base++) {
		if (ci_strcmp(base, ".z") == 0 || ci_strcmp(base, ".asm") == 0)
			break;

		if (*base >= 'a' && *base <= 'z') {
			*out++ = *base - ('a' - 'A');
			i++;
		}
		else if (*base >= 'A' && *base <= 'Z') {
			*out++ = *base;
			i++;
		}
	}
}

int relbit, relbitcnt = 0;

void putrelbits(int count, int bits)
{
	if (!outpass || !relopt)
		return;

	relbit <<= count;
	relbit |= bits & ((1 << count) - 1);
	relbitcnt += count;

	while (relbitcnt >= 8) {
		relbitcnt -= 8;
		fputc(relbit >> relbitcnt, frel);
	}
}

void putrel(int byte)
{
	// Add 0 bit indicating byte to load at next counter
	putrelbits(1, 0);
	// Add byte to load
	putrelbits(8, byte);
}

void putrelname(char *str)
{
	int len = strlen(str);
	int maxlen = mras ? 7 : relsymlen;

	// .rel file format can do strings 7 long but for compatibility
	// we restrict them to 6.  I believe this is important because
	// extended link functions require a character when they wish to
	// operate on an external symbol.
	if (len > maxlen)
		len = maxlen;
	putrelbits(3, len);
	while (len-- > 0) {
		int ch = *str++;
		if (ch >= 'a' && ch <= 'z')
			ch -= 'a' - 'A';
		putrelbits(8, ch);
	}
}

void putrelsegref(int scope, int addr)
{
	putrelbits(2, scope);
	putrelbits(8, addr);
	putrelbits(8, addr >> 8);
}

void putrelextaddr(int extaddr)
{
	putrelsegref(extaddr >> 16, extaddr);
}


void putrelcmd(int relcmd)
{
	putrelbits(1, 1);
	putrelbits(2, 0);
	putrelbits(4, relcmd);
}

void flushrel(void)
{
	if (relbitcnt > 0)
		putrelbits(8 - relbitcnt, 0);

	if (relopt)
		fflush(frel);
}

/*
 *  put out a line of output -- also put out binary
 */
void list(int optarg)
{

	if (!expptr)
		linecnt++;
	addtoline('\0');

	list_out(optarg, linebuf, ' ');

	lineptr = linebuf;
}

void list_out(int optarg, char *line_str, char type)
{
	unsigned char *	p;
	int	i;
	int  lst;
	char seg = ' ';

	if (outpass) {
		lst = iflist();
		if (lst) {
			lineout();
			if (nopt)
				fprintf(fout, "%4d:", linein[now_in]);

			if (copt)
			{
			    if (emitptr > emitbuf && (memflag[emit_addr] & MEM_INST))
			    {
			        int low, high, fetch;
			        get_tstates(memory + emit_addr, &low, &high, &fetch);

				// Special case to catch promotion of djnz to DEC B JP NZ
				if (memory[emit_addr] == 5 && emitptr - emitbuf == 4) {
					low += 10;
					high += 10;
				}

			    	fprintf(fout, nopt ? "%5d" : "%4d", tstatesum[emit_addr]);

				fprintf(fout, "+%d", low);
				if (low != high)
				    fprintf(fout, "+%d", high - low);
			    }
			    else
			    {
			        fprintf(fout, nopt ? "%5s-" : "%4s-", "");
			    }
			}

			if (nopt || copt)
				fprintf(fout, "\t");

			puthex(optarg >> 8, fout);
			puthex(optarg, fout);
			if (relopt)
				seg = SEGCHAR(segment);
			fputc(seg, fout);
			fputc(type, fout);
			for (p = emitbuf; (p < emitptr) && (p - emitbuf < 4); p++) {
				puthex(*p, fout);
			}
			for (i = 4 - (p-emitbuf); i > 0; i--)
				fputs("  ", fout);

			putc('\t', fout);
			fputs(line_str, fout);
		}

		if (fbds) {
			if (emitptr > emitbuf) {
				fprintf(fbds, "%04x %04x d ", dollarsign, emit_addr);
				for (p = emitbuf; p < emitptr; p++)
					fprintf(fbds, "%02x", *p & 0xff);
				fprintf(fbds, "\n");
			}
			fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, line_str);
		}

		for (p = emitbuf; p < emitptr; p++)
			putbin(*p);


		p = emitbuf+4;
		while (lst && gopt && p < emitptr) {
			lineout();
			if (nopt) putc('\t', fout);
			fputs("      ", fout);
			for (i = 0; (i < 4) && (p < emitptr);i++) {
				puthex(*p, fout);
				p++;
			}
			putc('\n', fout);
		}

		lsterr2(lst);
	} else
		lsterr1();

	dollarsign += emitptr - emitbuf;
	emit_addr += emitptr - emitbuf;
	dollarsign &= 0xffff;
	emit_addr &= 0xffff;
	emitptr = emitbuf;
}



/*
 *  keep track of line numbers and put out headers as necessary
 */
void lineout()
{
	if (!printer_output || !fout)
		return;

	if (line == 60) {
		if (popt)
			putc('\014', fout);	/* send the form feed */
		else
			fputs("\n\n\n\n\n", fout);
		line = 0;
	}
	if (line == 0) {
		fprintf(fout, "\n\n%s %s\t%s\t Page %d\n\n\n",
			&timp[4], &timp[20], title, page++);
		line = 4;
	}
	line++;
}


/*
 *  cause a page eject
 */
void eject()
{
	if (!printer_output)
		return;

	if (outpass && iflist()) {
		if (popt) {
			putc('\014', fout);	/* send the form feed */
		} else {
			while (line < 65) {
				line++;
				putc('\n', fout);
			}
		}
	}
	line = 0;
}


/*
 *  space n lines on the list file
 */
void space(int n)
{
	int	i ;
	if (outpass && iflist() && fout)
		for (i = 0; i<n; i++) {
			lineout();
			putc('\n', fout);
		}
}

/*
 *  Error handling - pass 1
 */
void lsterr1()
{
	int i;
	for (i = 0; i <= mflag; i++)
		if (err[i]) {
			if (topt)
				errorprt(i);
			passfail = 1;
			err[i] = 0;
		}
}


/*
 *  Error handling - pass 2.
 */
void lsterr2(int lst)
{
	int i;
	for (i=0; i<FLAGS; i++)
		if (err[i]) {
			if (i < FIRSTWARN)
				passfail = 1;
			if (lst) {
				char *desc = errname[i];
				char *type = i < FIRSTWARN ? " error" : " warning";
				if (errdetail[i][0]) {
					desc = errdetail[i];
					type = "";
				}
				lineout();
				if (fout)
					fprintf(fout, "%c %s%s\n",
						errlet[i], desc, type);
			}
			err[i] = 0;
			keeperr[i]++;
			if (i > mflag && topt)
				errorprt(i);
		}

	if (fout)
		fflush(fout);	/*to avoid putc(har) mix bug*/
}

/*
 *  print diagnostic to error terminal
 */
void errorprt(int errnum)
{
	char *desc = errname[errnum];
	char *type = errnum < FIRSTWARN ? " error" : " warning";
	if (errdetail[errnum][0]) {
		desc = errdetail[errnum];
		type = "";
	}
	fprintf(stderr, "%s(%d) : %s%s",
		src_name[now_in], linein[now_in], desc, type);

	errdetail[errnum][0] = '\0';

	fprintf(stderr, "\n");
	fprintf(stderr, "%s\n", linebuf);
	fflush(stderr) ;
}

void errwarn(int errnum, char *message)
{
	if (errnum == uflag && mras_undecl_ok)
		return;

	err[errnum]++;
	strcpy(errdetail[errnum], message);
}


/*
 *  list without address -- for comments and if skipped lines
 */
void list1()
{
	int lst;

	addtoline('\0');
	lineptr = linebuf;
	if (!expptr) linecnt++;
	if (outpass) {
		if ((lst = iflist())) {
			lineout();
			if (nopt)
				fprintf(fout, "%4d:\t", linein[now_in]);
			if (copt)
				fprintf(fout, "\t");
			fprintf(fout, "\t\t%s", linebuf);
			lsterr2(lst);
		}
		if (fbds)
			fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, linebuf);
	}
	else
		lsterr1();
}


/*
 *  see if listing is desired
 */
int iflist()
{
	int i, j;

	if (!fout)
		return 0;

	if (inmlex)
		return mlex_list_on;

	if (lston)
		return(1) ;
	if (*ifptr && !fopt)
		return(0);
	if (!lstoff && !expptr)
		return(1);
	j = 0;
	for (i=0; i<FLAGS; i++)
		if (err[i])
			j++;
	if (expptr)
		return(mopt || j);
	if (eopt && j)
		return(1);
	return(0);
}

void do_equ(struct item *sym, struct expr *val, int call_list);
void do_defl(struct item *sym, struct expr *val, int call_list);

// GWP - This avoids an apparent bug in bison as it tries to start out the
// Not needed under gcc which defines __STDC__ so I guard it to prevent
// warnings.
// yyparse() function with yyparse() ; { }.
#ifndef __STDC__
#define __STDC__
#endif

#define PSTITL	(0)	/* title */
#define PSRSYM	(1)	/* rsym */
#define PSWSYM	(2)	/* wsym */
#define PSINC	(3)	/* include file */
#define PSMACLIB (4)	/* maclib (similar to include) */
#define PSIMPORT (5)	/* import file */
#define PSCMN	(6)	/* common block */

#define SPTITL	(0)	/* title */
#define SPSBTL	(1)	/* sub title */
#define SPNAME	(2)	/* name */
#define SPCOM	(3)	/* comment */
#define SPPRAGMA (4)	/* pragma */


#line 1781 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STRING = 258,
    NOOPERAND = 259,
    ARITHC = 260,
    ADD = 261,
    LOGICAL = 262,
    AND = 263,
    OR = 264,
    XOR = 265,
    ANDAND = 266,
    OROR = 267,
    BIT = 268,
    CALL = 269,
    INCDEC = 270,
    DJNZ = 271,
    EX = 272,
    IM = 273,
    PHASE = 274,
    DEPHASE = 275,
    TK_IN = 276,
    JR = 277,
    LD = 278,
    TK_OUT = 279,
    PUSHPOP = 280,
    RET = 281,
    SHIFT = 282,
    RST = 283,
    REGNAME = 284,
    IXYLH = 285,
    ACC = 286,
    TK_C = 287,
    RP = 288,
    HL = 289,
    INDEX = 290,
    AF = 291,
    AFp = 292,
    SP = 293,
    MISCREG = 294,
    COND = 295,
    SPCOND = 296,
    NUMBER = 297,
    UNDECLARED = 298,
    END = 299,
    ORG = 300,
    ASSERT = 301,
    TSTATE = 302,
    T = 303,
    TILO = 304,
    TIHI = 305,
    SETOCF = 306,
    OCF = 307,
    LOW = 308,
    HIGH = 309,
    DC = 310,
    DEFB = 311,
    DEFD = 312,
    DEFS = 313,
    DEFW = 314,
    EQU = 315,
    DEFL = 316,
    LABEL = 317,
    EQUATED = 318,
    WASEQUATED = 319,
    DEFLED = 320,
    COMMON = 321,
    MULTDEF = 322,
    SHL = 323,
    SHR = 324,
    LT = 325,
    EQ = 326,
    LE = 327,
    GE = 328,
    NE = 329,
    NOT = 330,
    MROP_ADD = 331,
    MROP_SUB = 332,
    MROP_MUL = 333,
    MROP_DIV = 334,
    MROP_MOD = 335,
    MROP_AND = 336,
    MROP_OR = 337,
    MROP_XOR = 338,
    MROP_NE = 339,
    MROP_EQ = 340,
    MROP_LT = 341,
    MROP_GT = 342,
    MROP_LE = 343,
    MROP_GE = 344,
    MROP_SHIFT = 345,
    MROP_SHL = 346,
    MROP_SHR = 347,
    MROP_NOT = 348,
    MROP_LOW = 349,
    MROP_HIGH = 350,
    IF_TK = 351,
    IF_DEF_TK = 352,
    ELSE_TK = 353,
    ENDIF_TK = 354,
    ARGPSEUDO = 355,
    INCBIN = 356,
    LIST = 357,
    MINMAX = 358,
    MACRO = 359,
    MNAME = 360,
    ARG = 361,
    ENDM = 362,
    ONECHAR = 363,
    TWOCHAR = 364,
    JRPROMOTE = 365,
    JPERROR = 366,
    PUBLIC = 367,
    EXTRN = 368,
    MRAS_MOD = 369,
    SETSEG = 370,
    INSTSET = 371,
    LXI = 372,
    DAD = 373,
    STAX = 374,
    STA = 375,
    SHLD = 376,
    LDAX = 377,
    LHLD = 378,
    LDA = 379,
    MVI = 380,
    MOV = 381,
    INXDCX = 382,
    INRDCR = 383,
    PSW = 384,
    JUMP8 = 385,
    JP = 386,
    CALL8 = 387,
    ALUI8 = 388,
    SPECIAL = 389,
    RAWTOKEN = 390,
    LOCAL = 391,
    LD_XY = 392,
    ST_XY = 393,
    MV_XY = 394,
    ALU_XY = 395,
    BIT_XY = 396,
    SHIFT_XY = 397,
    INP = 398,
    OUTP = 399,
    JR_COND = 400,
    LDST16 = 401,
    ARITH16 = 402,
    REPT = 403,
    IRPC = 404,
    IRP = 405,
    EXITM = 406,
    NUL = 407,
    MPARM = 408,
    GT = 409,
    UNARY = 410
  };
#endif
/* Tokens.  */
#define STRING 258
#define NOOPERAND 259
#define ARITHC 260
#define ADD 261
#define LOGICAL 262
#define AND 263
#define OR 264
#define XOR 265
#define ANDAND 266
#define OROR 267
#define BIT 268
#define CALL 269
#define INCDEC 270
#define DJNZ 271
#define EX 272
#define IM 273
#define PHASE 274
#define DEPHASE 275
#define TK_IN 276
#define JR 277
#define LD 278
#define TK_OUT 279
#define PUSHPOP 280
#define RET 281
#define SHIFT 282
#define RST 283
#define REGNAME 284
#define IXYLH 285
#define ACC 286
#define TK_C 287
#define RP 288
#define HL 289
#define INDEX 290
#define AF 291
#define AFp 292
#define SP 293
#define MISCREG 294
#define COND 295
#define SPCOND 296
#define NUMBER 297
#define UNDECLARED 298
#define END 299
#define ORG 300
#define ASSERT 301
#define TSTATE 302
#define T 303
#define TILO 304
#define TIHI 305
#define SETOCF 306
#define OCF 307
#define LOW 308
#define HIGH 309
#define DC 310
#define DEFB 311
#define DEFD 312
#define DEFS 313
#define DEFW 314
#define EQU 315
#define DEFL 316
#define LABEL 317
#define EQUATED 318
#define WASEQUATED 319
#define DEFLED 320
#define COMMON 321
#define MULTDEF 322
#define SHL 323
#define SHR 324
#define LT 325
#define EQ 326
#define LE 327
#define GE 328
#define NE 329
#define NOT 330
#define MROP_ADD 331
#define MROP_SUB 332
#define MROP_MUL 333
#define MROP_DIV 334
#define MROP_MOD 335
#define MROP_AND 336
#define MROP_OR 337
#define MROP_XOR 338
#define MROP_NE 339
#define MROP_EQ 340
#define MROP_LT 341
#define MROP_GT 342
#define MROP_LE 343
#define MROP_GE 344
#define MROP_SHIFT 345
#define MROP_SHL 346
#define MROP_SHR 347
#define MROP_NOT 348
#define MROP_LOW 349
#define MROP_HIGH 350
#define IF_TK 351
#define IF_DEF_TK 352
#define ELSE_TK 353
#define ENDIF_TK 354
#define ARGPSEUDO 355
#define INCBIN 356
#define LIST 357
#define MINMAX 358
#define MACRO 359
#define MNAME 360
#define ARG 361
#define ENDM 362
#define ONECHAR 363
#define TWOCHAR 364
#define JRPROMOTE 365
#define JPERROR 366
#define PUBLIC 367
#define EXTRN 368
#define MRAS_MOD 369
#define SETSEG 370
#define INSTSET 371
#define LXI 372
#define DAD 373
#define STAX 374
#define STA 375
#define SHLD 376
#define LDAX 377
#define LHLD 378
#define LDA 379
#define MVI 380
#define MOV 381
#define INXDCX 382
#define INRDCR 383
#define PSW 384
#define JUMP8 385
#define JP 386
#define CALL8 387
#define ALUI8 388
#define SPECIAL 389
#define RAWTOKEN 390
#define LOCAL 391
#define LD_XY 392
#define ST_XY 393
#define MV_XY 394
#define ALU_XY 395
#define BIT_XY 396
#define SHIFT_XY 397
#define INP 398
#define OUTP 399
#define JR_COND 400
#define LDST16 401
#define ARITH16 402
#define REPT 403
#define IRPC 404
#define IRP 405
#define EXITM 406
#define NUL 407
#define MPARM 408
#define GT 409
#define UNARY 410

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 1716 "third_party/zmac/zmac.y" /* yacc.c:355  */

	struct expr *exprptr;
	struct item *itemptr;
	int ival;
	char *cval;
	

#line 2136 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */
#line 1858 "third_party/zmac/zmac.y" /* yacc.c:358  */

char  *cp;
int  i;

void do_equ(struct item *sym, struct expr *val, int call_list)
{
	expr_reloc_check(val);
	switch(sym->i_token) {
	case UNDECLARED: case WASEQUATED:
		if (sym->i_token == WASEQUATED &&
			(sym->i_value != val->e_value ||
			 ((sym->i_scope ^ val->e_scope) & SCOPE_SEGMASK)))
		{
			if (outpass) {
				if (sym->i_value != val->e_value)
					sprintf(detail, "%s error - %s went from $%04x to $%04x",
						errname[pflag], sym->i_string, sym->i_value, val->e_value);
				else
					sprintf(detail, "%s error - %s changed scope",
						errname[pflag], sym->i_string);
				errwarn(pflag, detail);
			}
			else
				passretry = 1;
		}

		sym->i_token = EQUATED;
		sym->i_pass = npass;
		sym->i_value = val->e_value;
		sym->i_scope |= val->e_scope;
		break;
	default:
		// m80 allows multiple equates as long as the value
		// does not change.  So does newer zmac.
		if (sym->i_value != val->e_value ||
			((sym->i_scope ^ val->e_scope) & SCOPE_SEGMASK))
		{
			err[mflag]++;
			sym->i_token = MULTDEF;
			sym->i_pass = npass;
		}
	}
	sym->i_scope &= ~SCOPE_BUILTIN;
	if (call_list)
		list(val->e_value);
	expr_free(val);
}

void do_defl(struct item *sym, struct expr *val, int call_list)
{
	expr_reloc_check(val);
	switch(sym->i_token) {
	case UNDECLARED: case DEFLED:
		sym->i_token = DEFLED;
		sym->i_pass = npass;
		sym->i_value = val->e_value;
		sym->i_scope = (sym->i_scope & SCOPE_SEGMASK) | val->e_scope;
		break;
	default:
		err[mflag]++;
		sym->i_token = MULTDEF;
		sym->i_pass = npass;
		break;
	}
	if (call_list)
		list(val->e_value);
	expr_free(val);
}

void do_end(struct expr *entry)
{
	if (entry) {
		expr_reloc_check(entry);
		xeq_flag++;
		xeq = entry->e_value & 0xffff;
		rel_main = ((entry->e_scope & SCOPE_SEGMASK) << 16) | xeq;
		expr_free(entry);
	}

// TODO - no reason no to treat END as a simple mras or not.
// At least, give this a try and see how it goes.
//	if (mras) {
		while (expptr)
			popsi();

		peekc = NOPEEK;
		nextline_peek = EOF;
//	}
//	else
//		peekc = 0;

}

void common_block(char *unparsed_id)
{
	struct item *it;
	char *id = unparsed_id;
	char *p;
	int unnamed;

	if (*id == '/') {
		id++;
		for (p = id; *p; p++)
			if (*p == '/')
				*p = '\0';
	}

	unnamed = 1;
	for (p = id; *p; p++)
		if (*p != ' ')
			unnamed = 0;

	id = unnamed ? " " : id;

	it = locate(id);
	switch (it->i_token) {
	case 0:
		nitems++;
	case UNDECLARED:
	case COMMON:
		it->i_value = 0;
		it->i_token = COMMON;
		it->i_pass = npass;
		it->i_scope = SCOPE_COMBLOCK;
		it->i_uses = 0;
		if (!it->i_string)
			it->i_string = strdup(id);
		break;
	default:
		err[mflag]++;
		it->i_token = MULTDEF;
		it->i_pass = npass;
			it->i_string = strdup(id);
		break;
	}

	// Even if we change to the same COMMON block the address is
	// reset back to 0.
	if (relopt) {
		segment = SEG_COMMON;
		segchange = 1;
		dollarsign = seg_pos[SEG_COMMON] = seg_size[SEG_COMMON] = 0;
		// May not be necessary but too much trouble to suppress.
		putrelcmd(RELCMD_COMMON);
		putrelname(it->i_string);
	}

	cur_common = it;
}


#line 2304 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   4743

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  363
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  660

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   410

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     171,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   168,     2,   176,   177,   167,   158,     2,
     173,   174,   165,   163,   172,   164,     2,   166,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   155,     2,
     160,   159,   161,   154,     2,     2,     2,     2,     2,     2,
     175,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   178,     2,   179,   157,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   156,     2,   169,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   162,
     170
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  2012,  2012,  2015,  2020,  2037,  2037,  2041,  2047,  2056,
    2060,  2064,  2068,  2072,  2097,  2112,  2128,  2137,  2143,  2148,
    2153,  2178,  2193,  2195,  2197,  2208,  2217,  2258,  2269,  2273,
    2273,  2331,  2334,  2404,  2411,  2418,  2422,  2426,  2437,  2446,
    2451,  2451,  2451,  2475,  2475,  2496,  2496,  2524,  2525,  2524,
    2554,  2555,  2554,  2598,  2606,  2634,  2636,  2638,  2643,  2645,
    2698,  2700,  2704,  2718,  2720,  2724,  2741,  2742,  2743,  2744,
    2745,  2746,  2747,  2748,  2749,  2750,  2751,  2752,  2754,  2755,
    2756,  2757,  2758,  2759,  2760,  2761,  2762,  2763,  2764,  2768,
    2771,  2780,  2788,  2799,  2802,  2814,  2817,  2822,  2825,  2828,
    2831,  2834,  2842,  2845,  2848,  2851,  2854,  2857,  2860,  2863,
    2866,  2869,  2872,  2875,  2878,  2881,  2884,  2887,  2890,  2893,
    2896,  2899,  2902,  2905,  2908,  2911,  2916,  2919,  2922,  2925,
    2932,  2935,  2944,  2946,  2951,  2969,  2972,  2975,  2978,  2981,
    2996,  3006,  3017,  3027,  3033,  3039,  3042,  3045,  3048,  3051,
    3054,  3057,  3060,  3063,  3066,  3097,  3102,  3107,  3113,  3116,
    3121,  3124,  3132,  3138,  3150,  3156,  3159,  3165,  3171,  3177,
    3186,  3189,  3195,  3201,  3210,  3216,  3225,  3231,  3241,  3247,
    3257,  3260,  3263,  3276,  3283,  3286,  3289,  3292,  3295,  3302,
    3309,  3312,  3315,  3327,  3338,  3352,  3362,  3393,  3405,  3415,
    3425,  3425,  3427,  3427,  3429,  3429,  3431,  3434,  3436,  3438,
    3441,  3441,  3443,  3443,  3446,  3456,  3470,  3472,  3472,  3475,
    3477,  3479,  3483,  3493,  3496,  3498,  3503,  3513,  3513,  3524,
    3526,  3529,  3531,  3534,  3540,  3542,  3545,  3548,  3553,  3558,
    3564,  3569,  3572,  3577,  3583,  3588,  3598,  3600,  3603,  3605,
    3608,  3613,  3618,  3623,  3626,  3628,  3631,  3636,  3642,  3644,
    3650,  3655,  3661,  3663,  3669,  3674,  3678,  3680,  3683,  3689,
    3696,  3706,  3708,  3713,  3723,  3725,  3730,  3740,  3742,  3749,
    3751,  3755,  3772,  3778,  3783,  3788,  3793,  3798,  3803,  3808,
    3814,  3829,  3841,  3841,  3845,  3846,  3847,  3848,  3849,  3850,
    3851,  3852,  3853,  3854,  3855,  3856,  3857,  3858,  3859,  3860,
    3861,  3862,  3863,  3864,  3865,  3866,  3867,  3868,  3870,  3871,
    3872,  3873,  3874,  3875,  3876,  3877,  3878,  3879,  3880,  3881,
    3882,  3883,  3884,  3885,  3887,  3897,  3911,  3914,  3917,  3920,
    3923,  3926,  3929,  3932,  3935,  3942,  3951,  3960,  3967,  3972,
    3977,  3982,  3989,  3991,  3993,  3995,  3997,  3999,  4001,  4006,
    4019,  4023,  4027,  4031
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STRING", "NOOPERAND", "ARITHC", "ADD",
  "LOGICAL", "AND", "OR", "XOR", "ANDAND", "OROR", "BIT", "CALL", "INCDEC",
  "DJNZ", "EX", "IM", "PHASE", "DEPHASE", "TK_IN", "JR", "LD", "TK_OUT",
  "PUSHPOP", "RET", "SHIFT", "RST", "REGNAME", "IXYLH", "ACC", "TK_C",
  "RP", "HL", "INDEX", "AF", "AFp", "SP", "MISCREG", "COND", "SPCOND",
  "NUMBER", "UNDECLARED", "END", "ORG", "ASSERT", "TSTATE", "T", "TILO",
  "TIHI", "SETOCF", "OCF", "LOW", "HIGH", "DC", "DEFB", "DEFD", "DEFS",
  "DEFW", "EQU", "DEFL", "LABEL", "EQUATED", "WASEQUATED", "DEFLED",
  "COMMON", "MULTDEF", "SHL", "SHR", "LT", "EQ", "LE", "GE", "NE", "NOT",
  "MROP_ADD", "MROP_SUB", "MROP_MUL", "MROP_DIV", "MROP_MOD", "MROP_AND",
  "MROP_OR", "MROP_XOR", "MROP_NE", "MROP_EQ", "MROP_LT", "MROP_GT",
  "MROP_LE", "MROP_GE", "MROP_SHIFT", "MROP_SHL", "MROP_SHR", "MROP_NOT",
  "MROP_LOW", "MROP_HIGH", "IF_TK", "IF_DEF_TK", "ELSE_TK", "ENDIF_TK",
  "ARGPSEUDO", "INCBIN", "LIST", "MINMAX", "MACRO", "MNAME", "ARG", "ENDM",
  "ONECHAR", "TWOCHAR", "JRPROMOTE", "JPERROR", "PUBLIC", "EXTRN",
  "MRAS_MOD", "SETSEG", "INSTSET", "LXI", "DAD", "STAX", "STA", "SHLD",
  "LDAX", "LHLD", "LDA", "MVI", "MOV", "INXDCX", "INRDCR", "PSW", "JUMP8",
  "JP", "CALL8", "ALUI8", "SPECIAL", "RAWTOKEN", "LOCAL", "LD_XY", "ST_XY",
  "MV_XY", "ALU_XY", "BIT_XY", "SHIFT_XY", "INP", "OUTP", "JR_COND",
  "LDST16", "ARITH16", "REPT", "IRPC", "IRP", "EXITM", "NUL", "MPARM",
  "'?'", "':'", "'|'", "'^'", "'&'", "'='", "'<'", "'>'", "GT", "'+'",
  "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "UNARY", "'\\n'", "','", "'('",
  "')'", "'F'", "'#'", "'$'", "'['", "']'", "$accept", "statements",
  "statement", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8",
  "$@9", "$@10", "maybecolon", "label.part", "public.list", "public.part",
  "extrn.list", "extrn.part", "varop", "operation", "$@11", "$@12", "$@13",
  "parm.list", "parm.single", "$@14", "maybeocto", "parm.element",
  "locals", "local_decls", "$@15", "local.list", "local.element",
  "arg.list", "arg.element", "$@16", "allreg", "reg", "ixylhreg", "reg8",
  "m", "realreg", "mem", "memxy", "dxy", "evenreg", "evenreg8", "pushable",
  "pushable8", "bcdesp", "bcdehlsp", "mar", "condition", "spcondition",
  "db.list", "db.list.element", "dw.list", "dw.list.element", "dd.list",
  "dd.list.element", "lxexpression", "expression", "parenexpr",
  "noparenexpr", "$@17", "symbol", "al", "arg_on", "arg_off",
  "mras_undecl_on", "mras_undecl_off", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,    63,    58,   124,    94,    38,    61,
      60,    62,   409,    43,    45,    42,    47,    37,    33,   126,
     410,    10,    44,    40,    41,    70,    35,    36,    91,    93
};
# endif

#define YYPACT_NINF -544

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-544)))

#define YYTABLE_NINF -278

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -544,   738,  -544,  -544,   -67,  -544,  -544,  -544,  -544,  -544,
    -544,  4387,  -544,  -104,   -68,  -544,   -59,  4387,  4387,   125,
     125,     0,    12,    22,  -544,  -544,   167,  2186,  -544,  -544,
    -544,  4387,  4387,  4387,  4387,  4387,  4387,  -544,  -544,  -544,
    -544,  -544,  -544,  4387,  4387,  4387,  4387,  4387,  -544,  -544,
    4387,  4387,  4387,  4387,  4387,  -544,  4387,  1372,  -544,  -544,
      69,  -544,  -544,    97,  -544,  4387,  1398,  1423,  -544,   -94,
    -544,  -544,   -63,  -544,  -544,  -544,  -544,  -544,    70,  4183,
     482,  4387,  -544,  -544,  4387,  -544,  -544,    37,  -544,  4077,
    -544,  -544,  4387,  -544,  -544,  -544,  -544,  -544,  -544,  -544,
    -544,  -544,  -544,  -544,  -544,  -544,  4387,    58,  -544,  -544,
    -544,    67,    57,  -544,  -544,  -544,   -34,    74,  -123,   845,
     845,   845,   845,   845,   845,  -544,  -544,  -544,  -544,  -544,
      99,   845,   845,   845,   845,   908,   882,  4387,  4387,  4387,
    4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,
    4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,
    4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,
    4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  4387,  -544,
    -544,  -544,    64,  2028,  -544,  -544,  -544,   125,  -544,   125,
    -544,  -544,  1536,    65,    73,    76,  1068,  1042,   135,  -544,
    2028,    80,    72,    81,  -544,  4387,  2486,  2405,  2558,  2711,
    2792,  2864,  4387,  3242,  2333,  4387,    11,  4387,  4387,  -544,
    2945,  3386,  2252,  4423,    85,    83,  3098,  4387,  4387,  4387,
    4387,  4387,  -544,  -544,  -544,  -544,    28,    28,    75,  4387,
    4387,    75,  4387,  4387,    32,    32,    28,    32,  4387,  3314,
    4387,  4387,    75,    75,  4387,  4387,  4387,  4387,    75,    75,
    4387,  4387,    17,    84,  1562,  1093,  -544,    86,    90,  4387,
    4387,  4387,  -544,    14,   101,  -544,  -544,  -544,  -544,  4534,
     610,   741,  2079,  2054,  4576,  4576,  4559,  4091,  4559,  4559,
    4091,  -544,  -544,  -544,  -544,  -544,  -544,  -544,  -544,  -544,
    -544,  -544,  -544,  -544,  -544,  -544,  -544,  -544,  1915,   610,
     741,  4534,  4091,  4559,  4559,  4559,   362,   362,   845,   845,
     845,    91,    92,  -544,    93,  -544,  -544,  -544,  -544,  -544,
    -544,  4387,  -544,  4387,  -544,   -78,    94,  -544,  -544,  -544,
    2028,  -544,  -544,    98,  -544,   102,  -544,  3885,  -544,  -544,
    -544,  -544,  -544,  -544,  -544,    96,   933,   103,  -544,  -544,
    -544,  -544,   104,   933,   105,  -544,  -544,   933,   106,  -544,
     933,   109,  -544,   933,   110,  -544,   933,  1207,  -544,  -544,
    -544,   111,  -544,  2028,  -544,  -544,  -544,  -544,  -544,  -544,
    -544,  2028,  2028,   113,   114,   218,  2028,  2028,  3453,   115,
     116,  2028,   117,  2028,   121,  3673,   124,   126,   129,  3525,
    2028,   130,  -544,  -544,  -544,  -544,  -544,  -544,  -544,  -544,
    -544,  -544,   131,  2028,  2028,  2028,  2028,  2028,   563,  4387,
    4387,  -544,  -544,   132,  -544,  -544,  2028,  2028,  -544,  2028,
    2028,   133,  -544,  -544,   134,  -544,  -544,  2028,  3962,   140,
    2028,  2028,  2028,   141,   142,  1233,  -544,  1258,  -544,  -544,
    -544,  2028,  2028,  -544,  -544,  -544,  -544,  4387,  -544,  -544,
    1587,  1700,  1726,  -544,    72,  -544,  4387,  -544,  -544,  -544,
    1751,  1864,    95,  -544,  -544,    52,  -544,  -544,  -544,  -544,
    3017,    54,   145,  4219,   236,  3017,    17,  3017,  3017,  3017,
    3017,  3098,  4387,   239,   284,   149,   150,   152,   153,  4387,
     296,   154,  2639,  3821,   139,   155,   299,    75,  -544,  -544,
     159,  -544,  2028,   160,  -544,  2028,   161,  -544,  2028,  4255,
    4387,    32,   163,  4387,  4387,  4387,  4387,  4387,  1890,  -544,
    -544,  -544,  -544,  -544,  2028,  -544,  -544,  -544,  4387,  -544,
     -78,  -544,   -78,   -78,  -544,   933,  -544,  -544,  -544,  -544,
    -544,   165,   166,  -544,   933,  -544,  -544,  -544,   933,  -544,
     933,  -544,   933,  -544,   933,  -544,   162,  2028,  -544,  -544,
     169,  -544,   303,  3592,  -544,  2028,  -544,   170,  -544,  3740,
    -544,   172,   173,  -544,  -544,  -544,  2028,   174,   175,  -544,
    -544,   176,  -544,  -544,   563,  4387,  4387,  -544,  2028,  -544,
    -544,  2028,  -544,  -544,  -544,  -544,  -544,  -544,   211,  2028,
    -544,  -544,  -544,  -544,  -544,  -544,    75,    59,   177,   178,
     318,   179,  3170,  -544,  -544,  -544,  -544,   183,   185,  -544,
    -544,  -544,  -544,  -544,  -544,  -544,  2028,   197,  -544,  -544,
    -544,    55,  -544,  -544,  -544,  -544,   197,  -544,  -544,  -544
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    54,   352,   353,   355,   356,   357,   358,
     354,     0,   360,     0,     0,   360,   362,     0,     0,     0,
       0,     0,     0,     0,    29,     3,     5,    55,    40,   283,
     290,     0,     0,     0,     0,     0,     0,   282,   285,   286,
     287,   288,   291,     0,     0,     0,     0,     0,   284,   292,
       0,     0,     0,     0,     0,   289,     0,     0,   279,   280,
       0,    16,    17,   361,    31,     0,     0,     0,   352,     0,
      60,    62,     0,    63,    65,    37,    38,    39,     0,     0,
       0,     0,   360,   359,     0,   210,   210,     0,     4,     0,
      74,    75,     0,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    88,    86,    87,     0,    56,    72,    73,
      71,    66,    67,    68,    69,    70,    59,     0,   207,   344,
     345,   346,   347,   348,   350,   341,   343,   338,   349,   351,
       0,   340,   342,   339,   337,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
     361,   361,     0,   363,    33,    34,    35,     0,    36,     0,
      30,    18,     0,     0,   284,     0,     0,     0,     0,    43,
     359,     0,   212,     0,    53,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   195,
       0,     0,     0,     0,     0,   152,    91,     0,     0,     0,
       0,     0,   200,   204,   202,   206,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,   213,     0,     0,   208,   293,   281,   336,   300,
     302,   304,   316,   317,   305,   306,   308,   310,   313,   315,
     314,   318,   319,   321,   320,   322,   323,   324,   325,   332,
     329,   328,   330,   331,   333,   334,   326,   327,     0,   301,
     303,   299,   309,   307,   311,   312,   294,   295,   297,   296,
     298,     0,     0,    27,     0,    61,    64,    19,    24,    22,
      23,     0,    20,     0,   361,   223,     0,    47,   211,    50,
      90,   237,   233,   238,   239,     0,   236,     0,   112,   229,
     230,   114,   231,   232,   241,     0,    99,   238,   260,   261,
     109,   111,     0,    97,   238,   115,   116,   101,   238,   117,
     102,   238,   118,   103,   238,   119,   104,   139,   265,   263,
     264,     0,   262,    93,   238,   256,   257,   127,   135,   246,
     247,   245,   151,     0,     0,     0,   193,   194,     0,     0,
       0,   183,     0,   148,     0,     0,     0,     0,   279,     0,
     189,   279,   250,   251,   252,   255,   254,   137,   138,   253,
     153,   124,   241,    94,   196,   197,   198,   199,     0,     0,
       0,   249,   248,     0,   132,   166,   168,   176,   162,   174,
     164,     0,   235,   234,     0,   136,   128,   144,     0,     0,
      92,   147,    95,     0,     0,     0,    96,     0,   125,   185,
     191,   150,   178,   133,   134,     6,     9,     0,    11,    12,
       0,     0,     0,    41,   212,   214,     0,    15,    26,    32,
       0,     0,     0,   226,   227,     0,   224,    45,   359,   359,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   269,   268,
     201,   266,   270,   205,   274,   276,   203,   271,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       8,    10,   216,   209,   335,    25,    21,    28,     0,    44,
       0,   216,     0,     0,   113,   100,   259,   258,   129,   240,
     243,     0,     0,   110,    98,   130,   131,   120,   105,   121,
     106,   122,   107,   123,   108,   140,   241,   146,   179,   180,
       0,   187,     0,     0,   182,   149,   170,     0,   169,     0,
     154,   279,   280,   278,   177,   171,     0,   279,   280,   167,
     175,     0,   188,   126,     0,     0,     0,   172,   160,   157,
     145,   143,   155,   156,   159,   141,    13,    42,   215,   228,
     225,    46,   361,   361,   242,   244,     0,     0,     0,     0,
       0,     0,     0,   267,   275,   272,   217,     0,     0,   142,
     181,   186,   184,   165,   161,   190,   192,   219,    48,    51,
     222,     0,   220,   216,   216,   218,     0,    49,    52,   221
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -544,  -544,  -544,  -544,  -544,  -544,  -544,  -544,  -544,  -544,
    -544,  -544,  -544,  -544,  -544,  -544,   171,  -544,   168,  -544,
    -544,  -544,  -544,  -544,  -544,   273,  -544,  -544,  -190,  -543,
    -544,  -544,  -544,  -296,  -544,  -374,  -544,  -109,  -222,  -544,
    -234,   -26,  -162,  -544,  -221,  -226,  -220,  -166,  -544,  -544,
    -245,  -544,  -206,  -211,   143,  -544,  -243,  -544,  -244,  -544,
    -242,  -164,   -11,  -213,  -464,  -544,    26,  -197,     4,  -165,
    -544,  -544
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    25,    89,    78,   118,   542,   335,   551,   488,
     653,   489,   654,   116,    26,    69,    70,    72,    73,   117,
     263,   428,   430,   429,   273,   201,   202,   274,   275,   617,
     618,   647,   651,   652,   485,   486,   548,   348,   349,   350,
     441,   442,   352,   353,   354,   355,   388,   433,   417,   418,
     389,   558,   390,   381,   382,   520,   521,   526,   527,   523,
     524,   595,   391,    58,    59,   130,    71,   199,    60,   182,
      65,   324
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      57,   362,   407,   336,   421,   422,    66,    67,   621,   408,
     411,   444,   338,   446,   420,   321,   322,   463,   419,    63,
     119,   120,   121,   122,   123,   124,   269,    27,   483,   456,
    -212,   458,   125,   126,   127,   128,   129,    28,   449,   131,
     132,   133,   134,   135,   393,   136,    74,   394,   592,   598,
     385,   358,   359,   272,   183,   386,   464,   341,   400,   384,
     344,   341,   416,   384,   344,   598,   431,    61,   192,   196,
     197,   434,   346,   200,   432,   432,   435,   186,   187,   438,
     445,   264,   443,   443,   432,   443,   198,   385,   556,   484,
     453,   454,   386,   358,   359,   265,   459,   460,   360,   365,
     369,   372,   375,    62,   341,   387,   384,   344,   188,   189,
     657,   658,    64,   406,   341,   378,   384,   344,   412,   358,
     359,   413,   414,   379,   380,   270,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,    68,   482,
     599,    75,   385,   358,   359,   180,   620,   386,   622,   623,
     351,   361,   366,    76,   395,   473,   474,     5,     6,     7,
       8,     9,    10,    77,   340,   356,   363,   367,   370,   373,
     376,   377,   383,   181,   392,   190,   396,   397,   204,   401,
     403,    79,   410,   266,   415,    74,   423,   424,   425,   426,
     427,   268,    80,   549,   550,    81,   655,   656,   436,   437,
     267,   439,   440,   271,   276,   323,   328,   447,   450,   451,
     452,   334,   532,   455,   329,   457,   557,   330,   272,   461,
     462,   565,   337,   339,   475,   465,   505,   468,   470,   471,
     472,   469,   477,   478,   479,   487,   547,   561,    82,   494,
     490,   562,    83,   578,   491,   495,   496,   497,   498,   575,
     576,   499,   500,   502,   543,   503,   504,   507,   508,   509,
     566,   552,   553,   510,   600,   584,   512,   609,   513,   591,
     597,   514,   516,   517,   529,   530,   531,   594,   612,   613,
     614,   615,   533,   534,   535,    84,    85,    86,    87,   559,
     480,   579,   481,   580,   581,   582,   583,   586,   587,   601,
     602,   604,   605,   606,   626,   628,   135,   610,    88,   624,
     625,   627,   630,  -163,  -158,  -173,  -277,   636,   632,   643,
     650,   641,   642,   644,   648,   603,   649,   326,   325,   203,
     659,   633,   635,   634,   402,   607,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   554,     0,     0,     0,     0,   563,   135,   567,   569,
     571,   573,     0,     0,   135,     0,     0,     0,   135,     0,
       0,     0,     0,   590,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   522,   525,   528,
       0,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   135,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,   538,   637,   638,     0,
       0,     0,     0,     0,   639,   544,     0,     0,     0,     0,
     645,     0,     0,     0,     0,     0,     0,     0,     0,   555,
       0,     0,     0,     0,   564,   193,   568,   570,   572,   574,
       0,   577,     0,     0,     0,     0,     0,     0,   585,     0,
       0,     0,   596,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   596,   608,
       0,     0,   611,     0,    29,    30,     0,   176,   177,   178,
      31,    32,    33,     0,    34,    35,    36,   619,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    44,
       0,     0,     0,     0,     0,     0,   518,     0,     0,     0,
       0,     0,   135,     0,     0,    45,    46,    47,   135,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     194,   195,     0,   522,   525,   528,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,     0,     0,     0,
       0,    31,    32,    33,     0,    34,    35,    36,   137,     0,
     139,   646,     0,     0,     0,    37,    38,    39,    40,    41,
      42,     0,     0,     0,    49,     0,     0,     0,     0,    43,
      44,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,    53,     0,     0,     0,    54,    45,    46,    47,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,   519,     0,     0,     0,     0,     0,   142,   143,
     144,   145,   146,   147,   148,     0,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,    52,    53,     0,     0,     0,    54,     0,     2,     3,
      55,    56,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   137,
       0,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,     0,     0,
       0,     4,   -58,   -58,   -58,   -58,     0,     0,     0,   -58,
       0,     0,     0,   -58,   -58,   -58,   -58,   -58,     0,     0,
       5,     6,     7,     8,     9,    10,     0,     0,     0,   142,
     143,   144,   145,   146,   147,   148,     0,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,    11,    12,    13,    14,    15,   -58,
      16,     0,     0,   -58,     0,   -58,     0,     0,    17,    18,
      19,    20,    21,    22,    23,   -58,   -58,   -58,   -58,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,     0,   -58,   -58,
     -58,   -58,    24,     0,     0,   -58,   -58,   -58,   -58,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,
     137,   138,   139,   140,   141,     0,     0,     0,     0,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   -58,
       0,     0,     0,     0,     0,     0,   137,   138,   139,   140,
     141,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,     0,
       0,   137,   138,   139,   140,   141,     0,     0,     0,     0,
     142,   143,   144,   145,   146,   147,   148,     0,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,   142,   143,   144,   145,
     146,   147,   148,     0,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   142,   143,   144,   145,   146,   147,   148,     0,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     137,   138,   139,   140,   141,     0,     0,     0,     0,     0,
       0,   278,   166,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   137,   138,   139,   140,
     141,     0,   277,     0,     0,     0,     0,   166,     0,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   137,   138,   139,   140,   141,  -245,     0,     0,     0,
     142,   143,   144,   145,   146,   147,   148,     0,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,   142,   143,   144,   145,
     146,   147,   148,     0,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   142,   143,   144,   145,   146,   147,   148,     0,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,     0,     0,   332,   333,   137,   138,   139,   140,   141,
       0,     0,   166,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,     0,     0,     0,
     331,   137,   138,   139,   140,   141,     0,   166,     0,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,     0,     0,     0,     0,   467,   137,   138,   139,   140,
     141,     0,     0,     0,     0,   142,   143,   144,   145,   146,
     147,   148,     0,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
       0,   142,   143,   144,   145,   146,   147,   148,     0,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   142,   143,   144,   145,
     146,   147,   148,     0,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   166,     0,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,     0,     0,     0,   501,
     137,   138,   139,   140,   141,     0,     0,   166,     0,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,     0,     0,     0,     0,   536,   137,   138,   139,   140,
     141,     0,   166,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,     0,     0,     0,
     537,   137,   138,   139,   140,   141,     0,     0,     0,     0,
     142,   143,   144,   145,   146,   147,   148,     0,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,     0,   142,   143,   144,   145,
     146,   147,   148,     0,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   142,   143,   144,   145,   146,   147,   148,     0,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,     0,     0,   179,   137,   138,   139,   140,   141,     0,
       0,     0,   166,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,     0,     0,   184,
     137,   138,   139,   140,   141,     0,     0,   166,     0,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,     0,     0,     0,   185,   137,   138,   139,   140,   141,
       0,     0,     0,     0,   142,   143,   144,   145,   146,   147,
     148,     0,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,     0,
     142,   143,   144,   145,   146,   147,   148,     0,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   142,   143,   144,   145,   146,
     147,   148,     0,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     166,     0,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,     0,     0,     0,   327,   137,   138,
     139,   140,   141,     0,     0,     0,   166,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
       0,     0,     0,   466,   137,   138,   139,   140,   141,     0,
       0,   166,     0,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,     0,     0,     0,   539,   137,
     138,   139,   140,   141,     0,     0,     0,     0,   142,   143,
     144,   145,   146,   147,   148,     0,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,     0,   142,   143,   144,   145,   146,   147,
     148,     0,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   142,
     143,   144,   145,   146,   147,   148,     0,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   166,     0,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,     0,     0,
       0,   540,   137,   138,   139,   140,   141,     0,     0,     0,
     166,     0,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,     0,     0,     0,   541,   137,   138,
     139,   140,   141,     0,     0,   166,     0,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,     0,
       0,     0,   545,   137,   138,   139,   140,   141,     0,     0,
       0,     0,   142,   143,   144,   145,   146,   147,   148,     0,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,     0,   142,   143,
     144,   145,   146,   147,   148,     0,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   142,   143,   144,   145,   146,   147,   148,
       0,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,     0,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,     0,     0,     0,   546,   137,   138,   139,   140,
     141,     0,     0,     0,   166,     0,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,     0,     0,
       0,   616,   137,   138,   139,   140,     0,     0,     0,   166,
     476,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,     0,     0,     0,     0,   137,   138,   139,
       0,     0,     0,     0,     0,     0,   142,   143,   144,   145,
     146,   147,   148,     0,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,     0,   142,   143,   144,   145,   146,   147,   148,     0,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   142,   143,   144,
     145,   146,   147,   148,     0,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,     0,    90,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,    92,     0,     0,
       0,     0,     0,     0,    93,    94,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,   100,   101,   102,
       0,     0,     0,     0,     0,     0,   103,   104,   105,     0,
       0,   341,   342,   384,   344,   385,   358,   359,     0,   106,
     386,   404,     0,     0,    29,    30,     0,     0,     0,     0,
      31,    32,    33,     0,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   107,   108,   109,   110,    45,    46,    47,     0,   111,
     112,   113,   114,   115,     0,     0,     0,     0,     0,     0,
      48,     0,   341,   342,   384,   344,   385,   358,   359,     0,
       0,   386,     0,     0,     0,    29,    30,     0,     0,     0,
       0,    31,    32,    33,     0,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,     0,     0,     0,    49,     0,     0,     0,     0,    43,
      44,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,    53,     0,     0,     0,   405,    45,    46,    47,    55,
      56,     0,     0,     0,   341,   342,   357,   344,     0,   358,
     359,    48,     0,     0,     0,   346,     0,    29,    30,     0,
       0,     0,     0,    31,    32,    33,     0,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    45,    46,
      47,    52,    53,     0,     0,     0,   347,     0,     0,     0,
      55,    56,     0,    48,     0,   341,   342,   343,   344,     0,
     345,     0,     0,     0,     0,     0,   346,     0,    29,    30,
       0,     0,     0,     0,    31,    32,    33,     0,    34,    35,
      36,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,     0,     0,     0,    49,     0,     0,
       0,     0,    43,    44,     0,     0,     0,     0,    50,    51,
       0,     0,     0,    52,    53,     0,     0,     0,   347,    45,
      46,    47,    55,    56,     0,     0,     0,   341,   342,   364,
     344,     0,     0,     0,    48,     0,     0,     0,   346,     0,
      29,    30,     0,     0,     0,     0,    31,    32,    33,     0,
      34,    35,    36,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,     0,     0,     0,     0,
       0,     0,     0,     0,    43,    44,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    50,
      51,    45,    46,    47,    52,    53,     0,     0,     0,   347,
       0,     0,     0,    55,    56,     0,    48,     0,   341,   342,
     384,   344,     0,     0,     0,     0,     0,     0,   588,     0,
       0,    29,    30,     0,     0,     0,     0,    31,    32,    33,
       0,    34,    35,    36,     0,     0,     0,     0,     0,     0,
       0,    37,    38,    39,    40,    41,    42,     0,     0,     0,
      49,     0,     0,     0,     0,    43,    44,     0,     0,     0,
       0,    50,    51,     0,     0,     0,    52,    53,     0,     0,
       0,   347,    45,    46,    47,    55,    56,     0,     0,     0,
     341,   342,   368,   344,     0,     0,     0,    48,     0,     0,
       0,     0,     0,    29,    30,     0,     0,     0,     0,    31,
      32,    33,     0,    34,    35,    36,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,    45,    46,    47,    52,    53,     0,
       0,     0,   589,     0,     0,     0,    55,    56,     0,    48,
       0,   341,   342,   371,   344,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,     0,     0,     0,     0,
      31,    32,    33,     0,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,    49,     0,     0,     0,     0,    43,    44,
       0,     0,     0,     0,    50,    51,     0,     0,     0,    52,
      53,     0,     0,     0,   347,    45,    46,    47,    55,    56,
       0,     0,     0,   341,   342,   374,   344,     0,     0,     0,
      48,     0,     0,     0,     0,     0,    29,    30,     0,     0,
       0,     0,    31,    32,    33,     0,    34,    35,    36,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    44,     0,     0,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    45,    46,    47,
      52,    53,     0,     0,     0,   347,     0,     0,     0,    55,
      56,     0,    48,     0,   341,     0,   384,   344,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,     0,
       0,     0,     0,    31,    32,    33,     0,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,     0,     0,     0,    49,     0,     0,     0,
       0,    43,    44,     0,     0,     0,     0,    50,    51,     0,
       0,     0,    52,    53,     0,     0,     0,   347,    45,    46,
      47,    55,    56,     0,     0,     0,   341,   342,   384,   344,
       0,     0,     0,    48,     0,     0,     0,     0,     0,    29,
      30,     0,     0,     0,     0,    31,    32,    33,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,     0,     0,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
      45,    46,    47,    52,    53,     0,     0,     0,   398,     0,
     399,     0,    55,    56,     0,    48,     0,   341,     0,   384,
     344,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      29,    30,     0,     0,     0,     0,    31,    32,    33,     0,
      34,    35,    36,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    40,    41,    42,     0,     0,     0,    49,
       0,     0,     0,     0,    43,    44,     0,     0,     0,     0,
      50,    51,     0,     0,     0,    52,    53,     0,     0,     0,
     347,    45,    46,    47,    55,    56,     0,     0,     0,   341,
       0,   384,   344,     0,     0,     0,    48,     0,     0,     0,
       0,     0,    29,    30,     0,     0,     0,     0,    31,    32,
      33,     0,    34,    35,    36,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,     0,     0,
       0,     0,     0,     0,     0,     0,    43,    44,     0,     0,
      49,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    51,    45,    46,    47,    52,    53,     0,     0,
       0,   347,     0,     0,   378,    55,    56,     0,    48,     0,
       0,     0,   379,   380,    29,    30,     0,     0,     0,     0,
      31,    32,    33,     0,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    44,
       0,     0,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,    51,    45,    46,    47,    52,    53,
       0,     0,     0,    54,     0,     0,   378,    55,    56,     0,
      48,     0,     0,     0,   379,   380,    29,    30,     0,     0,
       0,     0,    31,    32,    33,     0,    34,    35,    36,     0,
       0,     0,     0,     0,     0,     0,    37,    38,    39,    40,
      41,    42,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    44,     0,     0,    49,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    50,    51,    45,    46,    47,
      52,    53,     0,     0,     0,    54,     0,     0,   378,    55,
      56,     0,    48,     0,     0,     0,     0,   380,    29,    30,
       0,     0,     0,     0,    31,    32,    33,     0,    34,    35,
      36,     0,     0,     0,     0,     0,     0,     0,    37,    38,
      39,    40,    41,    42,     0,     0,     0,     0,     0,     0,
       0,     0,    43,    44,     0,     0,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,    45,
      46,    47,    52,    53,     0,   506,     0,   448,     0,     0,
       0,    55,    56,     0,    48,    29,    30,     0,     0,     0,
       0,    31,    32,    33,     0,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,     0,     0,     0,     0,     0,     0,     0,    49,     0,
       0,     0,     0,     0,     0,     0,    45,    46,    47,    50,
      51,     0,     0,     0,    52,    53,     0,   515,     0,    54,
       0,    48,     0,    55,    56,     0,     0,    29,    30,     0,
       0,     0,     0,    31,    32,    33,     0,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    45,    46,
      47,    52,    53,     0,   629,     0,    54,     0,     0,     0,
      55,    56,     0,    48,    29,    30,     0,     0,     0,     0,
      31,    32,    33,     0,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    44,
       0,     0,     0,     0,     0,     0,     0,    49,     0,     0,
       0,     0,     0,     0,     0,    45,    46,    47,    50,    51,
       0,     0,     0,    52,    53,     0,     0,     0,    54,     0,
      48,     0,    55,    56,     0,     0,   511,   492,   493,     0,
       0,     0,     0,     0,     0,    29,    30,     0,     0,     0,
       0,    31,    32,    33,     0,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,     0,     0,     0,    49,     0,     0,     0,     0,    43,
      44,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,    53,     0,     0,     0,    54,    45,    46,    47,    55,
      56,     0,     0,   631,   492,   493,     0,     0,     0,     0,
       0,    48,    29,    30,     0,     0,     0,     0,    31,    32,
      33,     0,    34,    35,    36,     0,     0,     0,     0,     0,
       0,     0,    37,    38,    39,    40,    41,    42,     0,     0,
       0,     0,     0,     0,     0,     0,    43,    44,     0,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,    50,    51,     0,     0,
       0,    52,    53,     0,     0,     0,    54,     0,    48,     0,
      55,    56,     0,     0,     0,   358,   359,     0,     0,     0,
       0,     0,     0,    29,    30,     0,     0,     0,     0,    31,
      32,    33,     0,    34,    35,    36,     0,     0,     0,     0,
       0,     0,     0,    37,    38,    39,    40,    41,    42,     0,
       0,     0,    49,     0,     0,     0,     0,    43,    44,     0,
       0,     0,     0,    50,    51,     0,     0,     0,    52,    53,
       0,     0,     0,    54,    45,    46,    47,    55,    56,   492,
     493,     0,     0,     0,     0,     0,     0,    29,    30,    48,
     593,     0,     0,    31,    32,    33,     0,    34,    35,    36,
       0,     0,     0,     0,     0,     0,     0,    37,    38,    39,
      40,    41,    42,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    49,     0,     0,     0,     0,    45,    46,
      47,     0,     0,     0,    50,    51,     0,     0,     0,    52,
      53,     0,     0,    48,    54,     0,   358,   359,    55,    56,
       0,     0,     0,     0,    29,    30,     0,     0,     0,     0,
      31,    32,    33,     0,    34,    35,    36,     0,     0,     0,
       0,     0,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,     0,     0,     0,     0,    49,    43,    44,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,    52,    53,    45,    46,    47,    54,     0,
       0,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   205,   206,   207,   208,   209,   210,   211,     0,     0,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,     0,     0,     0,     0,
       0,     0,     0,     0,    49,     0,     0,     0,     0,     0,
       0,     0,   228,   229,   230,    50,    51,     0,   231,     0,
      52,    53,     0,   232,   233,    54,   234,     0,     0,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   142,
     143,   144,     0,   146,   147,     0,     0,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,     0,   248,   249,   250,
     251,     0,     0,     0,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,    29,    30,     0,     0,     0,
       0,    31,    32,    33,     0,    34,    35,    36,     0,     0,
       0,     0,     0,     0,     0,    37,    38,    39,    40,    41,
      42,   171,   172,   173,   174,   175,   176,   177,   178,    43,
      44,    29,    30,     0,     0,     0,     0,    31,    32,    33,
       0,    34,    35,    36,     0,     0,    45,    46,    47,     0,
       0,    37,    38,    39,    40,    41,    42,     0,     0,     0,
       0,    48,     0,     0,     0,    43,    44,    29,    30,     0,
       0,     0,     0,    31,    32,    33,     0,    34,    35,    36,
       0,     0,    45,    46,    47,     0,     0,    37,    38,    39,
      40,    41,    42,     0,     0,     0,     0,    48,     0,     0,
       0,    43,    44,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,    45,    46,
      47,    52,    53,     0,   191,     0,    54,     0,     0,     0,
      55,    56,     0,    48,   593,     0,     0,     0,     0,     0,
       0,    49,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,    52,    53,     0,
       0,     0,    54,   560,     0,     0,    55,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    49,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,    52,    53,     0,     0,     0,    54,    29,
      30,     0,    55,    56,     0,    31,    32,    33,     0,    34,
      35,    36,     0,     0,     0,     0,     0,     0,     0,    37,
      38,    39,    40,    41,    42,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,    29,    30,     0,     0,     0,
       0,    31,    32,    33,     0,    34,    35,    36,     0,     0,
      45,    46,    47,     0,     0,    37,    38,    39,    40,    41,
      42,     0,     0,     0,     0,    48,     0,     0,     0,    43,
      44,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    45,    46,    47,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,     0,     0,     0,     0,     0,    49,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,    52,    53,     0,     0,     0,
      54,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,    52,    53,     0,     0,     0,   409,     0,     0,     0,
      55,    56,   142,   143,   144,   145,   146,   147,   148,     0,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   142,   143,     0,
       0,     0,     0,     0,     0,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   170,   171,   172,   173,   174,   175,   176,
     177,   178,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,   177,   178,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,   177,   178
};

static const yytype_int16 yycheck[] =
{
      11,   207,   222,   200,   226,   226,    17,    18,   551,   222,
     223,   245,   202,   247,   225,   180,   181,   262,   224,    15,
      31,    32,    33,    34,    35,    36,    60,     1,   106,   255,
     153,   257,    43,    44,    45,    46,    47,   104,   249,    50,
      51,    52,    53,    54,    33,    56,    20,    36,   512,   513,
      33,    34,    35,   176,    65,    38,   262,    29,   220,    31,
      32,    29,   224,    31,    32,   529,    38,   171,    79,    80,
      81,   237,    40,    84,   236,   237,   238,   171,   172,   241,
     246,    92,   244,   245,   246,   247,    82,    33,    34,   167,
     252,   253,    38,    34,    35,   106,   258,   259,   207,   208,
     209,   210,   211,   171,    29,   214,    31,    32,   171,   172,
     653,   654,   171,   222,    29,    32,    31,    32,    33,    34,
      35,    36,    37,    40,    41,   159,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,    43,   334,
      31,   171,    33,    34,    35,   106,   550,    38,   552,   553,
     206,   207,   208,   171,   173,   171,   172,    62,    63,    64,
      65,    66,    67,   171,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   106,   215,   135,   217,   218,   171,   220,
     221,    44,   223,   155,   129,   189,   227,   228,   229,   230,
     231,   164,    55,   171,   172,    58,   171,   172,   239,   240,
     163,   242,   243,   159,   135,   171,   171,   248,   249,   250,
     251,   106,   448,   254,   171,   256,   491,   171,   176,   260,
     261,   496,   172,   172,   153,   171,    38,   171,   269,   270,
     271,   171,   171,   171,   171,   171,   171,   493,   101,   173,
     172,    35,   105,    34,   172,   172,   172,   172,   172,   501,
     501,   172,   172,   172,   474,   172,   172,   172,   172,   172,
     496,   488,   489,   172,   514,   508,   172,   531,   172,   512,
     513,   172,   172,   172,   172,   172,   172,   513,   534,   535,
     536,   537,   172,   172,   172,   148,   149,   150,   151,   174,
     331,    37,   333,   174,   174,   173,   173,    31,   174,   174,
      31,   172,   172,   172,   172,    32,   347,   174,   171,   174,
     174,   172,   172,   171,   171,   171,   171,   136,   172,    31,
     153,   174,   174,   174,   171,   517,   171,   189,   187,    86,
     656,   604,   606,   605,   221,   529,    -1,    -1,    -1,   531,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   490,    -1,    -1,    -1,    -1,   495,   398,   497,   498,
     499,   500,    -1,    -1,   405,    -1,    -1,    -1,   409,    -1,
      -1,    -1,    -1,   512,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   428,   429,   430,
      -1,   627,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   448,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,   467,   622,   623,    -1,
      -1,    -1,    -1,    -1,   626,   476,    -1,    -1,    -1,    -1,
     632,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   490,
      -1,    -1,    -1,    -1,   495,     3,   497,   498,   499,   500,
      -1,   502,    -1,    -1,    -1,    -1,    -1,    -1,   509,    -1,
      -1,    -1,   513,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   529,   530,
      -1,    -1,   533,    -1,    42,    43,    -1,   165,   166,   167,
      48,    49,    50,    -1,    52,    53,    54,   548,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,   583,    -1,    -1,    93,    94,    95,   589,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     108,   109,    -1,   604,   605,   606,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    52,    53,    54,     8,    -1,
      10,   632,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    76,
      77,    -1,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,
     168,   169,    -1,    -1,    -1,   173,    93,    94,    95,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,   109,    -1,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,    -1,    -1,
      -1,   168,   169,    -1,    -1,    -1,   173,    -1,     0,     1,
     177,   178,     4,     5,     6,     7,     8,     9,    10,     8,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,    43,    44,    45,    46,    47,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    -1,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    96,    97,    98,    99,   100,   101,
     102,    -1,    -1,   105,    -1,   107,    -1,    -1,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,    -1,   130,   131,
     132,   133,   134,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   171,
      -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,    11,
      12,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
       8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
      -1,   179,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,     8,     9,    10,    11,
      12,    -1,   174,    -1,    -1,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,     8,     9,    10,    11,    12,   173,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,   171,   172,     8,     9,    10,    11,    12,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
     172,     8,     9,    10,    11,    12,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,   172,     8,     9,    10,    11,
      12,    -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,   172,
       8,     9,    10,    11,    12,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,    -1,   172,     8,     9,    10,    11,
      12,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,    -1,
     172,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    -1,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    68,    69,    70,    71,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,   171,     8,     9,    10,    11,    12,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    -1,    -1,   171,
       8,     9,    10,    11,    12,    -1,    -1,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,    -1,    -1,   171,     8,     9,    10,    11,    12,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      68,    69,    70,    71,    72,    73,    74,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    68,    69,    70,    71,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,   171,     8,     9,
      10,    11,    12,    -1,    -1,    -1,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,   171,     8,     9,    10,    11,    12,    -1,
      -1,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,    -1,   171,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    -1,    68,    69,    70,    71,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    68,
      69,    70,    71,    72,    73,    74,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,   171,     8,     9,    10,    11,    12,    -1,    -1,    -1,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,    -1,    -1,    -1,   171,     8,     9,
      10,    11,    12,    -1,    -1,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
      -1,    -1,   171,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    -1,    68,    69,
      70,    71,    72,    73,    74,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    68,    69,    70,    71,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,   171,     8,     9,    10,    11,
      12,    -1,    -1,    -1,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
      -1,   171,     8,     9,    10,    11,    -1,    -1,    -1,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,    -1,    -1,    -1,     8,     9,    10,
      -1,    -1,    -1,    -1,    -1,    -1,    68,    69,    70,    71,
      72,    73,    74,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    -1,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    68,    69,    70,
      71,    72,    73,    74,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    61,    -1,    -1,
      -1,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    91,    92,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,   103,
      38,    39,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   155,   156,   157,   158,    93,    94,    95,    -1,   163,
     164,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    38,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    76,
      77,    -1,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,
     168,   169,    -1,    -1,    -1,   173,    93,    94,    95,   177,
     178,    -1,    -1,    -1,    29,    30,    31,    32,    -1,    34,
      35,   108,    -1,    -1,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,    93,    94,
      95,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
     177,   178,    -1,   108,    -1,    29,    30,    31,    32,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    42,    43,
      -1,    -1,    -1,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    76,    77,    -1,    -1,    -1,    -1,   163,   164,
      -1,    -1,    -1,   168,   169,    -1,    -1,    -1,   173,    93,
      94,    95,   177,   178,    -1,    -1,    -1,    29,    30,    31,
      32,    -1,    -1,    -1,   108,    -1,    -1,    -1,    40,    -1,
      42,    43,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    77,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
     164,    93,    94,    95,   168,   169,    -1,    -1,    -1,   173,
      -1,    -1,    -1,   177,   178,    -1,   108,    -1,    29,    30,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      -1,    42,    43,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    -1,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    76,    77,    -1,    -1,    -1,
      -1,   163,   164,    -1,    -1,    -1,   168,   169,    -1,    -1,
      -1,   173,    93,    94,    95,   177,   178,    -1,    -1,    -1,
      29,    30,    31,    32,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,    93,    94,    95,   168,   169,    -1,
      -1,    -1,   173,    -1,    -1,    -1,   177,   178,    -1,   108,
      -1,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,   168,
     169,    -1,    -1,    -1,   173,    93,    94,    95,   177,   178,
      -1,    -1,    -1,    29,    30,    31,    32,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    77,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,   164,    93,    94,    95,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,   177,
     178,    -1,   108,    -1,    29,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,    -1,    -1,   163,   164,    -1,
      -1,    -1,   168,   169,    -1,    -1,    -1,   173,    93,    94,
      95,   177,   178,    -1,    -1,    -1,    29,    30,    31,    32,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    42,
      43,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
      93,    94,    95,   168,   169,    -1,    -1,    -1,   173,    -1,
     175,    -1,   177,   178,    -1,   108,    -1,    29,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    43,    -1,    -1,    -1,    -1,    48,    49,    50,    -1,
      52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    63,    64,    65,    66,    67,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    76,    77,    -1,    -1,    -1,    -1,
     163,   164,    -1,    -1,    -1,   168,   169,    -1,    -1,    -1,
     173,    93,    94,    95,   177,   178,    -1,    -1,    -1,    29,
      -1,    31,    32,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,    -1,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,   164,    93,    94,    95,   168,   169,    -1,    -1,
      -1,   173,    -1,    -1,    32,   177,   178,    -1,   108,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   163,   164,    93,    94,    95,   168,   169,
      -1,    -1,    -1,   173,    -1,    -1,    32,   177,   178,    -1,
     108,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    48,    49,    50,    -1,    52,    53,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    77,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,   164,    93,    94,    95,
     168,   169,    -1,    -1,    -1,   173,    -1,    -1,    32,   177,
     178,    -1,   108,    -1,    -1,    -1,    -1,    41,    42,    43,
      -1,    -1,    -1,    -1,    48,    49,    50,    -1,    52,    53,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,
      64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    77,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,    93,
      94,    95,   168,   169,    -1,    32,    -1,   173,    -1,    -1,
      -1,   177,   178,    -1,   108,    42,    43,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,   163,
     164,    -1,    -1,    -1,   168,   169,    -1,    32,    -1,   173,
      -1,   108,    -1,   177,   178,    -1,    -1,    42,    43,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,    93,    94,
      95,   168,   169,    -1,    32,    -1,   173,    -1,    -1,    -1,
     177,   178,    -1,   108,    42,    43,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,   163,   164,
      -1,    -1,    -1,   168,   169,    -1,    -1,    -1,   173,    -1,
     108,    -1,   177,   178,    -1,    -1,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    76,
      77,    -1,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,
     168,   169,    -1,    -1,    -1,   173,    93,    94,    95,   177,
     178,    -1,    -1,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,   108,    42,    43,    -1,    -1,    -1,    -1,    48,    49,
      50,    -1,    52,    53,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    63,    64,    65,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    93,    94,    95,   163,   164,    -1,    -1,
      -1,   168,   169,    -1,    -1,    -1,   173,    -1,   108,    -1,
     177,   178,    -1,    -1,    -1,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    48,
      49,    50,    -1,    52,    53,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    64,    65,    66,    67,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    76,    77,    -1,
      -1,    -1,    -1,   163,   164,    -1,    -1,    -1,   168,   169,
      -1,    -1,    -1,   173,    93,    94,    95,   177,   178,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    42,    43,   108,
     109,    -1,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    93,    94,
      95,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,   168,
     169,    -1,    -1,   108,   173,    -1,    34,    35,   177,   178,
      -1,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    -1,
      48,    49,    50,    -1,    52,    53,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
      -1,    -1,    -1,   168,   169,    93,    94,    95,   173,    -1,
      -1,    -1,   177,   178,    -1,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    47,   163,   164,    -1,    51,    -1,
     168,   169,    -1,    56,    57,   173,    59,    -1,    -1,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,
      69,    70,    -1,    72,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,    -1,   130,   131,   132,
     133,    -1,    -1,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,    42,    43,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,
      67,   160,   161,   162,   163,   164,   165,   166,   167,    76,
      77,    42,    43,    -1,    -1,    -1,    -1,    48,    49,    50,
      -1,    52,    53,    54,    -1,    -1,    93,    94,    95,    -1,
      -1,    62,    63,    64,    65,    66,    67,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    76,    77,    42,    43,    -1,
      -1,    -1,    -1,    48,    49,    50,    -1,    52,    53,    54,
      -1,    -1,    93,    94,    95,    -1,    -1,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,   108,    -1,    -1,
      -1,    76,    77,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,    93,    94,
      95,   168,   169,    -1,   171,    -1,   173,    -1,    -1,    -1,
     177,   178,    -1,   108,   109,    -1,    -1,    -1,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,    -1,    -1,    -1,   168,   169,    -1,
      -1,    -1,   173,   174,    -1,    -1,   177,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,   164,
      -1,    -1,    -1,   168,   169,    -1,    -1,    -1,   173,    42,
      43,    -1,   177,   178,    -1,    48,    49,    50,    -1,    52,
      53,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    77,    42,    43,    -1,    -1,    -1,
      -1,    48,    49,    50,    -1,    52,    53,    54,    -1,    -1,
      93,    94,    95,    -1,    -1,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    76,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,   164,    -1,    -1,    -1,   168,   169,    -1,    -1,    -1,
     173,    -1,    -1,    -1,   177,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   152,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   163,   164,    -1,    -1,
      -1,   168,   169,    -1,    -1,    -1,   173,    -1,    -1,    -1,
     177,   178,    68,    69,    70,    71,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   163,   164,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
     164,   165,   166,   167
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   181,     0,     1,    43,    62,    63,    64,    65,    66,
      67,    96,    97,    98,    99,   100,   102,   110,   111,   112,
     113,   114,   115,   116,   134,   182,   194,   246,   104,    42,
      43,    48,    49,    50,    52,    53,    54,    62,    63,    64,
      65,    66,    67,    76,    77,    93,    94,    95,   108,   152,
     163,   164,   168,   169,   173,   177,   178,   242,   243,   244,
     248,   171,   171,   248,   171,   250,   242,   242,    43,   195,
     196,   246,   197,   198,   246,   171,   171,   171,   184,    44,
      55,    58,   101,   105,   148,   149,   150,   151,   171,   183,
      11,    12,    61,    68,    69,    76,    77,    78,    79,    80,
      81,    82,    83,    90,    91,    92,   103,   155,   156,   157,
     158,   163,   164,   165,   166,   167,   193,   199,   185,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     245,   242,   242,   242,   242,   242,   242,     8,     9,    10,
      11,    12,    68,    69,    70,    71,    72,    73,    74,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   154,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   171,
     106,   106,   249,   242,   171,   171,   171,   172,   171,   172,
     135,   171,   242,     3,   108,   109,   242,   242,   248,   247,
     242,   205,   206,   205,   171,     4,     5,     6,     7,     8,
       9,    10,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    45,    46,
      47,    51,    56,    57,    59,   107,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   130,   131,
     132,   133,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   200,   242,   242,   155,   163,   164,    60,
     159,   159,   176,   204,   207,   208,   135,   174,   179,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   249,   249,   171,   251,   196,   198,   171,   171,   171,
     171,   172,   171,   172,   106,   187,   247,   172,   208,   172,
     242,    29,    30,    31,    32,    34,    40,   173,   217,   218,
     219,   221,   222,   223,   224,   225,   242,    31,    34,    35,
     217,   221,   232,   242,    31,   217,   221,   242,    31,   217,
     242,    31,   217,   242,    31,   217,   242,   242,    32,    40,
      41,   233,   234,   242,    31,    33,    38,   217,   226,   230,
     232,   242,   242,    33,    36,   173,   242,   242,   173,   175,
     222,   242,   234,   242,    39,   173,   217,   226,   243,   173,
     242,   243,    33,    36,    37,   129,   222,   228,   229,   232,
     233,   218,   224,   242,   242,   242,   242,   242,   201,   203,
     202,    38,   222,   227,   227,   222,   242,   242,   222,   242,
     242,   220,   221,   222,   220,   227,   220,   242,   173,   233,
     242,   242,   242,   222,   222,   242,   225,   242,   225,   222,
     222,   242,   242,   230,   232,   171,   171,   172,   171,   171,
     242,   242,   242,   171,   172,   153,   155,   171,   171,   171,
     242,   242,   249,   106,   167,   214,   215,   171,   189,   191,
     172,   172,    34,    35,   173,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,    38,    32,   172,   172,   172,
     172,    33,   172,   172,   172,    32,   172,   172,     3,   109,
     235,   236,   242,   239,   240,   242,   237,   238,   242,   172,
     172,   172,   232,   172,   172,   172,   172,   172,   242,   171,
     171,   171,   186,   208,   242,   171,   171,   171,   216,   171,
     172,   188,   247,   247,   217,   242,    34,   230,   231,   174,
     174,   225,    35,   217,   242,   230,   232,   217,   242,   217,
     242,   217,   242,   217,   242,   218,   224,   242,    34,    37,
     174,   174,   173,   173,   243,   242,    31,   174,    39,   173,
     217,   243,   244,   109,   232,   241,   242,   243,   244,    31,
     226,   174,    31,   222,   172,   172,   172,   241,   242,   220,
     174,   242,   225,   225,   225,   225,   171,   209,   210,   242,
     215,   209,   215,   215,   174,   174,   172,   172,    32,    32,
     172,    33,   172,   236,   240,   238,   136,   249,   249,   222,
     232,   174,   174,    31,   174,   222,   242,   211,   171,   171,
     153,   212,   213,   190,   192,   171,   172,   209,   209,   213
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   180,   181,   181,   182,   183,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   184,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     185,   186,   182,   187,   182,   188,   182,   189,   190,   182,
     191,   192,   182,   182,   182,   193,   193,   193,   194,   194,
     195,   195,   196,   197,   197,   198,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   199,   199,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     201,   200,   202,   200,   203,   200,   200,   204,   204,   204,
     206,   205,   207,   207,   208,   209,   210,   211,   210,   212,
     212,   212,   213,   214,   214,   214,   215,   216,   215,   217,
     217,   218,   218,   219,   220,   220,   221,   222,   222,   222,
     223,   223,   224,   224,   224,   225,   226,   226,   227,   227,
     228,   228,   228,   228,   229,   229,   230,   230,   231,   231,
     232,   232,   233,   233,   234,   234,   235,   235,   236,   236,
     236,   237,   237,   238,   239,   239,   240,   241,   241,   242,
     242,   243,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   245,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   246,   246,   246,   246,   246,   246,   246,   247,
     248,   249,   250,   251
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     4,     5,     5,     4,
       5,     4,     4,     6,     3,     5,     2,     2,     3,     4,
       4,     6,     4,     4,     4,     6,     5,     4,     6,     0,
       3,     2,     5,     3,     3,     3,     3,     2,     2,     2,
       0,     0,     7,     0,     6,     0,     7,     0,     0,    11,
       0,     0,    11,     3,     1,     0,     1,     2,     0,     2,
       1,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     2,     2,     2,     2,     2,     4,     2,
       4,     2,     2,     2,     2,     4,     4,     4,     4,     2,
       4,     2,     2,     4,     2,     2,     2,     2,     2,     2,
       4,     4,     4,     4,     2,     2,     4,     2,     2,     4,
       4,     4,     2,     2,     2,     2,     2,     2,     2,     2,
       4,     4,     6,     4,     2,     4,     4,     2,     2,     4,
       2,     2,     1,     2,     4,     4,     4,     4,     4,     4,
       4,     6,     2,     4,     2,     6,     2,     4,     2,     4,
       4,     4,     4,     4,     2,     4,     2,     4,     2,     4,
       4,     6,     4,     2,     6,     2,     6,     4,     4,     2,
       6,     2,     6,     2,     2,     1,     2,     2,     2,     2,
       0,     3,     0,     3,     0,     3,     1,     0,     1,     3,
       0,     2,     0,     1,     2,     1,     0,     0,     5,     0,
       1,     3,     1,     0,     1,     3,     1,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     0,     0,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 2020 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { 
		// An identfier without a colon all by itself on a line
		// will be interpreted as a label.  But there's a very
		// good chance it is a misspelling of an instruction or
		// pseudo-op name creating silent errors.  Since the condition
		// is unusual we print a warning.  Unless it is followed by
		// a colon in which case there's no ambiguity.
		if ((yyvsp[-1].itemptr) && !firstcol && coloncnt == 0 && outpass) {
			fprintf(stderr, "%s(%d): warning: '%s' treated as label (instruction typo?)\n",
				src_name[now_in], linein[now_in], (yyvsp[-1].itemptr)->i_string);
			fprintf(stderr, "\tAdd a colon or move to first column to stop this warning.\n");
		}

		if ((yyvsp[-1].itemptr)) list(dollarsign);
		else  list1();
	}
#line 4715 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 2037 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { list_dollarsign = 1; }
#line 4721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 2037 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list(list_dollarsign ? dollarsign : list_addr);
	}
#line 4729 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 2041 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		do_equ((yyvsp[-4].itemptr), (yyvsp[-1].exprptr), 1);
		if ((yyvsp[-3].ival) == 2)
			(yyvsp[-4].itemptr)->i_scope |= SCOPE_PUBLIC;
	}
#line 4739 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 2047 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		do_defl((yyvsp[-4].itemptr), (yyvsp[-1].exprptr), 1); // TODO: is '=' equate or defl?
		// I don't even recall what assembler I saw that allows '='
		// Not MACR0-80.  Not MRAS.  Not MAC.
		// I choose "defl" since it works so nicely with +=, etc.
		if ((yyvsp[-3].ival) == 2)
			(yyvsp[-4].itemptr)->i_scope |= SCOPE_PUBLIC;
	}
#line 4752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 2056 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		do_defl((yyvsp[-3].itemptr), (yyvsp[-1].exprptr), 1);
	}
#line 4760 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 2060 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		do_defl((yyvsp[-4].itemptr), expr_mk(expr_var((yyvsp[-4].itemptr)), (yyvsp[-3].ival), (yyvsp[-1].exprptr)), 1);
	}
#line 4768 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 2064 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		do_defl((yyvsp[-3].itemptr), expr_mk(expr_var((yyvsp[-3].itemptr)), '+', expr_num(1)), 1);
	}
#line 4776 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 2068 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		do_defl((yyvsp[-3].itemptr), expr_mk(expr_var((yyvsp[-3].itemptr)), '-', expr_num(1)), 1);
	}
#line 4784 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 2072 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		int val3 = (yyvsp[-3].exprptr)->e_value;
		int val5 = (yyvsp[-1].exprptr)->e_value;
		expr_reloc_check((yyvsp[-3].exprptr));
		expr_reloc_check((yyvsp[-1].exprptr));
		expr_scope_same((yyvsp[-3].exprptr), (yyvsp[-1].exprptr));
		switch ((yyvsp[-5].itemptr)->i_token) {
		case UNDECLARED: case DEFLED:
			(yyvsp[-5].itemptr)->i_token = DEFLED;
			(yyvsp[-5].itemptr)->i_pass = npass;
			(yyvsp[-5].itemptr)->i_scope |= (yyvsp[-3].exprptr)->e_scope;
			if ((yyvsp[-4].itemptr)->i_value)	/* max */
				list((yyvsp[-5].itemptr)->i_value = (val3 > val5? val3:val5));
			else list((yyvsp[-5].itemptr)->i_value = (val3 < val5? val3:val5));
			break;
		default:
			err[mflag]++;
			(yyvsp[-5].itemptr)->i_token = MULTDEF;
			(yyvsp[-5].itemptr)->i_pass = npass;
			list((yyvsp[-5].itemptr)->i_value);
		}
		expr_free((yyvsp[-3].exprptr));
		expr_free((yyvsp[-1].exprptr));
	}
#line 4813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 2097 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		expr_number_check((yyvsp[-1].exprptr));
		if (ifptr >= ifstmax)
			error("Too many ifs");
		else
			*++ifptr = !((yyvsp[-1].exprptr)->e_value);

		saveopt = fopt;
		fopt = 1;
		list((yyvsp[-1].exprptr)->e_value);
		fopt = saveopt;
		expr_free((yyvsp[-1].exprptr));
	}
#line 4831 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 2112 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		struct item *ip = locate(tempbuf);
		int declared = ip && ip->i_pass == npass;
		int value = declared == (yyvsp[-4].itemptr)->i_value;

		if (ifptr >= ifstmax)
			error("Too many ifs");
		else
			*++ifptr = !value;

		saveopt = fopt;
		fopt = 1;
		list(value);
		fopt = saveopt;
	}
#line 4851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 2128 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		/* FIXME: it would be nice to spot repeated ELSEs, but how? */
		*ifptr = !*ifptr;
		saveopt = fopt;
		fopt = 1;
		list1();
		fopt = saveopt;
	}
#line 4864 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 2137 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		if (ifptr == ifstack) err[bflag]++;
		else --ifptr;
		list1();
	}
#line 4874 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 2143 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list(dollarsign);
		do_end(NULL);
	}
#line 4883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 2148 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list((yyvsp[-1].exprptr)->e_value);
		do_end((yyvsp[-1].exprptr));
	}
#line 4892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 2153 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		expr_number_check((yyvsp[-1].exprptr));
		if ((yyvsp[-1].exprptr)->e_value < 0) err[vflag]++;
		if ((yyvsp[-1].exprptr)->e_value > 0) {
			if (!phaseflag) {
				list(dollarsign);
				flushbin();
				flushoth();
				dollarsign += (yyvsp[-1].exprptr)->e_value;
				olddollar += (yyvsp[-1].exprptr)->e_value;
				oldothdollar += (yyvsp[-1].exprptr)->e_value;
				emit_addr += (yyvsp[-1].exprptr)->e_value;
				advance_segment((yyvsp[-1].exprptr)->e_value);
				putrelcmd(RELCMD_SETLOC);
				putrelsegref(segment, seg_pos[segment]);
			}
			else
				dc((yyvsp[-1].exprptr)->e_value, 0);
		}
		else
			list1();

		expr_free((yyvsp[-1].exprptr));
	}
#line 4921 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 2178 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		expr_number_check((yyvsp[-3].exprptr));
		expr_number_check((yyvsp[-1].exprptr));
		if ((yyvsp[-3].exprptr)->e_value < 0) err[vflag]++;
		if ((yyvsp[-1].exprptr)->e_value < -128 || (yyvsp[-1].exprptr)->e_value > 127) err[vflag]++;
		if ((yyvsp[-3].exprptr)->e_value > 0) {
			dc((yyvsp[-3].exprptr)->e_value, (yyvsp[-1].exprptr)->e_value);
		}
		else
			list1();

		expr_free((yyvsp[-3].exprptr));
		expr_free((yyvsp[-1].exprptr));
	}
#line 4940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 2193 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_DATA, expr_num((yyvsp[-1].ival) | 0x80)); list(dollarsign); }
#line 4946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 2195 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_DATA, expr_num((yyvsp[-1].ival))); emit(1, E_DATA, expr_num(((yyvsp[-1].ival) >> 8) | 0x80)); list(dollarsign); }
#line 4952 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 2198 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			for (cp = (yyvsp[-1].cval); *cp != '\0'; cp++)
				if (!cp[1])
					emit(1, E_DATA, expr_num(*cp | 0x80));
				else
					emit(1, E_DATA, expr_num(*cp));

			list(dollarsign);
		}
#line 4966 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 2209 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_number_check((yyvsp[-3].exprptr));
			expr_number_check((yyvsp[-1].exprptr));
			dc((yyvsp[-3].exprptr)->e_value, (yyvsp[-1].exprptr)->e_value);
			expr_free((yyvsp[-3].exprptr));
			expr_free((yyvsp[-1].exprptr));
		}
#line 4978 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 2217 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list1();
		switch ((yyvsp[-4].itemptr)->i_value) {

		case PSTITL:	/* title */
			lineptr = linebuf;
			cp = tempbuf;
			title = titlespace;
			while ((*title++ = *cp++) && (title < &titlespace[TITLELEN]));
			*title = 0;
			title = titlespace;
			break;

		case PSRSYM:	/* rsym */
			if (pass2) break;
			insymtab(tempbuf);
			break;

		case PSWSYM:	/* wsym */
			writesyms = malloc(strlen(tempbuf)+1);
			strcpy(writesyms, tempbuf);
			break;
		case PSINC:	/* include file */
			if (mras && !strchr(tempbuf, '.')) {
				strcat(tempbuf, ".asm");
			}
			next_source(tempbuf, 1);
			break ;
		case PSIMPORT:	/* import file */
			next_source(tempbuf, 0);
			break;
		case PSMACLIB:
			strcat(tempbuf, ".lib");
			next_source(tempbuf, 1);
			break;
		case PSCMN:
			common_block(tempbuf);
			break;
		}
	}
#line 5023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 2258 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-3].itemptr)->i_value == PSCMN) {
			common_block(" ");
		}
		else {
			fprintf(stderr, "Missing argument of '%s'\n", (yyvsp[-3].itemptr)->i_string);
			err[fflag]++;
		}
		list1();
	}
#line 5038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 2269 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		incbin(tempbuf);
	}
#line 5046 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 2273 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { raw = 1; }
#line 5052 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 2273 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		int quote = 0;
		char *p, *q;
		switch ((yyvsp[-2].itemptr)->i_value) {
		case SPTITL:
			cp = tempbuf;
			title = titlespace;
			if (*cp == '\'' || *cp == '"')
				quote = *cp++;
			while ((*title++ = *cp++) && (title < &titlespace[TITLELEN]));
			if (quote && title > titlespace + 1 && title[-2] == quote)
				title[-2] = '\0';
			title = titlespace;
			list1();
			break;
		case SPSBTL:
			err[warn_notimpl]++;
			list1();
			break;
		case SPNAME:
			// Drop surrounding ('') if present
			p = tempbuf;
			q = strchr(tempbuf, '\0') - 1;
			if (*p == '(' && *q == ')' && q > p) p++, q--;
			if (*p == '\'' && *q == '\'' && q > p) p++, q--;
			q[1] = '\0';
			strncpy(progname, p, sizeof progname);
			progname[sizeof progname - 1] = '\0';
			list1();
			break;
		case SPCOM:
			quote = *tempbuf;
			list1();
			for (;;) {
				raw = 1;
				yychar = yylex();
				list1();
				if (yychar == 0)
					break;
				if (*tempbuf == quote) {
					yychar = yylex();
					break;
				}
			}
			break;

		case SPPRAGMA:
			if (strncmp(tempbuf, "bds", 3) == 0 && fbds && outpass) {
				fprintf(fbds, "%s\n", tempbuf + 4);
			}
			if (strncmp(tempbuf, "mds", 3) == 0 && fmds && outpass) {
				fprintf(fmds, "%s\n", tempbuf + 4);
			}
			list1();
			break;
		}
	}
#line 5114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 2331 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		goto dolopt; }
#line 5121 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 2334 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		int enable = (yyvsp[-2].exprptr)->e_value;

		enable = (yyvsp[-2].exprptr)->e_value;

		if (mras) {
			if (ci_strcmp(tempbuf, "on") == 0)
				enable = 1;
			else if (ci_strcmp(tempbuf, "off") == 0)
				enable = -1;
			else {
				fprintf(stderr, "LIST not given 'on' or 'off'\n");
				err[fflag]++;
				list(dollarsign);
				goto dolopt_done;
			}
		}
		else {
			expr_number_check((yyvsp[-2].exprptr));
			expr_free((yyvsp[-2].exprptr));
		}
		goto doloptA;
	dolopt:
		enable = 1;
	doloptA:
		linecnt++;
		if (outpass) {
			lineptr = linebuf;
			switch ((yyvsp[-4].itemptr)->i_value) {
			case 0:	/* list */
				if (enable < 0) lstoff = 1;
				if (enable > 0) lstoff = 0;
				break;

			case 1:	/* eject */
				if (enable) eject();
				break;

			case 2:	/* space */
				if ((line + enable) > 60) eject();
				else space(enable);
				break;

			case 3:	/* elist */
				eopt = edef;
				if (enable < 0) eopt = 0;
				if (enable > 0) eopt = 1;
				break;

			case 4:	/* fopt */
				fopt = fdef;
				if (enable < 0) fopt = 0;
				if (enable > 0) fopt = 1;
				break;

			case 5:	/* gopt */
				gopt = gdef;
				if (enable < 0) gopt = 1;
				if (enable > 0) gopt = 0;
				break;

			case 6: /* mopt */
				mopt = mdef;
				if (enable < 0) mopt = 0;
				if (enable > 0) mopt = 1;
			}
		}
	dolopt_done: ;
	}
#line 5195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 2404 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		expr_number_check((yyvsp[-1].exprptr));
		jopt = !!(yyvsp[-1].exprptr)->e_value;
		list1();
		expr_free((yyvsp[-1].exprptr));
	}
#line 5206 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 2411 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		expr_number_check((yyvsp[-1].exprptr));
		JPopt = !!(yyvsp[-1].exprptr)->e_value;
		list1();
		expr_free((yyvsp[-1].exprptr));
	}
#line 5217 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 2418 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list1();
	}
#line 5225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 2422 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list1();
	}
#line 5233 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 2426 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		char *p = strchr(modstr, '\0') - 1;
		for (; p >= modstr; p--) {
			(*p)++;
			if (*p < 'Z')
				break;
			*p = 'A';
		}
		list1();
	}
#line 5248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 2437 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		if (relopt && segment != (yyvsp[-1].itemptr)->i_value) {
			segment = (yyvsp[-1].itemptr)->i_value;
			segchange = 1;
			dollarsign = seg_pos[(yyvsp[-1].itemptr)->i_value];
		}
		list1();
	}
#line 5261 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 2446 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		z80 = (yyvsp[-1].itemptr)->i_value;
		list1();
	}
#line 5270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 2451 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { param_parse = 1; }
#line 5276 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 2451 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		param_parse = 0;
		(yyvsp[-4].itemptr)->i_token = MNAME;
		(yyvsp[-4].itemptr)->i_pass = npass;
		(yyvsp[-4].itemptr)->i_value = mfptr;
		if (keyword((yyvsp[-4].itemptr)->i_string)) {
			sprintf(detail, "Macro '%s' will override the built-in '%s'",
				(yyvsp[-4].itemptr)->i_string, (yyvsp[-4].itemptr)->i_string);
			errwarn(warn_general, detail);
		}
#ifdef M_DEBUG
		fprintf (stderr, "%s(%d) [UNDECLARED MACRO %s]\n",
			src_name[now_in], linein[now_in], (yyvsp[-4].itemptr)->i_string);
#endif
		list1();
	}
#line 5297 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 2467 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		mlex_list_on++;
		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));
		mlex_list_on--;
		parm_number = 0;
	}
#line 5309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 2475 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { arg_state.macarg = 1; }
#line 5315 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 2475 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
#ifdef M_DEBUG
		fprintf (stderr, "%s(%d) [MNAME %s]\n",
			src_name[now_in], linein[now_in], (yyvsp[-4].itemptr)->i_string);
#endif
		(yyvsp[-4].itemptr)->i_uses++ ;
		arg_reset();
		parm_number = 0;
		list(dollarsign);
		expptr++;
		est = est2;
		est2 = NULL; // GWP - this may leak, but it avoids double-free crashes
		est[FLOC].value = floc;
		est[TEMPNUM].value = exp_number++;
		est[MIF].param = ifptr;
		est[REPNUM].value = 0;
		est[MSTR].param = NULL;
		floc = (yyvsp[-4].itemptr)->i_value;
		mfseek(mfile, (long)floc, 0);
	}
#line 5340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 2496 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		expr_reloc_check((yyvsp[-2].exprptr));
		list1();
		arg_reset();
	}
#line 5350 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 2501 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		int pos = mfptr;
		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));
		parm_number = 0;

		// MRAS compat would require treating the repeat count
		// as a byte value with 0 == 256.
		if ((yyvsp[-4].exprptr)->e_value > 0) {
			expptr++;
			est = est2;
			est2 = NULL;
			est[FLOC].value = floc;
			est[TEMPNUM].value = exp_number++;
			est[MIF].param = ifptr;
			est[REPNUM].value = (yyvsp[-4].exprptr)->e_value - 1;
			est[MSTART].value = pos;
			est[MSTR].param = NULL;
			floc = pos;
			mfseek(mfile, (long)floc, 0);
		}
	}
#line 5377 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 2524 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { parm_number = 0; }
#line 5383 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 2525 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list1();
	}
#line 5391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 2528 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		int pos = mfptr;

		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));

		parm_number = 0;

		if (est2[0].param[0]) {
			expptr++;
			est = est2;
			est2 = NULL;
			est[FLOC].value = floc;
			est[TEMPNUM].value = exp_number++;
			est[MIF].param = ifptr;
			est[REPNUM].value = 0;
			est[MSTART].value = pos;
			est[MSTR].param = est[0].param;
			est[0].param = malloc(2);
			est[0].param[0] = est[MSTR].param[0];
			est[0].param[1] = '\0';
			floc = pos;
			mfseek(mfile, (long)floc, 0);
		}
	}
#line 5421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 2554 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { parm_number = 0; }
#line 5427 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 2555 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		list1();
	}
#line 5435 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 2558 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		int pos = mfptr;
		mfseek(mfile, (long)mfptr, 0);
		mlex((yyvsp[0].cval));

		parm_number = 0;
		// if the sub list is not empty
		if (est2[0].param[0] && est2[0].param[0] != ';'
			&& est2[0].param[0] != '\n')
		{
			expptr++;
			est = est2;
			est2 = NULL;
			est[FLOC].value = floc;
			est[TEMPNUM].value = exp_number++;
			est[MIF].param = ifptr;
			est[REPNUM].value = -1;
			est[MSTART].value = pos;
			est[MSTR].param = NULL;

			est[MARGP].ap = malloc(sizeof *est[MARGP].ap);
			est[MARGP].ap->arg = malloc(TEMPBUFSIZE);
			est[MARGP].ap->argsize = TEMPBUFSIZE;
			est[MARGP].ap->getch = str_getch;
			est[MARGP].ap->user_ptr = est[0].param;
			est[MARGP].ap->user_int = 0;
			est[MARGP].ap->user_peek = -1;
			est[MARGP].ap->peek = &est[MARGP].ap->user_peek;
			est[MARGP].ap->macarg = 0;
			est[MARGP].ap->didarg = 0;
			est[MARGP].ap->numarg = 0;

			est[0].param = est[MARGP].ap->arg;
			getarg(est[MARGP].ap);

			floc = pos;
			mfseek(mfile, (long)floc, 0);
		}
	}
#line 5479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 2598 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		// XXX - popsi() is not safe, There is type-specific cleanup.
		//  But for now...
		// popsi() must be made safe as others use it.
		list1();
		popsi();
	}
#line 5491 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 2606 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		err[fflag]++;
		arg_reset();
		parm_number = 0;
		param_parse = 0;

		if (est2)
		{
			int i;
			for (i=0; i<PARMMAX; i++) {
				if (est2[i].param) {
#ifdef M_DEBUG
	fprintf (stderr, "[Freeing2 arg%u(%p)]\n", i, est2[i].param),
#endif
					free(est2[i].param);
				}
			}
			free(est2);
			est2 = NULL;
		}

		while(yychar != '\n' && yychar != '\0') yychar = yylex();
		list(dollarsign);
		yyclearin;yyerrok;
	}
#line 5521 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 2634 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 5527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 2636 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 5533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 2638 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = 2; }
#line 5539 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 2643 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	label = (yyval.itemptr) = NULL;	}
#line 5545 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 2645 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		coloncnt = (yyvsp[0].ival);
		itemcpy(&pristine_label, (yyvsp[-1].itemptr));
		label = coloncnt == 0 ? (yyvsp[-1].itemptr) : NULL;
		(yyvsp[-1].itemptr)->i_scope |= segment;
		if ((yyvsp[0].ival) == 2)
			(yyvsp[-1].itemptr)->i_scope |= SCOPE_PUBLIC;

		if ((yyvsp[-1].itemptr)->i_string[0] != '.')
			llseq++;

		switch((yyvsp[-1].itemptr)->i_token) {
		case UNDECLARED:
			if (pass2) {
				sprintf(detail, "%s error - label '%s' not declared",
					errname[pflag], (yyvsp[-1].itemptr)->i_string);
				errwarn(pflag, detail);
			}
			else {
				(yyvsp[-1].itemptr)->i_token = LABEL;
				(yyvsp[-1].itemptr)->i_pass = npass;
				(yyvsp[-1].itemptr)->i_value = dollarsign;
			}
			break;
		case LABEL:
			if (!pass2) {
				(yyvsp[-1].itemptr)->i_token = MULTDEF;
				(yyvsp[-1].itemptr)->i_pass = npass;
				err[mflag]++;
			} else if ((yyvsp[-1].itemptr)->i_value != dollarsign) {
				// XXX - perhaps only allow retrys if JR promotions are in play?
				if (outpass) {
					if (!passfail) {
						sprintf(detail, "%s error - label '%s' changed from $%04x to $%04x",
							errname[pflag], (yyvsp[-1].itemptr)->i_string, (yyvsp[-1].itemptr)->i_value, dollarsign);
						errwarn(pflag, detail);
					}
				}
				else {
					(yyvsp[-1].itemptr)->i_value = dollarsign;
					passretry = 1;
				}
			}
			break;
		default:
			err[mflag]++;
			(yyvsp[-1].itemptr)->i_token = MULTDEF;
			(yyvsp[-1].itemptr)->i_pass = npass;
		}
	}
#line 5600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 2704 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		if (!((yyvsp[0].itemptr)->i_scope & SCOPE_COMBLOCK))
			(yyvsp[0].itemptr)->i_scope |= SCOPE_PUBLIC;
		// Just being "public" does not #ifdef define a symbol.
		if (pass2) {
			if ((yyvsp[0].itemptr)->i_token == UNDECLARED) {
				sprintf(detail, "'%s' %s", (yyvsp[0].itemptr)->i_string, errname[uflag]);
				errwarn(uflag, detail);
			}
		}
	}
#line 5616 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 2724 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		if (pass2 && (yyvsp[0].itemptr)->i_scope != SCOPE_NONE && !((yyvsp[0].itemptr)->i_scope & SCOPE_EXTERNAL)) {
			fprintf(stderr, "Label scope change\n");
			err[fflag]++;
		}
		(yyvsp[0].itemptr)->i_pass = npass; // external labels seen as defined for #ifdef
		(yyvsp[0].itemptr)->i_scope |= SCOPE_EXTERNAL;
		if (pass2) {
			if ((yyvsp[0].itemptr)->i_token != UNDECLARED) {
				fprintf(stderr, "External label defined locally.\n");
				err[fflag]++;
			}
		}
	}
#line 5635 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 2741 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '+'; }
#line 5641 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 2742 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '-'; }
#line 5647 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 2743 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '*'; }
#line 5653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 2744 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '/'; }
#line 5659 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 2745 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '%'; }
#line 5665 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 2746 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '&'; }
#line 5671 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 2747 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '|'; }
#line 5677 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 2748 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '^'; }
#line 5683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 2749 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = ANDAND; }
#line 5689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 2750 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = OROR; }
#line 5695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 2751 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = SHL; }
#line 5701 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 2752 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = SHR; }
#line 5707 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 2754 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '+'; }
#line 5713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 2755 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '-'; }
#line 5719 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 2756 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '*'; }
#line 5725 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 2757 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '/'; }
#line 5731 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 2758 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '%'; }
#line 5737 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 2759 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '&'; }
#line 5743 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 2760 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '|'; }
#line 5749 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 2761 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = '^'; }
#line 5755 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 2762 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = MROP_SHL; }
#line 5761 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 2763 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = MROP_SHR; }
#line 5767 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 2764 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = MROP_SHIFT; }
#line 5773 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 2769 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1((yyvsp[0].itemptr)->i_value, 0, 0, ET_NOARG); }
#line 5779 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 2772 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			// XXX - maybe splitting out CPI is better?
			if (!z80 && (yyvsp[-1].itemptr)->i_value == 0166641)
				emit1(0376, 0, (yyvsp[0].exprptr), ET_BYTE);
			else
				err[fflag]++;
		}
#line 5791 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 2781 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if (!z80 && (yyvsp[0].itemptr)->i_value < 2)
				emit(1, E_CODE, 0, 007 | ((yyvsp[0].itemptr)->i_value << 3));
			else
				err[fflag]++;
		}
#line 5802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 2789 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		if (z80 || (yyvsp[-1].itemptr)->i_value == 0303) {
			checkjp(0, (yyvsp[0].exprptr));
			emit(1, E_CODE16, (yyvsp[0].exprptr), 0303);
		}
		else
			// can't optimize jump on plus
			emit(1, E_CODE16, (yyvsp[0].exprptr), 0362);
	}
#line 5816 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 2800 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	emit(1, E_CODE16, (yyvsp[0].exprptr), 0315);	}
#line 5822 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 2803 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		// accepts rst 0-7 or rst 0,8,16,...,56
		int vec = (yyvsp[0].exprptr)->e_value;
		expr_number_check((yyvsp[0].exprptr));
		if ((vec > 7 || vec < 0) && (vec & ~(7 << 3)))
			err[vflag]++;
		if (vec > 7) vec >>= 3;
		emit(1, E_CODE, 0, (yyvsp[-1].itemptr)->i_value + ((vec & 7) << 3));
		expr_free((yyvsp[0].exprptr));
	}
#line 5837 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 2815 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1((yyvsp[-1].itemptr)->i_value, 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5843 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 2818 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			emit(3, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value, disp);
		}
#line 5851 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 2823 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306, 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5857 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 2826 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306, 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5863 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 2829 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 + ((yyvsp[-1].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 2832 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 + ((yyvsp[-3].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 2835 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if (!z80 && (yyvsp[-1].itemptr)->i_value == 7)
				emit(1, E_CODE16, (yyvsp[0].exprptr), 0364);
			else
				emit1(0306 | ((yyvsp[-1].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE);
		}
#line 5886 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 2843 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 | ((yyvsp[-1].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 2846 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 | ((yyvsp[-1].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 2849 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 | ((yyvsp[-1].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5904 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 2852 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 | ((yyvsp[-3].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5910 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 2855 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 | ((yyvsp[-3].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5916 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 2858 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 | ((yyvsp[-3].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5922 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 2861 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0306 | ((yyvsp[-3].itemptr)->i_value << 3), 0, (yyvsp[0].exprptr), ET_BYTE); }
#line 5928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 2864 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5934 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 2867 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 2870 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_CODE, 0, 0206); }
#line 5946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 2873 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5952 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 2876 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-3].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5958 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 2879 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5964 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 2882 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5970 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 2885 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 2888 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 2891 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5988 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 2894 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 5994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 2897 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-3].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6000 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 2900 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-3].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6006 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 2903 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-3].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 2906 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0200 + ((yyvsp[-3].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 2909 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0145400 + ((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 2912 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			emit(4, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, 0xcb, disp, (yyvsp[-1].itemptr)->i_value);
		}
#line 6032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 2917 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0xCB00 + ((yyvsp[-3].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[-2].ival), 0, ET_NOARG_DISP); }
#line 6038 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 2920 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1((yyvsp[-1].itemptr)->i_value + (((yyvsp[0].ival) & 0377) << 3) + 4, (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6044 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 2923 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1((yyvsp[-1].itemptr)->i_value + (((yyvsp[0].ival) & 0377) << 3) + 4, (yyvsp[0].ival), 0, ET_NOARG_DISP); }
#line 6050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 2926 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { if ((yyvsp[-3].itemptr)->i_value == 1)
				emit(2,E_CODE,0,0355,0112+(yyvsp[0].ival));
			else
				emit(2,E_CODE,0,0355,0102+(yyvsp[0].ival));
		}
#line 6060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 2933 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emitdad((yyvsp[-2].ival),(yyvsp[0].ival)); }
#line 6066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 2936 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].ival) != (yyvsp[0].ival)) {
				fprintf(stderr,"ADD mar, mar error\n");
				err[gflag]++;
			}
			emitdad((yyvsp[-2].ival),(yyvsp[0].ival));
		}
#line 6078 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 2944 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emitdad(040, (yyvsp[0].ival)); }
#line 6084 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 2947 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			emit(2, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value | (yyvsp[0].ival));
		}
#line 6092 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 2952 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			int dst = (yyvsp[-1].itemptr)->i_value >> 8;
			int reg = (yyvsp[0].ival) >> 8;
			if (!reg) reg = 0xed;

			if (dst != reg) {
				if (dst == 0xed)
					fprintf(stderr, "dadc/dsbc cannot take ix or iy\n");
				else if (dst == 0xdd)
					fprintf(stderr, "dadx cannot take hl or iy\n");
				else
					fprintf(stderr, "dady cannot take hl or ix\n");
				err[gflag]++;
			}
			emit(2, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value | (yyvsp[0].ival));
		}
#line 6113 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 2970 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377) + 3, (yyvsp[0].ival), 0, ET_NOARG); }
#line 6119 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 2973 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(((yyvsp[-1].itemptr)->i_value << 3) + ((yyvsp[0].ival) & 0377) + 3, (yyvsp[0].ival), 0, ET_NOARG); }
#line 6125 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 2976 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG); }
#line 6131 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 2979 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1((yyvsp[-1].itemptr)->i_value + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG); }
#line 6137 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 2982 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if (strcmp((yyvsp[-1].itemptr)->i_string, "set") == 0 && label) {
				// Clear error that label definition will have been set
				err[mflag] = 0;
				itemcpy(label, &pristine_label);
				do_defl(label, (yyvsp[0].exprptr), 0);
				list_dollarsign = 0;
				list_addr = label->i_value;
			}
			else {
				err[fflag]++;
			}
		}
#line 6155 "y.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 2997 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			int bit = (yyvsp[-2].exprptr)->e_value;
			expr_number_check((yyvsp[-2].exprptr));
			expr_free((yyvsp[-2].exprptr));
			if (bit < 0 || bit > 7)
				err[vflag]++;
			emit1((yyvsp[-3].itemptr)->i_value + ((bit & 7) << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[0].ival), 0, ET_NOARG_DISP);
		}
#line 6168 "y.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 3007 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			int bit = (yyvsp[-2].exprptr)->e_value;
			expr_number_check((yyvsp[-2].exprptr));
			expr_free((yyvsp[-2].exprptr));
			if (bit < 0 || bit > 7)
				err[vflag]++;
			emit(4, E_CODE, 0, (yyvsp[-3].itemptr)->i_value >> 8, 0xcb, disp,
				(yyvsp[-3].itemptr)->i_value | (bit << 3));
		}
#line 6182 "y.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 3018 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			int bit = (yyvsp[-4].exprptr)->e_value;
			expr_number_check((yyvsp[-4].exprptr));
			expr_free((yyvsp[-4].exprptr));
			if (bit < 0 || bit > 7)
				err[vflag]++;
			emit1((yyvsp[-5].itemptr)->i_value + ((bit & 7) << 3) + ((yyvsp[0].ival) & 0377), (yyvsp[-2].ival), 0, ET_NOARG_DISP);
		}
#line 6195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 3028 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		checkjp((yyvsp[-2].ival), (yyvsp[0].exprptr));
		emit(1, E_CODE16, (yyvsp[0].exprptr), 0302 + (yyvsp[-2].ival));
	}
#line 6204 "y.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 3034 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		checkjp((yyvsp[-1].itemptr)->i_value, (yyvsp[0].exprptr));
		emit(1, E_CODE16, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value);
	}
#line 6213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 3040 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0351, (yyvsp[-1].ival), 0, ET_NOARG); }
#line 6219 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 3043 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_CODE16, (yyvsp[0].exprptr), 0304 + (yyvsp[-2].ival)); }
#line 6225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 3046 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_CODE16, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value); }
#line 6231 "y.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 3049 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emitjr(030,(yyvsp[0].exprptr)); }
#line 6237 "y.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 3052 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emitjr((yyvsp[-3].itemptr)->i_value + (yyvsp[-2].ival), (yyvsp[0].exprptr)); }
#line 6243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 3055 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emitjr((yyvsp[-1].itemptr)->i_value, (yyvsp[0].exprptr)); }
#line 6249 "y.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 3058 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emitjr((yyvsp[-1].itemptr)->i_value, (yyvsp[0].exprptr)); }
#line 6255 "y.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 3061 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_CODE, 0, (yyvsp[0].itemptr)->i_value); }
#line 6261 "y.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 3064 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_CODE, 0, 0300 + (yyvsp[0].ival)); }
#line 6267 "y.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 3067 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			// Many constraints on byte access to IX/IY registers.
			if (((yyvsp[-2].ival) | (yyvsp[0].ival)) >> 16) {
				int a = (yyvsp[-2].ival);
				int b = (yyvsp[0].ival);

				// Only ixh,ixh; ixh,ixl; ixl,ixh; ixl,ixl allowed.
				if (a >> 16 && b >> 16) {
					if (a >> 8 != b >> 8) {
						fprintf(stderr, "LD cannot move between ix and iy\n");
						err[gflag]++;
					}
				}
				else {
					int c = b >> 16 ? a : b;
					// No access to h, l, (hl), (ix), (iy)
					if (c == 4 || c == 5 || (c & 0xff) == 6) {
						fprintf(stderr, "LD cannot combine i/xy/lh and h,l,(hl),(ix) or (iy).\n");
						err[gflag]++;
					}
				}
			}

			if (((yyvsp[-2].ival) & 0377) == 6 && ((yyvsp[0].ival) & 0377) == 6) {
				fprintf(stderr,"LD reg, reg error: can't do memory to memory\n");
				err[gflag]++;
			}
			emit1(0100 + (((yyvsp[-2].ival) & 7) << 3) + ((yyvsp[0].ival) & 7),(yyvsp[-2].ival) | (yyvsp[0].ival), 0, ET_NOARG_DISP);
		}
#line 6301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 3098 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			emit(3, E_CODE, 0, (yyvsp[-3].itemptr)->i_value >> 8, (yyvsp[-3].itemptr)->i_value | ((yyvsp[-2].ival) << 3), disp);
		}
#line 6309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 3103 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			emit(3, E_CODE, 0, (yyvsp[-3].itemptr)->i_value >> 8, (yyvsp[-3].itemptr)->i_value | (yyvsp[-2].ival), disp);
		}
#line 6317 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 3108 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].ival) == 6 && (yyvsp[0].ival) == 6) err[gflag]++;
			emit1(0100 + (((yyvsp[-2].ival) & 7) << 3) + ((yyvsp[0].ival) & 7),(yyvsp[-2].ival) | (yyvsp[0].ival), 0, ET_NOARG_DISP);
		}
#line 6326 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 3114 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(6 + (((yyvsp[-2].ival) & 0377) << 3), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_BYTE); }
#line 6332 "y.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 3117 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			emit(3, E_CODE8, (yyvsp[-2].exprptr), (yyvsp[-3].itemptr)->i_value >> 8, (yyvsp[-3].itemptr)->i_value, disp);
		}
#line 6340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 3122 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(6 + (((yyvsp[-2].ival) & 0377) << 3), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_BYTE); }
#line 6346 "y.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 3125 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	if ((yyvsp[-4].ival) != 7) {
				fprintf(stderr,"LD reg, (RP) error\n");
				err[gflag]++;
			}
			else emit(1, E_CODE, 0, 012 + (yyvsp[-1].itemptr)->i_value);
		}
#line 6357 "y.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 3133 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[0].ival) != 0 && (yyvsp[0].ival) != 2) err[gflag]++;
			emit(1, E_CODE, 0, 012 + ((yyvsp[0].ival) << 3));
		}
#line 6366 "y.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 3139 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].ival) != 7) {
				fprintf(stderr,"LD reg, (expr) error: A only valid destination\n");
				err[gflag]++;
			}
			else {
				expr_word_check((yyvsp[0].exprptr));
				emit(1, E_CODE16, (yyvsp[0].exprptr), 072);
			}
		}
#line 6381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 3151 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			emit(1, E_CODE16, (yyvsp[0].exprptr), 072);
		}
#line 6390 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 3157 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_CODE, 0, 2 + (yyvsp[-3].itemptr)->i_value); }
#line 6396 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 3160 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[0].ival) != 0 && (yyvsp[0].ival) != 2) err[gflag]++;
			emit(1, E_CODE, 0, 2 + ((yyvsp[0].ival) << 3));
		}
#line 6405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 3166 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[-2].exprptr));
			emit(1, E_CODE16, (yyvsp[-2].exprptr), 062);
		}
#line 6414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 3172 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			emit(1, E_CODE16, (yyvsp[0].exprptr), 062);
		}
#line 6423 "y.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 3178 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].ival) != 7) {
				fprintf(stderr,"LD reg, MISCREG error: A only valid destination\n");
				err[gflag]++;
			}
			else emit(2, E_CODE, 0, 0355, 0127 + (yyvsp[0].itemptr)->i_value);
		}
#line 6435 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 3187 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(2, E_CODE, 0, 0355, 0107 + (yyvsp[-2].itemptr)->i_value); }
#line 6441 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 3190 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			emit1(1 + ((yyvsp[-2].ival) & 060), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_WORD);
		}
#line 6450 "y.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 3196 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			emit1(1 + ((yyvsp[-2].ival) & 060), (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_WORD);
		}
#line 6459 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 3202 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			if (((yyvsp[-2].ival) & 060) == 040)
				emit1(052, (yyvsp[-2].ival), (yyvsp[0].exprptr), ET_WORD);
			else
				emit(2, E_CODE16, (yyvsp[0].exprptr), 0355, 0113 + (yyvsp[-2].ival));
		}
#line 6471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 3211 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			emit1(052, 040, (yyvsp[0].exprptr), ET_WORD);
		}
#line 6480 "y.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 3217 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[-2].exprptr));
			if (((yyvsp[0].ival) & 060) == 040)
				emit1(042, (yyvsp[0].ival), (yyvsp[-2].exprptr), ET_WORD);
			else
				emit(2, E_CODE16, (yyvsp[-2].exprptr), 0355, 0103 + (yyvsp[0].ival));
		}
#line 6492 "y.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 3226 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			emit1(042, 040, (yyvsp[0].exprptr), ET_WORD);
		}
#line 6501 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 3232 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].ival) != 060) {
				fprintf(stderr,"LD evenreg error\n");
				err[gflag]++;
			}
			else
				emit1(0371, (yyvsp[0].ival), 0, ET_NOARG);
		}
#line 6514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 3242 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_word_check((yyvsp[0].exprptr));
			emit(2, E_CODE16, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value);
		}
#line 6523 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 3248 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].itemptr)->i_value != 020) {
				fprintf(stderr,"EX RP, HL error\n");
				err[gflag]++;
			}
			else
				emit(1, E_CODE, 0, 0353);
		}
#line 6536 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 3258 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(1, E_CODE, 0, 010); }
#line 6542 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 3261 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit1(0343, (yyvsp[0].ival), 0, ET_NOARG); }
#line 6548 "y.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 3264 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].ival) != 7) {
				fprintf(stderr,"IN reg, (expr) error\n");
				err[gflag]++;
			}
			else	{
				if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
					err[vflag]++;
				emit(1, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-3].itemptr)->i_value);
			}
		}
#line 6564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 3277 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
				err[vflag]++;
			emit(1, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value);
		}
#line 6574 "y.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 3284 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(2, E_CODE, 0, 0355, 0100 + ((yyvsp[-4].ival) << 3)); }
#line 6580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 3287 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(2, E_CODE, 0, 0355, 0100 + ((yyvsp[0].ival) << 3)); }
#line 6586 "y.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 3290 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(2, E_CODE, 0, 0355, 0160); }
#line 6592 "y.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 3293 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(2, E_CODE, 0, 0355, 0160); }
#line 6598 "y.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 3296 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[-2].exprptr)->e_value < 0 || (yyvsp[-2].exprptr)->e_value > 255)
				err[vflag]++;
			emit(1, E_CODE8, (yyvsp[-2].exprptr), (yyvsp[-3].itemptr)->i_value);
		}
#line 6608 "y.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 3303 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[0].exprptr)->e_value < 0 || (yyvsp[0].exprptr)->e_value > 255)
				err[vflag]++;
			emit(1, E_CODE8, (yyvsp[0].exprptr), (yyvsp[-1].itemptr)->i_value);
		}
#line 6618 "y.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 3310 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(2, E_CODE, 0, 0355, 0101 + ((yyvsp[0].ival) << 3)); }
#line 6624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 3313 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { emit(2, E_CODE, 0, 0355, 0101 + ((yyvsp[0].ival) << 3)); }
#line 6630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 3316 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_number_check((yyvsp[0].exprptr));
			if ((yyvsp[0].exprptr)->e_value != 0) {
				fprintf(stderr, "Can only output 0 to port C with OUT\n");
				err[vflag]++;
			}
			expr_free((yyvsp[0].exprptr));

			emit(2, E_CODE8, 0, 0355, 0101 + (6 << 3));
		}
#line 6645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 3328 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			int im = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			expr_free((yyvsp[0].exprptr));
			if (im > 2 || im < 0)
				err[vflag]++;
			else
				emit(2, E_CODE, 0, (yyvsp[-1].itemptr)->i_value >> 8, (yyvsp[-1].itemptr)->i_value + ((im + (im > 0)) << 3));
		}
#line 6659 "y.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 3339 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_number_check((yyvsp[0].exprptr));
			if (phaseflag) {
				err[oflag]++;
			} else {
				phaseflag = 1;
				phdollar = dollarsign;
				dollarsign = (yyvsp[0].exprptr)->e_value;
				phbegin = dollarsign;
			}
			expr_free((yyvsp[0].exprptr));
		}
#line 6676 "y.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 3353 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if (!phaseflag) {
				err[oflag]++;
			} else {
				phaseflag = 0;
				dollarsign = phdollar + dollarsign - phbegin;
			}
		}
#line 6689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 3363 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_reloc_check((yyvsp[0].exprptr));
			// Cannot org to the other segment (but absolute values are OK)
			if (relopt && segment && ((yyvsp[0].exprptr)->e_scope & SCOPE_SEGMASK) != segment)
				err[rflag]++;
			if (phaseflag) {
				err[oflag]++;
				dollarsign = phdollar + dollarsign - phbegin;
				phaseflag = 0;
			}
			if ((yyvsp[0].exprptr)->e_value-dollarsign) {
				flushbin();
				flushoth();
				olddollar = (yyvsp[0].exprptr)->e_value;
				oldothdollar = (yyvsp[0].exprptr)->e_value;
				dollarsign = (yyvsp[0].exprptr)->e_value;
				emit_addr = (yyvsp[0].exprptr)->e_value;
				seg_pos[segment] = dollarsign;
				if (seg_pos[segment] > seg_size[segment]) {
					seg_size[segment] = seg_pos[segment];
					if (segment == SEG_COMMON && cur_common)
						cur_common->i_value = seg_size[segment];
				}
				putrelcmd(RELCMD_SETLOC);
				putrelsegref(segment, seg_pos[segment]);
				segchange = 0;
			}
			expr_free((yyvsp[0].exprptr));
		}
#line 6723 "y.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 3394 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			list_dollarsign = 0;
			list_addr = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			if (outpass && !(yyvsp[0].exprptr)->e_value)
			{
				err[aflag]++;
			}
			expr_free((yyvsp[0].exprptr));
		}
#line 6738 "y.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 3406 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			list_dollarsign = 0;
			list_addr = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			tstates = (yyvsp[0].exprptr)->e_value;
			tstatesum[emit_addr] = tstates;
			expr_free((yyvsp[0].exprptr));
		}
#line 6751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 3416 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			list_dollarsign = 0;
			list_addr = (yyvsp[0].exprptr)->e_value;
			expr_number_check((yyvsp[0].exprptr));
			ocf = (yyvsp[0].exprptr)->e_value;
			ocfsum[emit_addr] = ocf;
			expr_free((yyvsp[0].exprptr));
		}
#line 6764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 3425 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { full_exprs = 1; }
#line 6770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 3425 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { full_exprs = 0; }
#line 6776 "y.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 3427 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { full_exprs = 1; }
#line 6782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 3427 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { full_exprs = 0; }
#line 6788 "y.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 3429 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { full_exprs = 1; }
#line 6794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 3429 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { full_exprs = 0; }
#line 6800 "y.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 3441 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { param_parse = 1; }
#line 6806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 3441 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { param_parse = 0; }
#line 6812 "y.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 3443 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = 0; }
#line 6818 "y.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 3443 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = 1; }
#line 6824 "y.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 3447 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if (parm_number >= PARMMAX)
				error("Too many parameters");
			(yyvsp[0].itemptr)->i_value = parm_number++;
			(yyvsp[0].itemptr)->i_scope = (yyvsp[-1].ival);
			(yyvsp[0].itemptr)->i_chain = 0;
		}
#line 6836 "y.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 3456 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
		static char macpush[LINEBUFFERSIZE];
		// Because of locals the parser has to look ahead.
		// We'll have buffered that as we usually do so just a
		// matter of picking that up and cancelling any look-ahead.
		*lineptr = '\0';
		strcpy(macpush, linebuf);
		lineptr = linebuf;
		peekc = NOPEEK;
		yychar = YYEMPTY;
		(yyval.cval) = macpush;
	}
#line 6853 "y.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 3472 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { param_parse = 1; }
#line 6859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 3472 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { param_parse = 0; list1(); }
#line 6865 "y.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 3484 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if (parm_number >= PARMMAX)
				error("Too many parameters");
			(yyvsp[0].itemptr)->i_value = parm_number++;
			(yyvsp[0].itemptr)->i_scope = 0;
			(yyvsp[0].itemptr)->i_chain = 1;
		}
#line 6877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 3504 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			cp = malloc(strlen(tempbuf)+1);
#ifdef M_DEBUG
			fprintf (stderr, "[Arg%u(%p): %s]\n", parm_number, cp, tempbuf);
#endif
			est2[parm_number++].param = cp;
			strcpy(cp, tempbuf);
		}
#line 6890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 3513 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { arg_flag = 0; }
#line 6896 "y.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 3514 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			arg_flag = 1;
			expr_reloc_check((yyvsp[0].exprptr));
			sprintf(tempbuf, "%d", (yyvsp[0].exprptr)->e_value);
			est2[parm_number++].param = strdup(tempbuf);
			expr_free((yyvsp[0].exprptr));
		}
#line 6908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 3535 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 6916 "y.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 3545 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { if ((yyvsp[0].itemptr)->i_value != 070) err[gflag]++; (yyval.ival) = 6; }
#line 6922 "y.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 3549 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 6930 "y.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 3554 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 6938 "y.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 3559 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 6946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 3565 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = 6;
		}
#line 6954 "y.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 3573 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = ((yyvsp[-2].itemptr)->i_value & 0177400) | 6;
		}
#line 6962 "y.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 3578 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			disp = 0;
			(yyval.ival) = ((yyvsp[-1].itemptr)->i_value & 0177400) | 6;
		}
#line 6971 "y.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 3584 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = ((yyvsp[-1].itemptr)->i_value & 0177400) | 6;
		}
#line 6979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 3589 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_number_check((yyvsp[0].exprptr));
			disp = (yyvsp[0].exprptr)->e_value;
			expr_free((yyvsp[0].exprptr));
			if (disp > 127 || disp < -128)
				err[vflag]++;
		}
#line 6991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 3603 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { if ((yyvsp[0].ival) & 1) err[gflag]++; (yyval.ival) = (yyvsp[0].ival) << 3; }
#line 6997 "y.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 3605 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].itemptr)->i_value; }
#line 7003 "y.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 3609 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7011 "y.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 3614 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7019 "y.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 3619 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = 060;
		}
#line 7027 "y.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 3626 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { if ((yyvsp[0].ival) & 1) err[gflag]++; (yyval.ival) = (yyvsp[0].ival) << 3; }
#line 7033 "y.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 3628 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].itemptr)->i_value; }
#line 7039 "y.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 3632 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7047 "y.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 3637 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 3645 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7063 "y.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 3651 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7071 "y.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 3656 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 3664 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7087 "y.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 3670 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.ival) = (yyvsp[0].itemptr)->i_value;
		}
#line 7095 "y.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 3675 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.ival) = 030;	}
#line 7101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 3684 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			emit(1, E_DATA, expr_num((yyvsp[0].ival)));
			emit(1, E_DATA, expr_num((yyvsp[0].ival)>>8));
		}
#line 7110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 3690 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			cp = (yyvsp[0].cval);
			while (*cp != '\0')
				emit(1,E_DATA,expr_num(*cp++));
		}
#line 7120 "y.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 3697 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if (is_number((yyvsp[0].exprptr)) && ((yyvsp[0].exprptr)->e_value < -128 || (yyvsp[0].exprptr)->e_value > 255))
				err[vflag]++;
			emit(1, E_DATA, (yyvsp[0].exprptr));
		}
#line 7130 "y.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 3714 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[0].exprptr)->e_value < -32768 || (yyvsp[0].exprptr)->e_value > 65535) {
				err[vflag]++;
			}
			emit(2, E_DATA, (yyvsp[0].exprptr));
		}
#line 7141 "y.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 3731 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			// Can't check overflow as I only have 32 bit ints.
			emit(4, E_DATA, (yyvsp[0].exprptr));
		}
#line 7150 "y.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 3743 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_num((yyvsp[0].ival));
		}
#line 7158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 3756 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = (yyvsp[-1].exprptr);	}
#line 7164 "y.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 3773 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
			(yyvsp[0].itemptr)->i_uses++;
		}
#line 7173 "y.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 3779 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_num((yyvsp[0].ival));
		}
#line 7181 "y.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 3784 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_num((yyvsp[0].ival));
		}
#line 7189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 3789 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7197 "y.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 3794 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 3799 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 3804 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_var((yyvsp[0].itemptr));
		}
#line 7221 "y.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 3809 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_num(dollarsign + emitptr - emitbuf);
			(yyval.exprptr)->e_scope = segment;
		}
#line 7230 "y.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 3815 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_alloc();
			(yyval.exprptr)->e_token = 'u';
			(yyval.exprptr)->e_item = (yyvsp[0].itemptr);
			(yyval.exprptr)->e_scope = (yyvsp[0].itemptr)->i_scope;
			(yyval.exprptr)->e_value = 0;

			if (!((yyvsp[0].itemptr)->i_scope & SCOPE_EXTERNAL)) {
				sprintf(detail, "'%s' %s", (yyvsp[0].itemptr)->i_string, errname[uflag]);
				// Would be nice to add to list of undeclared errors
				errwarn(uflag, detail);
			}
		}
#line 7248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 3830 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_alloc();
			(yyval.exprptr)->e_token = 'm';
			(yyval.exprptr)->e_item = (yyvsp[0].itemptr);
			(yyval.exprptr)->e_scope = (yyvsp[0].itemptr)->i_scope;
			// Use the current value.  Harmless enough as this
			// will normally error out yet vital to allow
			// "var set var+1" to function.
			(yyval.exprptr)->e_value = (yyvsp[0].itemptr)->i_value;
		}
#line 7263 "y.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 3841 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { raw = 2; }
#line 7269 "y.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 3842 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_num(tempbuf[0] ? -1 : 0);
		}
#line 7277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 3845 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '+', (yyvsp[0].exprptr)); }
#line 7283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 3846 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '-', (yyvsp[0].exprptr)); }
#line 7289 "y.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 3847 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '/', (yyvsp[0].exprptr)); }
#line 7295 "y.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 3848 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '*', (yyvsp[0].exprptr)); }
#line 7301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 3849 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '%', (yyvsp[0].exprptr)); }
#line 7307 "y.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 3850 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '&', (yyvsp[0].exprptr)); }
#line 7313 "y.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 3851 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '&', (yyvsp[0].exprptr)); }
#line 7319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 3852 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '|', (yyvsp[0].exprptr)); }
#line 7325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 3853 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '|', (yyvsp[0].exprptr)); }
#line 7331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 3854 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '^', (yyvsp[0].exprptr)); }
#line 7337 "y.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 3855 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '^', (yyvsp[0].exprptr)); }
#line 7343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 3856 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHL, (yyvsp[0].exprptr)); }
#line 7349 "y.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 3857 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHR, (yyvsp[0].exprptr)); }
#line 7355 "y.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 3858 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '<', (yyvsp[0].exprptr)); }
#line 7361 "y.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 3859 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '<', (yyvsp[0].exprptr)); }
#line 7367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 3860 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '=', (yyvsp[0].exprptr)); }
#line 7373 "y.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 3861 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '=', (yyvsp[0].exprptr)); }
#line 7379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 3862 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '>', (yyvsp[0].exprptr)); }
#line 7385 "y.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 3863 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '>', (yyvsp[0].exprptr)); }
#line 7391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 3864 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), LE, (yyvsp[0].exprptr)); }
#line 7397 "y.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 3865 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), NE, (yyvsp[0].exprptr)); }
#line 7403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 3866 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), GE, (yyvsp[0].exprptr)); }
#line 7409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 3867 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), ANDAND, (yyvsp[0].exprptr)); }
#line 7415 "y.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 3868 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), OROR, (yyvsp[0].exprptr)); }
#line 7421 "y.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 3870 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '+', (yyvsp[0].exprptr)); }
#line 7427 "y.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 3871 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '-', (yyvsp[0].exprptr)); }
#line 7433 "y.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 3872 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '/', (yyvsp[0].exprptr)); }
#line 7439 "y.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 3873 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '*', (yyvsp[0].exprptr)); }
#line 7445 "y.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 3874 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '%', (yyvsp[0].exprptr)); }
#line 7451 "y.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 3875 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '&', (yyvsp[0].exprptr)); }
#line 7457 "y.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 3876 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '|', (yyvsp[0].exprptr)); }
#line 7463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 3877 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '^', (yyvsp[0].exprptr)); }
#line 7469 "y.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 3878 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHL, (yyvsp[0].exprptr)); }
#line 7475 "y.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 3879 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHR, (yyvsp[0].exprptr)); }
#line 7481 "y.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 3880 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '<', (yyvsp[0].exprptr)); }
#line 7487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 3881 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '=', (yyvsp[0].exprptr)); }
#line 7493 "y.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 3882 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), '>', (yyvsp[0].exprptr)); }
#line 7499 "y.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 3883 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), LE, (yyvsp[0].exprptr)); }
#line 7505 "y.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 3884 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), NE, (yyvsp[0].exprptr)); }
#line 7511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 3885 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { (yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), GE, (yyvsp[0].exprptr)); }
#line 7517 "y.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 3888 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			if ((yyvsp[0].exprptr)->e_value < 0) {
				(yyvsp[0].exprptr)->e_value = -(yyvsp[0].exprptr)->e_value;
				(yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHR, (yyvsp[0].exprptr));
			}
			else
				(yyval.exprptr) = expr_mk((yyvsp[-2].exprptr), SHL, (yyvsp[0].exprptr));
		}
#line 7530 "y.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 3898 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_number_check((yyvsp[-4].exprptr));
			if ((yyvsp[-4].exprptr)->e_value) {
				(yyval.exprptr) = (yyvsp[-2].exprptr);
				expr_free((yyvsp[0].exprptr));
			}
			else {
				(yyval.exprptr) = (yyvsp[0].exprptr);
				expr_free((yyvsp[-2].exprptr));
			}
			expr_free((yyvsp[-4].exprptr));
		}
#line 7547 "y.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 3912 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = (yyvsp[-1].exprptr);	}
#line 7553 "y.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 3915 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '~', 0, ~(yyvsp[0].exprptr)->e_value);	}
#line 7559 "y.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 3918 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '~', 0, ~(yyvsp[0].exprptr)->e_value);	}
#line 7565 "y.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 3921 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '!', 0, !(yyvsp[0].exprptr)->e_value * trueval);	}
#line 7571 "y.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 3924 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = (yyvsp[0].exprptr); /* no effect */	}
#line 7577 "y.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 3927 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = (yyvsp[0].exprptr); /* no effect */	}
#line 7583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 3930 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '-', 0, -(yyvsp[0].exprptr)->e_value);	}
#line 7589 "y.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 3933 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	(yyval.exprptr) = expr_op((yyvsp[0].exprptr), '-', 0, -(yyvsp[0].exprptr)->e_value);	}
#line 7595 "y.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 3936 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_reloc_check((yyvsp[0].exprptr));
			(yyval.exprptr) = expr_num(tstatesum[phaseaddr((yyvsp[0].exprptr)->e_value)]);
			expr_free((yyvsp[0].exprptr));
		}
#line 7605 "y.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 3943 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			int low;
			expr_reloc_check((yyvsp[0].exprptr));
			get_tstates(memory + phaseaddr((yyvsp[0].exprptr)->e_value), &low, 0, 0);
			(yyval.exprptr) = expr_num(low);
			expr_free((yyvsp[0].exprptr));
		}
#line 7617 "y.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 3952 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			int high;
			expr_reloc_check((yyvsp[0].exprptr));
			get_tstates(memory + phaseaddr((yyvsp[0].exprptr)->e_value), 0, &high, 0);
			(yyval.exprptr) = expr_num(high);
			expr_free((yyvsp[0].exprptr));
		}
#line 7629 "y.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 3961 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			expr_reloc_check((yyvsp[0].exprptr));
			(yyval.exprptr) = expr_num(ocfsum[phaseaddr((yyvsp[0].exprptr)->e_value)]);
			expr_free((yyvsp[0].exprptr));
		}
#line 7639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 3968 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), LOW, 0, (yyvsp[0].exprptr)->e_value & 0xff);
		}
#line 7647 "y.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 3973 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), LOW, 0, (yyvsp[0].exprptr)->e_value & 0xff);
		}
#line 7655 "y.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 3978 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), HIGH, 0, ((yyvsp[0].exprptr)->e_value >> 8) & 0xff);
		}
#line 7663 "y.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 3983 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {
			(yyval.exprptr) = expr_op((yyvsp[0].exprptr), HIGH, 0, ((yyvsp[0].exprptr)->e_value >> 8) & 0xff);
		}
#line 7671 "y.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 4006 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    { int i;
		if (expptr >= MAXEXP)
			error("Macro expansion level too deep");
		est2 = (union exprec *) malloc((PARMMAX + PAREXT) * sizeof *est2);
		expstack[expptr] = est2;
		for (i=0; i<PARMMAX; i++)
			est2[i].param = 0;
		arg_start();
	}
#line 7685 "y.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 4019 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	arg_start();	}
#line 7691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 4023 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	arg_reset();	}
#line 7697 "y.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 4027 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	if (mras) mras_undecl_ok = 1; }
#line 7703 "y.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 4031 "third_party/zmac/zmac.y" /* yacc.c:1646  */
    {	if (mras) mras_undecl_ok = 0; }
#line 7709 "y.tab.c" /* yacc.c:1646  */
    break;


#line 7713 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 4034 "third_party/zmac/zmac.y" /* yacc.c:1906  */

/*extern int	yylval;*/

#define F_END	0
#define OTHER	1
#define SPACE	2
#define DIGIT	3
#define LETTER	4
#define STARTER 5
#define DOLLAR	6


/*
 *  This is the table of character classes.  It is used by the lexical
 *  analyser. (yylex())
 */
char	charclass[] = {
	F_END,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	OTHER,	SPACE,	OTHER,	OTHER,	OTHER,	SPACE,	OTHER,	OTHER,
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,
	SPACE,	OTHER,	OTHER,	OTHER,	DOLLAR,	OTHER,	OTHER,	OTHER,	//  !"#$%&'
	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	STARTER,OTHER,	// ()*+,-./
	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	DIGIT,	// 01234567
	DIGIT,	DIGIT,	OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	STARTER,// 89:;<=>?
	STARTER,LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// @ABCDEFG
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// HIJKLMNO
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// PQRSTUVW
	LETTER, LETTER, LETTER, OTHER,	OTHER,	OTHER,	OTHER,	LETTER,	// XYZ[\]^_
	OTHER,	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// `abcdefg
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// hijklmno
	LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER, LETTER,	// pqrstuvw
	LETTER, LETTER, LETTER, OTHER,	OTHER,	OTHER,	OTHER,	OTHER,	// xyz{|}~
};
#define CHARCLASS(x) charclass[(x) & 0xff]


/*
 *  the following table tells which characters are parts of numbers.
 *  The entry is non-zero for characters which can be parts of numbers.
 */
char	numpart[] = {
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	'0',	'1',	'2',	'3',	'4',	'5',	'6',	'7',
	'8',	'9',	0,	0,	0,	0,	0,	0,
	0,	'A',	'B',	'C',	'D',	'E',	'F',	0,
	'H',	0,	0,	0,	0,	0,	0,	'O',
	0,	'Q',	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	'a',	'b',	'c',	'd',	'e',	'f',	0,
	'h',	0,	0,	0,	0,	0,	0,	'o',
	0,	'q',	0,	0,	0,	0,	0,	0,
	'x',	0,	0,	0,	0,	0,	0,	0,
	0};




/*
 *  the following table is a list of assembler mnemonics;
 *  for each mnemonic the associated machine-code bit pattern
 *  and symbol type are given.
 *
 *  The i_uses field is overloaded to indicate the possible uses for
 *  a token.
 */

#define VERB	(1)	/* opcode or psuedo-op */
#define I8080	(2)	/* used in 8080 instructions */
#define Z80	(4)	/* used in Z80 instructions */
#define UNDOC	(8)	/* used only in undocumented instructions */
#define TERM	(16)	/* can appear in expressions (not all marked) */
#define ZNONSTD	(32)	/* non-standard Z-80 mnemonic (probably TDL or DRI) */
#define COL0	(64)	/* will always be recognized in logical column 0 */
#define MRASOP	(128)	/* dig operator out of identifiers */

struct	item	keytab[] = {
	{"*include",	PSINC,	ARGPSEUDO,	VERB | COL0 },
	{"*list",	0,	LIST,		VERB | COL0 },
	{"*mod",	0,	MRAS_MOD,	VERB },
	{".8080",	0,	INSTSET,	VERB },
	{"a",		7,	ACC,		I8080 | Z80 },
	{"aci",		0316,	ALUI8,		VERB | I8080 },
	{"adc",		1,	ARITHC,		VERB | I8080 | Z80  },
	{"adcx",	0xdd8e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"adcy",	0xfd8e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"add",		0,	ADD,		VERB | I8080 | Z80  },
	{"addx",	0xdd86,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"addy",	0xfd86,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"adi",		0306,	ALUI8,		VERB | I8080 },
	{"af",		060,	AF,		Z80 },
	{"ana",		4,	ARITHC,		VERB | I8080},
	{"and",		4,	AND,		VERB | Z80 | TERM },
	{".and.",	0,	MROP_AND,	TERM | MRASOP },
	{"andx",	0xdda6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"andy",	0xfda6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"ani",		0346,	ALUI8,		VERB | I8080 },
	{".ascii",	0,	DEFB,		VERB },
	{".aseg",	SEG_ABS,SETSEG,		VERB },
	{".aset",	0,	DEFL,		VERB },
	{".assert",	0,	ASSERT,		VERB },
	{"b",		0,	REGNAME,	I8080 | Z80 },
	{"bc",		0,	RP,		Z80 },
	{"bit",		0145500,BIT,		VERB | Z80 },
	{"bitx",	0xdd46,	BIT_XY,		VERB | Z80 | ZNONSTD },
	{"bity",	0xfd46,	BIT_XY,		VERB | Z80 | ZNONSTD },
	{".block",	0,	DEFS,		VERB },
	{".byte",	0,	DEFB,		VERB },
	{".bytes",	0,	DC,		VERB },
	{"c",		1,	TK_C,		I8080 | Z80 },
	{"call",	0315,	CALL,		VERB | I8080 | Z80 },
	{"cc",		0334,	CALL8,		VERB | I8080 },
	{"ccd",		0xeda9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ccdr",	0xedb9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ccf",		077,	NOOPERAND,	VERB | Z80 },
	{"cci",		0xeda1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ccir",	0xedb1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"cm",		0374,	CALL8,		VERB | I8080 },
	{"cma",		057,	NOOPERAND,	VERB | I8080 },
	{"cmc",		077,	NOOPERAND,	VERB | I8080 },
	{"cmp",		7,	LOGICAL,	VERB | I8080 },
	{"cmpx",	0xddbe,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"cmpy",	0xfdbe,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"cnc",		0324,	CALL8,		VERB | I8080 },
	{"cnz",		0304,	CALL8,		VERB | I8080 },
	{".comment",	SPCOM,	SPECIAL,	VERB },
	{".common",	PSCMN,	ARGPSEUDO,	VERB },
	{".cond",	0,	IF_TK,		VERB },
	{"cp",		7,	LOGICAL,	VERB | I8080 | Z80 },
	{"cpd",		0166651,NOOPERAND,	VERB | Z80 },
	{"cpdr",	0166671,NOOPERAND,	VERB | Z80 },
	{"cpe",		0354,	CALL8,		VERB | I8080 },
	{"cpi",		0166641,NOOPERAND,	VERB | I8080 | Z80 },
	{"cpir",	0166661,NOOPERAND,	VERB | Z80 },
	{"cpl",		057,	NOOPERAND,	VERB | Z80 },
	{"cpo",		0344,	CALL8,		VERB | I8080 },
	{".cseg",	SEG_CODE,SETSEG,	VERB },
	{"cz",		0314,	CALL8,		VERB | I8080 },
	{"d",		2,	REGNAME,	I8080 | Z80 },
	{"daa",		0047,	NOOPERAND,	VERB | I8080 | Z80 },
	{"dad",		0,	DAD,		VERB | I8080 },
	{"dadc",	0xed4a,	ARITH16,	VERB | Z80 | ZNONSTD },
	{"dadx",	0xdd09,	ARITH16,	VERB | Z80 | ZNONSTD },
	{"dady",	0xfd09,	ARITH16,	VERB | Z80 | ZNONSTD },
	{".db",		0,	DEFB,		VERB },
	{".dc",		0,	DC,		VERB },
	{"dcr",		1,	INRDCR,		VERB | I8080 },
	{"dcrx",	0xdd35,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"dcry",	0xfd35,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"dcx",		1,	INXDCX,		VERB | I8080 },
	{"dcxix",	0xdd2b,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"dcxiy",	0xfd2b,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"de",		020,	RP,		Z80 },
	{"dec",		1,	INCDEC,		VERB | I8080 | Z80 },
	{".defb",	0,	DEFB,		VERB },
	{".defd",	0,	DEFD,		VERB },
	{".defl",	0,	DEFL,		VERB },
	{".defm",	0,	DEFB,		VERB },
	{".defs",	0,	DEFS,		VERB },
	{".defw",	0,	DEFW,		VERB },
	{".dephase",	0,	DEPHASE,	VERB },
	{"di",		0363,	NOOPERAND,	VERB | I8080 | Z80 },
	{"djnz",	020,	DJNZ,		VERB | Z80 },
	{".dm",		0,	DEFB,		VERB },
	{".ds",		0,	DEFS,		VERB },
	{"dsbc",	0xed42,	ARITH16,	VERB | Z80 | ZNONSTD },
	{".dseg",	SEG_DATA,SETSEG,	VERB },
	{".dw",		0,	DEFW,		VERB },
	{".dword",	0,	DEFD,		VERB },
	{"e",		3,	REGNAME,	I8080 | Z80 },
	{"ei",		0373,	NOOPERAND,	VERB | I8080 | Z80 },
	{".eject",	1,	LIST,		VERB },
	{".elist",	3,	LIST,		VERB },
	{".else",	0,	ELSE_TK,	VERB },
	{".end",	0,	END,		VERB },
	{".endc",	0,	ENDIF_TK,	VERB },
	{".endif",	0,	ENDIF_TK,	VERB },
	{".endm", 	0,	ENDM,		VERB },
	{".entry",	0,	PUBLIC,		VERB },
	{"eq",		0,	EQ,		0 },
	{".eq.",	0,	MROP_EQ,	TERM | MRASOP },
	{".equ",	0,	EQU,		VERB },
	{"ex",		0,	EX,		VERB | Z80 },
	{"exaf",	0x08,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{".exitm", 	0,	EXITM,		VERB },
	{".ext",	0,	EXTRN,		VERB },
	{".extern",	0,	EXTRN,		VERB },
	{".extrn",	0,	EXTRN,		VERB },
	{"exx",		0331,	NOOPERAND,	VERB | Z80 },
	{".flist",	4,	LIST,		VERB },
	{"ge",		0,	GE,		0 },
	{".ge.",	0,	MROP_GE,	TERM | MRASOP },
	{".glist",	5,	LIST,		VERB },
	{".global",	0,	PUBLIC,		VERB },
	{"gt",		0,	GT,		0 },
	{".gt.",	0,	MROP_GT,	TERM | MRASOP },
	{"h",		4,	REGNAME,	I8080 | Z80 },
	{"halt",	0166,	NOOPERAND,	VERB | Z80 },
	{"high",	0,	HIGH,		0 },
	{".high.",	0,	MROP_HIGH,	TERM | MRASOP },
	{"hl",		040,	HL,		Z80 },
	{"hlt",		0166,	NOOPERAND,	VERB | I8080 },
	{"i",		0,	MISCREG,	Z80 },
	{".if",		0,	IF_TK,		VERB | COL0 },
	{".ifdef",	1,	IF_DEF_TK,	VERB | COL0 },
	{".ifndef",	0,	IF_DEF_TK,	VERB | COL0 },
	{"im",		0166506,IM,		VERB | Z80 },
	{"im0",		0xed46,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"im1",		0xed56,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"im2",		0xed5e,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{".import",	PSIMPORT,ARGPSEUDO,	VERB | COL0 },
	{"in",		0333,	TK_IN,		VERB | I8080 | Z80 },
	{"inc",		0,	INCDEC,		VERB | Z80 },
	{".incbin", 	0, 	INCBIN,		VERB },
	{".include",	PSINC,	ARGPSEUDO,	VERB | COL0 },	// COL0 only needed for MRAS mode
	{"ind",		0166652,NOOPERAND,	VERB | Z80 },
	{"indr",	0166672,NOOPERAND,	VERB | Z80 },
	{"ini",		0166642,NOOPERAND,	VERB | Z80 },
	{"inir",	0166662,NOOPERAND,	VERB | Z80 },
	{"inp",		0,	INP,		VERB | Z80 | ZNONSTD },
	{"inr",		0,	INRDCR,		VERB | I8080 },
	{"inrx",	0xdd34,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"inry",	0xfd34,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"inx",		0,	INXDCX,		VERB | I8080 },
	{"inxix",	0xdd23,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"inxiy",	0xfd23,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"irp",		0,	IRP,		VERB },
	{"irpc",	0,	IRPC,		VERB },
	{"ix",		0156440,INDEX,		Z80 },
	{"ixh",		0x1DD04,IXYLH,		Z80 | UNDOC },
	{"ixl",		0x1DD05,IXYLH,		Z80 | UNDOC },
	{"iy",		0176440,INDEX,		Z80 },
	{"iyh",		0x1FD04,IXYLH,		Z80 | UNDOC },
	{"iyl",		0x1FD05,IXYLH,		Z80 | UNDOC },
	{"jc",		0332,	JUMP8,		VERB | I8080 },
	{"jm",		0372,	JUMP8,		VERB | I8080 },
	{"jmp",		0303,	JP,		VERB | I8080 },
	{"jnc",		0322,	JUMP8,		VERB | I8080 },
	{"jnz",		0302,	JUMP8,		VERB | I8080 },
	{"jp",		0,	JP,		VERB | I8080 | Z80 },
	{"jpe",		0352,	JUMP8,		VERB | I8080 },
	{".jperror",	0,	JPERROR,	VERB },
	{"jpo",		0342,	JUMP8,		VERB | I8080 },
	{"jr",		040,	JR,		VERB | Z80 },
	{"jrc",		0x38,	JR_COND,	VERB | Z80 | ZNONSTD },
	{"jrnc",	0x30,	JR_COND,	VERB | Z80 | ZNONSTD },
	{"jrnz",	0x20,	JR_COND,	VERB | Z80 | ZNONSTD },
	{".jrpromote",	0,	JRPROMOTE,	VERB },
	{"jrz",		0x28,	JR_COND,	VERB | Z80 | ZNONSTD },
	{"jz",		0312,	JUMP8,		VERB | I8080 },
	{"l",		5,	REGNAME,	I8080 | Z80 },
	{"lbcd",	0xed4b,	LDST16,		VERB | Z80 | ZNONSTD },
	{"ld",		0,	LD,		VERB | Z80 },
	{"lda",		0,	LDA,		VERB | I8080 },
	{"ldai",	0xed57,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ldar",	0xed5f,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"ldax",	0,	LDAX,		VERB | I8080 },
	{"ldd",		0166650,NOOPERAND,	VERB | Z80 },
	{"lddr",	0166670,NOOPERAND,	VERB | Z80 },
	{"lded",	0xed5b,	LDST16,		VERB | Z80 | ZNONSTD },
	{"ldi",		0166640,NOOPERAND,	VERB | Z80 },
	{"ldir",	0166660,NOOPERAND,	VERB | Z80 },
	{"ldx",		0xdd46,	LD_XY,		VERB | Z80 | ZNONSTD},
	{"ldy",		0xfd46,	LD_XY,		VERB | Z80 | ZNONSTD},
	{"le",		0,	LE,		0 },
	{".le.",	0,	MROP_LE,	TERM | MRASOP },
	{"lhld",	0,	LHLD,		VERB | I8080 },
	{".list",	0,	LIST,		VERB | COL0 }, // COL0 only needed for MRAS
	{"lixd",	0xdd2a,	LDST16,		VERB | Z80 | ZNONSTD },
	{"liyd",	0xfd2a,	LDST16,		VERB | Z80 | ZNONSTD },
	{".local",	0,	LOCAL,		VERB },
	{"low",		0,	LOW,		0 },
	{".low.",	0,	MROP_LOW,	TERM | MRASOP },
	{"lspd",	0xed7b,	LDST16,		VERB | Z80 | ZNONSTD },
	{"lt",		0,	LT,		0 },
	{".lt.",	0,	MROP_LT,	TERM | MRASOP },
	{"lxi",		0,	LXI,		VERB | I8080 },
	{"lxix",	0xdd21,	LDST16,		VERB | Z80 | ZNONSTD },
	{"lxiy",	0xfd21,	LDST16,		VERB | Z80 | ZNONSTD },
	{"m",		070,	COND,		I8080 | Z80 },
	{".maclib",	PSMACLIB,ARGPSEUDO,	VERB },
	{".macro",	0,	MACRO,		VERB },
	{".max",	1,	MINMAX,		VERB },
	{".min",	0,	MINMAX,		VERB },
	{".mlist",	6,	LIST,		VERB },
	{"mod",		0,	'%',		0 },
	{".mod.",	0,	MROP_MOD,	TERM | MRASOP },
	{"mov",		0,	MOV,		VERB | I8080 },
	{"mvi",		0,	MVI,		VERB | I8080 },
	{"mvix",	0xdd36,	MV_XY,		VERB | Z80 | ZNONSTD },
	{"mviy",	0xfd36,	MV_XY,		VERB | Z80 | ZNONSTD },
	{".name",	SPNAME,	SPECIAL,	VERB },
	{"nc",		020,	SPCOND,		0 },
	{"ne",		0,	NE,		0 },
	{".ne.",	0,	MROP_NE,	TERM | MRASOP },
	{"neg",		0166504,NOOPERAND,	VERB | Z80 },
	{".nolist",	-1,	LIST,		VERB },
	{"nop",		0,	NOOPERAND,	VERB | I8080 | Z80 },
	{"not",		0,	'~',		0 },
	{".not.",	0,	MROP_NOT,	TERM | MRASOP },
	{"nul",		0,	NUL,		0 },
	{"nv",		040,	COND,		Z80 },
	{"nz",		0,	SPCOND,		Z80 },
	{"ocf",		0,	OCF,		0 },
	{"or",		6,	OR,		VERB | Z80 | TERM },
	{".or.",	6,	MROP_OR,	TERM | MRASOP },
	{"ora",		6,	LOGICAL,	VERB | I8080 },
	{".org",	0,	ORG,		VERB },
	{"ori",		0366,	ALUI8,		VERB | I8080 },
	{"orx",		0xddb6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"ory",		0xfdb6,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"otdr",	0166673,NOOPERAND,	VERB | Z80 },
	{"otir",	0166663,NOOPERAND,	VERB | Z80 },
	{"out",		0323,	TK_OUT,		VERB | I8080 | Z80 },
	{"outd",	0166653,NOOPERAND,	VERB | Z80 },
	{"outdr",	0166673,NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"outi",	0166643,NOOPERAND,	VERB | Z80 },
	{"outir",	0166663,NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"outp",	0,	OUTP,		VERB | Z80 | ZNONSTD },
	{"p",		060,	COND,		Z80 },
	{".page",	1,	LIST,		VERB },
	{"pchl",	0351,	NOOPERAND,	VERB | I8080 },
	{"pcix",	0xdde9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pciy",	0xfde9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pe",		050,	COND,		Z80 },
	{"pfix",	0xdd,	NOOPERAND,	VERB | Z80 | UNDOC },
	{"pfiy",	0xfd,	NOOPERAND,	VERB | Z80 | UNDOC },
	{".phase",	0,	PHASE,		VERB },
	{"po",		040,	COND,		Z80 },
	{"pop",		0301,	PUSHPOP,	VERB | I8080 | Z80 },
	{"popix",	0xdde1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"popiy",	0xfde1,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pragma",	SPPRAGMA,SPECIAL,	VERB },
	{"psw", 	060,	PSW,		I8080 },
	{".public",	0,	PUBLIC,		VERB },
	{"push",	0305,	PUSHPOP,	VERB | I8080 | Z80 },
	{"pushix",	0xdde5,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"pushiy",	0xfde5,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"r",		010,	MISCREG,	Z80 },
	{"ral",		027,	NOOPERAND,	VERB | I8080 },
	{"ralr",	2,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"ralx",	0xdd16,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"raly",	0xfd16,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rar",		037,	NOOPERAND,	VERB | I8080 },
	{"rarr",	3,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"rarx",	0xdd1e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rary",	0xfd1e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rc",		0330,	NOOPERAND,	VERB | I8080 },
	{".read",	PSINC,	ARGPSEUDO,	VERB },
	{"rept",	0,	REPT,		VERB },
	{"res",		0145600,BIT,		VERB | Z80 },
	{"resx",	0xdd86,	BIT_XY,		VERB | Z80 | ZNONSTD },
	{"resy",	0xfd86,	BIT_XY,		VERB | Z80 | ZNONSTD },
	{"ret",		0311,	RET,		VERB | I8080 | Z80 },
	{"reti",	0166515,NOOPERAND,	VERB | Z80 },
	{"retn",	0166505,NOOPERAND,	VERB | Z80 },
	{"rl",		2,	SHIFT,		VERB | Z80 },
	{"rla",		027,	NOOPERAND,	VERB | Z80 },
	{"rlc",		0,	SHIFT,		VERB | I8080 | Z80 },
	{"rlca",	07,	NOOPERAND,	VERB | Z80 },
	{"rlcr",	0,	SHIFT,		VERB | I8080 | Z80 | ZNONSTD },
	{"rlcx",	0xdd06,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rlcy",	0xfd06,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rld",		0166557,NOOPERAND,	VERB | Z80 },
	{"rm",		0370,	NOOPERAND,	VERB | I8080 },
	{".rmem",	0,	DEFS,		VERB },
	{"rnc",		0320,	NOOPERAND,	VERB | I8080 },
	{"rnz",		0300,	NOOPERAND,	VERB | I8080 },
	{"rp",		0360,	NOOPERAND,	VERB | I8080 },
	{"rpe",		0350,	NOOPERAND,	VERB | I8080 },
	{"rpo",		0340,	NOOPERAND,	VERB | I8080 },
	{"rr",		3,	SHIFT,		VERB | Z80 },
	{"rra",		037,	NOOPERAND,	VERB | Z80 },
	{"rrc",		1,	SHIFT,		VERB | I8080 | Z80 },
	{"rrca",	017,	NOOPERAND,	VERB | Z80 },
	{"rrcr",	1,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"rrcx",	0xdd0e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rrcy",	0xfd0e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"rrd",		0166547,NOOPERAND,	VERB | Z80 },
	{"rst",		0307,	RST,		VERB | I8080 | Z80 },
	{".rsym",	PSRSYM,	ARGPSEUDO,	VERB },
	{"rz",		0310,	NOOPERAND,	VERB | I8080 },
	{"sbb",		3,	ARITHC,		VERB | I8080 },
	{"sbc",		3,	ARITHC,		VERB | Z80 },
	{"sbcd",	0xed43,	LDST16,		VERB | Z80 | ZNONSTD },
	{"sbcx",	0xdd9e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"sbcy",	0xfd9e,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"sbi",		0336,	ALUI8,		VERB | I8080 },
	{"scf",		067,	NOOPERAND,	VERB | Z80 },
	{"sded",	0xed53,	LDST16,		VERB | Z80 | ZNONSTD },
	{"set",		0145700,BIT,		VERB | Z80 },
	{"setb",	0145700,BIT,		VERB | Z80 | ZNONSTD },
	{".setocf",	0,	SETOCF,		VERB },
	{".sett",	0,	TSTATE,		VERB },
	{"setx",	0xddc6,	BIT_XY,		VERB | Z80 | ZNONSTD },
	{"sety",	0xfdc6,	BIT_XY,		VERB | Z80 | ZNONSTD },
	{"shl",		0,	SHL,		TERM },
	{".shl.",	0,	MROP_SHL,	TERM | MRASOP },
	{"shld",	0,	SHLD,		VERB | I8080 },
	{"shr",		0,	SHR,		TERM },
	{".shr.",	0,	MROP_SHR,	TERM | MRASOP },
	{"sixd",	0xdd22,	LDST16,		VERB | Z80 | ZNONSTD },
	{"siyd",	0xfd22,	LDST16,		VERB | Z80 | ZNONSTD },
	{"sl1",		6,	SHIFT,		VERB | Z80 | UNDOC },
	{"sla",		4,	SHIFT,		VERB | Z80 },
	{"slar",	4,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"slax",	0xdd26,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"slay",	0xfd26,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"sll",		6,	SHIFT,		VERB | Z80 },
	{"sp",		060,	SP,		I8080 | Z80 },
	{".space",	2,	LIST,		VERB },
	{"sphl",	0371,	NOOPERAND,	VERB | I8080 },
	{"spix",	0xddf9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"spiy",	0xfdf9,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"sra",		5,	SHIFT,		VERB | Z80 },
	{"srar",	5,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"srax",	0xdd2e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"sray",	0xfd2e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"srl",		7,	SHIFT,		VERB | Z80 },
	{"srlr",	7,	SHIFT,		VERB | Z80 | ZNONSTD },
	{"srlx",	0xdd3e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"srly",	0xfd3e,	SHIFT_XY,	VERB | Z80 | ZNONSTD },
	{"sspd",	0xed73,	LDST16,		VERB | Z80 | ZNONSTD },
	{"sta",		0,	STA,		VERB | I8080 },
	{"stai",	0xed47,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"star",	0xed4f,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"stax",	0,	STAX,		VERB | I8080 },
	{"stc",		067,	NOOPERAND,	VERB | I8080 },
	{"stx",		0xdd70,	ST_XY,		VERB | Z80 | ZNONSTD},
	{"sty",		0xfd70,	ST_XY,		VERB | Z80 | ZNONSTD},
	{"sub",		2,	LOGICAL,	VERB | I8080 | Z80 },
	{".subttl",	SPSBTL,	SPECIAL,	VERB },
	{"subx",	0xdd96,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"suby",	0xfd96,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"sui",		0326,	ALUI8,		VERB | I8080 },
	{"t",		0,	T,		0 },
	{".text",	0,	DEFB,		VERB },
	{"tihi",	0,	TIHI,		0 },
	{"tilo",	0,	TILO,		0 },
	{".title",	SPTITL,	SPECIAL,	VERB },
	{".tstate",	0,	TSTATE,		VERB },
	{"v",		050,	COND,		Z80 },
	{".word",	0,	DEFW,		VERB },
	{".wsym",	PSWSYM,	ARGPSEUDO,	VERB },
	{"xchg",	0353,	NOOPERAND,	VERB | I8080 },
	{"xor",		5,	XOR,		VERB | Z80 | TERM },
	{".xor.",	5,	MROP_XOR,	TERM | MRASOP },
	{"xorx",	0xddae,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"xory",	0xfdae,	ALU_XY,		VERB | Z80 | ZNONSTD },
	{"xra",		5,	LOGICAL,	VERB | I8080 },
	{"xri",		0356,	ALUI8,		VERB | I8080 },
	{"xthl",	0343,	NOOPERAND,	VERB | I8080 },
	{"xtix",	0xdde3,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"xtiy",	0xfde3,	NOOPERAND,	VERB | Z80 | ZNONSTD },
	{"z",		010,	SPCOND,		Z80 },
	{".z80",	1,	INSTSET,	VERB },
};

/*
 *  user-defined items are tabulated in the following table.
 */

struct item	itemtab[ITEMTABLESIZE];
struct item	*itemmax = itemtab+ITEMTABLESIZE;



/*
 *  lexical analyser, called by yyparse.
 */
int yylex()
{
	int lex();
	int token = lex();

	if (mras) {
		switch (token)
		{
		// Operator translations because of different precedence
		case '+': token = MROP_ADD; break;
		case '-': token = MROP_SUB; break;
		case '*': token = MROP_MUL; break;
		case '/': token = MROP_DIV; break;
		case '%': token = MROP_MOD; break;
		case '&': token = MROP_AND; break;
		case '|': token = MROP_OR; break;
		case '^': token = MROP_XOR; break;
		case SHL: token = MROP_SHL; break;
		case SHR: token = MROP_SHR; break;
		case LT:  token = MROP_LT; break;
		case EQ:  token = MROP_EQ; break;
		case '>': token = MROP_GT; break;
		case GE:  token = MROP_GE; break;
		case NE:  token = MROP_NE; break;
		case LE:  token = MROP_LE; break;
		case NOT: token = MROP_NOT; break;
		case HIGH: token = MROP_HIGH; break;
		case LOW: token = MROP_LOW; break;
		// Operator translations to completely different operator.
		case '<': token = MROP_SHIFT; break;
		case '!': token = MROP_OR; break;
		default: break;
		// Sadly, AND, OR, XOR and '=' can't be translated unilaterally
		// because they are also opcodes or psuedo-ops.
		}
	}

	return token;
}

int lex()
{
	int c;
	char *p;
	int radix;
	int sep;
	int exclude, include, overflow, token, endc;

	if (arg_flag) {
		yylval.cval = arg_state.arg;
		c = getarg(&arg_state);
		if (c == '\0' || c == '\n' || c == ';')
			c = skipline(c);

		return c;
	}

	if (raw == 2) {
		while (charclass[c = nextchar()] == SPACE)
			;

		*tempbuf = c == '\n' || c == '\0';

		peekc = skipline(c);

		raw = 0;

		return RAWTOKEN;
	}
	else if (raw) {
		int skip = 1;
		p = tempbuf;
		while ((c = nextchar()) != '\n' && c) {
			if (p >= tempmax) {
				*p = '\0';
				printf("was parsing '%s'\n", tempbuf);
				error(symlong);
			}
			if (!skip || charclass[c] != SPACE) {
				*p++ = c;
				skip = 0;
			}
		}
		if (c == 0)
			peekc = c;

		*p-- = '\0';

		while (p >= tempbuf && CHARCLASS(*p) == SPACE)
			*p-- = '\0';

		raw = 0;

		return RAWTOKEN;
	}

	for (;;) switch(charclass[c = nextchar()]) {
	case F_END:
		if (!expptr)
			return 0;

		if (est[MSTR].param) {
			int ch;
			est[REPNUM].value++;
			ch = est[MSTR].param[est[REPNUM].value];
			if (ch) {
				est[0].param[0] = ch;
				floc = est[MSTART].value;
				mfseek(mfile, (long)floc, 0);
				est[TEMPNUM].value = exp_number++;
			}
			else {
				free(est[MSTR].param);
				est[MSTR].param = 0;
				popsi();
			}
		}
		else if (est[REPNUM].value < 0) {
			int arg;
			do {
				switch (getarg(est[MARGP].ap)) {
				case ARG:
					arg = 1;
					break;
				case ',':
					arg = 0;
					break;
				default:
					arg = 2;
					break;
				}
			} while (!arg);

			if (arg == 1) {
				floc = est[MSTART].value;
				mfseek(mfile, (long)floc, 0);
				est[TEMPNUM].value = exp_number++;
			}
			else {
				// XXX - memory leak
				est[0].param = NULL;
				free(est[MARGP].ap);
				popsi();
			}
		}
		else if (est[REPNUM].value-- > 0) {
			floc = est[MSTART].value;
			mfseek(mfile, (long)floc, 0);
			est[TEMPNUM].value = exp_number++;
		}
		else
			popsi();

		continue;

	case SPACE:
		while (charclass[c = nextchar()] == SPACE)
			;
		peekc = c;
		logcol++;
		break;
	case LETTER:
	case STARTER:
	case DIGIT:
	case DOLLAR:
	spectok:
		firstcol = getcol() == 1;

		radix = -1; // might be a number
		p = tempbuf;
		do {
			if (p >= tempmax) {
				*tempmax = '\0';
				printf("was parsing '%s'\n", tempbuf);
				error(symlong);
			}
			*p = (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
			if (mras && *p == '?') {
				char *q;

				radix = 0; // can't be a number even if it looks like it

				if (expptr)
					q = getmraslocal();
				else
					for (q = modstr; *q == '@'; q++)
						;

				if (*q) {
					strcpy(p, q);
					p = strchr(p, '\0') - 1;
				}
				else
					*p = '?';
			}
			p++;
			c = nextchar();
		} while	(charclass[c]==LETTER || charclass[c]==DIGIT ||
			charclass[c]==STARTER || charclass[c]==DOLLAR);

		*p = '\0';

		// When parsing macro parameters we use a custom symbol table.
		// And, heck, almost anything goes.
		if (param_parse) {
			struct item *param = item_lookup(tempbuf, paramtab, PARAMTABSIZE);
			peekc = c;
			if (param->i_token) {
				sprintf(detail, "%s error.  Macro parameter '%s' repeated",
					errname[fflag], tempbuf);
				errwarn(fflag, detail);
			}

			param->i_token = MPARM;
			param->i_string = malloc(strlen(tempbuf) + 1);
			strcpy(param->i_string, tempbuf);

			yylval.itemptr = param;
			return param->i_token;
		}

		// Special case for AF'
		if (c == '\'' && strcmp(tempbuf, "af") == 0)
			return AFp;

		endc = c;
		peekc = c;

		// Pass off '?' (XXX but, technically, should only be done in expression context)
		if (strcmp(tempbuf, "?") == 0)
			return '?';

		// Pass off '$'
		if (strcmp(tempbuf, "$") == 0)
			return '$';

		// Look ahead at what we have.
		while (charclass[c] == SPACE)
			c = nextchar();

		peekc = c;

		//printf("%d %s\n", logcol, tempbuf);
		// If logcol == 0 then if c == ':' we're a label for sure.
		// If logcol == 1 if c == ':' we're a label, change logcol
		//    otherwise we're op or pseudo
		// If logcol == 0 and c == '\n' or ';' then we're alone so
		//	we give tokenization a chance otherwise label
		// If logcol >= 2 we're in the arguments
		//
		// There is quite a lot of unrealized scope for error
		// detection and helpful warnings.

		 // Default to any tokenization.
		exclude = 0;
		include = 0;

		if (logcol >= 2) {
			// Arguments allow mnemonics and psuedo-ops
			exclude = VERB;
			include = TERM;
		}
		else if (logcol <= 1 && c == ':') {
			exclude = ~TERM;
			logcol = 0;
		}
		else if (logcol == 0 && c != ';' && c != '\n') {
			exclude = ~TERM;
			include = COL0;
		}

		logcol++;

		// Look for possible numbers.
		// 0x<hex> $<hex> <hex>h <octal>o <octal>q <binary>b
		// <decimal> <decimal>d
		// Suffix formats must start with 0-9.

		if (radix)
			radix = convert(tempbuf, p, &overflow);

		// If we're in the first logical column and the token starts with
		// '$' then we'll force it to be a label even though it could be
		// a $hex constant. This will allow $FCB as a label.
		// Thus we must also allow symbol lookup a chance to override number
		// parsing if we start with a '$'.

		if (tempbuf[0] == '$') {
			if (logcol == 1 || locate(tempbuf)->i_token) {
				if (radix > 0) {
					sprintf(detail, "warning: $hex constant '%s' interpreted as symbol", tempbuf);
					errwarn(warn_hex, detail);
				}
				radix = 0;
			}
		}

		if (radix > 0) {
			// Might be line skipping time, though.
			if (*ifptr)
				return skipline(c);

			if (overflow) {
				err[iflag]++;
				yylval.ival = 0;
			}
			return NUMBER;
		}

		// Too late to do '$' concatenation of numbers.  But zmac
		// didn't allow for that previously at any rate.
		if (zcompat) {
			char *q = tempbuf;
			// Normal zmac operation requires we ignore $ in identifiers
			for (p = q; *p; p++)
				if (*p != '$')
					*q++ = *p;

			*q = '\0';
			p = q;
		}

		// GWP - boy, this should be a warning or error
		if (p - tempbuf > MAXSYMBOLSIZE) {
			p = tempbuf + MAXSYMBOLSIZE;
			*p = '\0';
		}

		token = tokenofitem(UNDECLARED, exclude, include);

		// Initial "priming" hack

		if (endc == '\'')
		{
			//printf("allowing <<%s%c>> at %d, endc=%c\n", tempbuf, peekc, logcol, endc);
			peekc = NOPEEK;
// A step towards emitting the instructions
#if 0
			// Only if auto-exx on on...
			if (tempbuf[0] == 'a') {
				emit1(8, 0, 0, ET_NOARG);
				list_out(dollarsign, "\tex\taf,af'\n", '*');
			}
			else {
				emit1(0xd9, 0, 0, ET_NOARG);
				list_out(dollarsign, "\texx\n", '*');
			}
#endif
		}

		return token;

	default:
		if (*ifptr)
			return(skipline(c));

		if (mras && getcol() == 1 && c == '*')
			goto spectok;

		switch(c) {
		int corig;
		case ':':
			if (logcol == 1) {
				// Make sure "label:ret", "label: ret",
				// "label: :ret", "label: : ret" work out OK.
				// But stop fooling around once we've done the VERB
				peekc = nextchar();
				if (charclass[peekc] == SPACE)
					logcol--;
			}
			return c;
		case ';':
			return(skipline(c));
		case '\'':
		case '"':
			sep = c;
			p = tempbuf;
			p[1] = 0;
			do	switch(c = nextchar())	{
			case '\0':
			case '\n':
				err[bflag]++;
				goto retstring;
			default:
				if (c == sep && (c = nextchar()) != sep) {
				retstring:
					peekc = c;
					*p = '\0';
					switch (p - tempbuf) {
					case 2:
						p = tempbuf;
						yylval.ival = *p++ & 255;
						yylval.ival |= (*p & 255) << 8;
						return TWOCHAR;
					case 1:
						p = tempbuf;
						yylval.ival = *p++;
						return ONECHAR;
					case 0:
						if (!full_exprs) {
							yylval.ival = 0;
							return NUMBER;
						}
						// fall through
					default:
						yylval.cval = tempbuf;
						return STRING;
					}
				}
				*p++ = c;
			} while (p < tempmax);
			/*
			 *  if we break out here, our string is longer than
			 *  our input line
			 */
			error("string buffer overflow");
		case '<':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return LE;
			case '<':
				return SHL;
			case '>':
				return NE;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '>':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return GE;
			case '>':
				return SHR;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '!':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return NE;
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '=':
			corig = c;
			switch (c = nextchar ()) {
			case '=':
				return '=';
			default:
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */

		case '&':
			corig = c;
			if ((c = nextchar()) == '&')
				return ANDAND;
			else {
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		case '|':
			corig = c;
			if ((c = nextchar()) == '|')
				return OROR;
			else {
				peekc = c;
				return corig;
			}
			/* break; suppress "unreachable" warning for tcc */
		default:
			return(c);
		}
	}
}

// Convert string to number
// 0x<hex> $<hex> <hex>h <octal>o <octal>q <binary>b
// <decimal> <decimal>d
// Suffix formats must start with 0-9.
//
// Returns:
//	-1	not numeric
//	0	numeric but digit out of range
//	>0	radix of number,  yylval.ival holds value

int convert(char *buf, char *bufend, int *overflow)
{
	int radix = -1, dummy;
	char *d0, *dn;

	if (!bufend)
		bufend = strchr(buf, '\0');

	if (!overflow)
		overflow = &dummy;

	if (buf[0] == '0' && buf[1] == 'x' && buf[2]) {
		radix = 16;
		d0 = buf + 2;
		dn = bufend;
	} else if (buf[0] == '$') {
		radix = 16;
		d0 = buf + 1;
		dn = bufend;
	}
	else if (buf[0] >= '0' && buf[0] <= '9') {
		d0 = buf;
		dn = bufend - 1;
		switch (*dn) {
		case 'o':
		case 'q':
			radix = 8;
			break;
		case 'd':
			radix = 10;
			break;
		case 'h':
			radix = 16;
			break;
		case 'b':
			radix = 2;
			break;
		default:
			radix = 10;
			dn++;
			break;
		}
	}

	// We may have a number on our hands.
	if (radix > 0) {
		*overflow = 0;
		yylval.ival = 0;

		for (; d0 < dn; d0++) {
			unsigned int ovchk = (unsigned int)yylval.ival;
			int c = *d0 - (*d0 > '9' ? ('a' - 10) : '0');
			if (c < 0 || c >= radix) {
				radix = 0;
				break;
			}
			if (ovchk * radix / radix != ovchk)
				*overflow = 1;

			yylval.ival *= radix;
			yylval.ival += c;
		}
	}

	return radix;
}

// Verify keytab is in alphabetical order.
// And that all MRASOP keywords start with '.'

int check_keytab()
{
	int i;
	char *prev;

	for (i = 0; i < sizeof(keytab) / sizeof(keytab[0]); i++) {
		char *next = keytab[i].i_string;
		next += *next == '.';
		if (i != 0) {
			if (strcmp(prev, next) >= 0) {
				printf("keytab error: %s >= %s\n", prev, next);
				return 0;
			}
		}
		prev = next;

		if ((keytab[i].i_uses & MRASOP) && keytab[i].i_string[0] != '.') {
			printf("keytab error: %s does not start with '.'\n",
				keytab[i].i_string);
			return 0;
		}
	}

	printf("keytab OK\n");

	return 1;
}


struct item *keyword(char *name)
{
	int  r, l, u;
	struct item *ip;

	/*
	 *  binary search
	 */
	l = 0;
	u = (sizeof keytab/sizeof keytab[0])-1;
	while (l <= u) {
		char *key;
		i = (l+u)/2;
		ip = &keytab[i];
		key = ip->i_string;
		r = strcmp(name + (name[0] == '.'), key + (key[0] == '.'));
		if (r == 0) {
			// Do not allow ".foo" to match "foo"
			if (name[0] == '.' && key[0] != '.')
				break;

			return ip;
		}
		if (r < 0)
			u = i-1;
		else
			l = i+1;
	}

	return 0;
}

// Find 'name' in an item table.  Returns an empty slot if not found.
// Uses case-independent comparisions which are largely wasted as
// there is only 1 case where 'name' has not already been lowercased.

struct item *item_lookup(char *name, struct item *table, int table_size)
{
	struct item *ip;
	/*
	 *  hash into item table
	 */
	int hash = 0;
	char *p = name;
	while (*p) {
		char ch = *p++;
		if (ch >= 'A' && ch <= 'Z') ch += 'a' - 'A';
		hash += ch;
	}
	hash %= table_size;
	ip = &table[hash];

	for (;;) {
		if (ip->i_token == 0)
			break;
		if (ci_strcmp(name, ip->i_string) == 0)
			break;
		if (++ip >= table + table_size)
			ip = table;
	}

	return ip;
}

struct item *locate(char *name)
{
	return item_lookup(name, itemtab, ITEMTABLESIZE);
}

// Return the longest token that matches the start of the given name.
// Currently used for MRAS which will substitute macro parameters inside
// identifiers.
struct item *item_substr_lookup(char *name, int token, struct item *table, int table_size)
{
	struct item *ip = 0;
	int i;

	for (i = 0; i < table_size; i++) {
		unsigned int len;

		if (table[i].i_token != token)
			continue;

		len = strlen(table[i].i_string);
		if (ci_strcompare(name, table[i].i_string, len) == 0) {
			if (!ip || len > strlen(ip->i_string)) {
				ip = table + i;
			}
		}
	}

	return ip;
}

/*
 *  return the token associated with the string pointed to by
 *  tempbuf.  if no token is associated with the string, associate
 *  deftoken with the string and return deftoken.
 *  in either case, cause yylval to point to the relevant
 *  symbol table entry.
 *
 *  Only keys not matching the keyexclude will be returned allowing
 *  context-dependent tokenization.  Unless they match keyinclude.
 */

int tokenofitem(int deftoken, int keyexclude, int keyinclude)
{
	struct item *ip;
	int  i;

#ifdef T_DEBUG
	fputs("'tokenofitem entry'	", stderr) ;
	fputs(tempbuf, stderr) ;
#endif

	// Allow macros to override built-ins
	// Maybe shouldn't be done for identifiers that will undergo
	// '.' and '_' expansion.
	ip = locate(tempbuf);
	if (ip->i_token == MNAME)
		goto found;

	if (full_exprs)
		keyexclude = ~TERM;

	ip = keyword(tempbuf);
	if (ip) {
		if (ip->i_uses & keyinclude)
			goto found;

		if (!(ip->i_uses & keyexclude))
			goto found;
	}

	// This is really my own thing rather than old zmac, but zmac
	// didn't support it and it does depend on '$' crushing a bit.
	if (zcompat) {
	    // '_' prefixed labels are local to the file
	    if (tempbuf[0] == '_') {
		    strcat(tempbuf, "$");
		    strcat(tempbuf, basename(src_name[now_in]));
	    }

	    // '.' prefixed labels are local between labels
	    if (tempbuf[0] == '.') {
		    char *p = tempbuf;
		    while (*p) p++;
		    sprintf(p, "$%d", llseq);
	    }
	}

	ip = locate(tempbuf);

	if (ip->i_token)
		goto found;

	if (!deftoken) {
		i = 0 ;
		goto token_done ;
	}
	if (++nitems > ITEMTABLESIZE-20)
		error("item table overflow");
	ip->i_string = malloc(strlen(tempbuf)+1);
	ip->i_token = deftoken;
	ip->i_uses = 0;
	strcpy(ip->i_string, tempbuf);

found:
	if (*ifptr) {
		if (ip->i_token == ENDIF_TK) {
			i = ENDIF_TK;
			goto token_done ;
		}
		if (ip->i_token == ELSE_TK) {
			/* We must only honour the ELSE if it is not
			   in a nested failed IF/ELSE */
			char forbid = 0;
			char *ifstackptr;
			for (ifstackptr = ifstack; ifstackptr != ifptr; ++ifstackptr) {
				if (*ifstackptr) {
					forbid = 1;
					break;
				}
			}
			if (!forbid) {
				i = ELSE_TK;
				goto token_done;
			}
		}
		if (ip->i_token == IF_TK || ip->i_token == IF_DEF_TK) {
			if (ifptr >= ifstmax)
				error("Too many ifs");
			else *++ifptr = 1;
		}
		i = skipline(' ');
		goto token_done ;
	}
	yylval.itemptr = ip;
	i = ip->i_token;
token_done:
#ifdef T_DEBUG
	fputs("\t'tokenofitem exit'\n", stderr) ;
#endif
	return(i) ;
}

void itemcpy(struct item *dst, struct item *src)
{
	if (dst && src && dst != src) {
		dst->i_string = src->i_string;
		dst->i_value = src->i_value;
		dst->i_token = src->i_token;
		dst->i_uses = src->i_uses;
		dst->i_scope = src->i_scope;
		dst->i_chain = src->i_chain;
		dst->i_pass = src->i_pass;
	}
}

/*
 *  interchange two entries in the item table -- used by custom_qsort
 */
void interchange(int i, int j)
{
	struct item temp;

	itemcpy(&temp, itemtab + i);
	itemcpy(itemtab + i, itemtab + j);
	itemcpy(itemtab + j, &temp);
}



/*
 *  quick sort -- used by compactsymtab to sort the symbol table
 */
void custom_qsort(int m, int n)
{
	int  i, j;

	if (m < n) {
		i = m;
		j = n+1;
		for (;;) {
			do i++; while(strcmp(itemtab[i].i_string,
					itemtab[m].i_string) < 0);
			do j--; while(strcmp(itemtab[j].i_string,
					itemtab[m].i_string) > 0);
			if (i < j) interchange(i, j); else break;
		}
		interchange(m, j);
		custom_qsort(m, j-1);
		custom_qsort(j+1, n);
	}
}

int getcol()
{
	return inpptr - inpbuf;
}

/*
 *  get the next character
 */
int nextchar()
{
	int c, ch;
	unsigned char *p;
	char *getlocal();

	if (peekc != NOPEEK) {
		c = peekc;
		peekc = NOPEEK;
		return c;
	}

	if (inpptr) {
		int col = getcol();

		// Double nul indicates EOF for macros
		if (expptr && inpptr[0] == '\0' && inpptr[1] == '\0') {
			inpptr = 0;
			return 0;
		}

		if (col == 0) {
			void analyze_inpbuf(void);
			void mras_operator_separate(void);

			if (!expptr)
				linein[now_in]++;

			analyze_inpbuf();

			if (mras)
				mras_operator_separate();
		}

		if (inpbuf_insert[col]) {
			c = inpbuf_insert[col];
			inpbuf_insert[col] = '\0';
		}
		else {
			c = *inpptr++;
			addtoline(c);
		}

		if (*inpptr == '\0')
			inpptr = 0;

		return c;
	}

	inpptr = inpbuf;
	logcol = 0;
	p = inpbuf;

	// XXX - should check for input line overflow!

	// If invoking a macro then pull the next line from it.
	if (expptr) {
		for (;;) {
			ch = getm();

			if (ch == '\1') { /* expand argument */
				ch = getm() - 'A';
				if (ch >= 0 && ch < PARMMAX && est[ch].param) {
					strcpy((char *)p, est[ch].param);
					p = (unsigned char *)strchr((char *)p, '\0');
				}
			}
			else if (ch == '\2') {	/*  local symbol  */
				ch = getm() - 'A';
				if (ch >= 0 && ch < PARMMAX && est[ch].param)
					strcpy((char *)p, est[ch].param);
				else
					strcpy((char *)p, getlocal(ch, est[TEMPNUM].value));

				p = (unsigned char *)strchr((char *)p, '\0');
			}
			else {
				if (ch == 0)
					break;

				*p++ = ch;

				if (ch == '\n')
					break;
			}
		}
		*p = '\0';
		p[1] = ch;
	}
	else {
		for (;;) {
			ch = nextline_peek != NOPEEK ? nextline_peek : getc(now_file);
			nextline_peek = NOPEEK;

			if (ch == '\r') {
				nextline_peek = getc(now_file);
				if (nextline_peek == '\n')
					nextline_peek = NOPEEK;

				ch = '\n';
			}

			if (ch == EOF)
				break;

			*p++ = ch;

			if (ch == '\n') 
				break;
		}

		*p = '\0';

		/* if EOF, check for include file */
		if (ch == EOF) {
			if (now_in) {
				fclose(fin[now_in]) ;
				free(src_name[now_in]);
				now_file = fin[--now_in];
				nextline_peek = linepeek[now_in];
			}
			else if (p == inpbuf)
				return 0;
	
			if (linein[now_in] < 0) {
				lstoff = 1;
				linein[now_in] = -linein[now_in];
			} else {
				lstoff = 0 ;
			}

			if (outpass) {
				if (iflist()) {
					lineout();
					fprintf(fout, "**** %s ****\n", src_name[now_in]) ;
				}
				if (fbds)
					fprintf(fbds, "%04x %04x f %s\n", dollarsign, emit_addr, src_name[now_in]);
			}

			if (p != inpbuf) {
				*p++='\n';
				*p = '\0';
			}
			else
				inpptr = 0;
		}
	}

	return nextchar();
}

char *skipspace(char *p)
{
	while (CHARCLASS(*p) == SPACE)
		p++;

	return p;
}

// Look at inpbuf and try to determine what logical column we are starting
// at.  We could put all of the work in here and keep yylex simple but for
// now we share the load.

void analyze_inpbuf(void)
{
	int cc;
	char *p, *q, save;
	char *word1, *word2;
	struct item *ip, *word1item;
	int triplecase = 1;

	// No need to do this when pulling in data for a macro.  In fact,
	// in can be harmful to undef a macro.
	if (inmlex)
		return;

	// Default if we find nothing to override
	logcol = 0;

	// One case to check is when you start with whitespace yet there are
	// 3 columns.  If so then we change logcol to -1 to compensate.
	// If the 2nd column is a VERB.

	// If we start with whitespace then we can give up on triple word case.
	p = (char *)inpbuf;
	if (CHARCLASS(*p) != SPACE)
		triplecase = 0;

	p = skipspace(p);
	word1 = p;

	// Now skip over a token or abort if we don't find one

	cc = CHARCLASS(*p);
	if (cc != LETTER && cc != STARTER && cc != DIGIT && cc != DOLLAR)
		return;

	for (;;) {
		cc = CHARCLASS(*p);
		if (cc == LETTER || cc == STARTER || cc == DIGIT || cc == DOLLAR)
			p++;
		else
			break;
	}

	// We could skip space-separated colons now, but if we see a colon
	// both issues have been decided to do that because it is easier.
	if (*p == ':')
		return;

	save = *p;
	*p = '\0';
	word1item = locate(word1);
	*p = save;

	p = skipspace(p);

	// Another token to skip past.
	// But we need to examine it to see if it is a verb.

	cc = CHARCLASS(*p);
	if (cc != LETTER && cc != STARTER && cc != DIGIT && cc != DOLLAR)
		return;

	q = p;
	word2 = p;
	for (;;) {
		cc = CHARCLASS(*p);
		if (cc == LETTER || cc == STARTER || cc == DIGIT || cc == DOLLAR)
			p++;
		else
			break;
	}

	// Now we have a second word we can check for the "name macro" case.
	// Unless we're skipping.
	save = *p;
	*p = '\0';
	if (ci_strcmp(word2, "macro") == 0 && word1item->i_token && !*ifptr)
		word1item->i_token = UNDECLARED;

	*p = save;

	if (!triplecase)
		return;

	// Must have space to skip over
	if (CHARCLASS(*p) != SPACE)
		return;

	// This 2nd token must be a verb.
	cc = *p;
	*p = '\0';
	ip = keyword(q);
	*p = cc;
	if (!ip || !(ip->i_uses & VERB))
		return;

	// Now skip over space.  If there's anything but a comment or end
	// of the line then we've may have 3 logical columns.
	// "ld a, 5" can throw that off, but we've done the verb check.

	p = skipspace(p);

	if (*p != ';' && *p != '\n' && *p != '\0')
		logcol--;
}

void mras_operator_separate(void)
{
	int i, sep;

	// Only do operator separation after macro expansion.
	if (inmlex)
		return;

	// Apply operator separation across the entire line.
	// We could limit this to logical columns if need be,
	// but I imagine MRAS didn't allow "x.mod.y" as a label
	// or macro name.

	// Strings are handled in a simple but largly compatible way.
	// AF' is a problem which can be safely ignored since its appearance
	// will mean no expression is present.
	// My allowed salting of ' after identifiers is another matter.
	// But, again, original MRAS code won't do that.

	memset(inpbuf_insert, 0, strlen((char *)inpbuf));

	sep = '\0';
	for (i = 0; inpbuf[i]; i++) {
		int j;

		if (inpbuf[i] == sep) {
			sep = '\0';
			continue;
		}

		if (inpbuf[i] == '\'' || inpbuf[i] == '"')
			sep = inpbuf[i];

		// Don't do anthing if inside a string constant.
		if (sep)
			continue;

		if (inpbuf[i] == ';')
			break;

		// Slight nod to efficiency.  Assumption is tested in
		// check_keytab().

		if (inpbuf[i] != '.')
			continue;

		for (j = 0; j < sizeof(keytab) / sizeof(keytab[0]); j++) {
			char *p = keytab[j].i_string;
			int len = strlen(p);

			if (!(keytab[j].i_uses & MRASOP))
				continue;

			if (ci_strcompare((char *)inpbuf + i, p, len) == 0) {
				// Only need to add spaces if we're possibly
				// in an identifier.  But no harm if we're not.
				inpbuf_insert[i] = ' ';
				inpbuf_insert[i + len] = ' ';
				i += len - 1;
				break;
			}
		}
	}
}


/*
 *  skip to rest of the line -- comments and if skipped lines
 */
int skipline(int ac)
{
	int  c;

	c = ac;
	while (c != '\n' && c != '\0')
		c = nextchar();
	return('\n');
}

void add_incpath(char *dir)
{
	if (incpath_cnt >= MAXINCPATH) {
		fprintf(stderr, "Sorry, can only handle %d include paths\n", MAXINCPATH);
		exit(1);
	}

	incpath[incpath_cnt++] = strdup(dir);
}

FILE *open_incpath(char *filename, char *mode, char **path_used)
{
	char quote;
	int i;
	char path[1024];
	FILE *fp;

	// Due to the way parsing works the string can be specified
	// without quotes or will allow quotes but include them.  Instead
	// of fooling with the parsing I just strip the quotes.  I think
	// you can still include a file that starts with a single or double
	// quote by quoting it, but that's an awful thing to do to yourself.

	quote = *filename;
	if (quote == '"' || quote == '\'') {
		char *p;
		for (p = filename; *p; p++)
			p[0] = p[1];

		if (p[-2] == quote)
			p[-2] = '\0';
	}

	// First look for included file in same directory as source file.

	strcpy(path, src_name[now_in]);
	*basename(path) = '\0';
	strcat(path, filename);
	fp = fopen(path, mode);
	if (fp) {
		if (path_used)
			*path_used = strdup(path);

		if (note_depend && outpass)
			printf("%s\n", path);
		return fp;
	}

	for (i = 0; i < incpath_cnt; i++) {
		sprintf(path, "%s/%s", incpath[i], filename);
		fp = fopen(path, mode);
		if (fp) {
			if (path_used)
				*path_used = strdup(path);
			if (note_depend && outpass)
				printf("%s\n", path);
			return fp;
		}
	}

	if (note_depend && outpass)
		printf("%s\n", filename);

	fp = fopen(filename, mode);
	if (fp && path_used)
		*path_used = strdup(filename);

	return fp;
}

void version()
{
	fprintf(stderr, "zmac version " VERSION "        http://48k.ca/zmac.html\n");
}

//
// Print out a usage message and exit.
//
void usage(char *msg, char *param)
{
	fprintf(stderr, msg, param);
	fprintf(stderr, "\n");
	version();
	fprintf(stderr, "usage: zmac [-8bcefghijJlLmnopstz] [-I dir] [-Pk=n] file[.z]\n");
	fprintf(stderr, "other opts: --rel[7] --mras --zmac --dep --help --doc --version\n");
	fprintf(stderr, "  zmac -h for more detail about options.\n");
	exit(1);
}

void help()
{
	version();

	fprintf(stderr, "   --version\tshow version number\n");
	fprintf(stderr, "   --help\tshow this help message\n");
	fprintf(stderr, "   -8\t\tuse 8080 interpretation of mnemonics\n");
	fprintf(stderr, "   -b\t\tno binary (.hex,.cmd,.cas, etc.) output\n");
	fprintf(stderr, "   -c\t\tno cycle counts in listing\n");
	fprintf(stderr, "   -e\t\terror list only\n");
	fprintf(stderr, "   -f\t\tprint if skipped lines\n");
	fprintf(stderr, "   -g\t\tdo not list extra code\n");
	fprintf(stderr, "   -h\t\tshow this information about options and quit\n");
	fprintf(stderr, "   -i\t\tdo not list include files\n");
	fprintf(stderr, "   -I dir\tadd 'dir' to include file search path\n");
	fprintf(stderr, "   -j\t\tpromote relative jumps to absolute as needed\n");
	fprintf(stderr, "   -J\t\twarn when a jump could be relative\n");
	fprintf(stderr, "   -l\t\tlist to standard output\n");
	fprintf(stderr, "   -L\t\tforce listing of everything\n");
	fprintf(stderr, "   -m\t\tprint macro expansions\n");
	fprintf(stderr, "   -n\t\tput line numbers off\n");
	fprintf(stderr, "   -o file.hex\toutput only named file (multiple -o allowed)\n");
	fprintf(stderr, "   -p\t\tput out four \\n's for eject\n");
	fprintf(stderr, "   -P\t\tformat listing for a printer\n");
	fprintf(stderr, "   -s\t\tdon't produce a symbol list\n");
	fprintf(stderr, "   -t\t\toutput error count instead of list of errors\n");
	fprintf(stderr, "   -z\t\tuse Z-80 interpretation of mnemonics\n");
	fprintf(stderr, "   -Pk=num\tset @@k to num before assembly (e.g., -P4=10)\n");
	fprintf(stderr, "   --od\tdir\tdirectory unnamed output files (default \"zout\")\n");
	fprintf(stderr, "   --oo\thex,cmd\toutput only listed file suffix types\n");
	fprintf(stderr, "   --xo\tlst,cas\tdo not output listed file suffix types\n");
	fprintf(stderr, "   --dep\tlist files included\n");
	fprintf(stderr, "   --mras\tlimited MRAS/EDAS compatibility\n");
	fprintf(stderr, "   --rel\toutput .rel file only (--rel7 for 7 character symbol names)\n");
	fprintf(stderr, "   --zmac\tcompatibility with original zmac\n");
	fprintf(stderr, "   --doc\toutput documentation as HTML file\n");

	exit(0);
}

int main(int argc, char *argv[])
{
	struct item *ip;
	int  i, j;
	int  files;
	int used_o;
	int used_oo;
#ifdef DBUG
	extern  yydebug;
#endif

	fin[0] = stdin;
	now_file = stdin;
	files = 0;
	used_o = 0;
	used_oo = 0;

	// Special flag for unit testing.
	if (argc > 1 && strcmp(argv[1], "--test") == 0)
		exit(!check_keytab());

	for (i=1; i<argc; i++) {
		int skip = 0;
		if (strcmp(argv[i], "--mras") == 0) {
			mras = 1;
			trueval = -1;
			continue;
		}

		if (strcmp(argv[i], "--rel") == 0) {
			relsymlen = 6;
			continue;
		}

		if (strcmp(argv[i], "--rel7") == 0) {
			relsymlen = 7;
			continue;
		}

		if (strcmp(argv[i], "--zmac") == 0) {
			zcompat = 1;
			continue;
		}

		if (strcmp(argv[i], "--dep") == 0) {
			note_depend = 1;
			continue;
		}

		if (strcmp(argv[i], "--help") == 0) {
			help();
			continue;
		}

		if (strcmp(argv[i], "--doc") == 0) {
			help();
			#if 0 /* disabled by dg */
			extern void doc(void);
			doc();
			exit(0);
			#endif
			continue; // not reached
		}

		if (strcmp(argv[i], "--version") == 0) {
			version();
			exit(0);
			continue; // not reached
		}

		if (strcmp(argv[i], "--od") == 0) {
			output_dir = argv[i = getoptarg(argc, argv, i)];
			continue;
		}

		if (strcmp(argv[i], "--oo") == 0) {
			if (!used_oo)
				stop_all_outf();

			suffix_list(argv[i = getoptarg(argc, argv, i)], 0);

			used_oo = 1;
			continue;
		}

		if (strcmp(argv[i], "--xo") == 0) {
			suffix_list(argv[i = getoptarg(argc, argv, i)], 1);
			continue;
		}

		if (argv[i][0] == '-' && argv[i][1] == 'P' &&
			argv[i][2] >= '0' && argv[i][2] <= '9')
		{
			if (argv[i][3] == '=') {
				int overflow;
				int radix;
				int sign = 1;
				char *str = argv[i] + 4;
				if (*str == '-') {
					sign = -1;
					str++;
				}

				radix = convert(str, NULL, &overflow);
				if (radix <= 0 || overflow)
					usage("bad -Pn= parameter value", 0);

				mras_param[argv[i][2] - '0'] = sign * yylval.ival;
			}
			else if (argv[i][3] == '\0')
				mras_param[argv[i][2] - '0'] = -1;
			else
				usage("-Pn syntax error", 0);

			continue;
		}

		if (*argv[i] == '-') while (*++argv[i]) {
			switch(*argv[i]) {

			case '8':	/* Equivalent to .8080 */
				default_z80 = 0;
				continue;

			case 'b':	/*  no binary  */
				for (j = 0; j < CNT_OUTF; j++)
					if (strcmp(outf[j].suffix, "lst") != 0)
						outf[j].no_open = 1;
				continue;

			case 'c':	/*  no cycle counts in listing */
				copt-- ;
				continue;

#ifdef DBUG
			case 'd':	/*  debug  */
				yydebug++;
				continue;
#endif

			case 'e':	/*  error list only  */
				eopt = 0;
				edef = 0;
				continue;

			case 'f':	/*  print if skipped lines  */
				fopt++;
				fdef++;
				continue;

			case 'g':	/*  do not list extra code  */
				gopt = 0;
				gdef = 0;
				continue;

			case 'h':
				help();
				continue;

			case 'i':	/* do not list include files */
				iopt = 1 ;
				continue ;

			case 'I':
				if (argv[i][1])
					add_incpath(argv[i] + 1);
				else
					add_incpath(argv[i = getoptarg(argc, argv, i)]);
				skip = 1;
				break;

			case 'l':	/*  list to stdout */
				fout = stdout;
				continue;

			case 'L':	/*  force listing of everything */
				lston++;
				continue;

			case 'j':	// promote relative jumps to absolute as needed
				default_jopt = 1;
				continue;

			case 'J':	// error when JR instructions could replace JP
				default_JPopt = 1;
				continue;

			case 'm':	/*  print macro expansions  */
				mdef++;
				mopt++;
				continue;

			case 'n':	/*  put line numbers off */
				nopt-- ;
				continue;

			case 'o':	/*  select output */
				{
					char *outfile = 0;
					char *sfx;
					int found = 0;

					if (!used_o)
						stop_all_outf();

					if (argv[i][1])
						outfile = argv[i] + 1;
					else
						outfile = argv[i = getoptarg(argc, argv, i)];

					for (sfx = outfile; !found && *sfx; sfx++) {
						if (*sfx != '.')
							continue;

						for (j = 0; j < CNT_OUTF; j++) {
							if (ci_strcmp(sfx + 1, outf[j].suffix) == 0) {
								outf[j].no_open = 0;
								outf[j].wanted = 1;
								outf[j].filename = outfile;
								found = 1;
								break;
							}
						}
					}
					if (!found)
						usage("output file '%s' has unknown suffix", outfile);
				}
				used_o = 1;
				skip = 1;
				break;

			case 'p':	/*  put out four \n's for eject */
				popt-- ;
				continue;

			case 'P':	// GWP - printer style output (headers, page separation, etc.)
				printer_output = 1;
				continue;

			case 's':	/*  don't produce a symbol list  */
				sopt++;
				continue;

			case 't':	/*  output only number of errors */
				topt = 0;
				continue;

			case 'z':	/* Equivalent to .z80 */
				default_z80 = 1;
				continue;

			default:	/*  error  */
				usage("Unknown option", 0);

			}
			if (skip)
				break;
		}
		else if (files++ == 0) {
			sourcef = argv[i];
			strcpy(src, sourcef);
			if ((now_file = fopen(src, "r")) == NULL) {
				if (!*getsuffix(src))
					suffix(src, ".z");
				if ((now_file = fopen(src, "r")) == NULL)
					usage("Cannot open source file '%s'", src);
			}
			now_in = 0;
			fin[now_in] = now_file ;
			src_name[now_in] = src ;
		} else if (files)
			usage("Too many arguments", 0);
	}


	if (files == 0)
		usage("No source file", 0);

	// .wav file outputs must ensure their .cas antecedents are generated.
	// And also check for .rel output and set relopt while we're at it.
	for (j = 0; j < CNT_OUTF; j++) {
		char *p;

		if (strcmp(outf[j].suffix, "rel") == 0 && !outf[j].no_open)
			relopt = 1;

		p = strchr(outf[j].suffix, '.');
		// Only .wav file that open matter and only if .cas doesn't open.
		if (!p || strcmp(p, ".wav") != 0 || outf[j].no_open || !outf[j + 1].no_open)
			continue;

		outf[j + 1].no_open = 0;
		outf[j + 1].temp = 1;
		if (outf[j].filename) {
			// Replace .wav suffix with .cas.  This is safe for
			// now as only -o can choose a filename and it must end
			// with outf[j]'s suffix to be put in outf[j].
			outf[j + 1].filename = strdup(outf[j].filename);
			p = strrchr(outf[j + 1].filename, '.');
			strcpy(p + 1, "cas");
		}
	}

	if (relopt) {
		for (j = 0; j < CNT_OUTF; j++) {
			if (strcmp(outf[j].suffix, "lst") != 0)
			{
				outf[j].no_open = strcmp(outf[j].suffix, "rel") != 0;
			}
		}
	}

	for (j = 0; j < CNT_OUTF; j++) {
		if (outf[j].no_open || *outf[j].fpp)
			continue;

		if (!outf[j].filename) {
			char suffix[32];
			strcpy(suffix, ".");
			strcat(suffix, outf[j].suffix);
			outpath(oth, sourcef, suffix);
			outf[j].filename = strdup(oth);
		}

		*outf[j].fpp = fopen(outf[j].filename, outf[j].mode);
		if (!*outf[j].fpp) {
			fprintf(stderr, "Cannot create file '%s'", outf[j].filename);
			clean_outf();
			exit(1);
		}
	}

	if (fbds) {
		fprintf(fbds, "binary-debuggable-source\n");
		fprintf(fbds, "%04x %04x f %s\n", dollarsign, emit_addr, src_name[now_in]);
	}

	// Tape header
	for (i = 0; i < 255; i++) {
		if (flcas) fputc(0, flcas);
		if (flnwcas) fputc(0, flnwcas);
		if (fcas) fputc(0x55, fcas);
		if (ftcas) fputc(0, ftcas);
	}
	if (flcas) fputc(0xA5, flcas);
	if (flnwcas) fputc(0xA5, flnwcas);
	if (fcas) fputc(0x7F, fcas);
	if (ftcas) fputc(0xA5, ftcas);

	casname(oth, sourcef, 6);
	putcas(0x55);
	for (i = 0; i < 6; i++)
		putcas(oth[i]);

	if (relopt) {
		strncpy(progname, basename(sourcef), sizeof progname);
		progname[sizeof progname - 1] = '\0';
	}

	// mfopen() is always in-memory not a temporary file.

	mfile = mfopen("does-not-matter","w+b") ;

	/*
	 *  get the time
	 */
	time(&now);
	timp = ctime(&now);
	timp[16] = 0;
	timp[24] = 0;

	title = sourcef;
	/*
	 * pass 1
	 */
#ifdef DEBUG
	fputs("DEBUG-pass 1\n", stderr) ;
#endif
	clear();
	setvars();
	npass = 1;
	outpass = 0;
	yyparse();

	// GWP - errors should stop us, but the listing is very useful.

	pass2++;

	for (npass = 2; npass < MAXPASS; npass++) {
		if (passfail || npass == MAXPASS - 1)
			outpass = 1;

		if (outpass) {
			putrelcmd(RELCMD_PROGNAME);
			putrelname(progname);
		}

		for (ip = itemtab - 1; ++ip < itemmax; ) {
			// Output list of public labels.  m80 will let
			// equates and aseg values be public so we do, too.
			if (outpass && ip->i_token && (ip->i_scope & SCOPE_PUBLIC)) {
				putrelcmd(RELCMD_PUBLIC);
				putrelname(ip->i_string);
			}

			/* reset use count */
			ip->i_uses = 0 ;

			/* set macro names, equated and defined names */
			switch	(ip->i_token) {
			case MNAME:
				ip->i_token = UNDECLARED;
				break;

			case EQUATED:
				ip->i_token = WASEQUATED;
				break;

			case DEFLED:
				if (zcompat)
					ip->i_token = UNDECLARED;
				break;
			}
		}

		if (outpass) {
			// m80 outputs data size as an absolute value, but
			// code size as code segment relative.  Odd, but
			// I'll follow suit.
			putrelcmd(RELCMD_DATASIZE);
			putrelsegref(SEG_ABS, seg_size[SEG_DATA]);

			putrelcmd(RELCMD_CODESIZE);
			putrelsegref(SEG_CODE, seg_size[SEG_CODE]);
		}

		for (ip = itemtab - 1; ++ip < itemmax; ) {
			if (ip->i_token != COMMON)
				continue;

			// TODO: perhaps have WASCOMMON but this will suffice
			ip->i_token = UNDECLARED;

			putrelcmd(RELCMD_COMSIZE);
			putrelsegref(SEG_ABS, ip->i_value);
			putrelname(ip->i_string);
		}

		// In case we hit 'end' inside an included file
		while (now_in > 0) {
			fclose(fin[now_in]);
			free(src_name[now_in]);
			now_file = fin[--now_in];
			nextline_peek = linepeek[now_in];
		}
		setvars();
		fseek(now_file, (long)0, 0);

	#ifdef DEBUG
		fprintf(stderr, "DEBUG- pass %d\n", npass) ;
	#endif

		yyparse();

		if (outpass || passfail)
			break;

		if (!passretry)
			outpass = 1;
	}

	flushbin();
	flushoth();

	if (fbuf)
		putc(':', fbuf);

	if (xeq_flag) {
		if (fbuf) {
			puthex(0, fbuf);
			puthex(xeq >> 8, fbuf);
			puthex(xeq, fbuf);
			puthex(1, fbuf);
			puthex(255-(xeq >> 8)-xeq, fbuf);
		}
		if (fcmd) {
			fprintf(fcmd, "%c%c%c%c", 2, 2, xeq, xeq >> 8);
			fflush(fcmd);
		}
		putcas(0x78);
		putcas(xeq);
		putcas(xeq >> 8);
		if (fmds)
			fprintf(fmds, "pc=$%04x\ng\n", xeq);
	}
	else {
		// SYSTEM cassette files must have an execution address.
		// Without one we simply do not output .cas or .wav SYSTEM.

		int i;
		for (i = 0; i < CNT_OUTF; i++) {
			if (*outf[i].fpp && outf[i].system) {
				fclose(*outf[i].fpp);
				*outf[i].fpp = NULL;
				unlink(outf[i].filename);
				if (outf[i].wanted)
					fprintf(stderr, "Warning: %s not output -- no entry address (forgot \"end label\")\n", outf[i].filename);
			}
		}

		if (fbuf) {
			for (i = 0; i < 10; i++)
				putc('0', fbuf);
		}
	}

	if (fbuf) {
		putc('\n', fbuf);
		fflush(fbuf);
	}

	// "Play Cas" seems to require trailing zeros to work
	// properly.  And we need to output at least one zero byte
	// to flush out the final high speed bits.
	#define CAS_PAD 6
	for (i = 0; i < CAS_PAD; i++)
		putcas(0);

	if (relopt) {
		struct item *ip;
		// Output external symbols and value of public symbols
		for (ip = itemtab; ip < itemmax; ip++) {
			if (ip->i_token == UNDECLARED && (ip->i_scope & SCOPE_EXTERNAL)) {
				putrelcmd(RELCMD_EXTCHAIN);
				// Chain value will have top two bits set appropriately
				putrelextaddr(ip->i_chain);
				putrelname(ip->i_string);
			}
			if (ip->i_scope & SCOPE_PUBLIC)
			{
				putrelcmd(RELCMD_PUBVALUE);
				putrelsegref(ip->i_scope, ip->i_value);
				putrelname(ip->i_string);
			}
		}

		// End module, entry address if any
		putrelcmd(RELCMD_ENDMOD);
		putrelextaddr(rel_main);
		flushrel(); // byte alignment expected after end module

		// End .rel file
		putrelcmd(RELCMD_ENDPROG);
		flushrel();
	}

	if (xeq_flag == 0) {
#if WIN32
		char *username = getenv("USERNAME");
		if (username && strcmp(username, "George") == 0 && !relopt) {
			CONSOLE_SCREEN_BUFFER_INFO inf;
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(hOut, &inf);
			SetConsoleTextAttribute(hOut, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
			fprintf(stderr, "Warning: no entry address (forgot \"end label\")\n");
			fflush(stderr);
			SetConsoleTextAttribute(hOut, inf.wAttributes);
		}
#endif
	}
	else if (fbds) {
		fprintf(fbds, "%04x e\n", xeq);
	}

	if (fcim || fams || ftap || ftcas) {
		int low = 0;
		int high = sizeof(memory) - 1;
		int chk;
		int filelen;
		int i;
		char leafname[] = "FILENAMEBIN";

		while (low < sizeof(memory) && (memflag[low] & (MEM_INST | MEM_DATA)) == 0)
			low++;

		while (high >= 0 && (memflag[high] & (MEM_INST | MEM_DATA)) == 0)
			high--;

		if (high >= low && fcim)
			fwrite(memory + low, high + 1 - low, 1, fcim);

		filelen = (high + 1) - low;

		// AMSDOS binary file output (A for Amstrad, code from zmac 1.3)
		if (fams) {
			chk = 0;
			putc(0, fams);
			for (i = 0; i < 11; i++) {
				putc(leafname[i], fams);
				chk += leafname[i];
			}
			for (i = 0; i < 6; i++)
				putc(0, fams);

			putc(2, fams); // Unprotected binary
			chk += 2;
			putc(0, fams);
			putc(0, fams);
			putc(low & 0xff, fams);
			chk += low & 0xff;
			putc(low >> 8, fams);
			chk += low >> 8;
			putc(0, fams);
			putc(filelen & 0xff, fams);
			chk += filelen & 0xff;
			putc(filelen >> 8, fams);
			chk += filelen >> 8;
			putc(xeq & 0xff, fams);
			chk += xeq & 0xff;
			putc(xeq >> 8, fams);
			chk += xeq >> 8;
			for (i = 28; i < 64; i++)
				putc(0, fams);

			putc(filelen & 0xff, fams);
			chk += filelen & 0xff;
			putc(filelen >> 8, fams);
			chk += filelen >> 8;
			putc(0, fams); // this would be used if filelen > 64K
			putc(chk & 0xff, fams);
			putc(chk >> 8, fams);

			for (i = 69; i < 128; i++)
				putc(0, fams);

			if (filelen > 0)
				fwrite(memory + low, filelen, 1, fams);

			if (filelen & 0x7f)
				putc(0x1a, fams); // CP/M EOF character
		}

		if (ftap)
			write_tap(filelen, low, memory + low);
 
		if (ftcas)
			write_250(low, high);
	}

	// Output .wav files noting the padding bytes to ignore.
	writewavs(0, CAS_PAD, CAS_PAD);

	if (fbds) {
		struct item *tp;

		for (tp = itemtab; tp < itemmax; tp++) {
			if (tp->i_token == LABEL)
				fprintf(fbds, "%04x a %s\n", tp->i_value, tp->i_string);
			else if (tp->i_token == EQUATED)
				fprintf(fbds, "%04x v %s\n", tp->i_value, tp->i_string);
		}
	}

	if (fout)
		fflush(fout);
	if (writesyms)
		outsymtab(writesyms);
	compactsymtab();
	if (eopt)
		erreport();
	if (!sopt)
		putsymtab();
	if (fout) {
		eject();
		fflush(fout);
	}
	// GWP - some things (like balance errors in macro definitions) do
	// not show up until you use them.  So just in case we print an error
	// count here as not to confuse the programmer who is unlikely to check
	// the listing for errors if none are shown in the command window.
	if (counterr() > 0)
		fprintf(stderr, "%d errors (see listing if no diagnostics appeared here)\n", counterr());
	if (countwarn() > 0)
		fprintf(stderr, "%d warnings (see listing if no diagnostics appeared here)\n", countwarn());

	clean_outf_temp();
	if (counterr() > 0)
		clean_outf();
	exit(counterr() > 0);
}

int getoptarg(int argc, char *argv[], int i)
{
	i++;
	if (i < argc)
		return i;

	usage("missing argument for %s option", argv[i - 1]);
	return i; // not actually reached
}

void stop_all_outf()
{
	int i;
	for (i = 0; i < CNT_OUTF; i++)
		outf[i].no_open = 1;
}

void clean_outf()
{
	int i;

	for (i = 0; i < CNT_OUTF; i++) {
		if (*outf[i].fpp) {
			int size = ftell(*outf[i].fpp);
			fclose(*outf[i].fpp);
			*outf[i].fpp = NULL;
			// Listing file can stick around, but not if empty.
			if (strcmp(outf[i].suffix, "lst") != 0 || size == 0)
				unlink(outf[i].filename);
		}
	}
}

void clean_outf_temp()
{
	int i;
	for (i = 0; i < CNT_OUTF; i++) {
		if (*outf[i].fpp && outf[i].temp) {
			fclose(*outf[i].fpp);
			*outf[i].fpp = NULL;
			unlink(outf[i].filename);
		}
	}
}

// Set output files to open or not using a comma-separated list of suffixes.
// Loops over the outf[] suffix so that "wav" can be used to eliminate all
// .wav files.
void suffix_list(char *sfx_lst, int no_open)
{
	while (sfx_lst) {
		int i;
		char *p = strchr(sfx_lst, ',');
		if (p)
			*p = '\0';

		// Allow prefix '.' in case user puts them in.
		while (*sfx_lst == '.')
			sfx_lst++;

		for (i = 0; i < CNT_OUTF; i++) {
			char *sfx;
			for (sfx = outf[i].suffix; sfx; ) {
				if (ci_strcmp(sfx, sfx_lst) == 0) {
					outf[i].no_open = no_open;
					if (!no_open)
						outf[i].wanted = 1;
				}
				sfx = strchr(sfx, '.');
				if (sfx)
					sfx++;
			}
		}

		sfx_lst = p ? p + 1 : 0;
	}
}

void equate(char *id, int value)
{
	struct item *it;

	it = locate(id);
	if (!it->i_token) {
		nitems++;
		it->i_value = value;
		it->i_token = EQUATED;
		it->i_pass = npass;
		it->i_scope = SCOPE_BUILTIN;
		it->i_uses = 0;
		it->i_string = malloc(strlen(id)+1);
		strcpy(it->i_string, id);
	}
}

/*
 *  set some data values before each pass
 */
void setvars()
{
	int  i;

	peekc = NOPEEK;
	inpptr = 0;
	nextline_peek = NOPEEK;
	raw = 0;
	linein[now_in] = linecnt = 0;
	exp_number = 0;
	emitptr = emitbuf;
	lineptr = linebuf;
	ifptr = ifstack;
	*ifptr = 0;
	dollarsign = 0;
	emit_addr = 0;
	olddollar = 0;
	oldothdollar = 0;
	phaseflag = 0;
	for (i=0; i<FLAGS; i++) err[i] = 0;
	tstates = 0;
	ocf = 0;
	llseq = 0;
	passfail = 0;
	passretry = 0;
	njrpromo = 0;
	jopt = default_jopt;
	JPopt = default_JPopt;
	strcpy(modstr, "@@@@");
	segment = SEG_CODE;
	cur_common = NULL;
	memset(seg_pos, 0, sizeof(seg_pos));
	memset(seg_size, 0, sizeof(seg_size));
	segchange = 0;
	z80 = default_z80;
	outrec = 0;

	param_parse = 0;
	arg_reset();

	mfptr = 0;
	mfseek(mfile, mfptr, 0);

	// These are slighly harmful, but not too bad.  Only
	// absolutely necessary for MAC compatibility.  But there's
	// some use in having them available always.

	equate("b", 0);
	equate("c", 1);
	equate("d", 2);
	equate("e", 3);
	equate("h", 4);
	equate("l", 5);
	equate("m", 6);
	equate("a", 7);

	equate("sp", 6);
	equate("psw", 6);

	// There are a large number of symbols to add to support
	// "LXI H,MOV" and the like.

	// Technically only MRAS has these, but I'll wait and see if
	// anyone actually uses @@0 through @@9.  If so, then we can
	// DEFL them instead of EQU.
	for (i = 0; i < 10; i++) {
		char var[4];
		var[0] = '@';
		var[1] = '@';
		var[2] = '0' + i;
		var[3] = '\0';
		equate(var, mras_param[i]);
	}

	reset_import();
}

//
// Clear out cycle counts and memory.
//

void clear()
{
	int i;

	for (i = 0; i < sizeof(memory) / sizeof(memory[0]); i++)
	{
		memory[i] = 0;
		memflag[i] = 0;
		tstatesum[i] = 0;
	}
}

void setmem(int addr, int value, int type)
{
	value &= 0xff;
	memory[addr] = value;
	memflag[addr] |= type;
}

/*
 *  print out an error message and die
 */
void error(char *as)
{
	*linemax = 0;
	if (fout) {
		fprintf(fout, "%s\n", linebuf);
		fflush(fout);
	}
	fprintf(stderr, "%s\n", as) ;
	clean_outf();
	exit(1);
}


/*
 * Compact the symbol table, removing unused, UNDECLARED entries,
 * macros and built-in identifiers.
 */
void compactsymtab()
{
	struct item *tp, *fp;

	if (!nitems)
		return;

	tp = itemtab;
	tp--;
	for (fp = itemtab; fp<itemmax; fp++) {
		if (fp->i_token == UNDECLARED && !(fp->i_scope & SCOPE_EXTERNAL)) {
			nitems--;
			continue;
		}
		if (fp->i_token == 0)
			continue;

		// Don't list macros or internally defined symbols
		if (fp->i_token == MNAME || (fp->i_scope & SCOPE_BUILTIN)) {
			nitems--;
			continue;
		}

		tp++;
		itemcpy(tp, fp);
	}

	tp++;
	tp->i_string = "{";	/* } */

	/*  sort the table */
	custom_qsort(0, nitems-1);
}

/*
 *  output the symbol table
 */
void putsymtab()
{
	int  i, j, k, t, rows;
	char c, c1, seg = ' ';
	int numcol = printer_output ? 4 : 1;
	struct item *tp;

	if (!nitems || !fout)
		return;

	title = "**  Symbol Table  **";

	rows = (nitems+numcol-1) / numcol;
	if (rows+5+line > 60)
		eject();
	lineout();
	fprintf(fout,"\n\n\nSymbol Table:\n\n") ;
	line += 4;

	for (i=0; i<rows; i++) {
		for(j=0; j<numcol; j++) {
			k = rows*j+i;
			if (k < nitems) {
				tp = &itemtab[k];
				t = tp->i_token;
				c = ' ' ;
				if (t == EQUATED || t == DEFLED)
					c = '=' ;
				if (t == COMMON)
					c = '/';
				if (tp->i_uses == 0)
					c1 = '+' ;
				else
					c1 = ' ' ;

				// GWP - decided I don't care about uses
				// even if it were accurate.
				// TODO: Should use maxsymbol size in there,
				// but makes output harder to read.

				fprintf(fout, "%-15s%c", tp->i_string, c);

				if (relopt)
					seg = SEGCHAR(tp->i_scope & SCOPE_SEGMASK);

				if (tp->i_value >> 16)
					fprintf(fout, "%8x%c", tp->i_value, seg);
				else
					fprintf(fout, "%4x%c    ", tp->i_value & 0xffff, seg);

				if (tp->i_scope & SCOPE_EXTERNAL)
					fprintf(fout, " (extern)");

				if (tp->i_scope & SCOPE_PUBLIC)
					fprintf(fout, " (public)");
			}
		}
		lineout();
		putc('\n', fout);
	}
}




/*
 *  put out error report
 */
void erreport()
{
	int i, numerr, numwarn;

	if (!fout)
		return;

	if (line > 49) eject();
	lineout();
	numerr = 0;
	for (i=0; i<FIRSTWARN; i++) numerr += keeperr[i];
	numwarn = 0;
	for (i = FIRSTWARN; i < FLAGS; i++) numwarn += keeperr[i];
	if (numerr || numwarn) {
		fputs("\n\n\nError + Warning report:\n\n", fout);
		fprintf(fout, "%6d errors\n", numerr);
		fprintf(fout, "%6d warnings\n", numwarn);
		line += 6;
	} else {
		fputs("\n\n\nStatistics:\n", fout);
		line += 3;
	}

	for (i=0; i<FLAGS; i++)
		if (keeperr[i]) {
			lineout();
			fprintf(fout, "%6d %c -- %s %s\n",
				keeperr[i], errlet[i], errname[i],
				i < FIRSTWARN ? "error" : "warnings");
		}

	if (line > 52) eject();
	lineout();
	fprintf(fout, "\n%6d\tpasses\n", npass);
	fprintf(fout, "%6d\tjr promotions\n", njrpromo);
	fprintf(fout, "%6d\tsymbols\n", nitems);
	fprintf(fout, "%6d\tbytes\n", nbytes);
	line += 4;
	if (mfptr) {
		if (line > 53) eject();
		lineout();
		fprintf(fout, "\n%6d\tmacro calls\n", exp_number);
		fprintf(fout, "%6d\tmacro bytes\n", mfptr);
		fprintf(fout, "%6d\tinvented symbols\n", invented/2);
		line += 3;
	}
}

/*
 * count errors (GWP - added to set exit code)
 */
int counterr()
{
	int i, numerr = 0;
	for (i=0; i<FIRSTWARN; i++) numerr += keeperr[i];
	return numerr;
}

// Count warnings
int countwarn()
{
	int i, numwarn = 0;
	for (i = FIRSTWARN; i < FLAGS; i++)
		numwarn += keeperr[i];
	return numwarn;
}

char *mlook;

int nextmac()
{
	int ch;

	if (mlook) {
		if (*mlook) {
			ch = *mlook++;
			addtoline(ch);
		}
		else
			mlook = 0;
	}

	if (!mlook)
		ch = nextchar();

	return ch;
}

void putm_param_ref(struct item *param);
void putm_str(char *str, int look_for_param);

/*
 *  lexical analyser for macro definition
 */
void mlex(char *look)
{
	char  *p;
	int  c;
	int  t;
	int quote = 0, ampdrop = 0;
	struct item *param, *key;
	char symbuf[TEMPBUFSIZE];
	char *symbufmax = symbuf+TEMPBUFSIZE-1;

	/*
	 *  move text onto macro file, changing formal parameters
	 */
#ifdef	M_DEBUG
	fprintf(stderr,"enter 'mlex' at %d\n", mfptr) ;
#endif
	inmlex++;

	mlook = look;

	c = nextmac();
	for (;;) {
		int octo = 0, amp = 0, caret = 0;

		if (c == '#' || c == '&' || c == '^') {
			int esc = c;
			c = nextmac();
			if (charclass[c] != STARTER && charclass[c] != LETTER) {
				if (esc != '&' || !ampdrop)
					putm(esc);
				ampdrop = 0;
				continue;
			}
			if (esc == '#')
				octo = 1;
			else if (esc == '&')
				amp = 1;
			else
				caret = 1;
		}

		switch(charclass[c]) {

		case DIGIT:
			while (numpart[c]) {
				putm(c);
				c = nextmac();
			}
			continue;

		case STARTER:
		case LETTER:
			t = 0;
			p = symbuf;
			do {
				if (p >= symbufmax) {
					*symbufmax = '\0';
					printf("was parsing '%s' in macro definition\n", tempbuf);
					error(symlong);
				}
				*p++ = c;
				if (t < MAXSYMBOLSIZE)
					tempbuf[t++] = (c >= 'A' && c <= 'Z')  ?
						c+'a'-'A' : c;
				c = nextmac();
			} while	(charclass[c]==LETTER || charclass[c]==DIGIT || charclass[c]==STARTER);

			tempbuf[t] = 0;
			*p++ = '\0';
			p = symbuf;

			key = keyword(tempbuf);
			t = key ? key->i_token : 0;

			// Hmmm, that "item_lookup" is putting crap in the table, yes?
			if (mras)
				param = item_substr_lookup(tempbuf, MPARM, paramtab, PARAMTABSIZE);
			else
				param = item_lookup(tempbuf, paramtab, PARAMTABSIZE);

			// Accept almost anything as a parameter.  But a few
			// keywords will just screw things up royally.
			if (param && t != ENDM && t != REPT && t != IRPC && t != IRP && t != MACRO)
				t = param->i_token;

			// Caret escaping kills all expansion
			// And interpretation of ENDM, etc.  For better or worse.
			if (caret)
				t = 0;

			if (t == MPARM) {
				int pfx = octo || amp || c == '&';
				// # prefix only allowed if declared that way
				if (octo != param->i_scope)
					t = 0;
				else
					octo = 0;

				// Expansion in quoted strings only with prefix character.
				if (quote && !pfx && !zcompat)
					t = 0;

				amp = 0; // parameter substitution, eat '&'
			}
			else if (t && quote)
				t = 0;

			if (ampdrop)
				amp = 0;

			ampdrop = c == '&' && t == MPARM;

			if (octo) putm('#');
			if (amp) putm('&');

			if (t != MPARM) {
				putm(symbuf[0]);
				putm_str(symbuf + 1, mras && !quote);
			}
			else {
				putm_param_ref(param);
				// Only in MRAS will we have a remainder string
				putm_str(symbuf + strlen(param->i_string), mras && !quote);
			}

			if (t == ENDM) {
				if (--inmlex == 0)
					goto done;
			}
			else if (t == REPT || t == IRPC || t == IRP || t == MACRO) {
				inmlex++;
			}

			continue;

		case F_END:
			errwarn(warn_general, "macro definition went until end of file");
			if (expptr) {
				popsi();
				c = nextmac();
				continue;
			}

			goto done;

		default:
			switch (c) {
			case '\n':
				quote = 0;
				list1();
				break;
			case ';':
				if (!quote) {
					while (c != '\n' && c != 0) {
						putm(c);
						c = nextmac();
					}
					continue;
				}
				break;
			case '\'':
			case '"':
				if (c == quote)
					quote = 0;
				else
					quote = c;
				break;
			default:
				break;
			}
			if (c != '\1' && c != '`') putm(c);
			c = nextmac();
		}
	}

	/*
	 *  finish off the file entry
	 */
done:
	while(c != EOF && c != '\n' && c != '\0') c = nextmac();
	inmlex++;
	list1();
	inmlex--;
	// WHY two newlines??
//	putm('\n');
	putm('\n');
	putm(0);

	// TODO - here's where we could save parameter names for MRAS
	for (c = 0; c < PARAMTABSIZE; c++) {
		if (paramtab[c].i_token == MPARM) {
			free(paramtab[c].i_string);
			paramtab[c].i_string = NULL;
			paramtab[c].i_token = 0;
		}
	}
	inmlex = 0;
#ifdef	M_DEBUG
	fprintf(stderr,"exit 'mlex' at %d\n", mfptr) ;
#endif
}

void putm_param_ref(struct item *param)
{
	if (*(param->i_string) == '?' || param->i_chain)
		putm('\2');
	else
		putm('\1');

	putm(param->i_value + 'A');
}

void putm_str(char *str, int look_for_param)
{
	for (; *str; str++) {
		if (look_for_param) {
			struct item *param = item_substr_lookup(str, MPARM, paramtab, PARAMTABSIZE);
			if (param) {
				putm_param_ref(param);
				str += strlen(param->i_string) - 1;
				continue;
			}
		}
		putm(*str);
	}
}

int str_getch(struct argparse *ap)
{
	int ch = ap->user_peek;
	if (ch >= 0) {
		ap->user_peek = -1;
		return ch;
	}
	if (!ap->user_ptr || ap->user_ptr[ap->user_int] == '\0')
		return '\0';

	return ap->user_ptr[ap->user_int++];
}

int arg_getch(struct argparse *ap)
{
	(void)ap; // suppress warning
	return nextchar();
}

void arg_start()
{
	arg_reset();
	arg_flag = 1;
}

void arg_reset()
{
	arg_flag = 0;

	arg_state.arg = tempbuf;
	arg_state.argsize = sizeof tempbuf;
	arg_state.peek = &peekc;
	arg_state.getch = arg_getch;
	arg_state.macarg = 0;
	arg_state.user_ptr = 0;
	arg_state.user_int = 0;
	arg_state.didarg = 0;
	arg_state.numarg = 0;
}

/*
 *  lexical analyser for the arguments of a macro call
 */
int getarg(struct argparse *ap)
{
	int c;
	char *p;
	int quote;
	int depth;

	*ap->arg = 0;
	while (charclass[c = ap->getch(ap)] == SPACE);

	switch(c) {

	case '\0':
		if (!ap->user_ptr)
			popsi(); // a seemingly unlikely case?
	case '\n':
	case ';':
		if (!ap->didarg && ap->numarg) {
			*ap->peek = c;
			ap->didarg = 1;
			ap->numarg++;
			return ARG;
		}
		ap->didarg = 0;
		ap->numarg = 0;
		return c;

	case ',':
		if (!ap->didarg) {
			ap->didarg = 1;
			*ap->peek = c;
			ap->numarg++;
			return ARG;
		}
		ap->didarg = 0;
		return c;

	case '\'':
	case '\"':
		quote = c;
		p = ap->arg;
		if (!zcompat)
			*p++ = c;

		do {
			c = ap->getch(ap);
			if (c == '\0' || c == '\n') {
				*ap->peek = c;
				*p = 0;
				err[bflag]++;
				ap->didarg = 1;
				ap->numarg++;
				return ARG;
			}
			else if (c == quote) {
				if ((c = ap->getch(ap)) != quote) {
					if (!zcompat)
						*p++ = quote;
					*ap->peek = c;
					*p = '\0';
					ap->didarg = 1;
					ap->numarg++;
					return ARG;
				}
			}
			else
				*p++ = c;
		} while (p < ap->arg + ap->argsize - 1);
		ap->arg[ap->argsize - 1] = '\0';
		printf("was parsing macro argument '%s'\n", ap->arg);
		error(symlong);
		return 0; // not reached

	case '<':
		depth = 1;
		p = ap->arg;
		do {
			c = ap->getch(ap);
			if (c == '\0' || c == '\n') {
				*ap->peek = c;
				*p = 0;
				err[bflag]++;
				ap->didarg = 1;
				ap->numarg++;
				return ARG;
			}
			if (c == '>') {
				depth--;
				if (depth == 0) {
					*p = '\0';
					ap->didarg = 1;
					ap->numarg++;
					return ARG;
				}
			}
			else if (c == '<')
				depth++;

			*p++ = c;
		} while (p < ap->arg + ap->argsize - 1);
		ap->arg[ap->argsize - 1] = '\0';
		printf("was parsing macro argument '%s'\n", ap->arg);
		error(symlong);
		return 0; // not reached

	default:  /* unquoted string */
		if (c == '%' && ap->macarg) {
			ap->didarg = 1;
			ap->numarg++;
			return c;
		}

		p = ap->arg;
		*ap->peek = c;

		do {
			c = ap->getch(ap);
			switch(c) {
			case '\0':
			case '\n':
			case '\t':
			case ' ':
			case ',':
				*ap->peek = c;
				*p = '\0';
				ap->didarg = 1;
				ap->numarg++;
				return ARG;
			case '^':
				c = ap->getch(ap);
				switch (c) {
				case ',':
				case '^':
				case ' ':
				case '\t':
					*p++ = c;
					break;
				default:
					*p++ = '^';
					*ap->peek = c;
					break;
				}
				break;
			default:
				*p++ = c;
			}
		} while (p < ap->arg + ap->argsize - 1);
		ap->arg[ap->argsize - 1] = '\0';
		printf("was parsing macro argument '%s'\n", ap->arg);
		error("macro argument too long");
		return 0; // not reached
	}
}


/*
 *  add a suffix to a string
 */
void suffix(char *str, char *suff)
{
	strcpy(getsuffix(str), suff);
}

char *basename(char *filename)
{
	char *base, *p;

	base = filename;
	for (p = filename; *p; p++) {
		if (*p == '/' || *p == '\\') {
			base = p + 1;
		}
	}

	return base;
}

char *getsuffix(char *str)
{
	char *suffix = 0;
	str = basename(str);
	for (; *str; str++) {
		if (*str == '.')
			suffix = str;
	}
	return suffix ? suffix : str;
}

// Construct output file given input path.
// Essentially files for "file.z" are sent to "zout/file.suffix".
// And for "dir/file.z" they are "zout/file.suffix"
// Creates output directory as a side effect.

void outpath(char *out, char *src, char *suff)
{
	static int did_mkdir = 0;

	strcpy(out, output_dir);

	if (!did_mkdir) {
		char *dir = out;
		while (*dir) {
			char *p;
			int ch;
			for (p = dir; *p && *p != '/'
#ifdef WIN32
				 && *p != '\\'
#endif
				 ; p++) { };
			ch = *p;
			*p = '\0';
#ifdef WIN32
			_mkdir(out);
#else
			mkdir(out, 0777);
#endif
			*p = ch;
			dir = p;
			if (ch)
				dir++;
		}
		did_mkdir = 1;
	}

	if (!suff)
		return;

	if (*out)
		strcat(out, "/");
	strcat(out, basename(src));
	suffix(out, suff);
}


/*
 *  put out a byte to the macro file, keeping the offset
 */
void putm(int c)
{
	mfseek(mfile, mfptr, 0);
	mfptr++;
	mfputc(c, mfile);
}



/*
 *  get a byte from the macro file
 */
int getm()
{
	int ch;

	mfseek(mfile, floc, 0);
	floc++;
	ch = mfgetc(mfile);
	if (ch == EOF) {
		ch = 0;
		fprintf(stderr, "bad macro read\n");
	}
	return ch;
}



/*
 *  pop standard input
 */
void popsi()
{
	int  i;

	for (i=0; i<PARMMAX; i++) {
		if (est[i].param) free(est[i].param);
	}
	floc = est[FLOC].value;
	ifptr = est[MIF].param;
	free(est);
	expptr--;
	est = expptr ? expstack[expptr-1] : 0;
	mfseek(mfile, (long)floc, 0);
	if (lineptr > linebuf) lineptr--;
}



/*
 *  return a unique name for a local symbol
 *  c is the parameter number, n is the macro number.
 */

char *getlocal(int c, int n)
{
	static char local_label[10];

	invented++;
	if (c >= 26)
		c += 'a' - '0';
	sprintf(local_label, "?%c%04d", c+'a', n) ;
	return(local_label);
}

char *getmraslocal()
{
	static char mras_local[32];
	char *p = mras_local + sizeof mras_local - 1;
	int n = est[TEMPNUM].value;

	*p = '\0';
	for (; n > 0; n /= 26)
		*--p = 'A' + n % 26;


	return p;
}


/*
 *  read in a symbol table
 */
void insymtab(char *name)
{
	struct stab *t;
	int  s, i;
	FILE *sfile;

	t = (struct stab *) tempbuf;
	if (!(sfile = fopen(name, "rb")))
		return;
	fread((char *)t, 1, sizeof *t, sfile);
	if (t->t_value != SYMMAJIC)
		return;

	
	s = t->t_token;
	for (i=0; i<s; i++) {
		fread((char *)t, 1, sizeof *t, sfile);
		if (tokenofitem(UNDECLARED, 0, 0) != UNDECLARED)
			continue;
		yylval.itemptr->i_token = t->t_token;
		yylval.itemptr->i_value = t->t_value;
		if (t->t_token == MACRO)
			yylval.itemptr->i_value += mfptr;
	}

	while ((s = fread(tempbuf, 1, TEMPBUFSIZE, sfile)) > 0) {
		mfptr += s;
		mfwrite(tempbuf, 1, s, mfile) ;
	}
	fclose(sfile);
}



/*
 *  write out symbol table
 */
void outsymtab(char *name)
{
	struct stab *t;
	struct item *ip;
	int  i;
	FILE *sfile;

	t = (struct stab *) tempbuf;
	if (!(sfile = fopen(name, "wb")))
		return;
	for (ip=itemtab; ip<itemmax; ip++) {
		if (ip->i_token == UNDECLARED) {
			ip->i_token = 0;
			nitems--;
		}
	}

	copyname(title, (char *)t);
	t->t_value = SYMMAJIC;
	t->t_token = nitems;
	fwrite((char *)t, 1, sizeof *t, sfile);

	for (ip=itemtab; ip<itemmax; ip++) {
		if (ip->i_token != 0) {
			t->t_token = ip->i_token;
			t->t_value = ip->i_value;
			copyname(ip->i_string, (char *)t);
			fwrite((char *)t, 1, sizeof *t, sfile);
		}
	}

	mfseek(mfile, (long)0, 0);
	while((i = mfread(tempbuf, 1, TEMPBUFSIZE, mfile) ) > 0)
		fwrite(tempbuf, 1, i, sfile);

	fclose(sfile);
}



/*
 *  copy a name into the symbol file
 */
void copyname(char *st1, char *st2)
{
	char  *s1, *s2;
	int  i;

	i = (MAXSYMBOLSIZE+2) & ~01;
	s1 = st1;
	s2 = st2;

	while((*s2++ = *s1++)) i--;		/* -Wall-ishness :-) -RJM */
	while(--i > 0) *s2++ = '\0';
}

/* get the next source file */
void next_source(char *sp, int always)
{
	char *path;

	if (!always && imported(sp))
		return;

	if(now_in == NEST_IN -1)
		error("Too many nested includes") ;
	if ((now_file = open_incpath(sp, "r", &path)) == NULL) {
		char ebuf[1024] ;
		sprintf(ebuf,"Can't open include file: %s", sp) ;
		error(ebuf) ;
	}
	if (outpass && iflist()) {
		lineout() ;
		fprintf(fout, "**** %s ****\n", path) ;
	}

	if (outpass && fbds)
		fprintf(fbds, "%04x %04x f %s\n", dollarsign, emit_addr, sp);

	/* save the list control flag with the current line number */
	if (lstoff)
		linein[now_in] = - linein[now_in] ;

	/* no list if include files are turned off */
	lstoff |= iopt ;

	linepeek[now_in] = nextline_peek;
	nextline_peek = NOPEEK;
	/* save the new file descriptor. */
	fin[++now_in] = now_file ;
	/* start with line 0 */
	linein[now_in] = 0 ;
	/* save away the file name */
	src_name[now_in] = path;
}

int phaseaddr(int addr)
{
	if (!phaseflag)
		return addr;

	if (addr < phbegin || addr > dollarsign) {
		err[vflag]++;
		if (pass2)
			fprintf(stderr, "$%04x outside current phase area\n", addr);
		return 0;
	}

	return phdollar + (addr - phbegin);
}

// Include contents of named file as binary data.
void incbin(char *filename)
{
	FILE *fp = open_incpath(filename, "rb", NULL);
	int ch;
	int start = dollarsign;
	int last = start;
	int bds_count;

	if (!fp) {
		char ebuf[1024];
		sprintf(ebuf, "Can't binary include file: %s", filename);
		error(ebuf);
		return;
	}

	addtoline('\0');
	if (outpass && fbds)
		fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, linebuf);

	// Avoid emit() because it has a small buffer and it'll spam the listing.
	bds_count = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (outpass && fbds) {
			if (bds_count == 0)
				fprintf(fbds, "%04x %04x d ", dollarsign, emit_addr);
			fprintf(fbds, "%02x", ch);
			bds_count++;
			if (bds_count == 16) {
				fprintf(fbds, "\n");
				bds_count = 0;
			}
		}

		if (segment == SEG_CODE)
			setmem(emit_addr, ch, MEM_DATA);
		emit_addr++;
		emit_addr &= 0xffff;
		last = dollarsign;
		dollarsign++;
		dollarsign &= 0xffff;

		putbin(ch);
		putrel(ch);
		putout(ch);
	}
	if (outpass && fbds && bds_count)
		fprintf(fbds, "\n");

	fclose(fp);

	// Do our own list() work as we emit bytes manually.

	if (outpass && iflist()) {
		lineout();

		if (nopt)
			fprintf(fout, "%4d:", linein[now_in]);

		if (copt)
		        fprintf(fout, nopt ? "%5s-" : "%4s-", "");

		if (nopt || copt)
			fprintf(fout, "\t");

		puthex(start >> 8, fout);
		puthex(start, fout);
		if (relopt)
			fputc(SEGCHAR(segment), fout);
		fprintf(fout, " .. ");
		puthex(last >> 8, fout);
		puthex(last, fout);
		if (relopt)
			fputc(SEGCHAR(segment), fout);

		putc('\t', fout);

		fputs(linebuf, fout);

		lineptr = linebuf;
	}
}

void dc(int count, int value)
{
	int start = dollarsign;
	int bds_count;

	addtoline('\0');
	if (outpass && fbds)
		fprintf(fbds, "%04x %04x s %s", dollarsign, emit_addr, linebuf);

	// Avoid emit() because it has a small buffer and it'll spam the listing.
	bds_count = 0;
	while (count-- > 0) {
		if (outpass && fbds) {
			if (bds_count == 0)
				fprintf(fbds, "%04x %04x d ", dollarsign, emit_addr);
			fprintf(fbds, "%02x", value);
			bds_count++;
			if (bds_count == 16) {
				fprintf(fbds, "\n");
				bds_count = 0;
			}
		}

		if (segment == SEG_CODE)
			setmem(emit_addr, value, MEM_DATA);
		emit_addr++;
		emit_addr &= 0xffff;
		dollarsign++;
		dollarsign &= 0xffff;

		putbin(value);
		putrel(value);
		putout(value);
	}
	if (outpass && fbds && bds_count)
		fprintf(fbds, "\n");

	// Do our own list() work as we emit bytes manually.

	if (outpass && iflist()) {
		lineout();

		if (nopt)
			fprintf(fout, "%4d:", linein[now_in]);

		if (copt)
		        fprintf(fout, nopt ? "%5s-" : "%4s-", "");

		if (nopt || copt)
			fprintf(fout, "\t");

		puthex(start >> 8, fout);
		puthex(start, fout);
		if (relopt) {
			fputc(SEGCHAR(segment), fout);
			fprintf(fout, "..");
		}
		else
			fprintf(fout, " .. ");

		puthex((dollarsign - 1) >> 8, fout);
		puthex((dollarsign - 1), fout);
		if (relopt)
			fputc(SEGCHAR(segment), fout);
		putc(' ', fout);
		puthex(value, fout);
		putc('\t', fout);
		fputs(linebuf, fout);
		lsterr2(1);

		lineptr = linebuf;
	}
	else
		lsterr1();
}

#define OUTREC_SEG	outbuf[outrec]
#define OUTREC_ADDR	((outbuf[outrec + 1] << 8) | outbuf[outrec + 2])
#define OUTREC_LEN	outbuf[outrec + 3]
#define OUTREC_DATA	outbuf[outrec + 4 + outlen]
#define OUTREC_SIZEOF	5

void new_outrec(void)
{
	OUTREC_LEN = outlen;
	outrec += OUTREC_SIZEOF + outlen;

	outlen = 0;
	OUTREC_SEG = segment;
	outbuf[outrec + 1] = seg_pos[segment] >> 8;
	outbuf[outrec + 2] = seg_pos[segment];
}

void putout(int value)
{
	int addr = (OUTREC_ADDR + outlen) & 0xffff;
	if (OUTREC_SEG != segment || addr != seg_pos[segment])
		new_outrec();

	if (pass2 && OUTREC_DATA != value && !passfail) {
		char segname[2];
		if (relopt)
			sprintf(segname, "%c", SEGCHAR(segment));
		else
			segname[0] = '\0';

		sprintf(detail, "%s error - $%04x%s changed from $%02x to $%02x",
			errname[pflag], addr, segname, OUTREC_DATA, value);
		errwarn(pflag, detail);

		if (!outpass)
			passretry = 1;
	}
	OUTREC_DATA = value;
	outlen++;

	if (outlen >= 256)
		new_outrec();

	advance_segment(1);
}

void advance_segment(int step)
{
	int top = seg_pos[segment] += step;
	seg_pos[segment] &= 0xffff;
	if (top >= 0x10000)
		top = 0xffff;

	if (top > seg_size[segment]) {
		seg_size[segment] = top;
		if (segment == SEG_COMMON && cur_common)
			cur_common->i_value = top;
	}
}

void expr_reloc_check(struct expr *ex)
{
	if (!relopt) return;
	if (ex->e_scope & (SCOPE_EXTERNAL | SCOPE_NORELOC))
		err[rflag]++;
}

void expr_number_check(struct expr *ex)
{
	if (!relopt) return;
	expr_reloc_check(ex);
	if (ex->e_scope & SCOPE_SEGMASK)
		err[rflag]++;
}

void expr_scope_same(struct expr *ex1, struct expr *ex2)
{
	if (!relopt) return;
	if ((ex1->e_scope & SCOPE_SEGMASK) != (ex2->e_scope & SCOPE_SEGMASK))
		err[rflag]++;
}

void expr_word_check(struct expr *ex)
{
	if (ex->e_value < -32768 || ex->e_value > 65535) {
		err[vflag]++;
	}
}

int is_number(struct expr *ex)
{
	return ex && (ex->e_scope & ~SCOPE_PUBLIC) == 0;
}

int is_external(struct expr *ex)
{
	return ex && (ex->e_scope & SCOPE_EXTERNAL) && !ex->e_left && !ex->e_right &&
		ex->e_item;
}

struct expr *expr_alloc(void)
{
	struct expr *ex = malloc(sizeof *ex);

	ex->e_value = 0;
	ex->e_scope = 0;
	ex->e_token = 0;
	ex->e_item = 0;
	ex->e_left = 0;
	ex->e_right = 0;

	return ex;
}

struct expr *expr_var(struct item *it)
{
	struct expr *ex = expr_alloc();

	ex->e_token = 'v';
	ex->e_item = it;
	ex->e_scope = it->i_scope;
	ex->e_value = it->i_value;

	return ex;
}

struct expr *expr_num(int value)
{
	struct expr *ex = expr_alloc();
	ex->e_value = value;
	ex->e_token = '0';

	return ex;
}

// Build expression and update value based on the operator.
// Could be done inline in the grammar but there is a fair bit of
// repetition and MRAS operators have only made that worse.

struct expr *expr_mk(struct expr *left, int op, struct expr *right)
{
	struct expr *ex;
	int val = 0;
	int sc = 0;

	switch (op) {
	case '+':
		ex = expr_op(left, '+', right, left->e_value + right->e_value);

		// Can't operate on external labels.
		// But we can add constants to any scope.
		if (!((left->e_scope | right->e_scope) & SCOPE_EXTERNAL) &&
			((left->e_scope & SCOPE_SEGMASK) == 0 ||
			(right->e_scope & SCOPE_SEGMASK) == 0))
		{
			ex->e_scope &= ~(SCOPE_NORELOC | SCOPE_SEGMASK);
			ex->e_scope |= (left->e_scope | right->e_scope) & SCOPE_SEGMASK;
		}
		return ex;
	case '-':
		ex = expr_op_sc(left, '-', right, left->e_value - right->e_value);

		// But we can subtract a constant.
		if (!((left->e_scope | right->e_scope) & SCOPE_EXTERNAL) &&
			((right->e_scope & SCOPE_SEGMASK) == 0))
		{
			ex->e_scope &= ~(SCOPE_NORELOC | SCOPE_SEGMASK);
			ex->e_scope |= (left->e_scope & SCOPE_SEGMASK);
		}
		return ex;
	case '/':
		if (!(right->e_scope & SCOPE_EXTERNAL)) {
			if (right->e_value == 0)
				err[eflag]++;
			else
				val = left->e_value / right->e_value;
		}
		break;
	case '*':
		val = left->e_value * right->e_value;
		break;
	case '%':
		if (!(right->e_scope & SCOPE_EXTERNAL)) {
			if (right->e_value == 0)
				err[eflag]++;
			else
				val = left->e_value % right->e_value;
		}
		break;
	case '&':
		val = left->e_value & right->e_value;
		break;
	case '|':
		val = left->e_value | right->e_value;
		break;
	case '^':
		val = left->e_value ^ right->e_value;
		break;
	case SHL:
		val = left->e_value << right->e_value;
		break;
	case SHR:
		val = right->e_value == 0 ? left->e_value : ((left->e_value >> 1) & ((0x7fff << 16) | 0xffff)) >> (right->e_value - 1);
		break;
	case '<':
		val = (left->e_value < right->e_value) * trueval;
		sc = 1;
		break;
	case '=':
		val = (left->e_value == right->e_value) * trueval;
		sc = 1;
		break;
	case '>':
		val = (left->e_value > right->e_value) * trueval;
		sc = 1;
		break;
	case LE:
		val = (left->e_value <= right->e_value) * trueval;
		sc = 1;
		break;
	case NE:
		val = (left->e_value != right->e_value) * trueval;
		sc = 1;
		break;
	case GE:
		val = (left->e_value >= right->e_value) * trueval;
		sc = 1;
		break;
	case ANDAND:
		val = (left->e_value && right->e_value) * trueval;
		break;
	case OROR:
		val = (left->e_value || right->e_value) * trueval;
		break;
	default:
		fprintf(stderr, "internal expression evaluation error!\n");
		clean_outf();
		exit(-1);
		break;
	}

	if (sc)
		return expr_op_sc(left, op, right, val);

	return expr_op(left, op, right, val);
}


// Expression consruction for operators that subtract/compare.
// They produce a valid result if operating on numbers in the same segment.
struct expr *expr_op_sc(struct expr *left, int token, struct expr *right, int value)
{
	struct expr *ex = expr_op(left, token, right, value);

	if (!(ex->e_scope & SCOPE_EXTERNAL) &&
		((left->e_scope ^ right->e_scope) & SCOPE_SEGMASK) == 0)
	{
		// Result relocatable and a simple number
		ex->e_scope &= ~(SCOPE_NORELOC | SCOPE_SEGMASK);
	}

	return ex;
}

struct expr *expr_op(struct expr *left, int token, struct expr *right, int value)
{
	struct expr *ex = expr_alloc();

	ex->e_value = value;
	ex->e_token = token;
	ex->e_left = left;
	ex->e_right = right;

	// Combining two numbers will be fine as long as they're not
	// flagged as external or already not relocatable.  In which case
	// it is up to the particular operator to allow the value
	// to become valid.

	ex->e_scope = left->e_scope;
	if (left->e_scope & SCOPE_SEGMASK)
		ex->e_scope |= SCOPE_NORELOC;
	if (right) {
		ex->e_scope |= right->e_scope;
		if (right->e_scope & SCOPE_SEGMASK)
			ex->e_scope |= SCOPE_NORELOC;
	}

	return ex;
}

void expr_free(struct expr *ex)
{
	if (!ex)
		return;

	expr_free(ex->e_left);
	expr_free(ex->e_right);
	free(ex);
}

int synth_op(struct expr *ex, int gen)
{
	if (ex->e_token == '&' && is_number(ex->e_right) &&
		ex->e_right->e_value == 255)
	{
		if (gen) {
			extend_link(ex->e_left);
			putrelop(RELOP_LOW);
			return 1;
		}
		return can_extend_link(ex->e_left);
	}

	return 0;
}

int link_op(struct expr *ex)
{
	if (!ex)
		return 0;

	switch (ex->e_token) {
	case HIGH: return RELOP_HIGH;
	case LOW: return RELOP_LOW;
	case '~': return RELOP_NOT;
	case '-': return !ex->e_right ? RELOP_NEG : RELOP_SUB;
	case '+': return RELOP_ADD;
	case '*': return RELOP_MUL;
	case '/': return RELOP_DIV;
	case '%': return RELOP_MOD;
	default: return 0;
	}
}

int can_extend_link(struct expr *ex)
{
	if (!ex)
		return 1;

	// If we have a value available then we're good.
	if (!(ex->e_scope & SCOPE_NORELOC))
		return 1;

	// Might be able to synthesize the operation.
	if (synth_op(ex, 0))
		return 1;

	// Otherwise, the operator must be supported and the children
	// must be linkable.

	return link_op(ex) && can_extend_link(ex->e_left) && can_extend_link(ex->e_right);
}

void extend_link(struct expr *ex)
{
	int op;

	if (!ex)
		return;

	if (synth_op(ex, 1))
		return;

	extend_link(ex->e_left);
	extend_link(ex->e_right);

	op = link_op(ex);
	if (op) {
		putrelop(op);
		return;
	}

	putrelcmd(RELCMD_EXTLINK);

	if (is_external(ex)) {
		char *str = ex->e_item->i_string;
		int len = strlen(str);

		if (len > 6)
			len = 6;

		putrelbits(3, 1 + len);
		putrelbits(8, 'B');
		while (len-- > 0) {
			int ch = *str++;
			if (ch >= 'a' && ch <= 'z')
				ch -= 'a' - 'A';
			putrelbits(8, ch);
		}
	}
	else {
		putrelbits(3, 4);
		putrelbits(8, 'C');
		putrelbits(8, ex->e_scope & SCOPE_SEGMASK);
		putrelbits(8, ex->e_value);
		putrelbits(8, ex->e_value >> 8);
	}
}

void putrelop(int op)
{
	putrelcmd(RELCMD_EXTLINK);

	putrelbits(3, 2);
	putrelbits(8, 'A');
	putrelbits(8, op);
}

void write_tap_block(int type, int len, unsigned char *data)
{
	int i, parity;

	fputc((len + 2) & 0xff, ftap);
	fputc((len + 2) >> 8, ftap);

	fputc(type, ftap);
	parity = type;
	for (i = 0; i < len; i++) {
		fputc(data[i], ftap);
		parity ^= data[i];
	}
	fputc(parity, ftap);
}

// One supposes .tap files could load multiple blocks into memory.
// However, doesn't seem to be a lot of point and we'd have to write
// extra loader code to make it happen.  For now we just load the
// assembled data as one contiguous block with the holes getting
// filled with zeros.

void write_tap(int len, int org, unsigned char *data)
{
	unsigned char block[32], *p, orglo, orghi;
	unsigned char basic_loader[] = {
		239, 34, 34, 175, 58, 249, 192, 176, // LOAD ""CODE:RANDOMIZE USR VAL
		'"', '2', '3', '2', '9', '6', '"', 13 // aka 0x5b00 - start of RAM
	};
	int entry = org;

	if (xeq_flag)
		entry = xeq;

	// .tap file output borrowed heavily from skoolkit's bin2tap.py
	// It loads a short basic program which auto-executes and loads
	// a short machine-language loader that reads the block of code
	// and jumps to it.
	// Constrast this with pasmo which doesn't have the short machine
	// code loader but uses a native code block.  I think that means
	// it can only execute at the beginning of the loaded data.

	p = block;

	*p++ = 0; // Program block
	casname((char *)p, sourcef, 10);
	p += 10;
	*p++ = 4 + sizeof basic_loader; *p++ = 0; // length of BASIC program
	*p++ = 10; *p++ = 0; // run line 10 after loading
	*p++ = 4 + sizeof basic_loader; *p++ = 0; // length of BASIC program

	write_tap_block(0, p - block, block);

	p = block;
	*p++ = 0; *p++ = 10; // line 10
	*p++ = sizeof(basic_loader); *p++ = 0;
	memcpy(p, basic_loader, sizeof basic_loader);
	p += sizeof basic_loader;
	write_tap_block(0xff, p - block, block);

	p = block;

	*p++ = 3; // Code block
	casname((char *)p, sourcef, 10);
	p += 10;
	*p++ = 19; *p++ = 0; // length of loader program
	*p++ = 0; *p++ = 0x5b; // 0x5b00 == 23296 - start of RAM
	*p++ = 0; *p++ = 0; // ?

	write_tap_block(0, p - block, block);

	p = block;

	orglo = org & 0xff;
	orghi = org >> 8;
	/* LD IX,org   */ *p++ = 0xdd; *p++ = 0x21; *p++ = orglo; *p++ = orghi;
	/* LD DE,len   */ *p++ = 0x11; *p++ = len & 0xff; *p++ = len >> 8;
	/* SCF         */ *p++ = 0x37;
	/* SBC A,A     */ *p++ = 0x9f;
	/* LD SP,org   */ *p++ = 0x31; *p++ = orglo; *p++ = orghi;
	/* LD BC,entry */ *p++ = 0x01; *p++ = entry & 0xff; *p++ = entry >> 8;
	/* PUSH BC     */ *p++ = 0xc5;
	/* JP $556     */ *p++ = 0xc3; *p++ = 0x56; *p++ = 0x05;

	write_tap_block(0xff, p - block, block);

	write_tap_block(0xff, len, data);
}

#define WORD(w) (w) & 255, (w) >> 8

void write_250(int low, int high)
{
	int load = low;
	int len = high - low + 1;
	int last;
	int chk;

	if (len <= 0) {
		// Nothing to output.  So we'll just delete the output file.
		int i;
		for (i = 0; i < CNT_OUTF; i++) {
			if (*outf[i].fpp == ftcas || *outf[i].fpp == f250wav) {
				fclose(*outf[i].fpp);
				*outf[i].fpp = NULL;
				unlink(outf[i].filename);
				if (outf[i].wanted)
					fprintf(stderr, "Warning: %s not output -- no code or data\n", outf[i].filename);
			}
		}
		return;
	}

	if (xeq_flag) {
		// Only add relocation if they don't already put their
		// execution address in to $41FE.  This means programs will
		// be unchanged if they seem to be aware of the structure.

		if (low > 0x41FE || high < 0x41FF ||
			memory[0x41FE] != (xeq & 0xff) ||
			memory[0x41FF] != xeq >> 8)
		{
			if (low >= 0x4200 && low <= 0x4200 + 14) {
				// A little too high.  More efficient to
				// just load a bit extra.  Plus we can't
				// easily fit in the "copy up" code.
				low = 0x41FE;
				memory[0x41FE] = xeq;
				memory[0x41FF] = xeq >> 8;
				load = low;
			}
			else if (low < 0x4200) {
				// Moving down.
				int src = 0x4200;
				int dst = low;
				unsigned char relo[] = {
					0x21, WORD(src),	// LD HL,nn
					0x11, WORD(dst),	// LD DE,nn
					0x01, WORD(len),	// LD BC,len
					0xED, 0xB0,		// LDIR
					0xC3, WORD(xeq)		// JP nn
				};
				high++;
				low -= 2;
				memory[low] = src + len;
				memory[low + 1] = (src + len) >> 8;
				memcpy(memory + high, relo, sizeof relo);
				high += sizeof relo - 1;
				load = 0x41FE;
			}
			else {
				// Moving up
				int src = 0x41FE + 2 + 14 + len - 1;
				int dst = low + len - 1;
				unsigned char relo[] = {
					WORD(0x4200),
					0x21, WORD(src),	// LD HL,nn
					0x11, WORD(dst),	// LD DE,nn
					0x01, WORD(len),	// LD BC,len
					0xED, 0xB8,		// LDDR
					0xC3, WORD(xeq)		// JP nn
				};
				low -= sizeof relo;
				memcpy(memory + low, relo, sizeof relo);
				load = 0x41FE;
			}
		}
	}

	len = high + 1 - low;
	last = load + len;
	// Yeah, it is big endian.
	fprintf(ftcas, "%c%c%c%c", load >> 8, load, last >> 8, last);
	fwrite(memory + low, len, 1, ftcas);
	chk = 0;
	for (i = 0; i < len; i++)
		chk += memory[low + i];
	fprintf(ftcas, "%c", -chk);
}

int bitgetbuf;
int bitgetcnt;

void bitget_rewind(FILE *fp)
{
	bitgetcnt = 0;
	fseek(fp, 0, SEEK_SET);
}

int bitget(FILE *fp)
{
	int bit;

	if (bitgetcnt == 0) {
		bitgetbuf = fgetc(fp);
		bitgetcnt = 8;
	}

	bit = !!(bitgetbuf & 0x80);
	bitgetbuf <<= 1;
	bitgetcnt--;

	return bit;
}

void writewavs(int pad250, int pad500, int pad1500)
{
	FILE *cas[] = { ftcas, flcas, flnwcas, fcas };
	FILE *wav[] = { f250wav, f500wav, f1000wav, f1500wav };
	int padbytes[] = { pad250, pad500, pad500, pad1500 };
#define	NFMT (sizeof padbytes / sizeof padbytes[0])
	int bits[NFMT];
	int i, j, k, m;
	unsigned char pulse[][2][13] = {
		{ { 2, 0xff, 2, 0, 42 - 4, 0x80, 0 },
		  { 2, 0xff, 2, 0, 17, 0x80, 2, 0xff, 2, 0, 17, 0x80, 0 } },

		{ { 3, 0xff, 3, 0, 44 - 6, 0x80, 0 },
		  { 3, 0xff, 3, 0, 16, 0x80, 3, 0xff, 3, 0, 16, 0x80, 0 } },

		{ { 3, 0xff, 3, 0, 44 - 6, 0x80, 0 },
		  { 3, 0xff, 3, 0, 16, 0x80, 3, 0xff, 3, 0, 16, 0x80, 0 } },

		{ { 8, 0, 8, 0xff, 0 },
		  { 4, 0, 4, 0xff, 0 } }
	};
	int hz[] = { 11025, 22050, 44100, 22050 };
	int pulse_len[NFMT][2];

	for (i = 0; i < NFMT; i++) {
		for (j = 0; j < 2; j++) {
			pulse_len[i][j] = 0;
			for (k = 0; pulse[i][j][k]; k += 2)
				pulse_len[i][j] += pulse[i][j][k];
		}
	}

	for (i = 0; i < NFMT; i++) {
		if (!cas[i] || !wav[i])
			continue;

		bits[i] = (ftell(cas[i]) - padbytes[i]) * 8;
		if (i == 2 && casbitcnt > 0)
			bits[i] -= 8 - casbitcnt;
	}

	for (i = 0; i < NFMT; i++) {
		int headPad = 10, tailPad = hz[i] / 2;
		int audio_bytes = headPad;
		unsigned char hzlo = hz[i] & 0xff;
		unsigned char hzhi = hz[i] >> 8;

		unsigned char waveHeader[] = {
			'R', 'I', 'F', 'F',
			0, 0, 0, 0,
			'W', 'A', 'V', 'E', 'f', 'm', 't', ' ',
			16, 0, 0, 0, // wav information length
			1, 0, // PCM
			1, 0, // Single channel
			hzlo, hzhi, 0, 0, // samples/second
			hzlo, hzhi, 0, 0, // average bytes/second
			1, 0, // block alignment
			8, 0, // bits/sample
			'd', 'a', 't', 'a',
			0, 0, 0, 0
		};
		int waveHeaderSize = sizeof waveHeader;

		if (!cas[i] || !wav[i])
			continue;

		bitget_rewind(cas[i]);
		for (j = 0; j < bits[i]; j++)
			audio_bytes += pulse_len[i][bitget(cas[i])];

		audio_bytes += tailPad;

		waveHeader[waveHeaderSize - 4] = audio_bytes;
		waveHeader[waveHeaderSize - 3] = audio_bytes >> 8;
		waveHeader[waveHeaderSize - 2] = audio_bytes >> 16;
		waveHeader[waveHeaderSize - 1] = audio_bytes >> 24;

		waveHeader[4] = (audio_bytes + 36);
		waveHeader[5] = (audio_bytes + 36) >> 8;
		waveHeader[6] = (audio_bytes + 36) >> 16;
		waveHeader[7] = (audio_bytes + 36) >> 24;

		bitget_rewind(cas[i]);

		fwrite(waveHeader, waveHeaderSize, 1, wav[i]);

		for (j = 0; j < headPad; j++)
			fputc(0x80, wav[i]);

		bitget_rewind(cas[i]);
		for (j = 0; j < bits[i]; j++) {
			int bit = bitget(cas[i]);
			for (k = 0; pulse[i][bit][k]; k += 2)
				for (m = 0; m < pulse[i][bit][k]; m++)
					fputc(pulse[i][bit][k + 1], wav[i]);
		}

		for (j = 0; j < tailPad; j++)
			fputc(0x80, wav[i]);
	}
}

// Tracking whether a file has been imported.

struct import_list {
	struct import_list *next;
	char *filename;
	int imported;
} *imports;

void reset_import()
{
	struct import_list *il;
	for (il = imports; il; il = il->next)
		il->imported = 0;
}

// Returns 1 if filename has been imported.  Marks it as imported.
// Only uses the base name.

int imported(char *filename)
{
	struct import_list *il;
	int ret;
	char *p;

	for (p = filename; *p; p++)
		if (*p == '/' || *p == '\\')
			filename = p + 1;

	for (il = imports; il; il = il->next) {
		if (strcmp(filename, il->filename) == 0)
			break;
	}

	if (!il) {
		il = malloc(sizeof *il);
		il->filename = strdup(filename);
		il->imported = 0;
		il->next = imports;
		imports = il;
	}

	ret = il->imported;
	il->imported = 1;
	return ret;
}
