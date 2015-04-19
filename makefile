CC := gcc
PARALLELCC := mpicc

FLAGS := -Wall
LINKFLAGS := $(FLAGS) -Llibs -ltiff -lm
COMPILEFLAGS := $(FLAGS)  -std=c99

all: bin/exhaustive bin/pciam

bin/exhaustive: obj/exhaustive.o obj/common.o obj/ncc.o | bin
	$(CC) $(LINKFLAGS) -o $@ $^

obj/exhaustive.o: exhaustive.c common.h ncc.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

obj/common.o: common.c common.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

obj/ncc.o: ncc.c ncc.h common.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

bin/pciam: obj/pciam.o obj/common.o | bin
	$(CC) $(LINKFLAGS) -lfftw3 -o $@ $^

obj/pciam.o: pciam.c common.h | obj
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

obj:
	mkdir obj

bin:
	mkdir bin

.PHONY: test
test: bin/pciam
	LD_LIBRARY_PATH=libs bin/pciam img1.tif img2.tif

clean:
	rm bin/* obj/*

cleanall:
	rm -r bin/ obj/
