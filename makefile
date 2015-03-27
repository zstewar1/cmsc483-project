CC := gcc
PARALLELCC := mpicc

all: bin/exhaustive

bin/exhaustive: obj/exhaustive.o | bin
	$(CC) -o $@ $^

obj/exhaustive.o: exhaustive.c | obj
	$(CC) -c -o $@ $<

obj:
	mkdir obj

bin:
	mkdir bin

clean:
	rm bin/* obj/*

cleanall:
	rm -r bin/ obj/
