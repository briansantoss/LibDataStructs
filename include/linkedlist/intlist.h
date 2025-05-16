#ifndef INTLIST_H
#define INTLIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct intstack* IntStack;
typedef struct intqueue* IntQueue;

typedef struct intlist* IntList;

IntList intlist_new(void);
IntList intlist_from_array(int* arr, size_t size);
void intlist_clear(IntList list);

bool intlist_push_front(IntList list, int value);
bool intlist_push_at(IntList list, int value, size_t index);
bool intlist_push(IntList list, int value);

void intlist_pop_front(IntList list);
void intlist_pop_at(IntList list, size_t index);
void intlist_pop(IntList list);

bool intlist_is_empty(const IntList list);
size_t intlist_size(const IntList list);
bool intlist_get_at(const IntList list, size_t index, int* out);
int intlist_index(const IntList list, int target);
size_t intlist_count(const IntList list, int target);
bool intlist_contains(const IntList list, int target);
bool intlist_equals(const IntList list1, const IntList list2);

IntList intlist_copy(const IntList list);
IntList intlist_map(const IntList list, int (*callback_func)(int value));
IntList intlist_filter(const IntList list, bool (*predicate_func)(int value));
IntList intlist_zip(const IntList list1, const IntList list2);
int* intlist_to_array(const IntList list);
IntStack intlist_to_stack(const IntList list);
IntQueue intlist_to_queue(const IntList list);

void intlist_reverse(IntList list);
void intlist_foreach(IntList list, int (*callback_func)(int value));
long long intlist_reduce(const IntList list, long long (*reduce_func)(long long acc, int value), long long initial);
bool intlist_any(const IntList list, bool (*predicate_func)(int value));
bool intlist_all(const IntList list, bool (*predicate_func)(int value));
long long intlist_sum(const IntList list);
void intlist_print(const IntList list);

#endif // INTLIST_H