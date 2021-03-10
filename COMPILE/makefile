#!/bin/bash

CC=gcc
CC_FLAGS=-lSDL2_ttf -lm -Os

LogoLike_output: ../main.c ../OBJECTS/objects.c ../LEVEL/loadProject.c ../LEVEL/cmdToken/*/*.c
	$(CC) -o ./LogoLike_output ../main.c ../OBJECTS/objects.c ../LEVEL/loadProject.c ../LEVEL/cmdToken/*/*.c \
	../map/src/map.c \
	`sdl2-config --cflags --libs` \
	$(CC_FLAGS)
