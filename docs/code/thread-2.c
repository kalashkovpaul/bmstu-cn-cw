		int total_wrote = 0;
		char *response_header;
		char filebuffer[FILE_BUFFER_SIZE] = {'\0'};
		char req_buffer[REQUEST_BUFFER_SIZE] = {'\0'};

		pthread_mutex_lock(&(connqueue->lock));
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

		req_status = handle_http_request(req_buffer, \
			&htmlfd, &response_header);
		switch(req_status)
		{
			case INCOMPLETE:
			case BADFILE:
			{
				close_clientfd(clientfd, tindex, total_wrote);
				continue;
			}