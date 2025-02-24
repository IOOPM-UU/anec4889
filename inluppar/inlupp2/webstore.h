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
    int cart_num;
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
    ioopm_hash_table_t *cart_ht;
    size_t cart_index;
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

/// @brief Checks if cart exists in warehouse
/// @param wh The given warehouse
/// @param index Index of cart in wh
bool exists_cart(ioopm_warehouse_t *wh, int index);

/// @brief Remove merch from hashtable and all its instances in warehouse hash table
/// @param wh Warehouse hash table
/// @param inf Information/item hash table
/// @param rmv_item Item to be removed
void ioopm_remove_merch(ioopm_warehouse_t *wh, char *rmv_item);

/// @brief Create a new shelf and insert in warehouse
/// @param wh Warehouse
/// @param merch Given merch
void shelf_maker(ioopm_warehouse_t *wh, merch_t *merch);

/// @brief Retrieve the total quantity of a merch
/// @param merch The given merch
/// @return The size of merch
size_t merch_size(merch_t *merch);

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
/// @param wh Warehouse
/// @param merch Given merch to replenish
/// @param shelf_index Index of shelf
/// @param quantity Number to increase stock
void ioopm_replenish(ioopm_warehouse_t *wh, merch_t *merch, int shelf_index, int quantity);

/// @brief Print out merch of given cart
/// @param cart The given cart
void print_cart(ioopm_hash_table_t *cart);

/// @brief Creates a shopping cart
/// @param wh Warehouse
void ioopm_create_cart(ioopm_warehouse_t *wh);

/// @brief Removes a shopping cart
/// @param wh Warehouse
/// @param index Cart to be removed
void ioopm_remove_cart(ioopm_warehouse_t *wh, int index);

/// @brief Adds merch and quantity to cart
/// @param cart Specified cart
/// @param merch Given merch to add
/// @param quantity Quantity of merch
void ioopm_add_cart(ioopm_hash_table_t *cart, merch_t *merch, int quantity);

/// @brief Find the bucket where merch is placed in cart ht
/// @param cart The given cart ht
/// @param merch The given merch
/// @return Index in ht
int find_cart_index(ioopm_hash_table_t *cart, merch_t *merch);

/// @brief Removes merch and quantity from cart
/// @param cart Specified cart
/// @param merch Given merch to add
/// @param quantity Quantity of merch
void ioopm_remove_merch_cart(ioopm_hash_table_t *cart, merch_t *merch, int quantity);

/// @brief Removes merch and quantity from cart
/// @param cart Specified cart
/// @return Cost of the cart
int ioopm_calculate_cost(ioopm_hash_table_t *cart);

/// @brief Decrease stock from the warehouse and removes the cart
/// @param wh Warehouse
/// @param cart Specified cart
/// @param index Index of cart
void ioopm_checkout(ioopm_warehouse_t *wh, ioopm_hash_table_t *cart, int index);

#endif