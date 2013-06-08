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
SDL_Surface* s_npc = NULL;

SDL_Event event;

SDL_Rect clipTiles[50];

int curX, curY;
int lastCurX, lastCurY;
char curVal;
int camX, camY;
int width, height;
int currRoom;
int origRoom = 0;
bool chooseRoom = false;
mapData_t map[30];

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
	s_npc = load_image("resources/npc.png");
	if(s_npc == NULL) {
		printf("[ERROR] Could not find resources/npc.png!\n");
	}
}

void fillRoom(char tile) {
	for(int i=0; i<MAX_ROOMS; i++) {
		for(int j=0; j<map[currRoom].w*map[currRoom].h; j++) {
			map[i].data[j] = tile;
		}
	}
}

int hasDoor(int x, int y, int r) {
	for(int i=0; i<map[r].doorCnt; i++) {
		if(x == map[r].doorData[i].x && y == map[r].doorData[i].y) {
			return i;
		}
	}

	return -1;
}

void saveMap() {
	FILE* file;
	file = fopen("map.pke", "w");
	if(file == NULL) {
		printf("[ERROR] Could not create map.pke!\n");
	}

	printf("Saving map...\n");

	mapHeader_f mapHeader;
	mapHeader.version = CURRVERSION;
	mapHeader.count = MAX_ROOMS;
	fwrite(&mapHeader.version, sizeof(mapHeader.version), 1, file);
	fwrite(&mapHeader.count, sizeof(mapHeader.count), 1, file);
	printf("   Wrote %i rooms\n", mapHeader.count);

	roomHeader_f roomHeader[MAX_ROOMS];
	for(int i=0; i<MAX_ROOMS; i++) {
		roomHeader[i].value = i;
		roomHeader[i].music = map[i].music;
		roomHeader[i].w = map[i].w; roomHeader[i].h = map[i].h;
		fwrite(&roomHeader[i].value, sizeof(char), 1, file);
		fwrite(&roomHeader[i].music, sizeof(int), 1, file);
		fwrite(&roomHeader[i].w, sizeof(int), 1, file);
		fwrite(&roomHeader[i].h, sizeof(int), 1, file);
		for(int j=0; j<map[i].w*map[i].h; j++) {
			fwrite(&map[i].data[j], sizeof(char), 1, file);
		}

		fwrite(&map[i].doorCnt, sizeof(int), 1, file);
		for(int j=0; j<map[i].doorCnt; j++) {
			fwrite(&map[i].doorData[j].dest, sizeof(char), 1, file);
			fwrite(&map[i].doorData[j].x, sizeof(int), 1, file);
			fwrite(&map[i].doorData[j].y, sizeof(int), 1, file);
			fwrite(&map[i].doorData[j].destX, sizeof(int), 1, file);
			fwrite(&map[i].doorData[j].destY, sizeof(int), 1, file);
		}

		fwrite(&map[i].npcCnt, sizeof(int), 1, file);
		for(int j=0; j<map[i].npcCnt; j++) {
			fwrite(&map[i].npcData[j].val, sizeof(int), 1, file);
			fwrite(&map[i].npcData[j].x, sizeof(int), 1, file);
			fwrite(&map[i].npcData[j].y, sizeof(int), 1, file);
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
	fread(&header.version, sizeof(header.version), 1, file);
	if(header.version != CURRVERSION) {
		printf("[ERROR] File is too old/new!\n");
	}
	fread(&header.count, sizeof(header.count), 1, file);
	printf("   Found %i rooms\n", header.count);

	roomHeader_f roomHeader[header.count];
	for(int i=0; i<header.count; i++) {
		map[i].doorCnt = 0;
		fread(&roomHeader[i].value, sizeof(char), 1, file);
		fread(&roomHeader[i].music, sizeof(int), 1, file);
		fread(&roomHeader[i].w, sizeof(int), 1, file);
		fread(&roomHeader[i].h, sizeof(int), 1, file);
		for(int j=0; j<roomHeader[i].w*roomHeader[i].h; j++) {
			fread(&map[i].data[j], sizeof(char), 1, file);
		}

		map[i].w = roomHeader[i].w;
		map[i].h = roomHeader[i].h;

		fread(&map[i].doorCnt, sizeof(int), 1, file);
		for(int j=0; j<map[i].doorCnt; j++) {
			fread(&map[i].doorData[j].dest, sizeof(char), 1, file);
			fread(&map[i].doorData[j].x, sizeof(int), 1, file);
			fread(&map[i].doorData[j].y, sizeof(int), 1, file);
			fread(&map[i].doorData[j].destX, sizeof(int), 1, file);
			fread(&map[i].doorData[j].destY, sizeof(int), 1, file);
		}

		fread(&map[i].npcCnt, sizeof(int), 1, file);
		for(int j=0; j<map[i].npcCnt; j++) {
			fread(&map[i].npcData[j].val, sizeof(int), 1, file);
			fread(&map[i].npcData[j].x, sizeof(int), 1, file);
			fread(&map[i].npcData[j].y, sizeof(int), 1, file);
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
			if(curVal < CLIFF_MIDDLE+15) {
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
				int orDoor = hasDoor(lastCurX, lastCurY, origRoom);
				door_t* door;
				if(orDoor == -1) {
					door = &map[origRoom].doorData[map[origRoom].doorCnt];
					printf("   Created door %i\n", map[origRoom].doorCnt);
					map[origRoom].doorCnt++;
				} else {
					door = &map[origRoom].doorData[map[orDoor].doorCnt];
					printf("   Edited old door %i\n", orDoor);
				}
				door->dest = (char)currRoom;
				door->x = lastCurX;
				door->y = lastCurY;
				door->destX = curX;
				door->destY = curY;

				printf("   You chose room: %i\n", (int)map[origRoom]. \
					doorData[map[currRoom].doorCnt].dest);
				chooseRoom = false;

				currRoom = origRoom;
			} else {
				printf("Creating new door...\n");
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
			keyStatesBuf[SDLK_c] = true;
		}
	} else {
		keyStatesBuf[SDLK_c] = false;
	}

	if(keyStates[SDLK_v]) {
		if(!keyStatesBuf[SDLK_v]) {
			map[currRoom].npcCnt = 0;
			keyStatesBuf[SDLK_v] = true;
		}
	} else {
		keyStatesBuf[SDLK_v] = false;
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

	if(keyStates[SDLK_n]) {
		if(!keyStatesBuf[SDLK_n]) {
			printf("Enter the NPC value: ");
			scanf("%i", &map[currRoom].npcData[map[currRoom].npcCnt].val);
			map[currRoom].npcData[map[currRoom].npcCnt].x = curX;
			map[currRoom].npcData[map[currRoom].npcCnt].y = curY;
			map[currRoom].npcCnt++;
			keyStatesBuf[SDLK_n] = true;
		}
	} else {
		keyStatesBuf[SDLK_n] = false;
	}

	if(keyStates[SDLK_m]) {
		if(!keyStatesBuf[SDLK_m]) {
			printf("Enter the music value: ");
			scanf("%i", &map[currRoom].music);
			keyStatesBuf[SDLK_m] = true;
		}
	} else {
		keyStatesBuf[SDLK_m] = false;
	}
}

void loop() {
	for(int x=0; x<map[currRoom].w; x++) {
		for(int y=map[currRoom].h-1; y>=0; y--) {
			apply_surface(x*BLOCK_SIZE, y*BLOCK_SIZE, s_mapTile, s_screen,
				&clipTiles[map[currRoom].data[x+(y*map[currRoom].w)]], true);
		}
	}

	for(int i=0; i<map[currRoom].doorCnt; i++) {
		apply_surface(map[currRoom].doorData[i].x*BLOCK_SIZE,
			map[currRoom].doorData[i].y*BLOCK_SIZE, s_door, s_screen, 0, true);
	}

	for(int i=0; i<map[currRoom].npcCnt; i++) {
		apply_surface(map[currRoom].npcData[i].x*BLOCK_SIZE,
			map[currRoom].npcData[i].y*BLOCK_SIZE, s_npc, s_screen, 0, true);
	}

	apply_surface(curX*BLOCK_SIZE, ((map[currRoom].h)-(map[currRoom].h-curY))
		*BLOCK_SIZE, s_mapTile, s_screen, &clipTiles[curVal], true);
	apply_surface(curX*BLOCK_SIZE, ((map[currRoom].h)-(map[currRoom].h-curY))
		*BLOCK_SIZE, s_cursor, s_screen, &clipTiles[0], true);

	Uint8 *keystates = SDL_GetKeyState(NULL);
	checkKeys(keystates);
}

int main() {
	curX = curY = 1;
	camX = camY = 0;
	currRoom = 0;
	map[currRoom].w = map[currRoom].h = 16;

	for(int i=0; i<50; i++) {
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
