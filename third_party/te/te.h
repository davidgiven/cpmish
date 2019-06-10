/*	te.h

	Text editor.

	Definitions.

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

	04 May 2015 : 1st version.
	12 May 2015 : v1.01
	14 May 2015 : v1.02
	15 May 2015 : v1.03
	31 Aug 2015 : v1.04
	02 Jun 2016 : v1.05 : CRT_FILE, CRT_ENGLISH, CRT_SPANISH, etc. are now out of this file.
	10 Jun 2016 : v1.06 : Removed BOX_COL. Added PS_FNAME, PS_INF, PS_LIN_CUR, PS_LIN_NOW,
	                      PS_LIN_MAX, PS_COL_CUR, PS_COL_NOW, PS_COL_MAX.
	14 Jun 2016 : v1.07 : Hack for SamaruX.
	05 Jul 2017 : v1.08
	24 Jan 2018 : v1.09
	20 Feb 2018 : v1.10 : Defines for copyright and macros.
	22 Feb 2018 : v1.11
	26 Dec 2018 : v1.12 : Added MAC_SEP.
	23 Jan 2019 : Added key function and character control codes. Added support for key binding.
	27 Jan 2019 : Added support for macros.
	29 Jan 2019 : Added K_CLRCLP. Show clipboard status on information line.
	30 Jan 2019 : Removed support for SamaruX.
	15 Feb 2019 : v1.13

	Notes:

	Change TE_VERSION as required, before compilation.
*/

/* Version
   -------
*/
#define VERSION "v1.13 / 15 Feb 2019 for CP/M"

/* Copyright
   ---------
*/
#define COPYRIGHT "(c) 2015-2019 Miguel Garcia / FloppySoftware"

/* More defs.
   ----------
*/
#define MAX_LINES  512   /* Max. # of text lines: each empty line uses 2 bytes with the Z80 */

#define TAB_COLS   8     /* How many columns has a tab. (usually 8) */

#define FORCED_MAX 128   /* Keyboard forced entry buffer size (for paste, tabs, etc.) */

#define FIND_MAX   32    /* Find string buffer size */

#define KEYS_MAX   32    /* Max. # of key bindings */

#define PS_ROW     0     /* Information position */
#define PS_FNAME   4     /* Filename - position in row */
#define PS_TXT     "--- | Lin:0000/0000/0000 Col:00/00 Len:00"  /* Information layout */
#define PS_INF     (CRT_COLS - 41)  /* Information layout - position in row */
#define PS_CLP     (CRT_COLS - 41)  /* Clipboard status */
#define PS_LIN_CUR (CRT_COLS - 31)  /* Current line # - position in row */
#define PS_LIN_NOW (CRT_COLS - 26)  /* How many lines # - position in row */
#define PS_LIN_MAX (CRT_COLS - 21)  /* Max. # of lines - position in row */
#define PS_COL_CUR (CRT_COLS - 12)  /* Current column # - position in row */
#define PS_COL_NOW (CRT_COLS -  2)  /* Line length - position in row */
#define PS_COL_MAX (CRT_COLS -  9)  /* Max. line length - position in row */

#define BOX_ROW    2        /* Editor box position */

#define getchr     GetKey   /* Get a character from the keyboard */
#define putchr     CrtOut   /* Print a character on screen */

#if OPT_MACRO

/* Macros
   ------
*/
#define MAC_START   '{'  /* Left delimiter for symbol names in macros */
#define MAC_END     '}'  /* Right delimiter for symbol names in macros */
#define MAC_SEP     ':'  /* Separator between symbol names and # of repeats */
#define MAC_ESCAPE  '\\' /* For escaped characters in macros */
#define MAC_SYM_MAX  9   /* Max. length of macro symbol name in characters + '\0' */

#endif

/* Key function codes
   ------------------
*/
#define K_UP	    1000
#define K_DOWN	    1001
#define K_LEFT	    1002
#define K_RIGHT	    1003
#define K_BEGIN	    1004
#define K_END	    1005
#define K_TOP       1006
#define K_BOTTOM    1007
#define K_PGUP	    1008
#define K_PGDOWN    1009
#define K_TAB       1010
#define K_INTRO	    1011
#define K_ESC	    1012
#define K_RDEL	    1013
#define K_LDEL      1014
#define K_CUT       1015
#define K_COPY      1016
#define K_PASTE     1017
#define K_DELETE    1018
#define K_CLRCLP    1019
#define K_FIND      1020
#define K_NEXT      1021
#define K_GOTO      1022
#define K_LWORD     1023
#define K_RWORD     1024
#define K_BLK_START 1025
#define K_BLK_END   1026
#define K_BLK_UNSET 1027
#define K_MACRO     1028

/* Control characters
   ------------------
*/
#define CTL_A 1
#define CTL_B 2
#define CTL_C 3
#define CTL_D 4
#define CTL_E 5
#define CTL_F 6
#define CTL_G 7
#define CTL_H 8
#define CTL_I 9
#define CTL_J 10
#define CTL_K 11
#define CTL_L 12
#define CTL_M 13
#define CTL_N 14
#define CTL_O 15
#define CTL_P 16
#define CTL_Q 17
#define CTL_R 18
#define CTL_S 19
#define CTL_T 20
#define CTL_U 21
#define CTL_V 22
#define CTL_W 23
#define CTL_X 24
#define CTL_Y 25
#define CTL_Z 26

#define ZERO  0
#define ESC   27
#define DEL   127

