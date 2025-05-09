#include "map/intmap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_new() {
    IntMap map = intmap_new();
    assert(map);

    assert(intmap_is_empty(map));
    assert(intmap_size(map) == 0);

    assert(intmap_is_empty(NULL));
    assert(intmap_size(NULL) == 0);
}

void test_insert() {
    IntMap map = intmap_new();
    assert(map);

    int value;
    assert(intmap_insert(map, "a", 1) && intmap_has_key(map, "a"));
    assert(intmap_size(map) == 1);
    assert(intmap_get(map, "a", &value) && value == 1);
    assert(!intmap_insert(map, "a", 2));
    
    assert(intmap_insert(map, "key", 55) && intmap_has_key(map, "key"));
    assert(intmap_size(map) == 2);
    assert(intmap_get(map, "key", &value) && value == 55);
    assert(!intmap_insert(map, "key", 2));

    assert(!intmap_insert(NULL, "null", 10));
    assert(!intmap_insert(NULL, NULL, 45));
    assert(!intmap_insert(map, NULL, 108) && !intmap_has_key(map, NULL) && intmap_size(map) == 2);
}

void test_set() {
    IntMap map = intmap_new();
    assert(map);
    
    int value;
    assert(intmap_set(map, "new", 5) && intmap_has_key(map, "new"));
    assert(intmap_size(map) == 1);
    assert(intmap_get(map, "new", &value) && value == 5);
    
    assert(intmap_set(map, "new", 10)); 
    assert(intmap_get(map, "new", &value) && value == 10);
    assert(intmap_size(map) == 1);
    
    assert(!intmap_set(NULL, "a", 56));
    assert(!intmap_set(map, NULL, 190) && intmap_size(map) == 1);
}

void test_remove() {
    IntMap map = intmap_new();
    assert(map);
    intmap_insert(map, "delete", 123);
    assert(intmap_has_key(map, "delete"));
    intmap_remove(map, "delete");
    assert(!intmap_has_key(map, "delete"));
}

void test_clear() {
    IntMap map = intmap_new();
    assert(map);
    intmap_insert(map, "a", 1);
    intmap_insert(map, "b", 2);
    intmap_clear(map);
    assert(intmap_is_empty(map));
    assert(intmap_size(map) == 0);
}

void test_equals() {
    IntMap m1 = intmap_new();
    IntMap m2 = intmap_new();
    assert(m1 && m2);
    
    intmap_insert(m1, "x", 10);
    intmap_insert(m2, "x", 10);
    assert(intmap_equals(m1, m2));
    
    intmap_set(m2, "x", 20);
    assert(!intmap_equals(m1, m2));
}

void test_keys_and_values() {
    IntMap map = intmap_new();
    assert(map);
    intmap_insert(map, "one", 1);
    intmap_insert(map, "two", 2);
    
    char** keys = intmap_keys(map);
    int* values = intmap_values(map);

    assert(keys && values);
    assert(intmap_size(map) == 2);

    int found[2] = {0};
    for (int i = 0; i < 2; i++) {
        if (strcmp(keys[i], "one") == 0 && values[i] == 1) found[0] = 1;
        if (strcmp(keys[i], "two") == 0 && values[i] == 2) found[1] = 1;
    }
    assert(found[0] && found[1]);
}

void test_resize() {
    IntMap map = intmap_new();

    for (int i = 0; i < 1000; i++) {
        char key[16];
        sprintf(key, "key%d", i);
        assert(intmap_insert(map, key, i));
    }

    for (int i = 0; i < 1000; i++) {
        char key[16];
        int value;
        sprintf(key, "key%d", i);
        assert(intmap_get(map, key, &value));
        assert(value == i);
    }

    assert(intmap_size(map) == 1000);
}

int main() {
    test_new();
    test_insert();
    test_set();
    test_remove();
    test_clear();
    test_equals();
    test_keys_and_values();
    test_resize();

    printf("All tests passed succesfully.");
    return 0;
}