#include "test.h"
#include <stdio.h>
#include "stack/charstack.h"

TEST(new) {
    CharStack stack = charstack_new();
    ASSERT_NOT_NULL(stack);

    ASSERT_TRUE(charstack_is_empty(stack));
    ASSERT_EQUAL(charstack_size(stack), 0);
}

TEST(is_empty) {
    CharStack stack = charstack_new();
    ASSERT_NOT_NULL(stack);
    
    ASSERT_TRUE(charstack_is_empty(stack));
    
    ASSERT_TRUE(charstack_push(stack, 10));
    ASSERT_FALSE(charstack_is_empty(stack));
    
    ASSERT_TRUE(charstack_pop(stack, NULL));
    ASSERT_TRUE(charstack_is_empty(stack));
    
    ASSERT_TRUE(charstack_is_empty(NULL));
}

TEST(clear) {
    CharStack stack = charstack_new();
    ASSERT_NOT_NULL(stack);

    // Should be possible to reuse the stack after clear
    for (int i = 0; i < 2; i++) {
        for (char j = 'A'; j <= 'Z'; j++) {
            ASSERT_TRUE(charstack_push(stack, j + 1));
        }
        ASSERT_EQUAL(charstack_size(stack), 26);
        ASSERT_FALSE(charstack_is_empty(stack));

        charstack_clear(stack);
        ASSERT_TRUE(charstack_is_empty(stack));
        ASSERT_EQUAL(charstack_size(stack), 0);    
    }
}

TEST(push) {
    CharStack stack = charstack_new();
    ASSERT_NOT_NULL(stack);

    ASSERT_TRUE(charstack_is_empty(stack));
    ASSERT_EQUAL(charstack_size(stack), 0);

    ASSERT_TRUE(charstack_push(stack, 'H'));
    ASSERT_EQUAL(charstack_size(stack), 1);

    char top_value;
    ASSERT_TRUE(charstack_peek(stack, &top_value));
    ASSERT_EQUAL(top_value, 'H');

    ASSERT_TRUE(charstack_push(stack, 'J'));
    ASSERT_EQUAL(charstack_size(stack), 2);
    ASSERT_TRUE(charstack_peek(stack, &top_value));
    ASSERT_EQUAL(top_value, 'J');

    ASSERT_FALSE(charstack_push(NULL, 'Y'));
}

TEST(pop) {
    CharStack stack = charstack_new();
    ASSERT_NOT_NULL(stack);
    
    ASSERT_TRUE(charstack_is_empty(stack));
    ASSERT_EQUAL(charstack_size(stack), 0);
    
    char top_value = 0;
    ASSERT_FALSE(charstack_pop(stack, NULL));
    ASSERT_FALSE(charstack_pop(stack, &top_value));
    ASSERT_EQUAL(top_value, 0);
    ASSERT_TRUE(charstack_is_empty(stack));
    ASSERT_EQUAL(charstack_size(stack), 0);
    
    // Removing the only element
    ASSERT_TRUE(charstack_push(stack, 'A'));
    ASSERT_EQUAL(charstack_size(stack), 1);
    ASSERT_TRUE(charstack_pop(stack, NULL));
    ASSERT_TRUE(charstack_is_empty(stack));
    ASSERT_EQUAL(charstack_size(stack), 0);
    
    ASSERT_TRUE(charstack_push(stack, 'B'));
    ASSERT_TRUE(charstack_push(stack, 'C'));
    ASSERT_TRUE(charstack_push(stack, 'D'));
    ASSERT_EQUAL(charstack_size(stack), 3);

    ASSERT_TRUE(charstack_pop(stack, &top_value));
    ASSERT_EQUAL(charstack_size(stack), 2);
    ASSERT_EQUAL(top_value, 'D');
    
    ASSERT_TRUE(charstack_pop(stack, &top_value));
    ASSERT_EQUAL(charstack_size(stack), 1);
    ASSERT_EQUAL(top_value, 'C');
    
    ASSERT_TRUE(charstack_pop(stack, &top_value));
    ASSERT_TRUE(charstack_is_empty(stack));
    ASSERT_EQUAL(charstack_size(stack), 0);
    ASSERT_EQUAL(top_value, 'B');
    
    ASSERT_FALSE(charstack_pop(NULL, &top_value));
    ASSERT_FALSE(charstack_pop(NULL, NULL));
}

TEST(peek) {
    CharStack stack = charstack_new();
    ASSERT_NOT_NULL(stack);

    char top_value;
    ASSERT_FALSE(charstack_peek(stack, &top_value));

    ASSERT_TRUE(charstack_push(stack, 'M'));
    ASSERT_TRUE(charstack_push(stack, 'K'));
    ASSERT_TRUE(charstack_push(stack, 'L'));
    ASSERT_EQUAL(charstack_size(stack), 3);

    ASSERT_TRUE(charstack_peek(stack, &top_value));
    ASSERT_EQUAL(charstack_size(stack), 3);
    ASSERT_EQUAL(top_value, 'L');

    ASSERT_TRUE(charstack_pop(stack, &top_value));
    ASSERT_EQUAL(charstack_size(stack), 2);

    ASSERT_TRUE(charstack_peek(stack, &top_value));
    ASSERT_EQUAL(charstack_size(stack), 2);
    ASSERT_EQUAL(top_value, 'K');
    
    ASSERT_TRUE(charstack_pop(stack, &top_value));
    ASSERT_EQUAL(charstack_size(stack), 1);
    
    ASSERT_FALSE(charstack_peek(stack, NULL));
    ASSERT_TRUE(charstack_peek(stack, &top_value));
    ASSERT_EQUAL(charstack_size(stack), 1);
    ASSERT_EQUAL(top_value, 'M');
    
    ASSERT_FALSE(charstack_peek(NULL, &top_value));
    ASSERT_FALSE(charstack_peek(NULL, NULL));
}

TEST(size) {
    CharStack stack = charstack_new();
    ASSERT_NOT_NULL(stack);

    ASSERT_EQUAL(charstack_size(stack), 0);

    for (int i = 0; i < 100; i++) {
        ASSERT_TRUE(charstack_push(stack, i + 1));
        ASSERT_EQUAL(charstack_size(stack), i + 1);
    }
    
    for (int i = 0; i < 50; i++) {
        ASSERT_TRUE(charstack_pop(stack, NULL));
        ASSERT_EQUAL(charstack_size(stack), 100 - (i + 1));
    }

    ASSERT_EQUAL(charstack_size(NULL), 0);
}

int main() {
    TestCase tests[] = {
        {"new", test_new},
        {"is_empty", test_is_empty},
        {"clear", test_clear},
        {"push", test_push},
        {"pop", test_pop},
        {"peek", test_peek},
        {"size", test_size},
    };

    TestSuite suite = {.name = "CharStack", .tests = tests, .tests_num = sizeof (tests) / sizeof (tests[0])};

    run_suite_tests(&suite);
    return 0;
}