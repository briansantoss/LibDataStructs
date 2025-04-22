#ifndef CHARSTACK_H
#define CHARSTACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct charlist* CharList;
typedef struct charqueue* CharQueue;

typedef struct charstack* CharStack;

CharStack charstack_new(void);
bool charstack_is_empty(CharStack stack);
void charstack_clear(CharStack stack);
void charstack_free(CharStack stack);
void charstack_push(CharStack stack, char value);
bool charstack_pop(CharStack stack, char* out);
bool charstack_peek(CharStack stack, char* out);

size_t charstack_len(CharStack stack);

CharList charstack_to_list(CharStack stack);

// void charstack_print(CharStack stack);

#endif