#include <stdbool.h>
#include <stdlib.h>
#include "include/error.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/resolver.h"


bool is_defined(const Instruction *instr, size_t instrCount, const char *name, tokenType opcode) {
    for(size_t i = 0; i < instrCount; i++) {
        if(instr[i].opcode == opcode && strcmp(instr[i].src.val.name, name) == 0) { return true; }
    }
    return false;
}

void Resolver(const Instruction *instrutions, size_t InstrCount) {
    for(size_t i = 0; i < InstrCount; i++){
        switch(instrutions[i].opcode){
            case TOKEN_JMP:
            case TOKEN_JIE:
            case TOKEN_JINE:
            case TOKEN_JIG:
            case TOKEN_JIL:
                if(instrutions[i].src.type == OPERAND_LABEL){
                    if(!is_defined(instrutions, InstrCount, instrutions[i].src.val.name, TOKEN_LABEL)){
                        UsmError("at line %d\nlabel '%s' is not defind", instrutions[i].line, instrutions[i].src.val.name);
                    }
                }
                break;
            case TOKEN_LOAD:
                if(instrutions[i].dest.type == OPERAND_MEM) {
                    if(!is_defined(instrutions, InstrCount, instrutions[i].dest.val.name, TOKEN_ID)){
                        UsmError("at line %d\nvariable '%s' is not defined", instrutions[i].line, instrutions[i].dest.val.name);
                    }
                }
                break;
            case TOKEN_STR:
                if(instrutions[i].src.type == OPERAND_MEM) {
                    if(!is_defined(instrutions, InstrCount, instrutions[i].src.val.name, TOKEN_ID)){
                        UsmError("at line %d\nvariable '%s' is not defined", instrutions[i].line, instrutions[i].src.val.name);
                    }
                }
                break;
            case TOKEN_CALL:
                if(instrutions[i].src.type == OPERAND_LABEL) {
                    if(!is_defined(instrutions, InstrCount, instrutions[i].src.val.name, TOKEN_LABEL)) {
                        UsmError("at line %d\nfunction '%s' is not defined", instrutions[i].line, instrutions[i].src.val.name);
                    }
                }
                break;
            default: break;
        }
    }
}