//
// Created by terry on 24-5-19.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void){
    int fd;
    // 创建管道 在当前工作目录  权限所有人都能读写
    if(mkfifo("fifo",0666)){
        perror("pipe create error");
        exit(-1);
    }

    // 打开 管道
    if((fd=open("fifo", O_WRONLY)) < 0 ){
        perror("pipe open error");
        exit(-1);
    }

    while(1){
        sleep(1);
        write(fd,"this is father preocess", strlen("this is father preocess"));
    }
}
