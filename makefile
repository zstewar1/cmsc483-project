CC := gcc
PARALLELCC := mpicc

FLAGS := -Wall
LINKFLAGS := $(FLAGS) -ltiff
COMPILEFLAGS := $(FLAGS) -std=c11

all: bin/exhaustive

bin/exhaustive: obj/exhaustive.o obj/common.o | bin
	$(CC) $(LINKFLAGS) -o $@ $^

obj/exhaustive.o: exhaustive.c common.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

obj/common.o: common.c common.h | obj
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
