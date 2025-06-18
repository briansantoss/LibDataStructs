#include "queue/charqueue.h"
#include <stdlib.h>
#include "linkedlist/charlist.h"
#include "stack/charstack.h"
#include "internal/memmngr.h"

typedef struct _charnode {
    char value;
    struct _charnode* next;
} *CharNode;

struct _charqueue {
    CharNode front;
    CharNode rear;
    size_t size;
};

static bool _charqueue_not_exists(const CharQueue queue) {
    return !queue;
}

bool charqueue_is_empty(const CharQueue queue) {
    return _charqueue_not_exists(queue) || !queue->front;
}

static CharNode _charqueue_create_node(char value, CharNode next) {
    CharNode new_node = (CharNode) malloc(sizeof (struct _charnode));
    if (!new_node) return NULL;

    *new_node = (struct _charnode) {.value = value, .next = next};
    return new_node;
}

static void _charqueue_free(CharQueue queue) {
    charqueue_clear(queue);
    free(queue);
}

CharQueue charqueue_new(void) {
    CharQueue new_queue = (CharQueue) malloc(sizeof (struct _charqueue));
    if (_charqueue_not_exists(new_queue)) return NULL;

    if (!_memmngr_register(new_queue, (void (*)(void*)) _charqueue_free)) {
        free(new_queue);
        return NULL;
    }

    *new_queue = (struct _charqueue) {.front = NULL, .rear = NULL, .size = 0};
    return new_queue;
}

void charqueue_clear(CharQueue queue) {
    if (charqueue_is_empty(queue)) return;

    for (CharNode curr = queue->front, next; curr; curr = next) {
        next = curr->next;
        free(curr);
    }
    
    queue->front = queue->rear = NULL;
    queue->size = 0;
}

bool charqueue_enqueue(CharQueue queue, char value) {
    if (_charqueue_not_exists(queue)) return false;

    CharNode new_node = _charqueue_create_node(value, NULL);
    if (!new_node) return false;

    CharNode rear = queue->rear;
    queue->rear = new_node;
    if (!rear)  queue->front = new_node;
    else        rear->next = new_node;

    queue->size++;
    return true;
}

bool charqueue_dequeue(CharQueue queue, char* out) {
    if (charqueue_is_empty(queue)) return false;

    CharNode front = queue->front;
    if (out) *out = front->value;

    queue->front = front->next;
    if (!queue->front) queue->rear = NULL;
    
    free(front);

    queue->size--;
    return true;
}

bool charqueue_peek(CharQueue queue, char* out) {
    if (charqueue_is_empty(queue) || !out) return false;
    *out = queue->front->value;
    return true;
}

size_t charqueue_size(const CharQueue queue) {
    return _charqueue_not_exists(queue) ? 0 : queue->size;
}

CharList charqueue_to_list(const CharQueue queue) {
    if (_charqueue_not_exists(queue)) return NULL;

    CharList new_list = charlist_new();
    if (!new_list) return NULL;

    for (CharNode curr = queue->front; curr; curr = curr->next) {
        if (!charlist_push(new_list, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

CharStack charqueue_to_stack(const CharQueue queue) {
    if (_charqueue_not_exists(queue)) return NULL;

    CharStack new_stack = charstack_new();
    if (!new_stack) return NULL;

    for (CharNode curr = queue->front; curr; curr = curr->next) {
        if (!charstack_push(new_stack, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_stack;
}