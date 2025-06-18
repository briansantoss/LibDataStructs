#ifndef INTMAP_H
#define INTMAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct keyvaluepair {
    char* key;
    int value;
} KeyValuePair;

typedef struct _intmapiter* IntMapIter;
typedef struct _intmap* IntMap;

IntMap intmap_new(void);
void intmap_clear(IntMap map);

bool intmap_insert(IntMap map, const char* key, int value);
bool intmap_get(const IntMap map, const char* key, int* out);
bool intmap_set(IntMap map, const char* key, int new_value);
void intmap_remove(IntMap map, const char* key);

bool intmap_is_empty(const IntMap map);
char** intmap_keys(const IntMap map);
int* intmap_values(const IntMap map);
bool intmap_has_key(const IntMap map, const char* key);
bool intmap_equals(const IntMap map1, const IntMap map2);
uint32_t intmap_size(const IntMap map);

IntMapIter intmap_iter_new(const IntMap map);
bool intmap_iter_next(IntMapIter iter, KeyValuePair* out);
void intmap_iter_reset(IntMapIter iter);

#endif // INTMAP_H