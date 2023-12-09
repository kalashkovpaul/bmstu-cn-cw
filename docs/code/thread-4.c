				int flag = 0;
				while(method != 2 && (br = \
				read(htmlfd, filebuffer, FILE_BUFFER_SIZE-1)))
				{
					filebuffer[br] = '\0';
					bw = write(clientfd, filebuffer, br);
					if(bw <= 0)
					{
						close_clientfd(clientfd, tindex, \
							total_wrote);
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