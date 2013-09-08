# Makefile for Type Maker program

OBJ=main.o typeFile.o
CC=gcc
FLAGS=`pkg-config --libs --cflags gtk+-3.0` -std=gnu99 -export-dynamic

all: $(OBJ)
	$(CC) -o TypeMaker $(OBJ) $(FLAGS)

main.o: main.c
	$(CC) -c main.c $(FLAGS)

typeFile.o: ../common/typeFile.h ../common/typeFile.c
	$(CC) -c ../common/typeFile.c $(FLAGS)
