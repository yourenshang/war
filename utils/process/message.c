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
#include "../define/module.h"
#include "../log/log.h"

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
            print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/send_process_message获取共享内存失败");
            release_sem(message.key, 1, 0);
            return RETURN_GET_SHM_FAIL;
        }
        struct process_message * s_m = shmat(shm_id, NULL, 0);
        if (s_m == -1){
            print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/send_process_message绑定共享内存失败");
            release_sem(message.key, 1, 0);
            return RETURN_BIND_FAIL;
        }
        s_m->pid = message.pid;
        s_m->key = message.key;
        s_m->source = message.source;
        strcpy(s_m->command, message.command);
        strcpy(s_m->message, message.message);
        if (shmdt(s_m) == -1){
            print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/send_process_message剥离绑定共享内存失败");
        }
        int result = kill(message.pid, PROCESS_MESSAGE_SIGNAL);
        if(result != -1){
            signal(SIGCONT, goon);
            pause();
        }
        if (release_sem(message.key, 1, 0)){
            print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/send_process_message释放信号量失败");
        }
        if (result == -1){
            print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/send_process_message发送信号失败");
            if (shmctl(shm_id, 0, IPC_RMID)){
                print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/send_process_message销毁共享内存失败");
                return RETURN_MESSAGE_DESTORY_SHM_FAIL;
            }
            return RETURN_MESSAGE_SEND_FAIL;
        }
        return RETURN_SUCCESS;
    }else {
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/send_process_message获取信号量失败");
        return RETURN_GET_SEM_FAIL;
    }
}

/**
 * 读取共享内存中的消息
 */
int get_process_message(int key, struct process_message *target){
    int shm_id = shmget(key, sizeof(struct process_message), PROCESS_MANAGER_PROMESS | IPC_CREAT);
    if (shm_id == -1){
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/get_process_message获取共享内存失败");
        return RETURN_GET_SHM_FAIL;
    }
    struct process_message * s_m = shmat(shm_id, NULL, 0);
    if (s_m == -1){
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/get_process_message绑定共享内存失败");
        return RETURN_BIND_FAIL;
    }
    target->pid = s_m->pid;
    target->source = s_m->source;
    target->key = s_m->key;
    strcpy(target->message, s_m->message);
    strcpy(target->command, s_m->command);
    if (shmdt(s_m) == -1){
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/get_process_message剥离绑定共享内存失败");
    }
    if (shmctl(shm_id, 0, IPC_RMID)){
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/get_process_message销毁共享内存失败");
    }
    return 0;
}

/**
 * 绑定消息和信号量（进程初始化时调用）
 */
int bind_process_message_signal(int key, void (*p)(int)){
    if (init_sem(key, 1)){
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/bind_process_message_signal初始化信号量失败");
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
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/destory_process_message_sem销毁信号量失败");
        return -1;
    }
    return 0;
}

/**
 * 关闭通讯，唤醒消息源进程
 */
int close_process_message(pid_t source){
    if (kill(source, SIGCONT) == -1){
        print_log("error", MODULE_PROCESS_MESSAGE, "/utils/process/message/close_process_message唤醒消息源进程失败");
        return -1;
    }
    return 0;
}

