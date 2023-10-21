#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *pthread_fun(void *arg)
{
    int index = *(int *)arg;
    int i = 0;
    for (; i < 5; i++) {
        printf("index = %d\n", index);
        sleep(1);
    }
}

int main()
{
    pthread_t id[5];
    int i = 0;
    for (; i < 5; i++) {
        pthread_create(&id[i], NULL, pthread_fun, (void *)&i);
    }
    // 出现各种各样的情况是因为，i传入的是地址
    // 但是i在循环里面是会变的，i变换之后index也变了
    /*
    这是因为我们向pthread_fun传入i的地址。首先来说说为什么会出现多个线程拿到同一个i的值。
    线程创建在计算机中需要很多个步骤，我们进入for循环传入i的地址后就去进行下一个for循环，
    创建的线程还没有从地址中获取打印i的值，主函数就继续创建后面的线程了，导致多个线程并发，
    拿到同一个i值，而且不是创建该线程的时候i的值。
    注意到打印第一个运行结果都是打印0，这是因为主函数第一个for循环已经结束了，
    后面一个for循环将i又置为0，而这些线程在主函数第一个for循环执行的时候，
    都没有回获取i的值打印，直到下一个for循环，这些线程才获取i值打印，所以打印出来 都是0。
    */

    for (i = 0; i < 5; i++) {
        pthread_join(id[i], NULL);
    }

    exit(0);
}
