#include "stack/charstack.h"
#include <stdlib.h>
#include "linkedlist/charlist.h"
#include "queue/charqueue.h"
#include "internal/memmngr.h"

typedef struct _charnode {
    char value;
    struct _charnode* next;
} *CharNode;

struct _charstack {
    CharNode top;
    size_t size;
};

static bool _charstack_not_exists(const CharStack stack) {
    return !stack;
}

bool charstack_is_empty(const CharStack stack) {
    return _charstack_not_exists(stack) || !stack->top;
}

static CharNode _charstack_create_node(int value) {
    CharNode new_node = (CharNode) malloc(sizeof (struct _charnode));
    if (!new_node) return NULL;

    *new_node = (struct _charnode) {.value = value, .next = NULL};
    return new_node;
}

static void _charstack_free(CharStack stack) {
    charstack_clear(stack);
    free(stack);
}

CharStack charstack_new(void) {
    CharStack new_stack = (CharStack) malloc(sizeof (struct _charstack));
    if (_charstack_not_exists(new_stack)) return NULL;
    
    if (!_memmngr_register(new_stack, (void (*)(void*)) _charstack_free)) {
        free(new_stack);
        return NULL;
    }

    *new_stack = (struct _charstack) {.top = NULL, .size = 0};
    return new_stack;
}

void charstack_clear(CharStack stack) {
    if (charstack_is_empty(stack)) return;

    for (CharNode curr = stack->top, next; curr; curr = next) {
        next = curr->next;
        free(curr);
    }

    stack->top = NULL;
    stack->size = 0;
}

bool charstack_push(CharStack stack, char value) {
    if (_charstack_not_exists(stack)) return false;

    CharNode new_node = _charstack_create_node(value);
    if (!new_node) return false;

    new_node->next = stack->top;
    stack->top = new_node;

    stack->size++;
    return true;
}

bool charstack_pop(CharStack stack, char* out) {
    if (charstack_is_empty(stack)) return false;

    CharNode top = stack->top;
    if (out) *out = top->value;

    stack->top = top->next;
    free(top);

    stack->size--;
    return true;
}

bool charstack_peek(const CharStack stack, char* out) {
    if (charstack_is_empty(stack) || !out) return false;
    *out = stack->top->value;
    return true;
}

size_t charstack_size(const CharStack stack) {
    return _charstack_not_exists(stack) ? 0 : stack->size;
}

CharList charstack_to_list(const CharStack stack) {
    if (_charstack_not_exists(stack)) return NULL;

    CharList new_list = charlist_new();
    if (!new_list) return NULL;

    for (CharNode curr = stack->top; curr; curr = curr->next) {
        if (!charlist_push(new_list, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

CharQueue charstack_to_queue(const CharStack stack) {
    if (_charstack_not_exists(stack)) return NULL;
    
    CharQueue new_queue = charqueue_new();
    if (!new_queue) return NULL;
    
    for (CharNode curr = stack->top; curr; curr = curr->next) {
        if (!charqueue_enqueue(new_queue, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }    
    return new_queue;
}