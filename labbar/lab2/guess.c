#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include <time.h>

int main(void)
{
    srandom(time(NULL));
    int r = random() % 10;
    char *name;
    int num = -1;
    int counter = 0;

    name = ask_question_string("Skriv in ditt namn: ");
    printf("Du %s, jag tänker på ett tal ... kan du gissa vilket?", name);

    while (num != r && counter < 15)
    {   
        num = ask_question_int("");
        if (num > r)
        {
            printf("För stort!");
        }
        else if (num < r)
        {
            printf("För litet!");
        }
        counter++;
    }

    if (counter >= 15)
    {
        printf("\nNu har du slut på gissningar! Jag tänkte på %d\n", r);
    }
    else
    {
        printf("\nBingo\nDet tog %s %d gissningar att komma fram till %d\n", name, counter, r);
    }

    return 0;
}