#include <CUnit/Basic.h>
#include "logic.h"
#include "ui.h"
#include "stdbool.h"


int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}



// These are example test functions. You should replace them with
// functions of your own.
void test1(void) {
  CU_ASSERT(42);
}

void test2(void) {
  CU_ASSERT_EQUAL(1 + 1, 2);
}

void test_create_and_add_item(){
  database_t *db = create_database();
  char* name = "tröja";
  char* desc = "str L";
  int price = 200;
  
  item_t *item1 = create_item(strdup(name),strdup(desc),price);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name,db));
  database_destroy(db);

}

void test_create_and_add_item_false(){
  database_t *db = create_database();
  char* name = "tröja";
  
  CU_ASSERT_FALSE(item_exists(name,db));
  database_destroy(db);

}

void test_add_multiple(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* name2 = "tshirt";
  char* desc2 = "str S";
  int price2 = 100;
  
  item_t *item2 = create_item(strdup(name2),strdup(desc2),price2);
  CU_ASSERT_TRUE(add_merch(item2,db));
  CU_ASSERT_TRUE(item_exists(name2,db));

  
  char* name3 = "jeans";
  char* desc3 = "str 34";
  int price3 = 300;
  
  item_t *item3 = create_item(strdup(name3),strdup(desc3),price3);
  CU_ASSERT_TRUE(add_merch(item3,db));
  CU_ASSERT_TRUE(item_exists(name3,db));

  database_destroy(db);
}

void test_add_duplicate(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* name2 = "tröja";
  char* desc2 = "str S";
  int price2 = 100;
  
  item_t *item2 = create_item(strdup(name2),strdup(desc2),price2);

  CU_ASSERT_FALSE(add_merch(item2,db));

  database_destroy(db); 
}


void test_false_for_CI(){
  CU_ASSERT_EQUAL(true,false);
  
}

void test_add_and_remove(){
  database_t *db = create_database();
  char* name = "tröja";
  char* desc = "str L";
  int price = 200;
  
  item_t *item1 = create_item(strdup(name),strdup(desc),price);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name,db));

  CU_ASSERT_TRUE(remove_merch(item1,db));
  CU_ASSERT_FALSE(item_exists(name,db));
  // printf("%p",item1);
  // CU_ASSERT_PTR_NULL(item1);
  database_destroy(db);
}


void test_remove_false(){
  database_t *db = create_database();
  char* name = "tröja";
  char* desc = "str L";
  int price = 200;
  
  item_t *item1 = create_item(strdup(name),strdup(desc),price);
  CU_ASSERT_FALSE(remove_merch(item1,db));

   free_item(item1);
   database_destroy(db);
}

void test_remove_with_shelfs(){

}


void test_edit_merch(){
  database_t *db = create_database();
  char* name = "tröja";
  char* desc = "str L";
  int price = 200;
  
  item_t *item1 = create_item(strdup(name),strdup(desc),price);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name,db));

  char* new_name = "tshirt";
  char* new_desc = "avklippt hoodie";
  int new_price = 99;

  edit_merch(item1, db, strdup(new_name), strdup(new_desc), new_price);
  CU_ASSERT_TRUE(item_exists(new_name,db));
  CU_ASSERT_FALSE(item_exists(name,db));
  
  //list_merch(db);
  database_destroy(db);
}


void test_replenish(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 1;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  show_stock(item1);
  
  database_destroy(db);
}


void test_replenish_add_to_existing(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);
  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));

  size_t quantity2 = 9;

  shelf_t *new_shelf2 = create_shelf(strdup(place1),quantity2);
  CU_ASSERT_FALSE(replenish(item1,new_shelf2,db));

  free_shelf(new_shelf2); 

  //show_stock(item1);
  database_destroy(db);
}

void test_replenish_with_same_amount(){
   database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);
  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));

  size_t quantity2 = 10;

  shelf_t *new_shelf2 = create_shelf(strdup(place1),quantity2);
  CU_ASSERT_FALSE(replenish(item1,new_shelf2,db));

  free_shelf(new_shelf2); 

  //show_stock(item1);
  database_destroy(db);
}


void mem_test_cart(){
  database_t *db = create_database();
  int id = 1;

  create_cart(id,db);
  database_destroy(db);
}  



void test_add_to_cart_single(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 1;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id = 1;
  create_cart(id,db);

  CU_ASSERT_TRUE(add_to_cart(item1,id,10,db));

  database_destroy(db);
}

void test_multiple_carts_all_stock_gone(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 1;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  int id2 = 2;
  create_cart(id2,db);
  
  int id3 = 3;
  create_cart(id3,db);

  CU_ASSERT_TRUE(add_to_cart(item1,id1,10,db));
  CU_ASSERT_TRUE(add_to_cart(item1,id2,4,db));
  CU_ASSERT_TRUE(add_to_cart(item1,id3,5,db));

  database_destroy(db);
}


void test_add_to_cart_false(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 1;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  int id2 = 2;
  create_cart(id2,db);

  CU_ASSERT_TRUE(add_to_cart(item1,id1,10,db));
  CU_ASSERT_FALSE(add_to_cart(item1,id2,10,db));
  CU_ASSERT_TRUE(add_to_cart(item1,id1,4,db));

  database_destroy(db);
}

void test_cart_add_remove(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 1;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  CU_ASSERT_TRUE(add_to_cart(item1,id1,10,db));
  CU_ASSERT_TRUE(remove_from_cart(item1,id1,5,db));
  database_destroy(db);

}

void test_cart_add_remove_to_much(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 1;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  CU_ASSERT_TRUE(add_to_cart(item1,id1,10,db));
  CU_ASSERT_FALSE(remove_from_cart(item1,id1,11,db));

  database_destroy(db);
}

void test_add_remove_add_cart(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 1;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  CU_ASSERT_TRUE(add_to_cart(item1,id1,10,db));
  CU_ASSERT_TRUE(remove_from_cart(item1,id1,10,db));
  CU_ASSERT_TRUE(add_to_cart(item1,id1,18,db));

  database_destroy(db);
}

void test_calculate_cost_single_item(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  // char* place2 = "A11";
  // size_t quantity2 = 8;
  // shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  // char* place3 = "A12";
  // size_t quantity3 = 1;
  // shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  // CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  // CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  CU_ASSERT_TRUE(add_to_cart(item1,id1,10,db));
  CU_ASSERT_EQUAL(calculate_cost(id1,db),2000);

  database_destroy(db);
}

void test_multiple_shelfs_single_item(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 12;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  CU_ASSERT_TRUE(add_to_cart(item1,id1,20,db));
  CU_ASSERT_EQUAL(calculate_cost(id1,db),4000);

  database_destroy(db);
}

void test_multiple_items_shelfs(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 12;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  char* name2 = "skor";
  char* desc2 = "str 42";
  int price2 = 100;
  
  item_t *item2 = create_item(strdup(name2),strdup(desc2),price2);
  CU_ASSERT_TRUE(add_merch(item2,db));
  CU_ASSERT_TRUE(item_exists(name2,db));

  char* place4 = "A13";
  size_t quantity4 = 10;
  shelf_t *new_shelf4 = create_shelf(strdup(place4),quantity4);

  char* place5 = "A14";
  size_t quantity5 = 8;
  shelf_t *new_shelf5 = create_shelf(strdup(place5),quantity5);

  char* place6 = "A15";
  size_t quantity6 = 12;
  shelf_t *new_shelf6 = create_shelf(strdup(place6),quantity6);

  CU_ASSERT_TRUE(replenish(item2,new_shelf4,db));
  CU_ASSERT_TRUE(replenish(item2,new_shelf5,db));
  CU_ASSERT_TRUE(replenish(item2,new_shelf6,db));

  int id1 = 1;
  create_cart(id1,db);

  CU_ASSERT_TRUE(add_to_cart(item1,id1,20,db));
  CU_ASSERT_TRUE(add_to_cart(item2,id1,20,db));

  CU_ASSERT_EQUAL(calculate_cost(id1,db),6000);

  database_destroy(db);
}

void test_checkout_complete(){
  database_t *db = create_database();
  char* name1 = "tröja";
  char* desc1 = "str L";
  int price1 = 200;
  
  item_t *item1 = create_item(strdup(name1),strdup(desc1),price1);
  CU_ASSERT_TRUE(add_merch(item1,db));
  CU_ASSERT_TRUE(item_exists(name1,db));

  char* place1 = "A10";
  size_t quantity1 = 10;
  shelf_t *new_shelf1 = create_shelf(strdup(place1),quantity1);

  char* place2 = "A11";
  size_t quantity2 = 8;
  shelf_t *new_shelf2 = create_shelf(strdup(place2),quantity2);

  char* place3 = "A12";
  size_t quantity3 = 12;
  shelf_t *new_shelf3 = create_shelf(strdup(place3),quantity3);

  CU_ASSERT_TRUE(replenish(item1,new_shelf1,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf2,db));
  CU_ASSERT_TRUE(replenish(item1,new_shelf3,db));

  int id1 = 1;
  create_cart(id1,db);
  
  CU_ASSERT_TRUE(add_to_cart(item1,id1,20,db));
  CU_ASSERT_EQUAL(calculate_cost(id1,db),4000);
  CU_ASSERT_TRUE(checkout(id1,db));
  
  database_destroy(db);
}




int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(my_test_suite, "A simple test", test1) == NULL) ||
    (CU_add_test(my_test_suite, "Basic arithmetics", test2) == NULL) ||
    (CU_add_test(my_test_suite, "create and add item", test_create_and_add_item) == NULL) ||
    (CU_add_test(my_test_suite, "add multiple items", test_add_multiple) == NULL) ||
    (CU_add_test(my_test_suite, "create and add item (not mem)", test_create_and_add_item_false) == NULL) ||
    //(CU_add_test(my_test_suite, "Test for CI (fails)", test_false_for_CI) == NULL) ||
    (CU_add_test(my_test_suite, "add and remove", test_add_and_remove) == NULL) ||
    (CU_add_test(my_test_suite, "edit merch", test_edit_merch) == NULL) ||
    (CU_add_test(my_test_suite, "add duplicate", test_add_duplicate) == NULL) || 
    (CU_add_test(my_test_suite, "remove_false", test_remove_false) == NULL) || 
    (CU_add_test(my_test_suite, "memtest for cart", mem_test_cart) == NULL) || 
    (CU_add_test(my_test_suite, "test replenish +(print)", test_replenish) == NULL) || 
    (CU_add_test(my_test_suite, "test replenish add to existing", test_replenish_add_to_existing) == NULL) || 
    (CU_add_test(my_test_suite, "test replenish add to existing", test_replenish_with_same_amount) == NULL) || 
    (CU_add_test(my_test_suite, "add to cart single", test_add_to_cart_single) == NULL) || 
    (CU_add_test(my_test_suite, "add to cart multiple + all stock gone", test_multiple_carts_all_stock_gone) == NULL) || 
    (CU_add_test(my_test_suite, "add to cart false", test_add_to_cart_false) == NULL) || 
    (CU_add_test(my_test_suite, "add + remove from cart", test_cart_add_remove) == NULL) || 
    (CU_add_test(my_test_suite, "add + remove from cart false", test_cart_add_remove_to_much) == NULL) || 
    (CU_add_test(my_test_suite, "add + remove + add, egde", test_add_remove_add_cart) == NULL) || 
    (CU_add_test(my_test_suite, "calculate cost single item", test_calculate_cost_single_item) == NULL) || 
    (CU_add_test(my_test_suite, "calculate cost multiple shelfs single item", test_multiple_shelfs_single_item) == NULL) || 
    (CU_add_test(my_test_suite, "calculate cost multiple shelfs multiple item", test_multiple_items_shelfs) == NULL) || 
    (CU_add_test(my_test_suite, "checkout complete", test_checkout_complete) == NULL) || 









    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_NORMAL);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  unsigned int fails= CU_get_number_of_failures();
  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return fails == 0? 0:1;
}
