#include <stdio.h>

int main(void)
{
    char *msg = "*";
    for ( int i = 1 ; i <= 10 ; i = i + 1 )
    {
        for ( int j = 1 ; j <= i ; j = j + 1 )
        {
            printf("%s", msg);
        }
        printf("\n");
    }
    
    return 0;
}