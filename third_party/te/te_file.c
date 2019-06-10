/*	te_file.c

	Text editor.

	File I/O.

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
	22 Feb 2018 : Check for buffer changes.
	09 Jan 2018 : Optimize ReadFile().
	16 Jan 2019 : Added support for block selection.
	19 Jan 2019 : Optimize WriteFile().
*/

/* Reset lines array
   -----------------
*/
ResetLines()
{
	int i;

	FreeArray(lp_arr, MAX_LINES, 0);

	lp_cur = lp_now = lp_chg = box_shr = box_shc = 0;

#if OPT_BLOCK

	blk_start = blk_end = -1;
	blk_count = 0;

#endif

}

/* New file
   --------
*/
NewFile()
{
	/* Free current contents */
	ResetLines();

	/* No filename */
	file_name[0] = '\0';

	/* Build first line */
	InsertLine(0, NULL);
}

/* Read text file
   --------------
   Returns NZ on error.
*/
ReadFile(fn)
char *fn;
{
	FILE *fp;
	int ch, code, len, i, tabs, rare;
	unsigned char *p;

	/* Free current contents */
	ResetLines();

	/* Setup some things */
	code = tabs = rare = 0;

	/* Open the file */
	SysLine("Reading file.");

	if(!(fp = fopen(fn, "r")))
	{
		ErrLineOpen(); return -1;
	}

	/* Read the file */
	for(i = 0; i < 32000; ++i)
	{
		if(!fgets(ln_dat, ln_max + 2, fp)) /* ln_max + CR + ZERO */
			break;

		if(lp_now == MAX_LINES)
		{
			ErrLineTooMany(); ++code; break;
		}

		len = strlen(ln_dat);

		if(ln_dat[len - 1] == '\n')
			ln_dat[--len] = 0;
		else if(len > ln_max)
		{
			ErrLineLong(); ++code; break;
		}

		if(!(lp_arr[lp_now] = AllocMem(len + 1)))
		{
			++code; break;
		}

		p = strcpy(lp_arr[lp_now++], ln_dat);

		/* Change TABs to SPACEs, and check characters */
		//while((ch = (*p & 0xFF)))
		while((ch = *p))
		{
			if(ch < ' ')
			{
				if(ch == '\t')
				{
					*p = ' '; ++tabs;
				}
				else
				{
					*p = '?'; ++rare;
				}
			}

			++p;
		}
	}

	/* Close the file */
	fclose(fp);

	/* Check errors */
	if(code)
		return -1;

	/* Check if empty file */
	if(!lp_now)
	{
		/* Build first line */
		InsertLine(0, NULL);
	}

	/* Check TABs */
	if(tabs)
		ErrLine("Tabs changed to spaces.");

	/* Check rare chars. */
	if(rare)
		ErrLine("Illegal characters changed to '?'.");

	/* Success */
	return 0;
}

/* Backup the previous file with the same name
   -------------------------------------------
   Return NZ on error.
*/
BackupFile(fn)
char *fn;
{
	FILE *fp;
	char *bkp;

	/* Check if file exists */
	//if((fp = fopen(fn, "r")) != NULL)
	if((fp = fopen(fn, "r")))
	{
		fclose(fp);

		bkp = "te.bkp";

		/* Remove the previous backup file, if exists */
		remove(bkp);

		/* Rename the old file as backup */
		rename(fn, bkp);
	}
}

/* Write text file
   ---------------
   Returns NZ on error.
*/
WriteFile(fn)
char *fn;
{
	FILE *fp;
	int i;

	SysLine("Writing file.");

	/* Backup old file */
	BackupFile(fn);

	/* Open the file */

	if(!(fp = fopen(fn, "w")))
	{
		ErrLineOpen(); return -1;
	}

	/* Write the file */
	for(i = 0; i < lp_now; ++i)
	{
		if(fputs(lp_arr[i], fp) == EOF || fputc('\n', fp) == EOF) {

			fclose(fp); remove(fn);

			ErrLine("Can't write.");

			return -1;
		}
	}

	/* Close the file */
	if(fclose(fp) == EOF)
	{
		remove(fn);

		ErrLine("Can't close.");

		return -1;
	}

	/* Success */
	return (lp_chg = 0);
}

