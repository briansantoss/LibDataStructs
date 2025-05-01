#ifndef STRLIST_H
#define STRLIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct strstack* StrStack;
typedef struct strqueue* StrSQueue;

typedef struct strlist* StrList;

StrList strlist_new(void);
void strlist_clear(StrList list);
void strlist_free(StrList list);

bool strlist_push(StrList list, const char* value, size_t str_size);
bool strlist_push_at(StrList list, const char* value, size_t str_size, size_t index);
bool strlist_append(StrList list, const char* value, size_t str_size);

void strlist_pop_start(StrList list);
void strlist_pop_at(StrList list, size_t index);
void strlist_pop(StrList list);

bool strlist_is_empty(StrList list);
size_t strlist_len(StrList list);
char* strlist_get_at(StrList list, size_t index);
int strlist_index(StrList list, const char* target);
size_t strlist_count(StrList list, const char* target);
bool strlist_contains(StrList list, const char* target);
bool strlist_equals(StrList list1, StrList list2);

StrList strlist_copy(StrList list);
StrList strlist_zip(StrList list1, StrList list2);

void strlist_reverse(StrList list);
bool strlist_any(StrList list, bool (*predicate_func)(const char* value));
bool strlist_all(StrList list, bool (*predicate_func)(const char* value));
void strlist_print(StrList list);

#endif