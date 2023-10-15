#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int value = 5;
int *p = &value;
int main()
{
    pid_t pid;
    pid = fork();
    if (pid == 0) { /* child process */
        *p += 15;
    } else if (pid > 0) { /* parent process */
        wait(NULL);
        printf("PARENT: value = %d", *p); /* LINE A */
        exit(0);
    }
}