#ifndef MOVEFILE_H
#define MOVEFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MOVEFILE_VERSION		2

// Errors
#define PK_MF_FILE				1
#define PK_MF_VERSION			2

// File headers
typedef struct {
	uint32_t	version;
	uint32_t	count;
} moveHeader_f;

typedef struct {
	uint8_t		name[12];
	uint32_t	type;
	uint32_t	pp;
	uint32_t	scriptLen;
	uint8_t		*script;
} moveEntry_f;

// Object return headers
typedef struct {
	moveHeader_f header;
	moveEntry_f	*moves;
} moveFile_t;

int pk_openMoveFile(moveFile_t *obj, char* filename);
int pk_saveMoveFile(moveFile_t *obj, char* filename);

void pk_freeMoveFile(moveFile_t *obj);

#endif