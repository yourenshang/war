//
// Created by shangyouren on 2019-06-11.
//


#include <string.h>

int ctoi(char * source){
    long target = 0;
    int len = strlen(source);
    if (len <= 0){
        return 0;
    }
    long index = 1;
    int i;
    for (i = len - 1; i >= 0; i--){
        if (source[i] > '9' || source[i] < '0'){
            break;
        }
        target = target + index * (source[i] - '0');
        index = index * 10;
    }
    if (i > 0){
        return 0;
    }else if (i == 0){
        if (source[i] == '-'){
            return -1 * target;
        }
    }else {
        return target;
    }
}

int itoc(const int source, char * target){
    short a[32];
    int na = 0;
    int cpy = source;
    if (cpy < 0){
        cpy = cpy * -1;
    }
    while (cpy > 10){
        a[na] = cpy % 10;
        cpy = cpy / 10;
        na++;
    }
    a[na] = cpy;
    na++;
    if (na <= 0){
        return -1;
    }
    int nt = 0;
    if (source < 0){
        target[nt] = '-';
        nt++;
    }
    for (int i = na - 1; i >= 0 ;i--){
        target[nt] = a[i] + '0';
        nt++;
    }
    target[nt] = '\0';
}
