#include "linkedlist/strlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "internal/memmngr.h"

typedef struct strnode {
    char* value;
    size_t size;
    struct strnode* next;
    struct strnode* prev;
} *StrNode;

struct strlist {
    StrNode head;
    StrNode tail;
    size_t size;
};

static bool strlist_not_exists(const StrList list) {
    return !list;
}

bool strlist_is_empty(const StrList list) {
    return strlist_not_exists(list) || !list->head;
}

static void strlist_free_node(StrNode node) {
    free(node->value);
    free(node);
}

static void strlist_free(StrList list) {
    strlist_clear(list);
    free(list);
}

static StrNode strlist_create_node(const char* value, size_t
size) {
    StrNode new_node = (StrNode) malloc(sizeof (struct strnode));
    if (!new_node) return NULL;

    char* value_copy = (char*) malloc(size + 1);
    if (!value_copy) {
        free(new_node);
        return NULL;
    }
    strncpy(value_copy, value, size);
    value_copy[size] = '\0';

    *new_node = (struct strnode) {.value = value_copy, .prev = NULL, .next = NULL};
    return new_node;
}

StrList strlist_new(void) {
    StrList new_list = (StrList) malloc(sizeof (struct strlist));
    if (strlist_not_exists(new_list)) return NULL;

    if (!memmngr_register(new_list, (void (*)(void*)) strlist_free)) {
        free(new_list);
        return NULL;
    }

    *new_list = (struct strlist) {.head = NULL, .tail = NULL, .size = 0};
    return new_list;
}

void strlist_clear(StrList list) {
    if (strlist_is_empty(list)) return;

    StrNode curr = list->head;
    while (curr) {
        StrNode next = curr->next;

        strlist_free_node(curr);

        curr = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

bool strlist_push_front(StrList list, const char* value, size_t str_size) {
    if (strlist_not_exists(list) || !value || str_size == 0) return false;

    StrNode new_node = strlist_create_node(value, str_size);
    if (!new_node) return false;

    if (strlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        list->head->prev = new_node;
        new_node->next = list->head;
        list->head = new_node;
    }

    list->size++;
    return true;
}

bool strlist_push(StrList list, const char* value, size_t str_size) {
    if (strlist_not_exists(list) || !value || str_size == 0) return false;
    
    StrNode new_node = strlist_create_node(value, str_size);
    if (!new_node) return false;
    
    if (strlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    
    list->size++;
    return true;
}

bool strlist_push_at(StrList list, const char* value, size_t str_size,  size_t index) {
    if (strlist_not_exists(list) || !value || str_size == 0 || index > list->size) return false;

    if (index == 0) return strlist_push_front(list, value, str_size);
    if (index == list->size) return strlist_push(list, value, str_size);

    StrNode new_node = strlist_create_node(value, str_size);
    if (!new_node) return false;

    StrNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);

    new_node->prev = curr->prev;
    new_node->next = curr;
    curr->prev->next = new_node;
    curr->prev = new_node;

    list->size++;
    return true;
}

// Return a copy of the value???
char* strlist_get_at(StrList list, size_t index) {
    if (strlist_is_empty(list) || index >= list->size) return NULL;

    StrNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);
    return curr->value;
}

int strlist_index(const StrList list, const char* target) {
    if (strlist_is_empty(list) || !target) return -1;

    StrNode curr = list->head;
    for (size_t i = 0; curr; i++, curr = curr->next) {
        if (strcmp(curr->value, target) == 0) return i;
    }
    
    return -1;
}

size_t strlist_count(const StrList list, const char* target) {
    if (strlist_is_empty(list) || !target) return 0;

    size_t freq = 0;

    StrNode curr = list->head;
    while (curr) {
        if (strcmp(curr->value, target) == 0) freq++;
        curr = curr->next;
    }

    return freq;
}

void strlist_pop_front(StrList list) {
    if (strlist_is_empty(list)) return;

    StrNode old_head = list->head;
    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    strlist_free_node(old_head);
    list->size--;
}

void strlist_pop(StrList list) {
    if (strlist_is_empty(list)) return;

    if (list->size == 1) {
        strlist_free_node(list->head);
        list->head = list->tail = NULL;
    } else {
        StrNode old_tail = list->tail;

        list->tail = list->tail->prev;
        list->tail->next = NULL;

        strlist_free_node(old_tail);
    }

    list->size--;
}

void strlist_pop_at(StrList list, size_t index) {
    if (strlist_is_empty(list) || index >= list->size) return;

    if (index == 0) {
        strlist_pop_front(list);
        return;
    }

    if (index == list->size - 1) {
        strlist_pop(list);
        return;
    }

    StrNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;

    strlist_free_node(curr);

    list->size--;
}

size_t strlist_size(const StrList list) {
    return strlist_not_exists(list) ? 0 : list->size;
}

void strlist_reverse(StrList list) {
    if (strlist_is_empty(list)) return;

    StrNode curr = list->head;
    while (curr) {
        StrNode next = curr->next;
        
        curr->next = curr->prev;
        curr->prev = next;
        
        curr = next;
    }
    
    StrNode head = list->head;
    list->head = list->tail;
    list->tail = head;
}

StrList strlist_copy(const StrList list) {
    if (strlist_is_empty(list)) return NULL;

    StrList copy = strlist_new();
    if (strlist_not_exists(copy)) return NULL;

    StrNode curr = list->head;
    while (curr) {
        if (!strlist_push(copy, curr->value, curr->size)) {
            memmngr_rollback();
            return NULL;
        }

        curr = curr->next;
    }

    return copy;
}

StrList strlist_zip(const StrList list1, const StrList list2) {
    if (strlist_is_empty(list1) || strlist_is_empty(list2)) return NULL;

    StrList new_list = strlist_new();
    if (strlist_not_exists(new_list)) return NULL;
    
    StrNode curr1 = list1->head;
    StrNode curr2 = list2->head;
    while (curr1 && curr2) {
        if (!strlist_push(new_list, curr1->value, curr1->size) || !strlist_push(new_list, curr2->value, curr2->size)) {
            memmngr_rollback();
            return NULL;
        }
        
        curr1 = curr1->next;
        curr2 = curr2->next;
    }
    
    return new_list;
}

bool strlist_all(const StrList list, bool (*predicate_func)(const char* value)) {
    if (strlist_is_empty(list) || !predicate_func) return true;

    StrNode curr = list->head;
    while (curr) {
        if(!predicate_func(curr->value)) return false;
        curr = curr->next;
    }
    
    return true;
}

bool strlist_any(const StrList list, bool (*predicate_func)(const char* value)) {
    if (strlist_is_empty(list) || !predicate_func) return false;

    StrNode curr = list->head;
    while (curr) {
        if(predicate_func(curr->value)) return true;
        curr = curr->next;
    }

    return false;
}

bool strlist_contains(const StrList list, const char* target) {
    if (strlist_is_empty(list) || !target) return false;

    StrNode curr = list->head;
    while (curr) {
        if (strcmp(curr->value, target) == 0) return true;
        curr = curr->next;
    }
    return false;
}

void strlist_print(const StrList list) {
    if (strlist_is_empty(list)) {
        printf("NULL");
        return;
    }

    StrNode curr = list->head;
    while (curr) {
        printf("%s", curr->value);

        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
}

bool strlist_equals(const StrList list1, const StrList list2) {
    if (strlist_not_exists(list1) || strlist_not_exists(list2)) return false;

    if (list1->size != list2->size) return false;

    StrNode curr1 = list1->head;
    StrNode curr2 = list2->head;
    while (curr1 && curr2) {
        if (strcmp(curr1->value, curr2->value) != 0) return false;
       
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return true;
}
