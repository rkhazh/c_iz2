#ifndef MULTIPROC_SUM_H
#define MULTIPROC_SUM_H

#include <stdio.h>
long long* get_sums(size_t const size, int** const matrix);
int free_res(long long* res, size_t size);

#endif