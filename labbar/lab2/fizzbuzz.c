#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

void print_number(int num)
{
    if (num % 15 == 0)
    {
        printf("fizzbuzz");
    }
    else if (num % 3 == 0)
    {
        printf("fizz");
    }
    else if (num % 5 == 0)
    {
        printf("buzz");
    }
    else
    {
        printf("%d", num);
    }
}

int main(int argc, char *argv[])
{
    int T = atoi(argv[1]);
    for (int i = 1; i <= T; ++i)
    {
        print_number(i);
        if (i != T)
        {
            printf(", ");
        }
    }
    printf("\n");
    return 0;
}