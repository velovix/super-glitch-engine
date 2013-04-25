#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int count;
} pmHeader_t;

typedef struct {
	int value;
	char name[12];
	int hp, att, def, spAtt, spDef, speed;
} monEntry_t;

int main(int argc, char **argv) {
	pmHeader_t header;

	FILE* file;
	file = fopen("pk.pke", "w");

	if(file == NULL) {
		printf("ERROR: Can't find pk.pke!\n");
		return -1;
	}

	printf("#? ");
	scanf("%i", &header.count);
	monEntry_t mons[header.count];
	fwrite(&header.count, sizeof(int), 1, file);

	for(int i=0; i<header.count; i++) {
		printf("\nPokemon %i:\n", i);

		printf("Value ? ");		scanf("%i", &mons[i].value);

		char buffer[12];
		printf("Name  ? \n"); 
		printf("|----------|\n");	scanf("%s", &buffer[0]);
		bool readString = true;
		for(int j=0; j<11; j++) {
			mons[i].name[j] = buffer[j];
		}

		printf("HP    ? ");		scanf("%i", &mons[i].hp);
		printf("Att   ? ");		scanf("%i", &mons[i].att);
		printf("Def   ? ");		scanf("%i", &mons[i].def);
		printf("SpAtt ? ");		scanf("%i", &mons[i].spAtt);
		printf("SpDef ? ");		scanf("%i", &mons[i].spDef);
		printf("Speed ? ");		scanf("%i", &mons[i].speed);

		fwrite(&mons[i].value, sizeof(int), 1, file);
		fwrite(&mons[i].name, sizeof(char[12]), 1, file);
		fwrite(&mons[i].hp, sizeof(int), 1, file);
		fwrite(&mons[i].att, sizeof(int), 1, file);
		fwrite(&mons[i].def, sizeof(int), 1, file);
		fwrite(&mons[i].spAtt, sizeof(int), 1, file);
		fwrite(&mons[i].spDef, sizeof(int), 1, file);
		fwrite(&mons[i].speed, sizeof(int), 1, file);
	}

	fclose(file);
	return 0;
}
