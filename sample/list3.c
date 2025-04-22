#include <stdio.h>
#include "linkedlist/intlist.h"
#include "linkedlist/charlist.h"

int successor(int num) {
    return num + 1;
}

int main() {
    IntList odds = intlist_new();
    if (odds == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }

    for (int i = 1; i <= 10; i += 2) {
        intlist_append(odds, i);
    }

    printf("Odd numbers between 1 and 10 (inclusive): ");
    intlist_print(odds);

    IntList evens = intlist_map(odds, successor);
    if (evens == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }

    printf("\nEvens list: ");
    intlist_print(evens);
    
    IntList zipped = intlist_zip(odds, evens);
    if (zipped == NULL) {
        perror("Error: cannot zip the lists.");
        return 1;
    }
    
    printf("\nZip result list: ");
    intlist_print(zipped);
    
    printf("\nRemoving number 9... ");
    intlist_pop(odds);
    
    printf("\nUpdated odds: ");
    intlist_print(odds);
    
    IntList zipped2 = intlist_zip(odds, evens);
    if (zipped2 == NULL) {
        perror("Error: cannot zip the lists.");
        return 1;
    }

    intlist_free(odds);
    intlist_free(evens);
    intlist_free(zipped);
    intlist_free(zipped2);
    
    printf("\nZip result list after remove: ");
    intlist_print(zipped2);
    
    printf("\n");
    for (int i = 0; i < 50; i++) printf("-");

    printf("\nSimilar example but with chars");
    CharList chlist = charlist_new();
    if (chlist == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }

    charlist_append(chlist, 'a');
    charlist_append(chlist, 'c');
    charlist_append(chlist, 'e');
    charlist_append(chlist, 'g');

    CharList chlist2 = charlist_new();
    if (chlist2 == NULL) {
        perror("Error: cannot create the list.");
        return 1;
    }

    charlist_append(chlist2, 'b');
    charlist_append(chlist2, 'd');
    charlist_append(chlist2, 'f');
    charlist_append(chlist2, 'h');

    printf("\n\nCharlist 1: ");
    charlist_print(chlist);
    printf("\nCharlist 2: ");
    charlist_print(chlist2);

    CharList zipped_chars = charlist_zip(chlist, chlist2);
    if (zipped_chars == NULL) {
        perror("Error: cannot zip the lists.");
        return 1;
    }

    printf("\nZip result: ");
    charlist_print(zipped_chars);
    
    printf("\nIndex of letter 'g' in the result: %d ", charlist_index(zipped_chars, 'g'));

    charlist_free(chlist);
    charlist_free(chlist2);
    charlist_free(zipped_chars);

    return 0;
}