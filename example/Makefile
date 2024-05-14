CCNAME:=$(notdir $(CC))
BUILD_DIR := build_$(CCNAME)_$(shell $(CC) -dumpmachine)_$(shell $(CC) -dumpversion)
MESYTECPATH=../../mesytec-mvlc/

.PHONY: all

all: test.c
	g++ -o test test.c -I../include/ -L../$(BUILD_DIR) -lmvlcc -L$(MESYTECPATH)/build/ -lmesytec-mvlc -Wl,-rpath=$(MESYTECPATH)/build/
