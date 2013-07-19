/* Super Glitch Engine Client
 * sessionman.h
 * Copyright Tyler Compton 2013
 *
 * Declares the sessionMan_t class (short for session manager). sessionMan_t
 * acts as a wrapper for most other structures. It's designed so that the
 * main.c file need only declare a sessionMan_t object, which will then manage
 * all other SGE objects.
 */

#ifndef SESSIONMAN_H
#define SESSIONMAN_H

#include "../constants.h"
#include "../battle/monsters.h"
#include "../ai/npc.h"
#include "../ai/player.h"
 #include "../battle/moves.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int mode, currMap, battleStep, battleType;
	bool windOpen;

	monster_t attWild;
	npc_t* attTrainer;

	player_t p1;
	npc_t npcs[MAX_NPCS];
	move_t moves[MAX_MOVES];
	baseMonster_t bMons[MAX_MONSTERS];
	type_t types[MAX_TYPES];

	window_t w_menu;
	window_t w_bDialog, w_bMenu, w_bMoves, w_bMoveInfo;
	window_t* currWindow;

	map_t map;

} sessionMan_t;

void pk_initSMan(int, sessionMan_t*);
void pk_switchMode(int, sessionMan_t*);
void pk_snewMap(char*, sessionMan_t*);
void pk_supdateMapCols(int, sessionMan_t*);
void pk_sstartBattleW(monster_t, sessionMan_t*);
void pk_sstartBattleT(npc_t*, sessionMan_t*);
void pk_sstepBattle(sessionMan_t* ses, int step, monster_t aMon, monster_t dMon, int move);
void pk_ssetMoveWind(monster_t mon, sessionMan_t* ses);
void pk_supdateWindows(sessionMan_t*);
void pk_supdateNpcs(sessionMan_t*);
void pk_spruneNpcs(sessionMan_t*);

#endif