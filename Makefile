.PHONY: all clean

CC ?= gcc
LEX ?= flex
BUILD_PREFIX ?= build
SRC ?= src

all: $(BUILD_PREFIX)/scanner

$(BUILD_PREFIX)/scanner: $(BUILD_PREFIX)/lex.yy.c $(SRC)/token.h $(SRC)/list.h $(SRC)/symbol_table.h
	$(CC) -O3 -I$(SRC) -o$@ $<

$(BUILD_PREFIX)/lex.yy.c: $(SRC)/scanner.l
	mkdir -p -- $(BUILD_PREFIX)
	$(LEX) -o$@ $<

clean:
	rm -rf -- $(BUILD_PREFIX)