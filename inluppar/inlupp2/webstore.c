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

static int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

merch_t *ioopm_merch_create(char *n, char *d, int p)
{
    merch_t *new_merch = calloc(1, sizeof(merch_t));
    new_merch->name = strdup(n);
    new_merch->desc = strdup(d);
    new_merch->price = p;
    new_merch->locs = ioopm_linked_list_create(shelf_eq);

    return new_merch;
}

static char *shelf_maker(ioopm_hash_table_t *wh)
{
    char shelf[4];
    do
    {
        shelf[0] = 'A' + (rand() % 26);
        shelf[1] = '0' + (rand() % 10);
        shelf[2] = '0' + (rand() % 10);
        shelf[3] = '\0';
    } while (ioopm_hash_table_has_key(wh, (elem_t){.p = shelf}));
    return strdup(shelf);
}

void ioopm_add_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf, char *name, char *desc, int price)
{
    merch_t *new_merch = ioopm_merch_create(name, desc, price);

    char *new_shelf_name = shelf_maker(wh);
    shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
    new_shelf->name = new_shelf_name;
    new_shelf->quantity = 0;

    ioopm_linked_list_append(new_merch->locs, (elem_t){.p = new_shelf});
    ioopm_hash_table_insert(inf, (elem_t){.p = strdup(name)}, (elem_t){.p = new_merch});
    ioopm_hash_table_insert(wh, (elem_t){.p = strdup(new_shelf_name)}, (elem_t){.p = name});

    free(new_shelf_name);
    free(new_shelf);
    free(name);
    free(desc);
}

void ioopm_list_merch(ioopm_hash_table_t *inf)
{
    ioopm_list_t *list = ioopm_hash_table_keys(inf);
    ioopm_list_iterator_t *itr = ioopm_list_iterator(list);
    int size = ioopm_hash_table_size(inf);
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
            if (strcmp(cont, "N") == 0)
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
void ioopm_remove_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf, char *rmv_item)
{

    merch_t *to_remove = ioopm_hash_table_lookup(inf, (elem_t){.p = rmv_item}).value.p;

    // Remove every instance of item in the warehouse hashtable
    ioopm_list_iterator_t *itr = ioopm_list_iterator(to_remove->locs);
    while (ioopm_iterator_has_next(itr))
    {
        ioopm_hash_table_remove(wh, (elem_t){.p = ioopm_iterator_next(itr).p});
    }
    ioopm_iterator_destroy(itr);
    ioopm_linked_list_destroy(to_remove->locs);

    // Remove item from hashtable
    ioopm_hash_table_remove(inf, (elem_t){.p = rmv_item});

    free(rmv_item);
}

void ioopm_edit_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf, char *old_name, char *new_name, char *desc, int price)
{
    // Edit wh
    merch_t *item = ioopm_hash_table_lookup(inf, (elem_t){.p = old_name}).value.p;
    // ioopm_list_iterator_t *item_itr = ioopm_list_iterator(item->locs);

    // while (ioopm_iterator_has_next(item_itr))
    // {
    //     elem_t *to_change;
    //     to_change->p = ioopm_hash_table_lookup(wh, (elem_t){.p = item_itr->current->element.p}).value.p;
    //     to_change->p = new_name;
    // }
    // ioopm_iterator_destroy(item_itr);

    ioopm_hash_table_remove(inf, (elem_t){.p = old_name});

    item->name = strdup(new_name);
    item->desc = strdup(desc);
    item->price = price;

    ioopm_hash_table_insert(inf, (elem_t){.p = strdup(new_name)}, (elem_t){.p = item});

    free(old_name);
    free(new_name);
    free(desc);
}

void destroy_all(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf)
{
    for (int i = 0; i < No_Buckets; ++i)
    {
        entry_t *current_bucket = inf->buckets[i].next;
        while (current_bucket != NULL)
        {
            merch_t *merch_to_remove = current_bucket->value.p;
            ioopm_linked_list_destroy(merch_to_remove->locs);
        }
    }
    ioopm_hash_table_destroy(wh);
    ioopm_hash_table_destroy(inf);
}