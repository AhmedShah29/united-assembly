#ifndef CODEGEN_H
#define CODEGEN_H

#include <stddef.h>
#include <stdio.h>
#include "include/parser.h"

typedef enum {
    OS_WINDOWS,
    OS_UNIX
} TargetOS;

void GenerateCode(const Instruction *instructions, const size_t instrCount, const char *outFilePath, TargetOS targetArch);

void GenX86_64(FILE *out, const Instruction *instrctions, size_t instrCount, TargetOS targetArch);


#endif