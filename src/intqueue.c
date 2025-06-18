#include "queue/intqueue.h"
#include <stdlib.h>
#include "linkedlist/intlist.h"
#include "stack/intstack.h"
#include "internal/memmngr.h"


typedef struct _intnode {
    int value;
    struct _intnode* next;
} *IntNode;

struct _intqueue {
    IntNode front;
    IntNode rear;
    size_t size;
};

static bool _intqueue_not_exists(const IntQueue queue) {
    return !queue;
}

bool intqueue_is_empty(const IntQueue queue) {
    return _intqueue_not_exists(queue) || !queue->front;
}

static IntNode _intqueue_create_node(int value, IntNode next) {
    IntNode new_node = (IntNode) malloc(sizeof (struct _intnode));
    if (!new_node) return NULL;

    *new_node = (struct _intnode) {.value = value, .next = next};
    return new_node;
}

static void _intqueue_free(IntQueue queue) {
    intqueue_clear(queue);
    free(queue);
}

IntQueue intqueue_new(void) {
    IntQueue new_queue = (IntQueue) malloc(sizeof (struct _intqueue));
    if (_intqueue_not_exists(new_queue)) return NULL;

    if (!_memmngr_register(new_queue, (void (*)(void*)) _intqueue_free)) {
        free(new_queue);
        return NULL;
    }

    *new_queue = (struct _intqueue) {.front = NULL, .rear = NULL, .size = 0};
    return new_queue;
}

void intqueue_clear(IntQueue queue) {
    if (intqueue_is_empty(queue)) return;

    for (IntNode curr = queue->front, next; curr; curr = next) {
        next = curr->next;
        free(curr);
    }

    queue->front = queue->rear = NULL;
    queue->size = 0;
}

bool intqueue_enqueue(IntQueue queue, int value) {
    if (_intqueue_not_exists(queue)) return false;

    IntNode new_node = _intqueue_create_node(value, NULL);
    if (!new_node) return false;

    IntNode rear = queue->rear;
    queue->rear = new_node;
    if (!rear)  queue->front = new_node;
    else        rear->next = new_node;

    queue->size++;
    return true;
}

bool intqueue_dequeue(IntQueue queue, int* out) {
    if (intqueue_is_empty(queue)) return false;

    IntNode front = queue->front;
    if (out) *out = front->value;

    queue->front = front->next;
    if (!queue->front) queue->rear = NULL;

    free(front);

    queue->size--;
    return true;
}

bool intqueue_peek(const IntQueue queue, int* out) {
    if (intqueue_is_empty(queue) || !out) return false;
    *out = queue->front->value;
    return true;
}

size_t intqueue_size(IntQueue queue) {
    return _intqueue_not_exists(queue) ? 0 : queue->size;
}

IntList intqueue_to_list(const IntQueue queue) {
    if (_intqueue_not_exists(queue)) return NULL;

    IntList new_list = intlist_new();
    if (!new_list) return NULL;

    for (IntNode curr = queue->front; curr; curr = curr->next) {
        if (!intlist_push(new_list, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

IntStack intqueue_to_stack(const IntQueue queue) {
    if (_intqueue_not_exists(queue)) return NULL;

    IntStack new_stack = intstack_new();
    if (!new_stack) return NULL;

    for (IntNode curr = queue->front; curr; curr = curr->next) {
        if (!intstack_push(new_stack, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_stack;
}