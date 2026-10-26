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
    ioopm_hash_table_t *items; //conatins key ptr to item_t, value = quantity
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
    db->carts=ioopm_hash_table_create(int_hash,int_comp);

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

void free_shelf(shelf_t *shelf){
    free(shelf->place);
    free(shelf);
}

void free_cart(cart_t *cart){
    ioopm_hash_table_destroy(cart->items);
    free(cart);
}

void database_destroy(database_t *db){
    //frigör item_t's och resp lagerplatser
    
    ioopm_hash_table_t *carts = db->carts;
    ioopm_hash_table_iterator_t *it_carts = ioopm_hash_table_iterator_create(carts);
    
    while(!ioopm_hash_table_iterator_at_end(it_carts)){
        value_t current = ioopm_hash_table_iterator_current_value(it_carts);
        cart_t *cart = current.p;
    
        free_cart(cart);
    
        ioopm_hash_table_iterator_advance(it_carts);
    }
    ioopm_hash_table_iterator_destroy(it_carts);
    
        
    ioopm_hash_table_t *items = db->items;
    ioopm_hash_table_iterator_t *it_items = ioopm_hash_table_iterator_create(items);

    while(!ioopm_hash_table_iterator_at_end(it_items)){
        value_t current = ioopm_hash_table_iterator_current_value(it_items);
        item_t *item = current.p;

        free_item(item);

        ioopm_hash_table_iterator_advance(it_items);
    }
    ioopm_hash_table_iterator_destroy(it_items);

    ioopm_hash_table_destroy(db->items);
    ioopm_hash_table_destroy(db->shelfs);
    ioopm_hash_table_destroy(db->carts);

    //frigör db
    free(db);
}

shelf_t *create_shelf(char* place, size_t quantity){
    shelf_t *shelf = calloc(1,sizeof(shelf_t));
    shelf->place = place;
    shelf->quantity = quantity;
    return shelf;
}




//helper for remove merch, does not free
static bool remove_from_items(item_t *item, database_t *db){
    value_t result;
    ioopm_hash_table_t *items = db->items;   
    return ioopm_hash_table_remove(items,PTR_AS_ELEM(item->name),&result);
}
//used in remove from shelfs
static void free_items_shelf(item_t *item){
    ioopm_list_t *list = item->shelfs;

    while(!ioopm_list_is_empty(list))
    {
        value_t current_value = ioopm_list_remove(list,0);
        shelf_t *shelf = current_value.p;
        free_shelf(shelf);
    }
}
//helper for remove merch, frees from list shelfs. 
static bool remove_from_shelfs(item_t *item, database_t *db){
    ioopm_list_t *place_in_shelf=item->shelfs; //linked list for all places of item 
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(place_in_shelf);

    bool success = true;

    if(ioopm_list_is_empty(place_in_shelf)){ //if no shelfs? 
        ioopm_list_iterator_destroy(it);
        return true;
    }

    while(!ioopm_list_iterator_at_end(it)){
        value_t result;
        value_t current = ioopm_list_iterator_current(it);
        shelf_t *shelf = current.p;
        
        if(!ioopm_hash_table_remove(db->shelfs,PTR_AS_ELEM(shelf->place),&result)){
            success = false;
        }

        ioopm_list_iterator_advance(it); 
    }
    
    ioopm_list_iterator_destroy(it);
    free_items_shelf(item);
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
//helper for edit merch
static item_t *edit_item(item_t *item, char* new_name, char* new_desc, int new_price){
    item->name = new_name;
    item->desc = new_desc;
    item->price = new_price;

    return item;
}
//helper for edit merch
static void edit_ht_items(item_t *item, database_t *db, char* new_name, char* new_desc, int new_price){
    value_t result; 
    ioopm_hash_table_t *items = db->items;

    ioopm_hash_table_remove(items,PTR_AS_ELEM(item->name),&result);

    free(item->name);
    free(item->desc);


    edit_item(item,new_name,new_desc,new_price);

    
    ioopm_hash_table_insert(items,PTR_AS_ELEM(item->name),PTR_AS_ELEM(item)); 
}
//helper for show stock
static size_t linked_list_size(ioopm_list_t *ls){
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(ls);
    size_t count = 0;
    while(!ioopm_list_iterator_at_end(it)){
        ioopm_list_iterator_advance(it);
        count++;
    }
    ioopm_list_iterator_destroy(it);
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
//helper for add to cart
static size_t get_total_stock_of_db(item_t *item){
    //iterate thru item->shelfs and add up quantity
    size_t total_quantity = 0;

    if(item == NULL || item->shelfs == NULL){
        return total_quantity;
    }

    ioopm_list_t *shelf_list = item->shelfs;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(shelf_list);

    while(!ioopm_list_iterator_at_end(it)){
        value_t current_value = ioopm_list_iterator_current(it);
        shelf_t *current_shelf = current_value.p;
        total_quantity = total_quantity + current_shelf->quantity;

        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
    return total_quantity;
}
//helper for add to cart
static size_t get_reserved_stock(item_t *item, database_t *db){
    size_t reserved_stock = 0;
    ioopm_hash_table_t *carts = db->carts;

    ioopm_hash_table_iterator_t *it_carts = ioopm_hash_table_iterator_create(carts);

    while(!ioopm_hash_table_iterator_at_end(it_carts)){
        value_t current_value = ioopm_hash_table_iterator_current_value(it_carts);

        cart_t *current_cart = current_value.p;
        value_t quantity;

        if(ioopm_hash_table_lookup(current_cart->items,PTR_AS_ELEM(item),&quantity)){
            reserved_stock =reserved_stock + quantity.s;
        }

        ioopm_hash_table_iterator_advance(it_carts);
    }

    ioopm_hash_table_iterator_destroy(it_carts);

    return reserved_stock;
}
//helper for checkout
static bool remove_quantity_from_shelfs(item_t *item, size_t quantity, int id, database_t *db){
    //go in to item linked list, look at fist elem, 
    ioopm_list_t *item_list = item->shelfs;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(item_list);
    size_t stock_left = quantity; 

    while (stock_left != 0)
    {
        if(ioopm_list_iterator_at_end(it)){
            ioopm_list_iterator_destroy(it);
            return false; //should not be reachable due to add_to_cart implementaton
        }

        value_t current_item = ioopm_list_iterator_current(it);
        shelf_t *current_shelf = current_item.p;
        size_t shelf_quantity = current_shelf->quantity;
        value_t result;
        
        if(shelf_quantity>stock_left){
            //decrease the stock in linked list. (db->shelfs we dont have to touch)
            current_shelf->quantity =current_shelf->quantity - stock_left;
            stock_left=0;
        }

        else { //shelf_quantity=<quantity
            //remove first instanse of item_t, remove from both, upd stock_left
            ioopm_hash_table_remove(db->shelfs,PTR_AS_ELEM(current_shelf->place),&result);
            ioopm_list_iterator_remove(it);
            stock_left=stock_left-shelf_quantity;
            //free???????? remove from db->cart
            ioopm_hash_table_remove(db->carts,INT_AS_ELEM(id),&result);
            free_shelf(current_shelf);
        }
    }
    ioopm_list_iterator_destroy(it);
    return true;
}

//main functions:

int add_merch(item_t *item, database_t *db){
   ioopm_hash_table_t *items=db->items;
   char* name_check = item->name;

   if(ioopm_hash_table_has_key(items,PTR_AS_ELEM(name_check))){
    free_item(item);
    return -1; //merch with name already exists
   }

   else{
    ioopm_hash_table_insert(db->items,PTR_AS_ELEM(item->name),PTR_AS_ELEM(item));
    return 0; //success
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

int remove_merch(item_t *item, database_t *db){
    if(get_reserved_stock(item,db)>0){ //check for no carts have item in cart
        return -1; //carts with item resereved
    }

    if(remove_from_items(item,db)){

        if(remove_from_shelfs(item,db)){

            free_item(item);
            return 0;
        }
        else return -2; //remove from shelfs failed
    }
    else return -3; //remove from items failed
}

int edit_merch(item_t *old_item, database_t *db, char* new_name, char* new_desc, int new_price){
    //TODO edit in items, if edit name -> change key in shelfs.
    ioopm_hash_table_t *items = db->items;

    if(ioopm_hash_table_has_key(items,PTR_AS_ELEM(new_name))){
        free(new_name);
        free(new_desc);
        return -1; //new name already exists
    }
    else{
        edit_ht_items(old_item, db, new_name, new_desc, new_price);
        
        return 0; //success
    }
}

void show_stock(item_t *item){
    ioopm_list_t *shelfs = item->shelfs;
    size_t length = linked_list_size(shelfs);
    char* name = item->name;
    
    if(length==0){
        printf("Empty stock\n");
        return;
    }
    shelf_t** arr = linked_list_sorted(shelfs);
    printf("%s's stock:\n",name);

    for(size_t i=0; i<length; i++){
        printf("%s: %zu st\n",arr[i]->place, arr[i]->quantity);
    }
    free(arr);
}

int replenish(item_t *item, shelf_t *upd_shelf, database_t *db){
    char *place = upd_shelf->place;
    value_t result;

    if (upd_shelf->quantity == 0){
        return -1; //quantity of new shelf is 0
    }

    bool found = ioopm_hash_table_lookup(db->shelfs,PTR_AS_ELEM(place), &result);

    if (found){
        item_t *found_item = result.p;

        if (found_item != item){
            free_shelf(upd_shelf);
            return -2; //another item at upd_shelfs place
        }
        shelf_t *existing = find_shelf(item, place);

        if (existing == NULL || upd_shelf->quantity <= existing->quantity){
            return -3; //shelf not found or quantity less than existing stock
        }

        existing->quantity = upd_shelf->quantity;
        free_shelf(upd_shelf);
        return 0; //success
    }

    // Ny lagerplats
    ioopm_list_append(item->shelfs, PTR_AS_ELEM(upd_shelf));

    ioopm_hash_table_insert(db->shelfs,PTR_AS_ELEM(upd_shelf->place),PTR_AS_ELEM(item));

    return 0; //success
}

cart_t* create_cart(int id, database_t *db){
    cart_t *cart = calloc(1,sizeof(cart_t));
    cart->id = id;
    cart->items = ioopm_hash_table_create(ptr_hash,ptr_comp);
    ioopm_hash_table_insert(db->carts,INT_AS_ELEM(id),PTR_AS_ELEM(cart));
    return cart;
}

void remove_cart(int id, database_t *db){
    cart_t *cart = find_cart(id,db);
    free_cart(cart);
}
//notes: checkout handles removing actual quantity from warehouse
int add_to_cart(item_t *item, int id, size_t quantity, database_t *db){
    //TODO check stock
    cart_t *cart = find_cart(id,db);
    ioopm_hash_table_t* cart_ht = cart->items;
    
    //check if total stock is enought (have to go thru all carts too to check their carts to get available.)
    size_t total_stock = get_total_stock_of_db(item);
    //reserved stock
    size_t reserved_stock = get_reserved_stock(item,db);
    size_t stock_available = total_stock-reserved_stock;

    if(stock_available < quantity){
        return -1; //quantity is bigger than available
    }
    //check if item already in cart_ht
    value_t result;
    if(ioopm_hash_table_lookup(cart_ht,PTR_AS_ELEM(item),&result)){
        quantity=quantity+result.s;
    }

    //add to cart_ht
    ioopm_hash_table_insert(cart_ht,PTR_AS_ELEM(item),SIZE_AS_ELEM(quantity));
    return 0; //success

}

int remove_from_cart(item_t *item, int id, size_t quantity, database_t *db){
    //TODO removes said item from a cart, 
    cart_t *cart = find_cart(id,db);
    ioopm_hash_table_t *cart_ht = cart->items;

    if(cart == NULL || item == NULL){
        return -1; //cart not found or item not found
    }

    value_t result;

    //check if item in cart + get quantity from result
    if(!ioopm_hash_table_lookup(cart_ht,PTR_AS_ELEM(item),&result)){
        return -2; //item not in cart
    }

    //check valid quantity
    size_t quantity_in_db = result.s;
    if(quantity>quantity_in_db){
        return -3; //unvalid quantity (removed quantity bigger than total in cart)
    }
    //if removed quantity == total quantity of item -> remove completely
    if(quantity==quantity_in_db){
        //remove completely
        ioopm_hash_table_remove(cart_ht,PTR_AS_ELEM(item),&result);
        return 0;
    }
    size_t new_quantity = quantity_in_db-quantity;

    ioopm_hash_table_insert(cart_ht,PTR_AS_ELEM(item),SIZE_AS_ELEM(new_quantity));
    return 0;
}

size_t calculate_cost(int id, database_t *db){
    //TODO calculate the cost of the whole cart. 
    size_t total_price = 0;
    cart_t *cart = find_cart(id,db);

    if(cart==NULL){
        return 0;
    }
    //iterate thru whole cart, key = ptr to item_t price=item_t->price * quantity (the vaule of ht)
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(cart->items);

    while(!ioopm_hash_table_iterator_at_end(it)){
        value_t current_value = ioopm_hash_table_iterator_current_key(it);

        item_t *current_item = current_value.p;
        size_t price = current_item->price;

        value_t quantity_in_value_t = ioopm_hash_table_iterator_current_value(it);
        size_t quantity_of_item = quantity_in_value_t.s;

        size_t current_price = price*quantity_of_item;

        total_price=total_price+current_price;

        ioopm_hash_table_iterator_advance(it);
    }
    ioopm_hash_table_iterator_destroy(it);

    return total_price;
}

int checkout(int id, database_t *db){
    //make sure to remove same from db->shelf
    cart_t *cart = find_cart(id,db);

    if(cart==NULL){
        return -1; //cart not found
    }

    ioopm_hash_table_t *carts_ht = cart->items;

    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(carts_ht);

    while(!ioopm_hash_table_iterator_at_end(it)){
        value_t current_value = ioopm_hash_table_iterator_current_key(it);
        item_t *current_item = current_value.p;

        value_t quantity_in_value_t = ioopm_hash_table_iterator_current_value(it);
        size_t quantity_of_item = quantity_in_value_t.s;

        //now we got item_t & quantity, remove from shelfs
        if(!remove_quantity_from_shelfs(current_item,quantity_of_item,id,db)){
            ioopm_hash_table_iterator_destroy(it);
            return -2; //remove from shelfs failed
        }

        ioopm_hash_table_iterator_advance(it);
        }
    // destroy and clean the cart
    ioopm_hash_table_iterator_destroy(it);
    free_cart(cart);
    return 0;
}

item_t *find_item(char* name, database_t* db){
    ioopm_hash_table_t *items = db->items;
    value_t result;
    if(ioopm_hash_table_lookup(items,PTR_AS_ELEM(name),&result)){
        item_t *item = result.p;
        return item;
    }
    else return NULL;
}

shelf_t *find_shelf(item_t *item, char *place){
    ioopm_list_iterator_t *it =
        ioopm_list_iterator_create(item->shelfs);

    shelf_t *found = NULL;

    while (!ioopm_list_iterator_at_end(it))
    {
        value_t current = ioopm_list_iterator_current(it);
        shelf_t *shelf = current.p;

        if (strcmp(shelf->place, place) == 0)
        {
            found = shelf;
            break;
        }

        ioopm_list_iterator_advance(it);
    }

    ioopm_list_iterator_destroy(it);
    return found;
}

cart_t *find_cart(int id, database_t *db){
    ioopm_hash_table_t *cart_ht = db->carts;
    value_t result;
    if(ioopm_hash_table_lookup(cart_ht,INT_AS_ELEM(id),&result)){
        return result.p;
    }
    return NULL;
}