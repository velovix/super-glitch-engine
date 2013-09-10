#ifndef PALETTE_H
#define PALETTE_H

#include <stdint.h>
#include <SDL/SDL_video.h>

typedef struct {
	int surfaceCnt;
	SDL_Surface **surfaces;

	int patternMembers;
	uint32_t *pattern;

	int paletteCnt;
	uint32_t **palettes;

	SDL_Surface *s_palette;

} paletteMan_t;

void pk_initPaletteMan(SDL_Surface *palette, paletteMan_t *obj);

void pk_addPaletteManPattern(int cnt, uint32_t *colors, paletteMan_t *obj);
void pk_addPaletteManPalette(uint32_t *pColors, paletteMan_t *obj);

void pk_addPaletteManSurface(SDL_Surface *surface, paletteMan_t *obj);

void pk_switchPalette(int paletteVal, paletteMan_t *obj);

#endif