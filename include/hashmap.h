#ifndef HASHMAP_H
#define HASHMAP_H

#define HASHMAP_SIZE 512
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char name[64];
    bool isEmpty;
    uint32_t sourceIndex;
    uint16_t nameLen;
} MapItem;

typedef struct {
    MapItem items[HASHMAP_SIZE];
} HashMap;

static inline uint32_t hash_string(const char *str){
    uint32_t hash = 0;
    while(*str != '\0') {
        hash = (hash << 5) ^ (uint32_t)*str;
        str++;
    }
    return hash;
}

static inline void HashMap_init(HashMap *map) {
    for(uint32_t i = 0; i < HASHMAP_SIZE; i++) {
        map->items[i].isEmpty = true;
    }
}

static inline void HashMap_insert(HashMap *map, const char *name, uint32_t sourceIndex) {
    uint16_t index = hash_string(name) & (HASHMAP_SIZE - 1);

    while (map->items[index].isEmpty == false) {index = (index + 1) & (HASHMAP_SIZE - 1); }

    map->items[index].sourceIndex = sourceIndex;
    map->items[index].isEmpty = false;
    map->items[index].nameLen = (uint16_t)strlen(name);
    strcpy(map->items[index].name, name);
}

static inline uint32_t HashMap_get(HashMap *map, const char *name) {
    uint16_t targetLen = strlen(name);
    uint32_t index = hash_string(name) & (HASHMAP_SIZE - 1);

    while(map->items[index].isEmpty == false){

        if(map->items[index].nameLen != targetLen) { index = (index + 1) & (HASHMAP_SIZE - 1); continue; }
        
        if(map->items[index].name[0] != name[0]) { index = (index + 1) & (HASHMAP_SIZE - 1); continue; }

        if (strcmp(map->items[index].name, name) == 0) { return map->items[index].sourceIndex; }
        
        index = (index + 1) & (HASHMAP_SIZE - 1);
    }
    return UINT32_MAX;
}

#endif