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
		out.name[i] = s_id->basic.name[i];
	}

	return out;
}

baseMonster_t pk_initBaseMonster(monFileObj_t obj) {
	baseMonster_t out;

	out = (baseMonster_t)obj;

	return out;
}

stats_t pk_baseMonsterGetStats(bool ev, baseMonster_t *obj) {
	stats_t out;

	if(ev) {
		out.hp  = obj->stats.ev_hp;
		out.att = obj->stats.ev_att;
		out.def = obj->stats.ev_def;
		out.spAtt = obj->stats.ev_spAtt;
		out.spDef = obj->stats.ev_spDef;
		out.speed = obj->stats.ev_speed;
	} else {
		out.hp  = obj->stats.b_hp;
		out.att = obj->stats.b_att;
		out.def = obj->stats.b_def;
		out.spAtt = obj->stats.b_spAtt;
		out.spDef = obj->stats.b_spDef;
		out.speed = obj->stats.b_speed;
	}

	return out;
}

void pk_doMoveEvent(moveEvent_t event, monster_t* a, monster_t* d) {
	switch(event.type) {
	case ME_DAMAGE:
		switch(event.target) {
		case ME_USER:
			a->stats.mHp -= event.value/5;
			if(a->stats.mHp < 0) {
				a->stats.mHp = 0;
			}
			break;
		case ME_TARGET:
			d->stats.mHp -= event.value/5;
			if(d->stats.mHp < 0) {
				d->stats.mHp = 0;
			}
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
