SHELL=/bin/sh
CC=clang

SRCDIR=.
PREFIX=/usr/local
BINDIR=/bin

INSTALL_PROGRAM=install
INSTALL_DATA=install -m 644

CFLAGS=-Wall -Wextra -O3 -ggdb
BIN=myal

all: $(BIN)

myal: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(SRCDIR)/main.c -o myal

install:
	$(INSTALL_PROGRAM) myal $(PREFIX)$(BINDIR)/myal

clean:
	$(RM) myal

distclean: clean
	$(RM) /usr/local/bin/myal
