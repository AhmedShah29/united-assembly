#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char** strings;
    uint32_t* table;
    uint32_t count;
    uint32_t cap;
} Pool;

static inline bool pool_init(Pool* pool, uint32_t cap)
{
    pool->cap = cap;
    pool->count = 1;
    pool->strings = malloc(cap * sizeof(char*));
    if(pool->strings == NULL) return false;
    pool->table = calloc(cap, sizeof(uint32_t));
    if(pool->table == NULL) return false;

    return true;
}

static inline uint32_t hash_str(const char* str, size_t len)
{
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)str[i];
        hash *= 16777619u;
    }
    return hash;
}

static inline uint32_t pool_intern(Pool* pool, const char* str)
{
    uint32_t hash = hash_str(str, strlen(str));
    uint32_t index = hash & (pool->cap - 1);

    if(pool->count >= pool->cap) return UINT32_MAX;
    
    while(1){
        if(pool->table[index] == 0) { 
            pool->strings[pool->count] = strdup(str); 
            pool->table[index] = pool->count;
            pool->count++;
            return pool->count - 1;
        } else {
            uint32_t id = pool->table[index];
            if(strlen(pool->strings[id]) != strlen(str)) {
                index = (index + 1) & (pool->cap - 1);
                continue; 
            }
            if(!strcmp(pool->strings[id], str)) return id;
            else index = (index + 1) & (pool->cap - 1);
        }
    }
}

static inline const char* pool_get(const Pool* pool, uint32_t id) 
{
    if (id == 0 || id >= pool->count) return "";
    return pool->strings[id];
}

#endif

/*  resize code
    
    pool->cap *= 2;
    char** resized = realloc(pool->strings, pool->cap * sizeof(char*));
    if(resized == NULL) return UINT32_MAX;
    pool->strings = resized;
    
    uint32_t* resizedtable = realloc(pool->table, pool->cap * sizeof(uint32_t));
    if(resizedtable == NULL) return UINT32_MAX;
    pool->table = resizedtable;
    
 */