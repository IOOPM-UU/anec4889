#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stddef.h>
#include "webstore.h"

static bool shelf_eq(elem_t a, elem_t b)
{
    shelf_t shelf_a = {.name = a.p};
    shelf_t shelf_b = {.name = b.p};

    return strcmp(shelf_a.name, shelf_b.name) == 0;
}

static int string_sum_hash(elem_t e)
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

static bool cart_key(elem_t a, elem_t b)
{
    return a.i == b.i;
}

static int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void destroy_all_fun(elem_t key_ignored, elem_t *value, elem_t extra_ignored)
{
    merch_t *mrc_to_destroy = value->p;
    free(mrc_to_destroy->name);
    free(mrc_to_destroy->desc);

    while (mrc_to_destroy->locs->size > 0)
    {
        shelf_t *shf_to_remove = ioopm_linked_list_remove(mrc_to_destroy->locs, 0).p;
        free(shf_to_remove->name);
        free(shf_to_remove);
    }

    ioopm_linked_list_destroy(mrc_to_destroy->locs);
    free(mrc_to_destroy);
}

void destroy_all_fun2(elem_t key_ignored, elem_t *value, elem_t extra_ignored)
{
    free(value->p);
}

int merch_size(merch_t *merch)
{
    int counter = 0;
    ioopm_list_t *list = merch->locs;
    link_t *cursor = list->head->next;
    while (cursor)
    {
        shelf_t *shelf = cursor->element.p;
        counter += shelf->quantity;
        cursor = cursor->next;
    }
    return counter;
}

merch_t *ioopm_merch_create(char *n, char *d, int p)
{
    merch_t *new_merch = calloc(1, sizeof(merch_t));

    new_merch->name = n;
    new_merch->desc = d;
    new_merch->price = p;
    new_merch->locs = ioopm_linked_list_create(shelf_eq);

    return new_merch;
}

void shelf_maker(ioopm_warehouse_t *wh, merch_t *merch)
{
    char shelf[4];
    do
    {
        shelf[0] = 'A' + (rand() % 26);
        shelf[1] = '0' + (rand() % 10);
        shelf[2] = '0' + (rand() % 10);
        shelf[3] = '\0';
    } while (ioopm_hash_table_has_key(wh->location_ht, (elem_t){.p = shelf}));

    shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
    new_shelf->name = strdup(shelf);
    new_shelf->quantity = 0;

    ioopm_linked_list_append(merch->locs, (elem_t){.p = new_shelf});
    ioopm_hash_table_insert(wh->location_ht, (elem_t){.p = new_shelf->name}, (elem_t){.p = merch->name});
}

ioopm_warehouse_t *ioopm_warehouse_create()
{
    ioopm_hash_table_t *merch_h = ioopm_hash_table_create(string_sum_hash, key_eq, NULL);
    ioopm_hash_table_t *locations_h = ioopm_hash_table_create(string_sum_hash, shelf_eq, NULL);
    ioopm_hash_table_t *cart_h = ioopm_hash_table_create(NULL, cart_key, NULL);
    ioopm_warehouse_t *new_wh = calloc(1, sizeof(ioopm_warehouse_t));
    new_wh->merch_ht = merch_h;
    new_wh->location_ht = locations_h;
    new_wh->cart_ht = cart_h;
    new_wh->cart_index = 0;
    return new_wh;
}

void ioopm_add_merch(ioopm_warehouse_t *wh, char *name, char *desc, int price)
{
    merch_t *new_merch = ioopm_merch_create(name, desc, price);

    ioopm_hash_table_insert(wh->merch_ht, (elem_t){.p = name}, (elem_t){.p = new_merch});
}

void ioopm_list_merch(ioopm_warehouse_t *wh)
{
    ioopm_list_t *list = ioopm_hash_table_keys(wh->merch_ht);
    ioopm_list_t *list_merch = ioopm_hash_table_values(wh->merch_ht);
    ioopm_list_iterator_t *itr = ioopm_list_iterator(list);
    ioopm_list_iterator_t *itr_merch = ioopm_list_iterator(list_merch);
    int size = ioopm_hash_table_size(wh->merch_ht);
    char *keys_arr[size];
    merch_t *merch_arr[size];
    int arr_i = 0;

    while (ioopm_iterator_has_next(itr))
    {
        merch_arr[arr_i] = ioopm_iterator_next(itr_merch).p;
        keys_arr[arr_i] = ioopm_iterator_next(itr).p;
        arr_i++;
    }
    ioopm_iterator_destroy(itr);
    ioopm_linked_list_destroy(list);
    ioopm_iterator_destroy(itr_merch);
    ioopm_linked_list_destroy(list_merch);

    // sort_keys(keys_arr, size);
    // sort_keys(merch_arr->name, size);

    for (int i = 0; i < size; ++i)
    {
        merch_t *current_merch = merch_arr[i];
        if (i % 20 == 0 && i > 0)
        {
            char *cont = ask_question_letter("Continue listing? (<RET> for yes, [N] for no): ");
            if (toupper(cont[0]) == 78)
            {
                free(cont);
                break;
            }
            else
            {
                printf("- %s: %d\n", keys_arr[i], merch_size(current_merch));
            }
        }
        else
        {
            printf("- %s: %d\n", keys_arr[i], merch_size(current_merch));
        }
    }
    puts("");
}

void ioopm_remove_merch(ioopm_warehouse_t *wh, char *rmv_item)
{
    merch_t *to_remove = ioopm_hash_table_remove(wh->merch_ht, (elem_t){.p = rmv_item}).value.p;

    // Remove every instance of item in the warehouse hashtable
    ioopm_list_iterator_t *itr = ioopm_list_iterator(to_remove->locs);
    while (ioopm_iterator_has_next(itr))
    {
        ioopm_hash_table_remove(wh->location_ht, (elem_t){.p = ioopm_iterator_next(itr).p});
    }
    ioopm_iterator_destroy(itr);

    destroy_all_fun((elem_t){.p = NULL}, &((elem_t){.p = to_remove}), (elem_t){.p = NULL});
}

void ioopm_edit_merch(ioopm_warehouse_t *wh, char *old_name, char *new_name, char *desc, int price)
{
    merch_t *item_to_remove = ioopm_hash_table_remove(wh->merch_ht, (elem_t){.p = old_name}).value.p;
    free(item_to_remove->name);
    free(item_to_remove->desc);

    item_to_remove->name = new_name;
    item_to_remove->desc = desc;
    item_to_remove->price = price;

    ioopm_hash_table_insert(wh->merch_ht, (elem_t){.p = new_name}, (elem_t){.p = item_to_remove});

    ioopm_list_iterator_t *item_itr = ioopm_list_iterator(item_to_remove->locs);

    while (ioopm_iterator_has_next(item_itr))
    {
        ioopm_hash_table_insert(wh->location_ht, (elem_t){.p = ioopm_iterator_next(item_itr).p}, (elem_t){.p = new_name});
    }
    ioopm_iterator_destroy(item_itr);
}

void ioopm_show_stock(merch_t *merch)
{
    if (merch->locs->head->next == NULL)
    {
        printf("%s doesn't have stock, please replenish\n", merch->name);
    }
    else
    {
        link_t *shelf = merch->locs->head->next;
        int i = 0;
        while (shelf)
        {
            shelf_t *shf = ioopm_linked_list_get(merch->locs, i).p;
            printf("- %d: (%s: %d)\n", i + 1, shf->name, shf->quantity);
            shelf = shelf->next;
            i++;
        }
        puts("");
    }
}

void ioopm_replenish(ioopm_warehouse_t *wh, merch_t *merch)
{
    if (merch->locs->head->next == NULL)
    {
        shelf_maker(wh, merch);
    }

    ioopm_show_stock(merch);

    char *ans = ask_question_replenish();
    while (toupper(ans[0]) == 65)
    {
        free(ans);
        shelf_maker(wh, merch);
        ioopm_show_stock(merch);
        ans = ask_question_replenish();
    }
    free(ans);
    int shelf = ask_question_int("Shelf to replenish (index):");
    while (shelf > ioopm_linked_list_size(merch->locs) || shelf < 1)
    {
        printf("%s doesn't have shelf-index %d\n", merch->name, shelf);
        shelf = ask_question_int("Shelf to replenish (index):");
    }

    int i = ask_question_int("Increase stock with: ");

    shelf_t *to_increase = ioopm_linked_list_get(merch->locs, shelf - 1).p;
    to_increase->quantity = i;
}

void cart_print(ioopm_hash_table_t *cart)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = cart->buckets[i].next;
        while (current_bucket != NULL)
        {
            printf("Item: %s, Quantity: %d\n", current_bucket->key.p, current_bucket->value.i);
            current_bucket = current_bucket->next;
        }
    }
}

void ioopm_create_cart(ioopm_warehouse_t *wh)
{
    ioopm_hash_table_t *cart = ioopm_hash_table_create(string_sum_hash, key_eq, NULL);
    ioopm_hash_table_insert(wh->cart_ht, (elem_t){.i = wh->cart_index}, (elem_t){.p = cart});
    wh->cart_index++;
    printf("Cart %ld has been created\n\n", wh->cart_index);
}

void ioopm_remove_cart(ioopm_warehouse_t *wh, int index)
{
    ioopm_hash_table_t *cart = ioopm_hash_table_remove(wh->cart_ht, (elem_t){.i = index}).value.p;
    ioopm_hash_table_destroy(cart);
    printf("Cart %d has been destroyed\n\n", index + 1);
}

void ioopm_add_cart(ioopm_warehouse_t *wh, ioopm_hash_table_t *cart, merch_t *merch, int quantity)
{
    ioopm_hash_table_insert(cart, (elem_t){.p = merch}, (elem_t){.i = quantity});
    cart_print(cart);
}

void destroy_all(ioopm_warehouse_t *wh)
{
    ioopm_hash_table_apply_to_all(wh->merch_ht, destroy_all_fun, (elem_t){.p = NULL});
    // ioopm_hash_table_apply_to_all(wh->location_ht, destroy_all_fun2, (elem_t){.p = NULL});
    ioopm_hash_table_destroy(wh->location_ht);
    ioopm_hash_table_destroy(wh->merch_ht);
    ioopm_hash_table_destroy(wh->cart_ht);
    free(wh);
}