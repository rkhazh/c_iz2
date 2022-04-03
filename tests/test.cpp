#include <gtest/gtest.h>

extern "C"
{
#include "diag_sum.h"
}

long long *send_matrix_to_test(int **matrix,size_t size)
{
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
    long long *res = NULL;
    res = get_sums(size, matrix);
    return res;
}

TEST(GETSUM, isTrueResult)
{
    long long matrix_res[] = {9, 18, 27, 36, 45, 54, 63, 72, 81, 90, 81, 72, 63, 54, 45, 36, 27, 18, 9};
    size_t size = 10;
    int ** matrix = (int **)malloc(size * sizeof(int *));
    ASSERT_TRUE(matrix != NULL);
    long long *res = send_matrix_to_test(matrix,size);
    ASSERT_TRUE(res != NULL);
    for (int i = 0; i < size * 2 - 1; ++i)
    {
        EXPECT_EQ(res[i], matrix_res[i]);
    }
    free_res(res, (size * 2 - 1) * sizeof(long long));
}

TEST(GETSUM, StressTest)
{
    size_t size = 10000;
    int ** matrix = (int **)malloc(size * sizeof(int *));
    ASSERT_TRUE(matrix != NULL);
    long long *res = send_matrix_to_test(matrix,size);
    ASSERT_TRUE(res != NULL);
    free_res(res, (size * 2 - 1) * sizeof(long long));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
