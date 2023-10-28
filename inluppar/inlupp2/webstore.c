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

void destroy_ht(elem_t key_ignored, elem_t *value, elem_t extra_ignored)
{
    ioopm_hash_table_destroy(value->p);
}

size_t merch_size(merch_t *merch)
{
    size_t counter = 0;
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
    new_merch->cart_num = 0;

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
                printf("- %s: %ld\n", keys_arr[i], merch_size(current_merch));
            }
        }
        else
        {
            printf("- %s: %ld\n", keys_arr[i], merch_size(current_merch));
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

void ioopm_replenish(ioopm_warehouse_t *wh, merch_t *merch, int shelf_index, int quantity)
{
    if (merch->locs->head->next == NULL)
    {
        shelf_maker(wh, merch);
    }

    shelf_t *to_increase = ioopm_linked_list_get(merch->locs, shelf_index - 1).p;
    to_increase->quantity = quantity;
}

void print_cart(ioopm_hash_table_t *cart)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = cart->buckets[i].next;
        while (current_bucket != NULL)
        {
            merch_t *merch = current_bucket->key.p;
            printf("Item: %s, Quantity: %d\n", merch->name, current_bucket->value.i);
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
    ioopm_list_t *merch_list = ioopm_hash_table_keys(cart);
    ioopm_list_t *quantity_list = ioopm_hash_table_values(cart);
    ioopm_list_iterator_t *merch_itr = ioopm_list_iterator(merch_list);
    ioopm_list_iterator_t *quantity_itr = ioopm_list_iterator(quantity_list);
    while (ioopm_iterator_has_next(merch_itr))
    {
        merch_t *merch = ioopm_iterator_next(merch_itr).p;
        int quantity = ioopm_iterator_next(quantity_itr).i;
        merch->cart_num -= quantity;
    }
    ioopm_iterator_destroy(merch_itr);
    ioopm_iterator_destroy(quantity_itr);
    ioopm_linked_list_destroy(merch_list);
    ioopm_linked_list_destroy(quantity_list);

    ioopm_hash_table_destroy(cart);
    printf("Cart %d has been destroyed\n\n", index + 1);
}

void ioopm_add_cart(ioopm_hash_table_t *cart, merch_t *merch, int quantity)
{
    merch->cart_num += quantity;
    if (ioopm_hash_table_has_key(cart, (elem_t){.p = merch}))
    {
        quantity += ioopm_hash_table_lookup(cart, (elem_t){.p = merch}).value.i;
        ioopm_hash_table_insert(cart, (elem_t){.p = merch}, (elem_t){.i = quantity});
    }
    else
    {
        ioopm_hash_table_insert(cart, (elem_t){.p = merch}, (elem_t){.i = quantity});
    }

    print_cart(cart);
}

int find_cart_index(ioopm_hash_table_t *cart, merch_t *merch)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_cart = cart->buckets[i].next;
        if (current_cart)
        {
            merch_t *current_merch = current_cart->key.p;
            if (cart->key_eq_fun((elem_t){.p = current_merch->name}, (elem_t){.p = merch->name}))
            {
                return i;
            }
        }
    }
    return -1;
}

void ioopm_remove_merch_cart(ioopm_hash_table_t *cart, merch_t *merch, int quantity)
{
    int cart_index = find_cart_index(cart, merch);
    merch_t *mrc = cart->buckets[cart_index].next->key.p;
    cart->buckets[cart_index].next->value.i -= quantity;
    mrc->cart_num -= quantity;
}

int ioopm_calculate_cost(ioopm_hash_table_t *cart)
{
    int total_cost = 0;

    ioopm_list_t *merch = ioopm_hash_table_keys(cart);
    ioopm_list_t *quantity = ioopm_hash_table_values(cart);
    ioopm_list_iterator_t *merch_itr = ioopm_list_iterator(merch);
    ioopm_list_iterator_t *quantity_itr = ioopm_list_iterator(quantity);

    while (ioopm_iterator_has_next(merch_itr))
    {
        merch_t *mrc = ioopm_iterator_next(merch_itr).p;
        int quant = ioopm_iterator_next(quantity_itr).i;

        total_cost += (mrc->price * quant);
    }

    ioopm_iterator_destroy(quantity_itr);
    ioopm_linked_list_destroy(quantity);
    ioopm_iterator_destroy(merch_itr);
    ioopm_linked_list_destroy(merch);

    return total_cost;
}

void ioopm_checkout(ioopm_warehouse_t *wh, ioopm_hash_table_t *cart, int index)
{
    ioopm_list_t *merch = ioopm_hash_table_keys(cart);
    ioopm_list_t *quantity = ioopm_hash_table_values(cart);
    ioopm_list_iterator_t *merch_itr = ioopm_list_iterator(merch);
    ioopm_list_iterator_t *quantity_itr = ioopm_list_iterator(quantity);

    while (ioopm_iterator_has_next(merch_itr))
    {
        merch_t *mrc = ioopm_iterator_next(merch_itr).p;
        int quant = ioopm_iterator_next(quantity_itr).i;

        ioopm_list_iterator_t *shelfs = ioopm_list_iterator(mrc->locs);

        while (quant > 0)
        {
            shelf_t *shelf = ioopm_iterator_next(shelfs).p;

            if (shelf->quantity < quant)
            {
                quant -= shelf->quantity;
                shelf->quantity = 0;
            }
            else
            {
                shelf->quantity -= quant;
                quant = 0;
            }
        }
        ioopm_iterator_destroy(shelfs);
    }

    ioopm_iterator_destroy(quantity_itr);
    ioopm_linked_list_destroy(quantity);
    ioopm_iterator_destroy(merch_itr);
    ioopm_linked_list_destroy(merch);

    printf("Checked out cart %d:\n", index + 1);
    print_cart(cart);
    printf("Total cost: %d\n", ioopm_calculate_cost(cart));

    ioopm_remove_cart(wh, index);
}

void destroy_all(ioopm_warehouse_t *wh)
{
    ioopm_hash_table_apply_to_all(wh->merch_ht, destroy_all_fun, (elem_t){.p = NULL});
    ioopm_hash_table_apply_to_all(wh->cart_ht, destroy_ht, (elem_t){.p = NULL});
    ioopm_hash_table_destroy(wh->location_ht);
    ioopm_hash_table_destroy(wh->merch_ht);

    ioopm_hash_table_destroy(wh->cart_ht);
    free(wh);
}