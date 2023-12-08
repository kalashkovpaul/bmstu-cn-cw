CC = gcc
CFLAGS = -Wall -lpthread

server: server.c
	${CC} ${CFLAGS} server.c logger.h logger.c

debug: server.c
	${CC} ${CFLAGS} -g server.c logger.h logger.c

run: server
	./server

clean:
	rm -rf server debug

