#include "stack/intstack.h"
#include <stdio.h>
#include <stdlib.h>
#include "linkedlist/intlist.h"
#include "queue/intqueue.h"
#include "internal/memmngr.h"


typedef struct intnode {
    int value;
    struct intnode* next;
} *IntNode;

struct intstack {
    IntNode top;
    size_t size;
};

static bool intstack_not_exists(IntStack stack) {
    return stack == NULL;
}

bool intstack_is_empty(IntStack stack) {
    return intstack_not_exists(stack) || stack->top == NULL;
}

static IntNode intstack_create_node(int value) {
    IntNode new_node = (IntNode) malloc(sizeof (struct intnode));
    if (new_node == NULL) return NULL;

    *new_node = (struct intnode) {.value = value, .next = NULL};
    return new_node;
}

IntStack intstack_new(void) {
    IntStack new_stack = (IntStack) malloc(sizeof (struct intstack));
    if (intstack_not_exists(new_stack)) return NULL;

    if (!memmngr_register(new_stack, (void (*)(void*)) intstack_free)) {
        free(new_stack);
        return NULL;
    }

    *new_stack = (struct intstack) {.top = NULL, .size = 0};
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

bool intstack_push(IntStack stack, int value) {
    if (intstack_not_exists(stack)) return false;

    IntNode new_node = intstack_create_node(value);
    if (new_node == NULL) return false;

    new_node->next = stack->top;
    stack->top = new_node;

    stack->size++;
    
    return true;
}

bool intstack_pop(IntStack stack, int* out) {
    if (intstack_is_empty(stack)) return false;

    IntNode old_top = stack->top;
    
    if (out != NULL) *out = old_top->value;

    stack->top = old_top->next;
    free(old_top);

    stack->size--;

    return true;
}

bool intstack_peek(IntStack stack, int* out) {
    if (intstack_is_empty(stack) || out == NULL) return false;

    *out = stack->top->value;
    
    return true;
}

size_t intstack_len(IntStack stack) {
    return intstack_not_exists(stack) ? 0 : stack->size;
}

IntList intstack_to_list(IntStack stack) {
    if (intstack_is_empty(stack)) return NULL;

    IntList new_list = intlist_new();
    if (new_list == NULL) return NULL;

    IntNode curr = stack->top;
    while (curr != NULL) {
        if (!intlist_append(new_list, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }

    return new_list;
}

IntQueue intstack_to_queue(IntStack stack) {
    if (intstack_is_empty(stack)) return NULL;
    
    IntQueue new_queue = intqueue_new();
    if (new_queue == NULL) return NULL;
    
    IntNode curr = stack->top;
    while (curr != NULL) {
        if (!intqueue_enqueue(new_queue, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }    

    return new_queue;
}