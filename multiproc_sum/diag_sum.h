#ifndef MULTIPROC_SUM_H
#define MULTIPROC_SUM_H

#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
struct mymsgbuf
{
    long mtype;
    long long mdata[2];
};

long long *get_sums(size_t const size, int **const matrix);
int free_res(long long *res, size_t size);

#endif
