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