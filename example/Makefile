CCNAME:=$(notdir $(CC))
BUILD_DIR := build_$(CCNAME)_$(shell $(CC) -dumpmachine)_$(shell $(CC) -dumpversion)
MESYTECPATH=../../mesytec-mvlc/

.PHONY: all

all: test

test.o: test.c
	gcc -o test.o -c test.c -I../include/

test: test.o
	g++ -o test test.o -L../$(BUILD_DIR) -lmvlcc -L$(MESYTECPATH)/build/ -lmesytec-mvlc -Wl,-rpath=$(MESYTECPATH)/build/

clean:
	rm -rf test test.o
