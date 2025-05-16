#include "linkedlist/charlist.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack/charstack.h"
#include "queue/charqueue.h"
#include "internal/memmngr.h"

typedef struct charnode {
    char value;
    struct charnode* next;
    struct charnode* prev;
} *CharNode;

struct charlist {
    CharNode head;
    CharNode tail;
    size_t size;
};

static bool charlist_not_exists(const CharList list) {
    return !list;
}

bool charlist_is_empty(const CharList list) {
    return charlist_not_exists(list) || !list->head;
}

static CharNode charlist_create_node(char value) {
    CharNode new_node = (CharNode) malloc(sizeof (struct charnode));
    if (!new_node) return NULL;

    *new_node = (struct charnode) {.value = value, .prev = NULL, .next = NULL};
    return new_node;
}

static void charlist_free(CharList list) {
    charlist_clear(list);
    free(list);
}

CharList charlist_new(void) {
    CharList new_list = (CharList) malloc(sizeof (struct charlist));
    if (charlist_not_exists(new_list)) return NULL;

    if (!memmngr_register(new_list, (void (*)(void*)) charlist_free)) {
        free(new_list);
        return NULL;
    }
    
    *new_list = (struct charlist) {.head = NULL, .tail = NULL, .size = 0};
    return new_list;
}

void charlist_clear(CharList list) {
    if (charlist_is_empty(list)) return;

    CharNode curr = list->head;
    while (curr) {
        CharNode next = curr->next;

        free(curr);

        curr = next;
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

bool charlist_push_front(CharList list, char value) {
    if (charlist_not_exists(list)) return false;

    CharNode new_node = charlist_create_node(value);
    if (!new_node) return false;

    if (charlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        list->head->prev = new_node;
        new_node->next = list->head;
        list->head = new_node;
    }

    list->size++;
    return true;
}

bool charlist_push(CharList list, char value) {
    if (charlist_not_exists(list)) return false;

    CharNode new_node = charlist_create_node(value);
    if (!new_node) return false;
    
    if (charlist_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return true;
}

bool charlist_push_at(CharList list, char value, size_t index) {
    if (charlist_not_exists(list) || index > list->size) return false;

    if (index == 0) return charlist_push_front(list, value);
    if (index == list->size) return charlist_push(list, value);

    CharNode new_node = charlist_create_node(value);
    if (!new_node) return false;

    CharNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);

    new_node->prev = curr->prev;
    new_node->next = curr;
    curr->prev->next = new_node;
    curr->prev = new_node;

    list->size++;
    return true;
}

bool charlist_get_at(const CharList list, size_t index, char* out) {
    if (charlist_is_empty(list) || !out || index >= list->size) return false;

    CharNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);
    
    *out = curr->value;

    return true;
}

int charlist_index(const CharList list, char target) {
    if (charlist_is_empty(list)) return -1;

    CharNode curr = list->head;
    for (size_t i = 0; curr; i++, curr = curr->next) {
        if (curr->value == target) return i;
    }
    
    return -1;
}

size_t charlist_count(const CharList list, char target) {
    if (charlist_is_empty(list)) return 0;

    size_t freq = 0;

    CharNode curr = list->head;
    while (curr) {
        if (curr->value == target) freq++;
        curr = curr->next;
    }

    return freq;
}

void charlist_pop_front(CharList list) {
    if (charlist_is_empty(list)) return;

    CharNode old_head = list->head;
    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->next = NULL;
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
        CharNode old_tail = list->tail;

        list->tail = list->tail->next;
        list->tail->next = NULL;

        free(old_tail);
    }

    list->size--;
}

void charlist_pop_at(CharList list, size_t index) {
    if (charlist_is_empty(list) || index >= list->size) return;

    if (index == 0) {
        charlist_pop_front(list);
        return;
    }

    if (index == list->size - 1) {
        charlist_pop(list);
        return;
    }

    CharNode curr = list->head;
    for (size_t i = 0; i < index; i++, curr = curr->next);

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;

    free(curr);

    list->size--;
}

size_t charlist_size(const CharList list) {
    return charlist_not_exists(list) ? 0 : list->size;
}

void charlist_reverse(CharList list) {
    if (charlist_is_empty(list)) return;

    CharNode curr = list->head;
    while (curr) {
        CharNode next = curr->next;
        
        curr->next = curr->prev;
        curr->prev = next;
        
        curr = next;
    }
    
    CharNode head = list->head;
    list->head = list->tail;
    list->tail = head;
}

char* charlist_to_string(const CharList list) {
    if (charlist_is_empty(list)) return NULL;

    size_t str_size = list->size;

    char* str = (char*) malloc(sizeof (char) * (str_size + 1));
    if (!str) return NULL;

    if(!memmngr_register(str, free)) {
        free(str);
        return NULL;
    }

    CharNode curr = list->head;
    for (size_t i = 0; i < str_size; i++, curr = curr->next) {
        str[i] = curr->value;
    }

    str[str_size] = '\0';
    return str;
}

CharStack charlist_to_stack(const CharList list) {
    if (charlist_is_empty(list)) return NULL;

    CharStack new_stack = charstack_new();
    if (!new_stack) return NULL;

    CharNode curr = list->head;
    while (curr) {
        if (!charstack_push(new_stack, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }

    return new_stack;
}

CharQueue charlist_to_queue(const CharList list) {
    if (charlist_is_empty(list)) return NULL;
    
    CharQueue new_queue = charqueue_new();
    if (!new_queue) return NULL;
    
    CharNode curr = list->head;
    while (curr) {
        if (!charqueue_enqueue(new_queue, curr->value)) {
            memmngr_rollback();
            return NULL;
        }
        curr = curr->next;
    }    

    return new_queue;
}

CharList charlist_from_string(char* str, size_t size) {
    if (!str || size == 0) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        if (!charlist_push(new_list, str[i])) {
            memmngr_rollback();
            return NULL;
        }
    }
    
    return new_list;
}

void charlist_foreach(CharList list, char (*callback_func)(char value)) {
    if (charlist_is_empty(list) || !callback_func) return;

    CharNode curr = list->head;
    while (curr) {
        curr->value = callback_func(curr->value);
        curr = curr->next;
    }
}

CharList charlist_copy(const CharList list) {
    if (charlist_is_empty(list)) return NULL;

    CharList copy = charlist_new();
    if (charlist_not_exists(copy)) return NULL;

    CharNode curr = list->head;
    while (curr) {
        if (!charlist_push(copy, curr->value)) {
            memmngr_rollback();
            return NULL;
        }

        curr = curr->next;
    }

    return copy;
}

CharList charlist_map(const CharList list, char (*callback_func)(char value)) {
    if (charlist_is_empty(list) || !callback_func) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;

    CharNode curr = list->head;
    while (curr) {
        if (!charlist_push(new_list, callback_func(curr->value))) {
            memmngr_rollback();
            return NULL;
        }

        curr = curr->next;
    }

    return new_list;
}

CharList charlist_zip(const CharList list1, const CharList list2) {
    if (charlist_is_empty(list1) || charlist_is_empty(list2)) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;

    CharNode curr1 = list1->head;
    CharNode curr2 = list2->head;
    while (curr1 && curr2) {
        if (!charlist_push(new_list, curr1->value) || !charlist_push(new_list, curr2->value)) {
            memmngr_rollback();
            return NULL;
        }

        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return new_list;
}

CharList charlist_filter(const CharList list, bool (*predicate_func)(char value)) {
    if (charlist_is_empty(list) || !predicate_func) return NULL;

    CharList new_list = charlist_new();
    if (charlist_not_exists(new_list)) return NULL;

    CharNode curr = list->head;
    while (curr) {
        if (predicate_func(curr->value)) {
            if (!charlist_push(new_list, curr->value)) {
                memmngr_rollback();
                return NULL;
           }
        }

        curr = curr->next;
    }

    return new_list;
}

bool charlist_all(const CharList list, bool (*predicate_func)(char value)) {
    if (charlist_is_empty(list) || !predicate_func) return true;

    CharNode curr = list->head;
    while (curr) {
        if(!predicate_func(curr->value)) return false;
        curr = curr->next;
    }
    
    return true;
}

bool charlist_any(const CharList list, bool (*predicate_func)(char value)) {
    if (charlist_is_empty(list) || !predicate_func) return false;

    CharNode curr = list->head;
    while (curr) {
        if(predicate_func(curr->value)) return true;
        curr = curr->next;
    }

    return false;
}

long long charlist_reduce(const CharList list, long long (*reduce_func)(long long acc, char value), long long initial) {
    if (charlist_is_empty(list) || !reduce_func) return initial;

    CharNode curr = list->head;
    long long acc = initial;
    while (curr) {
        acc = reduce_func(acc, curr->value);
        curr = curr->next;
    }

    return acc;
}

void charlist_print(const CharList list) {
    if (charlist_is_empty(list)) {
        printf("NULL");
        return;
    }

    CharNode curr = list->head;
    while (curr) {
        printf("%c", curr->value);

        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
}

bool charlist_contains(const CharList list, char target) {
    if (charlist_is_empty(list)) return false;

    CharNode curr = list->head;
    while (curr) {
        if (curr->value == target) return true;
        curr = curr->next;
    }
    return false;
}

bool charlist_equals(const CharList list1, const CharList list2) {
    if (charlist_not_exists(list1) || charlist_not_exists(list2)) return false;

    if (list1->size != list2->size) return false;

    CharNode curr1 = list1->head;
    CharNode curr2 = list2->head;
    while (curr1 && curr2) {
        if (curr1->value != curr2->value) return false;
       
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    return true;
}