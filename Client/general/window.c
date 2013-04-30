#include "window.h"

void pk_initWindow(int s_x, int s_y, int s_w, int s_h, bool s_closeable, bool dispDelay, window_t* window) {
	printf("Initialized a window!\n");
	window->x = s_x;
	window->y = s_y;
	window->w = s_w;
	window->h = s_h;
	window->dispChar = 0;
	window->dispDelay = 3;
	window->optCnt = 0;
	window->active = window->finished = false;
	window->closeable = s_closeable;
	window->selection = WSEL_NONE;

	pk_clearWindow(window);
}

void pk_clearWindow(window_t* window) {
	for(int y=0; y<window->h; y++) {
		for(int x=0; x<window->w; x++) {
			if(x==0 && y==0) {
				window->dispTiles[x+(y*window->w)] = WCORNER_TL;
			} else if(x==window->w-1 && y==0) {
				window->dispTiles[x+(y*window->w)] = WCORNER_TR;
			} else if(x==window->w-1 && y==window->h-1) {
				window->dispTiles[x+(y*window->w)] = WCORNER_BR;
			} else if(x==0 && y==window->h-1) {
				window->dispTiles[x+(y*window->w)] = WCORNER_BL;
			} else if(x==0 || x==window->w-1) {
				window->dispTiles[x+(y*window->w)] = WVERT;
			} else if(y==0 || y==window->h-1) {
				window->dispTiles[x+(y*window->w)] = WHORIZ;
			} else {
				window->dispTiles[x+(y*window->w)] = SPACE;
			}
		}
	}
}

void pk_updateWindow(window_t* window) {
	if(window->active) {
		if(window->startLag > 0) {
			window->startLag--;
			return;
		}

		if(window->dispChar < window->strLength) {
			if(window->dispDelay <= 0) {
				window->dispChar++;
				window->dispDelay = 1;
			} else {
				window->dispDelay--;
			}
		}

		int inc = 0;
		int x, y;
		x = y = 1;
		int lastOptDraw = 0;
		for(int i=0; i<window->dispChar; i++) {
			if(pk_getCharValue(window->text[inc]) == CE_NEWLINE) {
				x=1;
				y++;
			} else if(pk_getCharValue(window->text[inc]) == CE_OPTION) {
				if(lastOptDraw == window->selOpt) {
					window->dispTiles[x+(y*window->w)] = SELARROW;
				} else {
					window->dispTiles[x+(y*window->w)] = SPACE;
				}
				x++;
				lastOptDraw++;
			} else {
				window->dispTiles[x+(y*window->w)] = pk_getCharValue(window->text[inc]);
				x++;
			}
			inc++;
		}
	} else {
		if(window->txtScroll) {
			window->dispChar = 0;
		}
		window->dispDelay = 5;
	}
}

void pk_setWindowText(char* s_text, bool s_txtScroll, window_t* window) {

	pk_clearWindow(window);

	window->txtScroll = s_txtScroll;
	window->optCnt = 0;

	int inc = 0;
	while(pk_getCharValue(s_text[inc]) != CE_ENDSTR) {
		window->text[inc] = s_text[inc];
		if(pk_getCharValue(s_text[inc]) == CE_OPTION) {
			window->optCnt++;
		}
		inc++;
	}

	window->text[inc] = '|';
	if(window->txtScroll) {
		window->dispChar = 0;
	} else {
		window->dispChar = inc;
	}
	window->strLength = inc;

	window->finished = false;
}

char* pk_getWindowText(window_t window) {
	char* out;

	if(window.dispChar > 0) {
		out = (char*)malloc(window.dispChar*sizeof(char)+1);
		for(int i=0; i<window.dispChar; i++) {
			out[i] = window.text[i];
		}
		out[window.dispChar] = '|';
		return out;
	} else {
		return "|";
	}
}

void pk_setWOptionFunc(int s_opt, void (*func)(), window_t* window) {
	window->funcPtrs[s_opt].func = func;
}

void pk_toggleWindow(window_t* window) {
	if(window->startLag <= 0) {
		window->active = !window->active;
		if(window->active) {
			pk_clearWindow(window);
			window->finished = false;
			window->startLag = 10;
			if(window->txtScroll) {
				window->dispChar = 0;
			}
		}
	}
}

void pk_moveSelArrow(int dir, window_t* window) {
	if(dir == UP) {
		if(window->selOpt > 0) {
			window->selOpt--;
		} else {
			window->selOpt = window->optCnt-1;
		}
	} else if(dir == DOWN) {
		if(window->selOpt < window->optCnt-1) {
			window->selOpt++;
		} else {
			window->selOpt = 0;
		}
	}
}

void pk_selectWindow(window_t* window) {
	if(window->dispChar >= window->strLength && window->optCnt > 0) {
		if(window->funcPtrs[window->selOpt].func != NULL) {
			window->funcPtrs[window->selOpt].func();
		}
		window->selection = window->selOpt;
		
	} else if(window->dispChar >= window->strLength && window->optCnt == 0) {
		window->finished = true;
		if(window->closeable) {
			window->active = false;
		}
	}
}

void pk_scrollWindowText(window_t* window) {
	if(window->dispChar < window->strLength) {
		// Speed up
	} else if(window->closeable) {
		pk_toggleWindow(window);
		window->selection = WSEL_BACK;
	}
}

bool pk_isWindowFinished(window_t window) {
	return window.finished;
}

int pk_getCharValue(char c) {
	if(c >= 'A' && c <= 'Z') {
		return LETTERS_C + (c-'A');
	} else if(c >= 'a' && c <= 'z') {
		return LETTERS_L + (c-'a');
	} else if(c >= '0' && c <= '9') {
		return NUMBERS + (c-'0');
	} else if(c == ' ') {
		return SPACE;
	} else if(c == '+') {
		return CE_OPTION;
	} else if(c == '^') {
		return CE_NEWLINE;
	} else if(c == '!') {
		return EXCLAMATION;
	} else if(c == '?') {
		return QUESTION;
	} else if(c == '.') {
		return PERIOD;
	} else if(c == '\"') {
		return QUOTATION;
	} else if(c == '|') {
		return CE_ENDSTR;
	} else if(c == '/') {
		return SLASH;
	} else {
		return SPACE;
	}
}