CCFLAGS = -ansi -pedantic -pthread
DEBUG = -Wall -g
CC = gcc

all:
	${CC} ${CCFLAGS} -c StreamManager.c
	${CC} ${CCFLAGS} -c tqueue.c
	${CC} ${CCFLAGS} -o StreamManager StreamManager.o tqueue.o

debug:
	${CC} ${CCFLAGS} ${DEBUG} -c StreamManager.c
	${CC} ${CCFLAGS} ${DEBUG} -c tqueue.c
	${CC} ${CCFLAGS} ${DEBUG} -o StreamManager StreamManager.o tqueue.o

clean:
	rm -rf *o StreamManager

