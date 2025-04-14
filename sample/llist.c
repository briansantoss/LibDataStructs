#include <stdio.h>
#include "linkedlist.h"

int main() {
    LinkedList list = list_init();
    if (list == NULL) {
        perror("Error: cannot create the list.");
    }
    
    list_push_int(list, 10);
    
    list_push_end_bool(list, true);
    
    list_insert_char_at(list, 1, 'e');
    
    list_remove_at(list, 2);
    
    list_insert_string_at(list, 2, "Erika");
    
    list_display(list);

    printf("Linked list len: %lld\n", list_len(list));
    
    list_reverse(list);

    list_display(list);

    list_free(list);

    return 0;
}