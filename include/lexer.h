#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    /* Special & End of File  */
    TOKEN_EOF = 0,
    TOKEN_NLN,      // New line
    TOKEN_COMMA,    // ,
    TOKEN_L_BRACK,  // [
    TOKEN_R_BRACK,  // ]

    /* Literals & Identifiers */
    TOKEN_INT,
    TOKEN_REG,
    TOKEN_ID,
    TOKEN_LABEL,   

    /* Opcodes: Data Movement & Memory */
    TOKEN_MOV,
    TOKEN_LOAD,
    TOKEN_STR,

    /* Opcodes: Arithmetic & Logical */
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_XOR,

    /* Opcodes: Control Flow */
    TOKEN_CMP,
    TOKEN_JMP,
    TOKEN_JIE,
    TOKEN_JINE,
    TOKEN_JIG,
    TOKEN_JIL,

    /* Opcodes: Stack & System */
    TOKEN_PUSH,
    TOKEN_POP,
    TOKEN_EXIT
} tokenType;

typedef  struct {
    tokenType type;
    uint32_t line;
    char value[32];
} token;

token* lexer(const char *source, size_t *outTokenCount);

static inline tokenType getTokenEnum(const char *word) {
    /* Opcodes: Data Movement & Memory */
    if(strcmp(word, "MOV") == 0) return TOKEN_MOV;
    if(strcmp(word, "LOAD") == 0) return TOKEN_LOAD;
    if(strcmp(word, "STR") == 0) return TOKEN_STR;

    /* Opcodes: Arithmetic & Logical */
    if(strcmp(word, "ADD") == 0) return TOKEN_ADD;
    if(strcmp(word, "SUB") == 0) return TOKEN_SUB;
    if(strcmp(word, "AND") == 0) return TOKEN_AND;
    if(strcmp(word, "OR") == 0) return TOKEN_OR;
    if(strcmp(word, "XOR") == 0) return TOKEN_XOR;

    /* Opcodes: Control Flow */
    if(strcmp(word, "CMP") == 0) return TOKEN_CMP;
    if(strcmp(word, "JMP") == 0) return TOKEN_JMP;
    if(strcmp(word, "JIE") == 0) return TOKEN_JIE;
    if(strcmp(word, "JINE") == 0) return TOKEN_JINE;
    if(strcmp(word, "JIG") == 0) return TOKEN_JIG;
    if(strcmp(word, "JIL") == 0) return TOKEN_JIL;

    /* Opcodes: Stack & System */
    if(strcmp(word, "PUSH") == 0) return TOKEN_PUSH;
    if(strcmp(word, "POP") == 0) return TOKEN_POP;
    if(strcmp(word, "EXIT") == 0) return TOKEN_EXIT;

    /* Registers (R0 - R9) */
    if(word[0] == 'R' && isdigit(word[1])) { return TOKEN_REG; }

    /* Default: Identifier */
    return TOKEN_ID; 
}

#endif