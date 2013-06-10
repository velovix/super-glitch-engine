#include "musicman.h"

void newMusic(char* filename, musicMan_t* obj)
{
	int len = 0;
	while(filename[len] != ' ') {
		len++;
	}

	obj->filenames[obj->cnt] = (char*)malloc(len*sizeof(char));
	for(int i=0; i<len; i++) {
		obj->filenames[obj->cnt][i] = filename[i];
	}

	obj->cnt++;
}

char* getMusic(int val, musicMan_t* obj)
{
	obj->lastMusic = val;
	return obj->filenames[val];
}

bool isDifferent(int val, musicMan_t* obj)
{
	if(val != obj->lastMusic) {
		return true;
	} else {
		return false;
	}
}