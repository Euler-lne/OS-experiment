#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    const int SIZE = 4096;
    const char *name = "OS_2.2";
    int N = 35;
    if (argc >= 2)
        N = atoi(argv[1]) > 0 ? atoi(argv[1]) : 35;
    int pid = fork();
    if (pid < 0) {
        printf("Error in fork.\n");
        return -1;
    } else if (pid == 0) {
        char *message;
        char *ptr1;
        printf("I am the child process, my process id is %d\n", getpid());
        int shm_fd1 = shm_open(name, O_CREAT | O_RDWR, 0777);
        if (shm_fd1 < 0) {
            printf("Open the shm1 failed!\n");
            return -1;
        } else {
            char cur_num[10] = {'\0'};
            sprintf(cur_num, "%d", N);
            message = (char *)malloc(sizeof(char));
            message = realloc(message, strlen(cur_num));
            if (message == NULL) {
                printf("Realloc error\n");
                exit(1);
            }
            sprintf(message, "%s", cur_num);
            while (N != 1) {
                if (N % 2 == 0) {
                    N = N / 2;
                } else {
                    N = 3 * N + 1;
                }
                sprintf(cur_num, "%d", N);
                message = realloc(message, strlen(message) + strlen(cur_num) + 1);
                if (message == NULL) {
                    printf("Realloc error\n");
                    exit(1);
                }
                // 新数字与旧数字之间隔一个空格
                sprintf(message, "%s %s", message, cur_num);
            }
            printf("Successfully done, transmitting...\n");
            // 改变共享内存文件的大小为SIZE
            ftruncate(shm_fd1, SIZE);
            ptr1 = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd1, 0);
            sprintf(ptr1, "%s", message);
            ptr1 += strlen(message);
            free(message);
        }
        printf("ID: %d quit\n", getpid());
    } else {
        printf("I am the parent process, my process id is %d\n", getpid());
        wait(NULL);
        char *ptr2;
        int shm_fd2 = shm_open(name, O_CREAT | O_RDWR, 0777);
        if (shm_fd2 < 0) {
            printf("Open the shm2 failed!\n");
            return -1;
        } else {
            ptr2 = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd2, 0);
            printf("%s\n", (char *)ptr2);
            // 断开共享内存
            shm_unlink(name);
        }
        printf("ID: %d quit\n", getpid());
    }
    return 0;
}
