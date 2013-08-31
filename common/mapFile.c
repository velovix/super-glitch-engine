#include "mapFile.h"

int pk_openMapFile(mapFile_t *obj, char *filename, int room) {
	// Load map file
	FILE * fMap;
	fMap = fopen(filename, "r");
	if(fMap == NULL) {
		return PK_MF_FILE;
	}

	// Get file headers
	fread(&obj->mapHeader.version, sizeof(int), 1, fMap);
	if(obj->mapHeader.version != MAPFILE_VERSION) {
		return PK_MF_BADVERSION;
	}
	fread(&obj->mapHeader.count, sizeof(int), 1, fMap);
	if(room != -1 && (room > obj->mapHeader.count || room < 0)) {
		return PK_MF_BADMOVEREQ;
	}

	// Load tile collision data
	fread(&obj->mapHeader.tileColCnt, sizeof(int), 1, fMap);
	obj->tileCols = (char*)malloc(obj->mapHeader.tileColCnt*sizeof(char));
	for(int i=0; i<obj->mapHeader.tileColCnt; i++) {
		fread(&obj->tileCols[i], sizeof(char), 1, fMap);
	}

	// Find desired room or load all rooms
	obj->rooms = (roomFileObj_t*)malloc(obj->mapHeader.count*sizeof(roomFileObj_t));
	for(int i=0; i<obj->mapHeader.count; i++) {
		// Load room headers
		fread(&obj->rooms[i].header.value, sizeof(char), 1, fMap);
		fread(&obj->rooms[i].header.music, sizeof(int), 1, fMap);
		fread(&obj->rooms[i].header.w, sizeof(int), 1, fMap);
		fread(&obj->rooms[i].header.h, sizeof(int), 1, fMap);

		if(i==room || room == -1) {
			// Load the room if it's the requested one or if all rooms are to be loaded
			obj->rooms[i].mapData = (char*)malloc(obj->rooms[i].header.w*obj->rooms[i].header.h);
			for(int j=0; j<obj->rooms[i].header.w*obj->rooms[i].header.h; j++) {
				fread(&obj->rooms[i].mapData[j], sizeof(char), 1, fMap);
			}

			// Load door data
			fread(&obj->rooms[i].header.doorCnt, sizeof(int), 1, fMap);
			obj->rooms[i].doorData = (doorEntry_t*)malloc(obj->rooms[i].header.doorCnt*sizeof(doorEntry_t));
			for(int j=0; j<obj->rooms[i].header.doorCnt; j++) {
				fread(&obj->rooms[i].doorData[j].dest, sizeof(char), 1, fMap);
				fread(&obj->rooms[i].doorData[j].x, sizeof(int), 1, fMap);
				fread(&obj->rooms[i].doorData[j].y, sizeof(int), 1, fMap);
				fread(&obj->rooms[i].doorData[j].destX, sizeof(int), 1, fMap);
				fread(&obj->rooms[i].doorData[j].destY, sizeof(int), 1, fMap);
			}

			// Load NPC data
			fread(&obj->rooms[i].header.npcCnt, sizeof(int), 1, fMap);
			obj->rooms[i].npcData = (npcEntry_t*)malloc(obj->rooms[i].header.npcCnt*sizeof(npcEntry_t));
			for(int j=0; j<obj->rooms[i].header.npcCnt; j++) {
				fread(&obj->rooms[i].npcData[j].value, sizeof(int), 1, fMap);
				fread(&obj->rooms[i].npcData[j].x, sizeof(int), 1, fMap);
				fread(&obj->rooms[i].npcData[j].y, sizeof(int), 1, fMap);
			}

		} else {
			// If this is the wrong room, skip through it
			int doorCnt;
			fseek(fMap, obj->rooms[i].header.w*obj->rooms[i].header.h*sizeof(char), SEEK_CUR);
			fread(&doorCnt, sizeof(int), 1, fMap);
			fseek(fMap, (sizeof(char)+(sizeof(int)*4))*doorCnt, SEEK_CUR);
			int npcCnt;
			fread(&npcCnt, sizeof(int), 1, fMap);
			fseek(fMap, (sizeof(int)*3)*npcCnt, SEEK_CUR);
		}
	}

	fclose(fMap);

	return 0;
}

int pk_saveMapFile(mapFile_t *obj, char *filename) {
	// Load map file location
	FILE * fMap;
	fMap = fopen(filename, "w");
	if(fMap == NULL) {
		return PK_MF_FILE;
	}

	// Write file headers
	fwrite(&obj->mapHeader.version, sizeof(int), 1, fMap);
	fwrite(&obj->mapHeader.count, sizeof(int), 1, fMap);

	// Save tile collision data
	// First, weed through any junk collisions
	int oldTileColCnt = obj->mapHeader.tileColCnt;
	for(int i=0; i<oldTileColCnt; i++) {
		if(obj->tileCols[i] == 255) {
			obj->mapHeader.tileColCnt--;
		}
	}
	fwrite(&obj->mapHeader.tileColCnt, sizeof(int), 1, fMap);
	// Save only legit collision values
	for(int i=0; i<oldTileColCnt; i++) {
		if(obj->tileCols[i] != 255) {
			fwrite(&obj->tileCols[i], sizeof(char), 1, fMap);
		}
	}

	// Save rooms
	for(int i=0; i<obj->mapHeader.count; i++) {
		// Save room headers
		fwrite(&obj->rooms[i].header.value, sizeof(char), 1, fMap);
		fwrite(&obj->rooms[i].header.music, sizeof(int), 1, fMap);
		fwrite(&obj->rooms[i].header.w, sizeof(int), 1, fMap);
		fwrite(&obj->rooms[i].header.h, sizeof(int), 1, fMap);

		// Save room tile data
		for(int j=0; j<obj->rooms[i].header.w*obj->rooms[i].header.h; j++) {
			fwrite(&obj->rooms[i].mapData[j], sizeof(char), 1, fMap);
		}

		// Save door data
		fwrite(&obj->rooms[i].header.doorCnt, sizeof(int), 1, fMap);
		for(int j=0; j<obj->rooms[i].header.doorCnt; j++) {
			fwrite(&obj->rooms[i].doorData[j].dest, sizeof(char), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].x, sizeof(int), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].y, sizeof(int), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].destX, sizeof(int), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].destY, sizeof(int), 1, fMap);
		}

		// Save NPC data
		fwrite(&obj->rooms[i].header.npcCnt, sizeof(int), 1, fMap);
		for(int j=0; j<obj->rooms[i].header.npcCnt; j++) {
			fwrite(&obj->rooms[i].npcData[j].value, sizeof(int), 1, fMap);
			fwrite(&obj->rooms[i].npcData[j].x, sizeof(int), 1, fMap);
			fwrite(&obj->rooms[i].npcData[j].y, sizeof(int), 1, fMap);
		}
	}

	fclose(fMap);

	return 0;
}

void pk_freeMapFile(mapFile_t *obj, int room) {
	// Free room-specific pointers
	if(room == -1) {
		for(int i=0; i<obj->mapHeader.count; i++) {
			free(obj->rooms[i].mapData);
			free(obj->rooms[i].doorData);
			free(obj->rooms[i].npcData);
		}
	} else {
		free(obj->rooms[room].mapData);
		free(obj->rooms[room].doorData);
		free(obj->rooms[room].npcData);
	}

	// Free general pointers
	free(obj->tileCols);
	free(obj->rooms);
}
