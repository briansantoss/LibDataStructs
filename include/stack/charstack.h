#ifndef CHARSTACK_H
#define CHARSTACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct _charlist* CharList;
typedef struct _charqueue* CharQueue;

typedef struct _charstack* CharStack;

CharStack charstack_new(void);
bool charstack_is_empty(const CharStack stack);
void charstack_clear(CharStack stack);

bool charstack_push(CharStack stack, char value);
bool charstack_pop(CharStack stack, char* out);
bool charstack_peek(const CharStack stack, char* out);

size_t charstack_size(const CharStack stack);

CharList charstack_to_list(const CharStack stack);
CharQueue charstack_to_queue(const CharStack stack);

#endif // CHARSTACK_H