#include "npc.h"

npc_t pk_initNpc(int s_x, int s_y, int s_x2, int s_y2, int s_reach, int s_sprite,
	int s_dir, int s_aiType, bool s_fought) {

	npc_t out;

	out.destX[0] = s_x;
	out.destY[0] = s_y;
	out.destX[1] = s_x2;
	out.destY[1] = s_y2;

	out.reach = s_reach;
	out.sprite = s_sprite;
	out.mover.dir = s_dir;
	out.aiType = s_aiType;
	out.actClock = out.dest = 0;
	out.fought = s_fought;

	out.active = true;
	out.pause = false;

	pk_initChar(s_x, s_y, &out.mover);

	return out;
}

void pk_updateNpc(npc_t* npc, col_t col) {
	if(npc->mover.animCycle > CHAR_SPEED) {
		npc->mover.animCycle = 0;
	}

	switch(npc->aiType) {
	case AI_TURN:
		if(npc->actClock <= 0  && !npc->pause) {
			if(npc->mover.dir == 2) {
				npc->mover.dir = UP;
			} else if(npc->mover.dir == 3) {
				npc->mover.dir = RIGHT;
			} else if(npc->mover.dir == 1) {
				npc->mover.dir = DOWN;
			} else if(npc->mover.dir == 4) {
				npc->mover.dir = LEFT;
			}
			npc->actClock = 50;
		} else if (!npc->pause) {
			npc->actClock--;
		}
		break;
	case AI_BANDF:
		if(npc->mover.x == npc->mover.nextX && npc->mover.y == npc->mover.nextY && !npc->pause) {
			if(npc->mover.x < npc->destX[npc->dest]) {
				pk_moveChar(RIGHT, true, &npc->mover);
			} else if(npc->mover.x > npc->destX[npc->dest]) {
				pk_moveChar(LEFT, true, &npc->mover);
			}

			if(npc->mover.y < npc->destY[npc->dest]) {
				pk_moveChar(UP, true, &npc->mover);
			} else if(npc->mover.y > npc->destY[npc->dest]) {
				pk_moveChar(DOWN, true, &npc->mover);
			}
		}

		pk_updateChar(&npc->mover);

		if(npc->mover.x == npc->destX[npc->dest] && npc->mover.y == npc->destY[npc->dest]) {
			if(npc->dest == 0) {
				npc->dest = 1;
			} else {
				npc->dest = 0;
			}
		}

		break;
	case AI_WANDER:
		if(npc->mover.x == npc->mover.nextX && npc->mover.y == npc->mover.nextY && !npc->pause) {

			int randDir = rand() % 150 + 1;

			switch(randDir) {
				case LEFT:
					pk_moveChar(LEFT, col.left, &npc->mover);
					break;
				case RIGHT:
					pk_moveChar(RIGHT, col.right, &npc->mover);
					break;
				case DOWN:
					pk_moveChar(DOWN, col.down, &npc->mover);
					break;
				case UP:
					pk_moveChar(UP, col.up, &npc->mover);
					break;
			}
		}

		pk_updateChar(&npc->mover);
		break;
	}

	pk_updateWindow(&npc->dialog);
}

void pk_setNpcWindow(window_t s_wind, npc_t* trainer) {
	trainer->dialog = s_wind;
}

void pk_setNpcMonster(monster_t mon, npc_t* trainer) {
	trainer->monsters[trainer->monCnt] = mon;
	trainer->monCnt++;
}

int pk_getNpcFrame(npc_t npc) {
	return pk_getCharFrame(npc.sprite, npc.mover);
}

void pk_toggleNpcPause(npc_t* npc) {
	npc->pause = !npc->pause;
}