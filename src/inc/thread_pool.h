#pragma once

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

pthread_t* create_threadpool(int poolsize, void *thread_func(void *));
