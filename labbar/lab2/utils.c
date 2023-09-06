#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

void clear_input_buffer()
{
    int c;
    do
    {
        c = getchar();
    }
    while (c != '\n' && c != EOF);
}

int read_string(char *buf, int buf_siz)
{
    int counter = 0;
    char c;
    do
    {
        c = getchar();
        if (c == '\n')
        {
            buf[counter] = '\0';
            return counter;
        }
        buf[counter] = c;
        counter++;
    }
    while (counter < (buf_siz - 1));
    buf[buf_siz - 1] = '\0';
    return counter;
    clear_input_buffer();
}

bool is_number(char *str)
{
    bool num = false;

    for (int i = 0; i < strlen(str); ++i)
    {
        if (i == 0)
        {
            if ((str[i] == 45 && strlen(str) > 1) || isdigit(str[i]) == 1)
            {
                num = true;
            }
            else if (isdigit(str[i]) == 0)
            {
                num = false;
                break;
            }
        }
        else
        {
            if (isdigit(str[i]) == 1)
            {
                num = true;
            }
            else
            {
                num = false;
                break;
            }
        }
    }
    return num;
}

bool not_empty(char *str)
{
    return strlen(str) > 0;
}

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

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func) atoi);
    return answer.int_value;
}

