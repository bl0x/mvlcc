MVLCC_DIR = ../

MVLCC_CONFIG = $(MVLCC_DIR)/bin/mvlcc-config.sh

CFLAGS +=  $(shell $(MVLCC_CONFIG) --cflags)
LDFLAGS += $(shell $(MVLCC_CONFIG) --ldflags)
LIBS +=    $(shell $(MVLCC_CONFIG) --libs)

.PHONY: all

all: test

test.o: test.c
	gcc -o $@ -c $< $(CFLAGS)

test: test.o
	g++ -o $@ $< $(LDFLAGS) $(LIBS)

clean:
	rm -rf test test.o
