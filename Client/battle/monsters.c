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

type_t pk_initType(char* name) {
	type_t out;

	for(int i=0; i<8; i++) {
		out.name[i] = name[i];
	}

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

int pk_calcStats(monster_t mon, int type) {
	int effect = 0;
	if(type == T_FIRE) {
		if(mon.type1 == T_FIRE || mon.type2 == T_FIRE) {
			effect--;
		}
		if(mon.type1 == T_WATER || mon.type2 == T_WATER) {
			effect--;
		}
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect--;
		}
		if(mon.type1 == T_DRAGON || mon.type2 == T_DRAGON) {
			effect--;
		}

		if(mon.type1 == T_BUG || mon.type2 == T_BUG) {
			effect++;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect++;
		}
		if(mon.type1 == T_ICE || mon.type2 == T_ICE) {
			effect++;
		}
	} else if(type == T_WATER) {
		if(mon.type1 == T_WATER || mon.type2 == T_WATER) {
			effect--;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect--;
		}
		if(mon.type1 == T_DRAGON || mon.type2 == T_DRAGON) {
			effect--;
		}

		if(mon.type1 == T_FIRE || mon.type2 == T_FIRE) {
			effect++;
		}
		if(mon.type1 == T_GROUND || mon.type2 == T_GROUND) {
			effect++;
		}
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect++;
		}
	} else if(type == T_GROUND) {
		if(mon.type1 == T_BUG || mon.type2 == T_BUG) {
			effect--;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect--;
		}

		if(mon.type1 == T_FIRE || mon.type2 == T_FIRE) {
			effect++;
		}
		if(mon.type1 == T_POISON || mon.type2 == T_POISON) {
			effect++;
		}
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect++;
		}
		if(mon.type1 == T_ELECTRIC || mon.type2 == T_ELECTRIC) {
			effect++;
		}

		if(mon.type1 == T_FLYING || mon.type2 == T_FLYING) {
			return RES_NONE;
		}
	} else if(type == T_NORMAL) {
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect--;
		}

		if(mon.type1 == T_GHOST || mon.type2 == T_GHOST) {
			return RES_NONE;
		}
	} else if(type == T_FIGHTING) {
		if(mon.type1 == T_POISON || mon.type2 == T_POISON) {
			effect--;
		}
		if(mon.type1 == T_FLYING || mon.type2 == T_FLYING) {
			effect--;
		}
		if(mon.type1 == T_BUG || mon.type2 == T_BUG) {
			effect--;
		}
		if(mon.type1 == T_PSYCHIC || mon.type2 == T_PSYCHIC) {
			effect--;
		}

		if(mon.type1 == T_NORMAL || mon.type2 == T_NORMAL) {
			effect++;
		}
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect++;
		}
		if(mon.type1 == T_ICE || mon.type2 == T_ICE) {
			effect++;
		}
	} else if(type == T_FLYING) {
		if(mon.type1 == T_ELECTRIC || mon.type2 == T_ELECTRIC) {
			effect--;
		}
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect--;
		}

		if(mon.type1 == T_BUG || mon.type2 == T_BUG) {
			effect++;
		}
		if(mon.type1 == T_FIGHTING || mon.type2 == T_FIGHTING) {
			effect++;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect++;
		}
	} else if(type == T_GRASS) {
		if(mon.type1 == T_BUG || mon.type2 == T_BUG) {
			effect--;
		}
		if(mon.type1 == T_DRAGON || mon.type2 == T_DRAGON) {
			effect--;
		}
		if(mon.type1 == T_FIRE || mon.type2 == T_FIRE) {
			effect--;
		}
		if(mon.type1 == T_FLYING || mon.type2 == T_FLYING) {
			effect--;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect--;
		}
		if(mon.type1 == T_POISON || mon.type2 == T_POISON) {
			effect--;
		}

		if(mon.type1 == T_GROUND || mon.type2 == T_GROUND) {
			effect++;
		}
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect++;
		}
		if(mon.type1 == T_WATER || mon.type2 == T_WATER) {
			effect++;
		}
	} else if(type == T_POISON) {
		if(mon.type1 == T_GHOST || mon.type2 == T_GHOST) {
			effect--;
		}
		if(mon.type1 == T_GROUND || mon.type2 == T_GROUND) {
			effect--;
		}
		if(mon.type1 == T_POISON || mon.type2 == T_POISON) {
			effect--;
		}
		if(mon.type1 == T_ROCK || mon.type2 == T_ROCK) {
			effect--;
		}

		if(mon.type1 == T_BUG || mon.type2 == T_BUG) {
			effect++;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect++;
		}
	} else if(type == T_ELECTRIC) {
		if(mon.type1 == T_DRAGON || mon.type2 == T_DRAGON) {
			effect--;
		}
		if(mon.type1 == T_ELECTRIC || mon.type2 == T_ELECTRIC) {
			effect--;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect--;
		}

		if(mon.type1 == T_FLYING || mon.type2 == T_FLYING) {
			effect++;
		}
		if(mon.type1 == T_WATER || mon.type2 == T_WATER) {
			effect++;
		}
	} else if(type == T_PSYCHIC) {
		if(mon.type1 == T_PSYCHIC || mon.type2 == T_PSYCHIC) {
			effect--;
		}

		if(mon.type1 == T_FIGHTING || mon.type2 == T_FIGHTING) {
			effect++;
		}
		if(mon.type1 == T_POISON || mon.type2 == T_POISON) {
			effect++;
		}
	} else if(type == T_ROCK) {
		if(mon.type1 == T_FIGHTING || mon.type2 == T_FIGHTING) {
			effect--;
		}
		if(mon.type1 == T_GROUND || mon.type2 == T_GROUND) {
			effect--;
		}

		if(mon.type1 == T_BUG || mon.type2 == T_BUG) {
			effect++;
		}
		if(mon.type1 == T_FIRE || mon.type2 == T_FIRE) {
			effect++;
		}
		if(mon.type1 == T_FLYING || mon.type2 == T_FLYING) {
			effect++;
		}
		if(mon.type1 == T_ICE || mon.type2 == T_ICE) {
			effect++;
		}
	} else if(type == T_ICE) {
		if(mon.type1 == T_ICE || mon.type2 == T_ICE) {
			effect--;
		}
		if(mon.type1 == T_WATER || mon.type2 == T_WATER) {
			effect--;
		}

		if(mon.type1 == T_DRAGON || mon.type2 == T_DRAGON) {
			effect++;
		}
		if(mon.type1 == T_FLYING || mon.type2 == T_FLYING) {
			effect++;
		}
		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect++;
		}
		if(mon.type1 == T_GROUND || mon.type2 == T_GROUND) {
			effect++;
		}
	} else if(type == T_BUG) {
		if(mon.type1 == T_FIGHTING || mon.type2 == T_FIGHTING) {
			effect--;
		}
		if(mon.type1 == T_FIRE || mon.type2 == T_FIRE) {
			effect--;
		}
		if(mon.type1 == T_FLYING || mon.type2 == T_FLYING) {
			effect--;
		}
		if(mon.type1 == T_GHOST || mon.type2 == T_GHOST) {
			effect--;
		}

		if(mon.type1 == T_GRASS || mon.type2 == T_GRASS) {
			effect++;
		}
		if(mon.type1 == T_POISON || mon.type2 == T_POISON) {
			effect++;
		}
		if(mon.type1 == T_PSYCHIC || mon.type2 == T_PSYCHIC) {
			effect++;
		}
	} else if(type == T_DRAGON) {
		if(mon.type1 == T_DRAGON || mon.type2 == T_DRAGON) {
			effect++;
		}
	} else if(type == T_GHOST) {
		if(mon.type1 == T_GHOST || mon.type2 == T_GHOST) {
			effect++;
		}

		if(mon.type1 == T_NORMAL || mon.type2 == T_NORMAL) {
			return RES_NONE;
		}
		if(mon.type1 == T_PSYCHIC || mon.type2 == T_PSYCHIC) {
			return RES_NONE;
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