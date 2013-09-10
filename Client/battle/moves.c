#include "moves.h"

move_t pk_initMove(int s_cpp, int s_bpp, unsigned char* name, int type) {
	move_t out;
	out.bpp = s_bpp;
	out.type = type;

	for(int i=0; i<12; i++) {
		out.name[i] = name[i];
	}

	return out;
}

bool pk_parseMoveScript(unsigned char *script, int scriptLen, move_t *obj) {
	// If script appears to be null
	if(script[0] == ' ') {
		return false;
	}

	// Find how many events there are
	obj->eventCnt = 0;
	int inc = 0;
	while(inc < scriptLen) {
		if(script[inc] == '%') {
			obj->eventCnt++;
		}

		inc++;
	}

	// Create size for the events
	obj->events = (moveEvent_t*)malloc(obj->eventCnt*sizeof(moveEvent_t));

	// Read through event script
	inc = 0;
	for(int i=0; i<obj->eventCnt; i++) {
		// Move past expected newline, in the case of multiple events
		if(script[inc] == '\n') {
			inc++;
		}

		// Make sure we're at the beginning of an event
		if(script[inc] != '%') {
			return false;
		}
		inc++;
		// Check event type
		switch(script[inc]) {
		case 'd':
			obj->events[i].type = ME_DAMAGE;
			break;
		case 'p':
			obj->events[i].type = ME_POISON;
			break;
		}
		// Check for expected space
		if(script[inc+1] != ' ') {
			return false;
		}
		inc+=2;

		// Check target
		switch(script[inc]) {
		case 't':
			obj->events[i].target = ME_TARGET;
			break;
		case 'u':
			obj->events[i].target = ME_USER;
			break;
		}
		// Check for expected space
		if(script[inc+1] != ' ') {
			return false;
		}
		inc+=2;

		// Find out how many digits we're dealing with
		int valDigits = 0;
		while(script[inc] >= 48 && script[inc] < 58 && inc < scriptLen) {
			valDigits++;
			inc++;
		}
		inc-=valDigits;

		// Convert the char values into an int
		int digits[3];
		obj->events[i].value = 0;
		for(int j=0; j<3; j++) {
			if(valDigits >= 3-j) {
				digits[j] = (int)script[inc]-48;
				inc++;
			} else {
				digits[j] = 0;
			}
		}
		obj->events[i].value = (digits[0]*100)+(digits[1]*10)+(digits[2]);
	}

	return true;
}