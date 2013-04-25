#include "player.h"

player_t pk_pinit(int x, int y, int sprite) {
	player_t out;
	out.sprite = sprite;

	pk_initChar(x, y, &out.mover);

	return out;
}

void pk_psetMonster(monster_t mon, player_t* player) {
	player->monsters[player->monCnt] = mon;
	player->monCnt++;
}

int pk_pgetFrame(player_t player) {
	return pk_getCharFrame(player.sprite, player.mover);
}