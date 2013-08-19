#include "charmover.h"

void pk_initChar(int s_x, int s_y, charMover_t* obj) {
	obj->x = s_x;
	obj->y = s_y;
	obj->lastX = s_x;
	obj->lastY = s_y;
	obj->nextX = s_x;
	obj->nextY = s_y;
	obj->dir = LEFT;
	obj->step = false;
	obj->animCycle = 0;
}

void pk_moveChar(int dir, bool move, charMover_t* obj) {
	if(pk_isFinishedMoving(*obj) && obj->animCycle == 0) {
		if(dir == LEFT && move) {
			obj->nextX = obj->x - CHAR_SPEED;
			obj->nextY = obj->y;
		} else if(dir == RIGHT && move) {
			obj->nextX = obj->x + CHAR_SPEED;
			obj->nextY = obj->y;
		} else if(dir == UP && move) {
			obj->nextX = obj->x;
			obj->nextY = obj->y - CHAR_SPEED;
		} else if(dir == DOWN && move) {
			obj->nextX = obj->x;
			obj->nextY = obj->y + CHAR_SPEED;
		}

		obj->step = !obj->step;
		obj->animCycle = CHAR_SPEED;
		obj->dir = dir;
	}
}

void pk_updateChar(charMover_t* obj) {
	// Move the player in whatever neccisary direction
	if(obj->x > obj->nextX) {
		obj->x--;
	} else if(obj->x < obj->nextX) {
		obj->x++;
	} else if(obj->y > obj->nextY) {
		obj->y--;
	} else if(obj->y < obj->nextY) {
		obj->y++;
	}

	// Roll through the player walking animation
	if(obj->animCycle > 0) {
		obj->animCycle --;
	}

	// Update the lastX and lastY coords for every finished move cycle
	if(pk_isFinishedMoving(*obj)) {
		obj->lastX = obj->x;
		obj->lastY = obj->y;
	}
}

void pk_resetChar(charMover_t* obj) {
	if(!pk_isFinishedMoving(obj[0])) {
		switch(obj->dir) {
		case RIGHT:
			obj->x = obj->nextX-BLOCK_SIZE;
			break;
		case LEFT:
			obj->x = obj->nextX+BLOCK_SIZE;
			break;
		case UP:
			obj->y = obj->nextY+BLOCK_SIZE;
			break;
		case DOWN:
			obj->y = obj->nextY-BLOCK_SIZE;
			break;
		}
	}

	obj->nextX = obj->x;
	obj->nextY = obj->y;

	obj->animCycle = 0;
}

int pk_getCharFrame(int spriteOffset, charMover_t obj) {
	switch(obj.dir) {
	case LEFT:
		if(obj.animCycle > CHAR_SPEED/2) {
			return spriteOffset+1;
		} else {
			return spriteOffset;
		}
		break;
	case RIGHT:
		if(obj.animCycle > CHAR_SPEED/2) {
			return spriteOffset+3;
		} else {
			return spriteOffset+2;
		}
	case UP:
		if(obj.animCycle > CHAR_SPEED/2) {
			if(obj.step)  return spriteOffset+8;
			if(!obj.step) return spriteOffset+9;
		} else {
			return spriteOffset+7;
		}
		break;
	case DOWN:
		if(obj.animCycle > CHAR_SPEED/2) {
			if(obj.step)  return spriteOffset+5;
			if(!obj.step) return spriteOffset+6;
		} else {
			return spriteOffset+4;
		}
		break;
	}

	return 1;
}

void pk_buildColMapC(charMover_t obj, map_t* map) {
	map->cData[obj.lastX/BLOCK_SIZE+( (obj.lastY/BLOCK_SIZE)*map->width )] = true;
	map->cData[obj.nextX/BLOCK_SIZE+( (obj.nextY/BLOCK_SIZE)*map->width )] = true;
}

int pk_faceChar(charMover_t p, charMover_t t) {
	if(p.x > t.x) {
		return LEFT;
	} else if(p.x < t.x) {
		return RIGHT;
	} else if(p.y > t.y) {
		return DOWN;
	} else if(p.y < t.y) {
		return UP;
	}
}

bool pk_isFinishedMoving(charMover_t obj) {
	if(obj.x == obj.nextX && obj.y == obj.nextY) {
		return true;
	}

	return false;
}