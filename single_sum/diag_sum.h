#ifndef SINGLE_SUM_H
#define SINGLE_SUM_H

#include <stdio.h>

long long *get_sums(size_t size, int** const matrix);
int free_res(long long* res, size_t size); // size для того, чтобы интерфейс библиотек был одинаковый

#endif
