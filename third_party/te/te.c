/*	te.c

	Text editor.

	Main module.

	Copyright (c) 2015-2019 Miguel Garcia / FloppySoftware

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the
	Free Software Foundation; either version 2, or (at your option) any
	later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

	Notes:

	Change TE_VERSION in 'te.h' as required, before compilation.

	Changes:

	29 Apr 2015 : Start working.
	02 May 2015 : Added Amstrad PCW version (te_pcw.c).
	03 May 2015 : Added K. Murakami's CP/M emulator for Win32 version (te_em1.c).
	04 May 2015 : Added te.h file. Added forced entry. Nice ruler with tab stops.
	07 May 2015 : While porting it to Pelles C & Windows 32 bit, it was discovered
	              collateral efects in 3 pieces of code. Recoded and marked below.
	              Solved bug in ReadFile() - characters > 0x7F caused 'bad
	              character' errors.
	              Added checking for filenames too long in command line.
	08 May 2015 : v1.00 : Added BackupFile().
	12 May 2015 : v1.01 : Now K_CUT copies the line to the clipboard before deleting it.
	14 May 2015 : v1.02 : Added support for word left & word right cursor movement.
	                      You must define K_LWORD and K_RWORD in your CRT_FILE
	                      to activate them.
	15 May 2015 : v1.03 : Modified getch & putch to getchr & putchr.
	31 Aug 2015 : v1.04 : Minor changes in comments and ReadLine().
	02 Jun 2016 : v1.05 : Minor changes.
	10 Jun 2016 : v1.06 : Screen optimizations in Menu(). Removed BOX_COL.
	                      Removed lp_max, box_cols, ps_fname, ps_lin_cur, ps_lin_now, ps_lin_max,
	                      ps_col_cur, ps_col_now, ps_col_max.
	14 Jun 2016 : v1.07 : Hack for SamaruX.
	05 Jul 2017 : v1.08 : Optimizations in NULL comparisons. Include CC_FGETS.
	24 Jan 2018 : v1.09 : Added find string and find next string commands.
	20 Feb 2018 : v1.10 : Added command to execute macro from file. Split te.c in modules. Added go to line #.
	                      Disable code for macros from strings, for now.
	22 Feb 2018 : v1.11 : Ask for confirmation only if changes were not saved.
	06 Jan 2019 : Included te_lines module and modified related functions. Added LoopCut(), LoopPaste().
	13 Jan 2019 : Included te_misc module. Minor changes.
	15 Jan 2019 : Added LoopBlkStart(), LoopBlkEnd(), LoopBlkUnset(), LoopCopy().
	18 Jan 2019 : Added K_DELETE, LoopDelete().
	19 Jan 2019 : Added LoopUp(), LoopDown().
	22 Jan 2019 : Added te_keys module. Added support for key bindings.
	27 Jan 2019 : Added support for macros.
	29 Jan 2019 : Added K_CLRCLP. Show clipboard status.
	30 Jan 2019 : Removed support for SamaruX.
	14 Feb 2019 : Added help items layout.

	Notes:

	See FIX-ME notes.
*/

/* Libraries
   ---------
*/
#define CC_FGETS
#define CC_FPUTS

#include <mescc.h>
#include <string.h>
#include <ctype.h>
#include <fileio.h>
#include <sprintf.h>

/* TE definitions
   --------------
*/
#include <te.h>

/* Array of text lines
   -------------------
*/
WORD *lp_arr; /* Text lines pointers array */
int   lp_now; /* How many lines are in the array */
int   lp_cur; /* Current line */
int   lp_chg; /* 0 if no changes are made */

/* Current line
   ------------
*/
char *ln_dat; /* Data buffer */
int   ln_max; /* Max. # of characters */

/* Clipboard & block selection
   ---------------------------
*/
#if OPT_BLOCK

int blk_start;   /* Start line # */
int blk_end;     /* End line # */
int blk_count;   /* # of lines */

WORD *clp_arr;   /* Multi-line */
int   clp_count; /* # of lines */

#else

char *clp_line;  /* Just one line */

#endif

/* Filename
   --------
*/
char file_name[FILENAME_MAX];

/* Editor box
   ----------
*/
int box_rows; /* Height in lines */
int box_shr;  /* Vertical   position of cursor in the box (0..box_rows - 1) */
int box_shc;  /* Horizontal position of cursor in the box (0..CRT_COLS - 1) */

/* Keyboard forced entry
   ---------------------
*/
int *fe_dat;  /* Data buffer */
int fe_now;   /* How many characters are now in the buffer */
int fe_set;   /* Set position */
int fe_get;   /* Get position */

/* System line
   -----------
*/
int sysln;    /* NZ when written - for Loop() */

#if OPT_FIND

/* Find string
   -----------
*/
char find_str[FIND_MAX];

#endif

#if OPT_MACRO

/* Macros
   ------
*/
FILE *mac_fp;  /* FP for a file macro, or NULL */
/*char *mac_str;*/ /* Address for a string macro, or NULL */

#endif

/* Key bindings
   ------------
*/
unsigned char keys[KEYS_MAX];
unsigned char keys_ex[KEYS_MAX];
WORD keys_name[KEYS_MAX]; /* char *[] */

/* Help items layout
   -----------------
*/
int help_items[] = {
	K_UP,        K_DOWN,    K_TAB,
	K_LEFT,      K_RIGHT,   K_INTRO,
	K_BEGIN,     K_END,     K_ESC,
	K_TOP,       K_BOTTOM,  K_MACRO,
	K_PGUP,      K_PGDOWN,  0,
	K_LWORD,     K_RWORD,   0,
	K_LDEL,      K_RDEL,    0,
	K_BLK_START, K_BLK_END, K_BLK_UNSET,
	K_CUT,       K_COPY,    K_PASTE,
	K_DELETE,    K_CLRCLP,  0,
	K_FIND,      K_NEXT,    K_GOTO,
	-1
};

/* TE modules
   ----------
*/
#include "te_ui.c"
#include "te_file.c"
#include "te_keys.c"
#include "te_edit.c"
#include "te_lines.c"
#include "te_misc.c"
#include "te_error.c"

#if OPT_MACRO

#include "te_macro.c"

#endif

/* Program entry
   -------------
*/
main(argc, argv)
int argc, argv[];
{
	int i;

	/* Setup some globals */
	box_rows = CRT_ROWS - 4;

	ln_max = CRT_COLS - 1;

	/* Setup CRT */
	CrtSetup();

	/* Print layout */
	Layout();

	/* Allocate buffers -- FIXME -- use AllocMem() ?? */
	ln_dat = malloc(ln_max + 2);

	fe_dat = malloc(FORCED_MAX * SIZEOF_INT);

	lp_arr = malloc(MAX_LINES * SIZEOF_PTR);

	i = 1;

	if(ln_dat && fe_dat && lp_arr) {

#if OPT_BLOCK
#else

		if((clp_line = malloc(ln_max + 1))) {

#endif

			i = 0;

#if OPT_BLOCK
#else

		}

#endif

	}

	if(i)
	{
		ErrLineMem(); CrtReset(); return 1;
	}

	/* Setup clipboard */
#if OPT_BLOCK

	clp_arr = NULL;
	clp_count = 0;

#else

	*clp_line = '\0';

#endif

	/* Setup lines */
	for(i = 0; i < MAX_LINES; ++i) {
		lp_arr[i] = NULL;
	}

	/* Check command line */
	if(argc == 1)
	{
		NewFile();
	}
	else if(argc == 2)
	{
		if(strlen(argv[1]) > FILENAME_MAX - 1)
		{
			ErrLine("Filename too long.");

			NewFile();
		}
		else if(ReadFile(argv[1]))
			NewFile();
		else
			strcpy(file_name, argv[1]);
	}
	else
	{
		ErrLine("Bad command line. Use: te [filename]");

		NewFile();
	}

	/* Main loop */
	Loop();

	/* Clear & reset CRT */
	CrtClear();
	CrtReset();

	/* Exit */
	return 0;
}

/* Main loop
   ---------
*/
Loop()
{
	int run, ch;

	/* Setup forced entry */
	fe_now = fe_get = fe_set = 0;

	/* Setup more things */
	run = sysln = 1;

	/* Print filename */
	ShowFilename();

	/* Refresh editor box */
	RefreshAll();

	/* Loop */
	while(run)
	{
		/* Refresh system line message if it changed */
		if(sysln)
		{
			SysLine("Press [");
			putstr(CRT_ESC_KEY);
			putstr("] to show the menu.");

			sysln = 0;
		}

		/* Print clipboard status */
		CrtLocate(PS_ROW, PS_CLP);

#if OPT_BLOCK

	putstr(clp_count ? "CLP" : "---");

#else

	putstr(*clp_line ? "CLP" : "---");

#endif

		/* Print current line number, etc. */
		CrtLocate(PS_ROW, PS_LIN_CUR); putint("%04d", lp_cur + 1);
		CrtLocate(PS_ROW, PS_LIN_NOW); putint("%04d", lp_now);

		/* Edit the line */
		ch = BfEdit();

		/* Note: BfEdit() does previous checks for following
		   actions, to not waste time when an action is not
		   possible */

		/* Check returned control character */
		switch(ch)
		{
			case K_UP :    /* Up one line --------------------- */
				LoopUp();
				break;
			case K_DOWN :  /* Down one line ------------------- */
				LoopDown();
				break;
			case K_INTRO : /* Insert CR ----------------------- */
				LoopIntro();
				break;
			case K_LDEL :  /* Delete CR on the left ----------- */
				LoopLeftDel();
				break;
			case K_RDEL :  /* Delete CR on the right ---------- */
				LoopRightDel();
				break;
			case K_PGUP :  /* Page up ------------------------- */
				LoopPgUp();
				break;
			case K_PGDOWN :/* Page down ----------------------- */
				LoopPgDown();
				break;
			case K_TOP :   /* Top of document ----------------- */
				LoopTop();
				break;
			case K_BOTTOM :/* Bottom of document -------------- */
				LoopBottom();
				break;
			case K_COPY :   /* Copy line or block ------------- */
				LoopCopy();
				break;
			case K_CUT :    /* Copy and delete line or block -- */
				LoopCut();
				break;
			case K_PASTE :  /* Paste line or block ------------ */
				LoopPaste();
				break;
			case K_DELETE : /* Delete line or block ----------- */
				LoopDelete();
				break;
			case K_CLRCLP : /* Clear the clipboard ------------ */
				LoopClrClp();
				break;

#if OPT_BLOCK
			case K_BLK_START : /* Set block start ------------- */
				LoopBlkStart();
				break;
			case K_BLK_END :   /* Set block end --------------- */
				LoopBlkEnd();
				break;
			case K_BLK_UNSET : /* Unset block ----------------- */
				LoopBlkUnset();
				break;
#endif

#if OPT_FIND
			case K_FIND :  /* Find string --------------------- */
				LoopFindFirst();
				break;
			case K_NEXT :  /* Find next string ---------------- */
				LoopFindNext();
				break;
#endif

#if OPT_GOTO
			case K_GOTO :  /* Go to line # -------------------- */
				LoopGoLine();
				break;
#endif


#if OPT_MACRO
			case K_MACRO : /* Execute macro from file --------- */
				LoopMacro();
				break;
#endif

			case K_ESC :   /* Show the menu ------------------- */
				if(Menu()) {
					run = 0;
				}
				else {
					ShowFilename(); /* Refresh filename */
					RefreshAll();   /* Refresh editor box */
				}
				break;
		}
	}
}

/* Go one line up
   --------------
*/
LoopUp()
{
	--lp_cur; // FIXME -- check if we are on the 1st line?

	if(box_shr)
		--box_shr;
	else
		Refresh(0, lp_cur);
}

/* Go one line down
   ----------------
*/
LoopDown()
{
	++lp_cur; // FIXME -- check if we are on the last line?

	if(box_shr < box_rows - 1)
		++box_shr;
	else
		Refresh(0, lp_cur - box_rows + 1);
}

/* Go to document top
   ------------------
*/
LoopTop()
{
	int first;

	first = GetFirstLine();

	lp_cur = box_shr = box_shc = 0;

	if(first > 0) {
		RefreshAll();
	}
}

/* Go to document bottom
   ---------------------
*/
LoopBottom()
{
	int first, last;

	first = GetFirstLine();
	last = GetLastLine();

	lp_cur = lp_now - 1;
	box_shc = 999;

	if(last < lp_now - 1) {
		box_shr = box_rows - 1;
		RefreshAll();
	}
	else {
		box_shr = last - first;
	}
}

/* Page up
   -------
*/
LoopPgUp()
{
	int first, to;

	first = GetFirstLine();

	if(first)
	{
		if((to = first - box_rows) < 0) /* max(to, 0) */
			to = 0;

		lp_cur = to; box_shr = box_shc = 0;

		RefreshAll();
	}
	else
		LoopTop();
}

/* Page down
   ---------
*/
LoopPgDown()
{
	int to;

	if(GetLastLine() < lp_now - 1)
	{
		to = GetFirstLine() + box_rows;

		if(to >= lp_now)
			to = lp_now - 1;

		lp_cur = to; box_shr = box_shc = 0;

		RefreshAll();
	}
	else
		LoopBottom();
}

/* Insert CR (intro)
   -----------------
*/
LoopIntro()
{
	int ok, rs;

	if(box_shc) {
		if(ln_dat[box_shc]) {
			/* Cursor is in the middle of the line */
			if((ok = SplitLine(lp_cur, box_shc))) {
				rs = 1;
			}
		}
		else {
			/* Cursor is at the end of the line */
			if((ok = AppendLine(lp_cur, NULL))) {
				rs = 0;
			}
		}
	}
	else {
		/* Cursor is in first column */
		if((ok = InsertLine(lp_cur, NULL))) {
			rs = 1;
		}
	}

	if(ok) {
		++lp_cur;

		if(box_shr < box_rows - 1) {

			++box_shr;

			Refresh(box_shr - rs, lp_cur - rs);
		}
		else {
			Refresh(0, lp_cur - box_rows + 1);
		}

		box_shc = 0;

		lp_chg = 1;
	}
}

#if OPT_BLOCK

/* Set block start
   ---------------
*/
LoopBlkStart()
{
	if(blk_start != -1 || (blk_end != -1 && lp_cur > blk_end)) {
		LoopBlkUnset();
	}

	blk_start = lp_cur;

	if(blk_end != -1) {
		RefreshBlock(box_shr, 1);

		blk_count = blk_end - blk_start + 1;
	}
}

/* Set block end
   -------------
*/
LoopBlkEnd()
{
	if(blk_end != -1 || (blk_start != -1 && lp_cur < blk_start) ) {
		LoopBlkUnset();
	}

	blk_end = lp_cur;

	if(blk_start != -1) {
		RefreshBlock(0, 1);

		blk_count = blk_end - blk_start + 1;
	}
}

/* Unset block
   -----------
*/
LoopBlkUnset()
{
	if(blk_count) {
		if(blk_start <= GetLastLine() && blk_end >= GetFirstLine()) {
			RefreshBlock(0, 0);  // FIXME -- optimize
		}
	}

	blk_start = blk_end = -1;
	blk_count = 0;
}

#endif

/* Copy line
   ---------
*/
LoopCopy()
{

#if OPT_BLOCK

	if(LoopCopyEx()) {
		LoopBlkUnset();
	}

#else

	strcpy(clp_line, ln_dat);

#endif

}

#if OPT_BLOCK

LoopCopyEx()
{
	int i;

	if(!blk_count) {
		blk_start = blk_end = lp_cur;
		blk_count = 1;
	}

	LoopClrClp();

	if((clp_arr = AllocMem(blk_count * SIZEOF_PTR))) {
		for(i = 0; i < blk_count; ++i) {
			if((clp_arr[i] = AllocMem(strlen(lp_arr[blk_start + i]) + 1))) {
				strcpy(clp_arr[i], lp_arr[blk_start + i]);
			}
			else {
				FreeArray(clp_arr, i, 1);

				return 0;
			}
		}

		clp_count = blk_count;

		return 1;
	}

	return 0;
}

#endif

/* Delete line
   -----------
*/
LoopDelete()
{

#if OPT_BLOCK

	if(blk_count) {
		LoopGo(blk_start);

		while(blk_count--) {
			if(blk_start != lp_now - 1) {
				DeleteLine(blk_start);
			}
			else {
				ClearLine(blk_start);
			}

			--blk_end;

			Refresh(box_shr, lp_cur);
		}

		blk_start = blk_end = -1;
		blk_count = 0;

		box_shc = 0;

		lp_chg = 1;
	}

#else

	if(lp_cur != lp_now - 1) {
			DeleteLine(lp_cur);
	}
	else {
		ClearLine(lp_cur);
	}

	Refresh(box_shr, lp_cur);

	box_shc = 0;

	lp_chg = 1;

#endif

}

/* Copy and delete line
   --------------------
*/
LoopCut()
{

#if OPT_BLOCK

	if(LoopCopyEx()) {
		LoopDelete();
	}

#else

	strcpy(clp_line, ln_dat);

	LoopDelete();

#endif

}

/* Paste line
   ----------
*/
LoopPaste()
{

#if OPT_BLOCK

	int i;

	if(clp_count) {
		for(i = 0; i < clp_count; ++i) {
			if(InsertLine(lp_cur, clp_arr[i])) {
				Refresh(box_shr, lp_cur);
				LoopDown();
			}
			else {
				break;
			}
		}

		box_shc = 0;

		lp_chg = 1;
	}

#else

	if((InsertLine(lp_cur, clp_line))) {
		Refresh(box_shr, lp_cur);
		LoopDown();

		box_shc = 0;

		lp_chg = 1;

	}

#endif

}

/* Clear the clipboard
   -------------------
*/
LoopClrClp()
{

#if OPT_BLOCK

	if(clp_count) {
		FreeArray(clp_arr, clp_count, 1);

		clp_count = 0;
	}

#else

	*clp_line = '\0';

#endif

}

/* Delete CR on the left
   ---------------------
*/
LoopLeftDel()
{
	char *p;
	int ok, rs, pos;

	p = lp_arr[lp_cur];

	if(*p) {
		p = lp_arr[lp_cur - 1];

		if(*p) {
			pos = strlen(p);

			if((ok = JoinLines(lp_cur - 1))) {
				rs = 0;
			}
		}
		else {
			if((ok = DeleteLine(lp_cur - 1))) {
				rs = 0;
				pos = 0;
			}
		}
	}
	else {
		if((ok = DeleteLine(lp_cur))) {
			rs = 1;
			pos = 999;
		}
	}

	if(ok) {

		--lp_cur;

		if(box_shr)	{

			--box_shr;

			Refresh(box_shr + rs, lp_cur + rs);
		}
		else {
			Refresh(0, lp_cur);
		}

		box_shc = pos;

		lp_chg = 1;
	}
}

/* Delete CR on the right
   ----------------------
*/
LoopRightDel()
{
	char *p;
	int ok, rs;

	p = lp_arr[lp_cur];

	if(*p) {
		p = lp_arr[lp_cur + 1];

		if(*p) {
			if((ok = JoinLines(lp_cur))) {
				rs = 0;
			}
		}
		else {
			if((ok = DeleteLine(lp_cur + 1))) {
				rs = 1;
			}
		}
	}
	else {
		if((ok = DeleteLine(lp_cur))) {
			rs = 0;
		}
	}

	if(ok) {
		if(box_shr + rs < box_rows) {
			Refresh(box_shr + rs, lp_cur + rs);
		}

		lp_chg = 1;
	}
}

#if OPT_FIND

/* Find string
   -----------
*/
LoopFind()
{
	int slen, flen, line, row, col, i;
	char *p;

	row  = box_shr;
	col  = box_shc;
	flen = strlen(find_str);

	for(line = lp_cur; line < lp_now; ++line)
	{
		for(slen = strlen((p = lp_arr[line])); flen <= (slen - col) && col < slen; ++col)
		{
			for(i = 0; i < flen && find_str[i] == p[col + i]; ++i)
				;

			if(i == flen)
			{
				/* Found, set new cursor position and refresh the screen if needed */
				lp_cur = line;
				box_shc = col;

				if(row < box_rows)
					box_shr = row;
				else
					Refresh((box_shr = 0), lp_cur);

				return 1;
			}
		}

		++row; col = 0;
	}

	/* Not found */
	return 0;
}

/* Find first string
   -----------------
*/
LoopFindFirst()
{
	find_str[0] = '\0';

	if(SysLineStr("Find", find_str, FIND_MAX - 1))
	{
		LoopFind();
	}
}

/* Find next string
   ----------------
*/
LoopFindNext()
{
	int old_box_shc;

	if(find_str[0])
	{
		old_box_shc = box_shc;

		/* Skip current character */
		if(box_shc < strlen(lp_arr[lp_cur]))
			++box_shc;

		/* Set old cursor position on find failure */
		if(!LoopFind())
			box_shc = old_box_shc;
	}
}

#endif

#if OPT_GOTO

/* Go to line # (1..X)
   -------------------
*/
LoopGoLine()
{
	char buf[6];
	int line, first, last;

	buf[0] = '\0';

	if(SysLineStr("Go to line #", buf, 5))
	{
		line = atoi(buf);

		if(line > 0 && line <= lp_now)
		{
			/*
			first = GetFirstLine();
			last = GetLastLine();

			lp_cur = line - 1;
			box_shc = 0;

			if(lp_cur >= first && lp_cur <= last)
				box_shr = lp_cur - first;
			else
				Refresh((box_shr = 0), lp_cur);
			*/

			LoopGo(line - 1);
		}
	}
}

#endif

/* Go to line # (0..X)
   -------------------
*/
LoopGo(line)
int line;
{
	int first, last;

	first = GetFirstLine();
	last = GetLastLine();

	lp_cur = line;
	box_shc = 0;

	if(lp_cur >= first && lp_cur <= last)
		box_shr = lp_cur - first;
	else
		Refresh((box_shr = 0), lp_cur);
}

#if OPT_MACRO

/* Execute macro from file
   -----------------------
*/
LoopMacro()
{
	char fn[FILENAME_MAX];

	fn[0] = '\0';

	if(SysLineStr("Macro filename", fn, FILENAME_MAX - 1))
	{
		MacroRunFile(fn);
	}
}

#endif

