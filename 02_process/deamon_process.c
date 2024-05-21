//
// Created by terry on 24-5-18.
//

#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

int main(void){
    pid_t pid;
    int i;
    int fd;

    pid= fork();
    if(pid < 0){
        perror("fork error");
        exit(1);
    }
    else if(pid > 0){ // 父进程
        printf("father process exit pid : %d \r\n",getpid());
        exit(0);
    }

    sleep(1);
    // 子进程
    if(setsid()<0){
        perror("set sesson error");
        exit(-1);
    }

    if(chdir("/") < 0){
        perror("set work dir error");
        exit(-1);
    }

    umask(0);

    // 将输出重定位到 空文件 0 1 2
    //标准输入、标准输出和标准错误重定向到/dev/null
    for(i=0; i< sysconf(_SC_OPEN_MAX); i++){
        close(i);
    }
    fd = open("/dev/null",O_RDWR);
    printf("fd : %d \r\n", fd);
    fd = dup(0);
    printf("fd : %d \r\n", fd);
    fd = dup(0);
    printf("fd : %d \r\n", fd);

    signal(SIGCHLD, SIG_IGN);
    for(;;){
        sleep(1);
        puts("守护进程运行中......");
    }
    exit(0);
}
