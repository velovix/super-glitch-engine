#ifndef OPENMAPFILE_H
#define OPENMAPFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAPFILE_VERSION			5

// Errors
#define PK_MF_FILE				1
#define PK_MF_BADVERSION		2
#define PK_MF_BADMOVEREQ		3

// File Headers
typedef struct {
	uint32_t	version;
	uint32_t	count;
} mapHeader_f;

typedef struct {
	uint8_t		value;
	uint32_t	music;
	uint32_t	palette;
	uint32_t	w, h;
	uint32_t	tileColCnt, doorCnt, npcCnt;
} roomHeader_f;

typedef struct {
	uint8_t		dest;
	uint32_t	x,y;
	uint32_t	destX, destY;
	uint32_t	type;
} doorEntry_t;

typedef struct {
	uint8_t		value;
	uint32_t	x, y;
} npcEntry_t;

// Object Return Header
typedef struct {
	roomHeader_f header;
	uint8_t 	*tileColData;
	uint8_t		*mapData;
	doorEntry_t	*doorData;
	npcEntry_t	*npcData;
} roomFileObj_t;

typedef struct {
	mapHeader_f	mapHeader;
	roomFileObj_t *rooms;
} mapFile_t;

int pk_openMapFile(mapFile_t *obj, char *filename, int room);
int pk_saveMapFile(mapFile_t *obj, char *filename);

void pk_freeMapFile(mapFile_t *obj, int room);

#endif