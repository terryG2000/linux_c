//
// Created by terry on 24-5-19.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <error.h>
#include <string.h>

int main(void){
    pid_t pid;
    int fd[2] = {0};
    u_int8_t buff[50];

    if(pipe(fd) < 0){
        perror("pipe create error");
        exit(-1);
    }

    pid = fork();
    if(pid < 0){
        perror("fork  error");
        exit(-1);
    }
    else if(pid>0){ // father
        while(1){
            sleep(1);
            write(fd[1],"this is father preocess", strlen("this is father preocess"));
        }
    }
    else if(pid == 0){ // son
        while(1){
            sleep(1);
            read(fd[0],buff,strlen("this is father preocess"));
            printf("%s \r\n",buff);
        }
    }
}
