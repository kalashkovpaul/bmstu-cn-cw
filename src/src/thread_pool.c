#include "thread_pool.h"

pthread_t* create_threadpool(int poolsize, void *thread_func(void *))
{
	pthread_t *workers = calloc(poolsize, sizeof(pthread_t));
	int i;
	int *arg;
	for(i = 0; i < poolsize; i++)
	{
		arg = malloc(sizeof(int));
		*arg = i;
		if(pthread_create(&workers[i], NULL, thread_func, (void *)arg) < 0)
		{
			perror("Error in pthread_create()\n");
			free(workers);
			return NULL;
		}
	}
	return workers;
}
