CC=g++
CFLAGS=-Wall -g -std=c++0x
OPT_FLAG=-O2
OUT=bin/funiq
OUT_TEST=bin/funiq_test

all: test build

build_test:
	$(CC) ${CFLAGS} -Ilib test/test.cpp -o $(OUT_TEST)

test: build_test
	bin/funiq_test

build:
	$(CC) $(CFLAGS) ${OPT_FLAG} -Ilib src/funiq.cpp -o $(OUT)
