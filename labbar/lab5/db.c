#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../lab2/utils.h"

struct item
{
    char *name;
    char *desc;
    int price;
    char *shelf;
};

typedef struct item item_t;

void print_item(item_t *itm)
{
    float tal = (float)itm->price / 100;

    printf("Name:  %s\n", itm->name);
    printf("Desc:  %s\n", itm->desc);
    printf("Price: %05.2f SEK\n", tal);
    printf("Shelf: %s\n", itm->shelf);
}

item_t make_item(char *n, char *d, int p, char *s)
{
    item_t itm = {.name = n, .desc = d, .price = p, .shelf = s};
    return itm;
}

item_t input_item()
{
    char *n = ask_question_string("Type in name: ");
    char *d = ask_question_string("Type in description: ");
    int p = ask_question_int("Type in price: ");
    char *s = ask_question_shelf("Type in shelf: ");

    return make_item(n, d, p, s);
}

char *magick(char *arr1[], char *arr2[], char *arr3[], int i)
{
    char buf[255];
    int r = random() % i;
    int buf_i = 0;
    char *src = arr1[r];
    for (int j = 0; j < strlen(src); j++)
    {
        buf[buf_i] = src[j];
        buf_i++;
    }
    buf[buf_i] = '-';
    buf_i++;

    r = random() % i;
    src = arr2[r];
    for (int j = 0; j < strlen(src); j++)
    {
        buf[buf_i] = src[j];
        buf_i++;
    }

    buf[buf_i] = ' ';
    buf_i++;

    r = random() % i;
    src = arr3[r];
    for (int j = 0; j < strlen(src); j++)
    {
        buf[buf_i] = src[j];
        buf_i++;
    }

    buf[buf_i] = '\0';

    return strdup(buf);
}

void list_db(item_t *items, int no_items)
{
    printf("%d: %s\n", no_items + 1, items->name);
}

void edit_db(item_t *item, int siz)
{
    int edit_id;
    do
    {
        edit_id = ask_question_int("Skriv in varu-id: ");
    } while (edit_id < 1 || edit_id > siz);

    printf("%s\n", item[edit_id - 1].name);

    item[edit_id - 1] = input_item();

    for (int i = 0; i < siz; ++i)
    {
        list_db(&item[i], i);
    }
}

void add_item_to_db(item_t *item, int *siz)
{
    item_t new_item = input_item();

    item[*siz] = new_item;
    *siz = *siz + 1;
}

void remove_item_from_db(item_t *item, int *siz)
{
    int rmv_id;
    do
    {
        rmv_id = ask_question_int("Skriv in varu-id: ");
    } while (rmv_id < 1 || rmv_id > *siz);

    for (int i = rmv_id - 1; i < *siz - 1; ++i)
    {
        item[i] = item[i + 1];
    }
    *siz = *siz - 1;
}

bool is_menu(char *str)
{
    if (strlen(str) != 1)
    {
        return false;
    }
    else
    {
        if (toupper(str[0]) == 76 || toupper(str[0]) == 84 || toupper(str[0]) == 82 || toupper(str[0]) == 71 || toupper(str[0]) == 72 || toupper(str[0]) == 65)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

char *ask_question_menu()
{
    char *menu_str = "[L]ägga till en vara\n[T]a bort en vara\n[R]edigera en vara\nÅn[g]ra senaste ändringen\nLista [h]ela varukatalogen\n[A]vsluta";
    return ask_question(menu_str, is_menu, (convert_func)toupper).string_value;
}

void event_loop(item_t *items, int *siz)
{
    char *ans = ask_question_menu();

    if (toupper(ans[0]) == 76)
    {
        add_item_to_db(items, siz);
    }
    else if (toupper(ans[0]) == 84)
    {
        remove_item_from_db(items, siz);
    }
    else if (toupper(ans[0]) == 82)
    {
        edit_db(items, *siz);
    }
    else if (toupper(ans[0]) == 71)
    {
        puts("Not yet implemented");
    }
    else if (toupper(ans[0]) == 72)
    {
        for (int i = 0; i < *siz; ++i)
        {
            list_db(&items[i], i);
        }
    }
    else if (toupper(ans[0]) == 65)
    {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s number\n", argv[0]);
    }
    else
    {
        item_t db[16];  // Array med plats för 16 varor
        int db_siz = 0; // Antalet varor i arrayen just nu

        int items = atoi(argv[1]); // Antalet varor som skall skapas

        if (items > 0 && items <= 16)
        {
            for (int i = 0; i < items; ++i)
            {
                // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
                item_t item = input_item();
                db[db_siz] = item;
                ++db_siz;
            }
        }
        else
        {
            puts("Sorry, must have [1-16] items in database.");
            return 1; // Avslutar programmet!
        }
        while (true)
        {
            event_loop(db, &db_siz);
        }
    }
    return 0;
}