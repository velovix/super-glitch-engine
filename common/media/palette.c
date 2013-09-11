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
	}

	// Set palettes
	obj->paletteCnt = palette->h;
	obj->palettes = (uint32_t**)malloc(obj->paletteCnt*sizeof(uint32_t*));
	for(int i=0; i<obj->paletteCnt; i++) {
		obj->palettes[i] = (uint32_t*)malloc(obj->patternMembers*sizeof(uint32_t));
		for(int j=0; j<obj->patternMembers; j++) {
			obj->palettes[i][j] = pix[(i*4)+j];
		}
	}

	// Set effect to default
	obj->effect = (int*)malloc(obj->patternMembers*sizeof(int));
	for(int i=0; i<obj->patternMembers; i++) {
		obj->effect[i] = i;
	}

	obj->surfaces = NULL;
	obj->surfaceCnt = 0;
}

void pk_addPaletteManSurface(SDL_Surface *surface, paletteMan_t *obj) {
	// Allocate more surface space
	obj->surfaces = (SDL_Surface**)realloc(obj->surfaces, sizeof(SDL_Surface*)*(obj->surfaceCnt+1));

	obj->surfaces[obj->surfaceCnt] = surface;

	obj->surfaceCnt++;
}

int pk_getPaletteEffectRealColor(int val, paletteMan_t *obj) {
	for(int i=0; i<obj->patternMembers; i++) {
		if(obj->effect[i] == val) {
			return i;
		}
	}

	return -1;
}

void pk_changePaletteEffect(int origVal, int newVal, paletteMan_t *obj) {
	if(origVal > obj->patternMembers || newVal > obj->patternMembers) {
		return;
	}

	obj->effect[origVal] = newVal;
}

void pk_resetPaletteEffect(paletteMan_t *obj) {
	for(int i=0; i<obj->patternMembers; i++) {
		obj->effect[i] = i;
	}
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
				if(pix[j] == obj->pattern[pk_getPaletteEffectRealColor(k, obj)]) {
					pix[j] = obj->palettes[paletteVal][obj->effect[k]];
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
