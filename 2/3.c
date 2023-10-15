#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 4096

int main()
{
    int p1[2], p2[2], pid;
    printf("Input string that you want to tranform.\n");
    if ((pipe(p1) < 0) || (pipe(p2) < 0)) {
        printf("Creat pipe error.\n");
        exit(1);
    }
    if ((pid = fork()) > 0) { // parent
        printf("I am the parent process, my process id is %d\n", getpid());
        char buf[SIZE] = {'\0'};
        int nbytes;
        fgets(buf, SIZE, stdin);
        if (write(p1[1], buf, strlen(buf)) < 0) {
            printf("Write error.\n");
            exit(1);
        }
        close(p1[1]);
        printf("Parent write %s", buf);
        wait(NULL);
        while ((nbytes = read(p2[0], buf, SIZE)) > 0) {
            printf("Parent read %s", buf);
            break;
        }
        if (nbytes < 0) {
            printf("Read error.\n");
            exit(1);
        }
        close(p2[0]);
    } else { // child
        printf("I am the child process, my process id is %d\n", getpid());
        char buf[SIZE] = {'\0'};
        int nbytes;
        while ((nbytes = read(p1[0], buf, SIZE)) > 0) {
            printf("Child read %s", buf);
            int i = 0;
            while (buf[i]) {
                if (isupper(buf[i])) {
                    buf[i] = tolower(buf[i]);
                } else if (islower(buf[i])) {
                    buf[i] = toupper(buf[i]);
                }
                i++;
            }
            if (write(p2[1], buf, strlen(buf)) < 0) {
                printf("Write error.\n");
                exit(1);
            }
            close(p2[1]);
            printf("Child write %s", buf);
            break;
        }
        if (nbytes < 0) {
            printf("Read error.\n");
            exit(1);
        }
        close(p1[0]);
        printf("ID: %d quit\n", getpid());
        exit(10);
    }
    printf("ID: %d quit\n", getpid());
    return 0;
}