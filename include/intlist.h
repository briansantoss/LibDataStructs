#ifndef INTLIST_H

#include <stddef.h>

typedef struct intlist* IntList;

IntList intlist_init(void);
void intlist_free(IntList list);
void intlist_clear(IntList list);

void intlist_push(IntList list, int value);
void intlist_push_end(IntList list, int value);
void intlist_insert_at(IntList list, int value, size_t index);
int intlist_get_at(IntList list, size_t index);

size_t intlist_len(IntList list);
void intlist_reverse(IntList list);
int intlist_contains(IntList list, int target);
void intlist_foreach(IntList list, int (*callback_func)(int value));

#endif