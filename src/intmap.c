#include "map/intmap.h"
#include <stdlib.h>
#include <string.h>
#include "internal/memmngr.h"

#define INITIAL_CAPACITY 16
#define MAX_CAPACITY (1u << 31)
#define GROWTH_FACTOR 2
#define THRESHOLD_LOAD_FACTOR 0.75

typedef struct intmapnode {
    uint32_t hash;
    char* key;
    int value;
    struct intmapnode* next;
} *IntMapNode;

struct intmap {
    IntMapNode* table;
    uint32_t size;
    uint32_t capacity;
};

struct intmapiter {
    KeyValuePair* items;
    uint32_t index;
    uint32_t size;
};

static bool intmap_not_exists(IntMap map) {
    return !map;
}

bool intmap_is_empty(IntMap map) {
    return intmap_not_exists(map) || map->size == 0;
}

static IntMapNode intmap_create_node(uint32_t hash, const char* key, int value) {
    IntMapNode new_node = (IntMapNode) malloc(sizeof (struct intmapnode));
    if (!new_node) return NULL;

    char* key_copy = (char*) malloc(strlen(key) + 1);
    if (!key_copy) {
        free(new_node);
        return NULL;
    }
    strcpy(key_copy, key);

    *new_node = (struct intmapnode) {.hash = hash, .key = key_copy, .value = value, .next = NULL};
    return new_node;
}

static void intmap_free_node(IntMapNode node) {
    free(node->key);
    free(node);
}

static uint32_t intmap_hash(const char* key) {
    uint32_t fnv_prime = 0x01000193;
    uint32_t off_basis = 0x811c9dc5;
    
    uint32_t h = off_basis;
    
    char ch;
    while ((ch = *key++)) {
        h ^= ch;
        h *= fnv_prime;
    }
    return h;
}

static uint32_t intmap_get_index(uint32_t hash, uint32_t capacity) {
    return hash & (capacity - 1);
}

static IntMapNode intmap_get_node_by_key(const IntMap map, const char* key) {
    if (intmap_is_empty(map) || !key) return NULL;

    uint32_t hash = intmap_hash(key);
    IntMapNode curr = map->table[intmap_get_index(hash, map->capacity)];
    while (curr) {
        if (hash == curr->hash && strcmp(curr->key, key) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

static void intmap_transfer(const IntMap map, IntMapNode* new_table, uint32_t new_capacity) {
    for (uint32_t i = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr) {
            IntMapNode next = curr->next;

            uint32_t index = intmap_get_index(curr->hash, new_capacity);
            curr->next = new_table[index];
            new_table[index] = curr; 

            curr = next;
        }
    }
}

static bool intmap_resize(IntMap map) {
    uint32_t new_capacity = map->capacity * GROWTH_FACTOR;
    if (new_capacity > MAX_CAPACITY) return false;

    IntMapNode* new_table = (IntMapNode*) calloc(new_capacity, sizeof (IntMapNode));
    if (!new_table) return false;

    intmap_transfer(map, new_table, new_capacity);
    
    free(map->table);
    map->table = new_table;
    map->capacity = new_capacity;
    return true;
}

static void intmap_free(IntMap map) {
    if (intmap_not_exists(map)) return;

    for (uint32_t i = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr) {
            IntMapNode next = curr->next;

            intmap_free_node(curr);

            curr = next;
        }
    }
    free(map->table);
    map->size = map->capacity = 0;
    free(map);
}

IntMap intmap_new(void) {
    IntMap new_map = (IntMap) malloc(sizeof (struct intmap));
    if (intmap_not_exists(new_map)) return NULL;

    IntMapNode* table = (IntMapNode*) calloc(INITIAL_CAPACITY, sizeof (IntMapNode));
    if (!table) {
        free(new_map);
        return NULL;
    }

    if (!memmngr_register(new_map, (void (*)(void*)) intmap_free)) {
        free(table);
        free(new_map);
        return NULL;
    }

    *new_map = (struct intmap) {.table = table, .size = 0, .capacity = INITIAL_CAPACITY};
    return new_map;
}

void intmap_clear(IntMap map) {
    if (intmap_is_empty(map)) return;

    for(uint32_t i = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr) {
            IntMapNode next = curr->next;

            intmap_free_node(curr);

            curr = next;
        }
        map->table[i] = NULL;
    }
    map->size = 0;
}

bool intmap_insert(IntMap map, const char* key, int value) {
    if (intmap_not_exists(map) || !key) return false;
    
    uint32_t hash = intmap_hash(key);
    uint32_t index = intmap_get_index(hash, map->capacity);

    IntMapNode curr = map->table[index];
    while (curr) { 
        if (hash == curr->hash && strcmp(curr->key, key) == 0) return false;
        curr = curr->next;
    }
    
    IntMapNode new_node = intmap_create_node(hash, key, value);
    if (!new_node) return false;

    if (++map->size > (uint32_t) (THRESHOLD_LOAD_FACTOR * map->capacity)) {
        if (!intmap_resize(map)) {
            intmap_free_node(new_node);
            return false;
        }
        index = intmap_get_index(hash, map->capacity);
    }

    new_node->next = map->table[index];
    map->table[index] = new_node;
    return true;
}

bool intmap_get(const IntMap map, const char* key, int* out) {
    if (!out) return false;

    IntMapNode target = intmap_get_node_by_key(map, key);
    if (!target) return false;

    *out = target->value;
    return true;
}

bool intmap_set(IntMap map, const char* key, int new_value) {
    IntMapNode target = intmap_get_node_by_key(map, key);
    if (!target) return intmap_insert(map, key, new_value);

    target->value = new_value;
    return true;
}

void intmap_remove(IntMap map, const char* key) {
    if (intmap_is_empty(map) || !key) return;

    uint32_t hash = intmap_hash(key);
    uint32_t index = intmap_get_index(hash, map->capacity);

    IntMapNode prev = NULL;
    IntMapNode curr = map->table[index];
    while (curr) {
        if (hash == curr->hash && strcmp(curr->key, key) == 0) {
            if (!prev) {
                map->table[index] = curr->next;
            } else {
                prev->next = curr->next;
            }

            intmap_free_node(curr);
            map->size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

bool intmap_has_key(const IntMap map, const char* key) {
    return intmap_get_node_by_key(map, key);
}

char** intmap_keys(const IntMap map) {
    if (intmap_is_empty(map)) return NULL;

    char** keys = (char**) malloc(sizeof (char*) * map->size);
    if (!keys) return NULL;

    if (!memmngr_register(keys, free)) {
        free(keys);
        return NULL;
    }

    for (uint32_t i = 0, j = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr) {
            char* key_copy = (char*) malloc(strlen(curr->key) + 1);
            if (!key_copy) {
                for (uint32_t k = 0; k < j; k++) {
                    free(keys[k]);
                }
                memmngr_rollback();
                return NULL;
            }

            strcpy(key_copy, curr->key);
            keys[j++] = key_copy;

            curr = curr->next;
        }
    }
    return keys;
}

int* intmap_values(const IntMap map) {
    if (intmap_is_empty(map)) return NULL;

    int* values = (int*) malloc(sizeof (int) * map->size);
    if (!values) return NULL;

    if (!memmngr_register(values, free)) {
        free(values);
        return NULL;
    }

    for (uint32_t i = 0, j = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr) {
            values[j++] = curr->value;
            curr = curr->next;
        }
    }
    return values;
}

bool intmap_equals(const IntMap map1, const IntMap map2) {
    if (intmap_not_exists(map1) || intmap_not_exists(map2)) return false;
    if (map1->size != map2->size) return false;

    for (uint32_t i = 0; i < map1->capacity; i++) {
        IntMapNode curr1 = map1->table[i];
        while (curr1) {
            int value2;
            if (!intmap_get(map2, curr1->key, &value2) || value2 != curr1->value) return false;
            curr1 = curr1->next;
        }
    }
    return true;
}

uint32_t intmap_size(const IntMap map) {
    return intmap_not_exists(map) ? 0 : map->size;
}

IntMapIter intmap_iter_new(const IntMap map) {
    if (intmap_is_empty(map)) return NULL;

    IntMapIter new_iter = (IntMapIter) malloc(sizeof (struct intmapiter));
    if (!new_iter) return NULL;
    
    KeyValuePair* items = (KeyValuePair*) malloc(sizeof (struct keyvaluepair) * map->size);
    if (!items) {
        free(new_iter);
        return NULL;
    }

    for (uint32_t i = 0, j = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr) {
            char* key_copy = (char*) malloc(strlen(curr->key) + 1);
            if (!key_copy) {
                for (uint32_t k = 0; k < j; k++) {
                    free(items[k].key);
                }
                free(items);
                free(new_iter);
                return NULL;
            }
            strcpy(key_copy, curr->key);

            items[j++] = (struct keyvaluepair) {.key = key_copy, .value = curr->value};
            curr = curr->next;
        }
    }

    *new_iter = (struct intmapiter) {.items = items, .index = 0, .size = map->size};
    return new_iter;
}

bool intmap_iter_next(const IntMapIter iter, KeyValuePair* out) {
    if (!iter || !out || iter->index >= iter->size) return false;

    *out = iter->items[iter->index++];
    return true;
} 