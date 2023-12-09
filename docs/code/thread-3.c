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
				bw = write(clientfd, response_header, \
					strlen(response_header));
				char filename[FILE_NAME_SIZE] = {'\0'};
				int method = get_file_name(req_buffer, \
					filename);
				if(bw <= 0)
				{
					close_clientfd(clientfd, tindex, \
						total_wrote);
					continue;
				}
				total_wrote += bw;