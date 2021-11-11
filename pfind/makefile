###############################################################################
# Author: Jack Schneiderhan and Cindy Zhang
# Date: February 21 2021
# Pledge: I pledge my honor that I have abided by the Stevens Honor System
# Description: Makefile for quicksort.c, quicksort.h, and sort.c
###############################################################################

pfind: pfind.o perms.o
		gcc pfind.o perms.o -o pfind
pfind.o: pfind.c
		gcc -Wall -Werror -pedantic-errors -c pfind.c
perms.o: perms.c perms.h
		gcc -Wall -Werror -pedantic-errors -c perms.c
clean:
		rm -f *.o pfind pfind.exe