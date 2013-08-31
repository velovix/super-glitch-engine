#ifndef TYPEFILE_H
#define TYPEFILE_H

#include <stdio.h>
#include <stdlib.h>

#define TYPEFILE_VERSION		2

#define INV_YES					1
#define INV_NO					0

// Errors
#define PK_TF_FILE				1
#define PK_TF_VERSION			2

// File headers
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

// Object return headers
typedef struct {
	int val;
	typeEntry_f header;
	resEntry_f *res;
	weakEntry_f *weak;
} typeFileObj_t;

typedef struct {
	typeHeader_f header;
	typeFileObj_t *types;
} typeFile_t;

int pk_openTypeFile(typeFile_t *obj, char* filename);
int pk_saveTypeFile(typeFile_t *obj, char* filename);

void pk_freeTypeFile(typeFile_t *obj);

#endif