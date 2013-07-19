OBJ = main.o mapman.o npc.o sessionman.o charmover.o window.o monsters.o stats.o player.o musicman.o moves.o
LIB = -lSDL -lSDL_image -lSDL_mixer
OFLAGS = -std=gnu99 -funsigned-char
COMP = gcc

all: $(OBJ)
	$(COMP) -o out/sge $(OBJ) $(LIB)

main.o: main.c
	$(COMP) -c main.c $(OFLAGS)

mapman.o: general/mapman.h general/mapman.c
	$(COMP) -c general/mapman.c $(OFLAGS)

npc.o: ai/npc.h ai/npc.c
	$(COMP) -c ai/npc.c $(OFLAGS)

sessionman.o: general/sessionman.h general/sessionman.c
	$(COMP) -c general/sessionman.c $(OFLAGS)

charmover.o: ai/charmover.h ai/charmover.c
	$(COMP) -c ai/charmover.c $(OFLAGS)

monsters.o: battle/monsters.h battle/monsters.c
	$(COMP) -c battle/monsters.c $(OFLAGS)

stats.o: battle/stats.h battle/stats.c
	$(COMP) -c battle/stats.c $(OFLAGS)

window.o: general/window.h general/window.c
	$(COMP) -c general/window.c $(OFLAGS)

player.o: ai/player.h ai/player.c
	$(COMP) -c ai/player.c $(OFLAGS)

moves.o: battle/moves.h battle/moves.c
	$(COMP) -c battle/moves.c $(OFLAGS)

musicman.o: media/musicman.h media/musicman.c
	$(COMP) -c media/musicman.c $(OFLAGS)

clean:
	rm *.o
