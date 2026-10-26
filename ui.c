#include "logic.h"

static bool is_in_menu(char *let){
    int c = toupper(let[0]);

    if (strlen(let)<1){return false;}

    if(c == 'A' || c == 'L' || c == 'D' || c == 'E' || c == 'S' || c == 'P' || c == 'R' || c == 'Q' || c == '+' || c == '-' || c == '=' || c == 'O'|| c == 'C' )
    {return true;}

    else{return false;}
}

answer_t strdup_answer_t(char* str){
    answer_t answer;
    answer.string_value=strdup(str);
    return answer;
}

answer_t atoi_answer_t(char *str){
    answer_t answer;
    answer.int_value = atoi(str);
    return answer;
}

static char ask_question_in_menu(){
    answer_t answer = ask_question("choose your alternative:",is_in_menu,(convert_func *) strdup_answer_t);
    char* str = answer.string_value;
    return toupper(str[0]);
}


void print_ui_menu(){

    printf("welcome to our webstore\n"
        "here is your alternatives;\n"
        "[A]dd merchandise\n"
        "[L]ist merchandise\n"
        "[D] remove merchandise\n"
        "[E]dit merchandise\n"
        "[S]how stock\n"
        "[P] Replenish stock\n"
        "[C]reate cart\n"
        "[R]emove Cart\n"
        "[+] add to cart\n"
        "[-] remove from cart\n"
        "[=] calculate cost\n"
        "[O] checkout\n"
        "[Q]uit\n"
        );
}

/*
list merch - 20 at a time 

remove merch y/n

edit merch y/n

remove cart y/n

quit y/n

*/

static char *ask_name(char *question){
    answer_t answer = ask_question(question, not_empty, strdup_answer_t);
    return answer.string_value;
}

static char *ask_desc(char *question){
    answer_t answer = ask_question(question, not_empty, strdup_answer_t);
    return answer.string_value;
}

static int ask_price(char *question){
    answer_t answer = ask_question(question, is_number, atoi_answer_t);
    return answer.int_value;
}

int ask_question_id(){
    answer_t answer = ask_question("id of cart: ", is_number, atoi_answer_t);
    return answer.int_value;
}

static size_t ask_quantity(char *question){
    answer_t answer = ask_question(question, is_number, atoi_answer_t);
    return (size_t) answer.int_value;
}

static char *ask_shelf_place(){
    answer_t answer = ask_question("name of storage location: ", not_empty, strdup_answer_t);
    return answer.string_value;
}

static bool yes_or_no(){
    char* question = "enter y if you are sure";
    answer_t answer = ask_question(question,not_empty,strdup_answer_t);
    char* answer_char = answer.string_value;

    bool result = (answer_char[0] == 'y' || answer_char[0] == 'Y');
    free(answer.string_value);
    return result;
}


item_t *ask_question_merch(){
    char *name = ask_name("name of merch: ");
    char *desc = ask_desc("description of merch: ");
    int price = ask_price("price of merch: ");

    return create_item(name, desc, price);
}

item_t *ask_question_rem_merch(database_t *db){
    while(true){
        char *name = ask_name("name of merch: ");

        item_t *item = find_item(name, db);
        free(name);

        if(item != NULL){
            return item;
        }

        printf("Item not found, try again!\n");
    }
}

bool ask_question_edit_merch(database_t *db){
    char *old_name = ask_name("name of old merch: ");
    item_t *item = find_item(old_name, db);
    free(old_name);

    if(item == NULL){
        return false;
    }

    char *name = ask_name("new name of merch: ");
    char *desc = ask_desc("new description of merch: ");
    int price = ask_price("new price of merch: ");

    bool success = edit_merch(item, db, name, desc, price);

    if(!success){
        free(name);
        free(desc);
    }

    return success;
}

bool ask_question_replenish(database_t *db){
    char *place = ask_shelf_place();
    char *name = ask_name("name of merch: ");
    size_t quantity = ask_quantity("quantity of shelf: ");

    shelf_t *new_shelf = create_shelf(place, quantity);
    item_t *item = find_item(name, db);

    free(name);

    bool success = replenish(item, new_shelf, db);

    if(!success){
        free_shelf(new_shelf);
    }

    return success;
}

bool ask_question_add_to_cart(database_t *db){
    int id = ask_question_id();
    char *name = ask_name("name of merch: ");
    size_t quantity = ask_quantity("quantity of item: ");

    item_t *item = find_item(name, db);
    free(name);

    return add_to_cart(item, id, quantity, db);
}

size_t calc_cost_question(database_t *db){
    int id = ask_question_id();

    return calculate_cost(id, db);
}

void ask_question_stock(database_t *db){
    char* name = ask_name("name of item");
    item_t *item = find_item(name,db);
    show_stock(item);
}

void choose_action(char answer,database_t *db){
    if(answer=='A'){
        item_t *item = ask_question_merch();
        add_merch(item,db);
    }
    if(answer=='L'){
        list_merch(db);
    }
    if(answer=='D'){
        yes_or_no();
        item_t *item = ask_question_rem_merch(db);
        remove_merch(item,db);
    }
    if(answer=='E'){
        yes_or_no();
        ask_question_edit_merch(db);
    }
    if(answer=='S'){
        ask_question_stock(db);
    }
    if(answer=='P'){
        ask_question_replenish(db);
    }
    if(answer=='C'){
        int id =ask_question_id();
        create_cart(id,db);
    }
    if(answer=='R'){
        yes_or_no();
        int id = ask_question_id();
        remove_cart(id,db);
    }
    if(answer=='+'){
        ask_question_add_to_cart(db);
    }
    if(answer=='='){
        size_t price = calc_cost_question(db);
        printf("the price of cart is %zu kr\n",price);
    }
    if(answer=='O'){
        int id = ask_question_id();
        checkout(id,db);
    }
    if(answer=='Q'){
        yes_or_no();
    }

}


void run_webstore(){
    char answer = ' ';
    database_t *db = create_database();

    while (answer != 'Q' && answer != 'q'){
    print_ui_menu();
    answer = ask_question_in_menu();
    choose_action(answer,db);
    }
}
