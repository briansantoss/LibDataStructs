#include <stdio.h>
#include "map/intmap.h"

int main() {
    IntMap ages = intmap_new();
    if (ages == NULL) {
        perror("Error: cannot create the map.");
        return 1;
    }

    intmap_insert(ages, "Brian", 21);
    intmap_insert(ages, "Erika", 19);
    intmap_insert(ages, "Felipe", 22);

    char** ages_keys = intmap_keys(ages);
    if (ages_keys == NULL) {
        perror("Error: cannot get the map keys.");
        return 1;
    }

    for (int i = 0; i < intmap_len(ages); i++) {
        printf("\nKey: %s", ages_keys[i]);
    }

    return 0;
}