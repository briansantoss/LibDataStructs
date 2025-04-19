#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "linkedlist/intlist.h"

#define ARR_SIZE 10

bool greater_than5000(int num) {
    return num > 5000;
}

bool less_than10(int num) {
    return num < 10;
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
    intlist_print(list);
    
    intlist_pop(list);
    intlist_pop(list);
    
    printf("\n\nList after modifications: ");
    intlist_print(list);

    intlist_append(list, 5001);
    
    IntList filtered = intlist_filter(list, greater_than5000);
    if (filtered == NULL) {
        perror("Error: cannot filter the list");
        return 1;
    }
    
    printf("\n\nElements greater than 5000: ");
    intlist_print(filtered);
    
    intlist_free(list);
    intlist_free(filtered);
    
    printf("\n\nAll elements are greather than 5000? %s", intlist_all(filtered, greater_than5000) ? "Yes" : "No");
    
    intlist_append(filtered, 5000);
    
    printf("\n\nStill all elements are greather than 5000? %s", intlist_all(filtered, greater_than5000) ? "Yes" : "No");
    
    printf("\n\nAny element less than 10? %s", intlist_any(filtered, less_than10) ? "Yes" : "No");

    return 0;
}