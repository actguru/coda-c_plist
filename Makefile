# Apple, Linux, Windows-7(has issues)
# 07/18/2024

UNAME := $(shell uname)
ARCMD = ar q
OSCF =
CC = gcc

ifeq ($(UNAME), Darwin)
$(info Make for APPLE, uname = $(UNAME))
CC = clang
ARCMD = libtool -static -o
OSCF = -mmacosx-version-min=10.8 -arch x86_64

else ifeq ($(UNAME), Linux)
$(info Make for LINUX, uname = $(UNAME))
ARCMD = ar q

else
$(info Make for Windows-64 and Others, uname = $(UNAME))
ARCMD = ar q

endif


CFILES = plist10gen.c

OFILES = $(CFILES:.c=.o)

CFLAGS = $(OSCF) -fPIC -std=gnu17 -g0 -O1 -Wall -Wfatal-errors

ARCHIVE = libcoda-c_plist.a

default: all

$(ARCHIVE): $(OFILES)
	$(ARCMD) $(ARCHIVE) $(OFILES) 

TESTER = pltester

$(TESTER): $(ARCHIVE)
	$(CC) -std=gnu17 -g0 -Wfatal-errors -Wall -O1 pltester.c $(ARCHIVE) -o $(TESTER) -I. -lm

runtest: $(TESTER)
	./pltester

all: $(ARCHIVE) $(TESTER) runtest

clean:
	rm -f $(OFILES) $(ARCHIVE) $(TESTER) $(TESTER).exe # DOS

rebuild: clean all

.PHONY:  all clean rebuild runtest install


# PREFIX is environment variable
ifeq ($(PREFIX),)
PREFIX := /usr/local
endif

HEADER = coda-c_plist.h coda-c_strings.h

install: $(ARCHIVE) $(HEADER)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(ARCHIVE) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(HEADER) $(DESTDIR)$(PREFIX)/include/

