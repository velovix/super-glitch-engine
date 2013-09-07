#include "mapFile.h"

int pk_openMapFile(mapFile_t *obj, char *filename, int room) {
	printf("Opening Mapfile!\n");
	// Load map file
	FILE * fMap;
	fMap = fopen(filename, "r");
	if(fMap == NULL) {
		return PK_MF_FILE;
	}

	// Get file headers
	fread(&obj->mapHeader.version, sizeof(uint32_t), 1, fMap);
	if(obj->mapHeader.version != MAPFILE_VERSION) {
		return PK_MF_BADVERSION;
	}
	fread(&obj->mapHeader.count, sizeof(uint32_t), 1, fMap);
	if(room != -1 && (room > obj->mapHeader.count || room < 0)) {
		return PK_MF_BADMOVEREQ;
	}
	printf("-V:  %i\n", obj->mapHeader.version);
	printf("-#R: %i\n", obj->mapHeader.count);

	// Find desired room or load all rooms
	obj->rooms = (roomFileObj_t*)malloc(obj->mapHeader.count*sizeof(roomFileObj_t));
	for(int i=0; i<obj->mapHeader.count; i++) {
		printf("-Loading Room %i\n", i);
		// Load room headers
		fread(&obj->rooms[i].header.value, sizeof(uint8_t), 1, fMap);
		fread(&obj->rooms[i].header.music, sizeof(uint32_t), 1, fMap);
		fread(&obj->rooms[i].header.w, sizeof(uint32_t), 1, fMap);
		fread(&obj->rooms[i].header.h, sizeof(uint32_t), 1, fMap);
		fread(&obj->rooms[i].header.tileColCnt, sizeof(uint32_t), 1, fMap);
		fread(&obj->rooms[i].header.doorCnt, sizeof(uint32_t), 1, fMap);
		fread(&obj->rooms[i].header.npcCnt, sizeof(uint32_t), 1, fMap);
		printf("--Tile Cnt: %i\n", obj->rooms[i].header.tileColCnt);
		printf("--Door Cnt: %i\n", obj->rooms[i].header.doorCnt);
		printf("--Npc  Cnt: %i\n", obj->rooms[i].header.npcCnt);

		if(i==room || room == -1) {
			// Load the room if it's the requested one or if all rooms are to be loaded
			// Load tile collision data
			obj->rooms[i].tileColData = (uint8_t*)malloc(obj->rooms[i].header.tileColCnt*sizeof(uint8_t));
			for(int j=0; j<obj->rooms[i].header.tileColCnt; j++) {
				fread(&obj->rooms[i].tileColData[j], sizeof(uint8_t), 1, fMap);
			}
			// Load the tile data
			obj->rooms[i].mapData = (uint8_t*)malloc(obj->rooms[i].header.w*obj->rooms[i].header.h*sizeof(uint8_t));
			for(int j=0; j<obj->rooms[i].header.w*obj->rooms[i].header.h; j++) {
				fread(&obj->rooms[i].mapData[j], sizeof(uint8_t), 1, fMap);
			}

			// Load door data
			obj->rooms[i].doorData = (doorEntry_t*)malloc(obj->rooms[i].header.doorCnt*sizeof(doorEntry_t));
			for(int j=0; j<obj->rooms[i].header.doorCnt; j++) {
				fread(&obj->rooms[i].doorData[j].dest, sizeof(uint8_t), 1, fMap);
				fread(&obj->rooms[i].doorData[j].x, sizeof(uint32_t), 1, fMap);
				fread(&obj->rooms[i].doorData[j].y, sizeof(uint32_t), 1, fMap);
				fread(&obj->rooms[i].doorData[j].destX, sizeof(uint32_t), 1, fMap);
				fread(&obj->rooms[i].doorData[j].destY, sizeof(uint32_t), 1, fMap);
			}

			// Load NPC data
			obj->rooms[i].npcData = (npcEntry_t*)malloc(obj->rooms[i].header.npcCnt*sizeof(npcEntry_t));
			for(int j=0; j<obj->rooms[i].header.npcCnt; j++) {
				fread(&obj->rooms[i].npcData[j].value, sizeof(uint32_t), 1, fMap);
				fread(&obj->rooms[i].npcData[j].x, sizeof(uint32_t), 1, fMap);
				fread(&obj->rooms[i].npcData[j].y, sizeof(uint32_t), 1, fMap);
			}

			// Stop if only this room is expected
			if(room != -1) {
				break;
			}

		} else {
			// If this is the wrong room, skip through it
			fseek(fMap, obj->rooms[i].header.tileColCnt*sizeof(uint8_t), SEEK_CUR);
			fseek(fMap, obj->rooms[i].header.w*obj->rooms[i].header.h*sizeof(uint8_t), SEEK_CUR);
			fseek(fMap, sizeof(doorEntry_t)*obj->rooms[i].header.doorCnt, SEEK_CUR);
			fseek(fMap, sizeof(npcEntry_t)*obj->rooms[i].header.npcCnt, SEEK_CUR);
		}
	}

	fclose(fMap);

	return 0;
}

int pk_saveMapFile(mapFile_t *obj, char *filename) {
	printf("Saving Mapfile!\n");
	// Load map file location
	FILE * fMap;
	fMap = fopen(filename, "w");
	if(fMap == NULL) {
		return PK_MF_FILE;
	}

	// Write file headers
	fwrite(&obj->mapHeader.version, sizeof(uint32_t), 1, fMap);
	fwrite(&obj->mapHeader.count, sizeof(uint32_t), 1, fMap);
	printf("-V:  %i\n", obj->mapHeader.version);
	printf("-#R: %i\n", obj->mapHeader.count);

	// Save rooms
	for(int i=0; i<obj->mapHeader.count; i++) {
		printf("-Saving Room %i\n", i);
		// Save room headers
		fwrite(&obj->rooms[i].header.value, sizeof(uint8_t), 1, fMap);
		fwrite(&obj->rooms[i].header.music, sizeof(uint32_t), 1, fMap);
		fwrite(&obj->rooms[i].header.w, sizeof(uint32_t), 1, fMap);
		fwrite(&obj->rooms[i].header.h, sizeof(uint32_t), 1, fMap);
		// Find out how many real tile collisions there are
		int oldTileColCnt = obj->rooms[i].header.tileColCnt;
		for(int j=0; j<oldTileColCnt; j++) {
			if(obj->rooms[i].tileColData[j] == 255) {
				obj->rooms[i].header.tileColCnt--;
			}
		}
		fwrite(&obj->rooms[i].header.tileColCnt, sizeof(uint32_t), 1, fMap);
		fwrite(&obj->rooms[i].header.doorCnt, sizeof(uint32_t), 1, fMap);
		fwrite(&obj->rooms[i].header.npcCnt, sizeof(uint32_t), 1, fMap);
		printf("--Tile Cnt: %i\n", obj->rooms[i].header.tileColCnt);
		printf("--Door Cnt: %i\n", obj->rooms[i].header.doorCnt);
		printf("--Npc  Cnt: %i\n", obj->rooms[i].header.npcCnt);


		// Save tile collision data
		// Save only legit collision values
		for(int j=0; j<oldTileColCnt; j++) {
			if(obj->rooms[i].tileColData[j] != 255) {
				fwrite(&obj->rooms[i].tileColData[j], sizeof(uint8_t), 1, fMap);
			}
		}

		// Save room tile data
		for(int j=0; j<obj->rooms[i].header.w*obj->rooms[i].header.h; j++) {
			fwrite(&obj->rooms[i].mapData[j], sizeof(uint8_t), 1, fMap);
		}

		// Save door data
		for(int j=0; j<obj->rooms[i].header.doorCnt; j++) {
			fwrite(&obj->rooms[i].doorData[j].dest, sizeof(uint8_t), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].x, sizeof(uint32_t), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].y, sizeof(uint32_t), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].destX, sizeof(uint32_t), 1, fMap);
			fwrite(&obj->rooms[i].doorData[j].destY, sizeof(uint32_t), 1, fMap);
		}

		// Save NPC data
		for(int j=0; j<obj->rooms[i].header.npcCnt; j++) {
			fwrite(&obj->rooms[i].npcData[j].value, sizeof(uint32_t), 1, fMap);
			fwrite(&obj->rooms[i].npcData[j].x, sizeof(uint32_t), 1, fMap);
			fwrite(&obj->rooms[i].npcData[j].y, sizeof(uint32_t), 1, fMap);
		}
	}

	fclose(fMap);

	return 0;
}

void pk_freeMapFile(mapFile_t *obj, int room) {
	// Free room-specific pointers
	if(room == -1) {
		for(int i=0; i<obj->mapHeader.count; i++) {
			free(obj->rooms[i].tileColData);
			free(obj->rooms[i].mapData);
			free(obj->rooms[i].doorData);
			free(obj->rooms[i].npcData);
		}
	} else {
		free(obj->rooms[room].tileColData);
		free(obj->rooms[room].mapData);
		free(obj->rooms[room].doorData);
		free(obj->rooms[room].npcData);
	}

	// Free general pointers
	free(obj->rooms);
}
