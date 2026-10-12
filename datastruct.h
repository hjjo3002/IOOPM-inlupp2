#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
* @file datastruct.h
* @author Hjalmar Johansson
* @date 09/10/2026
* @brief Hash table and linked list with their respective iterators from IOOPM inlupp 1
*
*/



#define INT_AS_ELEM(v)   ((value_t) { .i = (v) })
#define BOOL_AS_ELEM(v)  ((value_t) { .b = (v) })
#define FLOAT_AS_ELEM(v) ((value_t) { .f = (v) })
#define PTR_AS_ELEM(v)   ((value_t) { .p = (v) })

typedef union value value_t;
union value
{
    int i;
    bool b;
    float f;
    void *p;
};


//--from hash_table.h--

typedef struct hash_table ioopm_hash_table_t;
typedef bool ioopm_eq_function(const value_t a, const value_t b);
typedef size_t ioopm_hash_function(const value_t key);

/// @brief hashes keys as strings
/// @return hashed unsinged interger
size_t str_hash(const value_t key);


/// @brief compares keys as strings
/// @return true if the keys match as strings
bool str_comp(const value_t a, const value_t b);

//NOTE: Might have be better add create function for each of the diffrent types of key instead of 
// adding public hash and compare functions

/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *eq_fn);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, value_t key, value_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @param result ptr to value of the key to lookup, unchaged if failed
/// @return True if the key was found
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const value_t key, value_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @param result ptr to value of the removed key, unchaged if failed
/// @return True if the key was removed
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, const value_t key, value_t *result);

/// @brief Returns if a given key is in the hash table
/// @param ht hash table we check
/// @param key key to look up
/// @return true if key is present else false
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, const value_t key);

/// @brief Checks if hash table is empty
/// @param ht hash table we check
/// @return true if hash table is empty
bool ioopm_hash_table_is_empty(const ioopm_hash_table_t *ht);

/// @brief Returns size of given hash table
/// @param ht hash table we check
/// @return Size of the hash table
size_t ioopm_hash_table_size(const ioopm_hash_table_t *ht);

//--end of hash_table.h--

//--from hash_table_iterator.h--


typedef struct hash_table_iterator ioopm_hash_table_iterator_t;

/// @brief Create an iterator for a hash table.
/// @param ht hash table to iterate over
/// @return a new iterator positioned at the first entry if it exists, and positioned at-the-end if ht is empty
ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht);

/// @brief Destroy an iterator and free its memory
/// @param it iterator to destroy
void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it);

/// @brief Check if the current element exists, or equivalently, if it is positioned at an entry
/// @param it iterator operated upon
/// @return true iff the current element exists
bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it);

/// @brief Move the iterator forward to the next entry if it exists or to at-the-end if no more entries exist
/// @pre it is positioned at an entry
/// @param it iterator operated upon
void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it);

/// @brief Get the key of the current entry
/// @pre it is positioned at an entry
/// @param it iterator operated upon
/// @return the key of the current entry
value_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it);

/// @brief Get the value of the current entry
/// @pre it is positioned at an entry
/// @param it iterator operated upon
/// @return the value if the current entry
value_t ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it);

//--end of hash_table_iterator.h--

//--from linked_list.h--


typedef union value value_t;

typedef struct list ioopm_list_t; 

/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_list_create(void);

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_list_append(ioopm_list_t *list, value_t value);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void ioopm_list_prepend(ioopm_list_t *list, value_t value);

/// @brief Return the first element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the head of
value_t ioopm_list_head(ioopm_list_t *list);

/// @brief Return the last element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the last element of
value_t ioopm_list_last(ioopm_list_t *list);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @pre 0 <= index <= length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
void ioopm_list_insert(ioopm_list_t *list, size_t index, value_t value);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
value_t ioopm_list_remove(ioopm_list_t *list, size_t index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
value_t ioopm_list_get(ioopm_list_t *list, size_t index);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
size_t ioopm_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_list_is_empty(ioopm_list_t *list);

//--end of linked_list.h--

//--from linked_list_iterator.h--



typedef struct list_iterator ioopm_list_iterator_t;

/// @brief Create a new iterator
/// @param l the list to iterate over
ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter);

/// @brief Checks if there are more elements to iterate over
/// @param iter the iterator
/// @return true if there is at least one more element
bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @return the current element
value_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Remove the current element from the underlying list
/// @param iter the iterator
/// @return the removed element
value_t ioopm_list_iterator_remove(ioopm_list_iterator_t *iter);


/// @brief Insert a new element into the underlying list making the current element it's next
/// @param iter the iterator
/// @param element the element to be inserted
void ioopm_list_iterator_insert(ioopm_list_iterator_t *iter, value_t value);

//--end of linked_list_iterator.h--

