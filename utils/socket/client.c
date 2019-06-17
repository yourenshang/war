//
// Created by shangyouren on 2019-06-14.
//

int open_client(char * server_host, int server_port){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0){
        print_log("error", MODULE_CLIENT, "/utils/socket/client/open_client初始化客户端描述符失败");
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_host);
    server_addr.sin_port = htons(server_port);
    if (connect(client_socket, (const struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        char port[10];
        itoc(server_port, port);
        char param[50];
        strcat(param, server_host);
        strcat(param, ":");
        strcat(param, port);
        print_log_param("error", MODULE_CLIENT, "/utils/socket/client/open_client链接服务器失败，服务器：", port);
        return -1;
    }
    return client_socket;
}

int send_to_server(int client_socket, char * buffer, char * server_host){
    if (send(client_socket, buffer, strlen(buffer), 0) == -1){
        print_log_param("error", MODULE_CLIENT, "/utils/socket/client/send_to_server发送信息失败，服务器：", server_host);
        return -1;
    }
    return 0;
}

int close_client_in_client(int client_socket){
    if (close(client_socket) < 0){
        print_log("error", MODULE_CLIENT, "/utils/socket/client/close_client_in_client关闭客户端失败");
        return -1;
    }
    return 0;
}

int read_server_result(int client_socket, char * buffer, size_t max_buffer_size){
    int len = recv(client_socket, buffer, max_buffer_size, 0);
    if (len <= 0){
        print_log("error", MODULE_CLIENT, "/utils/socket/client/read_server_result读取服务器返回失败");
    }
    return len;
}