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
#include <stdint.h>

int main(void){
    int fd;
    uint8_t buff[50];

    // 打开管道
    if((fd=open("fifo", O_RDONLY)) < 0 ){
        perror("pipe open error");
        exit(-1);
    }

    while(1){
        sleep(1);
        read(fd,buff, strlen("this is father preocess"));
        printf("%s \n",buff);
    }
}
