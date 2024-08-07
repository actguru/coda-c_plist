# PList Tester program.  08/03/2024

MACMIN  = 10.8

UNAME := $(shell uname)
CC    = gcc
OSCF  =

ifeq ($(UNAME), Darwin)
CC = clang
OSCF = -mmacosx-version-min=$(MACMIN)
endif

default: all

CFILES = pltester.c

OFILES = $(CFILES:.c=.o)

CFLAGS = $(OSCF) -fPIC -std=gnu17 -g0 -O1 -Wall -Wfatal-errors

TESTER = pltester

ARCHIVE = ../plist10gen.o

$(TESTER): $(ARCHIVE)
	$(CC) $(CFLAGS) $(CFILES) $(ARCHIVE) -o $(TESTER) -lm -I..

all: $(TESTER)
	./$(TESTER)

clean:
	rm -f $(OFILES) $(TESTER) $(TESTER).exe

.PHONY:  all clean

