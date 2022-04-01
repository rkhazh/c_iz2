#include <stdio.h>
#include <malloc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>


struct msgbuf
{
    long mtype;
    long long mdata[2];
};

int send_message(int qid, struct msgbuf *qbuf, long type, long long data[2])
{
    qbuf->mtype = type;
    qbuf->mdata[0] = data[0];
    qbuf->mdata[1] = data[1];
    if (msgsnd(qid, (struct msgbuf *)qbuf, sizeof(qbuf->mdata), 0) == -1)
    { /* Error */ 
        return -1;
    }
    return 0;
}
int read_message(int qid, struct msgbuf *qbuf, long type)
{
    qbuf->mtype = type;
    msgrcv(qid, (struct msgbuf *)qbuf, sizeof(long long) * 2, type, 0);
    return 0;
}

long long* solve_paral(size_t const size, int** const matrix){
    size_t proc_count = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Cores: %d\n====================\n", proc_count);

    int qtype = 1, status, stat, pid[proc_count], msgqid;
    struct msgbuf qbuf;
    if ((msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1)
    { /* Queue creation error */
        return NULL;
    }

    // Creating processes
    for (int i = 0; i < proc_count; ++i)
    {
        pid[i] = fork();
        if (-1 == pid[i])
        { /* Error */
            return NULL;
        }
        else if (0 == pid[i])
        { /* Child process */
            read_message(msgqid, &qbuf, qtype);
            long long left = qbuf.mdata[0];
            long long right = qbuf.mdata[1];
            //printf("CHILD: RECIEVED %d %d\n", left, right);

            if (left == -1)
            {
                long long snd[2] = {-1, -1};
                if (send_message(msgqid, &qbuf, qtype + 1, snd) == -1)
                {
                    exit(1);
                };
                exit(0);
            }

            unsigned int rows_c = (right - left);
            long long* res = calloc(rows_c+size-1, sizeof(long long));
            if(!res) { exit(1); }
            for (size_t r = left; r < right; ++r)
            { // rows in area
                for (size_t c = 0; c < size; ++c)
                { // columns
                    res[r - c + size - 1 - left] = res[r - c + size - 1 - left] + matrix[r][c]; // i-j+size-1 = Номер диагонали сверху. Для матрицы 5х5: 0 - 4 + 5 - 1 = 0
                }
            }
            //return result
            for(long long j = 0; j < rows_c+size-1; ++j)
            {
                long long snd[2];
                snd[0] = j + left;
                snd[1] = res[j];
                if(send_message(msgqid, &qbuf, qtype + 1, snd) == -1){
                    free(res);
                    exit(1);
                };
                //printf("CHILD SENT %d %d\n", snd[0], snd[1]);
            }
            long long snd[2] = {-1, -1};
            if(send_message(msgqid, &qbuf, qtype + 1, snd) == -1){
                    free(res);
                    exit(1);
            };
            free(res);
            exit(0);
        }
    }
    // processes are started
    size_t part;
    part = !(size % proc_count) ? size / proc_count : (size + proc_count - 1) / proc_count; // else: round up int
    
    for(int i = 0; i < proc_count; ++i)
    {
        long long limits[2];
        if(i*part < size){
            limits[0] = i*part;
            limits[1] = ( (i+1)*part < size ) ? (i+1)*part : size; 
        } else {
            limits[0] = -1;
            limits[1] = -1;
        }
        send_message(msgqid, &qbuf, qtype, limits);
        //printf("SENT to i = %d\nleft = %d\nright = %d\n==========================\n", i, limits[0], limits[1]);
    }
    long long* sh_res = mmap(NULL, (size * 2 - 1 ) * sizeof(long long), PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pid_t checker_pid = fork();
    if (!checker_pid){
        size_t count_done = proc_count;
        for (;;)
        {
            read_message(msgqid, &qbuf, qtype + 1);
            long long index = qbuf.mdata[0];
            long long value = qbuf.mdata[1];
            if (index == -1)
            {
                if (!--count_done)
                    exit(0);
            }
            else
            {
                //printf("RECIEVED %d %d\n", index, value);
                sh_res[index] += value;
            }
        }
    }
    char is_ok = 1;
    for (int i = 1; i < proc_count; ++i)
    {
        status = waitpid(pid[i], &stat, 0);
        if (pid[i] == status)
        { /* Child process pid[i] exited */
            //printf("Child process %d exited with status: %d\n", pid[i], stat);
            if (stat){
                is_ok = 0;
            }
        }
    }
    if (!is_ok) {
        kill(checker_pid, SIGKILL);
        return NULL;
    }
    status = waitpid(checker_pid, &stat, 0);
    if(status != checker_pid){
        return NULL;
    }
    
    if (msgctl(msgqid, IPC_RMID, NULL) < 0)
    { /* Queue removal error */
        return NULL;
    }
    return sh_res;
}
long long *get_sums(size_t size, int** const matrix)
{
    long long sum = 0;
    long long *res = calloc(size * 2 - 1, sizeof(long long));
    if (!res)
        return NULL;
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            res[i - j + size - 1] = res[i - j + size - 1] + matrix[i][j]; // i-j+size-1 = Номер диагонали сверху. Для матрицы 5х5: 0 - 4 + 5 - 1 = 0
        }
    }
    return res;
}

int main(int argc, char* argv[])
{
    size_t size = 10000;
    //scanf("%d",&size);
    int** matrix = (int**)malloc(size * sizeof(int*));
    for (int index = 0; index < size; ++index)
    {
        matrix[index] = (int *)malloc(size * sizeof(int));
    }
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            matrix[i][j] = i;
        }
    }
    long long* res = get_sums(size, matrix);
    if(!res) return 1;
    for(int k = 0; k < size*2-1; ++k){
        printf("%d\n", res[k]);
    }
    free(res);
    printf("\n=====================\nPARAL METHOD:\n");
    long long* res1 = solve_paral(size, matrix);
    if(!res1) return 1;
    for(int k = 0; k < size*2-1; ++k){
        //printf("%d\n", res[k]);
    }
    printf("Done");
    munmap(res1, (size*2-1)*sizeof(long long));
    for (int index = 0; index < size; ++index)
    {
        free(matrix[index]);
    }
    free(matrix);
    return 0;
}