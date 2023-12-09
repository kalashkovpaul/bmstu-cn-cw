		if(FD_ISSET(listenfd, &client_fdset))
		{
			memset(&client_addr, 0, sizeof(client_addr));
			len = sizeof(client_addr);
			clientfd = accept(listenfd, \
                (struct sockaddr*)&client_addr, &len);
			if(clientfd < 0)
			{
				perror("accept error!\n");
				continue;
			}
			pthread_mutex_lock(&(connqueue->lock));
			if(enqueue(connqueue, clientfd) < 0)
			{
				printf("Connection capacity \
                    reached. Dropped new connection!\n");
				close(clientfd);
			} else
				pthread_cond_signal(&(connqueue->cond_var));
			pthread_mutex_unlock(&(connqueue->lock));
		}
	}
	printf("Closing socket...\n");
	close(listenfd);
	printf("Stopping server\n");
	for(i = 0; i < threads; i++)
		pthread_cancel(workers[i]);
	freequeue(connqueue);
	delete_logger();
	unsigned int total_wrote = 0;