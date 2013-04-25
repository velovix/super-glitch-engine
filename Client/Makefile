obj = main.o mapman.o trainer_npc.o sessionman.o charmover.o window.o monsters.o player.o
aflags = -lSDL -lSDL_image -lSDL_mixer
oflags = -std=gnu99
comp = gcc

all: $(obj)
	$(comp) -o out/PokeEngine $(obj) $(aflags)

main.o: main.c
	$(comp) -c main.c $(oflags)

mapman.o: general/mapman.h general/mapman.c
	$(comp) -c general/mapman.h general/mapman.c $(oflags)

trainer_npc.o: ai/trainer_npc.h ai/trainer_npc.c
	$(comp) -c ai/trainer_npc.h ai/trainer_npc.c $(oflags)

sessionman.o: general/sessionman.h general/sessionman.c
	$(comp) -c general/sessionman.h general/sessionman.c $(oflags)

charmover.o: ai/charmover.h ai/charmover.c
	$(comp) -c ai/charmover.h ai/charmover.c $(oflags)

monsters.o: battle/monsters.h battle/monsters.c
	$(comp) -c battle/monsters.h battle/monsters.c $(oflags)

window.o: general/window.h general/window.c
	$(comp) -c general/window.h general/window.c $(oflags)

player.o: ai/player.h ai/player.c
	$(comp) -c ai/player.h ai/player.c $(oflags)

clean:
	rm *.o *.h.gch
