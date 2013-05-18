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

typedef struct {
	int width, height;
	char data[32768];
	bool cData[32768];
} map_t;

typedef struct {
	bool left, right, up, down;
} col_t;

col_t pk_findCols(map_t map, int x, int y);
void pk_buildColMapM(map_t* map);
void pk_clearColMap(map_t* map);

#endif