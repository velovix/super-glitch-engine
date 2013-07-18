# Makefile for Type Maker program

CC=gcc
CFLAGS=`pkg-config --libs --cflags gtk+-3.0` -export-dynamic -std=gnu99 -funsigned-char

all:
	$(CC) -Wall -g -o MoveMaker main.c $(CFLAGS)
