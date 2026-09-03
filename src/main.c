#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
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

    printf("--- USM Lexer Debug ---\n");
        for (size_t i = 0; i < tokenCount; i++) {
            printf("Line: %u | Type: %-12s | Value: '%s'\n" 
                ,tokens[i].line
                ,tokenTypeToString(tokens[i].type)              // I feel like a rust dev making this but its more readable 
                ,tokens[i].value
            );
        }
    
    // stops da mem leak during dev for now later will free da tokens after its parsered
    free(tokens);
    return 0;
}