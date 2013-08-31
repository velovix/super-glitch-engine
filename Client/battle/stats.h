/* Super Glitch Engine Client
 * stats.h
 * Copyright Tyler Compton 2013
 *
 * Declares structures stats_t and type_t.
 * - stats_t represents stat information and any stat modifiers.
 * - type_t represents info for a type, and should only be referenced in a
 * struct and never edited beyond initial declaration.
 */

#ifndef STATS_H
#define STATS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../constants.h"
#include "../../common/typeFile.h"

typedef struct {
	int hp, att, def, spAtt, spDef, speed;
	int mHp, mAtt, mDef, mSpAtt, mSpDef, mSpeed, mAcc, mEvas;
} stats_t;

typedef struct {
	char name[8];
	int value, resCnt, weakCnt;
	int* res;
	int* weak;
	bool* inv;
} type_t;

// Initializers
stats_t pk_initStats(int hp, int att, int def, int spAtt, int spDef, int speed);
type_t pk_initType(char*, int);
type_t pk_initTypeFile(typeFileObj_t obj);

int pk_calcTyping(type_t, type_t, type_t);

// Type settings
void pk_setTypeRes(type_t*, int, int*);
void pk_setTypeWeak(type_t*, int, int*);
void pk_setTypeInv(type_t*, int, bool*);

void pk_freeType(type_t*);

#endif