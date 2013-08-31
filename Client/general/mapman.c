#include "mapman.h"

void pk_initMap(int w, int h, map_t* map) {
	map->width = w;
	map->height = h;

	map->data = (char*)malloc(w*h*sizeof(char));
	map->cData = (char*)malloc(w*h*sizeof(char));
	map->mapAlloced = true;

	map->doorAlloced = map->npcAlloced = map->tileColAlloced = false;
}

void pk_freeMap(map_t *map) {
	if(map->mapAlloced) {
		free(map->data);
		free(map->cData);
	}

	if(map->doorAlloced) {
		free(map->doorData);
	}

	if(map->npcAlloced) {
		free(map->npcData);
	}

	if(map->tileColAlloced) {
		free(map->tileColData);
	}
}

col_t pk_findCols(map_t map, int x, int y) {
	col_t out;
	out.left = true; out.right = true; out.up = true; out.down = true;

	if(x+1 >= map.width) {
		out.right = false;
	} else if(x-1 < 0) {
		out.left = false;
	}
	if(y+1 > map.height) {
		out.down = false;
	} else if(y-1 < 0) {
		out.up = false;
	}

	if(map.cData[x+((y-1)*map.width)] == 1) {
		out.up = false;
	}
	if(map.cData[x+((y+1)*map.width)] == 1) {
		out.down = false;
	}
	if(map.cData[x+1+(y*map.width)] == 1) {
		out.right = false;
	}
	if(map.cData[x-1+(y*map.width)] == 1) {
		out.left = false;
	}

	return out;
}

bool pk_isSolid(int val, map_t* map) {

	for(int i=0; i<map->tileColCnt; i++) {
		if(val == map->tileColData[i]) {
			return 0;
		}
	}
	
	return 1;
}

void pk_buildColMapM(map_t* map) {
	for(int i=0; i<map->width*map->height; i++) {
		map->cData[i] = pk_isSolid(map->data[i], map);
	}
}

void pk_clearColMap(map_t* map) {
	for(int i=0; i<map->width*map->height; i++) {
		map->cData[i] = 0;
	}
}

void pk_setTileData(char *data, map_t* map) {
	// This should only be called after initializing the map
	for(int i=0; i<map->width*map->height; i++) {
		map->data[i] = data[i];
	}
}

void pk_setDoorData(int doorCnt, doorEntry_t* doorData, map_t* map) {
	// Frees door data if memory is already allocated
	if(map->doorAlloced) {
		free(map->doorData);
	}

	// Allocate desired data
	map->doorData = (doorEntry_t*)malloc(doorCnt*sizeof(doorEntry_t));
	map->doorAlloced = true;

	// Load in door data and add missing info
	for(int i=0; i<doorCnt; i++) {
		if(!pk_isSolid(map->data[doorData[i].x+(doorData[i].y*map->width)], map)) {
			doorData[i].type = DT_WALKINTO;
		} else {
			doorData[i].type = DT_COLLIDE;
		}
		map->doorData[i] = doorData[i];
	}

	map->doorCnt = doorCnt;
}

void pk_setNpcData(int npcCnt, npcEntry_t *npcData, map_t* map) {
	// Frees NPC data if memory is already allocated
	if(map->npcAlloced) {
		free(map->npcData);
	}

	// Allocate desired data
	map->npcData = (npcEntry_t*)malloc(npcCnt*sizeof(npcEntry_t));
	map->npcAlloced = true;

	for(int i=0; i<npcCnt; i++) {
		map->npcData[i] = npcData[i];
	}
	map->npcCnt = npcCnt;
}

void pk_setTileColData(int cnt, unsigned char *data, map_t* map) {
	// Frees Tile Collision data if memory is already allocated
	if(map->tileColAlloced) {
		free(map->tileColData);
	}

	// Allocate desired data
	map->tileColData = (char*)malloc(cnt*sizeof(char));
	map->tileColAlloced = true;

	for(int i=0; i<cnt; i++) {
		map->tileColData[i] = data[i];
	}
	map->tileColCnt = cnt;
}

doorEntry_t pk_isOnDoor(int x, int y, map_t* map) {
	for(int i=0; i<map->doorCnt; i++) {
		if(map->doorData[i].x == x && map->doorData[i].y == y) {
			return map->doorData[i];
		}
	}

	doorEntry_t out;
	out.dest = 255;
	return out;
}