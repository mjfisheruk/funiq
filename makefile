CC=g++
CFLAGS=-Wall
OUT=bin/funiq

all: funiq

funiq:
	$(CC) $(CFLAGS) -Ilib/ src/funiq.cpp -o $(OUT)