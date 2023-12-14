						close(clientfd);
					pthread_mutex_unlock(&(connqueue->lock));
				}
			}
		}
	}
	close(listenfd);
	for(i = 0; i < threads; i++)
		pthread_cancel(workers[i]);
	freequeue(connqueue);
	delete_logger();
	return EXIT_SUCCESS;
}
