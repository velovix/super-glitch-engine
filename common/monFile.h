#ifndef monFILE_H
#define monFILE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MONFILE_VERSION		1

// Errors
#define PK_MNF_FILE			1
#define PK_MNF_VERSION		2

/* -- Header -- */
typedef struct {
	uint32_t version;
	uint32_t count;
} monHeader_f;

/* -- Basics -- */
typedef struct {
	uint8_t name[10];
	uint8_t type1;
	uint8_t type2;
	uint32_t height;
	uint32_t weight;
} monBasic_f;

/* -- Stats -- */
typedef struct {
	uint8_t b_hp;
	uint8_t b_att;
	uint8_t b_def;
	uint8_t b_spAtt;
	uint8_t b_spDef;
	uint8_t b_speed;

	uint8_t ev_hp;
	uint8_t ev_att;
	uint8_t ev_def;
	uint8_t ev_spAtt;
	uint8_t ev_spDef;
	uint8_t ev_speed;
} monStats_f;

/* -- Move List -- */
typedef struct {
	uint32_t move;
	uint8_t level;
} monNatMoveEntry_f;

typedef struct {
	uint32_t natLearnedCnt;
	monNatMoveEntry_f *natLearned;

	uint32_t tmLearnedCnt;
	uint32_t *tmLearned;
} monMovelist_f;

/* -- Experience -- */
#define EXP_ERRATIC		1
#define EXP_FAST		2
#define EXP_MEDFAST		3
#define EXP_MEDSLOW		4
#define EXP_SLOW 		5
#define EXP_FLUCTUATING	6

typedef struct {
	uint8_t type;
} monExperience_f;

/* -- Evolutions -- */
typedef struct {
	uint8_t evolves;
	uint32_t mon;
	uint8_t level;
} monEvolution_f;

/* -- Aesthetics -- */
typedef struct {
	uint32_t battleSprite;
	uint32_t icon;
	uint32_t body;
	uint32_t footprint;
	uint32_t cry;
} monAesthetics_f;

/* -- Others -- */
typedef struct {
	uint8_t species[13];
	uint8_t genderless;
	uint8_t malePerc;
	uint32_t ability;
	uint8_t catchRate;
	uint32_t heldItem;
} monOthers_f;

/* -- File Objects -- */
typedef struct {
	monBasic_f basic;
	monStats_f stats;
	monMovelist_f movelist;
	monExperience_f experience;
	monEvolution_f evolution;
	monAesthetics_f aesthetics;
	monOthers_f etc;
} monFileObj_t;

typedef struct {
	monHeader_f header;
	monFileObj_t *mons;
} monFile_t;

int pk_openMonFile(monFile_t *obj, char* filename);
int pk_saveMonFile(monFile_t *obj, char* filename);

void pk_freeMonFile(monFile_t *obj);

#endif