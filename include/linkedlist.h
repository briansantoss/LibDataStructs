#ifndef LINKEDLIST_H

#define LINKEDLIST_H

#include "dsdefs.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct ListNode {
    TypedValue data;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    ListNode* tail;
    size_t size;
} LinkedList;

LinkedList* list_init(void);
void list_free(LinkedList* list);
size_t list_len(LinkedList* list);

void list_push_bool(LinkedList* list, bool value);
void list_push_char(LinkedList* list, char value);
void list_push_int(LinkedList* list, int value);
void list_push_float(LinkedList* list, float value);
void list_push_double(LinkedList* list, double value);
void list_push_string(LinkedList* list, char* value);

void list_push_end_bool(LinkedList* list, bool value);
void list_push_end_char(LinkedList* list, char value);
void list_push_end_int(LinkedList* list, int value);
void list_push_end_float(LinkedList* list, float value);
void list_push_end_double(LinkedList* list, double value);
void list_push_end_string(LinkedList* list, char* value);

void list_insert_bool_at(LinkedList* list, size_t index, bool value);
void list_insert_char_at(LinkedList* list, size_t index, char value);
void list_insert_int_at(LinkedList* list, size_t index, int value);
void list_insert_float_at(LinkedList* list, size_t index, float value);
void list_insert_double_at(LinkedList* list, size_t index, double value);
void list_insert_string_at(LinkedList* list, size_t index, char* value);

void list_remove_at(LinkedList* list, size_t index);

// TODO
// void list_reverse(LinkedList* list);

void list_display(LinkedList* list);

#endif