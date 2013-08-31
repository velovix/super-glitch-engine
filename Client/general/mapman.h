/* Super Glitch Engine Client
 * mapman.h
 * Copyright Tyler Compton 2013
 *
 * Creates the map_t and col_t structs for use in managing maps and rooms.
 * the map_t struct contains data for what tiles are used in the room and the
 * colision data for that room.
 * col_t is returned by function pk_findCols and represents which directions
 * are blocked given a point on the map.
 */

#ifndef MAPMAN_H
#define MAPMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../constants.h"
#include "../../common/mapFile.h"

typedef struct {
	bool left, right, up, down;
} col_t;

typedef struct {
	int width, height;
	char *data;
	char *cData;

	// Advanced map data
	unsigned char *tileColData;
	doorEntry_t* doorData;
	npcEntry_t* npcData;
	int doorCnt, npcCnt, tileColCnt;

	// Keeps track of whether or not this map object has had data allocated
	bool doorAlloced, npcAlloced, tileColAlloced, mapAlloced;
} map_t;

void pk_initMap(int w, int h, map_t* map);
void pk_freeMap(map_t *map);
col_t pk_findCols(map_t map, int x, int y);
void pk_buildColMapM(map_t* map);
void pk_clearColMap(map_t* map);
bool pk_isSolid(int val, map_t* map);
void pk_setTileData(char *data, map_t* map);
void pk_setDoorData(int doorCnt, doorEntry_t* doorData, map_t* map);
void pk_setNpcData(int npcCnt, npcEntry_t *npcData, map_t* map);
void pk_setTileColData(int cnt, unsigned char *data, map_t* map);
doorEntry_t pk_isOnDoor(int x, int y, map_t* map);

#endif