CCFLAGS = -ansi -pedantic
DEBUG = -Wall -Werror -g
CC = gcc

all: debug

production:
	${CC} ${CCFLAGS} -c StreamManager.c
	${CC} ${CCFLAGS} -o StreamManager StreamManager.o

debug:
	${CC} ${CCFLAGS} ${DEBUG} -c StreamManager.c
	${CC} ${CCFLAGS} ${DEBUG} -o StreamManager StreamManager.o

clean:
	rm -rf *o StreamManager

