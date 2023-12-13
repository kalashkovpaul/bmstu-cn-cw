#include "logger.h"

#define LOG_FILE "server.log"

struct log_qnode
{
	char *message;
	struct log_qnode *next;
};
typedef struct log_qnode log_qnode;

struct log_queue
{
	int size;
	int capacity;
	log_qnode *head;
	log_qnode *tail;
	pthread_mutex_t lock;
	pthread_cond_t cond_var;
};
typedef struct log_queue log_queue;

FILE *log_file = NULL;
log_queue *logqueue;
pthread_t* log_worker = NULL;

log_queue *create_logqueue(int capacity)
{
	log_queue *q = malloc(sizeof(log_queue));
	q->capacity = capacity;
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;

	if (pthread_mutex_init(&(q->lock), NULL) != 0)
	{
		perror("Error Initialising mutex lock\n");
		free(q);
		q = NULL;
		return NULL;
	}
	if (pthread_cond_init(&(q->cond_var), NULL) != 0)
	{
		pthread_mutex_destroy(&(q->lock));
		perror("Error Initialising conditional variable\n");
		free(q);
		q = NULL;
		return NULL;
	}
	return q;
}

int add_to_log(char *message)
{
    log_queue *q = logqueue;
	// printf("Queue size: %d, capacity: %d\n", q->size, q->capacity);
	if(q->size == q->capacity)
	{
		return -1;
	}
	log_qnode *node = malloc(sizeof(log_qnode));
	node->message = message;
	node->next = NULL;
	if(q->size == 0)
	{
		q->head = node;
		q->tail = node;
	}
	else if (q->tail)
	{
		q->tail->next = node;
		q->tail = node;
	}
	q->size++;
	return 1;
}

char* log_dequeue(log_queue *q)
{
	if(q->size == 0)
		return NULL;

    char *message = NULL;
	if(q->size == 1 && q->head)
	{
        message = q->head->message;
		free(q->head);
		q->head = NULL;
		q->tail = NULL;
	}
	else
	{
		log_qnode *oldhead = q->head;
		if (oldhead)
		{
			q->head = oldhead->next;
			message = oldhead->message;
			free(oldhead);
			oldhead = NULL;
		}
	}
	q->size--;
    return message;
}

void log_freequeue(log_queue *q)
{
	log_qnode *cursor = q->head, *temp;
	while(cursor != NULL)
	{
		temp = cursor;
		cursor = cursor->next;
		free(temp);
	}
	free(q);

	pthread_mutex_destroy(&(q->lock));
	pthread_cond_destroy(&(q->cond_var));
	return ;
}

void log_message_to_file(char *message)
{
	if (!log_file)
	{
		log_file = fopen(LOG_FILE, "a");
		if (!log_file)
		{
			printf("Error while opening file for log %s\n", LOG_FILE);
			return;
		}
	}
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	// printf("%s, ", message);
	// printf("%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	fprintf(log_file, "%s, ", message);
	fprintf(log_file, "%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	fclose(log_file);
	log_file = NULL;
}

void* log_message(void)
{
    while(1)
    {
        pthread_mutex_lock(&(logqueue->lock));
		char *message = log_dequeue(logqueue);
        if (message)
		{
            log_message_to_file(message);
        	free(message);
		}
		pthread_mutex_unlock(&(logqueue->lock));
    }
    return NULL;
}

void create_logger(void)
{
    pthread_t *worker = calloc(1, sizeof(pthread_t));
    logqueue = create_logqueue(1000);
    if(pthread_create(worker, NULL, (void * (*)(void *))log_message, NULL) < 0)
	{
        perror("Error in pthread_create()\n");
        free(worker);
        worker = NULL;
    }
    log_worker = worker;
}

void delete_logger(void)
{
    log_freequeue(logqueue);
    pthread_cancel(*log_worker);
}

