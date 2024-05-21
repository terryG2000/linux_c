//
// Created by terry on 24-5-19.
//

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/type.h>
#include <sys/ipc.h>
#include <error.h>

#define MSG_FILE "/etc/"

struct msg_frame{
    long msg_type;
    char msg[256];
};


int main(void){
    int msqid;
    key_t key;
    struct msg_frame msg;

    if( (key = ftok(MSG_FILE,0)) < 0){
        perror("ftok error");
        exit(-1);
    }
    printf("Message Queue Key : %d", key);

    if( (msqid = msgget(key, IPC_CREAT|0666)) == -1){
        perror("msg get error");
        exit(-1);
    }

    printf("msq id %d,pid %d", msqid, getpid());

    while(1){
        msgrcv(msqid, &msg, 256, 888, 0);
    }


}