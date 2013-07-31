#include "window.h"

void pk_initWindow(int s_x, int s_y, int s_w, int s_h, bool s_closeable, bool dispDelay, window_t* window) {
	window->x = s_x;
	window->y = s_y;
	window->w = s_w;
	window->h = s_h;
	window->dispChar = window->startLag = 0;
	window->dispDelay = 3;
	window->optCnt = window->optW = window->optH = 0;
	window->active = window->finished = false;
	window->closeable = s_closeable;
	window->selection = WSEL_NONE;
	window->scrollNumb = 1;

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

		if(window->dispChar < window->stop) {
			if(window->dispDelay <= 0) {
				window->dispChar++;
				window->dispDelay = 1;
			} else {
				window->dispDelay--;
			}
		}

		int x, y;
		x = y = 1;
		int lastOptDraw = 0;
		for(int i=window->start; i<window->dispChar; i++) {
			if(pk_getCharValue(window->text[i]) == CE_NEWLINE) {
				x=1;
				y++;
			} else if(pk_getCharValue(window->text[i]) == CE_OPTION) {
				if(lastOptDraw == window->selOpt) {
					window->dispTiles[x+(y*window->w)] = SELARROW;
				} else {
					window->dispTiles[x+(y*window->w)] = SPACE;
				}
				x++;
				lastOptDraw++;
			} else {
				window->dispTiles[x+(y*window->w)] = pk_getCharValue(window->text[i]);
				x++;
			}
		}
	} else {
		window->dispDelay = 5;
	}
}

void pk_setWindowText(char* s_text, bool s_txtScroll, window_t* window) {

	pk_clearWindow(window);

	window->txtScroll = s_txtScroll;
	window->optCnt = 0;
	window->optW = window->optH = 0;
	window->scrollCnt = 0;
	window->scrollNumb = 1;

	int inc = 0;
	bool settingWidth = true;
	while(pk_getCharValue(s_text[inc]) != CE_ENDSTR) {
		window->text[inc] = s_text[inc];
		if(pk_getCharValue(s_text[inc]) == CE_NEWLINE && window->optCnt != 0) {
			settingWidth = false;
			window->optH++;
		}
		if(pk_getCharValue(s_text[inc]) == CE_OPTION) {
			window->optCnt++;
			if(settingWidth) {
				window->optW++;
			}
		}
		if(pk_getCharValue(s_text[inc]) == CE_FULLSCROLL) {
			window->scrollCnt++;
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

	pk_fullWindowScroll(window->scrollNumb, window);
}

void pk_setInsWindowText(char* baseText, char* insText, int insStart, int insLen, bool txtScroll, window_t* window) {

	char* text = (char*)malloc(128*sizeof(char));

	int inc = 0;
	while(pk_getCharValue(baseText[inc]) != CE_ENDSTR) {
		text[inc] = baseText[inc];
		inc++;
	}

	for(int i=0; i<insLen; i++) {
		text[insStart+i] = insText[i];
	}

	text[inc] = '|';

	pk_setWindowText(text, txtScroll, window);
}

char* pk_getWindowText(window_t window) {
	char* out;
	out = (char*)malloc((window.stop-window.start)*sizeof(char));

	int inc = 0;
	for(int i=window.start; i<window.stop; i++) {
		out[inc] = window.text[i];
		inc++;
	}

	out[window.dispChar] = '|';

	return out;
}

void pk_toggleWindow(window_t* window) {
	if(window->startLag <= 0) {
		window->active = !window->active;
		if(window->active) {
			pk_clearWindow(window);
			window->finished = false;
			window->startLag = 10;
			window->scrollNumb = 1;
			pk_fullWindowScroll(1, window);
			if(window->txtScroll) {
				window->dispChar = 0;
			}
		}
	}
}

void pk_moveSelArrow(int dir, window_t* window) {
	if(dir == UP) {
		if(window->optH > 1) {
			window->selOpt -= window->optW;
		}
	} else if(dir == DOWN) {
		if(window->optH > 1) {
			window->selOpt += window->optW;
		}
	} else if(dir == RIGHT) {
		if(window->optW > 1) {
			window->selOpt += window->optW-1;
		}
	} else if(dir == LEFT) {
		if(window->optW > 1) {
			window->selOpt -= window->optW-1;
		}
	}

	if(window->selOpt < 0) {
		window->selOpt = window->optCnt + window->selOpt;
	} else if(window->selOpt >= window->optCnt) {
		window->selOpt = window->selOpt - window->optCnt;
	}
}

void pk_selectWindow(window_t* window) {
	if(window->dispChar >= window->strLength && window->optCnt > 0) {
		window->selection = window->selOpt;
		
	} else if(window->dispChar >= window->strLength && window->optCnt == 0) {
		window->finished = true;
		if(window->closeable) {
			window->active = false;
		}
	} else if(window->dispChar >= window->stop && window->scrollNumb < window->scrollCnt) {
		window->scrollNumb++;
		pk_fullWindowScroll(window->scrollNumb, window);
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

void pk_fullWindowScroll(int scrollCnt, window_t* window) {
	pk_clearWindow(window);
	int cnt = 0;
	for(int i=0; i<128; i++) {
		if(pk_getCharValue(window->text[i]) == CE_FULLSCROLL) {
			cnt++;
		}

		if(cnt == scrollCnt) {
			window->start = i+1;
			break;
		}
	}

	for(int i=window->start; i<128; i++) {
		if(pk_getCharValue(window->text[i]) == CE_FULLSCROLL || pk_getCharValue(window->text[i]) == CE_ENDSTR) {
			window->stop = i;
			break;
		}
	}

	if(window->txtScroll) {
		window->dispChar = window->start;
	} else {
		window->dispChar = window->stop;
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
	} else if(c == '{') {
		return CE_FULLSCROLL;
	} else {
		return SPACE;
	}
}

char *pk_insString(char* baseText, char* insText, int insLoc, int insLen) {
	char* text = (char*)malloc(128*sizeof(char));

	// Tries to autodetect end of inserted string
	if(insLen == -1) {
		for(insLen=0; true; insLen++) {
			if(insText[insLen] == ' ' && insText[insLen+1] == ' ') {
				break;
			}
		}
	}

	int baseTextLen = 0;
	// Copy base text into output text
	for(int i=0; pk_getCharValue(baseText[i]) != CE_ENDSTR; i++) {
		text[i] = baseText[i];
		baseTextLen = i;
	}

	// Put the inserted text into the output text
	for(int i=0; i<insLen; i++) {
		text[insLoc+i] = insText[i];
	}

	text[baseTextLen] = '|';

	return text;
}