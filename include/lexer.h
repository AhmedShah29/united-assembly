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
    TOKEN_STRING,   // "string"
    TOKEN_SECTION,  // .data .text ....

    /* Literals & Identifiers */
    TOKEN_INT,
    TOKEN_REG,
    TOKEN_ID,
    TOKEN_LABEL,   

    /* Opcodes: Data Movement & Memory */
    TOKEN_MOV,
    TOKEN_LOAD,
    TOKEN_STR,

    /* Opcodes: Arithmetic */
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,

    /* Opcodes: Logical */
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

    /* Opcodes: bitwise */
    TOKEN_SHL,
    TOKEN_SHR,
    TOKEN_NOT,

    /* Opcodes: Stack & System */
    TOKEN_PUSH,
    TOKEN_POP,
    TOKEN_CALL,
    TOKEN_RET,
    TOKEN_SYSCALL,
    TOKEN_EXIT
} TokenType;

typedef  struct {
    TokenType type;
    uint32_t line;
    char value[32];
} Token;

Token* Lexer(const char *source, size_t *outTokenCount);

static inline TokenType get_token_enum(const char *word) {
    /* Opcodes: Data Movement & Memory */
    if(strcmp(word, "MOV") == 0) return TOKEN_MOV;
    if(strcmp(word, "LOAD") == 0) return TOKEN_LOAD;
    if(strcmp(word, "STR") == 0) return TOKEN_STR;

    /* Opcodes: Arithmetic */
    if(strcmp(word, "ADD") == 0) return TOKEN_ADD;
    if(strcmp(word, "SUB") == 0) return TOKEN_SUB;
    if(strcmp(word, "MUL") == 0) return TOKEN_MUL;
    if(strcmp(word, "DIV") == 0) return TOKEN_DIV;

    /* Opcodes: Logical */
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
    if(strcmp(word, "CALL") == 0) return TOKEN_CALL;
    if(strcmp(word, "RET") == 0) return TOKEN_RET;
    if(strcmp(word, "SYSCALL") == 0) return TOKEN_SYSCALL;
    if(strcmp(word, "EXIT") == 0) return TOKEN_EXIT;

    /* Opcodes: bitwise */
    if(strcmp(word, "SHL") == 0) return TOKEN_SHL;
    if(strcmp(word, "SHR") == 0) return TOKEN_SHR;
    if(strcmp(word, "NOT") == 0) return TOKEN_NOT;

    /* Registers (R0 - R9) & stack rigester */
    if(word[0] == 'R' && isdigit(word[1])) { return TOKEN_REG; }
    if(strcmp(word, "RSP") == 0) { return TOKEN_REG; }

    /* Default Identifier */
    return TOKEN_ID; 
}

#endif