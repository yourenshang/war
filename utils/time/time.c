//
// Created by shangyouren on 2019-06-11.
//

#include <time.h>
#include "../string/string.h"

int now(char * result){
    strcpy(result, "");
    time_t t;
    time(&t);
    struct tm * lt = localtime(&t);
    char year[6];
    char month[4];
    char day[4];
    char hour[4];
    char min[4];
    char sec[4];
    itoc(1900 + lt->tm_year, year);
    strcat(result, year);
    itoc(lt->tm_mon + 1, month);
    strcat(result, "-");
    if (lt->tm_mon + 1 < 10) {
        strcat(result, "0");
    }
    strcat(result, month);
    strcat(result, "-");
    itoc(lt->tm_mday, day);
    if (lt->tm_mday < 10){
       strcat(result, "0");
    }
    strcat(result, day);
    strcat(result, " ");
    itoc(lt->tm_hour, hour);
    if (lt->tm_hour < 10){
        strcat(result, "0");
    }
    strcat(result, hour);
    strcat(result, ":");
    itoc(lt->tm_min, min);
    if (lt->tm_min < 10){
        strcat(result, "0");
    }
    strcat(result, min);
    strcat(result, ":");
    itoc(lt->tm_sec, sec);
    if (lt->tm_sec < 10){
        strcat(result, "0");
    }
    strcat(result, sec);
    return 0;
}