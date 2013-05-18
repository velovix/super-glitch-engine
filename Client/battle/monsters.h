/* Super Glitch Engine Client
 * monsters.h
 * Copyright Tyler Compton 2013
 *
 * Declares structures stats_t, type_t, moveMask_t, baseMonster_t, monster_t,
 * and move_t.
 * - stats_t represents a monster's stat info and any modifiers to those stats.
 * - type_t represents information for a type, and should only be referenced by
 * other structs and never changed beyond initial creation.
 * - moveMask_t represents the modifyable portion of a move. moveMask_t objects
 * may be edited, but not the move_t it references.
 * - baseMonster_t represents information for a monster, and should only be
 * referenced by other structs and never changed beyond initial creation.
 * - monster_t is the modifiable portion of a monster. monster_t objects may be
 * edited, but not the baseMonster_t it references.
 * - move_t represents information for a move, and shoudl only be referenced by
 * other sturcts and never changed beyond initial creation.
 */

#ifndef MONSTERS_H
#define MONSTERS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../constants.h"

typedef struct {
	int hp, att, def, spAtt, spDef, speed;
	int mHp, mAtt, mDef, mSpAtt, mSpDef, mSpeed, mAcc, mEvas;
} stats_t;

typedef struct {
	char name[8];
	int value, resCnt, weakCnt, invCnt;
	int* res;
	int* weak;
	int* inv;
} type_t;

typedef struct {
	int cpp, bpp;
	int value;
} moveMask_t;

typedef struct {
	int frntSpr, backSpr, iconSpr;
	stats_t bs;
	stats_t ev;

	char name [12];

} baseMonster_t;

typedef struct {
	int experience, type1, type2;
	char name [12];
	baseMonster_t* id;
	bool shiny;
	stats_t stats;
	moveMask_t moves[4];

} monster_t;

typedef struct {
	int bpp, type, power;
	char name [12];
	bool (*movePtr) (monster_t*, monster_t*);
} move_t;

// Initializers
stats_t pk_initStats(int hp, int att, int def, int spAtt, int spDef, int speed);
type_t pk_initType(char*, int);
move_t pk_initMove(int s_cpp, int s_bpp, char* name, int type, bool (*s_movePtr) (monster_t*, monster_t*));
monster_t pk_initMonster(int s_health, int s_experience, baseMonster_t* s_id, bool s_shiny, stats_t s_stats);
baseMonster_t pk_initBaseMonster(stats_t baseStats, stats_t baseEVs, int gID, char* name);

int pk_damage(int amount, monster_t* a, monster_t* d);
int pk_calcTyping(type_t, type_t, type_t);

// Type settings
void pk_setTypeRes(type_t*, int, int*);
void pk_setTypeWeak(type_t*, int, int*);
void pk_setTypeInv(type_t*, int, int*);

// Monster Functions
bool pk_useMove(int numb, monster_t* mon);
void pk_setMove(int moveVal, int cpp, int bpp, int moveSlot, monster_t* mon);

// Moves Functions
bool pk_m_nomove(monster_t* a, monster_t* d);
bool pk_m_tackle(monster_t* a, monster_t* d);
bool pk_m_explosion(monster_t* a, monster_t* d);

#endif