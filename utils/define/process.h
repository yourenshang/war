//
// Created by shangyouren on 2019-06-06.
//

#ifndef WAR_DEFINE_PROCESS_DEFINE_H
#define WAR_DEFINE_PROCESS_DEFINE_H

#include <signal.h>

/**
 * 进程管理存储模型
 */
struct process{
    char name[10];
    pid_t pid;
};

/**
 * 程序最大进程数
 */
#define MAX_PROCESS_SIZE 50

/**
 * 共享内存
 */
struct process_manager{
    struct process value[MAX_PROCESS_SIZE];
};

/**
 * 进程管理共享内存key
 */
#define PROCESS_MANAGER_SHM_KEY 1002

/**
 * 共享内存信号量key
 */
#define PROCESS_MANAGER_SEM_KEY 1001

/**
 * 共享内存信号量组数量
 */
#define PROCESS_MANAGER_SEM_SIZE 1

/**
 * 操作共享内存权限
 */
#define PROCESS_MANAGER_PROMESS 0666

/**
 * 返回值
 */
#define RETURN_SUCCESS 0
#define RETURN_GET_SEM_FAIL -1
#define RETURN_GET_SHM_FAIL -2
#define RETURN_FULL_FAIL -8
#define RETURN_EXIST -3
#define RETURN_BIND_FAIL -4
#define RETURN_PARAM_FAIL -5
#define RETURN_RELEASE_SHM_FAIL -6
#define RETURN_NOT_FOUND -9
#define RETURN_RELEASE_SEM_FAIL -7
#define RETURN_MESSAGE_SEND_FAIL -10
#define RETURN_MESSAGE_DESTORY_SHM_FAIL -11

/**
 * 进程间通信消息模型
 */
struct process_message{
    pid_t source;
    pid_t pid;
    int key;
    char command[25];
    char message[255];
};

/**
 * 进程间通信信号
 */
#define PROCESS_MESSAGE_SIGNAL SIGUSR1


#endif //WAR_SEM_H
