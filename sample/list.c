#include <stdio.h>
#include "intlist.h"

int double_num(int num) {
    return num * 2;
}    

int square_num(int num) {
    return num * num;
}

int main() {
    IntList list = intlist_init();
    if (list == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }

    intlist_push(list, 10);
    intlist_append(list, 20);
    intlist_push(list, 30);

    printf("\nValue at position %d: %d", 0, intlist_get_at(list, 0));
    printf("\nValue at position %d: %d", 1, intlist_get_at(list, 1));
    printf("\nValue at position %d: %d", 2, intlist_get_at(list, 2));
    
    intlist_reverse(list);
    intlist_pop_start(list);

    printf("\n\nAfter reversing the list and remove the first node..");
    printf("\nValue at position %d: %d", 0, intlist_get_at(list, 0));
    printf("\nValue at position %d: %d", 1, intlist_get_at(list, 1));
    
    intlist_foreach(list, double_num);

    printf("\n\nAfter double each element in the list..");
    printf("\nValue at position %d: %d", 0, intlist_get_at(list, 0));
    printf("\nValue at position %d: %d", 1, intlist_get_at(list, 1));
    
    printf("\nThe list %s contain the value 1234", intlist_contains(list, 1234) ? "does" : "does not");
    
    IntList squared = intlist_map(list, square_num);
    if (squared == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }
    
    printf("\n\nA squared version.. ");
    printf("\nValue at position %d: %d", 0, intlist_get_at(squared, 0));
    printf("\nValue at position %d: %d", 1, intlist_get_at(squared, 1));
    
    printf("\n\nSquared version total sum: %lld", intlist_sum(squared));

    intlist_free(list);
    intlist_free(squared);

    return 0;
}
