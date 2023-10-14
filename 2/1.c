#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
void Collatz(int);
int main(int argc, char *argv[])
{
    int num = 35;
    if (argc >= 2)
        num = atoi(argv[1]);
    int fpid = fork();
    if (fpid < 0)
        printf("error in fork.\n");
    else if (fpid == 0)
    {
        printf("I am the child process, my process id is %d\n", getpid());
        Collatz(num);
        printf("ID: %d quit\n", getpid());
        exit(10);
    }
    printf("I am the parent process, my process id is %d\n", getpid());
    int status;
    wait(&status);
    printf("ID: %d quit\n", getpid());
    return 0;
}

void Collatz(int num)
{
    int i = num, j = 1;
    printf("%d ", i);
    while (i != 1)
    {
        if (i % 2)
        {
            i = 3 * i + 1;
        }
        else
        {
            i = i / 2;
        }
        printf("%d ", i);
        j++;
        if (j % 5 == 0)
        {
            printf("\n");
            j = 0;
        }
    }
}