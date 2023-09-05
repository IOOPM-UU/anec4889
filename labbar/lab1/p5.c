#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]);
    int prime = 1;
    for ( int x = 2; x <= sqrt(N); ++x )
    {
        for ( int y = 2; y < N; ++y)
        {
            if ((x * y) == N)
            {
                prime = 0;
            }
            else {}
        }
    }
    if (prime == 1)
    {
        printf("%d Är ett primtal\n", N);
    }
    else
    {
        printf("%d Är inte ett primtal\n", N);
    }
    return 0;
}