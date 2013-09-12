# Makefile for Type Maker program

OBJ=main.o typeFile.o moveFile.o monFile.o
CC=gcc
FLAGS=`pkg-config --libs --cflags gtk+-3.0` -export-dynamic -std=gnu99

all: $(OBJ)
	$(CC) -Wall -o PokeMaker $(OBJ) $(FLAGS)

main.o: main.c
	$(CC) -c main.c $(FLAGS)

typeFile.o: ../common/typeFile.h ../common/typeFile.c
	$(CC) -c ../common/typeFile.c $(FLAGS)

moveFile.o: ../common/moveFile.h ../common/moveFile.c
	$(CC) -c ../common/moveFile.c $(FLAGS)

monFile.o: ../common/monFile.h ../common/monFile.c
	$(CC) -c ../common/monFile.c $(FLAGS)
