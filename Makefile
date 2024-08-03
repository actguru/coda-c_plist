# Apple, Linux, Windows (has install issues) # 07/26/2024

VERSION = 1.1
COPYYEARS = 2024
MAJOR   = 1
COMPAT  = 1.0

MACMIN  = 10.8

UNAME := $(shell uname)
CC    = gcc
ARCMD = ar q
OSCF  =
LIBSUF= so.$(VERSION)
LDL   =

ifeq ($(UNAME), Darwin)
$(info Make for APPLE, uname = $(UNAME))
CC = clang
ARCMD = libtool -static -o
OSCF = -mmacosx-version-min=$(MACMIN)
LIBSUF= $(MAJOR).dylib
ifeq ($(PREFIX),)
PREFIX := /usr/local
endif
else ifeq ($(UNAME), Linux)
$(info Make for LINUX, uname = $(UNAME))
LDL = -ldl
endif

ifeq ($(PREFIX),)
PREFIX := /usr
endif

$(info Library: libcoda-c_plist Verson $(VERSION), Major $(MAJOR), Compatible $(COMPAT), PREFIX $(PREFIX))

default: all

CFILES = plist10gen.c

OFILES = $(CFILES:.c=.o)

CFLAGS = $(OSCF) -fPIC -std=gnu17 -g0 -O1 -Wall -Wfatal-errors \
	-DQVersion=\"$(VERSION)\" -DQCopyYears=\"$(COPYYEARS)\"

THELIB = libcoda-c_plist

ARCHIVE = $(THELIB).$(LIBSUF)

$(ARCHIVE): $(OFILES)
ifeq ($(UNAME), Darwin)
	clang -dynamiclib -compatibility_version $(COMPAT) -current_version $(VERSION) \
	-install_name $(DESTDIR)$(PREFIX)/lib/$(ARCHIVE) -headerpad_max_install_names \
	-o $(ARCHIVE) $(OSCF) $(OFILES) 
else
	$(CC) -shared -o $(ARCHIVE) -Wl,-soname,$(THELIB).so.$(MAJOR) $(OFILES) 
endif

UTIL = codalist

$(UTIL): $(ARCHIVE)
	$(CC) $(CFLAGS) clist10gen.c -o $(UTIL) -L. $(ARCHIVE) -lm $(LDL)

MANFILE = codalist.1coda-c

codalistdos: # stand-alone program -- bad binary plugin
	$(CC) $(CFLAGS) clist10gen.c plist10gen.c -o codalistdos -lm $(LDL)

all: $(ARCHIVE) $(UTIL)

test:
	$(MAKE) -C tester
	sleep 3
	$(MAKE) -C batch

clean:
	rm -f $(OFILES) $(ARCHIVE)
	rm -f $(UTIL) $(UTIL).o $(UTIL).exe codalistdos codalistdos.exe
	$(MAKE) -C tester clean
	$(MAKE) -C batch clean

.PHONY:  all clean test install codalistdos

HEADER = coda-c_plist.h coda-c_strings.h

install: $(ARCHIVE) $(HEADER) $(UTIL) $(MANFILE)
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(ARCHIVE) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 $(HEADER) $(DESTDIR)$(PREFIX)/include/
ifeq ($(UNAME), Darwin)
	ln -s -f $(ARCHIVE) $(DESTDIR)$(PREFIX)/lib/$(THELIB).dylib
else
	ln -s -f $(ARCHIVE) $(DESTDIR)$(PREFIX)/lib/$(THELIB).so
	ln -s -f $(ARCHIVE) $(DESTDIR)$(PREFIX)/lib/$(THELIB).so.$(MAJOR)
	# sudo ldconfig
endif
	install -d $(DESTDIR)$(PREFIX)/bin/
	install -m 755 $(UTIL) $(DESTDIR)$(PREFIX)/bin/
	install -d $(DESTDIR)$(PREFIX)/share/man/man1/
	install -m 644 $(MANFILE) $(DESTDIR)$(PREFIX)/share/man/man1/

