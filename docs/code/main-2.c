	create_logger();
	bytes_read = calloc(threads, sizeof(unsigned int));
	bytes_wrote = calloc(threads, sizeof(unsigned int));

	printf("Server running on port: %d\nthreads: \
        %d\nstatic directory: %s\nType Ctrl + C to exit\n",\
        port, threads, html_dir);

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
			break;
		FD_ZERO(&client_fdset);
		FD_SET(listenfd, &client_fdset);
		tv.tv_sec = 30;
		tv.tv_nsec = 0;
        ret = pselect(listenfd+1, &client_fdset, \
			NULL, NULL, &tv, NULL);
		if(ret < 0 && time_to_stop) break;
