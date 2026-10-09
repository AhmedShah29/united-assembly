#ifndef USM_ERROR
#define USM_ERROR

#include <stdio.h>
#define RED_BOLD "\033[1;31m"
#define RESET "\033[0m"

#define UsmError(format, ...) do { \
    fprintf(stderr, RED_BOLD "Error: " RESET format "\n", ##__VA_ARGS__); \
    exit(1); \
} while(0)

#endif