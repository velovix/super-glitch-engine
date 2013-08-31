#include "stats.h"

stats_t pk_initStats(int hp, int att, int def, int spAtt, int spDef, int speed) {
	stats_t out;
	out.hp = out.mHp = hp;
	out.att = out.mAtt = att;
	out.def = out.mDef = def;
	out.spAtt = out.mSpAtt = spAtt;
	out.spDef = out.mSpDef = spDef;
	out.speed = out.mSpeed = speed;

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

type_t pk_initTypeFile(typeFileObj_t obj) {
	type_t out;

	// Transform header
	for(int i=0; i<8; i++) {
		out.name[i] = obj.header.name[i];
	}
	out.value = obj.val;
	out.resCnt = obj.header.resCnt;
	out.weakCnt = obj.header.weakCnt;

	// Transform resistances, weaknesses, and invincibilities
	out.res = (int*)malloc(out.resCnt*sizeof(int));
	out.weak = (int*)malloc(out.weakCnt*sizeof(int));
	out.inv = (bool*)malloc(out.resCnt*sizeof(bool));
	for(int i=0; i<out.resCnt; i++) {
		out.res[i] = obj.res[i].type;
		if(obj.res[i].inv == 1) {
			out.inv[i] = true;
		} else {
			out.inv[i] = false;
		}
	}
	for(int i=0; i<out.weakCnt; i++) {
		out.weak[i] = obj.weak[i].type;
	}

	return out;
}

int pk_calcTyping(type_t mType1, type_t mType2, type_t attType) {
	int effect = 0;

	if(mType1.value != T_NONE) {
		for(int i=0; i<mType1.resCnt; i++) {
			if(mType1.res[i] == attType.value) {
				effect--;
				if(mType1.inv[i]) {
					return RES_NONE;
				}
				break;
			}
			if(mType1.weak[i] == attType.value) {
				effect++;
				break;
			}
		}
	}

	if(mType2.value != T_NONE) {
		for(int i=0; i<mType2.resCnt; i++) {
			if(mType2.res[i] == attType.value) {
				effect--;
				if(mType2.inv[i]) {
					return RES_NONE;
				}
				break;
			}
			if(mType2.weak[i] == attType.value) {
				effect++;
				break;
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
	default:
		printf("[WARNING] Invalid effect value. Returning default!\n");
		return RES_NORMAL;
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

void pk_setTypeInv(type_t* type, int cnt, bool* types) {
	type->inv = (bool*)malloc(cnt*sizeof(bool));
	for(int i=0; i<cnt; i++) {
		type->inv[i] = types[i];
	}
}

void pk_freeType(type_t* obj) {
	free(obj->res);
	free(obj->weak);
	free(obj->inv);
}
