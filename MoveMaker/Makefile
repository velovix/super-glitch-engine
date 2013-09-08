# Makefile for Type Maker program

OBJ=main.o moveFile.o typeFile.o
CC=gcc
FLAGS=`pkg-config --libs --cflags gtk+-3.0` -export-dynamic -std=gnu99 -funsigned-char

all: $(OBJ)
	$(CC) -Wall -o MoveMaker $(OBJ) $(FLAGS)

main.o: main.c
	$(CC) -c main.c $(FLAGS)

moveFile.o: ../common/moveFile.h ../common/moveFile.c
	$(CC) -c ../common/moveFile.c $(FLAGS)

typeFile.o: ../common/typeFile.h ../common/typeFile.c
	$(CC) -c ../common/typeFile.c $(FLAGS)
