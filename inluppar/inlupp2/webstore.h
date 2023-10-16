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

typedef struct warehouse ioopm_warehouse_t;

struct warehouse
{
    ioopm_hash_table_t *merch_ht;
    ioopm_hash_table_t *location_ht;
};

/// @brief Create a new warehouse
ioopm_warehouse_t *ioopm_warehouse_create();

/// @brief Add new merch
/// @param wh Hash table to insert name and shelf
/// @param inf Hash table to insert item
/// @param name Name for item
/// @param desc Description of item
/// @param price Item price
void ioopm_add_merch(ioopm_warehouse_t *wh, char *name, char *desc, int price);

/// @brief List name of merch in alphabetical order, 20 at a time.
/// @param inf Hash Table to retrieve names
void ioopm_list_merch(ioopm_warehouse_t *wh);

/// @brief Remove merch from hashtable and all its instances in warehouse hash table
/// @param wh Warehouse hash table
/// @param inf Information/item hash table
/// @param rmv_item Item to be removed
void ioopm_remove_merch(ioopm_warehouse_t *wh, char *rmv_item);

/// @brief Edit item in hash table
/// @param wh The given warehouse
/// @param inf The given hash table
/// @param old_name Name of item to be edited
/// @param new_name New name for item
/// @param desc Description of item
/// @param price Item price
void ioopm_edit_merch(ioopm_warehouse_t *wh, char *old_name, char *new_name, char *desc, int price);

/// @brief Destroys the whole warehouse
/// @param wh warehouse hash table
/// @param inf item hash table
void destroy_all(ioopm_warehouse_t *wh);

/// @brief Show stock of specified merch
/// @param merch The given merch
void ioopm_show_stock(merch_t *merch);

/// @brief Replenish merch
/// @param wh  Warehouse
/// @param merch Given merch to replenish
void ioopm_replenish(ioopm_warehouse_t *wh, merch_t *merch);

#endif