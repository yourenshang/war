//
// Created by shangyouren on 2019-06-10.
//

#include "../define/process.h"
#include "../sem/sem.h"
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/**
 * 绑定SIGCONT信号时用以唤醒沉睡的信号源线程
 */
void goon(int sig){}

/**
 * 向进程发送消息
 */
int send_process_message(struct process_message message){
    if (message.pid <= 0 || message.key <= 0 || strcmp(message.message, "") == 0 || strcmp(message.command, "") == 0){
        return RETURN_PARAM_FAIL;
    }
    if (!get_sem(message.key, 1, 0)){
        int shm_id = shmget(message.key, sizeof(struct process_message), PROCESS_MANAGER_PROMESS | IPC_CREAT);
        if (shm_id == -1){
            //todo 输出日志 获取共享内存失败
            printf("error 1\n");
            release_sem(message.key, 1, 0);
            return RETURN_GET_SHM_FAIL;
        }
        struct process_message * s_m = shmat(shm_id, NULL, 0);
        if (s_m == -1){
            //todo 输出日志 绑定共享内存失败
            printf("error 2\n");
            release_sem(message.key, 1, 0);
            return RETURN_BIND_FAIL;
        }
        s_m->pid = message.pid;
        s_m->key = message.key;
        s_m->source = message.source;
        strcpy(s_m->command, message.command);
        strcpy(s_m->message, message.message);
        if (shmdt(s_m) == -1){
            //todo 输出日志 剥离绑定共享内存失败
            printf("error 3\n");
        }
        int result = kill(message.pid, PROCESS_MESSAGE_SIGNAL);
        if(result != -1){
            signal(SIGCONT, goon);
            pause();
        }
        if (release_sem(message.key, 1, 0)){
            printf("error 4\n");
            //todo 输出日志 释放信号量失败
        }
        if (result == -1){
            //todo 输出日志 发送信号失败
            printf("error 5\n");
            if (shmctl(shm_id, 0, IPC_RMID)){
                //todo 输出日志 销毁共享内存失败
                printf("error 6\n");
                return RETURN_MESSAGE_DESTORY_SHM_FAIL;
            }
            return RETURN_MESSAGE_SEND_FAIL;
        }
        return RETURN_SUCCESS;
    }else {
        //todo 输出日志，获取信号量失败
        printf("error 7\n");
        return RETURN_GET_SEM_FAIL;
    }
}

/**
 * 读取共享内存中的消息
 */
int get_process_message(int key, struct process_message *target){
    int shm_id = shmget(key, sizeof(struct process_message), PROCESS_MANAGER_PROMESS | IPC_CREAT);
    if (shm_id == -1){
        //todo 输出日志 获取共享内存失败
        printf("error 8\n");
        return RETURN_GET_SHM_FAIL;
    }
    struct process_message * s_m = shmat(shm_id, NULL, 0);
    if (s_m == -1){
        //todo 输出日志 绑定共享内存失败
        printf("error 9\n");
        return RETURN_BIND_FAIL;
    }
    target->pid = s_m->pid;
    target->source = s_m->source;
    target->key = s_m->key;
    strcpy(target->message, s_m->message);
    strcpy(target->command, s_m->command);
    if (shmdt(s_m) == -1){
        //todo 输出日志 剥离绑定共享内存失败
        printf("error 10\n");
    }
    if (shmctl(shm_id, 0, IPC_RMID)){
        printf("error 11\n");
        //todo 输出日志 销毁共享内存失败
    }
    return 0;
}

/**
 * 绑定消息和信号量（进程初始化时调用）
 */
int bind_process_message_signal(int key, void (*p)(int)){
    if (init_sem(key, 1)){
        //todo 输出日志 初始化信号量失败
        printf("error 12\n");
        return -1;
    }
    signal(PROCESS_MESSAGE_SIGNAL, p);
    return 0;
}

/**
 * 销毁信号量（进程结束时调用）
 */
int destory_process_message_sem(int key){
    if (destory_sem(key, 1)){
        //todo 输出日志 销毁信号量失败
        printf("error 13\n");
        return -1;
    }
    return 0;
}

/**
 * 关闭通讯，唤醒消息源进程
 */
int close_process_message(pid_t source){
    if (kill(source, SIGCONT) == -1){
        //todo 输出日志 唤醒消息源进程失败
        printf("error 14\n");
        return -1;
    }
    return 0;
}

