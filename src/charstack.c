// #include <stdio.h>
#include <stdlib.h>
#include "stack/charstack.h"
#include "linkedlist/charlist.h"
#include "queue/charqueue.h"

typedef struct charnode {
    char value;
    struct charnode* next;
} *CharNode;

struct charstack {
    CharNode top;
    size_t size;
};

extern bool memmngr_register(void* dstruct, void (*destroy_func)(void* dstruct));
extern void memmngr_rollback(void);

static bool charstack_not_exists(CharStack stack) {
    return stack == NULL;
}

bool charstack_is_empty(CharStack stack) {
    return charstack_not_exists(stack) || stack->top == NULL;
}

static CharNode charstack_create_node(int value) {
    CharNode new_node = (CharNode) malloc(sizeof (struct charnode));
    if (new_node == NULL) return NULL;

    new_node->next = NULL;
    new_node->value = value;

    return new_node;
}

CharStack charstack_new(void) {
    CharStack new_stack = (CharStack) malloc(sizeof (struct charstack));
    if (charstack_not_exists(new_stack)) return NULL;
    
    if (!memmngr_register(new_stack, (void (*)(void*)) charstack_free)) {
        free(new_stack);
        return NULL;
    }

    new_stack->size = 0;
    new_stack->top = NULL;

    return new_stack;
}

void charstack_clear(CharStack stack) {
    if (charstack_is_empty(stack)) return;

    CharNode curr = stack->top;
    while (curr != NULL) {
        CharNode next = curr->next;

        free(curr);
        curr = next;
    }

    stack->top = NULL;
    stack->size = 0;
}

void charstack_free(CharStack stack) {
    charstack_clear(stack);
    free(stack);
}

bool charstack_push(CharStack stack, char value) {
    if (charstack_not_exists(stack)) return false;

    CharNode new_node = charstack_create_node(value);
    if (new_node == NULL) return false;

    new_node->next = stack->top;
    stack->top = new_node;

    stack->size++;
    return true;
}

bool charstack_pop(CharStack stack, char* out) {
    if (charstack_is_empty(stack)) return false;

    CharNode old_top = stack->top;
    
    if (out != NULL) *out = old_top->value;

    stack->top = old_top->next;
    free(old_top);

    stack->size--;

    return true;
}

bool charstack_peek(CharStack stack, char* out) {
    if (charstack_is_empty(stack) || out == NULL) return false;

    *out = stack->top->value;
    
    return true;
}

size_t charstack_len(CharStack stack) {
    return charstack_not_exists(stack) ? 0 : stack->size;
}

CharList charstack_to_list(CharStack stack) {
    if (charstack_is_empty(stack)) return NULL;

    CharList new_list = charlist_new();
    if (new_list == NULL) return NULL;

    CharNode curr = stack->top;
    while (curr != NULL) {
        if (!charlist_append(new_list, curr->value)) {
            memmngr_rollback();
            return NULL;
        }

        curr = curr->next;
    }

    return new_list;
}

CharQueue charstack_to_queue(CharStack stack) {
    if (charstack_is_empty(stack)) return NULL;
    
    CharQueue new_queue = charqueue_new();
    if (new_queue == NULL) return NULL;
    
    CharNode curr = stack->top;
    while (curr != NULL) {
        if (!charqueue_enqueue(new_queue, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }    

    return new_queue;
}