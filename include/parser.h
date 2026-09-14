#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <stddef.h>
#include "lexer.h"

typedef enum {
    SECTION_NONE = 0, // dont chnage dat 
    SECTION_DATA,
    SECTION_TEXT
} SectionType;

typedef enum {
    OPERAND_NONE,
    OPERAND_REG,
    OPERAND_IMM,
    //OPERAND_MEM,
    OPERAND_LABEL,
    OPERAND_VAR
} OperandType;

typedef struct {
    OperandType type; 
    union { 
        uint8_t reg; 
        int32_t imm;
        char name[64];
        char data[64];
    } val;
} Operand;

typedef struct {
    TokenType opcode; 
    Operand dest;     
    Operand src;      
    int line;         
} Instruction;

Instruction* Parser(const Token *tokens, size_t tokenCount, size_t *outInstructionCount);

#endif