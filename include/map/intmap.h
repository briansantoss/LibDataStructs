#ifndef INTMAP_H
#define INTMAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct intmap* IntMap;

IntMap intmap_new(void);
void intmap_clear(IntMap map);
void intmap_free(IntMap map);

bool intmap_insert(IntMap map, const char* key, int value);
bool intmap_get(IntMap map, const char* key, int* out);
bool intmap_set(IntMap map, const char* key, int new_value);
void intmap_remove(IntMap map, const char* key);

bool intmap_is_empty(IntMap map);
char** intmap_keys(IntMap map);
int* intmap_values(IntMap map);
bool intmap_contains_key(IntMap map, const char* key);

size_t intmap_len(IntMap map);

#endif