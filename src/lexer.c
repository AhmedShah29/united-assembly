#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/error.h"

token* lexer(const char *source, size_t *outTokenCount) {
    
    const char *currentIndex = source;
    int currentLine = 1;
    size_t capacity = strlen(source) / 2 + 8; // max size of the tokens array 
    size_t tokenCount = 0;  // current tokens array size

    token *tokens = malloc(capacity * sizeof(token));
    if (tokens == NULL) { UsmError("faild to allocate memory for the tokens"); }
    
    while(*currentIndex != '\0') {
        if (*currentIndex == ' ' || *currentIndex == '\t' || *currentIndex == '\r'){ currentIndex++; } /* skips spaces etc... */ 
        else if(*currentIndex == '\n') { 

            if (tokenCount >= capacity) {
                capacity = (capacity == 0) ? 8 : capacity * 2;
                tokens = realloc(tokens, capacity * sizeof(token));
            }
            tokens[tokenCount].type = TOKEN_NLN;
            currentLine++; 
            currentIndex++; 
            tokenCount++;
        }
        else if(*currentIndex == ';') { 
            while (*currentIndex != '\n' && *currentIndex != '\0') { 
                currentIndex++; 
            } 
        }
        else {
            if(isalpha(*currentIndex)) {
                char word[32];
                int i = 0;

                while(isalnum(*currentIndex) && i < 31) {
                    word[i] = *currentIndex;
                    i++;
                    currentIndex++;
                }
                word[i] = '\0';

                if(tokenCount >= capacity) { capacity *= 2; tokens = realloc(tokens, capacity * sizeof(token)); }
                tokens[tokenCount].type = getTokenEnum(word);
                strncpy(tokens[tokenCount].value, word, sizeof(tokens[tokenCount].value) - 1);
                tokens[tokenCount].value[sizeof(tokens[tokenCount].value) - 1] = '\0'; // null terminador thing (not BMPT)
                tokens[tokenCount].line = currentLine;

                tokenCount++;
            } else if(*currentIndex == ',') {
                if(tokenCount >= capacity) { capacity *= 2; tokens = realloc(tokens, capacity * sizeof(token)); }

                tokens[tokenCount].type = TOKEN_COMMA;
                tokens[tokenCount].line = currentLine;

                tokenCount++;
                currentIndex++;
            } else { currentIndex++; } //temp 
        }
    }
    if (tokenCount >= capacity) { tokens = realloc(tokens, (tokenCount + 1) * sizeof(token)); }
    tokens[tokenCount].type = TOKEN_EOF;
    tokens[tokenCount].line = currentLine;
    tokenCount++;

    if(outTokenCount) *outTokenCount = tokenCount;
    return tokens;
}