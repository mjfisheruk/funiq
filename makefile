CC=g++
CFLAGS=-Wall -g -std=c++0x
OUT=bin/funiq

all: funiq

funiq:
	$(CC) $(CFLAGS) -Ilib src/funiq.cpp -o $(OUT)