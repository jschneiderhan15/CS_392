###############################################################################
# Author: Jack Schneiderhan and Cindy Zhang
# Date: March 14th 2021
# Pledge: I pledge my honor that I have abided by the Stevens Honor System
# Description: Makefile for perms.c, perms.h, and spfind.c
###############################################################################

sspfind: spfind.o perms.o
		gcc spfind.o perms.o -o spfind
sspfind.o: spfind.c
		gcc -Wall -Werror -pedantic-errors -c spfind.c
perms.o: perms.c perms.h
		gcc -Wall -Werror -pedantic-errors -c perms.c
clean:
		rm -f *.o spfind spfind.exe