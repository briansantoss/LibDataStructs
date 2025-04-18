#include <stdio.h>
#include <stdlib.h>
#include "linkedlist/intlist.h"
#include "stack/intstack.h"

typedef struct intnode {
    int value;
    struct intnode* next;
} *IntNode;

struct intlist{
    IntNode head;
    IntNode tail;
    size_t size;
};

static bool intlist_not_exists(IntList list) {
    return list == NULL;
}

bool intlist_is_empty(IntList list) {
    return intlist_not_exists(list) || list->head == NULL;
}

static IntNode intlist_create_node(int value) {
    IntNode new_node = (IntNode) malloc(sizeof (struct intnode));
    if (new_node == NULL) return NULL;

    new_node->next = NULL;
    new_node->value = value;
    return new_node;
}

IntList intlist_init(void) {
    IntList new_list = (IntList) malloc(sizeof (struct intlist));
    if (intlist_not_exists(new_list)) return NULL;

    new_list->head = new_list->tail = NULL;
    new_list->size = 0;
    return new_list;
}

void intlist_clear(IntList list) {
    if (intlist_is_empty(list)) return;

    IntNode curr = list->head;
    while (curr != NULL) {
        IntNode next = curr->next;

        free(curr);

        curr = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

void intlist_free(IntList list) {
    intlist_clear(list);
    free(list);
}

void intlist_push(IntList list, int value) {
    if (intlist_not_exists(list)) return;

    IntNode new_node = intlist_create_node(value);
    if (new_node == NULL) return;

    if (intlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        new_node->next = list->head;
        list->head = new_node;
    }

    list->size++;
}

void intlist_append(IntList list, int value) {
    if (intlist_not_exists(list)) return;

    IntNode new_node = intlist_create_node(value);
    if (new_node == NULL) return;
    
    if (intlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
}

void intlist_push_at(IntList list, int value, size_t index) {
    if (intlist_not_exists(list) || index > list->size) return;

    if (index == 0) {
        intlist_push(list, value);
        return;
    }

    if (index == list->size) {
        intlist_append(list, value);
        return;
    }

    IntNode new_node = intlist_create_node(value);
    if (new_node == NULL) return;

    IntNode curr = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;

    list->size++;
}

int intlist_get_at(IntList list, size_t index) {
    // TODO: trocar valor de retorno nesse caso
    if (intlist_is_empty(list) || index >= list->size) return -123; // Valor temporário

    IntNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);
    return curr->value;
}

int intlist_find(IntList list, int target) {
    if (intlist_is_empty(list)) return -1;

    IntNode curr = list->head;
    for (size_t i = 0; curr != NULL; i++, curr = curr->next) {
        if (curr->value == target) return i;
    }
    
    return -1;
}

size_t intlist_count(IntList list, int target) {
    if (intlist_is_empty(list)) return 0;

    size_t freq = 0;

    IntNode curr = list->head;
    while (curr != NULL) {
        if (curr->value == target) freq++;
        curr = curr->next;
    }

    return freq;
}

void intlist_pop_start(IntList list) {
    if (intlist_is_empty(list)) return;

    IntNode old_head = list->head;
    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        list->head = list->head->next;
    }

    free(old_head);
    list->size--;
}

void intlist_pop(IntList list) {
    if (intlist_is_empty(list)) return;

    if (list->size == 1) {
        free(list->head);
        list->head = list->tail = NULL;
    } else {
        IntNode curr = list->head;
        while (curr->next != list->tail) {
            curr = curr->next;
        }

        free(list->tail);
        curr->next = NULL;
        list->tail = curr;
    }

    list->size--;
}

void intlist_pop_at(IntList list, size_t index) {
    if (intlist_is_empty(list) || index >= list->size) return;

    if (index == 0) {
        intlist_pop_start(list);
        return;
    }

    if (index == list->size - 1) {
        intlist_pop(list);
        return;
    }

    IntNode curr = list->head;
    for (size_t i = 0; i < index - 1; i++, curr = curr->next);

    IntNode target = curr->next;
    curr->next = target->next;
    free(target);

    list->size--;
}

size_t intlist_len(IntList list) {
    if (intlist_is_empty(list)) return 0;

    return list->size;
}

void intlist_reverse(IntList list) {
    if (intlist_is_empty(list)) return;

    IntNode prev = NULL;
    
    IntNode curr = list->head;
    while (curr != NULL) {
        IntNode next = curr->next;
        
        curr->next = prev;
        prev = curr;
        
        curr = next;
    }
    
    list->tail = list->head;
    list->head = prev;
}

int* intlist_to_array(IntList list) {
    if (intlist_is_empty(list)) return NULL;

    size_t arr_size = list->size;

    int* arr = (int*) malloc(sizeof (int) * arr_size);
    if (arr == NULL) return NULL;

    IntNode curr = list->head;
    for (size_t i = 0; i < arr_size; i++, curr = curr->next) {
        arr[i] = curr->value;
    }

    return arr;
}

IntStack intlist_to_stack(IntList list) {
    if (intlist_is_empty(list)) return NULL;

    IntStack new_stack = intstack_init();
    if (new_stack == NULL) return NULL;

    IntNode curr = list->head;
    while (curr != NULL) {
        intstack_push(new_stack, curr->value);
        curr = curr->next;
    }

    return new_stack;
}

IntList intlist_from_array(int* arr, size_t size) {
    if (arr == NULL) return NULL;

    IntList new_list = intlist_init();
    if (intlist_not_exists(new_list)) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        intlist_append(new_list, arr[i]);
    }
    
    return new_list;
}

void intlist_foreach(IntList list, int (*callback_func)(int value)) {
    if (intlist_is_empty(list) || callback_func == NULL) return;

    IntNode curr = list->head;
    while (curr != NULL) {
        curr->value = callback_func(curr->value);
        curr = curr->next;
    }
}

IntList intlist_copy(IntList list) {
    if (intlist_is_empty(list)) return NULL;

    IntList copy = intlist_init();
    if (intlist_not_exists(copy)) return NULL;

    IntNode curr = list->head;
    while (curr != NULL) {
        intlist_append(copy, curr->value);

        curr = curr->next;
    }

    return copy;
}

IntList intlist_map(IntList list, int (*callback_func)(int value)) {
    if (intlist_is_empty(list) || callback_func == NULL) return NULL;

    IntList new_list = intlist_init();
    if (intlist_not_exists(new_list)) return NULL;

    IntNode curr = list->head;
    while (curr != NULL) {
        intlist_append(new_list, callback_func(curr->value));

        curr = curr->next;
    }

    return new_list;
}

IntList intlist_filter(IntList list, bool (*predicate_func)(int value)) {
    if (intlist_is_empty(list) || predicate_func == NULL) return NULL;

    IntList new_list = intlist_init();
    if (intlist_not_exists(new_list)) return NULL;

    IntNode curr = list->head;
    while (curr != NULL) {
        if (predicate_func(curr->value)) intlist_append(new_list, curr->value);

        curr = curr->next;
    }

    return new_list;
}

bool intlist_all(IntList list, bool (*predicate_func)(int value)) {
    if (intlist_is_empty(list)) return true;

    IntNode curr = list->head;
    while (curr != NULL) {
        if(!predicate_func(curr->value)) return false;
        curr = curr->next;
    }
    
    return true;
}

bool intlist_any(IntList list, bool (*predicate_func)(int value)) {
    if (intlist_is_empty(list)) return false;

    IntNode curr = list->head;
    while (curr != NULL) {
        if(predicate_func(curr->value)) return true;
        curr = curr->next;
    }

    return false;
}

long long intlist_reduce(IntList list, long long (*reduce_func)(long long acc, int value), long long initial) {
    if (intlist_is_empty(list) || reduce_func == NULL) return 0;

    IntNode curr = list->head;
    long long acc = initial;
    while (curr != NULL) {
        acc = reduce_func(acc, curr->value);
        curr = curr->next;
    }

    return acc;
}

static long long sum_reduce(long long acc, int value) {
    return acc + value;
}

long long intlist_sum(IntList list) {
    return intlist_reduce(list, sum_reduce, 0);
}

void intlist_print(IntList list) {
    if (intlist_is_empty(list)) {
        printf("NULL");
        return;
    }

    IntNode curr = list->head;
    while (curr != NULL) {
        printf("%d", curr->value);
        printf(" -> ");

        curr = curr->next;
    }
    printf("NULL");
}

bool intlist_contains(IntList list, int target) {
    if (intlist_is_empty(list)) return false;

    IntNode curr = list->head;
    while (curr != NULL) {
        if (curr->value == target) return true;
        curr = curr->next;
    }
    return false;
}

bool intlist_equals(IntList list1, IntList list2) {
    if (intlist_not_exists(list1) || intlist_not_exists(list2)) return false;

    if (list1->size != list2->size) return false;

    IntNode curr1 = list1->head;
    IntNode curr2 = list2->head;
    while (curr1 != NULL && curr2 != NULL ) {
        if (curr1->value != curr2->value) return false;
       
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return true;
}