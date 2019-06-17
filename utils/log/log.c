//
// Created by shangyouren on 2019-06-13.
//

#define LOG_PATH "log.txt"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../time/time.h"
#include "../string/string.h"

int init_log(){
    int i;
    for (i = 0; i < 10; i++){
        if (freopen(LOG_PATH, "a", stdout) != NULL){
            break;
        }
    }
    if (i == 10){
        printf("重定向标准输出失败，不能打开日志\n");
        fprintf(stderr, "重定向标准输出失败，不能打开日志\n");
        exit(EXIT_FAILURE);
    }
}

int print_log_param(const char * lev, const char * module, const char * message, const char * param){
    char now_time[25];
    now(now_time);
    char lev_test[10];
    strcpy(lev_test, lev);
    to_upper(lev_test);
    printf("[%s] [%s] PID:%d %s: %s%s\n", now_time, lev_test, getpid(), module, message, param);
}

int print_log(const char * lev, const char * module, const char * message){
    char now_time[25];
    now(now_time);
    char lev_test[10];
    strcpy(lev_test, lev);
    to_upper(lev_test);
    printf("[%s] [%s] PID:%d %s: %s\n", now_time, lev_test, getpid(), module, message);
}
