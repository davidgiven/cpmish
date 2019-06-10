/*	te_lines.c

	Text editor.

	Operations with text lines.

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

	06 Jan 2019 : Extracted from te.c.
	07 Jan 2019 : Added InsertLine(), AppendLine(), SplitLine(), DeleteLine(), JoinLines().
	08 Jan 2019 : Added SetLine(), ModifyLine(), ClearLine(). Modified InsertLine().
*/

/* Return line # of first line printed on the editor box
   -----------------------------------------------------
*/
GetFirstLine()
{
	return lp_cur - box_shr;
}

/* Return line # of last line printed on the editor box
   ----------------------------------------------------
*/
GetLastLine()
{
	int last;

	last = GetFirstLine() + box_rows - 1;

	return last >= lp_now - 1 ? lp_now - 1 : last; /* min(lp_now - 1, last) */
}

/* Set text in line #
   ------------------
   Set 'text' to NULL for empty lines. Return NZ on success, else Z.
*/
SetLine(line, text, insert)
int line; char *text; int insert;
{
	char *p;
	int i;
	
	if(insert && lp_now >= MAX_LINES) {
		ErrLineTooMany();
		
		return 0;
	}

	if(!text) {
		text = "";
	}

	if((p = AllocMem(strlen(text) + 1))) {
		if(insert) {
			for(i = lp_now; i > line; --i) {
				lp_arr[i] = lp_arr[i - 1];
			}

			++lp_now;
		}
		else {
			if(lp_arr[line]) {
				free(lp_arr[line]);
			}
		}

		lp_arr[line] = strcpy(p, text);

		return 1;
	}

	return 0;
}

/* Modify text in line #
   ---------------------
	Set 'text' to NULL for empty lines. Return NZ on success, else Z.
*/

ModifyLine(line, text)
int line; char *text;
{
	return SetLine(line, text, 0);
}

/* Clear text in line #
   --------------------
   Return NZ on success, else Z.
*/

ClearLine(line)
int line;
{
	return ModifyLine(line, NULL);
}

/* Insert new line before line #
   -----------------------------
   Set 'text' to NULL for empty lines. Return NZ on success, else Z.
*/
InsertLine(line, text)
int line; char *text;
{
	return SetLine(line, text, 1);
}

/* Append new line after line #
   -----------------------------
   Set 'text' to NULL for empty lines. Return NZ on success, else Z.
*/
AppendLine(line, text)
int line; char *text;
{
	return InsertLine(line + 1, text);
}

/* Split line # into two lines
   ---------------------------
   Return NZ on success, else Z.
*/
SplitLine(line, pos)
int line, pos;
{
	char *p, *p2;

	if((p = AllocMem(pos + 1))) {
		if(AppendLine(line, lp_arr[line] + pos)) {

			/* We don't have strncpy() nor arrays of pointers yet, then... */
			p2 = lp_arr[line];

			p2[pos] = '\0';

			strcpy(p, p2);

			free(p2);

			lp_arr[line] = p;

			return 1;
		}

		free(p);
	}

	return 0;
}

/* Delete line #
   -------------
   Return NZ on success, else Z.
*/
DeleteLine(line)
int line;
{
	int i;

	free(lp_arr[line]);

	--lp_now;

	for(i = line; i < lp_now; ++i) {
		lp_arr[i] = lp_arr[i + 1];
	}

	lp_arr[lp_now] = NULL;

	return 1;
}

/* Join two consecutive lines
   --------------------------
   Return NZ on success, else Z.
*/
JoinLines(line)
int line;
{
	char *p, *p1, *p2;
	int s1, s2;

	p1 = lp_arr[line];
	p2 = lp_arr[line + 1];

	s1 = strlen(p1);
	s2 = strlen(p2);

	if(s1 + s2 <= ln_max) {
		if((p = AllocMem(s1 + s2 + 1))) {

			/*
			strcpy(p, p1); strcat(p, p2);

			lp_arr[line] = p;
			*/

			//
			lp_arr[line] = strcat(strcpy(p, p1), p2);
			//

			//lp_arr[line] = strcpy(strcpy(p, p1) + s1, p2);  FIXME - What's wrong with this?

			free(p1);

			DeleteLine(line + 1);

			return 1;
		}
	}

	return 0;
}

