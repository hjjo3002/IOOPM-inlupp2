#include "datastruct.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct item item_t;
typedef struct shelf shelf_t;
typedef struct cart cart_t;
typedef struct database database_t;

/// @brief creates and allocates an item_t 
/// @param name item_t's name
/// @param desc item_t's description
/// @param price item_t's price
/// @return the newly created item_t
item_t *create_item(char* name, char* desc, size_t price);

/// @brief a check if an item_t is in database db
/// @param name item_t's name
/// @param db the database operated upon
/// @return T/F depending if item_t exists
bool item_exists(char* name, database_t *db);

/// @brief  creates and allocates an database_t
/// @return returns the newly created database_t
database_t *create_database();

/// @brief frees item_t and its shelf list, name and desc
/// @param item the item_t operated upon
void free_item(item_t *item);

/// @brief frees the database_t and its item_t's and hash_table_t's
/// @param db the database_t operated upon
void database_destroy(database_t *db);

/// @brief adds item_t to database_t also checks so it is a new item
/// @param item the item_t added
/// @param db the database_t item_t got added to
/// @return T/F depending if item_t successfully was added to db
bool add_merch(item_t *item, database_t *db);

/// @brief creates an sorted array with all item_t's in db and prints said array
/// @param db the database_t containing all item_t's
void list_merch(database_t *db);

/// @brief remove item_t from items ht and shelfs ht, also frees item
/// @param item item_t removed
/// @param db database_t operated upon
/// @return T/F if said item was successfully removed
bool remove_merch(item_t *item, database_t *db);

/// @brief edits and item_t with new name, desc and price
/// @param old_item the item_t which is being edited
/// @param db the database_t cointaing said item_t
/// @param new_name the new name for item_t
/// @param new_desc the new desc for item_t
/// @param new_price the new price for item_t
/// @return T/F depening on if item_t was successfully edited
bool edit_merch(item_t *old_item, database_t *db, char* new_name, char* new_desc, int new_price);

/// @brief 
/// @param place 
/// @param quantity 
/// @return 
shelf_t *create_shelf(char* place, size_t quantity);

/// @brief 
/// @param shelf 
void free_shelf(shelf_t *shelf);

/// @brief prints item_t's stock (place and quantity)
/// @param item item_t with said stock
void show_stock(item_t *item);

/// @brief 
/// @param item 
/// @param upd_shelf 
/// @param db 
/// @return 
bool replenish(item_t *item, shelf_t *upd_shelf, database_t *db);

/// @brief 
/// @param id 
/// @param db 
/// @return 
cart_t* create_cart(int id,database_t *db);

/// @brief 
/// @param cart 
void remove_cart(cart_t *cart);

/// @brief 
/// @param item 
/// @param id 
/// @param quantity 
/// @param db 
/// @return 
bool add_to_cart(item_t *item, int id, size_t quantity, database_t *db);

/// @brief 
/// @param item 
/// @param id 
/// @param quantity 
/// @param db 
/// @return 
bool remove_from_cart(item_t *item, int id, size_t quantity, database_t *db);