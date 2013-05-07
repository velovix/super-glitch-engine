#include "sessionman.h"

void pk_initSMan(int s_mode, sessionMan_t* ses) {
	ses->mode = s_mode;
	ses->windOpen = false;

	for(int i=0; i<MAX_NPCS; i++) {
		ses->npcs[i].active = false;
	}

	pk_initWindow((WIND_WIDTH)*CHAR_SIZE, 0,WIND_WIDTH, 16, false, true, &ses->w_menu);
	pk_initWindow(0, SCREEN_HEIGHT-(6*CHAR_SIZE), WIND_WIDTH*2, 6, false, false, &ses->w_bDialog);
	pk_initWindow(SCREEN_WIDTH-(12*CHAR_SIZE), SCREEN_HEIGHT-(6*CHAR_SIZE), 12, 6, false, false, &ses->w_bMenu);
	pk_initWindow(SCREEN_WIDTH-(15*CHAR_SIZE), SCREEN_HEIGHT-(6*CHAR_SIZE), 15, 6, true, false, &ses->w_bMoves);
	pk_initWindow(0, SCREEN_HEIGHT-(10*CHAR_SIZE), 11, 5, false, false, &ses->w_bMoveInfo);

	pk_setWindowText("^+POKEDEX^^+POKEMON^^+ITEM^^+ASH^^+SAVE^^+OPTION^^+EXIT|", false, &ses->w_menu);
	pk_setWindowText("^No data specified|", true, &ses->w_bDialog);
	pk_setWindowText("^+FIGHT+PK^^+ITEM +RUN|", false, &ses->w_bMenu);
	pk_setWindowText("+NULL^+NULL^+NULL^+NULL|", false, &ses->w_bMoves);
	pk_setWindowText("TYPE/^ FIGHTING^    QQ/QQ|", false, &ses->w_bMoveInfo);
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

	pk_ssetMoveWind(ses->p1.monsters[0], ses);
}

void pk_sstartBattleT(trainerNPC_t* trainer, sessionMan_t* ses) {
	ses->battleStep = BATS_PRE;
	ses->battleType = BAT_TRAINER;
	ses->attTrainer = trainer;

	ses->mode = SES_BATTLE;

	ses->currWindow = &ses->w_bDialog;
	ses->w_bDialog.active = true;

	pk_ssetMoveWind(ses->p1.monsters[0], ses);
}

void pk_sstepBattle(monster_t pMon, sessionMan_t* ses) {
	if(ses->battleStep == BATS_PRE) {
		ses->battleStep = BATS_WPA;
		if(ses->battleType == BAT_WILD) {
			pk_clearWindow(&ses->w_bDialog);
			pk_setInsWindowText("^Wild             ^^appeared!|", ses->attWild.name, 6, 12, true, &ses->w_bDialog);
		}
	} else if(ses->battleStep == BATS_WPA) {
		pk_clearWindow(&ses->w_bDialog);
		ses->battleStep = BATS_GO;
		pk_setInsWindowText("^Go!              |", pMon.name, 5, 12, true, &ses->w_bDialog);
	} else if(ses->battleStep == BATS_GO) {
		pk_clearWindow(&ses->w_bDialog);
		ses->battleStep = BATS_SEL;
		ses->currWindow = &ses->w_bMenu;
		pk_toggleWindow(&ses->w_bMenu);
	}
}

void pk_ssetMoveWind(monster_t mon, sessionMan_t* ses) {
	char moveTxt[(12*4)+(2*4)+1];

	int inc = 0;
	for(int i=0; i<4; i++) {
		moveTxt[inc] = '+';	inc++;
		for(int j=0; j<12; j++) {
			moveTxt[inc] = ses->moves[ mon.moves[i].value ].name[j];
			inc++;
		}
		if(i!=3) {
			moveTxt[inc] = '^';
			inc++;
		}
	}
	moveTxt[inc] = '|';

	pk_setWindowText(moveTxt, false, &ses->w_bMoves);
}

void pk_ssetMoveInfoWind(move_t move, sessionMan_t* ses) {
	char* miTxt = ses->w_bMoveInfo.text;

	for(int i=0; i<8; i++) {
		miTxt[i+7] = ses->types[move.type].name[i];
	}

	char pp1[2];		char pp2[2];
	sprintf(pp1, "%d", ses->p1.monsters[0].moves[ses->w_bMoves.selOpt].cpp);
	sprintf(pp2, "%d", ses->p1.monsters[0].moves[ses->w_bMoves.selOpt].bpp);
	miTxt[20] = pp1[0];	miTxt[21] = pp1[1];
	miTxt[23] = pp2[0];	miTxt[24] = pp2[1];

	pk_setWindowText(miTxt, false, &ses->w_bMoveInfo);
}

void pk_supdateWindows(sessionMan_t* ses) {

	pk_ssetMoveInfoWind(ses->moves[ses->p1.monsters[0].moves[ses->w_bMoves.selOpt].value], ses);

	pk_updateWindow(&ses->w_menu);
	pk_updateWindow(&ses->w_bDialog);
	pk_updateWindow(&ses->w_bMenu);
	pk_updateWindow(&ses->w_bMoves);
	pk_updateWindow(&ses->w_bMoveInfo);

	for(int i=0; i<MAX_NPCS; i++) {
		pk_updateWindow(&ses->npcs[i].dialog);
	}

	if(ses->w_bMoves.active && !ses->w_bMoveInfo.active) {
		pk_toggleWindow(&ses->w_bMoveInfo);
	} else if(!ses->w_bMoves.active && ses->w_bMoveInfo.active) {
		pk_toggleWindow(&ses->w_bMoveInfo);
	}

	if(ses->mode == SES_BATTLE && ses->w_bMenu.active && ses->w_bMenu.selection != WSEL_NONE) {
		if(ses->w_bMenu.selection == 0) {
			if(!ses->w_bMoves.active)
				pk_toggleWindow(&ses->w_bMoves);
			ses->currWindow = &ses->w_bMoves;
		}

		ses->w_bMenu.selection = WSEL_NONE;
	}
	if(ses->mode == SES_BATTLE && ses->w_bMoves.selection != WSEL_NONE) {
		if(ses->w_bMoves.selection == WSEL_BACK) {
			ses->currWindow = &ses->w_bMenu;
		} else if(ses->w_bMoves.active) {
			if(pk_useMove(ses->w_bMoves.selection, &ses->p1.monsters[0])) {
				ses->moves[ses->p1.monsters[0].moves[ses->w_bMoves.selection].value].movePtr(&ses->p1.monsters[0], &ses->attWild);
			}

		}

		ses->w_bMoves.selection = WSEL_NONE;
	}
}

void pk_supdateNpcs(sessionMan_t* ses) {
	if(ses->currWindow != NULL) {
		return;
	}

	for(int i=0; i<MAX_NPCS; i++) {
		if(ses->npcs[i].active) {
			pk_tupdate(&ses->npcs[i],
			pk_findCols(ses->map, ses->npcs[i].mover.x/BLOCK_SIZE, ses->npcs[i].mover.y/BLOCK_SIZE));
		}
	}
}