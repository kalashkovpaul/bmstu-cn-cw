#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include "logger.h"
#include "http.h"

#define REQUEST_BUFFER_SIZE 2048
#define FILE_BUFFER_SIZE 1024
#define FILE_NAME_SIZE 64

#define LISTEN_BACKLOG 32
#define MAX_CONNECTIONS 100
#define DEFAULT_PORT 8000
#define DEFAULT_POOLSIZE 8
#define DEFAULT_STATIC "html"

struct qnode
{
	int clientfd;
	struct qnode *next;
};
typedef struct qnode qnode;

struct queue
{
	int size;
	int capacity;
	qnode *head;
	qnode *tail;
	pthread_mutex_t lock;
	pthread_cond_t cond_var;
};
typedef struct queue queue;

extern char *html_dir;
extern int html_dir_len;
extern queue *connqueue;
extern unsigned int *bytes_read;
extern unsigned int *bytes_wrote;
extern fd_set client_fdset;
extern int threads;
extern int *client_sockfd;
extern int time_to_stop;

void* handle_connection(void *args);

void stop_server(int signal);

int parse_args(int argc, char *argv[], int *port, int *threads, char **html_dir);

int check_html_dir(void);

int create_listen_socket(int port);

queue *create_queue(int capacity);

int enqueue(queue *q, int clientfd);

void freequeue(queue *q);
