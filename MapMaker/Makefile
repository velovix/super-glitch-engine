OBJ = main.o mapFile.o
CC=gcc
CFLAGS = -lSDL -lSDL_image
OFLAGS = -std=gnu99

all: $(OBJ)
	$(CC) -o MapMaker $(OBJ) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(OFLAGS)

mapFile.o: ../common/mapFile.h ../common/mapFile.c
	$(CC) -c ../common/mapFile.c $(OFLAGS)

clean:
	rm *.o
