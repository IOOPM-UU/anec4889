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

char *ask_question_string(char *question, char *buf, int buf_siz)
{
    do
    {
        printf("%s\n", question);
        read_string(buf, buf_siz);
    }
    while (strlen(buf) == 0);
    return strdup(buf);
}

int ask_question_int(char *question)
{
    int result = 0;
    int conversions = 0;
    do
    {
        printf("%s\n", question);
        conversions = scanf("%d", &result);
        clear_input_buffer();
        putchar('\n');
    }
    while (conversions < 1);
    return result;
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