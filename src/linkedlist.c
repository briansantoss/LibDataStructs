#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

static bool list_not_exists(LinkedList* list) {
    return list == NULL;
}

static bool list_is_empty(LinkedList* list) {
    return list->head == NULL;
}

static ListNode* list_create_node(TypedValue value_data) {
    ListNode* new_node = (ListNode*) malloc(sizeof (ListNode));
    if (new_node == NULL) {
        return NULL;
    }

    switch (value_data.value_type) {
        case BOOL_TYPE:
            new_node->data.value.bool_value = value_data.value.bool_value;
            break;

        case CHAR_TYPE:
            new_node->data.value.char_value = value_data.value.char_value;
            break;

        case INT_TYPE:
            new_node->data.value.int_value = value_data.value.int_value;
            break;

        case FLOAT_TYPE:
            new_node->data.value.float_value = value_data.value.float_value;
            break;

        case DOUBLE_TYPE:
            new_node->data.value.double_value = value_data.value.double_value;
            break;
        
        case STRING_TYPE:
            char* value_copy = (char*) malloc(strlen(value_data.value.string_value) + 1);
            if (value_copy == NULL) {
                free(new_node);
                return NULL;
            }

            strcpy(value_copy, value_data.value.string_value);
            new_node->data.value.string_value = value_copy;
    }

    new_node->data.value_type = value_data.value_type;
    new_node->next = NULL;
    return new_node;
}

static void list_free_node(ListNode* node) {
    if (node == NULL) {
        return;
    }
    
    if (node->data.value_type == STRING_TYPE) {
        free(node->data.value.string_value);
    }

    free(node);
}

static void list_push(LinkedList* list, TypedValue value_data) {
    if (list_not_exists(list)) {
        return;
    }

    ListNode* new_node = list_create_node(value_data);
    if (new_node == NULL) {
        return;
    }

    if (list_is_empty(list)) {
        list->tail = new_node;
    }
    
    new_node->next = list->head;
    list->head = new_node;

    list->size++;
}

static void list_push_end(LinkedList* list, TypedValue value_data) {
    if (list_not_exists(list)) {
        return;
    }

    ListNode* new_node = list_create_node(value_data);
    if (new_node == NULL) {
        return;
    }
    
    if (list_is_empty(list)) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
}

static void list_insert_at(LinkedList* list, size_t index, TypedValue value_data) {
    if (list_not_exists(list) || index > list->size) {
        return;
    }

    if (index == 0) {
        list_push(list, value_data);
        return;
    }

    if (index == list->size) {
        list_push_end(list, value_data);
        return;
    }
    
    ListNode* new_node = list_create_node(value_data);
    if (new_node == NULL) {
        return;
    }

    ListNode* curr = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;

    list->size++;
}

LinkedList* list_init(void) {
    LinkedList* new_list = (LinkedList*) malloc(sizeof (LinkedList));
    if (new_list == NULL) {
        return NULL;
    }

    new_list->size = 0;
    new_list->head = new_list->tail = NULL;
    return new_list;
}

void list_free(LinkedList* list) {
    if (list_not_exists(list)) {
        return;
    }

    ListNode* curr = list->head;
    while (curr != NULL) {
        ListNode* next = curr->next;

        list_free_node(curr);

        curr = next;
    }

    list->head = list->tail = NULL;

    free(list);
}

size_t list_len(LinkedList* list) {
    if (list_not_exists(list)) {
        return 0;
    }
    return list->size;
}

void list_push_bool(LinkedList* list, bool value) {
    TypedValue value_data = {.value = {.bool_value = value}, .value_type = BOOL_TYPE};

    list_push(list, value_data);
}

void list_push_char(LinkedList* list, char value) {
    TypedValue value_data = {.value = {.char_value = value}, .value_type = CHAR_TYPE};

    list_push(list, value_data);
}

void list_push_int(LinkedList* list, int value) {
    TypedValue value_data = {.value = {.int_value = value}, .value_type = INT_TYPE};

    list_push(list, value_data);
}

void list_push_float(LinkedList* list, float value) {
    TypedValue value_data = {.value = {.float_value = value}, .value_type = FLOAT_TYPE};

    list_push(list, value_data);
}

void list_push_double(LinkedList* list, double value) {
    TypedValue value_data = {.value = {.double_value = value}, .value_type = DOUBLE_TYPE};

    list_push(list, value_data);
}

void list_push_string(LinkedList* list, char* value) {
    TypedValue value_data = {.value = {.string_value = value}, .value_type = STRING_TYPE};

    list_push(list, value_data);
}

void list_push_end_bool(LinkedList* list, bool value) {
    TypedValue value_data = {.value = {.bool_value = value}, .value_type = BOOL_TYPE};
    list_push_end(list, value_data);
}

void list_push_end_char(LinkedList* list, char value) {
    TypedValue value_data = {.value = {.char_value = value}, .value_type = CHAR_TYPE};
    list_push_end(list, value_data);
}

void list_push_end_int(LinkedList* list, int value) {
    TypedValue value_data = {.value = {.int_value = value}, .value_type = INT_TYPE};
    list_push_end(list, value_data);
}

void list_push_end_float(LinkedList* list, float value) {
    TypedValue value_data = {.value = {.float_value = value}, .value_type = FLOAT_TYPE};
    list_push_end(list, value_data);
}

void list_push_end_double(LinkedList* list, double value) {
    TypedValue value_data = {.value = {.double_value = value}, .value_type = DOUBLE_TYPE};
    list_push_end(list, value_data);
}

void list_push_end_string(LinkedList* list, char* value) {
    TypedValue value_data = {.value = {.string_value = value}, .value_type = STRING_TYPE};
    list_push_end(list, value_data);
}

void list_insert_bool_at(LinkedList* list, size_t index, bool value) {
    TypedValue value_data = {.value = {.bool_value = value}, .value_type = BOOL_TYPE};
    list_insert_at(list, index, value_data);
}

void list_insert_char_at(LinkedList* list, size_t index, char value) {
    TypedValue value_data = {.value = {.char_value = value}, .value_type = CHAR_TYPE};
    list_insert_at(list, index, value_data);
}

void list_insert_int_at(LinkedList* list, size_t index, int value) {
    TypedValue value_data = {.value = {.int_value = value}, .value_type = INT_TYPE};
    list_insert_at(list, index, value_data);
}

void list_insert_float_at(LinkedList* list, size_t index, float value) {
    TypedValue value_data = {.value = {.float_value = value}, .value_type = FLOAT_TYPE};
    list_insert_at(list, index, value_data);
}

void list_insert_double_at(LinkedList* list, size_t index, double value) {
    TypedValue value_data = {.value = {.double_value = value}, .value_type = DOUBLE_TYPE};
    list_insert_at(list, index, value_data);
}

void list_insert_string_at(LinkedList* list, size_t index, char* value) {
    TypedValue value_data = {.value = {.string_value = value}, .value_type = STRING_TYPE};
    list_insert_at(list, index, value_data);
}

void list_display(LinkedList* list) {
    if (list_is_empty(list)) {
        return;
    }

    ListNode* curr = list->head;
    while (curr != NULL) {
        switch (curr->data.value_type) {
            case BOOL_TYPE:
                printf("%s", curr->data.value.bool_value == 0 ? "false" : "true");
                break;
    
            case CHAR_TYPE:
                printf("%c", curr->data.value.char_value);
                break;
    
            case INT_TYPE:
                printf("%d", curr->data.value.int_value);
                break;
    
            case FLOAT_TYPE:
                printf("%f", curr->data.value.float_value);
                break;
    
            case DOUBLE_TYPE:
                printf("%lf", curr->data.value.double_value);
                break;
            
            case STRING_TYPE:
                printf("%s", curr->data.value.string_value);
        }
        printf(" -> ");

        curr = curr->next;
    }
    puts("NULL");
}