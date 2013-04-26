#include "sessionman.h"

void pk_initSMan(int s_mode, sessionMan_t* ses) {
	ses->mode = s_mode;
	ses->windOpen = false;

	for(int i=0; i<MAX_NPCS; i++) {
		ses->npcs[i].active = false;
	}
}

void pk_switchMode(int s_mode, sessionMan_t* session) {
	session->mode = s_mode;

	if(session->mode == SES_BATTLE) {
		pk_toggleWindow(&session->w_bDialog);
		if(session->w_bMenu.active) {
			pk_toggleWindow(&session->w_bMenu);
		}
	} else if(session-> mode == SES_OVERWORLD) {
		pk_toggleWindow(&session->w_bDialog);
		if(session->w_bMenu.active) {
			pk_toggleWindow(&session->w_bMenu);
		}
	}
}

void pk_snewMap(char* filename, sessionMan_t* ses) {
	pk_loadMap(filename, &ses->map);
}

void pk_supdateMapCols(int which, sessionMan_t* ses) {
	pk_clearColMap(&ses->map);

	if(which == ENVIRON || which == ALL) {
		pk_buildColMapM(&ses->map);
	}
	if(which == NPC || which == ALL) {
		for(int i=0; i<MAX_NPCS; i++) {
			if(ses->npcs[i].active) {
				pk_buildColMapC(ses->npcs[i].mover, &ses->map);
			}
		}
	}
}

void pk_sstartBattleW(monster_t mon, sessionMan_t* ses) {
	ses->battleStep = 0;
	ses->battleType = BAT_WILD;
	ses->attWild = mon;

	ses->mode = SES_BATTLE;

	ses->currWindow = &ses->w_bDialog;
	ses->w_bDialog.active = true;
}

void pk_sstartBattleT(trainerNPC_t* trainer, sessionMan_t* ses) {
	ses->battleStep = BATS_PRE;
	ses->battleType = BAT_TRAINER;
	ses->attTrainer = trainer;

	ses->mode = SES_BATTLE;

	ses->currWindow = &ses->w_bDialog;
	ses->w_bDialog.active = true;
}

void pk_sstepBattle(monster_t pMon, sessionMan_t* ses) {
	if(ses->battleStep == BATS_PRE) {
		ses->battleStep = BATS_WPA;
		if(ses->battleType == BAT_WILD) {
			pk_clearWindow(&ses->w_bDialog);
			char message[30] =  "^Wild             ^^appeared!|";
			for(int i=0; i<12; i++) {
				message[6+i] = ses->attWild.name[i];
			}
			pk_setWindowText(message, true, &ses->w_bDialog);
		}
	} else if(ses->battleStep == BATS_WPA) {
		ses->battleStep = BATS_GO;
		char message[30] =  "^Go!^^             |";
		for(int i=0; i<12; i++) {
			message[6+i] = pMon.name[i];
			if(pMon.name[i] == ' ') {
				printf("!\n");
			}
		}
		pk_setWindowText(message, true, &ses->w_bDialog);
	} else if(ses->battleStep == BATS_GO) {
		pk_clearWindow(&ses->w_bDialog);
		ses->battleStep = BATS_SEL;
		ses->currWindow = &ses->w_bMenu;
		pk_toggleWindow(&ses->w_bMenu);
	}
}

void pk_supdate(sessionMan_t* session) {
	pk_updateWindow(&session->w_menu);
	pk_updateWindow(&session->w_bDialog);
	pk_updateWindow(&session->w_bMenu);
}

void pk_supdateNpcs(sessionMan_t* ses) {
	for(int i=0; i<MAX_NPCS; i++) {
		if(ses->npcs[i].active) {
			pk_tupdate(&ses->npcs[i],
				pk_findCols(ses->map, ses->npcs[i].mover.x/BLOCK_SIZE, ses->npcs[i].mover.y/BLOCK_SIZE));
			pk_updateWindow(&ses->npcs[i].dialog);
		}
	}
}