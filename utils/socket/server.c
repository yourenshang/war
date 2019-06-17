//
// Created by shangyouren on 2019-06-14.
//
#include "../log/log.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../define/module.h"

/**
 * 绑定端口
 */
int open_server(int port){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        print_log("error", MODULE_SERVER, "/utils/socket/server/open_server初始化服务器描述符失败");
        return -1;
    }
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        char port_c[10];
        itoc(port, port_c);
        print_log_param("error", MODULE_SERVER, "/utils/socket/server/open_server绑定端口失败，端口号", port_c);
        return -1;
    }
    if (listen(server_socket, 5) < 0){
        char port_c[10];
        itoc(port, port_c);
        print_log_param("error", MODULE_SERVER, "/utils/socket/server/open_server监听端口失败，端口号", port_c);
        return  -1;
    }
    return server_socket;
}

/**
 * 关闭端口
 */
int close_server(int server_socket, int port){
    if (close(server_socket) < 0){
        char port_c[10];
        itoc(port, port_c);
        print_log_param("error", MODULE_SERVER, "/utils/socket/server/close_server关闭端口失败，端口号", port_c);
        return -1;
    }
    return 0;
}

/**
 * 关闭客户端
 */
int close_client_in_server(int client_socket){
    if (close(client_socket) < 0){
        print_log("error", MODULE_SERVER, "/utils/socket/server/close_client_in_server关闭客户端失败");
        return -1;
    }
    return 0;
}

/**
 * 阻塞等待客户端
 */
int accept_client(int server_socket, struct sockaddr_in * client_addr, size_t client_size){
    int client_socket = accept(server_socket, (struct sockaddr *) client_addr, (socklen_t *) &client_size);
    if (client_socket < 0){
        print_log("error", MODULE_SERVER, "/utils/socket/server/accept_client阻塞等待客户端失败");
        return -1;
    }
    return client_socket;
}

/**
 * 阻塞等待客户端消息
 */
int read_client(int client_socket, char * buffer, int size){
    int read_len = (int) recv(client_socket, buffer, size, 0);
    if (read_len <= 0){
        print_log("error", MODULE_SERVER, "/utils/socket/server/read_client读取客户端消息失败");
        return -1;
    }
    return read_len;
}

