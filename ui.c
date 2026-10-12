#include "logic.h"

bool is_in_menu(char *let){
    int c = toupper(let[0]);

    if (strlen(let)<1){return false;}

    if(c == 'A' || c == 'L' || c == 'D' || c == 'E' || c == 'S' || c == 'P' || c == 'R' || c == 'Q' || c == '+' || c == '-' || c == '=')
    {return true;}

    else{return false;}
}

answer_t strdup_answer_t(char* str){
    answer_t answer;
    answer.string_value=strdup(str);
    return answer;
}

bool answer_loop(answer_t answer){
    answer = ask_question("choose your alternative:",is_in_menu,(convert_func *) strdup_answer_t);
    printf("you chose %s\n", answer.string_value);

    return (toupper(answer.string_value[0]) == 'Q');
}


void print_ui_menu(){
    answer_t answer;
    do{
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
    while(!answer_loop(answer)==true);
    printf("Thanks for using our webstore\n");
}

