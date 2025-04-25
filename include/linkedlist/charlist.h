#ifndef CHARLIST_H
#define CHARLIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct charstack* CharStack;
typedef struct charqueue* CharQueue;

typedef struct charlist* CharList;

CharList charlist_new(void);
CharList charlist_from_string(const char* str, size_t size);
void charlist_clear(CharList list);
void charlist_free(CharList list);

bool charlist_push(CharList list, char value);
bool charlist_push_at(CharList list, char value, size_t index);
bool charlist_append(CharList list, char value);

void charlist_pop_start(CharList list);
void charlist_pop_at(CharList list, size_t index);
void charlist_pop(CharList list);

bool charlist_is_empty(CharList list);
size_t charlist_len(CharList list);
bool charlist_get_at(CharList list, size_t index, char* out);
int charlist_index(CharList list, char target);
size_t charlist_count(CharList list, char target);
bool charlist_contains(CharList list, char target);
bool charlist_equals(CharList list1, CharList list2);

// void charlist_sort(CharList list); 

CharList charlist_copy(CharList list);
CharList charlist_map(CharList list, char (*callback_func)(char value));
CharList charlist_filter(CharList list, bool (*predicate_func)(char value));
CharList charlist_zip(CharList list1, CharList list2);
char* charlist_to_string(CharList list);
CharStack charlist_to_stack(CharList list);
CharQueue charlist_to_queue(CharList list);

void charlist_reverse(CharList list);
void charlist_foreach(CharList list, char (*callback_func)(char value));
long long charlist_reduce(CharList list, long long (*reduce_func)(long long acc, char value), long long initial);
bool charlist_any(CharList list, bool (*predicate_func)(char value));
bool charlist_all(CharList list, bool (*predicate_func)(char value));
void charlist_print(CharList list);

#endif