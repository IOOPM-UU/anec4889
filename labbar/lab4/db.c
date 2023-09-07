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
  int r = random() % 3;
  int buf_i = 0;
  char *src = arr1[r];
  for (int i = 0; i < strlen(src); i++)
  {
    buf[buf_i] = src[i];
    buf_i++;
  }
  buf[buf_i] = '-';
  buf_i++;

  r = random() % 3;
  src = arr2[r];
  for (int i = 0; i < strlen(src); i++)
  {
    buf[buf_i] = src[i];
    buf_i++;
  }

  buf[buf_i] = ' ';
  buf_i++;

  r = random() % 3;
  src = arr3[r];
  for (int i = 0; i < strlen(src); i++)
  {
    buf[buf_i] = src[i];
    buf_i++;
  }

  buf[buf_i] = '\0';
  buf_i++;

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

int main(int argc, char *argv[])
{
  srandom(time(NULL));
  char *array1[] = {"Laser", "Polka", "Extra"};
  char *array2[] = {"förnicklad", "smakande", "ordinär"};
  char *array3[] = {"skruvdragare", "kola", "uppgift"};
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

    for (int i = db_siz; i < 16; ++i)
    {
      char *name = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
      char *desc = magick(array1, array2, array3, 3); // TODO: Lägg till storlek
      int price = random() % 200000;
      char shelf[] = {random() % ('Z' - 'A') + 'A',
                      random() % 10 + '0',
                      random() % 10 + '0',
                      '\0'};
      item_t item = make_item(name, desc, price, strdup(shelf));

      db[db_siz] = item;
      ++db_siz;
    }

    // Skriv ut innehållet
    for (int i = 0; i < db_siz; ++i)
    {
      list_db(&db[i], i);
    }
    edit_db(db, db_siz);
  }
  return 0;
}