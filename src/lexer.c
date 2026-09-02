#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/error.h"
#include "include/utils.h"

token* lexer(const char *source, size_t *outTokenCount) {

    const char *currentIndex = source;
    int currentLine = 1; // file lines claculated thru the '\n'
    size_t capacity = strlen(source) / 2 + 8; // max size of the tokens array
    size_t tokenCount = 0;  // current tokens array size

    token *tokens = malloc(capacity * sizeof(token));
    CheckMem(tokens, tokenCount, capacity, "Failed to allocate memory tokens array");

    while(*currentIndex != '\0') {
        if (*currentIndex == ' ' || *currentIndex == '\t' || *currentIndex == '\r'){ currentIndex++; } /* skips spaces etc... */
        else if(*currentIndex == '\n') {

            CheckMem(tokens, tokenCount, capacity, "Failed to reallocate memory for tokens array");

            tokens[tokenCount].type = TOKEN_NLN;
            tokens[tokenCount].value[0] = '\0';
            tokens[tokenCount].line = currentLine;
            
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
            if(isalpha(*currentIndex) || *currentIndex == '_') {
                char word[32];
                int i = 0;

                while((isalnum(*currentIndex) || *currentIndex == '_') && i < 31) {  // pravents the size overflaw
                    word[i] = *currentIndex;
                    i++;
                    currentIndex++;
                }
                word[i] = '\0';

                if(*currentIndex == ':') { tokens[tokenCount].type = TOKEN_LABEL; currentIndex++; }
                else { tokens[tokenCount].type = getTokenEnum(word); }

                CheckMem(tokens, tokenCount, capacity, "Failed to reallocate memory for tokens array");


                strncpy(tokens[tokenCount].value, word, sizeof(tokens[tokenCount].value) - 1);
                tokens[tokenCount].value[sizeof(tokens[tokenCount].value) - 1] = '\0';
                tokens[tokenCount].line = currentLine;
                
                tokenCount++;
            } else if(isdigit(*currentIndex) || (*currentIndex == '-' && isdigit(*(currentIndex + 1)))) {
                char num[32] = "";
                uint8_t i = 0;
                if(*currentIndex == '-') { num[i] = *currentIndex; currentIndex++; i++; }

                while(isdigit(*currentIndex) && i < 31) { num[i] = *currentIndex++; i++; }
                num[i] = '\0';
                
                CheckMem(tokens, tokenCount, capacity, "Failed to reallocate memory for tokens array");

                tokens[tokenCount].type = TOKEN_INT;
                strncpy(tokens[tokenCount].value, num, sizeof(tokens[tokenCount].value) - 1);
                tokens[tokenCount].value[sizeof(tokens[tokenCount].value) - 1] = '\0';
                tokens[tokenCount].line = currentLine;

                tokenCount++;
            } else if(*currentIndex == ',' || *currentIndex == '[' || *currentIndex == ']') {
                CheckMem(tokens, tokenCount, capacity, "Failed to reallocate memory for tokens array");

                switch(*currentIndex) {
                    case ',': tokens[tokenCount].type = TOKEN_COMMA; break;
                    case '[': tokens[tokenCount].type = TOKEN_L_BRACK; break;
                    case ']': tokens[tokenCount].type = TOKEN_R_BRACK; break;
                }
                tokens[tokenCount].value[0] = '\0';
                tokens[tokenCount].line = currentLine;

                tokenCount++;
                currentIndex++;
            } else { currentIndex++; } //temp
        }
    }
    CheckMem(tokens, tokenCount, capacity, "Failed to reallocate memory for tokens array");

    tokens[tokenCount].type = TOKEN_EOF;
    tokens[tokenCount].line = currentLine;
    tokenCount++;

    if(outTokenCount) *outTokenCount = tokenCount;
    return tokens;
}
