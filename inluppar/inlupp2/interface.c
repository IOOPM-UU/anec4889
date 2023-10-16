#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stddef.h>
#include "webstore.h"

static bool key_eq(elem_t a, elem_t b)
{
    return strcmp(a.p, b.p) == 0;
}

void inter_add_merch(ioopm_warehouse_t *wh)
{
    char *name = ask_question_string("Enter name for merchandise: ");
    while (ioopm_hash_table_has_key(wh->merch_ht, (elem_t){.p = name}))
    {
        printf("%s is occupied!\n", name);
        free(name);
        name = ask_question_string("Enter name for merchandise: ");
    }

    char *desc = ask_question_string("Enter a description of merchandise: ");
    int price = ask_question_int("Enter a price for merchandise: ");

    ioopm_add_merch(wh, name, desc, price);
}

void inter_remove_merch(ioopm_warehouse_t *wh)
{
    char *input_rmv = ask_question_string("Name of item to remove: ");

    while (!ioopm_hash_table_has_key(wh->merch_ht, (elem_t){.p = input_rmv}))
    {
        printf("%s doesn't exist, try again!\n", input_rmv);
        free(input_rmv);
        input_rmv = ask_question_string("Name of item to remove: ");
    }

    char *confirmation = ask_question_letter("[Y] Do you really want to remove?");
    if (toupper(confirmation[0]) == 89)
    {
        free(confirmation);
        ioopm_remove_merch(wh, input_rmv);
    }
    free(input_rmv);
}

void inter_edit_merch(ioopm_warehouse_t *wh)
{
    // Ask for item to edit
    char *input_edit = ask_question_string("Name of item to edit: ");
    while (!ioopm_hash_table_has_key(wh->merch_ht, (elem_t){.p = input_edit}))
    {
        printf("%s doesn't exist, try again!\n", input_edit);
        free(input_edit);
        input_edit = ask_question_string("Name of item to remove: ");
    }

    char *confirmation = ask_question_letter("[Y] Do you really want to edit?");
    if (toupper(confirmation[0]) == 89)
    {
        // Ask for new values
        char *new_name = ask_question_string("Edit name for merchandise: ");
        while (ioopm_hash_table_has_key(wh->merch_ht, (elem_t){.p = new_name}))
        {
            printf("%s is occupied!\n", new_name);
            free(new_name);
            new_name = ask_question_string("Edit name for merchandise: ");
        }
        char *new_desc = ask_question_string("Edit a description of merchandise: ");
        int new_price = ask_question_int("Edit a price for merchandise: ");

        ioopm_edit_merch(wh, input_edit, new_name, new_desc, new_price);
    }
    free(confirmation);

    free(input_edit);
}

merch_t *ask_merch(ioopm_warehouse_t *wh)
{
    char *mrc_name = ask_question_string("Name of merch: ");
    while (!ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = mrc_name}).success)
    {
        printf("%s Doesn't exist!\n", mrc_name);
        free(mrc_name);
        mrc_name = ask_question_string("Name of merch: ");
    }

    merch_t *mrc = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = mrc_name}).value.p;
    free(mrc_name);
    return mrc;
}

void inter_show_stock(ioopm_warehouse_t *wh)
{
    merch_t *mrc = ask_merch(wh);

    ioopm_show_stock(mrc);
}

void inter_replenish(ioopm_warehouse_t *wh)
{
    merch_t *mrc = ask_merch(wh);

    ioopm_replenish(wh, mrc);
}

void event_loop(ioopm_warehouse_t *wh)
{
    char *ans = ask_question_menu();
    puts("");
    if (toupper(ans[0]) == 65)
    {
        inter_add_merch(wh);
    }
    else if (toupper(ans[0]) == 76)
    {
        ioopm_list_merch(wh);
    }
    else if (toupper(ans[0]) == 68)
    {
        inter_remove_merch(wh);
    }
    else if (toupper(ans[0]) == 69)
    {
        inter_edit_merch(wh);
    }
    else if (toupper(ans[0]) == 83)
    {
        inter_show_stock(wh);
    }
    else if (toupper(ans[0]) == 80)
    {
        inter_replenish(wh);
    }
    else if (toupper(ans[0]) == 81)
    {
        char *quit = ask_question_letter("[Y] Do you really want to quit?");
        if (toupper(quit[0]) == 89)
        {
            free(ans);
            free(quit);
            destroy_all(wh);
            exit(0);
        }
    }
    free(ans);
}

int main()
{
    ioopm_warehouse_t *wh = ioopm_warehouse_create();

    while (true)
    {
        event_loop(wh);
    }

    return 0;
}