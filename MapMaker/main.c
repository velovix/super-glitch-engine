#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "constants.h"
#include "fileheaders.h"

bool quit = false;

SDL_Surface* s_screen = NULL;
SDL_Surface* s_mapTile = NULL;
SDL_Surface* s_cursor = NULL;
SDL_Surface* s_door = NULL;

SDL_Event event;

SDL_Rect clipTiles[20];

int curX, curY;
int lastCurX, lastCurY;
char curVal;
int camX, camY;
int width, height;
int currRoom;
int origRoom = 0;
bool chooseRoom = false;
mapData_t map[20];

bool keyStatesBuf[256];

int init()
{
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1) {
		return -1;
	}

	s_screen = SDL_SetVideoMode(512, 512, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("SGE Map Maker", NULL);

	return 0;
}

SDL_Surface *load_image(char* filename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optImage = NULL;

	loadedImage = IMG_Load(filename);

	if(loadedImage != NULL) {
		optImage = SDL_DisplayFormatAlpha(loadedImage);

		SDL_FreeSurface(loadedImage);
	}

	return optImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip, bool scrolls)
{
	SDL_Rect offset;

	if(scrolls) {
		offset.x = x - camX;
		offset.y = y - camY;
	} else {
		offset.x = x;
		offset.y = y;
	}

	SDL_BlitSurface(source, clip, destination, &offset);
}

void load() {
	s_mapTile = load_image("resources/tiles.png");
	if(s_mapTile == NULL) {
		printf("[ERROR] Could not find resources/tiles.png!\n");
	}
	s_cursor = load_image("resources/cursor.png");
	if(s_cursor == NULL) {
		printf("[ERROR] Could not find resources/cursor.png!\n");
	}
	s_door = load_image("resources/door.png");
	if(s_door == NULL) {
		printf("[ERROR] Could not find resources/door.png!\n");
	}
}

void fillRoom(char tile) {
	for(int i=0; i<MAX_ROOMS; i++) {
		for(int j=0; j<map[currRoom].w*map[currRoom].h; j++) {
			map[i].data[j] = tile;
		}
	}
}

void saveMap() {
	FILE* file;
	file = fopen("map.pke", "w");
	if(file == NULL) {
		printf("[ERROR] Could not create map.pke!\n");
	}

	printf("Saving map...\n");

	mapHeader_f mapHeader;
	mapHeader.count = MAX_ROOMS;
	fwrite(&mapHeader.count, sizeof(mapHeader.count), 1, file);
	printf("   Wrote %i rooms\n", mapHeader.count);

	roomHeader_f roomHeader[MAX_ROOMS];
	for(int i=0; i<MAX_ROOMS; i++) {
		roomHeader[i].value = i;
		roomHeader[i].w = map[i].w; roomHeader[i].h = map[i].h;
		fwrite(&roomHeader[i].value, sizeof(char), 1, file);
		fwrite(&roomHeader[i].w, sizeof(int), 1, file);
		fwrite(&roomHeader[i].h, sizeof(int), 1, file);
		for(int j=0; j<map[i].w*map[i].h; j++) {
			fwrite(&map[i].data[j], sizeof(char), 1, file);
		}

		for(int j=0; j<map[i].doorCnt; j++) {
			fwrite(&map[i].doorData[j].dest, sizeof(char), 1, file);
			fwrite(&map[i].doorData[j].x, sizeof(int), 1, file);
			fwrite(&map[i].doorData[j].y, sizeof(int), 1, file);
		}
	}

	fclose(file);
}

void loadMap() {
	FILE * file;
	file = fopen("map.pke", "r");
	if(file == NULL) {
		printf("[ERROR] Couldn't find map.pke!\n");
	}

	printf("Loading map...\n");

	mapHeader_f header;
	fread(&header.count, sizeof(header.count), 1, file);
	printf("   Found %i rooms\n", header.count);

	roomHeader_f roomHeader[header.count];
	for(int i=0; i<header.count; i++) {
		map[i].doorCnt = 0;
		fread(&roomHeader[i].value, sizeof(char), 1, file);
		fread(&roomHeader[i].w, sizeof(int), 1, file);
		fread(&roomHeader[i].h, sizeof(int), 1, file);
		for(int j=0; j<roomHeader[i].w*roomHeader[i].h; j++) {
			fread(&map[i].data[j], sizeof(char), 1, file);
			if(map[i].data[j] >= 100) {
				map[i].doorCnt++;
			}
		}

		map[i].w = roomHeader[i].w;
		map[i].h = roomHeader[i].h;

		for(int j=0; j<map[i].doorCnt; j++) {
			fread(&map[i].doorData[j].dest, sizeof(char), 1, file);
			fread(&map[i].doorData[j].x, sizeof(int), 1, file);
			fread(&map[i].doorData[j].y, sizeof(int), 1, file);
		}
	}

	fclose(file);
}

void checkKeys(Uint8 *keyStates) {
	if(keyStates[SDLK_DOWN]) {
		if(!keyStatesBuf[SDLK_DOWN]) {
			curY++;
			keyStatesBuf[SDLK_DOWN] = true;
		}
	} else {
		keyStatesBuf[SDLK_DOWN] = false;
	}

	if(keyStates[SDLK_UP]) {
		if(!keyStatesBuf[SDLK_UP]) {
			curY--;
			keyStatesBuf[SDLK_UP] = true;
		}
	} else {
		keyStatesBuf[SDLK_UP] = false;
	}

	if(keyStates[SDLK_RIGHT]) {
		if(!keyStatesBuf[SDLK_RIGHT]) {
			curX++;
			keyStatesBuf[SDLK_RIGHT] = true;
		}
	} else {
		keyStatesBuf[SDLK_RIGHT] = false;
	}

	if(keyStates[SDLK_LEFT]) {
		if(!keyStatesBuf[SDLK_LEFT]) {
			curX--;
			keyStatesBuf[SDLK_LEFT] = true;
		}
	} else {
		keyStatesBuf[SDLK_LEFT] = false;
	}

	if(keyStates[SDLK_x]) {
		if(!keyStatesBuf[SDLK_x]) {
			if(curVal < GRASS_OVER) {
				curVal++;
			} else {
				curVal = GRASS;
			}
			keyStatesBuf[SDLK_x] = true;
		}
	} else {
		keyStatesBuf[SDLK_x] = false;
	}

	if(keyStates[SDLK_z]) {
		if(!keyStatesBuf[SDLK_z]) {
			map[currRoom].data[curX+(curY*map[currRoom].w)] = curVal;
			keyStatesBuf[SDLK_z] = true;
		}
	} else {
		keyStatesBuf[SDLK_z] = false;
	}

	if(keyStates[SDLK_t]) {
		if(!keyStatesBuf[SDLK_t]) {
			fillRoom(TREE);
			keyStatesBuf[SDLK_t] = true;
		}
	} else {
		keyStatesBuf[SDLK_t] = false;
	}

	if(keyStates[SDLK_d]) {
		if(!keyStatesBuf[SDLK_d]) {
			printf("Width: ");
			scanf("%d", &map[currRoom].w);
			printf("Height: ");
			scanf("%d", &map[currRoom].h);
			keyStatesBuf[SDLK_d] = true;
		}
	} else {
		keyStatesBuf[SDLK_d] = false;
	}

	if(keyStates[SDLK_p]) {
		if(!keyStatesBuf[SDLK_p]) {
			if(chooseRoom) {
				map[origRoom].doorData[map[currRoom].doorCnt].dest = (char)currRoom;
				map[origRoom].doorData[map[currRoom].doorCnt].x = curX;
				map[origRoom].doorData[map[currRoom].doorCnt].y = curY;
				printf("You chose room: %i\n", (int)map[origRoom]. \
					doorData[map[currRoom].doorCnt].dest);
				map[origRoom].doorCnt++;
				chooseRoom = false;
			
				currRoom = origRoom;
				if(map[currRoom].data[lastCurX+(lastCurY*map[currRoom].w)] < 100) {
					map[currRoom].data[lastCurX+(lastCurY*map[currRoom].w)]+=100;
				}
			} else {
				printf("Go to the desired room and press P again\n");
				chooseRoom = true;
				origRoom = currRoom;
				lastCurX = curX;
				lastCurY = curY;
			}
			keyStatesBuf[SDLK_p] = true;
		}
	} else {
		keyStatesBuf[SDLK_p] = false;
	}

	if(keyStates[SDLK_s]) {
		if(!keyStatesBuf[SDLK_s]) {
			saveMap();
			printf("Saved!\n");
			keyStatesBuf[SDLK_s] = true;
		}
	} else {
		keyStatesBuf[SDLK_s] = false;
	}

	if(keyStates[SDLK_c]) {
		if(!keyStatesBuf[SDLK_c]) {
			map[currRoom].doorCnt = 0;
			for(int i=0; i<	map[currRoom].w*map[currRoom].h; i++) {
				if(map[currRoom].data[i] >= 100) {
					map[currRoom].data[i]-=100;
				}
			}
			keyStatesBuf[SDLK_c] = true;
		}
	} else {
		keyStatesBuf[SDLK_c] = false;
	}

	if(keyStates[SDLK_l]) {
		if(!keyStatesBuf[SDLK_l]) {
			loadMap();
			printf("Loaded map.pke\n");
			keyStatesBuf[SDLK_l] = true;
		}
	} else {
		keyStatesBuf[SDLK_l] = false;
	}

	if(keyStates[SDLK_PAGEUP]) {
		if(!keyStatesBuf[SDLK_PAGEUP]) {
			if(currRoom < MAX_ROOMS) {
				currRoom++;
			}
			keyStatesBuf[SDLK_PAGEUP] = true;
		}
	} else {
		keyStatesBuf[SDLK_PAGEUP] = false;
	}

	if(keyStates[SDLK_PAGEDOWN]) {
		if(!keyStatesBuf[SDLK_PAGEDOWN]) {
			if(currRoom > 0) {
				currRoom--;
			}
			keyStatesBuf[SDLK_PAGEDOWN] = true;
		}
	} else {
		keyStatesBuf[SDLK_PAGEDOWN] = false;
	}
}

void loop() {
	for(int x=0; x<map[currRoom].w; x++) {
		for(int y=map[currRoom].h-1; y>=0; y--) {
			if(map[currRoom].data[x+(y*map[currRoom].w)] < 100) {
				apply_surface(x*BLOCK_SIZE, y*BLOCK_SIZE, s_mapTile, s_screen,
					&clipTiles[map[currRoom].data[x+(y*map[currRoom].w)]], true);
			} else {
				apply_surface(x*BLOCK_SIZE, y*BLOCK_SIZE, s_mapTile, s_screen,
					&clipTiles[map[currRoom].data[x+(y*map[currRoom].w)]-100], true);
				apply_surface(x*BLOCK_SIZE, y*BLOCK_SIZE, s_door,
					s_screen, 0, true);
			}
				
		}
	}

	apply_surface(curX*BLOCK_SIZE, ((map[currRoom].h)-(map[currRoom].h-curY))
		*BLOCK_SIZE, s_mapTile, s_screen, &clipTiles[curVal], true);
	apply_surface(curX*BLOCK_SIZE, ((map[currRoom].h)-(map[currRoom].h-curY))
		*BLOCK_SIZE, s_cursor, s_screen, &clipTiles[0], true);

	Uint8 *keystates = SDL_GetKeyState(NULL);
	checkKeys(keystates);
}

int main() {
	curX = curY = camX = camY = 0;
	currRoom = 0;
	map[currRoom].w = map[currRoom].h = 16;

	for(int i=0; i<20; i++) {
		clipTiles[i].w = clipTiles[i].h = BLOCK_SIZE;
		clipTiles[i].x = 0;
		clipTiles[i].y = i*BLOCK_SIZE;
	}

	fillRoom(CLEAR);

	printf("SGE Map Maker: Mockup Version\n");

	if(init() == -1) {
		return 1;
	}

	load();

	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = true;
			}
		}
		if(quit) {
			break;
		}

		SDL_FillRect( s_screen, &s_screen->clip_rect,
			SDL_MapRGB( s_screen->format, 0x00, 0x00, 0x00 ) );

		loop();

		SDL_Flip(s_screen);
	}
	return 0;
}
