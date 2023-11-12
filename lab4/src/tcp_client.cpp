#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};

    // 创建客户端套接字
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // 将IPv4地址从文本转换为二进制形式
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // 发送数据给服务器，这里就是要改的地方。改成获取一条数据，并发送。
    char *message = "Hello from client!";
    sleep(1);
    send(client_socket, message, strlen(message), 0);
    sleep(3);
    send(client_socket, message, strlen(message), 0);

    printf("Message sent to server\n");

    // // 从服务器接收数据
    // read(client_socket, buffer, sizeof(buffer));
    // printf("Received from server: %s\n", buffer);

    // 关闭套接字
    close(client_socket);

    return 0;
}
