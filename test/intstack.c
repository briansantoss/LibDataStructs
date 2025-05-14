#include "test.h"
#include <stdio.h>
#include "stack/intstack.h"

TEST(new) {
    IntStack stack = intstack_new();
    ASSERT_TRUE(stack);

    ASSERT_TRUE(intstack_is_empty(stack));
    ASSERT_TRUE(intstack_size(stack) == 0);
}

TEST(push) {
    IntStack stack = intstack_new();
    ASSERT_TRUE(stack);

    ASSERT_TRUE(intstack_is_empty(stack));
    ASSERT_TRUE(intstack_size(stack) == 0);

    ASSERT_TRUE(intstack_push(stack, 45));
    ASSERT_TRUE(intstack_size(stack) == 1);
    int top_value;
    ASSERT_TRUE(intstack_peek(stack, &top_value));
    ASSERT_TRUE(top_value == 45);

    ASSERT_TRUE(intstack_push(stack, 54));
    ASSERT_TRUE(intstack_size(stack) == 2);
    ASSERT_TRUE(intstack_peek(stack, &top_value));
    ASSERT_TRUE(top_value == 54);

    ASSERT_FALSE(intstack_push(NULL, 20));
}

TEST(pop) {
    IntStack stack = intstack_new();
    ASSERT_TRUE(stack);
    
    ASSERT_TRUE(intstack_is_empty(stack));
    ASSERT_TRUE(intstack_size(stack) == 0);
    
    int top_value = 0;
    ASSERT_FALSE(intstack_pop(stack, NULL));
    ASSERT_FALSE(intstack_pop(stack, &top_value));
    ASSERT_TRUE(top_value == 0);
    ASSERT_TRUE(intstack_is_empty(stack));
    ASSERT_TRUE(intstack_size(stack) == 0);
    
    // Removing the only element
    ASSERT_TRUE(intstack_push(stack, 50));
    ASSERT_TRUE(intstack_size(stack) == 1);
    ASSERT_TRUE(intstack_pop(stack, NULL));
    ASSERT_TRUE(intstack_is_empty(stack));
    ASSERT_TRUE(intstack_size(stack) == 0);
    
    ASSERT_TRUE(intstack_push(stack, 60));
    ASSERT_TRUE(intstack_push(stack, 70));
    ASSERT_TRUE(intstack_push(stack, 80));
    ASSERT_TRUE(intstack_size(stack) == 3);

    ASSERT_TRUE(intstack_pop(stack, &top_value))
    ASSERT_TRUE(intstack_size(stack) == 2);
    ASSERT_TRUE(top_value == 80);
    
    ASSERT_TRUE(intstack_pop(stack, &top_value));
    ASSERT_TRUE(intstack_size(stack) == 1);
    ASSERT_TRUE(top_value == 70);
    
    ASSERT_TRUE(intstack_pop(stack, &top_value));
    ASSERT_TRUE(intstack_is_empty(stack));
    ASSERT_TRUE(intstack_size(stack) == 0);
    ASSERT_TRUE(top_value == 60);
    
    ASSERT_FALSE(intstack_pop(NULL, &top_value));
    ASSERT_FALSE(intstack_pop(NULL, NULL));
}

TEST(peek) {
    IntStack stack = intstack_new();
    ASSERT_TRUE(stack);

    int top_value;
    ASSERT_FALSE(intstack_peek(stack, &top_value));

    ASSERT_TRUE(intstack_push(stack, 89));
    ASSERT_TRUE(intstack_push(stack, 93));
    ASSERT_TRUE(intstack_push(stack, 25));
    ASSERT_TRUE(intstack_size(stack) == 3);

    ASSERT_TRUE(intstack_peek(stack, &top_value));
    ASSERT_TRUE(intstack_size(stack) == 3);
    ASSERT_TRUE(top_value == 25);

    ASSERT_TRUE(intstack_pop(stack, &top_value));
    ASSERT_TRUE(intstack_size(stack) == 2);

    ASSERT_TRUE(intstack_peek(stack, &top_value));
    ASSERT_TRUE(intstack_size(stack) == 2);
    ASSERT_TRUE(top_value == 93);
    
    ASSERT_TRUE(intstack_pop(stack, &top_value));
    ASSERT_TRUE(intstack_size(stack) == 1);
    
    ASSERT_FALSE(intstack_peek(stack, NULL));
    ASSERT_TRUE(intstack_peek(stack, &top_value));
    ASSERT_TRUE(intstack_size(stack) == 1);
    ASSERT_TRUE(top_value == 89);
    
    ASSERT_FALSE(intstack_peek(NULL, &top_value));
    ASSERT_FALSE(intstack_peek(NULL, NULL));
}

int main() {
    TestCase tests[] = {
        {"new", test_new},
        {"push", test_push},
        {"pop", test_pop},
        {"peek", test_peek},
    };

    TestSuite suite = {.name = "IntStack", .tests = tests, .tests_num = sizeof (tests) / sizeof (tests[0])};

    run_suite_tests(&suite);
    return 0;
}