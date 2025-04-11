SHELL=/bin/sh
CC=gcc

SRCDIR=./src
PREFIX=/usr/local
BINDIR=/bin

INSTALL_PROGRAM=install
INSTALL_DATA=install -m 644

CFLAGS=-Wall -Wextra
LIBS=-lsqlite3
BIN=myal

all: $(BIN)

myal: $(SRCDIR)/main.c dbhandling
	$(CC) $(CFLAGS) $(LIBS) $(SRCDIR)/main.c dbhandling.o -o myal

dbhandling: $(SRCDIR)/dbhandling.c
	$(CC) $(CFLAGS) $(LIBS) -c $(SRCDIR)/dbhandling.c

install:
	$(INSTALL_PROGRAM) myal $(PREFIX)$(BINDIR)/myal

clean:
	$(RM) myal
	$(RM) dbhandling.o

uninstall:
	$(RM) /usr/local/bin/myal
