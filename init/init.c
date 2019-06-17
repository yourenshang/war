//
// Created by shangyouren on 2019-06-06.
//

#include "../utils/define/process.h"
#include "../utils/process/manager.h"
#include "../utils/log/log.h"

int init(){
    init_log();
    if (init_process_manager()){
        exit(EXIT_FAILURE);
    }
    return 0;
}

int destory(){
    return destory_process_manager();
}

