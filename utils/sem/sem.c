//
// Created by shangyouren on 2019-06-06.
//

#include <sys/sem.h>

int get_sem(int key, int size, int index){
    int sem_id = sem_id = semget((key_t)key, size, 0666 | IPC_CREAT);
    if (sem_id == -1){
        return -1;
    }
    struct sembuf buf;
    buf.sem_num = index;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;
    if (semop(sem_id, &buf, 1) < 0){
        return -1;
    }
    return 0;
}

int release_sem(int key, int size, int index){
    int sem_id = sem_id = semget((key_t)key, size, 0666 | IPC_CREAT);
    if (sem_id == -1){
        return -1;
    }
    struct sembuf buf;
    buf.sem_num = index;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;
    if (semop(sem_id, &buf, 1) < 0){
        return -1;
    }
    return 0;
}

int init_sem(int key, int size){
    int sem_id = semget((key_t)key, size, 0666 | IPC_CREAT);
    if (sem_id == -1){
        return -1;
    }
    union semun init;
    init.val = 1;
    for (int i = 0; i < size; i++) {
        if (semctl(sem_id, i, SETVAL, init) < 0) {
            //todo 输出日志 初始化信号量失败
            if (semctl(sem_id, 0, IPC_RMID)){
                //todo 输出日志 注销信号量失败
            }
            return -1;
        }
    }
    return 0;
}

int destory_sem(int key, int size){
    int sem_id = semget((key_t)key, size, 0666 | IPC_CREAT);
    if (sem_id == -1){
        return -1;
    }
    if (semctl(sem_id, 0, IPC_RMID) == -1){
        return -1;
    }
    return 0;
}

