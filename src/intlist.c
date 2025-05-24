#include "linkedlist/intlist.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack/intstack.h"
#include "queue/intqueue.h"
#include "internal/memmngr.h"

typedef struct intnode {
    int value;
    struct intnode* next;
    struct intnode* prev;
} *IntNode;

struct intlist {
    IntNode head;
    IntNode tail;
    size_t size;
};

static bool intlist_not_exists(const IntList list) {
    return !list;
}

bool intlist_is_empty(const IntList list) {
    return intlist_not_exists(list) || !list->head;
}

static IntNode intlist_create_node(int value, IntNode prev, IntNode next) {
    IntNode new_node = (IntNode) malloc(sizeof (struct intnode));
    if (!new_node) return NULL;

    *new_node = (struct intnode) {.value = value, .prev = prev, .next = next};
    return new_node;
}

static void intlist_free(IntList list) {
    intlist_clear(list);
    free(list);
}

static IntNode intlist_node_at(const IntList list, size_t index) {
    IntNode node;
    if (index < list->size / 2) {
        node = list->head;
        while (index--) node = node->next;
    } else {
        node = list->tail;
        index = list->size - index - 1;
        while (index--) node = node->prev;
    }
    return node;
}

static bool intlist_link_before(IntList list, int value, IntNode succ) {
    IntNode pred = succ ? succ->prev : list->tail;

    IntNode new_node = intlist_create_node(value, pred, succ);
    if (!new_node) return false;

    if (!pred)  list->head = new_node;
    else        pred->next = new_node;

    if (!succ)  list->tail = new_node;
    else        succ->prev = new_node;

    list->size++;
    return true;
}

static void intlist_unlink_node(IntList list, IntNode node) {
    IntNode pred = node->prev;
    IntNode succ = node->next;

    if (!pred)  list->head = succ;
    else        pred->next = succ;

    if (!succ)  list->tail = pred;
    else        succ->prev = pred;

    free(node);
    list->size--;
}

IntList intlist_new(void) {
    IntList new_list = (IntList) malloc(sizeof (struct intlist));
    if (intlist_not_exists(new_list)) return NULL;

    if (!memmngr_register(new_list, (void (*)(void*)) intlist_free)) {
        free(new_list);
        return NULL;
    }

    *new_list = (struct intlist) {.head = NULL, .tail = NULL, .size = 0};
    return new_list;
}

void intlist_clear(IntList list) {
    if (intlist_not_exists(list)) return;

    for (IntNode curr = list->head, next; curr; curr = next) {
        next = curr->next;
        free(curr);
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

bool intlist_push_front(IntList list, int value) { 
    return !intlist_not_exists(list) && intlist_link_before(list, value, list->head);
}

bool intlist_push(IntList list, int value) {
    return !intlist_not_exists(list) && intlist_link_before(list, value, NULL);
}

bool intlist_push_at(IntList list, int value, size_t index) {
    return !intlist_not_exists(list) && index <= list->size && intlist_link_before(list, value, index < list->size ? intlist_node_at(list, index) : NULL);
}

bool intlist_front(const IntList list, int* out) {
    if (intlist_is_empty(list) || !out) return false;
    *out = list->head->value;
    return true;
}

bool intlist_get_at(const IntList list, size_t index, int* out) {
    if (intlist_is_empty(list) || !out || index >= list->size) return false;
    *out = intlist_node_at(list, index)->value;
    return true;
}

bool intlist_back(const IntList list, int* out) {
    if (intlist_is_empty(list) || !out) return false;
    *out = list->tail->value;
    return true;
}

int intlist_index(const IntList list, int target) {
    if (intlist_not_exists(list)) return -1;

    IntNode curr = list->head;
    for (size_t i = 0; curr; i++, curr = curr->next) {
        if (curr->value == target) return i;
    }
    return -1;
}

size_t intlist_count(const IntList list, int target) {
    if (intlist_not_exists(list)) return 0;
    
    size_t freq = 0;
    for (IntNode curr = list->head; curr; curr = curr->next) {
        if (curr->value == target) freq++;
    }
    return freq;
}

void intlist_pop_front(IntList list) {
    if (intlist_is_empty(list)) return;
    intlist_unlink_node(list, list->head);
}

void intlist_pop(IntList list) {
    if (intlist_is_empty(list)) return;
    intlist_unlink_node(list, list->tail);
}

void intlist_pop_at(IntList list, size_t index) {
    if (intlist_is_empty(list) || index >= list->size) return;
    intlist_unlink_node(list, intlist_node_at(list, index));    
}

size_t intlist_size(IntList list) {
    return intlist_not_exists(list) ? 0 : list->size;
}

void intlist_reverse(IntList list) {
    if (intlist_not_exists(list) || list->size < 2) return;
    
    for (IntNode curr = list->head, next; curr; curr = next) {
        next = curr->next;
        curr->next = curr->prev;
        curr->prev = next;
    }
    
    IntNode head = list->head;
    list->head = list->tail;
    list->tail = head;
}

int* intlist_to_array(const IntList list) {
    if (intlist_is_empty(list)) return NULL;

    size_t arr_size = list->size;
    
    int* arr = (int*) malloc(sizeof (int) * arr_size);
    if (!arr) return NULL;

    if (!memmngr_register(arr, free)) {
        free(arr);
        return NULL;
    }
    
    IntNode curr = list->head;
    for (size_t i = 0; i < arr_size; i++, curr = curr->next) arr[i] = curr->value;
    return arr;
}

IntStack intlist_to_stack(const IntList list) {
    if (intlist_not_exists(list)) return NULL;
    
    IntStack new_stack = intstack_new();
    if (!new_stack) return NULL;
    
    for (IntNode curr = list->head; curr; curr = curr->next) {
        if (!intstack_push(new_stack, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
    }    
    return new_stack;
}

IntQueue intlist_to_queue(const IntList list) {
    if (intlist_not_exists(list)) return NULL;
    
    IntQueue new_queue = intqueue_new();
    if (!new_queue) return NULL;
    
    for (IntNode curr = list->head; curr; curr = curr->next) {
        if (!intqueue_enqueue(new_queue, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
    }    
    return new_queue;
}

IntList intlist_from_array(int* arr, size_t size) {
    if (!arr || size == 0) return NULL;

    IntList new_list = intlist_new();
    if (intlist_not_exists(new_list)) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        if (!intlist_push(new_list, arr[i])) {
            memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

void intlist_foreach(IntList list, int (*callback_func)(int value)) {
    if (intlist_not_exists(list) || !callback_func) return;
    
    for (IntNode curr = list->head; curr; curr = curr->next) curr->value = callback_func(curr->value);
}

IntList intlist_copy(const IntList list) {
    if (intlist_not_exists(list)) return NULL;
    
    IntList copy = intlist_new();
    if (intlist_not_exists(copy)) return NULL;
    
    for (IntNode curr = list->head; curr; curr = curr->next) {
        if (!intlist_push(copy, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
    }   
    return copy;
}

IntList intlist_map(const IntList list, int (*callback_func)(int value)) {
    if (intlist_not_exists(list)) return NULL;
    if (!callback_func) return intlist_copy(list);

    IntList new_list = intlist_new();
    if (intlist_not_exists(new_list)) return NULL;
    
    for (IntNode curr = list->head; curr; curr = curr->next) {
        if (!intlist_push(new_list, callback_func(curr->value))) {
            memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

IntList intlist_filter(const IntList list, bool (*predicate_func)(int value)) {
    if (intlist_not_exists(list)) return NULL;
    if (!predicate_func) return intlist_copy(list);

    IntList new_list = intlist_new();
    if (intlist_not_exists(new_list)) return NULL;
    
    for (IntNode curr = list->head; curr; curr = curr->next) {
        if (predicate_func(curr->value)) {
            if (!intlist_push(new_list, curr->value)) {
                memmngr_rollback();
                return NULL;
            }
        }
    }
    return new_list;
}

IntList intlist_zip(const IntList list1, const IntList list2) {
    if (intlist_not_exists(list1) || intlist_not_exists(list2)) return NULL;

    IntList new_list = intlist_new();
    if (intlist_not_exists(new_list)) return NULL;
    
    for (IntNode curr1 = list1->head, curr2 = list2->head; curr1 && curr2; curr1 = curr1->next, curr2 = curr2->next) {
        if (!intlist_push(new_list, curr1->value) || !intlist_push(new_list, curr2->value)) {
            memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

bool intlist_all(const IntList list, bool (*predicate_func)(int value)) {
    if (intlist_not_exists(list)) return false;
    if (!predicate_func) return true;

    for (IntNode curr = list->head; curr; curr = curr->next) if (!predicate_func(curr->value)) return false;
    return true;
}

bool intlist_any(const IntList list, bool (*predicate_func)(int value)) {
    if (intlist_not_exists(list) || !predicate_func) return false;

    for (IntNode curr = list->head; curr; curr = curr->next) if (predicate_func(curr->value)) return true;
    return false;
}

long long intlist_reduce(const IntList list, long long (*reduce_func)(long long acc, int value), long long initial) {
    if (intlist_not_exists(list) || !reduce_func) return initial;

    long long acc = initial;
    for (IntNode curr = list->head; curr; acc = reduce_func(acc, curr->value), curr = curr->next);
    return acc;
}

static long long sum_reduce(long long acc, int value) {
    return acc + value;
}

long long intlist_sum(const IntList list) {
    return intlist_reduce(list, sum_reduce, 0);
}

void intlist_print(const IntList list) {
    if (intlist_is_empty(list)) {
        printf("NULL");
        return;
    }

    for (IntNode curr = list->head; curr; curr = curr->next) {
        printf("%d", curr->value);
        if (curr->next) printf(" -> ");
    }
}

bool intlist_contains(const IntList list, int target) {
    if (intlist_not_exists(list)) return false;
    
    for (IntNode curr = list->head; curr; curr = curr->next) if (curr->value == target) return true;
    return false;
}

bool intlist_equals(const IntList list1, const IntList list2) {
    if (intlist_not_exists(list1) || intlist_not_exists(list2) || list1->size != list2->size) return false;

    for (IntNode curr1 = list1->head, curr2 = list2->head; curr1 && curr2; curr1 = curr1->next, curr2 = curr2->next) if (curr1->value != curr2->value) return false;
    return true;
}