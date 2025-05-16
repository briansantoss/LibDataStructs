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
    if (intlist_is_empty(list)) return;

    IntNode curr = list->head;
    while (curr) {
        IntNode next = curr->next;
        free(curr);
        curr = next;
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
    return !intlist_is_empty(list) && out && (*out = list->head->value);
}

bool intlist_get_at(const IntList list, size_t index, int* out) {
    return !intlist_is_empty(list) && out && index < list->size && (*out = intlist_node_at(list, index)->value);
}

bool intlist_back(const IntList list, int* out) {
    return !intlist_is_empty(list) && out && (*out = list->tail->value);
}

int intlist_index(const IntList list, int target) {
    if (intlist_is_empty(list)) return -1;

    IntNode curr = list->head;
    for (size_t i = 0; curr; i++, curr = curr->next) {
        if (curr->value == target) return i;
    }
    return -1;
}

size_t intlist_count(const IntList list, int target) {
    if (intlist_is_empty(list)) return 0;
    
    size_t freq = 0;
    
    IntNode curr = list->head;
    while (curr) {
        if (curr->value == target) freq++;
        curr = curr->next;
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
    
    IntNode curr = list->head;
    while (curr) {
        IntNode next = curr->next;
        
        curr->next = curr->prev;
        curr->prev = next;
        
        curr = next;
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
    for (size_t i = 0; i < arr_size; i++, curr = curr->next) {
        arr[i] = curr->value;
    }
    
    return arr;
}

IntStack intlist_to_stack(const IntList list) {
    if (intlist_is_empty(list)) return NULL;
    
    IntStack new_stack = intstack_new();
    if (!new_stack) return NULL;
    
    IntNode curr = list->head;
    while (curr) {
        if (!intstack_push(new_stack, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }    
    
    return new_stack;
}

IntQueue intlist_to_queue(const IntList list) {
    if (intlist_is_empty(list)) return NULL;
    
    IntQueue new_queue = intqueue_new();
    if (!new_queue) return NULL;
    
    IntNode curr = list->head;
    while (curr) {
        if (!intqueue_enqueue(new_queue, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
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
    if (intlist_is_empty(list) || !callback_func) return;
    
    IntNode curr = list->head;
    while (curr) {
        curr->value = callback_func(curr->value);
        curr = curr->next;
    }
}

IntList intlist_copy(const IntList list) {
    if (intlist_is_empty(list)) return NULL;
    
    IntList copy = intlist_new();
    if (intlist_not_exists(copy)) return NULL;
    
    IntNode curr = list->head;
    while (curr) {
        if (!intlist_push(copy, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        
        curr = curr->next;
    }
    
    return copy;
}

IntList intlist_map(const IntList list, int (*callback_func)(int value)) {
    if (intlist_is_empty(list) || !callback_func) return NULL;

    IntList new_list = intlist_new();
    if (intlist_not_exists(new_list)) return NULL;
    
    IntNode curr = list->head;
    while (curr) {
        if (!intlist_push(new_list, callback_func(curr->value))) {
            memmngr_rollback();
            return NULL;
        }
        
        curr = curr->next;
    }
    
    return new_list;
}

IntList intlist_filter(const IntList list, bool (*predicate_func)(int value)) {
    if (intlist_is_empty(list) || !predicate_func) return NULL;

    IntList new_list = intlist_new();
    if (intlist_not_exists(new_list)) return NULL;
    
    IntNode curr = list->head;
    while (curr) {
        if (predicate_func(curr->value)) {
            if (!intlist_push(new_list, curr->value)) {
                memmngr_rollback();
                return NULL;
            }
        }
        
        curr = curr->next;
    }
    
    return new_list;
}

IntList intlist_zip(const IntList list1, const IntList list2) {
    if (intlist_is_empty(list1) || intlist_is_empty(list2)) return NULL;

    IntList new_list = intlist_new();
    if (intlist_not_exists(new_list)) return NULL;
    
    IntNode curr1 = list1->head;
    IntNode curr2 = list2->head;
    while (curr1 && curr2) {
        if (!intlist_push(new_list, curr1->value) || !intlist_push(new_list, curr2->value)) {
            memmngr_rollback();
            return NULL;
        }
        
        curr1 = curr1->next;
        curr2 = curr2->next;
    }
    
    return new_list;
}

bool intlist_all(const IntList list, bool (*predicate_func)(int value)) {
    if (intlist_is_empty(list) || !predicate_func) return true;

    IntNode curr = list->head;
    while (curr) {
        if(!predicate_func(curr->value)) return false;
        curr = curr->next;
    }
    
    return true;
}

bool intlist_any(const IntList list, bool (*predicate_func)(int value)) {
    if (intlist_is_empty(list) || !predicate_func) return false;

    IntNode curr = list->head;
    while (curr) {
        if(predicate_func(curr->value)) return true;
        curr = curr->next;
    }

    return false;
}

long long intlist_reduce(const IntList list, long long (*reduce_func)(long long acc, int value), long long initial) {
    if (intlist_is_empty(list) || !reduce_func) return initial;

    IntNode curr = list->head;
    long long acc = initial;
    while (curr) {
        acc = reduce_func(acc, curr->value);
        curr = curr->next;
    }

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

    IntNode curr = list->head;
    while (curr) {
        printf("%d", curr->value);
        
        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
}

bool intlist_contains(const IntList list, int target) {
    if (intlist_is_empty(list)) return false;

    IntNode curr = list->head;
    while (curr) {
        if (curr->value == target) return true;
        curr = curr->next;
    }
    return false;
}

bool intlist_equals(const IntList list1, const IntList list2) {
    if (intlist_not_exists(list1) || intlist_not_exists(list2) || list1->size != list2->size) return false;

    IntNode curr1 = list1->head;
    IntNode curr2 = list2->head;
    while (curr1 && curr2) {
        if (curr1->value != curr2->value) return false;
       
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return true;
}