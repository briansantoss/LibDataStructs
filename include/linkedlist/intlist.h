#ifndef INTLIST_H

#define INTLIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct intlist* IntList;
typedef struct intstack* IntStack;

IntList intlist_init(void);
IntList intlist_from_array(int* arr, size_t size);
void intlist_clear(IntList list);
void intlist_free(IntList list);

void intlist_push(IntList list, int value);
void intlist_push_at(IntList list, int value, size_t index);
void intlist_append(IntList list, int value);

void intlist_pop_start(IntList list);
void intlist_pop_at(IntList list, size_t index);
void intlist_pop(IntList list);

bool intlist_is_empty(IntList list);
size_t intlist_len(IntList list);
int intlist_get_at(IntList list, size_t index);
int intlist_find(IntList list, int target);
size_t intlist_count(IntList list, int target);
bool intlist_contains(IntList list, int target);
bool intlist_equals(IntList list1, IntList list2);

// void intlist_sort(IntList list); 

IntList intlist_map(IntList list, int (*callback_func)(int value));
IntList intlist_filter(IntList list, bool (*predicate_func)(int value));
int* intlist_to_array(IntList list);
IntStack intlist_to_stack(IntList list);

void intlist_reverse(IntList list);
void intlist_foreach(IntList list, int (*callback_func)(int value));
long long intlist_reduce(IntList list, long long (*reduce_func)(long long acc, int value), long long initial);
bool intlist_any(IntList list, bool (*predicate_func)(int value));
bool intlist_all(IntList list, bool (*predicate_func)(int value));
long long intlist_sum(IntList list);
void intlist_print(IntList list);

#endif