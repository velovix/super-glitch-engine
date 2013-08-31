#ifndef OPENMAPFILE_H
#define OPENMAPFILE_H

#include <stdio.h>
#include <stdlib.h>

#define MAPFILE_VERSION			3

// Errors
#define PK_MF_FILE				1
#define PK_MF_BADVERSION		2
#define PK_MF_BADMOVEREQ		3

// File Headers
typedef struct {
	int version;
	int count;
	int tileColCnt;
} mapHeader_f;

typedef struct {
	char value;
	int music;
	int w, h;
	int tileColCnt, doorCnt, npcCnt;
} roomHeader_f;

typedef struct {
	char dest;
	int x,y;
	int destX, destY;
	int type;
} doorEntry_t;

typedef struct {
	char value;
	int x, y;
} npcEntry_t;

// Object Return Header
typedef struct {
	roomHeader_f header;
	char *mapData;
	doorEntry_t *doorData;
	npcEntry_t *npcData;
} roomFileObj_t;

typedef struct {
	mapHeader_f mapHeader;
	char *tileCols;
	roomFileObj_t *rooms;
} mapFile_t;

int pk_openMapFile(mapFile_t *obj, char *filename, int room);
int pk_saveMapFile(mapFile_t *obj, char *filename);

void pk_freeMapFile(mapFile_t *obj, int room);

#endif