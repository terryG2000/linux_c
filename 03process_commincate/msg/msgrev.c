//
// Created by terry on 24-5-19.
//

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <error.h>

#define MSG_FILE "/etc/passwd"

struct msg_frame{
    long msg_type; // 消息头
    char msg[2]; // 消息
};


int main(void){
    int msqid;
    key_t key;
    struct msg_frame msg;
    int ret;

    printf("rev start \n");
    // 根据已有文件 生成一个 唯一的key 作为 队列的唯一标识
    if( (key = ftok(MSG_FILE,0)) < 0){
        perror("ftok error");
        exit(-1);
    }
    printf("Message Queue Key : %d", key);

    // IPC_CREAT 标志没有队列时 创建队列    0666 表示三位8 进制的权限
    if( (msqid = msgget(key, 0666)) == -1){
        perror("msg get error");
        exit(-1);
    }

    printf("msq id %d,pid %d", msqid, getpid());


    while(1){
        sleep(1);
        ret = msgrcv(msqid, &msg, 2, 888, 0);
        printf("get msg %c - %c\n", msg.msg[0], msg.msg[1]);
    }


}