#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/error.h"
#include "include/utils.h"
#include "include/codegen.h"

int main(int argc, char *argv[]) {

    if (argc != 2) { UsmError("No file path provided to compile"); }
    const char *fileName = argv[1]; // stores the file name need to be a pointer 

    FILE *file = fopen(fileName, "r");
    if(!file) { UsmError("cannot open file '%s'\n", fileName); }

    fseek(file, 0, SEEK_END); // goes to the EOF

    long fileSize = ftell(file); // stores the source file size 
    rewind(file); // sets the cursor back to the start of the file

    char *buffer = malloc(fileSize + 1);
    if(buffer == NULL) { UsmError("cant allocates memory to compile"); }

    fread(buffer, sizeof(char), fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file); // closes the file

    size_t tokenCount = 0;
    Token *tokens = Lexer(buffer, &tokenCount);
    free(buffer);

    size_t instrCount = 0;
    Instruction *instructions = Parser(tokens, tokenCount, &instrCount);
    debug_lexer_parser(instructions, instrCount, tokens, tokenCount);
    free(tokens);

    GenerateCode(instructions, instrCount, "output.asm", OS_WINDOWS);
    
    // stops the memory leak during dev for now later will free da tokens after its parsed
    free(instructions);
    return 0;
}