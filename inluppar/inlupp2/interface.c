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

void inter_add_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf)
{
    char *name = ask_question_string("Enter name for merchandise: ");
    while (ioopm_hash_table_has_key(inf, (elem_t){.p = name}))
    {
        printf("%s is occupied!\n", name);
        name = ask_question_string("Enter name for merchandise: ");
    }

    char *desc = ask_question_string("Enter a description of merchandise: ");
    int price = ask_question_int("Enter a price for merchandise: ");

    ioopm_add_merch(wh, inf, name, desc, price);
}

void inter_remove_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf)
{
    char *input_rmv = ask_question_string("Name of item to remove: ");

    while (!ioopm_hash_table_has_key(inf, (elem_t){.p = input_rmv}))
    {
        printf("%s doesn't exist, try again!", input_rmv);
        input_rmv = ask_question_string("Name of item to remove: ");
    }

    char *confirmation = ask_question_letter("[Y] Do you really want to remove?");
    if (strcmp(confirmation, "Y") == 0)
    {
        ioopm_remove_merch(wh, inf, input_rmv);
    }
}

void inter_edit_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf)
{
    // Ask for item to edit
    char *input_edit = ask_question_string("Name of item to edit: ");
    while (!ioopm_hash_table_has_key(inf, (elem_t){.p = input_edit}))
    {
        printf("%s doesn't exist, try again!\n", input_edit);
        input_edit = ask_question_string("Name of item to remove: ");
    }

    char *confirmation = ask_question_letter("[Y] Do you really want to edit?");
    if (strcmp(confirmation, "Y") == 0)
    {
        // Ask for new values
        char *new_name = ask_question_string("Edit name for merchandise: ");
        while (ioopm_hash_table_has_key(inf, (elem_t){.p = new_name}))
        {
            printf("%s is occupied!\n", new_name);
            new_name = ask_question_string("Edit name for merchandise: ");
        }
        char *new_desc = ask_question_string("Edit a description of merchandise: ");
        int new_price = ask_question_int("Edit a price for merchandise: ");

        ioopm_edit_merch(wh, inf, input_edit, new_name, new_desc, new_price);
    }
}

void event_loop(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf)
{
    char *ans = ask_question_menu();
    puts("");
    if (strcmp(ans, "A") == 0)
    {
        inter_add_merch(wh, inf);
    }
    else if (strcmp(ans, "L") == 0)
    {
        ioopm_list_merch(inf);
    }
    else if (strcmp(ans, "D") == 0)
    {
        inter_remove_merch(wh, inf);
    }
    else if (strcmp(ans, "E") == 0)
    {
        inter_edit_merch(wh, inf);
    }
    else if (strcmp(ans, "Q") == 0)
    {
        char *quit = ask_question_letter("[Y] Do you really want to quit?");
        if (strcmp(quit, "Y") == 0)
        {
            // free(quit);
            // free(ans);
            destroy_all(wh, inf);
            exit(0);
        }
    }
    // free(ans);
}

int main()
{
    ioopm_hash_table_t *warehoushe_ht = ioopm_hash_table_create(string_sum_hash, shelf_eq, NULL);
    ioopm_hash_table_t *information_ht = ioopm_hash_table_create(string_sum_hash, key_eq, NULL);

    while (true)
    {
        event_loop(warehoushe_ht, information_ht);
    }

    return 0;
}