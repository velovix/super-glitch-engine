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

	int *effect;

} paletteMan_t;

void pk_initPaletteMan(SDL_Surface *palette, paletteMan_t *obj);

void pk_addPaletteManSurface(SDL_Surface *surface, paletteMan_t *obj);

int pk_getPaletteEffectRealColor(int val, paletteMan_t *obj);
void pk_changePaletteEffect(int origVal, int newVal, paletteMan_t *obj);
void pk_resetPaletteEffect(paletteMan_t *obj);

void pk_switchPalette(int paletteVal, paletteMan_t *obj);

#endif