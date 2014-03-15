CC=g++
CFLAGS=-Wall -g -std=c++0x -O2
OUT=bin/funiq

all: funiq

funiq:
	$(CC) $(CFLAGS) -Ilib src/funiq.cpp -o $(OUT)
