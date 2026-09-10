#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include "include/error.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/resolver.h"
#include "include/hashmap.h"


void Resolver(const Instruction *instrutions, size_t InstrCount) {

    HashMap map;
    HashMap_init(&map);

    for(size_t i = 0; i < InstrCount; i++){
        switch(instrutions[i].opcode){
            case TOKEN_LABEL:
                HashMap_insert(&map, instrutions[i].src.val.name, i, TYPE_LABEL);
                break;
            case TOKEN_ID:
                HashMap_insert(&map, instrutions[i].src.val.name, i, TYPE_VAR);
        }
    }
    
    for(size_t i = 0; i < InstrCount; i++){
        switch(instrutions[i].opcode){
            case TOKEN_JMP:
            case TOKEN_JIE:
            case TOKEN_JINE:
            case TOKEN_JIG:
            case TOKEN_JIL:
                if(instrutions[i].src.type == OPERAND_LABEL){
                    if(HashMap_get(&map, instrutions[i].src.val.name, TYPE_LABEL) == UINT32_MAX) {
                        UsmError("at line %d\nlabel '%s' is not defind", instrutions[i].line, instrutions[i].src.val.name);
                    }
                }
                break;
            case TOKEN_LOAD:
                if(instrutions[i].dest.type == OPERAND_VAR) {
                    if(HashMap_get(&map, instrutions[i].dest.val.name, TYPE_VAR) == UINT32_MAX){
                        UsmError("at line %d\nvariable '%s' is not defined", instrutions[i].line, instrutions[i].dest.val.name);
                    }
                }
                break;
            case TOKEN_STR:
                if(instrutions[i].src.type == OPERAND_VAR) {
                    if(HashMap_get(&map, instrutions[i].src.val.name, TYPE_VAR) == UINT32_MAX){
                        UsmError("at line %d\nvariable '%s' is not defined", instrutions[i].line, instrutions[i].src.val.name);
                    }
                }
                break;
            case TOKEN_CALL:
                if(instrutions[i].src.type == OPERAND_LABEL) {
                    if(HashMap_get(&map, instrutions[i].src.val.name, TYPE_LABEL) == UINT32_MAX) {
                        UsmError("at line %d\nfunction '%s' is not defined", instrutions[i].line, instrutions[i].src.val.name);
                    }
                }
                break;
            default: break;
        }
    }
}