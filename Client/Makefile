OBJ = main.o mapman.o npc.o sessionman.o charmover.o window.o monsters.o stats.o player.o moves.o mapFile.o typeFile.o moveFile.o palette.o monFile.o
LIB = -lSDL -lSDL_image -lSDL_mixer -lm
OFLAGS = -std=gnu99 -funsigned-char
COMP = gcc

all: $(OBJ)
	$(COMP) -o sge $(OBJ) $(LIB)

main.o: main.c
	$(COMP) -c -Wall main.c $(OFLAGS)

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

mapFile.o: ../common/mapFile.h ../common/mapFile.c
	$(COMP) -c ../common/mapFile.c $(OFLAGS)

typeFile.o: ../common/typeFile.h ../common/typeFile.c
	$(COMP) -c ../common/typeFile.c $(OFLAGS)

moveFile.o: ../common/moveFile.h ../common/moveFile.c
	$(COMP) -c ../common/moveFile.c $(OFLAGS)

palette.o: ../common/media/palette.h ../common/media/palette.c
	$(COMP) -c -Wall ../common/media/palette.c $(OFLAGS)

monFile.o: ../common/monFile.h ../common/monFile.c
	$(COMP) -c -Wall ../common/monFile.c $(OFLAGS)

clean:
	rm *.o
