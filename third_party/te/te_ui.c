/*	te_ui.c

	Text editor.

	User interface.

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

	Changes:

	30 Jan 2018 : Extracted from te.c.
	22 Feb 2018 : Ask for confirmation only if changes were not saved. INTRO equals Y on confirmation.
	16 Jan 2019 : Modified Refresh() to show block selection. Added RefreshBlock().
	19 Jan 2019 : Added ShowFilename().
	23 Jan 2019 : Refactorized MenuHelp().
	30 Jan 2019 : Added putchrx().
*/

/* Read character from keyboard
   ----------------------------
*/
/* **************************** SEE #define
getchr()
{
	return GetKey();
}
******************************* */

/* Print character on screen
   -------------------------
*/
/* **************************** SEE #define
putchr(ch)
int ch;
{
	CrtOut(ch);
}
******************************* */

/* Print character on screen X times
   ---------------------------------
*/
putchrx(ch, n)
int ch, n;
{
	while(n--) {
		putchr(ch);
	}
}

/* Print string on screen
   ----------------------
*/
putstr(s)
char *s;
{
	while(*s)
		putchr(*s++);
}

/* Print string + '\n' on screen
   -----------------------------
*/
putln(s)
char *s;
{
	putstr(s); putchr('\n');
}

/* Print number on screen
   ----------------------
*/
putint(format, value)
char *format; int value;
{
	char r[7]; /* -12345 + ZERO */

	sprintf(r, format, value);

	putstr(r);
}

/* Print program layout
   --------------------
*/
Layout()
{
	int i, k;

	/* Clear screen */
	CrtClear();

	/* Header */
	putln("te:");

	/* Information layout */
	CrtLocate(PS_ROW, PS_INF); putstr(PS_TXT);

	/* Max. # of lines */
	CrtLocate(PS_ROW, PS_LIN_MAX); putint("%04d", MAX_LINES);

	/* # of columns */
	CrtLocate(PS_ROW, PS_COL_MAX); putint("%02d", CRT_COLS);

	/* Ruler */
	CrtLocate(BOX_ROW - 1, 0);

	for(i = k = 0; i < CRT_COLS; ++i)
	{
		if(k++)
		{
			putchr(RULER_CHR);

			if(k == TAB_COLS)
				k = 0;
		}
		else
			putchr(RULER_TAB);
	}

	/* System line separator */
	CrtLocate(CRT_ROWS - 2, 0);

	/*
	for(i = CRT_COLS; i; --i)
		putchr(SYS_LINE_SEP);
	*/

	putchrx(SYS_LINE_SEP, CRT_COLS);
}

/* Print filename
   --------------
*/
ShowFilename()
{
	char *s;

	CrtLocate(PS_ROW, PS_FNAME);

	putstr((s = CurrentFile()));

	putchrx(' ', FILENAME_MAX - strlen(s) - 1);
}

/* Print message on system line
   ----------------------------
   Message can be NULL == blank line / clear system line.
*/
SysLine(s)
char *s;
{
	CrtClearLine(CRT_ROWS - 1);

	if(s)
		putstr(s);

	/* Set flag for Loop() */
	sysln = 1;
}

/* Print message on system line and wait
   for a key press
   -------------------------------------
   Message can be NULL.
*/
SysLineKey(s)
char *s;
{
	SysLine(s);

	if(s)
		putchr(' ');

	putstr("Press ANY key: "); getchr();

	SysLine(NULL);
}

/* Print message on system line and wait
   for confirmation
   -------------------------------------
   Message can be NULL. Returns NZ if YES, else Z.
*/
SysLineConf(s)
char *s;
{
	int ch;

	SysLine(s);

	if(s)
		putchr(' ');

	putstr("Confirm Y/N: ");

	ch = toupper(getchr());

	SysLine(NULL);

	return (ch == 'Y' || ch == K_INTRO);
}

/* Ask for a string
   ----------------
   Return NZ if entered, else Z.
*/
SysLineStr(what, buf, maxlen)
char *what, *buf; int maxlen;
{
	int ch;

	SysLine(what);
	putstr(" ([");
	putstr(CRT_ESC_KEY);
	putstr("] = cancel): ");

	ch = ReadLine(buf, maxlen);

	SysLine(NULL);

	if(ch == K_INTRO && *buf)
			return 1;

	return 0;
}

/* Ask for a filename
   ------------------
   Return NZ if entered, else Z.
*/
SysLineFile(fn)
char *fn;
{
	return SysLineStr("Filename", fn, FILENAME_MAX - 1);
}

/* Ask for confirmation on changes not saved
   -----------------------------------------
   Returns NZ if YES, else Z.
*/
SysLineChanges()
{
	return SysLineConf("The changes will be lost!");
}

/* Read simple line
   ----------------
   Returns last character entered: INTRO or ESC.
*/
ReadLine(buf, width)
char *buf;
int width;
{
	int len;
	int ch;

	putstr(buf); len=strlen(buf);

	while(1)
	{
		switch((ch = getchr()))
		{
			case K_LDEL :
				if(len)
				{
					putchr('\b'); putchr(' '); putchr('\b');

					--len;
				}
				break;
			case K_INTRO :
			case K_ESC :
				buf[len] = 0;
				return ch;
			default :
				if(len < width && ch >= ' ')
					putchr(buf[len++] = ch);
				break;
		}
	}
}

/* Return name of current file
   ---------------------------
*/
CurrentFile()
{
	return (file_name[0] ? file_name : "-");
}

/* Clear the editor box
   --------------------
*/
ClearBox()
{
	int i;

	for(i = 0; i < box_rows; ++i)
		CrtClearLine(BOX_ROW + i);
}

/* Print centered text on the screen
   ---------------------------------
*/
CenterText(row, txt)
int row; char *txt;
{
	CrtLocate(row, (CRT_COLS - strlen(txt)) / 2);

	putstr(txt);
}

#if OPT_BLOCK

/* Refresh block selection in editor box
   -------------------------------------
   Set 'sel' to NZ for reverse print, else Z for normal print.
*/
RefreshBlock(row, sel)
int row, sel;
{
	int i, line;

	line = GetFirstLine() + row;

	for(i = row; i < box_rows; ++i) {
		if(line >= blk_start) {
			if(line <= blk_end) {
				CrtClearLine(BOX_ROW + i);

				if(sel) {
					CrtReverse(1);
				}

				putstr(lp_arr[line]); putchr(' ');

				if(sel) {
					CrtReverse(0);
				}
			}
			else {
				break;
			}
		}

		++line;
	}
}

#endif

/* Refresh editor box
   ------------------
   Starting from box row 'row', line 'line'.
*/
Refresh(row, line)
int row, line;
{
	int i;

#if OPT_BLOCK

	int blk, sel;

	blk = (blk_count && blk_start <= GetLastLine() && blk_end >= GetFirstLine());
	sel = 0;

#endif

	for(i = row; i < box_rows; ++i)
	{
		CrtClearLine(BOX_ROW + i);

		if(line < lp_now) {

#if OPT_BLOCK

			if(blk) {
				if(line >= blk_start) {
					if(line <= blk_end) {
						CrtReverse((sel = 1));
					}
				}
			}

#endif

			putstr(lp_arr[line++]);

#if OPT_BLOCK

			if(sel) {
				putchr(' ');

				CrtReverse((sel = 0));
			}

#endif

		}
	}
}

/* Refresh editor box
   ------------------
*/
RefreshAll()
{
	Refresh(0, lp_cur - box_shr);
}

/* Show the menu
   -------------
   Return NZ to quit program.
*/
Menu()
{
	int run, row, stay, menu, ask;

	/* Setup some things */
	run = stay = menu = ask = 1;

	/* Loop */
	while(run)
	{
		/* Show the menu */
		if(menu)
		{
			row = BOX_ROW + 1;

			ClearBox();

			CenterText(row++, "OPTIONS");
			row++;
			CenterText(row++, "New");
			CenterText(row++, "Open");
			CenterText(row++, "Save");
			CenterText(row++, "save As");
			CenterText(row++, "Help");
			CenterText(row++, "aBout te");
			CenterText(row++, "eXit te");

			menu = 0;
		}

		/* Ask for option */
		if(ask)
		{
			SysLine("Enter option ([");
			putstr(CRT_ESC_KEY);
			putstr("] = return): ");
		}
		else
		{
			ask = 1;
		}

		/* Do it */
		switch(toupper(getchr()))
		{
			case 'N'   : run = MenuNew(); break;
			case 'O'   : run = MenuOpen(); break;
			case 'S'   : run = MenuSave(); break;
			case 'A'   : run = MenuSaveAs(); break;
			case 'B'   : MenuAbout(); menu = 1; break;
			case 'H'   : MenuHelp(); menu = 1; break;
			case 'X'   : run = stay = MenuExit(); break;
			case K_ESC : run = 0; break;
			default    : ask = 0; break;
		}
	}

	/* Clear editor box */
	ClearBox();

	SysLine(NULL);

	/* Return NZ to quit the program */
	return !stay;
}

/* Menu option: New
   ----------------
   Return Z to quit the menu.
*/
MenuNew()
{
	if(lp_chg)
	{
		if(!SysLineChanges())
			return 1;
	}

	NewFile();

	return 0;
}

/* Menu option: Open
   -----------------
   Return Z to quit the menu.
*/
MenuOpen()
{
	char fn[FILENAME_MAX];

	if(lp_chg)
	{
		if(!SysLineChanges())
			return 1;
	}

	fn[0] = 0;

	if(SysLineFile(fn))
	{
		if(ReadFile(fn))
			NewFile();
		else
			strcpy(file_name, fn);

		return 0;
	}

	return 1;
}

/* Menu option: Save
   -----------------
   Return Z to quit the menu.
*/
MenuSave()
{
	if(!file_name[0])
		return MenuSaveAs();

	WriteFile(file_name);

	return 1;
}

/* Menu option: Save as
   --------------------
   Return Z to quit the menu.
*/
MenuSaveAs()
{
	char fn[FILENAME_MAX];

	strcpy(fn, file_name);

	if(SysLineFile(fn))
	{
		if(!WriteFile(fn))
			strcpy(file_name, fn);

		return 0;
	}

	return 1;
}

/* Menu option: Help
   -----------------
*/
/*
MenuHelp()
{
	int i, k;
	char *s;

	ClearBox();

	CrtLocate(BOX_ROW + 1, 0);

	putstr("HELP for te & "); putstr(CRT_NAME); putln(":\n");

	for(i = k = 0; i < KEYS_MAX; ++i) {
		if(keys[i]) {
			putstr((s = GetKeyWhat(i)));

			putchrx(' ', 10 - strlen(s));

			if(keys[i] < 32) {
				putchr('^'); putchr('@' + keys[i]);
			}
			else {
				putint("%02x", keys[i]);
			}

			putchr(keys_ex[i] ? keys_ex[i] : ' ');

			putchr(' ');

			if(++k < 4) {
				putstr("| ");
			}
			else {
				k = 0;
				putchr('\n');
			}
		}
	}

	SysLineKey(NULL);
}
*/
MenuHelp()
{
	int i, k;
	char *s;

	ClearBox();

	CrtLocate(BOX_ROW + 1, 0);

	putstr("HELP for te & "); putstr(CRT_NAME); putln(":\n");

	for(i = 0; help_items[i] != -1; ++i) {

		// 12345678 123 12345678 (21 characters)
		// BlkEnd   ^BE RETURN

		if((k = help_items[i])) {
			if(*(s = GetKeyWhat(k)) == '?') {
				k = 0;
			}
		}

		if(k) {
			putstr(s); putchrx(' ', 9 - strlen(s));

			k -= 1000;

			if(keys[k] < 32) {
				putchr('^'); putchr('@' + keys[k]);
			}
			else {
				putint("%02x", keys[k]);
			}

			putchr(keys_ex[k] ? keys_ex[k] : ' ');

			putchr(' ');

			s = (keys_name[k] ? keys_name[k] : "");

			putstr(s); putchrx(' ', 8 - strlen(s));
		}
		else {
			putchrx(' ', 21);
		}

		if((i + 1) % 3) {
			putstr(" | ");
		}
		else {
			putchr('\n');
		}
	}

	SysLineKey(NULL);
}

/* Menu option: About
   ------------------
*/
MenuAbout()
{
	int row;

	row = BOX_ROW + 1;

	ClearBox();

	CenterText(row++, "te - Text Editor");
	row++;
	CenterText(row++, VERSION);
	row++;
	CenterText(row++, "Configured for");
	CenterText(row++, CRT_NAME);
	row++;
	CenterText(row++, COPYRIGHT);
	row++;
	CenterText(row++, "www.floppysoftware.es");
	CenterText(row++, "cpm-connections.blogspot.com");
	CenterText(row++, "floppysoftware@gmail.com");

	SysLineKey(NULL);
}

/* Menu option: Quit program
   -------------------------
*/
MenuExit()
{
	if(lp_chg)
	{
		return !SysLineChanges();
	}

	/* Quit program */
	return 0;
}

