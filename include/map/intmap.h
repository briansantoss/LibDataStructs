#ifndef INTMAP_H
#define INTMAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct intmap* IntMap;

IntMap intmap_new(void);
void intmap_free(IntMap map);

bool intmap_insert(IntMap map, const char* key, int value);
bool intmap_get(IntMap map, const char* key, int* out);
void intmap_remove(IntMap map, const char* key);
bool intmap_contains_key(IntMap map, const char* key);
char** intmap_keys(IntMap map);

size_t intmap_len(IntMap map);

#endif