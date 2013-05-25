/* Super Glitch Engine Client
 * main.c
 * Copyright Tyler Compton 2013
 *
 * Main.c file for the Client portion of SGE. The Client is the program your 
 * end user would actually run.
 */

#pragma GCC diagnostic ignored "-Wwrite-strings"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <stdbool.h>

#include "fileheaders.h"

#include "sge.h"

bool quit = false;
bool mute = false;

// PokeEngine stuff
sessionMan_t ses;

// Framerate stuff
struct timeb lastTime, lastTimeS;
int physDelay = PHYS_DELAY;

// SDL Music
Mix_Music* music = NULL;

// SDL Surfaces
SDL_Surface* s_screen = NULL;
SDL_Surface* s_player = NULL;
SDL_Surface* s_mapTile = NULL;
SDL_Surface* s_npc = NULL;
SDL_Surface* s_chars = NULL;
SDL_Surface* s_fPkmn = NULL;
SDL_Surface* s_bPkmn = NULL;

// SDL Clippers
SDL_Rect clipTiles[50];
SDL_Rect clipNPCs[70];
SDL_Rect clipChars[88];
SDL_Rect clipPkmn[8];

// SDL Etc
SDL_Event event;
Uint8 keyStatesBuf[512];

int init()
{
	if(SDL_Init( SDL_INIT_EVERYTHING ) == -1) {
		return -1;
	}

	s_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("Super Glitch Engine", NULL);

    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return -1;
    }

	return 0;
}

// Common Menu Functions
void quitGame() {
	quit = true;
}

void loadMap(int val) {
	FILE * fMap;
	mapHeader_f header;
	fMap = fopen("resources/maps/map.pke", "r");
	if(fMap == NULL) {
		printf("[ERROR] Missing map.pke in resources/maps folder\n");
	}
	fread(&header.count, sizeof(int), 1, fMap);

	printf("Loading %i Rooms...\n", header.count);

	int doorCnt = 0;
	roomHeader_f rHeader[header.count];
	for(int i=0; i<=val; i++) {
		fread(&rHeader[i].value, sizeof(char), 1, fMap);
		fread(&rHeader[i].w, sizeof(int), 1, fMap);
		fread(&rHeader[i].h, sizeof(int), 1, fMap);

		if(i==val) {
			printf("   Found room %i! W: %i H: %i\n", i, rHeader[i].w, rHeader[i].h);
			printf("   Map Size: %i\n", rHeader[i].w*rHeader[i].h);
			ses.map.width = rHeader[i].w;
			ses.map.height = rHeader[i].h;
			pk_initMap(ses.map.width, ses.map.height, &ses.map);
			for(int j=0; j<rHeader[i].w*rHeader[i].h; j++) {
				fread(&ses.map.data[j], sizeof(char), 1, fMap);
			}
		} else {
			fseek(fMap, rHeader[i].w*rHeader[i].h*sizeof(char), SEEK_CUR);
			fread(&doorCnt, sizeof(int), 1, fMap);
			fseek(fMap, (sizeof(char)+(sizeof(int)*4))*doorCnt, SEEK_CUR);
			int npcCnt;
			fread(&npcCnt, sizeof(int), 1, fMap);
			fseek(fMap, (sizeof(int)*3)*npcCnt, SEEK_CUR);
		}
	}

	fread(&doorCnt, sizeof(int), 1, fMap);
	door_t doorData[doorCnt];
	for(int i=0; i<doorCnt; i++) {
		fread(&doorData[i].dest, sizeof(char), 1, fMap);
		fread(&doorData[i].x, sizeof(int), 1, fMap);
		fread(&doorData[i].y, sizeof(int), 1, fMap);
		fread(&doorData[i].destX, sizeof(int), 1, fMap);
		fread(&doorData[i].destY, sizeof(int), 1, fMap);
	}
	pk_setDoorData(doorCnt, &doorData[0], &ses.map);
	printf("   Map Door Count: %i\n", doorCnt);

	int npcCnt;
	fread(&npcCnt, sizeof(int), 1, fMap);
	mapNpc_t npcData[npcCnt];
	for(int i=0; i<npcCnt; i++) {
		fread(&npcData[i].val, sizeof(int), 1, fMap);
		fread(&npcData[i].x, sizeof(int), 1, fMap);
		fread(&npcData[i].y, sizeof(int), 1, fMap);
	}
	pk_setNpcData(npcCnt, &npcData[0], &ses.map);
	printf("   Map NPC Count: %i\n", ses.map.npcCnt);

	fclose(fMap);

	pk_spruneNpcs(&ses);
}

void setTypes() {
	FILE * fTypes;
	typeHeader_f header;
	fTypes = fopen("resources/data/type.pke", "r");
	if(fTypes == NULL) {
		printf("[ERROR] Missing type.pke in resources/data folder!\n");
	}
	fread(&header.count, sizeof(int), 1, fTypes);
	typeEntry_f entry[header.count];
	for(int i=0; i<header.count; i++) {
		fread(&entry[i].name, sizeof(char[8]), 1, fTypes);
		fread(&entry[i].resCnt, sizeof(int), 1, fTypes);
		fread(&entry[i].weakCnt, sizeof(int), 1, fTypes);
		fread(&entry[i].invCnt, sizeof(int), 1, fTypes);

		int rEntry[entry[i].resCnt];
		int wEntry[entry[i].weakCnt];
		int iEntry[entry[i].invCnt];

		for(int j=0; j<entry[i].resCnt; j++) {
			fread(&rEntry[j], sizeof(int), 1, fTypes);
		}

		for(int j=0; j<entry[i].weakCnt; j++) {
			fread(&wEntry[j], sizeof(int), 1, fTypes);
		}

		for(int j=0; j<entry[i].invCnt; j++) {
			fread(&iEntry[j], sizeof(int), 1, fTypes);
		}

		ses.types[i] = pk_initType(entry[i].name, i);

		pk_setTypeRes(&ses.types[i], entry[i].resCnt, &rEntry[0]);
		pk_setTypeWeak(&ses.types[i], entry[i].weakCnt, &wEntry[0]);
		pk_setTypeInv(&ses.types[i], entry[i].invCnt, &iEntry[0]);
	}

	printf("Read type info.\n");
	printf("   %i types found.\n", header.count);
}

// Object Loading Functions
void setMonsters()
{
	ses.moves[PK_NOMOVE] 	= pk_initMove( 0,  0, "NULL        ", T_NONE, &pk_m_nomove);
	ses.moves[PK_TACKLE] 	= pk_initMove(30, 30, "TACKLE      ", T_NORMAL, &pk_m_tackle);
	ses.moves[PK_EXPLOSION] = pk_initMove(15, 15, "EXPLOSION   ", T_NORMAL, &pk_m_explosion);

	FILE * fMons;
	monHeader_f header;
	fMons = fopen("resources/data/pk.pke", "r");
	if(fMons == NULL) {
		printf("[ERROR] Missing pk.pke in resources/data folder!\n");
	}
	fread(&header.count, sizeof(int), 1, fMons);
	monEntry_f mons[header.count];
	for(int i=0; i<header.count; i++) {
		fread(&mons[i], sizeof(monEntry_f), 1, fMons);
		ses.bMons[mons[i].value] = pk_initBaseMonster(pk_initStats(30, mons[i].att, mons[i].def, mons[i].spAtt, mons[i].spDef,
			mons[i].speed), pk_initStats(0,0,0,0,0,0), mons[i].value, mons[i].name);
	}

	printf("Read monster info.\n");
	printf("   %i monsters found.\n", header.count);
}

void loadNpcs() {
	FILE * fNpcs;
	fNpcs = fopen("resources/data/npc.pke", "r");
	if(fNpcs == NULL) {
		printf("[ERROR] Missing npc.pke in resources/data folder!\n");
	}

	printf("Loading NPCs...\n");
	npcFileHeader_f header;
	fread(&header.count, sizeof(int), 1, fNpcs);
	printf("   Found %i Npcs\n", header.count);

	npc_f npc[header.count];
	for(int i=0; i<header.count; i++) {
		fread(&npc[i].aiType, sizeof(int), 1, fNpcs);

		fread(&npc[i].destX, sizeof(int), 1, fNpcs);
		fread(&npc[i].destY, sizeof(int), 1, fNpcs);

		fread(&npc[i].sprite, sizeof(int), 1, fNpcs);
		fread(&npc[i].reach, sizeof(int), 1, fNpcs);

		fread(&npc[i].fightable, sizeof(bool), 1, fNpcs);
		for(int j=0; j<6; j++) {
			fread(&npc[i].monsters[j], sizeof(int), 1, fNpcs);
		}

		fread(&npc[i].msg1, sizeof(char), 128, fNpcs);
		fread(&npc[i].msg2, sizeof(char), 128, fNpcs);
		fread(&npc[i].msg3, sizeof(char), 128, fNpcs);

		for(int j=0; j<128; j++) {
			if(npc[i].msg1[j] == '_') {
				npc[i].msg1[j] = ' ';
			}
			if(npc[i].msg2[j] == '_') {
				npc[i].msg2[j] = ' ';
			}
			if(npc[i].msg3[j] == '_') {
				npc[i].msg3[j] = ' ';
			}
		}

		ses.npcs[i] = pk_initNpc(BLOCK_SIZE*0, BLOCK_SIZE*0,
			BLOCK_SIZE*npc[i].destX, BLOCK_SIZE*npc[i].destY, npc[i].reach,
			npc[i].sprite, LEFT, npc[i].aiType, !npc[i].fightable);

		for(int j=0; j<6; j++) {
			pk_setNpcMonster(pk_initMonster(25, 50, &ses.bMons[npc[i].monsters[j]], false, 
				ses.bMons[npc[i].monsters[j]].bs), &ses.npcs[i]);
			for(int k=0; k<4; k++) {
				pk_setMove(PK_TACKLE, ses.moves[PK_TACKLE].bpp, ses.moves[PK_TACKLE].bpp, k, &ses.npcs[i].monsters[j]);
			}
		}

		pk_initWindow(0, SCREEN_HEIGHT-(6*CHAR_SIZE), WIND_WIDTH*2, 6, true, false, &ses.npcs[i].dialog);

		pk_setMessage(1, &npc[i].msg1[0], &ses.npcs[i]);
		pk_setMessage(2, &npc[i].msg1[0], &ses.npcs[i]);
		pk_setMessage(3, &npc[i].msg1[0], &ses.npcs[i]);

		pk_switchMessage(1, &ses.npcs[i]);
	}

	fclose(fNpcs);
	printf("Read NPC info.\n");
}

void setPlayer()
{
	ses.p1 = pk_pinit(6*BLOCK_SIZE,11*BLOCK_SIZE, C_PLAYER);

	pk_psetMonster(pk_initMonster(20, 2, &ses.bMons[PK_CHARIZARD], false, 
		ses.bMons[PK_CHARIZARD].bs), &ses.p1);

	for(int i=0; i<4; i++) {
		if(i == 3) {
			pk_setMove(PK_EXPLOSION, ses.moves[PK_EXPLOSION].bpp, ses.moves[PK_EXPLOSION].bpp, i, &ses.p1.monsters[0]);
		} else {
			pk_setMove(PK_TACKLE, ses.moves[PK_TACKLE].bpp, ses.moves[PK_TACKLE].bpp, i, &ses.p1.monsters[0]);
		}
	}
}

void setWindows()
{
	pk_setWOptionFunc(6, &quitGame, &ses.w_menu);
}

void setClips()
{
	for(int i=0; i<50; i++) {
		clipTiles[i].x = 0;
		clipTiles[i].y = BLOCK_SIZE*i;
		clipTiles[i].w = clipTiles[i].h = BLOCK_SIZE;
	}
	for(int i=0; i<70; i++) {
		clipNPCs[i].x = 0;
		clipNPCs[i].y = BLOCK_SIZE*i;
		clipNPCs[i].w = clipNPCs[i].h = BLOCK_SIZE;
	}
	for(int i=0; i<88; i++) {
		clipChars[i].x = 0;
		clipChars[i].y = CHAR_SIZE*i;
		clipChars[i].w = clipChars[i].h = CHAR_SIZE;
	}
	for(int i=0; i<6; i++) {
		clipPkmn[i].x = 0;
		clipPkmn[i].y = PKMN_SIZE*i;
		clipPkmn[i].w = clipPkmn[i].h = PKMN_SIZE;
	}
}

// SDL Shortcut Functions
void draw_rect(int x, int y, int w, int h, int color) {
	SDL_Rect rect = {x,y,w,h};
	SDL_FillRect(s_screen, &rect, color);
}

SDL_Surface *load_image(char* filename)
{
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optImage = NULL;

	loadedImage = IMG_Load(filename);

	if(loadedImage != NULL) {
		optImage = SDL_DisplayFormatAlpha(loadedImage);

		SDL_FreeSurface(loadedImage);
	}

	return optImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip, bool scrolls)
{
	SDL_Rect offset;

	if(scrolls) {
		offset.x = x - (ses.p1.mover.x-(SCREEN_WIDTH/2))-BLOCK_SIZE;
		offset.y = y - (ses.p1.mover.y-(SCREEN_HEIGHT/2));
	} else {
		offset.x = x;
		offset.y = y;
	}

	SDL_BlitSurface(source, clip, destination, &offset);
}

void loadSprites()
{
	s_player = load_image("resources/sprites/npcs.png");
	s_npc = load_image("resources/sprites/npcs.png");
	if(s_player == NULL || s_npc == NULL) {
		printf("ERROR: Couldn't load NPC sprites!\n");
	}
	s_mapTile = load_image("resources/sprites/tiles.png");
	if(s_mapTile == NULL) {
		printf("ERROR: Couldn't load tile sprites!\n");
	}
	s_chars = load_image("resources/sprites/characters.png");
	if(s_chars == NULL) {
		printf("ERROR: Couldn't load character sprites!\n");
	}
	s_fPkmn = load_image("resources/sprites/FrontPkmn.png");
	if(s_fPkmn == NULL) {
		printf("ERROR: Couldn't load front pokemon sprites!\n");
	}
	s_bPkmn = load_image("resources/sprites/BackPkmn.png");
	if(s_bPkmn == NULL) {
		printf("ERROR: Couldn't load back pokemon sprites!\n");
	}
	music = Mix_LoadMUS( "resources/music/vermillion.wav" );
	if(music == NULL) {
		printf("ERROR: Loading music!\n");
	}
}

void unloadSprites()
{
	SDL_FreeSurface(s_player);
	SDL_FreeSurface(s_npc);
	SDL_FreeSurface(s_mapTile);
	SDL_FreeSurface(s_chars);
	SDL_FreeSurface(s_fPkmn);
	SDL_FreeSurface(s_bPkmn);

	Mix_CloseAudio();
	SDL_Quit();
}

npc_t* isHere(int thing, int x, int y)
{
	switch(thing) {
	case NPC:
		for(int i=0; i<MAX_NPCS; i++) {
			if(ses.npcs[i].active && ses.npcs[i].mover.x == x && ses.npcs[i].mover.y == y &&
			   ses.npcs[i].mover.x == ses.npcs[i].mover.nextX && 
			   ses.npcs[i].mover.y == ses.npcs[i].mover.nextY) {
				return &ses.npcs[i];
			}
		}
		break;
	}

	return NULL;
}

void checkKeys(Uint8 *keyStates)
{
	col_t col = pk_findCols(ses.map, ses.p1.mover.x/BLOCK_SIZE, ses.p1.mover.y/BLOCK_SIZE);

	if(ses.mode == SES_OVERWORLD && !ses.windOpen) {
		if(keyStates[SDLK_DOWN]) {
			pk_pMove(DOWN, col.down, &ses.p1);
		} else if(keyStates[SDLK_UP]) {
			pk_pMove(UP, col.up, &ses.p1);
		} else if(keyStates[SDLK_LEFT]) {
			pk_pMove(LEFT, col.left, &ses.p1);
		} else if(keyStates[SDLK_RIGHT]) {
			pk_pMove(RIGHT, col.right, &ses.p1);
		}

		if(keyStates[SDLK_z]) {
			if(keyStatesBuf[SDLK_z] == false && ses.p1.mover.x == ses.p1.mover.nextX && ses.p1.mover.y == ses.p1.mover.nextY) {
				switch(ses.p1.mover.dir) {
				npc_t *tNPC;
				case LEFT:
					tNPC = isHere(NPC, ses.p1.mover.x-BLOCK_SIZE, ses.p1.mover.y);
					if(tNPC != NULL) {
						pk_toggleWindow(&tNPC->dialog);
						tNPC->mover.dir = RIGHT;
						ses.currWindow = &tNPC->dialog;
					}
					break;
				case RIGHT:
					tNPC = isHere(NPC, ses.p1.mover.x+BLOCK_SIZE, ses.p1.mover.y);
					if( tNPC != NULL) {
						pk_toggleWindow(&tNPC->dialog);
						tNPC->mover.dir = LEFT;
						ses.currWindow = &tNPC->dialog;
					}
					break;
				case UP:
					tNPC = isHere(NPC, ses.p1.mover.x, ses.p1.mover.y-BLOCK_SIZE);
					if( tNPC != NULL) {
						pk_toggleWindow(&tNPC->dialog);
						tNPC->mover.dir = DOWN;
						ses.currWindow = &tNPC->dialog;
					}
					break;
				case DOWN:
					tNPC = isHere(NPC, ses.p1.mover.x, ses.p1.mover.y+BLOCK_SIZE);
					if( tNPC != NULL) {
						pk_toggleWindow(&tNPC->dialog);
						tNPC->mover.dir = UP;
						ses.currWindow = &tNPC->dialog;
					}
					break;
				}
			}
			keyStatesBuf[SDLK_z] = true;
		} else {
			keyStatesBuf[SDLK_z] = false;
		}
	} else if(ses.mode == SES_BATTLE && !ses.windOpen) {

	} else if(ses.windOpen) {
		if(keyStates[SDLK_DOWN]) {
			if(keyStatesBuf[SDLK_DOWN] == false) {
				pk_moveSelArrow(DOWN, ses.currWindow);
				keyStatesBuf[SDLK_DOWN] = true;
			}
		} else {
			keyStatesBuf[SDLK_DOWN] = false;
		}
		if(keyStates[SDLK_UP]) {
			if(keyStatesBuf[SDLK_UP] == false) {
				pk_moveSelArrow(UP, ses.currWindow);
				keyStatesBuf[SDLK_UP] = true;
			}
		} else {
			keyStatesBuf[SDLK_UP] = false;
		}

		if(keyStates[SDLK_x]) {
			if(keyStatesBuf[SDLK_x] == false) {
				pk_scrollWindowText(ses.currWindow);
				keyStatesBuf[SDLK_x] = true;
			}
		} else {
			keyStatesBuf[SDLK_x] = false;
		}

		if(keyStates[SDLK_z]) {
			if(keyStatesBuf[SDLK_z] == false) {
				pk_selectWindow(ses.currWindow);
				keyStatesBuf[SDLK_z] = true;
			}
		} else {
			keyStatesBuf[SDLK_z] = false;
		}
	}

	if(keyStates[SDLK_b]) {
		if(keyStatesBuf[SDLK_b] == false && ses.mode == SES_OVERWORLD) {
			pk_sstartBattleW(ses.npcs[1].monsters[ses.npcs[1].currMon], &ses);
			pk_sstepBattle(&ses, BATS_WPA, ses.npcs[1].monsters[ses.npcs[1].currMon], ses.attWild, 0);
			keyStatesBuf[SDLK_b] = true;
		} else if(keyStatesBuf[SDLK_b] == false && ses.mode == SES_BATTLE) {
			pk_switchMode(SES_OVERWORLD, &ses);
			keyStatesBuf[SDLK_b] = true;
			ses.currWindow = NULL;
		}
	} else {
		keyStatesBuf[SDLK_b] = false;
	}

	if(keyStates[SDLK_m]) {
		if(keyStatesBuf[SDLK_m] == false && ses.p1.mover.x == ses.p1.mover.nextX && ses.p1.mover.y == ses.p1.mover.nextY) {
			pk_toggleWindow(&ses.w_menu);
			ses.currWindow = &ses.w_menu;
			keyStatesBuf[SDLK_m] = true;
		}
	} else {
		keyStatesBuf[SDLK_m] = false;
	}

	if(keyStates[SDLK_l]) {
		physDelay = 0;
	} else {
		physDelay = PHYS_DELAY;
	}

	if(keyStates[SDLK_p]) {
		if(keyStatesBuf[SDLK_p] == false) {
			Mix_PlayMusic(music, -1);
			keyStatesBuf[SDLK_p] = true;
		}
	} else {
		keyStatesBuf[SDLK_p] = false;
	}

	if(keyStates[SDLK_r]) {
		if(keyStatesBuf[SDLK_r] == false) {
			int room = 0;
			printf("Room? ");
			scanf("%i", &room);
			loadMap(room);
			keyStatesBuf[SDLK_r] = true;
		}
	} else {
		keyStatesBuf[SDLK_r] = false;
	}
}

void drawWindow(window_t wind)
{
	int tX = wind.x;
	int tY = wind.y;
	for(int y=0; y<wind.h; y++) {
		for(int x=0; x<wind.w; x++) {
			apply_surface(tX, tY, s_chars, s_screen, &clipChars[wind.dispTiles[x+(y*wind.w)]], false);
			tX+=8;
		}
		tY+=8;
		tX = wind.x;
	}
}

void drawBattle() {
	for(int i=0; i<12; i++) {
		apply_surface(8+(i*8), 8*0, s_chars, s_screen, &clipChars[pk_getCharValue(ses.attWild.name[i])], false);
	}

	apply_surface(8*4, 8*1, s_chars, s_screen, &clipChars[LEVEL], false);
	apply_surface(8*1, 8*2, s_chars, s_screen, &clipChars[LINE_V], false);
	apply_surface(8*1, 8*3, s_chars, s_screen, &clipChars[LINE_BL], false);

	for(int i=0; i<9; i++) {
		apply_surface(8*i+(8*2), 8*3, s_chars, s_screen, &clipChars[LINE_H], false);
	}
	apply_surface(8*10, 8*3, s_chars, s_screen, &clipChars[LINE_RARROW], false);

	apply_surface(8*2, 8*2, s_chars, s_screen, &clipChars[BATTLE_H], false);
	apply_surface(8*3, 8*2, s_chars, s_screen, &clipChars[BATTLE_P], false);
	apply_surface(8*4, 8*2, s_chars, s_screen, &clipChars[HPBAR_L], false);
	for(int i=0; i<6; i++) {
		apply_surface(8*5+(8*i), 8*2, s_chars, s_screen, &clipChars[HPBAR_MID], false);
	}
	apply_surface(8*10, 8*2, s_chars, s_screen, &clipChars[HPBAR_R], false);

	for(int i=0; i<12; i++) {
		apply_surface(8*11+(i*8), 8*7, s_chars, s_screen,
			&clipChars[pk_getCharValue(ses.p1.monsters[ses.p1.currMon].name[i])], false);
	}

	apply_surface(8*14, 8*8, s_chars, s_screen, &clipChars[LEVEL], false);
	apply_surface(8*9, 8*11, s_chars, s_screen, &clipChars[LINE_LARROW], false);

	for(int i=0; i<8; i++) {
		apply_surface(8*10+(8*i), 8*11, s_chars, s_screen, &clipChars[LINE_H], false);
	}
	apply_surface(8*18, 8*11, s_chars, s_screen, &clipChars[LINE_BR], false);
	apply_surface(8*18, 8*10, s_chars, s_screen, &clipChars[LINE_V], false);
	apply_surface(8*18, 8*9, s_chars, s_screen, &clipChars[LINE_V], false);

	apply_surface(8*12, 8*9, s_chars, s_screen, &clipChars[HPBAR_L], false);
	for(int i=0; i<5; i++) {
		apply_surface(8*(13+i), 8*9, s_chars, s_screen, &clipChars[HPBAR_MID], false);
	}
	apply_surface(8*18, 8*9, s_chars, s_screen, &clipChars[HPBAR_R], false);

	apply_surface(8*14, 8*10, s_chars, s_screen, &clipChars[SLASH], false);

	draw_rect(8*12+3, 8*9+3, 46*((double)ses.p1.monsters[ses.p1.currMon].stats.mHp / 
		(double)ses.p1.monsters[ses.p1.currMon].stats.hp), 2, 254);

	draw_rect(8*4+3, 8*2+3, 46*((double)ses.attWild.stats.mHp/(double)ses.attWild.stats.hp), 2, 254);

	apply_surface(0, SCREEN_HEIGHT-(PKMN_SIZE)-(6*CHAR_SIZE), s_bPkmn, s_screen,
		&clipPkmn[ses.p1.monsters[ses.p1.currMon].id->backSpr], false);
	apply_surface(SCREEN_WIDTH-PKMN_SIZE, 0, s_fPkmn, s_screen,
		&clipPkmn[ses.npcs[1].monsters[ses.npcs[1].currMon].id->backSpr], false);
}

void graphics()
{
	if(ses.mode == SES_OVERWORLD) {
		int mapX, mapY = 0;
		int incMapY = 1;

		for(int i=0; i<ses.map.width*ses.map.height; i++) {
			apply_surface(mapX, mapY, s_mapTile, s_screen, &clipTiles[ses.map.data[i]], true);
			mapX+=BLOCK_SIZE;

			if(incMapY == ses.map.width) {
				incMapY = 1;
				mapX = 0;
				mapY+=BLOCK_SIZE;
			} else {
				incMapY++;
			}
		}

		for(int i=0; i<MAX_NPCS; i++) {
			if(ses.npcs[i].active) {
				apply_surface(ses.npcs[i].mover.x, ses.npcs[i].mover.y-4, s_npc, s_screen, 
					&clipNPCs[pk_getNpcFrame(ses.npcs[i])], true);
				if(ses.npcs[i].aggro && ses.npcs[i].actClock > 0) {
					apply_surface(ses.npcs[i].mover.x, ses.npcs[i].mover.y-BLOCK_SIZE-4, s_mapTile, s_screen,
						&clipTiles[WORD_BUBBLE], true);
				}
			}

		}

		apply_surface(ses.p1.mover.x, ses.p1.mover.y-4, s_player, s_screen, &clipNPCs[pk_pgetFrame(ses.p1)], true);

	} else if(ses.mode == SES_BATTLE) {
		drawBattle();

		drawWindow(ses.w_bDialog);
		if(ses.w_bMenu.active) {
			drawWindow(ses.w_bMenu);
		}
		if(ses.w_bMoves.active) {
			drawWindow(ses.w_bMoves);
		}
		if(ses.w_bMoveInfo.active) {
			drawWindow(ses.w_bMoveInfo);
		}
	}

	if(ses.w_menu.active) {
		drawWindow(ses.w_menu);
	}

	for(int i=0; i<MAX_NPCS; i++) {
		if(ses.npcs[i].dialog.active)
			drawWindow(ses.npcs[i].dialog);
	}
}

void physics()
{
	if(ses.currWindow != NULL && ses.currWindow->active) {
		ses.windOpen = true;
	} else {
		ses.windOpen = false;
		ses.currWindow = NULL;
	}

	if(ses.battleStep == BATS_WPA && ses.w_bDialog.finished) {
		pk_sstepBattle(&ses, BATS_GO, ses.attWild, ses.p1.monsters[ses.p1.currMon], 0);
	}

	if(ses.battleStep == BATS_GO && ses.w_bDialog.finished) {
		pk_sstepBattle(&ses, BATS_SEL, ses.p1.monsters[ses.p1.currMon], ses.attWild, 0);
	}

	pk_supdateMapCols(ALL, &ses);
	pk_buildColMapC(ses.p1.mover, &ses.map);

	pk_supdateWindows(&ses);
	pk_supdateNpcs(&ses);

	pk_updateChar(&ses.p1.mover);

	if(pk_isFinishedMoving(ses.p1.mover) && ses.p1.mover.animCycle > 0) {
		door_t door;
		switch(ses.p1.mover.dir) {
		case LEFT:
			door = pk_isOnDoor((ses.p1.mover.x)/BLOCK_SIZE-1, (ses.p1.mover.y)/BLOCK_SIZE, &ses.map);
			break;
		case RIGHT:
			door = pk_isOnDoor((ses.p1.mover.x)/BLOCK_SIZE+1, (ses.p1.mover.y)/BLOCK_SIZE, &ses.map);
			break;
		case UP:
			door = pk_isOnDoor((ses.p1.mover.x)/BLOCK_SIZE, (ses.p1.mover.y)/BLOCK_SIZE-1, &ses.map);
			break;
		case DOWN:
			door = pk_isOnDoor((ses.p1.mover.x)/BLOCK_SIZE, (ses.p1.mover.y)/BLOCK_SIZE+1, &ses.map);
			break;
		}
		if(door.dest != -1) {
			loadMap(door.dest);
			ses.p1.mover.x = ses.p1.mover.nextX = door.destX*BLOCK_SIZE;
			ses.p1.mover.y = ses.p1.mover.nextY = door.destY*BLOCK_SIZE;
		}
	}

	door_t door = pk_isOnDoor(ses.p1.mover.x/BLOCK_SIZE, ses.p1.mover.y/BLOCK_SIZE, &ses.map);
	if(door.dest != -1 && door.type == DT_WALKINTO) {
		if(ses.p1.mover.x == ses.p1.mover.nextX && ses.p1.mover.y == ses.p1.mover.nextY) {
			loadMap(door.dest);
			ses.p1.mover.x = ses.p1.mover.nextX = door.destX*BLOCK_SIZE;
			ses.p1.mover.y = ses.p1.mover.nextY = door.destY*BLOCK_SIZE;
		}
	}
}

void gameLoop()
{
	graphics();

	Uint8 *keystates = SDL_GetKeyState(NULL);
	checkKeys(keystates);

	struct timeb currTime;
	ftime(&currTime);

	int cTime, lTime;
	cTime = (currTime.time)*1000;
	cTime+= currTime.millitm;
	lTime = (lastTime.time)*1000;
	lTime+= lastTime.millitm;

	if(cTime-lTime >= physDelay) {
		physics();
		ftime(&lastTime);
	}
}

int main(int argc, char **argv)
{
	printf("==========================================\n");
	printf("|           PokeEngine V0.0.01           |\n");
	printf("==========================================\n\n");

	pk_initSMan(SES_OVERWORLD, &ses);
	setTypes();
	setMonsters();
	setWindows();
	setPlayer();
	loadNpcs();
	loadMap(0);

	ftime(&lastTime);

	if(init() == -1) {
		return 1;
	}

	loadSprites();
	setClips();

	if(SDL_Flip(s_screen) == -1) {
		return 1;
	}

	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = true;
				unloadSprites();
				exit(0);
			}
		}

		if(ses.mode == SES_OVERWORLD) {
			SDL_FillRect( s_screen, &s_screen->clip_rect, SDL_MapRGB( s_screen->format, 0x00, 0x00, 0x00 ) );
		} else if(ses.mode == SES_BATTLE) {
			SDL_FillRect( s_screen, &s_screen->clip_rect, SDL_MapRGB( s_screen->format, 0xFF, 0xFF, 0xFF ) );
		}

		gameLoop();

		SDL_Flip(s_screen);
	}

	return 0;
}
