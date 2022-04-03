#include "diag_sum.h"
#include <stdio.h>
#include <stdlib.h>

long long *get_sums(size_t size, int **const matrix)
{
    if (!size || !matrix)
    {
        return NULL;
    }
    printf("SINGLEMODE\n");
    // rows + columns = diagonals count + 1 => diagonals count = rows + columns - 1
    long long *res = calloc(size * 2 - 1, sizeof(long long));
    if (!res)
        return NULL;
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            // i-j+size-1 = Номер диагонали сверху. Для матрицы 5х5: 0 - 4 + 5 - 1 = 0
            res[i - j + size - 1] = res[i - j + size - 1] + matrix[i][j];
        }
    }
    return res;
}

int free_res(long long *res, size_t size)
{
    /*
    Да, я знаю, что free(res) можно сделать и без этой функции,
    но она тут нужна, так как в многопроцессной реализации необходимо
    вызывать не free(), а munmap(). Эта функция позволяет юзеру не заморачиваться
    над тем, какую реализацию библиотеки он использует.
    Интерфейс в данном случае абсолютно одинаковый.
    */
    free(res);
    res = NULL;
    return 0;
}
