#include <stdio.h>
#include "intlist.h"

int double_num(int num) {
    return num * 2;
}

int main() {
    IntList list = intlist_init();
    if (list == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }

    intlist_push(list, 10);
    intlist_push_end(list, 20);
    intlist_push(list, 30);

    printf("\nValue at position %d: %d", 0, intlist_get_at(list, 0));
    printf("\nValue at position %d: %d", 1, intlist_get_at(list, 1));
    printf("\nValue at position %d: %d", 2, intlist_get_at(list, 2));
    
    intlist_reverse(list);

    printf("\n\nAfter reversing the list..");
    printf("\nValue at position %d: %d", 0, intlist_get_at(list, 0));
    printf("\nValue at position %d: %d", 1, intlist_get_at(list, 1));
    printf("\nValue at position %d: %d", 2, intlist_get_at(list, 2));
    
    intlist_foreach(list, double_num);

    printf("\n\nAfter double each element in the list..");
    printf("\nValue at position %d: %d", 0, intlist_get_at(list, 0));
    printf("\nValue at position %d: %d", 1, intlist_get_at(list, 1));
    printf("\nValue at position %d: %d", 2, intlist_get_at(list, 2));

    printf("\nThe list %s contain the value 1234", intlist_contains(list, 1234) ? "does" : "does not");
    
    intlist_free(list);
    
    return 0;
}
