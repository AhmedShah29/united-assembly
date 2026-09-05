#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/error.h"
#include "include/utils.h"

int main(int argc, char *argv[]) {

    if (argc != 2) { UsmError("No file path provided to compile"); }
    const char *fileName = argv[1]; // stores the file name need to be a pointer 

    FILE *file = fopen(fileName, "r");
    if(!file) { UsmError("cannot open file '%s'\n", fileName); }

    fseek(file, 0, SEEK_END); // goes to the EOF

    long file_size = ftell(file); // stores the source file size 
    rewind(file); // sets the cursor back to the srats of the file

    char *buffer = malloc(file_size + 1);
    if(buffer == NULL) { UsmError("cant allocates memory to compile"); }

    fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';
    fclose(file); // closes the file

    size_t tokenCount = 0;
    token *tokens = lexer(buffer, &tokenCount);
    free(buffer);

    //lexer debug
    printf("--- USM Lexer Debug ---\n");
        for (size_t i = 0; i < tokenCount; i++) {
            printf("Line: %u | Type: %-12s | Value: '%s'\n" 
                ,tokens[i].line
                ,tokenTypeToString(tokens[i].type)              // I feel like a rust dev making this but its more readable 
                ,tokens[i].value
            );
        }

    // pasrser debug
    printf("\n--- USM Parser Debug ---\n");
        size_t instrCount = 0;
        Instruction *instructions = parser(tokens, tokenCount, &instrCount);
        
        for (size_t i = 0; i < instrCount; i++) {
             printf("Line: %d | Opcode: %-6s | DestType: %d | SrcType: %d\n",
                instructions[i].line,
                tokenTypeToString(instructions[i].opcode),
                instructions[i].dest.type,
                instructions[i].src.type
            );
         }
    
    // stops da mem leak during dev for now later will free da tokens after its parsered
    free(instructions);
    free(tokens);
    return 0;
}