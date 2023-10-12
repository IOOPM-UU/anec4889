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
#include "common.h"

int default_hash(elem_t key)
{
    return key.i % No_Buckets;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_fun hash, ioopm_eq_function key_eq, ioopm_eq_function value_eq)
{
    /// Allocate space for a ioopm_hash_table_t = 17 pointers to
    /// entry_t's, which will be set to NULL
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    if (hash == NULL)
    {
        hash = default_hash;
    }
    // Initiate values of hash table
    result->hash_function = hash;
    result->key_eq_fun = key_eq;
    result->value_eq_fun = value_eq;
    return result;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    ioopm_hash_table_clear(ht);
    free(ht);
}

static void entry_destroy(entry_t *bucket)
{
    entry_t *current = bucket;
    entry_t *to_clear = current->next;
    while (to_clear != NULL)
    {
        entry_t *next_entry = to_clear->next;
        free(to_clear);
        to_clear = next_entry;
    }
}

entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *bucket, elem_t key)
{
    entry_t *current = bucket;
    while (current->next != NULL && !ht->key_eq_fun(current->next->key, key))
    {
        current = current->next;
    }

    return current;
}

entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
    entry_t *new_entry = calloc(1, sizeof(entry_t));
    // Initiate values for new entry
    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = next;
    return new_entry;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
    /// Search for an existing entry for a key
    entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[ht->hash_function(key)], key);
    entry_t *next = entry->next;

    /// Check if the next entry should be updated or not
    if (next != NULL && ht->key_eq_fun(next->key, key))
    {
        next->value = value;
    }
    else
    {
        entry->next = entry_create(key, value, next);
    }
}

option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
    /// Find the previous entry for key
    entry_t *tmp = find_previous_entry_for_key(ht, &ht->buckets[ht->hash_function(key)], key);
    entry_t *next = tmp->next;

    // Check if it was found or not
    if (next && ht->key_eq_fun(next->key, key))
    {
        return Success(next->value);
    }
    else
    {
        return Failure();
    }
}

option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
    entry_t *prev_entry = find_previous_entry_for_key(ht, &ht->buckets[ht->hash_function(key)], key);

    if (prev_entry->next == NULL)
    {
        return Failure();
    }
    else
    {
        entry_t *to_remove = prev_entry->next;
        elem_t result = to_remove->value;
        entry_t *tmp = to_remove;
        prev_entry->next = tmp->next;
        free(tmp);
        return Success(result);
    }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
    int counter = 0;
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            current_bucket = current_bucket->next;
            counter++;
        }
    }
    return counter;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        if (ht->buckets[i].next != NULL)
        {
            return false;
        }
    }
    return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_destroy(&ht->buckets[i]);
        ht->buckets[i].next = NULL;
    }
}

// Equivalent function for keys in linked list
static bool list_key_eq(elem_t a, elem_t b)
{
    return a.i == b.i;
}

// Equivalent function for values in linked list
static bool list_value_eq(elem_t a, elem_t b)
{
    return strcmp(a.p, b.p) == 0;
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
    // Create an empty list
    ioopm_list_t *list_of_keys = ioopm_linked_list_create(list_key_eq);

    // Iterate over hash table and append keys in list
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            ioopm_linked_list_append(list_of_keys, current_bucket->key);
            current_bucket = current_bucket->next;
        }
    }
    return list_of_keys;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
    // Create an empty list
    ioopm_list_t *list_of_values = ioopm_linked_list_create(list_value_eq);

    // Iterate over hash table and append keys in list
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            ioopm_linked_list_append(list_of_values, current_bucket->value);
            current_bucket = current_bucket->next;
        }
    }
    return list_of_values;
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            // Check if keys are equal using given eq function
            if (ht->key_eq_fun(current_bucket->key, key))
            {
                return true;
            }
            current_bucket = current_bucket->next;
        }
    }

    return false;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            // Check if keys are equal using given eq function
            if (ht->value_eq_fun(current_bucket->value, value))
            {
                return true;
            }
            current_bucket = current_bucket->next;
        }
    }
    return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate P, elem_t x)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            if (!P(current_bucket->key, current_bucket->value, x))
            {
                return false;
            }
            current_bucket = current_bucket->next;
        }
    }
    return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate P, elem_t x)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            if (P(current_bucket->key, current_bucket->value, x))
            {
                return true;
            }
            current_bucket = current_bucket->next;
        }
    }
    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function F, elem_t x)
{
    for (size_t i = 0; i < No_Buckets; i++)
    {
        entry_t *current_bucket = ht->buckets[i].next;
        while (current_bucket != NULL)
        {
            F(current_bucket->key, &(current_bucket->value), x);
            current_bucket = current_bucket->next;
        }
    }
}