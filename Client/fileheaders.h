/* Super Glitch Engine Client
 * fileheaders.h
 * Copyright Tyler Compton 2013
 *
 * Declares structs for various information headers in SGE files.
 */

#ifndef FILEHEADERS_H
#define FILEHEADERS_H

// --- Monsters

typedef struct {
	int count;
} monHeader_f;

typedef struct {
	int value;
	char name[12];
	int hp, att, def, spAtt, spDef, speed;
} monEntry_f;

// --- Types

typedef struct {
	int count;
} typeHeader_f;

typedef struct {
	char name[8];
	int resCnt;
	int weakCnt;
	int invCnt;
} typeEntry_f;

typedef struct {
	int type;
} resEntry_f;

typedef struct {
	int type;
} weakEntry_f;

typedef struct {
	int type;
} invEntry_f;

// --- Maps

typedef struct {
	int count;
} mapHeader_f;

typedef struct {
	int value;
	int w, h;
} roomHeader_f;

typedef struct {
	int count;
} npcListHeader_f;

typedef struct {
	char value;
	int x, y;
} npcEntry_f;

typedef struct {
	int count;
} npcFileHeader_f;

typedef struct {
	int x, y;
	int destX, destY;
	int aiType;
	int sprite;
	int reach;

	bool fightable;
	int monsters[6];

	char msg1[128];
	char msg2[128];
	char msg3[128];
} npc_f;

#endif