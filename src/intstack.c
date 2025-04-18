#include <stdlib.h>
#include "intstack.h"

typedef struct intnode {
    int value;
    struct intnode* next;
} *IntNode;

struct intstack {
    IntNode top;
    size_t size;
};

static int intstack_not_exists(IntStack stack) {
    return stack == NULL;
}

bool intstack_is_empty(IntStack stack) {
    return intstack_not_exists(stack) || stack->top == NULL;
}

static IntNode intstack_create_node(int value) {
    IntNode new_node = (IntNode) malloc(sizeof (struct intnode));
    if (new_node == NULL) return NULL;

    new_node->next = NULL;
    new_node->value = value;
    return new_node;
}

IntStack intstack_init(void) {
    IntStack new_stack = (IntStack) malloc(sizeof (struct intstack));
    if (intstack_not_exists(new_stack)) return NULL;

    new_stack->size = 0;
    new_stack->top = NULL;
    return new_stack;
}

void intstack_clear(IntStack stack) {
    if (intstack_is_empty(stack)) return;

    IntNode curr = stack->top;
    while (curr != NULL) {
        IntNode next = curr->next;

        free(curr);
        curr = next;
    }

    stack->top = NULL;
    stack->size = 0;
}

void intstack_free(IntStack stack) {
    intstack_clear(stack);
    free(stack);
}

void intstack_push(IntStack stack, int value) {
    if (intstack_not_exists(stack)) return;

    IntNode new_node = intstack_create_node(value);
    if (new_node == NULL) return;

    new_node->next = stack->top;
    stack->top = new_node;

    stack->size++;
}

bool intstack_pop(IntStack stack, int* out) {
    if (intstack_is_empty(stack)) return false;

    IntNode old_top = stack->top;
    int old_top_value = old_top->value;

    stack->top = old_top->next;
    free(old_top);

    *out = old_top_value;

    return true;
}

bool intstack_peek(IntStack stack, int* out) {
    if (intstack_is_empty(stack)) return false;

    *out = stack->top->value;
    
    return true;
}

size_t intstack_len(IntStack stack) {
    if (intstack_not_exists(stack)) return 0;

    return stack->size;
}