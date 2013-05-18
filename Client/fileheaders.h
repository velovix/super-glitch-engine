#ifndef FILEHEADERS_H
#define FILEHEADERS_H

// --- Monsters

typedef struct {
	int count;
} monHeader_f;

typedef struct {
	int value;
	char name[12];
	int hp, att, def, spAtt, spDef, speed;
} monEntry_f;

// --- Types

typedef struct {
	int count;
} typeHeader_f;

typedef struct {
	char name[8];
	int resCnt;
	int weakCnt;
	int invCnt;
} typeEntry_f;

typedef struct {
	int type;
} resEntry_f;

typedef struct {
	int type;
} weakEntry_f;

typedef struct {
	int type;
} invEntry_f;

// --- Maps

typedef struct {
	int count;
} mapHeader_f;

typedef struct {
	int value;
	int w, h;
} roomHeader_f;

typedef struct {
	int count;
} npcListHeader_f;

typedef struct {
	char value;
	int x, y;
} npcEntry_f;

#endif