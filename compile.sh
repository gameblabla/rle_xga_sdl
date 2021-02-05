#!/bin/sh
gcc -g3 -O0 main.c -o main.elf -lSDL -lSDL_image
gcc -g3 -O0 dec.c -o dec.elf -lSDL -lSDL_image
