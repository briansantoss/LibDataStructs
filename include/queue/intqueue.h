#ifndef INTQUEUE_H

#define INTQUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct intlist* IntList;
typedef struct intstack* IntStack;

typedef struct intqueue* IntQueue;

IntQueue intqueue_new(void);
bool intqueue_is_empty(IntQueue queue);
void intqueue_clear(IntQueue queue);
void intqueue_free(IntQueue queue);

bool intqueue_enqueue(IntQueue queue, int value);
bool intqueue_dequeue(IntQueue queue, int* out);
bool intqueue_peek(IntQueue queue, int* out);

size_t intqueue_len(IntQueue queue);

IntList intqueue_to_list(IntQueue queue);
IntStack intqueue_to_stack(IntQueue queue);

#endif