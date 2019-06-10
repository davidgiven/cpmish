/*	te_tak.c

	Text editor -- version for the Takeda Toshiya's CP/M emulator.

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

	Usage:

	te_tak [filename]

	Compilation:

	cc te_tak
	ccopt te_tak
	zsm te_tak
	hextocom te_tak

	Changes:

	06 May 2015 : 1st version.
	02 Jun 2016 : Minor changes.
	25 Jan 2018 : Find & find next keys.
	26 Jan 2018 : Key to execute macro from file.
	04 Feb 2018 : Key to go to line #.
	30 Dec 2018 : Refactorized i/o functions.
	15 Jan 2019 : Added CrtReverse().
	18 Jan 2019 : Added K_DELETE.
	23 Jan 2019 : Modified a lot for key bindings support.
	29 Jan 2019 : Added K_CLRCLP.

	Notes:

	For CPM.EXE / CP/M Player for Win32 console from Takeda Toshiya.

	It emulates a 25x80 VT-100.

	It needs to translate the IBM PC keyboard codes.
*/

/* Options
   -------
   Set to 1 to add the following functionalities, else 0.
*/
#define OPT_LWORD 0  /* Go to word on the left */
#define OPT_RWORD 0  /* Go to word on the right */
#define OPT_FIND  1  /* Find string */
#define OPT_GOTO  1  /* Go to line # */
#define OPT_BLOCK 1  /* Block selection */
#define OPT_MACRO 1  /* Enable macros */

/* Definitions
   -----------
*/
#define CRT_NAME "Takeda Toshiya's CP/M emulator"

#define CRT_ROWS 25       /* CRT rows */
#define CRT_COLS 80       /* CRT columns */

#define RULER_TAB    '!'  /* Ruler: Tab stop character - ie: ! */
#define RULER_CHR    '.'  /* Ruler: Character - ie: . */
#define SYS_LINE_SEP '-'  /* System line separator character - ie: - */

#define CRT_ESC_KEY "ESC" /* Escape key name */

/* Include main code
   -----------------
*/
#include "te.c"

/* Setup CRT: Used when the editor starts
   --------------------------------------
   void CrtSetup(void)
*/
CrtSetup()
{
	CrtSetupEx();

	SetKey(K_UP,        CTL_E, '\0', NULL);
	SetKey(K_DOWN,      CTL_X, '\0', NULL);
	SetKey(K_LEFT,      CTL_S, '\0', NULL);
	SetKey(K_RIGHT,     CTL_D, '\0', NULL);
	SetKey(K_BEGIN,     CTL_B, '\0', "HOME");
	SetKey(K_END,       CTL_A, '\0', "END");
	SetKey(K_TOP,       CTL_P, '\0', "CTL+HOME");
	SetKey(K_BOTTOM,    CTL_F, '\0', "CTL+END");
	SetKey(K_PGUP,      CTL_R, '\0', "PGUP");
	SetKey(K_PGDOWN,    CTL_C, '\0', "PGDN");	
	SetKey(K_TAB,       CTL_I, '\0', "TAB");
	SetKey(K_INTRO,     CTL_M, '\0', "ENTER");
	SetKey(K_ESC,       ESC,   '\0', "ESC");
	SetKey(K_RDEL,      DEL,   '\0', "DEL");
	SetKey(K_LDEL,      CTL_H, '\0', "BS");
	SetKey(K_CUT,       CTL_U, '\0', "F1");
	SetKey(K_COPY,      CTL_O, '\0', "F2");
	SetKey(K_PASTE,     CTL_W, '\0', "F3");
	SetKey(K_DELETE,    CTL_G, '\0', "F4");
	SetKey(K_CLRCLP,    CTL_T, '\0', NULL);
#if OPT_FIND
	SetKey(K_FIND,      CTL_K, '\0', NULL);
	SetKey(K_NEXT,      CTL_L, '\0', NULL);
#endif
#if OPT_GOTO
	SetKey(K_GOTO,      CTL_J, '\0', NULL);
#endif
#if OPT_LWORD	
	/*SetKey(K_LWORD,     '\0', '\0', NULL);*/
#endif
#if OPT_RWORD
	/*SetKey(K_RWORD,     '\0', '\0', NULL);*/
#endif
#if OPT_BLOCK
	SetKey(K_BLK_START, CTL_B, 'S', NULL);
	SetKey(K_BLK_END,   CTL_B, 'E', NULL);
	SetKey(K_BLK_UNSET, CTL_B, 'U', NULL);
#endif
#if OPT_MACRO
	SetKey(K_MACRO,     CTL_Y, '\0', NULL);
#endif
}

#asm
CrtSetupEx:
	ld  hl,(1)
	inc hl
	inc hl
	inc hl
	ld  de,BiosConst
	ld  bc,9
	ldir
	ret

BiosConst:  jp 0
BiosConin:  jp 0
BiosConout: jp 0
#endasm

/* Reset CRT: Used when the editor exits
   -------------------------------------
   void CrtReset(void)
*/
CrtReset()
{
}

/* Output character to the CRT
   ---------------------------
   All program output is done with this function.
   
   On '\n' outputs '\n' + '\r'.

   void CrtOut(int ch)
*/
#asm
CrtOut:
	ld   a,l
	cp   10
	jr   nz,CrtOutRaw
	ld   c,13
	call BiosConout
	ld   l,10
CrtOutRaw:
	ld   c,l
	jp   BiosConout
#endasm

/* Input character from the keyboard
   ---------------------------------
   All program input is done with this function.

   Translates the IBM PC key codes into single characters.

   int CrtIn(void)
*/
CrtIn()
{
	int ch;

	ch = CrtInEx();

	/* Translate key codes begining with 0 or 224 */

	if(!ch || ch == 224)
	{
		ch = CrtInEx();

		switch(ch)
		{
			case 72  : /* UP */
				return 5;
			case 80  : /* DOWN */
				return 24;
			case 75  : /* LEFT */
				return 19;
			case 77  : /* RIGHT */
				return 4;
			case 71  : /* HOME - INICIO */
				return 2;
			case 79  : /* END */
				return 1;
			case 73  : /* PGUP */
				return 18;
			case 81  : /* PGDN */
				return 3;
			case 83  : /* DELETE - SUPR */
				return 127;
			case 119 : /* CTL HOME */
				return 16;
			case 117 : /* CTL END */
				return 6;
			case 59  : /* F1 */
				return 21;
			case 60  : /* F2 */
				return 15;
			case 61  : /* F3 */
				return 23;
			case 62  : /* F4 */
				return 7;
			default  :
				return 0;
		}
	}

	return ch;
}

#asm
CrtInEx:
	call BiosConin
	ld h,0
	ld l,a
	ret
#endasm

/* Clear screen and send cursor to 0,0
   -----------------------------------
   void CrtClear(void)
*/
CrtClear()
{
	CrtOut(27); putstr("[1;1H"); /* Cursor to 0,0 */
	CrtOut(27); putstr("[2J");   /* Clear CRT */
}

/* Locate the cursor (HOME is 0,0)
   -------------------------------
   void CrtLocate(int row, int col)
*/
CrtLocate(row, col)
int row, col;
{
	CrtOut(27); CrtOut('[');
	putint("%d", row + 1); CrtOut(';');
	putint("%d", col + 1); CrtOut('H');
}

/* Erase line and cursor to row,0
   ------------------------------
   void CrtClearLine(int row)
*/
CrtClearLine(row)
int row;
{
	CrtLocate(row, 0); CrtClearEol();
}

/* Erase from the cursor to the end of the line
   --------------------------------------------
*/
CrtClearEol()
{
	CrtOut(27); putstr("[K");
}

/* Turn on / off reverse video
   ---------------------------
*/
CrtReverse(on)
int on;
{
	CrtOut(27); CrtOut('['); CrtOut(on ? '7' : '0'); CrtOut('m');
}

