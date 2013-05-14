#ifndef PLAYER_H
#define PLAYER_H

#include "../battle/monsters.h"
#include "../ai/charmover.h"

typedef struct {
	char name [12];
	int sprite, monCnt, currMon;
	bool pause;

	monster_t monsters[6];
	charMover_t mover;
} player_t;

player_t pk_pinit(int x, int y, int sprite);
void pk_psetMonster(monster_t mon, player_t* player);
void pk_pMove(int dir, bool move, player_t* player);
int pk_pgetFrame(player_t player);

#endif