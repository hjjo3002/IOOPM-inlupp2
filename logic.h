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

void add_merch(item_t *item, database_t *db);

void list_merch(ioopm_hash_table_t *ht);

bool remove_merch(item_t *item, database_t *db);



