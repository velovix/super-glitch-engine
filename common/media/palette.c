#include "palette.h"

void pk_initPaletteMan(SDL_Surface *palette, paletteMan_t *obj) {

	obj->s_palette = palette;

	// Cast the palette pixels
	uint32_t *pix = (uint32_t*)palette->pixels;

	// Set pattern
	obj->patternMembers = palette->w;
	obj->pattern = (uint32_t*)malloc(obj->patternMembers*sizeof(uint32_t));
	for(int i=0; i<obj->patternMembers; i++) {
		obj->pattern[i] = pix[i];
		uint8_t *pixSplit = (uint8_t*)&obj->pattern[i];
	}

	// Set palettes
	obj->paletteCnt = palette->h;
	obj->palettes = (uint32_t**)malloc(obj->paletteCnt*sizeof(uint32_t*));
	for(int i=0; i<obj->paletteCnt; i++) {
		obj->palettes[i] = (uint32_t*)malloc(obj->patternMembers*sizeof(uint32_t));
		for(int j=0; j<obj->patternMembers; j++) {
			obj->palettes[i][j] = pix[(i*4)+j];
			uint8_t *pixSplit = (uint8_t*)&obj->palettes[i][j];
		}
	}

	obj->surfaces = NULL;
	obj->surfaceCnt = 0;
}

void pk_addPaletteManPattern(int cnt, uint32_t *colors, paletteMan_t *obj) {
	obj->pattern = (uint32_t*)realloc(obj->pattern, cnt*sizeof(uint32_t));

	for(int i=0; i<cnt; i++) {
		obj->pattern[i] = colors[i];
	}

	obj->patternMembers = cnt;
}

void pk_addPaletteManPalette(uint32_t *palette, paletteMan_t *obj) {
	// Allocate more 32-bit pointer space
	obj->palettes = (uint32_t**)realloc(obj->palettes, sizeof(uint32_t*)*(obj->paletteCnt+1));

	// Allocate more 32-bit integer space
	obj->palettes[obj->paletteCnt] = (uint32_t*)malloc(obj->patternMembers*sizeof(uint32_t));

	for(int i=0; i<obj->patternMembers; i++) {
		obj->palettes[obj->paletteCnt][i] = palette[i];
	}

	obj->paletteCnt++;
}

void pk_addPaletteManSurface(SDL_Surface *surface, paletteMan_t *obj) {
	// Allocate more surface space
	obj->surfaces = (SDL_Surface**)realloc(obj->surfaces, sizeof(SDL_Surface*)*(obj->surfaceCnt+1));

	obj->surfaces[obj->surfaceCnt] = surface;

	obj->surfaceCnt++;
}

void pk_switchPalette(int paletteVal, paletteMan_t *obj) {
	// Change the pixels for every surface
	for(int i=0; i<obj->surfaceCnt; i++) {
		SDL_LockSurface(obj->surfaces[i]);

		// Cast the pixel pointer to a 32-bit int
		uint32_t *pix = (uint32_t*)obj->surfaces[i]->pixels;

		// Check the surface's pixels
		for(int j=0; j<obj->surfaces[i]->w*obj->surfaces[i]->h; j++) {
			// Compare a pixel to every pattern color
			for(int k=0; k<obj->patternMembers; k++) {
				if(pix[j] == obj->pattern[k]) {
					pix[j] = obj->palettes[paletteVal][k];
				}
			}
		}

		obj->surfaces[i]->pixels = pix;

		SDL_UnlockSurface(obj->surfaces[i]);
	}

	// Update the pattern colors
	for(int i=0; i<obj->patternMembers; i++) {
		obj->pattern[i] = obj->palettes[paletteVal][i];
	}
}
