#include "mapman.h"

void pk_loadMap(char* filename, map_t* map) {
	FILE * mFile;

	mFile = fopen(filename, "r");
	if(mFile == NULL) {
		printf("Error: Cannot load map!\n");
	}

	//fread(&map.width, 1, 4, mFile);
	//fread(&map.height, 1, 4, mFile);
	//fread(&map.data, 1, map.width*map.height, mFile);

	map->width = 34;
	map->height = 34;

	fread(map->data, 1, map->width*map->height, mFile);

	for(int i=0; i<map->width*map->height; i++) {
		switch(map->data[i]) {
			case '2':
				map->data[i] = DARK_DIRT;
				break;
			case '1':
				map->data[i] = 1;
				break;
			case '3':
				map->data[i] = TREE;
				break;
			case '5':
				map->data[i] = WATER;
				break;
			case 'c':
				map->data[i] = 12;
				break;
			case 'd':
				map->data[i] = 13;
				break;
			case 'b':
				map->data[i] = TWO_POLL;
				break;
			case '7':
				map->data[i] = 8;
				break;
			case '8':
				map->data[i] = 7;
				break;
			case '6':
				map->data[i] = 6;
				break;
			case '9':
				map->data[i] = 9;
				break;
			case 'a':
				map->data[i] = 10;
				break;
			case 'g':
				map->data[i] = GRASS;
				break;
			case 's':
				map->data[i] = SIGN;
				break;
		}
	}

	fclose(mFile);

	printf("Map loaded.\n");
}

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