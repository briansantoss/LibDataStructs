#include <stdlib.h>
#include "intlist.h"

typedef struct intnode {
    int value;
    struct intnode* next;
} *IntNode;

struct intlist{
    IntNode head;
    IntNode tail;
    size_t size;
};

static int intlist_not_exists(IntList list) {
    return list == NULL;
}

static int intlist_is_empty(IntList list) {
    return list->head == NULL;
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
    if (intlist_not_exists(list) || intlist_is_empty(list)) return;

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
        list->tail = new_node;
    }

    new_node->next = list->head;
    list->head = new_node;

    list->size++;
}

void intlist_push_end(IntList list, int value) {
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

void intlist_insert_at(IntList list, int value, size_t index) {
    if (intlist_not_exists(list) || intlist_is_empty(list) || index > list->size) return;

    if (index == 0) {
        intlist_push(list, value);
        return;
    }

    if (index == list->size) {
        intlist_push_end(list, value);
        return;
    }

    IntNode new_node = intlist_create_node(value);

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
    if (intlist_not_exists(list) || intlist_is_empty(list) || index >= list->size) return -123; // Valor temporário

    IntNode curr = list->head;
    for (size_t i = 0; i < index; i++) {
        curr = curr->next;
    }

    return curr->value;
}

void intlist_pop_start(IntList list) {
    if (intlist_not_exists(list) || intlist_is_empty(list)) return;

    IntNode old_head = list->head;
    if (list->size == 1) {
        list->head = list->tail = NULL;
        free(old_head);
        return;
    }

    list->head = list->head->next;
    free(old_head);
}
    
size_t intlist_len(IntList list) {
    if (intlist_not_exists(list) || intlist_is_empty(list)) return 0;

    return list->size;
}

void intlist_reverse(IntList list) {
    IntNode old_head = list->head;
    IntNode head = NULL;

    IntNode curr = list->head;
    while (curr != NULL) {
        IntNode next = curr->next;

        curr->next = head;
        head = curr;

        curr = next;
    }

    list->head = head;
    list->tail = old_head;
}

int* intlist_to_array(IntList list) {
    if (intlist_not_exists(list) || intlist_is_empty(list)) return NULL;

    size_t arr_size = list->size;

    int* arr = (int*) malloc(sizeof (int) * arr_size);
    if (arr == NULL) return NULL;

    IntNode curr = list->head;
    for (size_t i = 0; i < arr_size; i++, curr = curr->next) {
        arr[i] = curr->value;
    }

    return arr;
}

void intlist_foreach(IntList list, int (*callback_func)(int value)) {
    if (intlist_not_exists(list) || intlist_is_empty(list) || callback_func == NULL) return;

    IntNode curr = list->head;
    while (curr != NULL) {
        curr->value = callback_func(curr->value);
        curr = curr->next;
    }
}

IntList intlist_map(IntList list, int (*callback_func)(int value)) {
    if (intlist_not_exists(list) || intlist_is_empty(list) || callback_func == NULL) return NULL;

    IntList new_list = intlist_init();
    if (new_list == NULL) return NULL;

    IntNode curr = list->head;
    while (curr != NULL) {
        intlist_push_end(new_list, callback_func(curr->value));

        curr = curr->next;
    }

    return new_list;
}

IntList intlist_filter(IntList list, int (*callback_func)(int value)) {
    if (intlist_not_exists(list) || intlist_is_empty(list) || callback_func == NULL) return NULL;

    IntList new_list = intlist_init();
    if (new_list == NULL) return NULL;

    IntNode curr = list->head;
    while (curr != NULL) {
        if (callback_func(curr->value)) intlist_push_end(new_list, curr->value);

        curr = curr->next;
    }

    return new_list;
}

int intlist_contains(IntList list, int target) {
    if (intlist_not_exists(list) || intlist_is_empty(list)) return 0;

    IntNode curr = list->head;
    while (curr != NULL) {
        if (curr->value == target) return 1;
        curr = curr->next;
    }
    return 0;
}

int intlist_equals(IntList list1, IntList list2) {
    if ((intlist_not_exists(list1) || intlist_not_exists(list2)) || (intlist_is_empty(list1) ^ intlist_is_empty(list2))) return 0;

    if (intlist_is_empty(list1) && intlist_is_empty(list2)) return 1;

    IntNode curr1 = list1->head;
    IntNode curr2 = list2->head;
    while (curr1 != NULL && curr2 != NULL ) {
        if (curr1->value != curr2->value) return 0;
       
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return curr1 == NULL && curr2 == NULL;
}