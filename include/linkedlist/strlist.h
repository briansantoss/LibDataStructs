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

bool strlist_push_front(StrList list, const char* value, size_t str_size);
bool strlist_push_at(StrList list, const char* value, size_t str_size, size_t index);
bool strlist_push(StrList list, const char* value, size_t str_size);

void strlist_pop_front(StrList list);
void strlist_pop_at(StrList list, size_t index);
void strlist_pop(StrList list);

bool strlist_is_empty(const StrList list);
size_t strlist_size(const StrList list);
char* strlist_get_at(const StrList list, size_t index);
int strlist_index(const StrList list, const char* target);
size_t strlist_count(const StrList list, const char* target);
bool strlist_contains(const StrList list, const char* target);
bool strlist_equals(const StrList list1, const StrList list2);

StrList strlist_copy(const StrList list);
StrList strlist_zip(const StrList list1, const StrList list2);

void strlist_reverse(StrList list);
bool strlist_any(const StrList list, bool (*predicate_func)(const char* value));
bool strlist_all(const StrList list, bool (*predicate_func)(const char* value));
void strlist_print(const StrList list);

#endif // STRLIST_H