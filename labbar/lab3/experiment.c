#include "../lab2/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/*extern char *strdup(const char *);

typedef union { 
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool(*check_func)(char *);
typedef answer_t(*convert_func)(char *);

answer_t ask_question(char *question, check_func check, convert_func convert)
{
    int buf_siz = 255;
    char buf[buf_siz];

    do
    {
        printf("'%s", question);
        read_string(buf, buf_siz);
    }
    while (!check(buf) || strlen(buf) == 0);

    return convert(buf);
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func) atoi);
    return answer.int_value;
}

answer_t make_float(char *str)
{
    return (answer_t) { .float_value = atof(str) };
}

double ask_question_float(char *question)
{
  return ask_question(question, is_float, make_float).float_value;
}

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func) strdup).string_value;
}*/

int main(void)
{
    int res;
    res = ask_question_int("Mata in ett tal: ");
    printf("'%d'\n", res);
    return 0;
}