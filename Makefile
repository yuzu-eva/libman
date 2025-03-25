SHELL=/bin/sh
CC=clang

SRCDIR=.
PREFIX=/usr/local
BINDIR=/bin

INSTALL_PROGRAM=install
INSTALL_DATA=install -m 644

CFLAGS=-Wall -Wextra -O3 -ggdb
LIBS=-lsqlite3
BIN=myal

all: $(BIN)

myal: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $(LIBS) $(SRCDIR)/main.c -o myal

install:
	$(INSTALL_PROGRAM) myal $(PREFIX)$(BINDIR)/myal

clean:
	$(RM) myal

uninstall:
	$(RM) /usr/local/bin/myal
