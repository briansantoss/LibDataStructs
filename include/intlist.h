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
void intlist_pop_start(IntList list);
// TODO: implementar
// void intlist_pop(Intlist list);
// void intlist_remove_at(Intlist list, size_t index);

size_t intlist_len(IntList list);
void intlist_reverse(IntList list);
int intlist_contains(IntList list, int target);
int intlist_equals(IntList list1, IntList list2);

// void intlist_sort(IntList list); 

void intlist_foreach(IntList list, int (*callback_func)(int value));
IntList intlist_map(IntList list, int (*callback_func)(int value));
IntList intlist_filter(IntList list, int (*callback_func)(int value));

#endif