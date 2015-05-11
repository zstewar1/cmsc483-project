CC := gcc
PARALLELCC := mpicc

FLAGS := -Wall
LINKFLAGS := $(FLAGS) -Ltiff-3.9.7/libtiff/.libs -ltiff -lm -Lfftw-3.3.4/lib
COMPILEFLAGS := $(FLAGS) -Itiff-3.9.7/libtiff -Ifftw-3.3.4/include -std=gnu99

all: bin/exhaustive bin/pciam bin/saserial bin/saparallel



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

bin/saserial: obj/saserial.o obj/common.o obj/ncc.o | bin
	$(CC) $(LINKFLAGS) -o $@ $^

obj/saserial.o: saserial.c ncc.h common.h
	$(CC) $(COMPILEFLAGS) -c -o $@ $<

bin/saparallel: obj/saparallel.o obj/common.o obj/ncc.o | bin
	$(PARALLELCC) $(LINKFLAGS) -o $@ $^

obj/saparallel.o: saparallel.c ncc.h common.h | obj
	$(PARALLELCC) $(COMPILEFLAGS) -c -o $@ $<

bin/mcparallel: obj/mcparallel.o obj/common.o obj/ncc.o | bin
	$(PARALLELCC) $(LINKFLAGS) -o $@ $^

obj/mcparallel.o: mcparallel.c ncc.h common.h | obj
	$(PARALLELCC) $(COMPILEFLAGS) -c -o $@ $<


obj:
	mkdir obj

bin:
	mkdir bin

.PHONY: test
test: bin/exhaustive
	LD_LIBRARY_PATH=tiff-3.9.7/libtiff/.libs bin/exhaustive I1_sm.tif I2_sm.tif

clean:
	rm bin/* obj/*

cleanall:
	rm -r bin/ obj/
