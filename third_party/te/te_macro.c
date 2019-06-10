/*	te_macro.c

	Text editor.

	Macros.

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
	20 Feb 2018 : Disable code for macros from strings, for now.
	26 Dec 2018 : Allow # of repeats in macros - ie {up:12}. Rename MacroGetCh() to MacroGet().
	29 Dec 2018 : Added MacroRunning().
*/

/* Run a macro from file
   ---------------------
*/
MacroRunFile(fname)
char *fname;
{
	if(!(mac_fp = fopen(fname, "r")))
	{
		ErrLineOpen();
	}
}

/* Run a macro from string
   -----------------------
*/
/*
MacroRunStr(s)
char *s;
{
	mac_str = s;
}
*/

/* Tell if a macro is running
   --------------------------
*/
MacroRunning()
{
	return mac_fp != NULL /* || mac_str != NULL */;
}

/* Stop a macro
   ------------
*/
MacroStop()
{
	if(mac_fp)
	{
		fclose(mac_fp);
	}

	mac_fp = /*mac_str =*/ NULL;

	/* Flag end of input */
	//ForceCh(K_EOF);
}

/* Read raw character from macro input
   -----------------------------------
*/
MacroGetRaw()
{
	int ch;

	if(mac_fp)
	{
		while((ch = fgetc(mac_fp)) == '\n')
			;

		if(ch != EOF)
		{
			return ch;
		}

		MacroStop();
	}
	/*
	else if(mac_str)
	{
		if(*mac_str)
		{
			return *mac_str++;
		}

		MacroStop();
	}
	*/

	/* No character available */
	return '\0';
}

/* Process a macro input unit
   --------------------------
*/
MacroGet()
{
	int i, n, ch;
	char sym[MAC_SYM_MAX];

	/* Continue if there is a character available */
	if((ch = MacroGetRaw()))
	{
		/* Check for escaped characters */
		if(ch == MAC_ESCAPE)
		{
			if((ch = MacroGetRaw()))
			{
				ForceCh(ch);
			}
			else
			{
				/* Error: missing escaped character */
				ErrLine("Bad escape sequence.");

				MacroStop();
			}

			return;
		}

		/* Return character if it's not the start of a symbol */
		if(ch != MAC_START)
		{
			ForceCh(ch);
			return;
		}

		/* Get symbol name like {up} or {up:12} --> "up" */
		for(i = 0; isalpha(ch = MacroGetRaw()) && i < MAC_SYM_MAX - 1; ++i)
		{
			sym[i] = tolower(ch);
		}

		if(i)
		{
			/* End of symbol name */
			sym[i] = '\0';

			/* Get # of repeats if any - ie: {up:12} --> 12 */
			if(ch == MAC_SEP)
			{
				n = 0;

				while(isdigit(ch = MacroGetRaw()))
					n = n * 10 + ch - '0';

				if(n < 0 || n > FORCED_MAX)
				{
					n = 0;
				}
			}
			else
			{
				n = 1;
			}

			if(n && ch == MAC_END)
			{
				/* Do command action */
				ch = 0;

				if     (MatchStr(sym, "up"))       ch = K_UP;
				else if(MatchStr(sym, "down"))     ch = K_DOWN;
				else if(MatchStr(sym, "left"))     ch = K_LEFT;
				else if(MatchStr(sym, "right"))    ch = K_RIGHT;
				else if(MatchStr(sym, "begin"))    ch = K_BEGIN;
				else if(MatchStr(sym, "end"))      ch = K_END;
				else if(MatchStr(sym, "top"))      ch = K_TOP;
				else if(MatchStr(sym, "bottom"))   ch = K_BOTTOM;
				else if(MatchStr(sym, "intro"))    ch = K_INTRO;
				else if(MatchStr(sym, "tab"))      ch = K_TAB;
				else if(MatchStr(sym, "cut"))      ch = K_CUT;
				else if(MatchStr(sym, "copy"))     ch = K_COPY;
				else if(MatchStr(sym, "paste"))    ch = K_PASTE;
				else if(MatchStr(sym, "delete"))   ch = K_DELETE;
				else if(MatchStr(sym, "clrclip"))  ch = K_CLRCLP;

#if OPT_BLOCK
				else if(MatchStr(sym, "blkstart")) ch = K_BLK_START;
				else if(MatchStr(sym, "blkend"))   ch = K_BLK_END;
#endif

				if(ch)
				{
					while(n--)
					{
						if(ForceCh(ch))
							break;
					}

					return;
				}

				/* Special commands */
				if(MatchStr(sym, "filename"))
				{
					while(n--)
						ForceStr(CurrentFile());

					return;
				}
			}
		}

		/* Error: symbol name not found, bad formed, too large, bad # of repeats */
		ErrLine("Bad symbol.");

		MacroStop();
	}
}

