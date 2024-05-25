//
// Created by terry on 24-5-25.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <string.h>

// 消息队列 消息
struct msg_frame{
    uint64_t mtype;
    uint8_t text;
};

// p 获取资源
int sem_p(int sem_id){
    struct sembuf sbuf;
    sbuf.sem_num = 0;
    sbuf.sem_op = -1;
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1){
        perror("semop error");
        return -1;
    }
    return 0;
}


// v 获取资源
int sem_v(int sem_id){
    struct sembuf sbuf;
    sbuf.sem_num = 0;
    sbuf.sem_op = 1;
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1){
        perror("semop error");
        return -1;
    }
    return 0;
}


int main(void){
    key_t key;
    int shm_id,sem_id,msq_id;
    char *shm;
    char data[] = "this is server";
    struct msg_frame msg;

    if( (key = ftok("./", 'a')) < 0){
        perror("ftok error");
        exit(-1);
    }

    if( (shm_id = shmget(key, 0, IPC_CREAT|0666)) == -1){
        error("shmget error");
        exit(-1);
    }
    shm = (char*) shmat(shm_id, 0, 0);

    if( (msq_id = msgget(key, 0)) == -1){
        error("msgget error");
        exit(-1);
    }

    if( (sem_id = semget(key, 0,0)) == -1){
        error("semget error");
        exit(-1);
    }


    printf("***************************************\n");
    printf("*                 IPC                 *\n");
    printf("*    Input r to send data to server.  *\n");
    printf("*    Input q to quit.                 *\n");
    printf("***************************************\n");

    uint8_t flag = 1;
    while(flag){
        char c;
        printf("input command: ");
        scanf("%c", &c);
        switch (c) {
            case 'r': // command r 写入共享内存
                printf("send data:");
                sem_p(sem_id);// 获取资源
                scanf("%s",shm);// 写入共享内存
                sem_v(sem_id);
                // 当scanf()输入字符或字符串时，缓冲区中遗留下了\n，所以每次输入操作后都需要清空标准输入的缓冲区。
                while( (c = getchar()) != '\n' && c != EOF);
                msg.mtype = 888;
                msg.text = 'r';
                msgsnd(msq_id, &msg, sizeof(msg.text), 0);
                break;
            case 'q':

                msg.mtype = 888;
                msg.text = 'q'; // 发送退出消息 然服务端删除共享内存、信号量和消息队列
                msgsnd(msq_id, &msg, sizeof(msg.text), 0);
                flag = 0;
                break;
            default:
                printf("wrong input \n");
                // 当scanf()输入字符或字符串时，缓冲区中遗留下了\n，所以每次输入操作后都需要清空标准输入的缓冲区。
                while( (c = getchar()) != '\n' && c != EOF);
                break;
        }
    }

    shmdt(shm);
    exit(0);
}








