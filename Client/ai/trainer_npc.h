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

	int reach, sprite, aiType, actClock, dest, monCnt;
	bool fought, active, pause;

	monster_t monsters[6];
	charMover_t mover;
	window_t dialog;
} trainerNPC_t;

trainerNPC_t pk_tinit(int s_x, int s_y, int s_x2, int s_y2, int s_reach, int s_sprite,
	int s_dir, int s_aiType, bool s_fought);

void pk_tupdate(trainerNPC_t* npc, col_t col);
void pk_tsetWindow(window_t s_wind, trainerNPC_t* trainer);
void pk_tsetMonster(monster_t mon, trainerNPC_t* trainer);
int pk_tgetFrame(trainerNPC_t npc);
void pk_ttogglePause(trainerNPC_t* npc);

#endif