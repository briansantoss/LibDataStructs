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

static bool intmap_not_exists(IntMap map) {
    return map == NULL;
}

bool intmap_is_empty(IntMap map) {
    return intmap_not_exists(map) || map->size == 0;
}

static IntMapNode intmap_create_node(uint32_t hash, const char* key, int value) {
    IntMapNode new_node = (IntMapNode) malloc(sizeof (struct intmapnode));
    if (new_node == NULL) return NULL;

    char* key_copy = (char*) malloc(strlen(key) + 1);
    if (key_copy == NULL) {
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

static IntMapNode intmap_get_node_by_key(IntMap map, const char* key) {
    if (intmap_is_empty(map) || key == NULL) return NULL;

    uint32_t hash = intmap_hash(key);
    IntMapNode curr = map->table[intmap_get_index(hash, map->capacity)];
    while (curr != NULL) {
        if (hash == curr->hash && strcmp(curr->key, key) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

static void intmap_transfer(IntMap map, IntMapNode* new_table, uint32_t new_capacity) {
    for (uint32_t i = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr != NULL) {
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
    if (new_table == NULL) return false;

    intmap_transfer(map, new_table, new_capacity);
    
    free(map->table);
    map->table = new_table;
    map->capacity = new_capacity;
    return true;
}

IntMap intmap_new(void) {
    IntMap new_map = (IntMap) malloc(sizeof (struct intmap));
    if (intmap_not_exists(new_map)) return NULL;

    IntMapNode* table = (IntMapNode*) calloc(INITIAL_CAPACITY, sizeof (IntMapNode));
    if (table == NULL) {
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

    map->size = 0;
}

void intmap_free(IntMap map) {
    if (intmap_not_exists(map)) return;

    for (uint32_t i = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr != NULL) {
            IntMapNode next = curr->next;

            intmap_free_node(curr);

            curr = next;
        }
    }
    free(map->table);
    map->size = map->capacity = 0;
    free(map);
}

bool intmap_insert(IntMap map, const char* key, int value) {
    if (intmap_not_exists(map) || key == NULL) return false;
    
    uint32_t hash = intmap_hash(key);
    uint32_t index = intmap_get_index(hash, map->capacity);

    IntMapNode curr = map->table[index];
    while (curr != NULL) { 
        if (hash == curr->hash && strcmp(curr->key, key) == 0) return false;
        curr = curr->next;
    }
    
    IntMapNode new_node = intmap_create_node(hash, key, value);
    if (new_node == NULL) return false;

    new_node->next = map->table[index];
    map->table[index] = new_node;

    if (++map->size > (uint32_t) (THRESHOLD_LOAD_FACTOR * map->capacity)) {
        return intmap_resize(map);
    }
    return true;
}

bool intmap_get(IntMap map, const char* key, int* out) {
    if (out == NULL) return false;

    IntMapNode target = intmap_get_node_by_key(map, key);
    if (target == NULL) return false;

    *out = target->value;
    return true;
}

bool intmap_set(IntMap map, const char* key, int new_value) {
    IntMapNode target = intmap_get_node_by_key(map, key);
    if (target == NULL) return intmap_insert(map, key, new_value);

    target->value = new_value;
    return true;
}

void intmap_remove(IntMap map, const char* key) {
    if (intmap_is_empty(map) || key == NULL) return;

    uint32_t hash = intmap_hash(key);
    uint32_t index = intmap_get_index(hash, map->capacity);

    IntMapNode prev = NULL;
    IntMapNode curr = map->table[index];
    while (curr != NULL) {
        if (hash == curr->hash && strcmp(curr->key, key) == 0) {
            if (prev == NULL) {
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

bool intmap_has_key(IntMap map, const char* key) {
    return intmap_get_node_by_key(map, key) != NULL;
}

char** intmap_keys(IntMap map) {
    if (intmap_is_empty(map)) return NULL;

    char** keys = (char**) malloc(sizeof (char*) * map->size);
    if (keys == NULL) return NULL;

    if (!memmngr_register(keys, free)) {
        free(keys);
        return NULL;
    }

    for (uint32_t i = 0, j = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr != NULL) {
            char* key_copy = (char*) malloc(strlen(curr->key) + 1);
            if (key_copy == NULL) {
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

int* intmap_values(IntMap map) {
    if (intmap_is_empty(map)) return NULL;

    int* values = (int*) malloc(sizeof (int) * map->size);
    if (values == NULL) return NULL;

    if (!memmngr_register(values, free)) {
        free(values);
        return NULL;
    }

    for (uint32_t i = 0, j = 0; i < map->capacity; i++) {
        IntMapNode curr = map->table[i];
        while (curr != NULL) {
            values[j++] = curr->value;
            curr = curr->next;
        }
    }
    return values;
}

bool intmap_equals(IntMap map1, IntMap map2) {
    if (intmap_not_exists(map1) || intmap_not_exists(map2)) return false;
    if (map1->size != map2->size) return false;

    for (uint32_t i = 0; i < map1->capacity; i++) {
        IntMapNode curr1 = map1->table[i];
        while (curr1 != NULL) {
            int value2;
            if (!intmap_get(map2, curr1->key, &value2) || value2 != curr1->value) return false;
            curr1 = curr1->next;
        }
    }
    return true;
}

uint32_t intmap_len(IntMap map) {
    return intmap_not_exists(map) ? 0 : map->size;
}