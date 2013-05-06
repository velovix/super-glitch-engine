OBJ = main.o mapman.o trainer_npc.o sessionman.o charmover.o window.o monsters.o player.o
AFLAGS = -lSDL -lSDL_image -lSDL_mixer
POFLAGS = -c
OFLAGS = -std=gnu99
COMP = gcc

all: $(OBJ)
	$(COMP) -o out/sge $(OBJ) $(AFLAGS)

main.o: main.c
	$(COMP) $(POFLAGS) main.c $(OFLAGS)

mapman.o: general/mapman.h general/mapman.c
	$(COMP) $(POFLAGS) general/mapman.h general/mapman.c $(OFLAGS)

trainer_npc.o: ai/trainer_npc.h ai/trainer_npc.c
	$(COMP) $(POFLAGS) ai/trainer_npc.h ai/trainer_npc.c $(OFLAGS)

sessionman.o: general/sessionman.h general/sessionman.c
	$(COMP) $(POFLAGS) general/sessionman.h general/sessionman.c $(OFLAGS)

charmover.o: ai/charmover.h ai/charmover.c
	$(COMP) $(POFLAGS) ai/charmover.h ai/charmover.c $(OFLAGS)

monsters.o: battle/monsters.h battle/monsters.c
	$(COMP) $(POFLAGS) battle/monsters.h battle/monsters.c $(OFLAGS)

window.o: general/window.h general/window.c
	$(COMP) $(POFLAGS) general/window.h general/window.c $(OFLAGS)

player.o: ai/player.h ai/player.c
	$(COMP) $(POFLAGS) ai/player.h ai/player.c $(OFLAGS)

clean:
	rm *.o *.h.gch
