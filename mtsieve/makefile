###############################################################################
# Author: Jack Schneiderhan and Cindy Zhang
# Date: April 17 2021
# Pledge: I pledge my honor that I have abided by the Stevens Honor System.
# Description: makefile for mtsieve.c
###############################################################################
CC     = gcc
C_FILE = $(wildcard *.c)
TARGET = $(patsubst %.c,%,$(C_FILE))
CFLAGS = -O3 -Wall -Werror -pedantic-errors 

all:
	$(CC) $(CFLAGS) $(C_FILE) -o $(TARGET) -lm -pthread
clean:
	rm -f $(TARGET) $(TARGET).exe
