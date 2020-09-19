#!/bin/bash

gcc -o ./LogoLike_output ../main.c ../OBJECTS/objects.c ../LEVEL/loadProject.c ../LEVEL/cmdToken/*/*.c \
../map/src/map.c \
`sdl2-config --cflags --libs` \
-lSDL2_ttf \
-lm -Os
