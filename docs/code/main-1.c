int main(int argc, char *argv[])
{
	int port = DEFAULT_PORT;
	html_dir = DEFAULT_STATIC;

	if(parse_args(argc, argv, &port, \
        &threads, &html_dir) == -1)
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
	if((workers = create_threadpool(threads, \
        (void *)handle_connection)) == NULL)
	{
		close(listenfd);
		pthread_mutex_destroy(&(connqueue->lock));
		pthread_cond_destroy(&(connqueue->cond_var));
		exit(EXIT_FAILURE);
	}