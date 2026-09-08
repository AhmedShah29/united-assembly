#include <stdio.h>
#include <stdlib.h>
#include "include/codegen.h"
#include "include/error.h"

void GenrateCode(const Instruction *instructions, const size_t instrCount, const char *outFilePath, targetOS targetArch) {
    FILE *outFile = fopen(outFilePath, "w");
    if(!outFile) { UsmError("cannot write file"); }

    codegen_x86_64(outFile, instructions, instrCount, targetArch);
    
   fclose(outFile); 
}