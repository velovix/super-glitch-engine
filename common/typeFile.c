#include "typeFile.h"

int pk_openTypeFile(typeFile_t *obj, char* filename) {
	// Load types file
	FILE * fTypes;
	fTypes = fopen(filename, "r");
	if(fTypes == NULL) {
		return PK_TF_FILE;
	}

	// Read file headers
	fread(&obj->header.version, sizeof(int), 1, fTypes);
	if(obj->header.version != TYPEFILE_VERSION) {
		return PK_TF_VERSION;
	}
	fread(&obj->header.count, sizeof(int), 1, fTypes);

	// Load type information
	obj->types = (typeFileObj_t*)malloc(obj->header.count*sizeof(typeFileObj_t));
	for(int i=0; i<obj->header.count; i++) {
		obj->types[i].val = i;

		// Read type header
		fread(&obj->types[i].header.name, sizeof(char[8]), 1, fTypes);
		fread(&obj->types[i].header.resCnt, sizeof(int), 1, fTypes);
		fread(&obj->types[i].header.weakCnt, sizeof(int), 1, fTypes);

		obj->types[i].res = (resEntry_f*)malloc(obj->types[i].header.resCnt*sizeof(resEntry_f));
		obj->types[i].weak = (weakEntry_f*)malloc(obj->types[i].header.weakCnt*sizeof(weakEntry_f));

		// Load resistances
		for(int j=0; j<obj->types[i].header.resCnt; j++) {
			fread(&obj->types[i].res[j].type, sizeof(int), 1, fTypes);
			fread(&obj->types[i].res[j].inv, sizeof(char), 1, fTypes);
		}

		// Load weaknesses
		for(int j=0; j<obj->types[i].header.weakCnt; j++) {
			fread(&obj->types[i].weak[j].type, sizeof(int), 1, fTypes);
		}
	}

	fclose(fTypes);

	return 0;
}



void pk_freeTypeFile(typeFile_t *obj) {
	// Free each type's pointers
	for(int i=0; i<obj->header.count; i++) {
		free(obj->types[i].res);
		free(obj->types[i].weak);
	}

	// Free general type file's pointers
	free(obj->types);
}