#include "test.h"
#include <stdio.h>
#include "linkedlist/intlist.h"

TEST(new) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);

    int value;
    ASSERT_FALSE(intlist_front(list, &value) && intlist_back(list, &value));
}

TEST(push_front) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);
    
    ASSERT_TRUE(intlist_push_front(list, 10));
    ASSERT_FALSE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 1);
    
    int head_value, tail_value;
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 10);
    
    ASSERT_TRUE(intlist_push_front(list, 76));
    ASSERT_EQUAL(intlist_size(list), 2);

    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_NOT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 76);
    ASSERT_EQUAL(tail_value, 10);
    
    ASSERT_FALSE(intlist_push_front(NULL, 23));
}

TEST(push) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);
    
    ASSERT_TRUE(intlist_push(list, 34));
    ASSERT_FALSE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 1);
    
    int head_value, tail_value;
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 34);
    
    ASSERT_TRUE(intlist_push(list, 1023));
    ASSERT_EQUAL(intlist_size(list), 2);
    
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_NOT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(tail_value, 1023);
    ASSERT_EQUAL(head_value, 34);
    
    ASSERT_FALSE(intlist_push(NULL, 34));
}

TEST(push_at) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    
    
    // Inserting into invalid position should not be allowed
    ASSERT_FALSE(intlist_push_at(list, 28, 2));
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    

    // Check if head and tail pointers are correctly updated
    int head_value, tail_value, value;
    ASSERT_TRUE(intlist_push_at(list, 28, 0));
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 28);
    
    ASSERT_FALSE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 1);
    
    // Inserting at the end (index 1 in this case)
    ASSERT_TRUE(intlist_push_at(list, 28, 1));
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 28);
    
    ASSERT_EQUAL(intlist_size(list), 2);
    
    ASSERT_TRUE(intlist_push_at(list, 89, 0));
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_NOT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 89);
    
    ASSERT_EQUAL(intlist_size(list), 3);
    
    // Inserting in the middle
    ASSERT_TRUE(intlist_push_at(list, 55, 1));
    ASSERT_TRUE(intlist_get_at(list, 1, &value));
    ASSERT_EQUAL(value, 55);
    
    // Checking if the whole list order match the expected
    int expected[] = {89, 55, 28, 28};
    for (size_t i = 0; i < 4; i++) {
        ASSERT_TRUE(intlist_get_at(list, i, &value));
        ASSERT_EQUAL(value, expected[i]);
    }
    
    ASSERT_FALSE(intlist_push_at(NULL, 10, 1));
}

TEST(pop_front) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    
    
    // Trying to remove from an empty list should make no changes
    intlist_pop_front(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    
    
    ASSERT_TRUE(intlist_push_front(list, 456));
    ASSERT_TRUE(intlist_push_front(list, 4892));
    ASSERT_TRUE(intlist_push_front(list, 8090));
    ASSERT_EQUAL(intlist_size(list), 3);
    
    int head_value, tail_value;
    // Check if head and tail are correctly set
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(head_value, 8090);
    ASSERT_EQUAL(tail_value, 456);
    
    // Remove front (8090), list becomes: [4892, 456]
    intlist_pop_front(list);
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(intlist_size(list), 2);
    ASSERT_NOT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 4892);
    ASSERT_EQUAL(tail_value, 456);
    
    // Remove front (4892), list becomes: [456]
    intlist_pop_front(list);
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(intlist_size(list), 1);
    ASSERT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 456);
    
    // Remove last element, list becomes empty
    intlist_pop_front(list);
    ASSERT_FALSE(intlist_front(list, &head_value));
    ASSERT_FALSE(intlist_back(list, &tail_value));
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);
}

TEST(pop) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    
    
    // Trying to remove from an empty list should make no changes
    intlist_pop(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    
    
    ASSERT_TRUE(intlist_push_front(list, 456));
    ASSERT_TRUE(intlist_push_front(list, 4892));
    ASSERT_TRUE(intlist_push_front(list, 8090));
    ASSERT_EQUAL(intlist_size(list), 3);
    
    int head_value, tail_value;
    // Check if head and tail are correctly set
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(head_value, 8090); 
    ASSERT_EQUAL(tail_value, 456);
    
    // Remove back/tail (456), list becomes: [8090, 4892]
    intlist_pop(list);
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(intlist_size(list), 2);
    ASSERT_NOT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 8090);
    ASSERT_EQUAL(tail_value, 4892);
    
    // Remove back/tail (4892), list becomes: [8090]
    intlist_pop(list);
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(intlist_size(list), 1);
    ASSERT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(tail_value, 8090);
    
    // Remove last element, list becomes empty
    intlist_pop(list);
    ASSERT_FALSE(intlist_front(list, &head_value));
    ASSERT_FALSE(intlist_back(list, &tail_value));
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);
}

TEST(pop_at) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    
    
    // Trying to remove from an empty list should make no changes
    intlist_pop(list);
    ASSERT_TRUE(intlist_is_empty(list));
    ASSERT_EQUAL(intlist_size(list), 0);    
    
    ASSERT_TRUE(intlist_push_front(list, 456));
    ASSERT_TRUE(intlist_push_front(list, 4892));
    ASSERT_TRUE(intlist_push_front(list, 8090));
    ASSERT_TRUE(intlist_size(list) == 3);
    
    int head_value, tail_value;
    // Remove front (8090), list becomes: [4892, 456]
    intlist_pop_at(list, 0);
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_NOT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 4892);
    ASSERT_EQUAL(tail_value, 456);
    ASSERT_EQUAL(intlist_size(list), 2);
    
    // Remove back/tail (456), list becomes: [4892]
    intlist_pop_at(list, 1);
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(tail_value, 4892);
    ASSERT_EQUAL(intlist_size(list), 1);
    
    ASSERT_TRUE(intlist_push_front(list, 520));
    ASSERT_TRUE(intlist_push(list, 1));
    
    // Remove from the middle (4892), list becomes: [520, 1]
    intlist_pop_at(list, 1);
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_TRUE(intlist_back(list, &tail_value));
    ASSERT_NOT_EQUAL(head_value, tail_value);
    ASSERT_EQUAL(head_value, 520);
    ASSERT_EQUAL(tail_value, 1);
    ASSERT_EQUAL(intlist_size(list), 2);
}

TEST(front) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));

    int front_value = 0;
    // Peeking front from an empty list should fail and not change the output variable
    ASSERT_FALSE(intlist_front(list, &front_value));
    ASSERT_EQUAL(front_value, 0);
    
    ASSERT_TRUE(intlist_push_front(list, 10));
    ASSERT_TRUE(intlist_front(list, &front_value));
    ASSERT_EQUAL(front_value, 10);
    
    ASSERT_TRUE(intlist_push_front(list, 20));
    ASSERT_TRUE(intlist_front(list, &front_value));
    ASSERT_EQUAL(front_value, 20);

    intlist_pop_front(list);
    ASSERT_TRUE(intlist_front(list, &front_value));
    ASSERT_EQUAL(front_value, 10);
    
    front_value = 9999;
    ASSERT_FALSE(intlist_front(list, NULL));
    ASSERT_EQUAL(front_value, 9999);
    ASSERT_FALSE(intlist_front(NULL, &front_value));
    ASSERT_EQUAL(front_value, 9999);
    ASSERT_FALSE(intlist_front(NULL, NULL));
    ASSERT_EQUAL(front_value, 9999);
}

TEST(get_at) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));

    int value;
    ASSERT_FALSE(intlist_get_at(list, 0, &value));
    
    ASSERT_TRUE(intlist_push(list, 10));
    ASSERT_TRUE(intlist_get_at(list, 0, &value));
    ASSERT_EQUAL(value, 10);
    
    ASSERT_TRUE(intlist_push(list, 61));
    ASSERT_TRUE(intlist_get_at(list, 1, &value));
    ASSERT_EQUAL(value, 61);
    
    value = 9999;
    intlist_pop(list);
    ASSERT_FALSE(intlist_get_at(list, 1, &value));
    ASSERT_EQUAL(value, 9999);
    
    ASSERT_FALSE(intlist_get_at(list, 0, NULL));
    ASSERT_EQUAL(value, 9999);
    ASSERT_FALSE(intlist_get_at(NULL, 10, &value));
    ASSERT_EQUAL(value, 9999);
    ASSERT_FALSE(intlist_get_at(NULL, 10, NULL));
    ASSERT_EQUAL(value, 9999);
}

TEST(back) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));

    int back_value = 0;
    // Peeking front from an empty list should fail and not change the output variable
    ASSERT_FALSE(intlist_back(list, &back_value));
    ASSERT_EQUAL(back_value, 0);
    
    ASSERT_TRUE(intlist_push(list, 10));
    ASSERT_TRUE(intlist_back(list, &back_value));
    ASSERT_EQUAL(back_value, 10);
    
    ASSERT_TRUE(intlist_push(list, 20));
    ASSERT_TRUE(intlist_back(list, &back_value));
    ASSERT_EQUAL(back_value, 20);

    intlist_pop(list);
    ASSERT_TRUE(intlist_back(list, &back_value));
    ASSERT_EQUAL(back_value, 10);
    
    back_value = 9999;
    ASSERT_FALSE(intlist_back(list, NULL));
    ASSERT_EQUAL(back_value, 9999);
    ASSERT_FALSE(intlist_back(NULL, &back_value));
    ASSERT_EQUAL(back_value, 9999);
    ASSERT_FALSE(intlist_back(NULL, NULL));
    ASSERT_EQUAL(back_value, 9999);
}

TEST(index) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    // Searching in an empty list should return -1 (not found)
    ASSERT_TRUE(intlist_index(list, 20) == -1);

    ASSERT_TRUE(intlist_push(list, 40));
    ASSERT_TRUE(intlist_push(list, 49010));
    ASSERT_TRUE(intlist_push(list, 99201));

    // Verifying that each value maps to the correct index
    int values_index[][2] = {{40, 0}, {49010, 1}, {99201, 2}};
    for (int i = 0; i < 3; i++) ASSERT_TRUE(intlist_index(list, values_index[i][0]) == values_index[i][1]);

    ASSERT_EQUAL(intlist_index(list, 1234), -1);
    ASSERT_EQUAL(intlist_index(NULL, 10),  -1);
}

TEST(count) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    ASSERT_EQUAL(intlist_count(list, 10), 0);

    for (int i = 0; i < 100; i++) {
        ASSERT_TRUE(intlist_push(list, 10));
        ASSERT_EQUAL(intlist_count(list, 10), i + 1);  
    }

    for (int i = 0; i < 50; i++) {
        intlist_pop(list);
        ASSERT_EQUAL(intlist_count(list, 10), 100 - (i + 1));  
    }

    intlist_clear(list);

    ASSERT_TRUE(intlist_push(list, 5));
    ASSERT_TRUE(intlist_push(list, 10));
    ASSERT_TRUE(intlist_push(list, 5));
    ASSERT_EQUAL(intlist_count(list, 5), 2);
    ASSERT_EQUAL(intlist_count(list, 10), 1);
    ASSERT_EQUAL(intlist_count(list, 999), 0);

    ASSERT_EQUAL(intlist_count(NULL, 10), 0);  
}

TEST(contains) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    ASSERT_FALSE(intlist_contains(list, 10));
    
    ASSERT_TRUE(intlist_push_front(list, 10));
    ASSERT_TRUE(intlist_contains(list, 10));
    
    ASSERT_TRUE(intlist_push_front(list, 20));
    ASSERT_TRUE(intlist_contains(list, 10));
    
    intlist_pop(list);
    ASSERT_FALSE(intlist_contains(list, 10));

    ASSERT_FALSE(intlist_contains(NULL, 290));
}

TEST(equals) {
    IntList l1 = intlist_new();
    IntList l2 = intlist_new();
    ASSERT_NOT_NULL(l1 && l2);

    ASSERT_TRUE(intlist_is_empty(l1) && intlist_is_empty(l2));
    ASSERT_TRUE(intlist_equals(l1, l2));

    ASSERT_TRUE(intlist_push(l1, 10) && intlist_push(l2, 10));
    ASSERT_TRUE(intlist_equals(l1, l2));
    
    intlist_pop(l1);
    ASSERT_FALSE(intlist_equals(l1, l2));

    ASSERT_TRUE(intlist_push_front(l1, 10) && intlist_push(l1, 30));
    ASSERT_TRUE(intlist_push(l2, 20) && intlist_push(l2, 30));
    ASSERT_TRUE(intlist_push(l1, 20));
    ASSERT_FALSE(intlist_equals(l1, l2));

    intlist_clear(l1);
    intlist_clear(l2);
    
    // Same size but with different values
    ASSERT_TRUE(intlist_push(l1, 1));
    ASSERT_TRUE(intlist_push(l1, 2));
    ASSERT_TRUE(intlist_push(l2, 1));
    ASSERT_TRUE(intlist_push(l2, 3));
    ASSERT_FALSE(intlist_equals(l1, l2));

    // Compare with NULL list should return false
    ASSERT_FALSE(intlist_equals(l1, NULL));
    ASSERT_FALSE(intlist_equals(NULL, l2));
    ASSERT_FALSE(intlist_equals(NULL, NULL));
}

TEST(copy) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    // Trying to copy an empty list should return a new one (return new empty list)
    IntList empty_copy = intlist_copy(list);
    ASSERT_NOT_NULL(empty_copy);
    ASSERT_TRUE(intlist_is_empty(empty_copy));
    
    ASSERT_TRUE(intlist_push(list, 10));

    IntList copy = intlist_copy(list);
    ASSERT_NOT_NULL(copy);
    
    int front_value;
    ASSERT_TRUE(intlist_front(copy, &front_value));
    ASSERT_EQUAL(front_value, 10);
    ASSERT_EQUAL(intlist_size(list), 1);
    
    // Modifying original should not affect copy
    ASSERT_TRUE(intlist_push(list, 20));
    ASSERT_TRUE(intlist_contains(list, 20));
    ASSERT_FALSE(intlist_contains(copy, 20));
    
    // Trying to copy a NULL list should fail (return NULL back)
    ASSERT_NULL(intlist_copy(NULL));
}

TEST(map) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    
    int square(int num) { return num * num; };

    // Trying to map an empty list should return a new one (return new empty list)
    IntList empty_copy = intlist_map(list, square);
    ASSERT_NOT_NULL(empty_copy);
    ASSERT_TRUE(intlist_is_empty(empty_copy));

    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(intlist_push(list, i + 1));
    }
    
    IntList squared = intlist_map(list, square);
    ASSERT_NOT_NULL(squared);
    ASSERT_EQUAL(intlist_size(squared), intlist_size(list));

    int value, square_value;
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(intlist_get_at(list, i, &value));
        ASSERT_TRUE(intlist_get_at(squared, i, &square_value));
        ASSERT_EQUAL(square(value), square_value);
    }

    // Should return NULL if the given list is NULL, otherwise return something else;
    ASSERT_NOT_NULL(intlist_map(list, NULL));
    ASSERT_NULL(intlist_map(NULL, NULL));
    ASSERT_NULL(intlist_map(NULL, square));
}

TEST(filter) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    bool is_zero(int num) { return num == 0; };

    // Filtering an empty list should always return a non NULL value
    IntList empty_copy = intlist_filter(list, is_zero);
    ASSERT_NOT_NULL(empty_copy);
    ASSERT_TRUE(intlist_is_empty(empty_copy));
    
    empty_copy = intlist_filter(list, NULL);
    ASSERT_NOT_NULL(empty_copy);
    ASSERT_TRUE(intlist_is_empty(empty_copy));


    for (int i = 0; i < 50; i++) ASSERT_TRUE(intlist_push(list, 0));

    for (int i = 0; i < 50; i++) ASSERT_TRUE(intlist_push(list, i + 1));

    IntList filtered = intlist_filter(list, is_zero);
    ASSERT_NOT_NULL(filtered);
    ASSERT_EQUAL(intlist_size(filtered), 50);
    ASSERT_TRUE(intlist_all(filtered, is_zero));

    // Trying to apply a filter to a NULL list should return NULL
    ASSERT_NULL(intlist_filter(NULL, is_zero));
    ASSERT_NULL(intlist_filter(NULL, NULL));
}

TEST(zip) {
    IntList list1 = intlist_new();
    IntList list2 = intlist_new();
    ASSERT_NOT_NULL(list1 && list2);
    ASSERT_TRUE(intlist_is_empty(list1) && intlist_is_empty(list2));

    ASSERT_TRUE(intlist_push(list1, 10));
    IntList empty_zip = intlist_zip(list1, list2);
    ASSERT_NOT_NULL(empty_zip);
    ASSERT_TRUE(intlist_is_empty(empty_zip));

    empty_zip = intlist_zip(list2, list1);
    ASSERT_NOT_NULL(empty_zip);
    ASSERT_TRUE(intlist_is_empty(empty_zip));

    int head_value, tail_value;

    ASSERT_TRUE(intlist_push(list2, 100));
    IntList zipped = intlist_zip(list1, list2);
    ASSERT_NOT_NULL(zipped);
    ASSERT_FALSE(intlist_is_empty(zipped));

    ASSERT_TRUE(intlist_front(zipped, &head_value));
    ASSERT_TRUE(intlist_back(zipped, &tail_value));
    ASSERT_EQUAL(head_value, 10);
    ASSERT_EQUAL(tail_value, 100);
    
    zipped = intlist_zip(list2, list1);
    ASSERT_NOT_NULL(zipped);
    ASSERT_FALSE(intlist_is_empty(zipped));
    
    ASSERT_TRUE(intlist_front(zipped, &head_value));
    ASSERT_TRUE(intlist_back(zipped, &tail_value));
    ASSERT_EQUAL(head_value, 100);
    ASSERT_EQUAL(tail_value, 10);

    // Zipping with a NULL list should always return NULL
    ASSERT_NULL(intlist_zip(list1, NULL));
    ASSERT_NULL(intlist_zip(NULL, list2));
}

TEST(to_array) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    ASSERT_NULL(intlist_to_array(list));

    for (int i = 0; i < 100; i++) ASSERT_TRUE(intlist_push(list, i + 1));
    
    int* array = intlist_to_array(list);
    ASSERT_NOT_NULL(array);
    
    int value;
    for (int i = 0; i < intlist_size(list); i++) {
        ASSERT_TRUE(intlist_get_at(list, i, &value));
        ASSERT_EQUAL(value, array[i]);
    }
    
    ASSERT_NULL(intlist_to_array(NULL));
}

TEST(all) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    bool greater_than_120(int num) { return num > 120; };

    // An empty list satisfies every predicate
    ASSERT_TRUE(intlist_all(list, greater_than_120));
    ASSERT_TRUE(intlist_all(list, NULL));

    for (int i = 120; i < 240; i++) {
        ASSERT_TRUE(intlist_push(list, i + 1));
    }
    
    ASSERT_TRUE(intlist_all(list, greater_than_120));
    
    ASSERT_TRUE(intlist_push_front(list, 0));
    ASSERT_FALSE(intlist_all(list, greater_than_120));
    
    // Should return false only if given NULL list, otherwise return true
    ASSERT_TRUE(intlist_all(list, NULL));
    ASSERT_FALSE(intlist_all(NULL, greater_than_120));
    ASSERT_FALSE(intlist_all(NULL, NULL));
}

TEST(reverse) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    ASSERT_TRUE(intlist_is_empty(list));
    
    // Trying to reverse an empty list should fail silently
    intlist_reverse(list);
    ASSERT_TRUE(intlist_is_empty(list));
    
    ASSERT_TRUE(intlist_push(list, 10));
    ASSERT_EQUAL(intlist_size(list), 1);
    
    // Same to a list with size less than 2
    intlist_reverse(list);
    ASSERT_EQUAL(intlist_size(list), 1);
    
    ASSERT_TRUE(intlist_push(list, 1019));
    ASSERT_EQUAL(intlist_size(list), 2);
    
    intlist_reverse(list);
    ASSERT_EQUAL(intlist_size(list), 2);

    int head_value;
    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_EQUAL(head_value, 1019);

    intlist_pop_front(list);

    ASSERT_TRUE(intlist_front(list, &head_value));
    ASSERT_EQUAL(head_value, 10);

    int arr[100];
    for (int i = 0; i < 100; i++) arr[i] = i + 1;
    
    IntList l = intlist_from_array(arr, 100);
    ASSERT_NOT_NULL(l);
    
    intlist_reverse(l);
    for (int i = 99; i >= 0; i--) {
        ASSERT_TRUE(intlist_front(l, &head_value));
        ASSERT_EQUAL(arr[i], head_value);
        intlist_pop_front(l);
    }
}

TEST(any) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);
    
    bool greater_than_120(int num) { return num > 120; };
    
    // An empty list did not satisfy any predicate
    ASSERT_FALSE(intlist_any(list, greater_than_120));
    ASSERT_FALSE(intlist_any(list, NULL));
    
    for (int i = 0; i < 120; i++) {
        ASSERT_TRUE(intlist_push(list, i + 1));
    }
    
    ASSERT_FALSE(intlist_any(list, greater_than_120));

    ASSERT_TRUE(intlist_push(list, 121));
    ASSERT_TRUE(intlist_any(list, greater_than_120));
    
    ASSERT_FALSE(intlist_any(NULL, greater_than_120));
    ASSERT_FALSE(intlist_any(NULL, NULL));
}

TEST(sum) {
    IntList list = intlist_new();
    ASSERT_NOT_NULL(list);

    ASSERT_EQUAL(intlist_sum(list), 0);

    for (int i = 0; i < 1000000; i++) {
        ASSERT_TRUE(intlist_push(list, i + 1));
    }

    long long expected_sum = (1ll + 1000000ll) * 1000000ll / 2;
    ASSERT_EQUAL(intlist_sum(list), expected_sum);

    ASSERT_EQUAL(intlist_sum(NULL), 0);
}

int main() {
    TestCase tests[] = {
        {"new", test_new},
        {"push front", test_push_front},
        {"push", test_push},
        {"push_at", test_push_at},
        {"pop_front", test_pop_front},
        {"pop", test_pop},
        {"pop_at", test_pop_at},
        {"front", test_front},
        {"get_at", test_get_at},
        {"back", test_back},
        {"index", test_index},
        {"count", test_count},
        {"contains", test_contains},
        {"equals", test_equals},
        {"copy", test_copy},
        {"map", test_map},
        {"filter", test_filter},
        {"zip", test_zip},
        {"to_array", test_to_array},
        {"reverse", test_reverse},
        {"all", test_all},
        {"any", test_any},
        {"sum", test_sum},
    };

    TestSuite suite = {.name = "IntList", .tests = tests, .tests_num = sizeof (tests) / sizeof(tests[0])};

    run_suite_tests(&suite);
    return 0;
}