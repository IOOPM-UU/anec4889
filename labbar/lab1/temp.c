#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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

int main(int argc, char *argv[])
{
    if (argc > 1 && is_number(argv[1]))
    {
        printf("%s is a number\n", argv[1]);
    }
    else
    {
        if (argc > 1)
        {
            printf("%s is not a number\n", argv[1]);
        }
        else
        {
            printf("Please provide a command line argument!\n");
        }
    }
    return 0;
}