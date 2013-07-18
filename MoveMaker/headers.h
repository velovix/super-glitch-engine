#ifndef HEADERS_H
#define HEADERS_H

// Type Headers

#define TYPE_VERSION	2
#define INV_YES			1
#define INV_NO			0

typedef struct {
	int version;
	int count;
} typeHeader_f;

typedef struct {
	char name[8];
	int resCnt;
	int weakCnt;
} typeEntry_f;

typedef struct {
	int type;
	char inv;
} resEntry_f;

typedef struct {
	int type;
} weakEntry_f;

typedef struct {
	typeEntry_f info;
	resEntry_f res[20];
	weakEntry_f weak[20];
} typeObj_t;

// Move Headers

#define CURRVERSION		1

typedef struct {
	int version;
	int count;
} moveHeader_f;

typedef struct {
	char name[12];
	int type;
	int scriptLen;
} moveEntry_f;

typedef struct {
	moveEntry_f info;
	char *script;
} moveObj_t;

#endif
