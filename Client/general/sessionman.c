#include "sessionman.h"

void pk_initSMan(int s_mode, sessionMan_t* ses) {
	printf("Started SGE session.\n");
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

	pk_setWindowText("{^+POKEDEX^^+POKEMON^^+ITEM^^+ASH^^+SAVE^^+OPTION^^+EXIT|", false, &ses->w_menu);
	pk_setWindowText("{^No data specified|", true, &ses->w_bDialog);
	pk_setWindowText("{^+FIGHT+PK^^+ITEM +RUN|", false, &ses->w_bMenu);
	pk_setWindowText("{+NULL^+NULL^+NULL^+NULL|", false, &ses->w_bMoves);
	pk_setWindowText("{TYPE/^ FIGHTING^    QQ/QQ|", false, &ses->w_bMoveInfo);
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
	ses->battleStep = BATS_PRE;
	ses->battleType = BAT_WILD;
	ses->attWild = mon;

	ses->mode = SES_BATTLE;

	ses->currWindow = &ses->w_bDialog;
	ses->w_bDialog.active = true;

	pk_ssetMoveWind(ses->p1.monsters[0], ses);
}

void pk_sstartBattleT(npc_t* trainer, sessionMan_t* ses) {
	ses->battleStep = BATS_PRE;
	ses->battleType = BAT_TRAINER;
	ses->attTrainer = trainer;

	ses->mode = SES_BATTLE;

	ses->currWindow = &ses->w_bDialog;
	ses->w_bDialog.active = true;

	pk_ssetMoveWind(ses->p1.monsters[0], ses);
}

void pk_sstepBattle(sessionMan_t* ses, int step, monster_t aMon, monster_t dMon, int move) {
	switch(step) {
	case BATS_PRE:
		break;
	case BATS_WPA:
		if(ses->battleType == BAT_WILD) {
			pk_clearWindow(&ses->w_bDialog);
			pk_setInsWindowText("{^Wild             ^^appeared!|", aMon.name, 7, 12, true, &ses->w_bDialog);
		}
		break;
	case BATS_GO:
		pk_clearWindow(&ses->w_bDialog);
		pk_setInsWindowText("{^Go!              |", dMon.name, 6, 12, true, &ses->w_bDialog);
		break;
	case BATS_SEL:
		pk_clearWindow(&ses->w_bDialog);
		ses->currWindow = &ses->w_bMenu;
		pk_toggleWindow(&ses->w_bMenu);
		break;
	case BATS_ATT:
		pk_setInsWindowText("{^             ^^used             |", aMon.name,
			2, 12, true, &ses->w_bDialog);
		pk_setInsWindowText(ses->w_bDialog.text,
			ses->moves[aMon.moves[move].value].name,
			22, 12, true, &ses->w_bDialog);
		pk_toggleWindow(&ses->w_bMoves);
		pk_toggleWindow(&ses->w_bMenu);
		ses->currWindow = &ses->w_bDialog;
		break;
	case BATS_RESULT:
		switch(pk_calcTyping(ses->types[dMon.type1], ses->types[dMon.type2], ses->types[ses->moves[move].type])) {
		case RES_HYPER:
			pk_setWindowText("{^Major damage!|", true, &ses->w_bDialog);
			break;
		case RES_SUPER:
			pk_setWindowText("{^It is super^^effective!|", true, &ses->w_bDialog);
			break;
		case RES_NORMAL:
			pk_setWindowText("{^A decent hit!|", true, &ses->w_bDialog);
			break;
		case RES_NOTVERY:
			pk_setWindowText("{^It is not very^^effective...|", true, &ses->w_bDialog);
			break;
		case RES_HARDLY:
			pk_setWindowText("{^It did almost^^nothing...|", true, &ses->w_bDialog);
			break;
		case RES_NONE:
			pk_setWindowText("{^But it failed!|", true, &ses->w_bDialog);
			break;
		}
		ses->currWindow = &ses->w_bDialog;
	}

	ses->battleStep = step;
}

void pk_ssetMoveWind(monster_t mon, sessionMan_t* ses) {
	char moveTxt[(12*4)+(2*4)+2];

	moveTxt[0] = '{';
	int inc = 1;
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
		miTxt[i+8] = ses->types[move.type].name[i];
	}

	char pp1[2];		char pp2[2];
	sprintf(pp1, "%d", ses->p1.monsters[0].moves[ses->w_bMoves.selOpt].cpp);
	sprintf(pp2, "%d", ses->p1.monsters[0].moves[ses->w_bMoves.selOpt].bpp);
	miTxt[21] = pp1[0];	miTxt[22] = pp1[1];
	miTxt[24] = pp2[0];	miTxt[25] = pp2[1];

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
			if(pk_useMove(ses->w_bMoves.selection, &ses->p1.monsters[ses->p1.currMon])) {
				pk_sstepBattle(ses, BATS_ATT, ses->p1.monsters[ses->p1.currMon]
					, ses->attWild, ses->w_bMoves.selection);
				/*ses->moves[ses->p1.monsters[ses->p1.currMon].\
					moves[ses->w_bMoves.selection].value].\
					movePtr(&ses->p1.monsters[ses->p1.currMon], &ses->attWild);*/
			}

		}

		ses->w_bMoves.selection = WSEL_NONE;
	}
	if(ses->mode == SES_BATTLE && ses->w_bDialog.finished) {
		if(ses->battleStep == BATS_ATT) {
			pk_sstepBattle(ses, BATS_RESULT, ses->p1.monsters[ses->p1.currMon],
				ses->attWild, ses->w_bMoves.selection);
		}
	}
}

void pk_supdateNpcs(sessionMan_t* ses) {
	if(ses->currWindow != NULL) {
		return;
	}

	for(int i=0; i<MAX_NPCS; i++) {

		if(ses->npcs[i].active) {

			pk_updateNpc(&ses->npcs[i], pk_findCols(ses->map,ses->npcs[i].mover.x/BLOCK_SIZE, ses->npcs[i].mover.y/BLOCK_SIZE));
			if(!ses->npcs[i].fought && !ses->npcs[i].aggro && pk_canNpcSee(ses->p1.mover.nextX, ses->p1.mover.nextY, &ses->npcs[i])) {
				if(pk_isFinishedMoving(ses->p1.mover)) {
					pk_aggroNpc(ses->p1.mover.x, ses->p1.mover.y, &ses->npcs[i]);
				} else {
					ses->p1.pause = true;
				}
			} else if(ses->npcs[i].aggro && ses->npcs[i].mover.x == ses->npcs[i].destX[ses->npcs[i].dest]
				&& ses->npcs[i].mover.y == ses->npcs[i].destY[ses->npcs[i].dest] && !ses->npcs[i].dialog.finished) {
				pk_toggleWindow(&ses->npcs[i].dialog);
				ses->currWindow = &ses->npcs[i].dialog;
				ses->p1.mover.dir = pk_faceChar(ses->p1.mover, ses->npcs[i].mover);
			} else if(ses->npcs[i].aggro && ses->npcs[i].dialog.finished) {
				ses->p1.pause = false;
				ses->npcs[i].aggro = false;
				pk_sstartBattleW(ses->npcs[i].monsters[0], ses);
				pk_sstepBattle(ses, BATS_WPA, ses->npcs[i].monsters[0], ses->attWild, 0);
			}
		}
	}
}

void pk_spruneNpcs(sessionMan_t* ses) {
	for(int i=0; i<MAX_NPCS; i++) {
		bool isUsed = false;
		for(int j=0; j<ses->map.npcCnt; j++) {
			if(ses->map.npcData[j].val == i) {
				isUsed = true;
				ses->npcs[i].mover.x = ses->npcs[i].mover.nextX = ses->map.npcData[j].x*BLOCK_SIZE;
				ses->npcs[i].mover.y = ses->npcs[i].mover.nextY = ses->map.npcData[j].y*BLOCK_SIZE;

				ses->npcs[i].destX[1]-=ses->npcs[i].destX[0];
				ses->npcs[i].destY[1]-=ses->npcs[i].destY[0];
				ses->npcs[i].destX[0] = ses->map.npcData[j].x*BLOCK_SIZE;
				ses->npcs[i].destY[0] = ses->map.npcData[j].y*BLOCK_SIZE;
				ses->npcs[i].destX[1]+=ses->npcs[i].mover.x;
				ses->npcs[i].destY[1]+=ses->npcs[i].mover.y;
			}
		}

		ses->npcs[i].active = isUsed;
	}
}