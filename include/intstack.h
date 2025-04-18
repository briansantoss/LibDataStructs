#ifndef INTSTACK_H

#define INTSTACK_H

#include <stddef.h>

typedef struct intstack* IntStack;

IntStack intstack_init(void);
int intstack_is_empty(IntStack stack);
void intstack_clear(IntStack stack);
void intstack_free(IntStack stack);
void intstack_push(IntStack stack, int value);
int intstack_pop(IntStack stack);
int intstack_peek(IntStack stack);

size_t intstack_len(IntStack stack);

#endif