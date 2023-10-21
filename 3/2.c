#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 3
#define K 2
#define N 3

int A[M][K] = {{1, 4}, {2, 5}, {3, 6}};
int B[K][N] = {{8, 7, 6}, {5, 4, 3}};
int C[M][N];

void *Comput(void *arg);

int main()
{
    pthread_t id[M * N];
    int index[M * N];
    for (int i = 0; i < M * N; i++) {
        index[i] = i;
        pthread_create(&id[i], NULL, Comput, (void *)&index[i]);
    }
    for (int i = 0; i < M * N; i++) {
        pthread_join(id[i], NULL);
    }
    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {
            printf("%d ", C[j][i]);
        }
        printf("\n");
    }
    return 0;
}
void *Comput(void *arg)
{
    int row = (*(int *)arg) / N;
    int col = (*(int *)arg) % N;
    C[row][col] = 0;
    for (int i = 0; i < K; i++) {
        C[row][col] += A[row][i] * B[i][col];
    }
}