#include "map/intmap.h"
#include <stdlib.h>
#include <string.h>
#include "internal/memmngr.h"

#define INITIAL_CAPACITY 16
#define GROWTH_FACTOR 2
#define MAX_LOAD_FACTOR 0.75

typedef struct intmapnode {
    char* key;
    int value;
    struct intmapnode* next;
} *IntMapNode;

struct intmap {
    IntMapNode* buckets;
    size_t size;
    size_t capacity;
};

static bool intmap_not_exists(IntMap map) {
    return map == NULL;
}

static bool intmap_is_empty(IntMap map) {
    return intmap_not_exists(map) || map->capacity == 0;
}

static IntMapNode intmap_create_node(char* key, int value) {
    IntMapNode new_node = (IntMapNode) malloc(sizeof (struct intmapnode));
    if (new_node == NULL) return NULL;

    new_node->next = NULL;
    new_node->key = key;
    new_node->value = value;
    return new_node;
}

static void intmap_free_node(IntMapNode node) {
    if (node == NULL) return;

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

static uint32_t intmap_get_index(IntMap map, const char* key) {
    return intmap_hash(key) % map->capacity;
}

static bool intmap_resize(IntMap map) {
    size_t old_capacity = map->capacity;
    size_t new_capacity = old_capacity * GROWTH_FACTOR;

    IntMapNode* new_buckets = (IntMapNode*) calloc(new_capacity, sizeof (IntMapNode));
    if (new_buckets == NULL) return false;

    IntMapNode* old_buckets = map->buckets;
    size_t old_size = map->size;

    map->buckets = new_buckets;
    map->capacity = new_capacity;
    map->size = 0;
    
    for (size_t i = 0; i < old_capacity; i++) {
        IntMapNode curr = old_buckets[i];
        while (curr != NULL) {
            if (!intmap_insert(map, curr->key, curr->value)) {
                free(new_buckets);
                map->buckets = old_buckets;
                map->capacity = old_capacity;
                map->size = old_size;
                return false;
            }
            curr = curr->next;
        }
    }

    free(old_buckets);
    return true;
}

IntMap intmap_new(void) {
    IntMap new_map = (IntMap) malloc(sizeof (struct intmap));
    if (intmap_not_exists(new_map)) return NULL;

    IntMapNode* buckets = (IntMapNode*) calloc(INITIAL_CAPACITY, sizeof (IntMapNode));
    if (buckets == NULL) return NULL;

    if (!memmngr_register(new_map, (void (*)(void*)) intmap_free)) {
        free(buckets);
        free(new_map);
        return NULL;
    }

    new_map->buckets = buckets;
    new_map->capacity = INITIAL_CAPACITY;
    new_map->size = 0;
    return new_map;
}

void intmap_free(IntMap map) {
    if (intmap_not_exists(map)) return;

    for (size_t i = 0; i < map->capacity; i++) {
        IntMapNode curr = map->buckets[i];
        while (curr != NULL) {
            IntMapNode next = curr->next;

            intmap_free_node(curr);

            curr = next;
        }

    }
    free(map->buckets);
    map->capacity = 0;
    free(map);
}

bool intmap_insert(IntMap map, const char* key, int value) {
    if (intmap_not_exists(map)) return false;

    char* key_dup = (char*) malloc(strlen(key) + 1);
    if (key_dup == NULL) return false;

    strcpy(key_dup, key);
    
    uint32_t index = intmap_get_index(map, key);
    
    IntMapNode* head = &map->buckets[index];

    IntMapNode curr = *head;
    while (curr != NULL) { 
        if (strcmp(curr->key, key_dup) == 0) {
            free(key_dup);
            return false;
        }
        curr = curr->next;
    }
    
    IntMapNode new_node = intmap_create_node(key_dup, value);
    if(new_node == NULL) return false;

    new_node->next = *head;
    *head = new_node;

    map->size++;

    if ((double) map->size / map->capacity > MAX_LOAD_FACTOR) {
        return intmap_resize(map);
    }

    return true;
}

bool intmap_get(IntMap map, const char* key, int* out) {
    if (intmap_is_empty(map) || out == NULL) return false;

    uint32_t index = intmap_get_index(map, key);

    IntMapNode curr = map->buckets[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            *out = curr->value;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

void intmap_remove(IntMap map, const char* key) {
    if (intmap_is_empty(map)) return;

    uint32_t index = intmap_get_index(map, key);

    IntMapNode* head = &map->buckets[index];

    IntMapNode curr = *head;
    IntMapNode prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            if (prev == NULL) {
                *head = curr->next;
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

bool intmap_contains_key(IntMap map, const char* key) {
    if (intmap_is_empty(map)) return false;

    uint32_t index = intmap_get_index(map, key);

    IntMapNode curr = map->buckets[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) return true;
        curr = curr->next;
    }
    return false;
}

char** intmap_keys(IntMap map) {
    if (intmap_is_empty(map)) return NULL;

    char** keys = (char**) malloc(sizeof (char*) * map->size);
    if (keys == NULL) return NULL;

    for (size_t i = 0, j = 0; i < map->capacity; i++) {
        IntMapNode curr = map->buckets[i];
        while (curr != NULL) {
            char* key_copy = (char*) malloc(strlen(curr->key) + 1);
            if (key_copy == NULL) {
                for (size_t k = 0; k < j; k++) {
                    free(keys[k]);
                }
                free(keys);
                return NULL;
            }

            strcpy(key_copy, curr->key);
            keys[j++] = key_copy;

            curr = curr->next;
        }
    }
    return keys;
}

size_t intmap_len(IntMap map) {
    return intmap_not_exists(map) ? 0 : map->size;
}
