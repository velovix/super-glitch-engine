#include "player.h"

player_t pk_pinit(int x, int y, int sprite) {
	player_t out;
	out.sprite = sprite;
	out.pause = false;
	out.currMon = 0;
	out.monCnt = 0;

	pk_initChar(x, y, &out.mover);

	return out;
}

void pk_psetMonster(monster_t mon, player_t* player) {
	if(player->monCnt > 5) {
		printf("[ERROR] Max monster count reached on player!\n");
		return;
	}
	player->monsters[player->monCnt] = mon;
	player->monCnt++;
}

void pk_pMove(int dir, bool move, player_t* player) {
	if(player->pause) {
		return;
	}

	pk_moveChar(dir, move, &player->mover);
}

int pk_pgetFrame(player_t player) {
	return pk_getCharFrame(player.sprite, player.mover);
}