#pragma once
#ifndef __WEBSTORE__
#define __WEBSTORE__
#include <stdbool.h>
#include <stddef.h>
#include "datastrukturer/linked_list.h"
#include "datastrukturer/hash_table.h"
#include "datastrukturer/iterator.h"
#include "utils.h"

typedef struct merch merch_t;

struct merch
{
    char *name;
    char *desc;
    int price;
    ioopm_list_t *locs;
};

typedef struct shelf shelf_t;

struct shelf
{
    char *name;
    int quantity;
};

/// @brief Add new merch
/// @param wh Hash table to insert name and shelf
/// @param inf Hash table to insert item
/// @param name Name for item
/// @param desc Description of item
/// @param price Item price
void ioopm_add_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf, char *name, char *desc, int price);

/// @brief List name of merch in alphabetical order, 20 at a time.
/// @param inf Hash Table to retrieve names
void ioopm_list_merch(ioopm_hash_table_t *inf);

/// @brief Remove merch from hashtable and all its instances in warehouse hash table
/// @param wh Warehouse hash table
/// @param inf Information/item hash table
/// @param rmv_item Item to be removed
void ioopm_remove_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf, char *rmv_item);

/// @brief Edit item in hash table
/// @param wh The given warehouse
/// @param inf The given hash table
/// @param old_name Name of item to be edited
/// @param new_name New name for item
/// @param desc Description of item
/// @param price Item price
void ioopm_edit_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf, char *old_name, char *new_name, char *desc, int price);

/// @brief Destroys the whole warehouse
/// @param wh warehouse hash table
/// @param inf item hash table
void destroy_all(ioopm_hash_table_t *wh, ioopm_hash_table_t *inf);

#endif