OBJ = main.o mapman.o npc.o sessionman.o charmover.o window.o monsters.o stats.o player.o
AFLAGS = -lSDL -lSDL_image -lSDL_mixer
POFLAGS = -c
OFLAGS = -std=gnu99
COMP = gcc

all: $(OBJ)
	$(COMP) -o out/sge $(OBJ) $(AFLAGS)

main.o: main.c
	$(COMP) $(POFLAGS) main.c $(OFLAGS)

mapman.o: general/mapman.h general/mapman.c
	$(COMP) $(POFLAGS) general/mapman.c $(OFLAGS)

npc.o: ai/npc.h ai/npc.c
	$(COMP) $(POFLAGS) ai/npc.c $(OFLAGS)

sessionman.o: general/sessionman.h general/sessionman.c
	$(COMP) $(POFLAGS) general/sessionman.c $(OFLAGS)

charmover.o: ai/charmover.h ai/charmover.c
	$(COMP) $(POFLAGS) ai/charmover.c $(OFLAGS)

monsters.o: battle/monsters.h battle/monsters.c
	$(COMP) $(POFLAGS) battle/monsters.c $(OFLAGS)

stats.o: battle/stats.h battle/stats.c
	$(COMP) $(POFLAGS) battle/stats.c $(OFLAGS)

window.o: general/window.h general/window.c
	$(COMP) $(POFLAGS) general/window.c $(OFLAGS)

player.o: ai/player.h ai/player.c
	$(COMP) $(POFLAGS) ai/player.c $(OFLAGS)

clean:
	rm *.o
