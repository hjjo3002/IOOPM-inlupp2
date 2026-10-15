#include "logic.h"

/*
    logic: db->items => key=name, value=item_t
           db->shelfs => key=place, value=shelf_t
           db->cart => key=id, value=cart_t
*/



struct item
{
    char* name;
    char* desc;
    size_t price;
    ioopm_list_t *shelfs;
};

struct shelf
{
    char* place;
    //item_t* item;
    size_t quatity;
};

struct cart
{
    int id;
    ioopm_hash_table_t *items;
};

struct database
{
    ioopm_hash_table_t *items;
    ioopm_hash_table_t *shelfs;
    ioopm_hash_table_t *carts;
};

item_t *create_item(char* name, char* desc, size_t price){
    item_t *item = calloc(1,sizeof(item_t));
    ioopm_list_t *shelfs_for_item = ioopm_list_create(); //use this for gdb segfault 
    item->name = name;
    item->desc = desc;
    item->price = price;
    item->shelfs = shelfs_for_item;

    return item;
}

bool item_exists(char* name, database_t *db){
    ioopm_hash_table_t *item_ht = db->items; 
    value_t key = PTR_AS_ELEM(name); 
    value_t result;

    return ioopm_hash_table_lookup(item_ht, key, &result);
}

database_t *create_database(){
    database_t *db = calloc(1,sizeof(database_t));

    db->items=ioopm_hash_table_create(str_hash,str_comp);
    db->shelfs=ioopm_hash_table_create(str_hash,str_comp);
    db->carts=ioopm_hash_table_create(str_hash,str_comp);

    return db;
}


//helper for remove merch, does not free
static bool remove_from_items(item_t *item, database_t *db){
    value_t result;
    ioopm_hash_table_t *items = db->items;   
    return ioopm_hash_table_remove(items,PTR_AS_ELEM(item->name),&result);
}
//helper for remove merch, frees from list shelfs. 
static bool remove_from_shelfs(item_t *item, database_t *db){
    ioopm_list_t *place_in_shelf=item->shelfs; //linked list for all places of item 
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(place_in_shelf);
    bool success = true;

    while(!ioopm_list_iterator_at_end(it)){
        value_t result;
        value_t current = ioopm_list_iterator_current(it);
        shelf_t *shelf = current.p;
        
        if(!ioopm_hash_table_remove(db->shelfs,PTR_AS_ELEM(shelf->place),&result)){
            success = false;
        }

        free(shelf->place); 
        free(shelf);
        ioopm_list_iterator_advance(it);
        ioopm_list_destroy(place_in_shelf);
    }
    ioopm_list_iterator_destroy(it);
    return success;
} 
//helper free item
static void free_item(item_t *item){
    //free(item->name);
    //free(item->desc);
    free(item);
}


static item_t *edit_item(item_t *item, char* new_name, char* new_desc, int new_price){
    item->name = new_name;
    item->desc = new_desc;
    item->price = new_price;

    return item;
}

static void edit_ht_items(item_t *item, database_t *db, char* new_name, char* new_desc, int new_price){
    //TODO check if new item not in items and not to change to alreday existing item, (make sure remove is true)
    value_t result; 
    ioopm_hash_table_t *items = db->items;
    ioopm_hash_table_remove(items,PTR_AS_ELEM(item->name),&result);

    edit_item(item,new_name,new_desc,new_price);
    ioopm_hash_table_insert(items,PTR_AS_ELEM(item->name),PTR_AS_ELEM(item)); 
}

static void edit_shelfs(){

}

void add_merch(item_t *item, database_t *db){
    //TODO check merch must be new
    //add to items
   ioopm_hash_table_insert(db->items,PTR_AS_ELEM(item->name),PTR_AS_ELEM(item));
}

void list_merch(ioopm_hash_table_t *ht){
    //TODO prints (alphabetical order) items in ht items (not 20 at a time, ui's task)
    char** arr = hashkey_to_array(ht); 
    size_t count=ioopm_hash_table_size(ht);

    for(size_t i=0;i!=count;i++){
        printf("%zu. %s\n",i,arr[i]);
    }
    free(arr);
}

bool remove_merch(item_t *item, database_t *db){
    if(remove_from_items(item,db)){

        if(remove_from_shelfs(item,db)){

            free_item(item);
            return true;
        }
        else{return false;}
    }
    else{return false;}
    //TODO free mem
}

void edit_merch(item_t *old_item, database_t *db, char* new_name, char* new_desc, int new_price){
    //TODO edit in items, if edit name -> change key in shelfs. 
    edit_ht_items(old_item, db, new_name, new_desc, new_price);

    edit_shelfs();

}

void show_stock(){
    //TODO
    //puts("hej");
}

void create_cart(){
    //TODO
}

void remove_cart(){
    //TODO
}

void add_to_cart(){
    //TODO
}

void remove_from_cart(){
    //TODO
}

void calculate_cost(){
    //TODO
}

void checkout(){
    //TODO
}

