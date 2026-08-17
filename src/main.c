#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"

int main(int argc, char *argv[]) {

    if (argc != 2) { fprintf(stderr, "No file path provided to compile"); return 1; }
    const char *filename = argv[1]; // stores the file name need to be a pointer 

    FILE *file = fopen(filename, "r");
    if(!file) { fprintf(stderr, "Error: cannot open file '%s'\n", filename); return 1; }

    fseek(file, 0, SEEK_END); // goes to the EOF

    long file_size = ftell(file); // stores the source file size 
    rewind(file); // sets the cursor back to the srats of the file

    char *buffer = malloc(file_size + 1);
    if(buffer == NULL) { fprintf(stderr, "Error: cant allocates memory to compile"); return 1; }

    fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0';
    fclose(file); // closes the file

    size_t tokenCount = 0;
    token *tokens = lexer(buffer, &tokenCount);

    for (size_t i = 0; i < tokenCount; i++) {
        printf("Token [%02zu] | Type: %2d | Line: %d | Text: '%s'\n", i, tokens[i].type, tokens[i].line, tokens[i].orignalCodeLine);
    }

    free(tokens);
    free(buffer); // frees the memory 
    return 0;
}