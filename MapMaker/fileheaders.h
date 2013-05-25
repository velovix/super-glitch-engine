#ifndef FILEHEADERS_H
#define FILEHEADERS_H

typedef struct {
	int count;
} mapHeader_f;

typedef struct {
	char value;
	int w, h;
} roomHeader_f;

typedef struct {
	int count;
} npcListHeader_f;

typedef struct {
	char value;
	int x, y;
} npcEntry_f;

typedef struct {
	char dest;
	int x, y;
	int destX, destY;
} door_t;

typedef struct {
	int val;
	int x, y;
} mapNpc_t;

typedef struct {
	int w, h;
	char data[32400];
	int doorCnt, npcCnt;
	door_t doorData[20];
	mapNpc_t npcData[20];
} mapData_t;

#endif
