#ifndef INTQUEUE_H

#define INTQUEUE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct intlist* IntList;
typedef struct intstack* IntStack;

typedef struct intqueue* IntQueue;

IntQueue intqueue_new(void);
bool intqueue_is_empty(const IntQueue queue);
void intqueue_clear(IntQueue queue);

bool intqueue_enqueue(IntQueue queue, int value);
bool intqueue_dequeue(IntQueue queue, int* out);
bool intqueue_peek(const IntQueue queue, int* out);

size_t intqueue_size(const IntQueue queue);

IntList intqueue_to_list(const IntQueue queue);
IntStack intqueue_to_stack(const IntQueue queue);

#endif // INTQUEUE_H