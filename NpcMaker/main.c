#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
	int count;
} npcFileHeader_f;

typedef struct {
	int x, y;
	int destX, destY;
	int aiType;
	bool fightable;

	int monsters[6];

	char msg1[128];
	char msg2[128];
	char msg3[128];
} npc_f;

int main(int argc, char **argv) {
	FILE* file;
	file = fopen("npc.pke", "w");

	printf("NPC Maker: Mockup Version\n\n");

	npcFileHeader_f header;
	printf("NPC Count: ");
	scanf("%i", &header.count);
	printf("\n");
	fwrite(&header.count, sizeof(int), 1, file);

	npc_f npc[header.count];
	for(int i=0; i<header.count; i++) {
		printf("Starting Coordinates\n");
		printf("   x? ");		scanf("%i", &npc[i].x);
		printf("   y? ");		scanf("%i", &npc[i].y);
		fwrite(&npc[i].x, sizeof(int), 1, file);
		fwrite(&npc[i].y, sizeof(int), 1, file);

		printf("AI Type\n");
		printf("   Nothing:0 | Turn:1 | Back&Forth:3 | Wander:4\n");
		printf("   Type? ");	scanf("%i", &npc[i].aiType);
		fwrite(&npc[i].aiType, sizeof(int), 1, file);

		if(npc[i].aiType == 3) {
			printf("Destination\n");
			printf("   x? ");	scanf("%i", &npc[i].destX);
			printf("   y? ");	scanf("%i", &npc[i].destY);
		} else {
			npc[i].destX = npc[i].x;
			npc[i].destY = npc[i].y;
		}
		fwrite(&npc[i].destX, sizeof(int), 1, file);
		fwrite(&npc[i].destY, sizeof(int), 1, file);

		int fEntry = 1;
		printf("Fightability\n");
		printf("   Fightable:1 | Neutral:2\n");
		printf("   Fight? ");	scanf("%i", &fEntry);
		if(fEntry == 1) {
			npc[i].fightable = true;
		} else {
			npc[i].fightable = false;
		}
		fwrite(&npc[i].fightable, sizeof(bool), 1, file);

		printf("Team\n");
		for(int j=0; j<6; j++) {
			printf("   Monster %i? ", j);
			scanf("%i", &npc[i].monsters[j]);
			fwrite(&npc[i].monsters[j], sizeof(int), 1, file);
		}

		for(int j=0; j<128; j++) {
			npc[i].msg1[j] = ' ';
			npc[i].msg2[j] = ' ';
			npc[i].msg3[j] = ' ';
		}
		printf("Messages (Use _ for spaces)\n");
		printf("   Msg1? ");	scanf("%s", npc[i].msg1);
		printf("   Msg2? ");	scanf("%s", npc[i].msg1);
		printf("   Msg3? ");	scanf("%s", npc[i].msg1);
		fwrite(&npc[i].msg1, sizeof(char), 128, file);
		fwrite(&npc[i].msg2, sizeof(char), 128, file);
		fwrite(&npc[i].msg3, sizeof(char), 128, file);
	}
	fclose(file);
	return 0;
}
