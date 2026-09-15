#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/parser.h"
#include "include/error.h"
#include "include/lexer.h"
#include "include/utils.h"
#include "include/resolver.h"

static inline uint8_t parse_size(const char *sizeStr, uint32_t line) {
    int sizeNum = -1;

    if(sizeStr[0] == 'B') {
        sizeNum = atoi(&sizeStr[1]);
    }

    switch(sizeNum) {
        case 8: return 1;
        case 16: return 2;
        case 32: return 4;
        case 64: return 8;
        default:
            UsmError("at line %d\n'%s' is not a valid USM size\n sizes: B8/16/32/64", line, sizeStr);
            break;
    }
    return (uint8_t)sizeNum;
}

static inline int64_t parse_immediate(const char *immStr, int line) {
    char *endPtr;
    long long immVal = strtoll(immStr, &endPtr, 10);
    if(endPtr == immStr || *endPtr != '\0') { UsmError("at line %d \n %s | is not a integer", line, immStr); }
    return  (int64_t)immVal;
}

static inline uint8_t parse_register(const char *regStr, int line) {
    int regNum = -1;

    if(strcmp(regStr, "RSP") == 0) { return 10; }

    if (regStr[0] != 'R') {
        UsmError("At line %d \n %s | expected a register using R", line, regStr);
    } else {
        regNum = atoi(&regStr[1]);
        if (regNum < 0 || regNum > 9) {
            UsmError("At line %d \n Usm supports 10 registers from 0 - 9 and a stack register 'RSP' only", line);
        }
    }
    return (uint8_t)regNum;
}


Instruction* Parser(const Token *tokens, size_t tokenCount, size_t *outInstructionCount) {
    uint32_t instrCount = 0;
    uint32_t capacity = 8;
    Instruction *instructions = malloc(capacity * sizeof(Instruction));
    if(instructions == NULL) { UsmError("unable to allocate memory for the instructions"); }

    uint32_t i = 0;
    SectionType currentSection = SECTION_NONE; // section gaurd

    while(i < tokenCount) {


        switch (tokens[i].type) {
            case TOKEN_NLN: i++; continue;

            case TOKEN_ADD:
            case TOKEN_SUB:
            case TOKEN_MUL:
            case TOKEN_DIV:
            case TOKEN_AND:
            case TOKEN_OR:
            case TOKEN_XOR:
            case TOKEN_SHL:
            case TOKEN_SHR:
            case TOKEN_CMP: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
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

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;
                break;
            }
            case TOKEN_JIE:
            case TOKEN_JIL:
            case TOKEN_JINE:
            case TOKEN_JIG:
            case TOKEN_CALL:
            case TOKEN_JMP: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.opcode = tokens[i].type;
                instr.line = tokens[i].line;

                instr.dest.type = OPERAND_NONE;
                i++;

                switch(tokens[i].type){
                    case TOKEN_LABEL:
                    case TOKEN_ID:
                        instr.src.type = OPERAND_LABEL;
                        strcpy(instr.src.val.name, tokens[i].value);
                        break;
                    default:
                        UsmError("at line %d\nexpect a label after a jump command", tokens[i].line);
                        break;
                }
                i++;

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;
                break;
            }
            case TOKEN_MOV: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                i++;

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

                if(tokens[i].type == TOKEN_COMMA) { i++; } else { UsmError("at line %d \n expected a ',' ", tokens[i].line); }

                if (tokens[i].type == TOKEN_REG) {
                    instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                    instr.dest.type = OPERAND_REG;
                    i++;
                } else { UsmError("at line %d \n expected a register", tokens[i].line); }

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;
                break;
            }
            case TOKEN_LOAD: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                i++;

                if(tokens[i].type == TOKEN_L_BRACK) { i++; } else { UsmError("at line %d\n expected a '[' for a address", tokens[i].line); }

                switch(tokens[i].type) {
                    case TOKEN_REG:
                        instr.dest.type = OPERAND_REG;
                        instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                        break;
                    case TOKEN_ID:
                        instr.dest.type = OPERAND_VAR;
                        strcpy(instr.dest.val.name, tokens[i].value);
                        break;
                    default: UsmError("at line %d\nexpected a variable name or a register between [] \nexample: LOAD [VarName], REG", tokens[i].line); break;
                }
                i++;

                if(tokens[i].type == TOKEN_R_BRACK) { i++; } else { UsmError("at line %d\nexpected a ']' symbol after REG/VAR name", tokens[i].line); }

                if(tokens[i].type == TOKEN_COMMA) { i++; } else { UsmError("at line %d\nexpected a comma ',' after the address", tokens[i].line); }
                if(tokens[i].type == TOKEN_REG) {
                    instr.src.type = OPERAND_REG;
                    instr.src.val.reg = parse_register(tokens[i].value, tokens[i].line);
                    i++;
                } else { UsmError("at line %d\nexpected a register to load the value in", tokens[i].line); }

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;
                break;
            }
            case TOKEN_STR: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                i++;

                if(tokens[i].type == TOKEN_REG) {
                    instr.dest.type = OPERAND_REG;
                    instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                    i++;
                } else { UsmError("at line %d\nexpected a register  to store the value from", tokens[i].line); }

                if(tokens[i].type == TOKEN_COMMA) { i++; } else { UsmError("at line %d\nexpected a comma ',' after the register", tokens[i].line); }

                if(tokens[i].type == TOKEN_L_BRACK) { i++; } else { UsmError("at line %d\nexpected a '[' symbol", tokens[i].line); }

                switch (tokens[i].type) {
                    case TOKEN_REG:
                        instr.src.type = OPERAND_REG;
                        instr.src.val.reg = parse_register(tokens[i].value, tokens[i].line);
                        break;
                    case TOKEN_ID:
                        instr.src.type = OPERAND_VAR;
                        strcpy(instr.src.val.name, tokens[i].value);
                        break;
                    default: UsmError("at line %d\nexpected a variable name between [] \nexample: STR REG, [VarName]", tokens[i].line); break;
                }
                i++;

                if(tokens[i].type == TOKEN_R_BRACK) { i++; } else { UsmError("at line %d\nexpected a ']'", tokens[i].line); }

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;
                break;

            }
            case TOKEN_PUSH:
            case TOKEN_POP: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstructions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;

                if(tokens[i].type == TOKEN_PUSH) {
                    i++;

                    switch(tokens[i].type) {
                        case TOKEN_REG:
                            instr.src.type = OPERAND_REG;
                            instr.src.val.reg = parse_register(tokens[i].value, tokens[i].line);
                            i++;

                            instr.dest.type = OPERAND_NONE;
                        break;
                        case TOKEN_INT:
                            instr.src.type = OPERAND_IMM;
                            instr.src.val.imm = parse_immediate(tokens[i].value, tokens[i].line);
                            i++;

                            instr.dest.type = OPERAND_NONE;
                        break;
                        default:
                            UsmError("at line %d\nexpected a register or a immediate after PUSH command\n example: PUSH REG/IMM", tokens[i].line);
                        break;
                    }
                } else if(tokens[i].type == TOKEN_POP) {
                    i++;

                    switch(tokens[i].type) {
                        case TOKEN_REG:
                            instr.dest.type = OPERAND_REG;
                            instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                            i++;

                            instr.src.type = OPERAND_NONE;
                            break;
                        default:
                            UsmError("at line %d\nexpected a register to pop the value to\n example: POP REG", tokens[i].line);
                            break;
                    }
                }

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;
                break;
            }
            case TOKEN_ID: {
                if(currentSection != SECTION_DATA) { UsmError("at line %d\nvaribles declarations must be in the '.data' section", tokens[i].line); }
                Instruction instr;
                instr.opcode = tokens[i].type;
                instr.line = tokens[i].line;
                instr.src.type = OPERAND_VAR;
                strcpy(instr.src.val.name, tokens[i].value);

                i++;

                switch(tokens[i].type) {
                    case TOKEN_SIZE:
                        instr.size = parse_size(tokens[i].value, tokens[i].line);
                        i++;
                        break;
                    case TOKEN_STRING:
                        instr.size = 1;
                        break;
                    default:
                        UsmError("at line %d\nexpected a size after the variable name", tokens[i].line);
                        break;
                }

                switch(tokens[i].type){
                    case TOKEN_STRING:
                        instr.dest.type = OPERAND_VAR;
                        strcpy(instr.dest.val.name, tokens[i].value);
                        break;
                    case TOKEN_INT:
                        instr.dest.type = OPERAND_IMM;
                        instr.dest.val.imm = parse_immediate(tokens[i].value, tokens[i].line);
                        break;
                    default:
                        UsmError("at line %d\nexpected a integer or a string as a value", tokens[i].line);
                }
                i++;

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");
                instructions[instrCount] = instr;
                instrCount++;

                break;

            }
            case TOKEN_SECTION: {
                Instruction instr;
                instr.opcode = TOKEN_SECTION;
                instr.src.type = OPERAND_LABEL;
                instr.line = tokens[i].line;
                instr.dest.type = OPERAND_NONE;

                if(strcmp(tokens[i].value, "data") == 0) {
                    currentSection = SECTION_DATA;
                    strcpy(instr.src.val.name, tokens[i].value);
                } else  if(strcmp(tokens[i].value, "text") == 0) {
                    currentSection = SECTION_TEXT;
                    strcpy(instr.src.val.name, tokens[i].value);
                } else { UsmError("at line %d\nuknown section '.%s'\n Usm have .data & .text sections only", tokens[i].line, tokens[i].value); }

                i++;

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");
                instructions[instrCount] = instr;
                instrCount++;

                break;
            }
            case TOKEN_LABEL: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                instr.dest.type = OPERAND_NONE;

                instr.src.type = OPERAND_LABEL;
                strcpy(instr.src.val.name, tokens[i].value);
                i++;

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");
                instructions[instrCount] = instr;
                instrCount++;

                break;
            }
            case TOKEN_RET:
            case TOKEN_SYSCALL:
            case TOKEN_EXIT: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = tokens[i].type;
                instr.src.type = OPERAND_NONE;
                instr.dest.type = OPERAND_NONE;

                i++;

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;

                break;
            }
            case TOKEN_NOT: {
                if(currentSection != SECTION_TEXT) { UsmError("at line %d\ninstrctions must be in the text section\n help: add .text before your first command", tokens[i].line); }
                Instruction instr;
                instr.line = tokens[i].line;
                instr.opcode = TOKEN_NOT;
                instr.src.type = OPERAND_NONE;

                i++;

                switch(tokens[i].type) {
                    case TOKEN_REG:
                        instr.dest.type = OPERAND_REG;
                        instr.dest.val.reg = parse_register(tokens[i].value, tokens[i].line);
                        break;
                    default:
                        UsmError("at line %d\n expected a register\n    example: NOT REG", tokens[i].line);
                        break;
                }

                i++;

                check_mem(instructions, instrCount, capacity, Instruction, "unable to realloc memory for the instructions");

                instructions[instrCount] = instr;
                instrCount++;

                break;
            }
            case TOKEN_EOF:
                if (outInstructionCount != NULL) { *outInstructionCount = instrCount; }
                Resolver(instructions, instrCount);
                return instructions;

            default: UsmError("Uknown Token at line %d\nToken: %d", tokens[i].line, tokens[i].type); break;
        }
    }
    return instructions;
}
