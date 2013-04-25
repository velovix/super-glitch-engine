#ifndef FILEHEADERS_H
#define FILEHEADERS_H

typedef struct {
	int count;
} monHeader_t;

typedef struct {
	int value;
	char name[12];
	int hp, att, def, spAtt, spDef, speed;
} monEntry_t;

#endif