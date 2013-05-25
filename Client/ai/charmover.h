/* Super Glitch Engine Client
 * charmover.h
 * Copyright Tyler Compton 2013
 *
 * Declares a struct charMover_t, which manages overworld movement for objects.
 */

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <stdio.h>
#include "../constants.h"
#include "../general/mapman.h"

typedef struct {
	int x, y;
	int nextX, nextY;
	int animCycle, dir;
	bool step;
} charMover_t;

void pk_initChar(int s_x, int s_y, charMover_t* obj);
void pk_moveChar(int dir, bool move, charMover_t* obj);
void pk_updateChar(charMover_t* obj);
void pk_resetChar(charMover_t* obj);
int pk_getCharFrame(int spriteOffset, charMover_t obj);
void pk_buildColMapC(charMover_t obj, map_t* map);
int pk_faceChar(charMover_t, charMover_t);
bool pk_isFinishedMoving(charMover_t);

#endif