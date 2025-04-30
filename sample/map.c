#include <stdio.h>
#include "map/intmap.h"

int main() {
    IntMap ages_map = intmap_new();
    if (ages_map == NULL) {
        perror("Error: cannot create the map.");
        return 1;
    }

    intmap_insert(ages_map, "Brian", 21);
    intmap_insert(ages_map, "Erika", 19);
    intmap_insert(ages_map, "Felipe", 22);

    char** ages_keys = intmap_keys(ages_map);
    if (ages_keys == NULL) {
        perror("Error: cannot get the map keys.");
    }

    printf("\nActual map keys: ");
    for (int i = 0; i < intmap_len(ages_map); i++) {
        printf("\nKey: %s", ages_keys[i]);
    }
    
    printf("\n\nRemoving the key 'Felipe'...");
    intmap_remove(ages_map, "Felipe");

    ages_keys = intmap_keys(ages_map);
    if (ages_keys == NULL) {
        perror("Error: cannot get the map keys.");
    }
    
    int* ages = intmap_values(ages_map);
    if (ages == NULL) {
        perror("Error: cannot get the map values.");
        return 1;
    }

    printf("\n\nKeys and values after key removal: ");
    for (int i = 0; i < intmap_len(ages_map); i++) {
        printf("\nKey: %s\tValue: %d", ages_keys[i], ages[i]);
    }

    return 0;
}