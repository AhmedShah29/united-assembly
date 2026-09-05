#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/parser.h"
#include "include/error.h"
#include "include/lexer.h"
#include "include/utils.h"

static inline int32_t parse_immediate(const char *immStr, int line) {
    char *endPtr;
    long immVal = strtol(immStr, &endPtr, 10);
    if(endPtr == immStr || *endPtr != '\0') { UsmError("at line %d \n %s | is not a intger", line, immStr); }
    return  (int32_t)immVal;
}

static inline uint8_t parse_register(const char *regStr, int line) {
    int regNum = -1;

    if (regStr[0] != 'R') {
        UsmError("At line %d \n %s | expected a register using R", line, regStr);
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

        
        switch (tokens[i].type) {
            case TOKEN_NLN: i++; continue;

            case TOKEN_ADD:
            case TOKEN_SUB:
            case TOKEN_MOV: {
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                i++;

                if (tokens[i].type == TOKEN_REG) {
                    instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                    instr.dest.type = OPERAND_REG;
                    i++;
                } else { UsmError("at line %d \n expected a register", tokens[i].line); }

                if(tokens[i].type == TOKEN_COMMA) { i++; } else { UsmError("at line %d \n expected a ',' ", tokens[i].line); }

                switch (tokens[i].type) {
                    case TOKEN_REG: 
                        instr.src.type = OPERAND_REG;
                        instr.src.val.reg = parse_register(tokens[i].value, tokens[i].line);
                        i++;
                        break;
                    case TOKEN_INT:
                        instr.src.type = OPERAND_IMM;
                        instr.src.val.imm = parse_immediate(tokens[i].value, tokens[i].line);
                        i++;
                        break;
                    default: UsmError("at line %d \n expected a register or immediate integer", tokens[i].line); 
                }
                
                if(instrCount >= capacity) { 
                    capacity *= 2;
                    instructions = realloc(instructions, capacity * sizeof(Instruction));
                    if(instructions == NULL) { UsmError("unable to realloc memory for the instructions"); }
                }
                instructions[instrCount] = instr;
                instrCount++;
                break;
            }
            case TOKEN_LOAD: {
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                i++;

                if(tokens[i].type == TOKEN_L_BRACK) { i++; } else { UsmError("at line %d\n expected a '[' for a adress", tokens[i].line); }

                switch(tokens[i].type) {
                    case TOKEN_REG: 
                        instr.dest.type = OPERAND_MEM;
                        instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                        break;
                    case TOKEN_ID:
                        instr.dest.type = OPERAND_MEM;
                        strcpy(instr.dest.val.name, tokens[i].value);
                        break;
                    default: UsmError("at line %d\nexpected a varible name or a rigester between [] \nexample: LOAD [VarName], REG", tokens[i].line); break;
                }
                i++;

                if(tokens[i].type == TOKEN_R_BRACK) { i++; } else { UsmError("at line %d\nexpected a ']' symbol after REG/VAR name", tokens[i].line); }

                if(tokens[i].type == TOKEN_COMMA) { i++; } else { UsmError("at line %d\nexpected a comma ',' after the address", tokens[i].line); }
                if(tokens[i].type == TOKEN_REG) { 
                    instr.src.type = OPERAND_REG;
                    instr.src.val.reg = parse_register(tokens[i].value, tokens[i].line);
                    i++;
                } else { UsmError("at line %d\nexpected a rigester to load the value in", tokens[i].line); }

                CheckMem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                if(instrCount >= capacity) { 
                    capacity *= 2;
                    instructions = realloc(instructions, capacity * sizeof(Instruction));                   // temp untill the check mem function rewrite
                    if(instructions == NULL) { UsmError("unable to realloc memory for the instructions"); }
                }
                
                instructions[instrCount] = instr;
                instrCount++;
                break;
            }
            case TOKEN_STR: {
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                i++;

                if(tokens[i].type == TOKEN_REG) { 
                    instr.dest.type = OPERAND_REG;
                    instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                    i++;
                } else { UsmError("at line %d\nexpected a rigester  to store the value from", tokens[i].line); }

                if(tokens[i].type == TOKEN_COMMA) { i++; } else { UsmError("at line %d\nexpected a comma ',' after the rigester", tokens[i].line); }

                if(tokens[i].type == TOKEN_L_BRACK) { i++; } else { UsmError("at line %d\nexpected a '[' symbol", tokens[i].line); }
                
                switch (tokens[i].type) {
                    case TOKEN_REG: 
                        instr.src.type = OPERAND_MEM;
                        instr.src.val.reg = parse_register(tokens[i].value, tokens[i].line);
                        break;
                    case TOKEN_ID:
                        instr.src.type = OPERAND_MEM;
                        strcpy(instr.src.val.name, tokens[i].value);
                        break;
                    default: UsmError("at line %d\nexpected a varible name between [] \nexample: STR REG, [VarName]", tokens[i].line); break;
                }
                i++;

                if(tokens[i].type == TOKEN_R_BRACK) { i++; } else { UsmError("at line %d\nexpected a ']'", tokens[i].line); }

                CheckMem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");
                
                instructions[instrCount] = instr;
                instrCount++;
                break;
                
            }
            case TOKEN_EOF: 
                if (outInstructionCount != NULL) { *outInstructionCount = instrCount; }
                return instructions;

            default: UsmError("Uknown token at line %d", tokens[i].line); break;
        }
    }
    return instructions;
}