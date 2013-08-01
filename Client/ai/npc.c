#include "npc.h"

npc_t pk_initNpc(int s_x1, int s_y1, int s_x2, int s_y2, int s_reach, int s_sprite,
	int s_dir, int s_aiType, bool s_fought) {

	npc_t out;

	out.destX[0] = s_x1;
	out.destY[0] = s_y1;
	out.destX[1] = s_x2;
	out.destY[1] = s_y2;

	out.reach = s_reach;
	out.sprite = s_sprite;
	out.aiType = s_aiType;
	out.actClock = out.dest = out.currMon = out.monCnt = 0;
	out.fought = s_fought;

	out.active = true;
	out.pause = false;
	out.aggro = false;

	pk_initChar(s_x1, s_y1, &out.mover);
	out.mover.dir = s_dir;

	return out;
}

void pk_updateNpc(npc_t* npc, col_t col) {
	if(npc->mover.animCycle > CHAR_SPEED) {
		npc->mover.animCycle = 0;
	}

	switch(npc->aiType) {
	case AI_TURN:
		if(npc->actClock <= 0 && !npc->pause) {
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
		if(pk_isFinishedMoving(npc->mover) && !npc->pause) {
			if(npc->mover.x < npc->destX[npc->dest]) {
				pk_moveChar(RIGHT, col.right, &npc->mover);
			} else if(npc->mover.x > npc->destX[npc->dest]) {
				pk_moveChar(LEFT, col.left, &npc->mover);
			}

			if(npc->mover.y < npc->destY[npc->dest]) {
				pk_moveChar(UP, col.up, &npc->mover);
			} else if(npc->mover.y > npc->destY[npc->dest]) {
				pk_moveChar(DOWN, col.down, &npc->mover);
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
		if(pk_isFinishedMoving(npc->mover) && !npc->pause) {

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
	case AI_DEST:
		if(npc->actClock > 0) {
			npc->actClock--;
			break;
		}

		if(pk_isFinishedMoving(npc->mover) && !npc->pause) {
			if(npc->mover.x < npc->destX[npc->dest]) {
				pk_moveChar(RIGHT, true, &npc->mover);
			} else if(npc->mover.x > npc->destX[npc->dest]) {
				pk_moveChar(LEFT, true, &npc->mover);
			}

			if(npc->mover.y < npc->destY[npc->dest]) {
				pk_moveChar(DOWN, true, &npc->mover);
			} else if(npc->mover.y > npc->destY[npc->dest]) {
				pk_moveChar(UP, true, &npc->mover);
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

void pk_setMessage(int val, char* msg, npc_t* trainer) {
	int inc = 0;
	bool stop = false;
	while(inc < 128) {
		switch(val) {
		case 1:
			trainer->msg1[inc] = msg[inc];
			if(msg[inc] == '|') {
				stop = true;
			}
			break;
		case 2:
			trainer->msg2[inc] = msg[inc];
			if(msg[inc] == '|') {
				stop = true;
			}
			break;
		case 3:
			trainer->msg3[inc] = msg[inc];
			if(msg[inc] == '|') {
				stop = true;
			}
			break;
		}

		if(stop) {
			break;
		}
		inc++;
	}
}

void pk_switchMessage(int val, npc_t* trainer) {
	switch(val) {
	case 1:
		pk_setWindowText(&trainer->msg1[0], true, &trainer->dialog);
		break;
	case 2:
		pk_setWindowText(&trainer->msg2[0], true, &trainer->dialog);
		break;
	case 3:
		pk_setWindowText(&trainer->msg3[0], true, &trainer->dialog);
		break;
	}
}

void pk_setNpcMonster(monster_t mon, npc_t* trainer) {
	if(trainer->monCnt > 5) {
		printf("NPC: Max monster count reached\n");
		return;
	}
	trainer->monsters[trainer->monCnt] = mon;
	trainer->monCnt++;
}

int pk_getNpcFrame(npc_t npc) {
	return pk_getCharFrame(npc.sprite, npc.mover);
}

void pk_toggleNpcPause(npc_t* npc) {
	npc->pause = !npc->pause;
}

bool pk_canNpcSee(int x, int y, npc_t* npc) {
	bool out = false;

	if(npc->mover.dir == LEFT && x < npc->mover.x && y == npc->mover.y && 
		abs(npc->mover.x-x)/BLOCK_SIZE <= npc->reach) {
		out = true;
	} else if(npc->mover.dir == RIGHT && x > npc->mover.x && y == npc->mover.y &&
		abs(npc->mover.x-x)/BLOCK_SIZE <= npc->reach) {
		out = true;
	} else if(npc->mover.dir == UP && x == npc->mover.x && y < npc->mover.y &&
		abs(npc->mover.y-y)/BLOCK_SIZE <= npc->reach) {
		out = true;
	} else if(npc->mover.dir == DOWN && x == npc->mover.x && y > npc->mover.y &&
		abs(npc->mover.y-y)/BLOCK_SIZE <= npc->reach) {
		out = true;
	}

	return out;
}

void pk_aggroNpc(int tX, int tY, npc_t* npc) {
	npc->aiType = AI_DEST;
	npc->dest = 0;
	npc->aggro = true;
	npc->fought = true;
	npc->actClock = 30;

	if(npc->mover.dir == LEFT && tX < npc->mover.x && tY == npc->mover.y && 
		abs(npc->mover.x-tX)/BLOCK_SIZE <= npc->reach) {
		npc->destX[npc->dest] = tX+BLOCK_SIZE;
		npc->destY[npc->dest] = tY;
	} else if(npc->mover.dir == RIGHT && tX > npc->mover.x && tY == npc->mover.y &&
		abs(npc->mover.x-tX)/BLOCK_SIZE <= npc->reach) {
		npc->destX[npc->dest] = tX-BLOCK_SIZE;
		npc->destY[npc->dest] = tY;
	} else if(npc->mover.dir == UP && tX == npc->mover.x && tY > npc->mover.y &&
		abs(npc->mover.y-tY)/BLOCK_SIZE <= npc->reach) {
		npc->destX[npc->dest] = tX;
		npc->destY[npc->dest] = tY-BLOCK_SIZE;
	} else if(npc->mover.dir == DOWN && tX == npc->mover.x && tY < npc->mover.y &&
		abs(npc->mover.y-tY)/BLOCK_SIZE <= npc->reach) {
		npc->destX[npc->dest] = tX;
		npc->destY[npc->dest] = tY+BLOCK_SIZE;
	}

}