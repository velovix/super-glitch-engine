/* Super Glitch Engine Client
 * fileheaders.h
 * Copyright Tyler Compton 2013
 *
 * Declares structs for various information headers in SGE files.
 */

#ifndef FILEHEADERS_H
#define FILEHEADERS_H

// --- Monsters (VERY INDEV)
typedef struct {
	int count;
} monHeader_f;

typedef struct {
	int value;
	char name[12];
	int hp, att, def, spAtt, spDef, speed;
} monEntry_f;


// --- NPCs (VERY INDEV)
typedef struct {
	int count;
} npcFileHeader_f;

typedef struct {
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
