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

    // Insertion with a NULL key should fail silently and not alter the map
    assert(!intmap_insert(map, NULL, 108) && !intmap_has_key(map, NULL) );
    assert(intmap_size(map) == 2);
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

    assert(intmap_insert(map, "1", 123));
    assert(intmap_insert(map, "2", 1980));
    assert(intmap_size(map) == 2);

    intmap_remove(map, "2");
    assert(intmap_size(map) == 1);
    assert(!intmap_has_key(map, "2"));

    int value;
    assert(intmap_has_key(map, "1"));
    assert(intmap_get(map, "1", &value) && value == 123);

    intmap_remove(map, "not exists");
    assert(intmap_size(map) == 1);
}

void test_clear() {
    IntMap map = intmap_new();
    assert(map);
    
    assert(intmap_insert(map, "a", 1));
    assert(intmap_insert(map, "b", 2));
    assert(!intmap_is_empty(map) && intmap_size(map) > 0);
    assert(intmap_has_key(map, "a") && intmap_has_key(map, "b"));
    
    intmap_clear(map);
    assert(intmap_is_empty(map) && intmap_size(map) == 0);
    assert(!intmap_has_key(map, "a") && !intmap_has_key(map, "b"));
    
    assert(intmap_insert(map, "a", 234));
    assert(intmap_size(map) == 1);
    
    int value;
    assert(intmap_get(map, "a", &value) && value == 234);
}

void test_equals() {
    IntMap m1 = intmap_new();
    IntMap m2 = intmap_new();
    assert(m1 && m2);
    
    assert(intmap_is_empty(m1) && intmap_is_empty(m2));
    assert(intmap_equals(m1, m2));
    
    intmap_insert(m1, "x", 10);
    intmap_insert(m2, "x", 10);
    assert(intmap_equals(m1, m2));
    
    assert(intmap_set(m2, "x", 20));
    assert(!intmap_equals(m1, m2));
    
    assert(intmap_set(m1, "x", 20));
    assert(intmap_equals(m1, m2));
    
    // Inserting the same key-value pairs in different order should still result in equal maps
    assert(intmap_insert(m1, "z", 90) && intmap_insert(m2, "m", 70));
    assert(intmap_insert(m1, "m", 70) && intmap_insert(m2, "z", 90));
    assert(intmap_equals(m1, m2));
    
    assert(intmap_insert(m1, "new key", 30));
    assert(!intmap_equals(m1, m2));
    
    assert(!intmap_equals(NULL, NULL));
    assert(!intmap_equals(m1, NULL));
    assert(!intmap_equals(NULL, m2));
}

void test_keys_and_values() {
    IntMap map = intmap_new();
    assert(map);
    
    char** keys = intmap_keys(map);
    int* values = intmap_values(map);
    assert(!keys && !values);
    
    intmap_insert(map, "one", 1);
    intmap_insert(map, "two", 2);
    
    keys = intmap_keys(map);
    values = intmap_values(map);
    assert(keys && values);
    assert(intmap_size(map) == 2);
    
    // Check if the key-value pairs match
    int found[2] = {0};
    for (int i = 0; i < 2; i++) {
        if (strcmp(keys[i], "one") == 0 && values[i] == 1) found[0] = 1;
        if (strcmp(keys[i], "two") == 0 && values[i] == 2) found[1] = 1;
    }
    assert(found[0] && found[1]);
    
    keys = intmap_keys(NULL);
    values = intmap_values(NULL);
    assert(!keys && !values);
}

void test_resize() {
    IntMap map = intmap_new();
    assert(map);

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