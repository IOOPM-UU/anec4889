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
    ioopm_warehouse_t *wh = ioopm_warehouse_create();
    char *a = strdup("a");
    char *b = strdup("b");

    ioopm_add_merch(wh, a, b, 1);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).success);

    destroy_all(wh);
}

void test_remove_merch()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();
    char *a = strdup("a");
    char *b = strdup("b");

    ioopm_add_merch(wh, a, b, 1);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);
    ioopm_remove_merch(wh, "a");

    CU_ASSERT_FALSE(ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).success);

    destroy_all(wh);
}

void test_edit_merch()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();
    char *a = strdup("a");
    char *b = strdup("b");
    char *c = strdup("c");
    char *d = strdup("d");

    ioopm_add_merch(wh, a, b, 1);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);
    ioopm_edit_merch(wh, "a", c, d, 2);

    CU_ASSERT_FALSE(ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).success);
    CU_ASSERT_TRUE(ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "c"}).success);

    destroy_all(wh);
}

void test_replenish()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();
    char *a = strdup("a");
    char *b = strdup("b");

    ioopm_add_merch(wh, a, b, 1);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);

    shelf_t *shelf = ioopm_linked_list_get(merch1->locs, 0).p;

    CU_ASSERT_EQUAL(shelf->quantity, 50);

    destroy_all(wh);
}

void test_create_cart()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();

    ioopm_create_cart(wh);

    CU_ASSERT_TRUE(ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).success);

    destroy_all(wh);
}

void test_remove_cart()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();

    ioopm_create_cart(wh);

    char *a = strdup("a");
    char *b = strdup("b");

    ioopm_add_merch(wh, a, b, 1);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);

    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).value.p;

    ioopm_add_cart(cart, merch1, 30);

    ioopm_remove_cart(wh, 0);

    CU_ASSERT_FALSE(ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).success);

    destroy_all(wh);
}

void test_add_to_cart()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();

    ioopm_create_cart(wh);

    char *a = strdup("a");
    char *b = strdup("b");

    ioopm_add_merch(wh, a, b, 1);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);

    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).value.p;

    ioopm_add_cart(cart, merch1, 30);
    ioopm_add_cart(cart, merch1, 10);

    int i = ioopm_hash_table_lookup(cart, (elem_t){.p = merch1}).value.i;

    CU_ASSERT_EQUAL(i, 40);

    destroy_all(wh);
}

void test_remove_from_cart()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();

    ioopm_create_cart(wh);

    char *a = strdup("a");
    char *b = strdup("b");

    ioopm_add_merch(wh, a, b, 1);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);

    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).value.p;

    ioopm_add_cart(cart, merch1, 30);

    ioopm_remove_merch_cart(cart, merch1, 10);

    int i = ioopm_hash_table_lookup(cart, (elem_t){.p = merch1}).value.i;

    CU_ASSERT_EQUAL(i, 20);

    destroy_all(wh);
}

void test_calculate()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();

    ioopm_create_cart(wh);

    char *a = strdup("a");
    char *b = strdup("b");
    char *c = strdup("c");
    char *d = strdup("d");

    ioopm_add_merch(wh, a, b, 25);
    ioopm_add_merch(wh, c, d, 10);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    merch_t *merch2 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "c"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);
    ioopm_replenish(wh, merch2, 1, 10);

    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).value.p;

    ioopm_add_cart(cart, merch1, 30);
    ioopm_add_cart(cart, merch2, 5);

    CU_ASSERT_EQUAL(800, ioopm_calculate_cost(cart));

    destroy_all(wh);
}

void test_checkout()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();

    ioopm_create_cart(wh);

    char *a = strdup("a");
    char *b = strdup("b");
    char *c = strdup("c");
    char *d = strdup("d");

    ioopm_add_merch(wh, a, b, 25);
    ioopm_add_merch(wh, c, d, 10);
    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    merch_t *merch2 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "c"}).value.p;
    ioopm_replenish(wh, merch1, 1, 50);
    shelf_maker(wh, merch1);
    ioopm_replenish(wh, merch1, 2, 30);
    ioopm_replenish(wh, merch2, 1, 10);

    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).value.p;

    ioopm_add_cart(cart, merch1, 70);
    ioopm_add_cart(cart, merch2, 5);

    ioopm_checkout(wh, cart, 0);

    CU_ASSERT_FALSE(ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).success);
    shelf_t *shelf1 = merch1->locs->head->next->element.p;
    shelf_t *shelf2 = merch1->locs->head->next->next->element.p;
    shelf_t *shelf3 = merch2->locs->head->next->element.p;

    CU_ASSERT_EQUAL(shelf1->quantity, 0);
    CU_ASSERT_EQUAL(shelf2->quantity, 10);
    CU_ASSERT_EQUAL(shelf3->quantity, 5);

    destroy_all(wh);
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
        (CU_add_test(my_test_suite, "Test remove merch", test_remove_merch) == NULL) ||
        (CU_add_test(my_test_suite, "Test edit merch", test_edit_merch) == NULL) ||
        (CU_add_test(my_test_suite, "Test replenish merch", test_replenish) == NULL) ||
        (CU_add_test(my_test_suite, "Test create cart", test_create_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Test remove cart", test_remove_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Test add to cart", test_add_to_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Test remove from cart", test_remove_from_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Test calculate cost", test_calculate) == NULL) ||
        (CU_add_test(my_test_suite, "Test Checkout", test_checkout) == NULL) ||
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
