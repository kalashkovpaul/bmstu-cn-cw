	unsigned int total_read = 0;
	for(i = 0; i < threads; i++)
	{
		printf("thread %u\twrote: %u\tread: %u\n"\,\
			i, bytes_wrote[i], bytes_read[i]);
		total_wrote += bytes_wrote[i];
		total_read += bytes_read[i];
	}
	printf("Total bytes received: %u Bytes\n Total bytes \
	sent: %u Bytes\n", total_read, total_wrote);

	return EXIT_SUCCESS;
}