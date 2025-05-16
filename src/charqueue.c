#include "queue/charqueue.h"
#include <stdlib.h>
#include "linkedlist/charlist.h"
#include "stack/charstack.h"
#include "internal/memmngr.h"


typedef struct charnode {
    char value;
    struct charnode* next;
} *CharNode;

struct charqueue {
    CharNode front;
    CharNode rear;
    size_t size;
};

static bool charqueue_not_exists(const CharQueue queue) {
    return !queue;
}

bool charqueue_is_empty(const CharQueue queue) {
    return charqueue_not_exists(queue) || !queue->front;
}

static CharNode charqueue_create_node(char value) {
    CharNode new_node = (CharNode) malloc(sizeof (struct charnode));
    if (!new_node) return NULL;

    *new_node = (struct charnode) {.value = value, .next = NULL};
    return new_node;
}

static void charqueue_free(CharQueue queue) {
    charqueue_clear(queue);
    free(queue);
}

CharQueue charqueue_new(void) {
    CharQueue new_queue = (CharQueue) malloc(sizeof (struct charqueue));
    if (charqueue_not_exists(new_queue)) return NULL;

    if (!memmngr_register(new_queue, (void (*)(void*)) charqueue_free)) {
        free(new_queue);
        return NULL;
    }

    *new_queue = (struct charqueue) {.front = NULL, .rear = NULL, .size = 0};
    return new_queue;
}

void charqueue_clear(CharQueue queue) {
    if (charqueue_is_empty(queue)) return;

    CharNode curr = queue->front;
    while (curr) {
        CharNode next = curr->next;

        free(curr);
        
        curr = next;
    }

    queue->front = queue->rear = NULL;
    queue->size = 0;
}

bool charqueue_enqueue(CharQueue queue, char value) {
    if (charqueue_not_exists(queue)) return false;

    CharNode new_node = charqueue_create_node(value);
    if (!new_node) return false;

    if (charqueue_is_empty(queue)) {
        queue->front = queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }

    queue->size++;

    return true;
}

bool charqueue_dequeue(CharQueue queue, char* out) {
    if (charqueue_is_empty(queue)) return false;

    CharNode old_front = queue->front;

    if (out) *out = old_front->value;

    if (queue->size == 1) {
        queue->front = queue->rear = NULL;
    } else {
        queue->front = old_front->next;
    }
    
    free(old_front);

    queue->size--;

    return true;
}

bool charqueue_peek(CharQueue queue, char* out) {
    if (charqueue_is_empty(queue) || !out) return false;

    *out = queue->front->value;
    return true;
}

size_t charqueue_size(const CharQueue queue) {
    return charqueue_not_exists(queue) ? 0 : queue->size;
}

CharList charqueue_to_list(const CharQueue queue) {
    if (charqueue_is_empty(queue)) return NULL;

    CharList new_list = charlist_new();
    if (!new_list) return NULL;

    CharNode curr = queue->front;
    while (curr) {
        if (!charlist_push(new_list, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }

    return new_list;
}

CharStack charqueue_to_stack(const CharQueue queue) {
    if (charqueue_is_empty(queue)) return NULL;

    CharStack new_stack = charstack_new();
    if (!new_stack) return NULL;

    CharNode curr = queue->front;
    while (curr) {
        if (!charstack_push(new_stack, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }

    return new_stack;
}