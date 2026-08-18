#ifndef USM_ERROR
#define USM_ERROR

#include <stdio.h>

#define UsmError(format, ...) do { \
    fprintf(stderr, "Error: " format "\n", ##__VA_ARGS__); \
    exit(1); \
} while(0)

#endif