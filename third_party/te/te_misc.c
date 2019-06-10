/*	te_misc.c

	Miscelaneous tools and functions.

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

	13 Jan 2019 : Added AllocMem().
	19 Jan 2019 : Added FreeArray().
	30 Jan 2019 : Added MatchStr().
*/

/* Allocate memory
   ---------------
   Shows error message on failure. Returns same values as malloc(),
*/
AllocMem(bytes)
unsigned int bytes;
{
	char *p;

	if(!(p = malloc(bytes))) {
		ErrLineMem();
	}

	return p;
}

/* Free array
   ----------
   Deallocate array memory.
*/
FreeArray(arr, count, flag)
int *arr, count, flag;
{
	int i;

	for(i = 0; i < count; ++i) {
		if(arr[i]) {
			free(arr[i]);

			arr[i] = NULL;
		}
	}

	if(flag) {
		free(arr);
	}

	return NULL;
}

#if OPT_MACRO

/* Test if two strings are equal
   -----------------------------
*/
MatchStr(s1, s2)
char *s1, *s2;
{
	return !strcmp(s1, s2);
}

#endif

