CC := gcc
PARALLELCC := mpicc

FLAGS := -Wall
LINKFLAGS := $(FLAGS) -ltiff -lm
COMPILEFLAGS := $(FLAGS) -std=c99

all: bin/exhaustive

bin/exhaustive: obj/exhaustive.o obj/common.o obj/ncc.o | bin
	$(CC) $(LINKFLAGS) -o $@ $^

obj/exhaustive.o: exhaustive.c common.h ncc.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

obj/common.o: common.c common.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

obj/ncc.o: ncc.c ncc.h common.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

obj:
	mkdir obj

bin:
	mkdir bin

.PHONY: test
test: bin/exhaustive
	bin/exhaustive img1.tif img2.tif

clean:
	rm bin/* obj/*

cleanall:
	rm -r bin/ obj/
