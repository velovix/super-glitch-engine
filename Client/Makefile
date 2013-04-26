obj = main.o mapman.o trainer_npc.o sessionman.o charmover.o window.o monsters.o player.o
aflags = -lSDL -lSDL_image -lSDL_mixer
poflags = -c
oflags = -std=gnu99
comp = gcc

all: $(obj)
	$(comp) -o out/PokeEngine $(obj) $(aflags)

main.o: main.c
	$(comp) $(poflags) main.c $(oflags)

mapman.o: general/mapman.h general/mapman.c
	$(comp) $(poflags) general/mapman.h general/mapman.c $(oflags)

trainer_npc.o: ai/trainer_npc.h ai/trainer_npc.c
	$(comp) $(poflags) ai/trainer_npc.h ai/trainer_npc.c $(oflags)

sessionman.o: general/sessionman.h general/sessionman.c
	$(comp) $(poflags) general/sessionman.h general/sessionman.c $(oflags)

charmover.o: ai/charmover.h ai/charmover.c
	$(comp) $(poflags) ai/charmover.h ai/charmover.c $(oflags)

monsters.o: battle/monsters.h battle/monsters.c
	$(comp) $(poflags) battle/monsters.h battle/monsters.c $(oflags)

window.o: general/window.h general/window.c
	$(comp) $(poflags) general/window.h general/window.c $(oflags)

player.o: ai/player.h ai/player.c
	$(comp) $(poflags) ai/player.h ai/player.c $(oflags)

clean:
	rm *.o *.h.gch
