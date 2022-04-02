#include <gtest/gtest.h>

extern "C"
{
#include "diag_sum.h"
}

TEST(STRESS, S10000)
{
    size_t size = 10000;

    // init matrix

    int **matrix = (int **)malloc(size * sizeof(int *));
    ASSERT_EQ(matrix != NULL, 1);
    for (int index = 0; index < size; ++index)
    {
        matrix[index] = (int *)malloc(size * sizeof(int));
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            matrix[i][j] = i;
        }
    }
    long long *res = get_sums(size, matrix);
    ASSERT_EQ(res != NULL, true);
    free_res(res, (size * 2 - 1) * sizeof(long long));
}

TEST(GETSUM, EL0)
{
    size_t size = 100;

    // init matrix

    int **matrix = (int **)malloc(size * sizeof(int *));
    ASSERT_EQ(matrix != NULL, 1);
    for (int index = 0; index < size; ++index)
    {
        matrix[index] = (int *)malloc(size * sizeof(int));
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            matrix[i][j] = 0;
        }
    }

    long long *res = get_sums(size, matrix);
    ASSERT_EQ(res != NULL, true);
    free_res(res, (size * 2 - 1) * sizeof(long long));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}