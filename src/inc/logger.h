#pragma once

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int add_to_log(char *message);

void create_logger(void);

void delete_logger(void);
