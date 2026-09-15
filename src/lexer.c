#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/error.h"
#include "include/utils.h"

Token* Lexer(const char *source, size_t *outTokenCount) {

    const char *currentIndex = source;
    int currentLine = 1; // file lines calculated through the '\n'
    size_t capacity = strlen(source) / 2 + 8; // max size of the tokens array
    size_t tokenCount = 0;  // current tokens array size

    Token *tokens = malloc(capacity * sizeof(Token));
    check_mem(tokens, tokenCount, capacity, Token, "Failed to reallocate memory for tokens array");

    while(*currentIndex != '\0') {
        switch(*currentIndex) {
            case ' ':
            case '\t':           /* skips spaces etc... */
            case '\r':
                currentIndex++;
                break;
            case '\n':
                
                check_mem(tokens, tokenCount, capacity, Token, "Failed to reallocate memory for tokens array");
    
                tokens[tokenCount].type = TOKEN_NLN;
                tokens[tokenCount].value[0] = '\0';
                tokens[tokenCount].line = currentLine;
                
                currentLine++;
                currentIndex++;
                tokenCount++;
                break;
            case ';':
                while (*currentIndex != '\n' && *currentIndex != '\0') { currentIndex++; }
                break;
            default: { 
                if(isalpha(*currentIndex) || *currentIndex == '_') {
                    char word[32];
                    int i = 0;
    
                    while((isalnum(*currentIndex) || *currentIndex == '_') && i < 31) {  // prevents the size overflow
                        word[i] = *currentIndex;
                        i++;
                        currentIndex++;
                    }
                    word[i] = '\0';
    
                    if(*currentIndex == ':') { tokens[tokenCount].type = TOKEN_LABEL; currentIndex++; }
                    else { tokens[tokenCount].type = get_token_enum(word); }
    
                    check_mem(tokens, tokenCount, capacity, Token, "Failed to reallocate memory for tokens array");
    
    
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
                    
                    check_mem(tokens, tokenCount, capacity, Token, "Failed to reallocate memory for tokens array");
    
                    tokens[tokenCount].type = TOKEN_INT;
                    strncpy(tokens[tokenCount].value, num, sizeof(tokens[tokenCount].value) - 1);
                    tokens[tokenCount].value[sizeof(tokens[tokenCount].value) - 1] = '\0';
                    tokens[tokenCount].line = currentLine;
    
                    tokenCount++;
                } else if(*currentIndex == ',' || *currentIndex == '[' || *currentIndex == ']') {
                    check_mem(tokens, tokenCount, capacity, Token, "Failed to reallocate memory for tokens array");
    
                    switch(*currentIndex) {
                        case ',': tokens[tokenCount].type = TOKEN_COMMA; break;
                        case '[': tokens[tokenCount].type = TOKEN_L_BRACK; break;
                        case ']': tokens[tokenCount].type = TOKEN_R_BRACK; break;
                    }
                    tokens[tokenCount].value[0] = '\0';
                    tokens[tokenCount].line = currentLine;
    
                    tokenCount++;
                    currentIndex++;
                } else if(*currentIndex == '.') {
                    currentIndex++;
                    
                    char word[32] = "";
                    uint8_t i = 0;

                    while(isalpha(*currentIndex) && i < 31) { word[i] = *currentIndex++; i++; }
                    word[i] = '\0';

                    tokens[tokenCount].type = TOKEN_SECTION;
                    strncpy(tokens[tokenCount].value, word, sizeof(tokens[tokenCount].value) - 1);
                    tokens[tokenCount].value[sizeof(tokens[tokenCount].value) - 1] = '\0';
                    tokens[tokenCount].line = currentLine;

                    tokenCount++;
                } else if(*currentIndex == '"') {
                    currentIndex++;
                    
                    char word[32] = "";
                    uint8_t i = 0;

                    while(*currentIndex != '"' && *currentIndex != '\0' && i < 31) { word[i] = *currentIndex++; i++; }
                    if(*currentIndex == '\0') { UsmError("at line %d\nunterminated string", currentLine); }
                    currentIndex++;
                    word[i] = '\0';

                    tokens[tokenCount].type = TOKEN_STRING;
                    strncpy(tokens[tokenCount].value, word, sizeof(tokens[tokenCount].value) - 1);
                    tokens[tokenCount].value[sizeof(tokens[tokenCount].value) - 1] = '\0';
                    tokens[tokenCount].line = currentLine;
                    
                    tokenCount++;
                } else { 

                    // check other hidden char set up sometimes by the code editors
                    if (isspace((unsigned char)*currentIndex) || iscntrl((unsigned char)*currentIndex)) { currentIndex++; break; }
                    
                    UsmError("at line %d\nunknown symbol '%c' (ASCII %d)", currentLine, *currentIndex, (unsigned char)*currentIndex);
                }
            }
        }
    }
    check_mem(tokens, tokenCount, capacity, Token, "Failed to reallocate memory for tokens array");

    tokens[tokenCount].type = TOKEN_EOF;
    tokens[tokenCount].line = currentLine;
    tokens[tokenCount].value[0] = '\0';
    tokenCount++;

    if(outTokenCount) *outTokenCount = tokenCount;
    return tokens;
}
