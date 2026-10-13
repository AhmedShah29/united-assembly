#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/error.h"
#include "include/utils.h"
#include "include/pool.h"

typedef struct {
    const char *currIndex;
    size_t cap;
    uint32_t currLine;
    uint32_t currCol;
} LexerState;

static inline void emit_token(Token **tokens, size_t *count, size_t *capacity, 
    TokenType type, LexerState *state,
    const char *value, Pool *pool
);

static inline void advance(LexerState* state);

Token* Lexer(const char *source, size_t *outTokenCount, Pool* pool)
{
    //const char *state.currIndex = source;
    //size_t state.currLine = 1; 
    //size_t capacity = strlen(source) / 2 + 8;
    size_t tokenCount = 0;  // tokens array size

    LexerState state = {
        .currIndex = source,
        .cap = strlen(source) / 2 + 8,  // init capacity 1/2 of the source size
        .currLine = 1,
        .currCol = 1,
    };

    Token *tokens = malloc(state.cap * sizeof(Token));
    check_mem(tokens, tokenCount, state.cap, Token, "Failed to reallocate memory for tokens array");

    while(*state.currIndex != '\0') {
        switch(*state.currIndex) {
            case ' ':
            case '\t':           /* skips spaces etc... */
            case '\r':
                advance(&state);
                break;
            case '\n':
                emit_token(&tokens, &tokenCount, &state.cap, TOKEN_NLN, &state, "", pool);
                advance(&state);
                break;
            case ';':
                while (*state.currIndex != '\n' && *state.currIndex != '\0') advance(&state);
                break;
            case '[':
                emit_token(&tokens, &tokenCount, &state.cap, TOKEN_L_BRACK, &state, "", pool);
                advance(&state);
                break;
            case ']': 
                emit_token(&tokens, &tokenCount, &state.cap, TOKEN_R_BRACK, &state, "", pool);
                advance(&state);
                break;
            case '&':
                emit_token(&tokens, &tokenCount, &state.cap, TOKEN_ADDR, &state, "", pool);
                advance(&state);
                break;
            case ',':
                emit_token(&tokens, &tokenCount, &state.cap, TOKEN_COMMA, &state, "", pool);
                advance(&state);
                break;
            case '.': {
                advance(&state);
                const char* startIndex = state.currIndex;

                while(isalpha(*state.currIndex)) advance(&state);
                
                size_t len = state.currIndex - startIndex;
                char word[len + 1]; // <-- not good
                strncpy(word, startIndex, len);
                word[len] = '\0';

                emit_token(&tokens, &tokenCount, &state.cap, TOKEN_SECTION, &state, word, pool);
                break;
            }
            case '"': {
                advance(&state);
                const char* startIndex = state.currIndex;

                while(*state.currIndex != '"' && *state.currIndex != '\0') advance(&state);
                if(*state.currIndex == '\0') UsmError("at line %d\nunterminated string", state.currLine);

                advance(&state);

                size_t len = state.currIndex - startIndex - 1; // extra 1 from this ^ step
                char word[len + 1];
                strncpy(word, startIndex, len);
                word[len] = '\0';

                emit_token(&tokens, &tokenCount, &state.cap, TOKEN_STRING, &state, word, pool);
                break;
            }       
            default: { 
                if(isalpha(*state.currIndex) || *state.currIndex == '_') {
                    const char* startIndex = state.currIndex;
    
                    while((isalnum(*state.currIndex) || *state.currIndex == '_')) advance(&state);
                    
                    size_t len = state.currIndex - startIndex;
                    char word[len + 1]; // <-- not good
                    strncpy(word, startIndex, len);
                    word[len] = '\0';

                    TokenType tokentype = (*state.currIndex == ':') ? TOKEN_LABEL : get_token_enum(word);
                    if(tokentype == TOKEN_LABEL) advance(&state);
    
                    emit_token(&tokens, &tokenCount, &state.cap, tokentype, &state, word, pool);
                } else if(isdigit(*state.currIndex) || (*state.currIndex == '-' && isdigit(*(state.currIndex + 1)))) {
                    const char* startIndex = state.currIndex;
                    
                    if(*state.currIndex == '-') advance(&state);
    
                    while(isdigit(*state.currIndex)) advance(&state);

                    size_t len = state.currIndex - startIndex;
                    char word[len + 1];
                    strncpy(word, startIndex, len);
                    word[len] = '\0';
                    
                    emit_token(&tokens, &tokenCount, &state.cap, TOKEN_INT, &state, word, pool);
                } else { 

                    // check other hidden char set up sometimes by the code editors
                    if (isspace((unsigned char)*state.currIndex) || iscntrl((unsigned char)*state.currIndex)) { state.currIndex++; break; }
                    
                    UsmError("at line %d\nunknown symbol '%c' (ASCII %d)", state.currLine, *state.currIndex, (unsigned char)*state.currIndex);
                }
            }
        }
    }
    emit_token(&tokens, &tokenCount, &state.cap, TOKEN_EOF, &state, "", pool);

    if(outTokenCount) *outTokenCount = tokenCount;
    return tokens;
}

static inline void emit_token(Token **tokens, size_t *count, size_t *capacity, 
                              TokenType type, LexerState *state, const char *value, Pool *pool)
{
    check_mem(*tokens, *count, *capacity, Token, "Failed to reallocate memory for tokens array");
    
    (*tokens)[*count].type = type;
    (*tokens)[*count].line = state->currLine;
    (*tokens)[*count].col = state->currCol;
    (*tokens)[*count].strId = pool_intern(pool, value ? value : "");
    (*count)++;
}

static inline void advance(LexerState* state)
{
    if(*state->currIndex == '\n') { state->currLine++; state->currCol = 1; }
    else { state->currCol++; }
    state->currIndex++;
}

/*
    TODO:
     - fix the word buffer overflaw (lazy to do it now) and chnage name word -> wordBuff
     - add more docementing to the lexer code
    DONE:
     - added string pool (pool.h)
     - refactored the code
 */