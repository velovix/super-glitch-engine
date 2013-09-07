#ifndef TYPEFILE_H
#define TYPEFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TYPEFILE_VERSION		2

#define INV_YES					1
#define INV_NO					0

// Errors
#define PK_TF_FILE				1
#define PK_TF_VERSION			2

// File headers
typedef struct {
	uint32_t	version;
	uint32_t	count;
} typeHeader_f;

typedef struct {
	uint8_t		name[8];
	uint32_t	resCnt;
	uint32_t 	weakCnt;
} typeEntry_f;

typedef struct {
	uint32_t	type;
	uint8_t		inv;
} resEntry_f;

typedef struct {
	uint32_t	type;
} weakEntry_f;

// Object return headers
typedef struct {
	uint32_t	val;
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