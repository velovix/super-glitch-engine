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
	int w, h;
	char data[32400];
	int doorCnt;
	char doorData[20];
} mapData_t;

#endif
