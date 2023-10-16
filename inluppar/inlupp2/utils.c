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
    } while (c != '\n' && c != EOF);
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
    } while (counter < (buf_siz - 1));
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
            if ((str[i] == 45 && strlen(str) > 1) || isdigit(str[i]))
            {
                num = true;
            }
            else if (!isdigit(str[i]))
            {
                num = false;
                break;
            }
        }
        else
        {
            if (isdigit(str[i]))
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

bool is_shelf(char *str)
{
    if (strlen(str) == 3 && str[0] >= 65 && str[0] <= 90 && isdigit(str[1]) && isdigit(str[2]))
    {
        return true;
    }

    return false;
}

bool is_menu(char *str)
{
    if (strlen(str) != 1)
    {
        return false;
    }
    else
    {
        if (toupper(str[0]) == 65 ||
            toupper(str[0]) == 76 ||
            toupper(str[0]) == 68 ||
            toupper(str[0]) == 69 ||
            toupper(str[0]) == 83 ||
            toupper(str[0]) == 80 ||
            toupper(str[0]) == 81)
        {
            return true;
        }
        return false;
    }
}

bool is_replenish(char *str)
{
    if (strlen(str) != 1)
    {
        return false;
    }
    else
    {
        if (toupper(str[0]) == 65 ||
            toupper(str[0]) == 82)
        {
            return true;
        }
        return false;
    }
}

bool is_true(char *str)
{
    return true;
}

answer_t to_upper(char *str)
{
    for (int i = 0; i < strlen(str); ++i)
    {
        str[i] = toupper(str[i]);
    }
    return (answer_t){.string_value = str};
}

answer_t ask_question(char *question, check_func check, convert_func convert)
{
    int buf_siz = 255;
    char buf[buf_siz];

    do
    {
        printf("%s\n", question);
        read_string(buf, buf_siz);
    } while (!check(buf) || (strlen(buf) == 0 && check != is_true));
    return convert(buf);
}

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func)strdup).string_value;
}

int ask_question_int(char *question)
{
    return ask_question(question, (check_func)is_number, (convert_func)atoi).int_value;
}

char *ask_question_shelf(char *question)
{
    return ask_question(question, is_shelf, (convert_func)strdup).string_value;
}

char *ask_question_menu()
{
    char *menu_str = "[A] Add Merchandise\n[L] List Merchandise\n[D] Remove Merchandise\n[E] Edit Merchandise\n[S] Show Stock\n[P] Replenish Merchandise\n[Q] Quit";
    return ask_question(menu_str, (check_func)is_menu, (convert_func)strdup).string_value;
}

char *ask_question_letter(char *question)
{
    return ask_question(question, (check_func)is_true, (convert_func)strdup).string_value;
}

char *ask_question_replenish()
{
    char *str = "[A] Add shelf\n[R] Replenish existing shelf";
    return ask_question(str, (check_func)is_replenish, (convert_func)strdup).string_value;
}