#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <stdint.h>
#include <stddef.h>

typedef enum {
    OPERAND_NONE,
    OPERAND_REG,
    OPERAND_IMM  
} OperandType;

typedef struct {
    OperandType type; 
    union { 
        uint8_t reg; 
        int32_t imm; 
    } val;
} Operand;

typedef struct {
    tokenType opcode; 
    Operand dest;     
    Operand src;      
    int line;         
} Instruction;

Instruction* parse(const token *tokens, size_t tokenCount, size_t *outInstructionCount);

#endif