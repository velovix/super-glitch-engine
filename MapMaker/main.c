#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "constants.h"
#include "../common/mapFile.h"

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
uint8_t curVal;
int camX, camY;
int width, height;
int currRoom;
int origRoom = 0;
bool chooseRoom = false;

mapFile_t map;

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
	s_mapTile = load_image("../resources/sprites/tiles.png");
	if(s_mapTile == NULL) {
		printf("[ERROR] Could not find resources/sprites/tiles.png!\n");
	}
	s_cursor = load_image("../resources/sprites/cursor.png");
	if(s_cursor == NULL) {
		printf("[ERROR] Could not find resources/sprites/cursor.png!\n");
	}
	s_door = load_image("../resources/sprites/door.png");
	if(s_door == NULL) {
		printf("[ERROR] Could not find resources/sprites/door.png!\n");
	}
	s_npc = load_image("../resources/sprites/npc.png");
	if(s_npc == NULL) {
		printf("[ERROR] Could not find resources/sprites/npc.png!\n");
	}
}

void fillRoom(char tile) {
	for(int j=0; j<map.rooms[currRoom].header.w*map.rooms[currRoom].header.h; j++) {
		map.rooms[currRoom].mapData[j] = tile;
	}
}

int hasDoor(int x, int y, int r) {
	for(int i=0; i<map.rooms[r].header.doorCnt; i++) {
		if(x == map.rooms[r].doorData[i].x && y == map.rooms[r].doorData[i].y) {
			return i;
		}
	}

	return -1;
}

void saveMap(char *filename) {
	int err = pk_saveMapFile(&map, filename);
	if(err != 0) {
		printf("[ERROR] Saving map file! %i\n", err);
		exit(1);
	}
}

void loadMap(char *filename) {
	pk_freeMapFile(&map, -1);
	int err = pk_openMapFile(&map, filename, -1);
	if(err != 0) {
		printf("[ERROR] Loading map file! %i\n", err);
		exit(1);
	}
}

void newMap() {
	map.mapHeader.version = MAPFILE_VERSION;
	map.mapHeader.count = 3;
	map.rooms = (roomFileObj_t*)malloc(3*sizeof(roomFileObj_t));
	for(int i=0; i<map.mapHeader.count; i++) {
		map.rooms[i].tileColData = (uint8_t*)malloc(1*sizeof(uint8_t));
		map.rooms[i].mapData = (uint8_t*)malloc(1*1*sizeof(uint8_t));
		map.rooms[i].doorData = (doorEntry_t*)malloc(1*sizeof(doorEntry_t));
		map.rooms[i].npcData = (npcEntry_t*)malloc(1*sizeof(npcEntry_t));
	}
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
			map.rooms[currRoom].mapData[curX+(curY*map.rooms[currRoom].header.w)] = curVal;
			keyStatesBuf[SDLK_z] = true;
		}
	} else {
		keyStatesBuf[SDLK_z] = false;
	}

	if(keyStates[SDLK_d]) {
		if(!keyStatesBuf[SDLK_d]) {
			printf("Width: ");
			scanf("%d", &map.rooms[currRoom].header.w);
			printf("Height: ");
			scanf("%d", &map.rooms[currRoom].header.h);

			map.rooms[currRoom].mapData = (uint8_t*)realloc(map.rooms[currRoom].mapData,
				map.rooms[currRoom].header.w*map.rooms[currRoom].header.h*sizeof(uint8_t));

			fillRoom(CLEAR);
			keyStatesBuf[SDLK_d] = true;
		}
	} else {
		keyStatesBuf[SDLK_d] = false;
	}

	if(keyStates[SDLK_p]) {
		if(!keyStatesBuf[SDLK_p]) {
			if(chooseRoom) {
				int orDoor = hasDoor(lastCurX, lastCurY, origRoom);
				// Represents the door of interest
				doorEntry_t* door;
				if(orDoor == -1) {
					// Create a new door
					// Allocate more door data
					map.rooms[origRoom].doorData = (doorEntry_t*)realloc( map.rooms[origRoom].doorData,
						(map.rooms[origRoom].header.doorCnt+1)*sizeof(doorEntry_t) );

					door = &map.rooms[origRoom].doorData[map.rooms[origRoom].header.doorCnt];
					printf("   Created door %i\n", map.rooms[origRoom].header.doorCnt);
					map.rooms[origRoom].header.doorCnt++;
				} else {
					// Change an old one
					door = &map.rooms[origRoom].doorData[orDoor];
					printf("   Edited old door %i\n", orDoor);
				}
				door->dest = (uint8_t)currRoom;
				door->x = lastCurX;
				door->y = lastCurY;
				door->destX = curX;
				door->destY = curY;

				printf("   You chose room: %i\n", (uint32_t)map.rooms[origRoom]. \
					doorData[map.rooms[currRoom].header.doorCnt-1].dest);
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
			saveMap("../resources/maps/map.pke");
			keyStatesBuf[SDLK_s] = true;
		}
	} else {
		keyStatesBuf[SDLK_s] = false;
	}

	if(keyStates[SDLK_c]) {
		if(!keyStatesBuf[SDLK_c]) {
			map.rooms[currRoom].header.doorCnt = 0;
			map.rooms[currRoom].header.npcCnt = 0;
			keyStatesBuf[SDLK_c] = true;
		}
	} else {
		keyStatesBuf[SDLK_c] = false;
	}

	if(keyStates[SDLK_v]) {
		if(!keyStatesBuf[SDLK_v]) {
			map.rooms[currRoom].header.npcCnt = 0;
			keyStatesBuf[SDLK_v] = true;
		}
	} else {
		keyStatesBuf[SDLK_v] = false;
	}

	if(keyStates[SDLK_l]) {
		if(!keyStatesBuf[SDLK_l]) {
			loadMap("../resources/maps/map.pke");
			keyStatesBuf[SDLK_l] = true;
		}
	} else {
		keyStatesBuf[SDLK_l] = false;
	}

	if(keyStates[SDLK_PAGEUP]) {
		if(!keyStatesBuf[SDLK_PAGEUP]) {
			if(currRoom < map.mapHeader.count-1) {
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
			int npcValBuffer;
			scanf("%i", &npcValBuffer);
			// Allocate more NPC space
			map.rooms[currRoom].npcData =
				(npcEntry_t*)realloc(map.rooms[currRoom].npcData, (map.rooms[currRoom].header.npcCnt+1)*sizeof(npcEntry_t) );

			map.rooms[currRoom].npcData[map.rooms[currRoom].header.npcCnt].value = (uint8_t)npcValBuffer;
			map.rooms[currRoom].npcData[map.rooms[currRoom].header.npcCnt].x = curX;
			map.rooms[currRoom].npcData[map.rooms[currRoom].header.npcCnt].y = curY;
			map.rooms[currRoom].header.npcCnt++;
			keyStatesBuf[SDLK_n] = true;
		}
	} else {
		keyStatesBuf[SDLK_n] = false;
	}

	if(keyStates[SDLK_m]) {
		if(!keyStatesBuf[SDLK_m]) {
			printf("Enter the music value: ");
			scanf("%i", &map.rooms[currRoom].header.music);
			keyStatesBuf[SDLK_m] = true;
		}
	} else {
		keyStatesBuf[SDLK_m] = false;
	}

	if(keyStates[SDLK_u]) {
		if(!keyStatesBuf[SDLK_u]) {
			bool new = true;
			for(int i=0; i<map.rooms[currRoom].header.tileColCnt; i++) {
				if(map.rooms[currRoom].mapData[curX+(curY*map.rooms[currRoom].header.w)] == 
					map.rooms[currRoom].tileColData[i]) {
					new = false;
					map.rooms[currRoom].tileColData[i] = 255;
				}
			}
			if(new) {
				printf("De-registered %i as collision\n", 
					map.rooms[currRoom].mapData[curX+(curY*map.rooms[currRoom].header.w)]);

				// Allocate more collision space
				map.rooms[currRoom].tileColData =
					(uint8_t*)realloc(map.rooms[currRoom].tileColData,
					(map.rooms[currRoom].header.tileColCnt+1)*sizeof(uint8_t));
				// Save new collision data
				map.rooms[currRoom].tileColData[map.rooms[currRoom].header.tileColCnt] =
					map.rooms[currRoom].mapData[curX+(curY*map.rooms[currRoom].header.w)];
				map.rooms[currRoom].header.tileColCnt++;
			} else {
				printf("Re-registered %i as collision\n",
					map.rooms[currRoom].mapData[curX+(curY*map.rooms[currRoom].header.w)]);
			}
			keyStatesBuf[SDLK_u] = true;
		}
	} else {
		keyStatesBuf[SDLK_u] = false;
	}
}

void loop() {
	for(int x=0; x<map.rooms[currRoom].header.w; x++) {
		for(int y=map.rooms[currRoom].header.h-1; y>=0; y--) {
			apply_surface(x*BLOCK_SIZE, y*BLOCK_SIZE, s_mapTile, s_screen,
				&clipTiles[(int)map.rooms[currRoom].mapData[x+(y*map.rooms[currRoom].header.w)]], true);
		}
	}

	for(int i=0; i<map.rooms[currRoom].header.doorCnt; i++) {
		apply_surface(map.rooms[currRoom].doorData[i].x*BLOCK_SIZE,
			map.rooms[currRoom].doorData[i].y*BLOCK_SIZE, s_door, s_screen, 0, true);
	}

	for(int i=0; i<map.rooms[currRoom].header.npcCnt; i++) {
		apply_surface(map.rooms[currRoom].npcData[i].x*BLOCK_SIZE,
			map.rooms[currRoom].npcData[i].y*BLOCK_SIZE, s_npc, s_screen, 0, true);
	}

	apply_surface(curX*BLOCK_SIZE, ((map.rooms[currRoom].header.h)-(map.rooms[currRoom].header.h-curY))
		*BLOCK_SIZE, s_mapTile, s_screen, &clipTiles[curVal], true);
	apply_surface(curX*BLOCK_SIZE, ((map.rooms[currRoom].header.h)-(map.rooms[currRoom].header.h-curY))
		*BLOCK_SIZE, s_cursor, s_screen, &clipTiles[0], true);

	Uint8 *keystates = SDL_GetKeyState(NULL);
	checkKeys(keystates);
}

int main(int argc, char** argv) {

	newMap();

	curX = curY = 1;
	camX = camY = 0;

	for(int i=0; i<50; i++) {
		clipTiles[i].w = clipTiles[i].h = BLOCK_SIZE;
		clipTiles[i].x = 0;
		clipTiles[i].y = i*BLOCK_SIZE;
	}

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
			pk_freeMapFile(&map, -1);
			break;
		}

		SDL_FillRect( s_screen, &s_screen->clip_rect,
			SDL_MapRGB( s_screen->format, 0x00, 0x00, 0x00 ) );

		loop();

		SDL_Flip(s_screen);
	}
	return 0;
}
