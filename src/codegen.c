#include <stdio.h>
#include <stdlib.h>
#include "include/codegen.h"
#include "include/error.h"

void GenerateCode(const Instruction *instructions, const size_t instrCount, const char *outFilePath, TargetOS targetArch) {
    FILE *outFile = fopen(outFilePath, "w");
    if(!outFile) { UsmError("cannot write file"); }

    GenX86_64(outFile, instructions, instrCount, targetArch);
    
   fclose(outFile); 
}