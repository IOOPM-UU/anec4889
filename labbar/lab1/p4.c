#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *msg = "*";
    int row = atoi(argv[1]);
    int growth = atoi(argv[2]);
    int total = 0;
    for ( int i = 0; i <= row ; ++i)
    {
        for ( int j = 1; j <= growth * i ; ++j )
        {
            printf("%s", msg);
            ++total;
        }
        printf("\n");
    }
    printf("%d\n", total);
    
    
    return 0;
}