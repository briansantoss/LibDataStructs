#include "linkedlist/strlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "internal/memmngr.h"

typedef struct strnode {
    char* value;
    size_t size;
    struct strnode* next;
} *StrNode;

struct strlist {
    StrNode head;
    StrNode tail;
    size_t size;
};

static bool strlist_not_exists(StrList list) {
    return list == NULL;
}

bool strlist_is_empty(StrList list) {
    return strlist_not_exists(list) || list->head == NULL;
}

static StrNode strlist_create_node(const char* value, size_t
size) {
    StrNode new_node = (StrNode) malloc(sizeof (struct strnode));
    if (new_node == NULL) return NULL;

    new_node->value = (char*) malloc(size + 1);
    if (new_node->value == NULL) {
        free(new_node);
        return NULL;
    }

    strncpy(new_node->value, value, size);
    new_node->value[size] = '\0';
    new_node->next = NULL;
    new_node->size = size;
    return new_node;
}

static void strlist_free_node(StrNode node) {
    if (node == NULL) return;
    
    free(node->value);
    free(node);
}

StrList strlist_new(void) {
    StrList new_list = (StrList) malloc(sizeof (struct strlist));
    if (strlist_not_exists(new_list)) return NULL;

    if (!memmngr_register(new_list, (void (*)(void*)) strlist_free)) {
        free(new_list);
        return NULL;
    }

    new_list->head = new_list->tail = NULL;
    new_list->size = 0;

    return new_list;
}

void strlist_clear(StrList list) {
    if (strlist_is_empty(list)) return;

    StrNode curr = list->head;
    while (curr != NULL) {
        StrNode next = curr->next;

        strlist_free_node(curr);

        curr = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

void strlist_free(StrList list) {
    strlist_clear(list);
    free(list);
}

bool strlist_push(StrList list, const char* value, size_t str_size) {
    if (strlist_not_exists(list) || value == NULL || str_size == 0) return false;

    StrNode new_node = strlist_create_node(value, str_size);
    if (new_node == NULL) return false;

    if (strlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        new_node->next = list->head;
        list->head = new_node;
    }

    list->size++;
    return true;
}

bool strlist_append(StrList list, const char* value, size_t str_size) {
    if (strlist_not_exists(list) || value == NULL || str_size == 0) return false;
    
    StrNode new_node = strlist_create_node(value, str_size);
    if (new_node == NULL) return false;
    
    if (strlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    
    list->size++;
    return true;
}

bool strlist_push_at(StrList list, const char* value, size_t str_size,  size_t index) {
    if (strlist_not_exists(list) || value == NULL  || str_size == 0 || index > list->size) return false;

    if (index == 0) return strlist_push(list, value, str_size);
    if (index == list->size) return strlist_append(list, value, str_size);

    StrNode new_node = strlist_create_node(value, str_size);
    if (new_node == NULL) return false;

    StrNode curr = list->head;
    for (size_t i = 0; i < index - 1; i++, curr = curr->next);
    new_node->next = curr->next;
    curr->next = new_node;

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

int strlist_index(StrList list, const char* target) {
    if (strlist_is_empty(list) || target == NULL) return -1;

    StrNode curr = list->head;
    for (size_t i = 0; curr != NULL; i++, curr = curr->next) {
        if (strcmp(curr->value, target) == 0) return i;
    }
    
    return -1;
}

size_t strlist_count(StrList list, const char* target) {
    if (strlist_is_empty(list) || target == NULL) return 0;

    size_t freq = 0;

    StrNode curr = list->head;
    while (curr != NULL) {
        if (strcmp(curr->value, target) == 0) freq++;
        curr = curr->next;
    }

    return freq;
}

void strlist_pop_start(StrList list) {
    if (strlist_is_empty(list)) return;

    StrNode old_head = list->head;
    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        list->head = list->head->next;
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
        StrNode curr = list->head;
        while (curr->next != list->tail) {
            curr = curr->next;
        }

        strlist_free_node(list->tail);
        curr->next = NULL;
        list->tail = curr;
    }

    list->size--;
}

void strlist_pop_at(StrList list, size_t index) {
    if (strlist_is_empty(list) || index >= list->size) return;

    if (index == 0) {
        strlist_pop_start(list);
        return;
    }

    if (index == list->size - 1) {
        strlist_pop(list);
        return;
    }

    StrNode curr = list->head;
    for (size_t i = 0; i < index - 1; i++, curr = curr->next);

    StrNode target = curr->next;
    curr->next = target->next;
    strlist_free_node(target);

    list->size--;
}

size_t strlist_len(StrList list) {
    return strlist_is_empty(list) ? 0 : list->size;
}

void strlist_reverse(StrList list) {
    if (strlist_is_empty(list)) return;

    StrNode prev = NULL;
    
    StrNode curr = list->head;
    while (curr != NULL) {
        StrNode next = curr->next;
        
        curr->next = prev;
        prev = curr;
        
        curr = next;
    }
    
    list->tail = list->head;
    list->head = prev;
}

StrList strlist_copy(StrList list) {
    if (strlist_is_empty(list)) return NULL;

    StrList copy = strlist_new();
    if (strlist_not_exists(copy)) return NULL;

    StrNode curr = list->head;
    while (curr != NULL) {
        if (!strlist_append(copy, curr->value, curr->size)) {
            memmngr_rollback();
            return NULL;
        }

        curr = curr->next;
    }

    return copy;
}

StrList strlist_zip(StrList list1, StrList list2) {
    if (strlist_is_empty(list1) || strlist_is_empty(list2)) return NULL;

    StrList new_list = strlist_new();
    if (strlist_not_exists(new_list)) return NULL;
    
    StrNode curr1 = list1->head;
    StrNode curr2 = list2->head;
    while (curr1 != NULL && curr2 != NULL) {
        if (!strlist_append(new_list, curr1->value, curr1->size) || !strlist_append(new_list, curr2->value, curr2->size)) {
            memmngr_rollback();
            return NULL;
        }
        
        curr1 = curr1->next;
        curr2 = curr2->next;
    }
    
    return new_list;
}

bool strlist_all(StrList list, bool (*predicate_func)(const char* value)) {
    if (strlist_is_empty(list)) return true;

    StrNode curr = list->head;
    while (curr != NULL) {
        if(!predicate_func(curr->value)) return false;
        curr = curr->next;
    }
    
    return true;
}

bool strlist_any(StrList list, bool (*predicate_func)(const char* value)) {
    if (strlist_is_empty(list)) return false;

    StrNode curr = list->head;
    while (curr != NULL) {
        if(predicate_func(curr->value)) return true;
        curr = curr->next;
    }

    return false;
}

bool strlist_contains(StrList list, const char* target) {
    if (strlist_is_empty(list) || target == NULL) return false;

    StrNode curr = list->head;
    while (curr != NULL) {
        if (strcmp(curr->value, target) == 0) return true;
        curr = curr->next;
    }
    return false;
}

void strlist_print(StrList list) {
    if (strlist_is_empty(list)) {
        printf("NULL");
        return;
    }

    StrNode curr = list->head;
    while (curr != NULL) {
        printf("%s", curr->value);
        printf(" -> ");

        curr = curr->next;
    }
    printf("NULL");
}

bool strlist_equals(StrList list1, StrList list2) {
    if (strlist_not_exists(list1) || strlist_not_exists(list2)) return false;

    if (list1->size != list2->size) return false;

    StrNode curr1 = list1->head;
    StrNode curr2 = list2->head;
    while (curr1 != NULL && curr2 != NULL ) {
        if (strcmp(curr1->value, curr2->value) != 0) return false;
       
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return true;
}
