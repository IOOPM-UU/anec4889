#pragma once
#ifndef __HASH_TABLE__
#define __HASH_TABLE__
#include <stdbool.h>
#include "linked_list.h"
#include <stddef.h>
#include "common.h"

/**
 * @file hash_table.h
 * @author Andreas Johansson, Anton Eckervald
 * @date 20 Sep 2023
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gb54499f3b7b264e3af3b68c756090f52
 */

// Macro to be used as a constant for the amount of buckets
#define No_Buckets 17

typedef struct hash_table ioopm_hash_table_t;
typedef struct entry entry_t;

// Check if given elements hold for something
typedef bool (*ioopm_predicate)(elem_t key, elem_t value, elem_t extra);

// Apply something to given elements
typedef void (*ioopm_apply_function)(elem_t key, elem_t *value, elem_t extra);

// Hash the key
typedef int (*ioopm_hash_fun)(elem_t key);

struct entry
{
    elem_t key;    // holds the key
    elem_t value;  // holds the value
    entry_t *next; // points to the next entry (possibly NULL)
};

// Hash table struct
struct hash_table
{
    entry_t buckets[17];
    ioopm_hash_fun hash_function;
    ioopm_eq_function value_eq_fun;
    ioopm_eq_function key_eq_fun;
};

// Define an option structure that represents a result with success status and an associated value.
typedef struct option option_t;
struct option
{
    bool success; // Indicates whether the operation was successful (true) or not (false).
    elem_t value; // A pointer to the associated value, if applicable (NULL if not successful).
};

/**
 * @brief Macro to create a successful option value with an associated value.
 * @param v The associated value.
 * @return An option_t structure representing a successful result with the provided value.
 */
#define Success(v) (option_t){.success = true, .value = v};

/**
 * @brief Macro to create an unsuccessful option value.
 * @return An option_t structure representing an unsuccessful result.
 */
#define Failure() (option_t){.success = false};

/**
 * @brief Macro to check if an option value is successful.
 * @param o The option value to check.
 * @return true if the option value represents success, false otherwise.
 */
#define Successful(o) (o.success == true)

/**
 * @brief Macro to check if an option value is unsuccessful.
 * @param o The option value to check.
 * @return true if the option value represents failure, false otherwise.
 */
#define Unsuccessful(o) (o.success == false)

/// @brief Create a new hash table
/// @param hash The given hash function
/// @param key_eq The function to compare keys
/// @param value_eq The function to compare values
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_fun hash, ioopm_eq_function key_eq, ioopm_eq_function value_eq);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return the value mapped to by key
option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key
option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

/// @brief returns the number of key => value entries in the hash table
/// @param ht hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param ht hash table operated upon
/// @return true if size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param ht hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param ht hash table operated upon
/// @return a list of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys). NULL-terminated
/// @param ht hash table operated upon
/// @return a list of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param ht hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param ht hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate P, elem_t arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param ht hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate P, elem_t arg);

/// @brief apply a function to all entries in a hash table
/// @param ht hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, elem_t arg);

#endif