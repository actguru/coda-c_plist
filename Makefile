# Apple, Linux, Windows (has install issues) # 08/06/2024

VERSION = 1.2
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
	ln -s -f $(ARCHIVE) $(THELIB).dylib
else
	$(CC) -shared -o $(ARCHIVE) -Wl,-soname,$(THELIB).so.$(MAJOR) $(OFILES) 
	ln -s -f $(ARCHIVE) $(THELIB).so
	ln -s -f $(ARCHIVE) $(THELIB).so.$(MAJOR)
endif

UTIL = codalist

$(UTIL): $(ARCHIVE)
	$(CC) $(CFLAGS) clist10gen.c -o $(UTIL) -L. $(ARCHIVE) -lm $(LDL)

MANFILE = codalist.1coda-c

# stand-alone program -- bad binary plugin
codalistdos:
	$(CC) $(CFLAGS) clist10gen.c plist10gen.c -o codalistdos -lm $(LDL)

all: $(ARCHIVE) $(UTIL)

test: codalistdos
	$(MAKE) -C tester
	sleep 3
	$(MAKE) -C batch

clean:
	rm -f $(OFILES) $(ARCHIVE) $(THELIB).dylib
	rm -f $(THELIB).so $(THELIB).so.$(MAJOR)
	rm -f $(UTIL) $(UTIL).o $(UTIL).exe codalistdos codalistdos.exe
	$(MAKE) -C tester clean
	$(MAKE) -C batch clean

.PHONY:  all clean test install codalistdos

HEADER = coda-c_plist.h coda-c_strings.h

INSTALL ?= cp -a

MKDIR   ?= mkdir -p

# lib or lib64 -- linux
ifeq ($(LIB),)
LIB := lib
endif

install: $(ARCHIVE) $(HEADER) $(UTIL) $(MANFILE)
	$(MKDIR)              $(DESTDIR)$(PREFIX)/$(LIB)/
	$(INSTALL) $(ARCHIVE) $(DESTDIR)$(PREFIX)/$(LIB)/
	$(MKDIR)              $(DESTDIR)$(PREFIX)/include/
	$(INSTALL) $(HEADER)  $(DESTDIR)$(PREFIX)/include/
	$(MKDIR)              $(DESTDIR)$(PREFIX)/bin/
	$(INSTALL) $(UTIL)    $(DESTDIR)$(PREFIX)/bin/
	$(MKDIR)              $(DESTDIR)$(PREFIX)/share/man/man1/
	$(INSTALL) $(MANFILE) $(DESTDIR)$(PREFIX)/share/man/man1/
ifeq ($(UNAME), Darwin)
	$(INSTALL) $(THELIB).dylib       $(DESTDIR)$(PREFIX)/$(LIB)/
else
	$(INSTALL) $(THELIB).so          $(DESTDIR)$(PREFIX)/$(LIB)/
	$(INSTALL) $(THELIB).so.$(MAJOR) $(DESTDIR)$(PREFIX)/$(LIB)/
	# sudo ldconfig
endif

