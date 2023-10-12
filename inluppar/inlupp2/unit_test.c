#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include "webstore.h"

int string_sum_hash(elem_t e)
{
    char *str = e.p;
    int result = 0;
    do
    {
        result += *str;
    } while (*++str != '\0');
    return result % No_Buckets;
}

static bool key_eq(elem_t a, elem_t b)
{
    return strcmp(a.p, b.p) == 0;
}

static bool shelf_eq(elem_t a, elem_t b)
{
    shelf_t *shelf_a = a.p;
    shelf_t *shelf_b = b.p;
    return strcmp(shelf_a->name, shelf_b->name) == 0;
}

int init_suite(void)
{
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void)
{
    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

void test_add_merch()
{

    ioopm_hash_table_t *warehouse_ht = ioopm_hash_table_create(string_sum_hash, shelf_eq, NULL);
    ioopm_hash_table_t *information_ht = ioopm_hash_table_create(string_sum_hash, key_eq, NULL);

    ioopm_add_merch(warehouse_ht, information_ht, "a", "a", 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(information_ht, (elem_t){.p = "a"}).success);

    destroy_all(warehouse_ht, information_ht);
}

void test_test()
{
    for (int i = 0; i < 10; ++i)
    {
        char shelf[4];
        shelf[0] = 'A' + (rand() % 26);
        shelf[1] = '0' + (rand() % 10);
        shelf[2] = '0' + (rand() % 10);
        shelf[3] = '\0';

        printf("%s\n", shelf);
    }
}

int main()
{
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
    if (my_test_suite == NULL)
    {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information

    if (
        (CU_add_test(my_test_suite, "Test add merch", test_add_merch) == NULL) ||
        0)
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
}
