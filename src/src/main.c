#include "server.h"
#include "thread_pool.h"

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

	if((connqueue = create_queue(MAX_CONNECTIONS)) == NULL)
    {
		close(listenfd);
		exit(EXIT_FAILURE);
	}

	pthread_t *workers;
	if((workers = create_threadpool(threads, (void *)handle_connection)) == NULL)
    {
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
        if (time_to_stop)
		{
			break;
		}
		FD_ZERO(&client_fdset);
		FD_SET(listenfd, &client_fdset);
		tv.tv_sec = 30;
		tv.tv_nsec = 0;

		ret = pselect(listenfd+1, &client_fdset, NULL, NULL, &tv, NULL);
		if(ret < 0)
        {
			if (time_to_stop) break;
		}

		if(FD_ISSET(listenfd, &client_fdset))
		{
			memset(&client_addr, 0, sizeof(client_addr));
			len = sizeof(client_addr);
			clientfd = accept(listenfd, (struct sockaddr*)&client_addr, &len);
			if(clientfd < 0)
            {
				perror("accept error!\n");
				continue;
			}
			pthread_mutex_lock(&(connqueue->lock));
			if(enqueue(connqueue, clientfd) < 0)
            {
				printf("Connection capacity reached. Dropped new connection!\n");
				close(clientfd);
			} else
            {
				pthread_cond_signal(&(connqueue->cond_var));
			}
			pthread_mutex_unlock(&(connqueue->lock));
		}
	}

	printf("Closing socket...\n");
	close(listenfd);

	printf("stopping server\n");
	for(i = 0; i < threads; i++)
    {
		pthread_cancel(workers[i]);
	}
	freequeue(connqueue);
	delete_logger();

	unsigned int total_wrote = 0;
	unsigned int total_read = 0;
	for(i = 0; i < threads; i++)
    {
		printf("thread %u\twrote: %u\tread: %u\n", i, bytes_wrote[i], bytes_read[i]);
		total_wrote += bytes_wrote[i];
		total_read += bytes_read[i];
	}
	printf("Total bytes received: %u Bytes\nTotal bytes sent: %u Bytes\n", total_read, total_wrote);

	return EXIT_SUCCESS;
}

