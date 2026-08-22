#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_EOF = 0,
    TOKEN_NLN, // new line token
    TOKEN_ADD,
    TOKEN_MOV,
    TOKEN_SUB,
    TOKEN_REG, // rigester token
    TOKEN_COMMA,
    TOKEN_ID,
    TOKEN_INT
} tokenType;

typedef  struct {
    tokenType type;
    char value[32];
    uint32_t line;
} token;

token* lexer(const char *source, size_t *outTokenCount);

static inline tokenType getTokenEnum(const char *word) {
    if(strcmp(word, "MOV") == 0) return TOKEN_MOV;
    if(strcmp(word, "ADD") == 0) return TOKEN_ADD;
    if(strcmp(word, "SUB") == 0) return TOKEN_SUB;
    if(word[0] == 'R' && isdigit(word[1])) { return TOKEN_REG; } // checks if the starts with R and is 1 of the 9 rigesters
    return TOKEN_ID; // returns it as a id if non of above
}

#endif