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
#include <pthread.h>
#include "logger.h"

#define REQUEST_BUFFER_SIZE 2048
#define FILE_BUFFER_SIZE 1024
#define FILE_NAME_SIZE 64

#define LISTEN_BACKLOG 32
#define MAX_CONNECTIONS 100
#define DEFAULT_PORT 8000
#define DEFAULT_POOLSIZE 8
#define DEFAULT_STATIC "html"

#define HELPSTRING "\nStatic HTTP server\n"\
	"Usage: %s [-p port number] [-h html directory] [-t thread pool size]\n"\
	"\n[Optional arguments]\n"\
	"\t-p\tServer port (Default 8000)\n"\
	"\t-t\tThread pool size. Number of threads for the server to use. (Default 8)\n"\
	"\t-s\tHTML files in this directory are served. This directory must be in the same directory\n"\
	"\t\tas the server binary and don't add './' to the directory name! (Default 'html')\n"\
	"\t-h\tShows available arguments\n"

#define HTTP_BASE_OK "HTTP/1.1 200 OK\r\n"\
	"Server: Single File Server\r\n"\
	"Connection: Closed\r\n"\
	"Content-Type: "

#define HTTP_404 "HTTP/1.1 404 Not Found\r\n"\
	"Server: Single File Server\r\n"\
	"Content-Type: text/html; charset=utf-8\r\n"\
	"Connection: Closed\r\n\r\nPAGE NOT FOUND"

#define HTTP_405 "HTTP/1.1 405 Method Not Allowed\r\n"\
	"Server: Single File Server\r\n"\
	"Content-Type: text/html; charset=utf-8\r\n"\
	"Connection: Closed\r\n\r\nONLY GET AND HEAD ARE ACCEPTED"

#define HTTP_BASE_OK_len strlen(HTTP_BASE_OK)
#define HTTP_404_len strlen(HTTP_404)
#define HTTP_405_len strlen(HTTP_405)


enum filetype {
	HTML,
	CSS,
	JS,
	JPG,
	JPEG,
	SWF,
	GIF,
	PNG,
	TXT,
	SVG,
	UNKNOWN
};

enum http_status {
	OK,
	INCOMPLETE,
	NOTALLOWED,
	BADFILE,
	NOTFOUND
};

struct qnode {
	int clientfd;
	struct qnode *next;
};
typedef struct qnode qnode;

struct queue {
	int size;
	int capacity;
	qnode *head;
	qnode *tail;
	pthread_mutex_t lock;
	pthread_cond_t cond_var;
};
typedef struct queue queue;

char *html_dir;
int html_dir_len = 0;
queue *connqueue;
unsigned int *bytes_read = NULL;
unsigned int *bytes_wrote = NULL;
fd_set client_fdset;
int threads = DEFAULT_POOLSIZE;
int *client_sockfd = NULL;
int time_to_stop = 0;


queue *create_queue(int capacity)
{
	queue *q = malloc(sizeof(queue));
	q->capacity = capacity;
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;

	if (pthread_mutex_init(&(q->lock), NULL) != 0) {
		perror("Error Initialising mutex lock\n");
		free(q);
		q = NULL;
		return NULL;
	}
	if (pthread_cond_init(&(q->cond_var), NULL) != 0){
		pthread_mutex_destroy(&(q->lock));
		perror("Error Initialising conditional variable\n");
		free(q);
		q = NULL;
		return NULL;
	}
	return q;
}

int enqueue(queue *q, int clientfd)
{
	// printf("Queue size: %d, capacity: %d\n", q->size, q->capacity);
	if(q->size == q->capacity) {
		return -1;
	}
	qnode *node = malloc(sizeof(qnode));
	node->clientfd = clientfd;
	node->next = NULL;
	if(q->size == 0) {
		q->head = node;
		q->tail = node;
	} else {
		q->tail->next = node;
		q->tail = node;
	}
	q->size++;
	return 1;
}

int dequeue(queue *q)
{
	if(q->size == 0)
		return -1;

	int retval = q->head->clientfd;
	if(q->size == 1) {
		/* At this point, tail and head point to the same node, so don't free both. This was the cause of
		 * the occasional segfault */
		free(q->head);
		q->head = NULL;
		q->tail = NULL;
	} else {
		qnode *oldhead = q->head;
		q->head = oldhead->next;
		free(oldhead);
		oldhead = NULL;
	}
	q->size--;
	return retval;
}

void freequeue(queue *q)
{
	qnode *cursor = q->head, *temp;
	while(cursor != NULL) {
		temp = cursor;
		cursor = cursor->next;
		free(temp);
	}
	free(q);

	/* Destroy lock and cond_var */
	pthread_mutex_destroy(&(q->lock));
	pthread_cond_destroy(&(q->cond_var));
	return ;
}

int parse_args(int argc, char *argv[], int *port, int *threads, char **html_dir)
{
	int option;
	while ((option = getopt(argc, argv, "p:t:s:h")) != -1) {
		switch(option) {
			case 'p':
				*port = atoi(optarg);
				break;
			case 't':
				*threads = atoi(optarg);
				break;
			case 's':
				*html_dir = optarg;
				break;
			case 'h':
				printf(HELPSTRING, argv[0]);
				return -1;
			default:
				printf("Invalid usage. Try -h for help\n");
				return -1;
		}
	}
	return 1;
}

int check_html_dir()
{
	/* Make sure that root (/) or home (~) is not accessed. */
	if(html_dir[0] == '/' || html_dir[0] == '~') {
		printf("%s is invalid. Make sure that your static directory is present in the current directory\n", html_dir);
		return 0;
	}

	struct stat s;
	int res = stat(html_dir, &s);
	if(res == -1) {
		if(errno == ENOENT) {
			printf("%s does not exist\n", html_dir);
		} else {
			perror("stat error\n");
		}
	} else {
		if (S_ISDIR(s.st_mode)) {
			return 1;
		} else {
			printf("%s is not a directory\n", html_dir);
		}
	}
	return 0;
}

int create_listen_socket(int port)
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		perror("Failed to create listen socket\n");
		return -1;
	}
	int enable = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		perror("setsockopt(SO_REUSEADDR) failed");
		return -1;
	}
	struct sockaddr_in listen_addr;
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(port);
	listen_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(fd, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
		perror("Failed to bind listen socket\n");
		return -1;
	}
	if(listen(fd, LISTEN_BACKLOG) < 0) {
		perror("listen() error on socket\n");
		return -1;
	}
	return fd;
}

int get_file_name(char *request, char *filename)
{
	char method[8] = {'\0'}, version[8] = {'\0'};
	sscanf(request, "%7s %s %7s\r\n", method, filename, version);
	char *message_to_log = calloc(FILE_BUFFER_SIZE, sizeof(char));
	snprintf(message_to_log, FILE_BUFFER_SIZE, "%s %s %s", method, filename, version);
	add_to_log(message_to_log);
	if (strcmp(method, "GET") == 0)
		return 1;
	if (strcmp(method, "HEAD") == 0)
		return 2;
	return 0;
}

enum filetype get_filetype(char *filename)
{
	/* Find the last '.' in filename */
	int i, len, found_dot = 0;
	len = strlen(filename);
	for(i = len - 1; i > -1; i--) {
		if (filename[i] == '.') {
			found_dot = 1;
			break;
		}
	}
	if (!found_dot || i == 0 || i == len -1)
		return UNKNOWN;
	char *ext = filename + i + 1;
	fflush(stdout);
	if(strcmp(ext, "html") == 0)
		return HTML;
	else if(strcmp(ext, "css") == 0)
		return CSS;
	else if (strcmp(ext, "js") == 0)
		return JS;
	else if(strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0)
		return JPG;
	else if(strcmp(ext, "png") == 0)
		return PNG;
	else if (strcmp(ext, "swf") == 0)
		return SWF;
	else if (strcmp(ext, "gif") == 0)
		return GIF;
	else if(strcmp(ext, "svg") == 0)
		return SVG;
	else if (strcmp(ext, "txt") == 0)
		return TXT;
	else
		return UNKNOWN;
}

char *get_response_header(char *filename)
{
	enum filetype ft = get_filetype(filename);
	char *response_header = calloc(1, HTTP_BASE_OK_len + 32);
	switch(ft) {
		case HTML:
			sprintf(response_header, "%stext/html; charset=utf-8\r\n\r\n", HTTP_BASE_OK);
			break;
		case CSS:
			sprintf(response_header, "%stext/css; charset=utf-8\r\n\r\n", HTTP_BASE_OK);
			break;
		case JS:
			sprintf(response_header, "%stext/javascript; charset=utf-8\r\n\r\n", HTTP_BASE_OK);
			break;
		case SWF:
			sprintf(response_header, "%sapplication/x-shockwave-flash; charset=utf-8\r\n\r\n", HTTP_BASE_OK);
			break;
		case GIF:
			sprintf(response_header, "%simage/gif; charset=utf-8\r\n\r\n", HTTP_BASE_OK);
			break;
		case TXT:
			sprintf(response_header, "%stext/plain; charset=utf-8\r\n\r\n", HTTP_BASE_OK);
			break;
		case JPG:
			sprintf(response_header, "%simage/jpeg\r\n\r\n", HTTP_BASE_OK);
			break;
		case PNG:
			sprintf(response_header, "%simage/png\r\n\r\n", HTTP_BASE_OK);
			break;
		case SVG:
			sprintf(response_header, "%simage/svg+xml\r\n\r\n", HTTP_BASE_OK);
			break;
		default:
			sprintf(response_header, "%sapplication/octet-stream\r\n\r\n", HTTP_BASE_OK);
			break;
	}
	return response_header;
}

enum http_status handle_http_request(char *request, int *htmlfd, char **response_header)
{
	char filename[FILE_NAME_SIZE] = {'\0'};
	char fullpath[FILE_NAME_SIZE + html_dir_len + 4];
	memset(fullpath, 0, FILE_NAME_SIZE + html_dir_len + 4);
	int fd = -1;

	*htmlfd = -1;
	*response_header = NULL;

	if (strstr(request, "\r\n\r\n") == NULL)
		return INCOMPLETE;

	int method = get_file_name(request, filename);
	if(method == 0){
		return NOTALLOWED;
	}
	if (filename[0] == '.' || filename[0] == '~' || strstr(filename, "..") != NULL) {
		return BADFILE;
	}

	if(strcmp(filename, "/") == 0)
		sprintf(filename, "/index.html");

	sprintf(fullpath, "./%s%s", html_dir, filename);
	if ((fd = open(fullpath, O_RDONLY)) < 0) {
		return NOTFOUND;
	} else {
		*htmlfd = fd;
		*response_header = get_response_header(filename);
		return OK;
	}
}

void close_clientfd(int clientfd, int index, int total)
{
	bytes_wrote[index] += total;
	close(clientfd);
}

void* handle_connection(void *args)
{
	int tindex = *((int*)args);
	while(1) {
		int clientfd = -1;
		int htmlfd = -1;

		enum http_status req_status;
		int br,bw;
		int total_wrote = 0;

		char *response_header;
		char filebuffer[FILE_BUFFER_SIZE] = {'\0'};
		char req_buffer[REQUEST_BUFFER_SIZE] = {'\0'};

		pthread_mutex_lock(&(connqueue->lock));
		// pthread_cond_wait(&(connqueue->cond_var), &(connqueue->lock));
		clientfd = dequeue(connqueue);
		pthread_mutex_unlock(&(connqueue->lock));

		if(clientfd == -1)
			continue;

		br = read(clientfd, req_buffer, REQUEST_BUFFER_SIZE-1);
		if(br <= 0)
		{
			close_clientfd(clientfd, tindex, total_wrote);
			continue;
		}
		req_buffer[br] = '\0';
		bytes_read[tindex] += br;

		req_status = handle_http_request(req_buffer, &htmlfd, &response_header);
		switch(req_status){
			case INCOMPLETE:
			case BADFILE:
				{
					close_clientfd(clientfd, tindex, total_wrote);
					continue;
				}
			case NOTALLOWED:
				{
					bw = write(clientfd, HTTP_405, HTTP_405_len);
					if(bw > 0)
						total_wrote += bw;
					close_clientfd(clientfd, tindex, total_wrote);
					continue;
				}
			case NOTFOUND:
				{
					bw = write(clientfd, HTTP_404, HTTP_404_len);
					if(bw > 0)
						total_wrote += bw;
					close_clientfd(clientfd, tindex, total_wrote);
					continue;
				}
			case OK:
				{
					bw = write(clientfd, response_header, strlen(response_header));
					char filename[FILE_NAME_SIZE] = {'\0'};
					int method = get_file_name(req_buffer, filename);
					if(bw <= 0)
					{
						close_clientfd(clientfd, tindex, total_wrote);
						continue;
					}
					total_wrote += bw;
					int flag = 0;
					while(method != 2 && (br = read(htmlfd, filebuffer, FILE_BUFFER_SIZE-1))) {
						filebuffer[br] = '\0';
						bw = write(clientfd, filebuffer, br);
						if(bw <= 0)
						{
							close_clientfd(clientfd, tindex, total_wrote);
							flag = 1;
							break;
						}
						total_wrote += bw;
						memset(filebuffer, 0, bw);
					}
					close(htmlfd);
					if (flag)
						break;
				}
		}
		close_clientfd(clientfd, tindex, total_wrote);
	}
	return NULL;
}

pthread_t* create_threadpool(int poolsize, void *thread_func)
{
	pthread_t *workers = calloc(poolsize, sizeof(pthread_t));
	int i;
	int *arg;
	for(i = 0; i < poolsize; i++) {
		arg = malloc(sizeof(int));
		*arg = i;
		if(pthread_create(&workers[i], NULL, thread_func, (void *)arg) < 0){
			perror("Error in pthread_create()\n");
			free(workers);
			return NULL;
		}
	}
	return workers;
}

void stop_server(int signum)
{
	time_to_stop = 1;
}

int main(int argc, char *argv[])
{
	int port = DEFAULT_PORT;
	html_dir = DEFAULT_STATIC;

	if(parse_args(argc, argv, &port, &threads, &html_dir) == -1)
		return EXIT_SUCCESS;

	if(!check_html_dir())
		exit(EXIT_FAILURE);
	html_dir_len = strlen(html_dir);

	client_sockfd = calloc(threads, sizeof(int));
	int listenfd;
	if((listenfd = create_listen_socket(port)) < 0)
		exit(EXIT_FAILURE);

	if((connqueue = create_queue(MAX_CONNECTIONS)) == NULL){
		close(listenfd);
		exit(EXIT_FAILURE);
	}

	pthread_t *workers;
	if((workers = create_threadpool(threads, handle_connection)) == NULL) {
		close(listenfd);
		pthread_mutex_destroy(&(connqueue->lock));
		pthread_cond_destroy(&(connqueue->cond_var));
		exit(EXIT_FAILURE);
	}
	create_logger();

	bytes_read = calloc(threads, sizeof(unsigned int));
	bytes_wrote = calloc(threads, sizeof(unsigned int));

	printf("Server running on port: %d\nthreads: %d\nstatic directory: %s\nType Ctrl + C to exit\n", port, threads, html_dir);

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, stop_server);

	int clientfd = -1;
	struct sockaddr_in client_addr;
	socklen_t len;
	int i;
	struct timespec tv;
	bzero((void*)client_sockfd, sizeof(client_sockfd));
	int ret = 0;
	while (1)
	{
		FD_ZERO(&client_fdset);
		FD_SET(listenfd, &client_fdset);
		tv.tv_sec = 30;
		tv.tv_nsec = 0;

		ret = pselect(listenfd+1, &client_fdset, NULL, NULL, &tv, NULL);
		if(ret < 0) {
			printf("select error: %s\n", strerror(errno));
			// break;
		}
		else if(ret == 0) {
			// printf("timeout\n");
			// continue;
		}

		if(FD_ISSET(listenfd, &client_fdset))
		{
			memset(&client_addr, 0, sizeof(client_addr));
			len = sizeof(client_addr);
			clientfd = accept(listenfd, (struct sockaddr*)&client_addr, &len);
			if(clientfd < 0) {
				perror("accept error!\n");
				continue;
			}
			pthread_mutex_lock(&(connqueue->lock));
			if(enqueue(connqueue, clientfd) < 0){
				printf("Connection capacity reached. Dropped new connection!\n");
				close(clientfd);
			} else {
				pthread_cond_signal(&(connqueue->cond_var));
			}
			pthread_mutex_unlock(&(connqueue->lock));
		}
		if (time_to_stop)
		{
			break;
		}
	}

	printf("Closing socket...\n");
	close(listenfd);

	printf("stopping server\n");
	for(i = 0; i < threads; i++) {
		pthread_cancel(workers[i]);
	}
	freequeue(connqueue);
	delete_logger();

	unsigned int total_wrote = 0;
	unsigned int total_read = 0;
	for(i = 0; i < threads; i++) {
		printf("thread %u\twrote: %u\tread: %u\n", i, bytes_wrote[i], bytes_read[i]);
		total_wrote += bytes_wrote[i];
		total_read += bytes_read[i];
	}
	printf("Total bytes received: %u Bytes\nTotal bytes sent: %u Bytes\n", total_read, total_wrote);

	return EXIT_SUCCESS;
}


