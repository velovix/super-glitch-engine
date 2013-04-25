#ifndef MONSTERS_H
#define MONSTERS_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	int att, def, spAtt, spDef, speed;
	int mAtt, mDef, mSpAtt, mSpDef, mSpeed, mAcc, mEvas;
} stats_t;

typedef struct {
	int frntSpr, backSpr, iconSpr;
	stats_t bs;
	stats_t ev;

	char name [12];

} baseMonster_t;

typedef struct {
	int health, experience;
	char name [12];
	baseMonster_t* id;
	bool shiny;
	stats_t stats;
	int moves[4];

} monster_t;

typedef struct {
	int cpp, bpp;
	char name [12];
	bool (*movePtr) (monster_t*, monster_t*);
} move_t;

// Initializers
stats_t pk_initStats(int att, int def, int spAtt, int spDef, int speed);
move_t pk_initMove(int s_cpp, int s_bpp, char* name, bool (*s_movePtr) (monster_t*, monster_t*));
monster_t pk_initMonster(int s_health, int s_experience, baseMonster_t* s_id, bool s_shiny,
	stats_t s_stats, int* s_moves);
baseMonster_t pk_initBaseMonster(stats_t baseStats, stats_t baseEVs, int gID, char* name);

void pk_damage(int amount, monster_t* victim);

// Moves Functions
bool pk_m_nomove(monster_t* a, monster_t* d);
bool pk_m_tackle(monster_t* a, monster_t* d);
bool pk_m_explosion(monster_t* a, monster_t* d);

#endif