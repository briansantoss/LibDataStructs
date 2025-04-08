#ifndef DATASTRUCTS_H

#define DATASTRUCTS_H

#include <stdbool.h>

typedef enum {
    BOOL_TYPE,
    CHAR_TYPE,
    INT_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    STRING_TYPE,
} ValueType;

typedef union {
    bool bool_value;
    char char_value;
    int int_value;
    float float_value;
    double double_value;
    char* string_value;
} Value;

typedef struct {
    ValueType value_type;
    Value value;
} TypedValue;

#endif