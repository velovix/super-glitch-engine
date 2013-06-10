#ifndef MUSIC_H
#define MUSIC_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int cnt;
	int lastMusic;
	char* filenames[30];
} musicMan_t;

void newMusic(char* filename, musicMan_t* obj);
void editMusic(int val, char* filename, musicMan_t* obj);
char* getMusic(int val, musicMan_t* obj);
bool isDifferent(int val, musicMan_t* obj);


#endif