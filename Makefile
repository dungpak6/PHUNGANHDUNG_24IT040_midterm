CC=cc
CFLAGS=-O2 -Wall -Wextra -Wpedantic -std=c11 -D_POSIX_C_SOURCE=200809L -D_NETBSD_SOURCE -Iinclude
SRCS=src/main.c src/options.c src/entries.c src/sort.c src/directory.c src/display.c
OBJS=${SRCS:.c=.o}
TARGET=myls

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${OBJS} -o ${TARGET}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f ${OBJS} ${TARGET}

test: ${TARGET}
	sh tests/test.sh

# Installation configuration
PREFIX?=/usr/local
BINDIR=${PREFIX}/bin

# Install MYLS as a system command
install: ${TARGET}
	install -d ${DESTDIR}${BINDIR}
	install -m 755 ${TARGET} ${DESTDIR}${BINDIR}/myls

# Remove installed MYLS
uninstall:
	rm -f ${DESTDIR}${BINDIR}/myls
