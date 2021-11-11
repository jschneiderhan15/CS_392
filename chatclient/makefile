###############################################################################
# Author: Jack Schneiderhan and Cindy Zhang
# Date: May 5 2021
# Pledge: I pledge my honor that I have abided by the Stevens Honor System.
# Description: makefile for chatclient.c
###############################################################################
CC     = gcc
C_FILE = chatclient.c
TARGET = $(patsubst %.c,%,$(C_FILE))
CFLAGS = -g -Wall -Werror -pedantic-errors

all:
	$(CC) $(CFLAGS) $(C_FILE) -o $(TARGET)
clean:
	rm -f $(TARGET) $(TARGET).exe
