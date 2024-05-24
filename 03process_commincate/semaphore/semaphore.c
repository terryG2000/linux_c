//
// Created by terry on 24-5-24.
//


#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdint.h>

union semun {
    int val;                // 用于 SETVAL 命令
    struct semid_ds *buf;   // 用于 IPC_STAT 和 IPC_SET 命令
    unsigned short *array;  // 用于 GETALL 和 SETALL 命令
    struct seminfo *__buf;  // 用于 IPC_INFO 命令 (非标准)
};


//初始化信号量
int init_sem(int sem_id, int value){
    union semun tmp;
    tmp.val = value;
    if(semctl(sem_id, 0, SETVAL, tmp) == -1){
        perror("semctl SETVAL ERROR \n");
        return -1;
    }
    return 0;
}

// 尝试获取资源
int sem_p(int sem_id){
    struct sembuf sbuf;
    sbuf.sem_num = 0; // 序号
    sbuf.sem_op = -1;// p 尝试操作
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1){
        perror("P operate ERROR");
        return -1;
    }
    return 0;
}

// v 释放资源
int sem_v(int sem_id){
    struct sembuf sbuf;
    sbuf.sem_num = 0; // 序号
    sbuf.sem_op = 1;// v 释放资源
    sbuf.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sbuf, 1) == -1){
        perror("V operate ERROR");
        return -1;
    }
    return 0;
}


int del_sem(int sem_id){
    union semun tmp;
    if(semctl(sem_id, 0, IPC_RMID, tmp) == -1){
        perror("semctl SETVAL ERROR \n");
        return -1;
    }
    return 0;
}

int main(void){
    pid_t pid;
    int sem_id;
    key_t key;

    // 获取key
    if((key = ftok("./",'a')) < 0){
        perror("ftok error ");
        exit(-1);
    }

    // 通过key 创建sem
    if((sem_id = semget(key, 1, IPC_CREAT|0666)) == -1){
        perror("semget error ");
        exit(-1);
    }

    // 初始化信号量为 0
    init_sem(sem_id, 0);

    if( (pid = fork()) == -1){
        perror("fork error");
        exit(-1);
    }
    else if(pid == 0){ // sub process
        sem_p(sem_id); // 获取资源，此时为0 所以回在这里挂起
        printf("sub process : pid = %d\n ",getpid());
        sem_v(sem_id); // 释放资源
        del_sem(sem_id); // 删除信号量 （信号量和 消息队列共享内存一样 不主动删除都回常驻内核中）
    }
    else{
        sleep(2);
        sem_v(sem_id); // 释放资源
        printf("main process : pid = %d \n",getpid());
    }
    exit(0);
}