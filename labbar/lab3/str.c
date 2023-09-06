#include <stdlib.h>
#include <stdio.h>

typedef int(*int_fold_func)(int, int);

int foldl_int_int(int numbers[], int numbers_siz, int_fold_func f)
{
    int result = 0;

    for (int i = 0; i < numbers_siz; ++i)
    {
        result = f(result, numbers[i]);
    }

    return result;
}

int add(int a, int b)
{
    return a + b;
}

long sum(int numbers[], int numbers_siz)
{
  return foldl_int_int(numbers, numbers_siz, add);
}

int main(void)
{
    int num[4] = {1, 2, 3, 4};
    int res;

    res = sum(num, 4);
    printf("%d\n", res);
    return 0;
}