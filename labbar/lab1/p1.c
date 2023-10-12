#include <stdio.h>

int rekurs(int n)
{
    if (n == 11)
    {
        return n;
    }
    else
    {
        printf("%d\n", n);
        return rekurs(n + 1);
    }
}

int main(void)
{
    rekurs(1);
    puts("");
    for (int i = 1; i <= 10; i = i + 1)
    {
        printf("%d\n", i);
    }
    return 0;
}