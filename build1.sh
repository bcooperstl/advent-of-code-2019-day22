#!/bin/bash
gcc -g -c deck.c
gcc -g -c part1.c
gcc -g -c part2.c
gcc -g -c part1_combined.c
gcc -o part1 part1.o deck.o
gcc -o part2 part2.o deck.o
gcc -o part1_combined part1_combined.o deck.o

