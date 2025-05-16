#ifndef INTSTACK_H
#define INTSTACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct intlist* IntList;
typedef struct intqueue* IntQueue;

typedef struct intstack* IntStack;

IntStack intstack_new(void);
bool intstack_is_empty(const IntStack stack);
void intstack_clear(IntStack stack);

bool intstack_push(IntStack stack, int value);
bool intstack_pop(IntStack stack, int* out);
bool intstack_peek(const IntStack stack, int* out);

size_t intstack_size(IntStack stack);

IntList intstack_to_list(const IntStack stack);
IntQueue intstack_to_queue(const IntStack stack);

#endif // INTSTACK_H