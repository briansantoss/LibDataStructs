#ifndef INTSTACK_H

#define INTSTACK_H

#include <stdbool.h>
#include <stddef.h>

typedef struct intstack* IntStack;
typedef struct intlist* IntList;

IntStack intstack_new(void);
bool intstack_is_empty(IntStack stack);
void intstack_clear(IntStack stack);
void intstack_free(IntStack stack);
void intstack_push(IntStack stack, int value);
bool intstack_pop(IntStack stack, int* out);
bool intstack_peek(IntStack stack, int* out);

size_t intstack_len(IntStack stack);

IntList intstack_to_list(IntStack stack);

// void intstack_display(IntStack stack);

#endif