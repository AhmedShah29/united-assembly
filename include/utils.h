#ifndef UTILS
#define UTILS

#include "include/lexer.h"
#include "include/error.h"

/*
    a macro that checks the capacity 
    reallocates it if needed a tharw a error if it faild
 */

#define CheckMem(tokens, tokenCount, capacity, errorStr) \
do { \
    if((tokenCount) >= (capacity)) { \
        (capacity) = ((capacity) == 0) ? 8 : (capacity) * 2; \
       token *resizedMem = realloc((tokens), (capacity) * sizeof(token)); \
       if(resizedMem == NULL) { UsmError(errorStr); } \
       (tokens) = resizedMem; \
    } \
} while (0) 


// Debug function
const char* tokenTypeToString(tokenType type) {
    switch(type) {
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_NLN: return "TOKEN_NLN";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_L_BRACK: return "TOKEN_L_BRACK";
        case TOKEN_R_BRACK: return "TOKEN_R_BRACK";
        case TOKEN_INT: return "TOKEN_INT";
        case TOKEN_REG: return "TOKEN_REG";
        case TOKEN_ID: return "TOKEN_ID";
        case TOKEN_LABEL: return "TOKEN_LABEL";
        case TOKEN_MOV: return "TOKEN_MOV";
        case TOKEN_LOAD: return "TOKEN_LOAD";
        case TOKEN_STR: return "TOKEN_STR";
        case TOKEN_ADD: return "TOKEN_ADD";
        case TOKEN_SUB: return "TOKEN_SUB";
        case TOKEN_AND: return "TOKEN_AND";
        case TOKEN_OR: return "TOKEN_OR";
        case TOKEN_XOR: return "TOKEN_XOR";
        case TOKEN_CMP: return "TOKEN_CMP";
        case TOKEN_JMP: return "TOKEN_JMP";
        case TOKEN_JIE: return "TOKEN_JIE";
        case TOKEN_JINE: return "TOKEN_JINE";
        case TOKEN_JIG: return "TOKEN_JIG";
        case TOKEN_JIL: return "TOKEN_JIL";
        case TOKEN_PUSH: return "TOKEN_PUSH";
        case TOKEN_POP: return "TOKEN_POP";
        case TOKEN_EXIT: return "TOKEN_EXIT";
        default: return "UNKNOWN";
    }
}

#endif