//
// Created by shangyouren on 2019-06-06.
//

#include "../utils/define/process.h"
#include "../utils/process/manager.h"

int init(){
    if (init_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE)){
        return -1;
    }
    return 0;
}

int destory(){
    int result = destory_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE);
    if (result){
        //todo 输出日志 释放信号量失败
    }
    if (destory_process_shm()){
        return -1;
    }
    return result;
}

