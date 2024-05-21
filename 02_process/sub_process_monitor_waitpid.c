//
// Created by terry on 24-5-18.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
    int status;
    int ret;
    int i;

    pid_t  pid[3] = {0};

    // 创建三个进程
    for(i=0; i<3; i++){
        pid[i] = fork();
        switch (pid[i]) {
            case -1:
                perror("fork erro \n");
                exit(-1);
            case 0:
                printf("sub process %d \n",getpid());
                sleep(i+1);
                exit(i);
            default:
                break;
        }
    }
// 主进程

////阻塞等待
//    sleep(1);
//    printf("main process ~~~~~~~~~\n");
//    for(i=0; i<3; i++){
//        ret = waitpid(-1,&status,0);
//        // -1 等待任意进程结束  0 阻塞等待 和wait 行为一致
//        if(ret == -1){ // 错误
//            if(errno == ECHILD){
//                printf("no sub process \n"); // 没有子进程
//                exit(0);
//            }
//            else{
//                perror("wait error"); // 其他错误
//                exit(-1);
//            }
//        }
//        printf("process recycle pid: %d, status %d\n", ret, WEXITSTATUS(status)); // 子进程成功终止
//    }

////非阻塞等待
//    sleep(1);
//    printf("main process ~~~~~~~~~\n");
//    for(;;){
//        ret = waitpid(-1,&status,WNOHANG);
//        // -1 等待任意进程结束  0 阻塞等待 和wait 行为一致
//        if(ret == -1){ // 错误
//            if(errno == ECHILD){
//                printf("no sub process \n"); // 没有子进程
//                exit(0);
//            }
//            else{
//                perror("wait error"); // 其他错误
//                exit(-1);
//            }
//        }
//        if(ret == 0)
//            continue;
//        else
//            printf("process recycle pid: %d, status %d\n", ret, WEXITSTATUS(status)); // 子进程成功终止
//    }


//非阻塞等待 特定进程
    sleep(1);
    printf("main process ~~~~~~~~~\n");
    for(i=0; i<3; i++){
        ret = waitpid(pid[2-i],&status,0);
        // -1 等待任意进程结束
        if(ret == -1){ // 错误
            if(errno == ECHILD){
                printf("no sub process \n"); // 没有子进程
                exit(0);
            }
            else{
                perror("wait error"); // 其他错误
                exit(-1);
            }
        }
        if(ret == 0)
            continue;
        else
            printf("process recycle pid: %d, status %d\n", ret, WEXITSTATUS(status)); // 子进程成功终止
    }

}
