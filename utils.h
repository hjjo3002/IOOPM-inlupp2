#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "datastruct.h"

typedef union answer answer_t;

union answer { 
  int   int_value;
  float float_value;
  char *string_value;
};

typedef bool check_func(char *);

typedef answer_t convert_func(char *);


bool not_empty(char *str);

extern char *strdup(const char *);

int read_string(char *buf, int buf_siz);

bool is_number(char *str);

bool not_empty(char *str);

answer_t ask_question(char *question, check_func *check,convert_func *con);

int compare_str_alphabetical(const void *a, const void *b);

