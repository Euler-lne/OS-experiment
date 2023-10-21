#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int avg, max, min;
void ReadNumbers(int *arry);
void PrintNumbers(int *arry);
void *ComputAvg(void *arg);
void *ComputMax(void *arg);
void *ComputMin(void *arg);

void OutputResult();

int main()
{
    int len;
    printf("Input the length of the numbers.\n");
    scanf("%d", &len);
    while (len <= 0) {
        printf("Length must be positive.\n");
        scanf("%d", &len);
    }
    int *arry = (int *)malloc(sizeof(int) * (len + 1));
    if (arry == NULL) {
        printf("Malloc error.\n");
        return -1;
    }
    arry[0] = len;
    ReadNumbers(arry);
    pthread_t id[3];
    pthread_create(&id[0], NULL, ComputAvg, (void *)arry);
    pthread_create(&id[1], NULL, ComputMin, (void *)arry);
    pthread_create(&id[2], NULL, ComputMax, (void *)arry);
    for (int i = 0; i < 3; i++) {
        pthread_join(id[i], NULL);
    }
    OutputResult();
    return 0;
}
void ReadNumbers(int *arry)
{
    printf("Enter numbers separated by spaces.\n");
    for (int i = 1; i <= arry[0]; i++) {
        scanf("%d", &arry[i]);
    }
}

void PrintNumbers(int *arry)
{
    printf("Numbers you input.\n");
    for (int i = 1; i <= arry[0]; i++) {
        printf("%d ", arry[i]);
    }
}

void *ComputAvg(void *arg)
{
    int *arry = (int *)arg;
    avg = 0;
    for (int i = 1; i <= arry[0]; i++) {
        avg += arry[i];
    }
    avg /= arry[0];
}

void *ComputMax(void *arg)
{
    int *arry = (int *)arg;
    max = arry[1];
    for (int i = 1; i <= arry[0]; i++) {
        max = max < arry[i] ? arry[i] : max;
    }
}

void *ComputMin(void *arg)
{
    int *arry = (int *)arg;
    min = arry[1];
    for (int i = 1; i <= arry[0]; i++) {
        min = min > arry[i] ? arry[i] : min;
    }
}

void OutputResult()
{
    printf("The average value is %d.\n", avg);
    printf("The minimum value is %d.\n", min);
    printf("The maximum value is %d.\n", max);
}