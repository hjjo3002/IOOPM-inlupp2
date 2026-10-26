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
/// @return pointer to item_t
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

/// @brief 
/// @param shelf 
void free_shelf(shelf_t *shelf);

/// @brief frees the database_t and its item_t's and hash_table_t's
/// @param db the database_t operated upon
void database_destroy(database_t *db);

/// @brief adds item_t to database_t also checks so it is a new item
/// @param item the item_t added
/// @param db the database_t item_t got added to
/// @return 0 if succeeded, -1 if item already exist
int add_merch(item_t *item, database_t *db);

/// @brief Prints all items in the database in alphabetical order.
/// @param db the database_t containing all item_t's
void list_merch(database_t *db);

/// @brief remove item_t from items ht and shelfs ht, also frees item
/// @param item item_t removed
/// @param db database_t operated upon
/// @return 0 if succeeded, -1 if item reserved in cart, -2 remove from shelfs failed, -3 remove from items failed
int remove_merch(item_t *item, database_t *db);

/// @brief edits and item_t with new name, desc and price
/// @param old_item the item_t which is being edited
/// @param db the database_t cointaing said item_t
/// @param new_name the new name for item_t
/// @param new_desc the new desc for item_t
/// @param new_price the new price for item_t
/// @return 0 if succeeded, -1 if new name already exist
int edit_merch(item_t *old_item, database_t *db, char* new_name, char* new_desc, int new_price);

/// @brief allocates a new shelf_t
/// @param place char* the place of shelf_t
/// @param quantity size_t amount of item_t
/// @return pointer to shelf_t
shelf_t *create_shelf(char* place, size_t quantity);

/// @brief prints item_t's stock (place and quantity)
/// @param item item_t with said stock
void show_stock(item_t *item);

/// @brief Adds or updates stock for an item at a specified shelf.
/// @param item the item on the shelf
/// @param upd_shelf the uppdated shelf (new quantity is ui's task)
/// @param db the database_t operated upon
/// @return 0 if succeeded, -1 if quantity is 0, -2 if wrong item at shelf, -3 if shelf not found or quantiity to smal
int replenish(item_t *item, shelf_t *upd_shelf, database_t *db);

/// @brief creates and allocates a new cart_t
/// @param id the id of the new shelf
/// @param db the database_t cointaining cart_t
/// @return the created cart_t
cart_t* create_cart(int id,database_t *db);

/// @brief 
/// @param id 
/// @param db 
void remove_cart(int id, database_t *db);

/// @brief adds said item with said quantity to cart_t with said id in database_t 
/// @param item the item_t added 
/// @param id int of the id of the cart_t
/// @param quantity size_t amount we added to cart_t
/// @param db the database_t operated upon
/// @return 0 if succeeded, -1 if quantity bigger than available
/// @note Reserves stock without modifying the real shelf quantity.
int add_to_cart(item_t *item, int id, size_t quantity, database_t *db);

/// @brief removes said item from cart with said id, in database_t
/// @param item item_t removed from cart_t
/// @param id int of the id of our cart_t
/// @param quantity size_t amount removed from cart_t 
/// @param db database_t operated upon
/// @return 0 if succeeded, -1 if cart not found, -2 if item not in cart, -3 if unvalid quantity
/// @note Items reserved in shopping carts cannot be removed.
///       Successfully removed items and their shelves are freed.
int remove_from_cart(item_t *item, int id, size_t quantity, database_t *db);

/// @brief calculates the price of current cart 
/// @param id int of the cart in question
/// @param db database_t operated upon
/// @return size_t of the total price
size_t calculate_cost(int id, database_t *db);

/// @brief checkouts a cart with said id, removes carts stock from db
/// @param id int of cart_t id
/// @param db database_t operated upon
/// @return 0 if succeeded, -1 if cart not found, -2 if remove from shelfs failed
/// @note Deducts purchased quantities from shelves and
///       removes and frees the cart after successful checkout.
int checkout(int id, database_t *db);

/// @brief 
/// @param name 
/// @param db 
/// @return 
item_t *find_item(char* name, database_t* db);

/// @brief 
/// @param item 
/// @param place 
/// @return 
shelf_t *find_shelf(item_t *item, char *place);

/// @brief 
/// @param id 
/// @param db 
/// @return 
cart_t *find_cart(int id, database_t *db);