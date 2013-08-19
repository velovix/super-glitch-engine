/* Super Glitch Engine Client
 * window.h
 * Copyright Tyler Compton 2013
 *
 * Declares a structure window_t. window_t represents a text window.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include "../constants.h"

typedef struct {
	int x, y, w, h;
	int selOpt, optCnt, optW, optH;
	int strLength, dispChar, dispDelay, startLag, selection;
	int start, stop, scrollNumb, scrollCnt;
	bool active, txtScroll, finished, closeable;
	char text[128];
	int dispTiles[128*6];
} window_t;

void pk_initWindow(int s_x, int s_y, int s_w, int s_h, bool s_closeable, bool dispDelay, window_t* window);

void pk_clearWindow(window_t* window);
void pk_updateWindow(window_t* window);

void pk_setWindowText(char* s_text, bool s_txtScroll, window_t* window);
void pk_setInsWindowText(char* baseText, char* insText, int insStart, int insLen, bool txtScroll, window_t* window);

char* pk_getWindowText(window_t window);
char* pk_getWindowDisp(window_t* window);

void pk_toggleWindow(window_t* window);

void pk_moveSelArrow(int dir, window_t* window);
void pk_selectWindow(window_t* window);
void pk_scrollWindowText(window_t* window);
void pk_fullWindowScroll(int scrollCnt, window_t* window);
bool pk_isWindowFinished(window_t window);

int pk_getCharValue(char c);
char *pk_insString(char* baseText, char* insText, int insLoc, int insLen);
char *pk_intToStr(int numb, int maxStrSize);

#endif
