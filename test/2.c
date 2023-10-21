#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int g = 0;

void *pthread_fun(void *arg)
{
    for (int i = 0; i < 10; i++) {
        printf("g = %d\n", ++g);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t id[5];
    for (int i = 0; i < 5; i++) {
        pthread_create(&id[i], NULL, pthread_fun, NULL);
    }

    for (int j = 0; j < 5; j++) {
        char *s = NULL;
        pthread_join(id[j], (void **)&s);
    }

    exit(0);
    /*
    原因就是linux的线程是内核级线程。程序中对g++并不是原子操作，对g++，计算机需要很多次操作 ，
    比如将内存中的g读取到寄存器中，再从寄存器中读走进行++，再回头进行写入等等一系列操作。
    可能一个线程拿到了内存中的g，还没来得及++再写回去，另一个线程被分配到另一个处理器上，
    读取了相同值的g进行++。所以我们得到的值有时候会比5000要小。
    解决方法有：
    （1）将处理器设置为单核处理器；
    （2）进行线程同步。

    */
}
