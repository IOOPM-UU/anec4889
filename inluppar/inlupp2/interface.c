#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stddef.h>
#include "webstore.h"

// static bool key_eq(elem_t a, elem_t b)
// {
//     return strcmp(a.p, b.p) == 0;
// }

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

merch_t *ask_merch_remove(ioopm_warehouse_t *wh, ioopm_hash_table_t *cart)
{
    char *mrc_name = ask_question_string("Name of merch to remove: ");
    if (ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = mrc_name}).value.p)
    {
        merch_t *mrc = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = mrc_name}).value.p;
        while (!ioopm_hash_table_lookup(cart, (elem_t){.p = mrc}).success)
        {
            printf("%s Doesn't exist!\n", mrc_name);
            free(mrc_name);
            mrc_name = ask_question_string("Name of merch: ");
            mrc = ioopm_hash_table_lookup(cart, (elem_t){.p = mrc_name}).value.p;
        }
        free(mrc_name);
        return mrc;
    }
    free(mrc_name);
    return ask_merch_remove(wh, cart);
}

void inter_show_stock(ioopm_warehouse_t *wh)
{
    merch_t *mrc = ask_merch(wh);

    ioopm_show_stock(mrc);
}

void inter_replenish(ioopm_warehouse_t *wh)
{
    merch_t *mrc = ask_merch(wh);
    if (mrc->locs->head->next == NULL)
    {
        shelf_maker(wh, mrc);
    }
    ioopm_show_stock(mrc);

    char *ans = ask_question_replenish();
    while (toupper(ans[0]) == 65)
    {
        free(ans);
        shelf_maker(wh, mrc);
        ioopm_show_stock(mrc);
        ans = ask_question_replenish();
    }
    free(ans);
    int shelf = ask_question_int("Shelf to replenish (index):");
    while (shelf > ioopm_linked_list_size(mrc->locs) || shelf < 1)
    {
        printf("%s doesn't have shelf-index %d\n", mrc->name, shelf);
        shelf = ask_question_int("Shelf to replenish (index):");
    }

    int i = ask_question_int("Increase stock with: ");

    ioopm_replenish(wh, mrc, shelf, i);
}

bool exists_cart(ioopm_warehouse_t *wh, int index)
{
    if (index < 1)
    {
        return false;
    }
    else
    {
        elem_t x;
        x.i = index - 1;
        option_t t = ioopm_hash_table_lookup(wh->cart_ht, x);
        return t.success;
    }
    return false;
}

void inter_remove_cart(ioopm_warehouse_t *wh)
{
    printf("\nExisting carts:\n\n");
    for (int i = 1; i <= No_Buckets; i++)
    {
        if (exists_cart(wh, i))
        {
            ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = i - 1}).value.p;
            printf("Cart %d: \n", i);
            print_cart(cart);
            puts("");
        }
    }

    int index = ask_question_int("Cart to remove: ");
    while (!exists_cart(wh, index))
    {
        printf("Cart %d does not exist!\n", index);
        index = ask_question_int("Cart to remove: ");
    }

    char *confirmation = ask_question_letter("[Y] Do you really want to remove?");
    if (toupper(confirmation[0]) == 89)
    {
        ioopm_remove_cart(wh, index - 1);
    }
    free(confirmation);
}

bool cart_check(ioopm_warehouse_t *wh, merch_t *merch, int quantity)
{
    if ((quantity + merch->cart_num) <= merch_size(merch) && quantity > 0)
    {
        return true;
    }
    return false;
}

void inter_add_cart(ioopm_warehouse_t *wh)
{
    for (int i = 1; i <= No_Buckets; i++)
    {
        if (exists_cart(wh, i))
        {
            ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = i - 1}).value.p;
            printf("Cart %d: \n", i);
            print_cart(cart);
            puts("");
        }
    }

    int index = ask_question_int("Cart to add: ");
    while (!exists_cart(wh, index))
    {
        printf("Cart %d does not exist!\n", index);
        index = ask_question_int("Cart to add: ");
    }

    ioopm_list_merch(wh);
    merch_t *merch = ask_merch(wh);

    int quant = ask_question_int("How many: ");
    while (!cart_check(wh, merch, quant))
    {
        puts("Not a valid amount\n");
        quant = ask_question_int("How many: ");
    }
    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = index - 1}).value.p;
    ioopm_add_cart(cart, merch, quant);
}

void inter_remove_merch_cart(ioopm_warehouse_t *wh)
{
    for (int i = 1; i <= No_Buckets; i++)
    {
        if (exists_cart(wh, i))
        {
            ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = i - 1}).value.p;
            printf("Cart %d: \n", i);
            print_cart(cart);
            puts("");
        }
    }

    int index = ask_question_int("Cart to remove from: ");
    while (!exists_cart(wh, index))
    {
        printf("Cart %d does not exist!\n", index);
        index = ask_question_int("Cart to remove from: ");
    }
    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = index - 1}).value.p;
    merch_t *merch = ask_merch_remove(wh, cart);
    int quant = ask_question_int("How many: ");
    int cart_index = find_cart_index(cart, merch);
    while (cart->buckets[cart_index].next->value.i < quant || quant < 0)
    {
        puts("Not a valid amount\n");
        quant = ask_question_int("How many: ");
    }
    ioopm_remove_merch_cart(cart, merch, quant);
}

void inter_calculate_cost(ioopm_warehouse_t *wh)
{
    for (int i = 1; i <= No_Buckets; i++)
    {
        if (exists_cart(wh, i))
        {
            ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = i - 1}).value.p;
            printf("Cart %d: \n", i);
            print_cart(cart);
            puts("");
        }
    }
    int index = ask_question_int("Cart to calculate: ");
    while (!exists_cart(wh, index))
    {
        printf("Cart %d does not exist!\n", index);
        index = ask_question_int("Cart to calculate: ");
    }

    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = index - 1}).value.p;
    printf("Cost: %d\n", ioopm_calculate_cost(cart));
}

void inter_checkout(ioopm_warehouse_t *wh)
{
    for (int i = 1; i <= No_Buckets; i++)
    {
        if (exists_cart(wh, i))
        {
            ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = i - 1}).value.p;
            printf("Cart %d: \n", i);
            print_cart(cart);
            puts("");
        }
    }
    int index = ask_question_int("Cart to checkout: ");
    while (!exists_cart(wh, index))
    {
        printf("Cart %d does not exist!\n", index);
        index = ask_question_int("Cart to checkout: ");
    }

    ioopm_hash_table_t *cart = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = index - 1}).value.p;
    ioopm_checkout(cart);

    printf("Total cost of purchase: %d\n", ioopm_calculate_cost(cart));
}

void event_loop(ioopm_warehouse_t *wh)
{
    char *ans = ask_question_menu();
    puts("");
    if (toupper(ans[0]) == 'A')
    {
        inter_add_merch(wh);
    }
    else if (toupper(ans[0]) == 'L')
    {
        ioopm_list_merch(wh);
    }
    else if (toupper(ans[0]) == 'D')
    {
        inter_remove_merch(wh);
    }
    else if (toupper(ans[0]) == 'E')
    {
        inter_edit_merch(wh);
    }
    else if (toupper(ans[0]) == 'S')
    {
        inter_show_stock(wh);
    }
    else if (toupper(ans[0]) == 'P')
    {
        inter_replenish(wh);
    }
    else if (toupper(ans[0]) == 'C')
    {
        ioopm_create_cart(wh);
    }
    else if (toupper(ans[0]) == 'R')
    {
        inter_remove_cart(wh);
    }
    else if (ans[0] == '+')
    {
        inter_add_cart(wh);
    }
    else if (ans[0] == '-')
    {
        inter_remove_merch_cart(wh);
    }
    else if (ans[0] == '=')
    {
        inter_calculate_cost(wh);
    }
    else if (toupper(ans[0]) == 'O')
    {
        inter_checkout(wh);
    }
    else if (toupper(ans[0]) == 'Q')
    {
        char *quit = ask_question_letter("[Y] Do you really want to quit?");
        if (toupper(quit[0]) == 'Y')
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
    char *a = strdup("a");
    char *b = strdup("b");
    char *c = strdup("c");
    char *d = strdup("d");
    char *e = strdup("e");
    char *f = strdup("f");
    ioopm_add_merch(wh, a, b, 111);
    ioopm_add_merch(wh, c, d, 222);
    ioopm_add_merch(wh, e, f, 333);

    merch_t *merch1 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "a"}).value.p;
    merch_t *merch2 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "c"}).value.p;
    merch_t *merch3 = ioopm_hash_table_lookup(wh->merch_ht, (elem_t){.p = "e"}).value.p;

    ioopm_replenish(wh, merch1, 1, 50);
    ioopm_replenish(wh, merch2, 1, 80);
    ioopm_replenish(wh, merch3, 1, 100);
    ioopm_create_cart(wh);
    ioopm_create_cart(wh);
    ioopm_create_cart(wh);

    ioopm_hash_table_t *cart1 = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 0}).value.p;
    ioopm_hash_table_t *cart2 = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 1}).value.p;
    ioopm_hash_table_t *cart3 = ioopm_hash_table_lookup(wh->cart_ht, (elem_t){.i = 2}).value.p;

    ioopm_add_cart(cart1, merch1, 20);
    ioopm_add_cart(cart1, merch2, 30);
    ioopm_add_cart(cart1, merch3, 90);
    ioopm_add_cart(cart2, merch1, 10);
    ioopm_add_cart(cart2, merch2, 30);
    ioopm_add_cart(cart2, merch3, 3);
    ioopm_add_cart(cart3, merch1, 10);
    ioopm_add_cart(cart3, merch2, 10);
    ioopm_add_cart(cart3, merch3, 5);
    printf("\n");

    while (true)
    {
        event_loop(wh);
    }

    return 0;
}