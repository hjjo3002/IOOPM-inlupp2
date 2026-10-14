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
  
  item_t *item1 = create_item(name,desc,price);
  add_merch(item1,db);
  CU_ASSERT_TRUE(item_exists(name,db));
}

void test_create_and_add_item_false(){
  database_t *db = create_database();
  char* name = "tröja";
  
  CU_ASSERT_FALSE(item_exists(name,db));
}

void test_false_for_CI(){
  CU_ASSERT_EQUAL(true,false);
}

void test_add_and_remove(){
  database_t *db = create_database();
  char* name = "tröja";
  char* desc = "str L";
  int price = 200;
  
  item_t *item1 = create_item(name,desc,price);
  add_merch(item1,db);
  CU_ASSERT_TRUE(item_exists(name,db));

  remove_merch(item1,db);
  CU_ASSERT_FALSE(item_exists(name,db)); //segfault 
  // printf("%p",item1);
  // CU_ASSERT_PTR_NULL(item1);

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
    (CU_add_test(my_test_suite, "create and add item (not mem)", test_create_and_add_item) == NULL) ||
    (CU_add_test(my_test_suite, "create and add item (not mem)", test_create_and_add_item_false) == NULL) ||
    //(CU_add_test(my_test_suite, "Test for CI (fails)", test_false_for_CI) == NULL) ||
    (CU_add_test(my_test_suite, "add and remove", test_add_and_remove) == NULL) ||
    //segfault on add_and_remove
    
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
