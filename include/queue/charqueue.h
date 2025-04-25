#ifndef CHARQUEUE_H

#define CHARQUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct charlist* CharList;
typedef struct charstack* CharStack;

typedef struct charqueue* CharQueue;

CharQueue charqueue_new(void);
bool charqueue_is_empty(CharQueue queue);
void charqueue_clear(CharQueue queue);
void charqueue_free(CharQueue queue);

bool charqueue_enqueue(CharQueue queue, char value);
bool charqueue_dequeue(CharQueue queue, char* out);
bool charqueue_peek(CharQueue queue, char* out);

size_t charqueue_len(CharQueue queue);

CharList charqueue_to_list(CharQueue queue);
CharStack charqueue_to_stack(CharQueue queue);

#endif