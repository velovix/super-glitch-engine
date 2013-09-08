#include "moveFile.h"

int pk_openMoveFile(moveFile_t *obj, char* filename) {
	// Load moves file
	FILE * fMoves;
	fMoves = fopen(filename, "r");
	if(fMoves == NULL) {
		return PK_MF_FILE;
	}

	// Read file obj->header
	fread(&obj->header.version, sizeof(uint32_t), 1, fMoves);
	if(obj->header.version != MOVEFILE_VERSION) {
		return PK_MF_VERSION;
	}
	fread(&obj->header.count, sizeof(uint32_t), 1, fMoves);

	// Read moves
	obj->moves = (moveEntry_f*)malloc(obj->header.count*sizeof(moveEntry_f));
	for(int i=0; i<obj->header.count; i++) {
		// Read move header
		fread(&obj->moves[i].name, sizeof(uint8_t), 12, fMoves);
		fread(&obj->moves[i].type, sizeof(uint32_t), 1, fMoves);
		fread(&obj->moves[i].pp, sizeof(uint32_t), 1, fMoves);
		fread(&obj->moves[i].scriptLen, sizeof(uint32_t), 1, fMoves);

		// Read move script
		obj->moves[i].script = (uint8_t*)malloc(sizeof(uint8_t)*obj->moves[i].scriptLen);
		fread(obj->moves[i].script, sizeof(uint8_t), obj->moves[i].scriptLen, fMoves);
	}

	fclose(fMoves);

	return 0;
}

int pk_saveMoveFile(moveFile_t *obj, char* filename) {
	// Open moves file
	FILE * fMoves;
	fMoves = fopen(filename, "w");
	if(fMoves == NULL) {
		return PK_MF_FILE;
	}

	// Write file obj->header
	fwrite(&obj->header.version, sizeof(uint32_t), 1, fMoves);
	fwrite(&obj->header.count, sizeof(uint32_t), 1, fMoves);

	// Write moves
	for(int i=0; i<obj->header.count; i++) {
		// Read move header
		fwrite(&obj->moves[i].name, sizeof(uint8_t), 12, fMoves);
		fwrite(&obj->moves[i].type, sizeof(uint32_t), 1, fMoves);
		fwrite(&obj->moves[i].pp, sizeof(uint32_t), 1, fMoves);
		fwrite(&obj->moves[i].scriptLen, sizeof(uint32_t), 1, fMoves);

		// Read move script
		fwrite(obj->moves[i].script, sizeof(uint8_t), obj->moves[i].scriptLen, fMoves);
	}

	fclose(fMoves);

	return 0;
}


void pk_freeMoveFile(moveFile_t *obj) {
	for(int i=0; i<obj->header.count; i++) {
		free(obj->moves[i].script);
	}

	free(obj->moves);
}
