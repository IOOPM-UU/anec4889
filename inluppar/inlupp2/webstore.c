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
    ioopm_warehouse_t *new_wh = calloc(1, sizeof(ioopm_warehouse_t));
    new_wh->merch_ht = merch_h;
    new_wh->location_ht = locations_h;
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
    ioopm_list_iterator_t *itr = ioopm_list_iterator(list);
    int size = ioopm_hash_table_size(wh->merch_ht);
    char *keys_arr[size];
    int arr_i = 0;

    while (ioopm_iterator_has_next(itr))
    {
        keys_arr[arr_i] = ioopm_iterator_next(itr).p;
        arr_i++;
    }
    ioopm_iterator_destroy(itr);
    ioopm_linked_list_destroy(list);

    sort_keys(keys_arr, size);

    for (int i = 0; i < size; ++i)
    {
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
                printf("- %s\n", keys_arr[i]);
            }
        }
        else
        {
            printf("- %s\n", keys_arr[i]);
        }
    }
    puts("");
}

// TODO: Kanske behöver kika på remove wh
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

void destroy_all(ioopm_warehouse_t *wh)
{
    ioopm_hash_table_apply_to_all(wh->merch_ht, destroy_all_fun, (elem_t){.p = NULL});
    // ioopm_hash_table_apply_to_all(wh->location_ht, destroy_all_fun2, (elem_t){.p = NULL});
    ioopm_hash_table_destroy(wh->location_ht);
    ioopm_hash_table_destroy(wh->merch_ht);
    free(wh);
}