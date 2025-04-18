#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intstack.h"

int main() {
    IntStack st = intstack_init();
    if (st == NULL) {
        perror("Error: cannot create the stack.");
        return 1;
    }

    char *exp[3] = {
        "(2 + 1) * 3",
        "4 + (13 / 5)",
        "((10 * (6 / ((9 + 3) * -11))) + 17) + 5",
    };

    char *rpn_exp[3][13] = {
        {"2","1","+","3","*"}, // Expressão: (2 + 1) * 3
        {"4","13","5","/","+"}, // Expressão: 4 + 13 / 5
        {"10","6","9","3","+","-11","*","/","*","17","+","5","+"}, // Expressão: ((10 * (6 / ((9 + 3) * -11))) + 17) + 5
    }; 

    int rpn_exp_tokens_count[3] = {5, 5, 13};


    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < rpn_exp_tokens_count[i]; j++) {
            char *token = rpn_exp[i][j];

            if (strcmp(token, "+") && strcmp(token, "-") && strcmp(token, "*") && strcmp(token, "/")) {
                intstack_push(st, atoi(token));
            } else {
                int op2 = intstack_pop(st) ;
                int op1 = intstack_pop(st);

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

        printf("Expression: %s\nResult: %d\n\n", exp[i], intstack_peek(st));

        intstack_clear(st);
    }
    
    intstack_free(st);
    return 0;
}