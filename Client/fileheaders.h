#ifndef FILEHEADERS_H
#define FILEHEADERS_H

typedef struct {
	int count;
} monHeader_t;

typedef struct {
	int value;
	char name[12];
	int hp, att, def, spAtt, spDef, speed;
} monEntry_t;

typedef struct {
	int count;
} typeHeader_t;

typedef struct {
	char name[8];
	int resCnt;
	int weakCnt;
	int invCnt;
} typeEntry_t;

typedef struct {
	int type;
} resEntry_t;

typedef struct {
	int type;
} weakEntry_t;

typedef struct {
	int type;
} invEntry_t;

#endif