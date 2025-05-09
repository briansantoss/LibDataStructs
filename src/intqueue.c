#include "queue/intqueue.h"
#include <stdlib.h>
#include "linkedlist/intlist.h"
#include "stack/intstack.h"
#include "internal/memmngr.h"


typedef struct intnode {
    int value;
    struct intnode* next;
} *IntNode;

struct intqueue {
    IntNode front;
    IntNode rear;
    size_t size;
};

static bool intqueue_not_exists(const IntQueue queue) {
    return !queue;
}

bool intqueue_is_empty(const IntQueue queue) {
    return intqueue_not_exists(queue) || !queue->front;
}

static IntNode intqueue_create_node(int value) {
    IntNode new_node = (IntNode) malloc(sizeof (struct intnode));
    if (!new_node) return NULL;

    *new_node = (struct intnode) {.value = value, .next = NULL};
    return new_node;
}

IntQueue intqueue_new(void) {
    IntQueue new_queue = (IntQueue) malloc(sizeof (struct intqueue));
    if (intqueue_not_exists(new_queue)) return NULL;

    if (!memmngr_register(new_queue, (void (*)(void*)) intqueue_free)) {
        free(new_queue);
        return NULL;
    }

    *new_queue = (struct intqueue) {.front = NULL, .rear = NULL, .size = 0};
    return new_queue;
}

void intqueue_clear(IntQueue queue) {
    if (intqueue_is_empty(queue)) return;

    IntNode curr = queue->front;
    while (curr) {
        IntNode next = curr->next;

        free(curr);
        
        curr = next;
    }

    queue->front = queue->rear = NULL;
    queue->size = 0;
}

void intqueue_free(IntQueue queue) {
    intqueue_clear(queue);
    free(queue);
}

bool intqueue_enqueue(IntQueue queue, int value) {
    if (intqueue_not_exists(queue)) return false;

    IntNode new_node = intqueue_create_node(value);
    if (!new_node) return false;

    if (intqueue_is_empty(queue)) {
        queue->front = queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }

    queue->size++;

    return true;
}

bool intqueue_dequeue(IntQueue queue, int* out) {
    if (intqueue_is_empty(queue)) return false;

    IntNode old_front = queue->front;

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

bool intqueue_peek(const IntQueue queue, int* out) {
    if (intqueue_is_empty(queue) || !out) return false;

    *out = queue->front->value;
    return true;
}

size_t intqueue_size(IntQueue queue) {
    return intqueue_not_exists(queue) ? 0 : queue->size;
}

IntList intqueue_to_list(const IntQueue queue) {
    if (intqueue_is_empty(queue)) return NULL;

    IntList new_list = intlist_new();
    if (!new_list) return NULL;

    IntNode curr = queue->front;
    while (curr) {
        if (!intlist_push(new_list, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }

    return new_list;
}

IntStack intqueue_to_stack(const IntQueue queue) {
    if (intqueue_is_empty(queue)) return NULL;

    IntStack new_stack = intstack_new();
    if (!new_stack) return NULL;

    IntNode curr = queue->front;
    while (curr) {
        if (!intstack_push(new_stack, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }

    return new_stack;
}