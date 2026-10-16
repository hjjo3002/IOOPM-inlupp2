#include "utils.h"


int read_string(char *buf, int buf_siz)
{
    int i = 0;
    int c;

    c = getchar();

    while (c != '\n' && c != EOF && i < buf_siz - 1)
    {
        buf[i] = c;
        i++;
        c = getchar();
    }

    buf[i] = '\0';

    if (c != '\n' && c != EOF)
    {
        while (c != '\n' && c != EOF)
        {
            c = getchar();
        }
    }

    return i;
}

bool is_number(char *str)
{
    int len = strlen(str);

    for(int i = 0; i<len; i++)
    {
        bool res = isdigit(str[i]);
        if (res == false)
            
            if (str[i] == '-' && i == 0)
            {

            }
            else 
            {
                return false;
            }
        
        else 
        {
    
        }
    }
    return true;
}
/// Hjälpfunktion till ask_question_string
bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func *check,convert_func *con){

    int buf_siz = 255;
    char buf[buf_siz];
    bool done = false; 
    answer_t answer;
    
    do
    {
    
    printf("%s\n",question);

    read_string(buf, buf_siz); //buf = "svar"


    if (check(buf)){
        answer = con(buf);
        done = true;
    }

    else{puts("försök igen");}

    } while (!done);

    return answer;
    
    }

int compare_str_alphabetical(const void *a, const void *b){
    return strcmp(*(const char**)a, *(const char**)b);
}

