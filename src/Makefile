CC = gcc
CFLAGS = -g -std=gnu99 -Wall -Werror -pedantic -Wfloat-conversion -Wvla -Wfloat-equal -Wpedantic -Wextra -Iinc -g3 -Wno-vla -Wno-unused-parameter

server: out/main.o out/logger.o out/thread_pool.o out/server.o
	$(CC) -o $@ $^ -pthread

out/server.o: src/server.c inc/server.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/thread_pool.o: src/thread_pool.c inc/thread_pool.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/logger.o: src/logger.c inc/logger.h
	$(CC) $(CFLAGS) -o $@ -c $<

out/main.o: src/main.c inc/server.h inc/thread_pool.h
	$(CC) $(CFLAGS) -o $@ -c $<

run: server
	./server

clean:
	rm out/*.o

