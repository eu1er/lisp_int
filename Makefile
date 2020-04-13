TARGET = 001
SRC    = 001.c
CFLAGS = -std=c99 -Wall
LDLIBS = -lm
CC     = cc

${TARGET}: ${SRC}
	${CC} $^ -o $@ ${CFLAGS} ${LDLIBS}
