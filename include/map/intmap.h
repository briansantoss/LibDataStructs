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
bool intmap_get(const IntMap map, const char* key, int* out);
bool intmap_set(IntMap map, const char* key, int new_value);
void intmap_remove(IntMap map, const char* key);

bool intmap_is_empty(const IntMap map);
char** intmap_keys(const IntMap map);
int* intmap_values(const IntMap map);
bool intmap_has_key(const IntMap map, const char* key);
bool intmap_equals(const IntMap map1, const IntMap map2);
uint32_t intmap_size(const IntMap map);

#endif // INTMAP_H