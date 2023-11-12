#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE] = {0};

    // 创建服务器端套接字
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    // 绑定服务器端套接字
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }


    // 监听
    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }



    printf("Server listening on port %d...\n", PORT);



    // 接受客户端连接
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t*)&client_address)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 从客户端接收数据
    read(client_socket, buffer, sizeof(buffer));
    printf("Received from client: %s\n", buffer);

    // 发送数据给客户端
    char *message = "Hello from server!";
    send(client_socket, message, strlen(message), 0);
    printf("Message sent to client\n");

    // 关闭套接字
    close(server_socket);
    close(client_socket);

    return 0;
}
