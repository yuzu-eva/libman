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
BIN = libman

all: options ${BIN}

options:
	@echo ${BIN} build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "CC     = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} ${SRC}

libman: .c.o
	${CC} ${CFLAGS} ${LIBS} ${OBJ} -o ${BIN}

install:
	${INSTALL_PROGRAM} ${BIN} ${PREFIX}${BINDIR}/${BIN}

clean:
	${RM} ${BIN}
	${RM} ${OBJ}

uninstall:
	${RM} ${PREFIX}/${BINDIR}/${BIN}
