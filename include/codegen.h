#ifndef CODEGEN_H
#define CODEGEN_H

#include <stddef.h>
#include <stdio.h>
#include "include/parser.h"

typedef enum {
    OS_WINDOWS,
    OS_UNIX
} targetOS;

void GenrateCode(const Instruction *instructions, const size_t instrCount, const char *outFilePath, targetOS targetArch);

void codegen_x86_64(FILE *out, const Instruction *instrctions, size_t instrCount, targetOS targetArch);


#endif