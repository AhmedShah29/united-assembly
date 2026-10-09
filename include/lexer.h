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
    TOKEN_ADDR,     // &var

    /* Literals & Identifiers */
    TOKEN_INT,
    TOKEN_REG,
    TOKEN_ID,
    TOKEN_LABEL,
    TOKEN_SIZE,

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
    TOKEN_EXTERN,
    TOKEN_LEA,
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
    if(!strcmp(word, "MOV")) return TOKEN_MOV;
    else if(!strcmp(word, "LOAD")) return TOKEN_LOAD;
    else if(!strcmp(word, "STR")) return TOKEN_STR;

    /* Opcodes: Arithmetic */
    else if(!strcmp(word, "ADD")) return TOKEN_ADD;
    else if(!strcmp(word, "SUB")) return TOKEN_SUB;
    else if(!strcmp(word, "MUL")) return TOKEN_MUL;
    else if(!strcmp(word, "DIV")) return TOKEN_DIV;

    /* Opcodes: Logical */
    else if(!strcmp(word, "AND")) return TOKEN_AND;
    else if(!strcmp(word, "OR")) return TOKEN_OR;
    else if(!strcmp(word, "XOR")) return TOKEN_XOR;

    /* Opcodes: Control Flow */
    else if(!strcmp(word, "CMP")) return TOKEN_CMP;
    else if(!strcmp(word, "JMP")) return TOKEN_JMP;
    else if(!strcmp(word, "JIE")) return TOKEN_JIE;
    else if(!strcmp(word, "JINE")) return TOKEN_JINE;
    else if(!strcmp(word, "JIG")) return TOKEN_JIG;
    else if(!strcmp(word, "JIL")) return TOKEN_JIL;

    /* Opcodes: Stack & System */
    else if(!strcmp(word, "PUSH")) return TOKEN_PUSH;
    else if(!strcmp(word, "POP")) return TOKEN_POP;
    else if(!strcmp(word, "CALL")) return TOKEN_CALL;
    else if(!strcmp(word, "RET")) return TOKEN_RET;
    else if(!strcmp(word, "SYSCALL")) return TOKEN_SYSCALL;
    else if(!strcmp(word, "EXIT")) return TOKEN_EXIT;
    else if(!strcmp(word, "EXTERN")) return TOKEN_EXTERN;
    else if(!strcmp(word, "LEA")) return TOKEN_LEA;
    
    /* Opcodes: bitwise */
    else if(!strcmp(word, "SHL")) return TOKEN_SHL;
    else if(!strcmp(word, "SHR")) return TOKEN_SHR;
    else if(!strcmp(word, "NOT")) return TOKEN_NOT;

    /* Registers (R0 - R9) & stack rigester */
    else if(word[0] == 'R' && isdigit(word[1])) return TOKEN_REG;
    else if(!strcmp(word, "RSP")) return TOKEN_REG;

    /* Vars sizes */
    else if(word[0] == 'B' && isdigit(word[1])) return TOKEN_SIZE;
    
    /* Default Identifier */
    return TOKEN_ID; 
}

#endif