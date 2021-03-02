###############################################################################
# Author: Jack Schneiderhan and Cindy Zhang
# Date: February 21 2021
# Pledge: I pledge my honor that I have abided by the Stevens Honor System
# Description: Makefile for quicksort.c, quicksort.h, and sort.c
###############################################################################

sort: sort.o quicksort.o
		gcc -g sort.o quicksort.o -o sort
sort.o: sort.c
		gcc -g -Wall -Werror -pedantic-errors -c sort.c
quicksort.o: quicksort.c quicksort.h
		gcc -g -Wall -Werror -pedantic-errors -c quicksort.c
clean:
		rm -f *.o sort sort.exe