//
// Created by shangyouren on 2019-06-06.
//

#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include "../define/process.h"
#include "../sem/sem.h"
#include <string.h>
#include <stdio.h>
#include "../define/module.h"
#include "../log/log.h"

int release_process_manager(void *manager){
    int result = RETURN_SUCCESS;
    if (manager != NULL) {
        if (shmdt(manager) == -1) {
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/release_process_manager释放共享内存失败");
            result = RETURN_RELEASE_SHM_FAIL;
        }
    }
    if (!release_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE, 0)){
        return result;
    }else {
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/release_process_manager释放信号量失败");
        return RETURN_RELEASE_SEM_FAIL;
    }
}

/**
 * 存入进程号
 */
int push_process(const char * name, const pid_t pid){
    if (strlen(name) <= 0 || pid <= 0){
        return RETURN_PARAM_FAIL;
    }
    if (!get_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE, 0)){
        int shm_id = shmget(PROCESS_MANAGER_SHM_KEY, sizeof(struct process_manager), PROCESS_MANAGER_PROMESS | IPC_CREAT);
        if (shm_id < 0){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/push_process获取共享内存失败");
            release_process_manager(NULL);
            return RETURN_GET_SHM_FAIL;
        }
        struct process_manager* manager = shmat(shm_id, NULL, 0);
        if (manager == -1){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/push_process绑定共享内存失败");
            release_process_manager(NULL);
            return RETURN_BIND_FAIL;
        }
        int i;
        for (i = 0; i < MAX_PROCESS_SIZE; i++){
            if (strcmp(manager->value[i].name, name) == 0){
                release_process_manager(manager);
                return RETURN_EXIST;
            }else if (strcmp(manager->value[i].name, "") == 0){
                break;
            }
        }
        if (i == MAX_PROCESS_SIZE){
            release_process_manager(manager);
            return RETURN_FULL_FAIL;
        }
        strcpy(manager->value[i].name, name);
        manager->value[i].pid = pid;
        release_process_manager(manager);
        return RETURN_SUCCESS;
    }else {
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/push_process获取信号量失败");
        return RETURN_GET_SEM_FAIL;
    }
}


/**
 * 获取进程号
 */
pid_t get_pid(const char * name){
    if (strlen(name) <= 0){
        return RETURN_PARAM_FAIL;
    }
    if (!get_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE, 0)){
        int shm_id = shmget(PROCESS_MANAGER_SHM_KEY, sizeof(struct process_manager), PROCESS_MANAGER_PROMESS | IPC_CREAT);
        if (shm_id < 0){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/get_pid获取共享内存失败");
            release_process_manager(NULL);
            return RETURN_GET_SHM_FAIL;
        }
        struct process_manager * manager = shmat(shm_id, NULL, 0);
        if (manager == -1){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/get_pid绑定共享内存失败");
            release_process_manager(NULL);
            return RETURN_BIND_FAIL;
        }
        int i;
        for (i = 0; i < MAX_PROCESS_SIZE; i++){
            if (strcmp(manager->value[i].name, name) == 0){
                pid_t result = manager->value[i].pid;
                release_process_manager(manager);
                return result;
            }
        }
        release_process_manager(manager);
        return RETURN_NOT_FOUND;
    }else {
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/get_pid获取信号量失败");
        return RETURN_GET_SEM_FAIL;
    }
}

/**
 * 删除进程
 */
int remove_process(const pid_t pid){
    if (pid <= 0){
        return RETURN_PARAM_FAIL;
    }
    if (!get_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE, 0)){
        int shm_id = shmget(PROCESS_MANAGER_SHM_KEY, sizeof(struct process_manager), PROCESS_MANAGER_PROMESS | IPC_CREAT);
        if (shm_id < 0){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/remove_process获取共享内存失败");
            release_process_manager(NULL);
            return RETURN_GET_SHM_FAIL;
        }
        struct process_manager * manager = shmat(shm_id, NULL, 0);
        if (manager == -1){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/remove_process绑定共享内存失败");
            release_process_manager(NULL);
            return RETURN_BIND_FAIL;
        }
        int flag = 0;
        int i;
        for (i = 0; i < MAX_PROCESS_SIZE; i++){
            if (manager->value[i].pid == pid){
                flag = 1;
                continue ;
            }
            if (flag){
                strcpy(manager->value[i-1].name, manager->value[i].name);
                manager->value[i-1].pid = manager->value[i].pid;
            }
        }
        if (flag){
            strcpy(manager->value[MAX_PROCESS_SIZE-2].name, manager->value[MAX_PROCESS_SIZE-1].name);
            manager->value[MAX_PROCESS_SIZE-2].pid = manager->value[MAX_PROCESS_SIZE-1].pid;
            strcpy(manager->value[MAX_PROCESS_SIZE-1].name, "");
            manager->value[MAX_PROCESS_SIZE-1].pid = 0;
            release_process_manager(manager);
            return RETURN_SUCCESS;
        }
        release_process_manager(manager);
        return RETURN_NOT_FOUND;
    }else {
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/remove_process获取信号量失败");
        return RETURN_GET_SEM_FAIL;
    }
}

/**
 * 打印进程
 */
int print_process(){
    if (!get_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE, 0)){
        int shm_id = shmget(PROCESS_MANAGER_SHM_KEY, sizeof(struct process_manager), PROCESS_MANAGER_PROMESS | IPC_CREAT);
        if (shm_id < 0){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/print_process获取共享内存失败");
            release_process_manager(NULL);
            return RETURN_GET_SHM_FAIL;
        }
        struct process_manager * manager = shmat(shm_id, NULL, 0);
        if (manager == -1){
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/print_process绑定共享内存失败");
            release_process_manager(NULL);
            return RETURN_BIND_FAIL;
        }
        printf("----------------------print start\n");
        for (int i = 0; i < MAX_PROCESS_SIZE; i++){
            printf("name is %s, pid is %d\n", manager->value[i].name, manager->value[i].pid);
        }
        printf("----------------------print end\n");
        release_process_manager(manager);
        return RETURN_SUCCESS;
    }else {
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/print_process获取信号量失败");
        return RETURN_GET_SEM_FAIL;
    }
}

/**
 * 清空进程
 */
int clean_process(){
    if (!get_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE, 0)){
        int shm_id = shmget(PROCESS_MANAGER_SHM_KEY, sizeof(struct process_manager), PROCESS_MANAGER_PROMESS | IPC_CREAT);
        if (shm_id < 0){
            //todo 输出日志 获取共享内存失败
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/close_process获取共享内存失败");
            release_process_manager(NULL);
            return RETURN_GET_SHM_FAIL;
        }
        struct process_manager * manager = shmat(shm_id, NULL, 0);
        if (manager == -1){
            //todo 输出日志 绑定共享内存失败
            print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/close_process绑定共享内存失败");
            release_process_manager(NULL);
            return RETURN_BIND_FAIL;
        }
        for (int i = 0; i < MAX_PROCESS_SIZE; i++){
            strcpy(manager->value[i].name, "");
            manager->value[i].pid = 0;
        }
        release_process_manager(manager);
        return RETURN_SUCCESS;
    }else {
        //todo 输出日志 获取锁失败
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/close_process获取信号量失败");
        return RETURN_GET_SEM_FAIL;
    }
}

/**
 * 销毁线程管理共享内存
 */
int destory_process_shm(){
    int shm_id = shmget(PROCESS_MANAGER_SHM_KEY, sizeof(struct process_manager), PROCESS_MANAGER_PROMESS | IPC_CREAT);
    if (shm_id == -1){
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/destory_process_shm获取共享内存失败");
        return -1;
    }
    if (shmctl(shm_id, 0, IPC_RMID)){
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/destory_process_shm删除共享内存失败");
        return -1;
    }
    return 0;
}

/**
 * 初始化进程管理（主进程开始时执行）
 */
int init_process_manager(){
    if (init_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE)){
        print_log("error", MODULE_PROCESS_MANAGER, "utils/process/manager/init_process_sem初始化信号量失败");
        return -1;
    }
    return 0;
}

int destory_process_manager(){
    int result = destory_sem(PROCESS_MANAGER_SEM_KEY, PROCESS_MANAGER_SEM_SIZE);
    if (result){
        print_log("error", "init", "/init/init/destory释放信号量失败");
    }
    if (destory_process_shm()){
        return -1;
    }
    return result;
}


