#include "moves.h"

move_t pk_initMove(int s_cpp, int s_bpp, char* name, int type) {
	move_t out;
	out.bpp = s_bpp;
	out.type = type;

	for(int i=0; i<12; i++) {
		out.name[i] = name[i];
	}

	return out;
}

bool pk_parseMoveScript(char *script, int scriptLen, move_t *obj) {
	printf("Reading a move.\n");
	// If script appears to be null
	if(script[0] == ' ') {
		return false;
	}

	// Find how many events there are
	obj->eventCnt = 0;
	int inc = 0;
	while(true) {
		if(inc >= scriptLen) {
			break;
		}
		if(script[inc] == '%') {
			obj->eventCnt++;
		}

		inc++;
	}
	printf("   Event Cnt: %i\n", obj->eventCnt);

	// Create size for the events
	obj->events = (moveEvent_t*)malloc(obj->eventCnt*sizeof(moveEvent_t));

	// Read through event script
	inc = 0;
	for(int i=0; i<obj->eventCnt; i++) {
		if(script[inc] != '%') {
			return false;
		}
		inc++;
		// Check event type
		switch(script[inc]) {
		case 'd':
			printf("   Found a damage event!\n");
			obj->events[i].type = ME_DAMAGE;
			break;
		case 'p':
			obj->events[i].type = ME_POISON;
			break;
		}
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
		printf("   Found out there were %i digits!\n", valDigits);

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
		printf("   The value is %i+%i+%i=%i\n",digits[0], digits[1], digits[2], obj->events[i].value);
	}
}