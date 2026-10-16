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
    size_t quantity;
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

void free_item(item_t *item){
   ioopm_list_iterator_t *it = ioopm_list_iterator_create(item->shelfs);

    while (!ioopm_list_iterator_at_end(it))
    {
        value_t current = ioopm_list_iterator_current(it);
        shelf_t *shelf = current.p;

        free(shelf->place);
        free(shelf);

        ioopm_list_iterator_advance(it);
    }

    ioopm_list_iterator_destroy(it);
    ioopm_list_destroy(item->shelfs);

    free(item->name);
    free(item->desc);
    free(item);
}

void database_destroy(database_t *db){
    //frigör item_t's och resp lagerplatser
    ioopm_hash_table_t *items = db->items;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(items);

    while(!ioopm_hash_table_iterator_at_end(it)){
        value_t current = ioopm_hash_table_iterator_current_value(it);
        item_t *item = current.p;

        free_item(item);

        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    ioopm_hash_table_destroy(db->items);
    ioopm_hash_table_destroy(db->shelfs);
    ioopm_hash_table_destroy(db->carts);

    //frigör db
    free(db);
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
//helper for list merch
static char** hashkey_to_array(ioopm_hash_table_t *ht){
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
    size_t count = ioopm_hash_table_size(ht);

    char** arr = calloc(count,sizeof(char*));

    for(size_t i = 0; !ioopm_hash_table_iterator_at_end(it); i++){

        value_t key = ioopm_hash_table_iterator_current_key(it);
        arr[i] = key.p;

        ioopm_hash_table_iterator_advance(it);
    }
    qsort(arr,count,sizeof(const char*),compare_str_alphabetical);

    ioopm_hash_table_iterator_destroy(it);
    return arr;
}

static item_t *edit_item(item_t *item, char* new_name, char* new_desc, int new_price){
    item->name = new_name;
    item->desc = new_desc;
    item->price = new_price;

    return item;
}

static void edit_ht_items(item_t *item, database_t *db, char* new_name, char* new_desc, int new_price){
    value_t result; 
    ioopm_hash_table_t *items = db->items;

    ioopm_hash_table_remove(items,PTR_AS_ELEM(item->name),&result);

    free(item->name);
    free(item->desc);


    edit_item(item,new_name,new_desc,new_price);

    
    ioopm_hash_table_insert(items,PTR_AS_ELEM(item->name),PTR_AS_ELEM(item)); 
}

static size_t linked_list_size(ioopm_list_t *ls){
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(ls);
    size_t count = 0;
    while(!ioopm_list_iterator_at_end(it)){
        ioopm_list_iterator_advance(it);
        count++;
    }
    return count;
}
//used in linked_list_sorted
static int compare_shelves(const void *a, const void *b)
{
    const shelf_t *shelf1 = *(const shelf_t *const *)a;
    const shelf_t *shelf2 = *(const shelf_t *const *)b;

    return strcmp(shelf1->place, shelf2->place);
}
//helper for show stock
static shelf_t** linked_list_sorted(ioopm_list_t *l){
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(l);
    ioopm_list_iterator_t *it_count = ioopm_list_iterator_create(l);
    size_t count = linked_list_size(l);
    
    shelf_t** arr = calloc(count,sizeof(shelf_t*));

    for(size_t i = 0; !ioopm_list_iterator_at_end(it); i++){

        value_t current = ioopm_list_iterator_current(it);
        shelf_t *shelf = current.p;

        arr[i] = shelf;


        ioopm_list_iterator_advance(it);
    }
    qsort(arr,count,sizeof(const shelf_t*),compare_shelves);

    ioopm_list_iterator_destroy(it);
    ioopm_list_iterator_destroy(it_count);

    return arr;
}



bool add_merch(item_t *item, database_t *db){
   ioopm_hash_table_t *items=db->items;
   char* name_check = item->name;

   if(ioopm_hash_table_has_key(items,PTR_AS_ELEM(name_check))){
    return false;
   }

   else{
    ioopm_hash_table_insert(db->items,PTR_AS_ELEM(item->name),PTR_AS_ELEM(item));
    return true;
   }
}

void list_merch(database_t *db){
    //TODO prints (alphabetical order) items in ht items (not 20 at a time, ui's task)
    ioopm_hash_table_t *ht = db->items;
    char** arr = hashkey_to_array(ht); 
    size_t count=ioopm_hash_table_size(ht);

    for(size_t i=0;i!=count;i++){
        printf("%zu. %s\n",i+1,arr[i]);
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

bool edit_merch(item_t *old_item, database_t *db, char* new_name, char* new_desc, int new_price){
    //TODO edit in items, if edit name -> change key in shelfs.
    ioopm_hash_table_t *items = db->items;

    if(ioopm_hash_table_has_key(items,PTR_AS_ELEM(new_name))){
        return false;
    }
    else{
        edit_ht_items(old_item, db, new_name, new_desc, new_price);
        
        return true;
    }
}

void show_stock(item_t *item){
    ioopm_list_t *shelfs = item->shelfs;
    size_t length = linked_list_size(shelfs);
    
    if(length==0){
        printf("Empty stock");
        return;
    }
    shelf_t** arr = linked_list_sorted(shelfs);

    for(size_t i=0; i<length; i++){
        printf("%s, %zu st",arr[i]->place, arr[i]->quantity);
    }
    free(arr);
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

