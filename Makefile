.PHONY: all clean

CC ?= gcc
LEX ?= flex
BUILD ?= build
SRC ?= src

all: $(BUILD)/scanner

$(BUILD)/scanner: $(BUILD)/lex.yy.c $(SRC)/token.h
	$(CC) -O3 -I$(SRC) -o$@ $<

$(BUILD)/lex.yy.c: $(SRC)/scanner.l
	mkdir -p -- $(BUILD)
	$(LEX) -o$@ $<

clean:
	rm -rf -- $(BUILD)