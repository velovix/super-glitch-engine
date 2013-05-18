#include "mapman.h"

col_t pk_findCols(map_t map, int x, int y) {

	col_t out;
	out.left = true; out.right = true; out.up = true; out.down = true;
	if(map.cData[x+((y-1)*map.width)]) {
		out.up = false;
	}
	if(map.cData[x+((y+1)*map.width)]) {
		out.down = false;
	}
	if(map.cData[x+1+(y*map.width)]) {
		out.right = false;
	}
	if(map.cData[x-1+(y*map.width)]) {
		out.left = false;
	}

	return out;
}

void pk_buildColMapM(map_t* map) {
	for(int i=0; i<map->width*map->height; i++) {
		if(map->data[i] != CLEAR && map->data[i] != DARK_DIRT && map->data[i] != GRASS) {
			map->cData[i] = true;
		} else {
			map->cData[i] = false;
		}
	}
}

void pk_clearColMap(map_t* map) {
	for(int i=0; i<map->width*map->height; i++) {
		map->cData[i] = false;
	}
}