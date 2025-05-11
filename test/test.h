#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stddef.h>

#define GREEN   "\033[1;32m"
#define RED     "\033[1;31m"
#define RESET   "\033[0m"

#define CHAR_N_TIMES(char, times) printf("\n"); for (size_t i = 0; i < times; i++) putchar(char);

static int last_test_status = 1;
static size_t failed_tests = 0;

#define TEST(name) void test_##name(void)

#define ASSERT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            last_test_status = 0; \
            fprintf(stderr, RED "\nAssertion failed (%s:%d): %s\n" RESET, __FILE__, __LINE__ , #cond); \
            return; \
        } \
    } while (0);
    
    #define ASSERT_FALSE(cond)  \
    do { \
        if ((cond)) { \
            last_test_status = 0; \
            fprintf(stderr, RED "\nAssertion failed (%s:%d): %s\n" RESET, __FILE__, __LINE__ , #cond); \
            return; \
        } \
    } while (0);

typedef void (*test_func)(void);

typedef struct {
    const char* name;
    test_func test;
} TestCase;

typedef struct {
    const char* name;
    TestCase* tests;
    size_t tests_num;
} TestSuite;

void run_test(TestCase* test) {
    last_test_status = 1;
    printf("[ RUN ] %s\n", test->name);
    test->test();  // Executa o teste

    if (last_test_status) {
        printf(GREEN "[ PASS ]" RESET "\n");
    } else {
        failed_tests++;
        printf(RED "[ FAILED ]" RESET "\n");
    }
}

void run_suite_tests(TestSuite* suite) {
    CHAR_N_TIMES('=', 80);
    printf("\nRunning %s test suite...\n", suite->name);

    size_t tests_num = suite->tests_num;
    for (size_t i = 0; i < tests_num; i++) {
        run_test(&suite->tests[i]);
    }
    CHAR_N_TIMES('=', 80);
    printf("\nSummary: Ran %zu tests, " GREEN "%zu" RESET " passed, " RED "%zu" RESET " failed.", tests_num,  tests_num - failed_tests, failed_tests);
    CHAR_N_TIMES('=', 80);
    printf("\n");
}

#endif // TEST_H