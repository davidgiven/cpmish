#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "libbdf.h"

static bool startswith(const char* string, const char* prefix)
{
	return strncmp(prefix, string, strlen(prefix)) == 0;
}

BDF* bdf_load(const char* filename)
{
	BDF* bdf = calloc(1, sizeof(BDF));
	FILE* fp = fopen(filename, "rt");

	char* ptr = NULL;
	size_t size = 0;
	Glyph* glyph = NULL;
	for (;;)
	{
		if (getline(&ptr, &size, fp) == -1)
			break;

		int p1, p2, p3, p4;
		if (startswith(ptr, "STARTCHAR "))
		{
			glyph = calloc(1, sizeof(Glyph));
			glyph->data = calloc(bdf->height, sizeof(&glyph->data));
		}
		else if (sscanf(ptr, "ENCODING %d\n", &p1) == 1)
			bdf->glyphs[p1] = glyph;
		else if (sscanf(ptr, "FONTBOUNDINGBOX %d %d %d %d\n", &p1, &p2, &p3, &p4) == 4)
		{
			bdf->width = p1;
			bdf->height = p2;
		}
		else if (sscanf(ptr, "FONT_ASCENT %d\n", &p1) == 1)
			bdf->ascent = p1;
		else if (sscanf(ptr, "FONT_DESCENT %d\n", &p1) == 1)
			bdf->descent = p1;
		else if (sscanf(ptr, "BBX %d %d %d %d\n", &p1, &p2, &p3, &p4) == 4)
		{
			if (!glyph)
				goto malformed;
			glyph->width = p1;
			glyph->height = p2;
			glyph->dx = p3;
			glyph->dy = p4;
		}
		else if (strcmp(ptr, "BITMAP\n") == 0)
		{
			int yo = bdf->ascent - glyph->dy - glyph->height;

			for (int y=0; y<glyph->height; y++)
			{
				if (getline(&ptr, &size, fp) == -1)
					goto malformed;
				if (sscanf(ptr, "%x\n", &p1) != 1)
					goto malformed;
				int yy = y + yo;
				if ((yy >= 0) && (yy < bdf->height))
					glyph->data[yy] = p1 >> (glyph->dx);
			}
		}
	}
	free(ptr);

	fclose(fp);
	return bdf;

toobig:
	fprintf(stderr, "libbdf: glyphs may be a maximum of 8x8\n");
	goto error;
malformed:
	fprintf(stderr, "libbdf: malformed BDF file\n");
error:
	bdf_free(bdf);
	if (fp)
		fclose(fp);
	return NULL;
}

void bdf_free(BDF* bdf)
{
	if (bdf)
	{
		for (int i=0; i<256; i++)
		{
			Glyph* g = bdf->glyphs[i];
			if (g)
			{
				free(g->data);
				free(g);
			}
		}
		free(bdf);
	}
}

