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

#include "PokeEngine.h"

bool quit = false;
bool mute = false;

// PokeEngine stuff
player_t p1;
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
SDL_Rect clipTiles[20];
SDL_Rect clipNPCs[60];
SDL_Rect clipChars[70];
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
	SDL_WM_SetCaption("PokeEngine", NULL);

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

// Object Loading Functions
void setMonsters()
{
	ses.moves[PK_NOMOVE] 	= pk_initMove( 0,  0, "---         ", &pk_m_nomove);
	ses.moves[PK_TACKLE] 	= pk_initMove(30, 30, "TACKLE      ", &pk_m_tackle);
	ses.moves[PK_EXPLOSION] = pk_initMove( 5,  5, "EXPLOSION   ", &pk_m_explosion);

	FILE * fMons;
	monHeader_t header;
	fMons = fopen("resources/data/pk.pke", "r");
	if(fMons == NULL) {
		printf("ERROR: Missing pk.pke in resources/data folder!\n");
	}
	fread(&header.count, sizeof(int), 1, fMons);
	monEntry_t mons[header.count];
	for(int i=0; i<header.count; i++) {
		fread(&mons[i], sizeof(monEntry_t), 1, fMons);
		ses.bMons[mons[i].value] = pk_initBaseMonster(pk_initStats(mons[i].att, mons[i].def, mons[i].spAtt, mons[i].spDef,
			mons[i].speed), pk_initStats(0,0,0,0,0), mons[i].value, mons[i].name);
	}
}

void setNpcs()
{
	p1 = pk_pinit(10*BLOCK_SIZE,11*BLOCK_SIZE, C_PLAYER);
	ses.npcs[0] = pk_tinit(BLOCK_SIZE*12, BLOCK_SIZE*13, BLOCK_SIZE*12, BLOCK_SIZE*12, 1, C_GIRL, LEFT, AI_WANDER, false);
	ses.npcs[1] = pk_tinit(BLOCK_SIZE*13, BLOCK_SIZE*12, BLOCK_SIZE*13, BLOCK_SIZE*11, 1, C_SCI, LEFT, AI_TURN, false);

	int moves[4];
	for(int i=0; i<4; i++) {
		moves[i] = PK_TACKLE;
	}
	pk_psetMonster(pk_initMonster(20, 2, &ses.bMons[PK_CHARIZARD], false, 
		ses.bMons[PK_NIDOQUEEN].bs, &moves[0]), &p1);
	pk_tsetMonster(pk_initMonster(25, 50, &ses.bMons[PK_NIDOQUEEN], false, 
		ses.bMons[PK_NIDOQUEEN].bs, &moves[0]), &ses.npcs[1]);
}

void setWindows()
{
	pk_initWindow((WIND_WIDTH)*CHAR_SIZE, 0,WIND_WIDTH, 16, false, &ses.w_menu);
	pk_setWindowText("^+POKEDEX^^+POKEMON^^+ITEM^^+ASH^^+SAVE^^+OPTION^^+EXIT|", false, &ses.w_menu);
	pk_setWOptionFunc(6, &quitGame, &ses.w_menu);
	
	pk_initWindow(0, SCREEN_HEIGHT-(6*CHAR_SIZE), WIND_WIDTH*2, 6, true, &ses.npcs[0].dialog);
	pk_setWindowText("^Hi I am a girl no^^joke dude|", true, &ses.npcs[0].dialog);
	pk_initWindow(0, SCREEN_HEIGHT-(6*CHAR_SIZE), WIND_WIDTH*2, 6, true, &ses.npcs[1].dialog);
	pk_setWindowText("^I am but a^^hapless nerd|", true, &ses.npcs[1].dialog);

	pk_initWindow(0, SCREEN_HEIGHT-(6*CHAR_SIZE), WIND_WIDTH*2, 6, false, &ses.w_bDialog);
	pk_setWindowText("^No data specified|", true, &ses.w_bDialog);
	pk_initWindow(SCREEN_WIDTH-(12*CHAR_SIZE), SCREEN_HEIGHT-(6*CHAR_SIZE), 12, 6, false, &ses.w_bMenu);
	pk_setWindowText("^+FIGHT+PK^^+ITEM +RUN|", false, &ses.w_bMenu);
}

void setClips()
{
	for(int i=0; i<20; i++) {
		clipTiles[i].x = 0;
		clipTiles[i].y = BLOCK_SIZE*i;
		clipTiles[i].w = clipTiles[i].h = BLOCK_SIZE;
	}
	for(int i=0; i<60; i++) {
		clipNPCs[i].x = 0;
		clipNPCs[i].y = BLOCK_SIZE*i;
		clipNPCs[i].w = clipNPCs[i].h = BLOCK_SIZE;
	}
	for(int i=0; i<70; i++) {
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
		offset.x = x - (p1.mover.x-(SCREEN_WIDTH/2))-16;
		offset.y = y - (p1.mover.y-(SCREEN_HEIGHT/2));
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

trainerNPC_t* isHere(int thing, int x, int y)
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
	col_t col = pk_findCols(ses.map, p1.mover.x/BLOCK_SIZE, p1.mover.y/BLOCK_SIZE);

	if(ses.mode == SES_OVERWORLD && !ses.windOpen) {
		if(keyStates[SDLK_DOWN]) {
			pk_moveChar(DOWN, col.down, &p1.mover);
		} else if(keyStates[SDLK_UP]) {
			pk_moveChar(UP, col.up, &p1.mover);
		} else if(keyStates[SDLK_LEFT]) {
			pk_moveChar(LEFT, col.left, &p1.mover);
		} else if(keyStates[SDLK_RIGHT]) {
			pk_moveChar(RIGHT, col.right, &p1.mover);
		}

		if(keyStates[SDLK_z]) {
			if(keyStatesBuf[SDLK_z] == false && p1.mover.x == p1.mover.nextX && p1.mover.y == p1.mover.nextY) {
				switch(p1.mover.dir) {
				trainerNPC_t *tNPC;
				case LEFT:
					tNPC = isHere(NPC, p1.mover.x-BLOCK_SIZE, p1.mover.y);
					if(tNPC != NULL) {
						pk_toggleWindow(&tNPC->dialog);
						tNPC->mover.dir = RIGHT;
						ses.currWindow = &tNPC->dialog;
					}
					break;
				case RIGHT:
					tNPC = isHere(NPC, p1.mover.x+BLOCK_SIZE, p1.mover.y);
					if( tNPC != NULL) {
						pk_toggleWindow(&tNPC->dialog);
						tNPC->mover.dir = LEFT;
						ses.currWindow = &tNPC->dialog;
					}
					break;
				case UP:
					tNPC = isHere(NPC, p1.mover.x, p1.mover.y-BLOCK_SIZE);
					if( tNPC != NULL) {
						pk_toggleWindow(&tNPC->dialog);
						tNPC->mover.dir = DOWN;
						ses.currWindow = &tNPC->dialog;
					}
					break;
				case DOWN:
					tNPC = isHere(NPC, p1.mover.x, p1.mover.y+BLOCK_SIZE);
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
			pk_sstartBattleW(ses.npcs[1].monsters[0], &ses);
			pk_sstepBattle(p1.monsters[0], &ses);
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
		if(keyStatesBuf[SDLK_m] == false && p1.mover.x == p1.mover.nextX && p1.mover.y == p1.mover.nextY) {
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
		}
	} else {
		keyStatesBuf[SDLK_p] = false;
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

void graphics()
{
	if(ses.mode == SES_OVERWORLD) {
		int mapX, mapY = 0;

		int grassLocs[64], grassLocsX[64], grassLocsY[64];
		int gInc = 0;
		for(int i=0; i<ses.map.width*ses.map.height; i++) {
			apply_surface(mapX, mapY, s_mapTile, s_screen, &clipTiles[ses.map.data[i]], true);
			if(ses.map.data[i] == GRASS) {
				grassLocs[gInc] = i;
				grassLocsX[gInc] = mapX;
				grassLocsY[gInc] = mapY;
				gInc++;
			}
			mapX+=BLOCK_SIZE;
			if((i+1) % ses.map.width == 0 && i != 0) {
				mapY+=BLOCK_SIZE;
				mapX= 0;
			}
		}

		for(int i=0; i<MAX_NPCS; i++) {
			if(ses.npcs[i].active)
				apply_surface(ses.npcs[i].mover.x, ses.npcs[i].mover.y-4, s_npc, s_screen, 
					&clipNPCs[pk_tgetFrame(ses.npcs[i])], true);
		}

		apply_surface(p1.mover.x, p1.mover.y-4, s_player, s_screen, &clipNPCs[pk_pgetFrame(p1)], true);

		for(int i=0; i<gInc; i++) {
			apply_surface(grassLocsX[i], grassLocsY[i], s_mapTile, s_screen, &clipTiles[GRASS_OVER], true);
		}
	} else if(ses.mode == SES_BATTLE) {
		drawWindow(ses.w_bDialog);
		if(ses.w_bMenu.active) {
			drawWindow(ses.w_bMenu);
		}
		apply_surface(0, SCREEN_HEIGHT-(PKMN_SIZE)-(6*CHAR_SIZE), s_bPkmn, s_screen,
			&clipPkmn[p1.monsters[0].id->backSpr], false);
		apply_surface(SCREEN_WIDTH-PKMN_SIZE, 0, s_fPkmn, s_screen,
			&clipPkmn[ses.npcs[1].monsters[0].id->backSpr], false);
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
		pk_sstepBattle(p1.monsters[0], &ses);
	}

	if(ses.battleStep == BATS_GO && ses.w_bDialog.finished) {
		pk_sstepBattle(p1.monsters[0], &ses);
	}

	pk_supdateMapCols(ALL, &ses);
	pk_buildColMapC(p1.mover, &ses.map);

	pk_supdate(&ses);
	pk_supdateNpcs(&ses);

	pk_updateChar(&p1.mover);
		/*if(ses.windOpen && !ses.npcs[i].pause) {
			pk_ttogglePause(&ses.npcs[i]);
		} else if(!ses.windOpen && ses.npcs[i].pause) {
			pk_ttogglePause(&ses.npcs[i]);
		}*/

	/*if(pk_isWindowFinished(ses.w_bDialog) && !ses.w_bMenu.active) {
		pk_toggleWindow(&ses.w_bMenu);
		pk_clearWindow(&ses.w_bDialog);
		pk_setWindowText("|", true, &ses.w_bDialog);
		ses.currWindow = &ses.w_bMenu;
	}*/
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
	setMonsters();
	setNpcs();
	setWindows();
	pk_snewMap("resources/maps/test.map", &ses);

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

		SDL_FillRect( s_screen, &s_screen->clip_rect, SDL_MapRGB( s_screen->format, 0xFF, 0xFF, 0xFF ) );

		gameLoop();

		SDL_Flip(s_screen);
	}

	return 0;
}
