#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
    if (argc > 1 && is_number(argv[1]) && is_number(argv[2]))
    {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        if (a < 0 || b < 0) {
            puts("Incorrect input");
        }
        else
        {
            int n1 = a;
            int n2 = b;
            while (a != b)
            {
                if (a > b)
                {
                    a = a - b;
                }
                else
                {
                    b = b - a;
                }
            }
            printf("gcd(%d, %d) = %d\n", n1, n2, a);
        }
    }
    else
    {
        puts("Incorrect input");
    }
    
    return 0;
}