SHELL = /bin/sh
CC = gcc

PREFIX = /usr/local
BINDIR = /bin

INSTALL_PROGRAM = install
INSTALL_DATA = install -m 644

CFLAGS = -Wall -Wextra
LIBS = -lsqlite3
SRC = main.c dbhandling.c
OBJ = ${SRC:.c=.o}
BIN = myal

all: options ${BIN}

options:
	@echo myal build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "CC     = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} ${SRC}

myal: .c.o
	${CC} ${CFLAGS} ${LIBS} ${OBJ} -o myal

install:
	${INSTALL_PROGRAM} myal ${PREFIX}${BINDIR}/myal

clean:
	${RM} myal
	${RM} ${OBJ}

uninstall:
	${RM} /usr/local/bin/myal
