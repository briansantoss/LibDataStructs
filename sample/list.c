#include <stdio.h>
#include "linkedlist/intlist.h"

int double_num(int num) {
    return num * 2;
}    

int square_num(int num) {
    return num * num;
}

int main() {
    IntList list = intlist_new();
    if (list == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }

    intlist_push(list, 10);
    intlist_append(list, 20);
    intlist_push(list, 30);

    printf("List: ");
    intlist_print(list);
    
    intlist_reverse(list);
    intlist_pop_start(list);

    printf("\n\nAfter reversing the list and remove the first node: ");
    intlist_print(list);
    
    intlist_foreach(list, double_num);

    printf("\n\nAfter double each element in the list: ");
    intlist_print(list);
    
    printf("\nThe list %s contain the value 1234", intlist_contains(list, 1234) ? "does" : "does not");
    
    IntList squared = intlist_map(list, square_num);
    if (squared == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }
    
    printf("\n\nA squared version: ");
    intlist_print(squared);
    
    printf("\n\nSquared version total sum: %lld", intlist_sum(squared));

    return 0;
}
