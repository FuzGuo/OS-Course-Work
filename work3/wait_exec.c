#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>  

void main() {
    // execl("/bin/echo", "echo", "I am in execl", (char*)0);
    pid_t pid;
    int status = -10;
    pid = fork();
    if (pid == 0) {
        printf("i am in son proc\n");
        printf("pid = %d\n", getpid());
        int t = wait(&status);
        printf("t = %d\n", t);
        printf("status = %d\n", status);
        // 子进程下面没有子进程，wait返回-1，status也未变化
        sleep(3);
    } else {
        // wait(&status) 返回子进程的pid， status为执行exit(5)的一个值
        // 当然不为5， WEXITSTATUS(status)返回值为5
        // sleep(3);
        // wait()会等待子进程执行结束，然后回过来的执行父进程
        printf("i am in father proc\n");
        int t = wait(&status);
        printf("t = %d\n", t);
        printf("status = %d\n", status);
        printf("WEXITSTATUS(status)=%d\n", WEXITSTATUS(status));
    }
    exit(5);
}