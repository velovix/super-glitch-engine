/* Super Glitch Engine Client
 * npc.h
 * Copyright Tyler Compton 2013
 *
 * Creates a struct npc_t and provides functions for modifying it's values.
 * The npc_t class represents an overworld/battle npc and makes use of the
 * charmover_t struct for overworld movement. Signs are also traditionally
 * NPCs with no special movement AI.
 */

#ifndef TNPC_H
#define TNPC_H

#include "charmover.h"
#include "../constants.h"
#include "../general/window.h"
#include "../battle/monsters.h"
#include <stdio.h>

typedef struct {
	int destX[2];
	int destY[2];

	int reach, sprite, aiType, actClock, dest, monCnt, currMon;
	bool fought, active, pause, aggro;
	int targetX, targetY;

	monster_t monsters[6];
	charMover_t mover;
	window_t dialog;
} npc_t;

npc_t pk_initNpc(int s_x, int s_y, int s_x2, int s_y2, int s_reach, int s_sprite,
	int s_dir, int s_aiType, bool s_fought);

void pk_updateNpc(npc_t* npc, col_t col);
void pk_setNpcWindow(window_t s_wind, npc_t* trainer);
void pk_setNpcMonster(monster_t mon, npc_t* trainer);
int pk_getNpcFrame(npc_t npc);
void pk_toggleNpcPause(npc_t* npc);
bool pk_canNpcSee(int x, int y, npc_t* npc);
void pk_aggroNpc(int tX, int tY, npc_t* npc);

#endif