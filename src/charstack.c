// #include <stdio.h>
#include <stdlib.h>
#include "stack/charstack.h"
#include "linkedlist/charlist.h"

typedef struct charnode {
    char value;
    struct charnode* next;
} *CharNode;

struct charstack {
    CharNode top;
    size_t size;
};

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

void charstack_push(CharStack stack, char value) {
    if (charstack_not_exists(stack)) return;

    CharNode new_node = charstack_create_node(value);
    if (new_node == NULL) return;

    new_node->next = stack->top;
    stack->top = new_node;

    stack->size++;
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

CharList charstack_to_list(CharStack stack) {
    if (charstack_is_empty(stack)) return NULL;

    CharList new_list = charlist_new();
    if (new_list == NULL) return NULL;

    CharNode curr = stack->top;
    while (curr != NULL) {
        charlist_append(new_list, curr->value);

        curr = curr->next;
    }

    return new_list;
}

size_t charstack_len(CharStack stack) {
    return charstack_not_exists(stack) ? 0 : stack->size;
}