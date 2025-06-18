#include "linkedlist/charlist.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack/charstack.h"
#include "queue/charqueue.h"
#include "internal/memmngr.h"

typedef struct _charnode {
    char value;
    struct _charnode* next;
    struct _charnode* prev;
} *CharNode;

struct _charlist {
    CharNode head;
    CharNode tail;
    size_t size;
};

static bool _charlist_not_exists(const CharList list) {
    return !list;
}

bool charlist_is_empty(const CharList list) {
    return _charlist_not_exists(list) || !list->head;
}

static CharNode _charlist_create_node(char value, CharNode prev, CharNode next) {
    CharNode new_node = (CharNode) malloc(sizeof (struct _charnode));
    if (!new_node) return NULL;

    *new_node = (struct _charnode) {.value = value, .prev = prev, .next = next};
    return new_node;
}

static void _charlist_free(CharList list) {
    charlist_clear(list);
    free(list);
}

static CharNode _charlist_node_at(const CharList list, size_t index) {
    CharNode node;
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

static bool _charlist_link_before(CharList list, char value, CharNode succ) {
    CharNode pred = succ ? succ->prev : list->tail;

    CharNode new_node = _charlist_create_node(value, pred, succ);
    if (!new_node) return false;

    if (!pred)  list->head = new_node;
    else        pred->next = new_node;

    if (!succ)  list->tail = new_node;
    else        succ->prev = new_node;

    list->size++;
    return true;
}

static void _charlist_unlink_node(CharList list, CharNode node) {
    CharNode pred = node->prev;
    CharNode succ = node->next;

    if (!pred)  list->head = succ;
    else        pred->next = succ;

    if (!succ)  list->tail = pred;
    else        succ->prev = pred;

    free(node);
    list->size--;
}

CharList charlist_new(void) {
    CharList new_list = (CharList) malloc(sizeof (struct _charlist));
    if (_charlist_not_exists(new_list)) return NULL;

    if (!_memmngr_register(new_list, (void (*)(void*)) _charlist_free)) {
        free(new_list);
        return NULL;
    }
    
    *new_list = (struct _charlist) {.head = NULL, .tail = NULL, .size = 0};
    return new_list;
}

void charlist_clear(CharList list) {
    if (_charlist_not_exists(list)) return;
    
    for (CharNode curr = list->head, next; curr; curr = next) {
        next = curr->next;
        free(curr);
    }

    list->head = list->tail = NULL;
    list->size = 0;
}

bool charlist_push_front(CharList list, char value) {
    return !_charlist_not_exists(list) && _charlist_link_before(list, value, list->head);
}

bool charlist_push(CharList list, char value) {
    return !_charlist_not_exists(list) && _charlist_link_before(list, value, NULL);
}

bool charlist_push_at(CharList list, char value, size_t index) {
    return !_charlist_not_exists(list) && index <= list->size && _charlist_link_before(list, value, index < list->size ? _charlist_node_at(list, index) : NULL);
}

bool charlist_front(const CharList list, char* out) {
    if (charlist_is_empty(list) || !out) return false;
    *out = list->head->value;
    return true;
}

bool charlist_get_at(const CharList list, size_t index, char* out) {
    if (charlist_is_empty(list) || !out || index >= list->size) return false;
    *out = _charlist_node_at(list, index)->value;
    return true;
}

bool charlist_back(const CharList list, char* out) {
    if (charlist_is_empty(list) || !out) return false;
    *out = list->tail->value;
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
    for (CharNode curr = list->head; curr; curr = curr->next) {
        if (curr->value == target) freq++;
    }
    return freq;
}

void charlist_pop_front(CharList list) {
    if (charlist_is_empty(list)) return;
    _charlist_unlink_node(list, list->head);
}

void charlist_pop(CharList list) {
    if (charlist_is_empty(list)) return;
    _charlist_unlink_node(list, list->tail);
}

void charlist_pop_at(CharList list, size_t index) {
    if (charlist_is_empty(list) || index >= list->size) return;
    _charlist_unlink_node(list, _charlist_node_at(list, index)); 
}

size_t charlist_size(const CharList list) {
    return _charlist_not_exists(list) ? 0 : list->size;
}

void charlist_reverse(CharList list) {
    if (charlist_is_empty(list)) return;

    for (CharNode curr = list->head, next; curr; curr = next) {
        next = curr->next;
        curr->next = curr->prev;
        curr->prev = next;
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

    if(!_memmngr_register(str, free)) {
        free(str);
        return NULL;
    }

    CharNode curr = list->head;
    for (size_t i = 0; i < str_size; i++, curr = curr->next) str[i] = curr->value;
    str[str_size] = '\0';
    return str;
}

CharStack charlist_to_stack(const CharList list) {
    if (_charlist_not_exists(list)) return NULL;

    CharStack new_stack = charstack_new();
    if (!new_stack) return NULL;

    for (CharNode curr = list->head; curr; curr = curr->next) {
        if (!charstack_push(new_stack, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_stack;
}

CharQueue charlist_to_queue(const CharList list) {
    if (_charlist_not_exists(list)) return NULL;
    
    CharQueue new_queue = charqueue_new();
    if (!new_queue) return NULL;
    
    for (CharNode curr = list->head; curr; curr = curr->next) {
        if (!charqueue_enqueue(new_queue, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }    
    return new_queue;
}

CharList charlist_from_string(char* str, size_t size) {
    if (!str || size == 0) return NULL;

    CharList new_list = charlist_new();
    if (_charlist_not_exists(new_list)) return NULL;
    
    for (size_t i = 0; i < size; i++) {
        if (!charlist_push(new_list, str[i])) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

void charlist_foreach(CharList list, char (*callback_func)(char value)) {
    if (_charlist_not_exists(list) || !callback_func) return;

    for (CharNode curr = list->head; curr; curr = curr->next) curr->value = callback_func(curr->value);
}

CharList charlist_copy(const CharList list) {
    if (_charlist_not_exists(list)) return NULL;

    CharList copy = charlist_new();
    if (_charlist_not_exists(copy)) return NULL;

    for (CharNode curr = list->head; curr; curr = curr->next) {
        if (!charlist_push(copy, curr->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return copy;
}

CharList charlist_map(const CharList list, char (*callback_func)(char value)) {
    if (_charlist_not_exists(list)) return NULL;
    if (!callback_func) return charlist_copy(list);

    CharList new_list = charlist_new();
    if (_charlist_not_exists(new_list)) return NULL;

    
    for (CharNode curr = list->head; curr; curr = curr->next) {
        if (!charlist_push(new_list, callback_func(curr->value))) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

CharList charlist_filter(const CharList list, bool (*predicate_func)(char value)) {
    if (_charlist_not_exists(list)) return NULL;
    if (!predicate_func) return charlist_copy(list);

    CharList new_list = charlist_new();
    if (_charlist_not_exists(new_list)) return NULL;

    for (CharNode curr = list->head; curr; curr = curr->next) {
        if (predicate_func(curr->value)) {
            if (!charlist_push(new_list, curr->value)) {
                _memmngr_rollback();
                return NULL;
           }
        }
    }
    return new_list;
}

CharList charlist_zip(const CharList list1, const CharList list2) {
    if (_charlist_not_exists(list1) || _charlist_not_exists(list2)) return NULL;

    CharList new_list = charlist_new();
    if (_charlist_not_exists(new_list)) return NULL;

    for (CharNode curr1 = list1->head, curr2 = list2->head; curr1 && curr2; curr1 = curr1->next, curr2 = curr2->next) {
        if (!charlist_push(new_list, curr1->value) || !charlist_push(new_list, curr2->value)) {
            _memmngr_rollback();
            return NULL;
        }
    }
    return new_list;
}

bool charlist_all(const CharList list, bool (*predicate_func)(char value)) {
    if (_charlist_not_exists(list)) return false;
    if (!predicate_func) return true;

    for (CharNode curr = list->head; curr; curr = curr->next) if (!predicate_func(curr->value)) return false;
    return true;
}

bool charlist_any(const CharList list, bool (*predicate_func)(char value)) {
    if (_charlist_not_exists(list) || !predicate_func) return false;

    for (CharNode curr = list->head; curr; curr = curr->next) if (predicate_func(curr->value)) return true;
    return false;
}

long long charlist_reduce(const CharList list, long long (*reduce_func)(long long acc, char value), long long initial) {
    if (charlist_is_empty(list) || !reduce_func) return initial;

    long long acc = initial;
    for (CharNode curr = list->head; curr; curr = curr->next) acc = reduce_func(acc, curr->value);
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
    if (_charlist_not_exists(list)) return false;
    
    for (CharNode curr = list->head; curr; curr = curr->next) if (curr->value == target) return true;
    return false;
}

bool charlist_equals(const CharList list1, const CharList list2) {
     if (_charlist_not_exists(list1) || _charlist_not_exists(list2) || list1->size != list2->size) return false;

    for (CharNode curr1 = list1->head, curr2 = list2->head; curr1 && curr2; curr1 = curr1->next, curr2 = curr2->next) if (curr1->value != curr2->value) return false;
    return true;
}