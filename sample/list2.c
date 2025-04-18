#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "intlist.h"

#define ARR_SIZE 10

int greater_than5000(int num) {
    return num > 5000;
}

int main() {
    int arr[ARR_SIZE];

    srand(time(NULL));

    for (int i = 0; i < ARR_SIZE; i++) {
        arr[i] = rand();
    }

    printf("Array created: ");
    for (int i = 0; i < ARR_SIZE; i++) {
        printf("[ %d ] ", arr[i]);
    }
    
    IntList list = intlist_from_array(arr, ARR_SIZE);
    if (list == NULL) {
        perror("Error: cannot create the list");
        return 1;
    }
    
    printf("\nList created:  ");
    for (int i = 0; i < intlist_len(list); i++) {
        printf("[ %d ] ", intlist_get_at(list, i));
    }
    
    intlist_pop(list);
    intlist_pop(list);
    
    printf("\n\nList after modifications: ");
    for (int i = 0; i < intlist_len(list); i++) {
        printf("[ %d ] ", intlist_get_at(list, i));
    }

    intlist_append(list, 5001);

    IntList filtered = intlist_filter(list, greater_than5000);
    if (filtered == NULL) {
        perror("Error: cannot filter the list");
        return 1;
    }

    printf("\n\nElements greater than 5000: ");
    for (int i = 0; i < intlist_len(filtered); i++) {
        printf("[ %d ] ", intlist_get_at(filtered, i));
    }    
    
    intlist_free(list);
    intlist_free(filtered);

    return 0;
}