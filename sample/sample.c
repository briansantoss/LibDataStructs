#include <stdio.h>
#include "linkedlist.h"

int main() {
    LinkedList* list = list_init();
    if (list == NULL) {
        perror("Error: cannot create the list.");
    }

    
    list_push_int(list, 10);
    list_push_int(list, 35);
    
    list_push_bool(list, false);
    
    list_push_end_bool(list, true);

    list_display(list);
    
    list_free(list);

    list_display(list);
    return 0;
}