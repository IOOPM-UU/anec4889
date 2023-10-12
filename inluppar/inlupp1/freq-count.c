#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stddef.h>
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"
static int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

bool value_eq(elem_t a, elem_t b)
{
    return a.i == b.i;
}

bool key_eq(elem_t a, elem_t b)
{
    return strcmp(a.p, b.p) == 0;
}

void process_word(char *word, ioopm_hash_table_t *ht, ioopm_list_t *strdup_list)
{
    int freq = 0;
    if (ioopm_hash_table_has_key(ht, (elem_t){.p = word}))
    {
        freq = (ioopm_hash_table_lookup(ht, (elem_t){.p = word})).value.i;
        ioopm_hash_table_insert(ht, (elem_t){.p = word}, (elem_t){.i = freq + 1});
    }
    else
    {
        char *tmp = strdup(word);
        ioopm_linked_list_append(strdup_list, (elem_t){.p = tmp});
        ioopm_hash_table_insert(ht, (elem_t){.p = tmp}, (elem_t){.i = freq + 1});
    }
}

void process_file(char *filename, ioopm_hash_table_t *ht, ioopm_list_t *strdup_list)
{
    FILE *f = fopen(filename, "r");
    while (true)
    {
        char *buf = NULL;
        size_t len = 0;
        getline(&buf, &len, f);

        if (feof(f))
        {
            free(buf);
            break;
        }
        for (char *word = strtok(buf, Delimiters); word && *word; word = strtok(NULL, Delimiters))
        {
            process_word(word, ht, strdup_list);
        }
        free(buf);
    }
    fclose(f);
}

int string_sum_hash(elem_t e)
{
    char *str = e.p;
    int result = 0;
    do
    {
        result += *str;
    } while (*++str != '\0');
    return result % No_Buckets;
}

// static void apply_free_all(elem_t key, elem_t *value, elem_t extra)
// {
//     free(key.p);
// }

int main(int argc, char *argv[])
{
    ioopm_list_t *strdup_list = ioopm_linked_list_create(key_eq);
    ioopm_hash_table_t *ht = ioopm_hash_table_create((ioopm_hash_fun)string_sum_hash, key_eq, value_eq);
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            process_file(argv[i], ht, strdup_list);
        }
        ioopm_list_t *list = ioopm_hash_table_keys(ht);
        ioopm_list_iterator_t *itr = ioopm_list_iterator(list);
        int size = ioopm_hash_table_size(ht);
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
        printf("%d\n", size);
        int result = 0;
        for (int i = 0; i < size; ++i)
        {
            int freq = (ioopm_hash_table_lookup(ht, (elem_t){.p = keys_arr[i]})).value.i;
            printf("%s: %d\n", keys_arr[i], freq);
            result += freq;
        }
        printf("%d\n", result);
    }
    else
    {
        puts("Usage: freq-count file1 ... filen");
    }
    ioopm_list_iterator_t *strdup_iter = ioopm_list_iterator(strdup_list);
    if (!ioopm_linked_list_is_empty(strdup_list))
    {
        do
        {
            free(ioopm_iterator_current(strdup_iter).p);
            ioopm_iterator_next(strdup_iter);
        } while (ioopm_iterator_has_next(strdup_iter));
        free(ioopm_iterator_current(strdup_iter).p);
    }

    ioopm_iterator_destroy(strdup_iter);
    ioopm_linked_list_destroy(strdup_list);
    ioopm_hash_table_destroy(ht);
    return 0;
}