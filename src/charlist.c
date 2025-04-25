#include <stdio.h>
#include <stdlib.h>
#include "linkedlist/charlist.h"
#include "stack/charstack.h"
#include "queue/charqueue.h"

typedef struct charnode {
    char value;
    struct charnode* next;
} *CharNode;

struct charlist{
    CharNode head;
    CharNode tail;
    size_t size;
};

extern bool memmngr_register(void* dstruct, void (*destroy_func)(void* dstruct));
extern void memmngr_rollback(void);

static bool charlist_not_exists(CharList list) {
    return list == NULL;
}

bool charlist_is_empty(CharList list) {
    return charlist_not_exists(list) || list->head == NULL;
}

static CharNode charlist_create_node(char value) {
    CharNode new_node = (CharNode) malloc(sizeof (struct charnode));
    if (new_node == NULL) return NULL;

    new_node->next = NULL;
    new_node->value = value;
    return new_node;
}

CharList charlist_new(void) {
    CharList new_list = (CharList) malloc(sizeof (struct charlist));
    if (charlist_not_exists(new_list)) return NULL;

    if (!memmngr_register(new_list, (void (*)(void*)) charlist_free)) {
        free(new_list);
        return NULL;
    }
    
    new_list->head = new_list->tail = NULL;
    new_list->size = 0;

    return new_list;
}

void charlist_clear(CharList list) {
    if (charlist_is_empty(list)) return;

    CharNode curr = list->head;
    while (curr != NULL) {
        CharNode next = curr->next;

        free(curr);

        curr = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

void charlist_free(CharList list) {
    charlist_clear(list);
    free(list);
}

bool charlist_push(CharList list, char value) {
    if (charlist_not_exists(list)) return false;

    CharNode new_node = charlist_create_node(value);
    if (new_node == NULL) return false;

    if (charlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        new_node->next = list->head;
        list->head = new_node;
    }

    list->size++;
    return true;
}

bool charlist_append(CharList list, char value) {
    if (charlist_not_exists(list)) return false;

    CharNode new_node = charlist_create_node(value);
    if (new_node == NULL) return false;
    
    if (charlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return true;
}

bool charlist_push_at(CharList list, char value, size_t index) {
    if (charlist_not_exists(list) || index > list->size) return false;

    if (index == 0) return charlist_push(list, value);
    if (index == list->size) return charlist_append(list, value);

    CharNode new_node = charlist_create_node(value);
    if (new_node == NULL) return false;

    CharNode curr = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;

    list->size++;
    return true;
}

bool charlist_get_at(CharList list, size_t index, char* out) {
    if (charlist_is_empty(list) || out == NULL || index >= list->size) return false;

    CharNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);
    
    *out = curr->value;

    return true;
}

int charlist_index(CharList list, char target) {
    if (charlist_is_empty(list)) return -1;

    CharNode curr = list->head;
    for (size_t i = 0; curr != NULL; i++, curr = curr->next) {
        if (curr->value == target) return i;
    }
    
    return -1;
}

size_t charlist_count(CharList list, char target) {
    if (charlist_is_empty(list)) return 0;

    size_t freq = 0;

    CharNode curr = list->head;
    while (curr != NULL) {
        if (curr->value == target) freq++;
        curr = curr->next;
    }

    return freq;
}

void charlist_pop_start(CharList list) {
    if (charlist_is_empty(list)) return;

    CharNode old_head = list->head;
    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        list->head = list->head->next;
    }

    free(old_head);
    list->size--;
}

void charlist_pop(CharList list) {
    if (charlist_is_empty(list)) return;

    if (list->size == 1) {
        free(list->head);
        list->head = list->tail = NULL;
    } else {
        CharNode curr = list->head;
        while (curr->next != list->tail) {
            curr = curr->next;
        }

        free(list->tail);
        curr->next = NULL;
        list->tail = curr;
    }

    list->size--;
}

void charlist_pop_at(CharList list, size_t index) {
    if (charlist_is_empty(list) || index >= list->size) return;

    if (index == 0) {
        charlist_pop_start(list);
        return;
    }

    if (index == list->size - 1) {
        charlist_pop(list);
        return;
    }

    CharNode curr = list->head;
    for (size_t i = 0; i < index - 1; i++, curr = curr->next);

    CharNode target = curr->next;
    curr->next = target->next;
    free(target);

    list->size--;
}

size_t charlist_len(CharList list) {
    return charlist_is_empty(list) ? 0 : list->size;
}

void charlist_reverse(CharList list) {
    if (charlist_is_empty(list)) return;

    CharNode prev = NULL;
    
    CharNode curr = list->head;
    while (curr != NULL) {
        CharNode next = curr->next;
        
        curr->next = prev;
        prev = curr;
        
        curr = next;
    }
    
    list->tail = list->head;
    list->head = prev;
}

char* charlist_to_string(CharList list) {
    if (charlist_is_empty(list)) return NULL;

    size_t str_size = list->size;

    char* str = (char*) malloc(sizeof (char) * (str_size + 1));
    if (str == NULL) return NULL;

    CharNode curr = list->head;
    for (size_t i = 0; i < str_size; i++, curr = curr->next) {
        str[i] = curr->value;
    }

    str[str_size] = '\0';
    return str;
}

CharStack charlist_to_stack(CharList list) {
    if (charlist_is_empty(list)) return NULL;

    CharStack new_stack = charstack_new();
    if (new_stack == NULL) return NULL;

    CharNode curr = list->head;
    while (curr != NULL) {
        charstack_push(new_stack, curr->value);
        curr = curr->next;
    }

    return new_stack;
}

CharQueue charlist_to_queue(CharList list) {
    if (charlist_is_empty(list)) return NULL;
    
    CharQueue new_queue = charqueue_new();
    if (new_queue == NULL) return NULL;
    
    CharNode curr = list->head;
    while (curr != NULL) {
        if (!charqueue_enqueue(new_queue, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }    

    return new_queue;
}

CharList charlist_from_string(const char* str, size_t size) {
    if (str == NULL || size == 0) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        if (!charlist_append(new_list, str[i])) {
            memmngr_rollback();
            return NULL;
        }
    }
    
    return new_list;
}

void charlist_foreach(CharList list, char (*callback_func)(char value)) {
    if (charlist_is_empty(list) || callback_func == NULL) return;

    CharNode curr = list->head;
    while (curr != NULL) {
        curr->value = callback_func(curr->value);
        curr = curr->next;
    }
}

CharList charlist_copy(CharList list) {
    if (charlist_is_empty(list)) return NULL;

    CharList copy = charlist_new();
    if (charlist_not_exists(copy)) return NULL;

    CharNode curr = list->head;
    while (curr != NULL) {
        if (!charlist_append(copy, curr->value)) {
            memmngr_rollback();
            return NULL;
        }

        curr = curr->next;
    }

    return copy;
}

CharList charlist_map(CharList list, char (*callback_func)(char value)) {
    if (charlist_is_empty(list) || callback_func == NULL) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;

    CharNode curr = list->head;
    while (curr != NULL) {
        if (!charlist_append(new_list, callback_func(curr->value))) {
            memmngr_rollback();
            return NULL;
        }

        curr = curr->next;
    }

    return new_list;
}

CharList charlist_zip(CharList list1, CharList list2) {
    if (charlist_is_empty(list1) || charlist_is_empty(list2)) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;

    CharNode curr1 = list1->head;
    CharNode curr2 = list2->head;
    while (curr1 != NULL && curr2 != NULL) {
        if (!charlist_append(new_list, curr1->value) || !charlist_append(new_list, curr2->value)) {
            memmngr_rollback();
            return NULL;
        }

        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return new_list;
}

CharList charlist_filter(CharList list, bool (*predicate_func)(char value)) {
    if (charlist_is_empty(list) || predicate_func == NULL) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;

    CharNode curr = list->head;
    while (curr != NULL) {
        if (predicate_func(curr->value)) {
            if (!charlist_append(new_list, curr->value)) {
                memmngr_rollback();
                return NULL;
           }
        }

        curr = curr->next;
    }

    return new_list;
}

bool charlist_all(CharList list, bool (*predicate_func)(char value)) {
    if (charlist_is_empty(list)) return 1;

    CharNode curr = list->head;
    while (curr != NULL) {
        if(!predicate_func(curr->value)) return false;
        curr = curr->next;
    }
    
    return true;
}

bool charlist_any(CharList list, bool (*predicate_func)(char value)) {
    if (charlist_is_empty(list)) return false;

    CharNode curr = list->head;
    while (curr != NULL) {
        if(predicate_func(curr->value)) return true;
        curr = curr->next;
    }

    return false;
}

long long charlist_reduce(CharList list, long long (*reduce_func)(long long acc, char value), long long initial) {
    if (charlist_is_empty(list) || reduce_func == NULL) return 0;

    CharNode curr = list->head;
    long long acc = initial;
    while (curr != NULL) {
        acc = reduce_func(acc, curr->value);
        curr = curr->next;
    }

    return acc;
}

void charlist_print(CharList list) {
    if (charlist_is_empty(list)) {
        printf("NULL");
        return;
    }

    CharNode curr = list->head;
    while (curr != NULL) {
        printf("%c", curr->value);
        printf(" -> ");

        curr = curr->next;
    }
    printf("NULL");
}

bool charlist_contains(CharList list, char target) {
    if (charlist_is_empty(list)) return false;

    CharNode curr = list->head;
    while (curr != NULL) {
        if (curr->value == target) return true;
        curr = curr->next;
    }
    return false;
}

bool charlist_equals(CharList list1, CharList list2) {
    if (charlist_not_exists(list1) || charlist_not_exists(list2)) return false;

    if (list1->size != list2->size) return false;

    CharNode curr1 = list1->head;
    CharNode curr2 = list2->head;
    while (curr1 != NULL && curr2 != NULL ) {
        if (curr1->value != curr2->value) return false;
       
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return true;
}