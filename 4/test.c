#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define N 25
#define SLEEP_TIME 5
#define PRODUCER 10
#define CONSUMER 5

sem_t mutex, full, empty;
int buffer[N];
int start = 0, end = 0;

int is_all_digits(char *str);
void *Produser(void *arg);
void *Consumer(void *arg);

int main(int argc, char *argv[])
{
    // 参数初始化
    int sleep_time = SLEEP_TIME;
    int producer = PRODUCER;
    int consumer = CONSUMER;
    srand((unsigned)time(NULL));
    if (argc >= 2) {
        if (is_all_digits(argv[1]))
            sleep_time = atoi(argv[1]);
    }
    if (argc >= 3) {
        if (is_all_digits(argv[2]))
            producer = atoi(argv[2]);
    }
    if (argc >= 4) {
        if (is_all_digits(argv[3]))
            consumer = atoi(argv[3]);
    }
    // 初始化信号量
    if (sem_init(&empty, 0, N) == -1 || sem_init(&full, 0, 0) == -1 || sem_init(&mutex, 0, 1) == -1) {
        printf("sem_init error.");
        return 0;
    }
    // 创建线程
    pthread_t *pid = (pthread_t *)malloc(sizeof(pthread_t) * (producer + consumer));
    int *id = (int *)malloc(sizeof(int) * (producer + consumer));
    if (id == NULL || pid == NULL) {
        printf("malloc error.");
        return 0;
    }
    for (int i = 0; i < producer; i++) {
        id[i] = i;
        pthread_create(&pid[i], NULL, Produser, (void *)&id[i]);
    }
    for (int i = 0; i < consumer; i++) {
        id[i + producer] = i + producer;
        pthread_create(&pid[i + producer], NULL, Consumer, (void *)&id[i]);
    }
    sleep(sleep_time);
    return 0;
}

int is_all_digits(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

void *Produser(void *arg)
{
    int id = *(int *)arg;
    int num;
    int sleep_time;
    while (1) {
        sleep_time = rand() % 3000 + 1000;
        sleep(0.001 * sleep_time);
        num = rand() % 100;
        printf("The producer %d produces a number %d\n", id, num);
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[end] = num;
        printf("The producer %d inserts the data %d into the buffer[%d]\n", id, num, end);
        end = end + 1 == N ? 0 : end + 1;
        sem_post(&mutex);
        sem_post(&full);
    }
}

void *Consumer(void *arg)
{
    int id = *(int *)arg;
    int sleep_time;
    while (1) {
        srand((unsigned)time(NULL));
        sleep_time = rand() % 3000 + 1000;
        sleep(0.001 * sleep_time);
        sem_wait(&full);
        sem_wait(&mutex);
        printf("The consumer %d pulls the data %d out of the buffer[%d]\n", id, buffer[start], start);
        start = start + 1 == N ? 0 : start + 1;
        sem_post(&mutex);
        sem_post(&empty);
    }
}