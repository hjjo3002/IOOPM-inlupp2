#include "datastruct.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct item item_t;
typedef struct shelf shelf_t;
typedef struct cart cart_t;
typedef struct database database_t;

item_t *create_item(char* name, char* desc, size_t price);

bool item_exists(char* name, database_t *db);

database_t *create_database();

void free_item(item_t *item);

void database_destroy(database_t *db);

bool add_merch(item_t *item, database_t *db);

void list_merch(database_t *db);

bool remove_merch(item_t *item, database_t *db);

bool edit_merch(item_t *old_item, database_t *db, char* new_name, char* new_desc, int new_price);

void show_stock(item_t *item);


