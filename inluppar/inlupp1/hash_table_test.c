#include <CUnit/Basic.h>
#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include "linked_list.h"
#include "iterator.h"
#include "common.h"

static bool key_eq(elem_t a, elem_t b)
{
    return a.i == b.i;
}
static bool value_eq(elem_t a, elem_t b)
{
    return strcmp(a.p, b.p) == 0;
}

static bool value_equiv_any(elem_t key_ignored, elem_t value, elem_t x)
{
    elem_t other_value_ptr;
    other_value_ptr.p = x.p;
    return strcmp(value.p, other_value_ptr.p) == 0;
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

void test_create_destroy()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, (elem_t){.i = 1}).success);
    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "AAA"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "BBB"});
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, (elem_t){.i = 1}).success);
    ioopm_hash_table_destroy(ht);
}

void test_lookup_empty()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);
    elem_t test;
    test.i = 1;
    CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, test).value.p);
    ioopm_hash_table_destroy(ht);
}

void test_remove()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    // Case 1:
    CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, (elem_t){.i = 1}).success);

    // Case 2:
    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "AAA"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 18}, (elem_t){.p = "BBB"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 35}, (elem_t){.p = "CCC"});

    ioopm_hash_table_remove(ht, (elem_t){.i = 18});

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, (elem_t){.i = 1}).success);
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, (elem_t){.i = 18}).success);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, (elem_t){.i = 35}).success);

    // Case 3:
    ioopm_hash_table_remove(ht, (elem_t){.i = 35});
    CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, (elem_t){.i = 35}).success);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, (elem_t){.i = 1}).success);

    ioopm_hash_table_destroy(ht);
}

void test_size_empty_clear()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "AAA"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 18}, (elem_t){.p = "BBB"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 35}, (elem_t){.p = "CCC"});

    CU_ASSERT_EQUAL(3, ioopm_hash_table_size(ht));
    CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));
    ioopm_hash_table_clear(ht);
    CU_ASSERT_EQUAL(0, ioopm_hash_table_size(ht));
    CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

    ioopm_hash_table_destroy(ht);
}

void test_keys()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    int keys[5] = {3, 10, 42, 0, 99};
    bool found[5] = {false};

    ioopm_hash_table_insert(ht, (elem_t){.i = 3}, (elem_t){.p = ""});
    ioopm_hash_table_insert(ht, (elem_t){.i = 10}, (elem_t){.p = ""});
    ioopm_hash_table_insert(ht, (elem_t){.i = 42}, (elem_t){.p = ""});
    ioopm_hash_table_insert(ht, (elem_t){.i = 0}, (elem_t){.p = ""});
    ioopm_hash_table_insert(ht, (elem_t){.i = 99}, (elem_t){.p = ""});

    ioopm_list_t *found_keys = ioopm_hash_table_keys(ht);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (ioopm_linked_list_get(found_keys, i).i == keys[j])
            {
                found[j] = true;
                continue;
            }
            else if (j == 4)
            {
                // CU_FAIL("Found a key that was never inserted");
            }
        }
    }

    ioopm_linked_list_destroy(found_keys);

    for (int i = 0; i < 5; i++)
    {
        CU_ASSERT_TRUE(found[i]);
    }

    ioopm_hash_table_destroy(ht);
}

void test_values()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    int keys[5] = {3, 10, 42, 0, 99};
    char *values[5] = {"three", "ten", "fortytwo", "zero", "ninetynine"};

    ioopm_hash_table_insert(ht, (elem_t){.i = 3}, (elem_t){.p = "three"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 10}, (elem_t){.p = "ten"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 42}, (elem_t){.p = "fortytwo"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 0}, (elem_t){.p = "zero"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 99}, (elem_t){.p = "ninetynine"});

    ioopm_list_t *found_keys = ioopm_hash_table_keys(ht);
    ioopm_list_t *found_values = ioopm_hash_table_values(ht);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (ioopm_linked_list_get(found_keys, i).i == keys[j])
            {
                CU_ASSERT_EQUAL(values[j], ioopm_linked_list_get(found_values, i).p);
            }
        }
    }

    ioopm_linked_list_destroy(found_keys);
    ioopm_linked_list_destroy(found_values);

    ioopm_hash_table_destroy(ht);
}

void test_any_key()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "AAA"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 18}, (elem_t){.p = "BBB"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 35}, (elem_t){.p = "CCC"});

    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, (elem_t){.i = 1}));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, (elem_t){.i = 18}));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, (elem_t){.i = 35}));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, (elem_t){.i = 10}));

    ioopm_hash_table_destroy(ht);
}

void test_any_value()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "AAA"});

    CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, (elem_t){.p = "AAA"}));
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, (elem_t){.p = "BBB"}));

    ioopm_hash_table_destroy(ht);
}

void test_any_function()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "AAA"});

    CU_ASSERT_TRUE(ioopm_hash_table_any(ht, value_equiv_any, (elem_t){.p = "AAA"}));
    CU_ASSERT_FALSE(ioopm_hash_table_any(ht, value_equiv_any, (elem_t){.p = "BBB"}));

    ioopm_hash_table_destroy(ht);
}

static bool not_empty(elem_t key_ignored, elem_t value, elem_t x_ignored)
{

    return strlen(value.p) > 0;
}

void test_all()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.p = "AAA"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 18}, (elem_t){.p = "BBB"});
    ioopm_hash_table_insert(ht, (elem_t){.i = 35}, (elem_t){.p = "CCC"});

    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, not_empty, (elem_t){.p = NULL}));
    ioopm_hash_table_insert(ht, (elem_t){.i = 10}, (elem_t){.p = ""});
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, not_empty, (elem_t){.p = NULL}));

    ioopm_hash_table_destroy(ht);
}

static void make_zero_all(elem_t key_ignored, elem_t *value, elem_t x_ignored)
{
    value->i = 0;
}

void test_apply()
{
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, key_eq, value_eq);

    ioopm_hash_table_insert(ht, (elem_t){.i = 1}, (elem_t){.i = 10});
    ioopm_hash_table_insert(ht, (elem_t){.i = 18}, (elem_t){.i = 105});
    ioopm_hash_table_insert(ht, (elem_t){.i = 35}, (elem_t){.i = 0});

    ioopm_hash_table_apply_to_all(ht, make_zero_all, (elem_t){.p = NULL});
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, (elem_t){.i = 1}).success);
    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, (elem_t){.i = 1}).value.i, 0);

    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, (elem_t){.i = 18}).value.i, 0);

    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, (elem_t){.i = 35}).value.i, 0);

    ioopm_hash_table_insert(ht, (elem_t){.i = 10}, (elem_t){.i = 5});
    CU_ASSERT_NOT_EQUAL(ioopm_hash_table_lookup(ht, (elem_t){.i = 10}).value.i, 0);

    ioopm_hash_table_destroy(ht);
}

static bool equal_int(elem_t value, elem_t extra)
{
    return value.i == extra.i;
}

void test_linked_list()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);

    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(ll));
    CU_ASSERT_FALSE(ioopm_linked_list_contains(ll, (elem_t){.i = 10}));

    // Test prepend and append
    ioopm_linked_list_append(ll, (elem_t){.i = 1});
    ioopm_linked_list_append(ll, (elem_t){.i = 10});
    // (1, 10)
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 1}));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 10}));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(ll), 2);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 0).i, 1);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 1).i, 10);

    // Test insert
    ioopm_linked_list_insert(ll, 1, (elem_t){.i = 5});
    ioopm_linked_list_insert(ll, 2, (elem_t){.i = 15});
    ioopm_linked_list_insert(ll, 0, (elem_t){.i = 0});
    // (0, 1, 5, 15, 10)
    CU_ASSERT_EQUAL(ioopm_linked_list_size(ll), 5);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 2).i, 5);

    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 5}));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 15}));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 0}));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 1).i, 1);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 0).i, 0);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 4).i, 10);

    // Test remove
    ioopm_linked_list_remove(ll, 1);
    // (0, 5, 15, 10)
    CU_ASSERT_FALSE(ioopm_linked_list_contains(ll, (elem_t){.i = 1}));
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get(ll, 1).i, 1);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 0).i, 0);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 1).i, 5);

    ioopm_linked_list_remove(ll, 1);
    // (0, 15, 10)
    // CU_ASSERT_FALSE(ioopm_linked_list_contains(ll, 0));
    CU_ASSERT_NOT_EQUAL(ioopm_linked_list_get(ll, 1).i, 0);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 1).i, 15);

    ioopm_linked_list_remove(ll, 2);
    // (5, 15)
    CU_ASSERT_FALSE(ioopm_linked_list_contains(ll, (elem_t){.i = 10}));

    ioopm_linked_list_remove(ll, 1); // (5)
    ioopm_linked_list_remove(ll, 0);

    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(ll));

    ioopm_linked_list_destroy(ll);
}

void test_ll_insert()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);

    ioopm_linked_list_append(ll, (elem_t){.i = 1});
    ioopm_linked_list_append(ll, (elem_t){.i = 10});

    ioopm_linked_list_insert(ll, 1, (elem_t){.i = 5});
    ioopm_linked_list_insert(ll, 2, (elem_t){.i = 15});
    ioopm_linked_list_insert(ll, 0, (elem_t){.i = 0});

    // (0, 1, 5, 15, 10)
    CU_ASSERT_EQUAL(ioopm_linked_list_size(ll), 5);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 2).i, 5);

    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 5}));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 15}));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll, (elem_t){.i = 0}));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 1).i, 1);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 0).i, 0);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 4).i, 10);

    ioopm_linked_list_insert(ll, 5, (elem_t){.i = 2});

    ioopm_linked_list_remove(ll, 0);

    ioopm_linked_list_destroy(ll);
}

// Tests ioopm_linked_list_all
void test_linked_list_all()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);

    ioopm_linked_list_prepend(ll, (elem_t){.i = 2});
    ioopm_linked_list_prepend(ll, (elem_t){.i = 2});

    CU_ASSERT_TRUE(ioopm_linked_list_all(ll, equal_int, (elem_t){.i = 2}));
    CU_ASSERT_FALSE(ioopm_linked_list_all(ll, equal_int, (elem_t){.i = 5}));

    ioopm_linked_list_destroy(ll);
}

void test_linked_list_any()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);

    ioopm_linked_list_prepend(ll, (elem_t){.i = 2});
    ioopm_linked_list_prepend(ll, (elem_t){.i = 4});

    CU_ASSERT_FALSE(ioopm_linked_list_any(ll, equal_int, (elem_t){.p = NULL}));

    CU_ASSERT_TRUE(ioopm_linked_list_any(ll, equal_int, (elem_t){.i = 2}));

    ioopm_linked_list_destroy(ll);
}

static void make_zero(elem_t *value, elem_t extra)
{
    value->i = 0;
}

void test_linked_list_apply_to_all()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);

    ioopm_linked_list_prepend(ll, (elem_t){.i = 2});
    ioopm_linked_list_prepend(ll, (elem_t){.i = 4});

    ioopm_linked_list_apply_to_all(ll, make_zero, (elem_t){.p = NULL});

    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll, 1).i, 0);

    ioopm_linked_list_destroy(ll);
}

void test_iter_has_next()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);
    ioopm_list_iterator_t *itr = ioopm_list_iterator(ll);

    CU_ASSERT_FALSE(ioopm_iterator_has_next(itr));
    elem_t k1 = {.i = 5};
    ioopm_linked_list_append(ll, k1);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(itr));

    ioopm_iterator_destroy(itr);
    ioopm_linked_list_destroy(ll);
}

void test_iter_next()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);
    ioopm_list_iterator_t *itr = ioopm_list_iterator(ll);

    elem_t k1 = {.i = 5};
    ioopm_linked_list_append(ll, k1);

    CU_ASSERT_EQUAL(ioopm_iterator_next(itr).i, 5);

    ioopm_iterator_destroy(itr);
    ioopm_linked_list_destroy(ll);
}

void test_iter_current()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);
    ioopm_list_iterator_t *itr = ioopm_list_iterator(ll);

    elem_t k1 = {.i = 5};
    ioopm_linked_list_append(ll, k1);
    ioopm_iterator_next(itr);
    CU_ASSERT_EQUAL(ioopm_iterator_current(itr).i, 5);
    elem_t k2 = {.i = 6};
    ioopm_linked_list_append(ll, k2);
    ioopm_iterator_next(itr);
    CU_ASSERT_EQUAL(ioopm_iterator_current(itr).i, 6);

    ioopm_iterator_destroy(itr);
    ioopm_linked_list_destroy(ll);
}

void test_iter_reset()
{
    ioopm_list_t *ll = ioopm_linked_list_create(key_eq);
    ioopm_list_iterator_t *itr = ioopm_list_iterator(ll);

    elem_t k1 = {.i = 5};
    ioopm_linked_list_append(ll, k1);
    ioopm_iterator_next(itr);
    CU_ASSERT_EQUAL(ioopm_iterator_current(itr).i, 5);
    elem_t k2 = {.i = 6};
    ioopm_linked_list_append(ll, k2);
    ioopm_iterator_next(itr);
    CU_ASSERT_EQUAL(ioopm_iterator_current(itr).i, 6);
    ioopm_iterator_reset(itr);
    CU_ASSERT_EQUAL(ioopm_iterator_next(itr).i, 5);

    ioopm_iterator_destroy(itr);
    ioopm_linked_list_destroy(ll);
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
        (CU_add_test(my_test_suite, "Test the create and destroy", test_create_destroy) == NULL) ||
        (CU_add_test(my_test_suite, "Test Lookup empty", test_lookup_empty) == NULL) ||
        (CU_add_test(my_test_suite, "Test Insert once", test_insert_once) == NULL) ||
        (CU_add_test(my_test_suite, "Test remove", test_remove) == NULL) ||
        (CU_add_test(my_test_suite, "Test size empty clear", test_size_empty_clear) == NULL) ||
        (CU_add_test(my_test_suite, "Test keys", test_keys) == NULL) ||
        (CU_add_test(my_test_suite, "Test values", test_values) == NULL) ||
        (CU_add_test(my_test_suite, "Test any key", test_any_key) == NULL) ||
        (CU_add_test(my_test_suite, "Test any value", test_any_value) == NULL) ||
        (CU_add_test(my_test_suite, "Test any function", test_any_function) == NULL) ||
        (CU_add_test(my_test_suite, "Test all", test_all) == NULL) ||
        (CU_add_test(my_test_suite, "Test apply", test_apply) == NULL) || // mem-leaks
        (CU_add_test(my_test_suite, "Test Linked list", test_linked_list) == NULL) ||
        (CU_add_test(my_test_suite, "Test Linked list insert", test_ll_insert) == NULL) ||
        (CU_add_test(my_test_suite, "Test Linked all", test_linked_list_all) == NULL) ||
        (CU_add_test(my_test_suite, "Test Linked any", test_linked_list_any) == NULL) ||
        (CU_add_test(my_test_suite, "Test Linked apply to all", test_linked_list_apply_to_all) == NULL) ||
        (CU_add_test(my_test_suite, "Test iterator has next", test_iter_has_next) == NULL) ||
        (CU_add_test(my_test_suite, "Test iterator next", test_iter_next) == NULL) ||
        (CU_add_test(my_test_suite, "Test iterator current", test_iter_current) == NULL) ||
        (CU_add_test(my_test_suite, "Test iterator reset", test_iter_reset) == NULL) ||
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
