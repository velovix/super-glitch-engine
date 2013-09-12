#include "monFile.h"

int pk_openMonFile(monFile_t *obj, char* filename) {
	FILE * file;
	file = fopen(filename, "r");
	if(file == NULL) {
		return PK_MNF_FILE;
	}

	// Read header
	fread(&obj->header.version, sizeof(uint32_t), 1, file);
	if(obj->header.version != MONFILE_VERSION) {
		return PK_MNF_VERSION;
	}

	fread(&obj->header.count, sizeof(uint32_t), 1, file);
	// Start reading monsters
	obj->mons = (monFileObj_t*)malloc(sizeof(monFileObj_t)*obj->header.count);
	for(int i=0; i<obj->header.count; i++) {
		// Read basic data
		fread(&obj->mons[i].basic.name, sizeof(uint8_t), 10, file);
		fread(&obj->mons[i].basic.type1, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].basic.type2, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].basic.height, sizeof(uint32_t), 1, file);
		fread(&obj->mons[i].basic.weight, sizeof(uint32_t), 1, file);

		// Read stats data
		fread(&obj->mons[i].stats.b_hp, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.b_att, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.b_def, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.b_spAtt, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.b_spDef, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.b_speed, sizeof(uint8_t), 1, file);

		fread(&obj->mons[i].stats.ev_hp, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.ev_att, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.ev_def, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.ev_spAtt, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.ev_spDef, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].stats.ev_speed, sizeof(uint8_t), 1, file);

		// Read move data
		fread(&obj->mons[i].movelist.natLearnedCnt, sizeof(uint32_t), 1, file);
		obj->mons[i].movelist.natLearned =
			(monNatMoveEntry_f*)malloc(sizeof(monNatMoveEntry_f)*obj->mons[i].movelist.natLearnedCnt);
		for(int j=0; j<obj->mons[i].movelist.natLearnedCnt; j++) {
			fread(&obj->mons[i].movelist.natLearned[j].move, sizeof(uint32_t), 1, file);
			fread(&obj->mons[i].movelist.natLearned[j].level, sizeof(uint8_t), 1, file);
		}
		fread(&obj->mons[i].movelist.tmLearnedCnt, sizeof(uint32_t), 1, file);
		obj->mons[i].movelist.tmLearned =
			(uint32_t*)malloc(sizeof(uint32_t)*obj->mons[i].movelist.tmLearnedCnt);
		for(int j=0; j<obj->mons[i].movelist.tmLearnedCnt; j++) {
			fread(&obj->mons[i].movelist.tmLearned[j], sizeof(uint32_t), 1, file);
		}

		// Read experience data
		fread(&obj->mons[i].experience.type, sizeof(uint8_t), 1, file);

		// Read evolution data
		fread(&obj->mons[i].evolution.evolves, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].evolution.mon, sizeof(uint32_t), 1, file);
		fread(&obj->mons[i].evolution.level, sizeof(uint8_t), 1, file);

		// Read aesthetics data
		fread(&obj->mons[i].aesthetics.battleSprite, sizeof(uint32_t), 1, file);
		fread(&obj->mons[i].aesthetics.icon, sizeof(uint32_t), 1, file);
		fread(&obj->mons[i].aesthetics.body, sizeof(uint32_t), 1, file);
		fread(&obj->mons[i].aesthetics.footprint, sizeof(uint32_t), 1, file);
		fread(&obj->mons[i].aesthetics.cry, sizeof(uint32_t), 1, file);

		// Read others data
		fread(&obj->mons[i].etc.species, sizeof(uint8_t), 13, file);
		fread(&obj->mons[i].etc.genderless, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].etc.malePerc, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].etc.ability, sizeof(uint32_t), 1, file);
		fread(&obj->mons[i].etc.catchRate, sizeof(uint8_t), 1, file);
		fread(&obj->mons[i].etc.heldItem, sizeof(uint32_t), 1, file);
	}

	fclose(file);

	return 0;
}

int pk_saveMonFile(monFile_t *obj, char* filename) {
	printf("Saving monster file...\n");

	FILE * file;
	file = fopen(filename, "w");
	if(file == NULL) {
		return PK_MNF_FILE;
	}

	// Read header
	obj->header.version = MONFILE_VERSION;
	fwrite(&obj->header.version, sizeof(uint32_t), 1, file);

	fwrite(&obj->header.count, sizeof(uint32_t), 1, file);
	// Start writing monsters
	for(int i=0; i<obj->header.count; i++) {
		// Read basic data
		fwrite(&obj->mons[i].basic.name, sizeof(uint8_t), 10, file);
		fwrite(&obj->mons[i].basic.type1, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].basic.type2, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].basic.height, sizeof(uint32_t), 1, file);
		fwrite(&obj->mons[i].basic.weight, sizeof(uint32_t), 1, file);

		// Read stats data
		fwrite(&obj->mons[i].stats.b_hp, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.b_att, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.b_def, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.b_spAtt, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.b_spDef, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.b_speed, sizeof(uint8_t), 1, file);

		fwrite(&obj->mons[i].stats.ev_hp, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.ev_att, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.ev_def, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.ev_spAtt, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.ev_spDef, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].stats.ev_speed, sizeof(uint8_t), 1, file);

		// Read move data
		fwrite(&obj->mons[i].movelist.natLearnedCnt, sizeof(uint32_t), 1, file);
		for(int j=0; j<obj->mons[i].movelist.natLearnedCnt; j++) {
			fwrite(&obj->mons[i].movelist.natLearned[j].move, sizeof(uint32_t), 1, file);
			fwrite(&obj->mons[i].movelist.natLearned[j].level, sizeof(uint8_t), 1, file);
		}
		fwrite(&obj->mons[i].movelist.tmLearnedCnt, sizeof(uint32_t), 1, file);
		for(int j=0; j<obj->mons[i].movelist.tmLearnedCnt; j++) {
			fwrite(&obj->mons[i].movelist.tmLearned[j], sizeof(uint32_t), 1, file);
		}

		// Read experience data
		fwrite(&obj->mons[i].experience.type, sizeof(uint8_t), 1, file);

		// Read evolution data
		fwrite(&obj->mons[i].evolution.evolves, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].evolution.mon, sizeof(uint32_t), 1, file);
		fwrite(&obj->mons[i].evolution.level, sizeof(uint8_t), 1, file);

		// Read aesthetics data
		fwrite(&obj->mons[i].aesthetics.battleSprite, sizeof(uint32_t), 1, file);
		fwrite(&obj->mons[i].aesthetics.icon, sizeof(uint32_t), 1, file);
		fwrite(&obj->mons[i].aesthetics.body, sizeof(uint32_t), 1, file);
		fwrite(&obj->mons[i].aesthetics.footprint, sizeof(uint32_t), 1, file);
		fwrite(&obj->mons[i].aesthetics.cry, sizeof(uint32_t), 1, file);

		// Read others data
		fwrite(&obj->mons[i].etc.species, sizeof(uint8_t), 13, file);
		fwrite(&obj->mons[i].etc.genderless, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].etc.malePerc, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].etc.ability, sizeof(uint32_t), 1, file);
		fwrite(&obj->mons[i].etc.catchRate, sizeof(uint8_t), 1, file);
		fwrite(&obj->mons[i].etc.heldItem, sizeof(uint32_t), 1, file);
	}

	fclose(file);

	printf("--done.\n");
	return 0;
}

void pk_freeMonFile(monFile_t *obj) {
	// Free object-specific pointers
	for(int i=0; i<obj->header.count; i++) {
		free(obj->mons[i].movelist.natLearned);
		free(obj->mons[i].movelist.tmLearned);
	}

	// Free main pointer
	free(obj->mons);
}
