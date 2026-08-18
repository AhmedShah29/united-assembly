#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/parser.h"
#include "include/error.h"
#include "include/lexer.h"


static uint8_t parse_register(const char *regStr, int line) {
    int regNum = -1;

    if (regStr[0] != 'R' && regStr[0] != 'r') {
        UsmError("At line %d \n %s \n expected a register using r or R", line, regStr);
    } else {
        regNum = atoi(&regStr[1]);
        if (regNum < 0 || regNum > 9) {
            UsmError("At line %d \n Usm supports 10 registers from 0 - 9", line);
        }
    }
    return (uint8_t)regNum;
}


Instruction* parser(const token *tokens, size_t tokenCount, size_t *outInstructionCount) {
    uint32_t instrCount = 0;
    uint32_t capacity = 8;
    Instruction *instructions = malloc(capacity * sizeof(Instruction));
    if(instructions == NULL) { UsmError("unable to allocate memory for the instructions"); }

    uint32_t i = 0;
    
    while(i < tokenCount) {
        if(tokens[i].type == TOKEN_NLN) { i++; continue; }
        else if (tokens[i].type == TOKEN_EOF) { break; }
        
        else if(tokens[i].type == TOKEN_MOV) {
            Instruction instr;
            instr.line = tokens[i].line;
            instr.opcode = TOKEN_MOV;
            i++;

            if (tokens[i].type == TOKEN_REG) {
                instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                instr.dest.type = OPERAND_REG;
                i++;
            } else { UsmError("at line %d expected a register", tokens[i].line); }

            if(tokens[i].type == TOKEN_COMMA) { i++; } else { UsmError("at line %d expected a ',' ", tokens[i].line); }

            if(tokens[i].type == TOKEN_REG) {
                instr.src.type = OPERAND_REG;
                instr.src.val.reg = parse_register(tokens[i].value, tokens[i].line);
                i++;
            } else { UsmError("at line %d expected a register", tokens[i].line); }

            if(instrCount >= capacity) { 
                capacity *= 2;
                instructions = realloc(instructions, capacity * sizeof(Instruction));
                if(instructions == NULL) { UsmError("unable to realloc memory for the instructions"); }
            }
            instructions[instrCount] = instr;
            instrCount++;
        }
        
        else { UsmError("Uknown token at line %d", tokens[i].line); }
    }
    if (outInstructionCount != NULL) { *outInstructionCount = instrCount; }
    return instructions;
}