#include "monsters.h"

stats_t pk_initStats(int att, int def, int spAtt, int spDef, int speed) {
	stats_t out;
	out.att = att;
	out.def = def;
	out.spAtt = spAtt;
	out.spDef = spDef;
	out.speed = speed;

	return out;
}

type_t pk_initType(char* name, int value) {
	type_t out;

	for(int i=0; i<8; i++) {
		out.name[i] = name[i];
	}

	out.value = value;

	return out;
}

move_t pk_initMove(int s_cpp, int s_bpp, char* name, int type, bool (*s_movePtr) (monster_t*, monster_t*)) {
	move_t out;
	out.cpp = s_cpp;
	out.bpp = s_bpp;
	out.movePtr = s_movePtr;
	out.type = type;

	for(int i=0; i<12; i++) {
		out.name[i] = name[i];
	}

	return out;
}

monster_t pk_initMonster(int s_health, int s_experience, baseMonster_t* s_id, bool s_shiny,
	stats_t s_stats, int* s_moves) {
	monster_t out;
	out.health = s_health;
	out.experience = s_experience;
	out.id = s_id;
	out.shiny = s_shiny;
	out.stats = s_stats;
	for(int i=0; i<4; i++) {
		out.moves[i] = s_moves[i];
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

void pk_damage(int amount, monster_t* victim) {
	victim->health = amount - victim->stats.def;
}

int pk_calcTyping(type_t mType1, type_t mType2, type_t attType) {
	int effect = 0;

	if(mType1.value != T_NONE) {
		for(int i=0; i<mType1.resCnt; i++) {
			if(mType1.res[i] == attType.value) {
				effect--;
				break;
			}
			if(mType1.weak[i] == attType.value) {
				effect++;
				break;
			}
			if(mType1.inv[i] == attType.value) {
				return RES_NONE;
			}
		}
	}

	if(mType2.value != T_NONE) {
		for(int i=0; i<mType2.resCnt; i++) {
			if(mType2.res[i] == attType.value) {
				effect--;
				break;
			}
			if(mType2.weak[i] == attType.value) {
				effect++;
				break;
			}
			if(mType2.inv[i] == attType.value) {
				return RES_NONE;
			}
		}
	}

	switch(effect) {
	case 2:
		return RES_HYPER;
	case 1:
		return RES_SUPER;
	case 0:
		return RES_NORMAL;
	case -1:
		return RES_NOTVERY;
	case -2:
		return RES_HARDLY;
	}
}

void pk_setTypeRes(type_t* type, int cnt, int* types) {
	type->res = (int*)malloc(cnt*sizeof(int));
	for(int i=0; i<cnt; i++) {
		type->res[i] = types[i];
	}

	type->resCnt = cnt;
}

void pk_setTypeWeak(type_t* type, int cnt, int* types) {
	type->weak = (int*)malloc(cnt*sizeof(int));
	for(int i=0; i<cnt; i++) {
		type->weak[i] = types[i];
	}

	type->weakCnt = cnt;
}

void pk_setTypeInv(type_t* type, int cnt, int* types) {
	type->inv = (int*)malloc(cnt*sizeof(int));
	for(int i=0; i<cnt; i++) {
		type->inv[i] = types[i];
	}

	type->invCnt = cnt;
}

bool pk_m_nomove(monster_t* a, monster_t* d) {
	return false;
}

bool pk_m_tackle(monster_t* a, monster_t* d) {
	int willHit = rand() % 100;
	if(willHit >= a->stats.mAcc-0) {
		pk_damage(40, d);
		return true;
	}
	
	return false;
}

bool pk_m_explosion(monster_t* a, monster_t* d) {
	int willHit = rand() % 100;
	if(willHit >= a->stats.mAcc-0) {
		pk_damage(180, d);
		pk_damage(999, a);
		return true;
	}

	return false;
}