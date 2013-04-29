#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int count;
} typeHeader_t;

typedef struct {
	char name[8];
	int resCnt;
	int weakCnt;
	int invCnt;
} typeEntry_t;

typedef struct {
	int type;
} resEntry_t;

typedef struct {
	int type;
} weakEntry_t;

typedef struct {
	int type;
} invEntry_t;

int main(int argc, char **argv) {
	typeHeader_t header;

	FILE* file;
	file = fopen("type.pke", "w");

	if(file == NULL) {
		printf("ERROR: Can't find type.pke!\n");
		return -1;
	}

	printf("How many types will you have?\n#? ");
	scanf("%d", &header.count);
	typeEntry_t typeHeader[header.count];
	printf("\n");
	fwrite(&header.count, sizeof(int), 1, file);

	for(int i=0; i<header.count; i++) {
		printf("Type #%d\n", i+1);
		printf("-| What is the name?\n");
		printf("-| s? ");
		scanf ("%s", &typeHeader[i].name);

		printf("-| How many resistances will it have?\n");
		printf("-| #? ");
		scanf ("%d", &typeHeader[i].resCnt);
		printf("-| How many weaknesses will it have?\n");
		printf("-| #? ");
		scanf ("%d", &typeHeader[i].weakCnt);
		printf("-| How many invulnerabilities will it have?\n");
		printf("-| #? ");
		scanf ("%d", &typeHeader[i].invCnt);
		fwrite(&typeHeader[i].name[0], sizeof(char[8]), 1, file);
		fwrite(&typeHeader[i].resCnt, sizeof(int), 1, file);
		fwrite(&typeHeader[i].weakCnt, sizeof(int), 1, file);
		fwrite(&typeHeader[i].invCnt, sizeof(int), 1, file);

		resEntry_t res[typeHeader[i].resCnt];
		weakEntry_t weak[typeHeader[i].weakCnt];
		invEntry_t inv[typeHeader[i].invCnt];
		printf("-| Resistances Time!!!\n");
		for(int j=0; j<typeHeader[i].resCnt; j++) {
			printf("---| Resistance #%d\n", j+1);
			printf("---| #? ");
			scanf("%d", &res[j].type);
			fwrite(&res[j].type, sizeof(int), 1, file);
		}

		printf("-| Weaknesses Time!!!\n");
		for(int j=0; j<typeHeader[i].weakCnt; j++) {
			printf("---| Weakness #%d\n", j+1);
			printf("---| #? ");
			scanf("%d", &weak[j].type);
			fwrite(&weak[j].type, sizeof(int), 1, file);
		}

		printf("-| Invulnerability Time!!!\n");
		for(int j=0; j<typeHeader[i].invCnt; j++) {
			printf("---| Invulnerability #%d\n", j+1);
			printf("---| #? ");
			scanf("%d", &inv[j].type);
			fwrite(&inv[j].type, sizeof(int), 1, file);
		}
	}

	fclose(file);
	return 0;
}
