#!/bin/bash

cd content; find . -type f -name '*.h' -print -delete; ./pack.sh; cd ..; gcc main.c include/glad.c -o engine -lGL -lglfw -lm -flto -Os -funroll-loops -fexpensive-optimizations -pipe -s
