#include <stdio.h>
#include <malloc.h>
#include <sys/mman.h>
#include "diag_sum.h"

int main(int argc, char *argv[])
{
    size_t size = 10;
    //init matrix
    int **matrix = (int **)malloc(size * sizeof(int *));
    if (matrix == NULL)
    {
        return 1;
    }
    for (int index = 0; index < size; ++index)
    {
        matrix[index] = (int *)malloc(size * sizeof(int));
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            matrix[i][j] = i + j;
        }
    }

    // usage lib
    long long *res = get_sums(size, matrix);
    if (!res)
    {
        return 1;
    }
    for (size_t k = 0; k < size * 2 - 1; ++k)
    {
        printf("%d\n", res[k]);
    }
    free_res(res, (size * 2 - 1) * sizeof(long long));
    return 0;
}
