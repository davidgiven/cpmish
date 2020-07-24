#ifndef LIBBDF_H
#define LIBBDF_H

#include <stdint.h>

typedef struct bdf BDF;
typedef struct glyph Glyph;

struct bdf
{
	int width, height;
	int ascent, descent;
	Glyph* glyphs[256];
};

struct glyph
{
	int width, height;
	int dx, dy;
	uint8_t* data;
};

extern BDF* bdf_load(const char* filename);
extern void bdf_free(BDF* bdf);

#endif

