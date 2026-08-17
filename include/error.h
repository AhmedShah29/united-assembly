#ifndef USM_ERROR
#define USM_ERROR

#include <stdio.h>

#define UsmError(format, ...) fprintf(stderr, "Error: " format "\n", #__VA_ARGS__)

#endif