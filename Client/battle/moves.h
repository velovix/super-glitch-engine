#ifndef MOVES_H
#define MOVES_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../constants.h"

typedef struct {
	int type;
	int target;
	int value;
} moveEvent_t;

typedef struct {
	int bpp, type, power;
	char name [12];
	moveEvent_t *events;
	int eventCnt;
} move_t;

// Initializers
move_t pk_initMove(int s_cpp, int s_bpp, char* name, int type);

bool pk_parseMoveScript(char *script, int scriptLen, move_t *obj);

#endif