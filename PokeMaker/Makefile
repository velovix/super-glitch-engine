# Makefile for Type Maker program

CC=gcc
CFLAGS=`pkg-config --libs --cflags gtk+-3.0` -export-dynamic -std=gnu99

all:
	$(CC) -Wall -g -o PokeMaker main.c $(CFLAGS)
