//
// Created by terry on 24-5-17.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
    pid_t  pid;

    pid = fork();
    switch ((pid)) {
        case -1: // 错误
            perror("fork error");
            break;
        case 0: // 子进程
            printf("this is sub process\r\n");
            printf("%d\n",pid);
            _exit(0);
        default: // 父进程
            printf("this is main process\r\n");
            printf("%d\n", pid);
            _exit(0);
    }
}