#include "monsters.h"

move_t pk_initMove(int s_cpp, int s_bpp, char* name, int type, bool (*s_movePtr) (monster_t*, monster_t*)) {
	move_t out;
	out.bpp = s_bpp;
	out.movePtr = s_movePtr;
	out.type = type;

	for(int i=0; i<12; i++) {
		out.name[i] = name[i];
	}

	return out;
}

void pk_setMove(int moveVal, int cpp, int bpp, int moveSlot, monster_t* mon) {
	mon->moves[moveSlot].value = moveVal;
	mon->moves[moveSlot].cpp = cpp;
	mon->moves[moveSlot].bpp = bpp;
}

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

int pk_damage(int amount, monster_t* a, monster_t* d) {
	int damage = (amount)/5;
	if(damage <= 0) {
		damage = 1;
	}
	d->stats.mHp -= damage;

	if(d->stats.mHp < 0) {
		d->stats.mHp = 0;
	}

	return damage;
}

bool pk_useMove(int numb, monster_t* mon) {
	if(mon->moves[numb].cpp > 0) {
		mon->moves[numb].cpp--;
		return true;
	} else {
		return false;
	}
}

bool pk_m_nomove(monster_t* a, monster_t* d) {
	printf("--- Executed!\n   Hit:   NO\n   Attacker HP: %i\n   Defender HP: %i\n", a->stats.mHp, d->stats.mHp);
	return false;
}

bool pk_m_tackle(monster_t* a, monster_t* d) {
	int willHit = rand() % 100;
	if(willHit >= a->stats.mAcc-0) {
		pk_damage(40, a, d);
		printf("TACKLE Executed!\n   Hit:  YES\n   Attacker HP: %i\n   Defender HP: %i\n", a->stats.mHp, d->stats.mHp);
		return true;
	}
	
	printf("TACKLE Executed!\n   Hit:   NO\n   Attacker HP: %i\n   Defender HP: %i\n", a->stats.mHp, d->stats.mHp);
	return false;
}

bool pk_m_explosion(monster_t* a, monster_t* d) {
	int willHit = rand() % 100;
	if(willHit >= a->stats.mAcc-0) {
		pk_damage(180, a, d);
		a->stats.mHp = 0;
		printf("EXPLOSION Executed!\n   Hit:  YES\n   Attacker HP: %i\n   Defender HP: %i\n", a->stats.mHp, d->stats.mHp);
		return true;
	}

	printf("EXPLOSION Executed!\n   Hit:   NO\n   Attacker HP: %i\n   Defender HP: %i\n", a->stats.mHp, d->stats.mHp);
	return false;
}