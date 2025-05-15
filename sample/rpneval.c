#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack/intstack.h"

/**
 * @file rpneval.c
 * @brief Implements a Reverse Polish Notation (RPN) evaluator using a stack.
 * 
 * This program evaluates mathematical expressions written in RPN.
 * It processes a tokenized expression: pushing tokens onto the stack when they are numbers;
 * otherwise, it performs the appropriate operation and pushes the result back to the stack.
 */

int is_operator(char* token) {
    return strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0;
}

int main() {
    IntStack st = intstack_new();
    if (!st) {
        fprintf(stderr, "Error: cannot create the stack.");
        return 1;
    }

    // Original expressions in infix notation (for display only)
    char *exp[3] = {
        "(2 + 1) * 3",
        "4 + (13 / 5)",
        "6 / ((9 + 3) * (-11)) * 10 + 17 + 5"
    };

    // Corresponding expression in Reverse Polish Notation
    char *rpn_exp[3][13] = {
        {"2","1","+","3","*"}, // Expression: (2 + 1) * 3
        {"4","13","5","/","+"}, // Expression: 4 + 13 / 5
        {"10","6","9","3","+","-11","*","/","*","17","+","5","+"}, // Expresion: ((10 * (6 / ((9 + 3) * (-11))) + 17) + 5
    }; 

    int rpn_exp_tokens_count[3] = {5, 5, 13}; // Number of tokens in each RPN expression

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < rpn_exp_tokens_count[i]; j++) {
            char* token = rpn_exp[i][j];

            // Checks token's type: number or operator
            if (!is_operator(token)) {
                intstack_push(st, atoi(token));
            } else {
                int op1, op2;
                if (intstack_pop(st, &op2) && intstack_pop(st, &op1)) {
                    int result;
                    if (strcmp(token, "+") == 0) {
                        result = op1 + op2;
                    } else if (strcmp(token, "-") == 0) {
                        result = op1 - op2;
                    } else if (strcmp(token, "*") == 0) {
                        result = op1 * op2;
                    } else {
                        result = op1 / op2;
                    }

                    intstack_push(st, result);
                }  
            }
        }

        int total;
        if (intstack_peek(st, &total)) {
            printf("Expression: %s\nResult: %d\n\n", exp[i], total);
        }

        intstack_clear(st);
    }
    
    return 0;
}