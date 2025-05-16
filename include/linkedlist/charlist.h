#ifndef CHARLIST_H
#define CHARLIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct charstack* CharStack;
typedef struct charqueue* CharQueue;

typedef struct charlist* CharList;

CharList charlist_new(void);
CharList charlist_from_string(char* str, size_t size);
void charlist_clear(CharList list);

bool charlist_push_front(CharList list, char value);
bool charlist_push_at(CharList list, char value, size_t index);
bool charlist_push(CharList list, char value);

void charlist_pop_front(CharList list);
void charlist_pop_at(CharList list, size_t index);
void charlist_pop(CharList list);

bool charlist_is_empty(const CharList list);
size_t charlist_size(const CharList list);
bool charlist_get_at(const CharList list, size_t index, char* out);
int charlist_index(const CharList list, char target);
size_t charlist_count(const CharList list, char target);
bool charlist_contains(const CharList list, char target);
bool charlist_equals(const CharList list1, const CharList list2);

CharList charlist_copy(const CharList list);
CharList charlist_map(const CharList list, char (*callback_func)(char value));
CharList charlist_filter(const CharList list, bool (*predicate_func)(char value));
CharList charlist_zip(const CharList list1, const CharList list2);
char* charlist_to_string(const CharList list);
CharStack charlist_to_stack(const CharList list);
CharQueue charlist_to_queue(const CharList list);

void charlist_reverse(CharList list);
void charlist_foreach(CharList list, char (*callback_func)(char value));
long long charlist_reduce(const CharList list, long long (*reduce_func)(long long acc, char value), long long initial);
bool charlist_any(const CharList list, bool (*predicate_func)(char value));
bool charlist_all(const CharList list, bool (*predicate_func)(char value));
void charlist_print(const CharList list);

#endif // CHARLIST_H