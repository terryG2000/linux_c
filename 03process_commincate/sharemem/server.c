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

//联合体 semctrl 初始化
union semun{
    uint32_t  val;
    struct semid_ds *buf;
    uint16_t *array;
};

//初始化 信号量
int init_sem(int sem_id, int val){
    union  semun tmp;
    tmp.val = val;
    if(semctl(sem_id, 0, SETVAL, tmp) == -1){
        perror("semctl error ");
        return -1;
    }
    return 0;
}

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

// 删除信号量
int del_sem(int sem_id){
    union semun tmp;
    if(semctl(sem_id, 0, IPC_RMID, tmp) == -1){
        perror("sem del error");
        return -1;
    }
    return 0;
}

// 创建信号量
int create_sem(key_t key){
    int sem_id;
    if( (sem_id = semget(key, 1, IPC_CREAT|0666)) == -1){
        perror("sem create error");
        return -1;
    }
    init_sem(sem_id, 1);
    return sem_id;
}

int main(void){
    key_t key;
    int shm_id,sem_id,msq_id;
    char *shm;
    char data[] = "this is server";
    struct shmid_ds shmds;
    struct msqid_ds msqds; // 用于删除消息队列
    struct msg_frame msg;

    if( (key = ftok("./", 'a')) < 0){
        perror("ftok error");
        exit(-1);
    }

    if( (shm_id = shmget(key, 1024, IPC_CREAT|0666)) == -1){
        error("shmget error");
        exit(-1);
    }

    shm = (char*) shmat(shm_id, 0, 0);

    if( (msq_id = msgget(key, IPC_CREAT|0666)) == -1){
        error("msgget error");
        exit(-1);
    }

    sem_id = create_sem(key);

    while(1){
        msgrcv(msq_id, &msg, 1, 888, 0);
        if(msg.text == 'q')
            break;
        else if(msg.text == 'r'){
            sem_p(sem_id);
            printf("%s \n", shm);
            sem_v(sem_id);
        }
    }

    shmdt(shm);

    shmctl(shm_id, IPC_RMID, &shmds);
    msgctl(msq_id, IPC_RMID, &msqds);
    del_sem(sem_id);

    exit(0);
}








