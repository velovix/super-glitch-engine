#ifndef SESSIONMAN_H
#define SESSIONMAN_H

#include "../constants.h"
#include "../battle/monsters.h"
#include "../ai/trainer_npc.h"
#include <stdbool.h>

typedef struct {
	int mode, currMap, battleStep, battleType;
	bool windOpen;

	monster_t attWild;
	trainerNPC_t* attTrainer;

	trainerNPC_t npcs[MAX_NPCS];
	move_t moves[MAX_MOVES];
	baseMonster_t bMons[MAX_MONSTERS];

	window_t w_menu;
	window_t w_bDialog;
	window_t w_bMenu;
	window_t* currWindow;

	map_t map;

} sessionMan_t;

void pk_initSMan(int, sessionMan_t*);
void pk_switchMode(int, sessionMan_t*);
void pk_snewMap(char*, sessionMan_t*);
void pk_supdateMapCols(int, sessionMan_t*);
void pk_sstartBattleW(monster_t, sessionMan_t*);
void pk_sstartBattleT(trainerNPC_t*, sessionMan_t*);
void pk_sstepBattle(monster_t pMon, sessionMan_t*);
void pk_supdate(sessionMan_t*);
void pk_supdateNpcs(sessionMan_t*);

#endif