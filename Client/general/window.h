#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "../constants.h"

typedef struct {
	void (*func)();
} funcPointer_t;

typedef struct {
	int x, y, w, h;
	int selOpt, optCnt, strLength, dispChar, dispDelay, startLag;
	bool active, txtScroll, finished, closeable;
	char text[128];
	int dispTiles[128*6];
	funcPointer_t funcPtrs[16];
} window_t;

void pk_initWindow(int s_x, int s_y, int s_w, int s_h, bool s_closeable, window_t* window);
void pk_clearWindow(window_t* window);
void pk_updateWindow(window_t* window);
void pk_setWindowText(char* s_text, bool s_txtScroll, window_t* window);
char* pk_getWindowDisp(window_t* window);
void pk_setWOptionFunc(int s_opt, void (*func)(), window_t* window);
void pk_toggleWindow(window_t* window);
void pk_moveSelArrow(int dir, window_t* window);
void pk_selectWindow(window_t* window);
void pk_scrollWindowText(window_t* window);
bool pk_isWindowFinished(window_t window);
int pk_getCharValue(char c);

#endif