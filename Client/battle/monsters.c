#include "monsters.h"

monster_t pk_initMonster(int s_health, int s_experience, baseMonster_t* s_id, bool s_shiny, stats_t s_stats) {
	monster_t out;
	out.stats.mHp = s_health;
	out.experience = s_experience;
	out.id = s_id;
	out.shiny = s_shiny;
	out.stats = s_stats;
	out.type1 = out.type2 = 0;
	for(int i=0; i<4; i++) {
		out.moves[i].value = 0;
		out.moves[i].cpp = out.moves[i].bpp = 0;
	}

	for(int i=0; i<12; i++) {
		out.name[i] = s_id->name[i];
	}

	return out;
}

baseMonster_t pk_initBaseMonster(stats_t baseStats, stats_t baseEVs, int gID, char* name) {
	baseMonster_t out;
	out.bs = baseStats;
	out.ev = baseEVs;
	out.frntSpr = out.backSpr = out.iconSpr = gID;
	for(int i=0; i<12; i++) {
		out.name[i] = name[i];
	}

	return out;
}

void pk_doMoveEvent(moveEvent_t event, monster_t* a, monster_t* d) {
	switch(event.type) {
	case ME_DAMAGE:
		switch(event.target) {
		case ME_USER:
			a->stats.hp -= event.value;
			break;
		case ME_TARGET:
			d->stats.hp -= event.value;
			break;
		}
		break;
	case ME_POISON:
		switch(event.target) {
		case ME_USER:
			a->condition = ME_POISON;
			break;
		case ME_TARGET:
			d->condition = ME_POISON;
			break;
		}
		break;
	}
}

void pk_setMove(int moveVal, int cpp, int bpp, int moveSlot, monster_t* mon) {
	mon->moves[moveSlot].value = moveVal;
	mon->moves[moveSlot].cpp = cpp;
	mon->moves[moveSlot].bpp = bpp;
}

bool pk_useMove(int numb, monster_t* mon) {
	if(mon->moves[numb].cpp > 0) {
		mon->moves[numb].cpp--;
		return true;
	} else {
		return false;
	}
}
